#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include "expat.h"
#include "XML.h"

class IXMLParser
{
public:
	const XML_Char* ElementName;
public:
	IXMLParser(const XML_Char *element_name, XML_Parser &parser, IXMLParser* caller=NULL) 
		: _parser(parser), _caller(caller), ElementName(element_name) {};
	~IXMLParser() {};

	/**
	 *  Interface
	 */
	static void OnStartCallback(void *userData, const XML_Char* name, const XML_Char** atts)
	{
		((IXMLParser*)userData)->OnStartElement(name,atts);
	}
	static void OnEndCallback(void *userData, const XML_Char* name)
	{
		((IXMLParser*)userData)->OnEndElement(name);
		// test if we are done here
		if( ((IXMLParser*)userData)->_caller && (strcmp(name, ((IXMLParser*)userData)->ElementName) == 0) )
		{
			((IXMLParser*)userData)->_caller->GrabParser();
		}
	}
	static void OnTextCallback(void *userData, const XML_Char *str, int len)
	{
		((IXMLParser*)userData)->OnTextElement(str,len);
	}
	void GrabParser()
	{
		XML_SetUserData(_parser,this);
		XML_SetElementHandler(_parser, IXMLParser::OnStartCallback, IXMLParser::OnEndCallback);
		XML_SetCharacterDataHandler(_parser, IXMLParser::OnTextCallback);
	}

	/**
	 *  Override these methods in your parser
	 */
	virtual void OnStartElement(const XML_Char* name, const XML_Char** atts)
	{};
	virtual void OnEndElement(const XML_Char* name)
	{};
	virtual void OnTextElement(const XML_Char *str, int len)
	{};
protected:
	XML_Char const* GetAttribute(const XML_Char* attr_name, const XML_Char** atts)
	{
		while(*atts)
		{
			if(strcmp(*atts++,attr_name) == 0)
			{
				return *atts;
			}
			atts++;
		}
		return "";
	}
	bool GetAttributeAsInt(const XML_Char* attr_name, const XML_Char** attr, int &value)
	{
		XML_Char const* result = GetAttribute(attr_name,attr);
		if( strcmp(result,"") != 0)
		{
			value = (int)atol(result);
			return true;
		}
		return false;
	}
	bool GetAttributeAsFloat(const XML_Char* attr_name, const XML_Char** attr, float &value)
	{
		XML_Char const* result = GetAttribute(attr_name,attr);
		if( strcmp(result,"") != 0)
		{
			value = (float)atof(result);
			return true;
		}
		return false;
	}

	int GetCurrentLineNumber()
	{
		return XML_GetCurrentLineNumber(this->_parser);
	}
protected:
	std::string		sCharData;

	XML_Parser&		_parser;
	IXMLParser*		_caller;
};

#endif // _XML_PARSER_H_
