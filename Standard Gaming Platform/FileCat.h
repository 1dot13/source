//
// Snap: Declaration of the TFileCat class
//
// This class catalogues files in a directory and all its subdirectories
//
#ifndef FILECAT_H
#define FILECAT_H

#include "stringicmp.h"
#include <string>
#include <set>


class TFileCat {
public:

	TFileCat(std::string root) { NewCat(root); }
	TFileCat() {}

	// Build a new file catalogue by recursively traversing the root directory
	void NewCat(std::string root);

	std::string GetRootDir() const { return fRootDir; }

	// Look for a given file in the catalogue
	// Unless pathIncludesRoot == true, will prepend the root directory to path
	bool FindFile(std::string path, bool pathIncludesRoot = false) const;

	// Delete a given file from the catalogue
	// Unless pathIncludesRoot == true, will prepend the root directory to path
	size_t RemoveFile(std::string path, bool pathIncludesRoot = false);

	// Delete all files from a given directory in the catalogue
	// Unless pathIncludesRoot == true, will prepend the root directory to path
	size_t RemoveDir(std::string dir, bool pathIncludesRoot = false);

private:

	typedef std::set<std::string, TStringiLess> TCatalogue;

	std::string fRootDir;
	TCatalogue fFileCat;

	// Recursively traverse a directory, adding regular files to the catalogue
	void TraverseDir(std::string dir, int depth = -1);
};

#endif // FILECAT_H
