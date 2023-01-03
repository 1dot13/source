#ifndef __ANIMATED_PROGRESSBAR_H
#define __ANIMATED_PROGRESSBAR_H

#include "types.h"

#define MAX_PROGRESSBARS 4

typedef struct PROGRESSBAR
{
	UINT8 ubProgressBarID;
	UINT16 usBarLeft, usBarTop, usBarRight, usBarBottom;
	BOOLEAN fPanel;
	UINT16 usPanelLeft, usPanelTop, usPanelRight, usPanelBottom;
	UINT16 usColor, usLtColor, usDkColor;
	STR16 swzTitle;
	UINT16 usTitleFont;
	UINT8 ubTitleFontForeColor, ubTitleFontShadowColor;
	UINT16 usMsgFont;
	UINT8 ubMsgFontForeColor, ubMsgFontShadowColor;
	UINT8 ubRelativeStartPercentage, ubRelativeEndPercentage;
	UINT8 ubColorFillRed;
	UINT8 ubColorFillGreen;
	UINT8 ubColorFillBlue;
	double rStart, rEnd;
	BOOLEAN	fDisplayText;
	BOOLEAN	fUseSaveBuffer;	//use the save buffer when display the text
	double rLastActual;
	UINT32 uiFrameBuffer;
	BOOLEAN fDrawBorder;
}PROGRESSBAR;

extern PROGRESSBAR *pBar[ MAX_PROGRESSBARS ];

// Flugente: a structure for clothing items
typedef struct
{
	UINT16			uiIndex;
	CHAR16			szName[190];			// text of the hint. Note that this size + any text it is used in must be < than CHAR16	pzText[ 200 ]; of VIDEO_OVERLAY_DESC if you display this in any logs
	
	UINT32			usFlags;				// flags that can be used to determine wether a hint is currently appropriate
	BOOLEAN			fAlreadyShown;			// to remember if this hint has already been shown in this game (reset on starting exe)
} LOADSCREENHINT_STRUCT;

#define LOADSCREENHINT_MAX	1000

extern LOADSCREENHINT_STRUCT zLoadScreenHint[LOADSCREENHINT_MAX];

// Flugente: this function selects the next hint to display, and makes sure it is not played again during this run of the exe
void SetNewLoadScreenHint();
void ResetLoadScreenHint();
void ShowLoadScreenHintInLoadScreen(UINT16 bottomPosition);
void ShowLoadScreenHintInStrategicLog();
void ShowLoadScreenHintInTacticalLog();

// -------- added by Flugente: various flags for loadscreen hints--------
// easier than adding 32 differently named variables. DO NOT CHANGE THEM, UNLESS YOU KNOW WHAT YOU ARE DOING!!!
#define LOADSCREEN_LORE					0x00000001	//1			// ingame history and background
#define LOADSCREEN_WEAPON				0x00000002	//2			// tips on weapons
#define LOADSCREEN_ITEM					0x00000004	//4			// general tip on items
#define LOADSCREEN_KEYBIND				0x00000008	//8			// tip on new key commands

#define LOADSCREEN_FOOD					0x00000010	//16		// food tips
#define LOADSCREEN_ZOMBIE				0x00000020	//32		// zombie tips
#define LOADSCREEN_OVERHEAT				0x00000040	//64		// tips on overheating
#define LOADSCREEN_NCTH					0x00000080	//128		// tips on using NCTH

#define LOADSCREEN_COVERTOPS			0x00000100	//256		// tips on using the covert ops trait
//#define SOLDIER_HEADSHOT			0x00000200	//512		// last hit received was a headshot (attack to the head, so knifes/punches also work)
//#define SOLDIER_POW					0x00000400	//1024		// we are a prisoner of war
//#define SOLDIER_ASSASSIN			0x00000800	//2048		// we are an enemy assassin, and thus we will behave very different from normal enemies

