//
// Snap: Implementation of the TFileCat class
//
#include "FileCat.h"
#include "readdir.h"


// Remove a slash or backslash (if any) from the end of a string
void ChompSlash(std::string& s)
{
	if ( s.empty() ) return;

	if ( *s.rbegin() == '\\' || *s.rbegin() == '/' ) {
		s.erase( s.length() - 1 );
	}
}


// Build a new file catalogue by recursively traversing the root directory
void TFileCat::NewCat(std::string root)
{
	fRootDir = root;
	ChompSlash(fRootDir);

	fFileCat.clear();

	TraverseDir(fRootDir);
}


// Look for a given file in the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
bool TFileCat::FindFile(std::string path, bool pathIncludesRoot) const
{
	if (pathIncludesRoot) return fFileCat.find(path) != fFileCat.end();
	else return fFileCat.find(fRootDir + '\\' + path) != fFileCat.end();
}


// Delete a given file from the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
size_t TFileCat::RemoveFile(std::string path, bool pathIncludesRoot)
{
	if (pathIncludesRoot) return fFileCat.erase(path);
	else return fFileCat.erase(fRootDir + '\\' + path);
}


// Delete all files from a given directory in the catalogue
// Unless pathIncludesRoot == true, will prepend the root directory to path
size_t TFileCat::RemoveDir(std::string dir, bool pathIncludesRoot)
{
	if ( !pathIncludesRoot ) dir = fRootDir + '\\' + dir;
	ChompSlash(dir);
	std::string dirlower = dir + '\\';
	std::string dirupper = dir + char('\\'+1);

	TCatalogue::iterator upper = fFileCat.upper_bound(dirupper);
	TCatalogue::iterator lower;

	int deleted = 0;

	while ( ( lower = fFileCat.lower_bound(dirlower) ) != upper) {
		fFileCat.erase(lower);
		deleted++;
	}

	return deleted;
}


// Recursively traverse a directory, adding regular files to the catalogue
void TFileCat::TraverseDir(std::string dir, int depth)
{
	using std::string;
	static string dot( ".");
	static string dotdot( "..");
	static string svn( ".svn");

	if (!dir.empty()) dir += '\\';

	TReadDir readDir((dir + "*").c_str());

	char const* fileName;
	unsigned attrib;

	while ( readDir.NextFile(fileName, attrib) ) {
		if (dot == fileName || dotdot == fileName || svn == fileName) continue;

		string fullPath = dir + fileName;

		if (attrib & FILE_ATTRIBUTE_DIRECTORY) {
			if (depth < 0) TraverseDir(fullPath);
			else if (depth > 0) TraverseDir(fullPath, depth-1);
		}
		else {
			fFileCat.insert(fullPath);
		}
	}
}
