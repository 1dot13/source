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
	CIniReader(const STR8 szFileName, BOOL Force_Custom_Data_Path); // force path for nonexisting INI files

	// Warning: the following function will be removed
	int ReadInteger(const STR8 szSection, const STR8 szKey, int iDefaultValue);
	int ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue);

	//UINT32 CIniReader::testReadUINT32(void);//various limit tests of UINT and double/float handling
	//front end functions that control type interpretation and range control, each calls internal ReadUINT
	UINT32 CIniReader::ReadUINT32(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue);
	UINT16 CIniReader::ReadUINT16(const STR8 szSection, const STR8 szKey, UINT16 defaultValue, UINT16 minValue, UINT16 maxValue);
	UINT8  CIniReader::ReadUINT8 (const STR8 szSection, const STR8 szKey, UINT8  defaultValue, UINT8  minValue, UINT8  maxValue);

	// Warning: the following function will be removed
	//double ReadDouble(const STR8 szSection, const STR8 szKey, double fltDefaultValue);

	// Read_reals
	DOUBLE ReadDouble(const STR8 szSection, const STR8 szKey, DOUBLE defaultValue, DOUBLE minValue, DOUBLE maxValue);
	FLOAT  ReadFloat (const STR8 szSection, const STR8 szKey, FLOAT  defaultValue, FLOAT  minValue, FLOAT  maxValue);

	BOOL ReadBoolean(const STR8	szSection, const STR8 szKey, BOOL bolDefaultValue);

	STR8 ReadString(const STR8 szSection, const STR8 szKey, const STR8	szDefaultValue);
	
	BOOL Is_CIniReader_File_Found(void) {return (CIniReader_File_Found);}

private:
	char m_szFileName[MAX_PATH];
	BOOL CIniReader_File_Found;

	UINT32 CIniReader::ReadUINT(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue);

};

#endif//INIREADER_H