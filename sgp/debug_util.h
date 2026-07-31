// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Crash reporting: capture a fault (or an assertion) as a report file, and hand
// the pending reports to the telemetry uploader at startup.

#ifndef BASE_DEBUG_UTIL_H_
#define BASE_DEBUG_UTIL_H_

struct _EXCEPTION_POINTERS;

// Assertion failures raise this software exception so they get the same crash
// report as a real fault. Bit 29 set marks it customer-defined, so it can never
// collide with a system code; the top two bits mark it an error.
#define SGP_EXCEPTION_ASSERT 0xE1A55E27

namespace sgp
{
	// Raise SGP_EXCEPTION_ASSERT and swallow it again, so the vectored crash
	// handler writes a report for an assertion that never faults. Carries the
	// assert's line, file and message (NULL for a plain Assert) as exception
	// parameters.
	void raiseAssertException(unsigned lineNum, const char* sourceFileName,
		const char* message);

	// Write a heap-free crash report (registers + a raw return-address backtrace)
	// for the faulting context to a numbered crash_report file; symbolize it
	// offline against the build's PDB. Call first-chance from a vectored handler.
	void writeExceptionBacktrace(_EXCEPTION_POINTERS* ep);

	// Record the build-id (game version string) to stamp into crash reports, so
	// a report can be matched to the exact build's PDB. Call once at startup.
	void setCrashBuildId(const char* id);

	// What to tell the player: why we died and where the report landed. NULL if
	// no crash was recorded. Built by the handler, shown from the exit path.
	const wchar_t* crashReportMessage();

	// Record the player's optional self-chosen handle (HANDLE in Ja2 Settings) to
	// stamp into crash reports, so a report can be tied to whoever raises it with
	// us on Discord. Empty or unset simply omits the field. Call once at startup.
	void setCrashUserHandle(const wchar_t* handle);

	// Startup crash-telemetry pass (heap is healthy here — never call from a
	// crash handler). If url is empty the feature is off. On first run, asks the
	// player for consent (native dialog) and remembers it; if granted, POSTs each
	// pending crash_report_*.txt to url and deletes the ones that upload cleanly.
	void processCrashTelemetry(const wchar_t* url);
}

#endif  // BASE_DEBUG_UTIL_H_
