// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("CHINESE")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( CHINESE )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_ChineseText_public_symbol(void){;}

#if defined( CHINESE )

/*
 
******************************************************************************************************
**                                  IMPORTANT TRANSLATION NOTES                                     **
******************************************************************************************************

GENERAL INSTRUCTIONS
- Always be aware that foreign strings should be of equal or shorter length than the English equivalent.  
	I know that this is difficult to do on many occasions due to the nature of foreign languages when 
	compared to English.  By doing so, this will greatly reduce the amount of work on both sides.  In 
	most cases (but not all), JA2 interfaces were designed with just enough space to fit the English word.  
	The general rule is if the string is very short (less than 10 characters), then it's short because of 
	interface limitations.  On the other hand, full sentences commonly have little limitations for length.  
	Strings in between are a little dicey.
- Never translate a string to appear on multiple lines.  All strings L"This is a really long string...", 
	must fit on a single line no matter how long the string is.  All strings start with L" and end with ",
- Never remove any extra spaces in strings.  In addition, all strings containing multiple sentences only 
	have one space after a period, which is different than standard typing convention.  Never modify sections 
	of strings contain combinations of % characters.  These are special format characters and are always 
	used in conjunction with other characters.  For example, %s means string, and is commonly used for names,
	locations, items, etc.  %d is used for numbers.  %c%d is a character and a number (such as A9).  
	%% is how a single % character is built.  There are countless types, but strings containing these 
	special characters are usually commented to explain what they mean.  If it isn't commented, then 
	if you can't figure out the context, then feel free to ask SirTech.
- Comments are always started with // Anything following these two characters on the same line are 
	considered to be comments.  Do not translate comments.  Comments are always applied to the following 
	string(s) on the next line(s), unless the comment is on the same line as a string.  
- All new comments made by SirTech will use "//@@@ comment" (without the quotes) notation.  By searching 
	for @@@ everytime you recieve a new version, it will simplify your task and identify special instructions.
  Commonly, these types of comments will be used to ask you to abbreviate a string.  Please leave the 
	comments intact, and SirTech will remove them once the translation for that particular area is resolved.
- If you have a problem or question with translating certain strings, please use "//!!! comment" 
	(without the quotes).  The syntax is important, and should be identical to the comments used with @@@ 
	symbols.  SirTech will search for !!! to look for your problems and questions.  This is a more 
	efficient method than detailing questions in email, so try to do this whenever possible.


	
FAST HELP TEXT -- Explains how the syntax of fast help text works.
**************

1) BOLDED LETTERS
	The popup help text system supports special characters to specify the hot key(s) for a button.  
	Anytime you see a '|' symbol within the help text string, that means the following key is assigned
	to activate the action which is usually a button.  

	EX:  L"|Map Screen" 

	This means the 'M' is the hotkey.  In the game, when somebody hits the 'M' key, it activates that
	button.  When translating the text to another language, it is best to attempt to choose a word that
	uses 'M'.  If you can't always find a match, then the best thing to do is append the 'M' at the end
	of the string in this format:

	EX:  L"Ecran De Carte (|M)"  (this is the French translation)

	Other examples are used multiple times, like the Esc key  or "|E|s|c" or Space -> (|S|p|a|c|e)

2) NEWLINE
  Any place you see a \n within the string, you are looking at another string that is part of the fast help
	text system.  \n notation doesn't need to be precisely placed within that string, but whereever you wish 
	to start a new line. 

	EX:  L"Clears all the mercs' positions,\nand allows you to re-enter them manually." 

	Would appear as:

				Clears all the mercs' positions,
				and allows you to re-enter them manually.

	NOTE:  It is important that you don't pad the characters adjacent to the \n with spaces.  If we did this
	       in the above example, we would see

	WRONG WAY -- spaces before and after the \n
	EX:  L"Clears all the mercs' positions, \n and allows you to re-enter them manually." 

	Would appear as: (the second line is moved in a character)
		
				Clears all the mercs' positions,
 				 and allows you to re-enter them manually.


@@@ NOTATION
************

	Throughout the text files, you'll find an assortment of comments.  Comments are used to describe the
	text to make translation easier, but comments don't need to be translated.  A good thing is to search for
	"@@@" after receiving new version of the text file, and address the special notes in this manner.  

!!! NOTATION
************

	As described above, the "!!!" notation should be used by you to ask questions and address problems as
	SirTech uses the "@@@" notation.

*/

// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Delete",
};

STR16 FaceDirs[8] = 
{ 
	L"north",
	L"northeast",
	L"east",
	L"southeast",
	L"south",
	L"southwest",
	L"west",
	L"northwest"
};

STR16 iEditorMercsToolbarText[] = 
{
 L"Toggle viewing of players", //0
 L"Toggle viewing of enemies",
 L"Toggle viewing of creatures",
 L"Toggle viewing of rebels",
 L"Toggle viewing of civilians",
 
 L"Player",
 L"Enemy",
 L"Creature",
 L"Rebels",
 L"Civilian",
 
 L"DETAILED PLACEMENT", //10
 L"General information mode",
 L"Physical appearance mode",
 L"Attributes mode",
 L"Inventory mode",
 L"Profile ID mode",
 L"Schedule mode",
 L"Schedule mode",
 L"DELETE",
 L"Delete currently selected merc (DEL).",
 L"NEXT", //20
 L"Find next merc (SPACE).",
 L"Toggle priority existance",
 L"Toggle whether or not placement has/naccess to all doors.",
 
 //Orders
 L"STATIONARY",
 L"ON GUARD",
 L"ON CALL",
 L"SEEK ENEMY",
 L"CLOSE PATROL",
 L"FAR PATROL",
 L"POINT PATROL", //30
 L"RND PT PATROL",
 
 //Attitudes
 L"DEFENSIVE",
 L"BRAVE SOLO",
 L"BRAVE AID",
 L"AGGRESSIVE",
 L"CUNNING SOLO",
 L"CUNNING AID",
 
 L"Set merc to face %s",
 
 L"Find",
 L"BAD", //40
 L"POOR",
 L"AVERAGE",
 L"GOOD",
 L"GREAT",
 
 L"BAD",
 L"POOR",
 L"AVERAGE",
 L"GOOD",
 L"GREAT",
 
 L"Previous color set", //50
 L"Next color set",
 
 L"Previous body type",
 L"Next body type",
 
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 L"Toggle time variance (+ or - 15 minutes)",
 
 L"No action",
 L"No action",
 L"No action", //60
 L"No action",
 
 L"Clear Schedule",
 
 L"Find selected merc",
};

STR16 iEditorBuildingsToolbarText[] =
{
	L"ROOFS",  //0
	L"WALLS",
	L"ROOM INFO",

	L"Place walls using selection method",
	L"Place doors using selection method",
	L"Place roofs using selection method",
	L"Place windows using selection method",
	L"Place damaged walls using selection method.",
	L"Place furniture using selection method",
	L"Place wall decals using selection method",
	L"Place floors using selection method", //10
	L"Place generic furniture using selection method",
	L"Place walls using smart method",
	L"Place doors using smart method",
	L"Place windows using smart method",
	L"Place damaged walls using smart method",
	L"Lock or trap existing doors",

	L"Add a new room",
	L"Edit cave walls.",
	L"Remove an area from existing building.",
	L"Remove a building", //20
	L"Add/replace building's roof with new flat roof.",
	L"Copy a building",
	L"Move a building",
	L"Draw room number",
	L"Erase room numbers",

	L"Toggle erase mode",
	L"Undo last change",
	L"Cycle brush size",

};

STR16 iEditorItemsToolbarText[] =
{
	L"Weapons", //0
	L"Ammo",
	L"Armour",
	L"LBE",
	L"Exp",
	L"E1",
	L"E2",
	L"E3",
	L"Triggers",
	L"Keys",
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Add ambient light source", //0
	L"Toggle fake ambient lights.",
	L"Add exit grids (r-clk to query existing).",
	L"Cycle brush size",
	L"Undo last change",
	L"Toggle erase mode",
	L"Specify north point for validation purposes.",
	L"Specify west point for validation purposes.",
	L"Specify east point for validation purposes.",
	L"Specify south point for validation purposes.",
	L"Specify center point for validation purposes.", //10
	L"Specify isolated point for validation purposes.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"New map",  //0
	L"New basement",
	L"New cave level",
	L"Save map",
	L"Load map",
	L"Select tileset",
	L"Leave Editor mode",
	L"Exit game.",
	L"Create radar map",
	L"When checked, the map will be saved in original JA2 map format.\nThis option is only valid on 'normal' size maps that do not reference grid numbers (e.g: exit grids) > 25600.",
	L"When checked and you load a map, the map will be enlarged automatically depending on the selected Rows and Cols.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Draw ground textures", //0
	L"Set map ground textures",
	L"Place banks and cliffs",
	L"Draw roads",
	L"Draw debris",
	L"Place trees & bushes",
	L"Place rocks",
	L"Place barrels & other junk",
	L"Fill area",
	L"Undo last change",
	L"Toggle erase mode", //10
	L"Cycle brush size",
	L"Raise brush density",
	L"Lower brush density",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Terrain", //0
	L"Buildings",
	L"Items",
	L"Mercs",
	L"Map Info",
	L"Options",
};

//Editor Taskbar Utils.cpp

STR16 iRenderMapEntryPointsAndLightsText[] =
{
	L"North Entry Point", //0
	L"West Entry Point",
	L"East Entry Point",
	L"South Entry Point",
	L"Center Entry Point",
	L"Isolated Entry Point",
	
	L"Prime",
	L"Night",
	L"24Hour",
};

STR16 iBuildTriggerNameText[] =
{
	L"Panic Trigger1", //0
	L"Panic Trigger2",
	L"Panic Trigger3",
	L"Trigger%d",
	 
	L"Pressure Action",
	L"Panic Action1",
	L"Panic Action2",
	L"Panic Action3",
	L"Action%d",
};

STR16 iRenderDoorLockInfoText[]=
{
	L"No Lock ID", //0
	L"Explosion Trap",
	L"Electric Trap",
	L"Siren Trap",
	L"Silent Alarm",
	L"Super Electric Trap", //5
	L"Brothel Siren Trap",
	L"Trap Level %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Save map in vanilla JA2 (v1.12) map format (Version: 5.00 / 25)", //0
	L"No map currently loaded.",
	L"File:  %S, Current Tileset:  %s",
	L"Enlarge map on loading",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"TOGGLE", //0
	L"VIEWS",
	L"SELECTION METHOD",
	L"SMART METHOD",
	L"BUILDING METHOD",
	L"Room#", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Editing lock attributes at map index %d.",
	L"Lock ID",
	L"Trap Type",
	L"Trap Level",
	L"Locked",
};

//EditorItems.cpp

STR16 pInitEditorItemsInfoText[] = 
{
	L"Pressure Action", //0
	L"Panic Action1",
	L"Panic Action2",
	L"Panic Action3",
	L"Action%d",
	
	L"Panic Trigger1", //5
	L"Panic Trigger2",
	L"Panic Trigger3",
	L"Trigger%d",
};

STR16 pDisplayItemStatisticsTex[] =
{
	L"Status Info Line 1",
	L"Status Info Line 2",
	L"Status Info Line 3",
	L"Status Info Line 4",
	L"Status Info Line 5",
};

//EditorMapInfo.cpp
STR16 pUpdateMapInfoText[] =
{
	L"R", //0
	L"G",
	L"B",
	
	L"Prime",
	L"Night",
	L"24Hrs", //5

	L"Radius",

	L"Underground",
	L"Light Level",

	L"Outdoors",
	L"Basement", //10
	L"Caves",

	L"Restricted",
	L"Scroll ID",

	L"Destination",
	L"Sector", //15
	L"Destination",
	L"Bsmt. Level",
	L"Dest.",
	L"GridNo",
};
//EditorMercs.cpp
CHAR16 gszScheduleActions[ 11 ][20] =
{
	L"No action",
	L"Lock door",
	L"Unlock door",
	L"Open door",
	L"Close door",
	L"Move to gridno",
	L"Leave sector",
	L"Enter sector",
	L"Stay in sector",
	L"Sleep",
	L"Ignore this!"
};

STR16 zDiffNames[5] =	// NUM_DIFF_LVLS = 5
{ 
	L"Wimp", 
	L"Easy", 
	L"Average",
	L"Tough", 
	L"Steroid Users Only" 
};

STR16 EditMercStat[12] = 
{ 
	L"Max Health",
	L"Cur Health",
	L"Strength",
	L"Agility",
	L"Dexterity",
	L"Charisma",
	L"Wisdom",
	L"Marksmanship",
	L"Explosives",
	L"Medical",
	L"Scientific",
	L"Exp Level", 
};


STR16 EditMercOrders[8] = 
{ 
	L"Stationary",
	L"On Guard",
	L"Close Patrol",
	L"Far Patrol",
	L"Point Patrol",
	L"On Call",
	L"Seek Enemy", 
	L"Random Point Patrol",
};

STR16 EditMercAttitudes[6] = 
{ 
	L"Defensive",
	L"Brave Loner",
	L"Brave Buddy",
	L"Cunning Loner",
	L"Cunning Buddy",
	L"Aggressive", 
};

STR16 pDisplayEditMercWindowText[] =
{
	L"Merc Name:", //0
	L"Orders:",
	L"Combat Attitude:",
};

STR16 pCreateEditMercWindowText[] = 
{
	L"Merc Colors", //0
	L"Done",
	
	L"Previous merc standing orders",
	L"Next merc standing orders", 
	
	L"Previous merc combat attitude",
	L"Next merc combat attitude",	//5
	
	L"Decrease merc stat",
	L"Increase merc stat",
};

STR16 pDisplayBodyTypeInfoText[] =
{
	L"Random", //0
	L"Reg Male",
	L"Big Male",
	L"Stocky Male",
	L"Reg Female",
	L"NE Tank", //5
	L"NW Tank",
	L"Fat Civilian",
	L"M Civilian",
	L"Miniskirt",
	L"F Civilian", //10
	L"Kid w/ Hat",
	L"Humvee",
	L"Eldorado",
	L"Icecream Truck",
	L"Jeep", //15
	L"Kid Civilian",
	L"Domestic Cow",
	L"Cripple",
	L"Unarmed Robot",
	L"Larvae", //20
	L"Infant",
	L"Yng F Monster",
	L"Yng M Monster",
	L"Adt F Monster",
	L"Adt M Monster", //25
	L"Queen Monster",
	L"Bloodcat",
};

STR16 pUpdateMercsInfoText[] = 
{
	L" --=ORDERS=-- ", //0
	L"--=ATTITUDE=--",
	
	L"RELATIVE",
	L"ATTRIBUTES",
	
	L"RELATIVE",
	L"EQUIPMENT",
			
	L"RELATIVE",
	L"ATTRIBUTES",
	
	L"Army",
	L"Admin",
	L"Elite", //10
	
	L"Exp. Level",
	L"Life",
	L"LifeMax",
	L"Marksmanship",
	L"Strength",
	L"Agility",
	L"Dexterity",
	L"Wisdom",
	L"Leadership",
	L"Explosives", //20
	L"Medical",
	L"Mechanical",
	L"Morale",
	
	L"Hair color:",
	L"Skin color:",
	L"Vest color:",
	L"Pant color:",
	
	L"RANDOM",
	L"RANDOM",
	L"RANDOM", //30
	L"RANDOM",
	
	L"By specifying a profile index, all of the information will be extracted from the profile ",
	L"and override any values that you have edited.  It will also disable the editing features ",
	L"though, you will still be able to view stats, etc.  Pressing ENTER will automatically ",
	L"extract the number you have typed.  A blank field will clear the profile.  The current ",
	L"number of profiles range from 0 to ",
	
	L"Current Profile:  n/a              ",
	L"Current Profile: %s",
	
	L"STATIONARY",
	L"ON CALL", //40
	L"ON GUARD",
	L"SEEK ENEMY",
	L"CLOSE PATROL",
	L"FAR PATROL",
	L"POINT PATROL",
	L"RND PT PATROL",

	L"Action",
	L"Time",
	L"V",
	L"GridNo 1", //50
	L"GridNo 2",
	L"1)",
	L"2)",
	L"3)",
	L"4)",
	
	L"lock",
	L"unlock",
	L"open",
	L"close",
	
	L"Click on the gridno adjacent to the door that you wish to %s.", //60
	L"Click on the gridno where you wish to move after you %s the door.",
	L"Click on the gridno where you wish to move to.",
	L"Click on the gridno where you wish to sleep at.	Person will automatically return to original position after waking up.",
	L" Hit ESC to abort entering this line in the schedule.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
	L"Patrol orders with no waypoints",
	L"Waypoints with no patrol orders",
};

STR16 pClearCurrentScheduleText[] =
{
	L"No action",
};

STR16 pCopyMercPlacementText[] =
{
	L"Placement not copied because no placement selected.",
	L"Placement copied.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Placement not pasted as no placement is saved in buffer.",
	L"Placement pasted.",
	L"Placement not pasted as the maximum number of placements for this team is already used.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Exit editor?",
};

STR16 pHandleKeyboardShortcutsText[] = 
{
	L"Are you sure you wish to remove all lights?", //0
	L"Are you sure you wish to reverse the schedules?",
	L"Are you sure you wish to clear all of the schedules?",
	
	L"Clicked Placement Enabled",
	L"Clicked Placement Disabled",
	
	L"Draw High Ground Enabled", //5
	L"Draw High Ground Disabled",
	
	L"Number of edge points: N=%d E=%d S=%d W=%d",
	
	L"Random Placement Enabled",
	L"Random Placement Disabled",
	
	L"Removing Treetops", //10
	L"Showing Treetops",
	
	L"World Raise Reset",
	
	L"World Raise Set Old",
	L"World Raise Set",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Creating radar map for %S", //0
	
	L"Delete current map and start a new basement level?",
	L"Delete current map and start a new cave level?",
	L"Delete current map and start a new outdoor level?",
	
	L" Wipe out ground textures? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"HOME", //0
	L"Toggle fake editor lighting ON/OFF",

	L"INSERT",
	L"Toggle fill mode ON/OFF",

	L"BKSPC",
	L"Undo last change",

	L"DEL",
	L"Quick erase object under mouse cursor",

	L"ESC",
	L"Exit editor",

	L"PGUP/PGDN", //10
	L"Change object to be pasted",

	L"F1",
	L"This help screen",

	L"F10",
	L"Save current map",

	L"F11",
	L"Load map as current",

	L"+/-",
	L"Change shadow darkness by .01",

	L"SHFT +/-",  //20
	L"Change shadow darkness by .05",

	L"0 - 9",
	L"Change map/tileset filename",

	L"b",
	L"Change brush size",

	L"d",
	L"Draw debris",

	L"o",
	L"Draw obstacle",

	L"r", //30
	L"Draw rocks",

	L"t",
	L"Toggle trees display ON/OFF",

	L"g",
	L"Draw ground textures",

	L"w",
	L"Draw building walls",

	L"e",
	L"Toggle erase mode ON/OFF",

	L"h",  //40
	L"Toggle roofs ON/OFF",
};

STR16 pAutoLoadMapText[] =
{
	L"Map data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
	L"Schedule data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
};

STR16 pShowHighGroundText[] =
{
	L"Showing High Ground Markers",
	L"Hiding High Ground Markers",
};

//Item Statistics.cpp
CHAR16 gszActionItemDesc[ 34 ][ 30 ] =
{
	L"Klaxon Mine",
	L"Flare Mine",
	L"Teargas Explosion",
	L"Stun Explosion",
	L"Smoke Explosion",
	L"Mustard Gas",
	L"Land Mine",
	L"Open Door",
	L"Close Door",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Small Explosion",
	L"Medium Explosion",
	L"Large Explosion",
	L"Toggle Door",
	L"Toggle Action1s",
	L"Toggle Action2s",
	L"Toggle Action3s",
	L"Toggle Action4s",
	L"Enter Brothel",
	L"Exit Brothel",
	L"Kingpin Alarm",
	L"Sex with Prostitute",
	L"Reveal Room",
	L"Local Alarm",
	L"Global Alarm",
	L"Klaxon Sound",
	L"Unlock door",
	L"Toggle lock",
	L"Untrap door",
	L"Tog pressure items",
	L"Museum alarm",
	L"Bloodcat alarm",
	L"Big teargas",
};

STR16 pUpdateItemStatsPanelText[] =
{
	L"Toggle hide flag", //0
	L"No item selected.",
	L"Slot available for",
	L"random generation.",
	L"Keys not editable.",
	L"ProfileID of owner",
	L"Item class not implemented.",
	L"Slot locked as empty.",
	L"Status",
	L"Rounds",
	L"Trap Level", //10
	L"Quantity",
	L"Trap Level",
	L"Status",
	L"Trap Level",
	L"Status",
	L"Quantity",
	L"Trap Level",
	L"Dollars",
	L"Status",
	L"Trap Level", //20
	L"Trap Level",
	L"Tolerance",
	L"Alarm Trigger",
	L"Exist Chance",
	L"B",
	L"R",
	L"S",
};

STR16 pSetupGameTypeFlagsText[] =
{
	L"Item appears in both Sci-Fi and Realistic modes. (|B)", //0
	L"Item appears in |Realistic mode only.",
	L"Item appears in |Sci-Fi mode only.",
};

STR16 pSetupGunGUIText[] =
{
	L"SILENCER", //0
	L"SNIPERSCOPE",
	L"LASERSCOPE",
	L"BIPOD",
	L"DUCKBILL",
	L"G-LAUNCHER", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"CERAMIC PLATES", //0
};

STR16 pSetupExplosivesGUIText[] =
{
	L"DETONATOR",
};

STR16 pSetupTriggersGUIText[] =
{
	L"If the panic trigger is an alarm trigger,\nenemies won't attempt to use it if they\nare already aware of your presence.",
};

//Sector Summary.cpp

STR16 pCreateSummaryWindowText[]=
{
	L"Okay", //0
	L"A",
	L"G",
	L"B1",
	L"B2",
	L"B3", //5
	L"LOAD",
	L"SAVE",
	L"Update",
};

STR16 pRenderSectorInformationText[] =
{
	L"Tileset:  %s", //0
	L"Version Info:  Summary:  1.%02d,  Map:  %1.2f / %02d",
	L"Number of items:  %d", 
	L"Number of lights:  %d",
	L"Number of entry points:  %d",
	
	L"N",
	L"E",
	L"S",
	L"W",
	L"C",
	L"I", //10
	
	L"Number of rooms:  %d",
	L"Total map population:  %d",
	L"Enemies:  %d",
	L"Admins:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Troops:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Elites:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Civilians:  %d",  //20
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	
	L"Humans:  %d",
	L"Cows:  %d",
	L"Bloodcats:  %d",
	
	L"Creatures:  %d",
	
	L"Monsters:  %d",
	L"Bloodcats:  %d",
	
	L"Number of locked and/or trapped doors:  %d",
	L"Locked:  %d",
	L"Trapped:  %d", //30
	L"Locked & Trapped:  %d",
	
	L"Civilians with schedules:  %d",
	
	L"Too many exit grid destinations (more than 4)...",
	L"ExitGrids:  %d (%d with a long distance destination)",
	L"ExitGrids:  none",
	L"ExitGrids:  1 destination using %d exitgrids",
	L"ExitGrids:  2 -- 1) Qty: %d, 2) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d, 4) Qty: %d",
	L"Enemy Relative Attributes:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)", //40
	L"Enemy Relative Equipment:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)",
	L"%d placements have patrol orders without any waypoints defined.",
	L"%d placements have waypoints, but without any patrol orders.",
	L"%d gridnos have questionable room numbers.  Please validate.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Enemy",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"Panic1",
	L"Panic2",
	L"Panic3",
	L"Norm1",
	L"Norm2",
	L"Norm3",
	L"Norm4", //10
	L"Pressure Actions",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"PRIORITY ENEMY DROPPED ITEMS",
	L"None",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	L"NORMAL ENEMY DROPPED ITEMS",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"None",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"ERROR:  Can't load the items for this map.  Reason unknown.", //20	
};

STR16 pRenderSummaryWindowText[] =
{
	L"CAMPAIGN EDITOR -- %s Version 1.%02d", //0
	L"(NO MAP LOADED).",
	L"You currently have %d outdated maps.",
	L"The more maps that need to be updated, the longer it takes.  It'll take ",
	L"approximately 4 minutes on a P200MMX to analyse 100 maps, so",
	L"depending on your computer, it may vary.",
	L"Do you wish to regenerate info for ALL these maps at this time (y/n)?",
	
	L"There is no sector currently selected.",
	
	L"Entering a temp file name that doesn't follow campaign editor conventions...",
	
	L"You need to either load an existing map or create a new map before being",
	L"able to enter the editor, or you can quit (ESC or Alt+x).", //10

	L", ground level",					
	L", underground level 1",	
	L", underground level 2",	
	L", underground level 3",	
	L", alternate G level",					
	L", alternate B1 level",	
	L", alternate B2 level",	
	L", alternate B3 level",
	
	L"ITEM DETAILS -- sector %s",
	L"Summary Information for sector %s:",//20
	
	L"Summary Information for sector %s",
	L"does not exist.",
	
	L"Summary Information for sector %s",
	L"does not exist.",
	
	L"No information exists for sector %s.",
	
	L"No information exists for sector %s.",
	
	L"FILE:  %s",
	
	L"FILE:  %s",
	
	L"Override READONLY",
	L"Overwrite File", //30
	
	L"You currently have no summary data.  By creating one, you will be able to keep track",
	L"of information pertaining to all of the sectors you edit and save.  The creation process",
	L"will analyse all maps in your \\MAPS directory, and generate a new one.  This could",
	L"take a few minutes depending on how many valid maps you have.  Valid maps are",
	L"maps following the proper naming convention from a1.dat - p16.dat.  Underground maps", 
	L"are signified by appending _b1 to _b3 before the .dat (ex:  a9_b1.dat). ",
	
	L"Do you wish to do this now (y/n)?",
	
	L"No summary info.  Creation denied.",
	
	L"Grid",
	L"Progress", //40
	L"Use Alternate Maps",
	
	L"Summary",
	L"Items",
};

STR16 pUpdateSectorSummaryText[] =
{
	L"Analyzing map:  %s...",
};

STR16 pSummaryLoadMapCallbackText[] =
{
	L"Loading map:  %s",
};

STR16 pReportErrorText[] =
{
	L"Skipping update for %s.  Probably due to tileset conflicts...",
};

STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Generating map information",
};

STR16 pSummaryUpdateCallbackText[] =
{
	L"Generating map summary",
};

STR16 pApologizeOverrideAndForceUpdateEverythingText[] =
{
	L"MAJOR VERSION UPDATE",
	L"There are %d maps requiring a major version update.",
	L"Updating all outdated maps",
};

//selectwin.cpp
STR16 pDisplaySelectionWindowGraphicalInformationText[] =
{
	L"%S[%d] is from default tileset %s (%S)",
	L"File:  %S, subindex:  %d (%S)",
	L"Current Tileset:  %s",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Small", 
 L"Medium", 
 L"Large", 
 L"XLarge", 
 L"Width: xx", 
 L"Area" 
 };

