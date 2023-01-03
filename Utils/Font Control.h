#ifndef __FONT_CONTROL_H
#define __FONT_CONTROL_H

#include "builddefines.h"
#include "font.h"

extern		BOOLEAN	 gfUseWinFonts;
extern		INT32		giCurWinFont;

// ATE: Use this define to enable winfonts in JA2
// #define	 WINFONTS

#define	 GET_WINFONT( )				( giCurWinFont )
#define	 SET_USE_WINFONTS( fSet )		( gfUseWinFonts = fSet );
#define	 SET_WINFONT( fFont )			( giCurWinFont = fFont );

// ATE: A few winfont wrappers..
UINT16	WFGetFontHeight( INT32 FontNum );
INT16	 WFStringPixLength( STR16 string,INT32 UseFont );





// Global variables for video objects
extern INT32						gpLargeFontType1;
extern HVOBJECT				gvoLargeFontType1;

extern INT32						gpSmallFontType1;
extern HVOBJECT				gvoSmallFontType1;

extern INT32						gpTinyFontType1;
extern HVOBJECT				gvoTinyFontType1;

extern INT32						gp12PointFont1;
extern HVOBJECT				gvo12PointFont1;

extern INT32			gpClockFont;
extern HVOBJECT		gvoClockFont;

extern INT32			gpCompFont;
extern HVOBJECT		gvoCompFont;

extern INT32			gpSmallCompFont;
extern HVOBJECT		gvoSmallCompFont;

extern INT32						gp10PointRoman;
extern HVOBJECT				gvo10PointRoman;

extern INT32						gp12PointRoman;
extern HVOBJECT				gvo12PointRoman;

extern INT32						gp14PointSansSerif;
extern HVOBJECT				gvo14PointSansSerif;

//INT32						gpMilitaryFont1;
//HVOBJECT				gvoMilitaryFont1;

extern INT32						gp10PointArial;
extern HVOBJECT				gvo10PointArial;

extern INT32						gp14PointArial;
extern HVOBJECT				gvo14PointArial;

extern INT32						gp12PointArial;
extern HVOBJECT				gvo12PointArial;

extern INT32						gpBlockyFont;
extern HVOBJECT				gvoBlockyFont;

extern INT32						gpBlockyFont2;
extern HVOBJECT				gvoBlockyFont2;

extern INT32						gpBlockyFont3;
extern HVOBJECT				gvoBlockyFont3;

extern INT32						gp10PointArialBold;
extern HVOBJECT				gvo10PointArialBold;

extern INT32						gp12PointArialFixedFont;
extern HVOBJECT				gvo12PointArialFixedFont;

extern INT32						gp16PointArial;
extern HVOBJECT				gvo16PointArial;

extern INT32						gpBlockFontNarrow;
extern HVOBJECT				gvoBlockFontNarrow;

extern INT32						gp14PointHumanist;
extern HVOBJECT				gvo14PointHumanist;

#ifdef JA2EDITOR
extern INT32						gpHugeFont;
extern HVOBJECT				gvoHugeFont;
#endif

//extern INT32			giSubTitleWinFont;


extern BOOLEAN					gfFontsInit;

// Defines
#define					LARGEFONT1				gpLargeFontType1
#define					SMALLFONT1				gpSmallFontType1
#define					TINYFONT1				gpTinyFontType1
#define					FONT12POINT1			gp12PointFont1
//#define					CLOCKFONT			 gpClockFont //remove comment if change ok! "FONTS\\CLOCKFONT.sti" does not exist in Data Folder!
#define					CLOCKFONT			 gpCompFont
#define		 COMPFONT			gpCompFont
#define		 SMALLCOMPFONT	 gpSmallCompFont
#define					FONT10ROMAN				gp10PointRoman
#define					FONT12ROMAN				gp12PointRoman
#define					FONT14SANSERIF		gp14PointSansSerif
#define					MILITARYFONT1			BLOCKFONT		//gpMilitaryFont1
#define					FONT10ARIAL				gp10PointArial
#define					FONT14ARIAL				gp14PointArial
#define		 FONT12ARIAL		gp12PointArial
#define		 FONT10ARIALBOLD	gp10PointArialBold
#define		 BLOCKFONT		 gpBlockyFont
#define		 BLOCKFONT2		gpBlockyFont2
#define		 BLOCKFONT3		gpBlockyFont3
#define					FONT12ARIALFIXEDWIDTH gp12PointArialFixedFont
#define					FONT16ARIAL				gp16PointArial
#define					BLOCKFONTNARROW		gpBlockFontNarrow
#define					FONT14HUMANIST		gp14PointHumanist

#if defined( JA2EDITOR ) && defined( ENGLISH )
	#define				HUGEFONT					gpHugeFont
#else
	#define				HUGEFONT					gp16PointArial
#endif


#define					FONT_SHADE_RED					6
#define					FONT_SHADE_BLUE					1
#define					FONT_SHADE_GREEN				2
#define					FONT_SHADE_YELLOW				3
#define					FONT_SHADE_NEUTRAL			4
#define					FONT_SHADE_WHITE				5

#define					FONT_MCOLOR_BLACK				0
#define					FONT_MCOLOR_WHITE				208
#define					FONT_MCOLOR_DKWHITE			134
#define					FONT_MCOLOR_DKWHITE2		134
#define					FONT_MCOLOR_LTGRAY			134
#define					FONT_MCOLOR_LTGRAY2			134
#define					FONT_MCOLOR_DKGRAY			136
#define					FONT_MCOLOR_LTBLUE			203
#define					FONT_MCOLOR_LTRED				162
#define					FONT_MCOLOR_RED					163
#define					FONT_MCOLOR_DKRED				164
#define					FONT_MCOLOR_LTGREEN			184
#define					FONT_MCOLOR_LTYELLOW		144

//Grayscale font colors
#define	FONT_WHITE				208	//lightest color
#define	FONT_GRAY1				133	
#define	FONT_GRAY2				134	//light gray
#define	FONT_GRAY3				135
#define	FONT_GRAY4				136	//gray
#define	FONT_GRAY5				137	
#define	FONT_GRAY6				138
#define	FONT_GRAY7				139	//dark gray
#define	FONT_GRAY8				140
#define FONT_NEARBLACK		141
#define	FONT_BLACK				0		//darkest color
//Color font colors
#define	FONT_LTRED				162
#define	FONT_RED					163
#define	FONT_DKRED				218
#define FONT_ORANGE				76
#define	FONT_YELLOW				145
#define FONT_DKYELLOW			80
#define FONT_LTGREEN			184
#define FONT_GREEN				185
#define FONT_DKGREEN			186
#define FONT_LTBLUE				71
#define FONT_BLUE					203
#define FONT_DKBLUE				205

#define FONT_BEIGE				130
#define FONT_METALGRAY		94
#define FONT_BURGUNDY			172
#define FONT_LTKHAKI			88
#define FONT_KHAKI				198
#define FONT_DKKHAKI			201

BOOLEAN	InitializeFonts( );
void ShutdownFonts( );

BOOLEAN SetFontShade( UINT32 uiFontID, INT8 bColorID );

#endif
