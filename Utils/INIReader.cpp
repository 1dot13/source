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

#include "VFS/vfs.h"

std::stack<std::string> iniErrorMessages;

template<typename ValueType>
void PushErrorMessage(std::string const& filename,
					  std::string const& section,
					  std::string const& key, 
					  ValueType value, ValueType used_value,
					  ValueType minVal, ValueType maxVal)
{
	std::stringstream errMessage;
	errMessage << "The value [" << section << "][" <<  key << "] = \"" << value << "\" "
		<< "in file [" << filename << "] "
		<< "is outside the valid range [" << minVal << " , " << maxVal << "].  "
		<< used_value << " will be used.";
	iniErrorMessages.push(errMessage.str());
}

CIniReader::CIniReader(const STR8	szFileName)
{
	memset(m_szFileName,0,sizeof(m_szFileName));
#ifndef USE_VFS
	// Snap: Look for the INI file in the custom Data directory.
	// If not there, leave at default location.
	if ( gCustomDataCat.FindFile(szFileName) ) {
		sprintf(m_szFileName, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), szFileName);
	}
	else {
		sprintf(m_szFileName, "%s\\%s", gDefaultDataCat.GetRootDir().c_str(), szFileName);
	}
#else
	strncpy(m_szFileName,szFileName, std::min<int>(strlen(szFileName), sizeof(m_szFileName)-1));
	m_oProps.InitFromIniFile(vfs::Path(szFileName));
#endif
}

CIniReader::CIniReader(const STR8	szFileName, BOOLEAN Force_Custom_Data_Path)
{
	memset(m_szFileName,0,sizeof(m_szFileName));
	// ary-05/05/2009 : force custom data path for potential non existing file -or- force default data path
	//       : Also, flag file detection to allow functions to determine course of action for case of file [not found/is found].
#ifndef USE_VFS
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
#else
	strncpy(m_szFileName,szFileName, std::min<int>(strlen(szFileName), sizeof(m_szFileName)-1));
	CIniReader_File_Found = m_oProps.InitFromIniFile(vfs::Path(szFileName));
#endif
}

void CIniReader::Clear()
{
#ifndef USE_VFS
	memset(m_szFileName, 0, MAX_PATH);
#else
	memset(m_szFileName, 0, MAX_PATH);
	m_oProps.ClearContainer();
#endif
}


int CIniReader::ReadInteger(const STR8	szSection, const STR8	szKey, int iDefaultValue)
{
#ifndef USE_VFS
	return GetPrivateProfileInt(szSection,	szKey, iDefaultValue, m_szFileName);
#else
	return m_oProps.GetIntProperty(szSection, szKey, iDefaultValue);
#endif
}


int CIniReader::ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue)
{
#ifndef USE_VFS
	int iniValueReadFromFile = GetPrivateProfileInt(szSection,	szKey, defaultValue, m_szFileName);
#else
	int iniValueReadFromFile = m_oProps.GetIntProperty(szSection, szKey, defaultValue);
#endif
	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);
	if (iniValueReadFromFile < minValue)
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, minValue, minValue, maxValue);
		return minValue;
	} 
	else if (iniValueReadFromFile > maxValue)
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, maxValue, minValue, maxValue);
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
	double iniValueReadFromFile;
#ifndef USE_VFS
	char szResult[255];
	char szDefault[255];
	sprintf(szDefault, "%f", defaultValue);
	GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
	iniValueReadFromFile = (float) atof(szResult);
#else
	iniValueReadFromFile = m_oProps.GetFloatProperty(szSection, szKey, defaultValue);
#endif
	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);
	if (iniValueReadFromFile < minValue)
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey,iniValueReadFromFile, minValue, minValue, maxValue);
		return minValue;
	}
	else if (iniValueReadFromFile > maxValue)
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, maxValue, minValue, maxValue);
		return maxValue;
	}
	return iniValueReadFromFile;
}

FLOAT CIniReader::ReadFloat(const STR8 szSection, const STR8 szKey, FLOAT defaultValue, FLOAT minValue, FLOAT maxValue)
{
	FLOAT iniValueReadFromFile;
#ifndef USE_VFS
	char  szResult[255];
	char  szDefault[255];
	sprintf(szDefault, "%f", defaultValue);
	GetPrivateProfileString(szSection,	szKey, szDefault, szResult, 255, m_szFileName);
	iniValueReadFromFile = (FLOAT) atof(szResult);
#else
	iniValueReadFromFile = (FLOAT) m_oProps.GetFloatProperty(szSection, szKey, (float)defaultValue);
#endif

	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);

	if (iniValueReadFromFile < minValue) 
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, minValue, minValue, maxValue);
		return minValue;
	}
	else if (iniValueReadFromFile > maxValue)
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, maxValue, minValue, maxValue);
		return maxValue;
	}
	return iniValueReadFromFile;
}

