//**************************************************************************
//
// Filename :	Install.c
//
//	Purpose :	install routines
//
// Modification history :
//
//		02dec96:HJH				- Creation
//
//**************************************************************************

//**************************************************************************
//
//				Includes
//
//**************************************************************************

	#include "types.h"
	#include <windows.h>
	#include "Install.h"
	#include "RegInst.h"

//**************************************************************************
//
//				Defines
//
//**************************************************************************

//**************************************************************************
//
//				Typedefs
//
//**************************************************************************

//**************************************************************************
//
//				Functions
//
//**************************************************************************

BOOLEAN	InstallApplication( STR strAppname, STR strPath )
{
	HKEY	hKey;
	BOOL	fRet = TRUE;

	hKey = GetAppRegistryKey();
	RegCloseKey( hKey );

	//hKeySection = GetSectionKey("Startup");
	//RegCloseKey( hKeySection );

	fRet = fRet && WriteProfileChar( "Startup", "InstPath", strPath );

	return(fRet);
}
