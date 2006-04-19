//
// Snap: Implementation of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#include "readdir.h"

TReadDir::TReadDir(char const* searchPattern) 
{
	fSearchHandle = FindFirstFile(searchPattern, &fFileInfo);
	fFirstRequest = true;
}


bool TReadDir::NextFile(char const* &fileName, unsigned &attrib)
{
	if (fSearchHandle == INVALID_HANDLE_VALUE) return false;
	if (fFirstRequest) fFirstRequest = false;
	else if ( !FindNextFile(fSearchHandle, &fFileInfo) ) return false;
	fileName = fFileInfo.cFileName;
	attrib = fFileInfo.dwFileAttributes;
	return true;
}
