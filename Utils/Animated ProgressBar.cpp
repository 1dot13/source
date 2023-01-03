#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "types.h"
	#include "Animated ProgressBar.h"
	#include "MemMan.h"
	#include "debug.h"
	#include "Font Control.h"
	#include "vsurface.h"
	#include "video.h"
	#include "Render Dirty.h"
	#include "music control.h"
	#include "Timer Control.h"
	#include "sysutil.h"
	#include "random.h"
	#include "math.h"
	#include "WordWrap.h"
	#include "Message.h"
	#include "Text.h"
#endif

double rStart, rEnd;
double rActual;



#define MAX_PROGRESSBARS 4

PROGRESSBAR *pBar[ MAX_PROGRESSBARS ];

UINT16 gusLeftmostShaded = 0;

extern BOOLEAN bShowSmallImage;

// Flugente: stuff needed for loadscreen hints
extern UINT16 num_found_loadscreenhints;
static UINT16 usCurrentLoadScreenHint = 0;
static BOOLEAN bShowLoadScreenHintInLog = FALSE;	// FALSE: No Display in Strategic/Tactical Log, TRUE: Display in Strategic/Tactical Log

void CreateLoadingScreenProgressBar(BOOLEAN resetLoadScreenHint)
{
	if (resetLoadScreenHint)
		ResetLoadScreenHint();

	gusLeftmostShaded = 162;

	//// Special case->show small image centered
	//if (bShowSmallImage == TRUE)
	//{
	//	if (iResolution >= _800x600)
	//	{
	//		CreateProgressBar(0, iScreenWidthOffset + 162, iScreenHeightOffset + 427, iScreenWidthOffset + 480, iScreenHeightOffset + 443);
	//	}
	//}
	//else
	//{
	//	if (iResolution >= _640x480 && iResolution < _800x600)
	//	{
	//		CreateProgressBar(0, 162, 427, 480, 443);
	//	}
	//	else if (iResolution < _1024x768)
	//	{
	//		CreateProgressBar(0, (202 + (SCREEN_WIDTH - 800)/2), 533 + ((SCREEN_HEIGHT - 600) / 2), 600 + ((SCREEN_WIDTH - 800) / 2), 554 + ((SCREEN_HEIGHT - 600) / 2));
	//	}
	//	else
	//	{
	//		CreateProgressBar(0, 259 + ((SCREEN_WIDTH - 1024) / 2), 683 + ((SCREEN_HEIGHT - 768) / 2), 767 + ((SCREEN_WIDTH - 1024) / 2), 708 + ((SCREEN_HEIGHT - 768) / 2));
	//	}
	//}
	

	CreateProgressBar(0, SCREEN_WIDTH*162/640, SCREEN_HEIGHT*427/480, SCREEN_WIDTH*480/640, SCREEN_HEIGHT*443/480);


	SetProgressBarUseBorder(0, FALSE );
}

void RemoveLoadingScreenProgressBar()
{
	// WANNE: Sleep for a while, so we have time to read the load screen hint
	if (gGameExternalOptions.gfUseLoadScreenHints && gGameExternalOptions.ubAdditionalDelayUntilLoadScreenDisposal > 0 && usCurrentLoadScreenHint )
	{
		Sleep(gGameExternalOptions.ubAdditionalDelayUntilLoadScreenDisposal * 1000);
	}

	RemoveProgressBar( 0 );
	SetFontShadow(DEFAULT_SHADOW);
}

// OJW - 20090422
void CreateProgressBarNoBorder( UINT8 ubProgressBarID, UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom )
{
	CreateProgressBar( ubProgressBarID, usLeft, usTop, usRight, usBottom );
	SetProgressBarUseBorder( ubProgressBarID, FALSE );
}

