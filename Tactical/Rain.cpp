#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "types.h"
	//#include "soldier control.h"
	#include "overhead.h"
	#include "animation control.h"
	#include "timer.h"
	#include "event pump.h"
//	#include "Sound Control.h"
	#include "interface.h"
	#include "Font Control.H"
	#include "ai.h"
	#include "interface.h"
	#include "message.h"
	#include "text.h"
	#include "TeamTurns.h"
	#include "Smell.h"
	#include "Soldier Functions.h"
	#include "cursors.h"
	#include "Queen Command.h"
	#include "lighting.h"
	#include "environment.h"
	#include "dialogue control.h"
	#include "Soldier Profile Type.h"
	#include "air raid.h"
	#include "SkillCheck.h"
	#include "AIInternals.h"
	#include "AIList.h"
	#ifdef DEBUG_INTERRUPTS
		#include "debug.h"
	#endif
	#include "render dirty.h"
	#include "GameSettings.h"
	#include "screenids.h"
	#include "vsurface.h"
	#include "math.h"
	#include "merctextbox.h"
	#include "Video.h"
	#include "local.h"
	#include "line.h"
	#include "overhead map.h"
	#include "interface dialogue.h"
#endif

#include "Rain.h"

//forward declarations of common classes to eliminate includes
class OBJECTTYPE;
class SOLDIERTYPE;


// Shop Keeper Interface
#define SKI_X_OFFSET						(((SCREEN_WIDTH - 536) / 2))
#define SKI_Y_OFFSET						((((SCREEN_HEIGHT - 140) - 340) / 2))

// Overhead Map
#define OM_X_OFFSET							0
#define OM_Y_OFFSET							0

#define RAIN_UPDATE_RATE 60
#define PERCENT_OF_DROPS_GOING_TO_THE_EDGE_OF_SCREEN 0.25f

#define DROP_ANGLE_CHANGE_RATE	1.0f

#define MIN_DROP_LENGTH 2.0f
#define ADD_DROP_LENGTH_IF_STORM 2.0f
#define DROP_LENGTH_RANGE 2.0f
#define DROP_LENGTH_CHANGE_RATE 0.1f
#define DROP_LENGTH_RAND 2.0f

// HEADROCK HAM 5 X: Externalized for snow.
FLOAT BASE_DROP_SPEED;
FLOAT DROP_SPEED_RANGE;
FLOAT DROP_SPEED_CHANGE_RATE;
FLOAT DROP_SPEED_RAND;

UINT32 guiMaxRainDrops = 79;

#define RESOLUTION_FACTOR_ON_MAXIMUM_DROPS			((FLOAT) ( SCREEN_WIDTH * SCREEN_HEIGHT ) / ((FLOAT) 640 * 480 ) )
#define BASE_MAXIMUM_DROPS													guiMaxRainDrops * RESOLUTION_FACTOR_ON_MAXIMUM_DROPS

#define DEGREE(a) ( 3.14159 / 180 * a )

typedef struct
{
	BOOLEAN fAlive;

	FLOAT fpX;
	FLOAT fpY;
	FLOAT fpIncrX;
	FLOAT fpIncrY;

	FLOAT fpEndRelX;
	FLOAT fpEndRelY;

	UINT32 uiAmountOfTicksToLive;
}TRainDrop;


static BOOLEAN fFirstTime = TRUE;
UINT32 guiRainRenderSurface = 0;
UINT32 guiLastRainUpdate = 0;
RECT gRainRegion;

TRainDrop *pRainDrops = 0;
UINT32 guiCurrMaxAmountOfRainDrops = 0;
UINT32 guiCurrAmountOfDeadRainDrops = 0;

UINT8 gbCurrentWeather = 0;

FLOAT fpCurrDropAngleOfFalling = 0;
FLOAT fpCurrDropLength = 0;
FLOAT fpCurrDropSpeed = 0;

FLOAT fpMaxDropAngleOfFalling = 0;
FLOAT fpMaxDropLength = 0;
FLOAT fpMaxDropSpeed = 0;

FLOAT fpMinDropAngleOfFalling = 0;
FLOAT fpMinDropLength = 0;
FLOAT fpMinDropSpeed = 0;


extern UINT32 guiCurrentScreen;

extern INT16 gsVIEWPORT_WINDOW_END_Y;
extern INT16 gsVIEWPORT_WINDOW_START_Y;

#define VIDEO_OVERLAYS 100
extern VIDEO_OVERLAY gVideoOverlays[ VIDEO_OVERLAYS ];
extern UINT32 guiNumVideoOverlays;

