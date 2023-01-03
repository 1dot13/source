// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is a cross platform interface for helper functions related to debuggers.
// You should use this to test if you're running under a debugger, and if you
// would like to yield (breakpoint) into the debugger.

#ifndef BASE_DEBUG_UTIL_H_
#define BASE_DEBUG_UTIL_H_

#include <iosfwd>
#include <vector>

#include "sgp_logger.h"

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName)	\
	TypeName(const TypeName&);				\
	void operator=(const TypeName&)

// An older, deprecated, politically incorrect name for the above.
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

// A stacktrace can be helpful in debugging. For example, you can include a
// stacktrace member in a object (probably around #ifndef NDEBUG) so that you
// can later see where the given object was created from.
class StackTrace {
public:
	// Create a stacktrace from the current location
	StackTrace();
	// Get an array of instruction pointer values.
	//   count: (output) the number of elements in the returned array
	const void *const *Addresses(size_t* count);
	// Print a backtrace to stderr
	void PrintBacktrace(const char* msg);

	// Resolve backtrace to symbols and write to stream.
	void OutputToStream(const char* msg, sgp::Logger::LogInstance* os);

private:
	std::vector<void*> trace_;
	int count_;

	DISALLOW_EVIL_CONSTRUCTORS(StackTrace);
};

namespace sgp
{
	void dumpStackTrace(vfs::String const& msg);
}

#endif  // BASE_DEBUG_UTIL_H_
