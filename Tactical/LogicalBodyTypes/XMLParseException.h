#pragma once
#ifndef _LBT_XMLPARSEEXCEPTION__H_
#define _LBT_XMLPARSEEXCEPTION__H_

#include "LogicalBodyTypes/XMLParseException.h"
#include "expat.h"
#include <exception>
#include <string>

class XMLParseException : public std::exception {
public:
	XMLParseException(const char* text, const char* function, int line, const char* file);
	XMLParseException(const char* text, const XML_Char* name, XML_Parser* parser = NULL);
	const char* what() const throw();
	~XMLParseException(void);

public:
	int _LINE;
	std::string message;
	std::string _FILE;
	std::string _FUNCTION;
	std::string _time;

};

#endif
