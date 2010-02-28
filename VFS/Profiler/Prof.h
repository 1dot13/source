#ifndef _PROF_H_
#define _PROF_H_

#include "HPTimer.h"
#include "../vfs_types.h"
#include "../vfs_path.h"
#include <string>
#include <vector>

#define DO_PROFILE 1
#if DO_PROFILE
	#define REGISTERMARKER(id,name) static CProf::tMarkerID id = CProf::getProf().registerMarker(name) 
	#define STARTMARKER(id) (CProf::getProf().startMarker(id))
	#define STOPMARKER(id,success) (CProf::getProf().stopMarker(id,success))

	#define DUMPPROFILERSTATSTOFILE(file) CProf::getProf().printProfilerState(file)
#else
	#define REGISTERMARKER(id,name)
	#define STARTMARKER(id)
	#define STOPMARKER(id,success)

	#define DUMPPROFILERSTATSTOFILE(file)
#endif


class VFS_API CProf
{
public:
	typedef unsigned int tMarkerID;
	static CProf& getProf();

	void clear();

	tMarkerID registerMarker(const char *marker);

	void startMarker(tMarkerID id);
	void stopMarker(tMarkerID id, bool success);

	bool printProfilerState(vfs::Path const& file);
private:
	CProf();

	struct MARKER
	{
		MARKER() : call_count(0), success_count(0), fail_count(0), time(0.0) {};
		std::string markername;
		unsigned long call_count;
		unsigned long success_count;
		unsigned long fail_count;
		long double time;
		CHPTimer timer;
	};
	std::vector<MARKER> m_vMarker;
	unsigned int _nextMarker;
};

class ProfileMarker
{
public:
	ProfileMarker(CProf::tMarkerID id, bool default_exit=true)
		: _id(id), _exit_success(default_exit)
	{
		STARTMARKER(_id);
	}
	~ProfileMarker()
	{
		STOPMARKER(_id,_exit_success);
	}
	void exit(bool success)
	{
		_exit_success = success;
	}
private:
	CProf::tMarkerID _id;
	bool _exit_success;
};

VFS_API void DumpProfileState(vfs::Path const& path);

#endif // _PROF_H_