extern MercPopUpBox *gPopUpTextBox;

BOOLEAN IsItAllowedToRenderRain()
{
	if ( !(gGameExternalOptions.gfAllowRain || gGameExternalOptions.gfAllowSandStorms || gGameExternalOptions.gfAllowSnow) )
		return FALSE;

	if ( !(GetWeatherInCurrentSector( ) == WEATHER_FORECAST_RAIN || GetWeatherInCurrentSector( ) == WEATHER_FORECAST_THUNDERSHOWERS 
		|| GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SANDSTORM || GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SNOW ) )
		return FALSE;

	if( guiCurrentScreen != GAME_SCREEN && guiCurrentScreen != SHOPKEEPER_SCREEN )
		return FALSE;

	return TRUE;
}

void InitializeRainVideoObject( )
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

	AddVideoSurface( &vs_desc, &guiRainRenderSurface );
}

void InitializeRainData()
{
	InitializeRainVideoObject();

	gRainRegion.left = 0;
	gRainRegion.top = 0;
	gRainRegion.right = SCREEN_WIDTH;
	gRainRegion.bottom = SCREEN_HEIGHT - 120;
}

void ResetRain()
{
	if( pRainDrops )
	{
		MemFree( pRainDrops );
		pRainDrops = NULL;
	}

	// Rain
	BASE_DROP_SPEED = 3.5f;
	DROP_SPEED_RANGE = 3.5f;
	DROP_SPEED_CHANGE_RATE = 0.1f;
	DROP_SPEED_RAND = 5.0f;

	guiCurrMaxAmountOfRainDrops = 0;
}

void GenerateRainDropsList()
{
	switch ( gbCurrentWeather )
	{
	case WEATHER_FORECAST_NORMAL:
		guiCurrMaxAmountOfRainDrops = 0;
		break;
		
	case WEATHER_FORECAST_THUNDERSHOWERS:
		guiCurrMaxAmountOfRainDrops = (UINT32)(BASE_MAXIMUM_DROPS)* 2;
		break;

	case WEATHER_FORECAST_SANDSTORM:
		guiCurrMaxAmountOfRainDrops = (UINT32)(BASE_MAXIMUM_DROPS)* 8;
		break;

	case WEATHER_FORECAST_SNOW:
		guiCurrMaxAmountOfRainDrops = (UINT32)(BASE_MAXIMUM_DROPS)* 4;
		break;

	case WEATHER_FORECAST_RAIN:
	default:
		guiCurrMaxAmountOfRainDrops = (UINT32)(BASE_MAXIMUM_DROPS);
		break;
	}

	pRainDrops = (TRainDrop *)MemAlloc( sizeof( TRainDrop ) * guiCurrMaxAmountOfRainDrops );
	memset( pRainDrops, 0, sizeof( TRainDrop ) * guiCurrMaxAmountOfRainDrops );
}

void KillOutOfRegionRainDrops()
{
	for ( UINT32 uiIndex = 0; uiIndex < guiCurrMaxAmountOfRainDrops; ++uiIndex )
	{
		TRainDrop *pCurr = &pRainDrops[ uiIndex ];

		if( ( pCurr->fpX < gRainRegion.left || pCurr->fpX >= gRainRegion.right ||
			pCurr->fpY < gRainRegion.top || pCurr->fpY >= gRainRegion.bottom ) &&
			( pCurr->fpX + pCurr->fpEndRelX < gRainRegion.left || pCurr->fpX + pCurr->fpEndRelX >= gRainRegion.right ||
			pCurr->fpY + pCurr->fpEndRelY < gRainRegion.top || pCurr->fpY + pCurr->fpEndRelY >= gRainRegion.bottom ) )
		{
				pCurr->fAlive = FALSE;
				++guiCurrAmountOfDeadRainDrops;
		}
	}
}

