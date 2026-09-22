// Crash telemetry: upload the crash_report_*.txt files the crash handler left
// behind, on the next launch.
//
// Deliberately a separate translation unit from crash_report.cpp. Everything in
// there runs inside a faulting thread and may not allocate; everything here runs at
// startup with a healthy heap and is ordinary code. Keeping the two apart keeps the
// no-heap rule easy to see and easy to hold.

#if defined(_MSC_VER)

#include "crash_report.h"

#include <windows.h>
#include <winhttp.h>
#include <process.h> // _beginthreadex for the detached upload thread

#include <cstring> // strstr
#include <optional>
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
		if (sent++ >= kMaxUploadsPerRun) break;
		if (reportIsSettled(postReport(s_telemetryUrl, *report)))
			DeleteFileA(fd.cFileName);
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	return 0;
}

} // anonymous namespace

namespace sgp {
void processCrashTelemetry(const wchar_t* url) {
	if (url == NULL || url[0] == L'\0') return; // no endpoint configured: feature off
	// Ahead of the consent question too: without its PDB this exe has nothing
	// readable to send, so there is nothing to ask about. See matchingPdbBesideExe().
	if (!matchingPdbBesideExe()) return;

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
	// _beginthreadex, not CreateThread: the upload path uses the CRT (std::string),
	// which wants its per-thread state set up and torn down.
	uintptr_t t = _beginthreadex(NULL, 0, telemetryThread, NULL, 0, NULL);
	if (t) CloseHandle((HANDLE)t);
}
} // namespace sgp

#endif // _MSC_VER
