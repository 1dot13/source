#ifndef INIREADER_H
#define INIREADER_H
#define NOMINMAX
#include <algorithm>
#include <Windows.h>
#include <Types.h>
#include <stack>
#include <string>

#include <vfs/Tools/vfs_property_container.h>

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
	CIniReader(const STR8 szFileName, BOOLEAN Force_Custom_Data_Path); // force path for nonexisting INI files

	// Warning: the following function will be removed
	int ReadInteger(const STR8 szSection, const STR8 szKey, int iDefaultValue);
	int ReadInteger(const STR8 szSection, const STR8 szKey, int defaultValue, int minValue, int maxValue);

	//UINT32 CIniReader::testReadUINT32(void);//various limit tests of UINT and double/float handling
	//front end functions that control type interpretation and range control, each calls internal ReadUINT
	UINT32 ReadUINT32(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue);
	UINT16 ReadUINT16(const STR8 szSection, const STR8 szKey, UINT16 defaultValue, UINT16 minValue, UINT16 maxValue);
	UINT8  ReadUINT8 (const STR8 szSection, const STR8 szKey, UINT8  defaultValue, UINT8  minValue, UINT8  maxValue);

	// Warning: the following function will be removed
	//double ReadDouble(const STR8 szSection, const STR8 szKey, double fltDefaultValue);

	// Read_reals
	DOUBLE ReadDouble(const STR8 szSection, const STR8 szKey, DOUBLE defaultValue, DOUBLE minValue, DOUBLE maxValue);
	FLOAT  ReadFloat (const STR8 szSection, const STR8 szKey, FLOAT  defaultValue, FLOAT  minValue, FLOAT  maxValue);

	void ReadFloatArray(const STR8 szSection, const STR8 szKey, std::vector<FLOAT>& vec);
	void ReadINT32Array(const STR8 szSection, const STR8 szKey, std::vector<INT32>& vec);

	BOOLEAN ReadBoolean(const STR8	szSection, const STR8 szKey, bool bolDefaultValue, bool bolDisplayError = true);

	void ReadString(const STR8 szSection, const STR8 szKey, const STR8 szDefaultValue, STR8 input_buffer, size_t buffer_size);

	// WANNE - MP: Old version, currently used by Multiplayer
	STR8 ReadString(const STR8 szSection, const STR8 szKey, const STR8	szDefaultValue);
	
	BOOLEAN Is_CIniReader_File_Found(void) {return (CIniReader_File_Found);}
	void Clear();

#ifdef USE_VFS
	static void RegisterFileForMerging(vfs::Path const& filename);
#endif
private:
	vfs::PropertyContainer m_oProps;
	char m_szFileName[MAX_PATH];
	BOOLEAN CIniReader_File_Found;

	UINT32 ReadUINT(const STR8 szSection, const STR8 szKey, UINT32 defaultValue, UINT32 minValue, UINT32 maxValue);
#ifdef USE_VFS
	static std::set<vfs::Path, vfs::Path::Less> m_merge_files;
#endif
};

#endif//INIREADER_H