//#define SOLDIER_POW_PRISON			0x00001000	//4096		// this guy is a prisoner of war in a prison sector. SOLDIER_POW refers to people we capture, this refers to people we hold captive
//#define SOLDIER_EQUIPMENT_DROPPED	0x00002000	//8192		// under certain circumstances, militia can be ordered to drop their gear twice. Thus we set a marker to avoid that
//#define ENEMY_NET_3_LVL_4 		0x00004000	//16384
/*#define WH40K_SOLDIER_BLANK								0x00008000	//32768			// we are a blank, the ultimate weapon against psykers and daemons

#define WH40K_SOLDIER_IDENTIFIED_BLANK					0x00010000	//65536			// we are a blank, and the player has learned this (used for laptop display)
#define WH40K_SOLDIER_DEMON_DAMMALUS_ACTIVE				0x00020000	//131072		// demon disability: damage malus is active
#define WH40K_SOLDIER_DEMON_LINKEDBULLETS				0x00040000	//262144		// demon dual wield: our first bullet hit, thus we get a huge bonus to the second bullet
#define WH40K_SOLDIER_OBLITERATOR						0x00080000	//524288		// soldier has the obliterator virus

#define WH40K_SOLDIER_ADEPTA_PRAYER_DAMAGE				0x00100000	//1048576
#define WH40K_SOLDIER_ADEPTA_PRAYER_CTH					0x00200000	//2097152
#define WH40K_SOLDIER_ADEPTA_PRAYER_SUPPRESSION			0x00400000	//4194304
#define WH40K_SOLDIER_ADEPTA_PRAYER_ENERGYREGEN			0x00800000	//8388608

#define WH40K_SOLDIER_TIMEBUBBLEEFFECT					0x01000000	//16777216
#define WH40K_SOLDIER_VOX_OPERATOR_LISTENING			0x02000000	//33554432
#define WH40K_SOLDIER_VOX_OPERATOR_JAMMING				0x04000000	//67108864
#define WH40K_SOLDIER_VOX_OPERATOR_SCANNING				0x08000000	//134217728

#define WH40K_SOLDIER_COMMISSAR_MILITIA_KILL_BONUS		0x10000000	//268435456		// guardsmen under our command killed a heretic. This grants a bonus to our next hit with one-handed weapons
#define WH40K_SOLDIER_ADEPTA_PRAYER_REDUCEDAMAGE		0x20000000	//536870912		// reduce damage taken by 2
#define WH40K_SOLDIER_ILLUSION							0x40000000	//1073741824	// Soldier is an Illusion
#define WH40K_SOLDIER_KILLSTREAK						0x80000000	//2147483648	// Soldier is on a kill streak*/

void CreateLoadingScreenProgressBar(BOOLEAN resetLoadScreenHint);
void RemoveLoadingScreenProgressBar();


//This creates a single progress bar given the coordinates without a panel (containing a title and background).
//A panel is automatically created if you specify a title using SetProgressBarTitle
BOOLEAN CreateProgressBar( UINT8 ubProgressBarID, UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom );

//You may also define a panel to go in behind the progress bar.	You can now assign a title to go with
//the panel.
void DefineProgressBarPanel( UINT32 ubID, UINT8 r, UINT8 g, UINT8 b,
														UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom );

//Assigning a title for the panel will automatically position the text horizontally centered on the
//panel and vertically centered from the top of the panel, to the top of the progress bar.
void SetProgressBarTitle( UINT32 ubID, STR16 pString, UINT32 usFont, UINT8 ubForeColor, UINT8 ubShadowColor );

//Unless you set up the attributes, any text you pass to SetRelativeStartAndEndPercentage will
//default to FONT12POINT1 in a black color.
void SetProgressBarMsgAttributes( UINT32 ubID, UINT32 usFont, UINT8 ubForeColor, UINT8 ubShadowColor );


//When finished, the progress bar needs to be removed.
void RemoveProgressBar( UINT8 ubID );

//An important setup function.	The best explanation is through example.	The example being the loading
//of a file -- there are many stages of the map loading.	In JA2, the first step is to load the tileset.
//Because it is a large chunk of the total loading of the map, we may gauge that it takes up 30% of the
//total load.	Because it is also at the beginning, we would pass in the arguments ( 0, 30, "text" ).
//As the process animates using UpdateProgressBar( 0 to 100 ), the total progress bar will only reach 30%
//at the 100% mark within UpdateProgressBar.	At that time, you would go onto the next step, resetting the
//relative start and end percentage from 30 to whatever, until your done.
void SetRelativeStartAndEndPercentage( UINT8 ubID, UINT16 uiRelStartPerc, UINT16 uiRelEndPerc, STR16 str);

//This part renders the progress bar at the percentage level that you specify.	If you have set relative
//percentage values in the above function, then the uiPercentage will be reflected based off of the relative
//percentages.	
void RenderProgressBar( UINT8 ubID, UINT32 uiPercentage );


//Sets the color of the progress bars main color.
void SetProgressBarColor( UINT8 ubID, UINT8 ubColorFillRed, UINT8 ubColorFillGreen, UINT8 ubColorFillBlue );

//Pass in TRUE to display the strings.
void SetProgressBarTextDisplayFlag( UINT8 ubID, BOOLEAN fDisplayText, BOOLEAN fUseSaveBuffer, BOOLEAN fSaveScreenToFrameBuffer );

// OJW - 20090422
// draw the progress bar with a white border, or not
void SetProgressBarUseBorder( UINT32 ubID , BOOLEAN bBorder );
// specify which buffer to render the bar to
void SetProgressBarRenderBuffer( UINT32 ubID , UINT32 uiBufferID );
// create a color bar with no border
void CreateProgressBarNoBorder( UINT8 ubProgressBarID, UINT16 usLeft, UINT16 usTop, UINT16 usRight, UINT16 usBottom );


#endif