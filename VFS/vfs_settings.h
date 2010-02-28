#ifndef _VFS_SETTIGNS_H_
#define _VFS_SETTIGNS_H_

namespace vfs
{
	class Settings
	{
	public:
		static void setUseUnicode(bool useUnicode);
		static bool getUseUnicode();
	};
}

#endif // _VFS_SETTIGNS_H_
