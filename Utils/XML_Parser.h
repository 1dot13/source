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
	static void onStartCallback(void *userData, const XML_Char* name, const XML_Char** atts)
	{
		((IXMLParser*)userData)->onStartElement(name,atts);
	}
	static void onEndCallback(void *userData, const XML_Char* name)
	{
		((IXMLParser*)userData)->onEndElement(name);
		// test if we are done here
		if( ((IXMLParser*)userData)->_caller && (strcmp(name, ((IXMLParser*)userData)->ElementName) == 0) )
		{
			((IXMLParser*)userData)->_caller->grabParser();
		}
	}
	static void onTextCallback(void *userData, const XML_Char *str, int len)
	{
		((IXMLParser*)userData)->onTextElement(str,len);
	}
	void grabParser()
	{
		XML_SetUserData(_parser,this);
		XML_SetElementHandler(_parser, IXMLParser::onStartCallback, IXMLParser::onEndCallback);
		XML_SetCharacterDataHandler(_parser, IXMLParser::onTextCallback);
	}

	/**
	 *  Override these methods in your parser
	 */
	virtual void onStartElement(const XML_Char* name, const XML_Char** atts)
	{};
	virtual void onEndElement(const XML_Char* name)
	{};
	virtual void onTextElement(const XML_Char *str, int len)
	{};
protected:
	XML_Char const* getAttribute(const XML_Char* attr_name, const XML_Char** atts)
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
	bool getAttributeAsInt(const XML_Char* attr_name, const XML_Char** attr, int &value)
	{
		XML_Char const* result = getAttribute(attr_name,attr);
		if( strcmp(result,"") != 0)
		{
			value = (int)atol(result);
			return true;
		}
		return false;
	}
	bool getAttributeAsFloat(const XML_Char* attr_name, const XML_Char** attr, float &value)
	{
		XML_Char const* result = getAttribute(attr_name,attr);
		if( strcmp(result,"") != 0)
		{
			value = (float)atof(result);
			return true;
		}
		return false;
	}

	int getCurrentLineNumber()
	{
		return XML_GetCurrentLineNumber(this->_parser);
	}
protected:
	std::string		sCharData;

	XML_Parser&		_parser;
	IXMLParser*		_caller;
};

#endif // _XML_PARSER_H_