//This creates a single progress bar given the coordinates without a panel (containing a title and background).
//A panel is automatically created if you specify a title using SetProgressBarTitle
BOOLEAN CreateProgressBar( UINT8 ubProgressBarID, UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom )
{

	PROGRESSBAR *pNew;
	//Allocate new progress bar
	pNew = (PROGRESSBAR*)MemAlloc( sizeof( PROGRESSBAR ) );
	Assert( pNew );

	if( pBar[ ubProgressBarID ] )
		RemoveProgressBar( ubProgressBarID );

	memset( pNew, 0, sizeof( PROGRESSBAR ) );

	pBar[ ubProgressBarID ] = pNew;
	pNew->ubProgressBarID = ubProgressBarID;
	//Assign coordinates
	pNew->usBarLeft = usLeft;
	pNew->usBarTop = usTop;
	pNew->usBarRight = usRight;
	pNew->usBarBottom = usBottom;
	//Init default data
	pNew->fPanel = FALSE;
	pNew->usMsgFont = (UINT16)FONT12POINT1;
	pNew->ubMsgFontForeColor = FONT_BLACK;
	pNew->ubMsgFontShadowColor = 0;
	SetRelativeStartAndEndPercentage( pNew->ubProgressBarID, 0, 100, NULL );
	pNew->swzTitle = NULL;

	//Default the progress bar's color to be red
	pNew->ubColorFillRed = 150;
	pNew->ubColorFillGreen = 0;
	pNew->ubColorFillBlue = 0;

	pNew->fDisplayText = FALSE;
	//OJW - 20090222
	pNew->uiFrameBuffer = FRAME_BUFFER;
	pNew->fDrawBorder = TRUE;


	return TRUE;
}

//You may also define a panel to go in behind the progress bar.	You can now assign a title to go with
//the panel.
void DefineProgressBarPanel( UINT32 ubID, UINT8 r, UINT8 g, UINT8 b,
														UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom )
{
	PROGRESSBAR *pCurr;
	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;

	pCurr->fPanel = TRUE;
	pCurr->usPanelLeft = usLeft;
	pCurr->usPanelTop = usTop;
	pCurr->usPanelRight = usRight;
	pCurr->usPanelBottom = usBottom;
	pCurr->usColor = Get16BPPColor( FROMRGB( r, g, b ) );
	//Calculate the slightly lighter and darker versions of the same rgb color
	pCurr->usLtColor = Get16BPPColor( FROMRGB( (UINT8)min( 255, (UINT16)(r*1.33)),
																						(UINT8)min( 255, (UINT16)(g*1.33)),
																						(UINT8)min( 255, (UINT16)(b*1.33)) ));
	pCurr->usDkColor = Get16BPPColor( FROMRGB( (UINT8)(r*0.75), (UINT8)(g*0.75), (UINT8)(b*0.75) ) );
}

//Assigning a title for the panel will automatically position the text horizontally centered on the
//panel and vertically centered from the top of the panel, to the top of the progress bar.
void SetProgressBarTitle( UINT32 ubID, STR16 pString, UINT32 usFont, UINT8 ubForeColor, UINT8 ubShadowColor )
{
	PROGRESSBAR *pCurr;
	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;
	if( pCurr->swzTitle )
	{
		MemFree( pCurr->swzTitle );
		pCurr->swzTitle = NULL;
	}
	if( pString && wcslen( pString ) )
	{
		pCurr->swzTitle = (STR16) MemAlloc( (UINT32) ( sizeof( CHAR16 ) * ( wcslen( pString ) + 1 ) ) );
		swprintf( pCurr->swzTitle, pString );
	}
	pCurr->usTitleFont = (UINT16)usFont;
	pCurr->ubTitleFontForeColor = ubForeColor;
	pCurr->ubTitleFontShadowColor = ubShadowColor;
}

//Unless you set up the attributes, any text you pass to SetRelativeStartAndEndPercentage will
//default to FONT12POINT1 in a black color.
void SetProgressBarMsgAttributes( UINT32 ubID, UINT32 usFont, UINT8 ubForeColor, UINT8 ubShadowColor )
{
	PROGRESSBAR *pCurr;
	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;
	pCurr->usMsgFont = (UINT16)usFont;
	pCurr->ubMsgFontForeColor = ubForeColor;
	pCurr->ubMsgFontShadowColor = ubShadowColor;
}

//OJW - 20090422
void SetProgressBarRenderBuffer( UINT32 ubID , UINT32 uiBufferID )
{
	PROGRESSBAR *pCurr;
	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;
	pCurr->uiFrameBuffer = uiBufferID;
}

void SetProgressBarUseBorder( UINT32 ubID , BOOLEAN bBorder )
{
	PROGRESSBAR *pCurr;
	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;
	pCurr->fDrawBorder = bBorder;
}


