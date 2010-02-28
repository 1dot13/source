#ifndef _VFS_TYPES_H_
#define _VFS_TYPES_H_

#include <iostream>
#include "utf8string.h"
#include <string>
#include <vector>

namespace vfs
{
#ifdef WIN32
	typedef unsigned __int64	UInt64;
	typedef unsigned __int32	UInt32;
	typedef unsigned __int16	UInt16;
	typedef unsigned __int8		UInt8;
	typedef unsigned __int8		UByte;

	typedef __int64				Int64;
	typedef __int32				Int32;
	typedef __int16				Int16;
	typedef __int8				Int8;
	typedef __int8				Byte;
#elif __linux__
	typedef uint64_t			UInt64;
	typedef uint32_t  			UInt32;
	typedef uint16_t			UInt16;
	typedef uint8_t				UInt8;
	typedef uint8_t				UByte;

	typedef int64_t				Int64;
	typedef int32_t				Int32;
	typedef int16_t				Int16;
	typedef int8_t				Int8;
	typedef char				Byte;
#endif

	typedef ::size_t			size_t;
	typedef ::off_t				offset_t;

	extern const vfs::size_t	npos;
}

namespace vfs
{
	namespace Const
	{
		inline const utf8string::str_t		EMPTY()				{ return L""; };
		//inline const utf8string::char_t	EMPTY_CHAR()		{ return L''; };

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
		inline const utf8string::char_t		SEPARATOR_CHAR()	{ return L'/'; };
#endif
	}
}



namespace vfs
{
	// remove leading and trailing white characters;
	template<typename StringType>
	StringType trimString(StringType const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	template<>
	std::string trimString<std::string>(std::string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	template<>
	std::wstring trimString<std::wstring>(std::wstring const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);

	template<>
	utf8string trimString<utf8string>(utf8string const& sStr, vfs::size_t iMinPos, vfs::size_t iMaxPos);
}

class VFS_API BuildString
{
public:
	template<typename T>
	BuildString&				add(T const& value)
	{
		_strstr << value;
		return *this;
	}

	utf8string::str_t			get()
	{
		return _strstr.str();
	}
private:
	std::basic_stringstream<utf8string::char_t> _strstr;
};

template<>
BuildString& BuildString::add<utf8string>(utf8string const& value);

/*************************************************************************/

bool matchPattern(utf8string const& sPattern, utf8string const& sStr);
bool matchPattern(utf8string const& sPattern, utf8string::str_t const& sStr);

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
	void operator=(ObjBlockAllocator<T> const& t);

	typedef std::vector<T>	tBlock;
	std::vector<tBlock*>	_ObjPool;
	unsigned int			_ObjNew;
};

#endif // _VFS_TYPES_H_
