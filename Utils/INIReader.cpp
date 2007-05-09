#include "IniReader.h"
#include "FileMan.h"
#include <stdio.h>
#include <string.h>

// Kaiden: INI reading function definitions:


CIniReader::CIniReader(const STR8  szFileName)
{
	// Snap: Look for the INI file in the custom Data directory.
	// If not there, leave at default location.
	if ( gCustomDataCat.FindFile(szFileName) ) {
		sprintf(m_szFileName, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), szFileName);
	}
	else {
		sprintf(m_szFileName, "%s\\%s", gDefaultDataCat.GetRootDir().c_str(), szFileName);
	}
}


int CIniReader::ReadInteger(const STR8  szSection, const STR8  szKey, int iDefaultValue)
{
	return GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
}


int CIniReader::ReadInteger(const STR8  szSection, const STR8  szKey, int iDefaultValue, int iMinValue, int iMaxValue)
{
	int i = GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
	if (i < iMinValue)
		return iMinValue;
	else if (i > iMaxValue)
		return iMaxValue;
	return i;
}

	int ReadInteger(const STR8  szSection, const STR8  szKey, int iDefaultValue, int iMinValue, int iMaxValue);



float CIniReader::ReadFloat(const STR8  szSection, const STR8  szKey, float fltDefaultValue)
{
 char szResult[255];
 char szDefault[255];
 float fltResult;
 sprintf(szDefault, "%f",fltDefaultValue);
 GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
 fltResult = (float) atof(szResult);
 return fltResult;
}


bool CIniReader::ReadBoolean(const STR8  szSection, const STR8  szKey, bool bolDefaultValue)
{
 char szResult[255];
 char szDefault[255];
 bool bolResult;
 sprintf(szDefault, "%s", bolDefaultValue? "TRUE" : "FALSE");
 GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
 bolResult =  (strcmp(szResult, "TRUE") == 0 || strcmp(szResult, "TRUE") == 0) ? true : false;
 return bolResult;
}


STR8  CIniReader::ReadString(const STR8  szSection, const STR8  szKey, const STR8  szDefaultValue)
{
 STR8  szResult = new char[255];
 memset(szResult, 0x00, 255);
 GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName); 
 return szResult;
}






