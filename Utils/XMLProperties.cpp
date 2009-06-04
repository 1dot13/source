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
utf8string const& CPropertyContainer::TagMap::Container(utf8string::char_t* container)
{
	if(container)
	{
		_map[L"Container"] = container;
	}
	return _map[L"Container"];
}
utf8string const& CPropertyContainer::TagMap::Section(utf8string::char_t* section)
{
	if(section)
	{
		_map[L"Section"] = section;
	}
	return _map[L"Section"];
}
utf8string const& CPropertyContainer::TagMap::SectionID(utf8string::char_t* section_id)
{
	if(section_id)
	{
		_map[L"SectionID"] = section_id;
	}
	return _map[L"SectionID"];
}
utf8string const& CPropertyContainer::TagMap::Key(utf8string::char_t* key)
{
	if(key)
	{
		_map[L"Key"] = key;
	}
	return _map[L"Key"];
}
utf8string const& CPropertyContainer::TagMap::KeyID(utf8string::char_t* key_id)
{
	if(key_id)
	{
		_map[L"KeyID"] = key_id;
	}
	return _map[L"KeyID"];
}

bool CPropertyContainer::WriteToXMLFile(vfs::Path const& sFileName, CPropertyContainer::TagMap& tagmap)
{
	XMLWriter xmlw;

	xmlw.OpenNode(tagmap.Container());

	tSections::iterator sit = m_mapProps.begin();
	for(; sit != m_mapProps.end(); ++sit)
	{
		xmlw.AddAttributeToNextValue(tagmap.SectionID(),sit->first.utf8());
		xmlw.OpenNode(tagmap.Section());

		CPropertyContainer::CSection& section = sit->second;
		CPropertyContainer::CSection::tProps::iterator kit = section.mapProps.begin();
		for(; kit != section.mapProps.end(); ++kit)
		{
			xmlw.AddAttributeToNextValue(tagmap.KeyID(), kit->first.utf8());
			xmlw.AddValue(tagmap.Key(), kit->second.utf8());
		}

		xmlw.CloseNode();

	}

	xmlw.CloseNode();

	return xmlw.WriteToFile(sFileName);
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
	virtual void OnStartElement(const XML_Char* name, const XML_Char** atts);
	virtual void OnEndElement(const XML_Char* name);
	virtual void OnTextElement(const XML_Char *str, int len);
private:
	CPropertyContainer&				_container;
	CPropertyContainer::TagMap&		_tagmap;
	DOM_OBJECT						current_state;
	utf8string						current_section;
	utf8string						current_key;
};


void CPropertyXMLParser::OnStartElement(const XML_Char *name, const XML_Char **atts)
{
	utf8string utf8_name(name);
	if(current_state == DO_ELEMENT_NONE && StrCmp::Equal(utf8_name,_tagmap.Container()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && StrCmp::Equal(utf8_name, _tagmap.Section()))
	{
		current_state = DO_ELEMENT_Section;
		current_section = this->GetAttribute(_tagmap.SectionID().utf8().c_str(),atts);
	}
	else if(current_state == DO_ELEMENT_Section && StrCmp::Equal(utf8_name, _tagmap.Key()))
	{
		current_state = DO_ELEMENT_Key;
		current_key = this->GetAttribute(_tagmap.KeyID().utf8().c_str(),atts);
	}
	sCharData = "";
}

void CPropertyXMLParser::OnEndElement(const XML_Char* name)
{
	utf8string utf8_name(name);
	if(current_state == DO_ELEMENT_Key && StrCmp::Equal(utf8_name, _tagmap.Key()))
	{
		_container.SetStringProperty(current_section, current_key, vfs::TrimString(sCharData,0,sCharData.length()));
		current_state = DO_ELEMENT_Section;
	}
	else if(current_state == DO_ELEMENT_Section && StrCmp::Equal(utf8_name, _tagmap.Section()))
	{
		current_state = DO_ELEMENT_Container;
	}
	else if(current_state == DO_ELEMENT_Container && StrCmp::Equal(utf8_name, _tagmap.Container()))
	{
		current_state = DO_ELEMENT_NONE;
	}
}

void CPropertyXMLParser::OnTextElement(const XML_Char *str, int len)
{
	if(current_state == DO_ELEMENT_Key)
	{
		sCharData.append(str,len);
	}
}

bool CPropertyContainer::InitFromXMLFile(vfs::Path const& sFileName, CPropertyContainer::TagMap& tagmap)
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
		vfs::CFile* rfile = new vfs::CFile(sFileName);
		delete_file = true;
		file = vfs::tReadableFile::Cast(rfile);
		if(!file->OpenRead())
		{
			file->Close();
			delete file;
			return false;
		}
	}
	if(!file)
	{
		return false;
	}

	vfs::UInt32 size = file->GetFileSize();

	std::vector<vfs::Byte> buffer(size+1);

	vfs::UInt32 has_read;
	file->Read(&buffer[0],size,has_read);
	buffer[size] = 0;

	file->Close();
	if(delete_file) delete file;

	XML_Parser parser = XML_ParserCreate(NULL);

	CPropertyXMLParser pp(*this,tagmap,parser,NULL);
	pp.GrabParser();

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