//---

CHAR16  gszAimPages[ 6 ][ 20 ] =
{
	L"页 1/2", //0
	L"页 2/2",
	
	L"页 1/3",
	L"页 2/3",
	L"页 3/3",
	
	L"Page 1/1", //5
};

// by Jazz:
CHAR16 zGrod[][500] =
{
	L"机器人", //0    // Robot
};

STR16 pCreditsJA2113[] =
{
	L"@T,{;JA2 v1.13 Development Team",
	L"@T,C144,R134,{;Coding",
	L"@T,C144,R134,{;Graphics and Sounds",
	L"@};(Various other mods!)",
	L"@T,C144,R134,{;Items",
	L"@T,C144,R134,{;Other Contributors",
	L"@};(All other community members who contributed input and feedback!)",
};

CHAR16 ItemNames[MAXITEMS][80] =
{
	L"",
};


CHAR16 ShortItemNames[MAXITEMS][80] =
{
	L"",
};

// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
CHAR16 AmmoCaliber[MAXITEMS][20];// =
//{
//	L"0",
//	L".38 cal",
//	L"9mm",
//	L".45 cal",
//	L".357 cal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm NATO",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rocket",
//	L"", // dart
//	L"", // flame
//	L".50 cal", // barrett
//	L"9mm Hvy", // Val silent
//};

// This BobbyRayAmmoCaliber is virtually the same as AmmoCaliber however the bobby version doesnt have as much room for the words.
// 
// Different weapon calibres
// CAWS is Close Assault Weapon System and should probably be left as it is
// NATO is the North Atlantic Treaty Organization
// WP is Warsaw Pact
// cal is an abbreviation for calibre
CHAR16 BobbyRayAmmoCaliber[MAXITEMS][20] ;//=
//{
//	L"0",
//	L".38 cal",
//	L"9mm",
//	L".45 cal",
//	L".357 cal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm N.",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rocket",
//	L"dart", // dart
//	L"", // flamethrower
//	L".50 cal", // barrett
//	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"其他",
	L"手枪",
	L"自动手枪",
	L"冲锋枪",
	L"步枪",
	L"狙击步枪",
	L"突击步枪",
	L"轻机枪",
	L"霰弹枪",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"玩家的回合",
	L"敌军的回合",
	L"异形的回合",
	L"民兵的回合",
	L"平民的回合",
	L"玩家部署",
	L"#1 客户端",
	L"#2 客户端",
	L"#3 客户端",
	L"#4 客户端",

};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s被射中了头部，并且失去了1点智慧！",
	L"%s被射中了肩部，并且失去了1点灵巧！",
	L"%s被射中了胸膛，并且失去了1点力量！",
	L"%s被射中了腿部，并且失去了1点敏捷！",
	L"%s被射中了头部，并且失去了%d点智慧！",
	L"%s被射中了肩部，并且失去了%d点灵巧！",
	L"%s被射中了胸膛，并且失去了%d点力量！",
	L"%s被射中了腿部，并且失去了%d点敏捷！",
	L"中断！",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"你的援军到达了！",

	// In the following four lines, all %s's are merc names

	L"%s装填弹药。",
	L"%s没有足够的行动点数。",
	L"%s正在进行包扎。(按任意键取消)",
	L"%s和%s正在进行包扎。(按任意键取消)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"耐用",
	L"不耐用",
	L"容易修复",
	L"不易修复",
	L"杀伤力高",
	L"杀伤力低",
	L"射击快",
	L"射击慢",
	L"射程远",
	L"射程近",
	L"轻盈",
	L"笨重",
	L"小巧",
	L"射速高",
	L"没有点射限制",
	L"大容弹量",
	L"小容弹量",

	// In the following two lines, all %s's are merc names

	L"%s的伪装失效了。",
	L"%s的伪装被洗掉了。",

	// The first %s is a merc name and the second %s is an item name

	L"副手武器没有弹药了！",//	L"Second weapon is out of ammo!",
	L"%s偷到了%s。",		//	L"%s has stolen the %s.",

	// The %s is a merc name

	L"%s的武器不能扫射。",	//	L"%s's weapon can't burst fire.",

	L"你已经装上了该附件。",//	L"You've already got one of those attached.",
	L"组合物品？",			//	L"Merge items?",

	// Both %s's are item names

	L"你不能把%s和%s组合在一起。",

	L"无",
	L"退出子弹",
	L"附件",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"你不能同时使用%s和%s。",

	L"请把光标选中的物品放到另一物品的任意附件格中，这样就可能合成新物品。",
	L"请把光标选中的物品放到另一物品的任意附件格中，这样就可能合成新物品。(但是这一次，该物品不相容。)",
	L"该分区的敌军尚未被肃清",
	L"你还得给%s%s",
	L"%s被射中了头部。",
	L"放弃战斗？",
	L"这个组合是永久性的。你确认要这样做吗?",
	L"%s感觉精力充沛!",
	L"%s踩到了大理石珠子，滑倒了!",
	L"%s没能从敌人手里抢到%s!",
	L"%s修复了%s。",
	L"中断: ",
	L"投降？",
	L"此人拒绝你的包扎。",
	L"我不这么认为。",
  L"要搭乘Skyrider的直升飞机, 你得先把佣兵分配到交通工具/直升飞机。",
	L"%s的时间只够给一支枪装填弹药",
	L"血猫的回合",
	L"全自动",
	L"无全自动",
	L"精确",
	L"不精确",
	L"无半自动",
	L"敌人已经没有装备可偷了！",
	L"敌人手中没有装备！",

	L"%s 的沙漠迷彩油已经耗竭失效了.",
	L"%s 的沙漠迷彩油已经冲刷失效了.",
	
	L"%s 的丛林迷彩油已经耗竭失效了.",
	L"%s 的丛林迷彩油已经冲刷失效了.",
	
	L"%s 的城市伪装迷彩油已经耗竭失效了.",
	L"%s 的城市伪装迷彩油已经冲刷失效了.",
	
	L"%s 的雪地迷彩油已经耗竭失效了.",
	L"%s 的雪地迷彩油已经冲刷失效了.",

	L"你不能将其附加在当前功能槽.",
	L"%s 不适合打开的任何功能槽.",
};


// the names of the towns in the game

CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT] =
{
	L"",
	L"Omerta",
	L"Drassen",
	L"Alma",
	L"Grumm",
	L"Tixa",
	L"Cambria",
	L"San Mona",
	L"Estoni",
	L"Orta",
	L"Balime",
	L"Meduna",
	L"Chitzena",
};

// the types of time compression. For example: is the timer paused? at normal speed, 5 minutes per second, etc.
// min is an abbreviation for minutes

STR16 sTimeStrings[] =
{
	L"暂停",
	L"普通",
	L"5分钟",
	L"30分钟",
	L"60分钟",
	L"6小时",
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",// on active duty
	L"医生",// administering medical aid
	L"病人", // getting medical aid
	L"交通工具", // in a vehicle
	L"在途中",// in transit - abbreviated form
	L"修理", // repairing
	L"锻炼", // training themselves
  L"民兵", // training a town to revolt
  L"游击队", //L"M.Militia", //training moving militia units //ham3.6
	L"教练", // training a teammate
	L"学员", // being trained by someone else
	L"工作", // L"Staff", // operating a strategic facility //ham3.6
	L"休息", //L"Rest",// Resting at a facility //ham3.6
	L"死亡", // dead
	L"无力中", // abbreviation for incapacitated
	L"战俘", // Prisoner of war - captured
	L"医院", // patient in a hospital
	L"空",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"民兵", // the title of the militia box
	L"未分配的民兵", //the number of unassigned militia troops
	L"本地区有敌军存在，你无法重新分配民兵！",
	L"一些民兵未分派到防区，要不要把他们解散？", // L"Some militia were not assigned to a sector. Would you like to disband them?", // HEADROCK HAM 3.6
};


STR16 pMilitiaButtonString[] =
{
	L"自动", // auto place the militia troops for the player
	L"完成", // done placing militia troops
	L"解散", // L"Disband", // HEADROCK HAM 3.6: Disband militia
};

STR16 pConditionStrings[] =
{
	L"极好", //the state of a soldier .. excellent health
	L"良好", // good health
	L"普通", // fair health
	L"受伤", // wounded health
	L"疲劳", // tired
	L"失血", // bleeding to death
	L"昏迷", // knocked out
	L"垂死", // near death
	L"死亡", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"编队", // set merc on active duty
	L"病人",// set as a patient to receive medical aid
	L"交通工具", // tell merc to enter vehicle
	L"无护送", // let the escorted character go off on their own
	L"取消", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"在途中",
	L"修理",
	L"锻炼",
	L"训练民兵",
	L"训练游击队",	// TODO.Translate
	L"教练",
	L"学员",
	L"设施职员",			// TODO.Translate
	L"Resting at Facility",		// TODO.Translate
	L"休息",
	L"无力中",
	L"战俘",
	L"医院",
	L"空",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"在途中",
	L"修理",
	L"练习",
	L"训练民兵",
	L"训练游击队", //L"Train Mobiles",		// TODO.Translate // done by kkkk@tbs 10-07-21
	L"训练队友",
	L"学员",
	L"设施职员",  //L"Staff Facility",		// TODO.Translate // done by kkkk@tbs 10-07-21 
	L"休息",  //L"Rest at Facility",	// TODO.Translate // done by kkkk@tbs 10-07-21
	L"死亡",
	L"无力中",
	L"战俘",
	L"医院",// patient in a hospital
	L"空", // Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"合同选项: ",
	L"", // a blank line, required
	L"雇佣一日",// offer merc a one day contract extension
	L"雇佣一周", // 1 week
	L"雇佣两周", // 2 week
	L"解雇",// end merc's contract
	L"取消", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"囚禁",  //an acronym for Prisoner of War
	L" ± ± ",
};

STR16 pLongAttributeStrings[] =
{
	L"力量",
	L"灵巧",
	L"敏捷",
	L"智慧",
	L"枪法",
	L"医疗",
	L"机械",
	L"领导",
	L"爆破",
	L"级别",
};

STR16 pInvPanelTitleStrings[] =
{
	L"护甲", // the armor rating of the merc
	L"负重", // the weight the merc is carrying
	L"伪装", // the merc's camouflage rating
	L"伪装",
	L"防护",
};

STR16 pShortAttributeStrings[] =
{
	L"敏捷", // the abbreviated version of : agility
	L"灵巧", // dexterity
	L"力量", // strength
	L"领导", // leadership
	L"智慧", // wisdom
	L"级别", // experience level
	L"枪法", // marksmanship skill
	L"爆破", // explosive skill
	L"机械", // mechanical skill
	L"医疗", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"任务", // the mercs current assignment
	L"合同",// the contract info about the merc
	L"生命", // the health level of the current merc
	L"士气", // the morale of the current merc
	L"状态",	// the condition of the current vehicle
	L"油量",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"锻炼", // tell merc to train self
  L"民兵",// tell merc to train town
	L"教练", // tell merc to act as trainer
	L"学员", // tell merc to be train by other
};

STR16 pGuardMenuStrings[] =
{
	L"射速: ", // the allowable rate of fire for a merc who is guarding
	L" 进攻式射击", // the merc can be aggressive in their choice of fire rates
	L" 节约弹药", // conserve ammo
	L" 有保留的射击", // fire only when the merc needs to
	L"其它选择: ", // other options available to merc
	L" 可以撤退", // merc can retreat
	L" 可以寻找掩体",  // merc is allowed to seek cover
	L" 可以帮助队友", // merc can assist teammates
	L"完成", // done with this menu
	L"取消", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"射速: ",
	L" *进攻式射击*",
	L" *节约弹药o*",
	L" *有保留的射击*",
	L"其它选择: ",
	L" *可以撤退*",
	L" *可以看见覆盖物*",
	L" *可以帮助队友*",
	L"完成",
	L"取消",
};

STR16 pAssignMenuStrings[] =
{
	L"编队",
	L"医生",
	L"病人",
	L"交通工具",
	L"修理",
	L"训练",
	L"设施", // L"Facility", // the merc is using/staffing a facility //ham3.6
	L"取消",
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"进攻", // set militia to aggresive
	L"原地坚守", // set militia to stationary
	L"撤退", // retreat militia
	L"向我靠拢", // retreat militia
	L"卧倒", // retreat militia
	L"隐蔽",
	L"全体: 进攻",
	L"全体: 原地坚守",
	L"全体: 撤退",
	L"全体: 向我靠拢",
	L"全体: 分散",
	L"全体: 卧倒",
	L"全体: 隐蔽",
	//L"All: Find items",
	L"取消", // cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"Attack", // set militia to aggresive
//	L"Hold Position", // set militia to stationary
//	L"Retreat", // retreat militia
//	L"Come to me", // retreat militia
//	L"Get down", // retreat militia		 
//	L"Cancel", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"移除佣兵", // remove dead merc from current team
	L"取消"
};

STR16 pAttributeMenuStrings[] =
{
	L"力量",	//"Strength",
	L"灵巧",	//"Dexterity",
	L"敏捷",	//"Agility",
	L"生命",	//"Health",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"机械",	//"Mechanical",
	L"领导",	//"Leadership",
	L"爆破",	//"Explosives",
	L"取消",	//"Cancel",
};

STR16 pTrainingMenuStrings[] =
{
	L"锻炼", // train yourself
	L"民兵", // train the town
	L"游击队", // L"Mobile Militia", //ham3.6
	L"教练", // train your teammates
	L"学员", // be trained by an instructor
	L"取消", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"第1小队",
	L"第2小队",
	L"第3小队",
	L"第4小队",
	L"第5小队",
	L"第6小队",
	L"第7小队",
	L"第8小队",
	L"第9小队",
	L"第10小队",
	L"第11小队",
	L"第12小队",
	L"第13小队",
	L"第14小队",
	L"第15小队",
	L"第16小队",
	L"第17小队",
	L"第18小队",
	L"第19小队",
	L"第20小队",
	L"取消",
};

STR16 pPersonnelTitle[] =
{
	L"人事", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"生命: ", // health of merc
	L"敏捷: ",
	L"灵巧: ",
	L"力量: ",
	L"领导: ",
	L"智慧: ",
	L"级别: ", // experience level
	L"枪法: ",
	L"机械: ",
	L"爆破: ",
	L"医疗: ",
	L"医疗保证金: ", // amount of medical deposit put down on the merc
	L"合同剩余时间: ", // cost of current contract
	L"击毙数: ", // number of kills by merc
	L"击伤数: ",// number of assists on kills by merc
	L"日薪: ", // daily cost of merc
	L"花费总数: ",// total cost of merc
	L"合同花费: ",
	L"总日数: ",// total service rendered by merc
	L"欠付佣金: ",// amount left on MERC merc to be paid
	L"命中率: ",// percentage of shots that hit target
	L"战斗次数: ", // number of battles fought
	L"受伤次数: ", // number of times merc has been wounded
	L"技能: ",
	L"没有技能",
	L"成就:", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"精兵: %d\n",
	L"杂兵: %d\n",
	L"头目: %d\n",
	L"刁民: %d\n",
	L"动物: %d\n",
	L"坦克: %d\n",
	L"其他: %d\n",

	L"帮助佣兵: %d\n",
	L"帮助民兵: %d\n",
	L"帮助其他: %d\n",

	L"枪弹射击: %d\n",
	L"火箭发射: %d\n",
	L"榴弹投掷: %d\n",
	L"飞刀投掷: %d\n",
	L"白刃砍杀: %d\n",
	L"徒手攻击: %d\n",
	L"有效攻击总数: %d\n",

	L"工具撬锁: %d\n",
	L"暴力开锁: %d\n",
	L"排除陷阱: %d\n",
	L"拆除炸弹: %d\n",
	L"修理物品: %d\n",
	L"合成物品: %d\n",
	L"偷窃物品: %d\n",
	L"训练民兵: %d\n",
	L"战地急救: %d\n",
	L"外科手术: %d\n",
	L"遇见人物: %d\n",
	L"探索区域: %d\n",
	L"避免伏击: %d\n",
	L"游戏任务: %d\n",

	L"参加战斗: %d\n",
	L"自动战斗: %d\n",
	L"撤退战斗: %d\n",
	L"偷袭次数: %d\n",
	L"参加过最多有: %d 名敌军的战斗\n",

	L"中枪: %d\n",
	L"刀砍: %d\n",
	L"拳打: %d\n",
	L"爆炸: %d\n",
	L"设施伤害: %d\n",
	L"经历手术: %d\n",
	L"设施事故: %d\n",

	L"性格:",
	L"弱点:",
	L"态度:",	// WANNE: For old traits display instead of "Character:"!
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	// SANDRO - tweaked this
	L"没有技能",
	L"开锁",
	L"徒手格斗",		//JA25: modified
	L"电子",
	L"夜战",			//JA25: modified
	L"投掷",
	L"教学",
	L"重武器",
	L"自动武器",
	L"潜行",
	L"双手武器",
	L"偷窃",
	L"武术",
	L"刀技",
	L"狙击手",
	L"伪装",					//JA25: modified
	L"专家",
};
//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"没有技能",
	L"自动武器",
	L"重武器",
	L"射手",
	L"猎手",
	L"枪手",
	L"格斗家",
	L"谈判专家",
	L"技师",
	L"救护兵",
	// Minor traits
	L"搏击术",
	L"近战",
	L"投掷",
	L"夜战",
	L"尾行",
	L"体能训练",
	L"肌肉男",
	L"爆破",
	L"教学",
	L"侦察",
	// second names for major skills
	L"机枪手",
	L"投弹手",
	L"狙击手",
	L"游侠",
	L"大镖客",
	L"武术家",
	L"小队长",
	L"工程师",
	L"医生",
	L"更多...",
};
//////////////////////////////////////////////////////////

// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"站立/行走 (|S)",
	L"蹲伏/蹲伏前进(|C)",
	L"站立/奔跑 (|R)",
	L"匍匐/匍匐前进(|P)",
	L"查看(|L)",
	L"行动",
	L"交谈",
	L"检查 (|C|t|r|l)",

	// Pop up door menu
	L"用手开门",
	L"检查陷阱",
	L"开锁",
	L"踢门",
	L"解除陷阱",
	L"关门",
	L"开门",
	L"使用破门炸药",
	L"使用撬棍",
	L"取消 (|E|s|c)",
	L"关闭",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"没有陷阱",
	L"一个爆炸陷阱",
	L"一个带电陷阱",
	L"一个警报陷阱",
	L"一个无声警报陷阱",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"日",
	L"周",
	L"两周",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"选择人物",
	L"分配任务",
	L"安排行军路线",
	L"签约 (|C)",
	L"移除佣兵",
	L"睡觉",
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"微弱的",
	L"清晰的",
	L"大声的",
	L"非常大声的",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"未知",
	L"移动声",
	L"辗扎声",
	L"溅泼声",
	L"撞击声",
	L"枪声",
	L"爆炸声",
	L"尖叫声",
	L"撞击声",
	L"撞击声",
	L"粉碎声",
	L"破碎声",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"东北方",
	L"东方",
	L"东南方",
	L"南方",
	L"西南方",
	L"西方",
	L"西北方",
	L"北方"
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"城市",
	L"公路",
	L"平原",
	L"沙漠",
	L"灌木",
	L"森林",
	L"沼泽",
	L"湖泊",
	L"山地",
	L"不可通行",
	L"河流",	//river from north to south
	L"河流",	//river from east to west
	L"外国",
	//NONE of the following are used for directional travel, just for the sector description.
	L"热带",
	L"农田",
	L"平原，公路",
	L"灌木，公路",
	L"农庄，公路",
	L"热带，公路",
	L"森林，公路",
	L"海滨",
	L"山地，公路",
	L"海滨，公路",
	L"沙漠，公路",
	L"沼泽，公路",
	L"灌木，SAM导弹基地",
	L"沙漠，SAM导弹基地",
	L"热带，SAM导弹基地",
	L"Meduna, SAM导弹基地",

	//These are descriptions for special sectors
	L"Cambria医院",
	L"Drassen机场",
	L"Meduna机场",
	L"SAM导弹基地",
	L"抵抗军隐蔽处",//The rebel base underground in sector A10
	L"Tixa地牢",//The basement of the Tixa Prison (J9)
	L"异形巢穴",//Any mine sector with creatures in it
	L"Orta地下室",	//The basement of Orta (K4)
	L"地道",		//The tunnel access from the maze garden in Meduna
										//leading to the secret shelter underneath the palace
	L"地下掩体",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s在%c%d分区被发现了，另一小队即将到达。",	//STR_DETECTED_SINGULAR
	L"%s在%c%d分区被发现了，其它几个小队即将到达。",	//STR_DETECTED_PLURAL
	L"你想调整为同时到达吗？",												//STR_COORDINATE

	//Dialog strings for enemies.

	L"敌军给你一个投降的机会。",
	L"敌军俘虏了昏迷中的佣兵。",

	//The text that goes on the autoresolve buttons							

	L"撤退",		//The retreat button
	L"完成",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"防守",							//STR_AR_DEFEND_HEADER
	L"攻击",						//STR_AR_ATTACK_HEADER
	L"遭遇战",						//STR_AR_ENCOUNTER_HEADER
	L"分区",	//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions

	L"胜利！",								//STR_AR_OVER_VICTORY
	L"失败！",								//STR_AR_OVER_DEFEAT
	L"投降！",							//STR_AR_OVER_SURRENDERED
	L"被俘！",								//STR_AR_OVER_CAPTURED
	L"撤退！",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"民兵",							//STR_AR_MILITIA_NAME,
	L"精兵",							//STR_AR_ELITE_NAME,
	L"部队",							//STR_AR_TROOP_NAME,
	L"行政人员",								//STR_AR_ADMINISTRATOR_NAME,
	L"异形",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"战斗用时",						//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"已撤退",						//STR_AR_MERC_RETREATED,
	L"正在撤退",						//STR_AR_MERC_RETREATING,
	L"撤退",						//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"自动战斗",							//STR_PB_AUTORESOLVE_BTN,
	L"进入战区",							//STR_PB_GOTOSECTOR_BTN,
	L"撤退佣兵",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"遭遇敌军",						//STR_PB_ENEMYENCOUNTER_HEADER,
	L"敌军入侵",						//STR_PB_ENEMYINVASION_HEADER, // 30
	L"敌军伏击",
	L"进入敌占区",			//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"异形攻击",					//STR_PB_CREATUREATTACK_HEADER
	L"血猫伏击",					//STR_PB_BLOODCATAMBUSH_HEADER
	L"进入血猫巢穴",

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"地区",
	L"敌军",
	L"佣兵",
	L"民兵",
	L"异形",
	L"血猫",
	L"分区",
	L"无人",	//If there are no uninvolved mercs in this fight.
	L"N/A",			//Acronym of Not Applicable
	L"日",			//One letter abbreviation of day
	L"小时",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"清除",
	L"分散",
	L"集中",
	L"完成",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"清除所有佣兵的布阵，然后你可以\n重新手动对他们进行安排。(|c)",
	L"每次按本按钮，就会重新 \n随机分散地布阵佣兵。(|s)",
	L"你可以选择你想集中地布阵佣兵的地方。(|g)",
	L"当你完成对佣兵布阵后，\n请按本按钮。 (|E|n|t|e|r)",
	L"开始战斗前，你必须\n对所有佣兵完成布阵。",

	//Various strings (translate word for word)

	L"分区",
	L"选择进入的位置",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"看起来不太好。无法进入这里。换个不同的位置吧。",
	L"请把佣兵放在地图的高亮分区里。",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"已到达该地区",

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"自动解决战斗，不需要\n载入该分区地图。",
	L"当玩家在攻击时，无法使用\n自动战斗功能。",
	L"进入该分区和敌军作战(|E)",
	L"将小队撤退到先前的分区。(|R)",				//singular version
	L"将所有小队撤退到先前的分区。(|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"敌军向你在%c%d分区的民兵发起了攻击。",
	//%c%d is the sector -- ex:  A9
	L"异形向你在%c%d分区的民兵发起了攻击。",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"异形攻击了分区%s，吃掉了%d名平民。",
	//%s is the sector location -- ex:  A9: Omerta
	L"敌军向你在分区%s的佣兵发起了攻击。你的佣兵中没人能进行战斗。",
	//%s is the sector location -- ex:  A9: Omerta
	L"异形向你在分区%s的佣兵发起了攻击。你的佣兵中没人能进行战斗。",

};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"日",
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"找到钥匙的分区: ",
	L"找到钥匙的日期: ",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 17 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"状态: ",
	L"重量: ",
	L"AP 消耗",
	L"射程: ",		// Range
	L"杀伤力: ",		// Damage
	L"弹药", 	// Number of bullets left in a magazine
	L"AP: ",			// abbreviation for Action Points
	L"=",
	L"=",
					//Lal: additional strings for tooltips
	L"准确性: ", //9
	L"射程: ", //10
	L"杀伤力: ", //11
	L"重量: ", //12
	L"晕眩杀伤力: ",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	// HEADROCK HAM 3: Replaced #14 with string for Possible Attachments (BR's Tooltips Only)
	// Obviously, THIS SHOULD BE DONE IN ALL LANGUAGES...
	L"附件:",	//14 //ham3.6
	L"连发/5AP: ",		//15
	L"剩余弹药:",		//16	// TODO.Translate
	L"默认:",	//17 //WarmSteel - So we can also display default attachments

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
// Please note, several of these are artificially inflated to 19 entries to help fix a complication with
// changing item type while watching its description box
STR16		gzWeaponStatsFasthelp[ 32 ] =
{
	L"精度",
	L"杀伤",
	L"射程",
	L"精瞄等级",
	L"精瞄修正",
	L"平均最佳激光瞄准距离",
	L"消焰",
	L"噪音（越低越好）",
	L"可靠性",
	L"修理难度",
	L"精瞄加成所需最小距离",
	L"命中修正",
	L"",	//12
	L"举枪AP",
	L"单发AP",
	L"点射AP",
	L"连发AP",
	L"上弹AP",
	L"手动上弹AP",
	L"",	//19
	L"两角架修正",
	L"连发数量/5AP",
	L"点射/自动惩罚（越低越好）",	//22
	L"投掷AP",	//20
	L"发射AP",
	L"捅人AP",
	L"无法单发！",
	L"无点射模式！",
	L"无连发模式！",
	L"格斗AP",
	L"连发惩罚（越低越好）",
    L"点射惩罚（越低越好）",
};

