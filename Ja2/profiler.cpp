#ifdef PROFILER_ENABLED

#include "builddefines.h"
#include <sstream>

#ifdef PERIODIC_PROFILING
bool gRecordingProfile = false;
#endif

PerfManager* PerfManager::_instance(NULL);

PerfDatum::PerfDatum(	const char* const fileName, 
						const char* const functionName,
						const int lineNumber,
						__int64 cycles,
						__int64 calls)
	:
	_fileName(fileName),
	_functionName(functionName),
	_lineNumber(lineNumber),
	_cycles(cycles),
	_calls(calls)
{
}

PerfManager::PerfManager(void)
	:
	_callTime(0),
	_totalTime(0)
{
	//Set up a root function so that the stack is never empty:
	//(this will be the only entry with _lineNumber equal to 0)
	_perfLog.insert(PerfDatum(__FILE__, "Root",0));
	_perfStack.push_back(&*(_perfLog.begin()));
	_lastTime = getCPUCount();
	calibrate();
}

void PerfManager::calibrate()
{
}

PerfManager::~PerfManager(void)
{
}

PerfManager* PerfManager::instance()
{
	if(!_instance)		//Double-check locking (first check)
	{	_instance = new PerfManager();
	}
	return _instance;
}

void PerfManager::enterFunction(const char* const fileName, 
								const char* const functionName, 
								const int lineNumber)
{
	//Mark the time when we end processing the previous function
	__int64 endTime = getCPUCount();
	//assuming we haven't corrupted the stack and reduced it to zero elements
	if(_perfStack.size())
	{	PERF_STACK_T::reverse_iterator lastMarker = _perfStack.rbegin();
		//Update the total time spent processing the previous function
		__int64 runTime = (endTime - _lastTime) - _callTime;
		(**lastMarker)._cycles += runTime;
		_totalTime += runTime;
	}

	//If we fail to insert the performance marker into our log, it means it is
	//a duplicate.	We can ignore this "error" as using the existing copy is
	//the correct behavior.
	std::pair<PERF_LOG_T::iterator, bool> result = 
		_perfLog.insert(PerfDatum(fileName, functionName, lineNumber));
	//Update the call count on the new function:
	PerfDatum* marker = &*(result.first);
	marker->_calls++;
	//Add performance marker to the stack as well as the log:
	_perfStack.push_back(marker); 
	//Now that we are clear of any complex operations, we can set the call
	//time of the new marker more accurately:
	_lastTime = getCPUCount();
}

void PerfManager::exitFunction()
{
	//Mark the time when we exited the function
	__int64 endTime = getCPUCount();
	//Update the function in our log with its new runtime
	PERF_STACK_T::reverse_iterator lastMarker = _perfStack.rbegin();
	__int64 runTime = endTime - _lastTime;
	(**lastMarker)._cycles += runTime;
	_totalTime += runTime;
	_perfStack.pop_back();
	//Lastly, update the function we've fallen back to with the new current
	//run time.
	_lastTime = getCPUCount();
}

__int64 PerfManager::getCPUCount () const
{
	_asm rdtsc
}

bool PerfSort::operator ()(const PerfDatum& lhs, const PerfDatum&rhs) const
{
	if(rhs._lineNumber == lhs._lineNumber)
	{	return 0 < strcmp(rhs._fileName, lhs._fileName);
	}
	else
	{	return rhs._lineNumber > lhs._lineNumber;
	}
}

bool PerfSort2::operator ()(const PerfDatum& lhs, const PerfDatum&rhs) const
{
	return lhs._cycles > rhs._cycles;
}

int PerfManager::getPercision(const double value) const
{
	int retVal = 1;
	if(10.0 <= value)
	{	retVal = 3;
	}
	else if (1.0 <= value)
	{	retVal = 2;
	}
	return retVal;
}

