#ifndef INIREADER_H
#define INIREADER_H

#include <Windows.h>

// Kaiden: This will read any value out of 
// an INI file as long as the correct type is specified.
// Methods should be fairly self explainatory:
class CIniReader
{
public:
	CIniReader(const char* szFileName);
	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue);
	float ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue);
	char* ReadString(const char* szSection, const char* szKey, const char* szDefaultValue);
private:
  char m_szFileName[MAX_PATH];
};

#endif//INIREADER_H