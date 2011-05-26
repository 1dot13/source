#ifndef _EXPORT_JSD_H_
#define _EXPORT_SLF_H_

#include <vfs/Core/vfs_path.h>
#include <vfs/Core/Interface/vfs_file_interface.h>

#include <exporter_base.h>

namespace ja2xp
{
	class CExportJSD : public IExporterBase
	{
	public:
		typedef IExporterBase::param_list_t param_list_t;
		
		static const wchar_t* commandString;// = L"jsd";

		virtual void handleCommand(param_list_t const& params);
		virtual void printHelp();
	};

	bool convertJSDtoXML(vfs::Path const& sSrc, vfs::Path const& sDst);
};

#endif // _EXPORT_SLF_H_
