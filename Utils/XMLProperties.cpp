#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Core/File/vfs_file.h>

#include <vfs/Tools/vfs_tools.h>
#include <vfs/Tools/vfs_property_container.h>

#include "XML_Parser.h"
#include "XMLWriter.h"
#include "Debug.h"

vfs::PropertyContainer::TagMap::TagMap()
{
	// setup default map
	_map[L"Container"] = L"Container";
	_map[L"Section"] = L"Section";
	_map[L"SectionID"] = L"name";
	_map[L"Key"] = L"Key";
	_map[L"KeyID"] = L"name";
}
vfs::String const& vfs::PropertyContainer::TagMap::container(vfs::String::char_t* container)
{
	if(container)
	{
		_map[L"Container"] = container;
	}
	return _map[L"Container"];
}
vfs::String const& vfs::PropertyContainer::TagMap::section(vfs::String::char_t* section)
{
	if(section)
	{
		_map[L"Section"] = section;
	}
	return _map[L"Section"];
}
vfs::String const& vfs::PropertyContainer::TagMap::sectionID(vfs::String::char_t* section_id)
{
	if(section_id)
	{
		_map[L"SectionID"] = section_id;
	}
	return _map[L"SectionID"];
}
vfs::String const& vfs::PropertyContainer::TagMap::key(vfs::String::char_t* key)
{
	if(key)
	{
		_map[L"Key"] = key;
	}
	return _map[L"Key"];
}
vfs::String const& vfs::PropertyContainer::TagMap::keyID(vfs::String::char_t* key_id)
{
	if(key_id)
	{
		_map[L"KeyID"] = key_id;
	}
	return _map[L"KeyID"];
}

bool vfs::PropertyContainer::writeToXMLFile(vfs::Path const& sFileName, vfs::PropertyContainer::TagMap& tagmap)
{
	XMLWriter xmlw;

	xmlw.openNode(tagmap.container());

	tSections::iterator sit = m_mapProps.begin();
	for(; sit != m_mapProps.end(); ++sit)
	{
		xmlw.addAttributeToNextValue(tagmap.sectionID(),sit->first.utf8());
		xmlw.openNode(tagmap.section());

		vfs::PropertyContainer::Section& section = sit->second;
		vfs::PropertyContainer::Section::tProps::iterator kit = section.mapProps.begin();
		for(; kit != section.mapProps.end(); ++kit)
		{
			xmlw.addAttributeToNextValue(tagmap.keyID(), kit->first.utf8());
			xmlw.addValue(tagmap.key(), kit->second.utf8());
		}

		xmlw.closeNode();

	}

	xmlw.closeNode();

	return xmlw.writeToFile(sFileName);
}

/*********************************************************************************/
/*********************************************************************************/

class CPropertyXMLParser : public IXMLParser
{
	enum DOM_OBJECT
	{
		DO_ELEMENT_Container,
		DO_ELEMENT_Section,
		DO_ELEMENT_Key,
		DO_ELEMENT_NONE,
	};
public:
	CPropertyXMLParser(
			vfs::PropertyContainer& container,
			vfs::PropertyContainer::TagMap& tagmap,
			XML_Parser &parser,
			IXMLParser* caller = NULL)
		: IXMLParser("",&parser,caller), 
		_container(container),
		_tagmap(tagmap),
		current_state(DO_ELEMENT_NONE) // doesn't matter where we come from, we start fresh
	{};
	virtual void onStartElement(const XML_Char* name, const XML_Char** atts);
	virtual void onEndElement(const XML_Char* name);
	virtual void onTextElement(const XML_Char *str, int len);
private:
	vfs::PropertyContainer&				_container;
	vfs::PropertyContainer::TagMap&		_tagmap;
	DOM_OBJECT						current_state;
	vfs::String						current_section;
	vfs::String						current_key;
};


void CPropertyXMLParser::onStartElement(const XML_Char *name, const XML_Char **atts)
{
	vfs::String utf8_name(name);
	if(current_state == DO_ELEMENT_NONE && vfs::StrCmp::Equal(utf8_name,_tagmap.container()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && vfs::StrCmp::Equal(utf8_name, _tagmap.section()))
	{
		current_state = DO_ELEMENT_Section;
		current_section = this->getAttribute(_tagmap.sectionID().utf8().c_str(),atts);
	}
	else if(current_state == DO_ELEMENT_Section && vfs::StrCmp::Equal(utf8_name, _tagmap.key()))
	{
		current_state = DO_ELEMENT_Key;
		current_key = this->getAttribute(_tagmap.keyID().utf8().c_str(),atts);
	}
	sCharData = "";
}

void CPropertyXMLParser::onEndElement(const XML_Char* name)
{
	vfs::String utf8_name(name);
	if(current_state == DO_ELEMENT_Key && vfs::StrCmp::Equal(utf8_name, _tagmap.key()))
	{
		_container.setStringProperty(current_section, current_key, vfs::trimString(sCharData,0,sCharData.length()));
		current_state = DO_ELEMENT_Section;
	}
	else if(current_state == DO_ELEMENT_Section && vfs::StrCmp::Equal(utf8_name, _tagmap.section()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && vfs::StrCmp::Equal(utf8_name, _tagmap.container()))
	{
		current_state = DO_ELEMENT_NONE;
	}
}

void CPropertyXMLParser::onTextElement(const XML_Char *str, int len)
{
	if(current_state == DO_ELEMENT_Key)
	{
		sCharData.append(str,len);
	}
}

bool vfs::PropertyContainer::initFromXMLFile(vfs::Path const& sFileName, vfs::PropertyContainer::TagMap& tagmap)
{
	vfs::tReadableFile *file = NULL;
	bool delete_file = false;
	if(getVFS()->fileExists(sFileName))
	{
		vfs::COpenReadFile rfile(sFileName);
		file = &rfile.file();
		rfile.release();
	}
	else
	{
		vfs::CFile* rfile = new vfs::CFile(sFileName);
		delete_file = true;
		file = vfs::tReadableFile::cast(rfile);
		if(!file->openRead())
		{
			delete file;
			return false;
		}
	}
	if(!file)
	{
		return false;
	}

	vfs::size_t size = file->getSize();

	std::vector<vfs::Byte> buffer(size+1);

	SGP_TRYCATCH_RETHROW( file->read(&buffer[0],size), L"" );
	buffer[size] = 0;

	file->close();
	if(delete_file) delete file;

	XML_Parser parser = XML_ParserCreate(NULL);

	CPropertyXMLParser pp(*this,tagmap,parser,NULL);
	pp.grabParser();

	if(!XML_Parse(parser, &buffer[0], size, TRUE))
	{
		std::wstringstream wss;
		wss << L"XML Parser Error in Groups.xml: "
			<< vfs::String::as_utf16(XML_ErrorString(XML_GetErrorCode(parser))) 
			<< L" at line "
			<< XML_GetCurrentLineNumber(parser);
		SGP_THROW(wss.str().c_str());
		//return false;
	}

	return true;
}
