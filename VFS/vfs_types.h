#ifndef _VFS_TYPES_H_
#define _VFS_TYPES_H_

#include <iostream>
#include "utf8string.h"
#include <string>
#include <vector>

namespace vfs
{
	typedef unsigned long	UInt64;
	typedef unsigned int	UInt32;
	typedef unsigned short	UInt16;
	typedef unsigned char	UInt8;
	typedef unsigned char	UByte;

	typedef long			Int64;
	typedef int				Int32;
	typedef short			Int16;
	typedef char			Int8;
	typedef char			Byte;
}

namespace vfs
{
	namespace Const
	{
		inline const utf8string::str_t		EMPTY()				{ return L""; };
		inline const utf8string::char_t		EMPTY_CHAR()		{ return L''; };

		inline const utf8string::str_t		DOT()				{ return L"."; };
		inline const utf8string::char_t		DOT_CHAR()			{ return L'.'; };

		inline const utf8string::str_t		DOTDOT()			{ return L".."; };
		inline const utf8string::str_t		DOTSVN()			{ return L".svn"; };

		inline const utf8string::str_t		STAR()				{ return L"*"; };
		inline const utf8string::str_t		DSTAR()				{ return L"**"; };

#ifdef WIN32
		inline const utf8string::str_t		SEPARATOR()			{ return L"\\"; };
		inline const utf8string::char_t		SEPARATOR_CHAR()	{ return L'\\'; };
#else
		inline const utf8string::str_t		SEPARATOR()			{ return L"/"; };
		inline const utf8string::char_type	SEPARATOR_CHAR()	{ return L'/'; };
#endif
	}
}



namespace vfs
{
	// remove leading and trailing white characters;
	template<typename StringType>
	StringType TrimString(StringType const& sStr, Int32 iMinPos, Int32 iMaxPos);

	template<>
	std::string TrimString<std::string>(std::string const& sStr, Int32 iMinPos, Int32 iMaxPos);

	template<>
	std::wstring TrimString<std::wstring>(std::wstring const& sStr, Int32 iMinPos, Int32 iMaxPos);

	template<>
	utf8string TrimString<utf8string>(utf8string const& sStr, Int32 iMinPos, Int32 iMaxPos);
}

class PathAccess;
namespace vfs
{
	class Path
	{
		friend class PathAccess;
	public:
		class Less{
		public:
			bool operator()(vfs::Path const& s1, vfs::Path const& s2) const;
		};
		class Equal{
		public:
			bool operator()(vfs::Path const& s1, vfs::Path const& s2) const;
		};
	public:
		Path() : _first(-1), _last(-1) {};
		Path(const char* sPath);
		Path(const wchar_t* sPath);
		Path(utf8string const& sPath);

		const utf8string&		operator()() const;	
		Path&					operator+=(Path const& p);

		bool					empty() const;

		utf8string::size_t		length() const;

		void					DoCheck();

		bool					SplitLast(utf8string& rsHead, utf8string& rsLast) const;
		bool					SplitLast(Path &rsHead, Path &rsLast) const;

		bool					SplitFirst(utf8string& rsFirst, utf8string& rsTail) const;
		bool					SplitFirst(Path &rsFirst, Path &rsTail) const;

		bool					Extension(utf8string &sExt) const;

		bool					operator==(vfs::Path const& p2);
	private:
		utf8string	_path;
		vfs::Int32	_first,_last;
	};
}

// add only valid Path objects
vfs::Path	operator+(vfs::Path const& p1, vfs::Path const& p2);

// compare path to string (that can be an invalid path)
bool		operator==(vfs::Path const& p1, vfs::Path const& p2);
// use with care as these string can be different from the internal representation although they seem to be equal
bool		operator==(vfs::Path const& p1, utf8string const& p2);
bool		operator==(vfs::Path const& p1, utf8string::str_t const& p2);
bool		operator==(vfs::Path const& p1, const utf8string::char_t* p2);

/*************************************************************************/

bool MatchPattern(utf8string const& sPattern, utf8string const& sStr);
bool MatchPattern(utf8string const& sPattern, utf8string::str_t const& sStr);

/*************************************************************************/

class BasicAllocator
{
public:
	virtual ~BasicAllocator() {};
};

template<typename T>
class ObjBlockAllocator : public BasicAllocator
{
public:
	ObjBlockAllocator(unsigned int blockSize=1024)
		: BasicAllocator(), BLOCK_SIZE(blockSize), _ObjNew(0) {};
	const unsigned int BLOCK_SIZE;
	///
	T* New(unsigned int *ID = NULL)
	{
		unsigned int block_id = _ObjNew/BLOCK_SIZE;
		unsigned int file_id = _ObjNew % BLOCK_SIZE;
		if(block_id >= _ObjPool.size())
		{
			tBlock* b = new tBlock();
			b->resize(BLOCK_SIZE);
			_ObjPool.push_back(b);
		}
		tBlock* block = _ObjPool[block_id];
		T* obj = &(*block)[file_id];
		if(ID)
		{
			*ID = _ObjNew;
		}
		_ObjNew++;
		return obj;
	}
	///
	virtual ~ObjBlockAllocator()
	{
		for(unsigned int i = 0; i < _ObjPool.size(); ++i)
		{
			delete _ObjPool[i];
		}
	}
private:
	typedef std::vector<T>	tBlock;
	std::vector<tBlock*>	_ObjPool;
	unsigned int			_ObjNew;
};

#endif // _VFS_TYPES_H_