BOOLEAN CIniReader::ReadBoolean(const STR8 szSection, const STR8 szKey, bool defaultValue)
{
#ifndef USE_VFS
	char szResult[255];
	char szDefault[255];
	sprintf(szDefault, "%s", defaultValue? "TRUE" : "FALSE");
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	for (int idx=0; (szResult[idx] != 0) && (idx < 255); ++idx)
		szResult[idx] = toupper(szResult[idx]);
	if (strcmp(szResult, "TRUE") == 0)
		return TRUE;
	else if (strcmp(szResult, "FALSE") == 0)
		return FALSE;
#else
	utf8string str = m_oProps.GetStringProperty(szSection, szKey, L"");
	if( StrCmp::Equal(str, L"true") )
	{
		return TRUE;
	}
	else if( StrCmp::Equal(str, L"false") )
	{
		return FALSE;
	}
	std::string szResult = str.utf8();
	char szDefault[255];
	sprintf(szDefault, "%s", defaultValue? "TRUE" : "FALSE");
#endif
	std::stringstream errMessage;
	errMessage << "The value [" << szSection << "][" << szKey << "] = \"" << szResult << "\" "
		<< "in file [" << this->m_szFileName << "] is neither TRUE nor FALSE.  The value " << szDefault << " will be used.";
	iniErrorMessages.push(errMessage.str());
	return defaultValue;
}

// ary-05/15/2009 : snippet on how to use CIniReader::ReadString
//	const  STR8 test_ini_string = new char[255];
//	memset(test_ini_string, 0x00, 255);
//	iniReader.ReadString("JA2 Game Settings" , "TEST_STRING" , "default string" , test_ini_string , 255 );

void CIniReader::ReadString(const STR8 szSection, const STR8 szKey, const STR8 szDefaultValue, STR8 input_buffer, size_t buffer_size)
{
#ifndef USE_VFS
	GetPrivateProfileString(szSection,	szKey, szDefaultValue, input_buffer, buffer_size, m_szFileName);
#else
	std::string s = m_oProps.GetStringProperty(szSection, szKey, szDefaultValue).utf8();
	int len = std::min<unsigned int>(s.length(),buffer_size-1);
	strncpy(input_buffer, s.c_str(), len);
	input_buffer[len] = 0;
#endif
}

// WANNE - MP: Old version, currently used by Multiplayer
STR8	CIniReader::ReadString(const STR8	szSection, const STR8	szKey, const STR8	szDefaultValue)
{
	// >>>>> Memory Leak <<<<<
	STR8	szResult = new char[255];
	memset(szResult, 0x00, 255);
#ifndef USE_VFS
	GetPrivateProfileString(szSection,	szKey, szDefaultValue, szResult, 255, m_szFileName);
#else
	std::string s = m_oProps.GetStringProperty(szSection, szKey, szDefaultValue).utf8();
	strncpy(szResult, s.c_str(), std::min<int>(s.length(),254));
#endif
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
#ifndef USE_VFS
	STR8	szResult = new char[255];
	STR8	szDefault = new char[255];

	memset(szResult, 0x00, 255);
	memset(szDefault, 0x00, 255);

	sprintf(szDefault, "%u", defaultValue);

	this->ReadString (szSection , szKey , szDefault, szResult, (size_t) 255 );
	iniValueReadFromFile = (UINT32) strtoul(szResult,NULL,0);
#else
	iniValueReadFromFile = (UINT32) m_oProps.GetUIntProperty(szSection, szKey, defaultValue);
#endif
	//AssertGE(iniValueReadFromFile, minValue);
	//AssertLE(iniValueReadFromFile, maxValue);

	if (iniValueReadFromFile < minValue) 
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, minValue, minValue, maxValue);
		iniValueReadFromFile = minValue;
	} 
	else if (iniValueReadFromFile > maxValue) 
	{
		PushErrorMessage(this->m_szFileName, szSection, szKey, iniValueReadFromFile, maxValue, minValue, maxValue);
		iniValueReadFromFile = maxValue;
	}

#ifndef USE_VFS
	delete [] szResult  ;
	delete [] szDefault ;
#endif
	return iniValueReadFromFile;

}