void CreateRainDrops()
{
	UINT32 uiIndex;
	UINT32 uiCreatedDrops = 0;
	FLOAT fpCos, fpSin, fpAbsTg;
	FLOAT fpDropLength = fpCurrDropLength;
	FLOAT fpDropSpeed = fpCurrDropSpeed;
	FLOAT fpNumDropsToXBorder = 0;
	BOOLEAN fLoopIsDone;
	UINT32 uiIndRand;

	fpCos = (FLOAT)(cos( DEGREE(fpCurrDropAngleOfFalling) ));
	fpSin = (FLOAT)(sin( DEGREE(fpCurrDropAngleOfFalling) ));

	if( fpCos )
		fpAbsTg = fabs( fpSin / fpCos ); // take only absolute valuse
	else
		fpAbsTg = 0; // well, it can't really be ;)

	for( uiIndex = 0; uiIndex < guiCurrMaxAmountOfRainDrops; ++uiIndex )
	{
		TRainDrop *pCurr = &pRainDrops[ uiIndex ];

		if( pCurr->fAlive )continue;

		uiIndRand	= (((UINT32)pCurr) / sizeof(TRainDrop) ) % 20;

		fpDropLength = fpCurrDropLength + ( (((UINT32)pCurr) / sizeof(TRainDrop) ) % 7 ) * DROP_LENGTH_RAND / 6;
		fpDropSpeed = fpCurrDropSpeed + ( ( ((UINT32)pCurr) / sizeof(TRainDrop) + 43 ) % 13 ) * DROP_SPEED_RAND / 12;

		pCurr->fAlive = TRUE;

		pCurr->fpIncrX = fpCos * fpDropSpeed;
		pCurr->fpIncrY = fpSin * fpDropSpeed;

		pCurr->fpEndRelX = -fpCos * fpDropLength;
		pCurr->fpEndRelY = -fpSin * fpDropLength;

		// where we want the drops to fall

		if( uiIndex < guiCurrMaxAmountOfRainDrops * PERCENT_OF_DROPS_GOING_TO_THE_EDGE_OF_SCREEN )
		{
			if( !fpCos )
				fpNumDropsToXBorder = 0;
			else
			{
				if( fpAbsTg < 1 )
				{
					fpNumDropsToXBorder = 1.0f - fpAbsTg;
					fpNumDropsToXBorder *= (((FLOAT)SCREEN_HEIGHT - 120) / (FLOAT)SCREEN_WIDTH);
					fpNumDropsToXBorder *= guiCurrMaxAmountOfRainDrops * PERCENT_OF_DROPS_GOING_TO_THE_EDGE_OF_SCREEN / 2;

					fpNumDropsToXBorder += guiCurrMaxAmountOfRainDrops * PERCENT_OF_DROPS_GOING_TO_THE_EDGE_OF_SCREEN / 2;
				}
				else
				{
					fpNumDropsToXBorder = 1 / fpAbsTg;
					fpNumDropsToXBorder *= (((FLOAT)SCREEN_HEIGHT - 120) / (FLOAT)SCREEN_WIDTH);
					fpNumDropsToXBorder *= guiCurrMaxAmountOfRainDrops * PERCENT_OF_DROPS_GOING_TO_THE_EDGE_OF_SCREEN / 2;
				}
			}

			if( uiIndex >= fpNumDropsToXBorder )
			{
				pCurr->fpX = (FLOAT)(gRainRegion.left + Random( gRainRegion.right - gRainRegion.left ));
				pCurr->fpY = (FLOAT)(gRainRegion.bottom - 1);
			}
			else if( fpCos > 0 )
			{
				pCurr->fpX = (FLOAT)(gRainRegion.right - 1);
				pCurr->fpY = (FLOAT)(gRainRegion.top + Random( gRainRegion.bottom - gRainRegion.top ));
			}else{
				pCurr->fpX = 0;
				pCurr->fpY = (FLOAT)(gRainRegion.top + Random( gRainRegion.bottom - gRainRegion.top ));
			}
		}else{
			pCurr->fpX = (FLOAT)(gRainRegion.left + Random( gRainRegion.right - gRainRegion.left ));
			pCurr->fpY = (FLOAT)(gRainRegion.top + Random( gRainRegion.bottom - gRainRegion.top ));
		}

		pCurr->uiAmountOfTicksToLive = 0;
		fLoopIsDone = FALSE;

		while( !fLoopIsDone )
		{
			pCurr->fpX -= pCurr->fpIncrX;
			pCurr->fpY -= pCurr->fpIncrY;
			pCurr->uiAmountOfTicksToLive++;

			if( pCurr->fpX >= gRainRegion.right - 1 )
			{
				pCurr->fpX = gRainRegion.right - 1 - uiIndRand * pCurr->fpIncrX / 20 ;
				fLoopIsDone = TRUE;
			}
			if( pCurr->fpX < gRainRegion.left )
			{
				pCurr->fpX = gRainRegion.left - uiIndRand * pCurr->fpIncrX / 20 ;
				fLoopIsDone = TRUE;
			}
			if( pCurr->fpY >= gRainRegion.bottom - 1 )
			{
				pCurr->fpY = gRainRegion.bottom - 1 - uiIndRand * pCurr->fpIncrX / 20 ;
				fLoopIsDone = TRUE;
			}
			if( pCurr->fpY < gRainRegion.top )
			{
				pCurr->fpY = gRainRegion.top - uiIndRand * pCurr->fpIncrX / 20 ;
				fLoopIsDone = TRUE;
			}
		}

		++uiCreatedDrops;

		// drop creation is done!!!
	}
}

