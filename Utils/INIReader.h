#ifndef INIREADER_H
#define INIREADER_H

#include <Windows.h>
#include <Types.h>
#include <stack>
#include <string>

// Kaiden: This will read any value out of 
// an INI file as long as the correct type is specified.
// Methods should be fairly self explainatory
//
// Note: readers without valid range parameters will be removed to "encourage" developers to specify
// valid ranges.  Consider carefully what those values should be.
//

// Queue of error messages.  They are queued because screen is not ready for I/O yet.
extern std::stack<std::string> iniErrorMessages;

class CIniReader
{
public:
	CIniReader(const STR8 szFileName);

	// Warning: the following function will be removed
	int ReadInteger(const STR8 szSection, const STR8 szKey, int iDefaultValue);

	int ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue);

	// Warning: the following function will be removed
	//double ReadDouble(const STR8 szSection, const STR8 szKey, double fltDefaultValue);

	double ReadDouble(const STR8 szSection, const STR8 szKey, double defaultValue, double minValue, double maxValue);

	bool ReadBoolean(const STR8	szSection, const STR8 szKey, bool bolDefaultValue);

	STR8 ReadString(const STR8 szSection, const STR8 szKey, const STR8	szDefaultValue);

private:
	char m_szFileName[MAX_PATH];
};

#endif//INIREADER_H