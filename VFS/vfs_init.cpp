#include "vfs_settings.h"
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


/********************************************************************/
/********************************************************************/

bool initVirtualFileSystem(vfs::Path const& vfs_ini)
{
	std::list<vfs::Path> li;
	li.push_back(vfs_ini);
	return initVirtualFileSystem(li);
}
bool initVirtualFileSystem(std::list<vfs::Path> const& vfs_ini_list)
{
	CPropertyContainer oVFSProps;
	std::list<vfs::Path>::const_iterator clit = vfs_ini_list.begin();
	for(; clit != vfs_ini_list.end(); ++clit)
	{
		oVFSProps.initFromIniFile(*clit);
	}
	return initVirtualFileSystem(oVFSProps);
}
bool initVirtualFileSystem(CPropertyContainer& oVFSProps)
{
	LOG(CLog _LOG(vfs::Path(L"vfs_init.log")));

	vfs::CVirtualFileSystem *pVirtFileSys = getVFS();

	LOG(_LOG << "Initializing Virtual File System" << CLog::ENDL);

	if(!vfs::Settings::getUseUnicode()){ LOG(_LOG.endl() << "UNICODE disabled" << CLog::ENDL); }

	LOG(_LOG.endl() << "reading profiles .. ");
	std::list<utf8string> lProfiles, lLocSections;
	oVFSProps.getStringListProperty(L"vfs_config",L"PROFILES",lProfiles,L"");
	if(lProfiles.empty())
	{
		LOG(_LOG << " ERROR");
		return false;
	}
	else
	{
		LOG(_LOG << " OK");
	}
	LOG(_LOG.endl() << "  profiles to read : ");
	std::list<utf8string>::const_iterator cit_profiles = lProfiles.begin();
	for(; cit_profiles != lProfiles.end(); ++cit_profiles)
	{
		LOG(_LOG << (*cit_profiles) << ", ");
	}
	LOG(_LOG.endl());
	
	std::list<utf8string>::const_iterator prof_cit = lProfiles.begin();
	for(; prof_cit != lProfiles.end(); ++prof_cit)
	{
		LOG(_LOG.endl() << "  reading profile [");
		utf8string sProfSection = utf8string("PROFILE_") + utf8string(*prof_cit);
		utf8string sProfName = oVFSProps.getStringProperty(sProfSection,L"NAME",L"");
		LOG(_LOG << sProfName << "] .. ");

		vfs::Path profileRoot = oVFSProps.getStringProperty(sProfSection,L"PROFILE_ROOT",L"");
		
		lLocSections.clear();
		oVFSProps.getStringListProperty(sProfSection,L"LOCATIONS",lLocSections,L"");

		LOG(_LOG << "OK");
		LOG(_LOG.endl() << "  locations to read : ");
		std::list<utf8string>::const_iterator cit_locations = lLocSections.begin();
		for(; cit_locations != lLocSections.end(); ++cit_locations)
		{
			LOG(_LOG << (*cit_locations) << ", ");
		}
		LOG(_LOG.endl().endl());

		std::list<utf8string>::iterator loc_it = lLocSections.begin();
		bool bIsWritable = oVFSProps.getBoolProperty(sProfSection,L"WRITE",false);
		for(; loc_it != lLocSections.end(); ++loc_it)
		{
			LOG(_LOG << "    reading location [ ");
			utf8string sLocSection = utf8string("LOC_") + utf8string(*loc_it);
			vfs::Path sLocPath, sLocMountPoint;
			utf8string sLocType;
			
			sLocPath = oVFSProps.getStringProperty(sLocSection,"PATH","");
			sLocMountPoint = oVFSProps.getStringProperty(sLocSection,L"MOUNT_POINT",L"");
			sLocType = oVFSProps.getStringProperty(sLocSection,L"TYPE",L"NOT_FOUND");
			bool bOptional = oVFSProps.getBoolProperty(sLocSection,L"OPTIONAL",false);
			if(StrCmp::Equal(sLocType,L"LIBRARY"))
			{
				LOG(_LOG << sLocType << " | " << (*loc_it) << " ] .. ");
				vfs::tReadableFile *pLibFile = NULL;
				bool bOwnFile = false;
				if(!sLocPath.empty())
				{
					pLibFile = vfs::tReadableFile::cast( new vfs::CFile(profileRoot + sLocPath) );
					bOwnFile = true;
				}
				if(!pLibFile)
				{
					sLocPath = oVFSProps.getStringProperty(sLocSection,L"VFS_PATH",L"");
					if(!sLocPath.empty())
					{
						pLibFile = pVirtFileSys->getReadFile(profileRoot + sLocPath);
					}
				}
				if(pLibFile)
				{
					utf8string full_str = pLibFile->getName()();
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
						LOG(_LOG << "ERROR" << CLog::ENDL);
						THROWEXCEPTION(BuildString().add(L"File [").add(sLocPath).add(L"] in not an SLF or 7z library").get());
					}
					if(!pLib->init())
					{
						if(!bOptional)
						{
							LOG(_LOG << "ERROR" << CLog::ENDL);
							BuildString bs;
							bs.add(L"Could not initialize library [").add(sLocPath).add(L" ]").add(
								L" in : profile [ ").add(sProfName).add(L" ],").add(
								L" location [ ").add((*loc_it)).add(L" ],").add(
								L" path [ ").add((profileRoot + sLocPath)).add(L" ]");
							THROWEXCEPTION(bs.get());
						}
						LOG(_LOG << "optional library ignored" << CLog::ENDL);
					}
					else
					{
						LOG(_LOG << "OK" << CLog::ENDL);
					}
					pVirtFileSys->addLocation(vfs::tReadLocation::cast(pLib), sProfName, bIsWritable);
				}
				else
				{
					LOG(_LOG << "ERROR" << CLog::ENDL);
					THROWEXCEPTION(L"File not found");
				}
			}
			else if(StrCmp::Equal(sLocType,L"DIRECTORY"))
			{
				LOG(_LOG << sLocType << " | " << (*loc_it) << " ] .. ");
				vfs::IBaseLocation *pDirLocation = NULL;
				bool init_success = false;
				if(bIsWritable)
				{
					vfs::CDirectoryTree *pDirTree = new vfs::CDirectoryTree(sLocMountPoint,profileRoot + sLocPath);
					init_success = pDirTree->init();
					pDirLocation = pDirTree;
				}
				else
				{
					vfs::CReadOnlyDirectoryTree *pDirTree = new vfs::CReadOnlyDirectoryTree(sLocMountPoint,profileRoot + sLocPath);
					init_success = pDirTree->init();
					pDirLocation = pDirTree;
				}
				if(!init_success)
				{
					LOG(_LOG << "ERROR" << CLog::ENDL);
					BuildString bs;
					bs.add(L"Could not initialize directory [\"").add(sLocPath).add(L"\"]").add(
						L" in : profile [\"").add(sProfName).add(L"\"],").add(
						L" location [\"").add((*loc_it)).add(L"\"],").add(
						L" path [\"").add(profileRoot + sLocPath).add(L"\"]");
					THROWEXCEPTION(bs.get());
				}
				pVirtFileSys->addLocation(pDirLocation,sProfName,bIsWritable);
				LOG(_LOG << "OK" << CLog::ENDL);
			}
			else
			{
				LOG(_LOG << "]" << CLog::ENDL);
			}
			//else if( sLocType == "NOT_FOUND")
			//{
			//	std::wstringstream wss;
			//	wss << L"No Type specified for location [" << sLocSection << L"]";
			//	THROWEXCEPTION(wss.str().c_str());
			//}
		}
		if(bIsWritable)
		{
			vfs::CProfileStack *pPS = pVirtFileSys->getProfileStack();
			vfs::CVirtualProfile *pProf = pPS->getProfile(sProfName);
			if(!pProf)
			{
				pProf = new vfs::CVirtualProfile(sProfName,true);
				pPS->pushProfile(pProf);
			}
			else if(!pProf->cWritable)
			{
				std::wstringstream wss;
				wss << L"Profile [" << sProfName << L"] is supposed to be writable!";
				THROWEXCEPTION(wss.str().c_str());
			}
			initWriteProfile(*pProf, profileRoot);
		}
	}
	LOG(_LOG.endl() << "VFS successfully initialized" << CLog::ENDL);
	return true;
}

bool initWriteProfile(vfs::CVirtualProfile &rProf, vfs::Path const& profileRoot)
{
	typedef vfs::TDirectory<vfs::IWritable> tWDir;
	tWDir *pDir = NULL;
	vfs::IBaseLocation *pLoc = rProf.getLocation(vfs::Path(vfs::Const::EMPTY()));
	if(pLoc)
	{
		pDir = dynamic_cast<tWDir*>(pLoc);
	}
	else
	{
		vfs::CDirectoryTree *pDirTree = NULL;
		pDirTree = new vfs::CDirectoryTree(vfs::Path(vfs::Const::EMPTY()),profileRoot);
		if(!pDirTree->init())
		{
			return false;
		}
		getVFS()->addLocation(pDirTree,rProf.cName,true);
		pDir = pDirTree;
	}
	return pDir != NULL;
}