void UpdateRainDrops()
{
	for ( UINT32 uiIndex = 0; uiIndex < guiCurrMaxAmountOfRainDrops; ++uiIndex )
	{
		TRainDrop *pCurr = &pRainDrops[ uiIndex ];

		if( !pCurr->fAlive )
			continue;

		if( pCurr->uiAmountOfTicksToLive )
		{
			pCurr->fpX += pCurr->fpIncrX;
			pCurr->fpY += pCurr->fpIncrY;
			pCurr->uiAmountOfTicksToLive--;
		}
		else
		{
			pCurr->fpEndRelX += pCurr->fpIncrX;
			pCurr->fpEndRelY += pCurr->fpIncrY;

			if( ( pCurr->fpEndRelX > 0 && pCurr->fpIncrX > 0 ) ||
				( pCurr->fpEndRelX < 0 && pCurr->fpIncrX < 0 ) ||
				( pCurr->fpEndRelY > 0 && pCurr->fpIncrY > 0 ) ||
				( pCurr->fpEndRelY < 0 && pCurr->fpIncrY < 0 ) )
			{
				pCurr->fAlive = FALSE;
				++guiCurrAmountOfDeadRainDrops;
			}
		}
	}
}

void BlankRainRenderSurface()
{
	ColorFillVideoSurfaceArea( guiRainRenderSurface, gRainRegion.left, gRainRegion.top, gRainRegion.right, gRainRegion.bottom, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
}

UINT16 GetDropColor()
{
	UINT32 uiRGBPart = 32 + ( 15 - GetTimeOfDayAmbientLightLevel() ) * 8;

	uiRGBPart = max( 0, uiRGBPart );
	uiRGBPart = min( 255, uiRGBPart );

	UINT32 red	 = uiRGBPart;
	UINT32 green = uiRGBPart;
	UINT32 blue	 = uiRGBPart;

	// in a sandstorm, the 'raindrops' are tiny sand particles
	if ( GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SANDSTORM )
	{
		red		= 252;
		green	= 141;
		blue	= 41;
	}
	else if ( GetWeatherInCurrentSector() == WEATHER_FORECAST_SNOW )
	{
		red		= 255;
		green	= 255;
		blue	= 255;
	}

	return Get16BPPColor( FROMRGB( red, green, blue ) );
}

void RenderRainOnSurface()
{
	UINT8 *pDestBuf;
	UINT32 uiDestPitchBYTES;
	UINT32 uiIndex;
	UINT16 sDropsColor = GetDropColor();

	pDestBuf = LockVideoSurface( guiRainRenderSurface, &uiDestPitchBYTES );
	SetClippingRegionAndImageWidth( uiDestPitchBYTES, 0, gsVIEWPORT_WINDOW_START_Y, SCREEN_WIDTH, gsVIEWPORT_WINDOW_END_Y - gsVIEWPORT_WINDOW_START_Y );

	for( uiIndex = 0; uiIndex < guiCurrMaxAmountOfRainDrops; ++uiIndex )
	{
		TRainDrop *pCurr = &pRainDrops[ uiIndex ];

		if( !pCurr->fAlive )continue;

		// Rain
		LineDraw( TRUE, (int)pCurr->fpX, (int)pCurr->fpY, (int)pCurr->fpX + (int)pCurr->fpEndRelX, (int)(pCurr->fpY + pCurr->fpEndRelY),	sDropsColor, pDestBuf );
	}

	UnLockVideoSurface( guiRainRenderSurface );
}

extern UINT32 guiRainLoop;


void GenerateRainMaximums()
{
	if ( GetWeatherInCurrentSector( ) == WEATHER_FORECAST_THUNDERSHOWERS )
	{
		fpMinDropAngleOfFalling = 45;
		fpMaxDropAngleOfFalling = 135;
	}
	else if ( GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SANDSTORM )
	{
		fpMinDropAngleOfFalling = 5;
		fpMaxDropAngleOfFalling = 15;
	}
	else if ( GetWeatherInCurrentSector() == WEATHER_FORECAST_SNOW )
	{
		fpMinDropAngleOfFalling = 80;
		fpMaxDropAngleOfFalling = 100;
	}
	else
	{
		if( Random( 2 ) )
		{	
			fpMinDropAngleOfFalling = 20;
			fpMaxDropAngleOfFalling = 70;
		}
		else
		{
			fpMinDropAngleOfFalling = 110;
			fpMaxDropAngleOfFalling = 160;
		}
	}

	fpCurrDropAngleOfFalling = fpMinDropAngleOfFalling + Random( (UINT32)(fpMaxDropAngleOfFalling - fpMinDropAngleOfFalling) );

	fpMinDropLength = MIN_DROP_LENGTH;
	if ( GetWeatherInCurrentSector( ) == WEATHER_FORECAST_THUNDERSHOWERS )
		fpMinDropLength += ADD_DROP_LENGTH_IF_STORM;
	else if ( GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SANDSTORM || GetWeatherInCurrentSector( ) == WEATHER_FORECAST_SNOW )
		fpMinDropLength = 1.0f;

	fpMaxDropLength = fpMinDropLength + DROP_LENGTH_RANGE;
		
	fpCurrDropLength = fpMinDropLength + Random( (UINT32)(fpMaxDropLength - fpMinDropLength) );

	switch ( gbCurrentWeather )
	{
	case WEATHER_FORECAST_NORMAL:
		fpMinDropSpeed = 0;
		break;

	case WEATHER_FORECAST_THUNDERSHOWERS:
		fpMinDropSpeed = BASE_DROP_SPEED * 4;
		break;

	case WEATHER_FORECAST_SANDSTORM:
		fpMinDropSpeed = BASE_DROP_SPEED * 8;
		break;

	case WEATHER_FORECAST_SNOW:
		fpMinDropSpeed = 1.0f;//BASE_DROP_SPEED;
		break;

	case WEATHER_FORECAST_RAIN:
	default:
		fpMinDropSpeed = BASE_DROP_SPEED * 2;
		break;
	}

	// haxx
	if ( guiRainLoop != NO_SAMPLE )
	{
		SoundStop( guiRainLoop );
		guiRainLoop = NO_SAMPLE;
	}
		
	fpMaxDropSpeed = fpMinDropSpeed + DROP_SPEED_RANGE;

	fpCurrDropSpeed = fpMinDropSpeed + Random( (UINT32)(fpMaxDropSpeed - fpMinDropSpeed) );
}

void UpdateRainDropsProperities()
{
	switch ( gbCurrentWeather )
	{
	case WEATHER_FORECAST_NORMAL:
		fpCurrDropAngleOfFalling = 0;
		break;

	case WEATHER_FORECAST_THUNDERSHOWERS:
		fpCurrDropAngleOfFalling += Random( (UINT32)(1000 * DROP_ANGLE_CHANGE_RATE * 4) ) / 1000.0f - DROP_ANGLE_CHANGE_RATE * 2;
		break;

	case WEATHER_FORECAST_SANDSTORM:
		fpCurrDropAngleOfFalling += Random( (UINT32)(1000 * DROP_ANGLE_CHANGE_RATE * 6) ) / 1000.0f - DROP_ANGLE_CHANGE_RATE * 3;
		break;

	case WEATHER_FORECAST_SNOW:
		fpCurrDropAngleOfFalling += Random( (UINT32)(1000 * DROP_ANGLE_CHANGE_RATE * 1) ) / 1000.0f - DROP_ANGLE_CHANGE_RATE * 1;
		break;

	case WEATHER_FORECAST_RAIN:
	default:
		fpCurrDropAngleOfFalling += Random( (UINT32)(1000 * DROP_ANGLE_CHANGE_RATE * 2) ) / 1000.0f - DROP_ANGLE_CHANGE_RATE;
		break;
	}
	
	fpCurrDropAngleOfFalling = max( fpMinDropAngleOfFalling, fpCurrDropAngleOfFalling );
	fpCurrDropAngleOfFalling = min( fpMaxDropAngleOfFalling, fpCurrDropAngleOfFalling );
	
	fpCurrDropLength += Random( (UINT32)(1000 * DROP_LENGTH_CHANGE_RATE * 2 )) / 1000.0f - DROP_LENGTH_CHANGE_RATE;

	fpCurrDropLength = max( fpMinDropLength, fpCurrDropLength );
	fpCurrDropLength = min( fpMaxDropLength, fpCurrDropLength );
	
	fpCurrDropSpeed += Random( (UINT32)(1000 * DROP_SPEED_CHANGE_RATE * 2 )) / 1000.0f - DROP_SPEED_CHANGE_RATE;

	fpCurrDropSpeed = max( fpMinDropSpeed, fpCurrDropSpeed );
	fpCurrDropSpeed = min( fpMaxDropSpeed, fpCurrDropSpeed );
}

void RandomizeRainDropsPosition()
{
	UINT32 ubMoveTo;

	for ( UINT32 uiIndex = 0; uiIndex < guiCurrMaxAmountOfRainDrops; ++uiIndex )
	{
		TRainDrop *pCurr = &pRainDrops[ uiIndex ];

		if( !pCurr->fAlive )continue;

		if( pCurr->uiAmountOfTicksToLive )
		{
			ubMoveTo = Random( pCurr->uiAmountOfTicksToLive ) + 1;

			while( ubMoveTo < pCurr->uiAmountOfTicksToLive )
			{
				pCurr->fpX += pCurr->fpIncrX;
				pCurr->fpY += pCurr->fpIncrY;
				pCurr->uiAmountOfTicksToLive--;
			}
		}
	}
}

void RainClipVideoOverlay()
{
	BACKGROUND_SAVE *pCurr;

	for ( UINT32 uiIndex = 0; uiIndex < guiNumVideoOverlays; ++uiIndex )
	{
		pCurr = gVideoOverlays[ uiIndex ].pBackground;

		if( pCurr->sLeft < gRainRegion.right ||
			pCurr->sTop < gRainRegion.bottom ||
			pCurr->sRight >= gRainRegion.left ||
			pCurr->sBottom >= gRainRegion.top )
				ColorFillVideoSurfaceArea( guiRainRenderSurface, pCurr->sLeft, pCurr->sTop, pCurr->sRight, pCurr->sBottom, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	}
}

void RenderRain()
{
	if ( !GetWeatherInCurrentSector( ) && gbCurrentWeather )
	{
		gbCurrentWeather = GetWeatherInCurrentSector( );
		ResetRain();
	}

	if( !IsItAllowedToRenderRain() )
		return;

	if( guiCurrentScreen == SHOPKEEPER_SCREEN )
	{
		ColorFillVideoSurfaceArea( guiRainRenderSurface, SKI_X_OFFSET, SKI_Y_OFFSET, 535, 340, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
		return;
	}

	if( InOverheadMap() )
	{
		ColorFillVideoSurfaceArea( guiRainRenderSurface, OM_X_OFFSET, OM_Y_OFFSET, SCREEN_WIDTH, SCREEN_HEIGHT - 120, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
		return;
	}

	if( fFirstTime )
	{
		InitializeRainData();
		fFirstTime = FALSE;
	}

	if( GetJA2Clock() < guiLastRainUpdate )
		guiLastRainUpdate = GetJA2Clock();

	if( GetJA2Clock() < guiLastRainUpdate + 1000 / RAIN_UPDATE_RATE )
		return;
	else
		guiLastRainUpdate = GetJA2Clock();

	if ( gbCurrentWeather != GetWeatherInCurrentSector( ) )
	{
		gbCurrentWeather = GetWeatherInCurrentSector( );
		ResetRain();
		GenerateRainDropsList();
		GenerateRainMaximums();
		
		guiCurrAmountOfDeadRainDrops = guiCurrMaxAmountOfRainDrops;

		CreateRainDrops();
		RandomizeRainDropsPosition();
	}

	guiCurrAmountOfDeadRainDrops = 0;

	UpdateRainDropsProperities();
	UpdateRainDrops();
	KillOutOfRegionRainDrops();
	CreateRainDrops();

	BlankRainRenderSurface();
	RenderRainOnSurface();

	RainClipVideoOverlay();

	if( gfInTalkPanel )
	{
		ColorFillVideoSurfaceArea( guiRainRenderSurface, gTalkPanel.sX, gTalkPanel.sY, gTalkPanel.sX + gTalkPanel.usWidth, gTalkPanel.sY + gTalkPanel.usHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
		if ( gTalkPanel.fRenderSubTitlesNow )
		{
			if( gPopUpTextBox )
				ColorFillVideoSurfaceArea( guiRainRenderSurface, gTalkPanel.sPopupX, gTalkPanel.sPopupY, (INT16)( gTalkPanel.sPopupX + gPopUpTextBox->sWidth ), (INT16)( gTalkPanel.sPopupY + gPopUpTextBox->sHeight ), Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
		}
	}
}
