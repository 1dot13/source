#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

#include "expat.h"
//#include "XML.h"
#include <string>

template<typename OperandType>
class LazyCondition
{
public:
	typedef bool (*cond_fct)(OperandType op1, OperandType op2);
	LazyCondition(OperandType op1, OperandType op2, cond_fct fct)
		: _op1(op1), _op2(op2), _fct(fct)
	{};
	bool check()
	{
		return _fct(_op1,_op2);
	}
private:
	OperandType	_op1;
	OperandType	_op2;
	cond_fct	_fct;
};

inline bool lazyStrEqual(const char* str1, const char* str2)
{
	return strcmp(str1,str2) == 0;
}

class LazyStrEqual : public LazyCondition<const char*>
{
public:
	LazyStrEqual(const char* str1, const char* str2)
		: LazyCondition<const char*>(str1, str2, lazyStrEqual)
	{};
};

class LazyTrue : public LazyCondition<bool>
{
	static bool True(bool a, bool b){return true;};
public:
	LazyTrue() : LazyCondition<bool>(true,true,LazyTrue::True) {};
};


class IXMLParser
{
public:
	const XML_Char* ElementName;
public:
	IXMLParser(const XML_Char *element_name, XML_Parser* parser, IXMLParser* caller=NULL) 
		: _caller(caller), ElementName(element_name)
	{
		if(parser)
		{
			this->setParser(parser);
		}
	};
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
		XML_SetUserData(*_parser,this);
		XML_SetElementHandler(*_parser, IXMLParser::onStartCallback, IXMLParser::onEndCallback);
		XML_SetCharacterDataHandler(*_parser, IXMLParser::onTextCallback);
	}

	/**
	 *  Override these methods in your parser
	 */
	virtual void onStartElement(const XML_Char* name, const XML_Char** atts)
	{};
	virtual void onEndElement(const XML_Char* name)
	{};
	virtual void onTextElement(const XML_Char *str, int len)
	{
	};

	class Attributes
	{
	public:
		Attributes(const XML_Char** atts) : _atts(atts) {};
		XML_Char const* get(const XML_Char* attr_name) const
		{
			const XML_Char** atts = _atts;
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
		bool getLong(const XML_Char* attr_name, long& attr) const
		{
			XML_Char const* result = this->get(attr_name);
			if( strcmp(result,"") != 0)
			{
				attr = (int)atol(result);
				return true;
			}
			return false;
		}
		bool getDouble(const XML_Char* attr_name, double &attr) const
		{
			XML_Char const* result = this->get(attr_name);
			if( strcmp(result,"") != 0)
			{
				attr = atof(result);
				return true;
			}
			return false;
		}
	private:
		const XML_Char** _atts;
	};

	template<typename StateType>
	class ParserState
	{
	public:
		ParserState(const StateType start_state)
			: _my_state(start_state)
		{};

		StateType const& state()
		{
			return _my_state;
		}

		template<typename ConditionOperandType>
		bool stateTransition(const StateType current_state, LazyCondition<ConditionOperandType> condition, const StateType next_state)
		{
			if(_my_state == current_state && condition.check() )
			{
				_my_state = next_state;
				return true;
			}
			return false;
		}
	private:
		StateType _my_state;
	};

	XML_Parser& getParser()
	{
		return *_parser;
	}
protected:
	void setParser(XML_Parser* parser)
	{
		_parser = parser;
		// grabParser(); ???
	}

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
		return XML_GetCurrentLineNumber(*this->_parser);
	}
protected:
	std::string		sCharData;

	XML_Parser*		_parser;
	IXMLParser*		_caller;
};

#endif // _XML_PARSER_H_