STR16		gzWeaponStatsFasthelpTactical[ 32 ] =
{
	L"精度",
	L"杀伤",
	L"射程",
	L"精瞄等级",
	L"精瞄修正",
	L"平均最佳激光瞄准距离",
	L"消焰",
	L"噪音（越低越好）",
	L"可靠性",
	L"修理难度",
	L"精瞄加成所需最小距离",
	L"命中修正",
	L"",	//12
	L"举枪AP",
	L"单发AP",
	L"点射AP",
	L"连发AP",
	L"上弹AP",
	L"手动上弹AP",
	L"",	//19
	L"两脚架修正",
	L"连发数量/5AP",
	L"点射/自动惩罚（越低越好）",	//22
	L"投掷AP",	//20
	L"发射AP",
	L"捅人AP",
	L"无法单发！",
	L"无点射模式！",
	L"无连发模式！",
	L"格斗AP",
	L"连发惩罚（越低越好）",
    L"点射惩罚（越低越好）",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
{
	L"物品大小修正（越低越好）", // 0
	L"可靠性修正",
	L"噪音修正（越低越好）",
	L"枪口消焰",
	L"脚架修正",
	L"射程修正", // 5
	L"命中率修正",
	L"最佳激光瞄准距离",
	L"精瞄加成修正",
	L"点射长度修正",
	L"点射惩罚修正（越高越好）", // 10
	L"连发惩罚修正（越高越好）",
	L"AP修正",
	L"点射AP修正（越低越好）",
	L"连发AP修正（越低越好）",
	L"举枪AP修正（越低越好）", // 15
	L"上弹AP修正（越低越好）",
	L"弹容量修正",
	L"攻击AP修正（越低越好）",
	L"杀伤修正",
	L"近战杀伤修正", // 20
	L"丛林迷彩",
	L"城市迷彩",
	L"沙漠迷彩",
	L"雪地迷彩",
	L"潜行修正", // 25
	L"听觉距离修正",
	L"视距修正",
	L"白天视距修正",
	L"夜晚视距修正",
	L"亮光下视距修正", //30
	L"洞穴视距修正",
	L"隧道视野百分比（越低越好）",
	L"精瞄加成所需最小距离",
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"处于 ",
	L"完美",
	L"优秀",
	L"好的",
	L"尚可",
	L"不良",
	L"差的",
	L"糟糕的",
	L" 状态."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 13 ] =
{
	L"剩余",
	L"金额: ",//this is the overall balance
	L"分割",
	L"金额: ", // the amount he wants to separate from the overall balance to get two piles of money

	L"当前",
	L"余额:",
	L"提取",
	L"金额:",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"垂死",	//"DYING",		//	>= 0
	L"濒危",	//"CRITICAL", 		//	>= 15
	L"虚弱",	//"POOR",		//	>= 30
	L"受伤",	//"WOUNDED",    	//	>= 45
	L"健康",	//"HEALTHY",    	//	>= 60
	L"强壮",	//"STRONG",     	// 	>= 75
  L"极好",	//"EXCELLENT",		// 	>= 90
};

STR16	gzMoneyAmounts[6] = 
{ 
	L"$1000",
	L"$100",
	L"$10",
	L"完成",
	L"分割",
	L"提取",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons." 
CHAR16		gzProsLabel[10] = 
{
	L"优点: ",
};

CHAR16		gzConsLabel[10] = 
{
	L"缺点: ",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"再说一次？",	//meaning "Repeat yourself"
	L"友好",		//approach in a friendly
	L"直率",		//approach directly - let's get down to business
	L"恐吓",		//approach threateningly - talk now, or I'll blow your face off
	L"给予",
	L"招募",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"买/卖",
	L"买",
	L"卖",
	L"修理",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"完成",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"凯迪拉克",
 L"悍马",	// a hummer jeep/truck -- military vehicle
 L"冰激凌车",
 L"吉普",
 L"坦克",
 L"直升飞机",
};

STR16 pShortVehicleStrings[] =
{
	L"凯迪拉克",
	L"悍马",				// the HMVV
	L"冰激凌车",
	L"吉普",
	L"坦克",
	L"直升飞机", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"凯迪拉克",
	L"悍马",			//a military jeep. This is a brand name.
	L"冰激凌车",			// Ice cream truck
	L"吉普",
	L"坦克",
	L"直升飞机", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"空袭",
	L"自动包扎?",

	// CAMFIELD NUKE THIS and add quote #66.

	L"%s发现运来的货品短缺了几件。",

	// The %s is a string from pDoorTrapStrings

	L"锁上有%s。",
	L"没有上锁。",
	L"成功！",
	L"失败。",
	L"成功！",
	L"失败",
	L"锁上没有被设置陷阱。",
	L"成功！",
	// The %s is a merc name
	L"%s没有对应的钥匙。",
	L"锁上的陷阱被解除了。",
	L"锁上没有被设置陷阱。",
	L"锁住了。",
	L"门",
	L"有陷阱的",
	L"锁住的",
	L"没锁的",
	L"被打烂的",
	L"这里有一个开关。启动它吗？",
	L"解除陷阱？",
	L"上一个...",
	L"下一个...",
	L"更多的...",

	// In the next 2 strings, %s is an item name

	L"%s被放在地上了。",
	L"%s被交给%s了。",

	// In the next 2 strings, %s is a name

	L"%s已经被完全支付。",
	L"%s还拖欠%d。",
	L"选择引爆的频率", //in this case, frequency refers to a radio signal
	L"设定几个回合后爆炸: ", //how much time, in turns, until the bomb blows
	L"设定遥控雷管的频率: ",//in this case, frequency refers to a radio signal
	L"解除诡雷?",
	L"移掉蓝旗？",
	L"在这里插上蓝旗吗？",
	L"结束回合",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"你确定要攻击%s吗？",
	L"车辆无法变动姿势。",
	L"机器人无法变动姿势。",

	// In the next 3 strings, %s is a name

	L"%s无法在这里变为该姿势。",
	L"%s无法在这里被包扎。",
	L"%s不需要包扎。",
	L"不能移动到那儿。",
	L"你的队伍已经满员了。没有空位雇佣新队员。",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s已经被招募。",

	// Here %s is a name and %d is a number

	L"尚拖欠%s$%d.",

	// In the next string, %s is a name

	L"护送%s吗?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"要雇佣%s吗(每日得支付%s)?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"你要进行拳击比赛吗?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"要买%s吗(得支付%s)?",

	// In the next string, %s is a name

	L"%s接受第%d小队的护送。",

	// These messages are displayed during play to alert the player to a particular situation

	L"卡壳",					//weapon is jammed.
	L"机器人需要%s口径的子弹。",	//Robot is out of ammo
	L"扔到那儿？那不可能。",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"潜行模式 (|Z)",
	L"地图屏幕 (|M)",
	L"结束回合 (|D)",
	L"谈话",
	L"禁音",
	L"起身 (|P|g|U|p)",
	L"光标层次 (|T|a|b)",
	L"攀爬/跳跃",
	L"伏下 (|P|g|D|n)",
	L"检查",
	L"上一个佣兵",
	L"下一个佣兵 (|S|p|a|c|e)",
	L"选项 (|O)",
	L"扫射模式 (|B)",
	L"查看/转向(|L)",
	L"生命: %d/%d\n精力: %d/%d\n士气: %s",
	L"厄?",		//this means "what?"
	L"继续",		//an abbrieviation for "Continued"
	L"对%s关闭禁音模式。",
	L"对%s打开禁音模式。",
	L"耐久度: %d/%d\n油量: %d/%d",
	L"下车",
	L"切换小队 ( |S|h|i|f|t |S|p|a|c|e )",
	L"驾驶",
	L"N/A",		//this is an acronym for "Not Applicable."
	L"使用 (拳头)",
	L"使用 (武器)",
	L"使用 (刀具)",
	L"使用 (爆炸品)",
	L"使用 (医疗用品)",
	L"(抓住)",
	L"(装填弹药)",
	L"(给予)",
	L"%s被触发了。",
	L"%s已到达。",
	L"%s用完了行动点数(AP)。",
	L"%s无法行动。",
	L"%s包扎好了。",
	L"%s用完了绷带。",
	L"这个分区中有敌军。",
	L"视野中没有敌军。",
	L"没有足够的行动点数(AP)。",
	L"没人使用遥控器。",
	L"扫射光了子弹!",
	L"敌兵",
	L"异形",
	L"民兵",
	L"平民",
	L"离开分区",
	L"确定",
	L"取消",
	L"选择佣兵",
	L"小队的所有佣兵",
	L"前往分区",
	L"前往地图",
	L"你不能从这边离开这个分区。",
	L"%s太远了。",
	L"不显示树冠",
	L"显示树冠",
	L"乌鸦"	,				//Crow, as in the large black bird
	L"颈部",
	L"头部",
	L"躯体",
	L"腿部",
	L"要告诉女王她想知道的情报吗？",
	L"获得指纹ID",
	L"指纹ID无效。无法使用该武器。",
	L"达成目标",
	L"路被堵住了",
	L"存钱/取钱",	//Help text over the $ button on the Single Merc Panel
	L"没人需要包扎。",
	L"卡壳",											// Short form of JAMMED, for small inv slots
	L"无法到达那里。", // used ( now ) for when we click on a cliff
	L"路被堵住了。你要和这个人交换位置吗?",
	L"那人拒绝移动。",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"你同意支付%s吗？",
	L"你要接受免费治疗吗?",
	L"你同意让佣兵和Daryl结婚吗?",
	L"钥匙面板",
	L"你不能这样用EPC。",
	L"不杀Krott?",
	L"超出武器的有效射程。",
	L"矿工",
	L"车辆只能在分区间旅行",
	L"现在不能自动包扎",
	L"%s被堵住了",
	L"被Deidranna的军队俘虏的佣兵，被关押在这里！",
	L"锁被击中了",
	L"锁被破坏了",
	L"其他人在使用这扇门。",
	L"耐久度: %d/%d\n油量: %d/%d",
	L"%s看不见%s。", // Cannot see person trying to talk to
	L"附件被移除",
	L"你已经有了两辆车，无法拥有更多的车辆。",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"如果勾中，将立即进入邻近的分区。",
	L"如果勾中，你将被立即自动放置在地图屏幕，\n因为你的佣兵要花些时间来行军。",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"该分区被敌军占据。你不能将佣兵留在这里。\n在进入其它分区前，你必须把这里的问题解决。",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"让留下的佣兵离开本分区，\n将立即进入邻近的分区。",
	L"让留下的佣兵离开本分区，\n你将被立即自动放置在地图屏幕，\n因为你的佣兵要花些时间来行军。",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s需要被你的佣兵护送，他（她）无法独自离开本分区。",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s无法独自离开本分区，因为他得护送%s。", //male singular
	L"%s无法独自离开本分区，因为她得护送%s。", //female singular
	L"%s无法独自离开本分区，因为他得护送多人。", //male plural
	L"%s无法独自离开本分区，因为她得护送多人。", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"如果要让小队在分区间移动的话，\n你的全部队员都必须在附近。",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"如果勾中， %s将独自行军，\n而且被自动重新分配到一个单独的小队中。",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"如果勾中，你当前选中的小队\n将会离开本分区，开始行军。",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s正在被你的佣兵护送，他（她）无法独自离开本分区。你的佣兵必须在附近以护送他（她）离开。",
};



STR16 pRepairStrings[] = 
{
	L"物品",	// tell merc to repair items in inventor
	L"SAM导弹基地",		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"取消",		// cancel this menu
	L"机器人",		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 

STR16 sPreStatBuildString[] =
{
	L"丧失",// the merc has lost a statistic
	L"获得",// the merc has gained a statistic
	L"点",// singular
	L"点",// plural
	L"级",//singular
	L"级",//plural
};

STR16 sStatGainStrings[] =
{
	L"生命。",
	L"敏捷。",
	L"灵巧。",
	L"智慧。",
	L"医疗技能。",
	L"爆破技能。",
	L"机械技能。",
	L"枪法技能。",
	L"级别",
	L"力量",
	L"领导",
};


STR16 pHelicopterEtaStrings[] =
{
	L"总距离: ", 			// total distance for helicopter to travel
	L"安全: ", 			// distance to travel to destination
	L"不安全: ",		// distance to return from destination to airport
	L"总价: ", 		// total cost of trip by helicopter
	L"耗时: ", 			// ETA is an acronym for "estimated time of arrival"
	L"直升机油量不够，必须在敌占区着陆。", 	// warning that the sector the helicopter is going to use for refueling is under enemy control ->
  L"乘客: ",
  L"选择Skyrider还是“着陆点”？",
  L"Skyrider",
  L"着陆点",
};

STR16 sMapLevelString[] =
{
	L"地层: ", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"忠诚度: ",	// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"不能在地底下达行军命令。",
};

STR16 gsTimeStrings[] =
{
	L"小时",				// hours abbreviation
	L"分钟",				// minutes abbreviation
	L"秒",				// seconds abbreviation
	L"日",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"无",
	L"医院",
	L"工厂",
	L"监狱",
	L"军事基地",
	L"机场",
	L"靶场",	// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"存货",
	L"离开",
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"大小",				// 0 // size of the town in sectors
	L"", 					// blank line, required
	L"占领度",				// how much of town is controlled
	L"无",				// none of this town
	L"矿区",			// mine associated with this town
	L"忠诚度",				// 5 // the loyalty level of this town
	L"民兵",				// the forces in the town trained by the player
	L"",
	L"主要设施",			// main facilities in this town
	L"等级",			// the training level of civilians in this town
	L"民兵训练度",		// 10 // state of civilian training in town
	L"民兵",			// the state of the trained civilians in the town
	L"巡逻民兵",	// L"Mobile Training",			// HEADROCK HAM 3.6: The stat of Mobile militia training in town
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"矿井",			// 0
	L"银块",
	L"金块",
	L"当前日产量",
	L"最高产量",
	L"废弃",				// 5
	L"关闭",
	L"矿脉耗尽",
	L"生产",
	L"状态",
	L"生产率",
	L"矿石类型",			// 10
	L"占领度",
	L"忠诚度",
//	L"在岗矿工",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"敌军",
	L"分区",
	L"物品数量",
	L"未知",
	L"已占领",
	L"是",
	L"否",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s不够近。",	//Merc is in sector with item but not close enough
	L"无法选择该佣兵。",  //MARK CARTER
	L"%s不在这个分区，不能拿到这个物品。",
	L"在战斗时，你只能动手捡起物品。",
	L"在战斗时，你只能动手放下物品。",
	L"%s不在该分区，不能放下那个物品。",
	L"在战斗时你没有时间开启成箱的弹药。",
};

STR16 pMapInventoryStrings[] =
{
	L"位置",		// sector these items are in
	L"物品总数",	// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"如果要改变佣兵的分配任务，比如分到另一个小队、当医生、进行修理等，请按 '任务' 栏。",
	L"要让佣兵以另一个分区为行军目标，请按'Dest'栏。",
	L"一旦对佣兵下达了行军命令 ，请按时间压缩按钮以让他们开始行进。",
	L"鼠标左击以选择该分区。再次鼠标左击以对佣兵下达行军命令, 或者鼠标右击以获取分区信息小结。",
	L"任何时候在该屏幕下都可以按'h'键，以弹出帮助窗口。",
	L"测试文本",
	L"测试文本",
	L"测试文本",
	L"测试文本",
	L"您尚未开始Arulco之旅，现在在这个屏幕上您无事可做。当您把队员都雇佣好后，请左击右下方的“时间压缩”按钮。这样在您的队伍到达Arulco前，时间就前进了。",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"调动佣兵", 	// title for movement box
	L"安排行军路线",	// done with movement menu, start plotting movement
	L"取消",		// cancel this menu
	L"其它",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"糟了: ", 			// an error has occured
	L"佣兵合同到期了: ", 	// this pop up came up due to a merc contract ending
	L"佣兵完成了分配的任务: ", // this pop up....due to more than one merc finishing assignments
	L"佣兵醒来了，继续干活: ", // this pop up ....due to more than one merc waking up and returing to work
	L"佣兵困倦了: ", // this pop up ....due to more than one merc being tired and going to sleep
	L"合同快到期了: ", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"显示城镇 (|W)",
	L"显示矿井 (|M)",
	L"显示队伍和敌人 (|T)",
	L"显示领空 (|A)",
	L"显示物品 (|I)",
	L"显示民兵和敌人 (|Z)",
	L"显示民兵移动 (|R)", // HEADROCK HAM 4: Mobile Restrictions Button
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"下一页 (|.)",		// next page // TODO.Translate
	L"上一页 (|,)",		// previous page // TODO.Translate
	L"Exit Sector Inventory (|E|s|c)",	// exit sector inventory // TODO.Translate
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"笔记本电脑 (|L)",
	L"战术屏幕 (|E|s|c)",
	L"选项 (|O)",
	L"时间压缩 (|+)", 	// time compress more
	L"时间压缩 (|-)", 	// time compress less
	L"上一信息 (|U|p)\n上一页 (|P|g|U|p)", // previous message in scrollable list
	L"下一信息 (|D|o|w|n)\n下一页 (|P|g|D|n)", 	// next message in the scrollable list
	L"开始/停止时间压缩 (|S|p|a|c|e)",	//"Start/Stop Time (|S|p|a|c|e)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"帐户余额", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s死了。",
};


STR16 pDayStrings[] =
{
	L"日",
};

// the list of email sender names

CHAR16 pSenderNameList[500][128] =
{
	L"",
};
/*
{
	L"Enrico",
	L"Psych Pro Inc",
	L"桌面帮助",
	L"Psych Pro Inc",
	L"Speck",
	L"R.I.S.",		//5
	L"Barry",
	L"Blood",
	L"Lynx",
	L"Grizzly",
	L"Vicki",			//10
	L"Trevor",
	L"Grunty",
	L"Ivan",
	L"Steroid",
	L"Igor",			//15
	L"Shadow",
	L"Red",
	L"Reaper",
	L"Fidel",
	L"Fox",				//20
	L"Sidney",
	L"Gus",
	L"Buns",
	L"Ice",
	L"Spider",		//25
	L"Cliff",
	L"Bull",
	L"Hitman",
	L"Buzz",
	L"Raider",		//30
	L"Raven",
	L"Static",
	L"Len",
	L"Danny",
	L"Magic",
	L"Stephen",
	L"Scully",
	L"Malice",
	L"Dr.Q",
	L"Nails",
	L"Thor",
	L"Scope",
	L"Wolf",
	L"MD",
	L"Meltdown",
	//----------
	L"M.I.S 保险公司",
	L"Bobby Rays",
	L"Kingpin",
	L"John Kulba",
	L"A.I.M.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"上一个",
  L"下一个",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"你有新的邮件...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
  L"删除邮件？",
  L"删除未读的邮件？",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"来自: ",
	L"标题: ",
	L"日期: ",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"邮箱",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"帐簿",	//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"收入: ", 				// credit (subtract from) to player's account
	L"支出: ",				// debit (add to) to player's account
	L"昨日实际收入: ",
	L"昨日其它存款: ",
	L"昨日支出: ",
	L"昨日日终余额: ",
	L"今日实际收入: ",
	L"今日其它存款: ",
	L"今日支出: ",
	L"今日当前余额: ",
	L"预期收入: ",
	L"明日预计余额: ",		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
  L"天数",			// the day column
	L"收入", 			// the credits column (to ADD money to your account)
	L"支出",				// the debits column (to SUBTRACT money from your account)
	L"交易记录", // transaction type - see TransactionText below
	L"余额",		// balance at this point in time
	L"页数",				// page number
	L"日", 		// the day(s) of transactions this page displays
};


STR16 pTransactionText[] =
{
	L"自然增值利息",		// interest the player has accumulated so far
	L"匿名存款",
	L"交易费用",
	L"已雇佣",			// Merc was hired
	L"在Bobby Ray购买货品", // Bobby Ray is the name of an arms dealer
	L"在M.E.R.C开户。",
	L"%s的医疗保证金",  	// medical deposit for merc
	L"IMP心理剖析分析", 		// IMP is the acronym for International Mercenary Profiling
	L"为%s购买保险",
	L"缩短%s的保险期限",
	L"延长%s的保险期限",			// johnny contract extended
	L"取消%s的保险",
	L"%s的保险索赔",	// insurance claim for merc
	L"1日", 				// merc's contract extended for a day
	L"1周",				// merc's contract extended for a week
	L"2周",				// ... for 2 weeks
	L"采矿收入",
	L"", //String nuked
	L"买花",
	L"%s的医疗保证金的全额退款",
	L"%s的医疗保证金的部分退款",
	L"%s的医疗保证金没有退款",
	L"付给%s金钱",// %s is the name of the npc being paid
	L"支付给%s的佣金", 	// transfer funds to a merc
	L"%s退回的佣金",	 // transfer funds from a merc
	L"在%s训练民兵",	 // initial cost to equip a town's militia
	L"向%s购买了物品。", //is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s存款",
	L"将装备卖给了当地人",
	L"工厂使用", // L"Facility Use", // HEADROCK HAM 3.6
	L"民兵保养", // L"Militia upkeep", // HEADROCK HAM 3.6
};

STR16 pTransactionAlternateText[] =
{
	L"的保险",  		// insurance for a merc
	L"延长%s的合同一日。",	// entend mercs contract by a day
	L"延长%s的合同一周。",
	L"延长%s的合同两周。",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"付给 Skyrider $%d",	// skyrider was paid an amount of money
	L"还欠 Skyrider $%d",	// skyrider is still owed an amount of money
	L"Skyrider 完成补给汽油",// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider 已作好再次飞行的准备。", // Skyrider was grounded but has been freed
	L"Skyrider 没有乘客。如果你试图运送这个分区的佣兵，首先要分配他们进入“交通工具”－>“直升飞机”。",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"高涨",
	L"良好",
	L"稳定",
	L"低下",
	L"恐慌",
	L"糟糕",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s的装备现在可以在Omerta (A9)获得。",
	L"%s的装备现在可以在Drassen (B13)获得。",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"生命",
	L"精力",
	L"士气",
	L"状态",	// the condition of the current vehicle (its "health")
	L"油量",	// the fuel level of the current vehicle (its "energy")
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"上一佣兵 (|L|e|f|t)",	//"Previous Merc (|L|e|f|t)", 			// previous merc in the list
	L"下一佣兵 (|R|i|g|h|t)",	//"Next Merc (|R|i|g|h|t)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"耗时: ",				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"你将不会再见到它了。你确定吗？",	 	// do you want to continue and lose the item forever
	L"这个物品看起来非常非常重要。你真的要扔掉它吗？",	// does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"小队不能行军，因为有人在睡觉。",

//1-5
	L"首先要回到地面，才能移动小队。",
	L"行军？那里是敌占区!",
	L"必须给佣兵分配小队或者交通工具才能开始行军。",
	L"你现在没有任何队员。", 	// you have no members, can't do anything
	L"佣兵无法遵从命令。",		// merc can't comply with your order
//6-10
	L"需要有人护送才能行军。请把他分进一个小队里。", // merc can't move unescorted .. for a male
	L"需要有人护送才能行军。请把她分进一个小队里。", // for a female
	L"佣兵尚未到达Arulco!",
	L"看来得先谈妥合同。",
	L"无法发出行军命令。目前有空袭。",
//11-15
	L"行军? 这里正在战斗中!",
	L"你在分区%s被血猫伏击了!",
	L"你刚刚进入了 %s 分区，这里是血猫的巢穴!", // HEADROCK HAM 3.6: Added argument.
	L"",
	L"在%s的SAM导弹基地被敌军占领了。",
//16-20
	L"在%s的矿井被敌军占领了。你的日收入下降为每日%s。",
	L"敌军未遭到抵抗，就占领了%s",
	L"至少有一名你的佣兵不能被分配这个任务。",
	L"%s无法加入%s，因为它已经满员了",
	L"%s无法加入%s，因为它太远了。",
//21-25
	L"在%s的矿井被敌军占领了!",
	L"敌军入侵了%s处的SAM导弹基地",
	L"敌军入侵了%s",
	L"敌军在%s出没。",
	L"敌军占领了%s.",
//26-30
	L"你的佣兵中至少有一人不能睡眠。",
	L"你的佣兵中至少有一人不能醒来。",
	L"训练完毕，才会出现民兵。",
	L"现在无法对%s下达行军命令。",
	L"不在城镇边界的民兵无法行军到另一个分区。",
//31-35
	L"你不能在%s拥有民兵。",
	L"车是空的，无法移动。!",
	L"%s受伤太严重了，无法行军!",
	L"你必须首先离开博物馆！",
	L"%s死了！",
//36-40
	L"%s无法转到%s因为它在移动中",
	L"%s无法那样进入交通工具",
	L"%s无法加入%s",
	L"在你雇佣新的佣兵前，你不能压缩时间。",
	L"车辆只能在公路上开！",
//41-45
	L"在佣兵移动时，你不能重新分配任务",
	L"车辆没油了！",
	L"%s太累了，以致不能行军。",
	L"车上没有人能够驾驶。",
	L"这个小队的佣兵现在不能移动。",
//46-50
	L"其他佣兵现在不能移动。",
	L"车辆被损坏得太严重了！",
	L"每个分区只能由两名佣兵来训练民兵。",
	L"没有遥控员，机器人无法移动。请把他们分配在同一个小队。",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"再点击一下目的地以确认你的最后路线，或者点击下一个分区以设置更多的路点。",
	L"行军路线已确认。",
	L"目的地未改变。",
	L"行军路线已取消。",
	L"行军路线已缩短。",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"点击你想让佣兵着陆的分区。",
	L"好的。佣兵将在%s着陆",
	L"佣兵不能飞往那里，领空不安全!",
	L"取消。着陆分区未改变",
	L"%s上的领空现在不安全了!着陆分区被改为%s。",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"进入存货屏幕 (|E|n|t|e|r)",
	L"扔掉物品",	//"Throw Item Away",
	L"离开存货屏幕 (|E|n|t|e|r)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"让%s把装备留在他现在所在的地方(%s)，或者在Drassen (B13)登机飞离Arulco，把装备留在那里?",
	L"让%s把装备留在他现在所在的地方(%s)，或者在Omerta (A9)登机飞离Arulco，把装备留在那里?",
	L"要离开了，他的装备将被留在Omerta (A9)。",
	L"要离开了，他的装备将被留在Drassen (B13)。",
	L"%s要离开了，他的装备将被留在%s。",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"让%s把装备留在她现在所在的地方(%s)，或者在Drassen (B13)登机飞离Arulco，把装备留在那里?",
	L"让%s把装备留在她现在所在的地方(%s)，或者在Omerta (A9)登机飞离Arulco，把装备留在那里?",
	L"要离开了，她的装备将被留在Omerta (A9)。",
	L"要离开了，她的装备将被留在Drassen (B13)。",
	L"%s要离开了，她的装备将被留在%s。",
};


STR16 pMercContractOverStrings[] =
{
	L"的合同到期了，所以他回家了。",
	L"的合同到期了，所以她回家了。",
	L"的合同中止了，所以他离开了。",
	L"的合同中止了，所以她离开了。",
	L"你欠了M.E.R.C.太多钱，所以%s离开了。",
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"无效的授权号",
	L"你试图重新开始整个测试。你确定吗？",
	L"请输入正确的全名和性别。",
	L"对你的财政状况的预先分析显示了你无法负担心理剖析的费用。",
	L"现在不是个有效的选择。",
	L"要进行心理剖析，你的队伍中必须至少留一个空位。",
	L"测试完毕。",
	L"无法从磁盘上读入I.M.P.人物数据。",
	L"你已经达到I.M.P.人物上限。",
	L"你已经达到I.M.P.该性别人物上限。",
	L"你无法支付此I.M.P.人物的费用。",
	L"新的I.M.P.人物加入了你的队伍。",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"关于我们",			// about the IMP site
	L"开始",		// begin profiling
	L"性格",		// personality section
	L"属性", 		// personal stats/attributes section
	L"外表", 			// changed from portrait - SANDRO
	L"嗓音%d",		// the voice selection
	L"完成",		// done profiling
	L"重新开始",	// start over profiling
	L"是的，我选择了高亮突出的回答。",
	L"是",
	L"否",
	L"结束",			// finished answering questions
	L"上一个",			// previous question..abbreviated form
	L"下一个", 			// next question
	L"是的，我确定。",	// yes, I am certain
	L"不，我想重新开始。",
	L"是",
	L"否",
	L"后退",		// back one page
	L"取消",		// cancel selection
	L"是的，我确定。",
	L"不，让我再看看。",
	L"注册",			// the IMP site registry..when name and gender is selected
	L"分析...", 		// analyzing your profile results
	L"完成",
	L"性格特征", // Change from "Voice" - SANDRO
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed - SANDRO
	L"选择完角色性格特征之后，接着选择您所掌握的技能。",
	L"最后调整好你的各项属性值完成整个自创角色过程。",
	L"开始实际分析，请先选择头像、声音和颜色。",
	L"初步阶段完成，现在开始角色性格特征分析部分。",
};

STR16 pFilesTitle[] =
{
  L"文件查看器",
};

STR16 pFilesSenderList[] =
{
  L"侦察报告",
	L"1号通缉令",
	L"2号通缉令",
	L"3号通缉令",
	L"4号通缉令",
	L"5号通缉令",
	L"6号通缉令",
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"日志",
};

STR16 pHistoryHeaders[] =
{
	L"日",		// the day the history event occurred
	L"页数",	// the current page in the history report we are in
	L"日数",	// the days the history report occurs over
	L"位置",	// location (in sector) the event occurred
	L"事件",	// the event label
};

// various history events
// THESE STRINGS ARE "HISTORY LOG" STRINGS AND THEIR LENGTH IS VERY LIMITED.
// PLEASE BE MINDFUL OF THE LENGTH OF THESE STRINGS. ONE WAY TO "TEST" THIS
// IS TO TURN "CHEAT MODE" ON AND USE CONTROL-R IN THE TACTICAL SCREEN, THEN
// GO INTO THE LAPTOP/HISTORY LOG AND CHECK OUT THE STRINGS. CONTROL-R INSERTS
// MANY (NOT ALL) OF THE STRINGS IN THE FOLLOWING LIST INTO THE GAME.
STR16 pHistoryStrings[] =
{
	L"",																						// leave this line blank
	//1-5
	L"从A.I.M雇佣了%s。",
	L"从M.E.R.C雇佣了%s。",
	L"%s死了。",	//"%s died.", 															// merc was killed
	L"在M.E.R.C开户。",
	L"接受Enrico Chivaldori的委托",	//"Accepted Assignment From Enrico Chivaldori",
	//6-10
	L"IMP已生成",
	L"为%s购买保险。",
	L"取消%s的保险合同。",
	L"收到%s的保险索赔。",
	L"延长一日%s的合同。",
	//11-15
	L"延长一周%s的合同。",
	L"延长两周%s的合同。",
	L"%s被解雇了。",
	L"%s退出了。",
	L"开始任务。",
	//16-20
	L"完成任务。",
	L"和%s的矿主交谈",
	L"解放了%s",
	L"启用作弊",
	L"食物会在明天送达Omerta",
	//21-25
	L"%s离队并成为了Daryl Hick的妻子",
	L"%s的合同到期了。",
	L"招募了%s。",
	L"Enrico抱怨进展缓慢",
	L"战斗胜利",
	//26-30
	L"%s的矿井开始缺乏矿石",
	L"%s的矿井采完了矿石",
	L"%s的矿井关闭了",	//"%s mine was shut down",
	L"%s的矿井复工了",
	L"发现一个叫Tixa的监狱。",
	//31-35
	L"打听到一个叫Orta的秘密武器工厂。",
	L"在Orta的科学家捐赠了大量的火箭枪。",
	L"Deidranna女王在利用死尸做某些事情。",
	L"Frank谈到了在San Mona的拳击比赛。",
	L"一个病人说他在矿井里看到了一些东西。",
	//36-40
	L"遇到一个叫Devin的人，他出售爆炸物。",
	L"偶遇Mike，前AIM名人！",
	L"遇到Tony，他做武器买卖。",
	L"从Krott中士那里得到一把火箭枪。",
	L"把Angel皮衣店的契约给了Kyle。",
	//41-45
	L"Madlab提议做一个机器人。",
	L"Gabby能制作对付虫子的隐形药。",
	L"Keith歇业了。",
	L"Howard给Deidranna女王提供氰化物。",
	L"遇到Keith -Cambria的杂货商。",
	//46-50
	L"遇到Howrd，一个在Balime的医药商。",
	L"遇到Perko，他开了一家小修理档口。。",
	L"遇到在Balime的Sam，他有一家五金店。",
	L"Franz做电子产品和其他货物的生意。",
	L"Arnold在Grumm开了一家修理店。",
	//51-55
	L"Fredo在Grumm修理电子产品。",
	L"收到在Balime的有钱人的捐款。",
	L"遇到一个叫Jake的废品商人。",
	L"一个流浪者给了我们一张电子钥匙卡。",
	L"贿赂了Walter，让他打开地下室的门。",
	//56-60
	L"如果Dave有汽油，他会免费进行加油。",
	L"贿赂Pablo。",
	L"Kingping拿回了San Mona矿井中的钱。",
	L"%s赢了拳击赛",
	L"%s输了拳击赛",
	//61-65
	L"%s丧失了拳击赛的参赛资格",
	L"在废弃的矿井里找到一大笔钱。",
	L"遭遇Kingpin派出的杀手。",
	L"该分区失守",	//ENEMY_INVASION_CODE
	L"成功防御该分区",
	//66-70
	L"作战失败",			//ENEMY_ENCOUNTER_CODE
	L"致命的伏击",		//ENEMY_AMBUSH_CODE
	L"杀光了敌军的伏兵",
	L"攻击失败",		//ENTERING_ENEMY_SECTOR_CODE
	L"攻击成功！",
	//71-75
	L"异形攻击",		//CREATURE_ATTACK_CODE
	L"被血猫吃掉了",	//BLOODCAT_AMBUSH_CODE
	L"宰掉了血猫",
	L"%s被干掉了",
	L"把一个恐怖分子的头颅给了Carmen。",
	L"Slay走了",
	L"干掉了%s",
};

STR16 pHistoryLocations[] =
{
	L"N/A",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"邮箱",
	L"网页",
	L"财务",
	L"人事",
	L"日志",
	L"文件",
	L"关闭",
	L"sir-FER 4.0 简体中文版",		// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Ray's",
	L"I.M.P",
	L"M.E.R.C.",
	L"公墓",
	L"花店",
	L"M.I.S 保险公司",
	L"取消",
};

STR16 pBookmarkTitle[] =
{
	L"收藏夹",
	L"右击以放进收藏夹，便于以后访问本页面。",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"下载...",
	L"重载...",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"好的",
	L"拿取",		// take money from merc
	L"给予",		//give money to merc
	L"取消",		// cancel transaction
	L"清除",		//clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"转帐 $",  	// transfer money to merc -- short form
	L"状态",			// view stats of the merc
	L"物品清单", 			// view the inventory of the merc
	L"雇佣合同",
};

STR16 sATMText[ ]=
{
	L"转帐？",		// transfer funds to merc?
	L"确定？",		// are we certain?
	L"输入金额",		// enter the amount you want to transfer to merc
	L"选择类型",	// select the type of transfer to merc
	L"资金不足",  //not enough money to transfer to merc
	L"必须是$10的倍数", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"错误",
	L"服务器没有DNS入口。",
	L"请检查URL地址，再次尝试连接。",
	L"好的",
	L"主机连接时断时续。预计需要较长的传输时间。",
};


STR16 pPersonnelString[] =
{
	L"佣兵: ", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0 简体中文版",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. 成员",
	L"A.I.M. 肖像",		// a mug shot is another name for a portrait
	L"A.I.M. 排序",
	L"A.I.M.",
	L"A.I.M. 前成员",
	L"A.I.M. 规则",
	L"A.I.M. 历史",
	L"A.I.M. 链接",
	L"M.E.R.C.",
	L"M.E.R.C. 账号",
	L"M.E.R.C. 注册",
	L"M.E.R.C. 索引",
	L"Bobby Ray's",
	L"Bobby Ray's - 枪械",
	L"Bobby Ray's - 弹药",
	L"Bobby Ray's - 护甲",
	L"Bobby Ray's - 杂货",	//"Bobby Ray's - Misc",							//misc is an abbreviation for miscellaneous
	L"Bobby Ray's - 二手货",
	L"Bobby Ray's - 邮购",
	L"I.M.P.",
	L"I.M.P.",
	L"联合花卉服务公司",
	L"联合花卉服务公司 - 花卉",
	L"联合花卉服务公司 - 订单",
	L"联合花卉服务公司 - 贺卡",
	L"Malleus, Incus & Stapes 保险公司",
	L"信息",
	L"合同",
	L"评论",
	L"McGillicutty's 公墓",
	L"",
	L"无法找到URL",
	L"Bobby Ray's - 最近的运货",
	L"",
	L"",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Help",
	L"再次点击页面以放进收藏夹。",
};

STR16 pLaptopTitles[] =
{
	L"邮箱",
	L"文件查看器",
	L"人事",
	L"帐簿",
	L"日志",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"阵亡",
	L"解雇",
	L"其它: ",
	L"结婚",
	L"合同到期",
	L"退出",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"当前成员: ",
	L"离队成员: ",
	L"每日花费: ",
	L"最高日薪: ",
	L"最低日薪: ",
	L"行动中牺牲: ",
	L"解雇: ",
	L"其它: ",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"最低",
	L"平均",
	L"最高",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",
};


// horizontal and vertical indices on the map screen

STR16 pMapVertIndex[] =
{
	L"X",
	L"A",
	L"B",
	L"C",
	L"D",
	L"E",
	L"F",
	L"G",
	L"H",
	L"I",
	L"J",
	L"K",
	L"L",
	L"M",
	L"N",
	L"O",
	L"P",
};

STR16 pMapHortIndex[] =
{
	L"X",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"10",
	L"11",
	L"12",
	L"13",
	L"14",
	L"15",
	L"16",
};

STR16 pMapDepthIndex[] =
{
	L"",
	L"-1",
	L"-2",
	L"-3",
};

// text that appears on the contract button

STR16 pContractButtonString[] =
{
	L"合同",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"继续",
	L"停止",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"你在这个地区战败了！",
	L"敌人冷酷无情地处死了你的队员！",
	L"你的昏迷的队员被俘虏了！",
	L"你的队员成了敌军的俘虏。",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] =
{
	L"上一页",
	L"下一页",
	L"接受",
	L"清除",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"上一页",
	L"下一页",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"支付",
	L"主页",
	L"账号: ",
	L"佣兵",
	L"日数",
	L"日薪",	//5
	L"索价",
	L"合计: ",
	L"你确定要支付%s吗？",
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"上一页",
  L"下一页",
};


//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",

	L"上一位",	//"Previous",
	L"雇佣",	//"Hire",
	L"下一位",	//"Next",
	L"附加信息",	//"Additional Info",
	L"主页",	//"Home",
	L"已雇佣",	//"Hired",
	L"日薪: ",	//"Salary:",
	L"每日",		//"Per Day",
	L"阵亡",	//"Deceased",

	L"看起来你雇佣了太多的佣兵。最多只能雇18人。",	//"Looks like you're trying to hire too many mercs. Your limit is 18.",
	L"不可雇佣",	//"Unavailable",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"开户",	//"Open Account",
	L"取消",	//"Cancel",
	L"你没有帐户。你希望开一个吗？",	//"You have no account.  Would you like to open one?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, 创办者和所有权拥有者",
	L"开户点击这里",	//"To open an account press here",
	L"查看帐户点击这里",	//"To view account press here",
	L"查看文件点击这里",	//"To view files press here",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer failed. No funds available.",	// TODO.Translate
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"McGillicutty公墓: 1983开业，办理家庭悼念业务。",
	L"葬礼部经理兼A.I.M.前佣兵 Murray \"Pops\" McGillicutty是一名经验丰富、业务熟练的殡仪业者。",
	L"Pops跟死亡和葬礼打了一辈子交道，他非常熟悉该业务。",
	L"McGillicutty公墓提供各种各样的悼念服务: 从可以依靠着哭泣的肩膀到对严重变形的遗体做美容美体服务。",
	L"McGillicutty公墓是你所爱的人的安息地。",

	// Text for the various links available at the bottom of the page
	L"献花",
	L"骨灰盒",
	L"火葬服务",
	L"安排葬礼",
	L"葬礼规则",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"很抱歉，由于家里有人去世，本网站的剩余部分尚未完成。一旦解决了宣读遗嘱和财产分配问题，本网站会尽快建设好。",
	L"很抱歉，但是，现在还是测试期间，请以后再来访问。",
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"花卉",

	//Address of United Florist

	L"\"We air-drop anywhere\"",
	L"1-555-SCENT-ME",
	L"333 NoseGay Dr, Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"我们快速高效!",
	L"保证把鲜花在第二天送到世界上大部分地区，但有少量限制。",
	L"保证价格是世界上最低廉的!",
	L"向我们反应比我们价格更低的送花服务广告，我们会送你一打绝对免费的玫瑰。",
	L"自从1981年来，我们送植物、送动物、送鲜花。",
	L"我们雇请了被颁发过勋章的前轰炸机飞行员，他们能把你的鲜花空投在指定位置的十英里半径内。总是这样 - 每次这样!",
	L"让我们满足你对鲜花的品位。",
	L"让Bruce，我们的世界闻名的花卉设计师，从我们的花房里为你亲手摘取最新鲜、最优质的花束。",
	L"还有请记住，如果我们没有你要的花，我们能种出来 - 很快!",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"后退",	//"Back",
	L"发送",	//"Send",
	L"清除",	//"Clear",
	L"花卉",	//"Gallery",

	L"花卉名称: ",			//"Name of Bouquet:",
	L"价格: ",			//"Price:",//5
	L"订单号: ",			//"Order Number:",
	L"送货日期",			//"Delivery Date",
	L"第二天",			//"next day",
	L"慢慢送去",			//"gets there when it gets there",
	L"送货目的地",			//"Delivery Location",//10
	L"额外服务",			//"Additional Services",
	L"变形的花卉($10)",		//"Crushed Bouquet($10)",
	L"黑玫瑰($20)",			//"Black Roses($20)",
	L"枯萎的花卉($10)",		//"Wilted Bouquet($10)",
	L"水果蛋糕(如果有的话)($10)",	//"Fruit Cake (if available)($10)",	//15
	L"私人密语: ",			//"Personal Sentiments:",
	L"你写的话不能多于75字。",
	L"...或者选择我们提供的",	//L"...or select from one of our",

	L"标准贺卡",			//"STANDARDIZED CARDS",
	L"传单信息",			//"Billing Information",	//20

	//The text that goes beside the area where the user can enter their name

	L"姓名: ",				//"Name:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"上一页",	//abbreviation for previous
	L"下一页",	//abbreviation for next

	L"点击你想要订购的花卉。",
	L"请注意: 为了防止运输中的枯萎和变形，每束花另收$10包装费。",

	//text on the button

	L"主页",	//"Home",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"请点击你想要订购的贺卡",	//"Click on your selection",
	L"后退",			//"Back",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"订单",		//"Order Form",		//Title of the page
	L"数量",			//"Qty",			// The number of items ordered
	L"重量 （%s）",		//"Weight (%s)",	// The weight of the item
	L"物品名称",		//"Item Name",		// The name of the item
	L"物品单价",		//"Unit Price",			// the item's weight
	L"总价",		//"Total",		//5	// The total price of all of items of the same type
	L"全部物品总价",		//"Sub-Total",		// The sub total of all the item totals added
	L"运费 (视目的地而定)",	//	"S&H (See Delivery Loc.)",	// S&H is an acronym for Shipping and Handling
	L"全部费用",		//"Grand Total",		// The grand total of all item totals + the shipping and handling
	L"送货目的地",	//"Delivery Location",
	L"运输速度",	//"Shipping Speed",		//10	// See below
	L"运费(每%s)",	//"Cost (per %s.)",		// The cost to ship the items
	L"连夜速递",	//"Overnight Express",		// Gets deliverd the next day
	L"2工作日",	//"2 Business Days",		// Gets delivered in 2 days
	L"标准服务",	//"Standard Service",		// Gets delivered in 3 days
	L"清除订单",		//"Clear Order",	//15// Clears the order page
	L"确认订单",	//"Accept Order",		// Accept the order
	L"后退",		//"Back",	// text on the button that returns to the previous page
	L"主页",		//"Home",	// Text on the button that returns to the home page
	L"*代表二手货",	//"* Denotes Used Items",	// Disclaimer stating that the item is used
	L"你无法支付所需费用。",	//"You can't afford to pay for this.",		//20	// A popup message that to warn of not enough money
	L"<无>",		//"<NONE>",		// Gets displayed when there is no valid city selected
	L"你确定要把该订单里订购的物品送往%s吗?",	//"Are you sure you want to send this order to %s?",	// A popup that asks if the city selected is the correct one
	L"包裹重量**",	//"Package Weight**",		// Displays the weight of the package
	L"** 最小重量: ",		//"** Min. Wt.",			// Disclaimer states that there is a minimum weight for the package
	L"运货",		//"Shipments",
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"手枪", //"Pistol",
	L"自动手枪", //"M. Pistol",
	L"冲锋枪", //"SMG",
	L"歩枪", //"Rifle",
	L"狙击歩枪", //"SN Rifle",
	L"突击歩枪", //"AS Rifle",
	L"机枪", //"MG",
	L"霰弹枪", //"Shotgun",
	L"重武器", //"Heavy W.",

	// Ammo
	L"手枪", //"Pistol",
	L"自动手枪", //"M. Pistol",
	L"冲锋枪", //"SMG",
	L"歩枪", //"Rifle",
	L"狙击歩枪", //"SN Rifle",
	L"突击歩枪", //"AS Rifle",
	L"机枪", //"MG",
	L"霰弹枪", //"Shotgun",

	// Used
	L"枪械", //"Guns",
	L"护甲", //"Armor",
	L"携行具", //"LBE Gear",
	L"其他", //"Misc",

	// Armour
	L"头盔", //"Helmets",
	L"防弹衣", //"Vests",
	L"作战裤", //"Leggings",
	L"防弹板", //"Plates",

	// Misc
	L"刀具", //"Blades",
	L"飞刀", //"Th. Knives",
	L"格斗武器", //"Blunt W.", // TODO.Translate
	L"手雷/榴弹", //"Grenades",
	L"炸药", //"Bombs",
	L"医疗用品", //"Med. Kits",
	L"工具套装", //"Kits",
	L"通讯/夜视", //"Face Items",
	L"携行具", //"LBE Gear",
	L"附件/瞄准具", //"Misc.",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"订购",				//"To Order",	// Title
	// instructions on how to order
	L"请点击该物品。如果要订购多件，请连续点击。右击以减少要订购的数量。一旦选好了你要订购的，请前往订单页面。",

	//Text on the buttons to go the various links

	L"上一页",	//"Previous Items",		//
	L"枪械",	//"Guns", 			//3
	L"弹药",	//"Ammo",			//4
	L"护甲",	//"Armor",			//5
	L"其他",	//"Misc.",			//6	//misc is an abbreviation for miscellaneous
	L"二手货",	//"Used",			//7
	L"下一页",	//"More Items",
	L"订货单",	//"ORDER FORM",
	L"主页",	//"Home",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"你的队伍有",		//"Your team has",//11
	L"件可使用该类型弹药的武器", 	//"weapon(s) that use this type of ammo", //12

	//The following lines provide information on the items

	L"重量: ",	//"Weight:",		// Weight of all the items of the same type
	L"口径: ",	//"Cal:",			// the caliber of the gun
	L"载弹量: ",	//"Mag:",			// number of rounds of ammo the Magazine can hold
	L"射程: ",	//"Rng:",			// The range of the gun
	L"杀伤力: ",	//"Dam:",			// Damage of the weapon
	L"射速: ",	//"ROF:",			// Weapon's Rate Of Fire, acronym ROF
	L"单价: ",	//"Cost:",			// Cost of the item
	L"库存: ",	//"In stock:",			// The number of items still in the store's inventory
	L"购买量: ",	//"Qty on Order:",		// The number of items on order
	L"已损坏",	//"Damaged",			// If the item is damaged
	L"重量: ",	//"Weight:",			// the Weight of the item
	L"小计: ",	//"SubTotal:",			// The total cost of all items on order
	L"* %％ 可用",	//"* %% Functional",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time

	L"靠!  我们这里的在线订单一次只接受10件物品的订购。如果你想要订购更多东西（我们希望如此），请接受我们的歉意，再开一份订单。",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"抱歉，这种商品我们现在正在进货。请稍后再来订购。",

	//A popup that tells the user that the store is temporarily sold out

	L"抱歉，这种商品我们现在缺货。",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"这里有最新最火爆的军火供应",	//"This is the place to be for the newest and hottest in weaponry and military supplies",
	L"我们提供硬件满足您所有破坏欲望！",	//"We can find the perfect solution for all your explosives needs",
	L"二手货",	//"Used and refitted items",

	//Text for the various links to the sub pages

	L"杂货",	//"Miscellaneous",
	L"枪械",	//"GUNS",
	L"弹药",	//"AMMUNITION",		//5
	L"护甲",	//"ARMOR",

	//Details on the web site

	L"独此一家，别无分店。",	//"If we don't sell it, you can't get it!",
	L"网站建设中",	//"Under Construction",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"A.I.M成员",	//"A.I.M. Members",				// Title
	// Title for the way to sort
	L"排序: ",	//"Sort By:",

	// sort by...

	L"薪金",	//"Price",
	L"级别",	//"Experience",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"爆破",	//"Explosives",
	L"机械",	//"Mechanical",

	//Text of the links to other AIM pages

	L"查看佣兵的肖像索引",	//"View the mercenary mug shot index",
	L"查看单独的佣兵档案",	//"Review the individual mercenary's file",
	L"浏览A.I.M前成员",	//"Browse the A.I.M. Alumni Gallery",

	// text to display how the entries will be sorted

	L"升序",	//"Ascending",
	L"降序",	//"Descending",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"上一页",	//"Previous Page",
	L"AIM主页",	//"AIM HomePage",
	L"规则索引",	//"Policy Index",
	L"下一页",	//"Next Page",
	L"不同意",	//Disagree",
	L"同意",	//"Agree",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"鼠标左击",	//"Left Click",
	L"联系佣兵。",	//"to Contact Merc.",
	L"鼠标右击",	//"Right Click",
	L"回到肖像索引。",	//"for Mug Shot Index.",
};

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"生命",
	L"敏捷",
	L"灵巧",
	L"力量",
	L"领导",
	L"智慧",
	L"级别",
	L"枪法",
	L"机械",
	L"爆破",
	L"医疗",

	// the contract expenses' area

	L"费用",	//"Fee",
	L"合同",	//"Contract",
	L"一日",	//"one day",
	L"一周",	//"one week",
	L"两周",	//"two weeks",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"上一位",	//"Previous",
	L"联系",	//"Contact",
	L"下一位",	//"Next",

	L"附加信息",	//"Additional Info",				// Title for the additional info for the merc's bio
	L"现役成员",	//"Active Members",		//20		// Title of the page
	L"可选装备: ",	//"Optional Gear:",				// Displays the optional gear cost
	L"装备",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"所需医疗保证金",	//"MEDICAL deposit required",			// If the merc required a medical deposit, this is displayed
	L"装备1",				// Text on Starting Gear Selection Button 1	// TODO.Translate
	L"装备2",				// Text on Starting Gear Selection Button 2
	L"装备3",				// Text on Starting Gear Selection Button 3
	L"装备4",				// Text on Starting Gear Selection Button 4
	L"装备5",				// Text on Starting Gear Selection Button 5
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"合同总价: ",	//"Contract Charge:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"一日",	//"One Day",
	L"一周",	//"One Week",
	L"两周",	//"Two Weeks",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"不买装备",	//"No Equipment",
	L"购买装备",	//"Buy Equipment",

	// Text on the Buttons

	L"转帐",	//"TRANSFER FUNDS",			// to actually hire the merc
	L"取消",	//"CANCEL",				// go back to the previous menu
	L"雇佣",	//"HIRE",				// go to menu in which you can hire the merc
	L"挂断",	//"HANG UP",				// stops talking with the merc
	L"完成",	//"OK",
	L"留言",	//"LEAVE MESSAGE",			// if the merc is not there, you can leave a message

	//Text on the top of the video conference popup

	L"视频通讯: ",	//"Video Conferencing with",
	L"建立连接……",	//"Connecting. . .",

	L"包括医保",	//"with medical"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"电子转帐成功",	//"ELECTRONIC FUNDS TRANSFER SUCCESSFUL",	// You hired the merc
	L"无法处理转帐",	//"UNABLE TO PROCESS TRANSFER",		// Player doesn't have enough money, message 1
	L"资金不足",	//"INSUFFICIENT FUNDS",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"执行任务中",	//"On Assignment"
	L"请留言",	//"Please Leave Message",
	L"阵亡",	//"Deceased",

	//If you try to hire more mercs than game can support

	L"你的队伍现有18人，已经满员了。",	//"You have a full team of 18 mercs already.",

	L"预录消息",	//"Pre-recorded message",
	L"留言已记录",	//"Message recorded",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M 链接",//	L"A.I.M. Links",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M 历史",	//"A.I.M. History",					//Title

	// Text on the buttons at the bottom of the page

	L"上一页",	//"Previous Page",
	L"主页",	//"Home",
	L"A.I.M 前成员",	//"A.I.M. Alumni",
	L"下一页",	//"Next Page",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"薪金",	//"Price",
	L"级别",	//"Experience",
	L"枪法",	//"Marksmanship",
	L"医疗",	//"Medical",
	L"爆破",	//"Explosives",
	L"机械",	//"Mechanical",

	// The title of the page, the above text gets added at the end of this text

	L"根据%s升序排列的A.I.M成员",	//"A.I.M. Members Sorted Ascending By %s",
	L"根据%s降序排列的A.I.M成员",	//"A.I.M. Members Sorted Descending By %s",

	// Instructions to the players on what to do

	L"鼠标左击",	//"Left Click",
	L"选择佣兵。",	//"To Select Merc",			//10
	L"鼠标右击",	//"Right Click",
	L"回到排序选项。",	//"For Sorting Options",

	// Gets displayed on top of the merc's portrait if they are...

	L"离开",	//"Away",
	L"阵亡",	//"Deceased",						//14
	L"任务中",	//"On Assign",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"第一页",
	L"第二页",
	L"第三页",

	L"A.I.M 前成员",

	L"完成",
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers

	L"A.I.M. 和 A.I.M. 图标在世界大多数国家已经注册。",
	L"版权所有，仿冒必究。",
	L"Copyright 1998-1999 A.I.M., Ltd.  All rights reserved.",

	//Text for an advertisement that gets displayed on the AIM page

	L"联合花卉服务公司",
	L"\"我们将花空运到任何地方\"",				//10
	L"把活干好",
	L"... 第一次",
	L"枪械和杂货，只此一家，别无分店。",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"主页",	//"Home",
	L"成员",	//"Members",
	L"前成员",	//"Alumni",
	L"规则",	//"Policies",
	L"历史",	//"History",
	L"链接",	//"Links",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"库存商品",	//"MERCHANDISE IN STOCK",	//Header for the merchandise available
	L"页面",			//"PAGE",	//The current store inventory page being displayed
	L"总价格",			//"TOTAL COST",		//The total cost of the the items in the Dealer inventory area
	L"总价值",			//"TOTAL VALUE",	//The total value of items player wishes to sell
	L"估价",			//"EVALUATE",		//Button text for dealer to evaluate items the player wants to sell
	L"确认交易",			//"TRANSACTION",	//Button text which completes the deal. Makes the transaction.
	L"完成",			//"DONE",	//Text for the button which will leave the shopkeeper interface.
	L"修理费",			//"REPAIR COST",	//The amount the dealer will charge to repair the merc's goods
	L"1小时",			//"1 HOUR",// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d小时",		//"%d HOURS",// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"已经修好",		//"REPAIRED",// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"你没有空余的位置来放东西了。",	//"There is not enough room in your offer area.",//Message box that tells the user there is no more room to put there stuff
	L"%d分钟",		//"%d MINUTES",	// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"把物品放在地上。",	//"Drop Item To Ground.",
};

//ShopKeeper Interface
//for the bank machine panels. Referenced here is the acronym ATM, which means Automatic Teller Machine

STR16	SkiAtmText[] =
{
	//Text on buttons on the banking machine, displayed at the bottom of the page
	L"0",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"7",
	L"8",
	L"9",
	L"确认",	//"OK",				// Transfer the money
	L"拿",		//"Take",				// Take money from the player
	L"给",		//"Give",				// Give money to the player
	L"取消",	//"Cancel",				// Cancel the transfer
	L"清除",	//"Clear",				// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"选择类型",			//"Select Type",	// tells the user to select either to give or take from the merc
	L"输入数额",		//"Enter Amount",	// Enter the amount to transfer
	L"把钱给佣兵",	//"Transfer Funds To Merc",// Giving money to the merc
	L"从佣兵那拿钱",	//"Transfer Funds From Merc",	// Taking money from the merc
	L"资金不足",		//"Insufficient Funds",	// Not enough money to transfer
	L"余额",			//"Balance",	// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"你要从主帐户中提取%s来支付吗?",
	L"资金不足。你缺少%s。",
	L"你要从主帐户中提取%s来支付吗?",
	L"请求商人开始交易",
	L"请求商人修理选定物品",
	L"结束对话",
	L"当前余额",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"保存游戏",	//"Save Game",
	L"载入游戏",	//"Load Game",
	L"退出",	//"Quit",
	L"下一页",	//L"Next",
	L"上一页",	//L"Prev",
	L"完成",	//"Done",

	//Text above the slider bars
	L"效果音",	//"Effects",
	L"语音",	//"Speech",
	L"音乐",	//"Music",

	//Confirmation pop when the user selects..
	L"退出并回到游戏主菜单？",

	L"你必须选择“语音”和“对话显示”中的至少一项。",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"保存游戏",
	L"载入游戏",
	L"取消",
	L"选择要存档的位置",
	L"选择要读档的位置",

	L"保存游戏成功",
	L"保存游戏错误！",
	L"载入游戏成功",
	L"载入游戏错误！",
 
	L"存档的游戏版本不同于当前的游戏版本。读取它的话很可能游戏可以正常进行。要读取该存档吗？",
	L"存档可能已经无效。你要删除它们吗？",
 
	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"存档版本已改变。如果出现问题请报告。继续？",
#else
	L"试图载入老版本的存档。自动修正并载入存档？",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"存档版本和游戏版本已改变。如果出现问题请报告。继续？",
#else
	L"试图载入老版本的存档。你要自动更新并载入存档吗？",
#endif

	L"你确认你要将#%d位置的存档覆盖吗?",
	L"你要从#号位置载入存档吗",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"你的硬盘空间不够。你现在只有 %dM 可用空间，JA2需要至少 %dM 可用空间。",

	L"保存...",	//"Saving...",			//When saving a game, a message box with this string appears on the screen

	L"普通武器",		//"Normal Guns",
	L"包括前华约武器",	//"Tons of Guns",
	L"现实风格",		//"Realistic style",
	L"科幻风格",		//"Sci Fi style",

	L"难度",		//"Difficulty",
	L"Platinum Mode", //Placeholder English

	L"Bobby Ray's库存",
	L"普通",
	L"较多",
	L"很多",
	L"囧...多",

	L"新携行系统不兼容640x480的屏幕分辨率，请重新设置分辨率。",
	L"新携行系统无法使用默认的 Data 文件夹，请仔细读说明。",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"地图层次",	//"Map Level",
	L"你没有民兵。要拥有民兵，你得先训练镇民。",	//"You have no militia.  You need to train town residents in order to have a town militia.",
	L"每日收入",	//"Daily Income",
	L"佣兵有人寿保险",	//"Merc has life insurance",
	L"%s不疲劳。",	//"%s isn't tired.",
	L"%s行军中，不能睡觉",	//"%s is on the move and can't sleep",
	L"%s太累了，等会儿再试。",	//"%s is too tired, try a little later.",
	L"%s正在开车。",	//"%s is driving.",
	L"有人在睡觉时，整个队伍不能行动。",	//"Squad can't move with a sleeping merc on it.",

	// stuff for contracts
	L"你能支付合同所需费用，但是你的钱不够给该佣兵购买人寿保险。",
	L"要给%s花费保险金%s 以延长保险合同%d天。你要付费吗？",
	L"分区存货",	//"Sector Inventory",
	L"佣兵有医疗保证金。",	//"Merc has a medical deposit.",

	// other items
	L"医生", 	//"Medics", // people acting a field medics and bandaging wounded mercs
	L"病人", 	//"Patients", // people who are being bandaged by a medic
	L"完成", 	//"Done", // Continue on with the game after autobandage is complete
	L"停止", 	//"Stop", // Stop autobandaging of patients by medics now
	L"抱歉。游戏取消了该选项的功能。",
	L"%s没有工具箱。",	//"%s doesn't have a repair kit.",
	L"%s没有医药箱。",	//"%s doesn't have a medical kit.",
	L"现在没有足够的人愿意加入民兵。",
	L"%s的民兵已经训练满了。",	//"%s is full of militia.",
	L"佣兵有一份限时的合同。",	//"Merc has a finite contract.",
	L"佣兵的合同没投保", //"Merc's contract is not insured",
	L"地图概况",//"Map Overview",		// 24
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"你目前没有民兵。当你招募了一些后再返回这个显示模式。",
	L"这个视图显示你的移动民兵能去和不能去哪里。 灰色 = 移动民兵拒绝来这里. 红色 = 移动民兵能来这里, 但是你告诉他们不要这样. 黄色 = 移动民兵能进入这个区域, 但是不能离开. 绿色 = 移动民兵可以自由的进入这里. 右击一个绿色/黄色区域循环它的行为.",
};


STR16 pLandMarkInSectorString[] =
{
	L"第%d小队在%s地区发现有人",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"训练一队民兵要花费$",
	L"同意支付吗？",
	L"你无法支付。",
	L"继续在%s (%s %d)训练民兵吗？",
	L"花费$",
	L"( Y/N )",   // abbreviated yes/no
	L"",	// unused
	L"在%d地区训练民兵将花费$%d。%s",
	L"你无法支付$%d以供在这里训练民兵。",
	L"%s的忠诚度必须达到%d以上方可训练民兵。",
	L"你不能在%s训练民兵了。",
	L"你无法支付$%d以供在这里训练巡逻民兵。", // L"You cannot afford the $%d to train mobile militia here.", // HEADROCK HAM 3.6: Mobile Militia
	L"继续在%s (%s %d)训练巡逻民兵吗？", // L"Continue training mobile militia in %s (%s %d)?", // HEADROCK HAM 3.6: Mobile Militia
	L"在%d地区训练巡逻民兵将花费$%d。%s", // L"Training mobile militia in %d sectors will cost $ %d. %s", // HEADROCK HAM 3.6: Mobile Militia
	L"训练一队民兵要花费$", // L"Training a squad of mobile militia will cost $", // HEADROCK HAM 3.6: Mobile Militia
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"你每次最多能提取$20,000。",
	L"你确认要把%s存入你的帐户吗？",
};

STR16	gzCopyrightText[] = 
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd.  All rights reserved.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"语音",		//"Speech",
	L"确认静默",		//"Mute Confirmations",
	L"显示对话文字",	//"Subtitles",
	L"显示对话文字时暂停",		//"Pause Text Dialogue",
	L"烟火效果",		//"Animate Smoke",
	L"血腥效果",		//"Blood n Gore",
	L"不移动鼠标",		//"Never Move My Mouse!",
	L"旧的选择方式",	//"Old Selection Method",
	L"显示移动路径",	//"Show Movement Path",
	L"显示未击中",		//"Show Misses",
	L"实时确认",		//"Real Time Confirmation",
	L"显示睡觉/醒来时的提示",	//"Display sleep/wake notifications",
	L"使用公制系统",		//"Use Metric System",
	L"佣兵移动时高亮显示",	//"Merc Lights during Movement",
	L"锁定佣兵",	//"Snap Cursor to Mercs",
	L"锁定门",		//"Snap Cursor to Doors",
	L"物品闪亮",	//"Make Items Glow",
	L"显示树冠",	//"Show Tree Tops",
	L"显示轮廓",		//"Show Wireframes",
	L"显示3D光标",	//"Show 3D Cursor",
	L"显示命中机率",	//"Show Chance to Hit on cursor",
	L"榴弹发射器连发时使用枪击光标",	//"GL Burst uses Burst cursor",
	L"允许敌人嘲讽", // Changed from "Enemies Drop all Items" - SANDRO
	L"允许高仰角榴弹发射",	//"High angle Grenade launching",
	L"允许实时潜行", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"按空格键选择下一支小队",	//"Space selects next Squad",
	L"显示物品阴影",	//"Show Item Shadow",
	L"用格数显示武器射程",	//"Show Weapon Ranges in Tiles",
	L"单发曳光弹显示曳光",	//"Tracer effect for single shot",
	L"雨声",	//"Rain noises",
	L"允许乌鸦",	//"Allow crows",
	L"允许士兵工具提示",	// Show Soldier Tooltips
	L"自动存盘",	//"Auto save",
	L"沉默的Skyrider",	//"Silent Skyrider",
	//L"降低CPU的使用率",	//"Low CPU usage",
	L"Enhanced Description Box",
	L"强制回合制模式",						// add forced turn mode
	L"属性进度条",	//L"Stat Progress Bars", // Show progress towards stat increase //ham3.6
	L"替代战略地图颜色", // Change color scheme of Strategic Map
	L"替代子弹图像", // Show alternate bullet graphics (tracers)
	L"开启新的 CTH 系统",				// use NCTH
	L"显示脸部装备图",				
	L"显示脸部装备图标",
	L"禁止光标切换",		            // Disable Cursor Swap		// TODO.Translate
	L"--作弊模式选项--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"强制 Bobby Ray 送货",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--DEBUG 选项--",					// an example options screen options header (pure text)
	L"Report Miss Offsets",					// Screen messages showing amount and direction of shot deviation.	// TODO.Translate
	L"重置所有选项",							// failsafe show/hide option to reset all options
	L"确定要重置？",							// a do once and reset self option (button like effect)
	L"Debug Options in other builds",		// allow debugging in release or mapeditor
	L"DEBUG Render Option group",			// an example option that will show/hide other options
	L"Render Mouse Regions",				// an example of a DEBUG build option
	L"-----------------",					// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"THE_LAST_OPTION",
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	// HEADROCK HAM 4: Added more tooltip text to some toggles, in order to explain them better.

	//speech
	L"如果你想听到人物对话，打开这个选项。",

	//Mute Confirmation
	L"打开或关闭人物的口头确认。",

	//Subtitles
	L"是否显示对话的文字。",

	//Key to advance speech
	L"如果“显示对话文字”已打开，这个选项会让你有足够的时间来阅读NPC的对话。",

	//Toggle smoke animation
	L"如果烟火效果使得游戏变慢，关闭这个选项。",

	//Blood n Gore
	L"如果鲜血使你觉得恶心，关闭这个选项。",

	//Never move my mouse
	L"关闭这个选项会使你的光标自动移到弹出的确认对话框上。",

	//Old selection method
	L"打开这个选项，使用铁血联盟1代的佣兵选择方式。",

	//Show movement path
	L"打开这个选项，会实时显示移动路径(关闭此选项。如果你想要显示路径的话，使用SHIFT键)。",

	//show misses
	L"打开这个选项时，会显示未击中目标的子弹落点。",

	//Real Time Confirmation
	L"当打开时，进入实时模式会有一个确认对话框。",

	//Sleep/Wake notification
	L"当打开时，被分配任务的佣兵睡觉和醒来时会提示你。",

	//Use the metric system
	L"当打开时，使用公制系统，否则使用英制系统。",

	//Merc Lighted movement
	L"当打开时，佣兵移动时会照亮地表。关闭这个选项会使游戏的显示速度变快。",

	//Smart cursor
	L"当打开时，光标移动到佣兵身上时会高亮显示佣兵。",

	//snap cursor to the door
	L"当打开时，光标靠近门时会自动定位到门上。",

	//glow items
	L"当打开时，物品会不断的闪烁。(|I)",

	//toggle tree tops
	L"当打开时，显示树冠。(|T)",

	//toggle wireframe
	L"显示未探明的墙的轮廓。(|W)",

	L"打开时，移动时的光标为3D式样。(|H)",

	// Options for 1.13
	L"当打开时，在光标上显示命中机率。",
	L"开启该选项时，榴弹发射器点射使用点射的准星",
	L"当启用时，敌人行动中有时会带有对白。", // Changed from Enemies Drop All Items - SANDRO
	L"当打开时，榴弹发射器允许采用较高仰角发射榴弹。(|Q)",
	L"当启用时, 潜行时没有被敌人发现时不会进入回合制模式，除非按下 |C|t|r+|X.", // Changed from Restrict Extra Aim Levels - SANDRO
	L"当打开时，按空格键自动切换到下一小队。(|S|p|a|c|e)",
	L"开启该选项时，会显示物品阴影",
	L"当打开时，用格数显示武器射程。",
	L"当打开时，单发曳光弹也显示曳光。",
	L"当打开时，下雨时能听到雨水音效。",	//"When ON, you will hear rain noises when it is raining.",
	L"当打开时，允许乌鸦出现。",
	L"当启用时，把光标定位在敌人身上并且按下 |A|l|t 键会显示一个提示工具窗口。",
	L"当打开时，游戏将在玩家回合后自动存盘",
	L"当打开时，Skyrider将保持沉默。",
	//L"当打开时，游戏将使用更少的CPU资源。",
	L"当打开时，将出现物品及武器的“增强描述框”（EDB）。",
	L"当打开时，且在战术画面内存在敌军时，将一直处于回合制模式直至该地区所有敌军被消灭（可以通过快捷键|C|T|R|L+|S|H|I|F|T+|A|L|T+|T来控制打开或关闭强制回合制模式）",
	L"当打开时，可显示各属性的成长进度。", // L"When ON, shows character progress towards gaining levels.", //ham3.6
	L"当启用时, 战略地图将会根据探索状态显示不同的着色。",
	L"当启用时, 当你射击时会显示间隔子弹图像。", // TODO.Translate
	L"当启用时, 使用新命中率系统和光标。",
	L"当启用时, 将会看到佣兵脸部装备图。",
	L"当启用时, 将在佣兵肖像右下角显示脸部装备图标",
	L"当启用时，在交换位置和其它动作时光标不切换。键入 |x 可以快速切换。", // L"When ON, the cursor will not toggle between exchange position and other actions. Press |x to initiate quick exchange.",	// TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"强制 Bobby Ray 出货",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER", // an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: 当启动时, 将报告每个子弹偏离目标中心点的距离，考虑各种NCTH因素。",
	L"点击这里修复损坏的游戏设置", // failsafe show/hide option to reset all options
	L"点击这里修复损坏的游戏设置", // a do once and reset self option (button like effect)
	L"在建立releas或mapeditor时，允许调试操作", // allow debugging in release or mapeditor
	L"切换以显示调试渲染选项", // an example option that will show/hide other options
	L"尝试在鼠标周围地区显示斜线矩形", // an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END", // an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};


STR16	gzGIOScreenText[] =
{
	L"游戏初始设置",
	L"游戏风格",
	L"现实",
	L"科幻",
	L"金版",
	L"武器数量", // changed by SANDRO
	L"大量武器",
	L"少量武器", // changed by SANDRO
	L"难度",
	L"新手",
	L"老手",
	L"专家",
	L"疯子",
	L"确定",	// TODO.Translate
	L"取消",
	L"额外难度",
	L"可随时存盘",
	L"铁人模式",
	L"在Demo中禁用",
	L"Bobby Ray's 库存",
	L"普通",
	L"较多",
	L"很多",
	L"囧……多",
	L"携行系统 / 附件系统",	// TODO.Translate
	L"NOT USED",
	L"NOT USED",
	L"读取联机游戏",
	L"游戏初始设置（仅在服务器设置时有效）",
	// Added by SANDRO
	L"特殊技能",
	L"旧",
	L"新",
	L"IMP 数量",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"敌人物品全掉",
	L"关",
	L"开",
	L"通缉犯出现几率",
	L"随机",
	L"全部",
	L"武器弹药储藏出现几率",
	L"随机",
	L"全部",
	L"装备进展速度",
	L"超慢",
	L"慢",
	L"一般",
	L"快",
	L"超快",

	L"旧 / 旧",
	L"新 / 旧",
	L"新 / 新",
};

STR16	gzMPJScreenText[] =
{
	L"多人游戏",//L"MULTIPLAYER",
	L"加入",//L"Join",
	L"主机",//L"Host",
	L"取消",//L"Cancel",
	L"刷新",//L"Refresh",
	L"玩家名称",//L"Player Name",
	L"服务器 IP",//L"Server IP",
	L"端口",//L"Port",
	L"服务器名",//L"Server Name",
	L"# Plrs",
	L"版本",//L"Version",
	L"游戏类型",//L"Game Type",
	L"Ping",
	L"你必须输入你的玩家名称。",//L"You must enter a player name.",
	L"你必须输入有效的服务器IP地址。 (例如 84.114.195.239).",//L"You must enter a valid server IP address.\n (eg 84.114.195.239).",
	L"您必须输入正确的服务器端口，范围1~65535。",//L"You must enter a valid Server Port between 1 and 65535.",
};

STR16 gzMPJHelpText[] =
{
	L"Visit http://webchat.quakenet.org/?channels=ja2-multiplayer to find other players.",

	L"HOST",
	L"Enter '127.0.0.1' for the IP and the Port number should be greater than 60000.",
	L"Be sure that the Port (UDP, TCP) is forwarded on your Router. For more information see: http://portforward.com",
	L"You have to send (via IRC, ICQ, etc) your external IP (http://www.whatismyip.com) and the Port number to the other players.",
	L"Click on 'Host' to host a new Multiplayer Game.",
	
	L"JOIN",
	L"The host has to send (via IRC, ICQ, etc) you the external IP and the Port number.",
	L"Enter the external IP and the Port number from the host.",
	L"Click on 'Join' to join an already hosted Multiplayer Game.",
};

STR16	gzMPHScreenText[] =
{
	L"建立主机",//L"HOST GAME",
	L"开始",//L"Start",
	L"取消",//L"Cancel",
	L"服务器名",//L"Server Name",
	L"游戏类型",//L"Game Type",
	L"死亡模式",//L"Deathmatch",
	L"团队死亡模式",//L"Team Deathmatch",
	L"合作模式",//L"Co-operative",
	L"最大玩家数",//L"Max Players",
	L"小队规模",//L"Squad Size",
	L"选择佣兵",//L"Merc Selection",
	L"随机佣兵",//L"Random Mercs",
	L"已被雇佣",//L"Hired by Player",
	L"起始平衡",//L"Starting Cash",
	L"可以雇佣相同佣兵",//L"Can Hire Same Merc",
	L"Report Hired Mercs",
	L"开启Bobby Rays网上商店",
	L"Sector Starting Edge",
	L"必须输入服务器名",
	L"",
	L"",
	L"开始时间",
	L"",
	L"",
	L"武器伤害",
	L"",
	L"计时器",
	L"",
	L"合作模式中允许平民",
	L"",
	L"Maximum Enemies in CO-OP",
	L"同步游戏目录",
	L"同步多人模式目录",
	L"你必须进入一个文件传输目录.",
	L"（使用 '/' 代替 '\\' 作为目录分隔符）",
	L"指定的同步目录不存在。",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	// Max. Enemies / Report Hired Merc / Enable Civs in CO-OP
	L"Yes",
	L"No",
	// Starting Time
	L"Morning",
	L"Afternoon",
	L"Night",
	// Starting Cash
	L"Low",
	L"Medium",
	L"Heigh",
	L"Unlimited",
	// Time Turns
	L"Never",
	L"Slow",
	L"Medium",
	L"Fast",
	// Weapon Damage
	L"Very low",
	L"Low",
	L"Normal",
	// Merc Hire
	L"Random",
	L"Normal",
	// Sector Edge
	L"Random",
	L"Selectable",
	// Bobby Ray / Hire same merc
	L"Disable",
	L"Allow",
};

STR16 pDeliveryLocationStrings[] =
{
	L"奥斯汀",	//"Austin",			//Austin, Texas, USA
	L"巴格达",	//"Baghdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"香港",	//"Hong Kong",		//Hong Kong, Hong Kong
	L"贝鲁特",	//"Beirut",			//Beirut, Lebanon	(Middle East)
	L"伦敦",	//"London",			//London, England
	L"洛杉矶",	//"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
	L"迈阿密",	//"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"莫斯科",	//"Moscow",			//Moscow, USSR
	L"纽约",	//"New York",		//New York, New York, USA
	L"渥太华",	//"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"巴黎",	//"Paris",				//Paris, France
	L"的黎波里",	//"Tripoli",			//Tripoli, Libya (eastern Mediterranean)
	L"东京",	//"Tokyo",				//Tokyo, Japan
	L"温哥华",	//"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"你确定吗？零意味着你不能拥有这项技能。",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"(最多8个字符)",	//"( 8 Characters Max )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"分析...",
};

STR16 pIMPFinishStrings[ ]=
{
	L"谢谢你，%s",
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"嗓音",
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"阵亡",	//"Killed in Action",
	L"解雇",		//"Dismissed",
	L"其它",		//"Other",
};

// title for program
STR16 pPersTitleText[] =
{
	L"人事管理",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"游戏暂停",	//"Game Paused",
	L"继续游戏 (|P|a|u|s|e)",	//"Resume Game (|P|a|u|s|e)",
	L"暂停游戏 (|P|a|u|s|e)",	//"Pause Game (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"退出游戏",	//"Exit Game?",
	L"确定",	//"OK",
	L"是",	//"YES",
	L"否",	//"NO",
	L"取消",	//"CANCEL",
	L"再次雇佣",	//"REHIRE",
	L"撒谎",	//"LIE",		//
	L"没有描述",	//"No description", //Save slots that don't have a description.
	L"游戏已保存。",	//"Game Saved.",
	L"游戏已保存。",	//"Game Saved.",
	L"QuickSave", 	//"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//"SaveGame",//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"日",	//"Day",
	L"个佣兵",	//"Mercs",
	L"空",	//"Empty Slot", //An empty save game slot
	L"Demo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"Release",			//Release build for JA2
	L"rpm",					//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"分钟",	//"min",					//Abbreviation for minute.
	L"米",	//"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"发",  //L"rnds",				//Abbreviation for rounds (# of bullets)
	L"公斤",	//"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"磅",	//"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"主页",	//"Home",				//Home as in homepage on the internet.
	L"USD", //L"USD",					//Abbreviation to US dollars
	L"n/a",					//Lowercase acronym for not applicable.
	L"与此同时",	//"Meanwhile",	//Meanwhile
	L"%s已到达%s%s分区",	//"%s has arrived in sector %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying																		//SirTech

	L"版本", //L"Version",
	L"没有快速存档",	//"Empty Quick Save Slot",
	L"该位置用来放Quick Save（快速存档）。请在战术屏幕或者地图屏幕按ALT+S进行快速存档。",
	L"打开的",	//"Opened",
	L"关闭的",	//"Closed",
	L"磁盘空间不足。只有%sMB可用空间，《铁血联盟2》需要%sMB。",
	L"从AIM雇佣了%s", 		//"Hired %s from AIM",
	L"%s抓住了%s.",	//"%s has caught %s.",	//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s打了针剂。", 	//"%s has taken the drug.", //'Merc name' has taken the drug
	L"%s没有医疗技能",	//"%s has no medical skill",//'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"The integrity of the game has been compromised.",
	L"ERROR: Ejected CD-ROM",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"没有空间施展你的武器。",	//"There is no room to fire from here.",

	//Can't change stance due to objects in the way...
	L"现在无法改变姿势。",	//"Cannot change stance at this time.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"放下",	//"Drop",
	L"投掷",	//"Throw",
	L"交给",	//"Pass",

	L"把%s交给了%s。", //"%s passed to %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"没有足够空位以把%s交给%s",	//"No room to pass %s to %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L"附加 )",	//" Attached )",

	//Cheat modes
	L"开启作弊等级一",	//"Cheat level ONE reached",
	L"开启作弊等级二",	//"Cheat level TWO reached",

	//Toggling various stealth modes
	L"小队进入潜行模式。",	//"Squad on stealth mode.",
	L"小队退出潜行模式。",	//"Squad off stealth mode.",
	L"%s进入潜行模式。",	//"%s on stealth mode.",
	L"%s退出潜行模式。",	//"%s off stealth mode.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"打开显示轮廓",	//"Extra Wireframes On",
	L"关闭显示轮廓",	//"Extra Wireframes Off",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.
	L"无法从这层上去...",	//"Can't go up from this level...",
	L"没有更低的层了...",	//"There are no lower levels...",
	L"进入地下室%d层...",	//"Entering basement level %d...",
	L"离开地下室...",			//"Leaving basement...",

	L"的",	//"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"关闭跟随模式。",	//"Follow mode OFF.",
	L"打开跟随模式。",	//"Follow mode ON.",
	L"不显示3D光标。",	//"3D Cursor OFF.",
	L"显示3D光标。",	//"3D Cursor ON.",
	L"第%d小队激活。",	//"Squad %d active.",
	L"你无法支付%s的%s日薪",	//"You cannot afford to pay for %s's daily salary of %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"跳过",			//"Skip",
	L"%s不能独自离开",	//"%s cannot leave alone.",
	L"一个文件名为SaveGame99.sav的存档被创建了。如果需要的话，将其更名为SaveGame01 - SaveGame10，然后你就能载入这个存档了。",	//"A save has been created called, SaveGame99.sav.  If needed, rename it to SaveGame01 - SaveGame10 and then you will have access to it in the Load screen.",
	L"%s喝了点%s",	//"%s drank some %s",
	L"Drassen收到了包裹。",	//"A package has arrived in Drassen.",
 	L"%s将到达指定的着陆点(分区%s)， 于%d日%s。",	//"%s should arrive at the designated drop-off point (sector %s) on day %d, at approximately %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"日志已经更新。",	//"History log updated.",
	L"榴弹发射器点射时使用准星光标（可以扫射）",
	L"榴弹发射器连发时使用弹道光标(不可以扫射",	//"Grenade Bursts use Trajectory Cursor (Spread fire disabled)",
	L"开启敌兵装备提示", // Changed from Drop All On - SANDRO
	L"关闭敌兵装备提示", // Changed from Drop All Off - SANDRO
	L"榴弹发射器以正常仰角发射榴弹",	//"Grenade Launchers fire at standard angles",
	L"榴弹发射器以较高仰角发射榴弹",	//L"Grenade Launchers fire at higher angles",
	// forced turn mode strings
    L"强制回合制模式",
	L"正常回合制模式",
	L"离开战斗",
	L"强制回合制模式启动，进入战斗",
#ifdef JA2BETAVERSION
	L"自动储存成功。",
#endif
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.//84
	L"客户端", //"Client",
	L"旧携行系统不能与新附件系统同时使用.",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"确认",		//"OK",
	L"向上滚动",		//"Scroll Up",
	L"选择全部",		//"Select All",
	L"向下滚动",		//"Scroll Down",
	L"取消",		//"Cancel",
};

STR16 pDoctorWarningString[] =
{
	L"%s不够近，不能被治疗。",
	L"你的医生不能包扎完每个人。",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"调离(右击)/安置(左击)新兵",
	L"调离(右击)/安置(左击)老兵",
	L"调离(右击)/安置(左击)精兵",
	L"在各个分区平均分布民兵",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"去AIM雇几个佣兵( *提示* 在笔记本电脑里)",
	L"当你准备出发前往Arulco，点击屏幕右下方的时间压缩按钮。",
};

STR16 pAntiHackerString[] = 
{
	L"错误。丢失或损坏文件。游戏将退出。",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"查看邮件",
	L"浏览网页",
	L"查看文件和邮件的附件",
	L"阅读事件日志",
	L"查看队伍信息",
	L"查看财务简报和记录",
	L"关闭笔记本电脑",

	//Bottom task bar icons (if they exist):
	L"你有新的邮件",
	L"你有新的文件",

	//Bookmarks:
	L"国际佣兵联盟",
	L"Bobby Ray网上武器店",
	L"佣兵心理剖析研究所",
	L"廉价新兵中心",
	L"McGillicutty公墓",
	L"联合花卉服务公司",
	L"A.I.M指定保险代理人",
};


STR16	gzHelpScreenText[] =
{
	L"退出帮助屏幕",
};

STR16 gzNonPersistantPBIText[] =
{
	L"战斗正在进行中，你只能在战术屏幕进行撤退。",
	L"进入该分区，继续战斗。(|E)",
	L"自动解决这次战斗。(|A)",
	L"当你进攻时，不能自动解决战斗。",
	L"当你遭遇伏兵时，不能自动解决战斗。",
	L"当在矿井里和异形作战时，不能自动解决战斗。",
	L"还有敌对的平民时，不能自动解决战斗。",
	L"有血猫时，不能自动解决战斗。",
	L"战斗进行中",
	L"你不能在这时撤退。",
};

STR16 gzMiscString[] =
{
	L"在没有你的佣兵的支援下，民兵继续战斗...",
	L"现在车辆不需要加油。",	//"The vehicle does not need anymore fuel right now.",
	L"油箱装了%d％的油。",	//"The fuel tank is %d%% full.",
	L"Deidranna女王的军队重新完全占领了%s。",
	L"你丢失了加油点。",	//"You have lost a refueling site.",
};

STR16	gzIntroScreen[] = 
{
	L"找不到视频文件",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s听到%s声音来自%s。",
	L"%s听到%s移动声来自%s。",
	L"%s听到%s吱吱声来自%s。",
	L"%s听到%s溅水声来自%s。",
	L"%s听到%s撞击声来自%s。",
	L"%s听到%s爆炸声发向%s。",
	L"%s听到%s尖叫声发向%s。",
	L"%s听到%s撞击声发向%s。",
	L"%s听到%s撞击声发向%s。",
	L"%s听到%s粉碎声来自%s。",
	L"%s听到%s破碎声来自%s。",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"按姓名排序 (|F|1)",
	L"按任务排序 (|F|2)",
	L"按睡眠状态排序 (|F|3)",
	L"按地点排序 (|F|4)",
	L"按目的地排序 (|F|5)",
	L"按预计离队时间排序 (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"错误404",	//"Error 404",
	L"网站未找到",	//"Site not found.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"近期运货",	//"Recent Shipments",
	L"订单 #",		//"Order #",
	L"物品数量",	//"Number Of Items",
	L"订购日期",		//"Ordered On",
};


STR16	gzCreditNames[]=
{
	L"Chris Camfield",
	L"Shaun Lyng",
	L"Kris M鋜nes",
	L"Ian Currie",
	L"Linda Currie",
	L"Eric \"WTF\" Cheng",
	L"Lynn Holowka",
	L"Norman \"NRG\" Olsen",
	L"George Brooks",
	L"Andrew Stacey",
	L"Scot Loving",
	L"Andrew \"Big Cheese\" Emmons",
	L"Dave \"The Feral\" French",
	L"Alex Meduna",
	L"Joey \"Joeker\" Whelan",
};


STR16	gzCreditNameTitle[]=
{
	L"Game Internals Programmer", 			// Chris Camfield
	L"Co-designer/Writer",							// Shaun Lyng
	L"Strategic Systems & Editor Programmer",					//Kris \"The Cow Rape Man\" Marnes
	L"Producer/Co-designer",						// Ian Currie
	L"Co-designer/Map Designer",				// Linda Currie
	L"Artist",													// Eric \"WTF\" Cheng
	L"Beta Coordinator, Support",				// Lynn Holowka
	L"Artist Extraordinaire",						// Norman \"NRG\" Olsen
	L"Sound Guru",											// George Brooks
	L"Screen Designer/Artist",					// Andrew Stacey
	L"Lead Artist/Animator",						// Scot Loving
	L"Lead Programmer",									// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmer",											// Dave French
	L"Strategic Systems & Game Balance Programmer",					// Alex Meduna
	L"Portraits Artist",								// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(still learning punctuation)",					// Shaun Lyng
	L"(\"It's done. I'm just fixing it\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(getting much too old for this)",				// Ian Currie
	L"(and working on Wizardry 8)",						// Linda Currie
	L"(forced at gunpoint to also do QA)",			// Eric \"WTF\" Cheng
	L"(Left us for the CFSA - go figure...)",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(Dead Head and jazz lover)",						// Andrew Stacey
	L"(his real name is Robert)",							// Scot Loving
	L"(the only responsible person)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(can now get back to motocrossing)",	// Dave French
	L"(stolen from Wizardry 8)",							// Alex Meduna
	L"(did items and loading screens too!)",	// Joey \"Joeker\" Whelan",
};

// HEADROCK: Adjusted strings for better feedback, and added new string for LBE repair.
STR16 sRepairsDoneString[] =
{
	L"%s 修复了自己的物品。",
	L"%s 修复了所有人的枪和护甲。",
	L"%s 修复了所有人的装备。",
	L"%s 修复所有人携带的大型物品",//L"%s finished repairing everyone's large carried items",
	L"%s 修复所有人携带的中型物品",//L"%s finished repairing everyone's medium carried items",
	L"%s 修复所有人携带的小型物品",//L"%s finished repairing everyone's small carried items",
	L"%s 修复所有人的携行具",//L"%s finished repairing everyone's LBE gear",
};

STR16 zGioDifConfirmText[]=
{
	L"你选择了“新手”模式。这个设置是为那些刚玩铁血联盟的玩家准备的，他们刚接触策略游戏，或者他们希望快点结束战斗。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“新手”模式吗？",
	L"你选择了“老手”模式。这个设置是为那些已经熟悉铁血联盟或类似游戏的玩家准备的。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“老手”模式吗？",
	L"你选择了“专家”模式。我们警告你，如果你被装在尸袋里运回来，不要来向我们抱怨。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“专家”模式吗？",
	L"你选择了“疯狂”模式。警告: 如果你被装在塑料袋里一块块运回来，不要来向我们抱怨。女王会狠狠地凌虐你。你的选择会在整个游戏中生效，所以请作出明智的选择。你真的要玩“疯狂”模式吗？",
};

STR16 gzLateLocalizedString[] =
{
	L"没有找到loadscreen数据文件%S...",	//"%S loadscreen data file not found...",

	//1-5
	L"由于没有人在用遥控器，机器人无法离开本分区。",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"你现在无法压缩时间。请等待炸弹爆炸！",

	//'Name' refuses to move.
	L"%s拒绝移动。",

	//%s a merc name
	L"%s精力不足，无法改变姿势。",	//"%s does not have enough energy to change stance.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s汽油耗尽，现在在%c%d抛锚了。",	//"The %s has run out of gas and is now stranded in %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"上方",
	L"下方",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"佣兵中没人有医疗技能。",
	L"没有足够的医疗物品进行包扎。",
	L"没有足够的医疗物品给所有人进行包扎。",
	L"佣兵中没人需要包扎。",	//"None of your mercs need bandaging.",
	L"自动包扎佣兵。",	//"Bandages mercs automatically.",
	L"全部佣兵已被包扎完毕。",	//"All your mercs are bandaged.",

	//14
	L"Arulco",

	L"屋顶",

	L"生命: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d vs. %d",
	
	L"%s满了。",
 
  L"%s现在不用包扎，他（她）需要认真的治疗和休息。",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s被击中腿部，并且倒下了！",
	//Name can't speak right now.
	L"%s现在不能说话。",

	//22-24 plural versions
	L"%d个新兵被提升为精兵。",
	L"%d个新兵被提升为老兵。",
	L"%d个老兵被提升为精兵。",

	//25
	L"开关",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%疯狂了！",

	//27-28
	//Messages why a player can't time compress.
	L"现在压缩时间不安全，因为你有佣兵在分区%s。",
	L"现在压缩时间不安全，因为你有佣兵在被异形感染的矿井。",

	//29-31 singular versions
	L"1个新兵被晋升为精兵。",
	L"1个新兵被晋升为老兵。",
	L"1个老兵被晋升为精兵。",

	//32-34
	L"%s无语。",	//"%s doesn't say anything.",
	L"回到地面？",
	L"(第%d小队)",	//"(Squad %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s修复了%s的%s。",

	//36
	L"血猫",

	//37-38 "Name trips and falls"
	L"%s踩到陷阱，跌倒了。",
	L"这个物品不能从这里捡起。",

	//39
	L"你现有的佣兵中没人能进行战斗。民兵们将独自和异形作战。",

	//40-43
	//%s is the name of merc.
	L"%s用完了医药箱里的药品！",	//"%s ran out of medical kits!",
	L"%s没有所需技能来医疗他人！",	//"%s lacks the necessary skill to doctor anyone!",
	L"%s用完工具箱里的工具！",	//"%s ran out of tool kits!",
	L"%s没有所需技能来修理物品！",	//"%s lacks the necessary skill to repair anything!",

	//44-45
	L"Repair Time",
	L"%s看不到这个人。",

	//46-48
	L"%s的增程枪管掉下来了！",	//"%s's gun barrel extender falls off!",
	// HEADROCK HAM 3.5: Changed to reflect facility effect.
	L"只允许不多于%d名佣兵在这个分区训练民兵。",	//"No more than %d militia trainers are permitted per sector.", //L"No more than %d militia trainers are permitted in this sector.",//ham3.6
  	L"你确定吗？",			//"Are you sure?",

	//49-50
	L"时间压缩",
	L"车辆的油箱已经加满油了。",

	//51-52 Fast help text in mapscreen.
	L"继续时间压缩 (|S|p|a|c|e)",
	L"停止时间压缩 (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11"
	L"%s修理好了卡壳的%s",	//L"%s has unjammed the %s",
	L"%s修理好了卡壳的%s的%s",	//L"%s has unjammed %s's %s",

	//55
	L"查看分区存货时候无法压缩时间",	//L"Can't compress time while viewing sector inventory.",

	L"The Jagged Alliance 2 v1.13 PLAY DISK was not found. Program will now exit.",

	L"物品组合成功。",
 
	//58
	//Displayed with the version information when cheats are enabled.
	L"当前/最大进展:  %d%％/%d%％",		//"Current/Max Progress: %d%%/%d%%",//zww
 
	L"护送John和Mary？",
 
	// 60
	L"开关被激活",		//"Switch Activated.",

	L"%s的陶瓷片已经粉碎了!",	//"%s's ceramic plates have been smashed!",
	L"%s多打了%d发子弹！",	//"%s fires %d more rounds than intended!",
	L"%s多打了1发子弹！",	//"%s fires %d more round than intended!",
};

// HEADROCK HAM 3.5: Added sector name
STR16 gzCWStrings[] = 
{
	L"是否呼叫邻近区域的援兵到%s？", //L"Call reinforcements to %s from adjacent sectors?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|位|置: %d\n",
	L"%s|亮|度: %d / %d\n",
	L"%s|目|标|距|离: %d\n",
	L"%s|I|D: %d\n",
	L"%s|订|单: %d\n",
	L"%s|属|性: %d\n",
	L"%s|当|前 |A|Ps: %d\n",
	L"%s|当|前 |生|命: %d\n",
	// Full info
	L"%s|头|盔: %s\n",
	L"%s|防|弹|衣: %s\n",
	L"%s|作|战|裤: %s\n",
	// Limited, Basic
	L"|护|甲: ",
	L"头盔 ",
	L"防弹衣 ",
	L"作战裤",
	L"装备了",
	L"无护甲",
	L"%s|夜|视|仪: %s\n",
	L"无夜视仪",
	L"%s|防|毒|面|具: %s\n",
	L"无防毒面具",
	L"%s|头|部|1: %s\n",
	L"%s|头|部|2: %s\n",
	L"\n(背包内) ",
	L"%s|武|器: %s ",
	L"空手",
	L"手枪",
	L"冲锋枪",
	L"步枪",
	L"机枪",
	L"霰弹枪",
	L"刀",
	L"重武器",
	L"无头盔",
	L"无防弹衣",
	L"无作战裤",
	L"|护|甲: %s\n",
	 // Added - SANDRO
	L"%s|技能 1: %s\n",
	L"%s|技能 2: %s\n", 
	L"%s|技能 3: %s\n",
};

STR16 New113Message[] =
{
	L"风暴开始了。",
	L"风暴结束了。",
	L"下雨了。",
	L"雨停了。",
	L"小心狙击手……",
	L"火力压制！",
	L"点射",
	L"自动",
	L"榴弹",
	L"榴弹点射",
	L"榴弹自动",
	L"狙击手！",
	L"已经点选物品，此时无法分钱。",
	L"新兵的会合地被挪至 %s, 因降落地点 %s 目前由敌人占据。",
	L"物品销毁",
	L"此类物品全部销毁",
	L"物品卖出",
	L"此类物品全部卖出",
	L"你得检查一下你的眼部装备",
	// Real Time Mode messages
	L"进入战斗模式",
	L"视野中没有敌人",
	L"即时潜行模式 关闭",
	L"即时潜行模式 开启",
	//L"Enemy spotted! (Ctrl + x to enter turn based)",
	L"发现敌人！", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s 偷窃成功！",// L"%s was successful on stealing!",
	L"%s 没有足够的行动点来偷取所有所选物品。",// L"%s had not enough action points to steal all selected items.",
	L"是否在包扎前对 %s 实施手术？（可回复 %i 生命。）",// L"Do you want to make surgery on %s before bandaging? (You can heal about %i Health.)",
	L"是否对 %s 实施手术？（可回复 %i 生命。）",// L"Do you want to make surgery on %s? (You can heal about %i Health.)",
	L"是否进行必要的手术？（%i 个伤者）",// L"Do you wish to make necessary surgeries first? (%i patient(s))",
	L"是否在该伤者身上进行手术？",// L"Do you wish to make the surgery on this patient first?",
	L"在包扎前是否进行手术？",// L"Apply first aid automatically with necessary surgeries or without them?",
	L"%s 手术完毕。",// L"Surgery on %s finished.",
	L"%s 胸部中弹，失去1点生命上限！",// L"%s is hit in the chest and loses a point of maximum health!",
	L"%s 胸部中弹，失去 %d 点生命上限！",// L"%s is hit in the chest and loses %d points of maximum health!",
	L"%s 重获1点失去的 %s",// L"%s has regained one point of lost %s",
	L"%s 重获 %d 点失去的 %s",// L"%s has regained %d points of lost %s",
	L"你的侦察能力避免了敌人的偷袭！",// L"Your scouting skills prevented you to be ambushed by the enemy!",
	L"多亏了你的侦查技能，你成功的避开了大群血猫！",// L"Thanks to your scouting skills you have successfuly avoided a pack of bloodcats!",
	L"%s 命根子中弹，痛苦的倒下了！",// L"%s is hit to groin and falls down in pain!",	
	//////////////////////////////////////////////////////////////////////////////////////
	L"注意: 敌人尸体被发现!!!",
	L"%s [%d rnds]\n%s %1.1f %s",
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s 害怕得退缩了！",//	L"%s cowers in fear!",
	L"%s 被压制住了！",// L"%s is pinned down!", //ham3.6
	L"%s 多打了几发子弹！",// L"%s fires more rounds than intended!",
	L"你不能在这个地区训练民兵。",// L"You cannot train militia in this sector.",
	L"民兵拾起 %s。",// L"Militia picks up %s.",
	L"有敌人出没时无法训练民兵！", // L"Cannot train militia with enemies present!", 
	// 6 - 10
	L"%s缺乏训练民兵所需要的领导能力。",// L"%s lacks sufficient Leadership score to train militia.",
	L"此地训练民兵的教官不能超过%d名。",// L"No more than %d Mobile Militia trainers are permitted in this sector.",
	L"%s和周边地区的游击队已经满员了！",// L"No room in %s or around it for new Mobile Militia!",
	L"你需要有至少%d个民兵在%s每个被解放的地区才能训练游击队。",// L"You need to have %d Town Militia in each of %s's liberated sectors before you can train Mobile Militia here.",
	L"有敌人出没时不能在任何设施内工作！",// L"Can't staff a facility while enemies are present!",
	// 11 - 15
	L"%s缺乏就任于该设施所需智慧。",// L"%s lacks sufficient Wisdom to staff this facility.",
	L"%s已经满员了。",// L"The %s is already fully-staffed.",
	L"使用该设施每小时消耗$%d，你确定吗？",// L"It will cost $%d per hour to staff this facility. Do you wish to continue?",
	L"你没有足够的资金来支付今天的设施费用。付出$%d，还欠$%d，当地人很不满。",// L"You have insufficient funds to pay for all Facility work today. $%d have been paid, but you still owe $%d. The locals are not pleased.",
	L"没有足够的资金来支付今天的设施费用。欠款$%d，当地人很不满.。",// L"You have insufficient funds to pay for all Facility work today. You owe $%d. The locals are not pleased.",
	// 16 - 20
	L"你仍有$%d的欠款，同时你已经身无分文了！",// L"You have an outstanding debt of $%d for Facility Operation, and no money to pay it off!",
	L"你仍有$%d的欠款，在有钱还清这笔债务之前你不能分配雇佣兵去这个设施。",// L"You have an outstanding debt of $%d for Facility Operation. You can't assign this merc to facility duty until you have enough money to pay off the entire debt.",
	L"你有$%d的欠款，是否支付？",// L"You have an outstanding debt of $%d for Facility Operation. Would you like to pay it all back?",
	L"这个区域没有",// L"N/A in this sector",
	L"日常支出",// L"Daily Expenses",
	// 21 - 25
	L"维持民兵的资金不足！%d名民兵回老家结婚去了。",// L"Insufficient funds to pay all enlisted militia! %d militia have disbanded and returned home.",

};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"鉴于Gaston最近发挥异常突出，他的服务费也跟着上涨。以我个人的观点，这一点也不让我惊讶。 ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"请注意，Stogie近期能力有所提升，他的价格也要随之增长。 ± ± Speck T. Kline ± ",
	// Tex: Text from Line 45 in Email.edt
	L"请注意，因为Tex新获得的经验， 更高的身价才匹配他的能力。 ± ± Speck T. Kline ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"鉴于Biggins发挥有所提高， 他的价格也同时上涨。 ± ± Speck T. Kline ± ",
};

// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"转发自AIM服务器：Victor Kolesnikov的信件",
	L"你好，这是Monk，留言已收到。我已经回来了，你可以联系我了。 ± ± 等你的电话。 ± ",

	// Brain: Text from Line 60
	L"转发自AIM服务器：Janno Allik的信件",
	L"我已经准备好接受任务了。我有空干任何事情 ± ± Janno Allik ± ",

	// Scream: Text from Line 62
	L"转发自AIM服务器：Lennart Vilde的信件",
	L"Lennart Vilde已经准备好了！ ± ",

	// Henning: Text from Line 64
	L"转发自AIM服务器：Henning von Branitz的信件",
	L"你的留言我已收到，谢谢。请到AIM主页联系我，然后讨论招募事宜。 ± ± 那时见！ ± ± Henning von Branitz ± ",

	// Luc: Text from Line 66
	L"转发自AIM服务器：Luc Fabre的信件",
	L"收到留言，Merci（谢谢）！ 你能考虑我我非常高兴。你知道在哪里能找到我。 ± ± 希望能收到你的电话。 ± ",

	// Laura: Text from Line 68
	L"转发自AIM服务器：Dr. Laura Colin的信件",
	L"你好！非常高兴你能给我留言，我很感兴趣。 ± ± 请再上AIM，我愿意听听详细事宜 ± ± 此致敬礼！ ± ± Dr. Laura Colin ± ",

	// Grace: Text from Line 70
	L"转发自AIM服务器：Graziella Girelli的信件",
	L"你上次想联系我但是没有成功。± ± 你懂得？家庭聚会啦。我现在已经厌倦了我的家庭，你能再上AIM联系我的话我会非常高兴 ± ± Ciao（再见）！ ± ",

	// Rudolf: Text from Line 72
	L"转发自AIM服务器：Rudolf Steiger的信件",
	L"你知道我每天有多少个电话要接吗？每个蠢货都认为他可以Call我。 ± ± 反正我回来了，前提是你真的有有趣的工作给我的话。 ± ",
	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"转发自AIM服务器：Message about merc availability",
	L"我收到你的留言，等你联系。 ± ",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"狙击手: 拥有鹰的眼睛和百步穿杨的枪法！± ",
	// Camouflage
	L"伪装: 跟你的伪装迷彩比起来，树丛看起来倒象是假的！± ",
	// SANDRO - new strings for new traits added
	// MINTY - Altered the texts for more natural English, and added a little flavour too
	// Ranger
	L"猎人：你跟那些从德克萨斯来的土包子可大不一样！ ± ",
	// Gunslinger
	L"快枪手：拿着一两把手枪，你可以和Billy the Kid一样致命！ ± ",
	// Squadleader
	L"领队：天生的领袖，你的队员需要你的灵感！ ± ",
	// Technician
	L"技师：你比MacGyver牛逼多了！无论机械、电子产品还是爆破物，你都能修！ ± ",
	// Doctor
	L"医生：从轻微擦伤到开肠破肚，甚至截肢，你都能治！ ± ",
	// Athletics
	L"体能训练：你的速度和活力能和奥运会运动员相提并论！± ",
	// Bodybuilding
	L"健身：施瓦辛格？窝囊废一个。你只用一只手就能办掉他。 ± ",
	// Demolitions
	L"爆破：播种手雷，深埋炸弹，看羊羔飞。这就是你的生活！ ± ",
	// Scouting
	L"侦查：没有什么东西你觉察不到！ ± ",
};

STR16 NewInvMessage[] = 
{
    L"此时无法拾起背包",
	L"背包中无处可放",
	L"没发现背包",
	L"拉锁只在战斗中有效",
	L"背包拉锁打开时无法移动",
	L"你确定要变卖该地区所有物品吗？",
	L"你确定要销毁该地区所有物品吗？",
	L"装备大背包后无法爬上房顶",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"启动RakNet服务器...",
	L"服务器启动,等待连接...",
	L"你现在必须按'2'来连接你的客户端和服务器。",
	L"服务器已经在运行。",
	L"服务器启动失败。终止中。",
	// 5
	L"%d/%d 客户端已经准备好进入即时模式。",
	L"服务器断开并关闭。",
	L"服务器没有运行。",
	L"客户端仍在载入, 请稍等...",
	L"服务器启动之后你无法更改着陆点。",
	// 10
	L"已发送文件 '%S' - 100/100",//L"Sent file '%S' - 100/100",
	L"文件已发送到 '%S' ",//	L"Finished sending files to '%S'.",
	L"开始发送文件到 '%S'.",//	L"Started sending files to '%S'.",
	L"Use the Airspace view to select a map you wish to play. If you want to change the map you have to do this before clicking the 'Start Game' button.",	// TODO.Translate
};

STR16 MPClientMessage[] =
{
	// 0
	L"启动RakNet客户端...",
	L"正在连接IP: %S ...",
	L"接受游戏设置: ",
	L"你已经连接上了。",
	L"你已经开始连接...",
	// 5
	L"客户端 #%d - '%S' 已被雇佣 '%s'。",
	L"客户端 #%d - '%S' 已雇佣另一名佣兵。",
	L"你已准备 - 准备就绪 = %d/%d.",
	L"你不再准备 - 准备就绪 = %d/%d.",
	L"开始战斗...",
	// 10
	L"客户端 #%d - '%S' 已准备 - 准备就绪 = %d/%d.",
	L"客户端 #%d - '%S' 不再准备 - 准备就绪 = %d/%d",
	L"你已准备。 等候其它客户端... 按'OK'如果你已准备就绪。",
	L"让我们开始战斗！",
	L"要开始游戏必须运行一台客户端。",
	// 15
	L"游戏无法开始。没有佣兵被雇佣...",
	L"等待服务器解锁便携电脑出现'OK'...",
	L"中断",
	L"中断结束",
	L"鼠标方格坐标: ",
	// 20
	L"X: %d, Y: %d",
	L"坐标值: %d",
	L"服务器独占模式",
	L"手动选择服务器优先级:  ('1' - 授权 便携电脑/雇佣)  ('2' - 启动/载入 级别)  ('3' - 解锁 UI)  ('4' - 完成设置)",	
	L"分区=%s, 最大客户端数=%d, 最大佣兵数=%d, 游戏模式=%d, 同一佣兵=%d, 伤害倍数=%f, 时间前进=%d, 秒/Tic=%d, 取消 BobbyRay=%d, 取消 Aim/Merc 装备=%d, 取消士气=%d, 测试=%d",
	// 25
	L"",
	L"新建连接: 客户端 #%d - '%S'.",
	L"队: %d.",//not used any more
	L"'%s' (客户端 %d - '%S') 已被 '%s' (客户端 %d - '%S' 杀死)",
	L"踢出客户端 #%d - '%S'",
	// 30
	L"开始排序客户端号. #1: <取消>, #2: %S, #3: %S, #4: %S",
	L"开始客户端 #%d",
	L"请求即时模式...",
	L"转回即时模式。",
	L"错误: 转回过程中出现错误。",
	// 35
	L"要解锁笔记本电脑开始雇佣吗? (连接所有客户端?)",
	L"服务器已经解锁笔记本电脑。开始雇佣！",
	L"中断。",
	L"你无法更改着陆点，如果你只是客户端而不是服务器。",
	L"你拒绝投降, 因为你在一个多人游戏中。",
	// 40
	L"你所有的佣兵全部死亡！",
	L"激活观看模式。",
	L"你已被击败！",
	L"对不起, 在多人游戏中无法攀登。",
	L"你雇佣了 '%s'",
	// 45
	L"当开始购买后你不能更换地图",//	L"You cant change the map once purchasing has commenced",
	L"地图改为 '%s'",//	L"Map changed to '%s'",
	L"玩家 '%s' 断开连接, 踢出游戏",//	L"Client '%s' disconnected, removing from game",
	L"您已经断开连接，返回主菜单",//	L"You were disconnected from the game, returning to the Main Menu",
	L"连接失败, 5秒内重试, 还有 %i 次重试机会……",//	L"Connection failed, Retrying in 5 seconds, %i retries left...",
	//50
	L"连接失败，放弃……",//	L"Connection failed, giving up...",
	L"在另一个玩家进入前您无法开始游戏",//	L"You cannot start the game until another player has connected",
	L"%s : %s",
	L"发送给所有人",//	L"Send to All",
	L"发送给盟友",//	L"Allies only",
	// 55
	L"此游戏已经开始，无法加入。",//	L"Cannot join game. This game has already started.",
	L"%s (team): %s",
	L"#%i - '%s'",//	L"Client #%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"已收到服务器的所有文件。",//	L"Received all files from server.",
	L"'%S' 完成下载。",//	L"'%S' finished downloading from server.",
	L"'%S' 开始从服务器下载。",//	L"'%S' started downloading from server".,
	L"在全部客户端的文件未接收完以前不能开始游戏",//	L"Cannot start the game until all clients have finished receiving files",
	L"你需要下载修改后的文件才能继续游戏, 你想继续吗？",//	L"This server requires that you download modified files to play, do you wish to continue?",
	// 65
	L"点击 '准备' 进入战术画面。",//	L"Press 'Ready' to enter tactical screen.",
	L"不能连接到服务器，因为你的版本 %S 和服务器端的版本 %S 不同。",
	L"你击毙了一个敌人。",
	L"Cannot start the game, because all teams are the same.",    // TODO.Translate
	L"The server has choosen New Inventory (NIV), but your screen resolution does not support NIV.",
	// 70	// TODO.Translate
	L"Could not save received file '%S'",
	L"%s's bomb was disarmed by %s",
	L"You loose, what a shame",	// All over red rover
	L"Spectator mode disabled",
	L"Choose client to kick from game. #1: <Cancel>, #2: %S, #3: %S, #4: %S",
	// 75
	L"Team #%d is wiped out.",
	L"Client failed to start. Terminating.",
	L"Client disconnected and shutdown.",
	L"Client is not running.",
	L"INFO: If the game is stuck (the opponents progress bar is not moving), notify the server to press ALT + E to give the turn back to you!",	// TODO.Translate
};

STR16 gszMPEdgesText[] =
{
	L"北", //L"N",
	L"南", //L"S",
	L"东", //L"E",
	L"西", //L"W",
	L"中", //L"C",	// "C"enter
};

STR16 gszMPTeamNames[] =
{
	L"Foxtrot",
	L"Bravo",
	L"Delta",
	L"Charlie",
	L"N/A",		// Acronym of Not Applicable
};

STR16 gszMPMapscreenText[] =
{
	L"游戏类型: ",//	L"Game Type: ",
	L"玩家: ",//	L"Players: ",
	L"所拥有的佣兵: ",//	L"Mercs each: ",
	L"手提电脑开启时你无法开始移动。",//	L"You cannot change starting edge once Laptop is unlocked.",
	L"手提电脑开启后你无法更换队伍。",//	L"You cannot change teams once the Laptop is unlocked.",
	L"随机佣兵: ",//	L"Random Mercs: ",
	L"Y",
	L"难度:",//	L"Difficulty:",
	L"Server Version:",
};

STR16 gzMPSScreenText[] =
{
	L"记分板",//	L"Scoreboard",
	L"继续",//	L"Continue",
	L"取消",//	L"Cancel",
	L"玩家",//	L"Player",
	L"杀人数",//	L"Kills",
	L"死亡数",//	L"Deaths",
	L"女王的部队",//	L"Queen's Army",
	L"命中数",//	L"Hits",
	L"脱靶数",//	L"Misses",
	L"准确度",//	L"Accuracy",
	L"损害量",//	L"Damage Dealt",
	L"受损量",//	L"Damage Taken",
	L"请等待服务器指令按‘继续’。"	//L"Please wait for the server to press 'Continue'."
};

STR16 gzMPCScreenText[] =
{
	L"取消",//	L"Cancel",
	L"连接到服务器",//	L"Connecting to Server",
	L"获得服务器设置",//	L"Getting Server Settings",
	L"下载定制文件",//	L"Downloading custom files",
	L"按 'ESC' 取消， 'Y' 开始聊天",//	L"Press 'ESC' to cancel or 'Y' to chat",
	L"按 'ESC' 取消",//	L"Press 'ESC' to cancel",
	L"准备"//	L"Ready"
};

STR16 gzMPChatToggleText[] =
{
	L"发送给所有人",//	L"Send to All",
	L"发送给盟友",//	L"Send to Allies only",
};

STR16 gzMPChatboxText[] =
{
	L"多人聊天",//	L"Multiplayer Chat",
	L"聊天: 按 'Enter' 发送， 'Esc' 取消。",//	L"Chat: Press 'ENTER' to send of 'ESC' to cancel.",
};

// WANNE: Some Chinese specific strings that needs to be in unicode!
STR16 ChineseSpecString1 = L"%％";			//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString2 = L"*%3d%％%%";	//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString3 = L"%d%％";		//defined in _ChineseText.cpp as this file is already unicode
STR16 ChineseSpecString4 = L"%s (%s) [%d%％]\n%s %d\n%s %d\n%s %d (%d)\n%s (%d) %s\n%s %1.1f %s";
STR16 ChineseSpecString5 = L"%s [%d%％]\n%s %d\n%s %d\n%s %1.1f %s";
STR16 ChineseSpecString6 = L"%s [%d%％]\n%s %d%％ (%d/%d)\n%s %d%％\n%s %1.1f %s";
STR16 ChineseSpecString7 = L"%s [%d%％]\n%s %1.1f %s";

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits 用于旧特长
	L"下一页你必须依照你作为雇佣兵的专业选择特长。注意你只能选择两种一般特长或者一种专家级特长。",
	L"你可以只选择一个专家特长或者干脆什么也不选，作为回报你会得到额外的能力点数。注意电子、左右开弓和伪装是没有专家级的。",
	// For new major/minor traits 用于新的主/副特长
	L"下一步你必须依照你作为雇佣兵的专业选择特长。在第一页你可以最多选择两个主特长，以反映你在小队中扮演的主要角色。在第二页将有副特长的列表供你选择，以反映你个人的倾向。",
	L"主/副特长加起来最多不能超过三项，也就是说如果你不选择任何主特长，你就能选择三项副特长。如果你选择了两项主特长，(或者一项专家特长)，那么你就只能选择一项副特长...",
};

STR16 sgAttributeSelectionText[] = 
{
	L"请 按 照 你 对 自 己 的 感 觉 ， 调 整 你 的 各 项 能 力 值 。 各 项 能 力 值 的 最 大 值 为",
	L"IMP能力值和技能概览。",
	L"奖励点数 :",
	L"开始等级",
	// New strings for new traits
	L"下一页你将设定你的属性（生命, 敏捷, 灵敏，力量和智力）和基本技能。属性不能低于 %d。",
	L"其余的是“技能”，它可以设定为零，表示你对此一窍不通。",
	L"所有初始数值都设置在最低，其中有一些数值因为你选择的特技而被设在一个最低标准，不能低于该标准。",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"I.M.P. 性格分析",
	L"现在开始你个人档案的建立，在第一页会有一些性格特质给你的角色选择，可能你会发觉列出的选择未能完全反映你的性格，但是请至少选择一样你认为最接近你的。",
	L"下面来设定你性格上的缺陷，相信自己做一个诚实的孩子吧！每个人至少都有一种缺陷的。真实反映有助于让你的未来雇主更能了解你的潜力，避免给你安排不利的工作环境。",
};

STR16 gzIMPAttitudesText[]=
{
	L"平常",
	L"友善",
	L"独行侠",
	L"乐观主义者",
	L"悲观主义者",
	L"有侵略性",
	L"傲慢自大",
	L"大人物",
	L"神憎鬼厌",
	L"胆小鬼",
	L"I.M.P. 性格特征",
};

STR16 gzIMPCharacterTraitText[]=
{
	L"普通",
	L"喜欢社交",
	L"独行侠",
	L"乐观",
	L"坚定自信",
	L"知识份子",
	L"野性",
	L"侵略性",
	L"镇定",
	L"无所畏惧",
	L"和平主义者",
	L"恶毒",
	L"爱炫耀",
	L"I.M.P. 性格特征",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"I.M.P. 颜色及身型",
	L"I.M.P. 颜色",
	L"请选择你喜欢的IMP发色、肤色、服装颜色以及体型。",
	L"请选择你喜欢的IMP发色、肤色、服装颜色。",
	L"点选这里佣兵将单手持大枪",
	L"\n(提示: 你必须有强壮体格.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"头色",
	L"肤色",
	L"上衣颜色",
	L"裤子颜色",
	L"一般体型",
	L"魔鬼身材",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"身心健全",
	L"怕热",
	L"神经质",
	L"幽闭恐怖症",
	L"旱鸭子",
	L"怕虫",
	L"健忘",
	L"神经错乱",
	L"I.M.P. 性格缺陷",
};

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s 缺乏所需的力量。",
	L"%s 缺乏所需的敏捷。",
	L"%s 缺乏所需的灵活。",
	L"%s 缺乏所需的生命值。",
	L"%s 缺乏所需的智慧。",
	L"%s 缺乏所需的枪法。",
	// 6 - 10
	L"%s 缺乏所需的医疗技术。",
	L"%s 缺乏所需的机械技术。",
	L"%s 缺乏所需的领导能力。",
	L"%s 缺乏所需的爆破技术来完成这项任务。",
	L"%s 缺乏所需的经验来完成这项任务。",
	// 11 - 15
	L"%s 的士气不足以完成这项任务。",
	L"%s 过度疲劳, 无法完成任务。",
	L"%s 的忠诚度不足. 当地人拒绝让你执行这项任务。",
	L"已经有太多人被分配到%s了。",
	L"已经有太多人在%s执行这项任务了。",
	// 16 - 20
	L"%s找不到更多需要修理的物品。",
	L"%s损失了一些%s, 在%s的工作中！",
	L"%s损失了一些%s, 在%s, %s的工作中！",
	L"%s 在%s工作时负伤，急需医护！",
	L"%s 在%s，%s工作时负伤，急需医护！",
	// 21 - 25
	L"%s 在%s工作时负伤，只不过是皮外伤。",
	L"%s 在%s，%s工作时负伤，只不过是皮外伤。",
	L"%s地区的居民似乎对%s的出现不满。",
	L"%s地区的居民似乎对%s在%s的行为不满。",
	L"%s在%s的所作所为造成地区忠诚度下降！",
	// 26 - 30
	L"%s在%s, %s的所作所为造成 地区忠诚度下降!",
	L"%s喝高了", // <--- This is a log message string.
	L"%s在%s得了重病, 被暂时解职了。",
	L"%s得了重病, 无法继续在%s, %s的活动。",
	L"%s在%s负伤了。", // <--- This is a log message string.
	// 31 - 35
	L"%s在%s负了重伤。", //<--- This is a log message string.


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"力量",
	L"灵活",
	L"敏捷",
	L"智慧",
	L"生命",
	L"枪法",
	// 5-10
	L"领导",
	L"机械",
	L"医疗",
	L"爆破",
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"环境",
	L"工作人员",
	L"休息",
	L"修理物品",
	L"修理%s", // Vehicle name inserted here
	L"修理机器人",
	// 6-10
	L"医生",
	L"病人",
	L"锻炼力量",
	L"锻炼敏捷",
	L"锻炼灵活",
	L"锻炼生命",
	// 11-15
	L"锻炼枪法",
	L"锻炼医疗",
	L"锻炼机械",
	L"锻炼领导",
	L"锻炼爆破",
	// 16-20
	L"学习力量",
	L"学习敏捷",
	L"学习灵活",
	L"学习生命",
	L"学习枪法",
	// 21-25
	L"学习医疗",
	L"学习机械",
	L"学习领导",
	L"学习爆破",
	L"训练力量",
	// 26-30
	L"训练敏捷",
	L"训练灵活",
	L"训练生命",
	L"训练枪法",
	L"训练医疗",
	// 30-35
	L"训练医疗",
	L"训练领导",
	L"训练爆破",
};
STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 窗口模式需要一个16bpp或更少的颜色深度。",
	
	// WANNE: Savegame slots validation against INI file
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	L"Mercenary (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Enemy (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Creature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Militia (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civilian (MAX_NUMBER_CIVS_IN_TACTICAL)",

};

// SANDRO - Taunts (here for now, xml for future, I hope)
// MINTY - Changed some of the following taunts to sound more natural
STR16 sEnemyTauntsFireGun[]=
{
	L"吃我一发吧！",
	L"兔崽子，来见你外公！", // MINTY - "Touch this" just doesn't sound right, so I changed it to a Scarface quote.
	L"放马过来吧!",
	L"你是我的了!",
	L"去死吧!",
	L"害怕了吗?操你妈妈的!",
	L"这会很痛的哦!",
	L"来吧你这个混蛋!",
	L"战吧！很快就会分出胜负的！",
	L"过来爸爸的怀抱里吧!",
	L"你的葬礼马上就会举行的了!",
	L"小样！你很快就会被打包寄回去的！",
	L"嘿！想玩玩是吧？",
	L"臭婊子，你早应该呆在家里！",
	L"靠！",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"把你们做成烤肉!",// L"We have a barbecue here.",
	L"送给你的礼物!",// L"I got a present for ya.",
	L"灰飞烟灭吧!",// L"Bam!",
	L"说“茄子”!",// L"Smile!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"给我接住!",
	L"这是给你的!",
	L"老鼠给我出来吧！",
	L"这个是专门给你尝尝的.",
	L"灭哈哈哈哈哈!",
	L"接住吧！猪头！",
	L"我就喜欢这样！",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"我要把你的头皮活活剥下来!死人头！",
	L"来跟我玩玩吧！",
	L"我要让你开肠破肚!",
	L"我会把你碎尸万段!",
	L"他妈的!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"靠！看来我们遇到纯爷们了...",
	L"我参加军队是为了抢娘们的不是送死的!",
	L"我受够啦!",
	L"上帝啊!救救我吧！",
	L"我可不想白白送死..",
	L"妈妈咪啊",
	L"老子还会回来的！后会有期！",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"我似乎听见啥了!",
	L"谁在那里?",
	L"搞什么搞?",
	L"嘿！是谁？...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"他们就在那里!",
	L"现在我们来玩玩吧!",
	L"我可没想到事情会变成这样..",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"啊啊啊!",
	L"呜呜!",
	L"痛死我啦!",
	L"操你妈妈的!",
	L"你会后悔的!",
	L"什么..!",
	L"你现在可是真正把爷我惹火了.",

};

//////////////////////////////////////////////////////
// HEADROCK HAM 4: Begin new UDB texts and tooltips
//////////////////////////////////////////////////////
STR16 gzItemDescTabButtonText[] =
{
	L"说明",
	L"常规",
	L"进阶",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"说明",
	L"常规",
	L"进阶",
};

STR16 gzItemDescGenHeaders[] =
{
	L"基本性能",
	L"附加性能",
	L"AP 消耗",
	L"点射 / 连发",
};

STR16 gzItemDescGenIndexes[] =
{
	L"参数.",
	L"0",
	L"+/-",
	L"=",
};

STR16 gzUDBButtonTooltipText[]=
{
	L"|说|明|页|面:\n \n显示该物品基本的文字描述。",
	L"|常|规|性|能|页|面:\n \n显示该物品的详细性能数据。",
	L"|进|阶|性|能|页|面:\n \n显示使用该物品的额外效果。",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|基|本|性|能:\n \n由物品类别（武器/护甲/杂物）决定的基本性能与数据。",
	L"|附|加|性|能:\n \n该物品的附加特性，以及（或）附加能力。",
	L"|A|P |消|耗:\n \n使用这件武器需要耗费的AP。",
	L"|点|射 |/ |连|发|性|能:\n \n武器在点射/连发模式下的相关数据。",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|参|数|图|标:\n \n鼠标悬停显示参数名称",
	L"|基|本|数|值:\n \n该物品使用的基本数值，不包括由附件或弹药导致的\n奖励或惩罚。",
	L"|附|件|加|成:\n \n弹药，附件或较差的物品状态导致的\n奖励或惩罚。",
	L"|最|终|数|值:\n \n该物品使用的最终数值，包括所有由附件或弹药导致的\n奖励或惩罚。",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"参数图标（鼠标悬停显示名称）",
	L"|站|立 姿态的奖励/惩罚",
	L"|蹲|伏 姿态的奖励/惩罚",
	L"|匍|匐 姿态的奖励/惩罚",
	L"奖励/惩罚",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|精|度",
	L"|杀|伤|力",
	L"|射|程",
	L"|精|瞄|等|级",
	L"|瞄|准|镜|倍|率",
	L"|红|点|效|果",
	L"|消|焰",
	L"|噪|音",
	L"|可|靠|性",
	L"|修|理|难|度",
	L"|精|瞄|最|低|有|效|距|离",
	L"|命|中|率|修|正",
	L"", // (12)
	L"|举|枪|A|P",
	L"|单|发|A|P",
	L"|点|射|A|P",
	L"|连|发|A|P",
	L"|上|弹|A|P",
	L"|换|弹|夹|A|P",
	L"|水|平|后|坐|力",
	L"|垂|直|后|坐|力",
	L"|连|发|子|弹|数/5|A|P",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \n决定该武器发射的子弹偏离瞄准点的远近。\n \n数值范围：0~100。越高越好。",
	L"\n \n决定该武器发射的子弹的平均伤害，不包括\n护甲或穿甲修正。\n \n该数值越高越好。",
	L"\n \n从该枪射出的子弹在落地前的\n最大飞行距离（格数）。\n \n该数值越高越好。",
	L"\n \n这个数值显示了该武器的精瞄等级。\n \n精瞄等级越低，每次瞄准获得的命中率加成越\n高。因此，精瞄等级要求较低的武器可以在不损\n失精度的情况下更快地瞄准。\n \n该数值越低越好。",
	L"\n \n该值大于1.0时，远距离瞄准中的误差会按比例减小。\n \n \n请注意，高倍率瞄准镜不利于射击距离过近的目标。\n \n \n该值为1.0则说明该武器未安装瞄准镜。\n",
	L"\n \n在一定距离上按比例减少瞄准误差。\n \n红点效果只在一定距离内有效，因为超过该距离\n光点就会开始暗淡，最终在足够远处消失。\n \n该数值越高越好。",
	L"\n \n该属性起作用时，武器发射不会产生枪焰。\n \n \n敌人不能通过枪焰判断你的位置，但是他们仍然可\n能听到你。",
	L"\n \n该参数显示了武器开火时枪声传播的\n范围（格数）。\n \n此范围内的敌人均有可能听到枪声。\n \n该数值越低越好。",
	L"\n \n决定了该武器使用时损耗的快慢。\n \n该数值越高越好。",
	L"\n \n决定了修理该武器的难易。\n \n该数值越高越好。",
	L"\n \n瞄准镜提供瞄准命中率加成的最短距离。\n（再近就无效了）",
	L"\n \n激光瞄准器提供的命中率修正。",
	L"", // (12)
	L"\n \n端枪准备开火所需的AP。\n \n举起该武器后，连续发射不会再消耗\n举枪AP。\n \n但是，除转向和开火之外的其他动作均会放\n下武器。\n \n该数值越低越好。",
	L"\n \n该武器射出单发弹药所需的AP，\n对于枪械而言，\n该数值显示了在不精瞄的情况下发射\n一发子弹的AP消耗。\n \n如果该图标为灰色，则该武器不可单发射击。\n \n该数值越低越好。",
	L"\n \n一次点射所需的AP。\n \n每次点射的子弹数由枪支本身决定，\n并显示在该图标上。\n \n如果该图标发灰，则该武器不可点射。\n \n该数值越低越好。",
	L"\n \n连发模式下，该武器一次\n齐射三发子弹所需的AP。\n \n超过3发子弹，\n则需要额外的AP。\n \n如果该图标发灰，则该武器不可连发。\n \n该数值越低越好。",
	L"\n \n重新装填子弹所需的AP。\n \n该数值越低越好。",
	L"\n \n在射击间歇为该武器手动更换弹夹的AP消耗。\n \n该数值越低越好。",
	L"\n \n该参数显示了枪口在点射或连发模式下发射子弹\n时的水平位移。\n \n当该参数为正时，枪口右偏。反之，则向左偏。\n \n越接近0越好。",
	L"\n \n该参数显示了枪口在点射或连发模式下发射子弹\n时的垂直位移。\n \n当该参数为正时，枪口上偏。反之，则向下偏。\n \n越接近0越好。",
	L"\n \n该参数显示了该武器每多花费5AP在连发模式时\n可多发射的子弹数。\n \n该数值越高越好。",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|防|护|值",
	L"|覆|盖|率",
	L"|损|坏|率",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \n这是护具最重要的属性，决定了其可以吸收多\n少伤害。然而，穿甲攻击以及其他的随机因素\n都可能对最终的伤害值产生影响。\n \n该数值越高越好。",
	L"\n \n决定该护具对身体的防护面积。\n \n如果该值低于100%，则攻击有可能\n对未被护具保护的部分身体造成最大伤害。\n该值越高越好。",
	L"\n \n显示护具被击中时的磨损速率，\n与其吸收的伤害成比例。\n该值越低越好。",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|侵|彻|力|（|穿|甲|弹|）",
	L"|翻|搅|力|（|开|花|弹|）",
	L"|爆|炸|力|（|炸|子|儿|）",
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \n即子弹穿透目标护甲的能力。\n \n该值大于1时，被子弹命中的护甲的防护值会成比例减少。\n \n反之，当该值小于1时，\n则增加了目标护甲的防护值。\n \n该数值越高越好。",
	L"\n \n该值决定子弹打穿护甲击中身体时的伤害力\n加成。\n \n该值大于1时，子弹在穿过护甲后会增加伤害。\n \n当该值小于1时，子弹穿过护甲后会减少伤害。\n \n该数值越高越好。",
	L"\n \n该值是子弹在击中目标前已经造成的潜在伤害的倍率。\n \n大于1的数值可以增加伤害，\n反之则减少伤害。\n \n该数值越高越好。",
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|杀|伤|力",
	L"|眩|晕|杀|伤|力",
	L"|爆|炸|范|围",
	L"|眩|晕|爆|炸|范|围",
	L"|噪|音|扩|散|范|围",
	L"|催|泪|毒|气|初|始|范|围",
	L"|芥|子|毒|气|初|始|范|围",
	L"|照|明|初|始|范|围",
	L"|烟|雾|初|始|范|围",
	L"|燃|烧|初|始|范|围",
	L"|催|泪|毒|气|最|终|范|围",
	L"|芥|子|毒|气|最|终|范|围",
	L"|照|明|最|终|范|围",
	L"|烟|雾|最|终|范|围",
	L"|燃|烧|最|终|范|围",
	L"|效|果|持|续|时|间",
	L"|噪|音",
	L"|挥|发|性",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \n本次爆炸造成的伤害值。\n \n爆炸型爆破品只在被引爆时造成一次性伤害，\n而具有持续效果的爆破品每回合都可以造成伤\n害，直到效果消失。\n \n该数值越高越好。",
	L"\n \n爆破造成的非致命性眩晕伤害值。\n \n爆炸型爆破品只在被引爆时造成一次性伤害，\n而具有持续效果的爆破品每回合都可以造成伤\n害，直到效果消失。\n \n该数值越高越好。",
	L"\n \n这是该爆破品的有效杀伤半径。\n \n目标距爆炸中心越远，受到的伤害越少。\n \n该数值越高越好。",
	L"\n \n这是该爆破品的眩晕伤害半径。\n \n目标距爆炸中心越远，受到的伤害越少。\n \n该数值越高越好。",
	L"\n \n这是该陷阱所发出噪音的传播距离。\n \n在该距离之内的士兵可能听到这个噪音\n并有所警觉。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放出的催泪毒气的初始半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n \n下方则显示了该效果的作用半径与持续时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放出的芥子毒气的初始半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n \n下方则显示了该效果的作用半径与持续时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品发光的初始半径。\n \n距爆炸中心较近的格子会变得非常明亮，而接近\n边缘的格子只会比平常亮一点点。\n \n下方则显示了该效果的作用半径与持续时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放烟雾的初始半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n（如果有的话）。更重要的是，烟雾中的人\n极难被发现，同时他们也会失\n去很大一部分视距。\n \n请查看最大半径和有效时间（显示在下面）。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放出的火焰的初始半径。\n \n在该半径之内的敌人每回合都会受到所列出的\n物理伤害与眩晕伤害。\n \n下方则显示了该效果的作用半径与持续时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放出的催泪毒气消散前的最大\n半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n \n请同时查看初始半径和有效时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放出的芥子毒气消散前的最大\n半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n \n请同时查看初始半径和有效时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品发出的亮光消失前的半径。\n \n离爆炸中心较近的格子会变得非常亮，而接近\n边缘的格子只会比平常稍亮。\n \n留意初始半径和有效时间。\n \n也请记住，与其他爆破品不同的是照明效果会随\n着时间流逝越来越小直到消失。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放的烟雾消散前的最大半径。\n \n除非佩戴了防毒面具，否则在该半径之内的敌\n人每回合都会受到所列出的物理伤害与眩晕伤害\n（如果有的话）。更重要的是，烟雾中的人\n极难被发现，同时他们也会失\n去很大一部分视距。\n \n请同时查看初始半径和有效时间。\n \n该数值越高越好。",
	L"\n \n这是该爆破品释放的火焰熄灭前的最大半径。\n \n在该半径之内的敌人每回合都会受到所列出的\n物理伤害与眩晕伤害\n \n请同时查看初始半径和有效时间。\n \n该数值越高越好。",
	L"\n \n这是爆炸效果的持续时间。\n \n爆炸效果的范围每回合都会向所有的方向增加一\n格，直到其半径达到所列出的最大值。\n \n一旦持续时间过去，爆炸效果就会完全消失。\n \n注意照明类的爆炸与众不同，会随着时间\n流逝越来越小。\n \n该数值越高越好。",
	L"\n \n这是爆破品爆炸时发出的声音能够被佣兵和敌\n军听到的距离（格数）。\n \n听到爆炸声的敌人会察觉到你。\n \n该数值越低越好。",
	L"\n \n这个数值代表该爆破品受到伤害时（如其他爆破品在\n近处爆炸）自身爆炸的几率（100以内）。\n \因此携带高挥发性爆破品进入战斗极其危险，\n应当极力避免。\n \n数值范围：0~100，越低越好。",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|曳|光|弹",
	L"|反|坦|克|弹",
	L"|破|甲",
	L"|酸|腐|弹",
	L"|破|锁|弹",
	L"|防|爆",
	L"|防|水",
	L"|电|子|产|品",
	L"|防|毒|面|具",
	L"|需|要|电|池",
	L"|能|够|开|锁",
	L"|能|够|剪|线",
	L"|能|够|撬|锁",
	L"|金|属|探|测|器",
	L"|远|程|引|爆|装|置",
	L"|远|程|爆|破|引|信",
	L"|定|时|爆|破|引|信",
	L"|装|有|汽|油",
	L"|工|具|箱",
	L"|热|成|像|仪",
	L"|X|光|射|线|仪",
	L"|装|饮|用|水",
	L"|装|酒|精|饮|品",
	L"|急|救|包",
	L"|医|药|箱",
	L"|破|锁|炸|弹",
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \n在点射或者连射时，曳光弹会产生曳光效果。\n \n由于曳光能够帮助持枪者校准，所以即使考虑\n后座力，该子弹的杀伤仍是致命的；曳光弹\n也能在黑暗中照亮目标。\n \n然而，曳光弹也会暴露射手的位置！\n \n曳光弹会抵消枪口消焰器的效果。",
	L"\n \n这种子弹能够对坦克装甲造成伤害。\n \n没有穿甲属性的子弹不能\n对坦克造成任何伤害。\n \n即使拥有穿甲属性，大部分枪械对于坦克的伤害仍然\n十分有限，所以不要抱有太大的期望。",
	L"\n \n这种子弹完全无视防弹护甲。\n \n无论目标是否穿着防弹衣，被该子弹击中时，\n都将受到全额伤害！",
	L"\n \n当这种子弹击中目标身上的护甲时会使\n护甲快速损坏。\n \n也可能完全破坏目标的护甲。",
	L"\n \n这种子弹对于破锁十分有效。\n \n当门锁或者其他容器的锁被击中时，会被严重损坏。",
	L"\n \n这种防弹装甲对爆炸的防御力是其防护值的四倍。\n \n受到爆炸物伤害时，该护甲的防御数值按照\n装甲属性中列出数值的四倍计算。",
	L"\n \n该物品防水。\n \n它不会因为浸没在水中而受损。没有该属性的\n物品会在持有者游泳时逐渐损坏。",
	L"\n \n该物品是电子产品，含有复杂电路。\n \n电子产品在维修者没有电子技能时很难被修复\n。",
	L"\n \n将该物品佩戴于面部时，使用者不受任何\n有毒气体的伤害。\n \n然而有些腐蚀性气体可以通过腐蚀\n作用穿过这个面罩。",
	L"\n \n该物品需要电池。没有安装电池时使用者不\n能使用这个物品的主要功能。\n \n只要把所需电池安装于该物品的附件栏即可\n（步骤与将瞄准镜安装在步枪上一样）。",
	L"\n \n该物品能够用于开锁。\n \n（用技巧）开锁不会发出声音，但是开稍微复\n杂一些的锁需要足够的机械能力。",
	L"\n \n该物品能够绞断铁丝网。\n \n使用此物品，佣兵可以快速穿越用铁丝网封锁的地区，以便\n包围敌人！",
	L"\n \n该物品能够用于破坏锁具。\n \n破坏锁具需要很大的力量，既会发出很大的\n噪音，也很耗费佣兵的体力。但是在没有出色\n的技巧和复杂的工具时，用力量破坏锁具也是明智\n之举。",
	L"\n \n该物品能够探测地下的金属物品。\n \n显然其主要用于在没有肉眼识别地雷的能力时探测地\n雷。但是你也可以用它发现埋在地下的宝藏。",
	L"\n \n该物品能够用来引爆已经安装远程爆破引信的炸弹\n。\n \n先放置炸弹，时机一到再用它引爆。",
	L"\n \n安装该引信的爆破物设置完成后\n，可以被远程控制器引爆。\n \n远程引信是设置陷阱的不二选择，因为它只会在你需要\n它爆炸的时候被引爆，而且留给你足够的时间跑\n开！",
	L"\n \n安装该引信的爆破物设置完成后\n，该引信会开始倒数计时，并在设置的时间后\n被引爆。\n \n计时引信便宜并且易于安装，但是你必须给它\n设定合适的时间以便你能够跑开！",
	L"\n \n该物品承有汽油。\n \n在你需要加油时十分有用。",
	L"\n \n工具箱内装有各种能用来修复其他物品的工具。\n \n安排佣兵进行修复工作时该佣兵必须持有工具\n箱。",
	L"\n \n将该物品佩戴于面部时，\n可以利用热成像原理，\n发现墙壁后方的敌人。",
	L"\n \n这种功能强大的仪器利用X光搜索敌军。\n \n它可以在短时间内暴露一定范围中的敌人位置。\n请远离生殖器使用！",
	L"\n \n该物品装有饮用水。\n \n口渴时饮用。",
	L"\n \n该物品内含美酒、酒精饮料、洋酒，\n嘿嘿，你叫它什么都行。\n \n适量饮用，不要酒后驾驶，小心肝硬化！",
	L"\n \n这一战场的基础急救包提供了基本的医疗用品。\n \n可以被用来包扎受伤的角色以止血。\n \n如需要回复生命，使用名副其实的医药箱，并辅以大量的休息。",
	L"\n \n这是名副其实的医药箱，可以用于外科手术或其他复杂的治疗。\n \n安排佣兵进行医疗工作时，该佣兵必须持有医\n药箱。",
	L"\n \n该物品能够用于爆破锁具。\n \n使用它需要爆破技能以避免过早引爆。\n \n使用炸药是一个相对简单的破锁手段，但是会\n发出很大噪音，并且对于大部分佣兵来说过于\n危险。",
};

STR16 szUDBAdvStatsTooltipText[]=
{
	L"|精|度|修|正",
	L"|速|射|精|度|修|正|值",
	L"|速|射|精|度|修|正|百|分|比",
	L"|精|瞄|修|正|值",
	L"|精|瞄|修|正|百|分|比",
	L"|精|瞄|等|级|修|正",
	L"|精|瞄|上|限|修|正",
	L"|枪|械|使|用|修|正",
	L"|弹|道|下|坠|修|正",
	L"|瞄|准|误|差|修|正",
	L"|杀|伤|力|修|正",
	L"|近|战|杀|伤|力|修|正",
	L"|射|程|修|正",
	L"|瞄|准|镜|倍|率",
	L"|红|点|效|果",
	L"|水|平|后|坐|力|修|正",
	L"|垂|直|后|坐|力|修|正",
	L"|最|大|制|退|力|修|正",
	L"|制|退|力|精|度|修|正",
	L"|制|退|力|频|次|修|正",
	L"|A|P|总|量|修|正",
	L"|举|枪|A|P|修|正",
	L"|单|发|A|P|修|正",
	L"|点|射|A|P|修|正",
	L"|连|发|A|P|修|正",
	L"|上|弹|A|P|修|正",
	L"|弹|夹|容|量|修|正",
	L"|点|射|弹|数|修|正",
	L"|消|焰",
	L"|噪|音|修|正",
	L"|物|品|尺|寸|修|正",
	L"|可|靠|性|修|正",
	L"|丛|林|迷|彩",
	L"|城|市|迷|彩",
	L"|沙|漠|迷|彩",
	L"|雪|地|迷|彩",
	L"|潜|行|修|正",
	L"|听|觉|距|离|修|正",
	L"|一|般|视|距|修|正",
	L"|夜|晚|视|距|修|正",
	L"|白|天|视|距|修|正",
	L"|高|光|视|距|修|正",
	L"|洞|穴|视|距|修|正",
	L"|隧|道|视|野|效|应",
	L"|最|大|制|退|力",
	L"|制|退|力|频|次",
	L"|命|中|率|修|正",
	L"|精|瞄|修|正",
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular.
STR16 szUDBAdvStatsExplanationsTooltipText[]=
{
	L"\n \n当安装于远程武器上时，该物品将修正武器的精\n度值。\n \n精度的提高能够使武器在精瞄时更容易命中远\n距离的目标。\n \n数值范围：-100~+100，越高越好。",
	L"\n \n该物品按照所列数值修正射手使用远程武器打出去\n的每颗子弹的精度。\n \n数值范围：-100~+100，越高越好。",
	L"\n \n在原本射击精度的基础上，该物品按照所列百分比修正射手使用远程武器射出\n的每颗子弹。\n \n数值范围：-100~+100，越高越好。",
	L"\n \n该物品按照所列数值修正射手使用远程武器瞄准时，\n每次精瞄所增加的精度。\n \n数值范围：-100~+100，越高越好。",
	L"\n \n该物品按照所列百分比修正射手使用远程武器瞄准\n时，每次精瞄所增加的精度。\n \n数值范围：-100~+100，越高越好。",
	L"\n \n该物品修正该武器的精瞄等级。\n \n减少精瞄等级意味着每一次精瞄会增加更多的\n精度。因此，精瞄等级的减少，有助于这件武\n器在不损精度的情况下快速瞄准。\n \n该数值越低越好。",
	L"\n \n在原本射击精度的基础上，该物品按照百分比修正射手使用远程武器时能\n达到的最大精度。\n \n该数值越高越好。",
	L"\n \n当将该物品安装于远程武器上时，会修正武器的操\n控难度。\n \n易于操控的武器不论是否进行精瞄都更加\n准确。\n \n该修正基于武器的原始操控难度，步枪\n和重武器高而手枪和轻武器低。\n \n该数值越低越好。",
	L"\n \n该物品修正超射距命中的难度。\n \n较高的修正值可以增加武器的最大射程至少几\n格。\n \n该数值越高越好。",
	L"\n \n该物品修正命中移动目标的难度。\n \n较高的修正值能够增加在较远距离上命中移动目\n标的几率。\n \n该数值越高越好。",
	L"\n \n该物品修正您武器的杀伤力。\n \n该数值越高越好。",
	L"\n \n该物品按照所列数值修正您近战武器的伤害值。\n \n该物品只作用于近战武器，无论是利器还是\n钝器。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品可修正其\n最大有效射程。\n \n最大射程是指子弹明显坠落前可以飞行的距离。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品提供额外的瞄\n准倍率，使远距离射击相对来说更容易命中。\n \n请注意当目标距离小于最佳瞄准距离时，高倍率对于\n瞄准不利。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品在目标身上投\n影出一个红点，让其更容易被命中。\n \n红点效果只能在指定距离内使用，超过该距离\n光点就会暗淡直到最终消失。\n \n该数值越高越好。",
	L"\n \n当安装于可点射或连发的远程武器上时，该\n物品按照百分比修正该武器的水平后座力。\n \n在连续射击时，降低后坐力可以帮助射手\n保持枪口指向目标。\n \n该数值越低越好。",
	L"\n \n当安装于可点射或连发的远程武器上时，该\n物品按照百分比修正该武器的垂直后座力。\n \n在连续射击时，降低后坐力可以帮助射手\n保持枪口指向目标。\n \n该数值越低越好。",
	L"\n \n该物品修正射手在点射或者连发时，应对制退后坐\n力的能力。\n \n高修正值能帮助射手控制后坐力较高的武器，\n即使射手自身力量较低。\n \n该数值越高越好。",
	L"\n \n该物品修正射手在点射或者连发时，运用反作\n用力制退后坐力的精确度。\n \n高修正值能帮助射手维持枪口始终朝向目标，\n哪怕目标较远，也能提升精度。\n \n该数值越高越好。",
	L"\n \n在射手进行点射或者连发时，该物品修正其频繁评估\n制退力大小以应对后坐力的能力。\n \n低修正值使连发的总体精度更高，此外，由于射手能\n正确制退后坐力，其长点射也更\n加准确。\n \n该数值越高越好。",
	L"\n \n该物品直接修正佣兵每回合的初始AP量。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品修正举枪AP。\n \n该数值越低越好。",
	L"\n \n当安装于远程武器上时，该物品修正单发AP。\n \n注意对于可以点射或连发的武器来说，该物品\n也会影响点射和连发的AP。\n \n该数值越低越好。",
	L"\n \n当安装于可以进行点射的远程武器上时，该物品修正点射AP。\n \n该数值越低越好。",
	L"\n \n当安装于可以进行连发的远程武器上时，该物品修正连发AP。\n \n注意，这不改变连发增加子弹时的AP消耗，只\n影响连发时AP的初始消耗。\n \n该数值越低越好。",
	L"\n \n当安装于远程武器上时，该物品修正上弹AP。\n \n该数值越低越好。",
	L"\n \n当安装于远程武器上时，该物品修正该武器的\n弹夹容量。\n \n该武器便能够使用相同口径的不同容量的弹夹。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品修正该武器在\n点射时发射的子弹数。\n \n如果该武器不能点射而此修正值为正，该物品\n会使武器能够点射。\n \n相反，如果该武器原本能够点射，而此修正值\n为负，该物品可能使武器失去点射能力。\n \n该数值一般越高越好。当然连发时也需要注意\n节省弹药。",
	L"\n \n当安装于远程武器上时，该物品能够隐藏该武\n器的枪焰。.\n \n当射手在隐蔽的地方开枪，将不会被敌人发现\n，这在夜战中十分重要。",
	L"\n \n当安装于武器上时，该物品修正使用该武器时\n发出的噪音能被敌人和佣兵发觉的距离。\n \n如果该修正值将武器的噪音数值削减至0，那\n么该武器就被完全消音了。\n \n该数值越低越好。",
	L"\n \n该物品修正把它作为附件的物品的尺寸大小。\n \n物品大小在新携行系统中很重要，因为口袋只\n能装下特定大小和形状的物品。\n \n增加尺寸会使物品太大而不能放入某些口袋。\n \n反之，减少尺寸意味着该物品可以适合于更多\n的口袋，并且一个口袋可以装得更多。\n \n一般来说，该数值越低越好。",
	L"\n \n当安装于武器上时，该物品修正该武器的可靠\n性数值。\n \n如果该修正值为正，该武器在使用过程中的磨\n损会更慢，反之磨损会更快。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，会增加穿戴者在\n丛林环境中的伪装值。\n \n该伪装需靠近树木或较高的草丛才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，会增加穿戴者在\n城市环境中的伪装值。\n \n该伪装需靠近沥青或混凝土材质才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，会增加穿戴者在\n沙漠环境中的伪装值。\n \n该伪装需靠近沙地、沙砾地或沙漠植被才能发挥最大功\n效。\n \n该伪装数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，会增加穿戴者在\n雪地环境中的伪装值。\n \n该伪装需靠近雪地才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，修正使用者的潜\n行能力，使其在潜行时更难被听到。\n \n注意该物品并不修正潜行者的可视特征，\n而只是改变潜行中动静的大小。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的听觉感知范围。\n \n该值为正时可以从更远的距离听到噪音。\n \n与此同时，该值为负时会削减使用者的听力。\n \该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的视觉感知范围。\n \n这一修正值适用于所有情况。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的视觉感知范围。\n \n这一夜视修正只在光线明显不足时有效。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的视觉感知范围。\n \n这一白天视觉修正只在光照度为平均值或更高时有效。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的视觉感知范围。\n \n这一高光视觉修正只在光照度过高时有效，\n例如直视闪光弹照亮的格子或\n在正午时分。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，将按照所列\n百分比修正使用者的视觉感知范围。\n \n这一洞穴视觉修正只在昏暗且位于地下时有效。\n \n该数值越高越好。",
	L"\n \n当该物品戴在身上或附在穿戴品上时，会改变视\n野范围，视野范围减少会导致可视角度变窄。\n \n该数值越低越好。",
	L"\n \n这是射手在点射或者连发时，制退后坐力的能力。\n \n该数值越高越好。",
	L"\n \n这是射手在点射或者连发时，频繁评估\n制退力大小以应对后坐力的能力。\n \n较高的频率使连发的总体精度更高，此外，由于射手能\n正确制退后坐力，其长点射也更\n加准确。\n \n该数值越低越好。",
	L"\n \n当安装于远程武器上时，该物品修正武器的命\n中率。\n \n命中率的提高使该武器在精瞄时更容易命中\n目标。\n \n该数值越高越好。",
	L"\n \n当安装于远程武器上时，该物品修正武器的精\n瞄加成。\n \n精瞄加成的提高使该武器在精瞄时更容易命\n中远距离的目标。\n \n该数值越高越好。",
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \n由于所装的附件，弹药或其内置特性，这件武\n器的精度得到了修正。\n \n提高精度能够使该武器在精瞄时更容易命中远\n距离的目标。\n \n数值范围：-100~+100，该数值越高越好。",
	L"\n \n这件武器按照所列数值修正了射手\n的精度。\n \n数值范围：-100~+100，该数值越高越好。",
	L"\n \n这件武器按照所列百分比修正了射手打出去的每颗\n子弹的精度。\n \n该数值越高越好。",
	L"\n \n这件武器按照所列数值修正了每次精瞄所增加的精\n度。\n \n数值范围：-100~+100，该数值越高越好。",
	L"\n \n根据射手本身的射击精度，这件武器\n按照所列百分比修正每次精瞄所增加的\n精度。\n \n该数值越高越好。",
	L"\n \n由于其所装的附件，弹药或由于其内置特性，\n这件武器的精瞄等级得到了修正。\n \n如果精瞄等级减少了，则这件武器可以在不损\n失精度的情况下快速瞄准。\n \n反之，若精瞄等级增加，则这件武器瞄准的\n更慢，却不会额外增加精度。\n \n该数值越低越好。",
	L"\n \n这件武器按照一定百分比\n修正射手能够达到的最大精度。\n(依据射手本来的精度）\n \n该数值越高越好。",
	L"\n \n由于所装的附件或其固有特性，武器操\n控难度得到了修正。\n \n易于操控的武器不论是否进行精瞄都更加\n准确。\n \n该修正基于武器的原始操控难度，步枪\n和重武器高而手枪和轻武器低。\n \n该数值越低越好。",
	L"\n \n由于所装的附件或其固有特性，这件武\n器超射距命中的能力得到了修正。\n \n较高的修正值可以增加该武器的最大射程至少\n几个格。\n \n该数值越高越好。",
	L"\n \n由于所装的附件或其固有特性，这件武\n器命中远距移动目标的能力得到了修正。\n \n高修正值有助于在较远的距离上增加命中快速移动目\n标的几率。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的伤害值得到了修正。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的近战伤害值得到了修正。\n \n该修正值仅限于近战武器，无论是利器还是钝\n器。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的最大射程得到了修正。\n \n最大射程是指子弹明显坠落前所飞行的距离。\n \n该数值越高越好。",
	L"\n \n这件武器装备了光学瞄准镜，因而其远距离射击更\n容易命中。\n \n注意在目标比最佳瞄准距离近时，高倍率对于\n瞄准是不利的。\n \n该数值越高越好。",
	L"\n \n这件武器装备了瞄准指示设备（可能是激光），它\n可以在目标身上投影出一个点，使其更容易\n被命中。\n \n指示效果只能在指定距离内使用，超过该距离\n光点就会暗淡直到最终消失。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的水平后坐力得到了修正。\n \n如果武器缺少点射和连发功能，则此修正无效。\n \n在点射或连发时，降低后坐力可以帮助射手\n保持枪口持续对准目标。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的垂直后坐力得到了修正。\n \n如果武器缺少点射和连发功能，则此修正无效。\n \n在点射或连发时，降低后坐力可以帮助射手\n保持枪口持续对准目标。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器修正了射手在点射或者连发时，制退后坐力\n的能力。\n \n高修正数值能帮助射手控制后坐力较强的武器\n，即使射手自身力量较低。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器修正了射手运用反作用力制退后坐力的精确\n度。\n \n如果武器缺少点射和连发功能，则此修正无效。\n \n高修正值能帮助射手维持枪口始终朝向目标，\n哪怕目标较远，也能提升精度。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器修正了射手频繁估量制退力大小的能力。\n \n如果点射和连发功能都没有，则此修正无效。\n \n高修正值能够提高子弹的总体精度，在射手能\n正确制退后坐力的前提下，远距离的连发也更\n能加准确。\n \n该数值越高越好。",
	L"\n \n持有这件武器将修正佣兵每回合的初\n始AP量。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的举枪AP得到了修正。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的单发AP得到了修正。\n \n请注意对于可以点射或连发的武器来说，点射和\n连发AP也会被修正。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的点射AP得到了修正。\n \n如果武器没有点射功能，此修正自然无效。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的连发AP得到了修正。\n \n如果武器没有连发功能，此修正自然无效。\n \n注意，增加连发子弹时的AP消耗并不会改变，\n只影响连发时AP的初始消耗。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的上弹AP得到了修正。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，此武器\n的弹夹容量得到了修正。\n \n现在这件武器可以接受相同口径的更大（或更小）容量的\n弹夹。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器在点射时发射的子弹数得到了修正。\n \n如果此武器本不能点射而此修正值为正，将赋予武器\n点射能力。\n \n反之，如果此武器原本能够点射，而此修正值\n为负，则将使其失去点射能力。\n \n该数值一般越高越好。当然连发时也需要注意\n节省弹药。",
	L"\n \n由于所装的附件，弹药或其固有特性，此武器\n发射时没有枪焰。\n \n当射手在隐蔽的地方开枪，将不会被敌人发现，这在夜战中十分重要。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器发出的噪音得到了修正。因而敌人和佣兵能\n发觉枪响的距离也就修正了。\n \n如果该修正值将武器的噪音数值削减至0，那\n么该武器就被完全消音了。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的尺寸大小得到了修正。\n \n物品大小在新携行系统中很重要，因为口袋只\n能装下特定大小和形状的物品。\n \n增加尺寸会使物品太大而不能放入某些口袋。\n \n反之，减少尺寸意味着该物品可以被放入更多\n的口袋中，并且一个口袋可以装更多。\n \n该数值一般越低越好。",
	L"\n \n由于所装的附件，弹药或其固有特性，这件武\n器的可靠性得到了修正。\n \n如果该修正值为正，该武器在使用过程中的磨\n损会更慢，反之磨损会更快。\n \n该数值越高越好。",
	L"\n \n当手持这件武器时，使用者\n在丛林环境中的伪装值改变了。\n \n该伪装需靠近树木或草丛才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当手持这件武器时，使用者\n在城市环境中的伪装值改变了。\n \n该伪装需靠近沥青或水泥才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当手持这件武器时，使用者\n在沙漠环境中的伪装值改变了。\n \n该伪装需靠近沙砾或沙漠植被才能发挥最大功\n效。\n \n该伪装数值越高越好。",
	L"\n \n当手持这件武器时，使用者\n在雪地环境中的伪装值改变了。\n \n该伪装需靠近雪地才能发挥最大功效。\n \n该伪装数值越高越好。",
	L"\n \n当手持这件武器时，将修正士兵的潜行能力，使潜行\n者更难或更容易被听到。\n \n注意该物品并不修正潜行者的可视特征，\n而只是改变潜行中动静的大小。\n \n该数值越高越好。",
	L"\n \n当手持这件武器时，将按照所列\n百分比修正使用者的听觉感知范围。\n \n该值为正时可以从更远的距离听到噪音。\n \n若该值为负，则会削减使用者的听力。\n \该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n由于附件或武器的本身特点，使用者的\n视觉范围将依照所列比例得到修正\n \n这一一般修正适用于所有情形。\n \n该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n由于附件或武器的本身特点，使用者的\n视觉范围将依照所列比例得到修正\n \n这一夜视修正只在光线明显不足时有效。\n \n该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n由于附件或武器的本身特点，使用者的\n视觉范围将依照所列比例得到修正\n \n这一白天视觉修正只在光照度为平均值或更高时有效。\n \n该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n由于附件或武器的本身特点，使用者的\n视觉范围将依照所列比例得到修正\n \n这一高光视觉修正只在光照度过高时有效，\n例如直视闪光弹照亮的\n格子或在正午时分。\n \n该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n由于附件或武器的本身特点，使用者的\n视觉范围将依照所列比例得到修正\n \n这一洞穴视觉修正只在昏暗且位于地下时有效。\n \n该数值越高越好。",
	L"\n \n当完成该武器的举枪动作时，\n将改变使用者的视野范围，视野\n范围减少会导致可视角度变窄。\n \n该数值越高越好。",
	L"\n \n这是射手在点射和连发时，制退后坐力的能力。\n \n该数值越高越好。",
	L"\n \n这是射手频繁估量制退力大小以应对后坐力的能力。\n \n如果武器缺乏点射和连发功能，则此能力无\n效。\n \n低修正值能提高连发的总体精度，此外，由于射手能\n正确制退后坐力，其长点射也更\n加准确。\n \n该数值越低越好。",
	L"\n \n由于所装的附件，弹药或其内置特性，这件武\n器的命中率得到了修正。\n \n命中率的提高使该武器在精瞄时\n更容易命中目标。\n \n该数值越高越好。",
	L"\n \n由于所装的附件，弹药或其内置特性，这件武\n器的精瞄加成得到了修正。\n \n精瞄加成的提高能够使该武器在精瞄时更容易命\n中远距离的目标。\n \n该数值越高越好。",
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"单发",
	L"AP",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

#endif //CHINESE
