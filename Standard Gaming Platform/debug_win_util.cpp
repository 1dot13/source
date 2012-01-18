// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#if defined(_MSC_VER)

#include "debug_util.h"

#include <windows.h>


#include <dbghelp.h>
#include <iostream>

#include <vfs/Tools/vfs_log.h>



// The arraysize(arr) macro returns the # of elements in an array arr.
// The expression is a compile-time constant, and therefore can be
// used in defining new arrays, for example.  If you use arraysize on
// a pointer by mistake, you will get a compile-time error.
//
// One caveat is that arraysize() doesn't accept any array of an
// anonymous type or a type defined inside a function.  In these rare
// cases, you have to use the unsafe ARRAYSIZE_UNSAFE() macro below.  This is
// due to a limitation in C++'s template system.  The limitation might
// eventually be removed, but it hasn't happened yet.

// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef _MSC_VER
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))


namespace {
	// SymbolContext is a threadsafe singleton that wraps the DbgHelp Sym* family
	// of functions. The Sym* family of functions may only be invoked by one
	// thread at a time. SymbolContext code may access a symbol server over the
	// network while holding the lock for this singleton. In the case of high
	// latency, this code will adversly affect performance.
	//
	// There is also a known issue where this backtrace code can interact
	// badly with breakpad if breakpad is invoked in a separate thread while
	// we are using the Sym* functions. This is because breakpad does now
	// share a lock with this function. See this related bug:
	//
	// http://code.google.com/p/google-breakpad/issues/detail?id=311
	//
	// This is a very unlikely edge case, and the current solution is to
	// just ignore it.
	class SymbolContext {
	public:
		static SymbolContext* Get() {
			static SymbolContext* s_singleton = NULL;
			if(!s_singleton) s_singleton = new SymbolContext();
			return s_singleton;
			// We use a leaky singleton because code may call this during process
			// termination.

			//return Singleton<SymbolContext, LeakySingletonTraits<SymbolContext> >::get();
		}

		// Returns the error code of a failed initialization.
		DWORD init_error() const {
			return init_error_;
		}

		// For the given trace, attempts to resolve the symbols, and output a trace
		// to the ostream os. The format for each line of the backtrace is:
		//
		// <tab>SymbolName[0xAddress+Offset] (FileName:LineNo)
		//
		// This function should only be called if Init() has been called. We do not
		// LOG(FATAL) here because this code is called might be triggered by a
		// LOG(FATAL) itself.
		void OutputTraceToStream(const std::vector<void*>& trace, sgp::Logger::LogInstance* os) {
			//AutoLock lock(lock_);

			for (size_t i = 0; (i < trace.size()); ++i) {
				const int kMaxNameLength = 256;
				DWORD_PTR frame = reinterpret_cast<DWORD_PTR>(trace[i]);

				// Code adapted from MSDN example:
				// http://msdn.microsoft.com/en-us/library/ms680578(VS.85).aspx
				ULONG64 buffer[
					(sizeof(SYMBOL_INFO) +
						kMaxNameLength * sizeof(wchar_t) +
						sizeof(ULONG64) - 1) /
						sizeof(ULONG64)];

				// Initialize symbol information retrieval structures.
				DWORD64 sym_displacement = 0;
				PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(&buffer[0]);
				symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
				symbol->MaxNameLen = kMaxNameLength;
				BOOL has_symbol = SymFromAddr(GetCurrentProcess(), frame,
					&sym_displacement, symbol);

				// Attempt to retrieve line number information.
				DWORD line_displacement = 0;
				IMAGEHLP_LINE64 line = {};
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				BOOL has_line = SymGetLineFromAddr64(GetCurrentProcess(), frame,
					&line_displacement, &line);

				// Output the backtrace line.
				(*os) << "    ";
				if (has_symbol)
				{
					(*os) << " [0x" << trace[i] << "+" << sym_displacement << "]\t";
					(*os) << vfs::String(symbol->Name);
				}
				else
				{
					(*os) << "  [0x" << trace[i] << "]\t";
					// If there is no symbol information, add a spacer.
					(*os) << " (No symbol)";
				}
				if (has_line)
				{
					(*os) << " - (" << line.FileName << ":" << line.LineNumber << ")";
				}
				(*os) << sgp::endl;
			}
			(*os) << sgp::endl;
		}

	private:
		SymbolContext() : init_error_(ERROR_SUCCESS) {
			// Initializes the symbols for the process.
			// Defer symbol load until they're needed, use undecorated names, and
			// get line numbers.
			SymSetOptions(SYMOPT_DEFERRED_LOADS |
				SYMOPT_UNDNAME |
				SYMOPT_LOAD_LINES);
			if (SymInitialize(GetCurrentProcess(), NULL, TRUE)) {
				init_error_ = ERROR_SUCCESS;
			} else {
				__debugbreak();
				init_error_ = GetLastError();
			}
		}

		DWORD init_error_;
		//Lock lock_;
		DISALLOW_COPY_AND_ASSIGN(SymbolContext);
	};

} // namespace

#define ENABLE_STACK_TRACE 0
StackTrace::StackTrace() {
	// From http://msdn.microsoft.com/en-us/library/bb204633(VS.85).aspx,
	// the sum of FramesToSkip and FramesToCapture must be less than 63,
	// so set it to 62.
	const int kMaxCallers = 62;	
	// TODO(ajwong): Migrate this to StackWalk64.
	
	// WANNE: VS 2005 compilation error
	// WANNE: I disabled the method call "CaptureStackBackTrace()" because it gives a compilation error in VS 2005
#if ENABLE_STACK_TRACE
	void* callers[kMaxCallers];
	int count = CaptureStackBackTrace(0, kMaxCallers, callers, NULL);
	
	// WANNE: This also does not work in VS 2005
	//int count = RtlCaptureStackBackTrace(0, kMaxCallers, callers, NULL);
	if (count > 0) {
		trace_.resize(count);
		memcpy(&trace_[0], callers, sizeof(callers[0]) * count);
	} else {
		trace_.resize(0);
	}
#endif
}

static struct StackTraceLog {
	sgp::Logger_ID id;
	StackTraceLog() {
		id = sgp::Logger::instance().createLogger();
		sgp::Logger::instance().connectFile(id, L"stack_trace.log", false, sgp::Logger::FLUSH_ON_ENDL);
	};
} s_log;

void StackTrace::PrintBacktrace(const char* msg) {
	OutputToStream(msg, &(SGP_LOG(s_log.id)));
}

void StackTrace::OutputToStream(const char* msg, sgp::Logger::LogInstance* os) {
	SymbolContext* context = SymbolContext::Get();
	DWORD error = context->init_error();
	if (error != ERROR_SUCCESS)
	{
		(*os)	<< "Error initializing symbols (" << error 
				<< "). Dumping unresolved backtrace:"
				<< sgp::endl;
		for (size_t i = 0; (i < trace_.size()); ++i)
		{
			(*os) << "\t" << trace_[i] << sgp::endl;
		}
	}
	else
	{
		(*os) << L"Backtrace: " << (msg != NULL ? msg : "") << sgp::endl;
		context->OutputTraceToStream(trace_, os);
	}
}

#endif // _MSC_VER
