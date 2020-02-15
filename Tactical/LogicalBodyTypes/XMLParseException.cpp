#include "XMLParseException.h"

XMLParseException::XMLParseException(const char* text, const char* function, int line, const char* file) : std::exception(), message(text), _LINE(line) {
}

XMLParseException::XMLParseException(const char* text, const XML_Char* name, XML_Parser* parser) : std::exception(), message(text), _LINE(0) {
	message = text;
	if (parser != NULL) {
		_LINE = XML_GetCurrentLineNumber(*parser);
	}
}

XMLParseException::~XMLParseException(void) {
}

const char* XMLParseException::what() const throw() {
	return message.c_str();
}
