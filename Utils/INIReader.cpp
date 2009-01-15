#include "builddefines.h"
#include "IniReader.h"
#include "FileMan.h"
#include "debug.h"
#include "Font Control.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <sstream>

// Kaiden: INI reading function definitions:

std::stack<std::string> iniErrorMessages;


CIniReader::CIniReader(const STR8	szFileName)
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


int CIniReader::ReadInteger(const STR8	szSection, const STR8	szKey, int iDefaultValue)
{
	return GetPrivateProfileInt(szSection,	szKey, iDefaultValue, m_szFileName);
}


int CIniReader::ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue)
{
	int iniValueReadFromFile = GetPrivateProfileInt(szSection,	szKey, defaultValue, m_szFileName);
	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);
	if (iniValueReadFromFile < minValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outsize the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return minValue;
	} else if (iniValueReadFromFile > maxValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outsize the valid range of " << minValue << " to " << maxValue
			<< ".  " << maxValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return maxValue;
	}
	return iniValueReadFromFile;
}



//float CIniReader::ReadDouble(const STR8	szSection, const STR8	szKey, float fltDefaultValue)
//{
// char szResult[255];
// char szDefault[255];
// float fltResult;
// sprintf(szDefault, "%f",fltDefaultValue);
// GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
// fltResult = (float) atof(szResult);
// return fltResult;
//}

double CIniReader::ReadDouble(const STR8 szSection, const STR8 szKey, double defaultValue, double minValue, double maxValue)
{
	char szResult[255];
	char szDefault[255];
	double iniValueReadFromFile;
	sprintf(szDefault, "%f", defaultValue);
	GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
	iniValueReadFromFile = (float) atof(szResult);
	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);
	if (iniValueReadFromFile < minValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outsize the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return minValue;
	} else if (iniValueReadFromFile > maxValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outsize the valid range of " << minValue << " to " << maxValue
			<< ".  " << maxValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return maxValue;
	}
	return iniValueReadFromFile;
}


bool CIniReader::ReadBoolean(const STR8 szSection, const STR8 szKey, bool defaultValue)
{
 char szResult[255];
 char szDefault[255];
 bool boolResult;
 sprintf(szDefault, "%s", defaultValue? "TRUE" : "FALSE");
 GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
 boolResult =	(strcmp(szResult, "TRUE") == 0 || strcmp(szResult, "TRUE") == 0) ? true : false;
 return boolResult;
}


STR8	CIniReader::ReadString(const STR8	szSection, const STR8	szKey, const STR8	szDefaultValue)
{
 STR8	szResult = new char[255];
 memset(szResult, 0x00, 255);
 GetPrivateProfileString(szSection,	szKey, szDefaultValue, szResult, 255, m_szFileName);
 return szResult;
}







