// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("RUSSIAN")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( RUSSIAN )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_RussianText_public_symbol(void){;}

#ifdef RUSSIAN

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

CHAR16	XMLTacticalMessages[1000][MAX_MESSAGE_NAMES_CHARS] = 
{
	L"",
};

//Encyclopedia

STR16 pMenuStrings[] =
{
	//Encyclopedia
	L"Locations", // 0
	L"Characters",
	L"Items",
	L"Quests",
	L"Menu 5",
	L"Menu 6", //5
	L"Menu 7", 
	L"Menu 8",
	L"Menu 9",
	L"Menu 10",
	L"Menu 11", //10
	L"Menu 12",
	L"Menu 13",
	L"Menu 14",
	L"Menu 15",
	L"Menu 15", // 15
	
	//Briefing Room
	L"Enter",		// TODO.Translate
};

STR16	pOtherButtonsText[] =
{
	L"Briefing", 
	L"Accept",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Briefing",
	L"Accept missions",
};


STR16	pLocationPageText[] =
{
	L"Prev page",
	L"Photo", 
	L"Next page",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"Main page",
	L">>",
	L"Type: ",
	L"Empty data",
	L"Missing of defined missions. Add missions to the file TableData\\BriefingRoom\\BriefingRoom.xml. First mission has to be visible. Put value Hidden = 0.",
	L"Briefing Room. Please click the 'Enter' button.",		// TODO.Translate
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Unknown",// 0 - unknown
	L"City", //1 - cities
	L"SAM Site", //2 - SAM Site
	L"Other location", //3 - other location
	L"Mines", //4 - mines 
	L"Military complex", //5 - military complex 
	L"Laboratory complex",  //6 - laboratory complex 
	L"Factory complex", //7 - factory complex 
	L"Hospital", //8 - hospital 
	L"Prison", //9 - prison
    L"Airport", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Show all",
	L"Show AIM",
	L"Show MERC",
	L"Show RPC",
	L"Show NPC",
	L"Show Pojazd",
	L"Show IMP",
	L"Show EPC",
	L"Filter",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"All",
	L"AIM",
	L"MERC",
	L"RPC",
	L"NPC",
	L"Veh.",
	L"IMP",
	L"EPC",
	L"Filter",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Show all",
	L"Show cities",
	L"Show SAM Sites",
	L"Show other location",
	L"Show mines",
	L"Show military complex",
	L"Show laboratory complex",
	L"Show Factory complex",
	L"Show hospital",
	L"Show prison",
	L"Show air port",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"All",
	L"City",
	L"SAM",
	L"Other",
	L"Mine",
	L"Mil.",
	L"Lab.",
	L"Fact.",
	L"Hosp.",
	L"Prison",
	L"Air.",
};

//TODO.Translate
STR16	pEncyclopediaFilterLocationText[] =
{//major location filter button text max 7 chars
//..L"------v"
	L"All",//0
	L"City",
	L"SAM",
	L"Mine",
	L"Airport",
	L"Wilder.",
	L"Underg.",
	L"Facil.",
	L"Other",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Show All",//facility index + 1
	L"Show Cities",
	L"Show SAM sites",
	L"Show mines",
	L"Show airports",
	L"Show sectors in wilderness",
	L"Show underground sectors",
	L"Show sectors with facilities\n[|L|B]toggle filter\n[|R|B]reset filter",
	L"Show Other sectors",
};

STR16 pEncyclopediaSubFilterLocationText[] =
{//item subfilter button text max 7 chars
//..L"------v"
	L"",//reserved. Insert new city filters above!
	L"",//reserved. Insert new SAM filters above!
	L"",//reserved. Insert new mine filters above!
	L"",//reserved. Insert new airport filters above!
	L"",//reserved. Insert new wilderness filters above!
	L"",//reserved. Insert new underground sector filters above!
	L"",//reserved. facility filter texts are dynamicly loaded, leave this marker empty!
	L"",//reserved. Insert new other filters above!
};
//TODO.Translate
STR16	pEncyclopediaFilterCharText[] =
{//major char filter button text
//..L"------v"
	L"All",//0
	L"A.I.M.",
	L"MERC",
	L"RPC",
	L"NPC",
	L"IMP",
	L"Other",//add new filter buttons before other
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Show All",//Other index + 1
	L"Show A.I.M. members",
	L"Show M.E.R.C staff",
	L"Show Rebels",
	L"Show Non-hirable Characters",
	L"Show Player created Characters",
	L"Show Other\n[|L|B] toggle filter\n[|R|B] reset filter",
};
//TODO.Translate
STR16	pEncyclopediaSubFilterCharText[] =
{//item subfilter button text
//..L"------v"
	L"",//reserved. Insert new AIM filters above!
	L"",//reserved. Insert new MERC filters above!
	L"",//reserved. Insert new RPC filters above!
	L"",//reserved. Insert new NPC filters above!
	L"",//reserved. Insert new IMP filters above!
//Other-----v"
	L"Vehic.",
	L"EPC",
	L"",//reserved. Insert new Other filters above!
};
//TODO.Translate
STR16	pEncyclopediaFilterItemText[] = 
{//major item filter button text max 7 chars
//..L"------v"
	L"All",//0
	L"Gun",
	L"Ammo",
	L"Armor",
	L"LBE",
	L"Attach.",
	L"Misc",//add new filter buttons before misc
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Show All",//misc index + 1
	L"Show Guns\n[|L|B] toggle filter\n[|R|B] reset filter",
	L"Show Amunition\n[|L|B] toggle filter\n[|R|B] reset filter",
	L"Show Armor Gear\n[|L|B] toggle filter\n[|R|B] reset filter",
	L"Show LBE Gear\n[|L|B] toggle filter\n[|R|B] reset filter",
	L"Show Attachments\n[|L|B] toggle filter\n[|R|B] reset filter",
	L"Show Misc Items\n[|L|B] toggle filter\n[|R|B] reset filter",
};
//TODO.Translate
STR16 pEncyclopediaSubFilterItemText[] =
{//item subfilter button text max 7 chars
//..L"------v"
//Guns......v"
	L"Pistol",
	L"M.Pist.",
	L"SMG",
	L"Rifle",
	L"SN Rif.",
	L"AS Rif.",
	L"MG",
	L"Shotgun",
	L"H.Weap.",
	L"",//reserved. insert new gun filters above!
//Amunition.v"
	L"Pistol",
	L"M.Pist.",
	L"SMG",
	L"Rifle",
	L"SN Rif.",
	L"AS Rif.",
	L"MG",
	L"Shotgun",
	L"H.Weap.",
	L"",//reserved. insert new ammo filters above!
//Armor.....v"
	L"Helmet",
	L"Vest",
	L"Pant",
	L"Plate",
	L"",//reserved. insert new armor filters above!
//LBE.......v"
	L"Tight",
	L"Vest",
	L"Combat",
	L"Backp.",
	L"Pocket",
	L"Other",
	L"",//reserved. insert new LBE filters above!
//Attachments"
	L"Optic",
	L"Side",
	L"Muzzle",
	L"Extern.",
	L"Intern.",
	L"Other",
	L"",//reserved. insert new attachment filters above!
//Misc......v"
	L"Blade",
	L"T.Knife",
	L"Punch",
	L"Grenade",
	L"Bomb",
	L"Medikit",
	L"Kit",
	L"Face",
	L"Other",
	L"",//reserved. insert new misc filters above!
//add filters for a new button here
};
//TODO.Translate
STR16 pEncyclopediaFilterQuestText[] =
{//major quest filter button text max 7 chars
//..L"------v"
	L"All",
	L"Active",
	L"Compl.",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Show All",//misc index + 1
	L"Show Active Quests",
	L"Show Completed Quests",
};

STR16 pEncyclopediaSubFilterQuestText[] =
{//Quest subfilter button text max 7 chars, not used, but needed if any subfilters are added
//..L"------v"
	L"",//reserved. insert new active quest subfilters above!
	L"",//reserved. insert new completed quest subfilters above!
};

STR16	pEncyclopediaShortInventoryText[] = 
{
	L"All", //0
	L"Gun",
	L"Ammo",
	L"LBE",
	L"Misc",
	
	L"All", //5
	L"Gun",
	L"Ammo",
	L"LBE Gear",
	L"Misc",
};

STR16			BoxFilter[] =
{
	// Guns
	L"Heavy",
	L"Pistol",
	L"M. Pist.",
	L"SMG",
	L"Rifle",
	L"S. Rifle",
	L"A. Rifle",
	L"MG",
	L"Shotgun",

	// Ammo
	L"Pistol",
	L"M. Pist.", //10
	L"SMG",
	L"Rifle",
	L"S. Rifle",
	L"A. Rifle",
	L"MG",
	L"Shotgun",

	// Used
	L"Guns",
	L"Armor",
	L"LBE Gear",
	L"Misc", //20

	// Armour
	L"Helmets",
	L"Vests",
	L"Leggings",
	L"Plates",

	// Misc
	L"Blades",
	L"Th. Knife",
	L"Melee",
	L"Grenades",
	L"Bombs",
	L"Med.", //30
	L"Kits",
	L"Face",
	L"LBE",
	L"Misc.", //34
};
//-----------


// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Delete",
	L"Delete item (|D|e|l)",
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
 L"Delete currently selected merc (|D|e|l)",
 L"NEXT", //20
 L"Find next merc (|S|p|a|c|e)",
 L"Toggle priority existance",
 L"Toggle whether or not placement has/naccess to all doors",
 
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

	L"Toggle |Erase mode",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Cycle brush size (|A/|Z)",
	L"Roofs (|H)",
	L"|Walls", //30
	L"Room Info (|N)",
};

STR16 iEditorItemsToolbarText[] =
{
	L"Wpns", //0
	L"Ammo",
	L"Armour",
	L"LBE",
	L"Exp",
	L"E1",
	L"E2",
	L"E3",
	L"Triggers",
	L"Keys",
	L"Rnd", //10
	L"Previous (|,)",		// previous page
	L"Next (|.)",		// next page
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Add ambient light source", //0
	L"Toggle fake ambient lights.",
	L"Add exit grids (r-clk to query existing).",
	L"Cycle brush size (|A/|Z)",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Toggle |Erase mode",
	L"Specify north point for validation purposes.",
	L"Specify west point for validation purposes.",
	L"Specify east point for validation purposes.",
	L"Specify south point for validation purposes.",
	L"Specify center point for validation purposes.", //10
	L"Specify isolated point for validation purposes.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"New outdoor level",  //0
	L"New basement",
	L"New cave level",
	L"Save map (|C|t|r|l+|S)",
	L"Load map (|C|t|r|l+|L)",
	L"Select tileset",
	L"Leave Editor mode",
	L"Exit game (|A|l|t+|X)",
	L"Create radar map",
	L"When checked, the map will be saved in original JA2 map format.\nThis option is only valid on 'normal' size maps that do not reference grid numbers (e.g: exit grids) > 25600.",
	L"When checked and you load a map, the map will be enlarged automatically depending on the selected Rows and Cols.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Draw |Ground textures", //0
	L"Set map ground textures",
	L"Place banks and |Cliffs",
	L"Draw roads (|P)",
	L"Draw |Debris",
	L"Place |Trees & bushes",
	L"Place |Rocks",
	L"Place barrels & |Other junk",
	L"Fill area",
	L"Undo last change (|B|a|c|k|s|p|a|c|e)",
	L"Toggle |Erase mode", //10
	L"Cycle brush size (|A/|Z)",
	L"Raise brush density (|])",
	L"Lower brush density (|[)",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Terrain", //0
	L"Buildings",
	L"Items",
	L"Mercs",
	L"Map Info",
	L"Options",
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Terrain fasthelp text
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Buildings fasthelp text
	L"|C|t|r|l+|/: Place new item under mouse cursor\n|/: Place same item under mouse cursor", //Items fasthelp text
	L"|1-|9: Set waypoints\n|C|t|r|l+|C/|C|t|r|l+|V: Copy/Paste merc", //Mercs fasthelp text
	L"|C|t|r|l+|G: Go to grid no\n \n|I: Toggle overhead map\n|J: Toggle draw high ground\n|K: Toggle high ground markers\n|S|h|i|f|t+|L: Toggle map edge points\n|S|h|i|f|t+|T: Toggle treetops\n|U: Toggle world raise\n \n|./|,: Cycle 'width: xx' dimensions", //Map Info fasthelp text
	L"|C|t|r|l+|N: Create new map\n \n|F|5: Show Summary Info/Country Map\n|F|1|0: Remove all lights\n|F|1|1: Reverse schedules\n|F|1|2: Clear schedules\n \n|S|h|i|f|t+|R: Toggle random placement based on quantity of selected object(s)", //Options fasthelp text
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

STR16 zDiffNames[5] = 
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
	L"Placement not pasted as the maximum number of placements for this team has been reached.",
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
/*CHAR16 gszActionItemDesc[ 34 ][ 30 ] =	// NUM_ACTIONITEMS = 34
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
*/
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
	L"Item appears in both Sci-Fi and Realistic modes", //0
	L"Item appears in Realistic mode only",
	L"Item appears in Sci-Fi mode only",
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
	L"Summary Information for sector %s:", //20
	
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
	L"Cтp. 1/2", //0
	L"Cтp. 2/2",
	
	L"Cтp. 1/3",
	L"Cтp. 2/3",
	L"Cтp. 3/3",
	
	L"Cтp. 1/1", //5
};

// by Jazz
CHAR16 zGrod[][500] =
{
	L"Робот", //0    // Robot
};

STR16 pCreditsJA2113[] =
{
	L"@T,{;Разработчики JA2 v1.13",
	L"@T,C144,R134,{;Программирование",
	L"@T,C144,R134,{;Графика и звук",
	L"@};(Многое взято из других модов)",
	L"@T,C144,R134,{;Предметы",
	L"@T,C144,R134,{;Также помогали",
	L"@};(И многие другие, предложившие хорошие идеи и высказавшие важные замечания!)",
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
//	L",38 кал",
//	L"9мм",
//	L",45 кал",
//	L",357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45мм",
//	L"5,56мм",
//	L"7,62мм НАТО",
//	L"7,62мм ВД",
//	L"4,7мм",
//	L"5,7мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
//	L"", // пламя
////	L".50 cal", // barrett
////	L"9mm Hvy", // Val silent
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
//	L",38 кал",
//	L"9мм",
//	L",45 кал",
//	L",357 кал",
//	L"12 кал",
//	L"ОББ",
//	L"5,45мм",
//	L"5,56мм",
//	L"7,62мм Н.",
//	L"7,62мм ВД",
//	L"4,7мм",
//	L"5.7мм",
//	L"Монстр",
//	L"Ракета",
//	L"", // дротик
////	L"", // flamethrower
////	L".50 cal", // barrett
////	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"",			//Other
	L"Пистолет",		//Pistol
	L"Авт.пистолет",	//MP 'Автоматический пистолет'
	L"ПП",			//SMG 'Пистолет-пулемет'
	L"Винтовка",		//Rifle
	L"Сн.винтовка",		//Sniper rifle
	L"Шт.винтовка",		//Assault rifle
	L"Ручной пулемёт",	//LMG
	L"Ружьё",		//Shotgun
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Ход Игрока", // player's turn
	L"Ход Противника",
	L"Ход Тварей",
	L"Ход Ополчения",
	L"Ход Гражданских",
	L"Player_Plan",// planning turn
	L"Client №1",//hayden
	L"Client №2",//hayden
	L"Client №3",//hayden
	L"Client №4",//hayden
};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s получает ранение в голову и теряет в интеллекте!",
	L"%s получает ранение в плечо и теряет в ловкости!",
	L"%s получает ранение в грудь и теряет в силе!",
	L"%s получает ранение в ногу и теряет в проворности!",
	L"%s получает ранение в голову и теряет %d единиц интеллекта!",
	L"%s получает ранение в плечо и теряет %d единиц ловкости!",
	L"%s получает ранение в грудь и теряет %d единиц силы!",
	L"%s получает ранение в ногу и теряет %d единиц проворности!",
	L"Перехват!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"К вам на помощь прибыло подкрепление!",

	// In the following four lines, all %s's are merc names

	L"%s перезаряжает оружие.",
	L"%s недостаточно очков действия!",
	L"%s оказывает первую помощь (любая клавиша - отмена).",
	L"%s и %s оказывают первую помощь (любая клавиша - отмена).",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"надёжно",
	L"ненадёжно",
	L"простой ремонт",
	L"сложный ремонт",
	L"большой урон",
	L"малый урон",
	L"скорострельное",
	L"нескорострельное",
	L"дальний бой",
	L"ближний бой",
	L"лёгкое",
	L"тяжёлое",
	L"компактное",
	L"очередями",	//fast burst fire
	L"нет отсечки очереди",
	L"бол.магазин",
	L"мал.магазин",

	// In the following two lines, all %s's are merc names

	L"%s: камуфляжная краска стёрлась.",
	L"%s: камуфляжная краска смылась.",

	// The first %s is a merc name and the second %s is an item name

	L"Второе оружие: закончились патроны!",
	L"%s крадёт %s.",

	// The %s is a merc name

	L"%s: оружие не стреляет очередями.",

	L"Уже установлено!",
	L"Объединить?",

	// Both %s's are item names

	L"Нельзя присоединить %s к %s.",

	L"Ничего",
	L"Разрядить", 
	L"Навеска",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"Нельзя использовать %s и %s одновременно.",

	L"Этот предмет можно присоединить к другим, поместив его в одно из четырех мест для навески.",
	L"Этот предмет можно присоединить к другим, поместив его в одно из четырех мест для навески. (Однако эти предметы несовместимы)",
	L"В секторе еще остались враги!",
	L"%s требует полную оплату, нужно заплатить ещё %s", 
	L"%s: попадание в голову!",
	L"Покинуть битву?",
	L"Это несъёмное приспособление. Установить его?",
	L"%s чувствует прилив энергии!",
	L"%s поскальзывается на стеклянных шариках!",
	L"%s не удалось отобрать %s у врага!",
	L"%s чинит %s",
	L"Перехватили ход: ",
	L"Сдаться?",
	L"Человек отверг вашу помощь.",
	L"Вам это надо?",
  L"Чтобы воспользоваться вертолётом Небесного Всадника - выберите \"Транспорт/Вертолёт\".",
	L"%s успевает зарядить только одно оружие.",
	L"Ход кошек-убийц", //Bloodcats' turn
	L"автоматический", //full auto
	L"неавтоматический", //no full auto
	L"точный", //accurate
	L"неточный", //inaccurate
	L"нет одиночных", //no semi auto
	L"Враг обобран до нитки!",
	L"У врага в руках ничего нет!",

	L"%s: песчаный камуфляж нанесён.",
	L"%s: песчаный камуфляж смыт.",
	
	L"%s: растительный камуфляж нанесён.",
	L"%s: растительный камуфляж смыт.",
	
	L"%s: городской камуфляж нанесён.",
	L"%s: городской камуфляж смыт.",
	
	L"%s: арктический камуфляж нанесён.",
	L"%s: арктический камуфляж смыт.",

	L"Нельзя установить навеску %s на это место.",
	L"The %s will not fit in any open slots.",
	L"There's not enough space for this pocket.", //TODO:Translate
};


// the names of the towns in the game

CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT] =
{
	L"",
	L"Омерта",
	L"Драссен",
	L"Альма",
	L"Грам",
	L"Тикса",
	L"Камбрия",
	L"Сан-Мона",
	L"Эстони",
	L"Орта",
	L"Балайм",
	L"Медуна",
	L"Читзена",
};

// the types of time compression. For example: is the timer paused? at normal speed, 5 minutes per second, etc.
// min is an abbreviation for minutes

STR16 sTimeStrings[] =
{
	L"Пауза",
	L"Норма",
	L"5 мин",
	L"30 мин",
	L"60 мин",
	L"6 часов",
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",	// on active duty
	L"Медик",	// administering medical aid
	L"Пациент",	// getting medical aid
	L"Транспорт",	// in a vehicle
	L"В пути",	// in transit - abbreviated form
	L"Ремонт",	// repairing
	L"Radio Scan",	// scanning for nearby patrols	// TODO.Translate
	L"Практика",	// training themselves  
	L"Ополчение",		// training a town to revolt 
	L"Мобил.гр.", //training moving militia units		//M.Militia
	L"Тренер",	// training a teammate
	L"Ученик",	// being trained by someone else
	L"Move Item",	// move items // TODO.Translate
	L"Штат", // operating a strategic facility			//Staff
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Отдых", // Resting at a facility					//Rest
	L"Prison",		// Flugente: interrogate prisoners
	L"Мёртв",	// dead
	L"Недеесп.",	// abbreviation for incapacitated
	L"В плену",	// Prisoner of war - captured
	L"Госпиталь",	// patient in a hospital 
	L"Пуст",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"Ополчение", // the title of the militia box
	L"Запас", //the number of unassigned militia troops
	L"Нельзя перераспределять ополчение во время боя!",
	L"Здесь присутствуют ополченцы из других секторов. Распустить их по своим прежним позициям?", //Some militia were not assigned to a sector. Would you like to disband them?
};


STR16 pMilitiaButtonString[] =
{
	L"Авто", // auto place the militia troops for the player
	L"Готово", // done placing militia troops
	L"Распустить", // HEADROCK HAM 3.6: Disband militia	//Disband
	L"Unassign All", // move all milita troops to unassigned pool	// TODO.Translate
};

STR16 pConditionStrings[] = 
{
	L"Отличное",	//the state of a soldier .. excellent health
	L"Хорошее",	//good health
	L"Сносное",	//fair health
	L"Ранен",	//wounded health
	L"Устал",	//tired
	L"Кровоточит", //bleeding to death
	L"Без сознания", //knocked out 
	L"Умирает",	//near death
	L"Мертв",	//dead
};

STR16 pEpcMenuStrings[] =
{
	L"На службе", // set merc on active duty
	L"Пациент", // set as a patient to receive medical aid
	L"Транспорт", // tell merc to enter vehicle
	L"Без эскорта", // let the escorted character go off on their own
	L"Отмена", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",
	L"Медик",
	L"Пациент",
	L"Транспорт",
	L"В пути",
	L"Ремонт",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Практика", 
	L"Ополчение",
	L"Тренирует мобильную группу",	//Training Mobile Militia
	L"Тренер",
	L"Ученик",
	L"Move item",	// move items // TODO.Translate
	L"Работает с населением",			//Facility Staff
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Отдыхает",		//Resting at Facility
	L"Interrogate prisoners",		// Flugente: interrogate prisoners	TODO.Translate
	L"Мёртв",
	L"Недеесп.",
	L"В плену",
	L"Госпиталь", 
	L"Пуст",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Отряд 1",
	L"Отряд 2",
	L"Отряд 3",
	L"Отряд 4",
	L"Отряд 5",
	L"Отряд 6",
	L"Отряд 7",
	L"Отряд 8",
	L"Отряд 9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"На службе",
	L"Медик",
	L"Пациент",
	L"В транспорте",
	L"В пути",
	L"Ремонтирует",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Практикуется", 
	L"Тренирует ополчение",
	L"Тренирует мобильную группу",	//Train Mobiles
	L"Тренирует",
	L"Обучается",
	L"Move Item",	// move items // TODO.Translate
	L"Работает с населением",		//Staff Facility
	L"Отдыхает в заведении",		//Resting at Facility
	L"Interrogate prisoners",		// Flugente: interrogate prisoners TODO.Translate
	L"Мёртв",
	L"Недееспособен",
	L"В плену",
	L"В госпитале", // patient in a hospital 
	L"Без пассажиров",	// Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"Изменение контракта:", 
	L"", // a blank line, required
	L"Продлить на 1 день", // offer merc a one day contract extension
	L"Продлить на 7 дней", // 1 week
	L"Продлить на 14 дней", // 2 week
	L"Уволить", // end merc's contract
	L"Отмена", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"В плену",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
    L"СИЛА", 
	L"ЛОВКОСТЬ",  
	L"ПРОВОРНОСТЬ", 
	L"ИНТЕЛЛЕКТ", 
	L"МЕТКОСТЬ",
	L"МЕДИЦИНА",
	L"МЕХАНИКА",
	L"ЛИДЕРСТВО",
	L"ВЗРЫВЧАТКА",
	L"УРОВЕНЬ",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Броня", // the armor rating of the merc
	L"Вес", // the weight the merc is carrying
	L"Камуф.", // the merc's camouflage rating
	L"Камуфляж:",
	L"Броня:",
};

STR16 pShortAttributeStrings[] =
{
	L"Прв", // the abbreviated version of : agility
	L"Лов", // dexterity
	L"Сил", // strength
	L"Лид", // leadership
	L"Инт", // wisdom
	L"Опт", // experience level
	L"Мет", // marksmanship skill
	L"Мех", // mechanical skill
	L"Взр", // explosive skill
	L"Мед", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Назначение", // the mercs current assignment 
	L"Контракт", // the contract info about the merc
	L"Здоровье", // the health level of the current merc
	L"Боев.дух", // the morale of the current merc
	L"Сост.",	// the condition of the current vehicle
	L"Топливо",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Тренинг", // tell merc to train self 
    L"Ополчение", // tell merc to train town 
	L"Тренер", // tell merc to act as trainer
	L"Ученик", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Ведение огня:", // the allowable rate of fire for a merc who is guarding
	L" Агрессивная атака", // the merc can be aggressive in their choice of fire rates
	L" Беречь патроны", // conserve ammo 
	L" Воздержаться от стрельбы", // fire only when the merc needs to 
	L"Другие параметры:", // other options available to merc
	L" Может отступить", // merc can retreat
	L" Может искать укрытие",  // merc is allowed to seek cover
	L" Может помочь команде", // merc can assist teammates
	L"Готово", // done with this menu
	L"Отмена", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Ведение огня:",
	L" *Агрессивная атака*",
	L" *Беречь патроны*",
	L" *Воздержаться от стрельбы*",
	L"Другие параметры:",
	L" *Может отступить*",
	L" *Может искать укрытие*",
	L" *Может помочь команде*",
	L"Готово",
	L"Отмена",
};

STR16 pAssignMenuStrings[] =
{
	L"На службе", // merc is on active duty
	L"Медик", // the merc is acting as a doctor
	L"Пациент", // the merc is receiving medical attention
	L"Машина", // the merc is in a vehicle
	L"Ремонт", // the merc is repairing items 
	L"Radio Scan", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Обучение", // the merc is training
	L"Move Item",	// move items	// TODO.Translate
	L"Удобства", // the merc is using/staffing a facility	//Facility
	L"Отмена", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"В атаку", // set militia to aggresive
	L"Держать оборону", // set militia to stationary
	L"Отступать", // retreat militia
	L"За мной", // retreat militia
	L"Ложись", // retreat militia	
	L"Crouch",	// TODO.Translate
	L"В укрытие",
	L"Все в атаку", 
	L"Всем держать оборону",
	L"Всем отступать",
	L"Все за мной",
	L"Всем рассеяться",
	L"Всем залечь",
	L"All: Crouch",	// TODO.Translate
	L"Всем в укрытие",
	//L"Всем искать предметы",
	L"Отмена", // cancel this menu
};

//Flugente
STR16 pTraitSkillsMenuStrings[] =	// TODO.Translate
{
	// radio operator
	L"Artillery Strike",
	L"Jam communications",
	L"Scan frequencies",
	L"Eavesdrop",
	L"Call reinforcements",
	L"Switch off radio set",
};

//Flugente: short description of the above skills for the skill selection menu
STR16 pTraitSkillsMenuDescStrings[] =
{
	// radio operator
	L"Order an artillery strike from sector...",
	L"Fill all radio frequencies with white noise, making communications impossible.",
	L"Scan for jamming signals.",
	L"Use your radio equipment to continously listen for enemy movement",
	L"Call in reinforcements from neighbouring sectors.",
	L"Turn off radio set to save batteries.",
};

STR16 pTraitSkillsDenialStrings[] =
{
	L"Requires:\n",
	L" - %d AP\n",
	L" - %s\n",
	L" - %s or higher\n",
	L" - %s or higher or\n",
	L" - %d minutes to be ready\n",
	L" - mortar positions in neighbouring sectors\n",
	L" - %s |o|r %s |a|n|d %s or %s or higher\n"
	L" - posession by a demon",
};


//STR16 pTalkToAllMenuStrings[] =
//{
//	L"В атаку", // set militia to aggresive
//	L"Держать оборону", // set militia to stationary
//	L"Отступать", // retreat militia
//	L"За мной", // retreat militia
//	L"Ложись", // retreat militia		 
//	L"Отмена", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"Убрать бойца", // remove dead merc from current team
	L"Отмена",
};

STR16 pAttributeMenuStrings[] =
{
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Отмена",
};

STR16 pTrainingMenuStrings[] =
{
 L"Практика", // train yourself 
 L"Ополчение", // train the town 
 L"Мобил. группа",		//Mobile Militia
 L"Тренер", // train your teammates 
 L"Ученик",  // be trained by an instructor 
 L"Отмена", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Отряд  1",
	L"Отряд  2",
	L"Отряд  3",
	L"Отряд  4",
	L"Отряд  5",
	L"Отряд  6",
	L"Отряд  7",
	L"Отряд  8",
	L"Отряд  9",
	L"Отряд 10",
	L"Отряд 11",
	L"Отряд 12",
	L"Отряд 13",
	L"Отряд 14",
	L"Отряд 15",
	L"Отряд 16",
	L"Отряд 17",
	L"Отряд 18",
	L"Отряд 19",
	L"Отряд 20",
	L"Отмена",
};

STR16 pPersonnelTitle[] =
{
	L"Команда", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Здоровье:",		// health of merc
	L"Проворность:", 
	L"Ловкость:",
	L"Сила:",
	L"Лидерство:",
	L"Интеллект:",
	L"Опыт:",		// experience level
	L"Меткость:", 
	L"Механика:",
	L"Взрывчатка:",
	L"Медицина:",
	L"Мед. депозит:",	// amount of medical deposit put down on the merc
	L"До конца контракта:",	// cost of current contract
	L"Убил врагов:",	// number of kills by merc
	L"Помог убить:",	// number of assists on kills by merc
	L"Гонорар за день:",	// daily cost of merc
	L"Общая цена услуг:",	// total cost of merc
	L"Контракт:",		// cost of current contract
	L"У вас на службе:",	// total service rendered by merc
	L"Задолж. жалования:",	// amount left on MERC merc to be paid
	L"Процент попаданий:",	// percentage of shots that hit target
	L"Боёв:", 		// number of battles fought
	L"Ранений:",		// number of times merc has been wounded
	L"Навыки:",
	L"Нет навыков",
	L"Достижения:", //Achievements
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Элиты: %d\n",
	L"Солдат: %d\n",
	L"Полиции: %d\n",
	L"Враждебных граждан: %d\n",
	L"Животных: %d\n",
	L"Танков: %d\n", 
	L"Других объектов: %d\n",

	L"Своим: %d\n",
	L"Ополчению: %d\n",
	L"Другим: %d\n",

	L"Выпущено пуль: %d\n",
	L"Выпущено ракет: %d\n",
	L"Брошено гранат: %d\n",
	L"Брошено ножей: %d\n",
	L"Ударов ножом: %d\n",
	L"Ударов кулаками: %d\n",
	L"Удачных попаданий: %d\n",

	L"Замков взломано: %d\n",
	L"Замков сорвано: %d\n",
	L"Ловушек обезврежено: %d\n",
	L"Взрывчатки взорвано: %d\n",
	L"Предметов отремонтированно: %d\n",
	L"Предметов собрано: %d\n",
	L"Вещей украдено: %d\n",
	L"Ополченцев натренировано: %d\n",
	L"Бойцов перевязано: %d\n",
	L"Заданий: %d\n",
	L"Встречено информаторов: %d\n",
	L"Секторов разведано: %d\n",
	L"Выйдено из окружения: %d\n",	//Ambushes Prevented
	L"Заданий жителей выполнено: %d\n",

	L"Тактических сражений: %d\n",
	L"Автобитв: %d\n",
	L"Количество отступлений: %d\n",
	L"Попаданий в засады: %d\n",
	L"Крупнейшая битва: %d врагов\n",

	L"Стреляных ран: %d\n",
	L"Ножевых ран: %d\n",
	L"Пропущенных ударов: %d\n",
	L"Подорвался: %d\n",
	L"Ухудшений параметров: %d\n",
	L"Перенёс мед. вмешательств: %d\n",	//Surgeries undergone
	L"Травм на производстве: %d\n",	//Facility Accidents

	L"Характер:",
	L"Недостаток:",

	L"По жизни:",	//Attitudes For old traits display instead of "Character:"!

	L"Зомби: %d\n",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Нет навыка",
	L"Взлом замков",
	L"Рукопашный бой",
	L"Электроника",
	L"Ночные операции", 
	L"Метание",
	L"Инструктор",
	L"Тяжелое оружие",
	L"Автоматическое оружие",
	L"Скрытность",
	L"Ловкач",
	L"Воровство",
	L"Боевые искусства",
	L"Холодное оружие",
	L"Снайпер", 
	L"Камуфляж",
	L"(Эксперт)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Нет навыка",	//No Skill
	L"Автоматчик",	//Auto Weapons
	L"Гренадёр",	//Heavy Weapons
	L"Стрелок",	//Marksman
	L"Охотник",	//Hunter
	L"Ковбой",	//Gunslinger
	L"Боксёр",	//Hand to Hand
	L"Старшина",	//Deputy
	L"Механик-электронщик",	//Technician
	L"Санитар",	//Paramedic
	// Minor traits
	L"Ловкач",	//Ambidextrous
	L"Мастер клинка",	//Melee
	L"Мастер по метанию",	//Throwing
	L"Ночник",	//Night Ops
	L"Бесшумный убийца",	//Stealthy
	L"Спортсмен",	//Athletics
	L"Культурист",	//Bodybuilding
	L"Подрывник",	//Demolitions
	L"Инструктор",	//Teaching
	L"Разведчик",	//Scouting
	// covert ops is a major trait that was added later
	L"Covert Ops",		// 20	// TODO.Translate
	// new minor traits
	L"Radio Operator",	// 21
	// second names for major skills
	L"Пулемётчик",	//Machinegunner
	L"Артиллерист",	//Bombardier
	L"Снайпер",	//Sniper
	L"Рейнджер",	//Ranger
	L"Пистолетчик",	//Gunfighter
	L"Боевые искусства",	//Martial Arts
	L"Командир",	//Squadleader
	L"Инженер",	//Engineer
	L"Доктор",	//Doctor
	L"Placeholder",		// 31
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 36
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 40
	L"Шпион",			// 41
	L"Placeholder",		// for radio operator (minor trait)
	L"Ещё...",
};
//////////////////////////////////////////////////////////

// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"Встать/Идти (|S)", 
	L"Присесть/Гусиный шаг (|C)",
	L"Стоять/Бежать (|R)",
	L"Лечь/Ползти (|P)",
	L"Поворот (|L)",
	L"Действие",
	L"Поговорить",
	L"Осмотреть (|C|t|r|l)",

	// Pop up door menu
	L"Открыть",
	L"Искать ловушки", 
	L"Вскрыть отмычками", 
	L"Открыть cилой",
	L"Обезвредить",
	L"Запереть",
	L"Отпереть",
	L"Использовать заряд взрывчатки", 
	L"Взломать ломом",
	L"Отмена (|E|s|c)",
	L"Закрыть",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"нет ловушки",
	L"бомба-ловушка",
	L"электроловушка",
	L"сирена",
	L"сигнализация",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"1 день",
	L"7 дней",
	L"14 дней",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Выбрать наемника",
	L"Отдать приказ",
	L"Проложить путь движения",
	L"Контракт наемника (|C)",
	L"Местонахождение бойца",
	L"Спать", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"ТИХИЙ",
	L"ЧЕТКИЙ",
	L"ГРОМКИЙ",
	L"ОЧЕНЬ ГРОМКИЙ",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"НЕЗНАКОМЫЙ",
	L"ЗВУК ШАГОВ",
	L"СКРИП",
	L"ВСПЛЕСК",
	L"УДАР",
	L"ВЫСТРЕЛ",
	L"ВЗРЫВ",
	L"КРИК",
	L"УДАР",
	L"УДАР",
	L"ЗВОН",
	L"ГРОХОТ",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"c СЕВЕРО-ВОСТОКА",
	L"c ВОСТОКА",
	L"c ЮГО-ВОСТОКА",
	L"c ЮГА",
	L"c ЮГО-ЗАПАДА",
	L"c ЗАПАДА",
	L"c СЕВЕРО-ЗАПАДА",
	L"c СЕВЕРА",
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Город",
	L"Дорога",
	L"Равнина",
	L"Пустыня",
	L"Прерия",
	L"Лес",	 
	L"Болото",
	L"Вода",	
	L"Холмы",
	L"Непроходимо",
	L"Река",	//river from north to south
	L"Река",	//river from east to west
	L"Чужая страна",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Тропики",
	L"Ферма",
	L"Поля, дорога",
	L"Леса, дорога",
	L"Ферма, дорога",
	L"Тропики, дорога",
	L"Леса, дорога",
	L"Побережье",
	L"Горы, дорога",
	L"Берег, дорога",
	L"Пустыня, дорога",
	L"Болота, дорога",
	L"Прерия, ПВО", 
	L"Пустыня, ПВО",
	L"Тропики, ПВО",
	L"Медуна, ПВО",
	
	//These are descriptions for special sectors
	L"Госпиталь Камбрии",
	L"Аэропорт Драссена",
	L"Аэропорт Медуны",
	L"База ПВО",
	L"Убежище повстанцев",	//The rebel base underground in sector A10
	L"Подвалы Тиксы",	//The basement of the Tixa Prison (J9)
	L"Логово тварей",	//Any mine sector with creatures in it
	L"Подвалы Орты",	//The basement of Orta (K4)
	L"Туннель",		//The tunnel access from the maze garden in Meduna 
	//leading to the secret shelter underneath the palace
	L"Убежище",		//The shelter underneath the queen's palace
	L"",			//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s замечен в секторе %c%d, и другой отряд уже на подходе.",	//STR_DETECTED_SINGULAR
	L"%s замечен в секторе %c%d, и остальные отряды уже на подходе.",	//STR_DETECTED_PLURAL
	L"Желаете дождаться прибытия остальных?",													//STR_COORDINATE

	//Dialog strings for enemies.

	L"Враг предлагает вам сдаться.",			//STR_ENEMY_SURRENDER_OFFER
	L"Оставшиеся без сознания бойцы попали в плен.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Отступить", 		//The retreat button		//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button			//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"ОБОРОНА",								//STR_AR_DEFEND_HEADER
	L"АТАКА",								//STR_AR_ATTACK_HEADER
	L"ВСТРЕЧА",								//STR_AR_ENCOUNTER_HEADER
	L"Сектор",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"ПОБЕДА!",					//STR_AR_OVER_VICTORY
	L"ПОРАЖЕНИЕ!",					//STR_AR_OVER_DEFEAT
	L"СДАЛСЯ!",					//STR_AR_OVER_SURRENDERED
	L"ПЛЕНЁН!",					//STR_AR_OVER_CAPTURED
	L"ОТСТУПИЛ!",					//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Ополченец",					//STR_AR_MILITIA_NAME,
	L"Элита",					//STR_AR_ELITE_NAME,
	L"Солдат",					//STR_AR_TROOP_NAME,
	L"Полиция",					//STR_AR_ADMINISTRATOR_NAME,
	L"Рептион",					//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Прошло времени",				//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"ОТСТУПИЛ",					//STR_AR_MERC_RETREATED,
	L"ОТСТУПАЕТ",					//STR_AR_MERC_RETREATING,
	L"ОТСТУПИТЬ",					//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Авто битва",					//STR_PB_AUTORESOLVE_BTN,
	L"Перейти в сектор",				//STR_PB_GOTOSECTOR_BTN,
	L"Уйти из сектора",				//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"ВСТРЕЧА С ВРАГОМ",				//STR_PB_ENEMYENCOUNTER_HEADER,
	L"НАСТУПЛЕНИЕ ВРАГА",				//STR_PB_ENEMYINVASION_HEADER, // 30
	L"ВРАЖЕСКАЯ ЗАСАДА",				//STR_PB_ENEMYAMBUSH_HEADER
	L"ВРАЖЕСКИЙ СЕКТОР",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"АТАКА ТВАРЕЙ",				//STR_PB_CREATUREATTACK_HEADER
	L"ЗАСАДА КОШЕК-УБИЙЦ",				//STR_PB_BLOODCATAMBUSH_HEADER
	L"ВХОД В ЛОГОВИЩЕ КОШЕК-УБИЙЦ",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Локация",
	L"Враг",
	L"Наемники",
	L"Ополчение",
	L"Рептионы",
	L"Кошки-убийцы",
	L"Сектор",
	L"Нет",			//If there are no uninvolved mercs in this fight.
	L"Н/Д",			//Acronym of Not Applicable
	L"д",			//One letter abbreviation of day
	L"ч",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Отмена",
	L"Случайно",
	L"Группой",
	L"B aтaку!",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Убирает все позиции бойцов \nи позволяет заново расставить их. (|C)", 
	L"При каждом нажатии распределяет \nбойцов случайным образом. (|S)",
	L"Позволяет выбрать место, \nгде сгруппировать ваших бойцов. (|G)",
	L"Нажмите эту кнопку, когда завершите \nвыбор позиций для бойцов. (|В|в|о|д)",
	L"Вы должны разместить всех своих бойцов \nдо того, как начать бой.",

	//Various strings (translate word for word)

	L"Сектор",
	L"Выбор точек входа",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Препятствие. Место недоступно. Попробуйте пройти другим путем.",
	L"Поместите бойцов в незатененную часть карты.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"прибыл(а) в сектор", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Автоматически просчитывает бой\nбез загрузки карты. (|A)",
	L"Нельзя включить автобой\nво время нападения.",
	L"Войти в сектор, чтобы атаковать врага. (|E)",
	L"Отступить отрядом в предыдущий сектор. (|R)",				//singular version
	L"Всем отрядам отступить в предыдущий сектор. (|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"Враги атаковали ваших ополченцев в секторе %c%d.",
	//%c%d сектор -- напр:  A9
	L"Твари атаковали ваших ополченцев в секторе %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Твари убили %d гражданских во время атаки сектора %s.",
	//%s is the sector location -- ex:  A9: Omerta
	L"Враги атаковали ваших наемников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",
	//%s is the sector location -- ex:  A9: Omerta
	L"Твари атаковали ваших наемников в секторе %s. Ни один из ваших бойцов не в состоянии сражаться!",

};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"День",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Найдено в секторе:",
	L"Найдено за день:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 19 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Состояние:",
	L"Вес:", 
	L"Нужно ОД",	
	L"Дист:",		// Range
	L"Урон:",		// Damage
	L"Всего:", 		// Number of bullets left in a magazine
	L"ОД:",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Точность:",	//9
	L"Дист:",		//10	
	L"Урон:", 	//11
	L"Вес:",		//12
	L"Оглушение:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Навеска:",	//14	//Attachments
	L"AUTO/5:",		//15
	L"Осталось патрон:",		//16	//Remaining ammo

	L"Предустановка:",	//17 //WarmSteel - So we can also display default attachments
	L"Нагар:",	// 18
	L"Space:", // 19 //space left on Molle items //TODO.Translate

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
STR16		gzWeaponStatsFasthelpTactical[ 33 ] =
{
	// TODO.Translate
	L"|R|a|n|g|e\n \nThe effective range of this weapon. Attacking from\nbeyond this range will lead to massive penalties.\n \nHigher is better.",
	L"|D|a|m|a|g|e\n \nThis is the damage potential of the weapon.\nIt will usually deliver this much damage\n(or close to it) to any unprotected target.\n \nHigher is better.",
	L"|A|c|c|u|r|a|c|y\n \nThis is an innate Chance-to-Hit Bonus (or\npenalty!) given by this gun due to its\nparticular good (or bad) design.\n \nHigher is better.",
	L"|A|i|m|i|n|g |L|e|v|e|l|s\n \nThis is the maximum number of aiming clicks allowed\nwhen using this gun.\n \nEach aiming-click will make an attack more\naccurate.\n \nHigher is better.",
	L"|A|i|m|i|n|g |M|o|d|i|f|i|e|r\n \nA flat modifier, which alters the effectiveness\nof each aiming click you make while using this\nweapon.\n \nHigher is better.",
	L"|M|i|n|. |R|a|n|g|e |f|o|r |A|i|m|i|n|g |B|o|n|u|s\n \nThe minimum range-to-target required before this\nweapon can make use of its Aiming Modifier.\n \nIf the target is closer than this many tiles,\naiming clicks will stay at their default\neffectiveness.\n \nLower is better.",
	L"|T|o|-|H|i|t |M|o|d|i|f|i|e|r\n \nA flat modifier to Chance-to-Hit with any\nattack made using this weapon.\n \nHigher is better.",
	L"|B|e|s|t |L|a|s|e|r |R|a|n|g|e\n \nThe range (in tiles) at which the laser installed\non this weapon will be at its full effectiveness.\n \nWhen attacking a target beyond this range, the\nlaser will provide a smaller bonus or none at all.\n \nHigher is better.",
	L"|F|l|a|s|h |S|u|p|p|r|e|s|s|i|o|n\n \nWhen this icon appears, it means that the gun\ndoes not make a flash when it fires. This helps the\nshooter remain concealed.",
	L"|L|o|u|d|n|e|s|s\n \nAttacks made with this weapon can be heard up to\nthe listed distance (in tiles).\n \nLower is better.\n(unless deliberately trying to draw in enemies...)",
	L"|R|e|l|i|a|b|i|l|i|t|y\n \nThis value indicates (in general) how quickly\nthis weapon will degrade when used in combat.\n \nHigher is better.",
	L"|R|e|p|a|i|r |E|a|s|e\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only Technicians and special\nNPCs can repair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
	L"",	//12
	L"ОД на вскидку",						//APs to ready
	L"ОД на 1 выстрел",						//APs to fire Single
	L"ОД на огонь с отсечкой",					//APs to fire Burst
	L"ОД на огонь очередью",					//APs to fire Auto
	L"ОД на замену магазина",					//APs to Reload
	L"ОД на досылку патрона",					//APs to Reload Manually
	L"Штраф за отдачу при\nстрельбе очередью c отсечкой\n(чем меньше, тем лучше)",	//19		//Burst Penalty (Lower is better)
	L"Бонус от сошек\n(при стрельбе лёжа)",				//Bipod Modifier
	L"Выстрелов в автоматическом\nрежиме за 5 ОД",			//Autofire shots per 5 AP
	L"Штраф за отдачу при \nстрельбе очередью \n(чем меньше, тем лучше)",	//Autofire Penalty (Lower is better)
	L"Штраф за отдачу при\nстрельбе очередью\n(c отсечкой/без) (чем меньше, тем лучше)",	//Burst/Auto Penalty	//23
	L"ОД на бросок",						//APs to Throw
	L"ОД на выстрел",						//APs to Launch
	L"ОД на удар ножом",						//APs to Stab
	L"Не стреляет одиночными!",					//No Single Shot!
	L"Нет отсечки патрона!",					//No Burst Mode!
	L"Нет автоматического режима!",					//No Auto Mode!
	L"ОД на удар",							//APs to Bash
    L"",
	L"|R|e|p|a|i|r |E|a|s|e\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
{
	L"Модификатор размера предмета\n(чем меньше, тем лучше)",		//Item Size Modifier (Lower is better)
	L"Модификатор надёжности",						//Reliability Modifier
	L"Модификатор шумности\n(чем меньше, тем лучше)",			//Loudness Modifier (Lower is better)
	L"Скрывает вспышку",							//Hides Muzzle Flash
	L"Модификатор сошек",							//Bipod Modifier
	L"Модификатор дальнобойности",						//Range Modifier
	L"Модификатор точности",						//To-Hit Modifier
	L"Радиус наилучшей видимости\nточки прицеливания",			//Best Laser Range
	L"Модификатор бонусов оптики",						//Aiming Bonus Modifier
	L"Модификатор очереди с отсечкой",					//Burst Size Modifier
	L"Модификатор штрафа за отдачу\nпри стрельбе c отсечкой\n(чем больше, тем лучше)",	//Burst Penalty Modifier (Higher is better)
	L"Модификатор штрафа за отдачу\nпри стрельбе очередью\n(чем больше, тем лучше)",	//Auto-Fire Penalty Modifier (Higher is better)
	L"Модификатор ОД",							//AP Modifier
	L"Модификатор ОД\nна очередь с отсечкой\n(чем меньше, тем лучше)",	//AP to Burst Modifier (Lower is better)
	L"Модификатор ОД\nна очередь без отсечки\n(чем меньше, тем лучше)",	//AP to Auto-Fire Modifier (Lower is better)
	L"Модификатор ОД на вскидку\n(чем меньше, тем лучше)",			//AP to Ready Modifier (Lower is better)
	L"Модификатор ОД\nна замену магазина\n(чем меньше, тем лучше)",		//AP to Reload Modifier (Lower is better)
	L"Модификатор объёма магазина",						//Magazine Size Modifier
	L"Модификатор ОД на выстрел\n(чем меньше, тем лучше)",			//AP to Attack Modifier (Lower is better)
	L"Модификатор урона",							//Damage Modifier
	L"Модификатор урона\nв ближнем бою", 					//Melee Damage Modifier
	L"Камуфляж 'Лес'",
	L"Камуфляж 'Город'",
	L"Камуфляж 'Пустыня'",
	L"Камуфляж 'Снег'",
	L"Модификатор скрытности", // 25
	L"Модификатор диапазона\nслышимости",
	L"Модификатор диапазона\nвидимости",
	L"Модификатор диапазона\nвидимости днём",
	L"Модификатор диапазона\nвидимости ночью",
	L"Модификатор диапазона\nвидимости при ярком освещении", //30
	L"Модификатор диапазона\nвидимости в пещере",
	L"Сужение сектора обзора\n(чем меньше, тем лучше)",		//Tunnel Vision Percentage (Lower is better)
	L"Минимальная эффективная\nдальность оптики",				//Minimum Range for Aiming Bonus
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"В ",	//In 
	L"ИДЕАЛЬНОМ",
	L"ОТЛИЧНОМ",
	L"ХОРОШЕМ",	//GOOD
	L"НОРМАЛЬНОМ",	//FAIR
	L"ПЛОХОМ",	//POOR
	L"УЖАСНОМ",	//BAD
	L"НЕРАБОЧЕМ",
	L" состоянии."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Кол-во",
	L"Осталось:", //this is the overall balance
	L"Кол-во",
	L"Отделить:", //the amount he wants to separate from the overall balance to get two piles of money

	L"Текущий",
	L"Баланс:",
	L"Снимаемая",
	L"Сумма:",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"УМИРАЕТ",		//	>= 0
	L"КРИТИЧЕН", 		//	>= 15
	L"ПЛОХ",		//	>= 30
	L"РАНЕН",    	//	>= 45
	L"ЗДОРОВ",    	//	>= 60
	L"СИЛЕН",     	// 	>= 75
	L"ОТЛИЧНО",		// 	>= 90
	L"ЗАХВАЧЕН",
};

STR16 gzHiddenHitCountStr[1] =
{
	L"?",	
};

STR16	gzMoneyAmounts[6] = 
{ 
	L"1000$",
	L"100$",
	L"10$",
	L"Снять",
	L"Разделить",
	L"Взять",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons."
CHAR16		gzProsLabel[10] = 
{
	L"+",
};

CHAR16		gzConsLabel[10] = 
{
	L"-",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Повторить", 	//meaning "Repeat yourself"
	L"Дружественно",		//approach in a friendly
	L"Напрямую",		//approach directly - let's get down to business
	L"Угрожать",		//approach threateningly - talk now, or I'll blow your face off
	L"Дать",		
	L"Нанять",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Купить/Продать",
	L"Купить",
	L"Продать",
	L"Ремонтировать",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"До встречи",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"Эльдорадо",
 L"Хаммер", // a hummer jeep/truck -- military vehicle
 L"Фургон",
 L"Джип",
 L"Танк",
 L"Вертолёт",
};

STR16 pShortVehicleStrings[] =
{
	L"Эльдор",
	L"Хаммер",			// the HMVV
	L"Фургон",
	L"Джип",
	L"Танк",
	L"Верт.", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Эльдорадо",
	L"Хаммер",		//a military jeep. This is a brand name.
	L"Фургон",			// Ice cream truck
	L"Джип",
	L"Танк",
	L"Вертолёт", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Воздушный Рейд",
	L"Оказать первую помощь?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s замечает, что некоторые вещи отсутствуют в посылке.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Замок (%s).",
	L"Тут нет замка.",
	L"Успех!",
	L"Провал.",
	L"Успех!",
	L"Провал",
	L"На замке нет ловушки.",
	L"Успех!",
	// The %s is a merc name
	L"У %s нет подходящего ключа",
	L"Ловушка обезврежена",
	L"На замке не найдено ловушки.",
	L"Заперто",
	L"ДВЕРЬ",
	L"С ЛОВУШКОЙ",
	L"ЗАПЕРТАЯ",
	L"НЕЗАПЕРТАЯ",
	L"СЛОМАНАЯ",
	L"Тут есть кнопка. Нажать?",
	L"Разрядить ловушку?",
	L"Пред...",
	L"След...",
	L"Еще...",

	// In the next 2 strings, %s is an item name
	
	L"%s помещен(а) на землю.",
	L"%s отдан(а) %s.",

	// In the next 2 strings, %s is a name

	L"%s: Оплачено сполна.",
	L"%s: Еще должен %d.",
	L"Установить частоту радиодетонатора:",  	//in this case, frequency refers to a radio signal
	L"Количество ходов до взрыва:",	//how much time, in turns, until the bomb blows
	L"Выберите частоту радиодетонатора на пульте:", 	//in this case, frequency refers to a radio signal
	L"Обезвредить ловушку?",
	L"Убрать синий флаг?",
	L"Поставить здесь синий флаг?",
	L"Завершающий ход",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Вы действительно хотите атаковать %s?",
	L"Увы, в машине боец не может изменить положение.",
	L"Робот не может менять положение.",

	// In the next 3 strings, %s is a name

	L"%s не может поменять положение здесь.",
	L"%s не может получить первую помощь.",
	L"%s не нуждается в медицинской помощи.",
	L"Туда идти нельзя.",
	L"У вас уже полная команда, мест нет.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s нанят(а).",

	// Here %s is a name and %d is a number

	L"%s должен получить $%d.",

	// In the next string, %s is a name

	L"Сопроводить %s?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"Нанять %s за %s в день?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Хотите участвовать в поединке?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"Купить %s за %s?",

	// In the next string, %s is a name

	L"%s сопровождается отрядом %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"ОТКАЗ",					//weapon is jammed.
	L"Роботу нужны патроны %s калибра.",		//Robot is out of ammo
	L"Бросить туда не получится.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Режим скрытности (|Z)",
	L"Карта (|M)",
	L"Завершить ход (|D)",
	L"Говорить",
	L"Молчать",
	L"Подняться (|P|g|U|p)",
	L"Смена уровня (|T|a|b)",
	L"Забраться/Спрыгнуть (|J)",
	L"Присесть/Лечь (|P|g|D|n)",
	L"Осмотреть (|C|t|r|l)",
	L"Предыдущий боец",
	L"Следующий боец (|П|p|o|б|e|л)",
	L"Настройки (|O)",
	L"Режим очереди (|B)",
	L"Смотреть/Повернуться (|L)",
	L"Здоровье: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s",
	L"Ну и?",					//this means "what?"
	L"Продолж.",					// an abbrieviation for "Continued"
	L"%s будет говорить.",
	L"%s будет молчать.",
	L"Состояние: %d/%d\nТопливо: %d/%d",
	L"Выйти из машины",
	L"Сменить отряд (|S|h|i|f|t |П|p|о|б|e|л)",
	L"Ехать",
	L"Н/Д",						//this is an acronym for "Not Applicable."
	L"Рукопашный бой",
	L"Применить оружие",
	L"Воспользоваться ножом",
	L"Использовать взрывчатку",
	L"Воспользоваться аптечкой",
	L"(Ловит)",
	L"(Перезарядка)",
	L"(Дать)",
	L"Сработала %s.",	// The %s here is a string from pDoorTrapStrings  ASSUME all traps are female gender
	L"%s прибыл(а).",
	L"%s: истратил(а) все очки действия.",
	L"%s сейчас не может действовать.",
	L"%s перевязан(а).",
	L"%s: закончились бинты.",
	L"Враг в секторе!",
	L"Врагов в поле зрения нет.",
	L"Недостаточно очков действия.",
	L"Оденьте на голову одного из наемников пульт управления роботом.",
	L"Последняя очередь опустошила магазин!",
	L"СОЛДАТ",
	L"РЕПТИОН",
	L"ОПОЛЧЕНЕЦ",
	L"ЖИТЕЛЬ",
	L"ЗОМБИ",
	L"PRISONER",//TODO.Translate
	L"Выход из сектора",
	L"ДА",
	L"ОТМЕНА",
	L"Выбранный боец",
	L"Все бойцы отряда",
	L"Идти в сектор",
	L"Идти на карту",
	L"Этот сектор отсюда покинуть нельзя.",
	L"You can't leave in turn based mode.",		// TODO.Translate
	L"%s слишком далеко.",
	L"Скрыть кроны деревьев",
	L"Показать кроны деревьев",
	L"ВОРОНА",				//Crow, as in the large black bird
	L"ШЕЯ",
	L"ГОЛОВА",
	L"ТОРС",
	L"НОГИ",
	L"Рассказать королеве то, что она хочет знать?",
	L"Регистрация отпечатков пальцев пройдена.",
	L"Неопознанные отпечатки пальцев. Оружие заблокировано.",
	L"Цель захвачена",
	L"Путь заблокирован",
	L"Положить/Снять деньги",		// Help text over the $ button on the Single Merc Panel 
	L"Никто не нуждается в медицинской помощи.",
	L"отказ",								// Short form of JAMMED, for small inv slots
	L"Туда вскарабкаться невозможно.",					// used ( now ) for when we click on a cliff
	L"Путь блокирован. Хотите поменяться местами с этим человеком?",
	L"Человек отказывается двигаться.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Вы согласны заплатить %s?",
	L"Принять бесплатное лечение?",
	L"Согласиться выйти замуж за %s?", //Дэррела
	L"Связка ключей",
	L"С эскортируемыми этого сделать нельзя.",
	L"Пощадить %s?", //сержанта
	L"За пределами прицельной дальности.",
	L"Шахтер",
	L"Машина может ездить только между секторами.",
	L"Ни у кого из наемников нет аптечки",
	L"Путь для %s заблокирован",
	L"Ваши бойцы, захваченные армией %s, томятся здесь в плену!", //Дейдраны
	L"Замок поврежден.",
	L"Замок разрушен.",
	L"Кто-то с другой стороны пытается открыть эту дверь.",
	L"Состояние: %d/%d\nТопливо: %d/%d",
	L"%s не видит %s.",  // Cannot see person trying to talk to
	L"Навеска снята",
	L"Вы не можете содержать еще одну машину, довольствуйтесь уже имеющимися двумя.",

	// added by Flugente for defusing/setting up trap networks	// TODO.Translate
	L"Choose detonation frequency (1 - 4) or defuse frequency (A - D):",
	L"Set defusing frequency:",
	L"Set detonation frequency (1 - 4) and defusing frequency (A - D):",
	L"Set detonation time in turns (1 - 4) and defusing frequency (A - D):",
	L"Select tripwire hierarchy (1 - 4) and network (A - D):",

	// added by Flugente to display health and poisoning		// TODO.Translate
	L"Здоровье: %d/%d\nЯд: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s",

	// added by Flugente to display food status
	L"Здоровье: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s\nВода: %d%s\nПища: %d%s",
	L"Здоровье: %d/%d\nЯд: %d/%d\nЭнергия: %d/%d\nБоевой дух: %s\nВода: %d%s\nПища: %d%s",

	// added by Flugente: selection of a function to call in tactical	// TODO.Translate
	L"Что будем делать?",
	L"Наполнить фляги",
	L"Clean guns (Merc)",//TODO.Translate
	L"Clean guns (Team)",
	L"Снять одежду",
	L"Lose disguise",
	L"Militia inspection",
	L"Militia restock",
	L"Test disguise",
	L"unused",

	// added by Flugente: decide what to do with the corpses
	L"Что будем делать с телом?",
	L"Отсечь голову",
	L"Потрошить",
	L"Снять одежду",
	L"Забрать тело",

	// Flugente: weapon cleaning
	L"%s чистит %s",

	// added by Flugente: decide what to do with prisoners
	L"У вас нет тюрьмы для содержания заключённых, придётся отпустить их",
	L"Where do you want to send the prisoners?",//TODO.Translate
	L"Let them go",
	L"Что вы хотите сделать?",
	L"Требовать сдаться",
	L"Предлоить сдаться",
	L"Переговоры",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"Если выбрано, то карта соседнего сектора будет сразу же загружена.",
	L"Если выбрано, то вы автоматически попадете на экран карты,\nтак как путешествие займет некоторое время.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Этот сектор оккупирован врагом, и вы не можете выйти отсюда.\nВы должны разобраться с этим, прежде чем перейти в любой другой сектор.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Как только оставшиеся наемники покинут этот сектор,\nсразу будет загружен соседний сектор.",
	L"Выведя оставшихся наемников из этого сектора,\nвы автоматически попадете на экран карты,\nтак как на путешествие потребуется некоторое время.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s нуждается в сопровождении ваших наёмников и не может самостоятельно покинуть сектор.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s не может покинуть сектор один, так как он сопровождает %s.", //male singular
	L"%s не может покинуть сектор одна, так как она сопровождает %s.", //female singular
	L"%s не может покинуть сектор один, так как он сопровождает группу из нескольких человек.", //male plural
	L"%s не может покинуть сектор одна, так как она сопровождает группу из нескольких человек.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Все ваши наёмники должны быть в машине,\nчтобы отряд смог отправиться в место назначения.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Если выбрать, то %s отправится в одиночку\nи автоматически будет переведен в отдельный отряд.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Если выбрать, данный отряд отправится\nв место назначения, покинув этот сектор.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s сопровождается вашими наемниками и не может покинуть этот сектор в одиночку. Остальные наемники должны быть рядом, прежде чем вы сможете покинуть сектор.",
};



STR16 pRepairStrings[] = 
{
	L"Предметы", 		// tell merc to repair items in inventory
	L"База ПВО", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Отмена", 		// cancel this menu
	L"Робот", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill."

STR16 sPreStatBuildString[] =
{
	L"теряет", 			// the merc has lost a statistic
	L"получает", 		// the merc has gained a statistic
	L"единицу",	// singular
	L"единиц",	// plural
	L"уровень",	// singular
	L"уровня",	// plural
};

STR16 sStatGainStrings[] =
{
	L"здоровья.",
	L"проворности.",
	L"ловкости.",
	L"интеллекта.",
	L"медицины.",
	L"взрывного дела.",
	L"механики.",
	L"меткости.",
	L"опыта.",
	L"силы.",
	L"лидерства.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Общая дистанция:", 			// total distance for helicopter to travel
	L"Безопасно:  ", 			// distance to travel to destination
	L"Опасно:", 			// distance to return from destination to airport
	L"Итого:", 		// total cost of trip by helicopter
	L"ОВП:", 			// ETA is an acronym for "estimated time of arrival" 
	L"У вертолета закончилось топливо. Придется совершить посадку на вражеской территории!",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
  L"Пассажиры:",
  L"Выбрать вертолет или точку высадки?",
  L"Вертолёт",
  L"Высадка",
};

STR16 sMapLevelString[] =
{
	L"Подуровень:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Лояльность",	// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"не может выйти на марш в подземельях.", 
};

STR16 gsTimeStrings[] =
{
	L"ч",				// hours abbreviation
	L"м",				// minutes abbreviation
	L"с",				// seconds abbreviation
	L"д",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"Нет",	//важные объекты сектора
	L"Госпиталь",
	L"Завод",	//Factory
	L"Тюрьма",
	L"Военная база",
	L"Аэропорт",
	L"Стрельбище",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Инвентарь",
	L"Выйти",
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Размер",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Контроль", 					// how much of town is controlled
	L"Нет", 					// none of this town
	L"Шахта города", 				// mine associated with this town
	L"Лояльность",					// 5 // the loyalty level of this town
	L"Готовы", 					// the forces in the town trained by the player
	L"",
	L"Важные объекты", 				// main facilities in this town
	L"Уровень", 					// the training level of civilians in this town
	L"Тренировка ополчения",				// 10 // state of civilian training in town
	L"Ополчение", 					// the state of the trained civilians in the town
	L"Тренинг мобильных групп",			// HEADROCK HAM 3.6: The stat of Mobile militia training in town	//Mobile Training
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"Шахта",						// 0
	L"Серебро",
	L"Золото",
	L"Дневная выработка",
	L"Производственные возможности",
	L"Заброшена",				// 5
	L"Закрыта",
	L"Истощается",
	L"Идет добыча",
	L"Статус",
	L"Уровень добычи",
	L"Resource",				// 10	L"Тип руды",	// TODO.Translate
	L"Принадлежность",
	L"Лояльность",
//	L"Работ.шахтеры",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Силы врага",
	L"Сектор",
	L"Количество предметов",
	L"Неизвестно",
	L"Под контролем",
	L"Да",
	L"Нет",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s слишком далеко.",	//Merc is in sector with item but not close enough
	L"Нельзя выбрать этого бойца.",  //MARK CARTER
	L"%s вне этого сектора, и не может подобрать предмет.",
	L"Во время боя вам придется подбирать вещи с земли.",
	L"Во время боя вам придется выкладывать вещи на землю на тактической карте.",
	L"%s вне этого сектора, и не может оставить предмет.",
	L"Во время битвы вы не можете заряжать оружие патронами из короба.",
};

STR16 pMapInventoryStrings[] =
{
	L"Локация", 			// sector these items are in
	L"Всего предметов", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Чтобы перевести наемника в другой отряд, назначить его врачом или отдать приказ ремонтировать вещи, щелкните по колонке 'ЗАНЯТИЕ'.",
	L"Чтобы приказать наемнику перейти в другой сектор, щелкните в колонке 'КУДА'.",
	L"Как только наемник получит приказ на передвижение, включится сжатие времени.",
	L"Нажатием левой кнопки мыши выбирается сектор. Еще одно нажатие нужно, чтобы отдать наемникам приказы на передвижение. Нажатие правой кнопки мыши на секторе откроет экран дополнительной информации.",
	L"Чтобы вызвать экран помощи - в любой момент времени нажмите 'h'.",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Тестовый текст",
	L"Вы практически ничего не сможете сделать на этом экране, пока не прибудете в Арулько. Когда познакомитесь со своей командой, включите сжатие времени (кнопки в правом нижнем углу). Это ускорит течение времени, пока ваша команда не прибудет в Арулько.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Отправить наемников в сектор", 	// title for movement box 
	L"Путь", 		// done with movement menu, start plotting movement
	L"Отмена", 		// cancel this menu
	L"Другое",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"Ой!:", 			// an error has occured
	L"Срок контракта истек:", 	// this pop up came up due to a merc contract ending
	L"Задание выполнили:", // this pop up....due to more than one merc finishing assignments
	L"Бойцы вернулись к своим обязанностям:", // this pop up ....due to more than one merc waking up and returing to work
	L"Бойцы ложатся спать:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Скоро закончатся контракты у:", // this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Населенные пункты (|W)",
	L"Шахты (|M)",
	L"Отряды и враги (|T)",
	L"Карта воздушного пространства (|A)",
	L"Вещи (|I)",
	L"Ополчение и враги (|Z)",
	L"Мобильные группы ополченцев (|R)", //HAM 4: Show Mobile Militia Restrictions
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Следующая \nстраница (|.)",		// next page
	L"Предыдущая \nстраница (|,)",		// previous page
	L"Закрыть инвентарь сектора (|E|s|c)",	// exit sector inventory
	L"Увеличить предметы", // HEAROCK HAM 5: Inventory Zoom Button
	L"Сложить и объединить предметы", // HEADROCK HAM 5: Stack and Merge
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Собрать патроны в ящики\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Собрать патроны в коробки", // HEADROCK HAM 5: Sort ammo
	// 6 - 10
	L"Снять всю навеску \nс предметов", // HEADROCK HAM 5: Separate Attachments
	L"Разрядить всё оружие", //HEADROCK HAM 5: Eject Ammo
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Показать все предметы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Скрыть все предметы", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть оружие\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать оружие", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть аммуницию\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать аммуницию", // HEADROCK HAM 5: Filter Button
	// 11 - 15
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть взрывчатку\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать взрывчатку", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть холодное оружие\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать холодное оружие", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть броню\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать броню", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть разгрузочные системы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать разгрузочные системы", // HEADROCK HAM 5: Filter Button
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть наборы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать наборы", // HEADROCK HAM 5: Filter Button
	// 16 - 20
	L"|Л|е|в|ы|й| |щ|е|л|ч|о|к|: Скрыть прочие предметы\n|П|р|а|в|ы|й| |щ|е|л|ч|о|к|: Показать прочие предметы", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Move Item Display", // Flugente: move item display	// TODO.Translate
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"Лэптоп (|L)",
	L"Тактический экран (|E|s|c)",
	L"Настройки (|O)", 
	L"Сжатие времени (|+)", 	// time compress more 
	L"Сжатие времени (|-)", 	// time compress less
	L"Предыдущее сообщение (|С|т|р|е|л|к|а |в|в|е|р|х)\nПредыдущая страница (|P|g|U|p)", 	// previous message in scrollable list
	L"Следующее сообщение (|С|т|р|е|л|к|а |в|н|и|з)\nСледующая страница (|P|g|D|n)", 	// next message in the scrollable list
	L"Включить / выключить\nсжатие времени (|П|р|о|б|е|л)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Текущий баланс", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s мертв(а)",
};


STR16 pDayStrings[] =
{
	L"День",
};

// the list of email sender names

CHAR16 pSenderNameList[500][128] =
{
	L"",
};
/*
{
	L"Энрико",
	L"Psych Pro Inc.",
	L"Помощь",
	L"Psych Pro Inc.",
	L"Спек",
	L"R.I.S.",		//5
	L"Барри",
	L"Блад",
	L"Рысь",
	L"Гризли",
	L"Вики",			//10
	L"Тревор",
	L"Грунти (Хряп)",
	L"Иван",
	L"Анаболик",
	L"Игорь",			//15
	L"Тень",
	L"Рыжий",
	L"Жнец (Потрошитель)",
	L"Фидель",
	L"Лиска",				//20
	L"Сидней",
	L"Гас",
	L"Сдоба",
	L"Айс",
	L"Паук",		//25
	L"Скала (Клифф)",
	L"Бык",
	L"Стрелок",
	L"Тоска",
	L"Рейдер",		//30
	L"Сова",
	L"Статик",
	L"Лен",
	L"Дэнни",
	L"Маг",
	L"Стефан",
	L"Лысый",
	L"Злобный",
	L"Доктор Кью",
	L"Гвоздь",
	L"Тор",
	L"Стрелка",
	L"Волк",
	L"ЭмДи",
	L"Лава",
	//----------
	L"M.I.S. Страховка",	
	L"Бобби Рэй",
	L"Босс",
	L"Джон Кульба",
	L"А.I.М.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"<<",
  L">>",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Получена новая почта...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Удалить письмо?",
 L"Удалить, НЕ ПРОЧИТАВ?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"От:",
	L"Тема:",
	L"День:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"Почтовый ящик",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Финансовый отчет",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Доход:", 				// credit (subtract from) to player's account
	L"Расход:", 				// debit (add to) to player's account
	L"Вчерашний чистый доход:",
	L"Вчерашние другие поступления:",
	L"Вчерашний расход:",
	L"Баланс на конец дня:",
	L"Чистый доход сегодня:",
	L"Другие поступления за сегодня:",
	L"Расход за сегодня:",
	L"Текущий баланс:",
	L"Ожидаемый доход:",
	L"Ожидаемый баланс:", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
	L"День", 			// the day column
	L"Доход", 			// the credits column
	L"Расход",			// the debits column
	L"Операции", 		// transaction type - see TransactionText below
	L"Баланс", 			// balance at this point in time
	L"Стр.", 			// page number
	L"Дней", 			// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Начисленный процент",			// interest the player has accumulated so far
	L"Анонимный взнос",
	L"Перевод средств", 
	L"Нанят", 				// Merc was hired
	L"Покупки у Бобби Рэя", 		// Bobby Ray is the name of an arms dealer
	L"Оплата счета M.E.R.C.",
	L"%s: страховка.", 		// medical deposit for merc
	L"I.M.P.: Анализ профиля", 		// IMP is the acronym for International Mercenary Profiling
	L"%s: куплена страховка.", 
	L"%s: Страховка уменьшена",
	L"%s: Продление страховки", 				// johnny contract extended
	L"для %s: Страховка аннулирована", 
	L"%s: Требуется страховка", 		// insurance claim for merc
	L"1 день", 				// merc's contract extended for a day
	L"7 дней", 				// merc's contract extended for a week
	L"14 дней", 				// ... for 2 weeks
	L"Доход шахты", 
	L"", //String nuked
	L"Куплены цветы",
	L"%s: Возврат мед. депозита",
	L"%s: Остаток мед. депозита",
	L"%s: Мед. депозит удержан",
	L"%s: оплата услуг.",		// %s is the name of the npc being paid
	L"%s берет наличные.", 			// transfer funds to a merc
	L"%s: переводит деньги.", 		// transfer funds from a merc
	L"%s: оружие ополчению.", // initial cost to equip a town's militia
	L"%s продал вам вещи.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s кладет наличные на счет.",
	L"Снаряжение продано населению",
	L"Оснащение персонала", // HEADROCK HAM 3.6		//Facility Use
	L"Содержание ополчения", // HEADROCK HAM 3.6		//Militia upkeep
	L"Выкуп за освобожденных заключенных",	// Ransom for released prisoners
};

STR16 pTransactionAlternateText[] =
{
	L"Страховка для", 				// insurance for a merc
	L"%s: контракт продлен на 1 день.", 				// entend mercs contract by a day
	L"%s: контракт продлен на 7 дней.",
	L"%s: контракт продлен на 14 дней.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Небесному Всаднику заплачено $%d", 			// skyrider was paid an amount of money
	L"Вы все еще должны Небесному Всаднику $%d.", 		// skyrider is still owed an amount of money
	L"Небесный Всадник завершил заправку.",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Небесный Всадник готов к полету.", // Skyrider was grounded but has been freed
	L"У Небесного Всадника нет пассажиров. Если вы хотите переправить бойцов в этот сектор, посадите их в вертолет (приказ \"Транспорт/Вертолёт\")."
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"Отлично", 
	L"Хорошо",
	L"Норма",
	L"Низкая",
	L"Паника",
	L"Ужас",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s оставляет свою экипировку в Омерте (A9).", 	//%s может взять заказанную экипировку в Омерте (A9).
	L"%s оставляет свою экипировку в Драссене (B13).",	//%s может взять заказанную экипировку в Драссене (B13).
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Здоровье",
	L"Энергия",
	L"Боевой дух",
	L"Состояние",	// the condition of the current vehicle (its "health")
	L"Бензин",	// the fuel level of the current vehicle (its "energy")
	L"Яд",	// Poison
	L"Вода",		// drink level 	//Water
	L"Пища",		// food level		//Food
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Предыдущий боец\n(|С|т|р|е|л|к|а |В|л|е|в|о)", 			// previous merc in the list
	L"Следующий боец\n(|С|т|р|е|л|к|а |В|п|р|а|в|о)", 			// next merc in the list
};


STR16 pEtaString[] =
{
	L"РВП:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Вы больше никогда не увидите этот предмет. Уверены?", 	// do you want to continue and lose the item forever
	L"Этот предмет кажется ОЧЕНЬ важным. Вы ДЕЙСТВИТЕЛЬНО хотите выбросить его?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"Отряд не может выйти на марш, когда один из наемников спит.",

//1-5
	L"Сначала выведите отряд на поверхность.",
	L"Выйти на марш? Да тут же враги повсюду!",
	L"Чтобы выйти на марш, наемники должны быть назначены в отряд или посажены в машину.",
	L"У вас еще нет ни одного бойца.",	// you have no members, can't do anything 
	L"Наемник не может выполнить.",		// merc can't comply with your order
//6-10
	L"нуждается в сопровождении чтобы идти. Назначьте его с кем-нибудь в отряд.", // merc can't move unescorted .. for a male
	L"нуждается в сопровождении чтобы идти. Назначьте ее с кем-нибудь в отряд.", // for a female
#ifdef JA2UB
	L"Наёмник ещё не прибыл в Тракону!",
#else
	L"Наёмник ещё не прибыл в Арулько!",
#endif
	L"Кажется, сначала надо уладить проблемы с контрактом.",
	L"Бежать от самолета? Только после вас!",	// Cannot give a movement order. Air raid is going on.
//11-15
	L"Выступить на марш? Да у нас тут бой идет!",
	L"Вы попали в засаду кошек-убийц в секторе %s!",
	L"Вы только что вошли в сектор %s... И наткнулись на логово кошек-убийц!", 
	L"", 
	L"База ПВО в %s была захвачена.",
//16-20
	L"Шахта в %s была захвачена врагом. Ваш дневной доход сократился до %s в день.",
	L"Враг занял без сопротивления сектор %s.",
	L"Как минимум один из ваших бойцов не может выполнить этот приказ.",
	L"%s не может присоединиться к %s - нет места.",
	L"%s не может присоединиться к %s - слишком большое расстояние.",
//21-25
	L"Шахта в %s была захвачена войсками Дейдраны!",
	L"Войска Дейдраны только что вторглись на базу ПВО в %s.",
	L"Войска Дейдраны только что вторглись в %s.",
	L"Войска Дейдраны были замечены в %s.",
	L"Войска Дейдраны только что захватили %s.",
//26-30
	L"Как минимум один из ваших бойцов не хочет спать.",
	L"Как минимум один из ваших бойцов не может проснуться.",
	L"Ополченцы не появятся, пока не завершат тренировку.",
	L"%s сейчас не в состоянии принять приказ о перемещении.",
	L"Ополченцы вне границ города не могут перейти в другой сектор.",
//31-35
	L"Вы не можете держать ополченцев в %s.",
	L"Пустая машина не может двигаться!",
	L"%s из-за тяжелых ранений не может идти!",
	L"Сначала вам нужно покинуть музей!",
	L"%s мертв(а)!",
//36-40
	L"%s не может переключиться на %s, так как находится в движении.",
	L"%s не может сесть в машину с этой стороны.",
	L"%s не может вступить в %s",
	L"Вы не можете включить сжатие времени, пока не наймете новых бойцов!",
	L"Эта машина может двигаться только по дорогам!",
//41-45
	L"Вы не можете переназначить наемников на марше.",
	L"У машины закончился бензин!",
	L"%s еле волочит ноги и идти не может.",
	L"Ни один из пассажиров не в состоянии вести машину.",
	L"Один или несколько наемников из этого отряда не могут сейчас двигаться.",
//46-50
	L"Один или несколько наемников не могут сейчас двигаться.",
	L"Машина сильно повреждена!",
	L"Внимание! Тренировать ополченцев в одном секторе могут не более двух наемников.",
	L"Роботом обязательно нужно управлять. Назначьте наемника с пультом и робота в один отряд.",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Ещё раз щелкните по точке назначения, чтобы подтвердить путь или щелкните по другому сектору, чтобы установить больше путевых точек.",
	L"Путь движения подтверждён.",
	L"Точка назначения не изменена.",
	L"Путь движения отменён.",
	L"Путь сокращен.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Выберите сектор, в который прибудут наемники.",
	L"Вновь прибывшие наемники высадятся в %s.",
	L"Высадить здесь наемников нельзя. Воздушное пространство не безопасно!",
	L"Отменено. Сектор прибытия не изменился.",
	L"Небо над %s более не безопасно! Место высадки было перемещено в %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Показать снаряжение (|В|в|о|д)",
	L"Выкинуть предмет",
	L"Скрыть снаряжение (|В|в|о|д)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Должен ли %s оставить свою амуницию здесь (в %s) или позже (в %s) перед отлетом?",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Должна ли %s оставить свою амуницию здесь (в %s) или позже (в %s) перед отлетом?",
	L"%s скоро уходит и оставит свою амуницию в %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"отправляется домой, так как время его контракта истекло.", 		// merc's contract is over and has departed
	L"отправляется домой, так как время её контракта истекло.", 		// merc's contract is over and has departed
	L"уходит, так как его контракт был прерван.", 		// merc's contract has been terminated
	L"уходит, так как ее контракт был прерван.",		// merc's contract has been terminated
	L"Вы должны M.E.R.C. слишком много денег, так что %s уходит.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"Неверный код доступа.",
	L"Это приведет к потере уже полученных результатов тестирования. Вы уверены?",
	L"Введите правильное имя и укажите пол.",
	L"Предварительный анализ вашего счета показывает, что вы не можете позволить себе пройти тестирование.",
	L"Сейчас вы не можете выбрать этот пункт.",
	L"Чтобы закончить анализ, нужно иметь место еще хотя бы для одного члена команды.",
	L"Профиль уже создан.",
	L"Не могу загрузить I.M.P.-персонаж с диска.",
	L"Вы достигли максимального количества I.M.P.-персонажей.",
	L"У вас в команде уже есть три I.M.P.-персонажа того же пола.",
	L"Вы не можете позволить себе такой I.M.P.-персонаж.",
	L"Новый I.M.P.-персонаж присоединился к команде.",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"Информация о нас", 		// about the IMP site
	L"НАЧАТЬ", 			// begin profiling
	L"Способности", 		//Skills
	L"Характеристики", 		// personal stats/attributes section
	L"Внешность", 			// Appearance
	L"Голос: %d", 			// the voice selection
	L"Готово", 			// done profiling
	L"Начать сначала", 		// start over profiling
	L"Да, я выбираю отмеченный ответ.",
	L"Да", 
	L"Нет",
	L"Готово", 			// finished answering questions
	L"Назад", 			// previous question..abbreviated form
	L"Дальше", 			// next question
	L"ДА.", 			// yes, I am certain 
	L"НЕТ, Я ХОЧУ НАЧАТЬ СНОВА.",	// no, I want to start over the profiling process
	L"ДА",
	L"НЕТ",
	L"Назад", 			// back one page
	L"Отменить", 			// cancel selection
	L"Да, всё верно.",
	L"Нет, ещё раз взгляну.",
	L"Регистрация", 		// the IMP site registry..when name and gender is selected
	L"Анализ данных", 		// analyzing your profile results
	L"Готово",
	L"Личные качества", 			// Character
	L"Нет",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed
	L"Теперь, когда формирование внешности и личных качеств завершён, укажите ваши способности.",	//With your character traits chosen, it is time to select your skills.
	L"Для завершения, выберите свои характеристики.",	//To complete the process, select your attributes.
	L"Для начала, подберите наиболее подходящее вам лицо, голос, телосложение и соответствующую расцветку.",	//To commence actual profiling, select portrait, voice and colors.
	L"Теперь, когда вы завершили формирование своей внешности, перейдём к анализу ваших личных качеств.",	//Now that you have completed your appearence choice, procced to character analysis.
};

STR16 pFilesTitle[] =
{
  L"Просмотр данных",
};

STR16 pFilesSenderList[] =
{
  L"Отчет разведки", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
	L"В розыске №1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"В розыске №2",	   // second intercept file
	L"В розыске №3",			 // third intercept file
	L"В розыске №4", // fourth intercept file
	L"В розыске №5", // fifth intercept file
	L"В розыске №6", // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Журнал событий", 
};

STR16 pHistoryHeaders[] =
{
	L"День", 			// the day the history event occurred
	L"Стр.", 			// the current page in the history report we are in
	L"День", 			// the days the history report occurs over
	L"Локация", 			// location (in sector) the event occurred
	L"Событие", 			// the event label
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
	L"Нанят(а) %s из A.I.M.", 										// merc was hired from the aim site
	L"Нанят(а) %s из M.E.R.C.", 									// merc was hired from the aim site
	L"%s мертв(а).", 															// merc was killed
	L"Оплачены услуги M.E.R.C.",				// paid outstanding bills at MERC
	L"Принято задание от Энрико Чивалдори",	
	//6-10
	L"Воспользовались услугами I.M.P.",
	L"Оформлена страховка для %s.", 				// insurance contract purchased
	L"%s: cтраховой контракт аннулирован.", 				// insurance contract canceled
	L"Выплата страховки %s.", 							// insurance claim payout for merc
	L"%s: контракт продлен на 1 день.", 						// Extented "mercs name"'s for a day
	//11-15
	L"%s: контракт продлен на 7 дней.", 					// Extented "mercs name"'s for a week
	L"%s: контракт продлен на 14 дней.", 					// Extented "mercs name"'s 2 weeks
	L"Вы уволили %s.", 													// "merc's name" was dismissed.
	L"%s уходит от вас.", 																		// "merc's name" quit.
	L"получено задание.", 															// a particular quest started
	//16-20
	L"задание выполнено.",
	L"Поговорили со старшим горняком города %s",									// talked to head miner of town
	L"%s освобожден(а).",
	L"Включен режим чит-кодов",
	L"Провизия будет доставлена в Омерту завтра.",
	//21-25
	L"%s ушла, чтобы выйти замуж за Дерила Хика.",
	L"Истек контракт у %s.",
	L"Нанят(а) %s.",
	L"Энрико сетует на отсутствие успехов в кампании.",
	L"Победа в сражении!",
	//26-30
	L"В шахте %s иссякает запас руды.",
	L"Шахта %s истощилась.",
	L"Шахта %s закрыта.",
	L"Шахта %s снова работает.",
	L"Получена информация о тюрьме Тикса.",
	//31-35
	L"Узнали об Орте - секретном военном заводе.",
	L"Ученый из Орты подарил вам ракетные винтовки.",
	L"Королева Дейдрана нашла применение трупам.",
	L"Фрэнк говорил что-то о боях в Сан-Моне.",
	L"Пациенту кажется, что он что-то видел в шахтах.",
	//36-40
	L"Встретили Дэвина - торговца взрывчаткой.",
	L"Пересеклись с бывшим наемником A.I.M., Майком!", 
	L"Встретили Тони, торговца оружием.",
	L"Получена ракетная винтовка от сержанта Кротта.",
	L"Документы на магазин Энджела переданы Кайлу.",
	//41-45
	L"Шиз предлагает построить робота.",	//может, собрать робота?
	L"Болтун может сделать варево, обманывающее жуков.",
	L"Кит отошел от дел.",
	L"Говард поставлял цианиды Дейдране.",
	L"Встретили торговца Кита из Камбрии.",
	//46-50
	L"Встретили Говарда, фармацевта из Балайма.",
	L"Встретили Перко. Он держит небольшую мастерскую.",
	L"Встретили Сэма из Балайма. Он торгует железками.",
	L"Франц разбирается в электронике и других вещах.",
	L"Арнольд держит мастерскую в Граме.",
	//51-55
	L"Фредо из Грама чинит электронику.",
	L"Один богатей из Балайма дал вам денег.",
	L"Встретили старьевщика Джейка.",
	L"Один бродяга дал нам электронную карточку.",
	L"Вальтер подкуплен, он откроет дверь в подвал.",
	//56-60
	L"Дэйв заправит машину бесплатно, если будет бензин.",
	L"Дали взятку Пабло.",
	L"Босс держит деньги в шахте Сан-Моны.",
	L"%s: победа в бое без правил.",
	L"%s: проигрыш в бое без правил.",
	//61-65
	L"%s: дисквалификация в бое без правил.",
	L"В заброшенной шахте найдена куча денег.",
	L"Встречен убийца, посланный Боссом.",
	L"Потерян контроль над сектором",				//ENEMY_INVASION_CODE
	L"Отбита атака врага",
	//66-70
	L"Сражение проиграно",							//ENEMY_ENCOUNTER_CODE
	L"Смертельная засада",						//ENEMY_AMBUSH_CODE
	L"Вырвались из засады",
	L"Атака провалилась!",			//ENTERING_ENEMY_SECTOR_CODE
	L"Успешная атака!",
	//71-75
	L"Атака тварей",			//CREATURE_ATTACK_CODE
	L"Кошки-убийцы уничтожили ваш отряд.",			//BLOODCAT_AMBUSH_CODE
	L"Все кошки-убийцы убиты",
	L"%s был убит(а).",
	L"Отдали Кармену голову террориста.",
	L"Убийца ушёл.",
	L"%s убит(а) вашим отрядом.",
};

STR16 pHistoryLocations[] =
{
	L"Н/Д",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"Почта",
	L"Сайты",
	L"Финансы",
	L"Команда",
	L"Журнал",
	L"Данные",
	L"Выключить",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Бобби Рэй",
	L"I.M.P.",
	L"M.E.R.C.",
	L"Похороны",
	L"Цветы",
	L"Страховка",
	L"Отмена",
	L"Энциклопедия",
	L"Briefing Room",
};

STR16 pBookmarkTitle[] =
{
	L"Закладки",
	L"Позже это меню можно вызвать правым щелчком мыши.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Загрузка",
	L"Обновление",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Взять", 			// take money from merc
	L"Дать", 			// give money to merc
	L"Отмена", 			// cancel transaction
	L"Очист.", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Перевести $", 		// transfer money to merc -- short form
	L"Параметры", 			// view stats of the merc
	L"Снаряжение", 			// view the inventory of the merc
	L"Контракт",
};

STR16 sATMText[ ]=
{
	L"Перевести средства?", 		// transfer funds to merc?
	L"Уверены?", 			// are we certain?
	L"Ввести сумму", 		// enter the amount you want to transfer to merc
	L"Выбрать тип", 		// select the type of transfer to merc
	L"Не хватает денег", 	// not enough money to transfer to merc
	L"Сумма должна быть кратна $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Ошибка",
	L"Сервер не имеет записи DNS.",	
	L"Проверьте адрес и попробуйте ещё раз.",
	L"OK", //Превышено время ожидания ответа сервера.
	L"Обрыв соединения с сервером.",
};


STR16 pPersonnelString[] =
{
	L"Бойцов:", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. Состав",
	L"A.I.M. Портреты",		// a mug shot is another name for a portrait
	L"A.I.M. Сортировка",
	L"A.I.M.",
	L"A.I.M. Галерея",
	L"A.I.M. Правила",
	L"A.I.M. История",
	L"A.I.M. Ссылки",
	L"M.E.R.C.",
	L"M.E.R.C. Счета",
	L"M.E.R.C. Регистрация",
	L"M.E.R.C. Оглавление",
	L"Бобби Рэй",
	L"Бобби Рэй - оружие",
	L"Бобби Рэй - боеприпасы",
	L"Бобби Рэй - броня",
	L"Бобби Рэй - разное",					//misc is an abbreviation for miscellaneous
	L"Бобби Рэй - вещи б/у.",
	L"Бобби Рэй - почтовый заказ",
	L"I.M.P.",
	L"I.M.P.",
	L"\"Цветы по всему миру\"",
	L"\"Цветы по всему миру\" - галерея",
	L"\"Цветы по всему миру\" - бланк заказа",
	L"\"Цветы по всему миру\" - открытки",
	L"Страховые агенты: Малеус, Инкус и Стэйпс",
	L"Информация",
	L"Контракт",
	L"Комментарии",
	L"Похоронная служба Макгилликатти",
	L"",
	L"Адрес не найден.",
	L"Бобби Рэй - последние поступления",
	L"Энциклопедия",
	L"Энциклопедия - данные",
	L"Briefing Room",
	L"Briefing Room - Data",
};

STR16 pShowBookmarkString[] =
{
	L"Подсказка",
	L"Щелкните ещё раз по кнопке \"Сайты\" для открытия списка.",
};

STR16 pLaptopTitles[] =
{
	L"Почтовый ящик",
	L"Просмотр данных",
	L"Персонал",
	L"Финансовый отчет",
	L"Журнал",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Погиб в бою",
	L"Уволен",
	L"Другое",
	L"Вышла замуж",
	L"Контракт истёк",
	L"Выход",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"В команде",
	L"Выбывшие",
	L"Гонорар за день:",
	L"Высший гонорар:",
	L"Низший гонорар:",
	L"Погибло в боях:",
	L"Уволено:",
	L"Другое:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Худший",
	L"Среднее",
	L"Лучший",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"ЗДР",
	L"ПРВ",
	L"ЛОВ",
	L"СИЛ",
	L"ЛИД",
	L"ИНТ",
	L"ОПТ",
	L"МЕТ",
	L"МЕХ",
	L"ВЗР",
	L"МЕД",
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
	L"Контракт",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Далее",
	L"Стоп",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Вы потерпели поражение в этом секторе!",
	L"Рептионы, не испытывая угрызений совести, пожрут всех до единого!",
	L"Ваши бойцы захвачены в плен (некоторые без сознания)!",
	L"Ваши бойцы захвачены в плен.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Назад",
	L"Дальше",
	L"Да",
	L"Очистить",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Назад", 
	L"Дальше",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Внести сумму",
	L"В начало", 
	L"Номер счета:",
	L"Наемник",
	L"Дней",
	L"Ставка",
	L"Стоимость",
	L"Всего:",
	L"Вы подтверждаете платеж в размере %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Назад",
	L"Дальше",
};


//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Уровень опыта",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",

	L"Назад",
	L"Нанять",
	L"Дальше",
	L"Дополнительная информация",
	L"В начало",
	L"Нанят",
	L"Оплата",
	L"в день",
	L"Снаряж.:",
	L"Всего:",
	L"Погиб",

	L"У вас уже полная команда из наемников.",
	L"Со снаряжением?",
	L"Недоступно",
	L"Unsettled Bills", //TODO.Translate
	L"Информация",
	L"Снаряжение",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Открыть счет",
	L"Отмена",
	L"Вы ещё не зарегистрировались. Желаете открыть счёт?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Спек Т. Кляйн, основатель и хозяин",
	L"Открыть счёт",
	L"Просмотр счёта",
	L"Просмотр файлов",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Денежный перевод не состоялся. Недостаточно средств на счету.",
};

// For use at MiGillicutty's Web Page.

STR16	sFuneralString[] =
{
	L"Похоронное агентство Макгилликатти: скорбим вместе с семьями усопших с 1983.",
	L"Директор по похоронам и бывший наемник А.I.М. - Мюррэй Макгилликатти \"Папаша\", специалист по части похорон.",
	L"Всю жизнь Папашу сопровождали смерть и утраты, поэтому он, как никто, познал их тяжесть.",
	L"Похоронное агентство Макгилликатти предлагает широкий спектр ритуальных услуг - от жилетки, в которую можно поплакать, до восстановления сильно поврежденных останков.",
	L"Похоронное агентство Макгилликатти поможет вам и вашим родственникам покоиться с миром.",

	// Text for the various links available at the bottom of the page
	L"ДОСТАВКА ЦВЕТОВ",
	L"КОЛЛЕКЦИЯ УРН И ГРОБОВ",
	L"УСЛУГИ ПО КРЕМАЦИИ",
	L"ПОМОЩЬ В ПРОВЕДЕНИИ ПОХОРОН",
	L"ПОХОРОННЫЕ РИТУАЛЫ",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"К сожалению, наш сайт не закончен, в связи с утратой в семье. Мы постараемся продолжить работу после прочтения завещания и выплат долгов умершего. Сайт вскоре откроется.",
	L"Мы искренне сочувствуем вам в это трудное время. Заходите ещё.",
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Галерея",

	//Address of United Florist

	L"\"Мы сбросим ваш букет где угодно!\"",
	L"1-555-SCENT-ME",
	L"333 NoseGay Dr,Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"Мы работаем быстро и эффективно!",
	L"Гарантируем доставку на следующий день практически в любой уголок мира. Есть некоторые ограничения.",
	L"Гарантируем самые низкие цены в мире!",
	L"Покажите нам рекламу более дешевого сервиса и получите 10 бесплатных роз.",
	L"\"Крылатая Флора\", занимаемся фауной и цветами с 1981 г.",
	L"Наши летчики, бывшие пилоты бомбардировщиков, сбросят ваш букет в радиусе 10 миль от заданного района. Когда угодно и сколько угодно!",
	L"Позвольте нам удовлетворить ваши цветочные фантазии.",
	L"Пусть Брюс, известный во всем мире садовник, сам соберет вам отличный букет в нашем саду.",
	L"И запомните, если у нас нет таких цветов, мы быстро вырастим то, что вам надо!",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Назад",
	L"Послать",
	L"Отмена",
	L"Галерея",

	L"Название букета:",
	L"Цена:",
	L"Номер заказа:",
	L"Доставить",
	L"Завтра",
	L"Как будете в тех краях",
	L"Место доставки",
	L"Дополнительно",
	L"Сломать цветы ($10)",
	L"Черные розы ($20)",
	L"Увядший букет ($10)",
	L"Фруктовый пирог (если есть) ($10)",
	L"Текст поздравления:",
	L"Ввиду небольшого размера открытки, постарайтесь уложиться в 75 символов.",
	L"...или выберите одну из",

	L"СТАНДАРТНЫХ ОТКРЫТОК",
	L"Информация о счете",

	//The text that goes beside the area where the user can enter their name

	L"Название:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Назад",	//abbreviation for previous
	L"Дальше",	//abbreviation for next

	L"Выберите букет, которые хотите послать.",
	L"Примечание: Если Вам нужно послать увядший или сломанный букет - заплатите еще $10.",

	//text on the button

	L"В начало",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Выберите текст, который будет напечатан на открытке.",
	L"Назад",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Бланк заказа",		//Title of the page
	L"Штк",					// The number of items ordered
	L"Вес (%s)",			// The weight of the item
	L"Название",			// The name of the item
	L"цена 1 вещи",			// the item's weight
	L"Итого",				// The total price of all of items of the same type
	L"Стоимость",			// The sub total of all the item totals added
	L"ДиУ (см. Место Доставки)",		// S&H is an acronym for Shipping and Handling 
	L"Всего",				// The grand total of all item totals + the shipping and handling
	L"Место доставки",
	L"Скорость доставки",	// See below
	L"Цена (за %s.)",		// The cost to ship the items
	L"Экспресс-доставка",	// Gets deliverd the next day
	L"2 рабочих дня",		// Gets delivered in 2 days
	L"Обычная доставка",	// Gets delivered in 3 days
	L"ОЧИСТИТЬ",//15			// Clears the order page
	L"ЗАКАЗАТЬ",			// Accept the order
	L"Назад",				// text on the button that returns to the previous page
	L"В начало",			// Text on the button that returns to the home page
	L"* - вещи, бывшие в употреблении",		// Disclaimer stating that the item is used
	L"Вы не можете это оплатить.",		//20	// A popup message that to warn of not enough money
	L"<НЕ ВЫБРАНО>",					// Gets displayed when there is no valid city selected
	L"Вы действительно хотите отправить груз в %s?",		// A popup that asks if the city selected is the correct one
	L"Вес груза**",			// Displays the weight of the package
	L"** Мин. вес",			// Disclaimer states that there is a minimum weight for the package
	L"Заказы",
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Шт.винтовки",
	L"Пулеметы",
	L"Ружья",
	L"Тяжелое",

	// Ammo
	L"Пистолеты",
	L"Авт.пистол.",
	L"ПП",
	L"Винтовки",
	L"Сн.винтовки",
	L"Шт.винтовки",
	L"Пулеметы",
	L"Ружья",

	// Used
	L"Оружие",
	L"Броня",
	L"Разгр.с-мы",
	L"Разное",

	// Armour
	L"Каски",
	L"Жилеты",
	L"Брюки",
	L"Пластины",

	// Misc
	L"Режущие",
	L"Метательн.",
	L"Дробящие",
	L"Гранаты",
	L"Бомбы",
	L"Аптечки",
	L"Наборы",
	L"Головные",
	L"Разгр.с-мы",
	L"Прицелы", // Madd: new BR filters
	L"Grip/LAM",
	L"Muzzle",
	L"Stock",
	L"Mag/Trig.",
	L"Др. навеска",
	L"Разное",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Заказать",		// Title
	// instructions on how to order
	L"Нажмите на товар. Левая кнопка - добавить, правая кнопка - уменьшить. После того как выберете товар, оформите заказ.",

	//Text on the buttons to go the various links

	L"Назад",
	L"Оружие",
	L"Патроны",
	L"Броня",
	L"Разное",		//misc is an abbreviation for miscellaneous
	L"Б/У",
	L"Далее",
	L"БЛАНК ЗАКАЗА",
	L"В начало",

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"У вашей команды есть",
	L"оружее, использующее этот тип боеприпасов",

	//The following lines provide information on the items

	L"Вес:",			// Weight of all the items of the same type
	L"Кал.:",			// the caliber of the gun
	L"Маг:",			// number of rounds of ammo the Magazine can hold
	L"Дист:",			// The range of the gun
	L"Урон:",			// Damage of the weapon
	L"Скор:",			// Weapon's Rate Of Fire, acronym ROF
	L"ОД:",				// Weapon's Action Points, acronym AP
	L"Оглушение:",		// Weapon's Stun Damage
	L"Броня:",			// Armour's Protection
	L"Камуф.:",			// Armour's Camouflage
	L"Цена:",			// Cost of the item
	L"Склад:",			// The number of items still in the store's inventory
	L"Штук в заказе:",	// The number of items on order
	L"Урон:",			// If the item is damaged
	L"Вес:",			// the Weight of the item
	L"Итого:",			// The total cost of all items on order
	L"* %% до износа",	// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Чёрт! В эту форму можно внести не более " ,//First part
	L" позиций для одного заказа. Если хотите заказать больше (а мы надеемся, вы хотите), то заполните еще один заказ и примите наши извинения за неудобства.", 

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Извините, но данного товара нет на складе. Попробуйте заглянуть позже.",

	//A popup that tells the user that the store is temporarily sold out

	L"Извините, но данного товара пока нет на складе.",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Здесь вы найдете лучшие и новейшие образцы оружия",
	L"Мы снабдим вас всем, что нужно для победы над противником",
	L"ВЕЩИ Б/У",

	//Text for the various links to the sub pages

	L"РАЗНОЕ",
	L"ОРУЖИЕ",
	L"БОЕПРИПАСЫ",
	L"БРОНЯ",

	//Details on the web site

	L"Если у нас чего-то нет, то этого нет нигде!",
	L"В разработке",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"А.I.M. Состав",		// Title
	// Title for the way to sort
	L"Сортировка:",

	// sort by...

	L"Цена",
	L"Опыт",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Мудрость",
	L"Имя",

	//Text of the links to other AIM pages

	L"Фотографии наёмников",
	L"Информация о наёмниках",
	L"Архив A.I.M.", 

	// text to display how the entries will be sorted

	L"По возрастанию",
	L"По убыванию",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Назад",
	L"В начало", 
	L"Оглавление",
	L"Дальше",
	L"Не согласен",
	L"Согласен",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Левый щелчок",
	L"связаться с бойцом.",
	L"Правый щелчок - ",
	L"фотографии бойцов.",
};

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Интеллект",
	L"Уровень опыта",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",

	// the contract expenses' area

	L"Гонорар",
	L"Срок",
	L"1 день",
	L"7 дней",
	L"14 дней",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"<<",
	L"Связаться",
	L">>",

	L"Дополнительная информация",		// Title for the additional info for the merc's bio
	L"Действующий состав",				// Title of the page
	L"Снаряжение:",						// Displays the optional gear cost
	L"Снаряж.",				//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"Стоимость Мед. депозита",			// If the merc required a medical deposit, this is displayed
	L"Набор 1",				// Text on Starting Gear Selection Button 1
	L"Набор 2",				// Text on Starting Gear Selection Button 2
	L"Набор 3",				// Text on Starting Gear Selection Button 3
	L"Набор 4",				// Text on Starting Gear Selection Button 4
	L"Набор 5",				// Text on Starting Gear Selection Button 5
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"Сумма контракта:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"1 день",
	L"7 дней",
	L"14 дней",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Без снаряжения",
	L"Со снаряжением",

	// Text on the Buttons

	L"ОПЛАТИТЬ",			// to actually hire the merc
	L"ОТМЕНА",				// go back to the previous menu
	L"НАНЯТЬ",				// go to menu in which you can hire the merc
	L"ОТБОЙ",				// stops talking with the merc
	L"ЗАКРЫТЬ",
	L"СООБЩЕНИЕ",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Видеоконференция с",
	L"Подключение. . .",

	L"+ страховка"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"ПРОИЗВЕДЕН ЭЛЕКТРОННЫЙ ПЛАТЕЖ",	// You hired the merc
	L"НЕЛЬЗЯ ПЕРЕВЕСТИ СРЕДСТВА",		// Player doesn't have enough money, message 1
	L"НЕ ХВАТАЕТ СРЕДСТВ",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"На задании",
	L"Пожалуйста, оставьте сообщение",
	L"Скончался",

	//If you try to hire more mercs than game can support

	L"У вас уже полная команда из наемников.",

	L"Автоответчик",
	L"Сообщение оставлено на автоответчике",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M. Ссылки",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M. История",			//Title

	// Text on the buttons at the bottom of the page

	L"Назад",
	L"В начало", 
	L"A.I.M. Галерея",
	L"Дальше",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Цена",
	L"Опыт",
	L"Меткость",
	L"Механика",
	L"Взрывчатка",
	L"Медицина",
	L"Здоровье",
	L"Проворность",
	L"Ловкость",
	L"Сила",
	L"Лидерство",
	L"Мудрость",
	L"Имя",

	// The title of the page, the above text gets added at the end of this text

	L"Состав A.I.M. По возрастанию, критерий - %s",
	L"Состав A.I.M. По убыванию, критерий - %s",

	// Instructions to the players on what to do

	L"Левый щелчок",
	L"Выбрать наёмника",
	L"Правый щелчок",
	L"Критерий сортировки",

	// Gets displayed on top of the merc's portrait if they are...

	L"Выбыл",
	L"Скончался",
	L"На задании",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"СТР. 1",
	L"СТР. 2",
	L"СТР. 3",

	L"A.I.M. Галерея",	// Title of the page

	L"ОК"			// Stops displaying information on selected merc
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"A.I.M. и логотип A.I.M. - зарегистрированные во многих странах торговые марки.",
	L"Так что и не думай подражать нам.",
	L"(с) 1998-1999 A.I.M., Ltd. Все права защищены.",

	//Text for an advertisement that gets displayed on the AIM page

	L"\"Цветы по всему миру\"",
	L"\"Мы сбросим ваш букет где угодно!\"",
	L"Сделай как надо",
	L"...в первый раз",
	L"Если у нас нет такого ствола, то он вам и не нужен.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"В начало",
	L"Наёмники",
	L"Архив",
	L"Правила",
	L"Информация",
	L"Ссылки",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"ИМЕЮЩИЕСЯ ТОВАРЫ",	//Header for the merchandise available
	L"СТР.",				//The current store inventory page being displayed
	L"ОБЩАЯ ЦЕНА",			//The total cost of the the items in the Dealer inventory area
	L"ОБЩАЯ ЦЕННОСТЬ",		//The total value of items player wishes to sell
	L"ОЦЕНКА",				//Button text for dealer to evaluate items the player wants to sell
	L"ПЕРЕВОД",				//Button text which completes the deal. Makes the transaction.
	L"УЙТИ",				//Text for the button which will leave the shopkeeper interface.
	L"ЦЕНА РЕМОНТА",		//The amount the dealer will charge to repair the merc's goods
	L"1 ЧАС",				// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d ЧАСОВ",			// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"ИСПРАВНО",			// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Вам уже некуда класть вещи.",	//Message box that tells the user there is no more room to put there stuff
	L"%d МИНУТ",			// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Выбросить предмет на землю.",
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
	L"OK",					//Transfer the money
	L"Взять",				//Take money from the player
	L"Дать",				//Give money to the player
	L"Отмена",				//Cancel the transfer
	L"Очистить",			//Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Выберите тип",			//tells the user to select either to give or take from the merc
	L"Введите сумму",			//Enter the amount to transfer
	L"Перевести деньги бойцу",	//Giving money to the merc
	L"Забрать деньги у бойца",	//Taking money from the merc
	L"Недостаточно средств",	//Not enough money to transfer
	L"Баланс",					//Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Желаете снять со счета %s, чтобы покрыть разницу?",
	L"Недостаточно средств. Не хватает %s",
	L"Желаете снять со счета %s, чтобы оплатить полную стоимость?",
	L"Попросить торговца сделать перевод",
	L"Попросить торговца починить выбранные предметы",
	L"Закончить беседу",
	L"Текущий баланс",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Сохранить",
	L"Загрузить",
	L"Выход",
	L">>",
	L"<<",
	L"Готово",

	//Text above the slider bars
	L"Звуки",
	L"Речь",
	L"Музыка",

	//Confirmation pop when the user selects..
	L"Выйти из игры и вернуться в главное меню?",

	L"Необходимо выбрать или \"Речь\", или \"Субтитры\"",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Сохранить",
	L"Загрузить",
	L"Отмена",
	L"Сохранение выбрано",
	L"Загрузка выбрана",

	L"Игра успешно сохранена",
	L"ОШИБКА сохранения игры!",
	L"Игра успешно загружена",
	L"ОШИБКА загрузки игры!",

	L"Это сохранение было сделано иной версией игры. Скорее всего, загрузить его не удастся. Все равно продолжить?",

	L"Возможно файлы сохранений повреждены. Желаете их удалить?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия сохранений игры была изменена. Просим сообщить если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Версия игры и файлов сохранений была изменена. Просим сообщить если это изменение привело к какой-либо ошибке. Пытаемся загрузить?",
#else
	L"Попытка загрузить сохранение игры устаревшей версии. Автоматически обновить его и загрузить?",
#endif

	L"Вы решили записать игру на существующее сохранение #%d?",
	L"Хотите загрузить игру из сохранения #",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"У вас заканчивается свободное место на жестком диске. Сейчас свободно %d Мб, а требуется %d Мб свободного места для JA.",

	L"Сохраняю",		//When saving a game, a message box with this string appears on the screen

	L"Нормальный",
	L"Огромный",
	L"нет",
	L"да",

	L"Элементы фантастики",
	L"Платиновая серия",

	L"Ассортимент Бобби Рэя",
	L"Нормальный",
	L"Большой",
	L"Огромный",
	L"Всё и сразу",

	L"Сохраненная игра была начата в режиме \"Нового Инвентаря\", этот режим не работат при разрешении экрана 640х480. Измените разрешение и загрузите игру снова.",
	L"Загрузка игры, начатой в режиме \"Нового Инвентаря\", невозможна. Установите в Ja2.ini игровую папку 'Data-1.13' и повторите попытку.",

	L"The squad size from the savegame is not supported by the current screen resolution. Please increase the screen resolution and try again.", // TODO.Translate
	L"Количество товара у Бобби Рэя",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Уровень карты",
	L"У вас нет ополченцев. Чтобы они появились, вам нужно склонить на свою сторону горожан.",
	L"Доход в сутки",
	L"Наемник застрахован", 
	L"%s не нуждается в отдыхе.", 
	L"%s на марше и не может лечь спать.", 
	L"%s валится с ног от усталости, погоди немного.",
	L"%s ведет машину.",
	L"Отряд не может двигаться, когда один из наемников спит.",

	// stuff for contracts
	L"Хотя у вас и есть деньги на подписание контракта, но их не хватит, чтобы оплатить страховку наемника.",
	L"%s: продление страховки составит %s за %d дополнительных дней. Желаете заплатить?",
	L"Предметы в секторе",
	L"Жизнь наемника застрахована.",
	
	// other items
	L"Медики", // people acting a field medics and bandaging wounded mercs 
	L"Раненые", // people who are being bandaged by a medic 
	L"Готово", // Continue on with the game after autobandage is complete 
	L"Стоп", // Stop autobandaging of patients by medics now 
	L"Извините, этот пункт недоступен в демонстрационной версии.", // informs player this option/button has been disabled in the demo 
	L"%s: нет инструментов.",
	L"%s: нет аптечки.",
	L"Здесь недостаточно добровольцев для тренировки.",
	L"В %s максимальное количество ополченцев.",
	L"У наемника ограниченный контракт.", 
	L"Контракт наемника не застрахован",
	L"Стратегическая Карта",
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"Сейчас у вас нет мобильных групп ополчения. Включите этот режим в следующий раз, когда наберёте их.",
	L"Здесь показано куда ваши мобильные отряды могут пройти и куда нет. СЕРЫЙ = не пойдут туда. КРАСНЫЙ = могут пойти туда, но вы запретили им. ЖЁЛТЫЙ = могут вступить в сектор, но не могут его покинуть. ЗЕЛЁНЫЙ = могут свободно передвигаться. Правым щелчком кнопки мыши можно менять цветовой статус сектора.",
};


STR16 pLandMarkInSectorString[] =
{
	L"Отряд %d заметил кого-то в секторе %s.",
	L"Отряд %s заметил кого-то в секторе %s.",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Тренировка отряда ополченцев будет стоить $", // telling player how much it will cost
	L"Подтвердить платеж?", // asking player if they wish to pay the amount requested
	L"Вы не можете себе этого позволить.", // telling the player they can't afford to train this town
	L"Продолжить тренировку в %s (%s %d)?", // continue training this town?
	L"Цена $", // the cost in dollars to train militia
	L"( Д/Н )",   // abbreviated yes/no
	L"",	// unused
	L"Тренировка ополчения в секторе %d будет стоить $%d. %s", // cost to train sveral sectors at once
	L"У вас нет $%d, чтобы приступить к тренировке ополчения.",
	L"%s: Требуется не менее %d процентов лояльности, чтобы продолжить тренировку ополчения.",
	L"Больше вы не можете тренировать ополчение в %s.",
	L"У вас нет $%d чтобы тренировать здесь мобильное подразделение.",
	L"Продолжить тренировку мобильного подразделения в %s (%s %d)?",
	L"Тренировка мобильного подразделения в секторе %d обойдётся в $ %d. %s",
	L"Тренировка мобильного подразделения ополченцев обойдётся в $",
	L"You cannot train more mobile militia because the maximum is reached (%d/%d). You have to %s first in order to train more.", // TODO.Translate
	L"liberate more town sectors", // TODO.Translate
	L"liberate new town sectors", // TODO.Translate
	L"liberate more towns", // TODO.Translate
	L"regain your lost progress", // TODO.Translate
	L"progress further", // TODO.Translate
	L"recruit more rebels", // TODO.Translate
	L"The militia team leader has to inform you that %d fellow have deserted facing enemy in sector %s.", // TODO.Translate
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"За один раз вы можете снять со счета не более $20 000.",
	L"Вы решили положить %s на свой счет?",
};

STR16	gzCopyrightText[] = 
{
	L"Авторские права (C) 1999 Sir-Tech Canada Ltd. Все права защищены.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Речь",
	L"Молчаливые герои",
	L"Субтитры",
	L"Пауза в диалогах",
	L"Анимированный дым",
	L"Кровь и жестокость", 
	L"Не трогать мышь!",
	L"Старый метод выбора", 
	L"Показать путь движения",
	L"Показать промахи",
	L"Игра в реальном времени",
	L"Подтверждение сна/подъема",
	L"Метрическая система",
	L"Движущаяся подсветка бойца",
	L"Курсор на бойцов",
	L"Курсор на дверь", 
	L"Мерцание вещей",
	L"Показать кроны деревьев",
	L"Показать каркасы",
	L"Трехмерный курсор",
	L"Показать шанс попадания",
	L"Курсор очереди для гранат",
	L"Злорадные враги",	//Allow Enemy Taunts
	L"Стрельба гранатой навесом",
	L"Красться в реальном времени",
	L"Выбор пробелом след. отряда",
	L"Тени предметов в инвентаре",
	L"Дальность оружия в тайлах",
	L"Одиночный трассер",
	L"Шум дождя",
	L"Вороны",
	L"Подсказки над солдатами",	//Show Soldier Tooltips
	L"Автосохранение каждый ход",
	L"Молчаливый пилот вертолёта",
	L"Подробное описание предметов",		//Enhanced Description Box
	L"Только пошаговый режим",				// add forced turn mode
	L"Подсветить навык к повышению",		//Stat Progress Bars	// Show progress towards stat increase
	L"Новая расцветка стратег. карты",	//Alternate Strategy-Map Colors //Change color scheme of Strategic Map
	L"Заметная летящая пуля",				// Show alternate bullet graphics (tracers)
	L"Показать ранг бойца",						// shows mercs ranks
	L"Показать снаряжение на голове",				//Show Face gear graphics
	L"Показать иконки снаряжения",
	L"Отключить меняющийся курсор",		            // Disable Cursor Swap
	L"Тихая тренировка",						// Madd: mercs don't say quotes while training
	L"Тихий ремонт",						// Madd: mercs don't say quotes while repairing
	L"Тихое лечение",						// Madd: mercs don't say quotes while doctoring
	L"Быстрый ход компьютера",			// Automatic fast forward through AI turns
#ifdef ENABLE_ZOMBIES
	L"Зомби в игре!",						// Flugente Zombies 1.0
#endif
	L"Меню в инвентаре бойца",				// the_bob : enable popups for picking items from sector inv
	L"Отметить оставшихся врагов",
	L"Показывать содержимое разгрузок",
	L"Инвертировать колесо мыши",
	L"Formation Movement",					// when multiple mercs are selected, they will try to keep their relative distances	// TODO.Translate
	L"--Читерские настройки--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Ускорить доставку Бобби Рэя",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--Настройки отладочной версии--",		// an example options screen options header (pure text)
	L"Сообщать координаты промахов",		//Report Miss Offsets			// Screen messages showing amount and direction of shot deviation.
	L"Сброс всех игровых настроек",			// failsafe show/hide option to reset all options
	L"В самом деле хотите этого?",			// a do once and reset self option (button like effect)
	L"Отладочные настройки везде",			//Debug Options in other builds		// allow debugging in release or mapeditor
	L"Показать Отладочные настройки",		//DEBUG Render Option group	// an example option that will show/hide other options
	L"Отображать Mouse Regions",		//Render Mouse Regions	// an example of a DEBUG build option
	L"-----------------",					// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"Последняя_Строка_Настроек",	//THE_LAST_OPTION
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	// HEADROCK HAM 4: Added more tooltip text to some toggles, in order to explain them better.

	//speech
	L"Включить или выключить\nголос во время диалогов.",

	//Mute Confirmation
	L"Включить или выключить речевое\nподтверждение выполнения приказов.",

	//Subtitles
	L"Включить или выключить отображение\nсубтитров во время диалогов.",

	//Key to advance speech
	L"Если субтитры включены, выберите этот пункт,\nчтобы успеть прочитать диалоги персонажей.",

	//Toggle smoke animation
	L"Отключите анимацию дыма,\nесли он замедляет игру.",

	//Blood n Gore
	L"Отключите этот пункт, если боитесь крови.",

	//Never move my mouse
	L"Если выключено, то курсор автоматически перемещается\nна кнопку всплывающего окна диалога.", 

	//Old selection method
	L"Если включено, то будет использоваться старый метод выбора наемников\n(для тех, кто привык к управлению предыдущих частей Jagged Alliance).",

	//Show movement path
	L"Если включено, то в режиме реального времени будет отображаться путь передвижения\n(если выключено,  нажмите |S|h|i|f|t, чтобы увидеть путь).",

	//show misses
	L"Если включено, то камера будет отслеживать\nтраекторию пуль, прошедших мимо цели.",
	
	//Real Time Confirmation
	L"Если включено, то для приказа на передвижение будет требоваться\nдополнительный, подтверждающий щелчок мыши на месте назначения.",

	//Display the enemy indicator
	L"Если включено, то вы получите предупреждение,\nкогда наемники лягут спать или проснутся.",

	//Use the metric system
	L"Если включено, то используется метрическая система мер,\nиначе будет британская.",

	//Merc Lighted movement
	L"При ходьбе карта подсвечивается вокруг бойца (|G).\nОтключите эту настройку для повышения производительности системы.",

	//Smart cursor
	L"Если включено, то перемещение курсора возле наемника\nавтоматически выбирает его.",

	//snap cursor to the door
	L"Если включено, то перемещение курсора возле двери\nавтоматически помещает его на дверь.",

	//glow items 
	L"Если включено, то все предметы подсвечиваются. (|I)",

	//toggle tree tops
	L"Если включено, то отображаются кроны деревьев. (|T)",

	//toggle wireframe
	L"Если включено, то у препятствий\nдополнительно показывается каркас. (|W)",

	L"Если включено, то курсор передвижения\nотображается в 3D. (|H|o|m|e)",

	// Options for 1.13
	L"Если включено, шанс попадания\nпоказывается над курсором.",
	L"Если включено, очередь из гранатомета\nиспользует курсор стрельбы очередями.",
	L"Если включено, враг иногда будет комментировать свои действия.",
	L"Если включено, гранатомёты выстреливают \nзаряд под большим углом к горизонту. (|Q)",
	L"Если включено, игра не переходит в пошаговый режим \nпри обнаружении противника (если враг вас не видит). \nРучной вход в пошаговый режим - |C|t|r|l+|X. (|C|t|r|l+|S|h|i|f|t+|X)",
	L"Если включено, |П|р|о|б|е|л выделяет следующий отряд.",
	L"Если включено, показываются тени предметов в инвентаре.",
	L"Если включено, дальность оружия \nпоказывается в игровых квадратах.",
	L"Если включено, трассирующий эффект \nсоздаётся и одиночным выстрелом.",	
	L"Если включено, будет шум дождя во время непогоды.",
	L"Если включено, вороны присутствуют в игре.",
	L"Если включено, при нажатии кнопки |A|l|t \nи наведении курсора мыши на вражеского солдата \nбудет показана дополнительная информация.",
	L"Если включено, игра будет автоматически \nсохраняться после каждого хода игрока.",
	L"Если включено, Небесный Всадник\nне будет вас раздражать болтливостью.",
	L"Если включено, будет задействовано\nподробное описание предметов.",
	L"Если включено и в секторе присутствует враг, \nпошаговый режим будет задействован \nдо полной зачистки сектора (|C|t|r|l+|S|h|i|f|t+|A|l|t+|T).",
	L"Если включено, навык, \nкоторый вскоре повысится будет подсвечен.",
	L"Если включено, необследованные сектрора \nна стратегической карте будут чёрно-белыми.",
	L"Если включено, летящая пуля будет более заметной.",
	L"Если включено, на стратегическом экране будет подписан ранг бойца перед его именем.",
	L"Если включено, на портрете наёмника будет отображено одетое головное снаряжение.",
	L"Если включено, в правом нижнем углу \nна портрете наёмника будут отображены иконки \nодетого головного снаряжения.",
	L"Если включено, курсор не будет меняться показывая все возможные действия. \nЧтобы поменяться местами с человеком рядом, нажмите |X.",
	L"Если включено, бойцы не будут сообщать о повышении своих умений.",
	L"Если включено, бойцы не будут сообщать о статусе ремонта.",
	L"Если включено, бойцы не будут сообщать о лечения.",
	L"Если включено, ожидание пока походят противник, гражданские и животные будет значительно меньше.",

#ifdef ENABLE_ZOMBIES
	L"Если включено, будут появляться зомби. Развлекайся!",
#endif
	L"Если включено, при просмотре предметов сектора в инвентаре бойца \nбудет доступно меню по нажатии левой кнопки на пустой карман.",
	L"Если включено, высвечивается примерное положение последних врагов в секторе.",
	L"Если включено, показывает содержимое разгрузки, иначе - обычный интерфейс новой системы навески.",
	L"Если включено, инвертирует направление прокрутки колеса мыши.",
	L"When multiple mercs are selected, they will try to keep their relative distances while moving. (|C|t|r|l+|S|h|i|f|t+|G)",	// TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Выберите этот пункт чтобы груз Бобби Рэя прибыл немедленно.",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",	// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: When ON, will report the distance each bullet deviates from the\ncenter of the target, taking all NCTH factors into account.",
	L"Если включить, \nповреждённые игровые настройки будут восстановлены.",	// failsafe show/hide option to reset all options
	L"Отметьте строку для подтверждения сброса игровых настроек.",	// a do once and reset self option (button like effect)
	L"Если включено, \nотладочные настройки будут доступны как в игре, \nтак и в редакторе карт.",	// Allows debug options in release or mapeditor builds
	L"Если включено, отладочные настройки \nбудут показаны в общем списке.",	//Toggle to display debugging render options
	L"Attempts to display slash-rects around mouse regions",	// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",	// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"Последняя строка в списке настроек.",
};


STR16	gzGIOScreenText[] =
{
	L"УСТАНОВКИ НАЧАЛА ИГРЫ",
#ifdef JA2UB
	L"Случайная история Мануэля",
	L"да",
	L"нет",
#else
	L"Элементы фантастики",
	L"нет",
	L"есть",
#endif	
	L"Платиновая серия",
	L"Ассортимент оружия в игре",
	L"всё доступное",
	L"чуть поменьше",
	L"Уровень сложности",
	L"лёгкий",	//новичок
	L"средний",	//опытный
	L"трудный",	//эксперт
	L"БЕЗУМНЫЙ",	//помешанный
	L"Начать игру",
	L"Главное меню",
	L"Возможность сохранения",
	L"в любое время",
	L"лишь в мирное",
	L"Отключено в демо-версии", 
	L"Ассортимент Бобби Рэя",
	L"хороший",
	L"большой",
	L"огромный",
	L"всё и сразу",
	L"Инвентарь / Навеска",
	L"NOT USED",
	L"NOT USED",
	L"Загрузить",
	L"УСТАНОВКИ ИГРЫ (актуальны только настроки игры сервера)",
	// Added by SANDRO
	L"Умения персонажа",
	L"старые",
	L"новые",
	L"Создаваемых персонажей",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Враг оставляет всё снаряжение",
	L"нет",
	L"да",
#ifdef JA2UB
	L"Текс и Джон",
	L"Случайно",
	L"Оба сразу",
#else
	L"Число террористов",
	L"случайное",
	L"все сразу",
#endif
	L"Спрятанное оружие секторов",	//Secret Weapon Caches
	L"выборочно",
	L"всё возможное",
	L"Скорость обновления вооружения",	//Progress Speed of Item Choices
	L"очень медленно",
	L"медленно",
	L"умеренно",
	L"быстро",
	L"очень быстро",

	L"старый / старая",
	L"новый / старая",
	L"новый / новая",

	// Squad Size
	L"Бойцов в отряде",
	L"6",
	L"8",
	L"10",
	//L"Faster Bobby Ray Shipments",
	L"Манипуляции с инвентарём за ОД",

	L"Новая система прицеливания",	//New Chance to Hit System
	L"Новая система перехвата хода",
	L"Merc Story Backgrounds", // TODO.Translate
	L"Игра с едой",
	L"Количество товара у Бобби Рэя",
};

STR16	gzMPJScreenText[] =
{
	L"СЕТЕВАЯ ИГРА",	//MULTIPLAYER
	L"Присоединиться",	//Join
	L"Создать игру",	//Host
	L"Отмена",	//Cancel
	L"Обновить",	//Refresh
	L"Имя игрока",	//Player Name
	L"IP сервера",	//Server IP
	L"Порт",	//Port
	L"Имя сервера",	//Server Name
	L"# Plrs",
	L"Версия",	//Version
	L"Тип игры",	//Game Type
	L"Ping",
	L"Впишите имя игрока.",
	L"Впишите корректный IP адрес. (пример 84.114.195.239).",
	L"Впишите корректный порт сервера (используйте диапазон от 1 до 65535).",
};

STR16 gzMPJHelpText[] =
{
	L"Новых игроков можно найти здесь: http://webchat.quakenet.org/?channels=ja2-multiplayer",

	L"СОЗДАТЬ ИГРУ",
	L"Введите '127.0.0.1' в поле IP и выберите номер порта начиная с 60000.",	//Enter '127.0.0.1' for the IP and the Port number should be greater than 60000.
	L"Убедитесь что выбранный порт (UDP, TCP) не блокируется роутером. Подробнее читайте здесь: http://portforward.com",
	L"Так же сообщите по IRC или ICQ другим игрокам ваш внешний IP адрес и порт (http://www.whatismyip.com).",
	L"Жмите на кнопку 'Создать игру' для запуска сервера сетевой игры.",
	
	L"ПРИСОЕДИНИТЬСЯ К ИГРЕ",
	L"Создавший игру должен был вам сообщить (по IRC, ICQ и т.д.) свой внешний IP адрес и порт.",
	L"Впишите эти данные в поле IP адреса и номер порта.",
	L"Жмите 'Присоединиться' чтобы подключиться к уже созданной сетевой игре.",
};

STR16	gzMPHScreenText[] =
{
	L"СТАРТОВЫЕ УСТАНОВКИ СЕРВЕРА",			//HOST GAME
	L"Начать игру",					//Start
	L"Главное меню",				//Cancel
	L"Имя сервера",					//Server Name
	L"Тип игры",					//Game Type
	L"Deathmatch",
	L"Team-Deathmatch",
	L"Co-Operative",
	L"Количество игроков",				//Max Players
	L"Солдат в отряде",				//Maximum Mercs
	L"Merc Selection",
	L"Найм бойцов",
	L"Нанят игроком",				//Hired by Player
	L"Деньги при старте",			//Starting Cash
	L"Можно нанимать тех же бойцов",	//Allow Hiring Same Merc
	L"Сообщения о нанятых бойцах",			//Report Hired Mercs
	L"Бобби Рэй",				//Bobby Rays
	L"Место высадки",	//Sector Starting Edge
	L"Впишите имя сервера",				//You must enter a server name
	L"",
	L"",
	L"Время суток",	//Starting Time
	L"",
	L"",
	L"Убойность оружия",	//Weapon Damage
	L"",
	L"Время хода",	//Timed Turns
	L"",
	L"Гражданские в CO-OP",		//Enable Civilians in CO-OP
	L"",
	L"Максимум врагов в CO-OP",	//Maximum Enemies in CO-OP
	L"Синхронизация игровых файлов",	//Synchronize Game Directory
	L"MP Sync. Directory",
	L"Укажите директорию для синхронизации передаваемых файлов.",
	L"(Для разделения директорий используйте '/' вместо '\\'.)",
	L"Указанная директория для синхронизации не существует.",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	// Max. Enemies / Report Hired Merc / Enable Civs in CO-OP
	L"да",
	L"нет",
	// Starting Time
	L"утро",
	L"день",
	L"ночь",
	// Starting Cash
	L"мало",
	L"средне",
	L"много",
	L"неограничено",
	// Time Turns
	L"не ограничено",	//Never
	L"медленно",	//Slow
	L"умеренно",	//Medium
	L"быстро",	//Fast
	// Weapon Damage
	L"очень малая",	//Very low
	L"небольшая",	//Low
	L"хорошая",	//Normal
	// Merc Hire
	L"случайно",
	L"самостоятельно",	//Normal
	// Sector Edge
	L"случайно",
	L"выборочно",
	// Bobby Ray / Hire same merc
	L"нет",
	L"есть",
};

STR16 pDeliveryLocationStrings[] =
{
	L"Остин",		//Austin, Texas, USA
	L"Багдад",		//Baghdad, Iraq (Suddam Hussein's home)
	L"Драссен",		//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Гонконг",		//Hong Kong, Hong Kong
	L"Бейрут",		//Beirut, Lebanon	(Middle East)
	L"Лондон",		//London, England
	L"Лос-Анджелес",//Los Angeles, California, USA (SW corner of USA)
	L"Медуна",		//Meduna -- the other airport in JA2 that you can receive items.
	L"Метавира",	//The island of Metavira was the fictional location used by JA1
	L"Майами",		//Miami, Florida, USA (SE corner of USA)
	L"Москва",		//Moscow, USSR
	L"Нью-Йорк",	//New York, New York, USA
	L"Оттава",		//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Париж",		//Paris, France
	L"Триполи",		//Tripoli, Libya (eastern Mediterranean)
	L"Токио",		//Tokyo, Japan
	L"Ванкувер",	//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Вы уверены? Значение ноль - означает отсутствие этого навыка вообще.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( до 8 символов )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Анализ",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Спасибо, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Голос", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Погиб в бою",
	L"Уволен",
	L"Другое",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Досье",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Пауза в игре",
	L"Продолжить (|P|a|u|s|e)",
	L"Пауза (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Выйти из игры?",
	L"Да",
	L"ДА",
	L"НЕТ",
	L"ОТМЕНА",
	L"НАНЯТЬ",
	L"СОЛГАТЬ",
	L"Нет описания.", //Save slots that don't have a description.
	L"Игра сохранена.",
	L"Игра сохранена.",
	L"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"День",
	L"Наёмн",
	L"Свободное место", //An empty save game slot
	L"Демо",				//Demo of JA2
	L"Ловля Багов",				//State of development of a project (JA2) that is a debug build
	L"Релиз",			//Release build for JA2
	L"пвм",				//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"мин",					//Abbreviation for minute.
	L"м",					//One character abbreviation for meter (metric distance measurement unit).
	L"пуль",				//Abbreviation for rounds (# of bullets)
	L"кг",					//Abbreviation for kilogram (metric weight measurement unit)
	L"фунт",				//Abbreviation for pounds (Imperial weight measurement unit)
	L"В начало",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"н/д",					//Lowercase acronym for not applicable.
	L"Посмотрим что происходит тем временем в другом месте",		//Meanwhile
	L"%s: прибыл в сектор %s%s",//30	Name/Squad has arrived in sector A9.  Order must not change without notifying
//SirTech
	L"Версия",
	L"Пустая ячейка быстрого сохр",
	L"Эта ячейка зарезервирована для Быстрого Сохранения, которое можно провести с тактической карты или с глобальной карты, нажав клавиши ALT+S.",
	L"Открытая",
	L"Закрытая",
	L"У вас заканчивается свободное дисковое пространство. На диске есть всего %sMб свободного места, а для Jagged Alliance 2 требуется %s Mб.",
	L"Из A.I.M. нанят боец %s.", 
	L"%s ловит %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s принимает препарат.", //'Merc name' has taken the drug
	L"%s: отсутствуют навыки в медицине.",//40	'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"Нарушена целостность программы.",
	L"ОШИБКА: CD-ROM открыт.",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Нет места, чтобы вести отсюда огонь.",
	
	//Can't change stance due to objects in the way...
	L"Сейчас нельзя изменить положение тела.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Выкинуть",
	L"Бросить",
	L"Передать",

	L"%s передан %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Не хватает места, чтобы передать %s %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" присоединён]",			// 50

	//Cheat modes
	L"Ну и зачем тебе это надо?",
	L"Активирован режим кодов.",

	//Toggling various stealth modes
	L"Отряд идёт тихим шагом.",
	L"Отряд идёт обычным шагом.",
	L"%s идёт тихим шагом.",
	L"%s идёт обычным шагом.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Каркас зданий ВКЛ.",
	L"Каркас зданий ВЫКЛ.",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Нельзя подняться с этого уровня...",
	L"Нет нижних этажей...",
	L"Входим в подвал. Уровень %d...",
	L"Покидаем подвал...",

	L".",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Режим следования ВЫКЛ.",
	L"Режим следования ВКЛ.",
	L"3D курсор ВЫКЛ.",
	L"3D курсор ВКЛ.",
	L"Выбран %d-й отряд.",
	L"Не хватает денег, чтобы заплатить %s ежедневный гонорар %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Нет",	//Skip
	L"%s не может уйти в одиночку.",
	L"Файл сохранения был записан под названием SaveGame249.sav. Если необходимо, переименуйте его в SaveGame01 - SaveGame10 и тогда, он станет доступен в списке сохранений.", 
	L"%s: выпил(а) немного %s.",
	L"Посылка прибыла в Драссен.",
	L"%s прибудет в точку назначения (сектор %s) в %dй день, примерно в %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"В журнал добавлена запись!",
	L"Очереди из гранат используют курсор стрельбы очередями (стрельба по площадям возможна)",
	L"Очереди из гранат используют курсор метания (стрельба по площадям не возможна)",
	L"Включены подписи к солдатам", // Changed from Drop All On (Enabled Soldier Tooltips)
	L"Отключены подписи к солдатам", // Changed from Drop All Off (Disabled Soldier Tooltips)
	L"Гранатометы стреляют под обычным углом",
	L"Гранатометы стреляют навесом",
	// forced turn mode strings
    L"Только пошаговый режим",
	L"Режим реального времени",	//Normal turn mode
	L"Выход из пошагового режима",	//Exit combat mode
	L"Включен только пошаговый режим. Вступаем в бой!",	//Forced Turn Mode Active, Entering Combat
	L"Игра сохранена в поле авто-сохранения.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Клиент",	//Client
	L"Нельзя одновременно установить \"Старый\" инвентарь и \"Новую Систему Навески\".",	//You cannot use the Old Inventory and the New Attachment System at the same time.

	L"Auto Save #", //91		// Text des Auto Saves im Load Screen mit ID
	L"This Slot is reserved for Auto Saves, which can be enabled/disabled (AUTO_SAVE_EVERY_N_HOURS) in the ja2_options.ini.", //92	// The text, when the user clicks on the save screen on an auto save
	L"Empty Auto Save Slot #", //93	// The text, when the auto save slot (1 - 5) is empty (not saved yet)
	L"AutoSaveGame",		// 94	// The filename of the auto save, such as AutoSaveGame01 - AutoSaveGame05
	L"End-Turn Save #",	// 95	// The text for the tactical end turn auto save
	L"Saving Auto Save #",	// 96	// The message box, when doing auto save
	L"Saving",	// 97	// The message box, when doing end turn auto save
	L"Empty End-Turn Save Slot #",	// 98	// The message box, when doing auto save
	L"This Slot is reserved for Tactical End-Turn Saves, which can be enabled/disabled in the Option Screen.", //99	// The text, when the user clicks on the save screen on an auto save
	// Mouse tooltips
	L"QuickSave.sav",	// 100
	L"AutoSaveGame%02d.sav",	// 101
	L"Auto%02d.sav",	// 102
	L"SaveGame%02d.sav", //103
	// Lock / release mouse in windowed mode (window boundary)			// TODO.Translate
	L"Lock mouse cursor within game window boundary.",			// 104
	L"Release mouse cursor from game window boundary.",			// 105
	L"Move in Formation ON",	// TODO.Translate
	L"Move in Formation OFF",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"Взять",
	L"Вверх",
	L"Выбрать все",
	L"Вниз",
	L"Отмена",
};

STR16 pDoctorWarningString[] =
{
	L"%s слишком далеко, чтобы подлечиться.",
	L"Ваши медики не могут оказать первую помощь всем раненым.",
};

// TODO.Translate
STR16 pMilitiaButtonsHelpText[] =
{
	L"Unassign (|R|i|g|h|t |C|l|i|c|k)\nAssign (|L|e|f|t |C|l|i|c|k)\nGreen Troops", // button help text informing player they can pick up or drop militia with this button
	L"Unassign (|R|i|g|h|t |C|l|i|c|k)\nAssign (|L|e|f|t |C|l|i|c|k)\nRegular Troops",
	L"Unassign (|R|i|g|h|t |C|l|i|c|k)\nAssign (|L|e|f|t |C|l|i|c|k)\nVeteran Troops",
	L"Distribute available militia equally among all sectors",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"Отправляйтесь в A.I.M. и наймите бойцов (*Подсказка* - это в лэптопе).", // to inform the player to hired some mercs to get things going
#ifdef JA2UB
	L"Когда будете готовы отправиться в Tracona, включите сжатие времени в правом нижнем углу экрана.", // to inform the player to hit time compression to get the game underway
#else
	L"Когда будете готовы отправиться в Арулько, включите сжатие времени в правом нижнем углу экрана.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Ошибка. Пропущен или испорчен файл(ы). Игра прекращает работу.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Просмотреть почту",
	L"Посетить интернет сайты",
	L"Просмотреть полученные данные",
	L"Просмотреть журнал последних событий",
	L"Показать информацию о команде",
	L"Просмотреть финансовые отчеты",
	L"Закрыть лэптоп",

	//Bottom task bar icons (if they exist):
	L"Получена новая почта",
	L"Получены новые данные",

	//Bookmarks:
	L"Международная Ассоциация Наемников A.I.M.",
	L"Бобби Рэй - заказ оружия через Интернет",
	L"Институт Изучения Личности Наемника I.M.P.",
	L"Центр рекрутов M.E.R.C.",
	L"Похоронная служба Макгилликатти",
	L"'Цветы по всему миру'",
	L"Страховые агенты по контрактам A.I.M.",
	//New Bookmarks
	L"",
	L"Encyclopedia",
	L"Briefing Room",
};


STR16	gzHelpScreenText[] =
{
	L"Закрыть окно помощи",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Идет бой. Вы можете отступить только через тактический экран.",
	L"Войти в сектор, чтобы продолжить бой. (|E)",
	L"Провести бой автоматически (|A).",
	L"Во время атаки врага автоматическую битву включить нельзя.",
	L"После того как вы попали в засаду, автоматическую битву включить нельзя.",
	L"Рядом рептионы - автоматическую битву включить нельзя.",
	L"Рядом враждебные гражданские - автоматическую битву включить нельзя.",
	L"Рядом кошки-убийцы - автоматическую битву включить нельзя.",
	L"ИДЕТ БОЙ",
	L"Сейчас вы не можете отступить.",
};

STR16 gzMiscString[] =
{
	L"Ваши ополченцы продолжают бой без помощи наемников...",
	L"Сейчас машине топливо не требуется.",
	L"Топливный бак полон на %d%%.",
	L"%s полностью под контролем Дейдраны.",
	L"Вы потеряли заправочную станцию.",
};

STR16	gzIntroScreen[] = 
{
	L"Не удается найти вступительный видеоролик",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s слышит %s звук %s.",
	L"%s слышит %s звук движения %s.",
	L"%s слышит %s скрип, идущий %s.",
	L"%s слышит %s звук всплеска %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звук взрыва %s.",
	L"%s слышит %s крик %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звук удара %s.",
	L"%s слышит %s звон %s.",
	L"%s слышит %s грохот %s.",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Сортировка по имени (|F|1)",
	L"Сортировка по роду деятельности (|F|2)",
	L"Сортировка по состоянию сна (|F|3)",
	L"Сортировка по месту пребывания (|F|4)",
	L"Сортировка по месту назначения (|F|5)",
	L"Сортировка по времени контракта (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Ошибка 404",
	L"Сайт не найден.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Последние поступления",
	L"Заказ №",
	L"Количество",
	L"Заказано",
};


STR16	gzCreditNames[]=
{
	L"Chris Camfield",
	L"Shaun Lyng",
	L"Kris Märnes",
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
	L"Ведущий программист игры", 				// Chris Camfield
	L"Дизайнер/Сценарист",					// Shaun Lyng
	L"Программист стратегической части и редактора",	//Kris \"The Cow Rape Man\" Marnes
	L"Продюсер/Дизайнер",					// Ian Currie
	L"Дизайнер/Дизайн карт",				// Linda Currie
	L"Художник",						// Eric \"WTF\" Cheng
	L"Тестирование, поддержка",				// Lynn Holowka
	L"Главный художник",					// Norman \"NRG\" Olsen
	L"Мастер по звуку",					// George Brooks
	L"Дизайнер экранов/художник",				// Andrew Stacey
	L"Ведущий художник/аниматор",				// Scot Loving
	L"Ведущий программист",					// Andrew \"Big Cheese Doddle\" Emmons
	L"Программист",						// Dave French
	L"Программист стратегии и баланса игры",		// Alex Meduna
	L"Художник-портретист",					// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 												// Chris Camfield
	L"(Всё ещё зубрит правила пунктуации)",				// Shaun Lyng
	L"(\"Готово! Осталось только баги исправить.\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(Уже слишком стар для всего этого)",				// Ian Currie
	L"(Также работает над Wizardry 8)",					// Linda Currie
	L"(Заставили тестировать под дулом пистолета)",		// Eric \"WTF\" Cheng
	L"(Ушла от нас в CFSA - скатертью дорожка...)",		// Lynn Holowka
	L"",							// Norman \"NRG\" Olsen
	L"",							// George Brooks
	L"(Поклонник джаза и группы Dead Head)",			// Andrew Stacey
	L"(Его настоящее имя Роберт)",						// Scot Loving
	L"(Единственный ответственный человек)",			// Andrew \"Big Cheese Doddle\" Emmons
	L"(Может опять заняться мотогонками)",				// Dave French
	L"(Украден с работы над Wizardry 8)",				// Alex Meduna
	L"(Делал предметы и загрузочные экраны!)",			// Joey \"Joeker\" Whelan",
};

// HEADROCK: Adjusted strings for better feedback, and added new string for LBE repair.
STR16 sRepairsDoneString[] =
{
	L"%s: завершён ремонт личных вещей.",
	L"%s: завершён ремонт всего оружия и брони.",
	L"%s: завершён ремонт всей экипировки отряда.",
	L"%s: завершён ремонт всех крупных вещей отряда.",	//%s finished repairing everyone's large carried items
	L"%s: завершён ремонт всех малых вещей отряда.",	//%s finished repairing everyone's medium carried items
	L"%s: завершён ремонт всех мелких вещей отряда.",	//%s finished repairing everyone's small carried items
	L"%s: завершён ремонт разгрузочных систем отряда.",	//%s finished repairing everyone's LBE gear
};

STR16 zGioDifConfirmText[]=
{
	L"Вы выбрали ЛЁГКИЙ уровень сложности. Этот режим предназначен для первичного ознакомления с Jagged Alliance. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали СРЕДНИЙ уровень сложности. Этот режим предназначен для тех, кто знаком с Jagged Alliance и подобными играми. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали ТЯЖЁЛЫЙ уровень сложности. В этом режиме вам потребуется немалый опыт игры в Jagged Alliance. Ваш выбор определит ход всей игры, так что будьте осторожны. Вы действительно хотите начать игру в этом режиме?", 
	L"Вы выбрали БЕЗУМНЫЙ уровень сложности. Имейте в виду - в этом режиме возможности Дейдраны воистину за пределами разумного! Но если с головой вы не в ладах, то вам даже понравится. Рискнете?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S файл для загрузки экрана не найден...",

	//1-5
	L"Робот не сможет покинуть этот сектор, пока кто-нибудь не возьмет пульт управления.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Сейчас нельзя включить сжатие времени. Дождитесь взрыва!", 

	//'Name' refuses to move.
	L"%s отказывается подвинуться.",

	//%s a merc name
	L"%s: недостаточно очков действия для изменения положения.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s: закончилось топливо. Машина осталась в %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"сверху",
	L"снизу",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Никто из ваших наемников не имеет медицинских навыков.",
	L"Нечем бинтовать. Ни у кого из наемников нет аптечки.",
	L"Чтобы перевязать всех наемников, не хватило бинтов.",
	L"Никто из ваших наемников не нуждается в перевязке.",
	L"Автоматически перевязывать бойцов.",
	L"Все ваши наемники перевязаны.",

	//14
#ifdef JA2UB
	L"Tracona",
#else
	L"Арулько",
#endif

	L"(на крыше)",

	L"Здоровье: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d против %d",
	
	L"%s полон!",  //(ex "The ice cream truck is full")

	L"%s нуждается не в первой помощи или перевязке, а в серьезном лечении и/или отдыхе.", 

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"Из-за ранения в ногу %s падает на землю!",
	//Name can't speak right now.
	L"%s сейчас не может говорить",

	//22-24 plural versions @@@2 elite to veteran
	L"%d новобранца из ополчения произведены в элитных солдат.",
	L"%d новобранца из ополчения произведены в рядовые.",
	L"%d рядовых ополченца произведены в элитных солдат.",

	//25
	L"Кнопка",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"У %s приступ безумия!",

	//27-28
	//Messages why a player can't time compress.
	L"Сейчас небезопасно включать сжатие времени - у вас есть наемники в секторе %s.",
	L"Сейчас небезопасно включать сжатие времени - у вас есть наемники в пещерах с тварями.",

	//29-31 singular versions @@@2 elite to veteran
	L"1 новобранец из ополчения стал элитным солдатом.",
	L"1 новобранец из ополчения стал рядовым ополченцем.",
	L"1 рядовой ополченец стал элитным солдатом.",

	//32-34
	L"%s ничего не говорит.",
	L"Выбраться на поверхность?",
	L"(%dй отряд)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s отремонтировал(а) у %s %s",

	//36
	L"ГЕПАРД",

	//37-38 "Name trips and falls"
	L"%s спотыкается и падает.",
	L"Этот предмет отсюда взять невозможно.",

	//39
	L"Оставшиеся бойцы не могут сражаться. Сражение с тварями продолжит ополчение.",

	//40-43
	//%s is the name of merc.
	L"%s: закончились медикаменты!",
	L"%s: недостаточно навыков для лечения.",
	L"%s: закончился ремонтный набор!",
	L"%s: недостаточно навыков для ремонта.",

	//44-45
	L"Время ремонта",
	L"%s не видит этого человека.",

	//46-48
	L"%s: отвалилась ствольная насадка!",
	// HEADROCK HAM 3.5: Changed to reflect facility effect.
	L"В этом секторе ополченцев могут тренировать не более %d человек.",	//No more than %d militia trainers are permitted in this sector.
	L"Вы уверены?",

	//49-50
	L"Сжатие времени.",
	L"Бак машины полон.",

	//51-52 Fast help text in mapscreen.
	L"Возобновить сжатие времени (|П|р|о|б|е|л)",
	L"Прекратить сжатие времени (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s починил(а) %s",
	L"%s починил(а) %s (%s)",

	//55 
	L"Нельзя включить сжатие времени при просмотре предметов в секторе.",

	L"CD Агония Власти не найден. Программа выходит в ОС.",

	L"Предметы успешно совмещены.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Прогресс игры текущий/максимально достигнутый: %d%%/%d%%",

	L"Сопроводить Джона и Мэри?",
	
	//60
	L"Кнопка нажата.",

	L"%s чувствует что в бронежилете что-то треснуло!",
	L"%s выпустил на %d больше пуль!",
	L"%s выпустил на одну пулю больше!",

	L"Сперва закрой описание предмета!",

	L"Cannot compress time - hostile civilians and/or bloodcats are in this sector.", // 65	// TODO.Translate
};

// HEADROCK HAM 3.5: Added sector name
STR16 gzCWStrings[] = 
{
	L"Вызвать подкрепление из соседних секторов для %s?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Место: %d\n",
	L"%s|Яркость: %d / %d\n",
	L"%s|Дистанция до |Цели: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Приказы: %d\n",
	L"%s|Настрой: %d\n",
	L"%s|Текущие |A|Ps: %d\n",
	L"%s|Текущее |Здоровье: %d\n",
	L"%s|Current |Breath: %d\n",	// TODO.Translate
	L"%s|Current |Morale: %d\n",
	L"%s|Current |S|hock: %d\n",//TODO.Translate
	L"%s|Current |S|uppression Points: %d\n",//TODO.Translate
	// Full info
	L"%s|Каска: %s\n",
	L"%s|Жилет: %s\n",
	L"%s|Брюки: %s\n",
	// Limited, Basic
	L"|Броня: ",
	L"Каска ",
	L"Жилет ",
	L"Брюки",
	L"Одет",
	L"нет брони",
	L"%s|П|Н|В: %s\n",
	L"нет ПНВ",
	L"%s|Противогаз: %s\n",
	L"нет противогаза",
	L"%s|Голова,|Слот |1: %s\n",
	L"%s|Голова,|Слот |2: %s\n",
	L"\n(в рюкзаке) ",
	L"%s|Оружие: %s ",
	L"без оружия",
	L"Пистолет",
	L"Пистолет-пулемет",
	L"Винтовка",
	L"Ручной пулемет",
	L"Ружье",
	L"Нож",
	L"Тяжелое оружие",
	L"без каски",
	L"без бронежилета",
	L"без поножей",
	L"|Броня: %s\n", 
	 // Added - SANDRO
	L"%s|Навык 1: %s\n",	//%s|Skill 1: %s\n
	L"%s|Навык 2: %s\n", 
	L"%s|Навык 3: %s\n",
};

STR16 New113Message[] =
{
	L"Началась буря.",
	L"Буря закончилась.",
	L"Начался дождь.",
	L"Дождь закончился.",
	L"Опасайтесь снайперов...",
	L"Огонь на подавление!",	//suppression fire!
	L"*",			//BRST - стабильна по количеству выпущенных пуль
	L"***",			//AUTO - регулируемая очередь
	L"ГР",
	L"ГР *",
	L"ГР ***",
	L"UB",			// TODO.Translate
	L"UBRST",
	L"UAUTO",
	L"BAYONET",
	L"Снайпер!",
	L"Невозможно разделить деньги из-за предмета на курсоре.",
	L"Точка высадки прибывающих наёмников перенесена в %s, так как запланированное место высадки %s захвачено противником.",
	L"Выброшена вещь.",
	L"Выброшены все вещи выбранной группы.",
	L"Вещь продана голодающему населению Арулько.",
	L"Проданы все вещи выбранной группы.",
	L"Проверь что солдату мешает лучше видеть.",	//You should check your goggles
	// Real Time Mode messages
	L"Уже в бою.",	//In combat already
	L"В приделах видимости нет врагов.",	//No enemies in sight
	L"Красться в режиме реального времени ОТКЛ.",	//Real-time sneaking OFF
	L"Красться в режиме реального времени ВКЛ.",	//Real-time sneaking ON
	//L"Enemy spotted! (Ctrl + x to enter turn based)",
	L"Обнаружен враг!", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s отлично справился с кражей!",	//%s was successful at stealing!
	L"%s: недостаточно очков действия, чтобы украсть все выбранные вещи.",	//%s did not have enough action points to steal all selected items.
	L"Хотите провести хирургическую операцию %s перед перевязкой? (Вы сможете восстановить около %i здоровья).",	//Do you want to perform surgery on %s before bandaging? (You can heal about %i Health.)
	L"Хотите провести хирургическую операцию %s? (Вы сможете восстановить около %i здоровья).",	//Do you want to perform surgery on %s? (You can heal about %i Health.)
	L"Хотите сначала провести необходимую хирургическую операцию? (пациент(ы) - %i).",	//Do you wish to perform necessary surgeries first? (%i patient(s))
	L"Хотите провести операцию сначала этому пациенту?",	//Do you wish to perform the surgery on this patient first?
	L"Оказывать первую помощь с хирургическим вмешательством или без него?",	//Apply first aid automatically with necessary surgeries or without them?
	L"%s успешно прооперирован(а).",	//Surgery on %s finished.
	L"%s пропустил(а) удар в грудную клетку и теряет единицу максимального значения здоровья!",	//%s is hit in the chest and loses a point of maximum health!
	L"%s пропустил(а) удар в грудную клетку и теряет %d максимального значения здоровья!",	//%s is hit in the chest and loses %d points of maximum health!
	L"%s восстановил(а) одну единицу потерянного %s.",	//%s has regained one point of lost %s
	L"%s восстановил(а) %d единиц потерянного %s.",	//%s has regained %d points of lost %s
	L"Ваши навыки разведчика сорвали засаду противника.",
	L"Благодаря вашим навыкам разведчика вы успешно избежали встречи с кошками-убицами!",	
	L"%s получает удар в пах и падает на землю в адской боли!",
/////
	L"Внимание: враг обнаружил труп!!!",
	L"%s [%d патр.]\n%s %1.1f %s",
	L"Insufficient AP Points! Cost %d, you have %d.",	// TODO.Translate
	L"Hint: %s",	// TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s в страхе пытается укрыться!",	//%s cowers in fear!	%s съёжился от испуга!
	L"%s прижат(а) к земле вражеским огнём!",	//%s is pinned down!
	L"%s дал более длинную очередь!",	//%s fires more rounds than intended!
	L"Вы не можете тренировать ополчение в этом секторе.",	//You cannot train militia in this sector.
	L"Ополченец подобрал %s.",	//Militia picks up %s.
	L"Невозможно тренировать ополчение когда в секторе враг!",	//Cannot train militia with enemies present!
	// 6 - 10
	L"%s имеет низкий навык Лидерства, чтобы тренировать ополченцев.",	//%s lacks sufficient Leadership score to train militia.
	L"В этом секторе не может быть тренеров мобильных групп больше %d.",	//No more than %d Mobile Militia trainers are permitted in this sector.
	L"Нет свободных мест в %s или вокруг него для новой мобильной группы!",	//No room in %s or around it for new Mobile Militia!
	L"Нужно иметь по %d ополченцев в каждом освобождённом секторе города %s, прежде чем можно будет тренировать мобильные группы.",	//You need to have %d Town Militia in each of %s's liberated sectors before you can train Mobile Militia here.
	L"Невозможно работать в городском учереждении пока враг в секторе!",	//Can't staff a facility while enemies are present!
	// 11 - 15
	L"%s имеет мало Мудрости, чтобы работать в городском учереждении.",	//%s lacks sufficient Wisdom to staff this facility.
	L"Учереждение %s полностью укомплектованно персоналом.",	//The %s is already fully-staffed.
	L"Один час услуг этого заведения обойдётся вам в $%d. Согласны оплачивать?",	//It will cost $%d per hour to staff this facility. Do you wish to continue?
        L"У вас недостаточно денег чтобы оплатить работу в учреждениии за сегодня. $%d выплачено, ещё нужно $%d. Местным это не понравилось.", //You have insufficient funds to pay for all Facility work today. $%d have been paid, but you still owe $%d. The locals are not pleased.",
        L"У вас недостаточно денег чтобы выплатить заработную плату всем рабочим. Теперь долг составил $%d. Местным это не понравилось.",
	// 16 - 20
	L"Непогашенный долг составляет $%d для работы учереждения, и нет денег чтобы его погасить!",	//You have an outstanding debt of $%d for Facility Operation, and no money to pay it off!
	L"Непогашенный долг составляет $%d для работы учереждения. Вы не можете назначить бойца на работу в учереждении пока не погасите задолженность.",	//You have an outstanding debt of $%d for Facility Operation. You can't assign this merc to facility duty until you have enough money to pay off the entire debt.
	L"Непогашенный долг составляет $%d для работы учереждения. Выплатить деньги по задолженности?",	//You have an outstanding debt of $%d for Facility Operation. Would you like to pay it all back?
	L"Н/Д в этом секторе",	//N/A in this sector
	L"Дневной расход",
	// 21 - 25
	L"Недостаточно денег для выплат нанятому ополчению. %d ополченцев было распущенно и отправлено домой.",	//Insufficient funds to pay all enlisted militia! %d militia have disbanded and returned home.
	L"To examine an item's stats during combat, you must pick it up manually first.", // HAM 5 TODO.Translate
	L"To attach an item to another item during combat, you must pick them both up first.", // HAM 5
	L"To merge two items during combat, you must pick them both up first.", // HAM 5
};

// HEADROCK HAM 5: Text dealing exclusively with Item Transformations.
STR16 gzTransformationMessage[] = 
{
	L"No available adjustments",	// TODO.Translate
	L"%s was split into several parts.",
	L"%s was split into several parts. Check %s's inventory for the resulting items.",
	L"Due to lack of inventory space after transformation, some of %s's items have been dropped to the ground.",
	L"%s was split into several parts. Due to a lack of inventory space, %s has had to drop a few items to the ground.",
	L"Do you wish to adjust all %d items in this stack? (To transform only one item, remove it from the stack first)",
	// 6 - 10
	L"Split Crate into Inventory",
	L"Split into %d-rd Mags",
	L"%s was split into %d Magazines containing %d rounds each.",
	L"%s was split into %s's inventory.",
	L"There is insufficient room in %s's inventory to fit any magazines of this caliber!",
};

// WANNE: This hardcoded text should not be used anymore in the game, because we now have those texts externalized in the "TableData\Email\EmailMercLevelUp.xml" file!
// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Пожалуйста, примите к сведению, что с настоящего момента гонорар Гастона увеличивается вследствие повышения его профессионального уровня. ± ± Спек Т. Кляйн ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Пожалуйста, примите к сведению, что повышение боевых навыков лейтенанта Хорга 'Сигары' влечет за собой повышение его гонорара. ± ± Спек Т. Кляйн ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Прошу принять к сведению, что заслуги Текса позволяют ему требовать более достойной оплаты. Поэтому его гонорар был увеличен, чтобы соответствовать его умениям. ± ± Спек Т. Кляйн ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Ставим в известность, что отличная работа полковника Фредерика Биггенса заслуживает поощрения в виде повышения гонорара. Постановление считать действительным с текущего момента. ± ± Спек Т. Кляйн ± ",
};

// WANNE: This hardcoded text should not be used anymore in the game, because we now have those texts externalized in the "TableData\Email\EmailMercAvailable.xml" file!
// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"FW с сервера A.I.M.: Письмо от Виктора Колесникова",
	L"Привет. Это Монк. Получил твое сообщение. Я вернулся, так что можешь со мной связаться. ± ± Жду звонка. ±",

	// Brain
	L"FW с сервера A.I.M.: Письмо от Янно Аллика",
	L"Я готов обсудить задания. Для всего есть свое время и место. ± ± Янно Аллик ±",

	// Scream
	L"FW с сервера A.I.M.: Письмо от Леннарта Вильде",
	L"Леннарт Вильде вернулся!",

	// Henning
	L"FW с сервера A.I.M.: Письмо от Хеннинга фон Браница",
	L"Получил твое сообщение, спасибо. Если хочешь обсудить работу, свяжись со мной на сайте A.I.M. До встречи! ± ± Хеннинг фон Браниц ±",

	// Luc
	L"FW с сервера A.I.M.: Письмо от Люка Фабра",
	L"Послание получил, мерси! С удовольствием рассмотрю ваши предложения. Вы знаете, где меня найти. ± ± Жду с нетерпением ±",

	// Laura
	L"FW с сервера A.I.M.: Письмо от Лоры Колин",
	L"Привет! Спасибо, что оставили сообщение. Звучит интересно. ± ± Зайдите снова в A.I.M. Хотелось бы услышать больше. ± ± С уважением! ± ± Др. Лора Колин ± ± P.S. Надеюсь, Monk уже в вашей команде? ±",

	// Grace
	L"FW с сервера A.I.M.: Письмо от Грациеллы Джирелли",
	L"Вы хотели связаться со мной, но неудачно.± ± Семейное собрание. Думаю, вы понимаете. Я уже устала от семьи и буду рада. Если вы снова свяжетесь со мной через сайт A.I.M. ± ± Чао! ±",

	// Rudolf
	L"FW с сервера A.I.M.: Письмо от Рудольфа Штайгера",
	L"Ты знаешь, сколько звонков я получаю каждый день? Любой придурок считает, что может позвонить мне. ± ± Но я вернулся, если тебе есть чем меня заинтересовать. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"FW с сервера A.I.M.: Наёмник доступен",
	L"Я на месте. Жду звонка чтобы обсудить условия контракта. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Снайпер: У вас глаза ястреба. В свободное время вы развлекаетесь отстреливая крылышки у мух с расстояния 100 метров! ± ",	//Sniper: Eyes of a hawk, you can shoot the wings from a fly at a hundred yards!
	// Camouflage
	L"Маскировка: На вашем фоне кусты выглядят синтетическими! ± ",	//Camouflage: Beside you, even bushes look synthetic!
	// SANDRO - new strings for new traits added
	// MINTY - Altered the texts for more natural English, and added a little flavour too
	// Ranger
	L"Лесничий: Эти рейнджеры из Техаса, на вашем фоне выгледят дилетантами! Вы умеете рационально выбирать путь, стать незаметным для зверя, с одного выстрела из ружья попасть белке в глаз. ± ",	//Ranger: Those amateurs from Texas have nothing on you!
	// Gunslinger
	L"Ковбой: С одним револьвером, либо с двумя - вы так же опасны как Билли Кид! ± ",	//Gunslinger: With one handgun or two, you can be as lethal as Billy the Kid!
	// Squadleader
	L"Командир: Прирождённый лидер, солдаты просто боготворят вас! ± ",	//Squadleader: A natural leader, your squadmates look to you for inspiration!
	// Technician
	L"Механик: Ангус МакГайвер по сравнению с вами просто никто! Механика, электроника или взрывчатка - вы отремонтируете что угодно! ± ",	//Technician: MacGyver's got nothing on you! Mechanical, electronic or explosive, you can fix it!
	// Doctor
	L"Доктор: Будь то царапины или вскрытое брюхо, нужна ампутация или же наоборот, пришить чего-нибудь - вы с лёгкостью справитесь с любым недугом! ± ",	//Doctor: From grazes to gutshot, to amputations, you can heal them all!
	// Athletics
	L"Спортсмен: Ваша скорость и выносливость достойны олимпийца! ± ",	//Athletics: Your speed and vitality are worthy of an Olympian!
	// Bodybuilding
	L"Культурист: Шварц? Да он слабак! Вы с лёгкостью завалите его одной левой! ± ",	//Bodybuilding: Arnie? What a wimp! You could beat him with one arm behind your back!
	// Demolitions
	L"Подрывник: Сеять гранаты, как семена по полю; минировать поле, как картошку садить - густо и минимум 20 соток; а после созерцать на полет конечностей... Вот то, ради чего вы живёте! ± ",	//Demolitions: Sowing grenades like seeds, planting bombs, watching the limbs flying.. This is what you live for!
	// Scouting
	L"Разведчик: Ничто не скроется от вашего зоркого взгляда! ± ",	//Scouting: Nothing can escape your notice!
	// Covert ops
	L"Covert Operations: You make 007 look like an amateur! ± ",		// TODO.Translate
	// Radio Operator
	L"Radio Operator: Your usage of communication devices broaden your team's tactical and strategic skills. ± ",	// TOO.Translate
};

STR16 NewInvMessage[] = 
{
	L"В данный момент поднять рюкзак нельзя.",
	L"Вы не можете одновременно носить 2 рюкзака.",
	L"Вы потеряли свой рюкзак...",
	L"Замок рюкзака работает лишь во время битвы.",
	L"Вы не можете передвигаться с открытым рюкзаком.",
	L"Вы уверены что хотите продать весь хлам этого сектора голодающему населению Арулько?",
	L"Вы уверены что хотите выбросить весь хлам, валяющийся в этом секторе?",
	L"Тяжеловато будет взбираться с полным рюкзаком на крышу. Может снимем?",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Запускается сервер RakNet...",
	L"Сервер запущен, ожидание подключений...",
	L"Теперь вам надо подключиться к серверу, нажав '2'.",
	L"Сервер уже запущен.",
	L"Не удалось запустить сервер. Прекращаю работу.",
	// 5
	L"%d/%d клиентов готовы к режиму реального времени.",
	L"Сервер отключился и прекратил свою работу.",
	L"Сервер не запущен.",
	L"Подождите пожалуйста, игроки все еще загружаются...",
	L"Вы не можете изменять зону высадки после запуска сервера.",
	// 10
	L"Отправка файла '%S' - 100/100",	//Sent file '%S' - 100/100
	L"Завершена отправка файлов для '%S'.",	//Finished sending files to '%S'.
	L"Начата отправка файлов для '%S'.",	//Started sending files to '%S'.
	L"Use the Airspace view to select a map you wish to play. If you want to change the map you have to do this before clicking the 'Start Game' button.",	// TODO.Translate
};

STR16 MPClientMessage[] =
{
	// 0
	L"Запускается клиент RakNet...",		
	L"Подключение к IP: %S ...",
	L"Получены настройки игры:",
	L"Вы уже подключены.",
	L"Вы уже подключаетесь...",
	// 5
	L"Клиент №%d - '%S' нанял %s.",
	L"Клиент №%d - '%S' нанял еще бойца.",
	L"Вы готовы к бою (всего готово = %d/%d).",
	L"Вы отменили готовность к бою (всего готово = %d/%d).",
	L"Отряды подтягиваются к месту битвы...",	//'Starting battle...'
	// 10
	L"Клиент №%d - '%S' готов к бою (всего готово = %d/%d).",
	L"Клиент №%d - '%S' отменил готовность к бою (всего готово = %d/%d).",
	L"Похоже, вы уже готовы к бою, однако, придется подождать остальных. (Если хотите изменить расположение своих бойцов, нажмите кнопку 'ДА').",
	L"Начнем же битву!",
	L"Для начала игры необходимо запустить клиент.",
	// 15
	L"Игра не может быть начата, вы не наняли ни одного бойца.",
	L"Ждем, когда сервер даст добро на доступ к лэптопу...",
	L"Перехвачен",			//Interrupted
	L"Продолжение после перехвата",	//Finish from interrupt
	L"Координаты курсора:",		//Mouse Grid Coordinates
	// 20
	L"X: %d, Y: %d",
	L"Номер квадрата: %d",		//Grid Number
	L"Доступно лишь для сервера.",
	L"Выберите какую ступень игры принудительно запустить:  ('1' - открыть лэптоп/найм бойцов)  ('2' - запустить/загрузить уровень)  ('3' - разблокировать пользовательский интерфейс)  ('4' - завершить расстановку)",
	L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	// 25
	L"",
	L"Новый игрок: клиент №%d - '%S'.",
	L"Команда: %d.",//not used any more
	L"%s (клиент %d - '%S') был убит %s (клиент %d - '%S')",
	L"Клиент №%d - '%S' выкинут из игры.",
	// 30
	L"Принудительно дать ход клиенту. №1: <Отменить>, №2: %S, №3: %S, №4: %S",
	L"Начался ход клиента №%d",
	L"Запрос перехода в режим реального время...",
	L"Переход в режим реального времени.",
	L"Ошибка: что-то пошло не так, возвращаю обратно.",
	// 35
	L"Открыть доступ к лэптопу? (Уверены что все игроки подключились?)",
	L"Сервером был открыт доступ к лэптопу. Приступайте к найму бойцов!",
	L"Перехватчик.",
	L"Клиент не может изменять зону высадки, доступно лишь серверу.",
	L"Вы отказались от предложения сдаться, потому что это не актуально в сетевой игре.",
	// 40
	L"Все ваши бойцы были убиты!",
	L"Активизирован режим наблюдения.",
	L"Вы потерпели поражение!",
	L"Извините, залезать на крышу в сетевой игре запрещено.",
	L"Вы наняли %s.",
	// 45
	L"Вы не можете изменить карту после начала закупки.",
	L"Карта изменена на '%s'",
	L"Клиент '%s' отключился, убираем его из игры.",
	L"Вы были отключены от игры, возвращаемся в главное меню.",
	L"Подключиться не удалось. Повторная попытка через 5 секунд (осталось %i попыток)",
	//50
	L"Подключиться не удалось, сдаюсь...",
	L"Вы не можете начать игру во время подключения других игроков.",
	L"%s : %s",
	L"Отправить всем",
	L"Только союзникам",
	// 55
	L"Не могу присоединиться к игре. Игра уже началась.",
	L"%s (команда): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"От сервера получены все необходимые файлы.",
	L"'%S' закачка с сервера завершена.",
	L"'%S' начата закачка с сервера.",
	L"Нельзя начать игру пока все игроки не завершать приём файлов от сервера.",
	L"Для игры на этом сервере необходимо скачать некоторые изменённые файлы, желаете продолжить?",
	// 65
	L"Нажмите 'Готов' для входа на тактическую карту.",
	L"Не удаётся подключиться. Версия вашего клиента (%S) отличается от версии сервера (%S).",
	L"Вы убили вражеского солдата.",
	L"Нельзя запустить игру потому что все команды одинаковые.",
	L"Игра на сервере создана с Новым Инвентарём (NIV), а выбранное вами разрешение экрана не поддерживается NIV.",
	// 70	
	L"Невозможно сохранить принятый файл '%S'",
	L"%s's бомба была разряжена %s",
	L"Вы проиграли. Стыд и срам!",	// All over red rover
	L"Spectator mode disabled",
	L"Укажите номер клиента, который нужно кикнуть. №1: <Отменить>, №2: %S, №3: %S, №4: %S",
	// 75
	L"Команда %s уничтожена.",
	L"Ошибка при запуске клиента. Завершение операции.",
	L"Клиент отсоединился и закрыт.",
	L"Клиент не запущен.",
	L"INFO: If the game is stuck (the opponents progress bar is not moving), notify the server to press ALT + E to give the turn back to you!",	// TODO.Translate
	// 80
	L"AI's turn - %d left",		// TODO.Translate
};

STR16 gszMPEdgesText[] =
{
	L"С",	//N
	L"Ю",	//S
	L"В",	//E
	L"З",	//W
	L"Ц",	// "C"enter
};

STR16 gszMPTeamNames[] =
{
	L"Фокстрот",	//Foxtrot
	L"Браво",	//Bravo
	L"Дельта",	//Delta
	L"Чарли",	//Charlie
	L"Н/Д",		// Acronym of Not Applicable
};

STR16 gszMPMapscreenText[] =
{
	L"Тип игры: ",		//Game Type: 
	L"Игроков: ",		//Players: 
	L"Всего бойцов: ",	//Mercs each: 
	L"Нельзя изменять сторону высадки отряда после открытия лэптопа.",
	L"Нельзя изменить имя команды после открытия лэптопа.",
	L"Случ. бойцы: ",	//Random Mercs: 
	L"Да",			//Y
	L"Сложность:",		//Difficulty:
	L"Версия сервера:",	//Server Version:
};

STR16 gzMPSScreenText[] =
{
	L"Доска счёта",		//Scoreboard
	L"Продолжить",		//Continue
	L"Отмена",		//Cancel
	L"Игрок",		//Player
	L"Убито",		//Kills
	L"Погибло",		//Deaths
	L"Королевская армия",	//Queen's Army
	L"Выстрелов",		//Hits
	L"Промахи",		//Misses
	L"Меткость",		//Accuracy
	L"Нанесённый урон",	//Damage Dealt
	L"Полученный урон",	//Damage Taken
	L"Дождитесь, пожалуйста, пока сервер нажмёт кнопку 'Продолжить'."
};

STR16 gzMPCScreenText[] =
{
	L"Отмена",						//Cancel
	L"Подключаюсь к серверу...",				//Connecting to Server
	L"Получаю настройки от сервера...",			//Getting Server Settings
	L"Скачиваю выбранные файлы...",				//Downloading custom files
	L"Нажмите 'ESC' для отмены или 'Y' чтобы войти в чат.",	//Press 'ESC' to cancel or 'Y' to chat
	L"Нажмите 'ESC' для отмены",				//Press 'ESC' to cancel
	L"Выполнено."						//Ready
};

STR16 gzMPChatToggleText[] =
{
	L"Отправть всем",
	L"Отправть только союзникам",
};

STR16 gzMPChatboxText[] =
{
	L"Чат сетевой игры Jagged Alliance 2 v1.13",
	L"Заметка: нажмите |В|В|О|Д для отправки сообщения, |К|Л|Ю|Ч для выхода из чата.",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"На следующей странице вам нужно выбрать профессиональные навыки в соответствии со специализацией вашего наёмника. Вы можете выбрать не более двух разных навыка или один и владеть им в совершенстве.",
	L"Можно выбрать всего один навык или вообще остаться без него. Тогда вам будут даны дополнительные баллы для улучшения некоторых параметров. Внимание: навыки электроника, стрельба с двух рук и маскировка не могут быть экспертными.",
	// For new major/minor traits
	L"Следующий этап - выбор навыков, которые определят специализацию вашего наёмника. На первой странице можно выбрать до %d основных навыков, которые определят роль бойца в отряде. На второй - дополнительные навыки, подчеркивающие личные качества бойца.", 
	L"Всего можно взять не более %d навыков. Так, если Вы не выбрали основной навык, то можно взять %d дополнительных. Если же вы выбрали оба основных навыка (или один улучшенный), то будет доступен лишь %d дополнительный...",  
};

STR16 sgAttributeSelectionText[] = 
{
	L"Откорректируйте свои физические параметры согласно вашим истинным способностям. И не стоит их завышать.",
	L"I.M.P.: Параметры и умения.",	//I.M.P. Attributes and skills review.
	L"Бонус:",	//Bonus Pts.
	L"Ваш уровень",	//Starting Level
	// New strings for new traits
	L"На следующей странице укажите свои физические параметры и умения. \"Физические параметры\" - это здоровье, ловкость, проворность, сила и мудрость. Они не могут быть ниже %d.", 
	L"Оставшиеся \"умения\", в отличие от физических параметров, могут быть установлены в ноль, что означает абсолютную некомпетентность в данных областях.", 
	L"Изначально все параметры установлены на минимум. Заметьте, что минимум для некоторых параметров определяется выбранными навыками, и вы не можете понизить их значение.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"I.M.P.: Анализ личных качеств",	//I.M.P. Character Analysis
	L"Следующий шаг - анализ ваших личных качеств. На первой странице вам на выбор будет предложен список черт характера. Уверены, что вам могут быть свойственны несколько из указанных черт, но выбрать нужно лишь одну. Выберите лишь самую ярко выраженную вашу черту характера.",
	L"На второй странице вам будет предложен список недостатков, которые, возможно, есть у вас. Если найдёте свой недостаток в списке, отметьте его. Будьте предельно честны при ответах, очень важно предоставить вашим потенциальным работодателям достоверную информацию о вас.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Адекватный",	//Normal
	L"Общительный",	//Friendly
	L"Одиночка",	//Loner
	L"Оптимист",	//Optimist
	L"Пессимист",	//Pessimist
	L"Агрессивный",	//Aggressive
	L"Высокомерный",	//Arrogant
	L"Крутой",	//Big Shot
	L"Мудак",	//Asshole
	L"Трус",	//Coward
	L"I.M.P.: Жизненная позиция",	//I.M.P. Attitudes
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Обычный",	//Neutral
	L"Общительный",	//Sociable
	L"Одиночка",	//Loner
	L"Оптимист",	//Optimist
	L"Самоуверенный",	//Assertive
	L"Мозговитый",	//Intellectual
	L"Простофиля",	//Primitive
	L"Агрессивный",	//Aggressive
	L"Невозмутимый",	//Phlegmatic
	L"Бесстрашный",	//Dauntless
	L"Миролюбивый",	//Pacifist
	L"Злобный",	//Malicious
	L"Хвастун",	//Show-off
	L"I.M.P.: Личностные качества",	//I.M.P. Character Traits
};

STR16 gzIMPColorChoosingText[] = 
{
	L"I.M.P.: Расцветка и телосложение",
	L"I.M.P.: Расцветка",
	L"Выберите соответствующие цвета вашей кожи, волос и одежды, а так же укажите ваше телосложение.",
	L"Выберите соответствующие цвета вашей кожи, волос и одежды.",
	L"Отметьте здесь чтобы ваш персонаж \nдержал автомат одной рукой.",
	L"\n(Важно: вам понадобится прилично сил для этого.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Цвет волос",	//Hair Color
	L"Цвет кожи",	//Skin Color
	L"Цвет майки",	//Shirt Color
	L"Цвет штанов",	//Pants Color
	L"Нормальное телосложение",	//Normal Body
	L"Мускулистое телосложение",	//Big Body
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Идеален",	//No Disability
	L"Непереносимость жары",		//Heat Intolerant
	L"Нервный",		//Nervous
	L"Клаустрафоб",	//Claustrophobic
	L"Не умеющий плавать",	//Nonswimmer
	L"Боязнь насекомых",	//Fear of Insects
	L"Забывчивый",		//Forgetful
	L"Психопат",		//Psychotic
	L"Deaf",
	L"Shortsighted",
	L"I.M.P.: Недостатки",	//I.M.P. Disabilities
};

STR16 gzIMPDisabilityTraitEmailTextDeaf[] =// TODO.Translate
{
	L"We bet you're glad this isn't voicemail.",
	L"You've either visited to many discos in your teens, or were to close a massive artillery bombardment. Or just old. Either way, your team better learn sign language.",
};

STR16 gzIMPDisabilityTraitEmailTextShortSighted[] =
{
	L"You'll be screwed if you ever lose your glasses.",
	L"That happens when you spend your days in front of glowing rectangles. You should have eaten more carrots. Ever seen a rabbit with glasses? Figures.",
};

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s не хватает Силы чтобы выполнить это действие.",
	L"%s не хватает Ловкости чтобы выполнить это действие.",
	L"%s не хватает Проворности чтобы выполнить это действие.",
	L"%s не хватает Здоровья чтобы выполнить это действие..",
	L"%s не хватает Мудрости чтобы выполнить это действие.",
	L"%s не хватает Меткости чтобы выполнить это действие.",
	// 6 - 10
	L"%s не достаточно развит Медицинский навык, чтобы выполнить это действие.",
	L"%s не достаточно развит навык Механики, чтобы выполнить это действие.",
	L"%s не достаточно развито Лидерство, чтобы выполнить это действие.",
	L"%s не достаточно развит навык Взрывчатки, чтобы выполнить это действие.",
	L"%s не достаточно Опыта, чтобы выполнить это действие.",
	// 11 - 15
	L"У %s слишком плохой Боевой дух, чтобы выполнить это действие",
	L"%s слишком устал(а), чтобы выполнить это действие.",
	L"В городе %s вам пока не доверяют. Местные отказываются выполнить этот приказ.",
	L"Слишком много людей уже работают в %s.",
	L"Слишком много людей уже выполняют эту задачу в %s.",
	// 16 - 20
	L"%s не может найти вещи, которые нуждаются в ремонте.",
	L"%s потерял(а) часть %s пока работал в секторе %s!",
	L"%s потерял(а) часть %s пока работал над %s в %s!",
	L"%s получил(а) травму пока работал(а) в секторе %s, и требует незамедлительной медицинской помощи!",
	L"%s получил(а) травму пока работал(а) над %s в %s, и требует незамедлительной медицинской помощи!",
	// 21 - 25
	L"%s получил(а) травму пока работал(а) в секторе %s. Травма незначительная.",
	L"%s получил(а) травму пока работал(а) над %s в %s. Травма незначительная.",
	L"Жители города %s расстроены тем, что %s пребывает в их городе.",
	L"Жители города %s расстроены работой %s в %s.",
	L"%s в секторе %s своими действиями понизил репутацию во всём регионе!",
	// 26 - 30
	L"%s работая над %s в %s привёл(а) к понижению репутации во всём регионе!",
	L"%s пьян(а).",
	L"%s заболел(а) в секторе %s, и вынужден(а) отложить текущую задачу.",
	L"%s заболел(а) и не может продолжить работу над %s в %s.",
	L"%s получил(а) травму в секторе %s.",
	// 31 - 35
	L"%s получил(а) серьёзную травму в секторе %s.",


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Сила",	//Strength
	L"Проворность",	//Agility
	L"Ловкость",	//Dexterity
	L"Интеллект",	//Wisdom
	L"Здоровье",	//Health
	L"Меткость",	//Marksmanship
	// 5-10
	L"Лидерство",	//Leadership
	L"Механика",	//Mechanical skill
	L"Медицина",	//Medical skill
	L"Взрывчатка",	//Explosives skill
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"AMBIENT",
	L"Штат",	//Staff
	L"Eat",// TODO.Translate
	L"Отдых",
	L"Ремонт вещей",
	L"Ремонт %s", // Vehicle name inserted here
	L"Ремонт робота",
	// 6-10
	L"Доктор",
	L"Пациент",
	L"Тренинг Силы",
	L"Тренинг Ловкости",
	L"Тренинг Проворности",
	L"Тренинг Здоровья",
	// 11-15
	L"Тренинг Меткости",
	L"Тренинг Медицины",
	L"Тренинг Механики",
	L"Тренинг Лидерства",
	L"Тренинг Взрывчатки",
	// 16-20
	L"Ученик на Силу",
	L"Ученик на Ловкость",
	L"Ученик на Проворность",
	L"Ученик на Здоровье",
	L"Ученик на Меткость",
	// 21-25
	L"Ученик на Медицину",
	L"Ученик на Механику",
	L"Ученик на Лидерство",
	L"Ученик на Взрывчатку",
	L"Тренер на Силу",
	// 26-30
	L"Тренер на Ловкость",
	L"Тренер на Проворность",
	L"Тренер на Здоровье",
	L"Тренер на Меткость",
	L"Тренер на Медицину",
	// 30-35
	L"Тренер на Механику",
	L"Тренер на Лидерство",
	L"Тренер на Взрывчатку",
	L"Interrogate Prisoners",	// added by Flugente	TODO.Translate
};
STR16 Additional113Text[]=
{
	L"Для запуска Jagged Alliance 2 v1.13 в оконном режиме требуется установить 16-битное качество цветопередачи экрана",

	// WANNE: Savegame slots validation against INI file 	// TODO.Translate
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
	L"Отведай ка гостинца!",
	L"Поздоровайся с моим дружком!",
	L"Иди и получи!",
	L"Ты мой!",
	L"Сдохни!",
	L"Обосрался, говнюк?",
	L"Будет больно!",
	L"Давай, ублюдок!",
	L"Давай! Не весь же день тягаться!",
	L"Иди к папочке!",
	L"Закопаю моментом!",
	L"Домой поедешь в деревянном костюме, неудачник!",
	L"Эй, сыграем?",
	L"Сидел бы дома, мудила!",
	L"С-сука!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"Будет, будет... Шашлык из тебя будет!",
	L"Держи подарочек!",
	L"Бах!",
	L"Улыбочку!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Лови!",
	L"Держи!",
	L"Бум-бах, ой-ой-ой! Умирает зайчик мой!",
	L"Это тебе.",
	L"Муа-ха-ха!",
	L"Лови, свинтус!",
	L"Обожаю этот момент.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"Твой скальп мой, лошара!",
	L"Иди к папочке.",
	L"Сейчас посмотрим на твои кишечки!",
	L"Порву, как Тузик грелку!",
	L"Мясо!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"Кажется, мы в дерьме...",
	L"Мне говорили вступать в армию, а не в это дерьмо!",
	L"С меня хватит!",
	L"О мой Бог!",
	L"Нам не доплачивают за это дерьмо, валим отсюда...",
	L"Мамочка!",
	L"Я вернусь! И нас будут тысячи!",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"Я тебя слышу!",
	L"Кто здесь?",
	L"Что это было?",
	L"Эй! Какого...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Они здесь!",
	L"Сейчас начнётся веселье!",
	L"Я надеялся, что этого никогда не случится...",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"А-а-г-р-р!",
	L"А-а-а!",
	L"Как же... больно!",
	L"Твою мать!",
	L"Ты пожалеешь... у-м-хх... об этом.",
	L"Что за!..",
	L"Теперь ты меня... разозлил.",

};

STR16 sEnemyTauntsNoticedMerc[]=
{
	L"Da'ffff...!",	// TODO.Translate
	L"Oh my God!",
	L"Holy crap!",
	L"Enemy!!!",
	L"Alert! Alert!",
	L"There is one!",
	L"Attack!",

};

//////////////////////////////////////////////////////
// HEADROCK HAM 4: Begin new UDB texts and tooltips
//////////////////////////////////////////////////////
STR16 gzItemDescTabButtonText[] =
{
	L"Информация",
	L"Параметры",
	L"Дополнительно",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Инфо.",
	L"Пар.",
	L"Доп.",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Основное",
	L"Дополнительное",
	L"Затраты ОД",
	L"Стрельба очередью",
};

STR16 gzItemDescGenIndexes[] =
{
	L"Парам.",
	L"0",
	L"+/-",
	L"=",
};

STR16 gzUDBButtonTooltipText[]=
{
	L"|И|н|ф|о|р|м|а|ц|и|о|н|н|а|я |ч|а|с|т|ь:\n \nЗдесь вы сможете ознакомиться \nс общим описанием предмета.",
	L"|П|а|р|а|м|е|т|р|ы:\n \nЗдесь вы сможете ознакомиться \nс индивидуальными свойствами и параметрами предмета.\n \nWeapons: Click again to see second page.",	// TODO.Translate
	L"|Д|о|п|о|л|н|и|т|е|л|ь|н|а|я| |и|н|ф|о|р|м|а|ц|и|я:\n \nЗдесь вы сможете ознакомиться \nс бонусами, дающимися данным предметом.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|О|с|н|о|в|н|ы|е |п|а|р|а|м|е|т|р|ы:\n \nСвойства и данные этого предмета\n(Оружие / Броня / и другое).",
	L"|Д|о|п|о|л|н|и|т|е|л|ь|н|ы|е| |п|а|р|а|м|е|т|р|ы:\n \nДополнительные свойства \nи/или возможные вторичные характеристики.",
	L"|З|а|т|р|а|т|ы| |О|Д:\n \nКоличество Очков Действия необходимых \nна стрельбу и другие действия с оружием.",
	L"|С|т|р|е|л|ь|б|а| |о|ч|е|р|е|д|ь|ю| |- |п|а|р|а|м|е|т|р|ы|:\n \nПараметры данного оружия, \nкасающиеся стрельбы очередью.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|С|и|м|в|о|л|ь|н|о|е| |о|б|о|з|н|а|ч|е|н|и|е| |п|а|р|а|м|е|т|р|о|в\n \nУкажите курсором на символ \nчтобы увидеть что он значит.",
	L"|С|т|а|н|д|а|р|т|н|о|е |з|н|а|ч|е|н|и|е\n \nСтандартное значение праметров предмета \n(без штрафов и бонусов навески и аммуниции).",
	L"|Б|о|н|у|с|ы| |н|а|в|е|с|к|и\n \nБонусы или штрафы, обусловленные \nнавеской, аммуницией или повреждениями вещи.",
	L"|С|у|м|м|а|р|н|о|е| |з|н|а|ч|е|н|и|е\n \nСуммарное значение параметров предмета, \nучитывая все бонусы/штрафы навески и аммуниции.",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"Символьное обозначение параметров \n(укажите курсором на символ \nчтобы увидеть что он значит ).",
	L"Бонус/штраф если |с|т|о|и|ш|ь.",
	L"Бонус/штраф при |г|у|с|и|н|о|м| |ш|а|г|е.",
	L"Бонус/штраф если |п|о|л|з|ё|ш|ь.",
	L"Даден бонус/штраф",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|М|е|т|к|о|с|т|ь",
	L"|У|р|о|н", 
	L"|Р|а|с|с|т|о|я|н|и|е",
	L"|H|a|n|d|l|i|n|g |D|i|f|f|i|c|u|l|t|y",	// TODO.Translate
	L"|Д|о|с|т|у|п|н|ы|е |у|р|о|в|н|и |п|р|и|ц|е|л|и|в|а|н|и|я", 
	L"|К|о|э|ф|ф|и|ц|и|е|н|т |у|в|е|л|и|ч|е|н|и|я |п|р|и|ц|е|л|а",   
	L"|К|о|э|ф|ф|и|ц|и|е|н|т |п|р|о|е|к|т|о|р|а", 
	L"|С|к|р|ы|т|а|я |в|с|р|ы|ш|к|а |в|ы|с|т|р|е|л|а", 
	L"|Г|р|о|м|к|о|с|т|ь",
	L"|Н|а|д|ё|ж|н|о|с|т|ь",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
	L"|М|и|н|. |Р|а|с|с|т|о|я|н|и|е |д|л|я |б|о|н|у|с|н|о|г|о |п|р|и|ц|е|л|и|в|а|н|и|я", 
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|о|п|а|д|а|н|и|я", 	
	L"|О|Д |н|а |в|с|к|и|д|к|у", 
	L"|О|Д |н|а |в|ы|с|т|р|е|л",
	L"|О|Д |н|а |с|т|р|е|л|ь|б|у |о|ч|е|р|е|д|ю", 
	L"|О|Д |н|а |а|в|т|о.|с|т|р|е|л|ь|б|у",
	L"|О|Д |н|а |п|е|р|е|з|а|р|я|д|к|у", 
	L"|О|Д |н|а |д|о|с|ы|л|а|н|и|я |п|а|т|р|о|н|ы",
	L"|L|a|t|e|r|a|l |R|e|c|o|i|l",	// No longer used
	L"|T|o|t|a|l |R|e|c|o|i|l",		// TODO.Translate
	L"|Р|а|с|х|о|д |п|а|т|р|о|н |н|а |к|а|ж|д|ы|е |5 |О|Д",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines whether bullets fired by\nthis gun will stray far from where\nit is pointed.\n \nScale: 0-100.\nHigher is better.",
	L"\n \nDetermines the average amount of damage done\nby bullets fired from this weapon, before\ntaking into account armor or armor-penetration.\n \nHigher is better.",
	L"\n \nThe maximum distance (in tiles) that\nbullets fired from this gun will travel\nbefore they begin dropping towards the\nground.\n \nHigher is better.",
	L"\n \nDetermines the difficulty of holding and firing\nthis gun.\n \nHigher Handling Difficulty results in lower\nchance-to-hit - when aimed and especially when\nunaimed.\n \nLower is better.",	// TODO.Translate
	L"\n \nThis is the number of Extra Aiming\nLevels you can add when aiming this gun.\n \nThe FEWER aiming levels are allowed, the MORE\nbonus each aiming level gives you. Therefore,\nhaving FEWER levels makes the gun faster to aim,\nwithout making it any less accurate.\n \nLower is better.",
	L"\n \nWhen greater than 1.0, will proportionally reduce\naiming errors at a distance.\n \nRemember that high scope magnification is detrimental\nwhen the target is too close!\n \nA value of 1.0 means no scope is installed.",
	L"\n \nProportionally reduces aiming errors at a distance.\n \nThis effect works up to a given distance,\nthen begins to dissipate and eventually\ndisappears at sufficient range.\n \nHigher is better.",
	L"\n \nWhen this property is in effect, the weapon\nproduces no visible flash when firing.\n \nEnemies will not be able to spot you\njust by your muzzle flash (but they\nmight still HEAR you).",
	L"\n \nWhen firing this weapon, Loudness is the\ndistance (in tiles) that the sound of\ngunfire will travel.\n \nEnemies within this distance will probably\nhear the shot.\n \nLower is better.",
	L"\n \nDetermines how quickly this weapon will degrade\nwith use.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only Technicians and special\nNPCs can repair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
	L"\n \nThe minimum range at which a scope can provide it's aimBonus.",
	L"\n \nTo hit modifier granted by laser sights.",	
	L"\n \nThe number of APs required to bring this\nweapon up to firing stance.\n \nOnce the weapon is raised, you may fire repeatedly\nwithout paying this cost again.\n \nA weapon is automatically 'Unreadied' if its\nwielder performs any action other than\nfiring or turning.\n \nLower is better.",
	L"\n \nThe number of APs required to perform\na single attack with this weapon.\n \nFor guns, this is the cost of firing\na single shot without extra aiming.\n \nIf this icon is greyed-out, single-shots\n are not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\na burst.\n \nThe number of bullets fired in each burst is\ndetermined by the weapon itself, and indicated\nby the number of bullets shown on this icon.\n \nIf this icon is greyed-out, burst fire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\nan Autofire Volley of three bullets.\n \nIf you wish to fire more than 3 bullets,\nyou will need to pay extra APs.\n \nIf this icon is greyed-out, autofire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to reload\nthis weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to rechamber this weapon\nbetween each and every shot fired.\n \nLower is better.",
	L"\n \nThe distance this weapon's muzzle will shift\nhorizontally between each and every bullet in a\nburst or autofire volley.\n \nPositive numbers indicate shifting to the right.\nNegative numbers indicate shifting to the left.\n \nCloser to 0 is better.", // No longer used
	L"\n \nThe total distance this weapon's muzzle will shift\nbetween each and every bullet in a burst or\nautofire volley, if no Counter Force is applied.\n \nLower is better.", // HEADROCK HAM 5: Altered to reflect unified number.	// TODO.Translate
	L"\n \nIndicates the number of bullets that will be added\nto an autofire volley for every extra 5 APs\nyou spend.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|З|а|щ|и|т|н|а|я |ц|е|н|н|о|с|т|ь",
	L"|П|о|к|р|ы|в|а|е|м|о|с|т|ь",
	L"|С|к|о|р|о|с|т|ь |у|х|у|д|ш|е|н|и|я",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nThis primary armor property defines how much\ndamage the armor will absorb from any attack.\n \nRemember that armor-piercing attacks and\nvarious randomal factors may alter the\nfinal damage reduction.\n \nHigher is better.",
	L"\n \nDetermines how much of the protected\nbodypart is covered by the armor.\n \nIf coverage is below 100%, attacks have\na certain chance of bypassing the armor\ncompletely, causing maximum damage\nto the protected bodypart.\n \nHigher is better.",
	L"\n \nIndicates how quickly this armor's condition\ndrops when it is struck, proportional to\nthe damage caused by the attack.\n \nLower is better.",
	L"\n \nDetermines how difficult it is to repair\nthis armor and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only Technicians and special\nNPCs can repair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair\nthis armor and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|П|р|о|н|з|и|т|е|л|ь|н|о|с|т|ь |ч|е|р|е|з |б|р|о|н|ю",
	L"|П|а|д|е|н|и|е |п|а|т|р|о|н|ы",
	L"|П|р|е|д|в|а|р|и|т|е|л|ь|н|о|е |в|л|и|я|н|и|е |в|з|р|ы|в|а",
	L"|T|e|m|p|e|r|a|t|u|r|e |M|o|d|i|f|i|c|a|t|i|o|n",		// TODO.Translate
	L"|P|o|i|s|o|n |P|e|r|c|e|n|t|a|g|e",	// TODO.Translate
	L"|D|i|r|t |M|o|d|i|f|i|c|a|t|i|o|n",	// TODO.Translate
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nThis is the bullet's ability to penetrate\na target's armor.\n \nWhen below 1.0, the bullet proportionally\nreduces the Protection value of any\narmor it hits.\n \nWhen above 1.0, the bullet increases the\nprotection value of the armor instead.\n \nLower is better.",	// TODO.Translate
	L"\n \nDetermines a proportional increase of damage\npotential once the bullet gets through the\ntarget's armor and hits the bodypart behind it.\n \nWhen above 1.0, the bullet's damage\nincreases after penetrating the armor.\n \nWhen below 1.0, the bullet's damage\npotential decreases after passing through armor.\n \nHigher is better.",
	L"\n \nA multiplier to the bullet's damage potential\nthat is applied immediately before hitting the\ntarget.\n \nValues above 1.0 indicate an increase in damage,\nvalues below 1.0 indicate a decrease.\n \nHigher is better.",
	L"\n \nAdditional heat generated by this ammunition.\n \nLower is better.",		// TODO.Translate
	L"\n \nDetermines what percentage of a\nbullet's damage will be poisonous.",	// TODO.Translate
	L"\n \nAdditional dirt generated by this ammunition.\n \nLower is better.",		// TODO.Translate
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|У|р|о|н",
	L"|У|р|о|н |о|г|л|у|ш|е|н|и|я",
	L"|E|x|p|l|o|d|e|s |o|n| |I|m|p|a|c|t",		// HEADROCK HAM 5	// TODO.Translate
	L"|Р|а|д|и|у|с |в|з|р|ы|в|а",
	L"|Р|а|д|и|у|с о|г|л|у|ш|и|т|е|л|ь|н|о|г|о |в|з|р|ы|в|а",
	L"|Р|а|д|и|у|с |в|з|р|ы|в|н|о|г|о |ш|у|м|а", 
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |с|л|е|з|а|т|о|ч|и|в|о|г|о |г|а|з|а",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |г|о|р|ч|и|ч|н|о|г|о |г|а|з|а",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |в|с|п|ы|ш|к|и",
	L"|Н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |д|ы|м|а",
	L"|н|а|ч|а|л|ь|н|ы|й |р|а|д|и|у|с |з|а|ж|и|г|а|е|м|о|с|т|и",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |с|л|е|з|о|т|о|ч|и|в|о|г|о |г|а|з|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |г|о|р|ч|и|ч|н|о|г|о |г|а|з|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |с|в|е|т|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |г|ы|м|а",
	L"|К|о|н|е|ч|н|ы|й |р|а|д|и|у|с |з|а|ж|и|г|а|е|м|о|с|т|и",
	L"|Д|л|и|т|е|л|ь|н|о|с|т|ь |э|ф|ф|е|к|т|а",
	// HEADROCK HAM 5: Fragmentation		// TODO.Translate
	L"|N|u|m|b|e|r |o|f |F|r|a|g|m|e|n|t|s",
	L"|D|a|m|a|g|e |p|e|r |F|r|a|g|m|e|n|t",
	L"|F|r|a|g|m|e|n|t |R|a|n|g|e",
	// HEADROCK HAM 5: End Fragmentations
	L"|Г|р|о|м|к|о|с|т|ь",
	L"|Н|е|с|т|а|б|и|л|ь|н|о|с|т|ь",
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nThe amount of damage caused by this explosive.\n \nNote that blast-type explosives deliver this damage\nonly once (when they go off), while prolonged effect\nexplosives deliver this amount of damage every turn until the\neffect dissipates.\n \nHigher is better.",
	L"\n \nThe amount of non-lethal (stun) damage caused\nby this explosive.\n \nNote that blast-type explosives deliver their damage\nonly once (when they go off), while prolonged effect\nexplosives deliver this amount of stun damage every\nturn until the effect dissipates.\n \nHigher is better.",
	L"\n \nThis explosive will not bounce around -\nit will explode as soon as it hits any obstacle.",	// HEADROCK HAM 5	// TODO.Translate
	L"\n \nThis is the radius of the explosive blast caused by\nthis explosive item.\n \nTargets will suffer less damage the further they are\nfrom the center of the explosion.\n \nHigher is better.",
	L"\n \nThis is the radius of the stun-blast caused by\nthis explosive item.\n \nTargets will suffer less damage the further they are\nfrom the center of the blast.\n \nHigher is better.",
	L"\n \nThis is the distance that the noise from this\ntrap will travel. Soldiers within this distance\nare likely to hear the noise and be alerted.\n \nHigher is better.",
	L"\n \nThis is the starting radius of the tear-gas\nreleased by this explosive item.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn,\nunless wearing a gas mask.\n \nAlso note the end radius and duration\nof the effect (displayed below).\n \nHigher is better.",
	L"\n \nThis is the starting radius of the mustard-gas\nreleased by this explosive item.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn,\nunless wearing a gas mask.\n \nAlso note the end radius and duration\nof the effect (displayed below).\n \nHigher is better.",
	L"\n \nThis is the starting radius of the light\nemitted by this explosive item.\n \nTiles close to the center of the effect will become\nvery bright, while tiles nearer the edge\nwill only be a little brighter than normal.\n \nAlso note the end radius and duration\nof the effect (displayed below).\n \nAlso remember that unlike other explosives with\ntimed effects, the light effect gets SMALLER\nover time, until it disappears.\n \nHigher is better.",
	L"\n \nThis is the starting radius of the smoke\nreleased by this explosive item.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn\n(if any), unless wearing a gas mask. More importantly,\nanyone inside the cloud becomes extremely difficult to spot,\nand also loses a large chunk of sight-range themselves.\n \nAlso note the end radius and duration\nof the effect (displayed below).\n \nHigher is better.",
	L"\n \nThis is the starting radius of the flames\ncaused by this explosive item.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn.\n \nAlso note the end radius and duration of the effect\n(displayed below).\n \nHigher is better.",
	L"\n \nThis is the final radius of the tear-gas released\nby this explosive item before it dissipates.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn,\nunless wearing a gas mask.\n \nAlso note the start radius and duration\nof the effect.\n \nHigher is better.",
	L"\n \nThis is the final radius of the mustard-gas released\nby this explosive item before it dissipates.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn,\nunless wearing a gas mask.\n \nAlso note the start radius and duration\nof the effect.\n \nHigher is better.",
	L"\n \nThis is the final radius of the light emitted\nby this explosive item before it dissipates.\n \nTiles close to the center of the effect will become\nvery bright, while tiles nearer the edge\nwill only be a little brighter than normal.\n \nAlso note the start radius and duration\nof the effect.\n \nAlso remember that unlike other explosives with\ntimed effects, the light effect gets SMALLER\nover time, until it disappears.\n \nHigher is better.",
	L"\n \nThis is the final radius of the smoke released\nby this explosive item before it dissipates.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn\n(if any), unless wearing a gas mask. More importantly,\nanyone inside the cloud becomes extremely difficult to spot,\nand also loses a large chunk of sight-range themselves.\n \nAlso note the start radius and duration\nof the effect.\n \nHigher is better.",
	L"\n \nThis is the final radius of the flames caused\nby this explosive item before they dissipate.\n \nEnemies caught within the radius will suffer\nthe listed damage and stun-damage each turn.\n \nAlso note the start radius and duration of the effect.\n \nHigher is better.",
	L"\n \nThis is the duration of the explosive effect.\n \nEach turn, the radius of the effect will grow by\none tile in every direction, until reaching\nthe listed End Radius.\n \nOnce the duration has been reached, the effect\ndissipates completely.\n \nNote that light-type explosives become SMALLER\nover time, unlike other effects.\n \nHigher is better.",
	// HEADROCK HAM 5: Fragmentation		// TODO.Translate
	L"\n \nThis is the number of fragments that will\nbe ejected from the explosion.\n \nFragments are similar to bullets, and may hit\nanyone who's close enough to the explosion.\n \nHigher is better.",
	L"\n \nThe potential amount of damage caused by each\nfragment ejected from the explosion.\n \nHigher is better.",
	L"\n \nThis is the average range to which fragments\nfrom this explosion will fly.\n \nSome fragments may end up further away, or fail\nto cover the distance altogether.\n \nHigher is better.",
	// HEADROCK HAM 5: End Fragmentations
	L"\n \nThis is the distance (in Tiles) within which\nsoldiers and mercs will hear the explosion when\nit goes off.\n \nEnemies hearing the explosion will be alerted to your\npresence.\n \nLower is better.",
	L"\n \nThis value represents a chance (out of 100) for this\nexplosive to spontaneously explode whenever it is damaged\n(for instance, when other explosions go off nearby).\n \nCarrying highly-volatile explosives into combat\nis therefore extremely risky and should be avoided.\n \nScale: 0-100.\nLower is better.",
	L"\n \nDetermines how difficult it is to repair these explosives.\n \ngreen = Anybody can repair them.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16 szUDBGenCommonStatsTooltipText[]=
{
	L"|Л|ё|г|к|о|с|т|ь |п|о|ч|и|н|к|и",
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines how difficult it is to repair this item.\n \ngreen = Anybody can repair it.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|Т|р|а|с|с|и|р|у|ю|щ|и|е|с|я |п|а|т|р|о|н|ы",
	L"|П|р|о|т|и|в|о|т|а|н|к|о|в|ы|е |п|а|т|р|о|н|ы",
	L"|П|р|о|х|о|д|и|т |ч|е|р|е|з |б|р|о|н|ю",
	L"|К|и|с|л|о|т|н|ы|е |п|а|т|р|о|н|ы",
	L"|Р|а|з|р|у|ш|а|ю|щ|и|е |з|а|м|к|и |п|а|т|р|о|н|ы",
	L"|У|с|т|о|й|ч|и|в|ы|й |к|о |в|з|р|ы|в|а|м",
	L"|В|о|д|о|н|е|п|р|о|н|и|ц|а|е|м|ы|й",
	L"|Э|л|е|к|т|р|о|н|н|ы|й",
	L"|П|р|о|т|и|в|о|г|а|з",
	L"|Н|у|ж|д|а|е|т|с|я |в |б|а|т|а|р|е|й|к|а|х",
	L"|М|о|ж|е|т |в|з|л|а|м|ы|в|а|т|ь |з|а|м|к|и",
	L"|М|о|ж|е|т |р|е|з|а|т|ь |п|р|о|в|о|л|о|к|у",
	L"|М|о|ж|е|т |р|а|з|р|у|ш|а|т|ь |з|а|м|к|и",
	L"|М|е|т|а|л|л|о|и|с|к|а|т|е|л|ь",
	L"|У|д|а|л|ё|н|н|ы|й |с|п|у|с|к|а|в|о|й |к|р|ю|ч|о|к",
	L"|Д|и|с|т|а|н|ц|и|о|н|н|ы|й |д|е|т|а|н|а|т|о|р",
	L"|Д|е|т|о|н|а|т|о|р|-|т|а|й|м|е|р",
	L"|С|о|д|е|р|ж|и|т |т|о|п|л|и|в|о",
	L"|Н|а|б|о|р |и|н|с|т|р|у|м|е|н|т|о|в",
	L"|Т|е|р|м|а|л|ь|н|а|я |о|п|т|и|к|а",
	L"|Р|е|н|т|г|е|н|о|в|ы|й |п|р|и|б|о|р",
	L"|С|о|д|е|р|ж|т|ь |п|и|т|ь|е|в|у|ю |в|о|д|у",
	L"|С|о|д|е|р|ж|и|т |а|л|к|о|г|о|л|ь",
	L"|А|п|т|е|ч к|а |п|е|р|в|о|й |п|о|м|о|щ|и",
	L"|М|е|д|и|ц|и|нс|к|и|й |н|а|б|о|р",
	L"|Б|о|м|б|а |д|л|я |з|а|м|к|о|в",
	L"|D|r|i|n|k",// TODO.Translate
	L"|M|e|a|l",
	L"|A|m|m|o |B|e|l|t",
	L"|A|m|m|o |V|e|s|t",
	L"|D|e|f|u|s|a|l |K|i|t",	// TODO.Translate
	L"|C|o|v|e|r|t |I|t|e|m",	// TODO.Translate
	L"|C|a|n|n|o|t |b|e |d|a|m|a|g|e|d",
	L"|M|a|d|e |o|f |M|e|t|a|l",
	L"|S|i|n|k|s",
	L"|T|w|o|-|H|a|n|d|e|d",
	L"|B|l|o|c|k|s |I|r|o|n |S|i|g|h|t|s",
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \nThis ammo creates a tracer effect when fired in\nfull-auto or burst mode.\n \nTracer fire helps keep the volley accurate\nand thus deadly despite the gun's recoil.\n \nAlso, tracer bullets create paths of light that\ncan reveal a target in darkness. However, they\nalso reveal the shooter to the enemy!\n \nTracer Bullets automatically disable any\nMuzzle Flash Suppression items installed on the\nsame weapon.",
	L"\n \nThis ammo can damage the armor on a tank.\n \nAmmo WITHOUT this property will do no damage\nat all to tanks.\n \nEven with this property, remember that most guns\ndon't cause enough damage anyway, so don't\nexpect too much.",
	L"\n \nThis ammo ignores armor completely.\n \nWhen fired at an armored target, it will behave\nas though the target is completely unarmored,\nand thus transfer all its damage potential to the target!",
	L"\n \nWhen this ammo strikes the armor on a target,\nit will cause that armor to degrade rapidly.\n \nThis can potentially strip a target of its\narmor!",
	L"\n \nThis type of ammo is exceptional at breaking locks.\n \nFire it directly at a locked door or container\nto cause massive damage to the lock.",
	L"\n \nThis armor is three times more resistant\nagainst explosives than it should be, given\nits Protection value.\n \nWhen an explosion hits the armor, its Protection\nvalue is considered three times higher than\nthe listed value.",
	L"\n \nThis item is impervious to water. It does not\nreceive damage from being submerged.\n \nItems WITHOUT this property will gradually deteriorate\nif the person carrying them goes for a swim.",
	L"\n \nThis item is electronic in nature, and contains\ncomplex circuitry.\n \nElectronic items are inherently more difficult\nto repair, at least without the ELECTRONICS skill.",
	L"\n \nWhen this item is worn on a character's face,\nit will protect them from all sorts of noxious gasses.\n \nNote that some gases are corrosive, and might eat\nright through the mask...",
	L"\n \nThis item requires batteries. Without batteries,\nyou cannot activate its primary abilities.\n \nTo use a set of batteries, attach them to\nthis item as you would a scope to a rifle.",
	L"\n \nThis item can be used to pick open locked\ndoors or containers.\n \nLockpicking is silent, although it requires\nsubstantial mechanical skill to pick anything\nbut the simplest locks. This item modifies\nthe lockpicking chance by ", //JMich_SkillsModifiers: needs to be followed by a number		// TODO.Translate
	L"\n \nThis item can be used to cut through wire fences.\n \nThis allows a character to rapidly move through\nfenced areas, possibly outflanking the enemy!",
	L"\n \nThis item can be used to smash open locked\ndoors or containers.\n \nLock-smashing requires substantial strength,\ngenerates a lot of noise, and can easily\ntire a character out. However, it is a good\nway to get through locks without superior skills or\ncomplicated tools. This item improves \nyour chance by ", //JMich_SkillsModifiers: needs to be followed by a number		// TODO.Translate
	L"\n \nThis item can be used to detect metallic objects\nunder the ground.\n \nNaturally, its primary function is to detect\nmines without the necessary skills to spot them\nwith the naked eye.\n \nMaybe you'll find some buried treasure too.",
	L"\n \nThis item can be used to detonate a bomb\nwhich has been set with a remote detonator.\n \nPlant the bomb first, then use the\nRemote Trigger item to set it off when the\ntime is right.",
	L"\n \nWhen attached to an explosive device and set up\nin the right position, this detonator can be triggered\nby a (separate) remote device.\n \nRemote Detonators are great for setting traps,\nbecause they only go off when you tell them to.\n \nAlso, you have plenty of time to get away!",
	L"\n \nWhen attached to an explosive device and set up\nin the right position, this detonator will count down\nfrom the set amount of time, and explode once the\ntimer expires.\n \nTimer Detonators are cheap and easy to install,\nbut you'll need to time them just right to give\nyourself enough chance to get away!",
	L"\n \nThis item contains gasoline (fuel).\n \nIt might come in handy if you ever\nneed to fill up a gas tank...",
	L"\n \nThis item contains various tools that can\nbe used to repair other items.\n \nA toolkit item is always required when setting\na character to repair duty. This item modifies\nthe effectiveness of repair by ", //JMich_SkillsModifiers: need to be followed by a number		// TODO.Translate
	L"\n \nWhen worn in a face-slot, this item provides\nthe ability to spot enemies through walls,\nthanks to their heat signature.",
	L"\n \nThis powerful device can be used to scan\nfor enemies using X-rays.\n \nIt will reveal all enemies within a certain radius\nfor a short period of time.\n \nKeep away from reproductive organs!",
	L"\n \nThis item contains fresh drinking water.\nUse when thirsty.",
	L"\n \nThis item contains liquor, alcohol, booze,\nwhatever you fancy calling it.\n \nUse with caution. Do not drink and drive.\nMay cause cirrhosis of the liver.",
	L"\n \nThis is a basic field medical kit, containing\nitems required to provide basic medical aid.\n \nIt can be used to bandage wounded characters\nand prevent bleeding.\n \nFor actual healing, use a proper Medical Kit\nand/or plenty of rest.",
	L"\n \nThis is a proper medical kit, which can\nbe used in surgery and other serious medicinal\npurposes.\n \nMedical Kits are always required when setting\na character to Doctoring duty.",
	L"\n \nThis item can be used to blast open locked\ndoors and containers.\n \nExplosives skill is required to avoid\npremature detonation.\n \nBlowing locks is a relatively easy way of quickly\ngetting through locked doors. However,\nit is very loud, and dangerous to most characters.",
	L"\n \nThis item will still your thirst\nif you drink it.",// TODO.Translate
	L"\n \nThis item will still your hunger\nif you eat it.",
	L"\n \nWith this ammo belt you can\nfeed someone else's MG.",
	L"\n \nYou can feed an MG with ammo\nbelts stored in this vest.",
	L"\n \nThis item improves your trap disarm chance by ",		// TODO.Translate
	L"\n \nThis item and everything attached/inside\nit is hidden from curious eyes.",	// TODO.Translate
	L"\n \nThis item cannot be damaged.",
	L"\n \nThis item is made of metal.\nIt takes less damage than other items.",
	L"\n \nThis item sinks when put in water.",
	L"\n \nThis item requires both hands to be used.",
	L"\n \nThis item will block your iron sights\nso you cannot use them.",
};

STR16 szUDBAdvStatsTooltipText[]=
{
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|т|р|е|л|ь|б|ы |с |к|р|ы|ш",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|т|р|е|л|ь|б|ы |с |к|р|ы|ш |в |%",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я |с |к|р|ы|ш",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я |в |%",
	L"|М|о|д|и|ф|и|к|а|т|о|р |д|о|с|т|у|п|н|ы|х |у|р|о|в|н|е|й |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|р|и|ц|е|л|и|в|а|н|и|я |с |ш|а|п|к|о|й",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|у|к|о|я|т|к|и ",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|а|д|е|н|и|я |в|е|щ|е|й",
	L"|М|о|д|и|ф|и|к|а|т|о|р |о|т|с|л|е|ж|к|и |ц|е|л|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |у|р|о|н|а",	
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|у|к|о|п|а|ш|н|о|г|о |у|р|о|н|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|с|с|т|о|я|н|и|я",
	L"|Ф|а|к|т|о|р |у|в|е|л|и|ч|е|н|и|я |с |п|р|и|ц|е|л|о|м",
	L"|Ф|а|к|т|о|р| |п|р|о|е|к|ц|и|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |б|о|к|о|в|о|й |о|т|д|а|ч|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|е|р|т|и|к|а|л|ь|н|о|й |о|т|д|а|ч|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |м|а|к|с|и|м|а|л|ь|н|о|й |к|о|н|т|р|-|с|и|л|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |к|о|н|т|р|-|с|и|л|ы |м|е|т|к|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |ч|а|с|т|о|т|ы |к|о|н|т|р|-|с|и|л|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|с|е|х |О|Д",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |н|а |в|с|к|и|д|к|у",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |о|д|н|о|й |а|т|а|к|и", 
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |о|ч|е|р|е|д|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |а|в|т|о|м|а|т|и|ч|е|с|к|о|й |с|т|р|е|л|ь|б|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |О|Д |п|е|р|е|з|а|р|я|д|к|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |о|б|о|й|м|ы",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |о|ч|е|р|е|д|и",
	L"|С|к|р|ы|т|а|я |в|с|п|ы|ш|к|а |в|ы|с|т|р|е|л|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |г|р|о|м|к|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |р|а|з|м|е|р|а |п|р|е|д|м|е|т|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |н|а|р|ё|ж|н|о|с|т|и",
	L"|Л|е|с|н|о|й |к|а|м|у|ф|л|я|ж",
	L"|Г|о|р|о|д|с|к|о|й |к|а|м|у|ф|л|я|ж",
	L"|П|у|с|т|ы|н|н|ы|й |к|а|м|у|ф|л|я|ж",
	L"|С|н|е|ж|н|ы|й |к|а|м|у|ф|л|я|ж",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|к|р|ы|т|н|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |с|л|ы|ш|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|и|д|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |н|о|ч|н|о|й |в|и|д|и|м|о|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |д|н|е|в|н|о|й |в|и|д|и|м|o|с|т|и",
	L"|М|о|д|и|ф|и|к|а|т|о|р |в|и|д|и|м|о|с|т|и |я|р|к|о|г|о |с|в|е|т|а",
	L"|М|о|д|и|ф|и|к|а|т|о|р |п|е|щ|е|р|н|о|й |в|и|д|и|м|о|с|т|и",
	L"|П|р|и|ц|е|л|ь|н|а|я |в|и|д|и|м|о|с|т|ь",
	L"|М|а|к|с|и|м|а л|ь|н|а|я |к|о|н|т|р|-|с|и|л|а",
	L"|Ч|а|с|т|о|т|а |к|о|н|т|р|-|с|и|л|ы",
	L"|Б|о|н|у|с| |п|о|п|а|д|а|н|и|я",
	L"|Б|о|ну|с |п|р|и|ц|е|л|и|в|а|н|и|я",
	L"|S|i|n|g|l|e |S|h|o|t |T|e|m|p|e|r|a|t|u|r|e",		// TODO.Translate
	L"|C|o|o|l|d|o|w|n |F|a|c|t|o|r",
	L"|J|a|m |T|h|r|e|s|h|o|l|d",
	L"|D|a|m|a|g|e |T|h|r|e|s|h|o|l|d",
	L"|T|e|m|p|e|r|a|t|u|r|e |M|o|d|i|f|i|e|r",
	L"|C|o|o|l|d|o|w|n |M|o|d|i|f|i|e|r",
	L"|J|a|m |T|h|r|e|s|h|o|l|d |M|o|d|i|f|i|e|r",
	L"|D|a|m|a|g|e |T|h|r|e|s|h|o|l|d |M|o|d|i|f|i|e|r",
	L"|P|o|i|s|o|n |P|e|r|c|e|n|t|a|g|e",	// TODO.Translate
	L"|D|i|r|t |M|o|d|i|f|i|e|r",			// TODO.Translate
	L"|P|o|i|s|o|n |M|o|d|i|f|i|e|r",//TODO.translate
	L"|F|o|o|d| |P|o|i|n|t|s",//TODO.translate
	L"|D|r|i|n|k |P|o|i|n|t|s",//TODO.translate
	L"|P|o|r|t|i|o|n |S|i|z|e",//TODO.translate
	L"|M|o|r|a|l|e |M|o|d|i|f|i|e|r",//TODO.translate
	L"|D|e|c|a|y |M|o|d|i|f|i|e|r",//TODO.translate
	L"|B|e|s|t |L|a|s|e|r |R|a|n|g|e",//TODO.translate
	L"|P|e|r|c|e|n|t |R|e|c|o|i|l |M|o|d|i|f|i|e|r",	// 65	// TODO.Translate
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular.
STR16 szUDBAdvStatsExplanationsTooltipText[]=
{
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the weapon's Accuracy value.\n \nIncreased accuracy allows the gun to hit targets\nat longer ranges more often, assuming it is\nalso well-aimed.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis item modifies the shooter's accuracy\nfor ANY shot with a ranged weapon by the\nlisted amount.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis item modifies the shooter's accuracy\nfor ANY shot with a ranged weapon by the\nlisted percentage, based on their original accuracy.\n \nHigher is better.",
	L"\n \nThis item modifies the accuracy gained from each\nextra aiming level you pay for, when aiming\na ranged weapon, by the\nlisted amount.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis item modifies the accuracy gained from each\nextra aiming level you pay for, when aiming\na ranged weapon, by the\nlisted percentage based on the original value.\n \nHigher is better.",
	L"\n \nThis item modifies the number of extra aiming\nlevels this gun can take.\n \nReducing the number of allowed aiming levels\nmeans that each level adds proportionally\nmore accuracy to the shot.\nTherefore, the FEWER aiming levels are allowed,\nthe faster you can aim this gun, without losing\naccuracy!\n \nLower is better.",
	L"\n \nThis item modifies the shooter's maximum accuracy\nwhen using ranged weapons, as a percentage\nof their original maximum accuracy.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the weapon's Handling difficulty.\n \nBetter handling makes the gun more accurate to fire,\nwith or without extra aiming.\n \nNote that this is based on the gun's original\nGun Handling factor, which is higher for rifles and\nheavy weapons, and lower for pistols and small\nweapons.\n \nLower is better.",
	L"\n \nThis item modifies the difficulty of\ncompensating for shots beyond a weapon's range.\n \nA high bonus here can increase a weapon's\nnatural Maximum Range by at least a few tiles.\n \nHigher is better.",
	L"\n \nThis item modifies the difficulty of hitting\na moving target with a ranged weapon.\n \nA high bonus here can help hitting\nfast-moving targets, even at a distance.\n \nHigher is better.",
	L"\n \nThis item modifies the damage output of\nyour weapon, by the listed amount.\n \nHigher is better.",
	L"\n \nThis item modifies the damage output of\nyour melee weapon, by the listed amount.\n \nThis applies only to melee weapons, both sharp\nand blunt.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies its maximum effective range.\n \nMaximum Range mainly dictates how far a bullet\nfired from the weapon can fly before it begins\ndropping sharply towards the ground.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nprovides extra magnification, making shots at a distance\ncomparatively easier to make.\n \nNote that a high Magnification Factor is detrimental\nwhen used at targets CLOSER than the\noptimal distance.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nprojects a dot on the target, making it easier to hit.\n \nThe projection effect is only useful up to a given\ndistance, beyond which it begins to diminish and\neventually disappears.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Horizontal Recoil\nby the listed percentage.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Vertical Recoil\nby the listed percentage.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
	L"\n \nThis item modifies the shooter's ability to\ncope with recoil during Burst or Autofire volleys.\n \nWhen high, this can help a shooter to control\nguns with powerful recoil, even if the shooter\nhas low Strength.\n \nHigher is better.",
	L"\n \nThis item modifies the shooter's ability to\naccurately apply counter-force against a gun's\nrecoil, during Burst or Autofire volleys.\n \nA high bonus helps the shooter bring the gun's muzzle\nprecisely towards the target, even at longer ranges,\nmaking volleys more accurate as a result.\n \nHigher is better.",
	L"\n \nThis item modifies the shooter's ability to\nfrequently reasses how much counter-force they\nneed to apply against a gun's recoil, during Burst\nor Autofire volleys.\n \nHigher frequency makes volleys more accurate on the whole,\nand also makes longer volleys more accurate assuming\nthe shooter can overcome recoil correctly.\n \nHigher is better.",
	L"\n \nThis item directly modifies the amount of\nAPs the character gets at the start of each turn.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifiest the AP cost to bring the weapon to\n'Ready' mode.\n \nLower is better.",
	L"\n \nWhen attached to any weapon, this item\nmodifies the AP cost to make a single attack with\nthat weapon.\n \nNote that for Burst/Auto-capable weapons, the\ncost of using these modes is directly influenced\nby this modifier as well!\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon capable of\nBurst-fire mode, this item modifies the AP cost\nof firing a Burst.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon capable of\nAuto-fire mode, this item modifies the AP cost\nof firing an Autofire Volley.\n \nNote that it does NOT modify the extra AP\ncost for adding bullets to the volley, only\nthe initial cost for starting the volley.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the AP cost of reloading the weapon.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nchanges the size of magazines that can be loaded\ninto the weapon.\n \nThat weapon will now accept larger or smaller\nmagazines of the same caliber.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the amount of bullets fired\nby the weapon in Burst mode.\n \nIf the weapon was not initially Burst-Capable, and the\nmodifier is positive, attaching it to the weapon\nwill enable burst-fire mode.\n \nConversely, if the weapon is initially Burst-Capable,\na high-enough negative modifier here can disable\nburst mode completely.\n \nHigher is USUALLY better. Of course, part of the\npoint in Burst Mode is to conserve bullets...",
	L"\n \nWhen attached to a ranged weapon, this item\nwill hide the weapon's muzzle flash.\n \nThis makes sure that enemies cannot spot the shooter\nif he is firing while hidden, and is especially\nimportant at night.",
	L"\n \nWhen attached to a weapon, this item modifies\nthe range at which firing the weapon can be\nheard by both enemies and mercs.\n \nIf this modifier drops the weapon's Loudness value\nto 0, the weapon becomes completely silent.\n \nLower is better.",
	L"\n \nThis item modifies the size of any item it\nis attached to.\n \nSize is important when using the New Inventory system,\nwhere pockets only accept items of specific sizes and shapes.\n \nIncreasing an item's size makes it too big for some pockets\nit used to fit into.\n \nConversely, making an item smaller means it will fit into\nmore pockets, and pockets will be able to contain\nmore of it.\n \nLower is generall better.",
	L"\n \nWhen attached to any weapon, this item modifies\nthat weapon's Reliability value.\n \nIf positive, the weapon's condition will deteriorate\nslower when used in combat. Otherwise, the\nweapon deteriorates faster.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's camouflage in\nwoodland backgrounds.\n \nTo make good on a positive Woodland Camo modifier, the\nwearer needs to stay close to trees or tall grass.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's camouflage in\nurban backgrounds.\n \nTo make good on a positive Urban Camo modifier, the\nwearer needs to stay close to asphalt or concrete.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's camouflage in\ndesert backgrounds.\n \nTo make good on a positive Desert Camo modifier, the\nwearer needs to stay close to sand, gravel, or\ndesert vegetation.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's camouflage in\nsnowy backgrounds.\n \nTo make good on a positive Snow Camo modifier, the\nwearer needs to stay close to snowy tiles.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's stealth ability by\nmaking it more difficult to HEAR the character moving\nwhile in Sneaking mode.\n \nNote that this does NOT change a character's visibility,\nonly the amount of noise they make while sneaking.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Hearing Range by the\nlisted percent.\n \nA positive bonus makes it possible to hear noises\nfrom a greater distance.\n \nConversely, a negative modifier impairs the wearer's hearing.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted percent.\n \nThis General modifier works in all conditions.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted percent.\n \nThis Night-Vision modifier works only when light\nlevels are sufficiently low.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted percent.\n \nThis Day-Vision modifier works only when light\nlevels are average or higher.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted percent.\n \nThis Bright-Vision modifier works only when light\nlevels are very high, for example when looking\ninto tiles lit by Break-Lights or at high noon.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted percent.\n \nThis Cave-Vision modifier works only in the dark\nand only underground.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it changes the wearer's field-of-view.\n \nNarrowing the field of view shortens sightrange to\neither side.\n \nLower is better.",
	L"\n \nThis is the shooter's ability to\ncope with recoil during Burst or Autofire volleys.\n \n\n \nHigher is better.",
	L"\n \nThis is the shooter's ability to\nfrequently reasses how much counter-force they\nneed to apply against a gun's recoil, during Burst\nor Autofire volleys.\n \nLower frequency makes volleys more accurate on the whole,\nand also makes longer volleys more accurate assuming\nthe shooter can overcome recoil correctly.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the weapon's CTH value.\n \nIncreased CTH allows the gun to hit targets\nmore often, assuming it is also well-aimed.\n \nHigher is better.",
	L"\n \nWhen attached to a ranged weapon, this item\nmodifies the weapon's Aim Bonus.\n \nIncreased Aim Bonus allows the gun to hit\ntargets at longer ranges more often, assuming\nit is also well-aimed.\n \nHigher is better.",
	L"\n \nA single shot causes this much heat.\nAmmunition types and attachments can\naffect this value.\n \nLower is better.",					// TODO.Translate
	L"\n \nEvery turn, the temperature is lowered\nby this amount.\n \nHigher is better.",
	L"\n \nIf an item's temperature is above this,\nit will jam more frequently.\n \nHigher is better.",
	L"\n \nIf an item's temperature is above this,\nit will be damaged more easily.\n \nHigher is better.",
	L"\n \nA gun's single shot temperature is\nincreased by this percentage.\n \nLower is better.",
	L"\n \nA gun's cooldown factor is\nincreased by this percentage.\n \nHigher is better.",
	L"\n \nA gun's jam threshold is\nincreased by this percentage.\n \nHigher is better.",
	L"\n \nA gun's damage threshold is\nincreased by this percentage.\n \nHigher is better.",
	L"\n \nThis is the percentage of damage dealt\nby this item that will be poisonous.\n\nUsefulness depends on wether enemy\nhas poison resistance or absorption.",	// TODO.Translate
	L"\n \nA single shot causes this much dirt.\nAmmunition types and attachments can\naffect this value.\n \nLower is better.",	// TODO.Translate
	L"\n \nWhen this item is eaten\nit causes that much poison.\n \nLower is better.",	// TODO.TRANSLATE
	L"\n \nAmount of energy in kcal.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nAmount of water in liter.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nThe percentage of the item\nthat will be eaten at once.\n \nLower is better.",	// TODO.TRANSLATE
	L"\n \nMorale is adjusted by this amount.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nThis item becomes stale over time.\nIf more then 50% is molded it becomes poisoneous.\nThis is the rate at which mold is generated.\nLower is better.",	// TODO.TRANSLATE
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Recoil by the listed percentage.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",	// TODO.TRANSLATE
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \nThis weapon's accuracy is being modified by\nan ammo, attachment, or built-in attributes.\n \nIncreased accuracy allows the gun to hit targets\nat longer ranges more often, assuming it is\nalso well-aimed.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis weapon modifies its shooter's accuracy\nwith ANY shot by the listed amount.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis weapon modifies its shooter's accuracy\nwith ANY shot by the listed percentage\nbased on the shooter's original accuracy.\n \nHigher is better.",
	L"\n \nThis weapon modifies the amount of accuracy\ngained from each extra aiming level you\npay for by the listed amount.\n \nScale: -100 to +100.\nHigher is better.",
	L"\n \nThis weapon modifies the amount of accuracy\ngained from each extra aiming level you\npay for by the listed percentage, based\non the shooter's original accuracy.\n \nHigher is better.",
	L"\n \nThe number of Extra Aiming Levels allowed\nfor this gun has been modified by its ammo,\nattachments, or built-in attributes.\nIf the number of levels is being reduced, the gun is\nfaster to aim without being any less accurate.\n \nConversely, if the number of levels is increased,\nthe gun becomes slower to aim without being\nmore accurate.\n \nLower is better.",
	L"\n \nThis weapon modifies the shooter's maximum\naccuracy, as a percentage of the shooter's original\nmaximum accuracy.\n \nHigher is better.",
	L"\n \nThis weapon's attachments or inherent abilities\nmodify the weapon's Handling difficulty.\n \nBetter handling makes the gun more accurate to fire,\nwith or without extra aiming.\n \nNote that this is based on the gun's original\nGun Handling factor, which is higher for rifles and\nheavy weapons, and lower for pistols and small\nweapons.\n \nLower is better.",
	L"\n \nThis weapon's ability to compensate for shots\nbeyond its maximum range is being modified by\nattachments or the weapon's inherent abilities.\n \nA high bonus here can increase a weapon's\nnatural Maximum Range by at least a few tiles.\n \nHigher is better.",
	L"\n \nThis weapon's ability to hit moving targets\nat a distance is being modified by attachments\nor the weapon's inherent abilities.\n \nA high bonus here can help hitting\nfast-moving targets, even at a distance.\n \nHigher is better.",
	L"\n \nThis weapon's damage output is being modified\nby its ammo, attachments, or inherent abilities.\n \nHigher is better.",
	L"\n \nThis weapon's melee-combat damage output is being\nmodified by its ammo, attachments, or inherent abilities.\n \nThis applies only to melee weapons, both sharp\nand blunt.\n \nHigher is better.",
	L"\n \nThis weapon's maximum range has been increased\nor decreased thanks to its ammo, attachments,\nor inherent abilities.\n \nMaximum Range mainly dictates how far a bullet\nfired from the weapon can fly before it begins\ndropping sharply towards the ground.\n \nHigher is better.",
	L"\n \nThis weapon is equipped with optical magnification,\nmaking shots at a distance comparatively easier to make.\n \nNote that a high Magnification Factor is detrimental\nwhen used at targets CLOSER than the\noptimal distance.\n \nHigher is better.",
	L"\n \nThis weapon is equipped with a projection device\n(possibly a laser), which projects a dot on\nthe target, making it easier to hit.\n \nThe projection effect is only useful up to a given\ndistance, beyond which it begins to diminish and\neventually disappears.\n \nHigher is better.",
	L"\n \nThis weapon's horizontal recoil strength is being\nmodified by its ammo, attachments, or inherent\nabilities.\n \nThis has no effect if the weapon lacks both\nBurst and Auto-Fire modes.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
	L"\n \nThis weapon's vertical recoil strength is being\nmodified by its ammo, attachments, or inherent\nabilities.\n \nThis has no effect if the weapon lacks both\nBurst and Auto-Fire modes.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
	L"\n \nThis weapon modifies the shooter's ability to\ncope with recoil during Burst or Autofire volleys,\ndue to its attachments, ammo, or inherent abilities.\n \nWhen high, this can help a shooter to control\nguns with powerful recoil, even if the shooter\nhas low Strength.\n \nHigher is better.",
	L"\n \nThis weapon modifies the shooter's ability to\naccurately apply counter-force against its\nrecoil, due to its attachments, ammo, or inherent abilities.\n \nNaturally, this has no effect if the weapon lacks\nboth Burst and Auto-Fire modes.\n \nA high bonus helps the shooter bring the gun's muzzle\nprecisely towards the target, even at longer ranges,\nmaking volleys more accurate as a result.\n \nHigher is better.",
	L"\n \nThis weapon modifies the shooter's ability to\nfrequently reasses how much counter-force they\nneed to apply against a gun's recoil, due to its\nattachments, ammo, or inherent abilities.\n \nNaturally, this has no effect if the weapon lacks\nboth Burst and Auto-Fire modes.\n \nHigher frequency makes volleys more accurate on the whole,\nand also makes longer volleys more accurate assuming\nthe shooter can overcome recoil correctly.\n \nHigher is better.",
	L"\n \nWhen held in hand, this weapon modifies the amount of\nAPs its user gets at the start of each turn.\n \nHigher is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe AP cost to bring this weapon to 'Ready' mode has\nbeen modified.\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe AP cost to make a single attack with this\nweapon has been modified.\n \nNote that for Burst/Auto-capable weapons, the\ncost of using these modes is directly influenced\nby this modifier as well!\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe AP cost to fire a Burst with this weapon has\nbeen modified.\n \nNaturally, this has no effect if the weapon is not\ncapable of Burst fire.\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe AP cost to fire an Autofire Volley with this weapon\nhas been modified.\n \nNaturally, this has no effect if the weapon is not\ncapable of Auto Fire.\n \nNote that it does NOT modify the extra AP\ncost for adding bullets to the volley, only\nthe initial cost for starting the volley.\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe AP cost of reloading this weapon has been modified.\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe size of magazines that can be loaded into this\nweapon has been modified.\n \nThe weapon will now accept larger or smaller\nmagazines of the same caliber.\n \nHigher is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthe amount of bullets fired by this weapon in Burst mode\nhas been modified.\n \nIf the weapon was not initially Burst-Capable, and the\nmodifier is positive, then this is what\ngives the weapon its burst-fire capability.\n \nConversely, if the weapon was initially Burst-Capable,\na high-enough negative modifier here may have\ndisabled burst mode entirely for this weapon.\n \nHigher is USUALLY better. Of course, part of the\npoint in Burst Mode is to conserve bullets...",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthis weapon produces no muzzle flash.\n \nThis makes sure that enemies cannot spot the shooter\nif he is firing while hidden, and is especially\nimportant at night.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthis weapon's loudness has been modified. The distance\nat which enemies and mercs can hear the weapon being\nused has subsequently changed.\n \nIf this modifier drops the weapon's Loudness value\nto 0, the weapon becomes completely silent.\n \nLower is better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthis weapon's size category has changed.\n \nSize is important when using the New Inventory system,\nwhere pockets only accept items of specific sizes and shapes.\n \nIncreasing an item's size makes it too big for some pockets\nit used to fit into.\n \nConversely, making an item smaller means it will fit into\nmore pockets, and pockets will be able to contain\nmore of it.\n \nLower is generall better.",
	L"\n \nDue to its attachments, ammo or inherent abilities,\nthis weapon's reliability has been modified.\n \nIf positive, the weapon's condition will deteriorate\nslower when used in combat. Otherwise, the\nweapon deteriorates faster.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's camouflage in woodland backgrounds.\n \nTo make good on a positive Woodland Camo modifier, the\nwearer needs to stay close to trees or tall grass.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's camouflage in urban backgrounds.\n \nTo make good on a positive Urban Camo modifier, the\nwearer needs to stay close to asphalt or concrete.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's camouflage in desert backgrounds.\n \nTo make good on a positive Desert Camo modifier, the\nwearer needs to stay close to sand, gravel, or\ndesert vegetation.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's camouflage in snowy backgrounds.\n \nTo make good on a positive Snow Camo modifier, the\nwearer needs to stay close to snowy tiles.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's stealth ability by making it\nmore or less difficult to HEAR the character moving\nwhile in Sneaking mode.\n \nNote that this does NOT change a character's visibility,\nonly the amount of noise they make while sneaking.\n \nHigher is better.",
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's Hearing Range by the listed percent.\n \nA positive bonus makes it possible to hear noises\nfrom a greater distance.\n \nConversely, a negative modifier impairs the wearer's hearing.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted percent, thanks to attachments or\ninherent properties of the weapon.\n \nThis General modifier works in all conditions.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted percent, thanks to attachments or\ninherent properties of the weapon.\n \nThis Night-Vision modifier works only when light\nlevels are sufficiently low.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted percent, thanks to attachments or\ninherent properties of the weapon.\n \nThis Day-Vision modifier works only when light\nlevels are average or higher.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted percent, thanks to attachments or\ninherent properties of the weapon.\n \nThis Bright-Vision modifier works only when light\nlevels are very high, for example when looking\ninto tiles lit by Break-Lights or at high noon.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted percent, thanks to attachments or\ninherent properties of the weapon.\n \nThis Cave-Vision modifier works only in the dark\nand only underground.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit changes the wearer's field-of-view.\n \nNarrowing the field of view shortens sightrange to\neither side.\n \nHigher is better.",
	L"\n \nThis is the shooter's ability to\ncope with recoil during Burst or Autofire volleys.\n \nHigher is better.",
	L"\n \nThis is the shooter's ability to\nfrequently reasses how much counter-force they\nneed to apply against a gun's recoil.\n \nNaturally, this has no effect if the weapon lacks\nboth Burst and Auto-Fire modes.\n \nLower frequency makes volleys more accurate on the whole,\nand also makes longer volleys more accurate assuming\nthe shooter can overcome recoil correctly.\n \nLower is better.",
	L"\n \nThis weapon's to-hit is being modified by\nan ammo, attachment, or built-in attributes.\n \nIncreased To-Hit allows the gun to hit targets\nmore often, assuming it is also well-aimed.\n \nHigher is better.",
	L"\n \nThis weapon's Aim Bonus is being modified by\nan ammo, attachment, or built-in attributes.\n \nIncreased Aim Bonus allows the gun to hit\ntargets at longer ranges more often, assuming\nit is also well-aimed.\n \nHigher is better.",
	L"\n \nA single shot causes this much heat.\nThe type of ammunition can affect this value.\n \nLower is better.",					// TODO.Translate
	L"\n \nEvery turn, the temperature is lowered\nby this amount.\nWeapon attachments can affect this.\n \nHigher is better.",
	L"\n \nIf a gun's temperature is above this,\nit will jam more frequently.",
	L"\n \nIf a gun's temperature is above this,\nit will be damaged more easily.",
	L"\n \nThis weapon's recoil strength is being\nmodified by this percentage value by its ammo,\nattachments, or inherent abilities.\n \nThis has no effect if the weapon lacks both\nBurst and Auto-Fire modes.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",	// TODO.Translate
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"ОДИНОЧНЫЙ",	//SINGLE
	L"ОД",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

// HEADROCK HAM 5: Screen messages for sector inventory sorting reports.
STR16 gzMapInventorySortingMessage[] =
{
	L"В квадрате  %c%d завершена сборка аммуниции в ящики.",
	L"С предметов в квадрате %c%d снята вся навеска.",
	L"В квадрате %c%d всё оружие разряжено.",
	L"В квадрате %c%d все вещи сгруппированы и объединены.",
};

STR16 gzMapInventoryFilterOptions[] =
{
	L"Показать всё",
	L"Оружие",
	L"Патроны",
	L"Взрывчатка",
	L"Холодное оружие",
	L"Броня",
	L"Рузгруз. системы",
	L"Наборы",
	L"Прочие предметы",
	L"Скрыть всё",
};

// Flugente: Temperature-based text similar to HAM 4's condition-based text.
STR16 gTemperatureDesc[] =
{
	L"Температура ",
	L"очень низкая",
	L"низкая",
	L"умеренная",
	L"высокая",
	L"очень высокая",
	L"опасная",
	L"КРИТИЧЕСКАЯ",
	L"ФАТАЛЬНАЯ",
	L"неизвестна",
	L"."
};

// Flugente: food condition texts
STR16 gFoodDesc[] =
{
	L"Пища ",
	L"свежая",
	L"хорошая",
	L"нормальная",
	L"несвежая",
	L"порченная",
	L"подгнившая",
	L"."
};

CHAR16* ranks[] = 
{	L"",			//ExpLevel 0// TODO.Translate
	L"Рядовой ",		//ExpLevel 1
	L"Ефрейтор ",		//ExpLevel 2
	L"Мл. сержант ",		//ExpLevel 3
	L"Сержант ",		//ExpLevel 4
	L"Лейтенант ",		//ExpLevel 5
	L"Капитан ",		//ExpLevel 6
	L"Майор ",		//ExpLevel 7
	L"Подполк. ",	//ExpLevel 8
	L"Полковник ",		//ExpLevel 9
	L"Генерал "		//ExpLevel 10
};


STR16	gzNewLaptopMessages[]=
{
	L"Спрашивайте о нашем специальном предложении!",
	L"Временно недоступно",
	L"Ознакомительная игра 'Jagged Alliance 2: Цена Свободы' содержит только первые 6 карт секторов. Финальная версия игры будет включать в себя гораздо больше возможностей (полная информация содержится в приложенном файле Readme.txt).",
};

STR16	zNewTacticalMessages[]=
{
	//L"Расстояние до цели: %d ед., Освещенность: %d/%d",
	L"Передатчик подключен к вашему ноутбуку.",
	L"Вы не можете нанять %s",
	L"Предложение действует ограниченное время и покрывает стоимость найма на всю миссию, плюс вы так же получите оборудование, перечисленное ниже.",
	L"Наемник %s - наше невероятное суперпредложение 'одна плата за все'. Вы также бесплатно получите его персональную экипировку.",
	L"Гонорар",
	L"В секторе кто-то есть...",
	//L"Дальнобойность оружия: %d ед., Шанс попасть: %d%%",
	L"Показать укрытия",
	L"Линия прицела",
	L"Новые наемники не могут высадиться здесь.",
	L"Так как ваш ноутбук лишился антенны, то вы не сможете нанять новых наемников. Возможно, сейчас вам стоит загрузить одну из сохраненных игр, или начать игру заново!",
	L"%s слышит металлический хруст под телом Джерри. Кажется, это чмо сломало антенну вашего ноутбука.",  //the %s is the name of a merc.
	L"После прочтения записей, оставленных помощником командира Морриса, %s видит, что не все еще потеряно. В записке содержатся координаты городов Арулько для запуска по ним ракет. Кроме того, там также указаны координаты самой ракетной базы.",
	L"Изучив панель управления, %s понимает, что координаты цели можно изменить, и тогда ракета уничтожит эту базу.  %s не собирается умирать, а значит нужно быстрее отсюда выбираться. Похоже, что самый быстрый способ это лифт...",
	L"В начале игры вы выбрали сохранение лишь в \"мирное время\" и теперь не можете записываться во время боя.",
	L"(Нельзя сохраняться во время боя)",
	L"Текущая кампания длиннее 30 символов.",
	L"Текущая кампания не найдена.",
	L"Кампания: По умолчанию ( %S )",
	L"Кампания: %S",
	L"Вы выбрали кампанию %S. Эта кампания является модификацией оригинальной кампании Unfinished Business. Вы уверены, что хотите играть кампанию %S?",
	L"Чтобы воспользоваться редактором, смените кампанию по умолчанию на другую.",
};

// The_bob : pocket popup text defs	// TODO.Translate
STR16	gszPocketPopupText[]=
{
	L"Гранатомёты",	// POCKET_POPUP_GRENADE_LAUNCHERS,
	L"Рокетницы",	// POCKET_POPUP_ROCKET_LAUNCHERS
	L"Холодное и метательное оружие",	// POCKET_POPUP_MEELE_AND_THROWN
	L"- no matching ammo -",	//POCKET_POPUP_NO_AMMO
	L"- no guns in inventory -",	//POCKET_POPUP_NO_GUNS
	L"ещё...",		//POCKET_POPUP_MOAR
};

// Flugente: externalised texts for some features	// TODO.Translate
STR16	szCovertTextStr[]=
{
	L"%s has camo!",
	L"%s has a backpack!",
	L"%s is seen carrying a corpse!",
	L"%s's %s is suspicious!",
	L"%s's %s is considered military hardware!",
	L"%s carries too many guns!",
	L"%s's %s is too advanced for an arulcan soldier!",
	L"%s's %s has too many attachments!",
	L"%s was seen performing suspicious activities!",
	L"%s does not look like a civilian!",
	L"%s bleeding was discovered!",
	L"%s is drunk and doesn't behave like a soldier!",
	L"On closer inspection, %s's disguise does not hold!",
	L"%s isn't supposed to be here!",
	L"%s isn't supposed to be here at this time!",
	L"%s was seen near a fresh corpse!",
	L"%s equipment raises a few eyebrows!",
	L"%s is seen targetting %s!",
	L"%s has seen through %s's disguise!",
	L"No clothes item found in Items.xml!",
	L"This does not work with the old trait system!",
	L"Not enough APs!",
	L"Bad palette found!",
	L"You need the covert skill to do this!",
	L"No uniform found!",
	L"%s is now disguised as a civilian.",
	L"%s is now disguised as a soldier.",
	L"%s wears a disorderly uniform!",
	L"In retrospect, asking for surrender in disguise wasn't the best idea...",
	L"%s was uncovered!",
	L"%s's disguise seems to be ok...",
	L"%s's disguise will not hold.",
	L"%s was caught stealing!",
	L"%s tried to manipulate %s's inventory.",
	L"%s is too close to an elite soldier!",	// TODO.Translate
};

STR16	szCorpseTextStr[]=
{
	L"Предмета 'голова' не найдено в Items.xml!",
	L"Тело невозможно обезглавить!",
	L"Мясных изделий не найдено в Items.xml!",
	L"Ты больной! Тебе лечиться надо! Действие невозможно.",
	L"Снимать с тела нечего!",
	L"%s не может снять одежду с этого трупа!",
	L"Это тело невозможно забрать с собой!", //This corpse cannot be taken!
	L"Освободите руки чтобы тянуть тело!", //No free hand to carry corpse
	L"Предметов тела не найдено в Items.xml!", //No corpse item found in Items.xml
	L"Неверный ID тела!", //Invalid corpse ID
};

STR16	szFoodTextStr[]=
{
	L"%s не хочет есть %s",
	L"%s не хочет пить %s",
	L"%s ест %s",
	L"%s пьёт %s",
	L"%s's strength was damaged due to being overfed!",
	L"%s's strength was damaged due to lack of nutrition!",
	L"%s's health was damaged due to being overfed!",
	L"%s's health was damaged due to lack of nutrition!",
	L"%s's strength was damaged due to excessive drinking!",
	L"%s's strength was damaged due to lack of water!",
	L"%s's health was damaged due to excessive drinking!",
	L"%s's health was damaged due to lack of water!",
	L"Sectorwide canteen filling not possible, Food System is off!"
};

STR16	szPrisonerTextStr[]= // TODO.Translate
{
	L"%d elites, %d regulars and %d amins were interrogated.",
	L"%d prisoners paid ransom money.",
	L"%d prisoners revealed enemy positions.",
	L"%d elites, %d regulars and %d amins joined our cause.",
	L"Prisoners start a massive riot in %s!",
	L"%d prisoners were sent to %s!",
	L"Prisoners have been released!",
	L"The army now occupies the prison in %s, the prisoners were freed!",
	L"The enemy refuses to surrender!",
	L"The enemy refuses to take you as prisoners - they prefer you dead!",
	L"This behaviour is set OFF in your ini settings.",
	L"%s has freed %s!",
};

STR16	szMTATextStr[]=	// TODO.Translate
{
	L"nothing",
	L"building a fortification",
	L"removing a fortification",
	L"filling sandbags",
	L"%s had to stop %s.",
};

STR16	szInventoryArmTextStr[]= // TODO.Translate
{
	L"Blow up (%d AP)",
	L"Blow up",
	L"Arm (%d AP)",
	L"Arm",
	L"Disarm (%d AP)",
	L"Disarm",
};

// TODO.Translate
STR16		szBackgroundText_Flags[]=
{
	L" might consume drugs in inventory\n",
	L" disregard for other backgrounds\n",
	L" +1 level in  underground sectors\n",
	L" might steal items\n",

	L" +1 traplevel to planted bombs\n",
	L" spreads corruption to nearby mercs\n",
	L" female only",							// won't show up, text exists for compatibility reasons
	L" male only",								// won't show up, text exists for compatibility reasons
};

// TODO.Translate
STR16	szBackgroundText_Value[]=
{
	L" %s%d%% APs in polar sectors\n",
	L" %s%d%% APs in desert sectors\n",	
	L" %s%d%% APs in swamp sectors\n",
	L" %s%d%% APs in urban sectors\n",
	L" %s%d%% APs in river sectors\n",
	L" %s%d%% APs in tropical sectors\n",
	L" %s%d%% APs in coastal sectors\n",
	L" %s%d%% APs in mountainous sectors\n",

	L" %s%d%% agility stat\n",
	L" %s%d%% dexterity stat\n",
	L" %s%d%% strength stat\n",
	L" %s%d%% leadership stat\n",
	L" %s%d%% marksmanship stat\n",
	L" %s%d%% mechanical stat\n",	
	L" %s%d%% explosives stat\n",
	L" %s%d%% medical stat\n",
	L" %s%d%% wisdom stat\n",

	L" %s%d%% APs on rooftops\n",
	L" %s%d%% APs needed to swim\n",
	L" %s%d%% APs needed for fortification actions\n",
	L" %s%d%% APs needed for mortars\n",
	L" %s%d%% APs needed to access inventory\n",
	L" looks in other direction on airdrops\n %s%d%% APs after airdrop\n",
	L" %s%d%% APs on first turn when assaulting a sector\n",
		
	L" %s%d%% travel speed on foot\n",
	L" %s%d%% travel speed on land vehicles\n",
	L" %s%d%% travel speed on air vehicles\n",
	L" %s%d%% travel speed on water vehicles\n",

	L" %s%d%% poison resistance\n",
	L" %s%d%% fear resistance\n",
	L" %s%d%% suppression resistance\n",
	L" %s%d%% physical resistance\n",
	L" %s%d%% alcohol resistance\n",

	L" %s%d%% interrogation effectiveness\n",
	L" %s%d%% prison guard strength\n",
	L" %s%d%% better prices when trading guns and ammo\n",
	L" %s%d%% better prices when trading armour, lbe, blades, kits etc.\n",
	L" %s%d%% team capitulation strength if we lead negotiations\n",
	L" %s%d%% faster running\n",
	L" %s%d%% bandaging speed\n",
	L" drinking restores %s%d%% energy\n",
	L" %s%d%% strength to carry items\n",
	L" %s%d%% food consumption\n",
	L" %s%d%% water consumption\n",
	L" %s%d need for sleep\n",
	L" %s%d%% melee damage\n",	
	L" %s%d%% cth with blades\n",
	L" %s%d%% camo effectiveness\n",
	L" %s%d%% stealth\n",
	L" %s%d%% max CTH\n",
	L" %s%d hearing range during the night\n",
	L" %s%d hearing range during the day\n",

	L" %s%d%% effectiveness to friendly approach\n",
	L" %s%d%% effectiveness to direct approach\n",
	L" %s%d%% effectiveness to threaten approach\n",
	L" %s%d%% effectiveness to recruit approach\n",

	L" %s%d%% chance of success with door breaching charges\n",
	L" %s%d%% cth with firearms against creatures\n",
	L" %s%d%% insurance cost\n",
};

STR16		szBackgroundTitleText[] =	// TODO.Translate
{
	L"I.M.P. Background",
};

// Flugente: personality
STR16		szPersonalityTitleText[] =	// TODO.Translate
{
	L"I.M.P. Prejudices",
};

STR16		szPersonalityDisplayText[]=	// TODO.Translate
{
	L"You look",
	L"and appearance is",
	L"important to you.",
	L"You have",
	L"and care",
	L"about that.",
	L"You are",
	L"and hate everyone",
	L".",
	L"racist against non-",
	L"people.",
};

// texts showing up when hovering over the box, used to explain what a selection does. Do not use more than 200 characters!
STR16		szPersonalityHelpText[]=
{
	L"How do you look?",
	L"How important are the looks of others to you?",
	L"What are your manners?",
	L"How important are the manners of other people to you?",
	L"What is your nationality?",
	L"What nation o you dislike?",
	L"How much do you dislike that nation?",
	L"How racist are you?",
	L"What is your race? You will be\nracist against all other races.",
	L"How sexist are you against the other gender?",
};

STR16		szRaceText[]=
{
	L"white",
	L"black",
	L"asian",
	L"eskimo",
	L"hispanic",
};

STR16		szAppearanceText[]=
{
	L"average",
	L"ugly",
	L"homely",
	L"attractive",
	L"like a babe",
};

STR16		szRefinementText[]=
{
	L"average manners",
	L"manners of a slob",
	L"manners of a snob",
};

STR16		szNationalityText[]=
{
	L"american",		// 0
	L"arab",
	L"australian",
	L"british",
	L"canadian",
	L"cuban",			// 5
	L"danish",
	L"french",
	L"russian",
	L"nigerian",
	L"swiss",			// 10
	L"jamaican",
	L"polish",
	L"chinese",
	L"irish",
	L"south african",	// 15
	L"hungarian",
	L"scottish",
	L"arulcan",
	L"german",
	L"african",			// 20
	L"italian",
	L"dutch",
	L"romanian",
	L"metaviran",

	// newly added from here on
	L"greek",			// 25
	L"estonian",
	L"venezuelan",
	L"japanese",
	L"turkish",
	L"indian",			// 30
	L"mexican",
	L"norwegian",
	L"spanish",
	L"brasilian",
	L"finnish",			// 35
	L"iranian",
	L"israeli",
	L"bulgarian",
	L"swedish",
	L"iraqi",			// 40
	L"syrian",
	L"belgian",
	L"portoguese",
};

// special text used if we do not hate any nation (value of -1)
STR16		szNationalityText_Special[]=
{
	L"do not hate any other nationality.",		// used in personnel.cpp
	L"of no origin",		// used in IMP generation
};

STR16		szCareLevelText[]=
{
	L"not",
	L"somewhat",
	L"extremely",
};

STR16		szRacistText[]=
{
	L"not",
	L"somewhat",
	L"very",
};

STR16		szSexistText[]=
{
	L"no sexist",
	L"somewhat sexist",
	L"very sexist",
	L"a Gentleman",
};

// Flugente: power pack texts
STR16 gPowerPackDesc[] =
{
	L"Batteries are ",
	L"full",
	L"good",
	L"at half",
	L"low",
	L"depleted",
	L"."
};

// WANNE: Special characters like % or someting else should go here
// We can't put them directly in the CPP code files, because they need special encoding (UTF8) for some languages (e.g: Chinese)
STR16 sSpecialCharacters[] =
{
	L"%",		// Percentage character
};

#endif //RUSSIAN
