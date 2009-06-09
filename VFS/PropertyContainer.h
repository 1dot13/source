#ifndef _PROPERTY_CONTAINER_H_
#define _PROPERTY_CONTAINER_H_

#include "vfs_types.h"
#include "Interface/vfs_file_interface.h"

#include <map>
#include <string>
#include <list>
#include <set>

class CPropertyContainer
{
public:
	class TagMap
	{
		typedef std::map<utf8string,utf8string> tTagMap;
	public:
		TagMap();
		utf8string const& Container(utf8string::char_t* container = NULL);
		utf8string const& Section(utf8string::char_t* section = NULL);
		utf8string const& SectionID(utf8string::char_t* section_id = NULL);
		utf8string const& Key(utf8string::char_t* key = NULL);
		utf8string const& KeyID(utf8string::char_t* key_id = NULL);
	private:
		tTagMap _map;
	};
public:
	CPropertyContainer(){};
	~CPropertyContainer(){};
	
	void			ClearContainer();

	bool			InitFromIniFile(vfs::Path const& sFileName);
	bool			InitFromIniFile(vfs::tReadableFile *pFile);
	bool			WriteToIniFile(vfs::Path const& sFileName, bool bCreateNew = false);

	bool			InitFromXMLFile(vfs::Path const& sFileName, TagMap& tagmap);
	bool			WriteToXMLFile(vfs::Path const& sFileName, TagMap& tagmap);

	void			PrintProperties();

	//
	utf8string const& GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sDefaultValue=L"");
	bool			GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string& sValue, utf8string const& sDefaultValue=L"");
	bool			GetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string::char_t* sValue, vfs::UInt32 len, utf8string const& sDefaultValue=L"");
	//
	vfs::Int64		GetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue);
	vfs::Int64		GetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue);
	//
	vfs::UInt64		GetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue);
	vfs::UInt64		GetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue);
	//
	double			GetFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue);
	double			GetFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue, double fMinValue, double fMaxValue);
	//
	bool			GetBoolProperty(utf8string const& sSection, utf8string const& sKey, bool bDefaultValue);
	//
	bool			GetStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> &lValueList, utf8string sDefaultValue);
	bool			GetIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue);
	bool			GetUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue);
	bool			GetFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> &lValueList, double fDefaultValue);
	bool			GetBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> &lValueList, bool bDefaultValue);
	//
	void			SetStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const&  sValue);
	//
	void			SetIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 const&  iValue);
	void			SetUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 const&  iValue);
	void			SetFloatProperty(utf8string const& sSection, utf8string const& sKey, double const& fValue);
	void			SetBoolProperty(utf8string const& sSection, utf8string const& sKey, bool const& bValue);
	//
	void			SetStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> const&  slValue);
	void			SetIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> const& ilValue);
	void			SetUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> const& ilValue);
	void			SetFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> const& flValue);
	void			SetBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> const& blValue);
private:
	enum EOperation
	{
		Error, Set, Add,
	};
	bool			ExtractSection(utf8string::str_t const& readStr, size_t startPos, utf8string::str_t& sSection);
	EOperation		ExtractKeyValue(utf8string::str_t const &readStr, size_t startPos, utf8string::str_t& sKey, utf8string::str_t& sValue);
private:
	class CSection
	{
		friend class CPropertyContainer;
		typedef std::map<utf8string,utf8string, utf8string::Less> tProps;
	public:
		bool		add(utf8string const& key, utf8string const& value);
		bool		value(utf8string const& key, utf8string& value);
		utf8string&	value(utf8string const& key);
		void		Print(std::ostream& out, utf8string::str_t sPrefix = L"");
		void		Clear();
	private:
		tProps		mapProps;
	};
	typedef std::map<utf8string, CSection, utf8string::Less> tSections;

	bool			GetValueForKey(utf8string const& sSection, utf8string const& sKey, utf8string &sValue);

	CSection&		Section(utf8string const& sSection);

	tSections		m_mapProps;
};

#endif // _PROPERTY_CONTAINER_H_
