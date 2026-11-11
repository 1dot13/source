// Crash telemetry: upload the crash_report_*.txt files the crash handler left
// behind, on the next launch, with their frames named against the PDB beside the
// exe. Only faults go out, and only once they can be named; an assertion report
// stays on disk for the player.
//
// A local build uploads nothing. It names its own reports in place instead, so a
// developer reruns the exe that crashed to read one.
//
// Deliberately a separate translation unit from crash_report.cpp. Everything in
// there runs inside a faulting thread and may not allocate; everything here runs at
// startup with a healthy heap and is ordinary code. Keeping the two apart keeps the
// no-heap rule easy to see and easy to hold.

#if defined(_MSC_VER)

#include "crash_report.h"

#include <windows.h>
#include <winhttp.h>
#include <dbghelp.h>
#include <process.h> // _beginthreadex for the detached upload thread

#include <cstdio>
#include <cstring> // strstr
#include <optional>
#include <sstream>
#include <string>
#include <vector>

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// Persisted consent: 1 = yes, 0 = no, -1 = not asked yet.
int readConsent() {
	HANDLE h = CreateFileA("telemetry.consent", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return -1;
	char c = 0; DWORD got = 0;
	ReadFile(h, &c, 1, &got, NULL);
	CloseHandle(h);
	return (got == 1 && c == '1') ? 1 : 0;
}

void writeConsent(bool yes) {
	HANDLE h = CreateFileA("telemetry.consent", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return;
	DWORD w; WriteFile(h, yes ? "1" : "0", 1, &w, NULL);
	CloseHandle(h);
}

// A report bigger than this is not one of ours; never put it on the wire. Kept
// equal to MAX_BYTES in the sink, which answers a settling 400 above it: sending
// what the sink will not take is how an upload destroys the report it carries.
const DWORD kMaxReportBytes = 32 * 1024;

std::optional<std::string> readReport(const char* path) {
	HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return std::nullopt;
	std::string text(kMaxReportBytes + 1, '\0');
	DWORD got = 0;
	const BOOL read = ReadFile(file, text.data(), static_cast<DWORD>(text.size()), &got, NULL);
	CloseHandle(file);
	if (!read || got > kMaxReportBytes) return std::nullopt;
	text.resize(got);
	return text;
}

struct CrashLocation {
	std::string function;
	std::string file;
	DWORD line = 0;
};

struct SymbolBuffer {
	SYMBOL_INFO* get() {
		auto* symbol = reinterpret_cast<SYMBOL_INFO*>(storage);
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;
		return symbol;
	}
	alignas(SYMBOL_INFO) char storage[sizeof(SYMBOL_INFO) + MAX_SYM_NAME] = {};
};

// Innermost first: every level the optimizer inlined at the instruction a
// report's frame is executing, then the function that owns it. Empty when no
// function does. Frame 0 is the faulting instruction itself; every later frame
// is a return address, the byte after its call, so it is looked up one byte
// back -- otherwise a call that ends its function resolves to the next one.
std::vector<CrashLocation> resolveCrashFrame(HANDLE symbols, int frameIndex, DWORD64 frameAddress) {
	const DWORD64 address = frameIndex == 0 ? frameAddress : frameAddress - 1;
	SymbolBuffer buffer;
	DWORD64 symbolDisplacement = 0;
	DWORD lineDisplacement = 0;
	IMAGEHLP_LINE64 line{ sizeof(IMAGEHLP_LINE64) };

	if (!SymFromAddr(symbols, address, &symbolDisplacement, buffer.get()))
		return {};
	CrashLocation owner{ buffer.get()->Name };
	if (SymGetLineFromAddr64(symbols, address, &lineDisplacement, &line)) {
		owner.file = line.FileName;
		owner.line = line.LineNumber;
	}

	std::vector<CrashLocation> locations;
	const DWORD inlinedLevels = SymAddrIncludeInlineTrace(symbols, address);
	DWORD innermostContext = 0, inlineFrameIndex = 0;
	if (inlinedLevels > 0 && SymQueryInlineTrace(symbols, address, 0, address, address,
			&innermostContext, &inlineFrameIndex)) {
		for (DWORD level = 0; level < inlinedLevels; ++level) {
			CrashLocation inlined;
			if (SymFromInlineContext(symbols, address, innermostContext + level,
					&symbolDisplacement, buffer.get()))
				inlined.function = buffer.get()->Name;
			line = { sizeof(IMAGEHLP_LINE64) };
			if (SymGetLineFromInlineContext(symbols, address, innermostContext + level, 0,
					&lineDisplacement, &line)) {
				inlined.file = line.FileName;
				inlined.line = line.LineNumber;
			}
			locations.push_back(std::move(inlined));
		}
	}
	locations.push_back(std::move(owner));
	return locations;
}

class PdbBesideExe {
public:
	PdbBesideExe() = default;
	PdbBesideExe(const PdbBesideExe&) = delete;
	PdbBesideExe& operator=(const PdbBesideExe&) = delete;
	~PdbBesideExe() {
		if (loaded_.value_or(false))
			SymCleanup(dbghelpSessionKey());
	}

	bool loaded() {
		if (!loaded_)
			loaded_ = load();
		return *loaded_;
	}

	std::vector<CrashLocation> resolve(int frameIndex, DWORD rva) {
		return resolveCrashFrame(dbghelpSessionKey(), frameIndex, preferredBase_ + rva);
	}

private:
	HANDLE dbghelpSessionKey() { return this; }

	bool load() {
		char exePath[MAX_PATH];
		const DWORD length = GetModuleFileNameA(NULL, exePath, MAX_PATH);
		if (length == 0 || length == MAX_PATH)
			return false;
		const std::string exe(exePath, length);
		const std::string directory = exe.substr(0, exe.find_last_of('\\'));

		SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_NO_PROMPTS |
			SYMOPT_IGNORE_CVREC | SYMOPT_FAIL_CRITICAL_ERRORS);
		if (!SymInitialize(dbghelpSessionKey(), directory.c_str(), FALSE))
			return false;
		// Which PDB this is was settled before either pass started (see
		// matchingPdbBesideExe()). If DbgHelp still cannot read it, no frame
		// resolves -- the exe exports nothing to fall back on -- and no report is
		// named.
		preferredBase_ = SymLoadModuleEx(dbghelpSessionKey(), NULL, exe.c_str(), NULL, 0, 0, NULL, 0);
		if (preferredBase_ != 0)
			return true;
		SymCleanup(dbghelpSessionKey());
		return false;
	}

	std::optional<bool> loaded_;
	DWORD64 preferredBase_ = 0;
};

const size_t kMaxFunctionChars = 120;
const size_t kMaxFileChars = 80;

std::string describe(const CrashLocation& location) {
	std::string text = location.function.empty() ? "?" : location.function.substr(0, kMaxFunctionChars);
	if (!location.file.empty()) {
		const size_t directoryEnd = location.file.find_last_of("\\/");
		const std::string file =
			location.file.substr(directoryEnd == std::string::npos ? 0 : directoryEnd + 1);
		text += "  " + file.substr(0, kMaxFileChars) + ':' + std::to_string(location.line);
	}
	return text;
}

std::optional<std::string> nameFrames(const std::string& report, PdbBesideExe& pdb,
		bool withInlinedLevels) {
	std::string named;
	unsigned long imageBase = 0, imageSize = 0;
	int namedFrames = 0;
	std::istringstream lines(report);
	for (std::string line; std::getline(lines, line);) {
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		unsigned long base = 0, size = 0, address = 0;
		int frameIndex = 0, frameLineLength = 0;
		if (imageSize == 0 && line.compare(0, 4, "    ") == 0 &&
				sscanf_s(line.c_str(), "    %8lx %8lx", &base, &size) == 2) {
			imageBase = base;
			imageSize = size;
		} else if (imageSize != 0 &&
				sscanf_s(line.c_str(), "  [%d] %8lx%n", &frameIndex, &address, &frameLineLength) == 2 &&
				frameLineLength == static_cast<int>(line.size()) &&
				address >= imageBase && address - imageBase < imageSize) {
			const std::vector<CrashLocation> locations =
				pdb.resolve(frameIndex, address - imageBase);
			if (!locations.empty()) {
				line += ' ' + describe(locations.front());
				for (size_t level = 1; withInlinedLevels && level < locations.size(); ++level)
					line += "\r\n      inlined into " + describe(locations[level]);
				++namedFrames;
			}
		}
		named += line + "\r\n";
	}
	if (namedFrames == 0 || named.size() > kMaxReportBytes)
		return std::nullopt;
	return named;
}

bool writtenByThisExe(const std::string& report) {
	const std::string buildId = sgp::crashBuildId();
	char imageLine[32];
	snprintf(imageLine, sizeof(imageLine), "\r\n  image %08lX\r\n", sgp::imageFingerprint());
	return !buildId.empty() &&
		report.find("\r\n  build " + buildId + "\r\n") != std::string::npos &&
		report.find(imageLine) != std::string::npos;
}

std::optional<std::string> withFrameNames(const std::string& report, PdbBesideExe& pdb) {
	if (!writtenByThisExe(report) || !pdb.loaded())
		return std::nullopt;
	if (std::optional<std::string> named = nameFrames(report, pdb, true))
		return named;
	return nameFrames(report, pdb, false);
}

bool isAssertion(const std::string& report) {
	return report.find("\r\n  assertion failed at line ") != std::string::npos;
}

// Faults only, for now. Many assertions fire on a broken install or a mod that
// is missing files rather than on a bug in the code, and none of them is a
// crash: the game carries on. An assertion report stays on disk, for the player
// to attach to a bug report, until the reaper takes it.
std::optional<std::string> uploadableText(const std::string& report, PdbBesideExe& pdb) {
	if (isAssertion(report))
		return std::nullopt;
	return withFrameNames(report, pdb);
}

void replaceReport(const char* path, const std::string& text) {
	const std::string replacement = std::string(path) + ".tmp";
	HANDLE file = CreateFileA(replacement.c_str(), GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE) return;
	DWORD written = 0;
	const BOOL wrote = WriteFile(file, text.data(), static_cast<DWORD>(text.size()), &written, NULL);
	CloseHandle(file);
	if (!wrote || written != text.size() ||
			!MoveFileExA(replacement.c_str(), path, MOVEFILE_REPLACE_EXISTING))
		DeleteFileA(replacement.c_str());
}

// POST one report to url. Returns the HTTP status, or 0 if the request never
// completed (no connection, DNS failure, timeout) — see reportIsSettled().
DWORD postReport(const wchar_t* url, const std::string& body) {
	const DWORD size = static_cast<DWORD>(body.size());
	URL_COMPONENTS uc = {}; uc.dwStructSize = sizeof(uc);
	wchar_t host[256] = {}, urlpath[1024] = {};
	uc.lpszHostName = host;    uc.dwHostNameLength = _countof(host);
	uc.lpszUrlPath = urlpath;  uc.dwUrlPathLength  = _countof(urlpath);
	if (!WinHttpCrackUrl(url, 0, 0, &uc)) return 0;

	HINTERNET hSession = WinHttpOpen(L"JA2-1.13-crash-telemetry",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) return 0;
	// Bound every phase. Even off the main thread these must not hang forever: the
	// thread holds the report files open-ish and we want the queue drained or given
	// up on within seconds, not to leave a socket parked for the whole session.
	WinHttpSetTimeouts(hSession, 5000, 5000, 10000, 15000);

	DWORD status = 0;
	if (HINTERNET hConnect = WinHttpConnect(hSession, host, uc.nPort, 0)) {
		DWORD flags = (uc.nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
		if (HINTERNET hReq = WinHttpOpenRequest(hConnect, L"POST", urlpath, NULL,
				WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags)) {
			if (WinHttpSendRequest(hReq, L"Content-Type: text/plain\r\n", (DWORD)-1,
					const_cast<char*>(body.data()), size, size, 0) &&
				WinHttpReceiveResponse(hReq, NULL)) {
				DWORD len = sizeof(status);
				WinHttpQueryHeaders(hReq,
					WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
					WINHTTP_HEADER_NAME_BY_INDEX, &status, &len, WINHTTP_NO_HEADER_INDEX);
			}
			WinHttpCloseHandle(hReq);
		}
		WinHttpCloseHandle(hConnect);
	}
	WinHttpCloseHandle(hSession);
	return status;
}

// Whether a report is done with, i.e. safe to delete. Uploaded (2xx), or rejected
// as content the server will never take (a malformed or oversized body). Anything
// else — no connection, 5xx, and notably 404/403 from a mistyped or misconfigured
// CRASH_TELEMETRY_URL — keeps the file, so a bad setting loses nobody's report.
bool reportIsSettled(DWORD status) {
	return (status >= 200 && status < 300) ||
		status == 400 || status == 413 || status == 415;
}

// A report stamped "build local" comes from a developer build with no released
// PDB: nobody at the receiving end can symbolize it, so it never goes on the
// wire — and never gets reaped either, it is the developer's to delete.
bool isFromLocalBuild(const char* path) {
	HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return false;
	// The build line is within the first few lines of the header.
	char head[160] = {};
	DWORD got = 0;
	ReadFile(h, head, sizeof(head) - 1, &got, NULL);
	CloseHandle(h);
	return strstr(head, "  build local") != NULL;
}

// Reports older than this are stale: the crash they describe is long since shipped
// past, and a player who was offline for a season should not upload a season of them.
const DWORD kMaxReportAgeDays = 30;

bool olderThan(const FILETIME& ft, DWORD days) {
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	ULARGE_INTEGER t = { ft.dwLowDateTime, ft.dwHighDateTime };
	ULARGE_INTEGER n = { now.dwLowDateTime, now.dwHighDateTime };
	if (n.QuadPart <= t.QuadPart) return false; // clock skew: treat as fresh
	return (n.QuadPart - t.QuadPart) > days * 24ULL * 60 * 60 * 10000000ULL;
}

// The PDB this exe was linked with, beside it, is what makes a report readable:
// without it a report is a row of addresses into a build nobody may still have.
// So telemetry runs only when that PDB is there, and a player who deletes the
// PDBs for the disk space (README.md) has switched it off. "That PDB" means the
// GUID and age the linker wrote into both files -- the key a symbol server
// matches on -- so a PDB left over from another release does not count.

// The exe's own CodeView record: the linker writes the PDB's GUID, age and path
// into the debug directory, and the loader has mapped it, so this reads memory.
struct CodeViewRecord {
	char signature[4]; // "RSDS"
	GUID guid;
	DWORD age;
	char pdbPath[1];   // NUL-terminated, whatever path the linker wrote to
};

const CodeViewRecord* exeCodeView() {
	const BYTE* base = reinterpret_cast<const BYTE*>(&__ImageBase);
	const auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(base + __ImageBase.e_lfanew);
	const IMAGE_DATA_DIRECTORY& debug = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
	const auto* entries = reinterpret_cast<const IMAGE_DEBUG_DIRECTORY*>(base + debug.VirtualAddress);
	for (DWORD i = 0; debug.VirtualAddress && i < debug.Size / sizeof(IMAGE_DEBUG_DIRECTORY); ++i) {
		const IMAGE_DEBUG_DIRECTORY& entry = entries[i];
		if (entry.Type != IMAGE_DEBUG_TYPE_CODEVIEW || entry.AddressOfRawData == 0 ||
				entry.SizeOfData < sizeof(CodeViewRecord))
			continue;
		const auto* record = reinterpret_cast<const CodeViewRecord*>(base + entry.AddressOfRawData);
		if (memcmp(record->signature, "RSDS", 4) == 0)
			return record;
	}
	return NULL;
}

bool readAt(HANDLE file, ULONGLONG offset, void* out, DWORD size) {
	LARGE_INTEGER at;
	at.QuadPart = static_cast<LONGLONG>(offset);
	DWORD got = 0;
	return SetFilePointerEx(file, at, NULL, FILE_BEGIN) &&
		ReadFile(file, out, size, &got, NULL) && got == size;
}

// A PDB's GUID and age, from its info stream (stream 1) inside the MSF 7.0
// container: superblock, then the block listing the stream directory's blocks,
// then the directory, then the first block of stream 1. A few KB of reads, not a
// symbol load. The file is whatever sits beside the exe, so every count and index
// taken from it is checked before it is used.
bool readPdbIdentity(HANDLE file, GUID& guid, DWORD& age) {
	struct {
		char magic[32];
		DWORD blockSize, freeBlockMapBlock, numBlocks, numDirectoryBytes, unknown, blockMapAddr;
	} super;
	static const char kMagic[] = "Microsoft C/C++ MSF 7.00\r\n\x1a" "DS\0\0";
	static_assert(sizeof(kMagic) == sizeof(super.magic), "MSF magic is 32 bytes");
	if (!readAt(file, 0, &super, sizeof(super)) || memcmp(super.magic, kMagic, sizeof(kMagic)) != 0)
		return false;
	const DWORD blockSize = super.blockSize;
	if (blockSize != 512 && blockSize != 1024 && blockSize != 2048 && blockSize != 4096)
		return false;
	auto blockOffset = [&](DWORD block) { return static_cast<ULONGLONG>(block) * blockSize; };

	// The directory is: stream count, each stream's size, then each stream's block
	// list in stream order. Its own block list fills at most one block.
	const DWORD directoryBlocks = (super.numDirectoryBytes + blockSize - 1) / blockSize;
	if (super.numDirectoryBytes < 8 || directoryBlocks > blockSize / sizeof(DWORD) ||
			super.blockMapAddr >= super.numBlocks)
		return false;
	std::vector<DWORD> directoryBlockList(directoryBlocks);
	if (!readAt(file, blockOffset(super.blockMapAddr), directoryBlockList.data(),
			static_cast<DWORD>(directoryBlocks * sizeof(DWORD))))
		return false;
	std::vector<DWORD> directory(directoryBlocks * blockSize / sizeof(DWORD));
	for (DWORD i = 0; i < directoryBlocks; ++i)
		if (directoryBlockList[i] >= super.numBlocks ||
				!readAt(file, blockOffset(directoryBlockList[i]),
					directory.data() + i * (blockSize / sizeof(DWORD)), blockSize))
			return false;
	const size_t directoryWords = super.numDirectoryBytes / sizeof(DWORD);

	const DWORD numStreams = directory[0];
	if (numStreams < 2 || numStreams >= directoryWords)
		return false;
	const auto blocksOf = [&](DWORD streamSize) {
		return streamSize == 0xFFFFFFFF ? 0u : (streamSize + blockSize - 1) / blockSize; // -1: nil stream
	};
	const DWORD infoStreamSize = directory[2];
	// Stream 1's block list starts after stream 0's.
	const size_t infoFirstBlock = 1 + static_cast<size_t>(numStreams) + blocksOf(directory[1]);
	if (infoStreamSize == 0xFFFFFFFF || infoStreamSize < 28 || infoFirstBlock >= directoryWords ||
			directory[infoFirstBlock] >= super.numBlocks)
		return false;

	struct { DWORD version, signature, age; GUID guid; } info;
	static_assert(sizeof(info) == 28, "PDB info stream header is 28 bytes");
	if (!readAt(file, blockOffset(directory[infoFirstBlock]), &info, sizeof(info)))
		return false;
	guid = info.guid;
	age = info.age;
	return true;
}

bool matchingPdbBesideExe() {
	const CodeViewRecord* record = exeCodeView();
	if (record == NULL)
		return false; // linked without /DEBUG: there never was a PDB

	// Beside the exe, under the file name the linker recorded -- the directory it
	// recorded is the build machine's. Either separator: a cross-linked exe
	// records a POSIX path. Wide, so a game folder outside the ANSI code page
	// still resolves; the name itself is the linker's, and ASCII.
	wchar_t path[MAX_PATH];
	const DWORD length = GetModuleFileNameW(NULL, path, MAX_PATH);
	if (length == 0 || length == MAX_PATH)
		return false;
	wchar_t* directoryEnd = wcsrchr(path, L'\\');
	if (directoryEnd == NULL)
		return false;
	const char* name = record->pdbPath;
	for (const char* c = record->pdbPath; *c; ++c)
		if (*c == '\\' || *c == '/')
			name = c + 1;
	wchar_t* out = directoryEnd + 1;
	for (; *name; ++name, ++out) {
		if (out == path + MAX_PATH - 1 || static_cast<unsigned char>(*name) > 0x7F)
			return false;
		*out = static_cast<wchar_t>(*name);
	}
	*out = L'\0';

	HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return false;
	GUID guid;
	DWORD age = 0;
	const bool read = readPdbIdentity(file, guid, age);
	CloseHandle(file);
	return read && IsEqualGUID(guid, record->guid) && age == record->age;
}

// One launch drains at most this many, so a crash-looping build cannot turn startup
// into a long upload session. The rest wait for the next launch.
const int kMaxUploadsPerRun = 20;

wchar_t s_telemetryUrl[512];

// Drains the pending reports. Runs detached: if the player quits first the process
// exits from under it, which costs nothing — an interrupted upload leaves the file
// on disk and it goes out next launch.
unsigned __stdcall telemetryThread(void*) {
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("crash_report_*.txt", &fd);
	if (hFind == INVALID_HANDLE_VALUE) return 0;
	PdbBesideExe pdb;
	int sent = 0;
	do {
		if (isFromLocalBuild(fd.cFileName)) continue;
		if (olderThan(fd.ftLastWriteTime, kMaxReportAgeDays) ||
				fd.nFileSizeHigh != 0 || fd.nFileSizeLow > kMaxReportBytes) {
			DeleteFileA(fd.cFileName);
			continue;
		}
		const std::optional<std::string> report = readReport(fd.cFileName);
		if (!report) continue;
		const std::optional<std::string> upload = uploadableText(*report, pdb);
		if (!upload) continue;
		if (sent++ >= kMaxUploadsPerRun) break;
		if (reportIsSettled(postReport(s_telemetryUrl, *upload)))
			DeleteFileA(fd.cFileName);
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	return 0;
}

unsigned __stdcall nameLocalReports(void*) {
	WIN32_FIND_DATAA fd;
	HANDLE hFind = FindFirstFileA("crash_report_*.txt", &fd);
	if (hFind == INVALID_HANDLE_VALUE) return 0;
	PdbBesideExe pdb;
	do {
		const std::optional<std::string> report = readReport(fd.cFileName);
		if (!report) continue;
		if (const std::optional<std::string> named = withFrameNames(*report, pdb))
			replaceReport(fd.cFileName, *named);
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	return 0;
}

// _beginthreadex, not CreateThread: both passes use the CRT (std::string), which
// wants its per-thread state set up and torn down.
void runDetached(unsigned (__stdcall* pass)(void*)) {
	uintptr_t thread = _beginthreadex(NULL, 0, pass, NULL, 0, NULL);
	if (thread) CloseHandle((HANDLE)thread);
}

} // anonymous namespace

namespace sgp {
void processCrashTelemetry(const wchar_t* url) {
	// First, for both passes: without its own PDB this exe can name nothing, so
	// there is nothing to rewrite in place and nothing to ask about.
	if (!matchingPdbBesideExe()) return;
	// Ahead of the endpoint and the consent question: naming a report sends it
	// nowhere, and a developer who declines uploads still wants names.
	if (lstrcmpA(crashBuildId(), "local") == 0) {
		runDetached(nameLocalReports);
		return;
	}
	if (url == NULL || url[0] == L'\0') return; // no endpoint configured: feature off

	int consent = readConsent();
	if (consent < 0) { // first run: ask once, remember the answer
		int r = MessageBoxW(NULL,
			L"This build can send crash reports to the developers to help fix bugs.\n"
			L"A report contains where the game crashed, the names of the loaded\n"
			L"modules, and the HANDLE from your Ja2.ini if you set one. No file\n"
			L"paths, no save games, nothing else about your machine.\n\n"
			L"Send crash reports automatically?",
			L"Jagged Alliance 2 v1.13 \x2014 Crash Reporting",
			MB_YESNO | MB_ICONQUESTION);
		writeConsent(r == IDYES);
		consent = (r == IDYES) ? 1 : 0;
	}
	if (consent != 1) return; // declined: leave reports on disk, accumulating

	// Hand the draining to a detached thread. The uploads are synchronous WinHttp
	// calls with seconds-long timeouts, and this runs on the startup path: on the
	// main thread a slow or unreachable endpoint is a stall the player sees before
	// the splash screen. Nothing waits on the result, so it can take as long as it
	// takes. The consent prompt above stays here, on purpose — that one is a
	// question, and a question has to be asked before anything is sent.
	lstrcpynW(s_telemetryUrl, url, ARRAYSIZE(s_telemetryUrl));
	runDetached(telemetryThread);
}
} // namespace sgp

#endif // _MSC_VER
