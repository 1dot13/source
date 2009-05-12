#include "builddefines.h"
#include "IniReader.h"
#include "FileMan.h"
#include "debug.h"
#include "Font Control.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <sstream>

#include <float.h> //limits for float/double vars ie. DBL_MAX FLT_MAX

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

CIniReader::CIniReader(const STR8	szFileName, BOOL Force_Custom_Data_Path)
{
	// ary-05/05/2009 : force custom data path for potential non existing file -or- force default data path
	//       : Also, flag file detection to allow functions to determine course of action for case of file [not found/is found].
	if ( Force_Custom_Data_Path ) 
	{
		if ( gCustomDataCat.FindFile(szFileName) )
		{
			CIniReader_File_Found = TRUE;
		}
		else
		{
			CIniReader_File_Found = FALSE;
		}
		sprintf(m_szFileName, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), szFileName);
	}
	else 
	{
		if ( gDefaultDataCat.FindFile(szFileName) )
		{
			CIniReader_File_Found = TRUE;
		}
		else
		{
			CIniReader_File_Found = FALSE;
		}
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
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return minValue;
	} else if (iniValueReadFromFile > maxValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
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

DOUBLE CIniReader::ReadDouble(const STR8 szSection, const STR8 szKey, DOUBLE defaultValue, DOUBLE minValue, DOUBLE maxValue)
{
	char szResult[255];
	char szDefault[255];
	DOUBLE iniValueReadFromFile;

	sprintf(szDefault, "%f", defaultValue);
	GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
	iniValueReadFromFile = strtod(szResult, NULL);

	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);

	if (iniValueReadFromFile < minValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return minValue;
	} else if (iniValueReadFromFile > maxValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << maxValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return maxValue;
	}
	return iniValueReadFromFile;
}

FLOAT CIniReader::ReadFloat(const STR8 szSection, const STR8 szKey, FLOAT defaultValue, FLOAT minValue, FLOAT maxValue)
{
	char  szResult[255];
	char  szDefault[255];
	FLOAT iniValueReadFromFile;

	sprintf(szDefault, "%f", defaultValue);
	GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
	iniValueReadFromFile = (FLOAT) atof(szResult);

	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);

	if (iniValueReadFromFile < minValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return minValue;
	} else if (iniValueReadFromFile > maxValue) {
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << maxValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		return maxValue;
	}
	return iniValueReadFromFile;
}

BOOL CIniReader::ReadBoolean(const STR8 szSection, const STR8 szKey, BOOL defaultValue)
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



UINT8  CIniReader::ReadUINT8(const STR8 szSection, const STR8 szKey, UINT8  defaultValue, UINT8  minValue, UINT8  maxValue)
{

	UINT8 iniValueReadFromFile;

	iniValueReadFromFile = (UINT8) this->ReadUINT( szSection,  szKey, (UINT32) defaultValue, (UINT32) minValue, (UINT32) maxValue);

	return iniValueReadFromFile;

}

UINT16 CIniReader::ReadUINT16(const STR8 szSection, const STR8 szKey, UINT16 defaultValue, UINT16 minValue, UINT16 maxValue)
{

	UINT16 iniValueReadFromFile;

	iniValueReadFromFile = (UINT16) this->ReadUINT( szSection,  szKey, (UINT32) defaultValue, (UINT32) minValue, (UINT32) maxValue);

	return iniValueReadFromFile;

}

UINT32 CIniReader::ReadUINT32(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue)
{

	UINT32 iniValueReadFromFile;

	iniValueReadFromFile = (UINT32) this->ReadUINT( szSection,  szKey, (UINT32) defaultValue, (UINT32) minValue, (UINT32) maxValue);

	return iniValueReadFromFile;

}

UINT32 CIniReader::ReadUINT(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue )
{ 
	UINT32 iniValueReadFromFile;

	STR8	szResult = new char[255];
	STR8	szDefault = new char[255];

	memset(szResult, 0x00, 255);
	memset(szDefault, 0x00, 255);

	sprintf(szDefault, "%u", defaultValue);

	sprintf_s(	szResult , (size_t) 255 , "%s",   this->ReadString (szSection , szKey , szDefault ));
	iniValueReadFromFile = (UINT32) strtoul(szResult,NULL,0);

	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);

	if (iniValueReadFromFile < minValue) 
	{
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << minValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		iniValueReadFromFile = minValue;
	} 
	else if (iniValueReadFromFile > maxValue) 
	{
		std::stringstream errMessage;
		errMessage << "The value of the .ini setting " << szKey << "(" << iniValueReadFromFile
			<< ") is outside the valid range of " << minValue << " to " << maxValue
			<< ".  " << maxValue << " will be used.";
		iniErrorMessages.push(errMessage.str());
		iniValueReadFromFile = maxValue;
	}


	delete [] szResult  ;
	delete [] szDefault ;

	return iniValueReadFromFile;

}



