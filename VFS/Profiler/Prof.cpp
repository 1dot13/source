#define NOMINMAX
#include "Prof.h"

#include "../vfs_types.h"
#include "../File/vfs_file.h"
#include <sstream>

CProf* g_Profiler = NULL;

class CProfileStarter
{
public:
	CProfileStarter()
	{
		CProf::GetProf();
	}
};
static CProfileStarter starter;

CProf* CProf::GetProf()
{
	if(!g_Profiler)
	{
		g_Profiler = new CProf();
	}
	return g_Profiler;
}

CProf::CProf()
{
	m_vMarker.resize(1024);
	_nextMarker = 0;
}

void CProf::Clear()
{
	for(unsigned int i = 0; i < _nextMarker; ++i)
	{
		m_vMarker[i].markername = "";
		m_vMarker[i].time = 0;
		m_vMarker[i].call_count = 0;
		m_vMarker[i].success_count = 0;
		m_vMarker[i].fail_count = 0;
	}
	_nextMarker = 0;
}


CProf::tMarkerID CProf::RegisterMarker(const char *marker)
{
	m_vMarker[_nextMarker].markername = marker;
	return _nextMarker++;
}

void CProf::StartMarker(tMarkerID id)
{
	m_vMarker[id].timer.StartTimer();
}

void CProf::StopMarker(tMarkerID id, bool success)
{
	m_vMarker[id].timer.StopTimer();
	m_vMarker[id].time += m_vMarker[id].timer.GetElapsedTimeInSeconds();
	m_vMarker[id].call_count++;
	if(success) m_vMarker[id].success_count++;
	else m_vMarker[id].fail_count++;
}

inline std::string MultChar(std::string::value_type c, unsigned int multiplicity)
{
	std::string s;
	s.resize(multiplicity);
	for(unsigned int i=0; i<multiplicity; ++i)
	{
		s[i] = c;
	}
	return s;
}

inline long double perCent(unsigned long value, unsigned long ref)
{
	return 100.0 * ((double)(value)/double(ref));
}

inline long double oneDigit(long double number)
{
	unsigned long temp = (unsigned long)(number * 10);
	return (temp / 10.0);
}

bool CProf::PrintProfilerState(vfs::Path const& file)
{
	vfs::CFile oFile(file);
	if(!oFile.OpenWrite(true,true))
	{
		return false;
	}
	// get largest value
	long double max_time = 0;
	unsigned int max_prefix = 0;
	for(unsigned int i=0; i<m_vMarker.size(); ++i)
	{
		if(m_vMarker[i].time > max_time)
		{
			max_time = m_vMarker[i].time;
		}
		std::string::size_type prefix_length = m_vMarker[i].markername.length();
		if(prefix_length > max_prefix)
		{
			max_prefix = prefix_length;
		}
	}
	const unsigned int WIDTH = 40;
	std::stringstream line;
	long double ld_success, ld_failure;
	for(unsigned int i=0; i<m_vMarker.size(); ++i)
	{
		if(m_vMarker[i].markername.empty())
		{
			break;
		}
		if(m_vMarker[i].markername.length() < WIDTH)
		{
			unsigned int space = WIDTH - m_vMarker[i].markername.length();
			line << m_vMarker[i].markername << MultChar(' ',space) << " | ";
		}
		else
		{
			line << m_vMarker[i].markername.substr(0,WIDTH) << " | ";
		}
		if(max_time != 0)
		{
			unsigned int num_stars = (unsigned int)(WIDTH * (m_vMarker[i].time / max_time));
			ld_success = perCent(m_vMarker[i].success_count,m_vMarker[i].call_count);
			ld_failure = perCent(m_vMarker[i].fail_count,m_vMarker[i].call_count);
			line << "[" << oneDigit(ld_success)	 << "|" << oneDigit(ld_failure) << "] " 
				 << MultChar('*', num_stars) << MultChar(' ', WIDTH - num_stars) << " | ";
		}
		line << "C: " << m_vMarker[i].call_count << ", T: " << m_vMarker[i].time << std::endl;
		//line << std::endl;
	}
	vfs::UInt32 written;
	std::string useless_copy = line.str();
	oFile.Write(useless_copy.c_str(), useless_copy.length()*sizeof(std::string::value_type),written);
	oFile.Close();
	return true;
}

