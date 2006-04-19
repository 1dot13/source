//
// Snap: Declaration of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#ifndef READDIR_H
#define READDIR_H

#include <windows.h>

class TReadDir {
public:

	TReadDir(char const* searchPattern);

	~TReadDir() { FindClose(fSearchHandle); }

	bool NextFile(char const* &fileName, unsigned &attrib);

private:

	HANDLE fSearchHandle;
	WIN32_FIND_DATA fFileInfo;
	bool fFirstRequest;
};


#endif // #ifndef READDIR_H