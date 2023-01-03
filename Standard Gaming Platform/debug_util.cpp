// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "debug_util.h"

const void *const *StackTrace::Addresses(size_t* count) {
  *count = trace_.size();
  if (trace_.size())
    return &trace_[0];
  return NULL;
}

void sgp::dumpStackTrace(vfs::String const& msg)
{
	// the first error is the important one anyway
	static bool already_dumping = false;

	if(!already_dumping) // needs a mutes to be sure
	{
		already_dumping = true;
		StackTrace str;
		str.PrintBacktrace(msg.utf8().c_str());
		already_dumping = false;
	}
}

#ifdef __MINGW32__
StackTrace::StackTrace()
{
}

void StackTrace::PrintBacktrace(const char* msg)
{
}

#endif
