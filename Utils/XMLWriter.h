#ifndef _XMLWRITER_H_
#define _XMLWRITER_H_

#include "FileMan.h"
#include "VFS/Interface/vfs_file_interface.h"
#include "utf8string.h"

#include <stack>
#include <string>
#include <sstream>
#include <vector>

class XMLWriter
{
public:
	typedef std::pair<std::string, std::string> attribute_type;
public:
	XMLWriter() : m_iIndentLevel(0)  
	{
		m_ssBuffer << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n" ;
	};
	~XMLWriter()
	{};
	
	template<typename ValueType>
	void AddAttributeToNextValue(utf8string const& attribute, ValueType const& value)
	{
		std::stringstream temp_buffer;
		temp_buffer << value;
		m_stNextValAttributes.push_back( attribute_type(attribute.utf8(),temp_buffer.str()) );
	}

	template<typename ValueType>
	void AddValue(utf8string const& key, ValueType const& value)
	{
		std::string utf8key = key.utf8();
		m_ssBuffer << Indent() <<  "<" << utf8key;
		InsertAttributesIntoBuffer();
		m_ssBuffer << "> " << value << " </" << utf8key << ">\n";
	}

	template<>
	void AddValue<std::string >(utf8string const& key, std::string const& value)
	{
		std::string utf8key = key.utf8();
		m_ssBuffer << Indent() <<  "<" << utf8key;
		InsertAttributesIntoBuffer();
		m_ssBuffer << "> " << HandleSpecialCharacters(value) << " </" << utf8key << ">\n";
	}

	void		AddValue(utf8string const& key);
	void		AddComment(utf8string const& comment);

	void		OpenNode(utf8string const& key);
	bool		CloseNode();
	
	bool		WriteToFile(vfs::Path const& sFileName);
	bool		WriteToFile(vfs::tWriteableFile* pFile);

private:
	std::string	Indent();
	std::string HandleSpecialCharacters(std::string const& str);
	void		InsertAttributesIntoBuffer();
private:
	std::stringstream				m_ssBuffer;
	std::stack<std::string>			m_stOpenNodes;
	std::vector<attribute_type>		m_stNextValAttributes;
	int								m_iIndentLevel;
};


void testMXLWriter();

#endif // _XMLWRITER_H_

