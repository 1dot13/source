#ifndef _EXPORT_STI_H_
#define _EXPORT_STI_H_

#include <vfs/Core/vfs_path.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <exporter_base.h>

namespace ja2xp
{
	class CExportSTI : public IExporterBase
	{
	public:
		typedef IExporterBase::param_list_t param_list_t;
		
		static const wchar_t* commandString;// = L"sti";

		virtual void handleCommand(param_list_t const& params);
		virtual void printHelp();
	};

	bool convertSTItoJPC(vfs::tReadableFile *pFile, vfs::tWritableFile *pOutFile, bool bOffsets, bool rgba);
	bool convertSTItoJPC(vfs::tReadableFile *pFile, vfs::Path const base_out_path, bool bOffsets, bool rgba);
	bool convertSTItoPNG(vfs::tReadableFile *pFile, vfs::tWritableFile *pOutFile, bool rgba);

	bool convertSTItoJPC(vfs::Path const& sSrc, vfs::Path const& sDst,
			bool bPacked=true, bool bOffsets=false, bool bOnePng=false, bool rgba=false);
};

#endif // _EXPORT_STI_H_
