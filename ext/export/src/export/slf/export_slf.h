#ifndef _EXPORT_SLF_H_
#define _EXPORT_SLF_H_

#include <vfs/Core/vfs_path.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <exporter_base.h>

namespace ja2xp
{
	class CExportSLF : public IExporterBase
	{
	public:
		typedef IExporterBase::param_list_t param_list_t;
		
		static const wchar_t* commandString;// = L"slf";

		virtual void handleCommand(param_list_t const& params);
		virtual void printHelp();
	};

	bool convertSLFto7z(vfs::tReadableFile* pInFile, vfs::tWritableFile *pOutFile, bool bConvertSTIs=false, bool bPngOffsets=false);
	bool convertSLFto7z(vfs::Path const& sSrc, vfs::Path const& sDst, bool bConvertSTIs=false, bool bPngOffsets=false);
};

#endif // _EXPORT_SLF_H_
