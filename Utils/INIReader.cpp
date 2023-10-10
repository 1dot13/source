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

std::set<vfs::Path,vfs::Path::Less> CIniReader::m_merge_files;
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

void CIniReader::RegisterFileForMerging(vfs::Path const& filename)
{
	m_merge_files.insert(filename);
}

CIniReader::CIniReader(const STR8	szFileName)
{
	memset(m_szFileName,0,sizeof(m_szFileName));
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
}

CIniReader::CIniReader(const STR8	szFileName, BOOLEAN Force_Custom_Data_Path)
{
	memset(m_szFileName,0,sizeof(m_szFileName));
	// ary-05/05/2009 : force custom data path for potential non existing file -or- force default data path
	//       : Also, flag file detection to allow functions to determine course of action for case of file [not found/is found].
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
}

void CIniReader::Clear()
{
	memset(m_szFileName, 0, MAX_PATH);
	m_oProps.clearContainer();
}


int CIniReader::ReadInteger(const STR8	szSection, const STR8	szKey, int iDefaultValue)
{
	return (int)(m_oProps.getIntProperty(szSection, szKey, iDefaultValue));
}


int CIniReader::ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue)
{
	int iniValueReadFromFile = (int)(m_oProps.getIntProperty(szSection, szKey, defaultValue));
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
	iniValueReadFromFile = m_oProps.getFloatProperty(szSection, szKey, defaultValue);
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
	iniValueReadFromFile = (FLOAT) m_oProps.getFloatProperty(szSection, szKey, (float)defaultValue);

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
	std::string s = m_oProps.getStringProperty(szSection, szKey, szDefaultValue).utf8();
	int len = std::min<unsigned int>(s.length(),buffer_size-1);
	strncpy(input_buffer, s.c_str(), len);
	input_buffer[len] = 0;
}

// WANNE - MP: Old version, currently used by Multiplayer
STR8	CIniReader::ReadString(const STR8	szSection, const STR8	szKey, const STR8	szDefaultValue)
{
	// >>>>> Memory Leak <<<<<
	STR8	szResult = new char[255];
	memset(szResult, 0x00, 255);
	std::string s = m_oProps.getStringProperty(szSection, szKey, szDefaultValue).utf8();
	strncpy(szResult, s.c_str(), std::min<int>(s.length(),254));
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
	iniValueReadFromFile = (UINT32) m_oProps.getUIntProperty(szSection, szKey, defaultValue);
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

	return iniValueReadFromFile;
}