void PerfManager::log(std::ostream &os)
{
	//Create a copy of the log that sorts by cycles used
	std::set<PerfDatum, PerfSort2> logCopy;
	for(PERF_LOG_T::iterator i = _perfLog.begin(); i != _perfLog.end(); i++)
	{
		logCopy.insert(*i);
	}

	int cyclesWidth = 1;
	//I want the formatting to be nice!
	for(std::set<PerfDatum, PerfSort2>::iterator i = logCopy.begin(); 
		i != logCopy.end(); 
		i++)
	{
		if(i->_lineNumber) {
			std::stringstream cyclesString;
			cyclesString << i->_cycles;
			std::string tempString;
			cyclesString >> tempString;
			cyclesWidth = tempString.size();
			break;
		}
	}

	//Stream the sorted log to the supplied ostream
	for(std::set<PerfDatum, PerfSort2>::iterator i = logCopy.begin(); 
		i != logCopy.end(); 
		++i)
	{
		if(i->_lineNumber)
		{	//Truncate path from file name if the compiler included it.
			const char* j = i->_fileName + strlen(i->_fileName);
			while('\\' != *j && '/' != *j && j >= i->_fileName){j--;}
			if('\\' == *j || '/' == *j){j++;}
			std::stringstream cyclesString;
			std::string tempString;
			cyclesString << i->_cycles;
			cyclesString >> tempString;
			os.width(cyclesWidth);
			os << tempString.c_str() << ", ";
			__int64 cyclesPerCall = i->_cycles / i->_calls;
			cyclesString.clear();
			tempString.clear();
			cyclesString << cyclesPerCall;
			cyclesString >> tempString;
			os.width(cyclesWidth);
			os << tempString.c_str() << "] cycles (";
			double percent = double(i->_cycles) / double(_totalTime) * 100;
			os.precision(getPercision(percent));
			os.width(4);
			os << percent << "%) used in [";
			os.width(8);
			os << i->_calls << "] calls by " << j << "::" << i->_functionName\
				<< " at line " << i->_lineNumber << std::endl;
		}
	}
}

PerfMarker::PerfMarker( const char* const fileName, 
						const char* const functionName,
						const int lineNumber)
	:
#ifdef PERIODIC_PROFILING
	_onStack(gRecordingProfile)
#else
	_onStack(true)
#endif
{
#ifdef PERIODIC_PROFILING
	if (gRecordingProfile == true) {
#endif
		PerfManager::instance()->enterFunction(fileName,functionName,lineNumber);
#ifdef PERIODIC_PROFILING
	}
#endif
}

PerfMarker::~PerfMarker()
{
	endMark();
}

void PerfMarker::endMark()
{
	if(_onStack)
	{	PerfManager::instance()->exitFunction();
		_onStack = false;
	}
}
#include "profiler.h"
#include <sstream>

#ifdef PERIODIC_PROFILING
bool gRecordingProfile = false;
#endif

PerfManager* PerfManager::_instance(NULL);

PerfDatum::PerfDatum(	const char* const fileName, 
						const char* const functionName,
						const int lineNumber,
						__int64 cycles,
						__int64 calls)
	:
	_fileName(fileName),
	_functionName(functionName),
	_lineNumber(lineNumber),
	_cycles(cycles),
	_calls(calls)
{
}

PerfManager::PerfManager(void)
	:
	_callTime(0),
	_totalTime(0)
{
	//Set up a root function so that the stack is never empty:
	//(this will be the only entry with _lineNumber equal to 0)
	_perfLog.insert(PerfDatum(__FILE__, "Root",0));
	_perfStack.push_back(&*(_perfLog.begin()));
	_lastTime = getCPUCount();
	calibrate();
}

void PerfManager::calibrate()
{
}

PerfManager::~PerfManager(void)
{
}

PerfManager* PerfManager::instance()
{
	if(!_instance)		//Double-check locking (first check)
	{	_instance = new PerfManager();
	}
	return _instance;
}

void PerfManager::enterFunction(const char* const fileName, 
								const char* const functionName, 
								const int lineNumber)
{
	//Mark the time when we end processing the previous function
	__int64 endTime = getCPUCount();
	//assuming we haven't corrupted the stack and reduced it to zero elements
	if(_perfStack.size())
	{	PERF_STACK_T::reverse_iterator lastMarker = _perfStack.rbegin();
		//Update the total time spent processing the previous function
		__int64 runTime = (endTime - _lastTime) - _callTime;
		(**lastMarker)._cycles += runTime;
		_totalTime += runTime;
	}

	//If we fail to insert the performance marker into our log, it means it is
	//a duplicate.	We can ignore this "error" as using the existing copy is
	//the correct behavior.
	std::pair<PERF_LOG_T::iterator, bool> result = 
		_perfLog.insert(PerfDatum(fileName, functionName, lineNumber));
	//Update the call count on the new function:
	PerfDatum* marker = &*(result.first);
	marker->_calls++;
	//Add performance marker to the stack as well as the log:
	_perfStack.push_back(marker); 
	//Now that we are clear of any complex operations, we can set the call
	//time of the new marker more accurately:
	_lastTime = getCPUCount();
}

