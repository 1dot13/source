#ifdef PRECOMPILEDHEADERS
	#include "TileEngine All.h"
#else
	#include "sgp.h"
	#include "wcheck.h"
	#include "sysutil.h"
#endif

UINT32					guiBOTTOMPANEL = 0;
UINT32					guiRIGHTPANEL = 0;
UINT32					guiRENDERBUFFER = 0;
UINT32					guiSAVEBUFFER = 0;
UINT32					guiEXTRABUFFER = 0;

BOOLEAN gfExtraBuffer = FALSE;

BOOLEAN	InitializeSystemVideoObjects( )
{

	return( TRUE );
}

BOOLEAN	InitializeGameVideoObjects( )
{
	VSURFACE_DESC		vs_desc;
	UINT16					usWidth;
	UINT16					usHeight;
	UINT8						ubBitDepth;

	// Create render buffer
	GetCurrentVideoSettings( &usWidth, &usHeight, &ubBitDepth );
	vs_desc.fCreateFlags = VSURFACE_CREATE_DEFAULT | VSURFACE_SYSTEM_MEM_USAGE;
	vs_desc.usWidth = usWidth;
	vs_desc.usHeight = usHeight;
	vs_desc.ubBitDepth = ubBitDepth;

	CHECKF( AddVideoSurface( &vs_desc, &guiSAVEBUFFER ) );

	CHECKF( AddVideoSurface( &vs_desc, &guiEXTRABUFFER ) );
	gfExtraBuffer = TRUE;

	guiRENDERBUFFER=FRAME_BUFFER;

	return( TRUE );

}
