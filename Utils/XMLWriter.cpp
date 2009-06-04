#include "XMLWriter.h"

#include "VFS/vfs_file_raii.h"
#include "VFS/File/vfs_file.h"

void XMLWriter::AddValue(utf8string const& key)
{
	m_ssBuffer << Indent() <<  "<" << key.utf8();
	InsertAttributesIntoBuffer();
	m_ssBuffer << " />\n";
}

void XMLWriter::AddComment(utf8string const& comment)
{
	m_ssBuffer << Indent() << "<!-- " << comment.utf8() << " -->\n"; 
}

void XMLWriter::OpenNode(utf8string const& key)
{
	std::string utf8key = key.utf8();
	m_ssBuffer << Indent() << "<" << utf8key;
	InsertAttributesIntoBuffer();
	m_ssBuffer << ">\n";
	m_iIndentLevel += 1;
	m_stOpenNodes.push(utf8key);
}

bool XMLWriter::CloseNode()
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
	m_ssBuffer << Indent() << "</" << m_stOpenNodes.top() << ">\n";
	m_stOpenNodes.pop();
	return true;
}

bool XMLWriter::WriteToFile(vfs::Path const& sFileName)
{
	try
	{
		vfs::COpenWriteFile file(sFileName,true,true);
		return WriteToFile( &file.file() );
	}
	catch(CBasicException& ex)
	{
		vfs::CFile file(sFileName);
		if(file.OpenWrite(true,true))
		{
			return WriteToFile(vfs::tWriteableFile::Cast(&file));
		}
	}
	return false;
}

bool XMLWriter::WriteToFile(vfs::tWriteableFile* pFile)
{
	vfs::COpenWriteFile file(pFile);
	vfs::UInt32 written;
	std::string &str = m_ssBuffer.str();
	if(!pFile->Write(str.c_str(), str.length() * sizeof(std::string::value_type), written))
	{
		return false;
	}
	return true;
}

std::string XMLWriter::Indent()
{
	std::string indent_string;
	for(int i=0; i < m_iIndentLevel; ++i)
	{
		indent_string += "\t";
	}
	return indent_string;
}

void XMLWriter::InsertAttributesIntoBuffer()
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

std::string XMLWriter::HandleSpecialCharacters(std::string const& str)
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
	//xmlw.OpenNode(L"root");
	xmlw.OpenNode("root");
	xmlw.AddAttributeToNextValue("attr1",10);
	xmlw.AddAttributeToNextValue("attr2","string");
	//xmlw.AddValue(L"val1",10);
	xmlw.AddValue("val1",10);

	xmlw.AddAttributeToNextValue("node_attr",17);
	xmlw.AddComment("bbb -->\n <a> comment</a> <!-- aaa");
	xmlw.OpenNode("test");
	//xmlw.AddValue(L"val2",10.5);
	xmlw.AddValue("val2",10.5);
	xmlw.CloseNode();
	xmlw.CloseNode();
	//xmlw.WriteToFile(WideString("xml_output/test.xml")());
	xmlw.WriteToFile("xml_output/test.xml");
}
