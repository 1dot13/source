#ifndef _XMLWRITER_H_
#define _XMLWRITER_H_

#include <vfs/Core/Interface/vfs_file_interface.h>
#include <vfs/Core/vfs_string.h>

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
	void addAttributeToNextValue(vfs::String const& attribute, ValueType const& value)
	{
		std::stringstream temp_buffer;
		temp_buffer << value;
		m_stNextValAttributes.push_back( attribute_type(attribute.utf8(),temp_buffer.str()) );
	}

	template<typename ValueType>
	void addValue(vfs::String const& key, ValueType const& value)
	{
		std::string utf8key = key.utf8();
		m_ssBuffer << indent() <<  "<" << utf8key;
		insertAttributesIntoBuffer();
		m_ssBuffer << ">" << value << "</" << utf8key << ">\n";
	}

	template<>
	void addValue<std::string>(vfs::String const& key, std::string const& value)
	{
		std::string utf8key = key.utf8();
		m_ssBuffer << indent() <<  "<" << utf8key;
		insertAttributesIntoBuffer();
		m_ssBuffer << ">" << handleSpecialCharacters(value) << "</" << utf8key << ">\n";
	}

	void		addValue(vfs::String const& key);
	void		addComment(vfs::String const& comment);
	void		addFlag(vfs::UInt32 const& flags, vfs::UInt32 const& flag, vfs::String strFlag);

	void		openNode(vfs::String const& key);
	bool		closeNode();
	
	bool		writeToFile(vfs::Path const& sFileName);
	bool		writeToFile(vfs::tWritableFile* pFile);

private:
	std::string	indent();
	std::string handleSpecialCharacters(std::string const& str);
	void		insertAttributesIntoBuffer();
private:
	std::stringstream				m_ssBuffer;
	std::stack<std::string>			m_stOpenNodes;
	std::vector<attribute_type>		m_stNextValAttributes;
	int								m_iIndentLevel;
};


void testMXLWriter();

#endif // _XMLWRITER_H_

