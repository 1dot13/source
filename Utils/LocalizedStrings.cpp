#include "LocalizedStrings.h"

#include <vfs/Tools/vfs_tools.h>
#include <vfs/Tools/vfs_property_container.h>

bool g_bUseXML_Strings = false;

namespace Loc
{
	void Init(Topic t, vfs::String const& section);
	void Clear(Topic t);
	void ClearAll();

	class _Strings
	{
	public:
		_Strings() : initialized(false) {};
		bool					initialized;
		vfs::PropertyContainer	stringMap;
	};
	class _PropState
	{
	public:
		_PropState() : loaded(false) {};
		bool		loaded;
		vfs::Path	filename;
	};

	typedef std::map<vfs::String,_PropState,vfs::String::Less> tSectionState;

	static std::map<Topic, vfs::PropertyContainer>	_localizedStrings;
	static std::map<Topic, tSectionState>			_topicFiles;
};



bool Loc::AssociateWithFile(Loc::Topic t, vfs::Path const& sFilename)
{
	_topicFiles[t][L"_ALL"].filename = sFilename;
	_topicFiles[t][L"_ALL"].loaded = false;
	return true;
}

bool Loc::AssociateWithFile(Topic t, vfs::Path const& sFilename, vfs::String const& section)
{
	_topicFiles[t][section].filename = sFilename;
	_topicFiles[t][section].loaded = false;
	return false;
}


bool Loc::GetString(Loc::Topic t, vfs::String const& section, vfs::String const& key, vfs::String& value)
{
	Init(t,section);
	return _localizedStrings[t].getStringProperty(section, key, value);
}
bool Loc::GetString(Loc::Topic t, vfs::String const& section, int key, vfs::String& value)
{
	return GetString(t, section, vfs::toString<wchar_t>(key), value);
}

bool Loc::GetString(Topic t, vfs::String const& section, vfs::String const& key, vfs::String::char_t* value, vfs::UInt32 len)
{
	Init(t,section);
	return _localizedStrings[t].getStringProperty(section, key, value, len);
}
bool Loc::GetString(Topic t, vfs::String const& section, int key, vfs::String::char_t* value, vfs::UInt32 len)
{
	return GetString(t, section, vfs::toString<wchar_t>(key), value, len);
}

vfs::String const& Loc::GetString(Topic t, vfs::String const& section, vfs::String const& key)
{
	Init(t,section);
	return _localizedStrings[t].getStringProperty(section, key);
}
vfs::String const& Loc::GetString(Topic t, vfs::String const& section, int key)
{
	return GetString(t,section,vfs::toString<wchar_t>(key));
}



void Loc::Init(Topic t, vfs::String const& section)
{
	_PropState& state = _topicFiles[t][L"_ALL"];
	if(!state.filename.empty() && !state.loaded)
	{
		_localizedStrings[t].initFromXMLFile(state.filename, vfs::PropertyContainer::TagMap());
		state.loaded = true;
	}
	state = _topicFiles[t][section];
	if(!state.filename.empty() && !state.loaded)
	{
		_localizedStrings[t].initFromXMLFile(state.filename, vfs::PropertyContainer::TagMap());
		state.loaded = true;
	}
}

void Loc::Clear(Topic t)
{
	_localizedStrings[t].clearContainer();
	tSectionState::iterator it = _topicFiles[t].begin();
	for(; it != _topicFiles[t].end(); ++it)
	{
		it->second.loaded = false;
	}

}

void Loc::ClearAll()
{
	std::map<Topic, tSectionState>::iterator sit = _topicFiles.begin();
	for(; sit != _topicFiles.end(); ++sit)
	{
		tSectionState::iterator it = sit->second.begin();
		for(; it != sit->second.end(); ++it)
		{
			it->second.loaded = false;
		}
	}
	_localizedStrings.clear();
}