//When finished, the progress bar needs to be removed.
void RemoveProgressBar( UINT8 ubID )
{
	Assert( ubID < MAX_PROGRESSBARS );
	if( pBar[ubID] )
	{
		if( pBar[ubID]->swzTitle )
			MemFree( pBar[ubID]->swzTitle );
		MemFree( pBar[ubID] );
		pBar[ubID] = NULL;
		return;
	}
}

void ResetLoadScreenHint()
{
	usCurrentLoadScreenHint = 0;
	bShowLoadScreenHintInLog = FALSE;
}

void ShowLoadScreenHintInLoadScreen(UINT16 bottomPosition)
{
	DisplayWrappedString( 0, bottomPosition, SCREEN_WIDTH, 2, FONT14ARIAL, FONT_GRAY2, zLoadScreenHint[usCurrentLoadScreenHint].szName, FONT_MCOLOR_BLACK, TRUE, CENTER_JUSTIFIED );

	// Next show the hint in the log (strategic / tactical)
	bShowLoadScreenHintInLog = TRUE;
}

void ShowLoadScreenHintInStrategicLog()
{
	// We should display the hint in strategic log
	if (bShowLoadScreenHintInLog == TRUE && gGameExternalOptions.gfUseLoadScreenHints && usCurrentLoadScreenHint )
	{
		ScreenMsg( FONT_GRAY2, MSG_INTERFACE, New113Message[MSG113_HINT_TEXT], zLoadScreenHint[usCurrentLoadScreenHint].szName);
		bShowLoadScreenHintInLog = FALSE;	
	}		
}

void ShowLoadScreenHintInTacticalLog()
{	
	if (bShowLoadScreenHintInLog == TRUE && gGameExternalOptions.gfUseLoadScreenHints && usCurrentLoadScreenHint)
	{
		ScreenMsg( FONT_GRAY2, MSG_INTERFACE, New113Message[MSG113_HINT_TEXT], zLoadScreenHint[usCurrentLoadScreenHint].szName);
		bShowLoadScreenHintInLog = FALSE;
	}	
}

// Flugente: this function selects the next hint to display, and makes sure it is not played again during this run of the exe
void SetNewLoadScreenHint()
{
	if ( !gGameExternalOptions.gfUseLoadScreenHints )
		return;

	UINT16 lastfnd = 0;
	UINT8 possiblehints[LOADSCREENHINT_MAX];

	for(UINT32 cnt = 1; cnt <= num_found_loadscreenhints; ++cnt)
	{
		if ( !zLoadScreenHint[cnt].fAlreadyShown )
		{
			// we now check the flags of a hint to determine wether it can show up. It can if at least one flag is valid
			BOOLEAN fShow = FALSE;			
			// still unsure how to check for these...
			if ( zLoadScreenHint[cnt].usFlags & (LOADSCREEN_LORE|LOADSCREEN_WEAPON|LOADSCREEN_ITEM|LOADSCREEN_KEYBIND)  )
				fShow = TRUE;
			else if ( zLoadScreenHint[cnt].usFlags & LOADSCREEN_FOOD && UsingFoodSystem() )
				fShow = TRUE;
			else if ( zLoadScreenHint[cnt].usFlags & LOADSCREEN_ZOMBIE && gGameSettings.fOptions[TOPTION_ZOMBIES] )
				fShow = TRUE;
			else if ( zLoadScreenHint[cnt].usFlags & LOADSCREEN_OVERHEAT && gGameExternalOptions.fWeaponOverheating )
				fShow = TRUE;
			else if ( zLoadScreenHint[cnt].usFlags & LOADSCREEN_NCTH && UsingNewCTHSystem() )
				fShow = TRUE;
			else if ( zLoadScreenHint[cnt].usFlags & LOADSCREEN_COVERTOPS && gGameOptions.fNewTraitSystem )
				fShow = TRUE;
						
			if ( fShow )
				possiblehints[lastfnd++] = cnt;
		}
	}

	UINT16 sel = possiblehints[ Random(lastfnd) ];
	
	// WANNE: All the loadscreen hints we have, have already been displayed. No matter, re-display a random loadscreen
	if (sel <= 0 || sel > num_found_loadscreenhints)
	{
		sel = Random(num_found_loadscreenhints);
		if (sel == 0)
			sel++;
	}

	zLoadScreenHint[sel].fAlreadyShown = TRUE;

	usCurrentLoadScreenHint = sel;
}

