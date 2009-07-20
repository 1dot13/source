#include "vfs_init.h"
#include "vfs.h"
#include "File/vfs_file.h"
#include "File/vfs_memory_file.h"
#include "Location/vfs_directory_tree.h"
#include "Location/vfs_slf_library.h"
#include "Location/vfs_7z_library.h"
#include "Location/vfs_create_7z_library.h"

#include "PropertyContainer.h"
#include "Tools/Log.h"

#define LOG_VFS_INITIALIZATION
#ifdef LOG_VFS_INITIALIZATION
#define LOG(x) x
#else 
#define LOG(x)
#endif

extern bool g_VFS_NO_UNICODE;
/********************************************************************/
/********************************************************************/

bool InitVirtualFileSystem(vfs::Path const& vfs_ini)
{
	std::list<vfs::Path> li;
	li.push_back(vfs_ini);
	return InitVirtualFileSystem(li);
}
bool InitVirtualFileSystem(std::list<vfs::Path> const& vfs_ini_list)
{
	CPropertyContainer oVFSProps;
	std::list<vfs::Path>::const_iterator clit = vfs_ini_list.begin();
	for(; clit != vfs_ini_list.end(); ++clit)
	{
		oVFSProps.InitFromIniFile(*clit);
	}
	return InitVirtualFileSystem(oVFSProps);
}
bool InitVirtualFileSystem(CPropertyContainer& oVFSProps)
{
	LOG(CLog _LOG(vfs::Path(L"vfs_init.log")));

	vfs::CVirtualFileSystem *pVirtFileSys = GetVFS();

	LOG(_LOG << "Initializing Virtual File System" << CLog::endl);

	if(g_VFS_NO_UNICODE){ LOG(_LOG.Endl() << "UNICODE disabled" << CLog::endl); }

	LOG(_LOG.Endl() << "reading profiles .. ");
	std::list<utf8string> lProfiles, lLocSections;
	oVFSProps.GetStringListProperty(L"vfs_config",L"PROFILES",lProfiles,L"");
	if(lProfiles.empty())
	{
		LOG(_LOG << " ERROR");
		return false;
	}
	else
	{
		LOG(_LOG << " OK");
	}
	LOG(_LOG.Endl() << "  profiles to read : ");
	std::list<utf8string>::const_iterator cit_profiles = lProfiles.begin();
	for(; cit_profiles != lProfiles.end(); ++cit_profiles)
	{
		LOG(_LOG << (*cit_profiles) << ", ");
	}
	LOG(_LOG.Endl());
	
	std::list<utf8string>::const_iterator prof_cit = lProfiles.begin();
	for(; prof_cit != lProfiles.end(); ++prof_cit)
	{
		LOG(_LOG.Endl() << "  reading profile [");
		utf8string sProfSection = utf8string("PROFILE_") + utf8string(*prof_cit);
		utf8string sProfName = oVFSProps.GetStringProperty(sProfSection,L"NAME",L"");
		LOG(_LOG << sProfName << "] .. ");

		vfs::Path profileRoot = oVFSProps.GetStringProperty(sProfSection,L"PROFILE_ROOT",L"");
		
		lLocSections.clear();
		oVFSProps.GetStringListProperty(sProfSection,L"LOCATIONS",lLocSections,L"");

		LOG(_LOG << "OK");
		LOG(_LOG.Endl() << "  locations to read : ");
		std::list<utf8string>::const_iterator cit_locations = lLocSections.begin();
		for(; cit_locations != lLocSections.end(); ++cit_locations)
		{
			LOG(_LOG << (*cit_locations) << ", ");
		}
		LOG(_LOG.Endl().Endl());

		std::list<utf8string>::iterator loc_it = lLocSections.begin();
		bool bIsWriteable = oVFSProps.GetBoolProperty(sProfSection,L"WRITE",false);
		for(; loc_it != lLocSections.end(); ++loc_it)
		{
			LOG(_LOG << "    reading location [ ");
			utf8string sLocSection = utf8string("LOC_") + utf8string(*loc_it);
			vfs::Path sLocPath, sLocMountPoint;
			utf8string sLocType;
			
			sLocPath = oVFSProps.GetStringProperty(sLocSection,"PATH","");
			sLocMountPoint = oVFSProps.GetStringProperty(sLocSection,L"MOUNT_POINT",L"");
			sLocType = oVFSProps.GetStringProperty(sLocSection,L"TYPE",L"NOT_FOUND");
			bool bOptional = oVFSProps.GetBoolProperty(sLocSection,L"OPTIONAL",false);
			if(StrCmp::Equal(sLocType,L"LIBRARY"))
			{
				LOG(_LOG << sLocType << " | " << (*loc_it) << " ] .. ");
				vfs::tReadableFile *pLibFile = NULL;
				bool bOwnFile = false;
				if(!sLocPath.empty())
				{
					pLibFile = vfs::tReadableFile::Cast( new vfs::CFile(profileRoot + sLocPath) );
					bOwnFile = true;
				}
				if(!pLibFile)
				{
					sLocPath = oVFSProps.GetStringProperty(sLocSection,L"VFS_PATH",L"");
					if(!sLocPath.empty())
					{
						pLibFile = pVirtFileSys->GetRFile(profileRoot + sLocPath);
					}
				}
				if(pLibFile)
				{
					utf8string full_str = pLibFile->GetFileName()();
					utf8string ext = full_str.c_wcs().substr(full_str.length()-3,3);
					vfs::ILibrary *pLib = NULL;
					if(StrCmp::Equal(ext,L"slf"))
					{
						pLib = new vfs::CSLFLibrary( pLibFile, sLocMountPoint );
					}
					else if(StrCmp::Equal(ext,L".7z"))
					{
						pLib = new vfs::CUncompressed7zLibrary( pLibFile, sLocMountPoint );
					}
					else
					{
						LOG(_LOG << "ERROR" << CLog::endl);
						utf8string::str_t s = L"File [" + utf8string(sLocPath()).c_wcs() + L"] in not an SLF or 7z library";
						THROWEXCEPTION(s.c_str());
						return false;
					}
					if(!pLib->Init())
					{
						if(!bOptional)
						{
							LOG(_LOG << "ERROR" << CLog::endl);
							//std::cout << "ERROR : library initialization failed [ " << full_str << " ]" << std::endl;
							std::wstring s = L"Could not initialize library [ " + sLocPath().c_wcs()
								+ L" ] in : profile [ " + utf8string(sProfName).c_wcs()
								+ L" ], location [ " + (*loc_it).c_wcs()
								+ L" ], path [ " + (profileRoot + sLocPath)().c_wcs() + L" ]";
							THROWEXCEPTION(s.c_str());
							return false;
						}
						LOG(_LOG << "optional library ignored" << CLog::endl);
					}
					else
					{
						LOG(_LOG << "OK" << CLog::endl);
					}
					pVirtFileSys->AddLocation(vfs::tReadLocation::Cast(pLib), sProfName, bIsWriteable);
				}
				else
				{
					LOG(_LOG << "ERROR" << CLog::endl);
					THROWEXCEPTION(L"File not found");
				}
			}
			else if(StrCmp::Equal(sLocType,L"DIRECTORY"))
			{
				LOG(_LOG << sLocType << " | " << (*loc_it) << " ] .. ");
				vfs::CDirectoryTree *pDirTree = new vfs::CDirectoryTree(sLocMountPoint,profileRoot + sLocPath);
				if(!pDirTree->Init())
				{
					LOG(_LOG << "ERROR" << CLog::endl);
					std::wstring s = L"Could not initialize directory [\"" + sLocPath().c_wcs()
						+ L"\"] in : profile [\"" + utf8string(sProfName).c_wcs()
						+ L"\"], location [\"" + (*loc_it).c_wcs()
						+ L"\"], path [\"" + (profileRoot + sLocPath)().c_wcs() + L"\"]";
					THROWEXCEPTION(s.c_str());
					return false;
				}
				pVirtFileSys->AddLocation(vfs::tReadLocation::Cast(pDirTree),sProfName,bIsWriteable);
				LOG(_LOG << "OK" << CLog::endl);
			}
			else
			{
				LOG(_LOG << "]" << CLog::endl);
			}
			//else if( sLocType == "NOT_FOUND")
			//{
			//	std::wstringstream wss;
			//	wss << L"No Type specified for location [" << sLocSection << L"]";
			//	THROWEXCEPTION(wss.str().c_str());
			//}
		}
		if(bIsWriteable)
		{
			vfs::CProfileStack *pPS = pVirtFileSys->GetProfileStack();
			vfs::CVirtualProfile *pProf = pPS->GetProfile(sProfName);
			if(!pProf)
			{
				pProf = new vfs::CVirtualProfile(sProfName,true);
				pPS->PushProfile(pProf);
			}
			else if(!pProf->Writeable)
			{
				std::wstringstream wss;
				wss << L"Profile [" << sProfName << L"] is supposed to be writeable!";
				THROWEXCEPTION(wss.str().c_str());
			}
			InitWriteProfile(*pProf, profileRoot);
		}
	}
	LOG(_LOG.Endl() << "VFS successfully initialized" << CLog::endl);

	return true;
}

bool InitWriteProfile(vfs::CVirtualProfile &rProf, vfs::Path const& profileRoot)
{
	typedef vfs::IDirectory<vfs::IWriteable> tWDir;
	tWDir *pDir = NULL;
	vfs::CDirectoryTree *pDirTree = NULL;
	vfs::IBaseLocation *pLoc = rProf.GetLocation(vfs::Path(vfs::Const::EMPTY()));
	if(pLoc)
	{
		pDir = dynamic_cast<tWDir*>(pLoc);
	}
	else
	{
		vfs::CDirectoryTree *pDirTree = NULL;
		pDirTree = new vfs::CDirectoryTree(vfs::Path(vfs::Const::EMPTY()),profileRoot);
		if(!pDirTree->Init())
		{
			return false;
		}
		GetVFS()->AddLocation(pDirTree,rProf.Name,true);
		pDir = pDirTree;
	}
	return pDir != NULL;
}
