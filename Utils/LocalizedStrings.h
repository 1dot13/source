#ifndef _LOCALIZEDSTRINGS_H_
#define _LOCALIZEDSTRINGS_H_

#include <vfs/Core/vfs_path.h>

//#define USE_LOCALIZATION

namespace Loc
{
	enum Language
	{
		Chinese,
		Dutch,
		English,
		French,
		German,
		Italian,
		Polish,
		Russian,
		Taiwanese,
	};

	static const wchar_t* LangSuffix [] = {
		L"_cn",
		L"_nl",
		L"_en",
		L"_fr",
		L"_de",
		L"_it",
		L"_pl",
		L"_ru",
		L"_tw",
	};

	enum Topic
	{
		AIM_BIOGRAPHY,
		AIM_HISTORY,
		AIM_POLICY,
		GAME_STRINGS,
		DIALOGUE,
	};

	bool AssociateWithFile(Topic t, vfs::Path const& sFilename);
	bool AssociateWithFile(Topic t, vfs::Path const& sFilename, vfs::String const& section);

	bool GetString(Topic t, vfs::String const& section, vfs::String const& key, vfs::String& value);
	bool GetString(Topic t, vfs::String const& section, int key, vfs::String& value);

	bool GetString(Topic t, vfs::String const& section, vfs::String const& key, vfs::String::char_t* value, vfs::UInt32 len);
	bool GetString(Topic t, vfs::String const& section, int key, vfs::String::char_t* value, vfs::UInt32 len);
	
	vfs::String const& GetString(Topic t, vfs::String const& section, vfs::String const& key);
	vfs::String const& GetString(Topic t, vfs::String const& section, int key);
};

extern bool g_bUseXML_Strings;

#endif // _LOCALIZEDSTRINGS_H_
