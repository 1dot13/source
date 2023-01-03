#ifndef _EXPORTER_BASE_H_
#define _EXPORTER_BASE_H_

#include <list>
#include <string>

namespace ja2xp
{
	class IExporterBase
	{
	public:
		typedef std::list<std::wstring> param_list_t;

		virtual void handleCommand(std::list<std::wstring> const& params) = 0;
		virtual void printHelp() = 0;
	};
};

#endif // _EXPORTER_BASE_H_
