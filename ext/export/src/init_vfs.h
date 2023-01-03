#ifndef _INIT_VFS_H_
#define _INIT_VFS_H_

#include <vfs/Core/vfs_path.h>
#include <exporter_base.h>

namespace ja2xp
{
	class InitVFS
	{
	public:
		static InitVFS& instance();

		void checkParameters(IExporterBase::param_list_t const& params);
		bool init();

		vfs::Path const& srcPattern();
		vfs::Path const& dstPattern();
	private:
		InitVFS();

		bool has_src, has_dst;
		bool src_lib, dst_create;
		vfs::Path src_path, dst_path;
		vfs::Path src_pattern, dst_pattern;
	};
}

//bool InitVFS(vfs::Path const& sSrcPath, vfs::Path const& sDstPath,
//			 vfs::Path& src_pattern, vfs::Path& dst_pattern,
//			 bool bSourceAsLib, bool bCreateDstDirectory);

#endif // _INIT_VFS_H_
