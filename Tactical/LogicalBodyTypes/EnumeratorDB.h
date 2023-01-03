#pragma once
#ifndef _LBT_ENUMERATORDB__H_
#define _LBT_ENUMERATORDB__H_

#include "LogicalBodyTypes/Singleton.h"
#include <string>
#include <map>
#include <stdarg.h>
#include <iostream>

#define LOGBT_ENUMDB_ADD(enumName, count, ...) EnumeratorDB::Instance().AddEnum(enumName, count, #__VA_ARGS__, __VA_ARGS__)

namespace LogicalBodyTypes {

class EnumeratorDB : public Singleton<EnumeratorDB>  {

public:
	typedef std::map<std::string, int> EnumeratorMap;
	typedef std::map<std::string, EnumeratorMap> EnumerationMap;
	EnumerationMap enumerations;

public:
	EnumeratorDB();
	~EnumeratorDB();
	void AddEnum(std::string enumName, int num, char* strEnumerators, ...);
	// Note: this will return the FIRST enumerator of the specified enumeration
	// that matches the provided ordinal value!
	// Also: it is expected that this will only be called in situations where the efficiency is not relevant
	// (like for generating a list of enumerators for testing/documentation)
	// otherwise a different mechanism for storing the enumerations should be implemented.
	bool GetEnumeratorStr(const char* enumeration, int enumerator, std::string& str);
	bool GetEnumeratorByStr(const char* enumeration, const char* enumerationStr, int& enumerator);

};

};

#endif
