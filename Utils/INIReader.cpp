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

#include <vfs/Core/vfs.h>

#ifdef USE_VFS
std::set<vfs::Path,vfs::Path::Less> CIniReader::m_merge_files;
#endif
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

#ifdef USE_VFS
void CIniReader::RegisterFileForMerging(vfs::Path const& filename)
{
	m_merge_files.insert(filename);
}
#endif

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
	if(m_merge_files.find(szFileName) == m_merge_files.end())
	{
		m_oProps.initFromIniFile(vfs::Path(szFileName));
	}
	else
	{
		vfs::CProfileStack* profs = getVFS()->getProfileStack();
		vfs::CProfileStack::Iterator it = profs->begin();
		std::stack<vfs::CVirtualProfile*> rev_order;
		for(; !it.end(); it.next()) { rev_order.push(it.value()); }
		while(!rev_order.empty())
		{
			vfs::IBaseFile* file = rev_order.top()->getFile(szFileName);
			if(file)
			{
				m_oProps.initFromIniFile(vfs::tReadableFile::cast(file));
			}
			rev_order.pop();
		}
	}
	// check for override file
	{
		CHAR8 OvrFileName[256], Drive[128], Dir[128], Name[128], Ext[128];
		_splitpath(szFileName, Drive, Dir, Name, Ext);
		_makepath(OvrFileName, Drive, Dir, Name, "Override");
		if(getVFS()->fileExists(OvrFileName))
			m_oProps.initFromIniFile(vfs::Path(OvrFileName));
	}
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
	if(m_merge_files.find(szFileName) == m_merge_files.end())
	{
		CIniReader_File_Found = m_oProps.initFromIniFile(vfs::Path(szFileName));
	}
	else
	{
		CIniReader_File_Found = TRUE;
		vfs::CProfileStack* profs = getVFS()->getProfileStack();
		vfs::CProfileStack::Iterator it = profs->begin();
		std::stack<vfs::CVirtualProfile*> rev_order;
		for(; !it.end(); it.next()) { rev_order.push(it.value()); }
		while(!rev_order.empty())
		{
			vfs::IBaseFile* file = rev_order.top()->getFile(szFileName);
			if(file)
			{
				CIniReader_File_Found = ((CIniReader_File_Found != FALSE) && m_oProps.initFromIniFile(vfs::tReadableFile::cast(file))) ? TRUE : FALSE;
			}
			rev_order.pop();
		}
	}

#endif
}

void CIniReader::Clear()
{
#ifndef USE_VFS
	memset(m_szFileName, 0, MAX_PATH);
#else
	memset(m_szFileName, 0, MAX_PATH);
	m_oProps.clearContainer();
#endif
}


int CIniReader::ReadInteger(const STR8	szSection, const STR8	szKey, int iDefaultValue)
{
#ifndef USE_VFS
	return GetPrivateProfileInt(szSection,	szKey, iDefaultValue, m_szFileName);
#else
	return (int)(m_oProps.getIntProperty(szSection, szKey, iDefaultValue));
#endif
}


int CIniReader::ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue)
{
#ifndef USE_VFS
	int iniValueReadFromFile = (int)(GetPrivateProfileInt(szSection,	szKey, defaultValue, m_szFileName));
#else
	int iniValueReadFromFile = (int)(m_oProps.getIntProperty(szSection, szKey, defaultValue));
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
	iniValueReadFromFile = m_oProps.getFloatProperty(szSection, szKey, defaultValue);
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
	iniValueReadFromFile = (FLOAT) m_oProps.getFloatProperty(szSection, szKey, (float)defaultValue);
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

void CIniReader::ReadFloatArray(const STR8 szSection, const STR8 szKey, std::vector<FLOAT>& vec)
{
	// read the array as a string
	STRING512 textBuffer;
	ReadString(szSection, szKey, "", textBuffer, _countof(textBuffer));

	std::string str, token;
	std::string delim = ",";
	size_t offset = 0, prevOffset = 0;

	// sanitise input
	vec.clear();
	str = textBuffer;
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

	// split into array
	try
	{
		do
		{
			offset = str.find(delim, prevOffset);
			token = str.substr(prevOffset, offset - prevOffset);
			prevOffset = offset + delim.length();

			vec.push_back(std::stof(token.c_str()));
		} while (offset != std::string::npos);
	}
	catch (...)
	{
		std::stringstream errMessage;
		errMessage << "There was an error reading array [" << szSection << "][" << szKey << "] in file [" << m_szFileName << "]. Defaulting to [0].";
		iniErrorMessages.push(errMessage.str());

		vec.push_back(0);
	}
}

void CIniReader::ReadINT32Array(const STR8 szSection, const STR8 szKey, std::vector<INT32>& vec)
{
	// read the array as a string
	STRING512 textBuffer;
	ReadString(szSection, szKey, "", textBuffer, _countof(textBuffer));

	std::string str, token;
	std::string delim = ",";
	size_t offset = 0, prevOffset = 0;

	// sanitise input
	vec.clear();
	str = textBuffer;
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

	// split into array
	try
	{
		do
		{
			offset = str.find(delim, prevOffset);
			token = str.substr(prevOffset, offset - prevOffset);
			prevOffset = offset + delim.length();

			vec.push_back(std::stoi(token.c_str()));
		} while (offset != std::string::npos);
	}
	catch (...)
	{
		std::stringstream errMessage;
		errMessage << "There was an error reading array [" << szSection << "][" << szKey << "] in file [" << m_szFileName << "]. Defaulting to [0].";
		iniErrorMessages.push(errMessage.str());

		vec.push_back(0);
	}
}

BOOLEAN CIniReader::ReadBoolean(const STR8 szSection, const STR8 szKey, bool defaultValue, bool bolDisplayError)
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
	vfs::String str = m_oProps.getStringProperty(szSection, szKey, L"");
	if( vfs::StrCmp::Equal(str, L"true") )
	{
		return TRUE;
	}
	else if( vfs::StrCmp::Equal(str, L"false") )
	{
		return FALSE;
	}
	std::string szResult = str.utf8();
	char szDefault[255];
	sprintf(szDefault, "%s", defaultValue? "TRUE" : "FALSE");
#endif
	if(bolDisplayError){
		std::stringstream errMessage;
		errMessage << "The value [" << szSection << "][" << szKey << "] = \"" << szResult << "\" "
			<< "in file [" << this->m_szFileName << "] is neither TRUE nor FALSE.  The value " << szDefault << " will be used.";
		iniErrorMessages.push(errMessage.str());
	}
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
	std::string s = m_oProps.getStringProperty(szSection, szKey, szDefaultValue).utf8();
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
	std::string s = m_oProps.getStringProperty(szSection, szKey, szDefaultValue).utf8();
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
	iniValueReadFromFile = (UINT32) m_oProps.getUIntProperty(szSection, szKey, defaultValue);
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
