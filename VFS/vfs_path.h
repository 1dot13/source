#ifndef _VFS_PATH_H_
#define _VFS_PATH_H_

#include "vfs_config.h"
#include "vfs_types.h"
#include "utf8string.h"

namespace vfs
{
	class VFS_API Path
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
		Path();
		Path(const char* sPath);
		Path(std::string const& sPath);
		Path(const wchar_t* sPath);
		Path(utf8string const& sPath);

		const utf8string::char_t*	c_str() const;
		const utf8string::str_t&	c_wcs() const;
		const utf8string::str_t&	operator()() const;	

		std::string			to_string() const;

		Path&				operator+=(Path const& p);
		Path&				operator+=(utf8string const& p);

		bool				empty() const;

		utf8string::size_t	length() const;

		void				doCheck();

		void				splitLast(vfs::Path &rsHead, vfs::Path &rsLast) const;
		void				splitFirst(vfs::Path &rsFirst, vfs::Path &rsTail) const;

		bool				extension(utf8string &sExt) const;

		bool				operator==(vfs::Path const& p2);
	private:
		utf8string::str_t	_path;
		struct SeparatorPosition
		{
			SeparatorPosition() : first(vfs::npos), last(vfs::npos) {}
			vfs::size_t		first, last;
		} _sep;
	};
}

template<>
BuildString& BuildString::add<vfs::Path>(vfs::Path const& value);


// add only valid Path objects
vfs::Path	operator+(vfs::Path const& p1, vfs::Path const& p2);

// compare path to string (that can be an invalid path)
bool		operator==(vfs::Path const& p1, vfs::Path const& p2);
// use with care as these string can be different from the internal representation although they seem to be equal
bool		operator==(vfs::Path const& p1, utf8string const& p2);
bool		operator==(vfs::Path const& p1, utf8string::str_t const& p2);
bool		operator==(vfs::Path const& p1, const utf8string::char_t* p2);

#endif // _VFS_PATH_H_

