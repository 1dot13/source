#include "EnumeratorDB.h"

namespace LogicalBodyTypes {

EnumeratorDB::EnumeratorDB() {
}

EnumeratorDB::~EnumeratorDB(void) {
}

void EnumeratorDB::AddEnum(std::string enumName, int num, char* strEnumerators, ...) {
	int strLen = strlen(strEnumerators);
	char* str = new char[strLen + 1];
	strcpy(str, strEnumerators);

	// for detecting synchronization issue between the actual enum declaration and the DB insertion
	// assert that the number of provided enumerators matches the expected number
	int numSemiColons = 0;
	for (int i = 0; i < strLen; i++) if (strEnumerators[i] == ',') numSemiColons++;
	// TODO: remove output to cout Log this event somehow not just do the assertion!
	if (numSemiColons + 1 != num) {
		std::cout << enumName << " expected " << num << " got " << numSemiColons + 1 << std::endl;
	}
	assert(numSemiColons + 1 == num);

	// if enumeration already in DB, delete
	EnumerationMap::iterator i = enumerations.find(enumName);
	if (i != enumerations.end()) enumerations.erase(enumName);

	char* token = strtok(str, ", ");
	va_list vl;
	va_start(vl, strEnumerators);
	for (int i = 0; i < num; i++) {
		enumerations[enumName][token] = va_arg(vl, int);
		token = strtok(NULL, ", ");
	}

	va_end(vl);
	delete str;
}

bool EnumeratorDB::GetEnumeratorStr(const char* enumeration, int enumerator, std::string& str) {
	EnumerationMap::iterator i = enumerations.find(enumeration);
	if (i != enumerations.end()) {
		for (EnumeratorMap::iterator j = i->second.begin(); j != i->second.end(); j++) {
			if (j->second == enumerator) {
				str = j->first;
				return true;
			}
		}
	}
	return false;
}

bool EnumeratorDB::GetEnumeratorByStr(const char* enumeration, const char* enumerationStr, int& enumerator) {
	EnumerationMap::iterator i = enumerations.find(enumeration);
	if (i == enumerations.end()) return false;
	EnumeratorMap::iterator j = i->second.find(enumerationStr);
	if (j == i->second.end()) return false;
	enumerator = j->second;
	return true;
}

}