//An important setup function.	The best explanation is through example.	The example being the loading
//of a file -- there are many stages of the map loading.	In JA2, the first step is to load the tileset.
//Because it is a large chunk of the total loading of the map, we may gauge that it takes up 30% of the
//total load.	Because it is also at the beginning, we would pass in the arguments ( 0, 30, "text" ).
//As the process animates using UpdateProgressBar( 0 to 100 ), the total progress bar will only reach 30%
//at the 100% mark within UpdateProgressBar.	At that time, you would go onto the next step, resetting the
//relative start and end percentage from 30 to whatever, until your done.
void SetRelativeStartAndEndPercentage( UINT8 ubID, UINT16 uiRelStartPerc, UINT16 uiRelEndPerc, STR16 str)
{
	PROGRESSBAR *pCurr;
	UINT16 usStartX, usStartY;

	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ ubID ];
	if( !pCurr )
		return;

	pCurr->rStart = (double)uiRelStartPerc*0.01f;
	pCurr->rEnd = (double)uiRelEndPerc*0.01f;

	//Render the entire panel now, as it doesn't need update during the normal rendering
	if( pCurr->fPanel )
	{
		//Draw panel
		ColorFillVideoSurfaceArea( FRAME_BUFFER,
			pCurr->usPanelLeft, pCurr->usPanelTop, pCurr->usPanelRight, pCurr->usPanelBottom, pCurr->usLtColor );
		ColorFillVideoSurfaceArea( FRAME_BUFFER,
			pCurr->usPanelLeft+1, pCurr->usPanelTop+1, pCurr->usPanelRight, pCurr->usPanelBottom, pCurr->usDkColor );
		ColorFillVideoSurfaceArea( FRAME_BUFFER,
			pCurr->usPanelLeft+1, pCurr->usPanelTop+1, pCurr->usPanelRight-1, pCurr->usPanelBottom-1, pCurr->usColor );
		InvalidateRegion( pCurr->usPanelLeft, pCurr->usPanelTop, pCurr->usPanelRight, pCurr->usPanelBottom );
		//Draw title

		if( pCurr->swzTitle )
		{
			usStartX = pCurr->usPanelLeft +																					// left position
								(pCurr->usPanelRight - pCurr->usPanelLeft)/2 -								// + half width
								StringPixLength( pCurr->swzTitle, pCurr->usTitleFont ) / 2;	// - half string width
			usStartY = pCurr->usPanelTop + 3;
			SetFont( pCurr->usTitleFont );
			SetFontForeground( pCurr->ubTitleFontForeColor );
			SetFontShadow( pCurr->ubTitleFontShadowColor );
			SetFontBackground( 0 );
			mprintf( usStartX, usStartY, pCurr->swzTitle );
		}
	}

	if( pCurr->fDisplayText )
	{
		//Draw message
		if( str )
		{
			if( pCurr->fUseSaveBuffer )
			{
				UINT16 usFontHeight = GetFontHeight( pCurr->usMsgFont );

				RestoreExternBackgroundRect( pCurr->usBarLeft, pCurr->usBarBottom, (INT16)(pCurr->usBarRight-pCurr->usBarLeft), (INT16)(usFontHeight + 3) );
			}

			SetFont( pCurr->usMsgFont );
			SetFontForeground( pCurr->ubMsgFontForeColor );
			SetFontShadow( pCurr->ubMsgFontShadowColor );
			SetFontBackground( 0 );
			mprintf( pCurr->usBarLeft, pCurr->usBarBottom + 3, str );
		}
	}

	// Flugente: loadscreen hints
	if (gGameExternalOptions.gfUseLoadScreenHints && usCurrentLoadScreenHint )
	{		
		ShowLoadScreenHintInLoadScreen(pCurr->usBarBottom + 3 - 100);		
	}
}

