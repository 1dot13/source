#include "XML_Parser.h"
#include "vfs_types.h"
#include "PropertyContainer.h"
#include "XMLWriter.h"

#include "VFS/vfs_file_raii.h"
#include "VFS/File/vfs_file.h"

CPropertyContainer::TagMap::TagMap()
{
	// setup default map
	_map[L"Container"] = L"Container";
	_map[L"Section"] = L"Section";
	_map[L"SectionID"] = L"name";
	_map[L"Key"] = L"Key";
	_map[L"KeyID"] = L"name";
}
utf8string const& CPropertyContainer::TagMap::container(utf8string::char_t* container)
{
	if(container)
	{
		_map[L"Container"] = container;
	}
	return _map[L"Container"];
}
utf8string const& CPropertyContainer::TagMap::section(utf8string::char_t* section)
{
	if(section)
	{
		_map[L"Section"] = section;
	}
	return _map[L"Section"];
}
utf8string const& CPropertyContainer::TagMap::sectionID(utf8string::char_t* section_id)
{
	if(section_id)
	{
		_map[L"SectionID"] = section_id;
	}
	return _map[L"SectionID"];
}
utf8string const& CPropertyContainer::TagMap::key(utf8string::char_t* key)
{
	if(key)
	{
		_map[L"Key"] = key;
	}
	return _map[L"Key"];
}
utf8string const& CPropertyContainer::TagMap::keyID(utf8string::char_t* key_id)
{
	if(key_id)
	{
		_map[L"KeyID"] = key_id;
	}
	return _map[L"KeyID"];
}

bool CPropertyContainer::writeToXMLFile(vfs::Path const& sFileName, CPropertyContainer::TagMap& tagmap)
{
	XMLWriter xmlw;

	xmlw.openNode(tagmap.container());

	tSections::iterator sit = m_mapProps.begin();
	for(; sit != m_mapProps.end(); ++sit)
	{
		xmlw.addAttributeToNextValue(tagmap.sectionID(),sit->first.utf8());
		xmlw.openNode(tagmap.section());

		CPropertyContainer::CSection& section = sit->second;
		CPropertyContainer::CSection::tProps::iterator kit = section.mapProps.begin();
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
			CPropertyContainer& container,
			CPropertyContainer::TagMap& tagmap,
			XML_Parser &parser,
			IXMLParser* caller = NULL)
		: IXMLParser("",parser,caller), 
		_container(container),
		_tagmap(tagmap),
		current_state(DO_ELEMENT_NONE) // doesn't matter where we come from, we start fresh
	{};
	virtual void onStartElement(const XML_Char* name, const XML_Char** atts);
	virtual void onEndElement(const XML_Char* name);
	virtual void onTextElement(const XML_Char *str, int len);
private:
	CPropertyContainer&				_container;
	CPropertyContainer::TagMap&		_tagmap;
	DOM_OBJECT						current_state;
	utf8string						current_section;
	utf8string						current_key;
};


void CPropertyXMLParser::onStartElement(const XML_Char *name, const XML_Char **atts)
{
	utf8string utf8_name(name);
	if(current_state == DO_ELEMENT_NONE && StrCmp::Equal(utf8_name,_tagmap.container()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && StrCmp::Equal(utf8_name, _tagmap.section()))
	{
		current_state = DO_ELEMENT_Section;
		current_section = this->getAttribute(_tagmap.sectionID().utf8().c_str(),atts);
	}
	else if(current_state == DO_ELEMENT_Section && StrCmp::Equal(utf8_name, _tagmap.key()))
	{
		current_state = DO_ELEMENT_Key;
		current_key = this->getAttribute(_tagmap.keyID().utf8().c_str(),atts);
	}
	sCharData = "";
}

void CPropertyXMLParser::onEndElement(const XML_Char* name)
{
	utf8string utf8_name(name);
	if(current_state == DO_ELEMENT_Key && StrCmp::Equal(utf8_name, _tagmap.key()))
	{
		_container.setStringProperty(current_section, current_key, vfs::trimString(sCharData,0,sCharData.length()));
		current_state = DO_ELEMENT_Section;
	}
	else if(current_state == DO_ELEMENT_Section && StrCmp::Equal(utf8_name, _tagmap.section()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && StrCmp::Equal(utf8_name, _tagmap.container()))
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

bool CPropertyContainer::initFromXMLFile(vfs::Path const& sFileName, CPropertyContainer::TagMap& tagmap)
{
	vfs::tReadableFile *file = NULL;
	bool delete_file = false;
	try
	{
		vfs::COpenReadFile rfile(sFileName);
		file = &rfile.file();
		rfile.release();
	}
	catch(CBasicException& ex)
	{
		logException(ex);
		vfs::CFile* rfile = new vfs::CFile(sFileName);
		delete_file = true;
		file = vfs::tReadableFile::cast(rfile);
		if(!file->openRead())
		{
			file->close();
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

	TRYCATCH_RETHROW( file->read(&buffer[0],size), L"" );
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
			<< utf8string::as_utf16(XML_ErrorString(XML_GetErrorCode(parser))) 
			<< L" at line "
			<< XML_GetCurrentLineNumber(parser);
		THROWEXCEPTION(wss.str().c_str());
		//return false;
	}

	return true;
}
