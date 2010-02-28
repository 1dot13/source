#ifndef _PROPERTY_CONTAINER_H_
#define _PROPERTY_CONTAINER_H_

#include "vfs_types.h"
#include "Interface/vfs_file_interface.h"

#include <map>
#include <string>
#include <list>
#include <set>

class VFS_API CPropertyContainer
{
public:
	class TagMap
	{
		typedef std::map<utf8string,utf8string> tTagMap;
	public:
		TagMap();
		utf8string const& container(utf8string::char_t* container = NULL);
		utf8string const& section(utf8string::char_t* section = NULL);
		utf8string const& sectionID(utf8string::char_t* section_id = NULL);
		utf8string const& key(utf8string::char_t* key = NULL);
		utf8string const& keyID(utf8string::char_t* key_id = NULL);
	private:
		tTagMap _map;
	};
public:
	CPropertyContainer(){};
	~CPropertyContainer(){};
	
	void			clearContainer();

	bool			initFromIniFile(vfs::Path const& sFileName);
	bool			initFromIniFile(vfs::tReadableFile *pFile);
	bool			writeToIniFile(vfs::Path const& sFileName, bool bCreateNew = false);

	bool			initFromXMLFile(vfs::Path const& sFileName, TagMap& tagmap);
	bool			writeToXMLFile(vfs::Path const& sFileName, TagMap& tagmap);

	void			printProperties();

	bool			hasProperty(utf8string const& sSection, utf8string const& sKey);
	//
	utf8string const& getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const& sDefaultValue=L"");
	bool			getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string& sValue, utf8string const& sDefaultValue=L"");
	bool			getStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string::char_t* sValue, vfs::size_t len, utf8string const& sDefaultValue=L"");
	//
	vfs::Int64		getIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue);
	vfs::Int64		getIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 iDefaultValue, vfs::Int64 iMinValue, vfs::Int64 iMaxValue);
	//
	vfs::UInt64		getUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue);
	vfs::UInt64		getUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 iDefaultValue, vfs::UInt64 iMinValue, vfs::UInt64 iMaxValue);
	//
	double			getFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue);
	double			getFloatProperty(utf8string const& sSection, utf8string const& sKey, double fDefaultValue, double fMinValue, double fMaxValue);
	//
	bool			getBoolProperty(utf8string const& sSection, utf8string const& sKey, bool bDefaultValue);
	//
	bool			getStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> &lValueList, utf8string sDefaultValue);
	bool			getIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> &lValueList, vfs::Int64 iDefaultValue);
	bool			getUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> &lValueList, vfs::UInt64 iDefaultValue);
	bool			getFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> &lValueList, double fDefaultValue);
	bool			getBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> &lValueList, bool bDefaultValue);
	//
	void			setStringProperty(utf8string const& sSection, utf8string const& sKey, utf8string const&  sValue);
	//
	void			setIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::Int64 const&  iValue);
	void			setUIntProperty(utf8string const& sSection, utf8string const& sKey, vfs::UInt64 const&  iValue);
	void			setFloatProperty(utf8string const& sSection, utf8string const& sKey, double const& fValue);
	void			setBoolProperty(utf8string const& sSection, utf8string const& sKey, bool const& bValue);
	//
	void			setStringListProperty(utf8string const& sSection, utf8string const& sKey, std::list<utf8string> const&  slValue);
	void			setIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::Int64> const& ilValue);
	void			setUIntListProperty(utf8string const& sSection, utf8string const& sKey, std::list<vfs::UInt64> const& ilValue);
	void			setFloatListProperty(utf8string const& sSection, utf8string const& sKey, std::list<double> const& flValue);
	void			setBoolListProperty(utf8string const& sSection, utf8string const& sKey, std::list<bool> const& blValue);
private:
	enum EOperation
	{
		Error, Set, Add,
	};
	bool			extractSection(utf8string::str_t const& readStr, vfs::size_t startPos, utf8string::str_t& sSection);
	EOperation		extractKeyValue(utf8string::str_t const &readStr, vfs::size_t startPos, utf8string::str_t& sKey, utf8string::str_t& sValue);
private:
	class CSection
	{
		friend class CPropertyContainer;
		typedef std::map<utf8string,utf8string, utf8string::Less> tProps;
	public:
		bool		has(utf8string const& key);
		bool		add(utf8string const& key, utf8string const& value);
		bool		value(utf8string const& key, utf8string& value);
		utf8string&	value(utf8string const& key);
		void		print(std::ostream& out, utf8string::str_t sPrefix = L"");
		void		clear();
	private:
		tProps		mapProps;
	};
	typedef std::map<utf8string, CSection, utf8string::Less> tSections;

	bool			getValueForKey(utf8string const& sSection, utf8string const& sKey, utf8string &sValue);

	CSection&		section(utf8string const& sSection);

	tSections		m_mapProps;
};

#endif // _PROPERTY_CONTAINER_H_
