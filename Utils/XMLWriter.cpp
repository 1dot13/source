#include "XMLWriter.h"

#include "VFS/vfs_file_raii.h"
#include "VFS/File/vfs_file.h"

void XMLWriter::addValue(utf8string const& key)
{
	m_ssBuffer << indent() <<  "<" << key.utf8();
	insertAttributesIntoBuffer();
	m_ssBuffer << " />\n";
}

void XMLWriter::addComment(utf8string const& comment)
{
	m_ssBuffer << indent() << "<!-- " << comment.utf8() << " -->\n"; 
}

void XMLWriter::openNode(utf8string const& key)
{
	std::string utf8key = key.utf8();
	m_ssBuffer << indent() << "<" << utf8key;
	insertAttributesIntoBuffer();
	m_ssBuffer << ">\n";
	m_iIndentLevel += 1;
	m_stOpenNodes.push(utf8key);
}

bool XMLWriter::closeNode()
{
	if(m_iIndentLevel < 1)
	{
		// there is nothing to close
		return false;
	}
	if(m_stOpenNodes.empty())
	{
		return false;
	}
	m_iIndentLevel -= 1;
	m_ssBuffer << indent() << "</" << m_stOpenNodes.top() << ">\n";
	m_stOpenNodes.pop();
	return true;
}

bool XMLWriter::writeToFile(vfs::Path const& sFileName)
{
	try
	{
		vfs::COpenWriteFile file(sFileName,true,true);
		return writeToFile( &file.file() );
	}
	catch(CBasicException& ex)
	{
		logException(ex);
		vfs::CFile file(sFileName);
		if(file.openWrite(true,true))
		{
			return writeToFile(vfs::tWritableFile::cast(&file));
		}
	}
	return false;
}

bool XMLWriter::writeToFile(vfs::tWritableFile* pFile)
{
	try
	{
		vfs::COpenWriteFile file(pFile);
		std::string &str = m_ssBuffer.str();
		pFile->write(str.c_str(), str.length() * sizeof(std::string::value_type));
		return true;
	}
	catch(CBasicException& ex)
	{
		logException(ex);
		return false;
	}
}

std::string XMLWriter::indent()
{
	std::string indent_string;
	for(int i=0; i < m_iIndentLevel; ++i)
	{
		indent_string += "\t";
	}
	return indent_string;
}

void XMLWriter::insertAttributesIntoBuffer()
{
	if(!m_stNextValAttributes.empty())
	{
		std::vector<attribute_type>::iterator it = m_stNextValAttributes.begin();
		for(; it != m_stNextValAttributes.end(); ++it)
		{
			m_ssBuffer << " " << it->first << "=\"" << it->second << "\"";
		}
	}
	m_stNextValAttributes.clear();
}

std::string XMLWriter::handleSpecialCharacters(std::string const& str)
{
	std::stringstream ss;
	unsigned int current = 0, old = 0;
	while( current < str.length() )
	{
		char const& c = str.at(current);
		if( c == '&' )
		{
			ss << str.substr(old, current-old) << "&amp;";
			old = current + 1;
		}
		else if( c == '<' )
		{
			ss << str.substr(old, current-old) << "&lt;";
			old = current + 1;
		}
		else if( c == '>' )
		{
			ss << str.substr(old, current-old) << "&gt;";
			old = current + 1;
		}
		else if( c == '\"' )
		{
			ss << str.substr(old, current-old) << "&quot;";
			old = current + 1;
		}
		else if( c == '\'' )
		{
			ss << str.substr(old, current-old) << "&apos;";
			old = current + 1;
		}
		current++;
	}
	if(old)
	{
		ss << str.substr(old, current-old);
		return ss.str();
	}
	return str;
}

void testMXLWriter()
{
	//XMLWriter<char> xmlw;
	XMLWriter xmlw;
	//xmlw.openNode(L"root");
	xmlw.openNode("root");
	xmlw.addAttributeToNextValue("attr1",10);
	xmlw.addAttributeToNextValue("attr2","string");
	//xmlw.AddValue(L"val1",10);
	xmlw.addValue("val1",10);

	xmlw.addAttributeToNextValue("node_attr",17);
	xmlw.addComment("bbb -->\n <a> comment</a> <!-- aaa");
	xmlw.openNode("test");
	//xmlw.addValue(L"val2",10.5);
	xmlw.addValue("val2",10.5);
	xmlw.closeNode();
	xmlw.closeNode();
	//xmlw.WriteToFile(WideString("xml_output/test.xml")());
	xmlw.writeToFile("xml_output/test.xml");
}
