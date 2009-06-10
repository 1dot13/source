#ifndef _PARSER_TOOLS_H_
#define _PARSER_TOOLS_H_

#include "../vfs_types.h"
#include "../Interface/vfs_file_interface.h"

#include <list>

/**************************************************************/
/**************************************************************/
const vfs::UInt32 BUFFER_SIZE = 1024;

class CReadLine
{
public:
	CReadLine(vfs::tReadableFile& rFile);

	bool FillBuffer();
	bool FromBuffer(std::string& line);
	bool GetLine(std::string& line);
private:
	vfs::Byte			_buffer[BUFFER_SIZE+1];
	vfs::tReadableFile& _file;
	vfs::UInt32			_buffer_pos;
	vfs::UInt32			_buffer_last;
	bool				_eof;
};

/**************************************************************/
/**************************************************************/

class CSplitStringList
{
public:
	CSplitStringList(utf8string const& sList);
	~CSplitStringList();

	bool NextListEntry(utf8string &sEntry);
private:
	const utf8string	m_sList;
	int					iCurrent,iNext;
};


/**************************************************************/
/**************************************************************/

class CTransferRules
{
public:
	enum EAction
	{
		DENY,
		ACCEPT,
	};
public:
	CTransferRules();

	bool	InitFromTxtFile(vfs::Path const& sPath);
	bool	InitFromTxtFile(vfs::tReadableFile* pFile);

	void	SetDefaultAction(EAction act);
	EAction	GetDefaultAction();

	EAction	ApplyRule(utf8string const& sStr);
private:
	struct SRule
	{
		EAction		action;
		vfs::Path	pattern;
	};
	typedef std::list<SRule> tPatternList;
	tPatternList	m_listRules;
	EAction			m_eDefaultAction;
};

/**************************************************************/
/**************************************************************/

#define USE_CODE_PAGE

class CodePageReader
{
public:
	enum EMode
	{
		Error, Normal, Shift, Ctrl, Alt
	};
	void ReadCodePage(vfs::Path const& codepagefile);
private:
	CodePageReader::EMode ExtractMode(std::string const &readStr, size_t startPos);
	bool ExtractCode(std::string const& str, int iStart, vfs::UInt32& rInsertPoint, utf8string::str_t& u8s);
};

namespace charSet
{
	enum ECharSet
	{
		CS_SPACE			= 1,
		CS_ALPHA_LC			= 2,
		CS_ALPHA_UC			= 4,
		CS_ALPHA			= CS_ALPHA_LC + CS_ALPHA_UC,
		CS_NUMERIC			= 8,
		CS_ALPHA_NUM		= CS_ALPHA + CS_NUMERIC,
		CS_SPECIAL_ALPHA_LC	= 16,
		CS_SPECIAL_ALPHA_UC	= 32,
		CS_SPECIAL_ALPHA	= CS_SPECIAL_ALPHA_LC + CS_SPECIAL_ALPHA_UC,
		CS_SPECIAL_NON_CHAR	= 65,
	};

	bool IsFromSet(char wc,			unsigned int char_set);
	bool IsFromSet(int wc,			unsigned int char_set);
	bool IsFromSet(unsigned int wc,	unsigned int char_set);
	bool IsFromSet(wchar_t wc,		unsigned int char_set);

	void AddToCharSet(ECharSet eset, std::wstring cset);

	void InitializeCharSets();
};

#endif // _PARSER_TOOLS_H_

