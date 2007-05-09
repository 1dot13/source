#ifndef INIREADER_H
#define INIREADER_H

#include <Windows.h>
#include <Types.h>

// Kaiden: This will read any value out of 
// an INI file as long as the correct type is specified.
// Methods should be fairly self explainatory:
class CIniReader
{
public:
	CIniReader(const STR8  szFileName);
	int ReadInteger(const STR8  szSection, const STR8  szKey, int iDefaultValue);
	int ReadInteger(const STR8  szSection, const STR8  szKey, int iDefaultValue, int iMinValue, int iMaxValue);
	float ReadFloat(const STR8  szSection, const STR8  szKey, float fltDefaultValue);
	bool ReadBoolean(const STR8  szSection, const STR8  szKey, bool bolDefaultValue);
	STR8  ReadString(const STR8  szSection, const STR8  szKey, const STR8  szDefaultValue);
private:
  char m_szFileName[MAX_PATH];
};

#endif//INIREADER_H