void PerfManager::exitFunction()
{
	//Mark the time when we exited the function
	__int64 endTime = getCPUCount();
	//Update the function in our log with its new runtime
	PERF_STACK_T::reverse_iterator lastMarker = _perfStack.rbegin();
	__int64 runTime = endTime - _lastTime;
	(**lastMarker)._cycles += runTime;
	_totalTime += runTime;
	_perfStack.pop_back();
	//Lastly, update the function we've fallen back to with the new current
	//run time.
	_lastTime = getCPUCount();
}

__int64 PerfManager::getCPUCount () const
{
	_asm rdtsc
}

bool PerfSort::operator ()(const PerfDatum& lhs, const PerfDatum&rhs) const
{
	if(rhs._lineNumber == lhs._lineNumber)
	{	return 0 < strcmp(rhs._fileName, lhs._fileName);
	}
	else
	{	return rhs._lineNumber > lhs._lineNumber;
	}
}

bool PerfSort2::operator ()(const PerfDatum& lhs, const PerfDatum&rhs) const
{
	return lhs._cycles > rhs._cycles;
}

int PerfManager::getPercision(const double value) const
{
	int retVal = 1;
	if(10.0 <= value)
	{	retVal = 3;
	}
	else if (1.0 <= value)
	{	retVal = 2;
	}
	return retVal;
}

void PerfManager::log(std::ostream &os)
{
	//Create a copy of the log that sorts by cycles used
	std::set<PerfDatum, PerfSort2> logCopy;
	for(PERF_LOG_T::iterator i = _perfLog.begin(); i != _perfLog.end(); i++)
	{
		logCopy.insert(*i);
	}

	int cyclesWidth = 1;
	//I want the formatting to be nice!
	for(std::set<PerfDatum, PerfSort2>::iterator i = logCopy.begin(); 
		i != logCopy.end(); 
		i++)
	{
		if(i->_lineNumber) {
			std::stringstream cyclesString;
			cyclesString << i->_cycles;
			std::string tempString;
			cyclesString >> tempString;
			cyclesWidth = tempString.size();
			break;
		}
	}

	//Stream the sorted log to the supplied ostream
	for(std::set<PerfDatum, PerfSort2>::iterator i = logCopy.begin(); 
		i != logCopy.end(); 
		++i)
	{
		if(i->_lineNumber)
		{	//Truncate path from file name if the compiler included it.
			const char* j = i->_fileName + strlen(i->_fileName);
			while('\\' != *j && '/' != *j && j >= i->_fileName){j--;}
			if('\\' == *j || '/' == *j){j++;}
			std::stringstream cyclesString;
			std::string tempString;
			cyclesString << i->_cycles;
			cyclesString >> tempString;
			os.width(cyclesWidth);
			os << tempString.c_str() << ", ";
			__int64 cyclesPerCall = i->_cycles / i->_calls;
			cyclesString.clear();
			tempString.clear();
			cyclesString << cyclesPerCall;
			cyclesString >> tempString;
			os.width(cyclesWidth);
			os << tempString.c_str() << "] cycles (";
			double percent = double(i->_cycles) / double(_totalTime) * 100;
			os.precision(getPercision(percent));
			os.width(4);
			os << percent << "%) used in [";
			os.width(8);
			os << i->_calls << "] calls by " << j << "::" << i->_functionName\
				<< " at line " << i->_lineNumber << std::endl;
		}
	}
}

PerfMarker::PerfMarker( const char* const fileName, 
						const char* const functionName,
						const int lineNumber)
	:
#ifdef PERIODIC_PROFILING
	_onStack(gRecordingProfile)
#else
	_onStack(true)
#endif
{
#ifdef PERIODIC_PROFILING
	if (gRecordingProfile == true) {
#endif
		PerfManager::instance()->enterFunction(fileName,functionName,lineNumber);
#ifdef PERIODIC_PROFILING
	}
#endif
}

PerfMarker::~PerfMarker()
{
	endMark();
}

void PerfMarker::endMark()
{
	if(_onStack)
	{	PerfManager::instance()->exitFunction();
		_onStack = false;
	}
}

#endif
