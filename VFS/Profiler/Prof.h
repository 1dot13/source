#ifndef _PROF_H_
#define _PROF_H_

#include "HPTimer.h"
#include "../vfs_types.h"
#include <string>
#include <vector>

#define DO_PROFILE 1
#if DO_PROFILE
	#define REGISTERMARKER(id,name) static CProf::tMarkerID id = g_Profiler->RegisterMarker(name) 
	#define STARTMARKER(id) (g_Profiler->StartMarker(id))
	#define STOPMARKER(id,success) (g_Profiler->StopMarker(id,success))

	#define DUMPPROFILERSTATSTOFILE(file) if(g_Profiler){g_Profiler->PrintProfilerState(file);}
#else
	#define REGISTERMARKER(id,name)
	#define STARTMARKER(id)
	#define STOPMARKER(id,success)

	#define DUMPPROFILERSTATSTOFILE(file)
#endif


class CProf
{
public:
	typedef unsigned int tMarkerID;
	static CProf* GetProf();

	void Clear();

	tMarkerID RegisterMarker(const char *marker);

	void StartMarker(tMarkerID id);
	void StopMarker(tMarkerID id, bool success);

	bool PrintProfilerState(vfs::Path const& file);
private:
	CProf();

	struct MARKER
	{
		MARKER() : call_count(0), success_count(0), fail_count(0), time(0.0) {};
		std::string markername;
		long double time;
		unsigned long call_count;
		unsigned long success_count;
		unsigned long fail_count;
		CHPTimer timer;
	};
	std::vector<MARKER> m_vMarker;
	unsigned int _nextMarker;
};

extern CProf* g_Profiler;

#endif // _PROF_H_
