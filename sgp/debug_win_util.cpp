// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#if defined(_MSC_VER)

#include "debug_util.h"

#include <windows.h>


#include <winternl.h> // PEB loader list, so the module table costs no allocation




// Called first-chance from the vectored crash handler, so it runs *inside* the
// faulting thread with a possibly-wrecked heap: the very crash we want to report
// is often heap corruption (a trashed std::map, a wild pointer). Anything that
// allocates — the game Logger, std::vector, DbgHelp's Sym* — would fault again
// and take the report down with it. So this path is deliberately heap-free: only
// stack buffers and raw Win32. It emits runtime addresses plus the module table
// needed to make sense of them; symbolize offline against the build's PDB:
//   symbolize_crash crash_report_001.txt JA2.exe
namespace sgp {

// Stamped into every crash report so a report maps to the exact build's PDB.
// Copied into a fixed buffer (no heap) — safe to read from a crash handler.
static char s_buildId[64] = { 0 };
void setCrashBuildId(const char* id) {
	if (id) lstrcpynA(s_buildId, id, sizeof(s_buildId));
}

// Filled in once a report has been written; see crashReportMessage(). Sized to
// wsprintf's own output limit: it bounds nothing itself, and what goes in here is
// a headline plus a directory the player chose, which no arithmetic here controls.
static WCHAR s_crashMessage[1024] = { 0 };
const wchar_t* crashReportMessage() {
	return s_crashMessage[0] ? s_crashMessage : NULL;
}

// The player's optional handle, for tying a report to whoever raises it with us.
// Sanitized here rather than at crash time: it is user input that lands in a text
// report the telemetry server parses line by line, so drop anything that could
// forge a line (CR/LF, control and non-ASCII characters) and cap the length.
static char s_userHandle[33] = { 0 };
void setCrashUserHandle(const wchar_t* handle) {
	size_t out = 0;
	for (size_t i = 0; handle && handle[i] && out < sizeof(s_userHandle) - 1; ++i) {
		if (handle[i] >= L' ' && handle[i] <= L'~')
			s_userHandle[out++] = (char)handle[i];
	}
	s_userHandle[out] = 0;
}

// Set while a report is being written; see writeExceptionBacktrace.
static bool s_inDump = false;

// An assertion failure is a bug worth a report, but it faults nothing, so the
// vectored handler never sees it. Raise a software exception to hand the handler
// a real context, then swallow it: first-chance is all the handler needs, and
// letting it travel any further would kill a game that means to show its error
// screen. Lives here, alone in a function with no C++ objects, because SEH may
// not share a frame with anything that unwinds.
void raiseAssertException(unsigned lineNum, const char* sourceFileName,
	const char* message) {
	const ULONG_PTR args[3] = { lineNum, (ULONG_PTR)sourceFileName,
		(ULONG_PTR)message };
	__try {
		RaiseException(SGP_EXCEPTION_ASSERT, 0, 3, args);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		// Only reached if the report writer itself faulted: this __except is what
		// swallows that fault, so it is also the only place that can lift the
		// re-entry latch again. Leaving it set would silence every later crash.
		s_inDump = false;
	}
}

void writeExceptionBacktrace(_EXCEPTION_POINTERS* ep) {
	if (ep == NULL || ep->ContextRecord == NULL || ep->ExceptionRecord == NULL)
		return;

	const CONTEXT&          c = *ep->ContextRecord;
	const EXCEPTION_RECORD& r = *ep->ExceptionRecord;
	bool isAssert = (r.ExceptionCode == SGP_EXCEPTION_ASSERT && r.NumberParameters >= 2);

	// The same fault storms (Wine re-dispatches the crashing window message) and
	// walking a wrecked stack can fault us in turn. Dump each distinct crash site
	// once, never re-enter. ponytail: single globals, fine in a crash.
	//
	// An assertion is raised, not faulted, so its ExceptionAddress is RaiseException
	// inside the system DLL — the same address for every assert in the process. Key
	// those on the assert's own file and line, or the second one to fail would be
	// dropped as a duplicate of the first.
	static const void* s_lastSite = NULL;
	static DWORD       s_lastLine = 0;
	void* fault = r.ExceptionAddress;
	const void* site = isAssert ? (const void*)r.ExceptionInformation[1] : fault;
	DWORD assertLine = isAssert ? (DWORD)r.ExceptionInformation[0] : 0;
	if (s_inDump || (site == s_lastSite && assertLine == s_lastLine))
		return;
	s_lastSite = site;
	s_lastLine = assertLine;
	s_inDump = true;

	// One fresh, numbered file per crash. CREATE_NEW claims the first free number,
	// so reports never overwrite each other and pile up while a player is offline;
	// the telemetry uploader drains and deletes them when it can reach the server.
	char name[32];
	HANDLE h = INVALID_HANDLE_VALUE;
	for (int n = 1; n <= 999; ++n) {
		wsprintfA(name, "crash_report_%03d.txt", n);
		h = CreateFileA(name, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h != INVALID_HANDLE_VALUE) break;
		if (GetLastError() != ERROR_FILE_EXISTS) break; // real error, stop trying
	}
	if (h == INVALID_HANDLE_VALUE) { s_inDump = false; return; }

	char line[192];
	auto emit = [&](int n) { DWORD w; WriteFile(h, line, n, &w, NULL); };

	emit(wsprintfA(line,
		"\r\n*** CRASH  code=%08lX  eip=%08lX  esp=%08lX  ebp=%08lX ***\r\n",
		r.ExceptionCode, c.Eip, c.Esp, c.Ebp));
	// UTC, so a report stays unambiguous once it leaves the machine that wrote it.
	SYSTEMTIME t;
	GetSystemTime(&t);
	emit(wsprintfA(line, "  time %04d-%02d-%02d %02d:%02d:%02d UTC\r\n",
		t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond));
	if (s_buildId[0])
		emit(wsprintfA(line, "  build %s\r\n", s_buildId));
	if (s_userHandle[0])
		emit(wsprintfA(line, "  handle %s\r\n", s_userHandle));
	// File name only, and copied into a bounded buffer first: __FILE__ carries the
	// build machine's directory, and wsprintf's %s would happily run a long one off
	// the end of line[]. Same treatment as the module names below.
	if (isAssert) {
		const char* file = (const char*)r.ExceptionInformation[1];
		char base[64];
		base[0] = 0;
		if (file) {
			const char* name = file;
			for (const char* p = file; *p; ++p)
				if (*p == '\\' || *p == '/')
					name = p + 1;
			lstrcpynA(base, name, sizeof(base));
		}
		emit(wsprintfA(line, "  assertion failed at line %lu of %s\r\n",
			(DWORD)r.ExceptionInformation[0], base));

		// AssertMsg and the WDS assertions carry the one human-written clue about
		// what broke, so it belongs in the report. Filtered, not copied straight:
		// some call sites build the string at runtime out of game state, and it
		// lands in a text report the telemetry server reads line by line — a
		// newline in it would forge a field.
		const char* msg = r.NumberParameters >= 3 ?
			(const char*)r.ExceptionInformation[2] : NULL;
		if (msg && msg[0]) {
			char clean[128];
			size_t out = 0;
			for (size_t i = 0; msg[i] && out < sizeof(clean) - 1; ++i)
				if (msg[i] >= ' ' && msg[i] <= '~')
					clean[out++] = msg[i];
			clean[out] = 0;
			if (out)
				emit(wsprintfA(line, "  message: %s\r\n", clean));
		}
	}
	if (r.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && r.NumberParameters >= 2)
		emit(wsprintfA(line, "  access violation: %s %08lX\r\n",
			r.ExceptionInformation[0] == 1 ? "write to " :
			r.ExceptionInformation[0] == 8 ? "execute at" : "read from",
			(DWORD)r.ExceptionInformation[1]));

	// Where the loader actually put each image. Our executables are /DYNAMICBASE,
	// so a runtime address matches neither the preferred base recorded in the PDB
	// nor anything identifiable in a sibling module: without this table none of
	// the addresses below can be symbolized. Walked off the PEB loader list, which
	// costs no allocation, unlike EnumProcessModules or DbgHelp's module APIs.
	emit(wsprintfA(line, "  modules (base size name):\r\n"));
	PEB_LDR_DATA* ldr = NtCurrentTeb()->ProcessEnvironmentBlock->Ldr;
	LIST_ENTRY* head = &ldr->InMemoryOrderModuleList;
	int seen = 0;
	for (LIST_ENTRY* e = head->Flink; e != head && ++seen <= 128; e = e->Flink) {
		LDR_DATA_TABLE_ENTRY* m =
			CONTAINING_RECORD(e, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		DWORD base = (DWORD)(DWORD_PTR)m->DllBase;
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
		if (!base || dos->e_magic != IMAGE_DOS_SIGNATURE)
			continue;
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
		if (nt->Signature != IMAGE_NT_SIGNATURE)
			continue;
		// File name only, never the directory: a full path carries the player's
		// Windows account name and whatever else their install path says about
		// them, and the report leaves the machine. Symbolizing matches on module
		// name and base, so the directory was never read by anything.
		//
		// Copied into a terminated buffer: FullDllName is counted, not terminated,
		// and wsprintf supports neither "*" precision nor a way to bound %S, so a
		// long name would run off the end of both.
		const WCHAR* full = m->FullDllName.Buffer;
		int fullChars = (int)(m->FullDllName.Length / sizeof(WCHAR));
		if (!full || fullChars <= 0)
			continue;
		int start = 0;
		for (int i = 0; i < fullChars; ++i)
			if (full[i] == L'\\' || full[i] == L'/')
				start = i + 1;
		WCHAR modname[64];
		int nameChars = fullChars - start + 1;
		lstrcpynW(modname, full + start,
			nameChars < ARRAYSIZE(modname) ? nameChars : ARRAYSIZE(modname));
		emit(wsprintfA(line, "    %08lX %08lX %S\r\n", base,
			nt->OptionalHeader.SizeOfImage, modname));
	}

	// Committed stack bounds from the TEB, so a bad ebp can't fault our reads.
	NT_TIB* tib = (NT_TIB*)NtCurrentTeb();
	DWORD stkLo = (DWORD)(DWORD_PTR)tib->StackLimit;
	DWORD stkHi = (DWORD)(DWORD_PTR)tib->StackBase;

	emit(wsprintfA(line, "  [0] %08lX\r\n", c.Eip)); // exact crash site
	DWORD ebp = c.Ebp;
	for (int i = 1; i < 64; ++i) {
		if (ebp < stkLo || ebp + 8 > stkHi || (ebp & 3)) break;
		DWORD ret  = *(DWORD*)(ebp + 4);
		DWORD next = *(DWORD*)ebp;
		// Print every return address, not just our own module's: a fault inside
		// ddraw/fmod/bink is exactly the case worth seeing, and the module table
		// above already tells the reader which addresses land in real code.
		emit(wsprintfA(line, "  [%d] %08lX\r\n", i, ret));
		if (next <= ebp) break; // frames must climb the stack
		ebp = next;
	}
	FlushFileBuffers(h);
	CloseHandle(h);

	// Compose what the player gets told, while the fault details and the report's
	// name are still in hand — but do not show it here. First-chance means the
	// exception may yet be handled downstream, and a message box pumps messages,
	// which on the wrecked heap that just faulted is a second crash. SGPExit puts
	// it on screen once the game is actually going down.
	WCHAR dir[MAX_PATH];
	DWORD dirLen = GetCurrentDirectoryW(ARRAYSIZE(dir), dir);
	if (dirLen == 0 || dirLen >= ARRAYSIZE(dir))
		dir[0] = 0;
	WCHAR headline[128];
	if (isAssert)
		wsprintfW(headline,
			L"Jagged Alliance 2 1.13 hit an assertion failure at line %lu.",
			(DWORD)r.ExceptionInformation[0]);
	else
		wsprintfW(headline,
			L"Jagged Alliance 2 1.13 crashed: exception %08lX at %08lX.",
			r.ExceptionCode, (DWORD)(DWORD_PTR)fault);
	wsprintfW(s_crashMessage,
		L"%s\r\n\r\n"
		L"A crash report was written to\r\n\r\n    %s\\%S\r\n\r\n"
		L"Please attach that file to your bug report.",
		headline, dir, name);

	s_inDump = false;
}
} // namespace sgp

#endif // _MSC_VER