//This part renders the progress bar at the percentage level that you specify.	If you have set relative
//percentage values in the above function, then the uiPercentage will be reflected based off of the relative
//percentages.
void RenderProgressBar( UINT8 ubID, UINT32 uiPercentage )
{
	static UINT32 uiLastTime = 0;
	UINT32 uiCurTime = GetJA2Clock();
	double rActual;
	PROGRESSBAR *pCurr=NULL;
	//UINT32 r, g;
	INT32 end;

	Assert( ubID < MAX_PROGRESSBARS );
	pCurr = pBar[ubID];

	if( pCurr == NULL )
		return;

	if( pCurr )
	{
		rActual = pCurr->rStart+(pCurr->rEnd-pCurr->rStart)*uiPercentage*0.01;

		if( fabs(rActual - pCurr->rLastActual) < 0.01 )
		{
			return;
		}

		pCurr->rLastActual = ( DOUBLE )( ( INT32)( rActual * 100 ) * 0.01 );

		end = (INT32)(pCurr->usBarLeft+2.0+rActual*(pCurr->usBarRight-pCurr->usBarLeft-4));
		if( end < pCurr->usBarLeft+2 || end > pCurr->usBarRight-2 )
		{
			return;
		}
		if( !pCurr->fDrawBorder )
		{
			ColorFillVideoSurfaceArea( pCurr->uiFrameBuffer, //FRAME_BUFFER,
				pCurr->usBarLeft, pCurr->usBarTop, end, pCurr->usBarBottom,
				Get16BPPColor(FROMRGB( pCurr->ubColorFillRed, pCurr->ubColorFillGreen, pCurr->ubColorFillBlue )) );
			//if( pCurr->usBarRight > gusLeftmostShaded )
			//{
			//	ShadowVideoSurfaceRect( FRAME_BUFFER, gusLeftmostShaded+1, pCurr->usBarTop, end, pCurr->usBarBottom );
			//	gusLeftmostShaded = (UINT16)end;
			//}
		}
		else
		{
			//Border edge of the progress bar itself in gray
			ColorFillVideoSurfaceArea( pCurr->uiFrameBuffer,
				pCurr->usBarLeft, pCurr->usBarTop, pCurr->usBarRight, pCurr->usBarBottom,
				Get16BPPColor(FROMRGB(160, 160, 160)) );
			//Interior of progress bar in black
			ColorFillVideoSurfaceArea( pCurr->uiFrameBuffer,
				pCurr->usBarLeft+2, pCurr->usBarTop+2, pCurr->usBarRight-2, pCurr->usBarBottom-2,
				Get16BPPColor(FROMRGB(	0,	0,	0)) );
			ColorFillVideoSurfaceArea(pCurr->uiFrameBuffer,	pCurr->usBarLeft+2, pCurr->usBarTop+2, end, pCurr->usBarBottom-2, Get16BPPColor(FROMRGB(72 , 155, 24)));
		}
		InvalidateRegion( pCurr->usBarLeft, pCurr->usBarTop, pCurr->usBarRight, pCurr->usBarBottom );
		ExecuteBaseDirtyRectQueue();
		EndFrameBufferRender();
		RefreshScreen( NULL );
	}

	// update music here
	if( uiCurTime > ( uiLastTime + 200 ) )
	{
		MusicPoll( TRUE );
		uiLastTime = GetJA2Clock();
	}
}

void SetProgressBarColor( UINT8 ubID, UINT8 ubColorFillRed, UINT8 ubColorFillGreen, UINT8 ubColorFillBlue )
{
	PROGRESSBAR *pCurr=NULL;

	Assert( ubID < MAX_PROGRESSBARS );

	pCurr = pBar[ubID];
	if( pCurr == NULL )
		return;

	pCurr->ubColorFillRed = ubColorFillRed;
	pCurr->ubColorFillGreen = ubColorFillGreen;
	pCurr->ubColorFillBlue = ubColorFillBlue;
}


void SetProgressBarTextDisplayFlag( UINT8 ubID, BOOLEAN fDisplayText, BOOLEAN fUseSaveBuffer, BOOLEAN fSaveScreenToFrameBuffer )
{
	PROGRESSBAR *pCurr=NULL;


	Assert( ubID < MAX_PROGRESSBARS );

	pCurr = pBar[ubID];
	if( pCurr == NULL )
		return;

	pCurr->fDisplayText = fDisplayText;

	pCurr->fUseSaveBuffer = fUseSaveBuffer;

	//if we are to use the save buffer, blit the portion of the screen to the save buffer
	if( fSaveScreenToFrameBuffer )
	{
		UINT16 usFontHeight = GetFontHeight( pCurr->usMsgFont )+3;

		//blit everything to the save buffer ( cause the save buffer can bleed through )
		BlitBufferToBuffer(guiRENDERBUFFER, guiSAVEBUFFER, pCurr->usBarLeft, pCurr->usBarBottom, (UINT16)(pCurr->usBarRight-pCurr->usBarLeft), usFontHeight );
	}
}
