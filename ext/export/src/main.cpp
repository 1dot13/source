#include <iostream>
#include <map>

#include <export/sti/export_sti.h>
#include <export/slf/export_slf.h>
#include <export/jsd/export_jsd.h>

#include <exporter_base.h>

static const char* VERSION_STRING = "1.1.1";

typedef std::map<vfs::String, ja2xp::IExporterBase*, vfs::String::Less> CommandMap_t;
CommandMap_t g_command_map;

class HelpCommand : public ja2xp::IExporterBase
{
public:
	typedef ja2xp::IExporterBase::param_list_t param_list_t;
	
	static const wchar_t* commandString;// = L"help";

	virtual void handleCommand(param_list_t const& params)
	{
		param_list_t::const_iterator cit = params.begin();
		for(; cit != params.end(); ++cit)
		{
			CommandMap_t::const_iterator comm_it = g_command_map.find(*cit);
			if(comm_it != g_command_map.end())
			{
				this->printBasicUsage();
				comm_it->second->printHelp();
				return;
			}
		}
		this->printBasicUsage();
		this->printHelp();
	}

	void printBasicUsage()
	{
		std::wcout 
			<< L"*** ja2export : version " << VERSION_STRING << " ***" << std::endl
			<< std::endl
			<< L"Usage : ja2export command [OPTIONS] source destination" << std::endl
			<< std::endl
			<< L" converts file(s) specified by \"source\" and outputs according to filename" << std::endl
			<< L" specified by \"destination\"" << std::endl
			<< std::endl;
	}
	virtual void printHelp()
	{
		std::wcout 
			<< L" * if \"source\" is a file pattern with the wildcard *, then all files that" << std::endl
			<< L"   match this pattern will be converted" << std::endl
			<< L" * if \"destination\" is a file name, the converted file will take exactly " << std::endl
			<< L"   this name" << std::endl
			<< L" * if \"destination\" is a directory name, then the converted file will be" << std::endl
			<< L"   written in this directory and its extension will be modified according" << std::endl
			<< L"   to the command specification" << std::endl
			<< std::endl
			<< L" commands :" << std::endl
			<< L"   help : print this message, use help [command] to print specific help message" << std::endl
			<< L"   sti  : convert STI image file(s) to a series of png images in a 7z archive" << std::endl
			<< L"   slf  : convert SLF archive(s) to uncompressed 7z archive(s)" << std::endl
			<< L"   jsd  : convert JSD file(s) to XML file(s)" << std::endl
			<< std::endl
			<< L" options :" << std::endl
			<< L"   -create_dst : creates destination directory if it doesn't exist" << std::endl
			<< L"   -lib        : when set, the next two paramers are treated as" << std::endl
			<< L"                 'source path' and 'source file pattern'" << std::endl;
	}
};
const wchar_t* HelpCommand::commandString = L"help";

int wmain(int argc, wchar_t **argv)
{
	std::list<std::wstring> param_list;
	argv++;
	while(*argv)
	{
		param_list.push_back(*argv++);
	};

	std::auto_ptr<HelpCommand> cmd_help(new HelpCommand());
	g_command_map[HelpCommand::commandString] = cmd_help.get();
	
	std::auto_ptr<ja2xp::CExportSTI> cmd_sti(new ja2xp::CExportSTI());
	g_command_map[ja2xp::CExportSTI::commandString] = cmd_sti.get();
	
	std::auto_ptr<ja2xp::CExportSLF> cmd_slf(new ja2xp::CExportSLF());
	g_command_map[ja2xp::CExportSLF::commandString] = cmd_slf.get();
	
	std::auto_ptr<ja2xp::CExportJSD> cmd_jsd(new ja2xp::CExportJSD());
	g_command_map[ja2xp::CExportJSD::commandString] = cmd_jsd.get();

	if(!param_list.empty())
	{
		ja2xp::IExporterBase* command = g_command_map[param_list.front()];
		if(command)
		{
			std::list<std::wstring> command_params(++param_list.begin(), param_list.end());
			command->handleCommand(command_params);
			return 1;
		}
		else
		{
			std::wcout << L"Unknown command \"" << param_list.front() << "\"";
			return 0;
		}
	}
	cmd_help->printBasicUsage();
	cmd_help->printHelp();

	return 1;
}
