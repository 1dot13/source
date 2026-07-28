// Crash telemetry: upload the crash_report_*.txt files the crash handler left
// behind, on the next launch.
//
// Deliberately a separate translation unit from debug_win_util.cpp. Everything in
// there runs inside a faulting thread and may not allocate; everything here runs at
// startup with a healthy heap and is ordinary code. Keeping the two apart keeps the
// no-heap rule easy to see and easy to hold.

#if defined(_MSC_VER)

#include "debug_util.h"

#include <windows.h>
#include <winhttp.h>
#include <process.h> // _beginthreadex for the detached upload thread

#include <vector>

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

// A report bigger than this is not one of ours; never put it on the wire.
const DWORD kMaxReportBytes = 256 * 1024;

// POST one report file to url. Returns the HTTP status, or 0 if the request never
// completed (no connection, DNS failure, timeout) — see reportIsSettled().
DWORD postReport(const wchar_t* url, const char* path) {
	HANDLE fh = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE) return 0;
	DWORD size = GetFileSize(fh, NULL);
	if (size > kMaxReportBytes) { CloseHandle(fh); return 413; }
	std::vector<char> body(size ? size : 1);
	DWORD got = 0;
	BOOL read_ok = ReadFile(fh, body.data(), size, &got, NULL);
	CloseHandle(fh);
	if (!read_ok || got != size) return 0;

	URL_COMPONENTS uc = {}; uc.dwStructSize = sizeof(uc);
	wchar_t host[256] = {}, urlpath[1024] = {};
	uc.lpszHostName = host;    uc.dwHostNameLength = _countof(host);
	uc.lpszUrlPath = urlpath;  uc.dwUrlPathLength  = _countof(urlpath);
	if (!WinHttpCrackUrl(url, 0, 0, &uc)) return 400;

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
					body.data(), size, size, 0) &&
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
		if (olderThan(fd.ftLastWriteTime, kMaxReportAgeDays)) {
			DeleteFileA(fd.cFileName);
			continue;
		}
		if (sent++ >= kMaxUploadsPerRun) break;
		if (reportIsSettled(postReport(s_telemetryUrl, fd.cFileName)))
			DeleteFileA(fd.cFileName);
	} while (FindNextFileA(hFind, &fd));
	FindClose(hFind);
	return 0;
}

} // anonymous namespace

namespace sgp {
void processCrashTelemetry(const wchar_t* url) {
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
	// _beginthreadex, not CreateThread: the upload path uses the CRT (std::vector),
	// which wants its per-thread state set up and torn down.
	uintptr_t t = _beginthreadex(NULL, 0, telemetryThread, NULL, 0, NULL);
	if (t) CloseHandle((HANDLE)t);
}
} // namespace sgp

#endif // _MSC_VER
