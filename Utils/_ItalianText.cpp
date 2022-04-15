﻿// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("ITALIAN")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( ITALIAN )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_ItalianText_public_symbol(void){;}

#ifdef ITALIAN

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

// TODO.Translate
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
// TODO.Translate
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
// TODO.Translate
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
// TODO.Translate
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
// TODO.Translate
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
// TODO.Translate
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

// TODO.Translate
STR16		QuestDescText[] = 
{
	L"Deliver Letter",
	L"Food Route",
	L"Terrorists",
	L"Kingpin Chalice",
	L"Kingpin Money",
	L"Runaway Joey",
	L"Rescue Maria",
	L"Chitzena Chalice",
	L"Held in Alma",
	L"Interogation",

	L"Hillbilly Problem",								//10
	L"Find Scientist",
	L"Deliver Video Camera",
	L"Blood Cats",
	L"Find Hermit",
	L"Creatures",
	L"Find Chopper Pilot",
	L"Escort SkyRider",
	L"Free Dynamo",
	L"Escort Tourists",


	L"Doreen",									//20
	L"Leather Shop Dream",
	L"Escort Shank",
	L"No 23 Yet",
	L"No 24 Yet",
	L"Kill Deidranna",
	L"No 26 Yet",
	L"No 27 Yet",
	L"No 28 Yet",
	L"No 29 Yet",
};

// TODO.Translate
STR16		FactDescText[] = 
{
	L"Omerta Liberated",
	L"Drassen Liberated",
	L"Sanmona Liberated",
	L"Cambria Liberated",
	L"Alma Liberated",
	L"Grumm Liberated",
	L"Tixa Liberated",
	L"Chitzena Liberated",
	L"Estoni Liberated",
	L"Balime Liberated",

	L"Orta Liberated",					//10
	L"Meduna Liberated",
	L"Pacos approched",
	L"Fatima Read note",
	L"Fatima Walked away from player",
	L"Dimitri (#60) is dead",
	L"Fatima responded to Dimitri's supprise",
	L"Carlo's exclaimed 'no one moves'",
	L"Fatima described note",
	L"Fatima arrives at final dest",

	L"Dimitri said Fatima has proof",		//20
	L"Miguel overheard conversation",
	L"Miguel asked for letter",
	L"Miguel read note",
	L"Ira comment on Miguel reading note",
	L"Rebels are enemies",
	L"Fatima spoken to before given note",
	L"Start Drassen quest",
	L"Miguel offered Ira",
	L"Pacos hurt/Killed",

	L"Pacos is in A10",		//30
	L"Current Sector is safe",
	L"Bobby R Shpmnt in transit",
	L"Bobby R Shpmnt in Drassen",
	L"33 is TRUE and it arrived within 2 hours",
	L"33 is TRUE 34 is false more then 2 hours",
	L"Player has realized part of shipment is missing",
	L"36 is TRUE and Pablo was injured by player",
	L"Pablo admitted theft",
	L"Pablo returned goods, set 37 false",

	L"Miguel will join team",		//40
	L"Gave some cash to Pablo",
	L"Skyrider is currently under escort",
	L"Skyrider is close to his chopper in Drassen",
	L"Skyrider explained deal",
	L"Player has clicked on Heli in Mapscreen at least once",
	L"NPC is owed money",
	L"Npc is wounded",
	L"Npc was wounded by Player",
	L"Father J.Walker was told of food shortage",

	L"Ira is not in sector",		//50
	L"Ira is doing the talking",
	L"Food quest over",
	L"Pablo stole something from last shpmnt",
	L"Last shipment crashed",
	L"Last shipment went to wrong airport",
	L"24 hours elapsed since notified that shpment went to wrong airport",
	L"Lost package arrived with damaged goods. 56 to False",
	L"Lost package is lost permanently. Turn 56 False",
	L"Next package can (random) be lost",

	L"Next package can(random) be delayed",		//60
	L"Package is medium sized",
	L"Package is largesized",
	L"Doreen has conscience",
	L"Player Spoke to Gordon",
	L"Ira is still npc and in A10-2(hasnt joined)",
	L"Dynamo asked for first aid",
	L"Dynamo can be recruited",
	L"Npc is bleeding",
	L"Shank wnts to join",

	L"NPC is bleeding",		//70
	L"Player Team has head & Carmen in San Mona",
	L"Player Team has head & Carmen in Cambria",
	L"Player Team has head & Carmen in Drassen",
	L"Father is drunk",
	L"Player has wounded mercs within 8 tiles of NPC",
	L"1 & only 1 merc wounded within 8 tiles of NPC",
	L"More then 1 wounded merc within 8 tiles of NPC",
	L"Brenda is in the store ",
	L"Brenda is Dead",

	L"Brenda is at home",		//80
	L"NPC is an enemy",
	L"Speaker Strength >= 84 and < 3 males present",
	L"Speaker Strength >= 84 and at least 3 males present",
	L"Hans lets ou see Tony",
	L"Hans is standing on 13523",
	L"Tony isnt available Today",
	L"Female is speaking to NPC",
	L"Player has enjoyed the Brothel",
	L"Carla is available",

	L"Cindy is available",		//90
	L"Bambi is available",
	L"No girls is available",
	L"Player waited for girls",
	L"Player paid right amount of money",
	L"Mercs walked by goon",
	L"More thean 1 merc present within 3 tiles of NPC",
	L"At least 1 merc present withing 3 tiles of NPC",
	L"Kingping expectingh visit from player",
	L"Darren expecting money from player",

	L"Player within 5 tiles and NPC is visible",			//	100
	L"Carmen is in San Mona",
	L"Player Spoke to Carmen",
	L"KingPin knows about stolen money",
	L"Player gave money back to KingPin",
	L"Frank was given the money ( not to buy booze )",
	L"Player was told about KingPin watching fights",
	L"Past club closing time and Darren warned Player. (reset every day)",
	L"Joey is EPC",
	L"Joey is in C5",
	
	L"Joey is within 5 tiles of Martha(109) in sector G8",		//110
	L"Joey is Dead!",
	L"At least one player merc within 5 tiles of Martha",
	L"Spike is occuping tile 9817",
	L"Angel offered vest",
	L"Angel sold vest",
	L"Maria is EPC",
	L"Maria is EPC and inside leather Shop",
	L"Player wants to buy vest",
	L"Maria rescue was noticed by KingPin goons and Kingpin now enemy",
	
	L"Angel left deed on counter",		//120
	L"Maria quest over",
	L"Player bandaged NPC today",
	L"Doreen revealed allegiance to Queen",
	L"Pablo should not steal from player",
	L"Player shipment arrived but loyalty to low, so it left",
	L"Helicopter is in working condition",
	L"Player is giving amount of money >= $1000",
	L"Player is giving amount less than $1000",
	L"Waldo agreed to fix helicopter( heli is damaged )",
	
	L"Helicopter was destroyed",		//130
	L"Waldo told us about heli pilot",
	L"Father told us about Deidranna killing sick people",
	L"Father told us about Chivaldori family",
	L"Father told us about creatures",
	L"Loyalty is OK",
	L"Loyalty is Low",
	L"Loyalty is High",
	L"Player doing poorly",
	L"Player gave valid head to Carmen",
	
	L"Current sector is G9(Cambria)",		//140
	L"Current sector is C5(SanMona)",
	L"Current sector is C13(Drassen",
	L"Carmen has at least $10,000 on him",
	L"Player has Slay on team for over 48 hours",
	L"Carmen is suspicous about slay",
	L"Slay is in current sector",
	L"Carmen gave us final warning",
	L"Vince has explained that he has to charge",
	L"Vince is expecting cash (reset everyday)",
	
	L"Player stole some medical supplies once",		//150
	L"Player stole some medical supplies again",
	L"Vince can be recruited",
	L"Vince is currently doctoring",
	L"Vince was recruited",
	L"Slay offered deal",
	L"All terrorists killed",
	L"",
	L"Maria left in wrong sector",
	L"Skyrider left in wrong sector",
	
	L"Joey left in wrong sector",		//160
	L"John left in wrong sector",
	L"Mary left in wrong sector",
	L"Walter was bribed",
	L"Shank(67) is part of squad but not speaker",
	L"Maddog spoken to",
	L"Jake told us about shank",
	L"Shank(67) is not in secotr",
	L"Bloodcat quest on for more than 2 days",
	L"Effective threat made to Armand",
	
	L"Queen is DEAD!",												//170
	L"Speaker is with Aim or Aim person on squad within 10 tiles",
	L"Current mine is empty",
	L"Current mine is running out",
	L"Loyalty low in affiliated town (low mine production)",
	L"Creatures invaded current mine",
	L"Player LOST current mine",
	L"Current mine is at FULL production",
	L"Dynamo(66) is Speaker or within 10 tiles of speaker",		
	L"Fred told us about creatures",

	L"Matt told us about creatures",				//180
	L"Oswald told us about creatures",
	L"Calvin told us about creatures",
	L"Carl told us about creatures",
	L"Chalice stolen from museam",
	L"John(118) is EPC",
	L"Mary(119) and John (118) are EPC's",	
	L"Mary(119) is alive",
	L"Mary(119)is EPC",					
	L"Mary(119) is bleeding",		

	L"John(118) is alive",							//190
	L"John(118) is bleeding",					
	L"John or Mary close to airport in Drassen(B13)",
	L"Mary is Dead",
	L"Miners placed",
	L"Krott planning to shoot player",
	L"Madlab explained his situation",
	L"Madlab expecting a firearm",
	L"Madlab expecting a video camera.",			
	L"Item condition is < 70 ",										

	L"Madlab complained about bad firearm.",					//200
	L"Madlab complained about bad video camera.",
	L"Robot is ready to go!",
	L"First robot destroyed.",
	L"Madlab given a good camera.",
	L"Robot is ready to go a second time!",
	L"Second robot destroyed.",
	L"Mines explained to player.",
	L"Dynamo (#66) is in sector J9.",			
	L"Dynamo (#66) is alive.",

	L"One PC hasn't fought, but is able, and less than 3 fights have occured",							//210
	L"Player receiving mine income from Drassen, Cambria, Alma & Chitzena",
	L"Player has been to K4_b1",
	L"Brewster got to talk while Warden was alive",
	L"Warden (#103) is dead.",
	L"Ernest gave us the guns",
	L"This is the first bartender",
	L"This is the second bartender",
	L"This is the third bartender",
	L"This is the fourth bartender",


	L"Manny is a bartender.",									//220
	L"Nothing is repaired yet (some stuff being worked on, nothing to give player right now)",
	L"Player made purchase from Howard (#125)",
	L"Dave sold vehicle",
	L"Dave's vehicle ready",
	L"Dave expecting cash for car",
	L"Dave has gas. (randomized daily)",
	L"Vehicle is present",
	L"First battle won by player",
	L"Robot recruited and moved",			

	L"No club fighting allowed",							//230
	L"Player already fought 3 fights today",
	L"Hans mentioned Joey",
	L"Player is doing better than 50% (Alex's function)",
	L"Player is doing very well (better than 80%)",
	L"Father is drunk and sci-fi option is on",
	L"Micky (#96) is drunk",
	L"Player has attempted to force their way into brothel",
	L"Rat effectively threatened 3 times",
	L"Player paid for two people to enter brothel",		

	L"",						//240
	L"",
	L"Player owns 2 towns including omerta",
	L"Player owns 3 towns including omerta",//							243
	L"Player owns 4 towns including omerta",//							244
	L"",
	L"",
	L"",
	L"Fact male speaking female present",
	L"Fact hicks married player merc",//													249

	L"Fact museum open",//																				250
	L"Fact brothel open",//																			251
	L"Fact club open",//																					252
	L"Fact first battle fought",//																253
	L"Fact first battle being fought",//													254
	L"Fact kingpin introduced self",//														255
	L"Fact kingpin not in office",//															256
	L"Fact dont owe kingpin money",//														257
	L"Fact pc marrying daryl is flo",//													258
	L"",

	L"",					//260
	L"Fact npc cowering",																			//	261,
	L"",
	L"",
	L"Fact top and bottom levels cleared",
	L"Fact top level cleared",//																	265
	L"Fact bottom level cleared",//															266
	L"Fact need to speak nicely",//															267
	L"Fact attached item before",//															268
	L"Fact skyrider ever escorted",//														269

	L"Fact npc not under fire",//																270
	L"Fact willis heard about joey rescue",//										271
	L"Fact willis gives discount",//															272
	L"Fact hillbillies killed",//																273
	L"Fact keith out of business",	//														274												
	L"Fact mike available to army",//														275
	L"Fact kingpin can send assassins",//												276
	L"Fact estoni refuelling possible",//						277
	L"Fact museum alarm went off",//															278
	L"",

	L"Fact maddog is speaker", 																	//280,
	L"",
	L"Fact angel mentioned deed", 															// 282,
	L"Fact iggy available to army",//														283
	L"Fact pc has conrads recruit opinion",//										284
	L"",
	L"",
	L"",
	L"",
	L"Fact npc hostile or pissed off",													//289,

	L"",					//290
	L"Fact tony in building",																		//291,
	L"Fact shank speaking",	//																		292,
	L"Fact doreen alive",//																			293
	L"Fact waldo alive",//																				294
	L"Fact perko alive",//																				295
	L"Fact tony alive",//																				296
	L"",
	L"Fact vince alive",// 																				298,
	L"Fact jenny alive",//																				299

	L"",					//300
	L"",
	L"Fact arnold alive",//																				302,
	L"",
	L"Fact rocket rifle exists",// 																304,
	L"",
	L"",
	L"",
	L"",
	L"",

	L"",					//310
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//320

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//330

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//340

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//350

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//360

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//370

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//380

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//390

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//400

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//410

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//420

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//430

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//440

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//450

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//460

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//470

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//480

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//490

	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",					//500
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
 L"Find next merc (|S|p|a|c|e)\nFind previous merc (|C|t|r|l+|S|p|a|c|e)",
 L"Toggle priority existance",
 L"Toggle whether or not placement\nhas access to all doors",
 
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
	L"Draw room number\n(Hold |S|h|i|f|t to reuse room number)",
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
	L"When checked, the map will be saved in original JA2 map format. Items with ID > 350 will be lost.\nThis option is only valid on 'normal' size maps that do not reference grid numbers (e.g: exit grids) > 25600.",
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
	L"|S|p|a|c|e: Select next item\n|C|t|r|l+|S|p|a|c|e: Select previous item\n \n|/: Place same item under mouse cursor\n|C|t|r|l+|/: Place new item under mouse cursor", //Items fasthelp text
	L"|1-|9: Set waypoints\n|C|t|r|l+|C/|C|t|r|l+|V: Copy/Paste merc\n|P|g |U|p/|P|g |D|n: Toggle merc placement level", //Mercs fasthelp text
	L"|C|t|r|l+|G: Go to grid no\n|S|h|i|f|t: Scroll beyond map boundary\n \n(|t|i|l|d|e): Toggle cursor level\n|I: Toggle overhead map\n|J: Toggle draw high ground\n|K: Toggle high ground markers\n|S|h|i|f|t+|L: Toggle map edge points\n|S|h|i|f|t+|T: Toggle treetops\n|U: Toggle world raise\n \n|./|,: Cycle 'width: xx' dimensions", //Map Info fasthelp text
	L"|C|t|r|l+|N: Create new map\n \n|F|5: Show Summary Info/Country Map\n|F|1|0: Remove all lights\n|F|1|1: Reverse schedules\n|F|1|2: Clear schedules\n \n|S|h|i|f|t+|R: Toggle random placement based on quantity of selected object(s)\n \nCommand Line options\n|-|D|O|M|A|P|S: Batch radarmap generation\n|-|D|O|M|A|P|S|C|N|V: Batch radarmap generation and covert maps to latest version", //Options fasthelp text
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
	L"Humvee",		// TODO.Translate
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
/*
CHAR16 gszActionItemDesc[ 34 ][ 30 ] =	// NUM_ACTIONITEMS = 34
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
	L"%S[%d] from default tileset %s (%d, %S)",
	L"File: %S, subindex: %d (%d, %S)",
	L"Tileset:  %s",
};

STR16 pDisplaySelectionWindowButtonText[] =
{
	L"Accept selections (|E|n|t|e|r)",
	L"Cancel selections (|E|s|c)\nClear selections (|S|p|a|c|e)",
	L"Scroll window up (|U|p)",
	L"Scroll window down (|D|o|w|n)",
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

// TODO.Translate
CHAR16  gszAimPages[ 6 ][ 20 ] =
{
	L"Page 1/2", //0
	L"Page 2/2",
	
	L"Page 1/3",
	L"Page 2/3",
	L"Page 3/3",
	
	L"Page 1/1", //5
};

// by Jazz: TODO.Translate
CHAR16 zGrod[][500] =
{
	L"Robot", //0    // Robot
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
//	L"cal .38",
//	L"9 mm",
//	L"cal .45",
//	L"cal .357",
//	L"cal fisso 12",
//	L"CAW",
//	L"5.45 mm",
//	L"5.56 mm",
//	L"7.62 mm NATO",
//	L"7.62 mm WP",
//	L"4.7 mm",
//	L"5.7 mm",
//	L"Mostro",
//	L"Missile",
//	L"", // dart
//	L"", // flame
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
//	L"cal .38",
//	L"9 mm",
//	L"cal .45",
//	L"cal .357",
//	L"cal fisso 12",
//	L"CAWS",
//	L"5.45 mm",
//	L"5.56 mm",
//	L"7.62 mm N.",
//	L"7.62 mm WP",
//	L"4.7 mm",
//	L"5.7 mm",
//	L"Mostro",
//	L"Missile",
//	L"", // dart
//};


CHAR16 WeaponType[][30] =
{
	L"Altro",
	L"Arma",
	L"Mitragliatrice",
	L"Mitra",
	L"Fucile",
	L"Fucile del cecchino",
	L"Fucile d'assalto",
	L"Mitragliatrice leggera",
	L"Fucile a canne mozze",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Turno del giocatore", // player's turn
	L"Turno degli avversari",
	L"Turno delle creature",
	L"Turno dell'esercito",
	L"Turno dei civili",
	L"Player_Plan",// planning turn
	L"Client #1",//hayden
	L"Client #2",//hayden
	L"Client #3",//hayden
	L"Client #4",//hayden
};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s è stato colpito alla testa e perde un punto di saggezza!",
	L"%s è stato colpito alla spalla e perde un punto di destrezza!",
	L"%s è stato colpito al torace e perde un punto di forza!",
	L"%s è stato colpito alle gambe e perde un punto di agilità!",
	L"%s è stato colpito alla testa e perde %d punti di saggezza!",
	L"%s è stato colpito alle palle perde %d punti di destrezza!",
	L"%s è stato colpito al torace e perde %d punti di forza!",
	L"%s è stato colpito alle gambe e perde %d punti di agilità!",
	L"Interrompete!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"I vostri rinforzi sono arrivati!",

	// In the following four lines, all %s's are merc names

	L"%s ricarica.",
	L"%s non ha abbastanza Punti Azione!",
	L"%s ricorre al pronto soccorso. (Premete un tasto per annullare.)",
	L"%s e %s ricorrono al pronto soccorso. (Premete un tasto per annullare.)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"affidabile",
	L"non affidabile",
	L"facile da riparare",
	L"difficile da riparare",
	L"danno grave",
	L"danno lieve",
	L"fuoco veloce",
	L"fuoco",
	L"raggio lungo",
	L"raggio corto",
	L"leggero",
	L"pesante",
	L"piccolo",
	L"fuoco a raffica",
	L"niente raffiche",
	L"grande deposito d'armi",
	L"piccolo deposito d'armi",

	// In the following two lines, all %s's are merc names

	L"Il travestimento di %s è stato scoperto.",
	L"Il travestimento di %s è stato scoperto.",

	// The first %s is a merc name and the second %s is an item name

	L"La seconda arma è priva di munizioni!",
	L"%s ha rubato il %s.",

	// The %s is a merc name

	L"L'arma di %s non può più sparare a raffica.",

	L"Ne avete appena ricevuto uno di quelli attaccati.",
	L"Volete combinare gli oggetti?", 

	// Both %s's are item names

	L"Non potete attaccare %s a un %s.",

	L"Nessuno",
	L"Espelli munizioni",
	L"Attaccare",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"Non potete usare %s e il vostro %s contemporaneamente.",

	L"L'oggetto puntato dal vostro cursore può essere combinato ad alcuni oggetti ponendolo in uno dei quattro slot predisposti.",
	L"L'oggetto puntato dal vostro cursore può essere combinato ad alcuni oggetti ponendolo in uno dei quattro slot predisposti. (Comunque, in questo caso, l'oggetto non è compatibile.)",
	L"Il settore non è libero da nemici!",
	L"Vi dovete ancora dare %s %s", 
	L"%s è stato colpito alla testa!",
	L"Abbandonate la battaglia?",
	L"Questo attaco sarà definitivo. Andate avanti?",
	L"%s si sente molto rinvigorito!",
	L"%s ha dormito di sasso!",
	L"%s non è riuscito a catturare il %s!",
	L"%s ha riparato il %s",
	L"Interrompete per ",
	L"Vi arrendete?",
	L"Questa persona rifiuta il vostro aiuto.",
	L"NON sono d'accordo!",
    L"Per viaggiare sull'elicottero di Skyrider, dovrete innanzitutto ASSEGNARE mercenari al VEICOLO/ELICOTTERO.",
	L"solo %s aveva abbastanza tempo per ricaricare UNA pistola",
	L"Turno dei Bloodcat",
	L"automatic",
	L"no full auto",
	L"The enemy has no more items to steal!",
	L"The enemy has no item in its hand!",
// TODO.Translate
	L"%s's desert camouflage has worn off.",
	L"%s's desert camouflage has washed off.",
	
	L"%s's wood camouflage has worn off.",
	L"%s's wood camouflage has washed off.",
	
	L"%s's urban camouflage has worn off.",
	L"%s's urban camouflage has washed off.",
	
	L"%s's snow camouflage snow has worn off.",
	L"%s's snow camouflage has washed off.",

	L"You cannot attach %s to this slot.",
	L"The %s will not fit in any open slots.",
	L"There's not enough space for this pocket.", //TODO:Translate

	L"%s has repaired the %s as much as possible.", // TODO.Translate
	L"%s has repaired %s's %s as much as possible.",

	L"%s has cleaned the %s.",	// TODO.Translate
	L"%s has cleaned %s's %s.",

	L"Assignment not possible at the moment",	// TODO.Translate
	L"No militia that can be drilled present.",

	L"%s has fully explored %s.",	// TODO.Translate
};

// the country and its noun in the game
CHAR16 pCountryNames[][MAX_TOWN_NAME_LENGHT] =
{
#ifdef JA2UB
	L"Tracona",
	L"Traconian",
#else
	L"Arulco",
	L"Arulcan",
#endif
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
	L"Fermo",
	L"Normale",
	L"5 min",
	L"30 min",
	L"60 min",
	L"6 ore", 
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Squad. 1",
	L"Squad. 2",
	L"Squad. 3",
	L"Squad. 4",
	L"Squad. 5",
	L"Squad. 6",
	L"Squad. 7",
	L"Squad. 8",
	L"Squad. 9",
	L"Squad. 10",
	L"Squad. 11",
	L"Squad. 12",
	L"Squad. 13",
	L"Squad. 14",
	L"Squad. 15",
	L"Squad. 16",
	L"Squad. 17",
	L"Squad. 18",
	L"Squad. 19",
	L"Squad. 20",
	L"Servizio", // on active duty
	L"Dottore", // administering medical aid
	L"Paziente", // getting medical aid
	L"Veicolo", // in a vehicle
	L"Transito", // in transit - abbreviated form
	L"Riparare", // repairing
	L"Radio Scan",	// scanning for nearby patrols	// TODO.Translate
	L"Esercit.", // training themselves  
	L"Esercit.", // training a town to revolt 
	L"M.Militia", //training moving militia units		// TODO.Translate
	L"Istrutt.", // training a teammate
	L"Studente", // being trained by someone else
	L"Get Item",	// get items	// TODO.Translate
	L"Staff", // operating a strategic facility			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.)	// TODO.Translate
	L"Rest", // Resting at a facility					// TODO.Translate
	L"Prison",		// Flugente: interrogate prisoners
	L"Morto", // dead
	L"Incap.", // abbreviation for incapacitated
	L"PDG", // Prisoner of war - captured
	L"Ospedale", // patient in a hospital 
	L"Vuoto",	// Vehicle is empty
	L"Snitch",	// facility: undercover prisoner (snitch)	// TODO.Translate
	L"Propag.",	// facility: spread propaganda
	L"Propag.",	// facility: spread propaganda (globally)
	L"Rumours",	// facility: gather information
	L"Propag.",	// spread propaganda
	L"Rumours",	// gather information
	L"Command",	// militia movement orders
	L"Diagnose", // disease diagnosis	//TODO.Translate
	L"Treat D.", // treat disease among the population
	L"Dottore", // administering medical aid
	L"Paziente", // getting medical aid
	L"Riparare", // repairing
	L"Fortify",		// build structures according to external layout	// TODO.Translate
	L"Train W.",
	L"Hide",	// TODO.Translate
	L"GetIntel",
	L"DoctorM.",
	L"DMilitia",
	L"Burial",
	L"Admin", // TODO.Translate
	L"Explore",	// TODO.Translate
	L"Event"// rftr: merc is on a mini event // TODO: translate
};


STR16 pMilitiaString[] =
{
	L"Esercito", // the title of the militia box
	L"Non incaricato", //the number of unassigned militia troops
	L"Non potete ridistribuire reclute, se ci sono nemici nei paraggi!",
	L"Some militia were not assigned to a sector. Would you like to disband them?", // TODO.Translate
};


STR16 pMilitiaButtonString[] =
{
	L"Auto", // auto place the militia troops for the player
	L"Eseguito", // done placing militia troops
	L"Disband", // HEADROCK HAM 3.6: Disband militia	// TODO.Translate
	L"Unassign All", // move all milita troops to unassigned pool	// TODO.Translate
};

STR16 pConditionStrings[] = 
{
	L"Eccellente", //the state of a soldier .. excellent health
	L"Buono", // good health
	L"Discreto", // fair health
	L"Ferito", // wounded health
	L"Stanco", // tired
	L"Grave", // bleeding to death
	L"Svenuto", // knocked out 
	L"Morente", // near death
	L"Morto", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"In servizio", // set merc on active duty
	L"Paziente", // set as a patient to receive medical aid
	L"Veicolo", // tell merc to enter vehicle
	L"Non scortato", // let the escorted character go off on their own
	L"Cancella", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Squadra 1",
	L"Squadra 2",
	L"Squadra 3",
	L"Squadra 4",
	L"Squadra 5",
	L"Squadra 6",
	L"Squadra 7",
	L"Squadra 8",
	L"Squadra 9",
	L"Squadra 10",
	L"Squadra 11",
	L"Squadra 12",
	L"Squadra 13",
	L"Squadra 14",
	L"Squadra 15",
	L"Squadra 16",
	L"Squadra 17",
	L"Squadra 18",
	L"Squadra 19",
	L"Squadra 20",
	L"In servizio",
	L"Dottore",
	L"Paziente",
	L"veicolo",
	L"In transito",
	L"Riparare",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Esercitarsi", 
	L"Allenamento Esercito",
	L"Training Mobile Militia",	// TODO.Translate
	L"Allenatore",
	L"Studente",
	L"Get Item",	// get items	// TODO.Translate
	L"Facility Staff",			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Resting at Facility",		// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners	TODO.Translate
	L"Morto",
	L"Incap.",
	L"PDG",
	L"Ospedale", 
	L"Vuoto",	// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gathering Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spreading Propaganda",// TODO.Translate	// spread propaganda
	L"Gathering Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Dottore",
	L"Paziente",
	L"Riparare",
	L"Fortify sector",		// build structures according to external layout	// TODO.Translate
	L"Train workers",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
	L"Administration",	// TODO.Translate
	L"Exploration",	// TODO.Translate
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Squadra 1",
	L"Squadra 2",
	L"Squadra 3",
	L"Squadra 4",
	L"Squadra 5",
	L"Squadra 6",
	L"Squadra 7",
	L"Squadra 8",
	L"Squadra 9",
	L"Squadra 10",
	L"Squadra 11",
	L"Squadra 12",
	L"Squadra 13",
	L"Squadra 14",
	L"Squadra 15",
	L"Squadra 16",
	L"Squadra 17",
	L"Squadra 18",
	L"Squadra 19",
	L"Squadra 20",
	L"In servizio",
	L"Dottore",
	L"Paziente",
	L"Veicolo",
	L"In transito",
	L"Ripara",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Esercitarsi", 
	L"Allenatore esercito",
	L"Train Mobiles",		// TODO.Translate
	L"Allena squadra",
	L"Studente",
	L"Get Item",	// get items	// TODO.Translate
	L"Staff Facility",		// TODO.Translate
	L"Rest at Facility",	// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners TODO.Translate
	L"Morto",
	L"Incap.",
	L"PDG",
	L"Ospedale", // patient in a hospital 
	L"Vuoto",	// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gather Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spread Propaganda",// TODO.Translate	// spread propaganda
	L"Gather Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Dottore",
	L"Paziente",
	L"Ripara",
	L"Fortify sector",		// build structures according to external layout	// TODO.Translate
	L"Train workers",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
	L"Administration",	// TODO.Translate
	L"Exploration",	// TODO.Translate
};


// the contract options

STR16 pContractStrings[] =
{
	L"Opzioni del contratto:", 
	L"", // a blank line, required
	L"Offri 1 giorno", // offer merc a one day contract extension
	L"Offri 1 settimana", // 1 week
	L"Offri 2 settimane", // 2 week
	L"Termina contratto", // end merc's contract
	L"Annulla", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"PDG",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
  L"FORZA", 
	L"DESTREZZA",  
	L"AGILITÀ", 
	L"SAGGEZZA", 
	L"MIRA",
	L"PRONTO SOCC.",
	L"MECCANICA",
	L"COMANDO",
	L"ESPLOSIVI",
	L"LIVELLO",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Giubb. A-P", // the armor rating of the merc
	L"Peso", // the weight the merc is carrying
	L"Trav.", // the merc's camouflage rating
	L"Camouflage:",
	L"Protection:",
};

STR16 pShortAttributeStrings[] =
{
	L"Abi", // the abbreviated version of : agility
	L"Des", // dexterity
	L"For", // strength
	L"Com", // leadership
	L"Sag", // wisdom
	L"Liv", // experience level
	L"Tir", // marksmanship skill
	L"Mec", // mechanical skill
	L"Esp", // explosive skill
	L"PS", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Compito", // the mercs current assignment 
	L"Accordo", // the contract info about the merc
	L"Salute", // the health level of the current merc
	L"Morale", // the morale of the current merc
	L"Cond.",	// the condition of the current vehicle
	L"Benzina",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Esercitarsi", // tell merc to train self 
  L"Esercito", // tell merc to train town 
	L"Allenatore", // tell merc to act as trainer
	L"Studente", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Frequenza di fuoco:", // the allowable rate of fire for a merc who is guarding
	L"Fuoco aggressivo", // the merc can be aggressive in their choice of fire rates
	L"Conservare munizioni", // conserve ammo 
	L"Astenersi dal fuoco", // fire only when the merc needs to 
	L"Altre opzioni:", // other options available to merc
	L"Può ritrattare", // merc can retreat
	L"Può cercare rifugio",  // merc is allowed to seek cover
	L"Può assistere compagni di squadra", // merc can assist teammates
	L"Fine", // done with this menu
	L"Annulla", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Frequenza di fuoco:",
	L" *Fuoco aggressivo*",
	L" *Conservare munizioni*",
	L" *Astenersi dal fuoco*",
	L"Altre opzioni:",
	L" *Può ritrattare*",
	L" *Può cercare rifugio*",
	L" *Può assistere compagni di squadra*",
	L"Fine",
	L"Annulla",
};

STR16 pAssignMenuStrings[] =
{
	L"In servizio", // merc is on active duty
	L"Dottore", // the merc is acting as a doctor
	L"Disease", // merc is a doctor doing diagnosis TODO.Translate
	L"Paziente", // the merc is receiving medical attention
	L"Veicolo", // the merc is in a vehicle
	L"Ripara", // the merc is repairing items 
	L"Radio Scan", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Snitch", // TODO.Translate // anv: snitch actions
	L"Si esercita", // the merc is training
	L"Militia",		// all things militia
	L"Get Item",	// get items	// TODO.Translate
	L"Fortify",		// fortify sector	// TODO.Translate
	L"Intel", // covert assignments	// TODO.Translate
	L"Administer",	// TODO.Translate
	L"Explore",	// TODO.Translate
	L"Facility", // the merc is using/staffing a facility	// TODO.Translate
	L"Annulla", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Attack", // set militia to aggresive
	L"Hold Position", // set militia to stationary
	L"Retreat", // retreat militia
	L"Come to me", // retreat militia
	L"Get down", // retreat militia
	L"Crouch",	// TODO.Translate
	L"Take cover",
	L"Move to",	// TODO.Translate
	L"All: Attack", 
	L"All: Hold Position",
	L"All: Retreat",
	L"All: Come to me",
	L"All: Spread out",
	L"All: Get down",
	L"All: Crouch",	// TODO.Translate
	L"All: Take cover",
	//L"All: Find items",
	L"Cancel", // cancel this menu
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
	L"Radio: Activate all turncoats",

	// spy
	L"Hide assignment",		// TODO.Translate
	L"Get Intel assignment",
	L"Recruit turncoat",
	L"Activate turncoat",
	L"Activate all turncoats",

	// disguise
	L"Disguise",
	L"Remove disguise",
	L"Test disguise",
	L"Remove clothes",
	
	// various
	L"Spotter",		// TODO.Translate
	L"Focus",	// TODO.Translate
	L"Drag",
	L"Fill canteens",
};

//Flugente: short description of the above skills for the skill selection menu
STR16 pTraitSkillsMenuDescStrings[] =
{
	// radio operator
	L"Order an artillery strike from sector...",
	L"Fill all radio frequencies with white noise, making communications impossible.",
	L"Scan for jamming signals.",
	L"Use your radio equipment to continously listen for enemy movement.",
	L"Call in reinforcements from neighbouring sectors.",
	L"Turn off radio set.",	// TODO.Translate
	L"Order all previously turned soldiers in the sector to betray their comrades and join you.",

	// spy
	L"Assignment: hide among the population.",		// TODO.Translate
	L"Assignment: hide among the population and gather intel.",
	L"Try to turn an enemy into a turncoat.",
	L"Order previously turned soldier to betray their comrades and join you.",
	L"Order all previously turned soldiers in the sector to betray their comrades and join you.",

	// disguise
	L"Try to disguise with the merc's current clothes.",
	L"Remove the disguise, but clothes remain worn.",
	L"Test the viability of the disguise.",
	L"Remove any extra clothes.",

	// various
	L"Observe an area, granting allied snipers a bonus to cth on anything you see.",	// TODO.Translate
	L"Increase interrupt modifier (penalty outside of area).",	// TODO.Translate
	L"Drag a person, corpse or structure while you move.",
	L"Refill your squad's canteens with water from this sector.",
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
	L" - %s |o|r %s |a|n|d %s or %s or higher\n",
	L" - possession by a demon",
	L" - a gun-related trait\n",	// TODO.Translate
	L" - aimed gun\n",
	L" - prone person, corpse or structure next to merc\n",
	L" - crouched position\n",
	L" - free main hand\n",
	L" - covert trait\n",
	L" - enemy occupied sector\n",
	L" - single merc\n",
	L" - no alarm raised\n",
	L" - civilian or soldier disguise\n",
	L" - being our turn\n",
	L" - turned enemy soldier\n",
	L" - enemy soldier\n",
	L" - surface sector\n",
	L" - not being under suspicion\n",
	L" - not disguised\n",
	L" - not in combat\n",
	L" - friendly controlled sector\n",
};

STR16 pSkillMenuStrings[] =	// TODO.Translate
{
	L"Militia",
	L"Other Squads",
	L"Cancel",
	L"%d Militia",
	L"All Militia",

	L"More",	// TODO.Translate
	L"Corpse: %s",	// TODO.Translate
};

// TODO.Translate
STR16 pSnitchMenuStrings[] =
{
	// snitch
	L"Team Informant",
	L"Town Assignment",	
	L"Cancel",
};

STR16 pSnitchMenuDescStrings[] =
{
	// snitch
	L"Discuss snitch's behaviour towards his teammates.",
	L"Take an assignment in this sector.",
	L"Cancel",
};

STR16 pSnitchToggleMenuStrings[] =
{
	// toggle snitching
	L"Report complaints",
	L"Don't report",
	L"Prevent misbehaviour",
	L"Ignore misbehaviour",
	L"Cancel",
};

STR16 pSnitchToggleMenuDescStrings[] =
{
	L"Report any complaints you hear from other mercs to your commander.",
	L"Don't report anything.",
	L"Try to stop other mercs from getting wasted and scrounging.",
	L"Don't care what other mercs do.",
	L"Cancel",
};

STR16 pSnitchSectorMenuStrings[] =
{
	// sector assignments
	L"Spread propaganda",
	L"Gather rumours",
	L"Cancel",
};

STR16 pSnitchSectorMenuDescStrings[] =
{
	L"Glorify mercs' actions to increase town loyalty and suppress any bad news. ",
	L"Keep an ear to the ground on any rumours about enemy forces activity.",
	L"",
};

STR16 pPrisonerMenuStrings[] =	// TODO.Translate
{
	L"Interrogate admins",
	L"Interrogate troops",
	L"Interrogate elites",
	L"Interrogate officers",
	L"Interrogate generals",
	L"Interrogate civilians",
	L"Cancel",
};

STR16 pPrisonerMenuDescStrings[] =
{
	L"Administrators are easy to process, but give only poor results",
	L"Regular troops are common and don't give you high rewards.",
	L"If elite troops defect to you, they can become veteran militia.",
	L"Interrogating enemy officers can lead you to find enemy generals.",
	L"Generals cannot join your militia, but lead to high ransoms.",
	L"Civilians don't offer much resistance, but are second-rate troops at best.",
	L"Cancel",
};

STR16 pSnitchPrisonExposedStrings[] =
{
	L"%s was exposed as a snitch but managed to notice it and get out alive.",
	L"%s was exposed as a snitch but managed to defuse situation and get out alive.",
	L"%s was exposed as a snitch but managed to avoid assassination attempt.",
	L"%s was exposed as a snitch but guards managed to prevent any violence outbursts.",

	L"%s was exposed as a snitch and almost drowned by other inmates before guards saved him.",
	L"%s was exposed as a snitch and almost beaten to death before guards saved him.",
	L"%s was exposed as a snitch and almost stabbed to death before guards saved him.",
	L"%s was exposed as a snitch and strangled to death before guards saved him.",

	L"%s was exposed as a snitch and drowned in toilet by other inmates.",
	L"%s was exposed as a snitch and beaten to death by other inmates.",
	L"%s was exposed as a snitch and shanked to death by other inmates.",
	L"%s was exposed as a snitch and strangled to death by other inmates.",
};

STR16 pSnitchGatheringRumoursResultStrings[] =
{
	L"%s heard rumours about enemy activity in %d sectors.",

};
// /TODO.Translate

STR16 pRemoveMercStrings[] =
{
	L"Rimuovi Mercenario", // remove dead merc from current team
	L"Annulla",
};

STR16 pAttributeMenuStrings[] =
{
	L"Salute",
	L"Agilità",
	L"Destrezza",
	L"Forza",
	L"Comando",
	L"Mira",
	L"Meccanica",
	L"Esplosivi",
	L"Pronto socc.",
	L"Annulla",
};

STR16 pTrainingMenuStrings[] =
{
	L"Allenati", // train yourself
	L"Train workers",	// TODO.Translate
	L"Allenatore", // train your teammates 
	L"Studente",  // be trained by an instructor 
	L"Annulla", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Squadra  1",
	L"Squadra  2",
	L"Squadra  3",
	L"Squadra  4",
	L"Squadra  5",
	L"Squadra  6",
	L"Squadra  7",
	L"Squadra  8",
	L"Squadra  9",
	L"Squadra 10",
	L"Squadra 11",
	L"Squadra 12",
	L"Squadra 13",
	L"Squadra 14",
	L"Squadra 15",
	L"Squadra 16",
	L"Squadra 17",
	L"Squadra 18",
	L"Squadra 19",
	L"Squadra 20",
	L"Annulla",
};

STR16 pPersonnelTitle[] =
{
	L"Personale", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Salute: ", // health of merc
	L"Agilità: ", 
	L"Destrezza: ",
	L"Forza: ",
	L"Comando: ",
	L"Saggezza: ",
	L"Liv. esp.: ", // experience level
	L"Mira: ", 
	L"Meccanica: ",
	L"Esplosivi: ",
	L"Pronto socc.: ",
	L"Deposito med.: ", // amount of medical deposit put down on the merc
	L"Contratto in corso: ", // cost of current contract
	L"Uccisi: ", // number of kills by merc
	L"Assistiti: ", // number of assists on kills by merc
	L"Costo giornaliero:", // daily cost of merc
	L"Tot. costo fino a oggi:", // total cost of merc
	L"Contratto:", // cost of current contract
	L"Tot. servizio fino a oggi:", // total service rendered by merc
	L"Salario arretrato:", // amount left on MERC merc to be paid
	L"Percentuale di colpi:", // percentage of shots that hit target
	L"Battaglie:", // number of battles fought
	L"Numero ferite:", // number of times merc has been wounded
	L"Destrezza:",
	L"Nessuna abilità",
	L"Achievements:", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Elite soldiers: %d\n",
	L"Regular soldiers: %d\n",
	L"Admin soldiers: %d\n",
	L"Hostile groups: %d\n",
	L"Creatures: %d\n",
	L"Tanks: %d\n", 
	L"Others: %d\n",

	L"Mercs: %d\n",
	L"Militia: %d\n",
	L"Others: %d\n",

	L"Shots fired: %d\n",
	L"Missiles fired: %d\n",
	L"Grenades thrown: %d\n",
	L"Knives thrown: %d\n",
	L"Blade attacks: %d\n",
	L"Hand to hand attacks: %d\n",
	L"Successful hits: %d\n",

	L"Locks picked: %d\n",
	L"Locks breached: %d\n",
	L"Traps removed: %d\n",
	L"Explosives detonated: %d\n",
	L"Items repaired: %d\n",
	L"Items combined: %d\n",
	L"Items stolen: %d\n",
	L"Militia trained: %d\n",
	L"Mercs bandaged: %d\n",
	L"Surgeries made: %d\n",
	L"Persons met: %d\n",
	L"Sectors discovered: %d\n",
	L"Ambushes prevented: %d\n",
	L"Quests handled: %d\n",

	L"Tactical battles: %d\n",
	L"Autoresolve battles: %d\n",
	L"Times retreated: %d\n",
	L"Ambushes experienced: %d\n",
	L"Hardest battle: %d Enemies\n",

	L"Shot: %d\n",
	L"Stabbed: %d\n",
	L"Punched: %d\n",
	L"Blasted: %d\n",
	L"Suffered damages in facilities: %d\n",
	L"Surgeries undergone: %d\n",
	L"Facility accidents: %d\n",

	L"Character:",
	L"Weakness:",

	L"Attitudes:",	// WANNE: For old traits display instead of "Character:"!

	L"Zombies: %d\n", // TODO.Translate

	L"Background:", // TODO.Translate
	L"Personality:", // TODO.Translate

	L"Prisoners interrogated: %d\n",	// TODO.Translate
	L"Diseases caught: %d\n",
	L"Total damage received: %d\n",
	L"Total damage caused: %d\n",
	L"Total healing: %d\n",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Nessuna abilità",
	L"Forzare serrature",
	L"Corpo a corpo",
	L"Elettronica",
	L"Op. notturne",
	L"Lanciare",
	L"Istruire",
	L"Armi pesanti",
	L"Armi automatiche",
	L"Clandestino",
	L"Ambidestro",
	L"Furtività",
	L"Arti marziali",
	L"Coltelli",
	L"Sniper",
	L"Camuffato",
	L"(Esperto)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"No Skill",		// 0
	L"Auto Weapons",	// 1
	L"Heavy Weapons",
	L"Marksman",
	L"Hunter",
	L"Gunslinger",		// 5
	L"Hand to Hand",
	L"Deputy",
	L"Technician",
	L"Paramedic",		// 9	
	// Minor traits
	L"Ambidextrous",	// 10
	L"Melee",
	L"Throwing",
	L"Night Ops",
	L"Stealthy",
	L"Athletics",		// 15
	L"Bodybuilding",
	L"Demolitions",
	L"Teaching",
	L"Scouting",		// 19
	// covert ops is a major trait that was added later
	L"Covert Ops",		// 20
	// new minor traits
	L"Radio Operator",	// 21
	L"Snitch",	// 22
	L"Survival",

	// second names for major skills
	L"Machinegunner",	// 24
	L"Bombardier",
	L"Sniper",
	L"Ranger",
	L"Gunfighter",
	L"Martial Arts",
	L"Squadleader",
	L"Engineer",
	L"Doctor",
	// placeholders for minor traits
	L"Placeholder",		// 33
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 42
	L"Spy",				// 43
	L"Placeholder",		// for radio operator (minor trait)
	L"Placeholder",		// for snitch (minor trait)
	L"Placeholder",		// for survival (minor trait)
	L"More...",			// 47
	L"Intel",			// for INTEL	// TODO.Translate
	L"Disguise",		// for DISGUISE
	L"various",			// for VARIOUSSKILLS
	L"Bandage Mercs",	// for AUTOBANDAGESKILLS	//TODO.Translate
};
//////////////////////////////////////////////////////////

// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"|Stare fermi/Camminare",
	L"|Accucciarsi/Muoversi accucciato",
	L"Stare fermi/|Correre",
	L"|Prono/Strisciare",
	L"|Guardare",
	L"Agire",
	L"Parlare",
	L"Esaminare (|C|t|r|l)",

	// Pop up door menu
	L"Aprire manualmente",
	L"Esaminare trappole",
	L"Grimaldello",
	L"Forzare",
	L"Liberare da trappole",
	L"Chiudere",
	L"Aprire",
	L"Usare esplosivo per porta",
	L"Usare piede di porco",
	L"Annulla (|E|s|c)",
	L"Chiudere",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"Nessuna trappola",
	L"una trappola esplosiva",
	L"una trappola elettrica",
	L"una trappola con sirena",
	L"una trappola con allarme insonoro",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"giorno",
	L"settimana",
	L"due settimane",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Selezionare postazioni",
	L"Assegnare mercenario",
	L"Tracciare percorso di viaggio",
	L"Merc |Contratto",
	L"Eliminare mercenario",
	L"Dormire", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"DEBOLE",
	L"DEFINITO",
	L"FORTE",
	L"MOLTO FORTE",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"SCONOSCIUTO",
	L"rumore di MOVIMENTO",
	L"SCRICCHIOLIO",
	L"TONFO IN ACQUA",
	L"IMPATTO",
	L"SPARO",
	L"ESPLOSIONE",
	L"URLA",
	L"IMPATTO",
	L"IMPATTO",
	L"FRASTUONO",
	L"SCHIANTO",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"il NORD-EST",
	L"il EST",
	L"il SUD-EST",
	L"il SUD",
	L"il SUD-OVEST",
	L"il OVEST",
	L"il NORD-OVEST",
	L"il NORD",
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Urbano",
	L"Strada",
	L"Pianure",
	L"Deserto",
	L"Boschi",
	L"Foresta",	 
	L"Palude",
	L"Acqua",	
	L"Colline",
	L"Impervio",
	L"Fiume",	//river from north to south
	L"Fiume",	//river from east to west
	L"Paese straniero",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Tropicale",
	L"Campi",
	L"Pianure, strada",
	L"Boschi, strada",
	L"Fattoria, strada",
	L"Tropicale, strada",
	L"Foresta, strada",
	L"Linea costiera",
	L"Montagna, strada",
	L"Litoraneo, strada",
	L"Deserto, strada",
	L"Palude, strada",
	L"Boschi, postazione SAM",
	L"Deserto, postazione SAM",
	L"Tropicale, postazione SAM",
	L"Meduna, postazione SAM",
	
	//These are descriptions for special sectors
	L"Ospedale di Cambria",
	L"Aeroporto di Drassen",
	L"Aeroporto di Meduna",
	L"Postazione SAM",
	L"Refuel site", // TODO.Translate
	L"Nascondiglio ribelli", //The rebel base underground in sector A10
	L"Prigione sotterranea di Tixa",	//The basement of the Tixa Prison (J9)
	L"Tana della creatura",	//Any mine sector with creatures in it
	L"Cantina di Orta",	//The basement of Orta (K4)
	L"Tunnel",				//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Rifugio",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s sono stati individuati nel settore %c%d e un'altra squadra sta per arrivare.",	//STR_DETECTED_SINGULAR
	L"%s sono stati individuati nel settore %c%d e un'altra squadra sta per arrivare.",	//STR_DETECTED_PLURAL
	L"Volete coordinare un attacco simultaneo?",													//STR_COORDINATE

	//Dialog strings for enemies.

	L"Il nemico offre la possibilità di arrendervi.",			//STR_ENEMY_SURRENDER_OFFER
	L"Il nemico ha catturato i vostri mercenari sopravvissuti.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Ritirarsi", 		//The retreat button				//STR_AR_RETREAT_BUTTON
	L"Fine",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"DIFENDERE",								//STR_AR_DEFEND_HEADER
	L"ATTACCARE",								//STR_AR_ATTACK_HEADER
	L"INCONTRARE",								//STR_AR_ENCOUNTER_HEADER
	L"settore",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"VITTORIA!",								//STR_AR_OVER_VICTORY
	L"SCONFITTA!",								//STR_AR_OVER_DEFEAT
	L"ARRENDERSI!",							//STR_AR_OVER_SURRENDERED
	L"CATTURATI!",								//STR_AR_OVER_CAPTURED
	L"RITIRARSI!",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Esercito",							//STR_AR_MILITIA_NAME,
	L"Èlite",								//STR_AR_ELITE_NAME,
	L"Truppa",								//STR_AR_TROOP_NAME,
	L"Amministratore",								//STR_AR_ADMINISTRATOR_NAME,
	L"Creatura",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Tempo trascorso",							//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"RITIRATOSI",								//STR_AR_MERC_RETREATED,
	L"RITIRARSI",								//STR_AR_MERC_RETREATING,
	L"RITIRATA",								//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Esito",							//STR_PB_AUTORESOLVE_BTN,
	L"Vai al settore",							//STR_PB_GOTOSECTOR_BTN,
	L"Ritira merc.",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"SCONTRO NEMICO",							//STR_PB_ENEMYENCOUNTER_HEADER,
	L"INVASIONE NEMICA",							//STR_PB_ENEMYINVASION_HEADER, // 30
	L"IMBOSCATA NEMICA",								//STR_PB_ENEMYAMBUSH_HEADER
	L"INTRUSIONE NEMICA NEL SETTORE",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"ATTACCO DELLE CREATURE",							//STR_PB_CREATUREATTACK_HEADER
	L"IMBOSCATA DEI BLOODCAT",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"INTRUSIONE NELLA TANA BLOODCAT",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER
	L"ENEMY AIRDROP",					//STR_PB_ENEMYINVASION_AIRDROP_HEADER	// TODO.Translate

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Postazione",
	L"Nemici",
	L"Mercenari",
	L"Esercito",
	L"Creature",
	L"Bloodcat",
	L"Settore",
	L"Nessuno",		//If there are no uninvolved mercs in this fight.
	L"N/A",			//Acronym of Not Applicable
	L"g",			//One letter abbreviation of day
	L"o",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Sgombro",
	L"Sparsi",
	L"In gruppo",
	L"Fine",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"|Mostra chiaramente tutte le postazioni dei mercenari, \ne vi permette di rimetterli in gioco manualmente.", 
	L"A caso |sparge i vostri mercenari \nogni volta che lo premerete.",
	L"Vi permette di scegliere dove vorreste |raggruppare i vostri mercenari.",
	L"Cliccate su questo pulsante quando avrete \nscelto le postazioni dei vostri mercenari. (|I|n|v|i|o)",
	L"Dovete posizionare tutti i vostri mercenari \nprima di iniziare la battaglia.",

	//Various strings (translate word for word)

	L"Settore",
	L"Scegliete le postazioni di intervento",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Non sembra così bello qui. È inacessibile. Provate con una diversa postazione.",
	L"Posizionate i vostri mercenari nella sezione illuminata della mappa.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"è arivato nel settore", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"|Automaticamente svolge i combattimenti al vostro posto\nsenza caricare la mappa.",
	L"Non è possibile utilizzare l'opzione di risoluzione automatica quando\nil giocatore sta attaccando.",
	L"|Entrate nel settore per catturare il nemico.",
	L"|Rimandate il gruppo al settore precedente.",				//singular version
	L"|Rimandate tutti i gruppi ai loro settori precedenti.", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"I nemici attaccano il vostro esercito nel settore %c%d.",
	//%c%d is the sector -- ex:  A9
	L"Le creature attaccano il vostro esercito nel settore %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Le creature attaccano e uccidono %d civili nel settore %s.",
	//%s is the sector location -- ex:  A9: Omerta
	L"I nemici attaccano i vostri mercenari nel settore %s. Nessuno dei vostri mercenari è in grado di combattere!",
	//%s is the sector location -- ex:  A9: Omerta
	L"I nemici attaccano i vostri mercenari nel settore %s. Nessuno dei vostri mercenari è in grado di combattere!",

	// Flugente: militia movement forbidden due to limited roaming	// TODO.Translate
	L"Militia cannot move here (RESTRICT_ROAMING = TRUE).",
	L"War room isn't staffed - militia move aborted!",

	L"Robot",								//STR_AR_ROBOT_NAME, TODO: translate
	L"Tank",								//STR_AR_TANK_NAME,
	L"Jeep",								//STR_AR_JEEP_NAME	// TODO.Translate

	L"\nBreath regeneration per hour: %d",	// STR_BREATH_REGEN_SLEEP

	L"Zombies",		// TODO.Translate
	L"Bandits",
	L"BLOODCAT ATTACK",
	L"ZOMBIE ATTACK",
	L"BANDIT ATTACK",
	L"Zombie",
	L"Bandit",
	L"Bandits attack and kill %d civilians in sector %s.",
};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"Gg",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Settore trovato:",
	L"Giorno trovato:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 20 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Stato:",
	L"Peso:", 
	L"AP Costs",	
	L"Git:",		// Range
	L"Dan:",		// Damage
	L"Ammontare:", 		// Number of bullets left in a magazine
	L"PA:",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Accuracy:",	//9
	L"Range:",		//10	
	L"Damage:", 	//11
	L"Weight:",		//12
	L"Stun Damage:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Attachments:",	//14	// TODO.Translate
	L"AUTO/5:",		//15
	L"Remaining ammo:",		//16	// TODO.Translate

	// TODO.Translate
	L"Default:",	//17 //WarmSteel - So we can also display default attachments
	L"Dirt:",	// 18	//added by Flugente	// TODO.Translate
	L"Space:", // 19 //space left on Molle items // TODO.Translate
	L"Spread Pattern:",	// 20	// TODO.Translate

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
	L"APs to ready",
	L"APs to fire Single",
	L"APs to fire Burst",
	L"APs to fire Auto",
	L"APs to Reload",
	L"APs to Reload Manually",
	L"Burst Penalty (Lower is better)",	//19
	L"Bipod Modifier",
	L"Autofire shots per 5 AP",
	L"Autofire Penalty (Lower is better)",
	L"Burst/Auto Penalty (Lower is better)",	//23
	L"APs to Throw",
	L"APs to Launch",
	L"APs to Stab",
	L"No Single Shot!",
	L"No Burst Mode!",
	L"No Auto Mode!",
	L"APs to Bash",
    L"",
	L"|R|e|p|a|i|r |E|a|s|e\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16		gzMiscItemStatsFasthelp[] =
{
	L"Item Size Modifier (Lower is better)", // 0
	L"Reliability Modifier",
	L"Loudness Modifier (Lower is better)",
	L"Hides Muzzle Flash",
	L"Bipod Modifier",
	L"Range Modifier", // 5
	L"To-Hit Modifier",
	L"Best Laser Range",
	L"Aiming Bonus Modifier",
	L"Burst Size Modifier",
	L"Burst Penalty Modifier (Higher is better)", // 10
	L"Auto-Fire Penalty Modifier (Higher is better)",
	L"AP Modifier",
	L"AP to Burst Modifier (Lower is better)",
	L"AP to Auto-Fire Modifier (Lower is better)",
	L"AP to Ready Modifier (Lower is better)", // 15
	L"AP to Reload Modifier (Lower is better)",
	L"Magazine Size Modifier",
	L"AP to Attack Modifier (Lower is better)",
	L"Damage Modifier",
	L"Melee Damage Modifier", // 20
	L"Woodland Camo",
	L"Urban Camo",
	L"Desert Camo",
	L"Snow Camo",
	L"Stealth Modifier", // 25
	L"Hearing Range Modifier",
	L"Vision Range Modifier",
	L"Day Vision Range Modifier",
	L"Night Vision Range Modifier",
	L"Bright Light Vision Range Modifier", //30
	L"Cave Vision Range Modifier",
	L"Tunnel Vision Percentage (Lower is better)",
	L"Minimum Range for Aiming Bonus",
	L"Hold |C|t|r|l to compare items", // item compare help text
	L"Equipment weight: %4.1f kg",	// 35	// TODO.Translate
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"In ",
	L"PERFECT",
	L"EXCELLENT",
	L"GOOD",
	L"FAIR",
	L"POOR",
	L"BAD",
	L"TERRIBLE",
	L" condition."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Ammontare",
	L"Rimanenti:", //this is the overall balance
	L"Ammontare",
	L"Da separare:", // the amount he wants to separate from the overall balance to get two piles of money

	L"Bilancio",
	L"corrente:",
	L"Ammontare",
	L"del prelievo:",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"MORENTE",		//	>= 0
	L"CRITICO", 		//	>= 15
	L"DEBOLE",		//	>= 30
	L"FERITO",    	//	>= 45
	L"SANO",    	//	>= 60
	L"FORTE",     	// 	>= 75
	L"ECCELLENTE",		// 	>= 90
	L"CAPTURED",	// added by Flugente TODO.Translate
};

STR16 gzHiddenHitCountStr[1] =
{
	L"?",	
};

STR16	gzMoneyAmounts[6] = 
{ 
	L"$1000",
	L"$100",
	L"$10",
	L"Fine",
	L"Separare",
	L"Prelevare",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons." 
CHAR16		gzProsLabel[10] = 
{
	L"Vant.:",
};

CHAR16		gzConsLabel[10] = 
{
	L"Svant.:",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Vuoi ripetere?", 	//meaning "Repeat yourself" 
	L"Amichevole",		//approach in a friendly
	L"Diretto",		//approach directly - let's get down to business
	L"Minaccioso",		//approach threateningly - talk now, or I'll blow your face off
	L"Dai",		
	L"Recluta",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Compra/Vendi",
	L"Compra",
	L"Vendi",
	L"Ripara",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"Fine",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"Eldorado",
 L"Hummer", // a hummer jeep/truck -- military vehicle
 L"Icecream Truck",
 L"Jeep",
 L"Carro armato",
 L"Elicottero",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Truck",
	L"Jeep",
	L"Carro",
	L"Eli", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Truck",			// Ice cream truck
	L"Jeep",
	L"Carro",
	L"Eli", 		//an abbreviation for Helicopter
};

STR16 pVehicleSeatsStrings[] =
{
	 L"You cannot shoot from this seat.",	// TODO.Translate
	 L"You cannot swap those two seats in combat without exiting vehicle first.",	// TODO.Translate
};

//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Attacco aereo",
	L"Ricorrete al pronto soccorso automaticamente?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s nota ch egli oggetti mancano dall'equipaggiamento.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"La serratura ha %s", 
	L"Non ci sono serrature",
	L"Vittoria!",
	L"Fallimento",
	L"Vittoria!",
	L"Fallimento",
	L"La serratura non presenta trappole",
	L"Vittoria!",
	// The %s is a merc name
	L"%s non ha la chiave giusta",
	L"La serratura non presenta trappole",
	L"La serratura non presenta trappole",
	L"Serrato",
	L"",
	L"TRAPPOLE",
	L"SERRATO",
	L"APERTO",
	L"FRACASSATO",
	L"C'è un interruttore qui. Lo volete attivare?",
	L"Disattivate le trappole?",
	L"Prec...",
	L"Succ...",
	L"Più...",

	// In the next 2 strings, %s is an item name

	L"Il %s è stato posizionato sul terreno.",
	L"Il %s è stato dato a %s.",

	// In the next 2 strings, %s is a name

	L"%s è stato pagato completamente.",
	L"Bisogna ancora dare %d a %s.",
	L"Scegliete la frequenza di detonazione:",  	//in this case, frequency refers to a radio signal
	L"Quante volte finché la bomba non esploderà:",	//how much time, in turns, until the bomb blows
	L"Stabilite la frequenza remota di detonazione:", 	//in this case, frequency refers to a radio signal
	L"Disattivate le trappole?",
	L"Rimuovete la bandiera blu?",
	L"Mettete qui la bandiera blu?",
	L"Fine del turno",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Siete sicuri di volere attaccare %s ?",
	L"Ah, i veicoli non possono cambiare posizione.",
	L"Il robot non può cambiare posizione.",

	// In the next 3 strings, %s is a name

	L"%s non può cambiare posizione.",
	L"%s non sono ricorsi al pronto soccorso qui.",	
	L"%s non ha bisogno del pronto soccorso.",
	L"Non può muoversi là.",
	L"La vostra squadra è al completo. Non c'è spazio per una recluta.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s è stato reclutato.",

	// Here %s is a name and %d is a number

	L"Bisogna dare %d a $%s.",

	// In the next string, %s is a name

	L"Scortate %s?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"Il salario di %s ammonta a %s per giorno?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Volete combattere?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"Comprate %s per %s?",

	// In the next string, %s is a name

	L"%s è scortato dalla squadra %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"INCEPPATA",					//weapon is jammed.
	L"Il robot ha bisogno di munizioni calibro %s.",		//Robot is out of ammo
	L"Cosa? Impossibile.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Modalità furtiva (|Z)",
	L"Schermata della |mappa",
	L"Fine del turno (|D)",
	L"Parlato",
	L"Muto",
	L"Alza (|P|a|g|S|ù)",
	L"Livello della vista (|T|a|b)",
	L"Scala / Salta",
	L"Abbassa (|P|a|g|G|i|ù)",
	L"Esamina (|C|t|r|l)",
	L"Mercenario precedente",
	L"Prossimo mercenario (|S|p|a|z|i|o)",
	L"|Opzioni",
	L"Modalità a raffica (|B)",
	L"Guarda/Gira (|L)",
	L"Salute: %d/%d\nEnergia: %d/%d\nMorale: %s",
	L"Eh?",					//this means "what?" 
	L"Fermo",					//an abbrieviation for "Continued" 
	L"Audio on per %s.",
	L"Audio off per %s.",
	L"Salute: %d/%d\nCarburante: %d/%d",
	L"Uscita veicoli" ,
	L"Cambia squadra (|M|a|i|u|s|c |S|p|a|z|i|o)",
	L"Guida",
	L"N/A",						//this is an acronym for "Not Applicable." 
	L"Usa (Corpo a corpo)",
	L"Usa (Arma da fuoco)",
	L"Usa (Lama)",
	L"Usa (Esplosivo)",
	L"Usa (Kit medico)",
	L"Afferra",
	L"Ricarica",
	L"Dai",
	L"%s è partito.",
	L"%s è arrivato.",
	L"%s ha esaurito i Punti Azione.",
	L"%s non è disponibile.",
	L"%s è tutto bendato.",
	L"%s non è provvisto di bende.",
	L"Nemico nel settore!",
	L"Nessun nemico in vista.",
	L"Punti Azione insufficienti.",
	L"Nessuno sta utilizzando il comando a distanza.",
	L"Il fuoco a raffica ha svuotato il caricatore!",
	L"SOLDATO",
	L"CREPITUS",
	L"ESERCITO",
	L"CIVILE",
	L"ZOMBIE",		// TODO.Translate
	L"Settore di uscita",
	L"OK",
	L"Annulla",
	L"Merc. selezionato",
	L"Tutta la squadra",
	L"Vai nel settore",
	L"Vai alla mappa",
	L"Non puoi uscire dal settore da questa parte.",
	L"You can't leave in turn based mode.",		// TODO.Translate
	L"%s è troppo lontano.",
	L"Rimuovi le fronde degli alberi",
	L"Mostra le fronde degli alberi",
	L"CORVO",				//Crow, as in the large black bird
	L"COLLO",
	L"TESTA",
	L"TORSO",
	L"GAMBE",
	L"Vuoi dire alla Regina cosa vuole sapere?",
	L"Impronta digitale ID ottenuta",
	L"Impronta digitale ID non valida. Arma non funzionante",
	L"Raggiunto scopo",
	L"Sentiero bloccato",
	L"Deposita/Preleva soldi",		//Help text over the $ button on the Single Merc Panel 
	L"Nessuno ha bisogno del pronto soccorso.",
	L"Bloccato.",											// Short form of JAMMED, for small inv slots
	L"Non può andare là.",					// used ( now ) for when we click on a cliff
	L"Il sentiero è bloccato. Vuoi scambiare le posizioni con questa persona?",
	L"La persona rifiuta di muoversi.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Sei d'accordo a pagare %s?",
	L"Accetti il trattamento medico gratuito?",
	L"Vuoi sposare %s?", //Daryl
	L"Quadro delle chiavi",
	L"Non puoi farlo con un EPC.",
	L"Risparmi %s?", //Krott
	L"Fuori dalla gittata dell'arma",
	L"Minatore",
	L"Il veicolo può viaggiare solo tra i settori",
	L"Non è in grado di fasciarsi da solo ora",
	L"Sentiero bloccato per %s",
	L"I mercenari catturati dall'esercito di %s, sono stati imprigionati qui!", //Deidranna
	L"Serratura manomessa",
	L"Serratura distrutta",
	L"Qualcun altro sta provando a utilizzare questa porta.",
	L"Salute: %d/%d\nCarburante: %d/%d",
	L"%s non riesce a vedere %s.",  // Cannot see person trying to talk to
	L"Attachment removed",
	L"Non può guadagnare un altro veicolo poichè già avete 2",

	// added by Flugente for defusing/setting up trap networks	// TODO.Translate
	L"Choose detonation frequency (1 - 4) or defuse frequency (A - D):",
	L"Set defusing frequency:",
	L"Set detonation frequency (1 - 4) and defusing frequency (A - D):",
	L"Set detonation time in turns (1 - 4) and defusing frequency (A - D):",
	L"Select tripwire hierarchy (1 - 4) and network (A - D):",
	
	// added by Flugente to display food status
	L"Health: %d/%d\nEnergy: %d/%d\nMorale: %s\nWater: %d%s\nFood: %d%s",

	// added by Flugente: selection of a function to call in tactical
	L"What do you want to do?",
	L"Fill canteens",
	L"Clean guns (Merc)",
	L"Clean guns (Team)",
	L"Take off clothes",
	L"Lose disguise",
	L"Militia inspection",
	L"Militia restock",
	L"Test disguise",
	L"unused",

	// added by Flugente: decide what to do with the corpses
	L"What do you want to do with the body?",
	L"Decapitate",
	L"Gut",
	L"Take Clothes",
	L"Take Body",

	// Flugente: weapon cleaning
	L"%s cleaned %s",

	// added by Flugente: decide what to do with prisoners
	L"As we have no prison, a field interrogation is performed.",		// TODO.Translate
	L"Field interrogation",
	L"Where do you want to send the %d prisoners?",			// TODO.Translate
	L"Let them go",
	L"What do you want to do?",
	L"Demand surrender",
	L"Offer surrender",
	L"Distract",	// TODO.Translate
	L"Talk",
	L"Recruit Turncoat", // TODO: translate

	// TODO.Translate
    // added by sevenfm: disarm messagebox options, messages when arming wrong bomb
    L"Disarm trap",
    L"Inspect trap",
    L"Remove blue flag",
    L"Blow up!",
    L"Activate tripwire",
    L"Deactivate tripwire",
    L"Reveal tripwire",
    L"No detonator or remote detonator found!",
    L"This bomb is already armed!",
    L"Safe",
    L"Mostly safe",
    L"Risky",
    L"Dangerous",
    L"High danger!",

	L"Mask",	// TODO.Translate
    L"NVG",
    L"Item",
        
    L"This feature works only with New Inventory System",
    L"No item in your main hand",
    L"Nowhere to place item from main hand",
    L"No defined item for this quick slot",
    L"No free hand for new item",
    L"Item not found",
    L"Cannot take item to main hand",

	L"Attempting to bandage travelling mercs...",	//TODO.Translate

	L"Improve gear",
	L"%s changed %s for superior version",
	L"%s picked up %s",	// TODO.Translate

	L"%s has stopped chatting with %s", // TODO.Translate
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"Se selezionato, il settore adiacente verrà immediatamente caricato.",
	L"Se selezionato, sarete automaticamente posti nella schermata della mappa\nvisto che i vostri mercenari avranno bisogno di tempo per viaggiare.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Questo settore è occupato da nemicie non potete lasciare mercenari qui.\nDovete risolvere questa situazione prima di caricare qualsiasi altro settore.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Rimuovendo i vostri mercenari da questo settore,\nil settore adiacente verrà immediatamente caricato.",
	L"Rimuovendo i vostri mercenari da questo settore,\nverrete automaticamente postinella schermata della mappa\nvisto che i vostri mercenari avranno bisogno di tempo per viaggiare.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s ha bisogno di essere scortato dai vostri mercenari e non può lasciare questo settore da solo.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s non può lasciare questo settore da solo, perché sta scortando %s.", //male singular
	L"%s non può lasciare questo settore da solo, perché sta scortando %s.", //female singular
	L"%s non può lasciare questo settore da solo, perché sta scortando altre persone.", //male plural
	L"%s non può lasciare questo settore da solo, perché sta scortando altre persone.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Tutti i vostri personaggi devono trovarsi nei paraggi\nin modo da permettere alla squadra di attraversare.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Se selezionato, %s viaggerà da solo, e\nautomaticamente verrà riassegnato a un'unica squadra.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Se selezionato, la vostra \nsquadra attualmente selezionata viaggerà, lasciando questo settore.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s è scortato dai vostri mercenari e non può lasciare questo settore da solo. Gli altri vostri mercenari devono trovarsi nelle vicinanze prima che possiate andarvene.",
};



STR16 pRepairStrings[] = 
{
	L"Oggetti", 		// tell merc to repair items in inventory
	L"Sito SAM", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Annulla", 		// cancel this menu
	L"Robot", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 

STR16 sPreStatBuildString[] =
{
	L"perduto", 			// the merc has lost a statistic
	L"guadagnato", 		// the merc has gained a statistic
	L"punto di",	// singular
	L"punti di",	// plural
	L"livello di",	// singular
	L"livelli di",	// plural
};

STR16 sStatGainStrings[] =
{
	L"salute.",
	L"agilità.",
	L"destrezza.",
	L"saggezza.",
	L"pronto socc.",
	L"abilità esplosivi.",
	L"abilità meccanica.",
	L"mira.",
	L"esperienza.",
	L"forza.",
	L"comando.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Distanza totale: ", 			// total distance for helicopter to travel
	L"Sicura: ", 			// distance to travel to destination
	L"Insicura: ", 			// distance to return from destination to airport
	L"Costo totale: ", 		// total cost of trip by helicopter
	L"TPA: ", 			// ETA is an acronym for "estimated time of arrival" 
	L"L'elicottero ha poco carburante e deve atterrare in territorio nemico!",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
	L"Passeggeri: ",
	L"Seleziona Skyrider o gli Arrivi Drop-off?",
	L"Skyrider",
	L"Arrivi",
	L"Helicopter is seriously damaged and must land in hostile territory!",	// warning that the sector the helicopter is going to use for refueling is under enemy control ->	// TODO.Translate
	L"Helicopter will now return straight to base, do you want to drop down passengers before?",	// TODO.Translate
	L"Remaining Fuel:",	// TODO.Translate
	L"Dist. To Refuel Site:",	// TODO.Translate
};

STR16 pHelicopterRepairRefuelStrings[]=
{
	// anv: Waldo The Mechanic - prompt and notifications
	L"Do you want %s to start repairs? It will cost $%d, and helicopter will be unavailable for around %d hour(s).",
	L"Helicopter is currently disassembled. Wait until repairs are finished.",
	L"Repairs completed. Helicopter is available again.",
	L"Helicopter is fully refueled.",

	L"Helicopter has exceeded maximum range!",	// TODO.Translate
};

STR16 sMapLevelString[] =
{
	L"Sottolivello:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Leale", 			// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"non può portare ordini di viaggio sottoterra.", 
};

STR16 gsTimeStrings[] =
{
	L"h",				// hours abbreviation
	L"m",				// minutes abbreviation
	L"s",				// seconds abbreviation
	L"g",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"Nessuno",
	L"Ospedale",
	L"Factory",	// TODO.Translate
	L"Prigione",
	L"Militare",
	L"Aeroporto",
	L"Frequenza di fuoco",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Inventario",
	L"Uscita",
	L"Repair",	// TODO.Translate
	L"Factories",	// TODO.Translate
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Dimensione",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Controllo", 					// how much of town is controlled
	L"Nessuno", 					// none of this town
	L"Miniera", 				// mine associated with this town
	L"Lealtà",					// 5 // the loyalty level of this town
	L"Addestrato", 					// the forces in the town trained by the player
	L"",
	L"Servizi principali", 				// main facilities in this town
	L"Livello", 					// the training level of civilians in this town
	L"addestramento civili",				// 10 // state of civilian training in town
	L"Esercito", 					// the state of the trained civilians in the town
	
	// Flugente: prisoner texts	// TODO.Translate
	L"Prisoners",
	L"%d (capacity %d)",
	L"%d Admins",
	L"%d Regulars",
	L"%d Elites",
	L"%d Officers",
	L"%d Generals",
	L"%d Civilians",
	L"%d Special1",
	L"%d Special2",
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"Miniera",						// 0
	L"Argento",
	L"Oro",
	L"Produzione giornaliera",
	L"Produzione possibile",
	L"Abbandonata",				// 5
	L"Chiudi",
	L"Esci",
	L"Produci",
	L"Stato",
	L"Ammontare produzione",
	L"Resource",				// 10	L"Tipo di minerale",	// TODO.Translate
	L"Controllo della città",
	L"Lealtà della città",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Forze nemiche",
	L"Settore",
	L"# di oggetti",
	L"Sconosciuto",

	L"Controllato",
	L"Sì",
	L"No",
	L"Status/Software status:",	// TODO.Translate

	L"Additional Intel", // TODO:Translate
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s non è abbastanza vicino.",	//Merc is in sector with item but not close enough
	L"Non può selezionare quel mercenario.",  //MARK CARTER
	L"%s non si trova nel settore per prendere quell'oggetto.",
	L"Durante il combattimento, dovrete raccogliere gli oggetti manualmente.",
	L"Durante il combattimento, dovrete rilasciare gli oggetti manualmente.",
	L"%s non si trova nel settore per rilasciare quell'oggetto.",
	L"Durante il combattimento, non potete ricaricare con una cassa del ammo.",
};

STR16 pMapInventoryStrings[] =
{
	L"Posizione", 			// sector these items are in
	L"Totale oggetti", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Per cambiare l'incarico di un mercenario, come, ad esempio, cambiare la squadra, dottore o riparare, cliccate dentro la colonna 'Compito'",
	L"Per assegnare a un mercenario una destinazione in un altro settore, cliccate dentro la colonna 'Dest'",
	L"Una volta che a un mercenario è stato ordinato di procedere, una compressione di tempo gli permetterà di muoversi.",
	L"Cliccando di sinistro, selezionerete il settore. Cliccando di sinistro un'altra volta, darete al mercenario ordini di movimento. Cliccando di destro, darete informazioni sommarie al settore.",
	L"Premete 'h' in questo settore di questa schermata ogni volta che vorrete accedere a questa finestra d'aiuto.",
	L"Test Text",
	L"Test Text",
	L"Test Text",
	L"Test Text",
	L"Non potrete fare molto in questa schermata finché non arriverete ad Arulco. Quando avrete definito la vostra squadra, cliccate sul pulsante Compressione di Tempo in basso a destra. Questo diminuirà il tempo necessario alla vostra squadra per raggiungere Arulco.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Muovere mercenari nel settore", 	// title for movement box 
	L"Rotta spostamento esercito", 		// done with movement menu, start plotting movement
	L"Annulla", 		// cancel this menu
	L"Altro",		// title for group of mercs not on squads nor in vehicles
	L"Select all",		// Select all squads TODO: Translate
};


STR16 pUpdateMercStrings[] =
{
	L"Oops:", 			// an error has occured
	L"Scaduto contratto mercenari:", 	// this pop up came up due to a merc contract ending
	L"Portato a termine incarico mercenari:", // this pop up....due to more than one merc finishing assignments
	L"Mercenari di nuovo al lavoro:", // this pop up ....due to more than one merc waking up and returing to work
	L"Mercenari a riposo:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Contratti in scadenza:", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Mostra città (|w)",
	L"Mostra |miniere",
	L"Mos|tra squadre & nemici",
	L"Mostra spazio |aereo",
	L"Mostra oggett|i",
	L"Mostra esercito & nemici (|Z)",
	L"Show |Disease Data",	// TODO.Translate
	L"Show Weathe|r",
	L"Show |Quests & Intel",
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Next (|.)",		// next page // TODO.Translate
	L"Previous (|,)",		// previous page // TODO.Translate
	L"Exit Sector Inventory (|E|s|c)",	// exit sector inventory // TODO.Translate

	// TODO.Translate
	L"Zoom Inventory", // HEAROCK HAM 5: Inventory Zoom Button
	L"Stack and merge items", // HEADROCK HAM 5: Stack and Merge
	L"|L|e|f|t |C|l|i|c|k: Sort ammo into crates\n|R|i|g|h|t |C|l|i|c|k: Sort ammo into boxes", // HEADROCK HAM 5: Sort ammo
	// 6 - 10
	L"|L|e|f|t |C|l|i|c|k: Remove all item attachments\n|R|i|g|h|t |C|l|i|c|k: empty LBE in sector", // HEADROCK HAM 5: Separate Attachments; Bob: empty LBE items
	L"Eject ammo from all weapons", //HEADROCK HAM 5: Eject Ammo
	L"|L|e|f|t |C|l|i|c|k: Show all items\n|R|i|g|h|t |C|l|i|c|k: Hide all items", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Guns\n|R|i|g|h|t |C|l|i|c|k|: Show only Guns", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Ammunition\n|R|i|g|h|t |C|l|i|c|k: Show only Ammunition", // HEADROCK HAM 5: Filter Button
	// 11 - 15
	L"|L|e|f|t |C|l|i|c|k: Toggle Explosives\n|R|i|g|h|t |C|l|i|c|k: Show only Explosives", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Melee Weapons\n|R|i|g|h|t |C|l|i|c|k: Show only Melee Weapons", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Armor\n|R|i|g|h|t |C|l|i|c|k: Show only Armor", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle LBEs\n|R|i|g|h|t |C|l|i|c|k: Show only LBEs", // HEADROCK HAM 5: Filter Button
	L"|L|e|f|t |C|l|i|c|k: Toggle Kits\n|R|i|g|h|t |C|l|i|c|k: Show only Kits", // HEADROCK HAM 5: Filter Button
	// 16 - 20
	L"|L|e|f|t |C|l|i|c|k: Toggle Misc. Items\n|R|i|g|h|t |C|l|i|c|k: Show only Misc. Items", // HEADROCK HAM 5: Filter Button
	L"Toggle Get Item Display", // Flugente: move item display	// TODO.Translate
	L"Save Gear Template",	// TODO.Translate
	L"Load Gear Template...",
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"Portati|le",
	L"Tattico (|E|s|c)",
	L"|Opzioni",
	L"Dilata tempo (|+)", 	// time compress more
	L"Comprime tempo (|-)", 	// time compress less
	L"Messaggio precedente (|S|u)\nIndietro (|P|a|g|S|u)", 	// previous message in scrollable list
	L"Messaggio successivo (|G|i|ù)\nAvanti (|P|a|g|G|i|ù)", 	// next message in the scrollable list
	L"Inizia/Ferma tempo (|S|p|a|z|i|o)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Bilancio attuale", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s è morto.",
};


STR16 pDayStrings[] =
{
	L"Giorno",
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
	L"Help Desk",
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
	L"Stephan",
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
	L"Assicurazione M.I.S.",	
	L"Bobby Ray",
	L"Capo",
	L"John Kulba",
	L"A.I.M.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"Indietro",
  L"Avanti",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Avete una nuova E-mail...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Eliminate l'E-mail?",
 L"Eliminate l'E-mail NON LETTA?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"Da:",
	L"Sogg.:",
	L"Giorno:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"posta elettronica",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Contabile aggiuntivo",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Crediti:", 				// credit (subtract from) to player's account
	L"Debiti:", 				// debit (add to) to player's account
	L"Entrate effettive di ieri:",
	L"Altri depositi di ieri:",
	L"Debiti di ieri:",
	L"Bilancio di fine giornata:",
	L"Entrate effettive di oggi:",
	L"Altri depositi di oggi:",
	L"Debiti di oggi:",
	L"Bilancio attuale:",
	L"Entrate previste:",
	L"Bilancio previsto:", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
  L"Giorno", 				// the day column
	L"Crediti", 			// the credits column (to ADD money to your account)
	L"Debiti",				// the debits column (to SUBTRACT money from your account)
	L"Transazione", // transaction type - see TransactionText below
	L"Bilancio", 		// balance at this point in time
	L"Pagina", 				// page number
	L"Giorno(i)", 			// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Interessi maturati",			// interest the player has accumulated so far
	L"Deposito anonimo",
	L"Tassa di transazione", 
	L"Assunto", 				// Merc was hired
	L"Acquistato da Bobby Ray", 		// Bobby Ray is the name of an arms dealer
	L"Acconti pagati al M.E.R.C.",
	L"Deposito medico per %s", 		// medical deposit for merc
	L"Analisi del profilo I.M.P.", 		// IMP is the acronym for International Mercenary Profiling
	L"Assicurazione acquistata per %s", 
	L"Assicurazione ridotta per %s",
	L"Assicurazione estesa per %s", 				// johnny contract extended
	L"Assicurazione annullata %s", 
	L"Richiesta di assicurazione per %s", 		// insurance claim for merc
	L"1 giorno", 				// merc's contract extended for a day
	L"1 settimana", 				// merc's contract extended for a week
	L"2 settimane", 				// ... for 2 weeks
	L"Entrata mineraria", 
	L"", //String nuked
	L"Fiori acquistati",
	L"Totale rimborso medico per %s",
	L"Parziale rimborso medico per %s",
	L"Nessun rimborso medico per %s",
	L"Pagamento a %s",		// %s is the name of the npc being paid
	L"Trasferimento fondi a %s", 			// transfer funds to a merc
	L"Trasferimento fondi da %s", 		// transfer funds from a merc
	L"Equipaggiamento esercito in %s", // initial cost to equip a town's militia
	L"Oggetti acquistati da%s.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s soldi depositati.",
	L"Sold Item(s) to the Locals",
	L"Facility Use", // HEADROCK HAM 3.6		// TODO.Translate
	L"Militia upkeep", // HEADROCK HAM 3.6		// TODO.Translate
	L"Ransom for released prisoners",	// Flugente: prisoner system TODO.Translate
	L"WHO data subscription",	// Flugente: disease TODO.Translate
	L"Payment to Kerberus",	// Flugente: PMC
	L"SAM site repair", // Flugente: SAM repair // TODO.Translate
	L"Trained workers", // Flugente: train workers
	L"Drill militia in %s",	// Flugente: drill militia	// TODO.Translate
	L"Mini event", // rftr: mini events // TODO: translate
	L"Funds transferred from rebel command", // rftr: rebel command
	L"Funds transferred to rebel command", // rftr: rebel command
};

STR16 pTransactionAlternateText[] =
{
	L"Assicurazione per", 				// insurance for a merc
	L"Est. contratto di %s per 1 giorno.", 				// entend mercs contract by a day
	L"Est. %s contratto per 1 settimana.",
	L"Est. %s contratto per 2 settimane.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Skyrider è stato pagato $%d", 			// skyrider was paid an amount of money
	L"A Skyrider bisogna ancora dare $%d", 		// skyrider is still owed an amount of money
	L"Skyrider ha finito il carburante",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider è di nuovo pronto a volare.", // Skyrider was grounded but has been freed
	L"Skyrider non ha passeggeri. Se avete intenzione di trasportare mercenari in questo settore, assegnateli prima al Veicolo/Elicottero.",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"Ottimo", 
	L"Buono",
	L"Medio",
	L"Basso",
	L"Panico",
	L"Cattivo",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"L'equipaggio di %s è ora disponibile a Omerta (A9).", 
	L"L'equipaggio di %s è ora disponibile a Drassen (B13).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Salute",
	L"Energia",
	L"Morale",
	L"Condizione",	// the condition of the current vehicle (its "health")
	L"Carburante",	// the fuel level of the current vehicle (its "energy")
	L"Poison",	// TODO.Translate
	L"Water",		// drink level
	L"Food",		// food level
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Mercenario precedente (|S|i|n)", 			// previous merc in the list
	L"Mercenario successivo (|D|e|s)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"TAP", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Non lo vedrete mai più. Siete sicuri?", 	// do you want to continue and lose the item forever
	L"Questo oggetto sembra DAVVERO importante. Siete DAVVERO SICURISSIMI di volerlo gettare via?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"La squadra non può muoversi, se un mercenario dorme.",

//1-5
	L"Muovete la squadra al primo piano.",
	L"Ordini di movimento? È un settore nemico!",
	L"I mercenari devono essere assegnati a una squadra o a un veicolo per potersi muovere.",
	L"Non avete ancora membri nella squadra.", 		// you have no members, can't do anything 
	L"I mercenari non possono attenersi agli ordini.",			 		// merc can't comply with your order
//6-10
	L"ha bisogno di una scorta per muoversi. Inseritelo in una squadra che ne è provvista.", // merc can't move unescorted .. for a male
	L"ha bisogno di una scorta per muoversi. Inseritela in una squadra che ne è provvista.", // for a female
	L"Il mercenario non è ancora arrivato ad %s!",
	L"Sembra che ci siano negoziazioni di contratto da stabilire.",
	L"",
//11-15
	L"Ordini di movimento? È in corso una battaglia!",
	L"Siete stati vittima di un'imboscata da parte dai Bloodcat nel settore %s!",
	L"Siete appena entrati in quella che sembra una tana di un Bloodcat nel settore %s!", 
	L"", 
	L"La zona SAM in %s è stata assediata.",
//16-20
	L"La miniera di %s è stata assediata. La vostra entrata giornaliera è stata ridotta di %s per giorno.",
	L"Il nemico ha assediato il settore %s senza incontrare resistenza.",
	L"Almeno uno dei vostri mercenari non ha potuto essere affidato a questo incarico.",
	L"%s non ha potuto unirsi alla %s visto che è completamente pieno",
	L"%s non ha potuto unirsi alla %s visto che è troppo lontano.",
//21-25
	L"La miniera di %s è stata invasa dalle forze armate di Deidranna!",
	L"Le forze armate di Deidranna hanno appena invaso la zona SAM in %s",
	L"Le forze armate di Deidranna hanno appena invaso %s",
	L"Le forze armate di Deidranna sono appena state avvistate in %s.",
	L"Le forze armate di Deidranna sono appena partite per %s.",
//26-30
	L"Almeno uno dei vostri mercenari non può riposarsi.",
	L"Almeno uno dei vostri mercenari non è stato svegliato.",
	L"L'esercito non si farà vivo finché non avranno finito di esercitarsi.",
	L"%s non possono ricevere ordini di movimento adesso.",
	L"I militari che non si trovano entro i confini della città non possono essere spostati inquesto settore.",
//31-35
	L"Non potete avere soldati in %s.",
	L"Un veicolo non può muoversi se è vuoto!",
	L"%s è troppo grave per muoversi!",
	L"Prima dovete lasciare il museo!",
	L"%s è morto!",
//36-40
	L"%s non può andare a %s perché si sta muovendo",
	L"%s non può salire sul veicolo in quel modo",
	L"%s non può unirsi alla %s",
	L"Non potete comprimere il tempo finché non arruolerete nuovi mercenari!",
	L"Questo veicolo può muoversi solo lungo le strade!",
//41-45
	L"Non potete riassegnare i mercenari che sono già in movimento",
	L"Il veicolo è privo di benzina!",
	L"%s è troppo stanco per muoversi.",
	L"Nessuno a bordo è in grado di guidare il veicolo.",
	L"Uno o più membri di questa squadra possono muoversi ora.",
//46-50
	L"Uno o più degli altri mercenari non può muoversi ora.",
	L"Il veicolo è troppo danneggiato!",
	L"Osservate che solo due mercenari potrebbero addestrare i militari in questo settore.",
	L"Il robot non può muoversi senza il suo controller. Metteteli nella stessa squadra.",
	L"Items cannot be moved to %s, as no valid dropoff point was found. Please enter map to resolve this issue.",	// TODO.Translate
// 51-55
	L"%d items moved from %s to %s",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Cliccate di nuovo su una destinazione per confermare la vostra meta finale, oppure cliccate su un altro settore per fissare più tappe.",
	L"Rotta di spostamento confermata.",
	L"Destinazione immutata.",
	L"Rotta di spostamento annullata.",
	L"Rotta di spostamento accorciata.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Cliccate sul settore dove desiderate che i mercenari arrivino.",
	L"OK. I mercenari che stavano arrivando si sono dileguati a %s",
	L"I mercenari non possono essere trasportati, lo spazio aereo non è sicuro!",
	L"Annullato. Il settore d'arrivo è immutato",
	L"Lo spazio aereo sopra %s non è più sicuro! Il settore d'arrivo è stato spostato a %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Entra nell'inventario (|I|n|v|i|o)",
	L"Getta via l'oggetto",
	L"Esci dall'inventario (|I|n|v|i|o)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Volete che %s lasci il suo equipaggiamento dove si trova ora (%s) o in seguito a (%s) dopo aver preso il volo?",
	L"%s sta per partire e spedirà il suo equipaggiamento a %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Volete che %s lasci il suo equipaggiamento dove si trova ora (%s) o in seguito a (%s) dopo aver preso il volo?",
	L"%s sta per partire e spedirà il suo equipaggiamento a %s.",
};


STR16 pMercContractOverStrings[] =
{
	L": contratto scaduto. Egli è tornato a casa.", 		// merc's contract is over and has departed
	L": contratto scaduto. Ella è tornata a casa.", 		// merc's contract is over and has departed
	L": contratto terminato. Egli è partito.", 		// merc's contract has been terminated
	L": contratto terminato. Ella è partita.",		// merc's contract has been terminated
	L"Dovete al M.E.R.C. troppi soldi, così %s è partito.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

STR16 pImpPopUpStrings[] =
{
	L"Codice di autorizzazione non valido",
	L"State per riiniziare l'intero processo di profilo. Ne siete certi?",
	L"Inserite nome e cognome corretti oltre che al sesso",
	L"L'analisi preliminare del vostro stato finanziario mostra che non potete offrire un'analisi di profilo.",
	L"Opzione non valida questa volta.",
	L"Per completare un profilo accurato, dovete aver spazio per almeno uno dei membri della squadra.",
	L"Profilo già completato.",
	L"Cannot load I.M.P. character from disk.",
	L"You have already reached the maximum number of I.M.P. characters.",
	L"You have already three I.M.P characters with the same gender on your team.",
	L"You cannot afford the I.M.P character.",	// 10
	L"The new I.M.P character has joined your team.",
	L"You have already selected the maximum number of traits.",	// TODO.Translate
	L"No voicesets found.",	// TODO.Translate
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"Cosa offriamo", 			// about the IMP site
	L"INIZIO", 			// begin profiling
	L"Personalità", 		// personality section
	L"Attributi", 		// personal stats/attributes section
	L"Appearance", 			// changed from portrait 
	L"Voce %d", 			// the voice selection
	L"Fine", 			// done profiling
	L"Ricomincio", 		// start over profiling
	L"Sì, scelgo la risposta evidenziata.", 
	L"Sì", 
	L"No",
	L"Finito", 			// finished answering questions
	L"Prec.", 			// previous question..abbreviated form
	L"Avanti", 			// next question
	L"SÌ, LO SONO.", 		// yes, I am certain 
	L"NO, VOGLIO RICOMINCIARE.", // no, I want to start over the profiling process
	L"SÌ",
	L"NO",
	L"Indietro", 			// back one page
	L"Annulla", 			// cancel selection
	L"Sì, ne sono certo.",
	L"No, lasciami dare un'altra occhiata.",
	L"Immatricolazione", 			// the IMP site registry..when name and gender is selected
	L"Analisi", 			// analyzing your profile results
	L"OK",
	L"Character", // Change from "Voice"
	L"Nessuna",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed 
	L"With your character traits chosen, it is time to select your skills.",
	L"To complete the process, select your attributes.",
	L"To commence actual profiling, select portrait, voice and colors.",
	L"Now that you have completed your appearence choice, proceed to character analysis.",
};

STR16 pFilesTitle[] =
{
  L"Gestione risorse",
};

STR16 pFilesSenderList[] =
{
  L"Rapporto", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
	L"Intercetta #1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"Intercetta #2",	   // second intercept file
	L"Intercetta #3",			 // third intercept file
	L"Intercetta #4", // fourth intercept file
	L"Intercetta #5", // fifth intercept file
	L"Intercetta #6", // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Registro",
};

STR16 pHistoryHeaders[] =
{
	L"Giorno", 			// the day the history event occurred
	L"Pagina", 			// the current page in the history report we are in
	L"Giorno", 			// the days the history report occurs over
	L"Posizione", 			// location (in sector) the event occurred
	L"Evento", 			// the event label
};

// Externalized to "TableData\History.xml"
/*
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
	L"%s è stato assunto dall'A.I.M.", 										// merc was hired from the aim site
	L"%s è stato assunto dal M.E.R.C.", 									// merc was hired from the aim site
	L"%s morì.", 															// merc was killed
	L"Acconti stanziati al M.E.R.C.",								// paid outstanding bills at MERC
	L"Assegno accettato da Enrico Chivaldori",	
	//6-10
	L"Profilo generato I.M.P.",
	L"Acquistato contratto d'assicurazione per %s.", 				// insurance contract purchased
	L"Annullato contratto d'assicurazione per %s.", 				// insurance contract canceled
	L"Versamento per richiesta assicurazione per %s.", 							// insurance claim payout for merc
	L"Esteso contratto di %s di 1 giorno.", 						// Extented "mercs name"'s for a day
	//11-15
	L"Esteso contratto di %s di 1 settimana.", 					// Extented "mercs name"'s for a week
	L"Esteso contratto di %s di 2 settimane.", 					// Extented "mercs name"'s 2 weeks
	L"%s è stato congedato.", 													// "merc's name" was dismissed.
	L"%s è partito.", 																		// "merc's name" quit.
	L"avventura iniziata.", 															// a particular quest started
	//16-20
	L"avventura completata.",
	L"Parlato col capo minatore di %s",									// talked to head miner of town
	L"Liberato %s",
	L"Inganno utilizzato",
	L"Il cibo dovrebbe arrivare a Omerta domani",
	//21-25
	L"%s ha lasciato la squadra per diventare la moglie di Daryl Hick",
	L"contratto di %s scaduto.",
	L"%s è stato arruolato.",
	L"Enrico si è lamentato della mancanza di progresso",
	L"Vinta battaglia",
	//26-30
	L"%s miniera ha iniziato a esaurire i minerali",
	L"%s miniera ha esaurito i minerali",
	L"%s miniera è stata chiusa",
	L"%s miniera è stata riaperta",
	L"Trovata una prigione chiamata Tixa.",
	//31-35
	L"Sentito di una fabbrica segreta di armi chiamata Orta.",
	L"Alcuni scienziati a Orta hanno donato una serie di lanciamissili.",
	L"La regina Deidranna ha bisogno di cadaveri.",
	L"Frank ha parlato di scontri a San Mona.",
	L"Un paziente pensa che lui abbia visto qualcosa nella miniera.",
	//36-40
	L"Incontrato qualcuno di nome Devin - vende esplosivi.",
	L"Imbattutosi nel famoso ex-mercenario dell'A.I.M. Mike!",
	L"Incontrato Tony - si occupa di armi.",
	L"Preso un lanciamissili dal Sergente Krott.",
	L"Concessa a Kyle la licenza del negozio di pelle di Angel.",
	//41-45
	L"Madlab ha proposto di costruire un robot.",
	L"Gabby può effettuare operazioni di sabotaggio contro sistemi d'allarme.",
	L"Keith è fuori dall'affare.",
	L"Howard ha fornito cianuro alla regina Deidranna.",
	L"Incontrato Keith - si occupa di un po' di tutto a Cambria.",
	//46-50
	L"Incontrato Howard - si occupa di farmaceutica a Balime",
	L"Incontrato Perko - conduce una piccola impresa di riparazioni.",
	L"Incontrato Sam di Balime - ha un negozio di hardware.",
	L"Franz si occupa di elettronica e altro.",
	L"Arnold possiede un'impresa di riparazioni a Grumm.",
	//51-55
	L"Fredo si occupa di elettronica a Grumm.",
	L"Donazione ricevuta da un ricco ragazzo a Balime.",
	L"Incontrato un rivenditore di un deposito di robivecchi di nome Jake.",
	L"Alcuni vagabondi ci hanno dato una scheda elettronica.",
	L"Corrotto Walter per aprire la porta del seminterrato.",
	//56-60
	L"Se Dave ha benzina, potrà fare il pieno gratis.",
	L"Corrotto Pablo.",
	L"Kingpin tiene i soldi nella miniera di San Mona.",
	L"%s ha vinto il Combattimento Estremo",
	L"%s ha perso il Combattimento Estremo",
	//61-65
	L"%s è stato squalificato dal Combattimento Estremo",
	L"trovati moltissimi soldi nascosti nella miniera abbandonata.",
	L"Incontrato assassino ingaggiato da Kingpin.",
	L"Perso il controllo del settore",				//ENEMY_INVASION_CODE
	L"Difeso il settore",
	//66-70
	L"Persa la battaglia",							//ENEMY_ENCOUNTER_CODE
	L"Imboscata fatale",						//ENEMY_AMBUSH_CODE
	L"Annientata imboscata nemica",
	L"Attacco fallito",			//ENTERING_ENEMY_SECTOR_CODE
	L"Attacco riuscito!",
	//71-75
	L"Creature attaccate",			//CREATURE_ATTACK_CODE
	L"Ucciso dai Bloodcat",			//BLOODCAT_AMBUSH_CODE
	L"Massacrati dai Bloodcat",
	L"%s è stato ucciso",
	L"Data a Carmen la testa di un terrorista",
	//76-80
	L"Massacro sinistro",
	L"Ucciso %s",
	L"Met Waldo - aircraft mechanic.",
	L"Helicopter repairs started. Estimated time: %d hour(s).",
};
*/

STR16 pHistoryLocations[] =
{
	L"N/A",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"E-mail",
	L"Rete",
	L"Finanza",
	L"Personale",
	L"Cronologia",
	L"File",
	L"Chiudi",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Ray",
	L"I.M.P",
	L"M.E.R.C.",
	L"Pompe funebri",
	L"Fiorista",
	L"Assicurazione",
	L"Annulla",
	L"Campaign History",	// TODO.Translate
	L"MeLoDY",
	L"WHO",
	L"Kerberus",
	L"Militia Overview",	// TODO.Translate
	L"R.I.S.",
	L"Factories",	// TODO.Translate
};

STR16 pBookmarkTitle[] =
{
	L"Segnalibri",
	L"Cliccate con il destro per accedere a questo menu in futuro.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Caricamento",
	L"Caricamento",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Prendi", 			// take money from merc
	L"Dai", 			// give money to merc
	L"Annulla", 			// cancel transaction
	L"Pulisci", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Trasferisce $", 		// transfer money to merc -- short form
	L"Stato", 			// view stats of the merc
	L"Inventario", 			// view the inventory of the merc
	L"Impiego",
};

STR16 sATMText[ ]=
{
	L"Trasferisci fondi?", 		// transfer funds to merc?
	L"Ok?", 			// are we certain?
	L"Inserisci somma", 		// enter the amount you want to transfer to merc
	L"Seleziona tipo", 		// select the type of transfer to merc
	L"Fondi insufficienti", 	// not enough money to transfer to merc
	L"La somma deve essere un multiplo di $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Errore",
	L"Il server non ha entrata NSD.",	
	L"Controlla l'indirizzo LRU e prova di nuovo.",
	L"OK",
	L"Connessione intermittente all'host. Tempi d'attesa più lunghi per il trasferimento.",
};


STR16 pPersonnelString[] =
{
	L"Mercenari:", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"Membri dell'A.I.M.",
	L"Ritratti A.I.M.",		// a mug shot is another name for a portrait
	L"Categoria A.I.M.",
	L"A.I.M.",
	L"Membri dell'A.I.M.",
	L"Tattiche A.I.M.",
	L"Storia A.I.M.",
	L"Collegamenti A.I.M.",
	L"M.E.R.C.",
	L"Conti M.E.R.C.",
	L"Registrazione M.E.R.C.",
	L"Indice M.E.R.C.",
	L"Bobby Ray",
	L"Bobby Ray - Armi",
	L"Bobby Ray - Munizioni",
	L"Bobby Ray - Giubb. A-P",
	L"Bobby Ray - Varie",							//misc is an abbreviation for miscellaneous				
	L"Bobby Ray - Usato",
	L"Bobby Ray - Ordine Mail",
	L"I.M.P.",
	L"I.M.P.",
	L"Servizio Fioristi Riuniti",
	L"Servizio Fioristi Riuniti - Galleria",
	L"Servizio Fioristi Riuniti - Ordine",
	L"Servizio Fioristi Riuniti - Card Gallery",
	L"Agenti assicurativi Malleus, Incus & Stapes",
	L"Informazione",
	L"Contratto",
	L"Commenti",
	L"Servizio di pompe funebri di McGillicutty",
	L"",
	L"URL non ritrovato.",
	L"%s Press Council - Conflict Summary",	// TODO.Translate
	L"%s Press Council - Battle Reports",
	L"%s Press Council - Latest News",
	L"%s Press Council - About us",
	L"Mercs Love or Dislike You - About us",		// TODO.Translate
	L"Mercs Love or Dislike You - Analyze a team",
	L"Mercs Love or Dislike You - Pairwise comparison",
	L"Mercs Love or Dislike You - Personality",
	L"WHO - About WHO",
	L"WHO - Disease in Arulco",
	L"WHO - Helpful Tips",
	L"Kerberus - About Us",
	L"Kerberus - Hire a Team",
	L"Kerberus - Individual Contracts",
	L"Militia Overview",
	L"Recon Intelligence Services - Information Requests",		// TODO.Translate
	L"Recon Intelligence Services - Information Verification",
	L"Recon Intelligence Services - About us",
	L"Factory Overview",	// TODO.Translate
	L"Bobby Ray's - Recent Shipments",
	L"Encyclopedia",
	L"Encyclopedia - Data",
	L"Briefing Room",
	L"Briefing Room - Data",
};

STR16 pShowBookmarkString[] =
{
	L"Aiuto",
	L"Cliccate su Rete un'altra volta per i segnalibri.",
};

STR16 pLaptopTitles[] =
{
	L"Cassetta della posta",
	L"Gestione risorse",
	L"Personale",
	L"Contabile aggiuntivo",
	L"Ceppo storico",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Ucciso in azione",
	L"Licenziato",
	L"Altro",
	L"Sposato",
	L"Contratto Scaduto",
	L"Liberato",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"Squadra attuale",
	L"Partenze",
	L"Costo giornaliero:",
	L"Costo più alto:",
	L"Costo più basso:",
	L"Ucciso in azione:",
	L"Licenziato:",
	L"Altro:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Più basso",
	L"Normale",
	L"Più alto",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"SAL",
	L"AGI",
	L"DES",
	L"FOR",
	L"COM",
	L"SAG",
	L"LIV",
	L"TIR",
	L"MEC",
	L"ESP",
	L"PS",
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
	L"Contratto",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Continua",
	L"Fermati",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Siete stati sconfitti in questo settore!",
	L"Il nemico, non avendo alcuna pietà delle anime della squadra, divorerà ognuno di voi!",
	L"I membri inconscenti della vostra squadra sono stati catturati!",
	L"I membri della vostra squadra sono stati fatti prigionieri dal nemico.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Indietro",
	L"Avanti",
	L"Accetta",
	L"Pulisci",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Indietro", 
	L"Avanti",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Autorizza",
	L"Home Page",
	L"Conto #:",
	L"Merc",
	L"Giorni",
	L"Tasso",	//5
	L"Costo",
	L"Totale:",
	L"Conferma il pagamento di %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
	L"%s (+gear)",	// TODO.Translate
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Previous",
  L"Next",
};



//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"Salute",
	L"Agilità",
	L"Destrezza",
	L"Forza",
	L"Comando",
	L"Saggezza",
	L"Liv. esperienza",
	L"Mira",
	L"Meccanica",
	L"Esplosivi",
	L"Pronto socc.",

	L"Indietro",
	L"Ricompensa",
	L"Successivo",
	L"Info. addizionali",
	L"Home Page",
	L"Assoldato",
	L"Salario:",
	L"Al giorno",
	L"Gear:", // TODO.Translate
	L"Totale:",
	L"Deceduto",

	L"Avete già una squadra di mercenari.",
	L"Compra equip.?",
	L"Non disponibile",
	L"Unsettled Bills", // TODO.Translate
	L"Bio", // TODO.Translate
	L"Inv",								
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Apri conto",
	L"Annulla",
	L"Non hai alcun conto. Vuoi aprirne uno?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, fondatore e proprietario",
	L"Per aprire un conto, cliccate qui",
	L"Per visualizzare un conto, cliccate qui",
	L"Per visualizzare i file, cliccate qui",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer failed. No funds available.",	// TODO.Translate
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Impresa di pompe funebri di McGillicutty: Il dolore delle famiglie che hanno fornito il loro aiuto dal 1983.",
	L"Precedentemente mercenario dell'A.I.M. Murray \"Pops\" McGillicutty è un impresario di pompe funebri qualificato e con molta esperienza.",
	L"Essendo coinvolto profondamente nella morte e nel lutto per tutta la sua vita, Pops sa quanto sia difficile affrontarli.",
	L"L'impresa di pompe funebri di McGillicutty offre una vasta gamma di servizi funebri, da una spalla su cui piangere a ricostruzioni post-mortem per corpi mutilati o sfigurati.",
	L"Lasciate che l'impresa di pompe funebri di McGillicutty vi aiuti e i vostri amati riposeranno in pace.",

	// Text for the various links available at the bottom of the page
	L"SPEDISCI FIORI",
	L"ESPOSIZIONE DI BARE & URNE",
	L"SERVIZI DI CREMAZIONE",
	L"SERVIZI PRE-FUNEBRI",
	L"CERIMONIALE FUNEBRE",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Purtroppo, il resto di questo sito non è stato completato a causa di una morte in famiglia. Aspettando la lettura del testamento e la riscossione dell'eredità, il sito verrà ultimato non appena possibile.",
	L"Vi porgiamo, comunque, le nostre condolianze in questo momento di dolore. Contatteci ancora.",
};

// Text for the florist Home Page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Galleria",

	//Address of United Florist

	L"\"Ci lanciamo col paracadute ovunque\"",
	L"1-555-SCENT-ME",
	L"333 Dot. NoseGay, Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"Siamo veloci ed efficienti!",
	L"Consegna il giorno successivo in quasi tutto il mondo, garantito. Applicate alcune restrizioni.",
	L"I prezzi più bassi in tutto il mondo, garantito!",
	L"Mostrateci un prezzo concorrente più basso per qualsiasi progetto, e riceverete una dozzina di rose, gratuitamente.",
	L"Flora, fauna & fiori in volo dal 1981.",
	L"I nostri paracadutisti decorati ex-bomber lanceranno il vostro bouquet entro un raggio di dieci miglia dalla locazione richiesta. Sempre e ovunque!",
	L"Soddisfiamo la vostra fantasia floreale.",
	L"Lasciate che Bruce, il nostro esperto in composizioni floreali, selezioni con cura i fiori più freschi e della migliore qualità dalle nostre serre più esclusive.",
	L"E ricordate, se non l'abbiamo, possiamo coltivarlo - E subito!",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Indietro",
	L"Spedisci",
	L"Home",
	L"Galleria",

	L"Nome del bouquet:",
	L"Prezzo:",			//5
	L"Numero ordine:",
	L"Data consegna",
	L"gior. succ.",
	L"arriva quando arriva",
	L"Luogo consegna",			//10
	L"Servizi aggiuntivi",
	L"Bouquet schiacciato ($10)",
	L"Rose nere ($20)",
	L"Bouquet appassito ($10)",
	L"Torta di frutta (se disponibile 10$)",		//15
	L"Sentimenti personali:",
	L"Il vostro messaggio non può essere più lungo di 75 caratteri.",
	L"... oppure sceglietene uno dai nostri",

	L"BIGLIETTI STANDARD",	
	L"Informazioni sulla fatturazione",//20

	//The text that goes beside the area where the user can enter their name

	L"Nome:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Prec.",	//abbreviation for previous
	L"Succ.",	//abbreviation for next

	L"Clicca sul modello che vuoi ordinare.",
	L"Ricorda: c'è un supplemento di 10$ per tutti i bouquet appassiti o schiacciati.",

	//text on the button

	L"Home",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Cliccate sulla vostra selezione",
	L"Indietro",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Ordine",				//Title of the page
	L"Qta",					// The number of items ordered
	L"Peso (%s)",			// The weight of the item
	L"Nome oggetto",				// The name of the item
	L"Prezzo unit.",				// the item's weight
	L"Totale",				//5	// The total price of all of items of the same type
	L"Sotto-totale",				// The sub total of all the item totals added
	L"S&C (Vedete luogo consegna)",		// S&H is an acronym for Shipping and Handling 
	L"Totale finale",			// The grand total of all item totals + the shipping and handling
	L"Luogo consegna",				
	L"Spedizione veloce",			//10	// See below
	L"Costo (per %s.)",			// The cost to ship the items
	L"Espresso di notte",			// Gets deliverd the next day
	L"2 giorni d'affari",			// Gets delivered in 2 days
	L"Servizio standard",			// Gets delivered in 3 days
	L"Annulla ordine",//15			// Clears the order page
	L"Accetta ordine",			// Accept the order
	L"Indietro",				// text on the button that returns to the previous page
	L"Home Page",				// Text on the button that returns to the home Page
	L"* Indica oggetti usati",		// Disclaimer stating that the item is used
	L"Non potete permettervi di pagare questo.",		//20	// A popup message that to warn of not enough money
	L"<NESSUNO>",				// Gets displayed when there is no valid city selected
	L"Siete sicuri di volere spedire quest'ordine a %s?",		// A popup that asks if the city selected is the correct one
	L"peso del pacco**",			// Displays the weight of the package
	L"** Peso min.",				// Disclaimer states that there is a minimum weight for the package
	L"Spedizioni",	
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Pistol",
	L"M. Pistol",
	L"SMG",
	L"Rifle",
	L"SN Rifle",
	L"AS Rifle",
	L"MG",
	L"Shotgun",
	L"Heavy W.",

	// Ammo
	L"Pistol",
	L"M. Pistol",
	L"SMG",
	L"Rifle",
	L"SN Rifle",
	L"AS Rifle",
	L"MG",
	L"Shotgun",

	// Used
	L"Guns",
	L"Armor",
	L"LBE Gear",
	L"Misc",

	// Armour
	L"Helmets",
	L"Vests",
	L"Leggings",
	L"Plates",

	// Misc
	L"Blades",
	L"Th. Knives",
	L"Blunt W.",
	L"Grenades",
	L"Bombs",
	L"Med. Kits",
	L"Kits",
	L"Face Items",
	L"LBE Gear",
	L"Optics", // Madd: new BR filters		// TODO.Translate
	L"Grip/LAM",
	L"Muzzle",
	L"Stock",
	L"Mag/Trig.",
	L"Other Att.",
	L"Misc.",
};

// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Ordini:",				// Title
	// instructions on how to order
	L"Cliccate sull'oggetto. Sinistro per aggiungere pezzi, destro per toglierne. Una volta selezionata la quantità, procedete col nuovo ordine.",			

	//Text on the buttons to go the various links

	L"Oggetti prec.",		// 
	L"Armi", 			//3
	L"Munizioni",			//4
	L"Giubb. A-P",			//5
	L"Varie",			//6	//misc is an abbreviation for miscellaneous
	L"Usato",			//7
	L"Oggetti succ.",
	L"ORDINE",
	L"Home Page",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"La vostra squadra ha",//11
	L"arma(i) che usa(no) questo tipo di munizioni", //12

	//The following lines provide information on the items

	L"Peso:",		// Weight of all the items of the same type
	L"Cal.:",			// the caliber of the gun
	L"Mag.:",			// number of rounds of ammo the Magazine can hold
	L"Git.:",			// The range of the gun
	L"Dan.:",			// Damage of the weapon	
	L"FFA:",			// Weapon's Rate Of Fire, acronym ROF
	L"PA:",				// Weapon's Action Points, acronym AP
	L"Stun:",			// Weapon's Stun Damage
	L"Protect:",		// Armour's Protection
	L"Trav.:",			// Armour's Camouflage
	L"Armor Pen:",		// Ammo's Armour Piercing modifier (see AmmoTypes.xml - armourImpactReduction)
	L"Dmg Mod:",		// Ammo's Bullet Tumble modifier (see AmmoTypes.xml - afterArmourDamage)
	L"Projectiles:",	// Ammo's bullet count (for buckshot) (see AmmoTypes.xml - numberOfBullets)
	L"Costo:",			// Cost of the item
	L"Inventario:",			// The number of items still in the store's inventory
	L"Num. ordine:",		// The number of items on order
	L"Danneggiato",			// If the item is damaged
	L"Peso:",			// the Weight of the item
	L"Totale:",			// The total cost of all items on order
	L"* funzionale al %%",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Darn! Quest'ordine qui accetterà solo " ,//First part
	L" oggetti. Se avete intenzione di ordinare più merce (ed è quello che speriamo), fate un ordine a parte e accettate le nostre scuse.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Ci dispiace. Non disponiamo più di questo articolo. Riprovate più tardi.",

	//A popup that tells the user that the store is temporarily sold out

	L"Ci dispiace, ma siamo momentaneamente sprovvisti di oggetti di questo genere.",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Questo è il negozio con la fornitura militare e le armi più recenti e potenti!",
	L"Possiamo trovare la soluzione perfetta per tutte le vostre esigenze riguardo agli esplosivi.",
	L"Oggetti usati e riparati",

	//Text for the various links to the sub pages

	L"Varie",
	L"ARMI",
	L"MUNIZIONI",		//5
	L"GIUBB. A-P",

	//Details on the web site

	L"Se non lo vendiamo, non potrete averlo!",
	L"In costruzione",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"Membri dell'A.I.M.",				// Title
	// Title for the way to sort
	L"Ordine per:",					

	// sort by...

	L"Prezzo",
	L"Esperienza",
	L"Mira",
	L"Meccanica",
	L"Esplosivi",
	L"Pronto socc.",
	L"Salute",
	L"Agilità",
	L"Destrezza",
	L"Forza",
	L"Comando",
	L"Saggezza",
	L"Nome",

	//Text of the links to other AIM pages

	L"Visualizza le facce dei mercenari disponibili",
	L"Rivedi il file di ogni singolo mercenario",
	L"Visualizza la galleria degli associati dell'A.I.M.",

	// text to display how the entries will be sorted

	L"Crescente",
	L"Decrescente",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Indietro",
	L"Home Page", 
	L"Indice",
	L"Avanti",
	L"Disaccordo",
	L"Accordo",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Clic sinistro",
	L"per contattarlo",
	L"Clic destro",
	L"per i mercenari disponibili.",
};					

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Salute",
	L"Agilità",
	L"Destrezza",
	L"Forza",
	L"Comando",
	L"Saggezza",
	L"Esperienza",
	L"Mira",
	L"Meccanica",
	L"Esplosivi",
	L"Pronto socc.",				//10

	// the contract expenses' area

	L"Paga",							
	L"Durata",				
	L"1 giorno",					
	L"1 settimana",
	L"2 settimane",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Indietro",
	L"Contratto",
	L"Avanti",

	L"Ulteriori informazioni",				// Title for the additional info for the merc's bio
	L"Membri attivi",		//20		// Title of the page
	L"Dispositivo opzionale:",				// Displays the optional gear cost
	L"gear",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's // TODO.Translate
	L"Deposito MEDICO richiesto",			// If the merc required a medical deposit, this is displayed
	L"Kit 1",				// Text on Starting Gear Selection Button 1  // TODO.Translate
	L"Kit 2",				// Text on Starting Gear Selection Button 2
	L"Kit 3",				// Text on Starting Gear Selection Button 3
	L"Kit 4",				// Text on Starting Gear Selection Button 4
	L"Kit 5",				// Text on Starting Gear Selection Button 5
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"Costo del contratto:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"1 giorno",										
	L"1 settimana",
	L"2 settimane",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Nessun equip.",
	L"Compra equip.",

	// Text on the Buttons

	L"TRASFERISCI FONDI",			// to actually hire the merc
	L"ANNULLA",				// go back to the previous menu
	L"ARRUOLA",				// go to menu in which you can hire the merc
	L"TACI",				// stops talking with the merc
	L"OK",									
	L"LASCIA MESSAGGIO",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Videoconferenza con",		
	L"Connessione...",

	L"con deposito"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"TRASFERIMENTO ELETTRONICO FONDI RIUSCITO",	// You hired the merc
	L"NON IN GRADO DI TRASFERIRE",		// Player doesn't have enough money, message 1
	L"FONDI INSUFFICIENTI",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"In missione",											
	L"Lascia messaggio",
	L"Deceduto",

	//If you try to hire more mercs than game can support

	L"Avete già una squadra di mercenari.",

	L"Messaggio già registrato",
	L"Messaggio registrato",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"Collegamenti dell'A.I.M.",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"Storia dell'A.I.M.",					//Title

	// Text on the buttons at the bottom of the page

	L"Indietro",
	L"Home Page", 
	L"Associati",
	L"Avanti",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Prezzo",
	L"Esperienza",
	L"Mira",
	L"Meccanica",
	L"Esplosivi",
	L"Pronto socc.",
	L"Salute",
	L"Agilità",
	L"Destrezza",
	L"Forza",
	L"Comando",
	L"Saggezza",
	L"Nome",

	// The title of the page, the above text gets added at the end of this text

	L"Membri scelti dell'A.I.M. in ordine crescente secondo %s",
	L"Membri scelti dell'A.I.M. in ordine decrescente secondo %s",

	// Instructions to the players on what to do

	L"Clic sinistro",
	L"Per scegliere un mercenario.",			//10
	L"Clic destro",
	L"Per selezionare opzioni",

	// Gets displayed on top of the merc's portrait if they are...

	L"Via",
	L"Deceduto",						//14
	L"In missione",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"PAGINA 1",
	L"PAGINA 2",
	L"PAGINA 3",

	L"Membri dell'A.I.M.",	// Title of the page

	L"FINE",			// Stops displaying information on selected merc
	L"Next page",	// TODO.Translate
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"A.I.M. e il logo A.I.M. sono marchi registrati in diversi paesi.",
	L"Di conseguenza, non cercate di copiarci.",
	L"Copyright 1998-1999 A.I.M., Ltd. Tutti i diritti riservati.",

	//Text for an advertisement that gets displayed on the AIM page

	L"Servizi riuniti floreali",
	L"\"Atterriamo col paracadute ovunque\"",				//10
	L"Fallo bene",
	L"... la prima volta",
	L"Se non abbiamo armi e oggetti, non ne avrete bisogno.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"Home Page",
	L"Membri",
	L"Associati",
	L"Assicurazioni",
	L"Storia",
	L"Collegamenti",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"MERCANZIA IN STOCK",		//Header for the merchandise available
	L"PAGINA",				//The current store inventory page being displayed
	L"COSTO TOTALE",				//The total cost of the the items in the Dealer inventory area
	L"VALORE TOTALE",			//The total value of items player wishes to sell
	L"STIMATO",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSAZIONE",			//Button text which completes the deal. Makes the transaction.
	L"FINE",				//Text for the button which will leave the shopkeeper interface.
	L"COSTO DI RIPARAZIONE",			//The amount the dealer will charge to repair the merc's goods
	L"1 ORA",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d ORE",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"RIPARATO",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Non c'è abbastanza spazio nel vostro margine di ordine.",	//Message box that tells the user there is no more room to put there stuff
	L"%d MINUTI",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Lascia oggetto a terra.",
	L"BUDGET",	// TODO.Translate
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
	L"OK",						// Transfer the money
	L"Prendi",					// Take money from the player
	L"Dai",					// Give money to the player
	L"Annulla",					// Cancel the transfer
	L"Pulisci",					// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Seleziona tipo",			// tells the user to select either to give or take from the merc
	L"Inserisci somma",			// Enter the amount to transfer
	L"Trasferisci fondi al mercenario",		// Giving money to the merc
	L"Trasferisci fondi dal mercenario",		// Taking money from the merc
	L"Fondi insufficienti",			// Not enough money to transfer
	L"Bilancio",				// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Volete sottrarre %s dal vostro conto principale per coprire la differenza?",
	L"Fondi insufficienti. Avete pochi %s",
	L"Volete sottrarre %s dal vostro conto principale per coprire la spesa?",
	L"Rivolgetevi all'operatore per iniziare la transazione",
	L"Rivolgetevi all'operatore per riparare gli oggetti selezionati",
	L"Fine conversazione",
	L"Bilancio attuale",

	L"Do you want to transfer %s Intel to cover the difference?",	// TODO.Translate
	L"Do you want to transfer %s Intel to cover the cost?",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Salva partita",
	L"Carica partita",
	L"Abbandona",
	L"Next",
	L"Prev",
	L"Fine",
	L"1.13 Features",
	L"New in 1.13",
	L"Options",

	//Text above the slider bars
	L"Effetti",
	L"Parlato",
	L"Musica",

	//Confirmation pop when the user selects..
	L"Volete terminare la partita e tornare al menu principale?",

	L"Avete bisogno dell'opzione 'Parlato' o di quella 'Sottotitoli' per poter giocare.",
};

STR16	z113FeaturesScreenText[] =
{
	L"1.13 FEATURE TOGGLES",
	L"Changing these settings during a campaign will affect your experience.",
	L"Hover over a feature to display more information. Some features may be configurable in JA2_Options.ini (or other specified file).",
};

STR16	z113FeaturesToggleText[] =
{
	L"Use These Overrides",
	L"New Chance to Hit",
	L"Enemies Drop All",
	L"Enemies Drop All (Damaged)",
	L"Suppression Fire",
	L"Drassen Counterattack",
	L"City Counterattacks",
	L"Multiple Counterattacks",
	L"Intel",
	L"Prisoners",
	L"Mines Require Workers",
	L"Enemy Ambushes",
	L"Enemy Assassins",
	L"Enemy Roles",
	L"Enemy Role: Medic",
	L"Enemy Role: Officer",
	L"Enemy Role: General",
	L"Kerberus",
	L"Mercs Need Food",
	L"Disease",
	L"Dynamic Opinions",
	L"Dynamic Dialogue",
	L"Arulco Strategic Division",
	L"ASD: Helicopters",
	L"Enemy Vehicles Can Move",
	L"Zombies",
	L"Bloodcat Raids",
	L"Bandit Raids",
	L"Zombie Raids",
	L"Militia Volunteer Pool",
	L"Tactical Militia Command",
	L"Strategic Militia Command",
	L"Militia Uses Sector Equipment",
	L"Militia Requires Resources",
	L"Enhanced Close Combat",
	L"Improved Interrupt System",
	L"Weapon Overheating",
	L"Weather: Rain",
	L"Weather: Lightning",
	L"Weather: Sandstorms",
	L"Weather: Snow",
	L"Mini Events",
	L"Arulco Rebel Command",
};

STR16	z113FeaturesHelpText[] =
{
	L"|U|s|e |T|h|e|s|e |O|v|e|r|r|i|d|e|s\n \nAllow this screen to override some feature toggles present in JA2_Options.ini.\nHover over a feature to see which flag is overridden.\nThese toggles have no effect if this option is disabled.",
	L"|N|C|T|H\nOverrides [Tactical Gameplay Settings] NCTH\n \nUse the new chance to hit system.\n \nFor tweakable values, see CTHConstants.ini.",
	L"|E|n|e|m|i|e|s |D|r|o|p |E|v|e|r|y|t|h|i|n|g\nOverrides [Tactical Difficulty Settings] DROP_ALL\n \nEnemies drop all of their gear when killed.\n \nNot compatible with Enemies Drop All (Damaged).",
	L"|E|n|e|m|i|e|s |D|r|o|p |E|v|e|r|y|t|h|i|n|g |(|D|a|m|a|g|e|d|)\nOverrides [Tactical Difficulty Settings] DROP_ALL\n \nEnemies drop all of their gear when killed, but things that would normally not be dropped are severely damaged.\n \nNot compatible with Enemies Drop All.",
	L"|S|u|p|p|r|e|s|s|i|o|n |F|i|r|e\nOverrides [Tactical Suppression Fire Settings] SUPPRESSION_EFFECTIVENESS\n \nCombatants can be affected by suppression and shock.\n \nFor configurable options, see [Tactical Suppression Fire Settings].",
	L"|D|r|a|s|s|e|n |C|o|u|n|t|e|r|a|t|t|a|c|k\nOverrides [Strategic Event Settings] TRIGGER_MASSIVE_ENEMY_COUNTERATTACK_AT_DRASSEN\n \nThe Queen sends a massive counterattack to Drassen.",
	L"|C|i|t|y |C|o|u|n|t|e|r|a|t|t|a|c|k|s\nOverrides [Strategic Event Settings] AGGRESSIVE_STRATEGIC_AI\n \nThe Queen can send a massive counterattack to cities other than Drassen.\n \nNot compatible with Multiple Counterattacks.",
	L"|M|u|l|t|i|p|l|e |C|o|u|n|t|e|r|a|t|t|a|c|k|s\nOverrides [Strategic Event Settings] AGGRESSIVE_STRATEGIC_AI\n \nThe Queen can send massive counterattacks to cities other than Drassen.\nThis can occur multiple times.\n \nThis is a harder variant of City Counterattacks.",
	L"|I|n|t|e|l\nOverrides [Intel Settings] RESOURCE_INTEL\n \nA new resource gained through covert means.",
	L"|P|r|i|s|o|n|e|r|s\nOverrides [Strategic Gameplay Settings] ALLOW_TAKE_PRISONERS\n \nCapture enemy soldiers and interrogate them.\n \nConfigurable Options:\nENEMY_CAN_SURRENDER\nDISPLAY_SURRENDER_VALUES\nSURRENDER_MULTIPLIER\nPRISONER_RETURN_TO_ARMY_CHANCE\nPRISONER_DEFECT_CHANCE\nPRISONER_INTEL_CHANCE\nPRISONER_RANSOM_CHANCE\nPRISONER_INTERROGATION_POINTS_ADMIN\nPRISONER_INTERROGATION_POINTS_REGULAR\nPRISONER_INTERROGATION_POINTS_ELITE\nPRISONER_INTERROGATION_POINTS_OFFICER\nPRISONER_INTERROGATION_POINTS_GENERAL\nPRISONER_INTERROGATION_POINTS_CIVILIAN",
	L"|M|i|n|e|s |R|e|q|u|i|r|e |W|o|r|k|e|r|s\nOverrides [Financial Settings] MINE_REQUIRES_WORKERS\n \nMines require workers to operate.\n \nConfigurable Options:\nWORKERRATE_PRESENT_INITIALLY\nWORKER_TRAINING_COST\nWORKER_TRAINING_POINTS",
	L"|E|n|e|m|y |A|m|b|u|s|h|e|s\nOverrides [Tactical Difficulty Settings] ENABLE_CHANCE_OF_ENEMY_AMBUSHES\n \nEnemy forces can ambush your mercs as they move in the strategic view.\n \nConfigurable Options:\nENEMY_AMBUSHES_CHANCE_MODIFIER\nAMBUSH_MERCS_SPREAD\nAMBUSH_MERCS_SPREAD_RADIUS\nAMBUSH_ENEMY_ENCIRCLEMENT\nAMBUSH_ENEMY_ENCIRCLEMENT_RADIUS1\nAMBUSH_ENEMY_ENCIRCLEMENT_RADIUS2",
	L"|E|n|e|m|y |A|s|s|a|s|s|i|n|s\nOverrides [Tactical Difficulty Settings] ENEMY_ASSASSINS\n \nAssassins can infiltrate your militia.\nRequires the new trait system.\n \nConfigurable Options:\nASSASSIN_MINIMUM_PROGRESS\nASSASSIN_MINIMUM_MILITIA\nASSASSIN_PROPABILITY_MODIFIER",
	L"|E|n|e|m|y |R|o|l|e|s\nOverrides [Tactical Enemy Role Settings] ENEMYROLES\n \nSpecialists can appear in enemy groups.\n \nConfigurable Options:\nENEMYROLES_TURNSTOUNCOVER",
	L"|E|n|e|m|y |R|o|l|e|: |M|e|d|i|c\nOverrides [Tactical Enemy Role Settings] ENEMY_MEDICS\n \nMedics can appear in enemy groups.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_MEDICS_MEDKITDRAINFACTOR\nENEMY_MEDICS_SEARCHRADIUS\nENEMY_MEDICS_WOUND_MINAMOUNT\nENEMY_MEDICS_HEAL_SELF",
	L"|E|n|e|m|y |R|o|l|e|: |O|f|f|i|c|e|r\nOverrides [Tactical Enemy Role Settings] ENEMY_OFFICERS\n \nOfficers can appear in enemy groups.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_OFFICERS_REQUIREDTEAMSIZE\nENEMY_OFFICERS_MAX\nENEMY_OFFICERS_SUPPRESSION_RESISTANCE_BONUS\nENEMY_OFFICERS_MORALE_MODIFIER\nENEMY_OFFICERS_SURRENDERSTRENGTHBONUS",
	L"|E|n|e|m|y |R|o|l|e|: |G|e|n|e|r|a|l\nOverrides [Tactical Enemy Role Settings] ENEMY_GENERALS\n \nGenerals increase enemy strategic movement and decision speeds.\nRequires the Enemy Roles feature to be enabled.\n \nConfigurable Options:\nENEMY_GENERALS_NUMBER\nENEMY_GENERALS_BODYGUARDS_NUMBER\nENEMY_GENERALS_STRATEGIC_DECISION_SPEEDBONUS\nENEMY_GENERALS_STRATEGIC_MOVEMENT_SPEEDBONUS",
	L"|K|e|r|b|e|r|u|s\nOverrides [PMC Settings] PMC\n \nHire militia from a private military company.\n \nConfigurable Options:\nPMC_MAX_REGULARS\nPMC_MAX_VETERANS",
	L"|F|o|o|d\nOverrides [Tactical Food Settings] FOOD\n \nYour mercs require food and water to survive.\n \nConfigurable Options:\nFOOD_DIGESTION_HOURLY_BASE_FOOD\nFOOD_DIGESTION_HOURLY_BASE_DRINK\nFOOD_DIGESTION_SLEEP\nFOOD_DIGESTION_TRAVEL_VEHICLE\nFOOD_DIGESTION_TRAVEL\nFOOD_DIGESTION_ASSIGNMENT\nFOOD_DIGESTION_ONDUTY\nFOOD_DIGESTION_COMBAT\nFOOD_DECAY_IN_SECTORS\nFOOD_DECAY_MODIFICATOR\nFOOD_EATING_SOUNDS",
	L"|D|i|s|e|a|s|e\nOverrides [Disease Settings] DISEASE\n \nYour mercs can catch diseases.\n \nConfigurable Options:\nDISEASE_STRATEGIC\nDISEASE_WHO_SUBSCRIPTIONCOST\nDISEASE_CONTAMINATES_ITEMS\nDISEASE_SEVERE_LIMITATIONS",
	L"|D|y|n|a|m|i|c |O|p|i|n|i|o|n|s\nOverrides [Dynamic Opinion Settings] DYNAMIC_OPINIONS\n \nMercs can form dynamic opinions of each other, affecting morale.\n \nConfigurable Options:\nDYNAMIC_OPINIONS_SHOWCHANGE\nWAGE_ACCEPTANCE_FACTOR\nSee [Dynamic Opinion Modifiers Settings] in Morale_Settings.ini.",
	L"|D|y|n|a|m|i|c |D|i|a|l|o|g|u|e\nOverrides [Dynamic Dialogue Settings] DYNAMIC_DIALOGUE\n \nMercs can make brief comments, changing their relationship to each other.\nRequires the Dynamic Opinions feature to be active.\n \nConfigurable Options:\nDYNAMIC_DIALOGUE_TIME_OFFSET",
	L"|A|S|D\nOverrides [Strategic Additional Enemy AI Settings] ASD_ACTIVE\n \nThe Arulcan army gains mechanised forces.\n \nConfigurable Options:\nASD_COST_FUEL/JEEP/TANK/ROBOT\nASD_TIME_FUEL/JEEP/TANK/ROBOT\nASD_ASSIGNS_JEEPS/TANKS/ROBOTS\nASD_FUEL_REQUIRED_JEEP/TANK/ROBOT\nJEEP_MINIMUM_PROGRESS\nTANK_MINIMUM_PROGRESS\nROBOT_MINIMUM_PROGRESS",
	L"|A|S|D |H|e|l|i|c|o|p|t|e|r|s\nOverrides [Enemy Helicopter Settings] ENEMYHELI_ACTIVE\n \nThe AI can use helicopters to deploy troops.\nRequires the Arulco Special Division feature to be enabled.\n \nConfigurable Options:\nASD_COST_HELI\nASD_TIME_HELI\nENEMYHELI_DEFINITE_UNLOCK_AT_PROGRESS\nENEMYHELI_HP\nENEMYHELI_HP_REPAIRTIME\nENEMYHELI_HP_COST\nENEMYHELI_FUEL\nENEMYHELI_FUEL_REFUELTIME",
	L"|E|n|e|m|y |V|e|h|i|c|l|e|s |C|a|n |M|o|v|e\nOverrides [Tactical Gameplay Settings] ENEMY_TANKS_CAN_MOVE_IN_TACTICAL\n \nHostile jeeps and tanks can move in combat.\n \nConfigurable Options:\nALLOW_TANKS_DRIVING_OVER_PEOPLE\nTANKS_RAMMING_MAX_STRUCTURE_ARMOUR\nENEMY_JEEP_RAMMING_MAX_STRUCTURE_ARMOUR",
	L"|Z|o|m|b|i|e|s\nOverrides the \"Allow Zombies\" toggle in the options menu.\n \nThe dead walk!\n \nConfigurable Options:\nZOMBIE_RISE_BEHAVIOUR\nZOMBIE_SPAWN_WAVES\nZOMBIE_RISE_WAVE_FREQUENCY\nZOMBIE_CAN_CLIMB\nZOMBIE_CAN_JUMP_WINDOWS\nZOMBIE_EXPLODING_CIVS\nZOMBIE_DAMAGE_RESISTANCE\nZOMBIE_BREATH_DAMAGE_RESISTANCE\nZOMBIE_ONLY_HEADSHOTS_WORK\nZOMBIE_DIFFICULTY_LEVEL\nZOMBIE_RISE_WITH_ARMOUR\nZOMBIE_ONLY_HEADSHOTS_PERMANENTLY_KILL",
	L"|B|l|o|o|d|c|a|t |R|a|i|d|s\nOverrides [Raid Settings] RAID_BLOODCATS\n \nHungry predators stalk the night.\n \nConfigurable Options:\nRAID_MAXSIZE_BLOODCATS\nRAID_MAXATTACKSPERNIGHT_BLOODCATS",
	L"|B|a|n|d|i|t |R|a|i|d|s\nOverrides [Raid Settings] RAID_BANDITS\n \nOpportunistic bandits may attack your towns.\n \nConfigurable Options:\nRAID_MAXSIZE_BANDITS\nRAID_MAXATTACKSPERNIGHT_BANDITS",
	L"|Z|o|m|b|i|e |R|a|i|d|s\nOverrides [Raid Settings] RAID_ZOMBIES\n \nThe dead raid!\nRequires the Zombies feature to be enabled.\n \nConfigurable Options:\nRAID_MAXSIZE_ZOMBIES\nRAID_MAXATTACKSPERNIGHT_ZOMBIES",
	L"|M|i|l|i|t|i|a |V|o|l|u|n|t|e|e|r |P|o|o|l\nOverrides [Militia Volunteer Pool Settings] MILITIA_VOLUNTEER_POOL\n \nVolunteers are required to train militia.\n \nConfigurable Options:\nMILITIA_VOLUNTEER_POOL_GAINFACTOR_LIBERATION\nMILITIA_VOLUNTEER_POOL_MULTIPLIER_FARM\nMILITIA_VOLUNTEER_POOL_GAINFACTOR_HOURLY",
	L"|T|a|c|t|i|c|a|l |M|i|l|i|t|i|a |C|o|m|m|a|n|d\nOverrides [Tactical Interface Settings] ALLOW_TACTICAL_MILITIA_COMMAND\n \nIssue commands to militia in tactical view.",
	L"|S|t|r|a|t|e|g|i|c |M|i|l|i|t|i|a |C|o|m|m|a|n|d\nOverrides [Militia Strategic Movement Settings] ALLOW_MILITIA_STRATEGIC_COMMAND\n \nIssue commands to militia in the strategic map.\n \nConfigurable Options:\nMILITIA_STRATEGIC_COMMAND_REQUIRES_MERC",
	L"|M|i|l|i|t|i|a |U|s|e|s |S|e|c|t|o|r |E|q|u|i|p|m|e|n|t\nOverrides [Militia Equipment Settings] MILITIA_USE_SECTOR_EQUIPMENT\n \nMilitia uses gear from their current sector.\nNot compatible with the Militia Requires Resources feature.\n \nConfigurable Options:\nMILITIA_USE_SECTOR_EQUIPMENT_ARMOUR\nMILITIA_USE_SECTOR_EQUIPMENT_FACE\nMILITIA_USE_SECTOR_EQUIPMENT_MELEE\nMILITIA_USE_SECTOR_EQUIPMENT_GUN\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO\nMILITIA_USE_SECTOR_EQUIPMENT_GUN_ATTACHMENTS\nMILITIA_USE_SECTOR_EQUIPMENT_GRENADE\nMILITIA_USE_SECTOR_EQUIPMENT_LAUNCHER\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_MIN\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_MAX\nMILITIA_USE_SECTOR_EQUIPMENT_AMMO_OPTIMAL_MAG_COUNT\nMILITIA_USE_SECTOR_EQUIPMENT_CLASS_SPECIFIC_TABOOS",
	L"|M|i|l|i|t|i|a |R|e|q|u|i|r|e|s |R|e|s|o|u|r|c|e|s\nOverrides [Militia Resource Settings] MILITIA_REQUIRE_RESOURCES\n \nMilitia require resources to be trained.\nNot compatible with the Militia Uses Sector Equipment feature.\n \nConfigurable Options:\nMILITIA_RESOURCES_PROGRESSFACTOR\nMILITIA_RESOURCES_ITEMCLASSMOD_AMMO_BULLET\nMILITIA_RESOURCES_ITEMCLASSMOD_GUN\nMILITIA_RESOURCES_ITEMCLASSMOD_ARMOUR\nMILITIA_RESOURCES_ITEMCLASSMOD_MELEE\nMILITIA_RESOURCES_ITEMCLASSMOD_BOMB\nMILITIA_RESOURCES_ITEMCLASSMOD_GRENADE\nMILITIA_RESOURCES_ITEMCLASSMOD_FACE\nMILITIA_RESOURCES_ITEMCLASSMOD_LBE\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_LOW\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_MEDIUM\nMILITIA_RESOURCES_ITEMCLASSMOD_ATTACHMENT_HIGH\nMILITIA_RESOURCES_WEAPONMOD_PISTOL\nMILITIA_RESOURCES_WEAPONMOD_M_PISTOL\nMILITIA_RESOURCES_WEAPONMOD_SMG\nMILITIA_RESOURCES_WEAPONMOD_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_SN_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_AS_RIFLE\nMILITIA_RESOURCES_WEAPONMOD_LMG\nMILITIA_RESOURCES_WEAPONMOD_SHOTGUN",
	L"|E|n|h|a|n|c|e|d |C|l|o|s|e |C|o|m|b|a|t\nOverrides [Tactical Gameplay Settings] ENHANCED_CLOSE_COMBAT_SYSTEM\n \nA general improvement to the close combat system.",
	L"|I|m|p|r|o|v|e|d |I|n|t|e|r|r|u|p|t |S|y|s|t|e|m\nOverrides [Tactical Gameplay Settings] IMPROVED_INTERRUPT_SYSTEM\n \nAn overhaul of the interrupt mechanic.\n \nConfigurable Options:\nBASIC_PERCENTAGE_APS_REGISTERED\nPERCENTAGE_APS_REGISTERED_PER_EXP_LEVEL\nBASIC_REACTION_TIME_LENGTH\nALLOW_COLLECTIVE_INTERRUPTS\nALLOW_INSTANT_INTERRUPTS_ON_SPOTTING",
	L"|O|v|e|r|h|e|a|t|i|n|g\nOverrides [Tactical Weapon Overheating Settings] OVERHEATING\n \nWeapons can overheat.\n \nConfigurable Options:\nOVERHEATING_DISPLAY_JAMPERCENTAGE\nOVERHEATING_DISPLAY_THERMOMETER_RED_OFFSET\nOVERHEATING_COOLDOWN_MODIFICATOR_LONELYBARREL",
	L"|W|e|a|t|h|e|r|: |R|a|i|n\nOverrides [Tactical Weather Settings] ALLOW_RAIN\n \nRain can reduce visibility.\n \nConfigurable Options:\nRAIN_EVENTS_PER_DAY\nRAIN_CHANCE_PER_DAY\nRAIN_MIN_LENGTH_IN_MINUTES\nRAIN_MAX_LENGTH_IN_MINUTES\nMAX_RAIN_DROPS\nWEAPON_RELIABILITY_REDUCTION_RAIN\nBREATH_GAIN_REDUCTION_RAIN\nVISUAL_DISTANCE_DECREASE_RAIN\nHEARING_REDUCTION_RAIN",
	L"|W|e|a|t|h|e|r|: |L|i|g|h|t|n|i|n|g\nOverrides [Tactical Weather Settings] ALLOW_LIGHTNING\n \nLightning may occur during rainstorms.\nRequires Weather: Rain\n \nConfigurable Options:\nMIN_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS\nMAX_INTERVAL_BETWEEN_LIGHTNINGS_IN_REAL_TIME_SECONDS\nMIN_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS\nMAX_INTERVAL_BETWEEN_LIGHTNING_AND_THUNDERCLAPS_IN_SECONDS\nDELAY_IN_SECONDS_IF_SEEN_SOMEONE_DURING_LIGHTNING_IN_TURNBASED\nCHANCE_TO_DO_LIGHTNING_BETWEEN_TURNS\nWEAPON_RELIABILITY_REDUCTION_THUNDERSTORM\nBREATH_GAIN_REDUCTION_THUNDERSTORM\nVISUAL_DISTANCE_DECREASE_THUNDERSTORM\nHEARING_REDUCTION_THUNDERSTORM",
	L"|W|e|a|t|h|e|r|: |S|a|n|d|s|t|o|r|m|s\nOverrides [Tactical Weather Settings] ALLOW_SANDSTORMS\n \nSevere sandstorms make the battlefield more painful for everyone.\n \nConfigurable Options:\nSANDSTORM_EVENTS_PER_DAY\nSANDSTORM_CHANCE_PER_DAY\nSANDSTORM_MIN_LENGTH_IN_MINUTES\nSANDSTORM_MAX_LENGTH_IN_MINUTES\nWEAPON_RELIABILITY_REDUCTION_SANDSTORM\nBREATH_GAIN_REDUCTION_SANDSTORM\nVISUAL_DISTANCE_DECREASE_SANDSTORM\nHEARING_REDUCTION_SANDSTORM",
	L"|W|e|a|t|h|e|r|: |S|n|o|w\nOverrides [Tactical Weather Settings] ALLOW_SNOW\n \nSnowstorms decrease visibility.\n \nConfigurable Options:\nSNOW_EVENTS_PER_DAY\nSNOW_CHANCE_PER_DAY\nSNOW_MIN_LENGTH_IN_MINUTES\nSNOW_MAX_LENGTH_IN_MINUTES\nWEAPON_RELIABILITY_REDUCTION_SNOW\nBREATH_GAIN_REDUCTION_SNOW\nVISUAL_DISTANCE_DECREASE_SNOW\nHEARING_REDUCTION_SNOW",
	L"|M|i|n|i |E|v|e|n|t|s\nOverrides [Mini Events Settings] MINI_EVENTS_ENABLED\n \nRandom events can occur.\n \nConfigurable Options:\nMINI_EVENTS_MIN_HOURS_BETWEEN_EVENTS\nMINI_EVENTS_MAX_HOURS_BETWEEN_EVENTS\n \nSee MiniEvents.lua for more details.",
	L"|A|R|C\nOverrides [Rebel Command Settings] REBEL_COMMAND_ENABLED\n \nCommand the rebel movement at the strategic level, and upgrade captured towns.\n \nFor tweakable values, see RebelCommand_Settings.ini.",
};

STR16	z113FeaturesPanelText[] =
{
	L"Use the options here to enable some of 1.13's many features. If enabled, the toggle boxes here will take precedence over some booleans in JA2_Options.ini. If disabled, these boxes will have no effect.",
	L"New Chance to Hit (NCTH) is a complete overhaul of the vanilla chance-to-hit system. Compared to the vanilla system, it takes into account more variables when determining the trajectory of a fired bullet.",
	L"This controls whether enemies drop all of their items when they are killed. If enabled, everything is dropped. Otherwise, regular drop chances are used.",
	L"This controls whether enemies drop all of their items when they are killed. If enabled, everything is dropped, and the drop chances are used to determine whether an item is severely damaged.",
	L"Suppression Fire is a way of controlling a battlefield. When under heavy fire, a character accumulates suppression, causing AP loss. In addition to AP loss, Suppression SHOCK can also be accumulated, which makes the character less useful - both Chance-to-Hit and chance to be hit are reduced. Characters can go into negative APs when under suppression fire, losing APs off their NEXT turn. Use suppression fire to prevent enemies from approaching, pin them down, and then advance and kill them while they are hiding. Note that they will try to do the same thing to you!",
	L"When you first capture Drassen, the Queen actually follows through on her command to retake Drassen. This will make holding Drassen VERY difficult in the early game and enabling this option is not recommended for new players!",
	L"Similar to the Drassen Counterattack, other cities can be subjected to massive counterattacks by the Queen after you capture them.",
	L"If City Counterattacks still aren't enough for you, try enabling this! Not only will the Queen send counterattacks, but she may also try to retake multiple cities at the same time.",
	L"You can acquire and spend Intel, a resource closely tied to espionage and information brokering. Intel can be gained by spies, interrogating prisoners, and other ways. It can then be spent at a black market for rare weapons, or at the Recon Intelligence Services website for enemy info.",
	L"Allows you to take prisoners, which can be done by demanding their surrender in combat, or by handcuffing incapacitated soldiers. Once captured, they can be sent to a prison you own and interrogated, which can result in money, intel, or their defection to your militia.",
	L"You will need to invest a little bit in a mine before you can reap its full benefits. Workers are trained like militia, costing time and money. Note that losing control of a sector may cause worker loss!",
	L"Enemy groups have a chance to ambush your squad. If your squad is ambushed, they will be placed in the centre of the map with the enemy group encircling them.",
	L"The queen can now send out assassins. These are elite soldiers that are covert ops experts. They will disguise as members of your militia, and, when the time is right, they will suddenly attack your mercs. New trait system required and new inventory system recommended.",
	L"A variety of enemy types can appear in enemy groups, increasing their overall effectiveness. A small icon will appear next to an enemy who has been observed to have a role. Roles include weapon and equipment specialists, and soldiers carrying keys or keycards.",
	L"Requires the Enemy Roles feature to be enabled. Enemy medics can bandage their wounded comrades and perform field surgery on them.",
	L"Requires the Enemy Roles feature to be enabled. Enemy lieutenants and captains provide sector-wide bonuses for their squad.",
	L"Requires the Enemy Roles feature to be enabled. Generals provide strategic bonuses to the enemy army and may be present in enemy-controlled towns. As high-value targets, they are protected by a small retinue of bodyguards and may flee when they sense danger.",
	L"Some time after you have trained militia, Kerberus will offer its services to you. On their website, you can order security personnel from them, which will act as militia. They require a high down payment but require no training time.",
	L"Your team needs food and water to survive. Without them, they will starve and suffer severe penalties. Keep an eye on the quality of food you're consuming!",
	L"It is possible for your team to catch illnesses from a variety of sources: open wounds, corpses, swamp insects, etc. A disease will give certain mali, and in extreme cases, cause death. Most diseases can be treated by doctors or medicines, and there are items that can provide protection.",
	L"Mercs can form dynamic opinions of each other from a variety of events. These opinion events directly influence a merc's morale and can be both positive and negative. While an opinion event can occur once per day, the impact of one of these events can last for a few days, so it's possible for events to compound over this time period.",
	L"This is an add-on to the Dynamic Opinions feature. This feature allows mercs to talk to each other whenever an opinion event occurs. These dialogue choices may have further impacts on how mercs see each other. If an IMP merc takes part in this, you have a brief window to choose how that character responds.",
	L"The Arulco Special Division is responsible for ordering and deploying mechanised units to the Arulcan army. It uses income gained from mines to purchase vehicles for use against you.",
	L"Requires the Arulco Strategic Division feature to be enabled. At a certain point in your campaign, the ASD can start using helicopters to deploy small squads of elite soldiers to harass you.",
	L"Enemy jeeps and tanks can move around during combat, and may even try to run over your mercs. They will also destroy some obstacles by ramming them.",
	L"Zombies rise from corpses!",
	L"Arulco's deadly bloodcats can attack vulnerable town sectors at night. Civilian deaths will cause loyalty losses.",
	L"Taking advantage of open warfare, bandits can attack vulnerable town sectors. Civilian deaths will cause loyalty losses.",
	L"Requires the Zombies feature to be enabled. Zombies can swarm town sectors. Civilian deaths will cause loyalty losses.",
	L"Before you can train militia, you need willing volunteers. You will need to control both town sectors and the surrounding farmland to bolster your recruit pool.",
	L"Allows you to give commands to militia while in the tactical view. To do this, speak with any militia and a command menu will appear. Mercs wearing Extended Ears or Radio Sets can issue commands to militia that are out of line of sight.",
	L"Allows you to give sector movement commands to militia while in the strategic map. You may need to be in the same sector (unless you have a Radio Operator or someone staffing an HQ) to issue strategic movement orders.",
	L"Militia gear will not be randomly generated, but taken from the sector the militia is currently stationed in. Militia will return their gear to their sector when a new sector is loaded. Gear can be manually dropped in tactical via the 'Ctrl' + '.' menu, under 'militia inspection'. Gear can be prohibited to the militia by hovering over it in the strategic inventory and pressing 'TAB' + 'LMB'. It is up to you to sufficiently distribute gear to your militia.",
	L"In order to be trained, militia require 3 resources: Guns, Armour, and Misc. These can be obtained by converting dropped items in the strategic item view, and ALT + Right clicking on an item. Green Militia just require a Gun, Veteran Militia require a Gun + Armour, and Elite Militia require Gun + Armour + Misc.",
	L"General improvements to the close combat system. Head strikes deal more damage, but are harder to hit. Leg strikes are easier to land, but do less damage. Damage increased to surprised and prone targets. Stealing all items is possible on a knocked-down victim. Several other minor tweaks.",
	L"The Improved Interrupt System (IIS) completely changes how the game determines whether an interrupt occurs. Instead of interrupting as soon as a hostile enters line of sight, the game tracks several variables to simulate a soldier's ability to react and gain an interrupt.",
	L"Weapon barrels heat up as you fire them, potentially opening the door for jams, misfires, and faster weapon degradation. Weapons with replaceable barrels will probably be very useful to keep things cool.",
	L"Toggle rain in tactical. Rain will slightly decrease overall visibility and make it harder to hear things.",
	L"Toggle lightning and thunderstorms during rain. Lightning can reveal soldier positions for both you and the enemy. Thunder makes it significantly harder to hear, and overall breath regeneration is somewhat lowered.",
	L"Toggle sandstorms. Fighting in a sandstorm applies noticable maluses to all combatants - vision and hearing ranges are reduced, weapon degradation is significantly increased, and it is much harder to regain breath.",
	L"Toggle snow. In a snowstorm, it is harder to see, weapons degrade faster, and it is a little harder to regain breath.",
	L"During the course of a campaign, brief events can pop up. You can select one of two responses, which may have positive and/or negative effects. Events can affect a wide variety of things, but mostly your mercs.",
	L"After completing the food delivery quest for the rebels, they will grant you access to their command website (A.R.C.). You can set the rebels' country-wide directive there, and capturing towns allows you to enact policies in that region that provide powerful bonuses. This comes at a price - town loyalty will rise slower, so you will need to work harder to have the locals trust you.",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Salva partita",
	L"Carica partita",
	L"Annulla",
	L"Salvamento selezionato",
	L"Caricamento selezionato",

	L"Partita salvata con successo",
	L"ERRORE durante il salvataggio!",
	L"Partita caricata con successo",
	L"ERRORE durante il caricamento!",

	L"La versione del gioco nel file della partita salvata è diverso dalla versione attuale. È abbastanza sicuro proseguire. Continuate?",
	L"I file della partita salvata potrebbero essere annullati. Volete cancellarli tutti?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"La versionbe salvata è cambiata. Fateci avere un report, se incontrate problemi. Continuate?",
#else
	L"Tentativo di caricare una versione salvata più vecchia. Aggiornate e caricate automaticamente quella salvata?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"La versione salvata e la versione della partita sono cambiate. Fateci avere un report, se incontrate problemi. Continuate?",
#else
	L"Tentativo di caricare una vecchia versione salvata. Aggiornate e caricate automaticamente quella salvata?",
#endif
	L"Siete sicuri di volere sovrascrivere la partita salvata nello slot #%d?",
	L"Volete caricare la partita dallo slot #",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Lo spazio su disco si sta esaurendo. Sono disponibili solo %d MB, mentre per giocare a Jagged dovrebbero esserci almeno %d MB liberi .",	

	L"Salvataggio in corso",			//When saving a game, a message box with this string appears on the screen

	L"Armi normali",
	L"Tonn. di armi",
	L"Stile realistico",
	L"Stile fantascientifico",

	L"Difficoltà",
	L"Platinum Mode", //Placeholder English
	L"Bobby Ray Quality",
	L"Good",
	L"Great",
	L"Excellent",
	L"Awesome",

	L"New Inventory does not work in 640x480 screen resolution. Please increase the screen resolution and try again.",
	L"New Inventory does not work from the default 'Data' folder.",

	L"The squad size from the savegame is not supported by the current screen resolution. Please increase the screen resolution and try again.",
	L"Bobby Ray Quantity", // TODO.Translate
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Livello mappa",
	L"Non avete soldati. Avete bisogno di addestrare gli abitanti della città per poter disporre di un esercito cittadino.",
	L"Entrata giornaliera",
	L"Il mercenario ha l'assicurazione sulla vita", 
	L"%s non è stanco.", 
	L"%s si sta muovendo e non può riposare", 
	L"%s è troppo stanco, prova un po' più tardi.",
	L"%s sta guidando.",
	L"La squadra non può muoversi, se un mercenario dorme.",

	// stuff for contracts
	L"Visto che non potete pagare il contratto, non avete neanche i soldi per coprire il premio dell'assicurazione sulla vita di questo nercenario.",
	L"%s premio dell'assicurazione costerà %s per %d giorno(i) extra. Volete pagare?",
	L"Settore inventario",
	L"Il mercenario ha una copertura medica.",
	
	// other items
	L"Medici", // people acting a field medics and bandaging wounded mercs 
	L"Pazienti", // people who are being bandaged by a medic 
	L"Fine", // Continue on with the game after autobandage is complete 
	L"Ferma", // Stop autobandaging of patients by medics now 
	L"Siamo spiacenti. Questa opzione è stata disabilitata in questo demo.", // informs player this option/button has been disabled in the demo 
	L"%s non ha un kit di riparazione.",
	L"%s non ha un kit di riparazione.",
	L"Non ci sono abbastanza persone che vogliono essere addestrate ora.",
	L"%s è pieno di soldati.",
	L"Il mercenario ha un contratto a tempo determinato.", 
	L"Il contratto del mercenario non è assicurato",
	
	// Flugente: disease texts describing what a map view does TODO.Translate
	L"This view shows how many rotting corpses are in a sector. The white number are corpses, the green number is accumulated disease, the sector colour indicates how widespread it is. GREY= No disease known of. GREEN to RED = escalating levels of disease.",	// TODO.Translate

	// Flugente: weather texts describing what a map view does
	L"This view shows current weather. No colour=Sunny. CYAN=Rain. BLUE=Thunderstorm. ORANGE=Sandstorm. WHITE=Snow.",	// TODO.Translate

	// Flugente: describe what intel map view does
	L"This view shows which sectors relevant what ongoing quests. Some data bought with intel is also shown here.", // TODO.Translate
};


STR16 pLandMarkInSectorString[] =
{
	L"La squadra %d ha notato qualcuno nel settore %s",
	L"La squadra %s ha notato qualcuno nel settore %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Addestrare una squadra dell'esercito cittadino costerà $", // telling player how much it will cost
	L"Approvate la spesa?", // asking player if they wish to pay the amount requested
	L"Non potete permettervelo.", // telling the player they can't afford to train this town
	L"Continuate ad aeddestrare i soldati in %s (%s %d)?", // continue training this town?

	L"Costo $", // the cost in dollars to train militia
	L"(S/N)",   // abbreviated yes/no
	L"",	// unused
	L"Addestrare l'esrecito cittadino nei settori di %d costerà $ %d. %s", // cost to train sveral sectors at once

	L"Non potete permettervi il $%d per addestrare l'esercito cittadino qui.",
	L"%s ha bisogno di una percentuale di %d affinché possiate continuare ad addestrare i soldati.",
	L"Non potete più addestrare i soldati a %s.",
	L"liberate more town sectors", // TODO.Translate

	L"liberate new town sectors", // TODO.Translate
	L"liberate more towns", // TODO.Translate
	L"regain your lost progress", // TODO.Translate
	L"progress further", // TODO.Translate

	L"recruit more rebels", // TODO.Translate
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"Potete prelevare solo fino a $20,000 alla volta.",
	L"Sieti sicuri di voler depositare il %s sul vostro conto?",
};

STR16	gzCopyrightText[] = 
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd. Tutti i diritti riservati.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Parlato",
	L"Conferme mute",
	L"Sottotitoli",
	L"Mettete in pausa il testo del dialogo",
	L"Fumo dinamico",
	L"Sangue e violenza",
	L"Non è necessario usare il mouse!",
	L"Vecchio metodo di selezione",
	L"Mostra il percorso dei mercenari",
	L"Mostra traiettoria colpi sbagliati",
	L"Conferma in tempo reale",
	L"Visualizza gli avvertimenti sveglio/addormentato",
	L"Utilizza il sistema metrico",
	L"Tragitto illuminato durante gli spostamenti",
	L"Sposta il cursore sui mercenari",
	L"Sposta il cursore sulle porte",
	L"Evidenzia gli oggetti",
	L"Mostra le fronde degli alberi",
	L"Smart Tree Tops", // TODO. Translate
	L"Mostra strutture",
	L"Mostra il cursore 3D",
	L"Show Chance to Hit on cursor",
	L"GL Burst uses Burst cursor",
	L"Allow Enemy Taunts", // Changed from "Enemies Drop all Items" - SANDRO
	L"High angle Grenade launching",
	L"Allow Real Time Sneaking", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"Space selects next Squad",
	L"Show Item Shadow",
	L"Show Weapon Ranges in Tiles",
	L"Tracer effect for single shot",
	L"Rain noises",
	L"Allow crows",
	L"Show Soldier Tooltips",
	L"Auto save",
	L"Silent Skyrider",
	L"Enhanced Description Box",
	L"Forced Turn Mode",					// add forced turn mode
	L"Alternate Strategy-Map Colors",		// Change color scheme of Strategic Map
	L"Alternate bullet graphics",			// Show alternate bullet graphics (tracers) // TODO.Translate
	L"Logical Bodytypes (WIP)",
	L"Show Merc Ranks",						// shows mercs ranks	// TODO.Translate
	L"Show Face gear graphics",				// TODO.Translate
	L"Show Face gear icons",
	L"Disabilita Swap Cursore",	            // Disable Cursor Swap
	L"Quiet Training",						// Madd: mercs don't say quotes while training // TODO.Translate
	L"Quiet Repairing",						// Madd: mercs don't say quotes while repairing // TODO.Translate
	L"Quiet Doctoring",						// Madd: mercs don't say quotes while doctoring // TODO.Translate
	L"Auto Fast Forward AI Turns",			// Automatic fast forward through AI turns	// TODO.Translate
	L"Allow Zombies",						// Flugente Zombies
	L"Enable inventory popups",				// the_bob : enable popups for picking items from sector inv	// TODO.Translate
	L"Mark Remaining Hostiles",				// TODO.Translate
	L"Show LBE Content",					// TODO.Translate
	L"Invert mouse wheel",			// TODO.Translate
	L"Formation Movement",					// when multiple mercs are selected, they will try to keep their relative distances	// TODO.Translate
	L"--Cheat Mode Options--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Force Bobby Ray shipments",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--DEBUG OPTIONS--",					// an example options screen options header (pure text)
	L"Report Miss Offsets",					// Screen messages showing amount and direction of shot deviation.	// TODO.Translate
	L"Reset ALL game options",				// failsafe show/hide option to reset all options
	L"Do you really want to reset?",		// a do once and reset self option (button like effect)
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
	L"Attivate questa opzione, se volete ascoltare il dialogo dei personaggi.",

	//Mute Confirmation
	L"Attivate o disattivate le conferme verbali dei personaggi.",

	//Subtitles
	L"Controllate se il testo su schermo viene visualizzato per il dialogo.",

	//Key to advance speech
	L"Se i sottotitoli sono attivati, utilizzate questa opzione per leggere tranquillamente i dialoghi NPC.",

	//Toggle smoke animation
	L"Disattivate questa opzione, se il fumo dinamico diminuisce la frequenza d'aggiornamento.",

	//Blood n Gore
	L"Disattivate questa opzione, se il sangue vi disturba.",

	//Never move my mouse
	L"Disattivate questa opzione per muovere automaticamente il mouse sulle finestre a comparsa di conferma al loro apparire.",

	//Old selection method
	L"Attivate questa opzione per selezionare i personaggi e muoverli come nel vecchio JA (dato che la funzione è stata invertita).",

	//Show movement path
	L"Attivate questa opzione per visualizzare i sentieri di movimento in tempo reale (oppure disattivatela utilizzando il tasto |M|a|i|u|s|c).",

	//show misses
	L"Attivate per far sì che la partita vi mostri dove finiscono i proiettili quando \"sbagliate\".",
	
	//Real Time Confirmation
	L"Se attivata, sarà richiesto un altro clic su \"salva\" per il movimento in tempo reale.",

	//Sleep/Wake notification
	L"Se attivata, verrete avvisati quando i mercenari in \"servizio\" vanno a riposare e quando rientrano in servizio.",

	//Use the metric system
	L"Se attivata, utilizza il sistema metrico di misurazione; altrimenti ricorre al sistema britannico.",

	//Merc Lighted movement
	L"Se attivata, il mercenario mostrerà il terreno su cui cammina. Disattivatela per un aggiornamento più veloce.\nToggle artificial merc light. (|G)",	// TODO.Translate

	//Smart cursor
	L"Se attivata, muovendo il cursore vicino ai vostri mercenari li evidenzierà automaticamente.",

	//snap cursor to the door
	L"Se attivata, muovendo il cursore vicino a una porta farà posizionare automaticamente il cursore sopra di questa.",

	//glow items 
	L"Se attivata, l'opzione evidenzierà gli Oggetti automaticamente. (|C|t|r|l+|A|l|t+|I)",

	//toggle tree tops
	L"Se attivata, mostra le fronde degli alberi. (|T)",

	//smart tree tops
	L"When ON, hides tree tops near visible mercs and cursor position.",	// TODO.Translate

	//toggle wireframe
	L"Se attivata, visualizza le |Strutture dei muri nascosti. (|C|t|r|l+|A|l|t+|W)",

	L"Se attivata, il cursore di movimento verrà mostrato in 3D. (|H|o|m|e)",

	// Options for 1.13
	L"When ON, the chance to hit is shown on the cursor.",
	L"When ON, GL burst uses burst cursor.",
	L"When ON, enemies will occasionally comment certain actions.", // Changed from Enemies Drop All Items - SANDRO
	L"When ON, grenade launchers fire grenades at higher angles. (|A|l|t+|Q)",
	L"When ON, the turn based mode will not be entered when sneaking unnoticed and seeing an enemy unless pressing |C|t|r|l+|X. (|C|t|r|l+|S|h|i|f|t+|X)", // Changed from Restrict Extra Aim Levels - SANDRO
	L"When ON, |S|p|a|c|e selects next squad automatically.",
	L"When ON, item shadows will be shown.",
	L"When ON, weapon ranges will be shown in tiles.",
	L"When ON, tracer effect will be shown for single shots.",
	L"When ON, you will hear rain noises when it is raining.",
	L"When ON, the crows are present in game.",
	L"When ON, a tooltip window is shown when pressing |A|l|t and hovering cursor over an enemy.", 
	L"When ON, game will be saved in 2 alternate save slots after each players turn.",
	L"When ON, Skyrider will not talk anymore.",
	L"When ON, enhanced descriptions will be shown for items and weapons.",
	L"When ON and enemy present, Turn Base mode persists untill sector is free (|C|t|r|l+|T).",	// add forced turn mode
	L"When ON, the Strategic Map will be colored differently based on exploration.",
	L"When ON, alternate bullet graphics will be shown when you shoot.", // TODO.Translate
	L"When ON, mercenary body graphic can change along with equipped gear.\n(not fully implemented yet, will make mercs invisible if activated)",	// TODO.Translate
	L"When ON, ranks will be displayed before merc names in the strategic view.",		// TODO.Translate
	L"When ON, you will see the equipped face gear on the merc portraits.",	// TODO.Translate
	L"When ON, you will see icons for the equipped face gear on the merc portraits in the lower right corner.",
	L"Se attivato, il cursore non si alternerà tra la posizione di scambio e altre azioni. Premere |x per avviare lo scambio rapido.",
	L"When ON, mercs will not report progress during training.",
	L"When ON, mercs will not report progress during repairing.",	// TODO.Translate
	L"When ON, mercs will not report progress during doctoring.",	// TODO.Translate
	L"When ON, AI turns will be much faster.",		// TODO.Translate

	L"When ON, zombies will spawn. Beware!",							// allow zombies	// TODO.Translate
	L"When ON, enables popup boxes that appear when you left click on empty merc inventory slots while viewing sector inventory in mapscreen.",	// TODO.Translate
	L"When ON, approximate locations of the last enemies in the sector are highlighted.",  // TODO.Translate
	L"When ON, show the contents of an LBE item, otherwise show the regular NAS interface.", // TODO.Translate
	L"When ON, inverts mouse wheel directions.",		// TODO.Translate
	L"When ON and multiple mercs are selected, they will try to keep their relative distances while moving. (|C|t|r|l+|A|l|t+|G)",	// TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Force all pending Bobby Ray shipments",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: When ON, will report the distance each bullet deviates from the\ncenter of the target, taking all NCTH factors into account.",
	L"Click me to fix corrupt game settings",							// failsafe show/hide option to reset all options
	L"Click me to fix corrupt game settings",							// a do once and reset self option (button like effect)
	L"Allows debug options in release or mapeditor builds",				// allow debugging in release or mapeditor
	L"Toggle to display debugging render options",						// an example option that will show/hide other options
	L"Attempts to display slash-rects around mouse regions",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};


STR16	gzGIOScreenText[] =
{
	L"INSTALLAZIONE INIZIALE DEL GIOCO",
#ifdef JA2UB
	L"Random Manuel texts",
	L"Off",
	L"On",
#else
	L"Versione del gioco",
	L"Realistica",
	L"Fantascientifica",
#endif	
	L"Platinum", //Placeholder English
	L"Opzioni delle armi",
	L"Available Arsenal", // changed by SANDRO
	L"Varietà di armi",
	L"Normale",
	L"Livello di difficoltà",
	L"Principiante",
	L"Esperto",
	L"Professionista",
	L"Start",		// TODO.Translate
	L"Annulla",
	L"Difficoltà extra",
	L"Tempo illimitato",
	L"Turni a tempo",
	L"Disabilitato per Demo",
	L"Bobby Ray Quality",
	L"Good",
	L"Great",
	L"Excellent",
	L"Awesome",
	L"INSANE",
	L"Inventory / Attachments",	// TODO.Translate
	L"NOT USED",
	L"NOT USED",
	L"Load MP Game",
	L"INITIAL GAME SETTINGS (Only the server settings take effect)",
	// Added by SANDRO
	L"Skill Traits",
	L"Old",
	L"New",
	L"Max IMP Characters",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Enemies Drop All Items",
	L"Off",
	L"On",
#ifdef JA2UB
	L"Tex and John",
	L"Random",
	L"All",
#else
	L"Number of Terrorists",
	L"Random",
	L"All",
#endif	
	L"Secret Weapon Caches",
	L"Random",
	L"All",
	L"Progress Speed of Item Choices",
	L"Very Slow",
	L"Slow",
	L"Normal",
	L"Fast",
	L"Very Fast",

	// TODO.Translate
	L"Old / Old",
	L"New / Old",
	L"New / New",

	// TODO.Translate
	// Squad Size
	L"Max. Squad Size",
	L"6",
	L"8",
	L"10",
	//L"Faster Bobby Ray Shipments",
	L"Inventory Manipulation Costs AP",

	L"New Chance to Hit System",
	L"Improved Interrupt System",
	L"Merc Story Backgrounds", // TODO.Translate
	L"Food System",// TODO.Translate
	L"Bobby Ray Quantity", // TODO.Translate

	// anv: extra iron man modes
	L"Soft Iron Man",	// TODO.Translate
	L"Extreme Iron Man",	// TODO.Translate
};

STR16	gzMPJScreenText[] =
{
	L"MULTIPLAYER",
	L"Join",
	L"Host",
	L"Cancel",
	L"Refresh",
	L"Player Name",
	L"Server IP",
	L"Port",
	L"Server Name",
	L"# Plrs",
	L"Version",
	L"Game Type",
	L"Ping",
	L"You must enter a player name.",
	L"You must enter a valid server IP address. For example: 84.114.195.239",
	L"You must enter a valid Server Port between 1 and 65535.",
};

// TODO.Translate
STR16 gzMPJHelpText[] =
{
	L"Visit http://webchat.quakenet.org/?channels=ja2-multiplayer to find other players.",
	L"You can press 'y' to open the ingame chat window, after you have been connected to the server.",	// TODO.Translate

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

// TODO.Translate
STR16	gzMPHScreenText[] =
{
	L"HOST GAME",
	L"Start",
	L"Cancel",
	L"Server Name",
	L"Game Type",
	L"Deathmatch",
	L"Team-Deathmatch",
	L"Co-Operative",
	L"Maximum Players",
	L"Maximum Mercs",
	L"Merc Selection",
	L"Merc Hiring",
	L"Hired by Player",
	L"Starting Cash",
	L"Allow Hiring Same Merc",
	L"Report Hired Mercs",
	L"Bobby Rays",
	L"Sector Starting Edge",
	L"You must enter a server name",
	L"",
	L"",
	L"Starting Time",
	L"",
	L"",
	L"Weapon Damage",
	L"",
	L"Timed Turns",
	L"",
	L"Enable Civilians in CO-OP",
	L"",
	L"Maximum Enemies in CO-OP",
	L"Synchronize Game Directory",
	L"MP Sync. Directory",
	L"You must enter a file transfer directory.",
	L"(Use '/' instead of '\\' for directory delimiters.)",
	L"The specified synchronisation directory does not exist.",
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
	L"Austin",			//Austin, Texas, USA	
	L"Baghdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Hong Kong",		//Hong Kong, Hong Kong
	L"Beirut",			//Beirut, Lebanon	(Middle East)
	L"Londra",			//London, England
	L"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
	L"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"Mosca",			//Moscow, USSR
	L"New York",		//New York, New York, USA
	L"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Parigi",				//Paris, France
	L"Tripoli",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokyo",				//Tokyo, Japan
	L"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Siete sicuri? Un valore di zero significa NESSUNA abilità.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"(max 8 personaggi)",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Analisi",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Grazie, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Voce", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Ucciso in azione",
	L"Licenziato",
	L"Altro",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Manager del personale",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Partita in pausa",
	L"Riprendi la partita (|P|a|u|s|a)",
	L"Metti in pausa la partita (|P|a|u|s|a)",
};


STR16 pMessageStrings[] =
{
	L"Vuoi uscire dalla partita?",
	L"OK",
	L"SÌ",
	L"NO",
	L"ANNULLA",
	L"RIASSUMI",
	L"MENTI",
	L"Nessuna descrizione", //Save slots that don't have a description.
	L"Partita salvata.",
	L"Partita salvata.",
	L"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"Giorno",
	L"Mercenari",
	L"Slot vuoto", //An empty save game slot
	L"Demo",				//Demo of JA2
	L"Rimuovi",				//State of development of a project (JA2) that is a debug build
	L"Abbandona",			//Release build for JA2
	L"ppm",					//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"dm",					//Abbreviation for minute.
	L"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"colpi",				//Abbreviation for rounds (# of bullets)
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Home Page",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"n/a",					//Lowercase acronym for not applicable.
	L"In corso",		//Meanwhile
	L"%s si trova ora nel settore %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Versione",
	L"Slot di salvataggio rapido vuoto",
	L"Questo slot è riservato ai salvataggi rapidi fatti dalle schermate tattiche e dalla mappa utilizzando ALT+S.",
	L"Aperto",
	L"Chiuso",
	L"Lo spazio su disco si sta esaurendo. Avete liberi solo %s MB e Jagged Alliance 2 v1.13 ne richiede %s.",
	L"Arruolato %s dall'A.I.M.", 
	L"%s ha preso %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s ha assunto %s.", //'Merc name' has taken 'item name'
	L"%s non ha alcuna abilità medica",//'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"L'integrità del gioco è stata compromessa.",
	L"ERRORE: CD-ROM non valido",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Non c'è spazio per sparare da qui.",
	
	//Can't change stance due to objects in the way...
	L"Non potete cambiare posizione questa volta.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Fai cadere",
	L"Getta",
	L"Passa",

	L"%s è passato a %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Nessun spazio per passare %s a %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest (Ceramic Plate 'Attached)'
	L" compreso )",

	//Cheat modes
	L"Raggiunto il livello Cheat UNO",
	L"Raggiunto il livello Cheat DUE",

	//Toggling various stealth modes
	L"Squadra in modalità furtiva.",
	L"Squadra non in modalità furtiva.",
	L"%s in modalità furtiva.",
	L"%s non in modalità furtiva.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Strutture visibili",
	L"Strutture nascoste",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Non potete passare al livello superiore...",
	L"Non esiste nessun livello inferiore...",
	L"Entra nel seminterrato %d...",
	L"Abbandona il seminterrato...",

	L"di",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Modalità segui disattiva.",
	L"Modalità segui attiva.",
	L"Cursore 3D disattivo.",
	L"Cursore 3D attivo.",
	L"Squadra %d attiva.",
	L"Non potete permettervi di pagare a %s un salario giornaliero di %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Salta",
	L"%s non può andarsene da solo.",
	L"Un salvataggio è stato chiamato SaveGame249.sav. Se necessario, rinominatelo da SaveGame01 a SaveGame10 e così potrete accedervi nella schermata di caricamento.",
	L"%s ha bevuto del %s",
	L"Un pacco è arivato a Drassen.",
 	L"%s dovrebbe arrivare al punto designato di partenza (settore %s) nel giorno %d, approssimativamente alle ore %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"Registro aggiornato.",
	L"Grenade Bursts use Targeting Cursor (Spread fire enabled)",
	L"Grenade Bursts use Trajectory Cursor (Spread fire disabled)",
	L"Enabled Soldier Tooltips", // Changed from Drop All On - SANDRO
	L"Disabled Soldier Tooltips", 	// 80	// Changed from Drop All Off - SANDRO
	L"Grenade Launchers fire at standard angles",
	L"Grenade Launchers fire at higher angles",
	// forced turn mode strings
    L"Forced Turn Mode",
	L"Normal turn mode",
	L"Exit combat mode",
	L"Forced Turn Mode Active, Entering Combat",
	L"Salvataggio riuscito della partita nello slot End Turn Auto Save.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",

	// TODO.Translate
	L"You cannot use the Old Inventory and the New Attachment System at the same time.",
	
	// TODO.Translate
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
	L"Artificial Merc Light ON",	// TODO.Translate
	L"Artificial Merc Light OFF",
	L"Squad %s active.",	//TODO.Translate
	L"%s smoked %s.",
	L"Activate cheats?",
	L"Deactivate cheats?",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"OK",
	L"Scorrimento su",
	L"Seleziona tutto",
	L"Scorrimento giù",
	L"Annulla",
};

STR16 pDoctorWarningString[] =
{
	L"%s non è abbstanza vicina per poter esser riparata.",
	L"I vostri medici non sono riusciti a bendare completamente tutti.",
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
	L"Andate all'A.I.M. e arruolate alcuni mercenari (*Hint* è nel Laptop)", // to inform the player to hired some mercs to get things going
#ifdef JA2UB
	L"Quando sarete pronti per partire per Tracona, cliccate sul pulsante nella parte in basso a destra dello schermo.", // to inform the player to hit time compression to get the game underway
#else
	L"Quando sarete pronti per partire per Arulco, cliccate sul pulsante nella parte in basso a destra dello schermo.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Errore. File mancanti o corrotti. Il gioco verrà completato ora.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Visualizza E-mail",
	L"Siti web",
	L"Visualizza file e gli attach delle E-mail",
	L"Legge il registro degli eventi",
	L"Visualizza le informazioni inerenti la squadra",
	L"Visualizza la situazione finanziaria e la storia",
	L"Chiude laptop",

	//Bottom task bar icons (if they exist):
	L"Avete nuove E-mail",
	L"Avete nuovi file",

	//Bookmarks:
	L"Associazione Internazionale Mercenari",
	L"Ordinativi di armi online dal sito di Bobby Ray",
	L"Istituto del Profilo del Mercenario",
	L"Centro più economico di reclutamento",
	L"Impresa di pompe funebri McGillicutty",
	L"Servizio Fioristi Riuniti",
	L"Contratti assicurativi per agenti A.I.M.",
	//New Bookmarks
	L"",
	L"Encyclopedia",
	L"Briefing Room",
	L"Campaign History",	// TODO.Translate
	L"Mercenaries Love or Dislike You",	// TODO.Translate
	L"World Health Organization",
	L"Kerberus - Experience In Security",
	L"Militia Overview",	// TODO.Translate
	L"Recon Intelligence Services",	// TODO.Translate
	L"Controlled factories",	// TODO.Translate
};


STR16 gzHelpScreenText[] =
{
	L"Esci dalla schermata di aiuto",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"È in corso una battaglia. Potete solo ritirarvi dalla schermata delle tattiche.",
	L"|Entra nel settore per continuare l'attuale battaglia in corso.",
	L"|Automaticamente decide l'esito della battaglia in corso.",
	L"Non potete decidere l'esito della battaglia in corso automaticamente, se siete voi ad attaccare.",
	L"Non potete decidere l'esito della battaglia in corso automaticamente, se subite un'imboscata.",
	L"Non potete decidere l'esito della battaglia in corso automaticamente, se state combattendo contro le creature nelle miniere.",
	L"Non potete decidere l'esito della battaglia in corso automaticamente, se ci sono civili nemici.",
	L"Non potete decidere l'esito della battaglia in corso automaticamente, se ci sono dei Bloodcat.",
	L"BATTAGLIA IN CORSO",
	L"Non potete ritirarvi ora.",
};

STR16 gzMiscString[] =
{
	L"I vostri soldati continuano a combattere senza l'aiuto dei vostri mercenari...",
	L"Il veicolo non ha più bisogno di carburante.",
	L"La tanica della benzina è piena %d%%.",
	L"L'esercito di Deidrannaha riguadagnato il controllo completo su %s.",
	L"Avete perso una stazione di rifornimento.",
};

STR16	gzIntroScreen[] = 
{
	L"Video introduttivo non trovato",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s sente un %s rumore proveniente da %s.",
	L"%s sente un %s rumore di MOVIMENTO proveniente da %s.",
	L"%s sente uno %s SCRICCHIOLIO proveniente da %s.",
	L"%s sente un %s TONFO NELL'ACQUA proveniente da %s.",
	L"%s sente un %s URTO proveniente da %s.",
	L"%s hears a %s GUNFIRE coming from %s.", // anv: without this, all further noise notifications were off by 1!		// TODO.Translate
	L"%s sente una %s ESPLOSIONE verso %s.",
	L"%s sente un %s URLO verso %s.",
	L"%s sente un %s IMPATTO verso %s.",
	L"%s sente un %s IMPATTO a %s.",
	L"%s sente un %s SCHIANTO proveniente da %s.",
	L"%s sente un %s FRASTUONO proveniente da %s.",
	L"",											 // anv: placeholder for silent alarm		// TODO.Translate
	L"%s hears someone's %s VOICE coming from %s.", // anv: report enemy taunt to player		// TODO.Translate
};

// TODO.Translate
STR16 pTauntUnknownVoice[] =
{
	L"Unknown Voice",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Nome (|F|1)",
	L"Assegnato (|F|2)",
	L"Tipo di riposo (|F|3)",
	L"Postazione (|F|4)",
	L"Destinazione (|F|5)",
	L"Durata dell'incarico (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Errore 404",
	L"Luogo non trovato.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Spedizioni recenti",
	L"Ordine #",
	L"Numero di oggetti",
	L"Ordinato per",
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
	L"Programmatore del gioco", 			// Chris Camfield
	L"Co-designer / Autore",							// Shaun Lyng
	L"Programmatore sistemi strategici & Editor",					//Kris Marnes
	L"Produttore / Co-designer",						// Ian Currie
	L"Co-designer / Designer della mappa",				// Linda Currie
	L"Grafico",													// Eric \"WTF\" Cheng
	L"Coordinatore beta, supporto",				// Lynn Holowka
	L"Grafico straordinario",						// Norman \"NRG\" Olsen
	L"Guru dell'audio",											// George Brooks
	L"Designer delle schermate / Grafico",					// Andrew Stacey
	L"Capo grafico / Animatore",						// Scot Loving
	L"Capo programmatore",									// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmatore",											// Dave French
	L"Programmatore sistemi & bilancio di gioco",					// Alex Meduna
	L"Grafico dei ritratti",								// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(deve ancora esercitarsi con la punteggiatura)",					// Shaun Lyng
	L"(\"Fatto. Devo solo perfezionarmi\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(sta diventando troppo vecchio per questo)",				// Ian Currie
	L"(sta lavorando a Wizardry 8)",						// Linda Currie
	L"(obbligato a occuparsi anche del CQ)",			// Eric \"WTF\" Cheng
	L"(ci ha lasciato per CFSA...)",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(Testa matta e amante del jazz)",						// Andrew Stacey
	L"(il suo nome vero è Robert)",							// Scot Loving
	L"(l'unica persona responsabile)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(può ora tornare al motocross)",	// Dave French
	L"(rubato da Wizardry 8)",							// Alex Meduna
	L"",	// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s ha finito di riparare gli oggetti.",
	L"%s ha finito di riparare le armi e i giubbotti antiproiettile di tutti.",
	L"%s ha finito di riparare gli oggetti dell'equipaggiamento di tutti.",
	L"%s finished repairing everyone's large carried items.",
	L"%s finished repairing everyone's medium carried items.",
	L"%s finished repairing everyone's small carried items.",
	L"%s finished repairing everyone's LBE gear.",
	L"%s finished cleaning everyone's guns.",	// TODO.Translate
};

STR16 zGioDifConfirmText[]=
{
	//L"You have chosen NOVICE mode. This setting is appropriate for those new to Jagged Alliance, those new to strategy games in general, or those wishing shorter battles in the game. Your choice will affect things throughout the entire course of the game, so choose wisely. Are you sure you want to play in Novice mode?",
	L"Avete selezionato la modalità PRINCIPIANTE. Questo scenario è adatto a chi gioca per la prima volta a Jagged Alliance, a chi prova a giocare per la prima volta in generale o a chi desidera combattere battaglie più brevi nel gioco. La vostra decisione influirà sull'intero corso della partita; scegliete, quindi, con attenzione. Siete sicuri di voler giocare nella modalità PRINCIPIANTE?",

	//L"You have chosen EXPERIENCED mode. This setting is suitable for those already familiar with Jagged Alliance or similar games. Your choice will affect things throughout the entire course of the game, so choose wisely. Are you sure you want to play in Experienced mode?",
	L"Avete selezionato la modalità ESPERTO. Questo scenario è adatto a chi ha già una certa dimestichezza con Jagged Alliance o con giochi simili. La vostra decisione influirà sull'intero corso della partita; scegliete, quindi, con attenzione. Siete sicuri di voler giocare nella modalità ESPERTO?",

	//L"You have chosen EXPERT mode. We warned you. Don't blame us if you get shipped back in a body bag. Your choice will affect things throughout the entire course of the game, so choose wisely. Are you sure you want to play in Expert mode?",
	L"Avete selezionato la modalità PROFESSIONISTA. Siete avvertiti. Non malediteci, se vi ritroverete a brandelli. La vostra decisione influirà sull'intero corso della partita; scegliete, quindi, con attenzione. Siete sicuri di voler giocare nella modalità PROFESSIONISTA?",
 
	L"You have chosen INSANE mode. WARNING: Don't blame us if you get shipped back in little pieces... Deidranna WILL kick your ass.  Hard.  Your choice will affect things throughout the entire course of the game, so choose wisely. Are you sure you want to play in INSANE mode?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S file di dati della schermata di caricamento non trovato...",

	//1-5
	L"Il robot non può lasciare questo settore, se nessuno sta usando il controller.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Non potete comprimere il tempo ora. Aspettate le esplosioni!",

	//'Name' refuses to move.
	L"%s si rifiuta di muoversi.",

	//%s a merc name
	L"%s non ha abbastanza energia per cambiare posizione.",

	//A message that pops up when a vehicle runs out of gas.
	L"Il %s ha esaurito la benzina e ora è rimasto a piedi a %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"sopra",
	L"sotto",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Nessuno dei vostri mercenari non sa praticare il pronto soccorso.",
	L"Non ci sono supporti medici per bendare.",
	L"Non ci sono stati supporti medici sufficienti per bendare tutti.",
	L"Nessuno dei vostri mercenari ha bisogno di fasciature.",
	L"Fascia i mercenari automaticamento.",
	L"Tutti i vostri mercenari sono stati bendati.",

	//14
#ifdef JA2UB
	L"Tracona",
#else
	L"Arulco",
#endif
  L"(tetto)",

	L"Salute: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d contro %d",
	
	L"Il %s è pieno!",  //(ex "The ice cream truck is full")

  L"%s non ha bisogno immediatamente di pronto soccorso o di fasciature, quanto piuttosto di cure mediche più serie e/o riposo.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s è stato colpito alla gamba e collassa!",
	//Name can't speak right now.
	L"%s non può parlare ora.",

	//22-24 plural versions 
	L"%d l'esercito verde è stato promosso a veterano.",
	L"%d l'esercito verde è stato promosso a regolare.",
	L"%d l'esercito regolare è stato promosso a veterano.",

	//25
	L"Interruttore",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s è impazzito!",

	//27-28
	//Messages why a player can't time compress.
	L"Non è al momento sicuro comprimere il tempo visto che avete dei mercenari nel settore %s.",
	L"Non è al momento sicuro comprimere il tempo quando i mercenari sono nelle miniere infestate dalle creature.",

	//29-31 singular versions 
	L"1 esercito verde è stato promosso a veterano.",
	L"1 esercito verde è stato promosso a regolare.",
	L"1 eserciro regolare è stato promosso a veterano.",

	//32-34
	L"%s non dice nulla.",
	L"Andate in superficie?",
	L"(Squadra %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s ha riparato %s's %s",

	//36
	L"BLOODCAT",

	//37-38 "Name trips and falls"
	L"%s trips and falls",
	L"Questo oggetto non può essere raccolto qui.",

	//39
	L"Nessuno dei vostri rimanenti mercenari è in grado di combattere. L'esercito combatterà contro le creature da solo.",

	//40-43
	//%s is the name of merc.
	L"%s è rimasto sprovvisto di kit medici!",
	L"%s non è in grado di curare nessuno!",
	L"%s è rimasto sprovvisto di forniture mediche!",
	L"%s non è in grado di riparare niente!",

	//44-45
	L"Tempo di riparazione",
	L"%s non può vedere questa persona.",

	//46-48
	L"L'estensore della canna dell'arma di %s si è rotto!",
	L"No more than %d militia trainers are permitted in this sector.",		// TODO.Translate
  L"Siete sicuri?",

	//49-50
	L"Compressione del tempo",
	L"La tanica della benzina del veicolo è ora piena.",

	//51-52 Fast help text in mapscreen.
	L"Continua la compressione del tempo (|S|p|a|z|i|o)",
	L"Ferma la compressione del tempo (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s ha sbloccata il %s",
	L"%s ha sbloccato il %s di %s",

	//55 
	L"Non potete comprimere il tempo mentre visualizzate l'inventario del settore.",

	L"Il CD ddel gioco Jagged Alliance 2 v1.13 non è stato trovato. Il programma verrà terminato.",

	L"Oggetti combinati con successo.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Attuale/Massimo Progresso: %d%%/%d%%",

	//59
	L"Accompagnate John e Mary?",
	
  L"Interruttore attivato.",

  L"%s's armour attachment has been smashed!",
	L"%s fires %d more rounds than intended!",
	L"%s fires one more round than intended!",

	L"You need to close the item description box first!",	// TODO.Translate

	L"Cannot compress time - hostile civilians and/or bloodcats are in this sector.", // 65	// TODO.Translate
};

STR16 gzCWStrings[] = 
{
	L"Call reinforcements to %s from adjacent sectors?",		// TODO.Translate
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Location: %d\n",
	L"%s|Brightness: %d / %d\n",
	L"%s|Range to |Target: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Orders: %d\n",
	L"%s|Attitude: %d\n",
	L"%s|Current |A|Ps: %d\n",
	L"%s|Current |Health: %d\n",
	L"%s|Current |Breath: %d\n",	// TODO.Translate
	L"%s|Current |Morale: %d\n",
	L"%s|Current |S|hock: %d\n",// TODO.Translate
	L"%s|Current |S|uppression Points: %d\n",// TODO.Translate
	// Full info
	L"%s|Helmet: %s\n",
	L"%s|Vest: %s\n",
	L"%s|Leggings: %s\n",
	// Limited, Basic
	L"|Armor: ",
	L"Helmet",
	L"Vest",
	L"Leggings",
	L"worn",
	L"no Armor",
	L"%s|N|V|G: %s\n",
	L"no NVG",
	L"%s|Gas |Mask: %s\n",
	L"no Gas Mask",
	L"%s|Head |Position |1: %s\n",
	L"%s|Head |Position |2: %s\n",
	L"\n(in Backpack) ",
	L"%s|Weapon: %s ",
	L"no Weapon",
	L"Handgun",
	L"SMG",
	L"Rifle",
	L"MG",
	L"Shotgun",
	L"Knife",
	L"Heavy Weapon",
	L"no Helmet",
	L"no Vest",
	L"no Leggings",
	L"|Armor: %s\n", 
	 // Added - SANDRO
	L"%s|Skill 1: %s\n",
	L"%s|Skill 2: %s\n", 
	L"%s|Skill 3: %s\n",
    // Additional suppression effects - sevenfm	//  TODO.Translate
    L"%s|A|Ps lost due to |S|uppression: %d\n",
    L"%s|Suppression |Tolerance: %d\n",
    L"%s|Effective |S|hock |Level: %d\n",
    L"%s|A|I |Morale: %d\n",
};

STR16 New113Message[] =
{
	L"Storm started.",
	L"Storm ended.",
	L"Rain started.",
	L"Rain ended.",
	L"Watch out for snipers...",
	L"Suppression fire!",	
	L"BRST",
	L"AUTO",
	L"GL",
	L"GL BRST",
	L"GL AUTO",
	L"UB",
	L"UBRST",
	L"UAUTO",
	L"BAYONET",
	L"Sniper!",
	L"Unable to split money due to having an item on your cursor.",
	L"Arrival of new recruits is being rerouted to sector %s, as scheduled drop-off point of sector %s is enemy occupied.",
	L"Articolo cancellato",
	L"Ha cancellato tutti gli articoli di questo tipo",
	L"Articolo venduto",
	L"Ha venduto tutti gli articoli di questo tipo",
	L"You should check your goggles",
	// Real Time Mode messages
	L"In combat already",
	L"No enemies in sight",
	L"Real-time sneaking OFF",
	L"Real-time sneaking ON",
	//L"Enemy spotted! (Ctrl + x to enter turn based)",
	L"Enemy spotted!", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s was successful on stealing!",
	L"%s had not enough action points to steal all selected items.",
	L"Do you want to make surgery on %s before bandaging? (You can heal about %i Health.)",
	L"Do you want to make surgery on %s? (You can heal about %i Health.)",
	L"Do you wish to make surgeries first? (%i patient(s))",
	L"Do you wish to make the surgery on this patient first?",
	L"Apply first aid automatically with surgeries or without them?",
	L"Do you want to perform surgery on %s before bandaging? (You can heal about %i health, *: %i by blood bag use.)",	// TODO.Translate
	L"Do you want to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"Do you wish to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"Surgery on %s finished.",
	L"%s is hit in the chest and loses a point of maximum health!",
	L"%s is hit in the chest and loses %d points of maximum health!",
	L"%s is blinded by the blast!",
	L"%s has regained one point of lost %s",
	L"%s has regained %d points of lost %s",
	L"Your scouting skills prevented you to be ambushed by the enemy!",
	L"Thanks to your scouting skills you have successfuly avoided a pack of bloodcats!",
	L"%s is hit to groin and falls down in pain!",
	//////////////////////////////////////////////////////////////////////////////////////
	L"Warning: enemy corpse found!!!",
	L"%s [%d rnds]\n%s %1.1f %s",
	L"Insufficient AP Points! Cost %d, you have %d.",	// TODO.Translate
	L"Hint: %s",	// TODO.Translate
	L"Player strength: %d - Enemy strength: %6.0f",		// TODO.Translate Surrender values to be printed, if DISPLAY_SURRENDER_VALUES = TRUE

	L"Cannot use skill!",				// TODO.Translate
	L"Cannot build while enemies are in this sector!",
	L"Cannot spot that location!",
	L"Incorrect GridNo for firing artillery!",
	L"Radio frequencies are jammed. No communication possible!",
	L"Radio action failed!",
	L"Not enough mortar shells in sector to start a barrage!",
	L"No signal shell item found in Items.xml!",
	L"No mortars found, cannot commence barrage!",
	L"Already jamming signal, no need to do so again!",
	L"Already listening for nearby sounds, no need to do so again!",
	L"Already trying to spot, no need to do so again!",
	L"Already scanning for jam signals, no need to do so again!",
	L"%s could not apply %s to %s.",
	L"%s orders reinforcements from %s.",
	L"%s radio set is out of energy.",
	L"a working radio set",
	L"a binocular",
	L"patience",
	L"%s's shield has been destroyed!",	// TODO.Translate
	L" DELAY",	// TODO.Translate
	L"Yes*",
	L"Yes",
	L"No",
	L"%s applied %s to %s.", // TODO.Translate
};

// TODO.Translate
STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s cowers in fear!",
	L"%s is pinned down!",
	L"%s fires more rounds than intended!",
	L"You cannot train militia in this sector.",
	L"Militia picks up %s.",
	L"Cannot train militia with enemies present!", 
	// 6 - 10
	L"%s lacks sufficient Leadership score to train militia.",
	L"No more than %d Mobile Militia trainers are permitted in this sector.",
	L"No room in %s or around it for new Mobile Militia!",
	L"You need to have %d Town Militia in each of %s's liberated sectors before you can train Mobile Militia here.",
	L"Can't staff a facility while enemies are present!",
	// 11 - 15
	L"%s lacks sufficient Wisdom to staff this facility.",
	L"The %s is already fully-staffed.",
	L"It will cost $%d per hour to staff this facility. Do you wish to continue?",
	L"You have insufficient funds to pay for all Facility work today. $%d have been paid, but you still owe $%d. The locals are not pleased.",
	L"You have insufficient funds to pay for all Facility work today. You owe $%d. The locals are not pleased.",
	// 16 - 20
	L"You have an outstanding debt of $%d for Facility Operation, and no money to pay it off!",
	L"You have an outstanding debt of $%d for Facility Operation. You can't assign this merc to facility duty until you have enough money to pay off the entire debt.",
	L"You have an outstanding debt of $%d for Facility Operation. Would you like to pay it all back?",
	L"N/A in this sector",
	L"Daily Expenses",
	// 21 - 25
	L"Insufficient funds to pay all enlisted militia! %d militia have disbanded and returned home.",
	L"To examine an item's stats during combat, you must pick it up manually first.", // HAM 5
	L"To attach an item to another item during combat, you must pick them both up first.", // HAM 5
	L"To merge two items during combat, you must pick them both up first.", // HAM 5
};

// TODO.Translate
// HEADROCK HAM 5: Text dealing exclusively with Item Transformations.
STR16 gzTransformationMessage[] = 
{
	L"No available adjustments",
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
	L"Instant mode",	// TODO.Translate
	L"Delayed mode",
	L"Instant mode (%d AP)",
	L"Delayed mode (%d AP)",
};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Hereby be informed that due to Gastons's past performance his fees for services rendered have undergone an increase. Personally, I'm not surprised. ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Please be advised that, as of this moment, Stogies's fees for services rendered have increased to coincide with the increase in his abilities. ± ± Speck T. Kline ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Please be advised that Tex's experience entitles him to more equitable compensation. He's fees have therefore been increased to more accurately reflect his worth. ± ± Speck T. Kline ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Please take note. Due to the improved performance of Biggens his fees for services rendered have undergone an increase. ± ± Speck T. Kline ± ",
};

// TODO.Translate
// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk: Text from Line 58
	L"FW from AIM Server: Message from Victor Kolesnikov",
	L"Hello. Monk here. Message received. I'm back if you want to see me. ± ± Waiting for your call. ±",

	// Brain: Text from Line 60
	L"FW from AIM Server: Message from Janno Allik",
	L"Am now ready to consider tasks. There is a time and place for everything. ± ± Janno Allik ±",

	// Scream: Text from Line 62
	L"FW from AIM Server: Message from Lennart Vilde",
	L"Lennart Vilde now available! ±",

	// Henning: Text from Line 64
	L"FW from AIM Server: Message from Henning von Branitz",
	L"Have received your message, thanks. To discuss employment, contact me  at the AIM Website. ± ± Till then! ± ± Henning von Branitz ±",

	// Luc: Text from Line 66
	L"FW from AIM Server: Message from Luc Fabre",
	L"Mesage received, merci! Am happy to consider your proposals. You know where to find me. ± ± Looking forward to hearing from you. ±",

	// Laura: Text from Line 68
	L"FW from AIM Server: Message from Dr. Laura Colin",
	L"Greetings! Good of you to leave a message  It sounds interesting. ± ± Visit AIM again I would be happy to hear more. ± ± Best regards! ± ± Dr. Laura Colin ±",

	// Grace: Text from Line 70
	L"FW from AIM Server: Message from Graziella Girelli",
	L"You wanted to contact me, but were not successful.± ± A family gathering. I am sure you understand? I've now had enough of family and would be very happy if you would contact me again over the AIM Site. ± ± Ciao! ±",

	// Rudolf: Text from Line 72
	L"FW from AIM Server: Message from Rudolf Steiger",
	L"Do you know how many calls I get every day? Every tosser thinks he can call me.  ± ± But I'm back, if you have something of interest for me. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"FW from AIM Server: Message about merc availability",
	L"I got your message. Waiting for your call. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Sniper: Occhi di un hawk, potete sparare le ale da un mosca ad cento yarde! ± ",
	// Camouflage
	L"Camuffamento: Oltre voi persino i cespugli sembrano sintetici! ± ",	
	// SANDRO - new strings for new traits added
	// Ranger
	L"Ranger: You are the one from Texas deserts, aren't you! ± ",	
	// Gunslinger
	L"Gunslinger: With a handgun or two, you can be as lethal as the Billy Kid! ± ",
	// Squadleader
	L"Squadleader: Natural leader and boss, you are the big shot no kidding! ± ",
	// Technician
	L"Technician: Fixing stuff, removing traps, planting bombs, that's your bussiness! ± ",
	// Doctor
	L"Doctor: You can make a quick surgery with pocket-knife and chewing gum anywhere! ± ",
	// Athletics
	L"Athletics: Your speed and vitality is on top of possibilities! ± ",
	// Bodybuilding
	L"Bodybuilding: That big muscular figure which cannot be overlooked is you actually! ± ",
	// Demolitions
	L"Demolitions: You can blow up a whole city just by common home stuff! ± ",
	// Scouting
	L"Scouting: Nothing can escape your notice! ± ",
	// Covert ops
	L"Covert Operations: You make 007 look like an amateur! ± ",		// TODO.Translate
	// Radio Operator
	L"Radio Operator: Your usage of communication devices broaden your team's tactical and strategic skills. ± ",	// TOO.Translate
	// Survival
	L"Survival: Nature is a second home to you. ± ",	// TODO.Translate
};

STR16 NewInvMessage[] = 
{
	L"Non può il fagotto della raccolta attualmente",
	L"Nessun posto per mettere fagotto",
	L"Fagotto non trovato",
	L"La chiusura lampo funziona soltanto nel combattimento",
	L"Non può muoversi mentre la chiusura lampo del fagotto attiva",
	L"Siete sicuri voi desiderate vendere tutti gli articoli del settore?",
	L"Siete sicuri voi desiderate cancellare tutti gli articoli del settore?",
	L"Non può arrampicarsi mentre portano uno zaino",
	L"All backpacks dropped",	// TODO.Translate
	L"All owned backpacks picked up",
	L"%s drops backpack",
	L"%s picks up backpack",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Initiating RakNet server...",
	L"Server started, waiting for connections...",
	L"You must now connect with your client to the server by pressing '2'.",
	L"Server is already running.",
	L"Server failed to start. Terminating.",
	// 5
	L"%d/%d clients are ready for realtime-mode.",
	L"Server disconnected and shutdown.",
	L"Server is not running.",
	L"Clients are still loading, please wait...",
	L"You cannot change dropzone after the server has started.",
	// 10
	L"Sent file '%S' - 100/100",
	L"Finished sending files to '%S'.",
	L"Started sending files to '%S'.",
	L"Use the Airspace view to select a map you wish to play. If you want to change the map you have to do this before clicking the 'Start Game' button.",	// TODO.Translate
};

STR16 MPClientMessage[] =
{
	// 0
	L"Initiating RakNet client...",		
	L"Connecting to IP: %S ...",
	L"Received game settings:",
	L"You are already connected.",
	L"You are already connecting...",
	// 5
	L"Client #%d - '%S' has hired '%s'.",
	L"Client #%d - '%S' has hired another merc.",
	L"You are ready - Total ready = %d/%d.",
	L"You are no longer ready - Total ready = %d/%d.",
	L"Starting battle...",
	// 10
	L"Client #%d - '%S' is ready - Total ready = %d/%d.",
	L"Client #%d - '%S' is no longer ready - Total ready = %d/%d",
	L"You are ready. Awaiting other clients... Press 'OK' if you are not ready anymore.",
	L"Let us the battle begin!",
	L"A client must be running for starting the game.",
	// 15
	L"Game cannot start. No mercs are hired...",
	L"Awaiting 'OK' from server to unlock the laptop...",
	L"Interrupted",
	L"Finish from interrupt",
	L"Mouse Grid Coordinates:",
	// 20
	L"X: %d, Y: %d",
	L"Grid Number: %d",
	L"Server only feature",
	L"Choose server manual override stage:  ('1' - Enable laptop/hiring)  ('2' - Launch/load level)  ('3' - Unlock UI)  ('4' - Finish placement)",	
	L"Sector=%s, Max Clients=%d, Max Mercs=%d, Game_Mode=%d, Same Merc=%d, Damage Multiplier=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	// 25
	L"",
	L"New connection: Client #%d - '%S'.",
	L"Team: %d.",//not used any more
	L"'%s' (client %d - '%S') was killed by '%s' (client %d - '%S')",
	L"Kicked client #%d - '%S'",
	// 30
	L"Start a new turn for the selected client. #1: <Cancel>, #2: %S, #3: %S, #4: %S",
	L"Starting turn for client #%d",
	L"Requesting for realtime...",
	L"Switched back to realtime.",
	L"Error: Something went wrong switching back.",
	// 35
	L"Unlock laptop for hiring? (Are all clients connected?)",
	L"The server has unlocked the laptop. Begin hiring!",
	L"Interruptor.",
	L"You cannot change dropzone if you are only the client and not the server.",
	L"You declined the offer to surrender, because you are in a multiplayer game.",
	// 40
	L"All your mercs are wiped dead!",
	L"Spectator mode enabled.",
	L"You have been defeated!",
	L"Sorry, climbing is disable in MP",
	L"You Hired '%s'",
	// 45
	L"You cant change the map once purchasing has commenced",
	L"Map changed to '%s'",
	L"Client '%s' disconnected, removing from game",
	L"You were disconnected from the game, returning to the Main Menu",
	L"Connection failed, Retrying in 5 seconds, %i retries left...",
	//50
	L"Connection failed, giving up...",
	L"You cannot start the game until another player has connected",
	L"%s : %s",
	L"Send to All",
	L"Allies only",
	// 55
	L"Cannot join game. This game has already started.",
	L"%s (team): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Received all files from server.",
	L"'%S' finished downloading from server.",
	L"'%S' started downloading from server.",
	L"Cannot start the game until all clients have finished receiving files",
	L"This server requires that you download modified files to play, do you wish to continue?",
	// 65
	L"Press 'Ready' to enter tactical screen.",
	L"Cannot connect because your version %S is different from the server version %S.",
	L"You killed an enemy soldier.",
	L"Cannot start the game, because all teams are the same.",
	L"The server has choosen New Inventory (NIV), but your screen resolution does not support NIV.",
	// 70		// TODO.Translate
	L"Could not save received file '%S'",
	L"%s's bomb was disarmed by %s",
	L"You loose, what a shame",	// All over red rover
	L"Spectator mode disabled",
	L"Choose client to kick from game. #1: <Cancel>, #2: %S, #3: %S, #4: %S",
	// 75
	L"Team %s is wiped out.",
	L"Client failed to start. Terminating.",
	L"Client disconnected and shutdown.",
	L"Client is not running.",
	L"INFO: If the game is stuck (the opponents progress bar is not moving), notify the server to press ALT + E to give the turn back to you!",	// TODO.Translate
	// 80
	L"AI's turn - %d left",		// TODO.Translate
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"E",
	L"S",
	L"W",
	L"C",	// "C"enter
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
	L"Game Type: ",
	L"Players: ",
	L"Mercs each: ",
	L"You cannot change starting edge once Laptop is unlocked.",
	L"You cannot change teams once the Laptop is unlocked.",
	L"Random Mercs: ",
	L"Y",
	L"Difficulty:",
	L"Server Version:",
};

STR16 gzMPSScreenText[] =
{
	L"Scoreboard",
	L"Continue",
	L"Cancel",
	L"Player",
	L"Kills",
	L"Deaths",
	L"Queen's Army",
	L"Hits",
	L"Misses",
	L"Accuracy",
	L"Damage Dealt",
	L"Damage Taken",
	L"Please wait for the server to press 'Continue'."
};

STR16 gzMPCScreenText[] =
{
	L"Cancel",
	L"Connecting to Server",
	L"Getting Server Settings",
	L"Downloading custom files",
	L"Press 'ESC' to cancel or 'Y' to chat",
	L"Press 'ESC' to cancel",
	L"Ready"
};

STR16 gzMPChatToggleText[] =
{
	L"Send to All",
	L"Send to Allies only",
};

STR16 gzMPChatboxText[] =
{
	L"Multiplayer Chat",
	L"'ENTER' to send, 'ESC' to cancel",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"On the next page, you are going to choose your skill traits according to your proffessional specialization as a mercenary. No more than two different traits or one expert trait can be selected.",
	L"You can also choose only one or even no traits, which will give you a bonus to your attribute points as a compensation. Note that Electronics, Ambidextrous and Camouflage traits cannot be achieved at expert levels.",
	// For new major/minor traits
	L"Next stage is about choosing your skill traits according to your professional specialization as a mercenary. On first page you can select up to %d potential major traits, which mostly represent your main role in a team. While on second page is a list of possible minor traits, which represent personal feats.",
	L"No more then %d choices altogether are possible. This means that if you choose no major traits, you can choose %d minor traits. If you choose two major traits (or one enhanced), you can then choose only %d minor trait(s)...",
};
STR16 sgAttributeSelectionText[] = 
{
	L"Please adjust your physical attributes according to your true abilities. You cannot raise any score above",
	L"I.M.P. Attributes and skills review.",
	L"Bonus Pts.:",
	L"Starting Level",
	// New strings for new traits
	L"On the next page you are going to specify your physical attributes and skills. As 'attributes' are called: health, dexterity, agility, strength and wisdom. Attributes cannot go lower than %d.",
	L"The rest are called 'skills' and unlike attributes skills can be set to zero, meaning you have absolutely no proficiency in it.",
	L"All scores are set to a minimum at the beginning. Note that certain attributes are set to specific values according to skill traits you have selected. You cannot set those attributes lower than that.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"I.M.P. Character Analysis",
	L"The analysis of your character is the next step on your profile creation. On the first page you will be shown a list of attitudes to choose. We imagine you could identify yourself with more of them, but here you will be able to pick only one. Choose the one which you feel aligned with mostly. ",
	L"The second page enlists possible disabilities you might have. If you suffer from any of these disabilities, choose which one (we believe that everyone has only one such disablement). Be honest, as it is important to inform potential employers of your true condition.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Normal",
	L"Friendly",
	L"Loner",
	L"Optimist",
	L"Pessimist",
	L"Aggressive",
	L"Arrogant",
	L"Big Shot",
	L"Asshole",
	L"Coward",
	L"I.M.P. Attitudes",
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Normal",
	L"Sociable",
	L"Loner",
	L"Optimist",
	L"Assertive",
	L"Intellectual",
	L"Primitive",
	L"Aggressive",
	L"Phlegmatic",
	L"Dauntless",
	L"Pacifist",
	L"Malicious",
	L"Show-off",
	L"Coward",	// TODO.Translate
	L"I.M.P. Character Traits",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"I.M.P. Colors and Body Type",
	L"I.M.P. Colors",
	L"Please select the respective colors of your skin, hair and clothing. And select what body type you have.",
	L"Please select the respective colors of your skin, hair and clothing.",
	L"Toggle this to use alternative rifle holding.",
	L"\n(Caution: you will need a big strength for this.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Hair Color",
	L"Skin Color",
	L"Shirt Color",
	L"Pants Color",
	L"Normal Body",
	L"Big Body",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"No Disability",
	L"Heat Intolerant",
	L"Nervous",
	L"Claustrophobic",
	L"Nonswimmer",
	L"Fear of Insects",
	L"Forgetful",
	L"Psychotic",
	L"Deaf",
	L"Shortsighted",
	L"Hemophiliac",		// TODO.Translate
	L"Fear of Heights",
	L"Self-Harming",
	L"I.M.P. Disabilities",
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

STR16 gzIMPDisabilityTraitEmailTextHemophiliac[] =	// TODO.Translate
{
	L"Are you SURE this is the right job for you?",
	L"As long as you are so good to never ever get hit, or fight only in fully staffed hospitals, you should be fine.",
};

STR16 gzIMPDisabilityTraitEmailTextAfraidOfHeights[]=	// TODO.Translate
{
	L"Let's just say you are a grounded person.",
	L"You prefer missions where you don't have to scale tall buildings or mountains. We recommend conquering the Netherlands.",
};

STR16 gzIMPDisabilityTraitEmailTextSelfHarm[] =
{
	L"Might want to make sure your knives are always clean.",
	L"You have some issues with knives. Not that you tend to avoid them, quite the opposite, really.",
};

// TODO.Translate
// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s lacks sufficient Strength to perform this task.",
	L"%s lacks sufficient Dexterity to perform this task.",
	L"%s lacks sufficient Agility to perform this task.",
	L"%s is not Healthy enough to perform this task.",
	L"%s lacks sufficient Wisdom to perform this task.",
	L"%s lacks sufficient Marksmanship to perform this task.",
	// 6 - 10
	L"%s lacks sufficient Medical Skill to perform this task.",
	L"%s lacks sufficient Mechanical Skill to perform this task.",
	L"%s lacks sufficient Leadership to perform this task.",
	L"%s lacks sufficient Explosives Skill to perform this task.",
	L"%s lacks sufficient Experience to perform this task.",
	// 11 - 15
	L"%s lacks sufficient Morale to perform this task.",
	L"%s is too exhausted to perform this task.",
	L"Insufficient loyalty in %s. The locals refuse to allow you to perform this task.",
	L"Too many people are already working at the %s.",
	L"Too many people are already performing this task at the %s.",
	// 16 - 20
	L"%s can find no items to repair.",
	L"%s has lost some %s while working in sector %s!",
	L"%s has lost some %s while working at the %s in %s!",
	L"%s was injured while working in sector %s, and requires immediate medical attention!",
	L"%s was injured while working at the %s in %s, and requires immediate medical attention!",
	// 21 - 25
	L"%s was injured while working in sector %s. It doesn't seem too bad though.",
	L"%s was injured while working at the %s in %s. It doesn't seem too bad though.",
	L"The residents of %s seem upset about %s's presence.",
	L"The residents of %s seem upset about %s's work at the %s.",
	L"%s's actions in sector %s have caused loyalty loss throughout the region!",
	// 26 - 30
	L"%s's actions at the %s in %s have caused loyalty loss throughout the region!",
	L"%s is drunk.", // <--- This is a log message string.
	L"%s has become severely ill in sector %s, and has been taken off duty.",
	L"%s has become severely ill and cannot continue his work at the %s in %s.",
	L"%s was injured in sector %s.", // <--- This is a log message string.
	// 31 - 35
	L"%s was severely injured in sector %s.", //<--- This is a log message string.
	L"There are currently prisoners here who are aware of %s's identity.", // TODO.Translate
	L"%s is currently well known as a mercenary snitch. Wait at least %d more hours.", // TODO.Translate


};

// TODO.Translate
STR16 gzFacilityRiskResultStrings[]=
{
	L"Strength",
	L"Agility",
	L"Dexterity",
	L"Wisdom",
	L"Health",
	L"Marksmanship",
	// 5-10
	L"Leadership",
	L"Mechanical skill",
	L"Medical skill",
	L"Explosives skill",
};

// TODO.Translate
STR16 gzFacilityAssignmentStrings[]=
{
	L"AMBIENT",
	L"Staff",
	L"Eat",// TODO.Translate
	L"Rest",
	L"Repair Items",
	L"Repair %s", // Vehicle name inserted here
	L"Repair Robot",
	// 6-10
	L"Doctor",
	L"Patient",
	L"Practice Strength",
	L"Practice Dexterity",
	L"Practice Agility",
	L"Practice Health",
	// 11-15
	L"Practice Marksmanship",
	L"Practice Medical",
	L"Practice Mechanical",
	L"Practice Leadership",
	L"Practice Explosives",
	// 16-20
	L"Student Strength",
	L"Student Dexterity",
	L"Student Agility",
	L"Student Health",
	L"Student Marksmanship",
	// 21-25
	L"Student Medical",
	L"Student Mechanical",
	L"Student Leadership",
	L"Student Explosives",
	L"Trainer Strength",
	// 26-30
	L"Trainer Dexterity",
	L"Trainer Agility",
	L"Trainer Health",
	L"Trainer Marksmanship",
	L"Trainer Medical",
	// 30-35
	L"Trainer Mechanical",
	L"Trainer Leadership",
	L"Trainer Explosives",
	L"Interrogate Prisoners",	// added by Flugente	TODO.Translate
	L"Undercover Snitch",	// TODO.Translate
	// 36-40
	L"Spread Propaganda",
	L"Spread Propaganda",	// spread propaganda (globally)
	L"Gather Rumours",
	L"Command Militia",	// militia movement orders
};

STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 modalità finestra richiede una profondità di colore di 16bpp.",
	L"Jagged Alliance 2 v1.13 fullscreen mode (%d x %d) is not supported by your primary screen.\nPlease either change the game resolution or use 16bpp windowed mode.",	// TODO.Translate
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	// TODO.Translate
	// WANNE: Savegame slots validation against INI file
	L"Mercenary (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Enemy (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Creature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Militia (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civilian (MAX_NUMBER_CIVS_IN_TACTICAL)",
};

// SANDRO - Taunts (here for now, xml for future, I hope)
STR16 sEnemyTauntsFireGun[]=
{
	L"Suck this!",
	L"Touch this!",
	L"Come get some!",
	L"You're mine!",
	L"Die!",
	L"You scared, motherfucker?",
	L"This will hurt!",
	L"Come on you bastard!",
	L"Come on! I don't got all day!",
	L"Come to daddy!",
	L"You'll be six feet under in no time!",
	L"Will send ya home in a pinebox, loser!",
	L"Hey, wanna play?",
	L"You should have stayed home, bitch.",
	L"Sucker!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"We have a barbecue here.",
	L"I got a present for ya.",
	L"Bam!",
	L"Smile!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Catch!",
	L"Here ya go!",
	L"Pop goes the weasel.",
	L"This one's for you.",
	L"Muhehe.",
	L"Catch this, swine!",
	L"I like this.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"I'll get your scalp.",
	L"Come to papa.",
	L"Show me your guts!",
	L"I'll rip you to pieces!",
	L"Motherfucker!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"We're in some real shit...",
	L"They said join the army. Not for this shit!",
	L"I have enough.",
	L"Oh my God.",
	L"They ain't paying us enough for this.",
	L"It's just too much for me.",
	L"I'll bring some friends.",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"I heard that!",
	L"Who's there?",
	L"What was that?",
	L"Hey! What the...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"They are here!",
	L"Now the fun can start.",
	L"I hoped this will never happen.",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Ouch!",
	L"Ugh!",
	L"This.. hurts!",
	L"You fuck!",
	L"You will regret.. uhh.. this.",
	L"What the..!",
	L"Now you have.. pissed me off.",

};

// TODO.Translate
STR16 sEnemyTauntsNoticedMerc[]=
{
	L"Da'ffff...!",
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
	L"Description",
	L"General",
	L"Advanced",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Desc",
	L"Gen",
	L"Adv",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Primary",
	L"Secondary",
	L"AP Costs",
	L"Burst / Autofire",
};

STR16 gzItemDescGenIndexes[] =
{
	L"Prop.",
	L"0",
	L"+/-",
	L"=",
};

STR16 gzUDBButtonTooltipText[]=
{
	L"|D|e|s|c|r|i|p|t|i|o|n |P|a|g|e:\n \nShows basic textual information about this item.",
	L"|G|e|n|e|r|a|l |P|r|o|p|e|r|t|i|e|s |P|a|g|e:\n \nShows specific data about this item.\n \nWeapons: Click again to see second page.",
	L"|A|d|v|a|n|c|e|d| |P|r|o|p|e|r|t|i|e|s |P|a|g|e:\n \nShows bonuses given by this item.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|P|r|i|m|a|r|y |P|r|o|p|e|r|t|i|e|s:\n \nProperties and data related to this item's class\n(Weapon / Armor / etcetera).",
	L"|S|e|c|o|n|d|a|r|y |P|r|o|p|e|r|t|i|e|s:\n \nAdditional features of this item,\nand/or possible secondary abilities.",
	L"|A|P |C|o|s|t|s:\n \nVarious Action Point costs to fire\nor manipulate this weapon.",
	L"|B|u|r|s|t |/ |A|u|t|o|f|i|r|e |P|r|o|p|e|r|t|i|e|s:\n \nData related to firing this weapon in\nBurst or Autofire modes.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|P|r|o|p|e|r|t|y |i|c|o|n\n \nMouse-over to reveal the property's name.",
	L"|B|a|s|i|c |v|a|l|u|e\n \nThe basic value given by this item, excluding any\nbonuses or penalties from attachments or ammo.",
	L"|A|t|t|a|c|h|m|e|n|t |B|o|n|u|s|e|s\n \nBonus or penalty given by ammo, any attachments,\nor low item condition.",
	L"|F|i|n|a|l |V|a|l|u|e\n \nThe final value given by this item, including any\nbonuses/penalties from attachments or ammo.",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"Property icon (mouse-over to reveal name).",
	L"Bonus/penalty given while |s|t|a|n|d|i|n|g.",
	L"Bonus/penalty given while |c|r|o|u|c|h|i|n|g.",
	L"Bonus/penalty given while |p|r|o|n|e.",
	L"Bonus/penalty given",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|A|c|c|u|r|a|c|y",
	L"|D|a|m|a|g|e",
	L"|R|a|n|g|e",
	L"|H|a|n|d|l|i|n|g |D|i|f|f|i|c|u|l|t|y",	// TODO.Translate
	L"|A|l|l|o|w|e|d |A|i|m|i|n|g |L|e|v|e|l|s",
	L"|S|c|o|p|e |M|a|g|n|i|f|i|c|a|t|i|o|n |F|a|c|t|o|r",
	L"|P|r|o|j|e|c|t|i|o|n |F|a|c|t|o|r",
	L"|H|i|d|d|e|n |M|u|z|z|l|e |F|l|a|s|h",
	L"|L|o|u|d|n|e|s|s",
	L"|R|e|l|i|a|b|i|l|i|t|y",
	L"|R|e|p|a|i|r |E|a|s|e",
	L"|M|i|n|. |R|a|n|g|e |f|o|r |A|i|m|i|n|g |B|o|n|u|s",
	L"|T|o|-|H|i|t |M|o|d|i|f|i|e|r",	
	L"|A|P|s |t|o |R|e|a|d|y",
	L"|A|P|s |t|o |A|t|t|a|c|k",
	L"|A|P|s |t|o |B|u|r|s|t",
	L"|A|P|s |t|o |A|u|t|o|f|i|r|e",
	L"|A|P|s |t|o |R|e|l|o|a|d",
	L"|A|P|s |t|o |R|e|c|h|a|m|b|e|r",
	L"",	// No longer used!
	L"|T|o|t|a|l |R|e|c|o|i|l",		// TODO.Translate
	L"|A|u|t|o|f|i|r|e |B|u|l|l|e|t|s |p|e|r |5 |A|P|s",
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
	L"",	// No longer used!
	L"\n \nThe total distance this weapon's muzzle will shift\nbetween each and every bullet in a burst or\nautofire volley, if no Counter Force is applied.\n \nLower is better.", // HEADROCK HAM 5: Altered to reflect unified number.	// TODO.Translate
	L"\n \nIndicates the number of bullets that will be added\nto an autofire volley for every extra 5 APs\nyou spend.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|P|r|o|t|e|c|t|i|o|n |V|a|l|u|e",
	L"|C|o|v|e|r|a|g|e",
	L"|D|e|g|r|a|d|e |R|a|t|e",
	L"|R|e|p|a|i|r |E|a|s|e",
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
	L"|A|r|m|o|r |P|i|e|r|c|i|n|g",
	L"|B|u|l|l|e|t |T|u|m|b|l|e",
	L"|P|r|e|-|I|m|p|a|c|t |E|x|p|l|o|s|i|o|n",
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
	L"|D|a|m|a|g|e",
	L"|S|t|u|n |D|a|m|a|g|e",
	L"|E|x|p|l|o|d|e|s |o|n| |I|m|p|a|c|t",		// HEADROCK HAM 5	// TODO.Translate
	L"|B|l|a|s|t |R|a|d|i|u|s",
	L"|S|t|u|n |B|l|a|s|t |R|a|d|i|u|s",
	L"|N|o|i|s|e |B|l|a|s|t |R|a|d|i|u|s",
	L"|T|e|a|r|g|a|s |S|t|a|r|t |R|a|d|i|u|s",
	L"|M|u|s|t|a|r|d |G|a|s |S|t|a|r|t |R|a|d|i|u|s",
	L"|L|i|g|h|t |S|t|a|r|t |R|a|d|i|u|s",
	L"|S|m|o|k|e |S|t|a|r|t |R|a|d|i|u|s",
	L"|I|n|c|e|n|d|i|a|r|y |S|t|a|r|t |R|a|d|i|u|s",
	L"|T|e|a|r|g|a|s |E|n|d |R|a|d|i|u|s",
	L"|M|u|s|t|a|r|d |G|a|s |E|n|d |R|a|d|i|u|s",
	L"|L|i|g|h|t |E|n|d |R|a|d|i|u|s",
	L"|S|m|o|k|e |E|n|d |R|a|d|i|u|s",
	L"|I|n|c|e|n|d|i|a|r|y |E|n|d |R|a|d|i|u|s",
	L"|E|f|f|e|c|t |D|u|r|a|t|i|o|n",
	// HEADROCK HAM 5: Fragmentation		// TODO.Translate
	L"|N|u|m|b|e|r |o|f |F|r|a|g|m|e|n|t|s",
	L"|D|a|m|a|g|e |p|e|r |F|r|a|g|m|e|n|t",
	L"|F|r|a|g|m|e|n|t |R|a|n|g|e",
	// HEADROCK HAM 5: End Fragmentations
	L"|L|o|u|d|n|e|s|s",
	L"|V|o|l|a|t|i|l|i|t|y",
	L"|R|e|p|a|i|r |E|a|s|e",
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
	L"|R|e|p|a|i|r |E|a|s|e",
	L"|A|v|a|i|l|a|b|l|e |V|o|l|u|m|e",
	L"|V|o|l|u|m|e",
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines how difficult it is to repair this item.\n \ngreen = Anybody can repair it.\n \nred = This item can't be repaired.\n \nHigher is better.",
	L"\n \nDetermines how much space is available on this MOLLE carrier.\n \nHigher is better.",
	L"\n \nDetermines how much space this MOLLE pocket will occupy.\n \nLower is better.",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|T|r|a|c|e|r |A|m|m|o",
	L"|A|n|t|i|-|T|a|n|k |A|m|m|o",
	L"|I|g|n|o|r|e|s |A|r|m|o|r",
	L"|A|c|i|d|i|c |A|m|m|o",
	L"|L|o|c|k|-|B|u|s|t|i|n|g |A|m|m|o",
	L"|R|e|s|i|s|t|a|n|t |t|o |E|x|p|l|o|s|i|v|e|s",
	L"|W|a|t|e|r|p|r|o|o|f",
	L"|E|l|e|c|t|r|o|n|i|c",
	L"|G|a|s |M|a|s|k",
	L"|N|e|e|d|s |B|a|t|t|e|r|i|e|s",
	L"|C|a|n |P|i|c|k |L|o|c|k|s",
	L"|C|a|n |C|u|t |W|i|r|e|s",
	L"|C|a|n |S|m|a|s|h |L|o|c|k|s",
	L"|M|e|t|a|l |D|e|t|e|c|t|o|r",
	L"|R|e|m|o|t|e |T|r|i|g|g|e|r",
	L"|R|e|m|o|t|e |D|e|t|o|n|a|t|o|r",
	L"|T|i|m|e|r |D|e|t|o|n|a|t|o|r",
	L"|C|o|n|t|a|i|n|s |G|a|s|o|l|i|n|e",
	L"|T|o|o|l |K|i|t",
	L"|T|h|e|r|m|a|l |O|p|t|i|c|s",
	L"|X|-|R|a|y |D|e|v|i|c|e",
	L"|C|o|n|t|a|i|n|s |D|r|i|n|k|i|n|g |W|a|t|e|r",
	L"|C|o|n|t|a|i|n|s |A|l|c|o|h|o|l",
	L"|F|i|r|s|t |A|i|d |K|i|t",
	L"|M|e|d|i|c|a|l |K|i|t",
	L"|L|o|c|k |B|o|m|b",
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
	L"|A|n|t|i|-|M|a|t|e|r|i|a|l |A|m|m|o",	// TODO.Translate
	L"|F|a|c|e |P|r|o|t|e|c|t|i|o|n",
	L"|I|n|f|e|c|t|i|o|n |P|r|o|t|e|c|t|i|o|n",	// 39
	L"|S|h|i|e|l|d",	// TODO.Translate
	L"|C|a|m|e|r|a",		// TODO.Translate
	L"|B|u|r|i|a|l |M|o|d|i|f|i|e|r",
	L"|E|m|p|t|y |B|l|o|o|d |B|a|g",	// TODO.Translate
	L"|B|l|o|o|d |B|a|g",						// 44
	L"|R|e|s|i|s|t|a|n|t |t|o |F|i|r|e",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |M|o|d|i|f|i|e|r",
	L"|H|a|c|k|i|n|g |M|o|d|i|f|i|e|r",
	L"|M|e|d|i|c|a|l |S|p|l|i|n|t", // TODO.Translate
	L"|F|i|r|e |R|e|t|a|r|d|a|n|t |A|m|m|o",				// 49	TODO.Translate
	L"|I|n|c|e|n|d|i|a|r|y |A|m|m|o",
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
	L"\n \nThis ammo can destroy light walls\nand various other objects.",	// TODO.Translate
	L"\n \nIf worn on your face, this will lower\nthe chance to be infected by other people.",
	L"\n \nIf kept in your inventory, this will\nlower\nthe chance to be infected by other people.",
	L"\n \nIf equipped in a hand, this will block incoming damage.",	// TODO.Translate
	L"\n \nYou can take photos with this.",		// TODO.Translate
	L"\n \nThis item makes you more effective at burying corpses.",
	L"\n \nA paramedic can extract blood\nfrom a donor with this.",	// TODO.Translate
	L"\n \nA paramedic can use up this item to increase\nthe amount of health regenerated by surgery.",	// 44
	L"\n \nThis armor lowers fire damage by %i%%.",
	L"\n \nThis item makes you more effective at\nadministrative work by %i%%.",
	L"\n \nThis item improves your hacking skills by %i%%.",
	L"\n \nOnce applied, this item increases the healing\nspeed of severe wounds to either your arms or legs.", // TODO.Translate
	L"\n \nThis ammo can extinguish fire.",	// 49	 TODO.Translate
	L"\n \nThis ammo can cause fire.",
};

STR16 szUDBAdvStatsTooltipText[]=
{
	L"|A|c|c|u|r|a|c|y |M|o|d|i|f|i|e|r",
	L"|F|l|a|t |S|n|a|p|s|h|o|t |M|o|d|i|f|i|e|r",
	L"|P|e|r|c|e|n|t |S|n|a|p|s|h|o|t |M|o|d|i|f|i|e|r",
	L"|F|l|a|t |A|i|m|i|n|g |M|o|d|i|f|i|e|r",
	L"|P|e|r|c|e|n|t |A|i|m|i|n|g |M|o|d|i|f|i|e|r",
	L"|A|l|l|o|w|e|d |A|i|m|i|n|g |L|e|v|e|l|s |M|o|d|i|f|i|e|r",
	L"|A|i|m|i|n|g |C|a|p |M|o|d|i|f|i|e|r",
	L"|G|u|n |H|a|n|d|l|i|n|g |M|o|d|i|f|i|e|r",
	L"|D|r|o|p |C|o|m|p|e|n|s|a|t|i|o|n |M|o|d|i|f|i|e|r",
	L"|T|a|r|g|e|t |T|r|a|c|k|i|n|g |M|o|d|i|f|i|e|r",
	L"|D|a|m|a|g|e |M|o|d|i|f|i|e|r",
	L"|M|e|l|e|e |D|a|m|a|g|e |M|o|d|i|f|i|e|r",
	L"|R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|S|c|o|p|e |M|a|g|n|i|f|i|c|a|t|i|o|n |F|a|c|t|o|r",
	L"|P|r|o|j|e|c|t|i|o|n |F|a|c|t|o|r",
	L"|L|a|t|e|r|a|l |R|e|c|o|i|l |M|o|d|i|f|i|e|r",
	L"|V|e|r|t|i|c|a|l |R|e|c|o|i|l |M|o|d|i|f|i|e|r",
	L"|M|a|x|i|m|u|m |C|o|u|n|t|e|r|-|F|o|r|c|e |M|o|d|i|f|i|e|r",
	L"|C|o|u|n|t|e|r|-|F|o|r|c|e |A|c|c|u|r|a|c|y |M|o|d|i|f|i|e|r",
	L"|C|o|u|n|t|e|r|-|F|o|r|c|e |F|r|e|q|u|e|n|c|y |M|o|d|i|f|i|e|r",
	L"|T|o|t|a|l |A|P |M|o|d|i|f|i|e|r",
	L"|A|P|-|t|o|-|R|e|a|d|y |M|o|d|i|f|i|e|r",
	L"|S|i|n|g|l|e|-|a|t|t|a|c|k |A|P |M|o|d|i|f|i|e|r",
	L"|B|u|r|s|t |A|P |M|o|d|i|f|i|e|r",
	L"|A|u|t|o|f|i|r|e |A|P |M|o|d|i|f|i|e|r",
	L"|R|e|l|o|a|d |A|P |M|o|d|i|f|i|e|r",
	L"|M|a|g|a|z|i|n|e |S|i|z|e |M|o|d|i|f|i|e|r",
	L"|B|u|r|s|t |S|i|z|e |M|o|d|i|f|i|e|r",
	L"|H|i|d|d|e|n |M|u|z|z|l|e |F|l|a|s|h",
	L"|L|o|u|d|n|e|s|s |M|o|d|i|f|i|e|r",
	L"|I|t|e|m |S|i|z|e |M|o|d|i|f|i|e|r",
	L"|R|e|l|i|a|b|i|l|i|t|y |M|o|d|i|f|i|e|r",
	L"|W|o|o|d|l|a|n|d |C|a|m|o|u|f|l|a|g|e",
	L"|U|r|b|a|n |C|a|m|o|u|f|l|a|g|e",
	L"|D|e|s|e|r|t |C|a|m|o|u|f|l|a|g|e",
	L"|S|n|o|w |C|a|m|o|u|f|l|a|g|e",
	L"|S|t|e|a|l|t|h |M|o|d|i|f|i|e|r",
	L"|H|e|a|r|i|n|g |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|G|e|n|e|r|a|l |V|i|s|i|o|n |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|N|i|g|h|t|-|t|i|m|e |V|i|s|i|o|n |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|D|a|y|-|t|i|m|e |V|i|s|i|o|n |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|B|r|i|g|h|t|-|L|i|g|h|t |V|i|s|i|o|n |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|C|a|v|e |V|i|s|i|o|n |R|a|n|g|e |M|o|d|i|f|i|e|r",
	L"|T|u|n|n|e|l |V|i|s|i|o|n",
	L"|M|a|x|i|m|u|m |C|o|u|n|t|e|r|-|F|o|r|c|e",
	L"|C|o|u|n|t|e|r|-|F|o|r|c|e |F|r|e|q|u|e|n|c|y",
	L"|T|o|-|H|i|t |B|o|n|u|s",
	L"|A|i|m |B|o|n|u|s",
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
	L"|P|o|i|s|o|n |M|o|d|i|f|i|e|r",// TODO.Translate
	L"|F|o|o|d| |P|o|i|n|t|s",// TODO.Translate
	L"|D|r|i|n|k |P|o|i|n|t|s",// TODO.Translate
	L"|P|o|r|t|i|o|n |S|i|z|e",// TODO.Translate
	L"|M|o|r|a|l|e |M|o|d|i|f|i|e|r",// TODO.Translate
	L"|D|e|c|a|y |M|o|d|i|f|i|e|r",// TODO.Translate
	L"|B|e|s|t |L|a|s|e|r |R|a|n|g|e",// TODO.Translate
	L"|P|e|r|c|e|n|t |R|e|c|o|i|l |M|o|d|i|f|i|e|r",	// 65
	L"|F|a|n |t|h|e |H|a|m|m|e|r",	// TODO.Translate
	L"|B|a|r|r|e|l |C|o|n|f|i|g|u|r|a|t|i|o|n|s",
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
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Horizontal Recoil\nby the listed value.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Vertical Recoil\nby the listed value.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
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
	L"\n \nThis is the percentage of damage dealt\nby this item that will be poisonous.\n\nUsefulness depends on whether enemy\nhas poison resistance or absorption.",	// TODO.Translate
	L"\n \nA single shot causes this much dirt.\nAmmunition types and attachments can\naffect this value.\n \nLower is better.",	// TODO.Translate
	L"\n \nWhen this item is eaten\nit causes that much poison.\n \nLower is better.",	// TODO.Translate
	L"\n \nAmount of energy in kcal.\n \nHigher is better.",	// TODO.Translate
	L"\n \nAmount of water in liter.\n \nHigher is better.",	// TODO.Translate
	L"\n \nThe percentage of the item\nthat will be eaten at once.\n \nLower is better.",	// TODO.Translate
	L"\n \nMorale is adjusted by this amount.\n \nHigher is better.",	// TODO.Translate
	L"\n \nThis item becomes stale over time.\nIf more then 50% is molded it becomes poisoneous.\nThis is the rate at which mold is generated.\nLower is better.",	// TODO.Translate
	L"",
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Recoil by the listed percentage.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.", // 65
	L"\n \nIf a gunslinger wields this gun two-handed,\nthey can burst in hipfire.",	// TODO.Translate
	L"\n \nToggling firemodes also toggles how many\nbarrels you can fire at the same time.",
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
	L"\n \nThis weapon's recoil strength is being\nmodified by this percentage value by its ammo,\nattachments, or inherent abilities.\n \nThis has no effect if the weapon lacks both\nBurst and Auto-Fire modes.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"SINGLE",
	L"AP",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

// TODO.Translate
// HEADROCK HAM 5: Screen messages for sector inventory sorting reports.
STR16 gzMapInventorySortingMessage[] =
{
	L"Finished sorting ammo into crates in sector %c%d.",
	L"Finished removing attachments from items in sector %c%d.",
	L"Finished ejecting ammo from weapons in sector %c%d.",
	L"Finished stacking and merging all items in sector %c%d.",
	// Bob: new strings for emptying LBE items
	L"Finished emptying LBE items in sector %c%d.",
	L"Dropped %i item(s) from %s",	// Bunch of stuff removed from LBE item %s
	L"No droppable items in %s!",	// LBE item %s had a LBENode assigned but it contained no items (error!)
	L"%s is now empty.",			// LBE item %s contained stuff and was emptied
	L"Cannot empty %s!",			// Removed everything we could from LBE item %s but it's still not marked as empty (error!)
	L"%s contents lost!",			// LBE item %s not marked as empty but LBENode not found (error!!!)
};

STR16 gzMapInventoryFilterOptions[] =
{
	L"Show all",
	L"Guns",
	L"Ammo",
	L"Explosives",
	L"Melee Weapons",
	L"Armor",
	L"LBE",
	L"Kits",
	L"Misc. Items",
	L"Hide all",
};

STR16 gzMercCompare[] =
{
	L"???",
	L"Base opinion:",

	L"Dislikes %s %s",
	L"Likes %s %s",

	L"Strongly hates %s",
	L"Hates %s",			// 5

	L"Deep racism against %s",
	L"Racism against %s",

	L"Cares deeply about looks",
	L"Cares about looks",

	L"Very sexist",			// 10
	L"Sexist",

	L"Dislikes other background",
	L"Dislikes other backgrounds",

	L"Past grievances",
	L"____",				// 15
	L"/",
	L"* Opinion is always in [%d; %d]",		// TODO.Translate
};

// Flugente: Temperature-based text similar to HAM 4's condition-based text.
STR16 gTemperatureDesc[] =			// TODO.Translate
{
	L"Temperature is ",
	L"very low",
	L"low",
	L"medium",
	L"high",
	L"very high",
	L"dangerous",
	L"CRITICAL",
	L"DRAMATIC",
	L"unknown",
	L"."
};

// TODO.Translate
// Flugente: food condition texts
STR16 gFoodDesc[] =
{
	L"Food is ",
	L"fresh",
	L"good",
	L"ok",
	L"stale",
	L"shabby",
	L"rotting",
	L"."
};

// TODO.Translate
CHAR16* ranks[] = 
{	L"",			//ExpLevel 0
	L"Pvt. ",		//ExpLevel 1
	L"Pfc. ",		//ExpLevel 2
	L"Cpl. ",		//ExpLevel 3
	L"Sgt. ",		//ExpLevel 4
	L"Lt. ",		//ExpLevel 5
	L"Cpt. ",		//ExpLevel 6
	L"Maj. ",		//ExpLevel 7
	L"Lt.Col. ",	//ExpLevel 8
	L"Col. ",		//ExpLevel 9
	L"Gen. "		//ExpLevel 10
};

STR16	gzNewLaptopMessages[]=
{
	L"Ask about our special offer!",
	L"Temporarily Unavailable",
	L"This special press preview of Jagged Alliance 2: Unfinished Business contains the only first 6 sector maps. The final version of the game will feature many more - please see the included readme file for details.",
};

STR16	zNewTacticalMessages[]=
{
	//L"Distanza dal bersaglio: %d caselle, Luminosità: %d/%d",
	L"Colleghi il trasmettitore al tuo computer portatile.",
	L"Non puoi permetterti di ingaggiare %s",
	L"Per un periodo limitato, la tariffa qui sopra includerà i costi dell'intera missione, oltre all'equipaggiamento indicato sotto.",
	L"Assolda %s adesso e approfitta della nostra nuova tariffa 'tutto incluso'. Compreso in questa incredibile offerta anche l'equipaggiamento personale del mercenario, senza alcun costo aggiuntivo.",
	L"Tariffa",
	L"C'è qualcun altro nel settore...",
	//L"Gittata dell'arma: %d caselle, Probabilità di colpire: %d percent",
	L"Mostra nascondigli",
	L"Linea di Vista",
	L"Le nuove reclute non possono arrivare qui.",
	L"Poiché il tuo portatile non ha un trasmettitore, non potrai assoldare nuovi mercenari. Forse questo sarebbe un buon momento per caricare una partita salvata o ricominciare daccapo!",
	L"%s sente venire da sotto al corpo di Jerry il rumore di metallo che si accartoccia. E' un suono fastidioso, come se l'antenna del tuo portatile venisse schiacciata.",  //the %s is the name of a merc.
	L"Dopo aver dato un'occhiata al biglietto lasciato dal Vice Comandante Morris, %s vede una possibilità. La nota contiene le coordinate per il lancio di missili contro diverse città ad Arulco. C'è anche la locazione della base di lancio: la fabbrica di missili.",
	L"Guardando il pannello di controllo %s immagina che i numeri possano essere invertiti, cosicché il missile distrugga proprio questa fabbrica. %s deve trovare una via di fuga. L'ascensore sembra offrire la soluzione più rapida...",
	L"Questa è una partita a livello IRON MAN: non puoi salvare quando ci sono nemici nei dintorni.",
	L"(Non puoi salvare durante il combattimento.)",
	L"Il nome della campagna è più grande di 30 caratteri",
	L"Campagna non trovata.",
	L"Campagna: Default ( %S )",
	L"Campagna: %S",
	L"Hai selezionato la campagna %S. Questa campagna è una versione amatoriale della campagna originale di Unfinished Business. Sei sicuro di voler giocare la campagna %S?",
	L"Per usare l'editor, selezionare una campagna diversa da quella di default.",
	// anv: extra iron man modes
	L"This is a SOFT IRON MAN game and you cannot save during turn-based combat.",	// TODO.Translate	
	L"This is an EXTREME IRON MAN game and you can only save once per day, at %02d:00.",	// TODO.Translate
};

// The_bob : pocket popup text defs	// TODO.Translate
STR16	gszPocketPopupText[]=
{
	L"Grenade launchers",	// POCKET_POPUP_GRENADE_LAUNCHERS,
	L"Rocket launchers",	// POCKET_POPUP_ROCKET_LAUNCHERS
	L"Melee & thrown weapons",	// POCKET_POPUP_MEELE_AND_THROWN
	L"- no matching ammo -",	//POCKET_POPUP_NO_AMMO
	L"- no guns in inventory -",	//POCKET_POPUP_NO_GUNS
	L"more...",		//POCKET_POPUP_MOAR
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
	L"%s's %s is too advanced for an %s soldier!",
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
	L"An elite soldier did not recognize %s!",	// TODO.Translate
	L"A officer knew %s was unfamiliar!",
};

STR16	szCorpseTextStr[]=
{
	L"No head item found in Items.xml!",
	L"Corpse cannot be decapitated!",
	L"No meat item found in Items.xml!",
	L"Not possible, you sick, twisted individual!",
	L"No clothes to take!",
	L"%s cannot take clothes off of this corpse!",
	L"This corpse cannot be taken!",
	L"No free hand to carry corpse!",
	L"No corpse item found in Items.xml!",
	L"Invalid corpse ID!",
};

STR16	szFoodTextStr[]=
{
	L"%s does not want to eat %s",
	L"%s does not want to drink %s",
	L"%s ate %s",
	L"%s drank %s",
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

// TODO.Translate
STR16	szPrisonerTextStr[]=
{
	L"%d officers, %d elites, %d regulars, %d admins, %d generals and %d civilians were interrogated.",	// TODO.Translate
	L"Gained $%d as ransom money.",		// TODO.Translate
	L"%d prisoners revealed enemy positions.",
	L"%d officers, %d elites, %d regulars and %d admins joined our cause.",
	L"Prisoners start a massive riot in %s!",
	L"%d prisoners were sent to %s!",
	L"Prisoners have been released!",
	L"The army now occupies the prison in %s, the prisoners were freed!",
	L"The enemy refuses to surrender!",
	L"The enemy refuses to take you as prisoners - they prefer you dead!",
	L"This behaviour is set OFF in your ini settings.",
	L"%s has freed %s!",
	L"A high-ranking army officer in %s has been revealed!",	// TODO.Translate
	L"The enemy leader refuses to even consider surrender!",
	L"%d prisoners volunteered to join our forces.",
};

STR16	szMTATextStr[]=	// TODO.Translate
{
	L"nothing",
	L"building a fortification",
	L"removing a fortification",
	L"hacking",	// TODO.Translate
	L"%s had to stop %s.",
	L"The selected barricade cannot be built in this sector",	// TODO.Translate
};

// TODO.Translate
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
	L" disregard for all other backgrounds\n",
	L" +1 level in underground sectors\n",
	L" steals money from the locals sometimes\n", // TODO.Translate

	L" +1 traplevel to planted bombs\n",
	L" spreads corruption to nearby mercs\n",
	L" female only",							// won't show up, text exists for compatibility reasons
	L" male only",								// won't show up, text exists for compatibility reasons

	L" huge loyality penalty in all towns if we die\n",		// TODO.Translate

	L" refuses to attack animals\n",	// TODO.Translate
	L" refuses to attack members of the same group\n",	// TODO.Translate
};

// TODO.Translate
STR16	szBackgroundText_Value[]=
{
	L" %s%d%% APs in polar sectors\n",
	L" %s%d%% APs in desert sectors\n",	
	L" %s%d%% APs in swamp sectors\n",
	L" %s%d%% APs in urban sectors\n",
	L" %s%d%% APs in forest sectors\n",
	L" %s%d%% APs in plain sectors\n",
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

	L" %s%d%% fear resistance\n",
	L" %s%d%% suppression resistance\n",
	L" %s%d%% physical resistance\n",
	L" %s%d%% alcohol resistance\n",
	L" %s%d%% disease resistance\n",	// TODO.Translate

	L" %s%d%% interrogation effectiveness\n",
	L" %s%d%% prison guard strength\n",
	L" %s%d%% better prices when trading guns and ammo\n",
	L" %s%d%% better prices when trading armour, lbe, blades, kits etc.\n",
	L" %s%d%% team capitulation strength if we lead negotiations\n",
	L" %s%d%% faster running\n",
	L" %s%d%% bandaging speed\n",
	L" %s%d%% breath regeneration\n",	// TODO.Translate
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
	L" %s%d effectivity at disarming traps\n",	// TODO.Translate
	L" %s%d%% CTH with SAMs\n",	// TODO.Translate

	L" %s%d%% effectiveness to friendly approach\n",
	L" %s%d%% effectiveness to direct approach\n",
	L" %s%d%% effectiveness to threaten approach\n",
	L" %s%d%% effectiveness to recruit approach\n",

	L" %s%d%% chance of success with door breaching charges\n",
	L" %s%d%% cth with firearms against creatures\n",
	L" %s%d%% insurance cost\n",
	L" %s%d%% effectiveness as spotter for fellow snipers\n",	// TODO.Translate
	L" %s%d%% effectiveness at diagnosing diseases\n",	// TODO.Translate
	L" %s%d%% effectiveness at treating population against diseases\n",
	L"Can spot tracks up to %d tiles away\n",
	L" %s%d%% initial distance to enemy in ambush\n",
	L" %s%d%% chance to evade snake attacks\n",	// TODO.Translate

	L" dislikes some other backgrounds\n",	// TODO.Translate
	L"Smoker",
	L"Nonsmoker",
	L" %s%d%% enemy CTH if crouched against thick cover in their direction\n",
	L" %s%d%% building speed\n",
	L" hacking skill: %s%d ",	// TODO.Translate
	L" %s%d%% burial speed\n",	// TODO.Translate
	L" %s%d%% administration effectiveness\n",	// TODO.Translate
	L" %s%d%% exploration effectiveness\n",	// TODO.Translate
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

STR16		szRefinementTextTypes[] =	// TODO.Translate
{
	L"normal people",
	L"slobs",
	L"snobs",
};

STR16		szNationalityText[]=
{
	L"American",		// 0
	L"Arab",
	L"Australian",
	L"British",
	L"Canadian",
	L"Cuban",			// 5
	L"Danish",
	L"French",
	L"Russian",
	L"Nigerian",
	L"Swiss",			// 10
	L"Jamaican",
	L"Polish",
	L"Chinese",
	L"Irish",
	L"South African",	// 15
	L"Hungarian",
	L"Scottish",
	L"Arulcan",
	L"German",
	L"African",			// 20
	L"Italian",
	L"Dutch",
	L"Romanian",
	L"Metaviran",

	// newly added from here on
	L"Greek",			// 25
	L"Estonian",
	L"Venezuelan",
	L"Japanese",
	L"Turkish",
	L"Indian",			// 30
	L"Mexican",
	L"Norwegian",
	L"Spanish",
	L"Brasilian",
	L"Finnish",			// 35
	L"Iranian",
	L"Israeli",
	L"Bulgarian",
	L"Swedish",
	L"Iraqi",			// 40
	L"Syrian",
	L"Belgian",
	L"Portoguese",
	L"Belarusian",		// TODO.Translate
	L"Serbian",			// 45
	L"Pakistani",
	L"Albanian",
	L"Argentinian",
	L"Armenian",
	L"Azerbaijani", // 50
	L"Bolivian",
	L"Chilean",
	L"Circassian",
	L"Columbian",
	L"Egyptian", // 55
	L"Ethiopian",
	L"Georgian",
	L"Jordanian",
	L"Kazakhstani",
	L"Kenyan", // 60
	L"Korean",
	L"Kyrgyzstani",
	L"Mongolian",
	L"Palestinian",
	L"Panamanian", // 65
	L"Rhodesian",
	L"Salvadoran",
	L"Saudi",
	L"Somali",
	L"Thai", // 70
	L"Ukrainian",
	L"Uzbekistani",
	L"Welsh",
	L"Yazidi",
	L"Zimbabwean", // 75
};

STR16		szNationalityTextAdjective[] = // TODO.Translate
{
	L"americans",		// 0
	L"arabs",
	L"australians",
	L"britains",
	L"canadians",
	L"cubans",			// 5
	L"danes",
	L"frenchmen",
	L"russians",
	L"nigerians",
	L"swiss",			// 10
	L"jamaicans",
	L"poles",
	L"chinese",
	L"irishmen",
	L"south africans",	// 15
	L"hungarians",
	L"scotsmen",
	L"arulcans",
	L"germans",
	L"africans",			// 20
	L"italians",
	L"dutchmen",
	L"romanians",
	L"metavirans",

	// newly added from here on
	L"greek",			// 25
	L"estonians",
	L"venezuelans",
	L"japanese",
	L"turks",
	L"indians",			// 30
	L"mexicans",
	L"norwegians",
	L"spaniards",
	L"brasilians",
	L"finns",			// 35
	L"iranians",
	L"israelis",
	L"bulgarians",
	L"swedes",
	L"iraqis",			// 40
	L"syrians",
	L"belgians",
	L"portoguese",
	L"belarusian",
	L"serbians",		// 45
	L"pakistanis",
	L"albanians",
	L"argentinians",
	L"armenians",
	L"azerbaijani", // 50
	L"bolivians",
	L"chileans",
	L"circassians",
	L"columbians",
	L"egyptians", // 55
	L"ethiopians",
	L"georgians",
	L"jordanians",
	L"kazakhstani",
	L"kenyans", // 60
	L"koreans",
	L"kyrgyzstani",
	L"mongolians",
	L"palestinians",
	L"panamanians", // 65
	L"rhodesians",
	L"salvadorans",
	L"saudis",
	L"somalis",
	L"thais", // 70
	L"ukrainians",
	L"uzbekistani",
	L"welshs",
	L"yazidis",
	L"zimbabweans", // 75
};

// special text used if we do not hate any nation (value of -1)
STR16		szNationalityText_Special[]=
{
	L"and do not hate any other nationality.",		// used in personnel.cpp
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

STR16	szSoldierClassName[]=	// TODO.Translate
{
	L"Mercenary",
	L"Green militia",
	L"Regular militia",
	L"Elite militia",

	L"Civilian",

	L"Administrator",
	L"Army Soldier",
	L"Elite Soldier",
	L"Tank",

	L"Creature",
	L"Zombie",
};

STR16	szCampaignHistoryWebSite[]=
{
	L"%s Press Council",
	L"Ministry for %s Information Distribution",
	L"%s Revolutionary Movement",
	L"The Times International",
	L"International Times",
	L"R.I.S. (Recon Intelligence Service)",

	L"A collection of press sources from %s",
	L"We are a neutral source of information. We collect different news articles from %s. We do not judge these sources - we merely publish them, so you can judge yourself. We post articles from various sources, among them",
	
	L"Conflict Summary",
	L"Battle reports",
	L"News",
	L"About us",
};

STR16	szCampaignHistoryDetail[]=
{
	L"%s, %s %s %s in %s.",

	L"rebel forces",
	L"the army",

	L"attacked",
	L"ambushed",
	L"airdropped",

	L"The attack came from %s.",
	L"%s were reinforced from %s.",
	L"The attack came from %s, %s were reinforced from %s.",
	L"north",
	L"east",
	L"south",
	L"west",
	L"and",
	L"an unknown location",	// TODO.Translate

	L"Buildings in the sector were damaged.",	// TODO.Translate
	L"In the fighting, buildings in the sector were damaged, and %d civilians were killed and %d wounded.",
	L"During the attack, %s and %s called reinforcements.",
	L"During the attack, %s called reinforcements.",
	L"Eyewitnesses report the use of chemical weapons from both sides.",
	L"Chemical weapons were used by %s.",
	L"In a serious escalation of the conflict, both sides deployed tanks.",
	L"%d tanks were used by %s, %d of them were destroyed in the fierce fighting.",
	L"Both sides are said to have used snipers.",
	L"Unverified reports indicate %s snipers were involved in the firefight.",
	L"This sector is of huge strategic importance, as it houses one of the handful of anti-air missile batteries the %s army posesses. Aerial photographs show extensive damage to the command center. This will leave the airspace above %s undefended for the time being.",
	L"The situation on the ground has gotten even more confusing, as it seems rebel infighting has reached a new level. We now have confirmation that rebel militia engaged in active combat with foreign mercenaries.",
	L"The royalists position seems more precarious than previously thought. Reports of a split surfaced, with army personnel opening fire on each other.",
};

STR16	szCampaignHistoryTimeString[]=
{
	L"Deep in the night",		// 23 - 3
	L"At dawn",					// 3 - 6
	L"Early in the morning",	// 6 - 8
	L"In the morning hours",	// 8 - 11
	L"At noon",					// 11 - 14
	L"On the afternoon",		// 14 - 18
	L"On the evening",			// 18 - 21
	L"During the night",		// 21 - 23
};

STR16	szCampaignHistoryMoneyTypeString[]=
{
	L"Initial funding",
	L"Mine income",
	L"Trade",
	L"Other sources",
};

STR16	szCampaignHistoryConsumptionTypeString[]=
{
	L"Ammunition",
	L"Explosives",
	L"Food",
	L"Medical gear",
	L"Item maintenance",
};

STR16	szCampaignHistoryResultString[]=
{
	L"In an extremely one-sided battle, the army force was wiped out without much resistance.",

	L"The rebels easily defeated the army, inflicting heavy losses.",
	L"Without much effort, the rebels inflicted heavy losses upon the army and took several prisoners.",

	L"In a bloody fight, the rebels finally overcame the opposition. The army had severe losses.",
	L"The rebels had losses but defeated the royalists. Unverified information says several soldiers might have been taken prisoner.",

	L"In a phyrric victory, the rebels defeated the royalists but had severe casualties of their own. Whether they will be able to hold this position against continued attacks is doubtful.",
	
	L"The army's superiority in numbers came into full play. The rebels never had a chance and had to either retreat or be killed or captured.",
	L"Despite the high number of rebels in this sector, the army easily dispatched them.",

	L"The rebels were clearly unprepared against the army's superiority in numbers an equipment. They were easily defeated.",
	L"Even though the rebels had more boots on the ground, the army was better equipped. The rebels clearly lost.",

	L"Fierce fighting saw significant losses on both sides, but in the end, the army's higher number of bodies decided the battle. The rebel force was destroyed. There might have been survivors, but we cannot verify this at this point.",
	L"In an intense firefight, the superior training of the armed forces tipped the scales. The rebels had to retreat.",
	
	L"Neither side was willing to yield. While the army ultimately removed the rebel threat in the area, the staggering losses have resulted in the army unit continuing to exist in name only. But it is clear the rebels will soon be out of men and women if the army can keep on this rate of attrition.",
};

STR16	szCampaignHistoryImportanceString[]=
{
	L"Irrelevant",
	L"Insignificant",
	L"Notable",
	L"Noteworthy",
	L"Significant",
	L"Interesting",
	L"Important",
	L"Very important",
	L"Grave",
	L"Major",
	L"Momentous",
};

STR16	szCampaignHistoryWebpageString[]=
{
	L"Killed",
	L"Wounded",
	L"Prisoners",
	L"Shots fired",

	L"Money earned",
	L"Consumption",
	L"Losses",
	L"Participants",

	L"Promotions",
	L"Summary",
	L"Detail",
	L"Previous",

	L"Next",
	L"Incident",
	L"Day",
};

STR16	szCampaignStatsOperationPrefix[] =	// TODO.Translate
{
	L"Glorious %s",
	L"Mighty %s",
	L"Awesome %s",
	L"Intimidating %s",

	L"Powerful %s",
	L"Earth-Shattering %s",
	L"Insidious %s",
	L"Swift %s",

	L"Violent %s",
	L"Brutal %s",
	L"Relentless %s",
	L"Merciless %s",

	L"Cannibalistic %s",
	L"Gorgeous %s",
	L"Rogue %s",
	L"Dubious %s",

	L"Sexually Ambigious %s",
	L"Burning %s",
	L"Enraged %s",
	L"Visonary %s",

	// 20
	L"Gruesome %s",
	L"International-law-ignoring %s",
	L"Provoked %s",
	L"Ceaseless %s",

	L"Inflexible %s",
	L"Unyielding %s",
	L"Regretless %s",
	L"Remorseless %s",

	L"Choleric %s",
	L"Unexpected %s",
	L"Democratic %s",
	L"Bursting %s",

	L"Bipartisan %s",
	L"Bloodstained %s",
	L"Rouge-wearing %s",
	L"Innocent %s",

	L"Hateful %s",
	L"Underwear-staining %s",
	L"Civilian-devouring %s",
	L"Unflinching %s",

	// 40
	L"Expect No Mercy From Our %s",
	L"Very Mad %s",
	L"Ultimate %s",
	L"Furious %s",

	L"Its best to Avoid Our %s",
	L"Fear the %s",
	L"All Hail the %s!",
	L"Protect the %s",

	L"Beware the %s",
	L"Crush the %s",
	L"Backstabbing %s",
	L"Vicious %s",

	L"Sadistic %s",
	L"Burning %s",
	L"Wrathful %s",
	L"Invincible %s",

	L"Guilt-ridden %s",
	L"Rotting %s",
	L"Sanitized %s",
	L"Self-doubting %s",

	// 60
	L"Ancient %s",
	L"Very Hungry %s",
	L"Sleepy %s",
	L"Demotivated %s",

	L"Cruel %s",
	L"Annoying %s",
	L"Huffy %s",
	L"Bisexual %s",

	L"Screaming %s",
	L"Hideous %s",
	L"Praying %s",
	L"Stalking %s",

	L"Cold-blooded %s",
	L"Fearsome %s",
	L"Trippin' %s",
	L"Damned %s",

	L"Vegetarian %s",
	L"Grotesque %s",
	L"Backward %s",
	L"Superior %s",

	// 80
	L"Inferior %s",
	L"Okay-ish %s",
	L"Porn-consuming %s",
	L"Poisoned %s",

	L"Spontaneous %s",
	L"Lethargic %s",
	L"Tickled %s",
	L"The %s is a dupe!",

	L"%s on Steroids",
	L"%s vs. Predator",
	L"A %s with a twist",
	L"Self-Pleasuring %s",

	L"Man-%s hybrid",
	L"Inane %s",
	L"Overpriced %s",
	L"Midnight %s",

	L"Capitalist %s",
	L"Communist %s",
	L"Intense %s",
	L"Steadfast %s",

	// 100
	L"Narcoleptic %s",		// TODO.Translate
	L"Bleached %s",
	L"Nail-biting %s",
	L"Smite the %s",

	L"Bloodthirsty %s",
	L"Obese %s",
	L"Scheming %s",
	L"Tree-Humping %s",

	L"Cheaply made %s",
	L"Sanctified %s",
	L"Falsely accused %s",
	L"%s to the rescue",

	L"Crab-people vs. %s",
	L"%s in Space!!!",
	L"%s vs. Godzilla",
	L"Untamed %s",

	L"Durable %s",
	L"Brazen %s",
	L"Greedy %s",
	L"Midnight %s",

	// 120
	L"Confused %s",
	L"Irritated %s",
	L"Loathsome %s",
	L"Manic %s",

	L"Ancient %s",
	L"Sneaking %s",
	L"%s of Doom",
	L"%s's revenge",

	L"A %s on the run",
	L"A %s out of time",
	L"One with %s",
	L"%s from hell",

	L"Super-%s",
	L"Ultra-%s",
	L"Mega-%s",
	L"Giga-%s",

	L"A quantum of %s",
	L"Her Majesties' %s",
	L"Shivering %s",
	L"Fearful %s",

	// 140
};

STR16	szCampaignStatsOperationSuffix[] =
{
	L"Dragon",
	L"Mountain Lion",
	L"Copperhead Snake",
	L"Jack Russell Terrier",

	L"Arch-Nemesis",
	L"Basilisk",
	L"Blade",
	L"Shield",

	L"Hammer",
	L"Spectre",
	L"Congress",
	L"Oilfield",

	L"Boyfriend",
	L"Girlfriend",
	L"Husband",
	L"Stepmother",

	L"Sand Lizard",
	L"Bankers",
	L"Anaconda",
	L"Kitten",

	// 20
	L"Congress",
	L"Senate",
	L"Cleric",
	L"Badass",

	L"Bayonet",
	L"Wolverine",
	L"Soldier",
	L"Tree Frog",

	L"Weasel",
	L"Shrubbery",
	L"Tar pit",
	L"Sunset",

	L"Hurricane",
	L"Ocelot",
	L"Tiger",
	L"Defense Industry",

	L"Snow Leopard",
	L"Megademon",
	L"Dragonfly",
	L"Rottweiler",

	// 40
	L"Cousin",
	L"Grandma",
	L"Newborn",
	L"Cultist",

	L"Disinfectant",
	L"Democracy",
	L"Warlord",
	L"Doomsday Device",

	L"Minister",
	L"Beaver",
	L"Assassin",
	L"Rain of Burning Death",

	L"Prophet",
	L"Interloper",
	L"Crusader",
	L"Administration",

	L"Supernova",
	L"Liberty",
	L"Explosion",
	L"Bird of Prey",

	// 60
	L"Manticore",
	L"Frost Giant",
	L"Celebrity",
	L"Middle Class",

	L"Loudmouth",
	L"Scape Goat",
	L"Warhound",
	L"Vengeance",

	L"Fortress",
	L"Mime",
	L"Conductor",
	L"Job-Creator",

	L"Frenchman",
	L"Superglue",
	L"Newt",
	L"Incompetency",

	L"Steppenwolf",
	L"Iron Anvil",
	L"Grand Lord",
	L"Supreme Ruler",

	// 80
	L"Dictator",
	L"Old Man Death",
	L"Shredder",
	L"Vacuum Cleaner",

	L"Hamster",
	L"Hypno-Toad",
	L"Discjockey",
	L"Undertaker",

	L"Gorgon",
	L"Child",
	L"Mob",
	L"Raptor",

	L"Goddess",
	L"Gender Inequality",
	L"Mole",
	L"Baby Jesus",

	L"Gunship",
	L"Citizen",
	L"Lover",
	L"Mutual Fund",

	// 100
	L"Uniform",			// TODO.Translate
	L"Saber",
	L"Snow Leopard",
	L"Panther",

	L"Centaur",
	L"Scorpion",
	L"Serpent",
	L"Black Widow",

	L"Tarantula",
	L"Vulture",
	L"Heretic",
	L"Zombie",

	L"Role-Model",
	L"Hellhound",
	L"Mongoose",
	L"Nurse",

	L"Nun",
	L"Space Ghost",
	L"Viper",
	L"Mamba",

	// 120
	L"Sinner",
	L"Saint",
	L"Comet",
	L"Meteor",

	L"Can of worms",
	L"Fish oil pills",
	L"Breastmilk",
	L"Tentacle",

	L"Insanity",
	L"Madness",
	L"Cough reflex",
	L"Colon",

	L"King",
	L"Queen",
	L"Bishop",
	L"Peasant",

	L"Tower",
	L"Mansion",
	L"Warhorse",
	L"Referee",

	// 140
};

STR16	szMercCompareWebSite[] =	// TODO.Translate
{
	// main page
	L"Mercs Love or Dislike You",
	L"Your #1 teambuilding experts on the web",

	L"About us",
	L"Analyse a team",
	L"Pairwise comparison",
	L"Customer voices",

	L"If your business provides innovative solutions for critical applications with realtime demands, perhaps some of these observations sound familiar to you:",
	L"Your team struggles with itself.",
	L"Your employees waste time working against each other.",
	L"Your workforce experiences a high fluctuation rate.",
	L"You constantly receive low marks on workplace satisfaction ratings.",	
	L"If you can say 'yes' to one or more of these statements, then you might have a problem in your business. Your employees likely won't work at peak perfection. Thanks to our patented, easy-to-understand MeLoDY system, you can bring productivity back up in no time, and a happy smile on the faces all your staff!",

	// customer quotes
	L"A few citations from our satisfied customers:",
	L"My last relationship was terrible. I blamed myself... but now I know better. All men deserve a violent death! Thanks, MeLoDY, for enlightening me!",
	L"-Louisa G., Novelist-",
	L"I never got along with my brothers to start with, and recently its gotten worse. You've shown me that our trust problem with father is to blame. Thank you for that! I have to make a bold statement to open his eyes to the facts.",
	L"-Konrad C., Corrective law enforcement-",
	L"I've always been a loner, so joining a team was hard for me. Your insight showed me how to become part of a team. You've been a big help!",
	L"-Grant W., Snake charmer-",
	L"In my line of work, you need to trust every member of your team 100%%. That's why we went to the experts - we went to MeLoDY.",
	L"-Halle L., SPK-",
	L"I'll be the first to admit our crew was a rather illustrious assortion of characters, and we ran into some scuffles. But we learned to respect each other, and now complement each other perfectly.",
	L"-Michael C., NASA-",
	L"I fully recommend this site!",
	L"-Kasper H., H&C logistic Inc-",
	L"Our training process has to be very quick, so we need to know whom we're dealing with. MeLoDY were the logical choice for this.",
	L"-Stan Duke, Kerberus Inc-",

	// analyze
	L"Choose your employee",
	L"Choose your squad",

	// error messages
	L"You currently have no employees at their workplace. Sub-par morale often results in a high rate of absent staff.",
};

STR16	szMercCompareEventText[]=
{
	L"%s shot me!",
	L"%s is scheming behind my back",
	L"%s interfered in my business",
	L"%s is friends with my enemy",

	L"%s got a contract before I did",
	L"%s ordered a shameful retreat",
	L"%s massacred the innocent",
	L"%s slows us down",

	L"%s doesn't share food",
	L"%s jeopardizes the mission",
	L"%s is a drug addict",
	L"%s is thieving scum",

	L"%s is an incompetent commander",
	L"%s is overpaid",
	L"%s gets all the good stuff",
	L"%s mounted a gun on me",

	L"%s treated my wounds",
	L"Had a good drink with %s",
	L"%s is fun to get wasted with",
	L"%s is annoying when drunk",

	L"%s is an idiot when drunk",
	L"%s opposed our view in an argument",
	L"%s supported our position",
	L"%s agrees to our reasoning",

	L"%s's beliefs are contrary to ours",
	L"%s knows how to calm down people",
	L"%s is insensitive",
	L"%s puts people in their places",

	L"%s is way too impulsive",
	L"%s is disease-ridden",
	L"%s treated my diseases",
	L"%s does not hold back in combat",

	L"%s enjoys combat a bit too much",
	L"%s is a good teacher",
	L"%s led us to victory",
	L"%s saved my life",

	L"%s stole my kill",
	L"%s and me fought well together",
	L"%s made the enemy surrender",
};

STR16	szWHOWebSite[] =
{
	// main page
	L"World Health Organization",
	L"Bringing health to life",

	// links to other pages
	L"About WHO",
	L"Disease in Arulco",
	L"About diseases",

	// text on the main page
	L"WHO is the directing and coordinating authority for health within the United Nations system.",
	L"It is responsible for providing leadership on global health matters, shaping the health research agenda, setting norms and standards, articulating evidence-based policy options, providing technical support to countries and monitoring and assessing health trends.",
	L"In the 21st century, health is a shared responsibility, involving equitable access to essential care and collective defence against transnational threats.",

	// contract page
	L"The small country of Arulco is currently experiencing an outbreak of the deadly arulcan plague.",
	L"Due to the catastrophic state of the state's health system, only the armies' medical corps is there to combat the deadly disease.",
	L"With the country being of limits to UN affiliates, all we can currently do is provide detailed maps on the current status of infection in Arulco. Due to the difficulty in dealing with Arulco, we regret to have to ask for a daily fee of %d$ for anyone wishing to obtain these maps.",
	L"Do you wish to acquire detailed data on the current status of diease in Arulco? You can access this data on the strategic map once aquired.",
	L"You currently do not have access to WHO data on the arulcan plague.",
	L"You have acquired detailed maps on the status of the disease.",
	L"Subscribe to map updates",
	L"Unsubscribe map updates",

	// helpful tips page
	L"The arulcan plague is a deadly strain of the plague unique to the small country of Arulco. In a typical outbreak, the first victims get infected by a mosquito in a swamp or tropical sector. These first victims then inadvertently infect the population of nearby cities.",
	L"You won't immediately notice when you are infected - it might take days for the symptoms to show.",
	L"You can see the current effects of known diseases your mercs suffer from by hovering over their portrait in the strategic map.",
	L"Most diseases get worse over time, be sure to assign a doctor as soon as possible.",
	L"Some diseases can be treated with special medicine. You might find some in a well-equipped drugstore.",
	L"Doctors can be ordered to check on all local teammates for diseases. You can find out about a disease before it breaks out!",
	L"Doctors have a much higher chance to be infected when treating infected patients. Protective gear is very useful.",
	L"If a blade weapon hits an infected person, the blade becomes infected, and can be used to spread the infection further.",
};

STR16	szPMCWebSite[] =
{
	// main page
	L"Kerberus",
	L"Experience In Security",

	// links to other pages
	L"What is Kerberus?",
	L"Team Contracts",
	L"Individual Contracts",

	// text on the main page
	L"Kerberus is a well known international private military contractor. Founded in 1983, we provide security and armed forces training around the world.",
	L"Our extensively trained personnel provides security for over 30 governments areound the world. This includes several conflict zones.",
	L"We have several training centres around the globe, including in Indonesia, Colombia, Katar, South Africa and Romania. As a result, we can usually fulfil your contract requirements within 24 hours.",
	L"Under 'Individual Contracts', we offer individual contracts with experienced veterans in the field of security.",
	L"You can also hire an entire security team. In the 'Team Contracts' page, you can select how many of our personnel you want to hire, and where you require their services. Due to regrettable incidents in the past, we have to insist that the landing zone be under your control prior to debarkation.",
	L"Our team can deploy by air, in which case, of course, an airport is required. Depending on the country our services are required in, insetion via harbours or border posts is also possible.",
	L"An advance payment is required. After that, the daily fee for our personnel will be deducted from your account.",

	// militia contract page
	L"You can select the type and number of personnel you want to hire here:",
	L"Initial deployment",
	L"Regular personnel",
	L"Veteran personnel",

	L"%d available, %d$ each",
	L"Hire: %d",
	L"Cost: %d$",

	L"Select the initial operational area:",
	L"Total Cost: %d$",
	L"ETA: %02d:%02d",
	L"Close Contract",

	L"Thank you! Our personnel will be on site on %02d:%02d tomorrow.",
	L"Kerberus reinforcements have arrived in %s.",
	L"Next deployment: %d regulars and %d veterans at %s on %02d:%02d, day %d.",
	L"You do not control any location through which we could insert troops!",

	// individual contract page
};

STR16   szTacticalInventoryDialogString[]=
{
        L"Inventory Manipulations",

        L"NVG",
        L"Reload All",
        L"Move",		// TODO.Translate
        L"",

        L"Sort",
        L"Merge",
        L"Separate",
        L"Organize",

        L"Crates",
        L"Boxes",
        L"Drop B/P",
        L"Pickup B/P",

        L"",
        L"",
        L"",
        L"",
};

STR16   szTacticalCoverDialogString[]=
{
        L"Cover Display Mode",

        L"Off",
        L"Enemy",
        L"Merc",
        L"",

        L"Roles",	// TODO.Translate
		L"Fortification",	// TODO.Translate
		L"Tracker",
		L"CTH mode",

        L"Traps",
        L"Network",
        L"Detector",
        L"",

        L"Net A",
        L"Net B",
        L"Net C",
        L"Net D",
};

STR16   szTacticalCoverDialogPrintString[]=
{
        
        L"Turning off cover/traps display",
        L"Showing danger zones",
        L"Showing merc view",
        L"",

		L"Display enemy role symbols",	// TODO.Translate
		L"Display planned fortifications",
		L"Display enemy tracks",
        L"",

        L"Display trap network",
        L"Display trap network colouring",
        L"Display nearby traps",
        L"",

        L"Display trap network A",
        L"Display trap network B",
        L"Display trap network C",
        L"Display trap network D",
};

// TODO.Translate
STR16	szDynamicDialogueText[40][17] =	// TODO.Translate
{
	// OPINIONEVENT_FRIENDLYFIRE
	L"What the hell! $CAUSE$ attacked me!",
	L"",
	L"",
	L"What? Me? No way, I'm engaging at the enemy!",
	L"Oops.",
	L"",
	L"",
	L"$CAUSE$ has attacked $VICTIM$. What do you do?",
	L"Nah, that must have been enemy fire!",
	L"Yeah, I saw it too!",
	L"Don't play stupid, $CAUSE$. You had a clear line of sight! What side are you on?",
	L"I saw it, it was clearly enemy fire!",
	L"In the heat of battle, this can happen. Just be more careful next time, $CAUSE$.",
	L"This is war! People get shot all the time! Speaking of... shoot more people, people!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SNITCHSOLDMEOUT
	L"Hey! Keep your mouth shut, $CAUSE$! Freakin' snitch!",
	L"",
	L"",
	L"I would if you weren't such a wussy!",
	L"You heard that? Dammit.",
	L"",
	L"",
	L"$VICTIM$ is angry with $CAUSE$ because $CAUSE_GENDER$ spoke to you. What do you do?",
	L"Nono, $CAUSE$, speak up... What did $VICTIM$ do?",
	L"Yeah, mind your own business, $CAUSE$!",
	L"This isn't girls college, keep your snickering to yourself, $CAUSE$.",
	L"Yeah. Man up!",
	L"I'm not sure whether it was the correct way, but $CAUSE_GENDER$ does have a point...",
	L"This again? Keep your bickering to yourself, we have no time for this!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SNITCHINTERFERENCE
	L"$CAUSE$ is bullying me again!",
	L"",
	L"",
	L"You're jeopardising the entire mission, and I won't let that stand any longer!",
	L"Hey, it's for your own good. You'll thank me later.",
	L"",
	L"",
	L"$CAUSE$ has stopped $VICTIM$ from misbehaving, and $VICTIM_GENDER$ is out for revenge. What do you do?",
	L"Then stop giving reasons for that!",
	L"Drop it, $CAUSE$, who are you to tell us what to do?!",
	L"You won't let that stand? Cute. Who ever made you the boss?",
	L"Agreed. We can't let our guard down for a single moment!",
	L"Can't you two sort this out?",
	L"Meh meh meh. Have any of you losers lost their bib? You're pathetic. ",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FRIENDSWITHHATED
	L"Hmpf. Typical $CAUSE$, figured $CAUSE_GENDER$ would hang out with the wrong crowd!",
	L"",
	L"",
	L"My friends are none of your business!",
	L"Can't you two all get along, $VICTIM$?",
	L"",
	L"",
	L"$VICTIM$ doesn't like $CAUSE$'s friend. What do you do?",
	L"Everybody can hang out with whom $CAUSE_GENDER$ wants!",
	L"Yeah, you guys are ugly!",
	L"Then you should change your business. 'Cause its bad.",
	L"What's that to you, $VICTIM$?",
	L"Yeah yeah, terrible business, that. Are you sure that is the MOST PRESSING issue right now?",
	L"Nobody wants to hear all this crap...",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CONTRACTEXTENSION
	L"Yeah, sure. My contract's about to end, but no, gotta take care of $CAUSE$ first.",
	L"",
	L"",
	L"Oh yeah? I'm actually useful. Perhaps that's why you didn't get an extension.",
	L"I'm sure you'll get your extension soon. You know how odd online banking can be.",
	L"",
	L"",
	L"$VICTIM$ is jealous as we extended $CAUSE$'s contract early. What do you do?",
	L"You are still getting paid, no? What does it matter as long as you get the money?",
	L"And your contract ends so soon, $CAUSE$... I hope you get an extension.",
	L"Yeah. All that worth hasn't shown yet though.",
	L"Aww, that burns, doesn't it, $VICTIM$?",
	L"We have limited funds. Someone needs to get paid first, right?",
	L"You'll all get paid in time. Unless you continue like this. I'm sure there are less annoying mercs out there.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ORDEREDRETREAT
	L"Nice command, $CAUSE$! Why would you order retreat?",
	L"",
	L"",
	L"I just got us out of that hellhole, you should thank me for saving your life.",
	L"They were flanking us, there was no other way!",
	L"",
	L"",
	L"$VICTIM$ dislikes the retreat command $CAUSE$ gave. What do you do?",
	L"You know you can go back if you want... nobody's stopping you.",
	L"We were rockin' it until that point.",
	L"Oh, now YOU got us out? By being the first to leave? How noble of you.",
	L"I sure did, $CAUSE$. Man, I don't want to go back THERE again.",
	L"We're still alive, this is what counts.",
	L"The more pressing issue is when will we go back, and finish the job?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CIVKILLER
	L"What the hell is wrong with you, $CAUSE$? You just killed an innocent!",
	L"",
	L"",
	L"He had a gun, I saw it!",
	L"Oh god. No! What have I done?",
	L"",
	L"",
	L"$VICTIM$ is furious: $CAUSE$ killed a civilian. What do you do?",
	L"Better safe than sorry I say...",
	L"Yeah. The poor sod never had a chance. Damn.",
	L"Don't talk crap. That was an unarmed civilian. We are here to protect these people!",
	L"And you took him down nice and clean, good job!",
	L"This is war. People die all the time... though I'd prefer it if it were less obvious that we are so, ehm, proactive.",
	L"Who cares? Can't make a cake without breaking a few eggs.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SLOWSUSDOWN
	L"Can we get rid of $CAUSE$, please? That slowpoke is holding us back.",
	L"",
	L"",
	L"I wouldn't if you would carry your fair share of the gear, $VICTIM$!",
	L"It's all this stuff, it's so heavy!",
	L"",
	L"",
	L"$VICTIM$ feels $CAUSE$ slows down the team. What do you do?",
	L"We leave nobody behind, not even $CAUSE$!",
	L"We have to move fast, the enemy isn't far behind!",
	L"Everybody carries his gear. How do you expect to fight if you can't even carry your gun?",
	L"Aye, stop complaining. We go through this together or we don't do it at all.",
	L"If you are so annoyed that $CAUSE$ is slow, $VICTIM$, maybe you could lend a hand.",
	L"If you still have enough breath left to complain, $VICTIM$, you should lend $CAUSE_GENDER$ a hand.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_NOSHARINGFOOD
	L"Damn $CAUSE$, $CAUSE_GENDER$ is keeping all that tasty food to $CAUSE_PRONOUN$self...",
	L"",
	L"",
	L"Not my problem if you already ate all your rations.",
	L"Oh $VICTIM$, why didn't you say something then?",
	L"",
	L"",
	L"$VICTIM$ wants $CAUSE$'s food. What do you do?",
	L"We all get the same. If you used up yours already that's your problem.",
	L"If $CAUSE$ shares, I want some too!",
	L"Why do you have so much food anyway? Do I smell extra rations there?.",
	L"Right, everyone got enough back at the base...",
	L"There's enough food left at the base, so no need to fight, ok?",
	L"I wouldn't call that stuff 'tasty', but if you ladies wanna fight about it, fine by me.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ANNOYINGDISABILITY
	L"Oh, come on, $CAUSE$. It's not a good moment!",
	L"",
	L"",
	L"Just what I need. Good advice. Thanks, $VICTIM$, you are a big help.",
	L"It's my only weakness, I can't help it!",
	L"",
	L"",
	L"$CAUSE$' tick is getting on $VICTIM$'s nerves. What do you do?",
	L"What does it even matter to you? Don't you have something to do?",
	L"Really $CAUSE$. This is a military organization and not a ward.",
	L"Well, we are pros, an you're not, $CAUSE$.",
	L"Don't be such a snob, $VICTIM$.",
	L"Uhm. Is $CAUSE_GENDER$ okay?",
	L"Bla bla blah. Another notable moment of the crazies squad.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_ADDICT
	L"$CAUSE$ is high like a zeppelin! How am I supposed to work with that junkie?",
	L"",
	L"",
	L"Taking the stick out of your butt would be a starter, jeez...",
	L"I've seen things man. And some... stuff!",
	L"",
	L"",
	L"$CAUSE$ took drugs and $VICTIM$ saw it. What do you do?",
	L"Hey, $CAUSE$ needs to ease the stress, ok?",
	L"How unprofessional.",
	L"This is war and not a stoner party. Cut it out, $CAUSE$!",
	L"Hehe. So true.",
	L"I am sure there is a good explanation for this. Am I right, $CAUSE$?",
	L"You can inject yourself with whatever you like, but only after the battle is over.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_THIEF
	L"What the... Hey! $CAUSE$! Give it back, you damn thief!",
	L"",
	L"",
	L"Have you been drinking? What the hell is your problem?",
	L"You noticed? Dammit... 50/50?",
	L"",
	L"",
	L"$VICTIM$ saw $CAUSE$ steal an item. What do you do?",
	L"If you don't have any proof, don't throw around accusations, $VICTIM$.",
	L"So that's the kind of people were stuck with, $VICIM$? I better watch my wallet then.",
	L"HEY! You put that back right now!",
	L"No idea. All of a sudden $VICTIM$ is all drama.",
	L"Perhaps we could get a raise to resolve this... issue?",
	L"Shut up! There is more than enough loot for all of us!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_WORSTCOMMANDEREVER
	L"What a disaster. That was worst command ever, $CAUSE$!",
	L"",
	L"",
	L"I don't have to take that from a grunt like you!",
	L"Dream on, $VICTIM$. I'm worth every penny, and you know it!",
	L"",
	L"",
	L"$CAUSE$ does not trust $VICTIM$'s orders. What do you do?",
	L"Someone had take the lead and $CAUSE$ did it. Did you have a better plan?",
	L"Well, we sure aren't going to win the war at this rate...",
	L"Then take it from me... that wasn't 'commanding' as much as 'handwaving'.",
	L"We have a clear chain of command, and you sure as hell aren't on top, $VICTIM$!",
	L"We will not forget their sacrifice. They died so we could fight on.",
	L"What? This is the business. Screw up and you're dead. They knew the risks. Move on.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_RICHGUY
	L"How can $CAUSE$ earn this much? It's not fair!",
	L"",
	L"",
	L"Dream on, $VICTIM$. I'm worth every penny, and you know it!",
	L"You don't expect me to complain, do you?",
	L"",
	L"",
	L"$CAUSE$ is jealous of $VICTIM$'s paycheck. What do you do?",
	L"Quit whining about the money, you get more than enough yourself!",
	L"In all honesty, $VICTIM$, I think you should adjust your rate!",
	L"Worth it? All you do is pose!",
	L"Relax. At least some of us appreciate your service, $CAUSE$.",
	L"Perhaps $CAUSE_GENDER$ is just good at salary negotiations?",
	L"Everybody gets what the deserve. If you complain, you deserve less.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BETTERGEAR
	L"$CAUSE$ is stocking the good gear for $CAUSE_PRONOUN$self. Not fair!",
	L"",
	L"",
	L"Contrary to you, I actually know how to use them.",
	L"Well, someone has to use it, right? Better luck next time!",
	L"",
	L"",
	L"$CAUSE$ is jealous of $VICTIM$'s equipment. What do you do?",
	L"You're just making up an excuse for your poor marksmanship.",
	L"Yeah, this smells of cronyism to me.",
	L"If by 'use' you mean 'waste a lot bullets', then yeah, you are a pro.",
	L"I'll second that!",
	L"Is that so? Well, I expect superior marksmanship from $CAUSE_GENDER$ from now on.",
	L"Did it ever occur to you that our supplies are limited? We can't ALL get the one good gun.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_YOUMOUNTEDAGUNONMYBREASTS
	L"Do I look like a bipod? Get that thing off me, $CAUSE$!",
	L"",
	L"",
	L"Don't be such a wuss. This is war!",
	L"Oh. Didn't see you for a minute there.",
	L"",
	L"",
	L"$CAUSE$ used $VICTIM$'s chest as a gun rack. How odd. What do you do?",
	L"Don't be such a wuss. This is war!",
	L"Wow. Are you trying to be a jerk, $CAUSE$, or is this normal for you?",
	L"You are and always will be an insensitive jerk, $CAUSE$.",
	L"Sometimes you just have to use your surroundings!",
	L"Ehm... what are you two DOING?",
	L"This is hardly the time to fondle each other, dammit.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BANDAGED
	L"Thanks, $CAUSE$. I thought I was gonna bleed out.",
	L"",
	L"",
	L"I'm doing my job. Get back to yours!",
	L"Hey, we have to look after each other. You'd do the same, $VICTIM$.",
	L"",
	L"",
	L"$CAUSE$ has bandaged $VICTIM$. What do you do?",
	L"Patched together again? Good, now move!",
	L"You're welcome.",
	L"Jeez. Woken up on the wrong foot today?",
	L"Talk about a no-nonsense approach...",
	L"How did you even get wounded? Where did the attack come from?",
	L"You need to be more careful. Now, where did the attack come from?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_GOOD
	L"Yeah, $CAUSE$! You get it! How 'bout next round?",
	L"",
	L"",
	L"Pah. I think you've had enough.",
	L"Sure. Bring it on!",
	L"",
	L"",
	L"Drunk $VICTIM$ likes $CAUSE$. What do you do?",
	L"Yeah, enough booze for you today.",
	L"Hoho, party!",
	L"Party pooper!",
	L"You sure like to keep a cool head, $CAUSE$.",
	L"Alright, ladies, party's over, move on!",
	L"Who told you to slack off? You have a job to do!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_SUPER
	L"$CAUSE$... you're... you are... hic... you're the best!",
	L"",
	L"",
	L"Tehehe. $VICTIM$, you are soooo wasted. You.. hic. You need to restrain yourself. Discipline, you know? Like me!",
	L"Hic... yeah. You too, $VICTIM$. You. hic.. too!",
	L"",
	L"",
	L"Drunk $VICTIM$ dislikes $CAUSE$. What do you do?",
	L"Whatever. We still have a job to do, so this party is over for you, $VICTIM$.",
	L"Heeeey... this is actually kinda nice for a change.",
	L"'I know discipline', said the clueless drunk...",
	L"Yeargh. Dis... gulp! Disciplined like... ehm... us!",
	L"Drink one for me too! But not now, because war.",
	L"You celebrate already ? This in't over yet. Not by a longshot!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_BAD
	L"Damn, $CAUSE$! You... you got enough... Learn to keep your liquor...",
	L"",
	L"",
	L"Cut it out, $VICTIM$! You clearly don't know when to stop!",
	L"Tehehe. You are RIGHT. Hehe. Always right. Hic!",
	L"",
	L"",
	L"$VICTIM$ is wasted and wants to be $VICTIM$'s buddy. What do you do?",
	L"Relax, it's just a bit of booze.",
	L"Hey keep it down over there, okay?",
	L"You're just as drunk. Beat it, $CAUSE$!",
	L"Leave alcohol to the big ones, okay?",
	L"Later, ok?",
	L"We might've won this battle, but not this godamn war. So move it, soldier!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DRINKBUDDIES_WORSE
	L"What the hell, $CAUSE$! Ugh... I'm never drinking with you again, you sicko.",
	L"",
	L"",
	L"Oh SHUT UP $VICTIM$! You don't know how to beh.. beha... beha? Ehm... You make no sense. At all. Yeah. You're no fun.",
	L"Well you're no fu... fu... fu? Fun! You are not. Hic!. No.",
	L"",
	L"",
	L"$VICTIM$ is wasted and wants to tear $VICTIM$ a new one. What do you do?",
	L"Jeez $VICTIM$, why so uptight all of a sudden? Anything happened?",
	L"This party was cool until $CAUSE$ ruined it!",
	L"$CAUSE$! Shut it! You are a disgrace for this unit. Get out and sober up!",
	L"Word!",
	L"Why don't you two sober up? You're pretty wasted...",
	L"Both of you, shut up! You are a disgrace to this unit!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_AGAINST_US
	L"I knew I couldn't depend on you, $CAUSE$!",
	L"",
	L"",
	L"Depend? It was all your fault!",
	L"Touched a nerve there, didn't I?",
	L"",
	L"",
	L"$VICTIM$ does not like was $CAUSE$ has to say. What do you do?",
	L"So you depend on others to do your job? Then what good are you?!",
	L"Indeed. Way to let $VICTIM$ hang!",
	L"This isn't some schoolyard sympathy crap. It WAS your fault!",
	L"Yeah, what's with the attitude?",
	L"Zip it, both of you!",
	L"Silence, now!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FOR_US
	L"Ha! See? Even $CAUSE$ agrees with me.",
	L"",
	L"",
	L"'Even'? What does that mean?",
	L"Yeah. I'm 100%% with $VICTIM$ on this.",
	L"",
	L"",
	L"$VICTIM$ likes what $CAUSE$ has to say about $VICTIM_GENDER$. What do you do?",
	L"Don't let it go to your head.",
	L"Hey, don't forget about me!",
	L"Apparently, sometimes even $CAUSE$ is deemed important...",
	L"Do I smell trouble here??",
	L"This is pointless! Discuss that in private, but not now.",
	L"$VICTIM$! $CAUSE$! Less talk, more action, please!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_AGAINST_ENEMY
	L"Yeah, $CAUSE$ saw you do it!",
	L"",
	L"",
	L"No I did not!",
	L"And we won't be quiet about it, no sir!",
	L"",
	L"",
	L"$VICTIM$ likes what $CAUSE$ said about the others. What do you do?",
	L"I don't think so...",
	L"Yeah, totally!",
	L"Hu? You said you did.",
	L"Yeah, don't twist what happened!",
	L"Who cares? We have a job to do.",
	L"If you ladies keep this on, we're going to have a real problem soon.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FOR_ENEMY
	L"I can't believe you wouldn't agree with me on this, $CAUSE$.",
	L"",
	L"",
	L"It's because you're wrong, that's why.",
	L"I'm surprised too, but that's how it is.",
	L"",
	L"",
	L"$VICTIM$ does not like $CAUSE$ siding with the others. What do you do?",
	L"Ugh. What now...",
	L"Hum. Never saw that coming.",
	L"Oh come down from your high horse, $CAUSE$.",
	L"Yeah, you are wrong, $VICTIM$!",
	L"Can I shut down squad radio, so I don't have to listen to you?",
	L"Yes, very melodramatic. How is any of this relevant?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_REASON_GOOD
	L"Yeah... you're right, $CAUSE$. Peace?",
	L"",
	L"",
	L"No. I won't let this go.",
	L"Hmm... ok!",
	L"",
	L"",
	L"$VICTIM$ appreciates $CAUSE$'s conflict resolution. What do you do?",
	L"You're not getting away this easy.",
	L"Glad you guys are not angry anymore.",
	L"Oh come on. $VICTIM$ is letting it go, what's your issue now?",
	L"You tell 'em, $CAUSE$!",
	L"*Sigh* Shake hands or whatever you people do, and then back to business.",
	L"Yes, please. Discuss your petty grievings in full detail, we are all DYING to hear it.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_REASON_BAD
	L"No. This is a matter of principle.",
	L"",
	L"",
	L"As if you had any to start with.",
	L"Suit yourself then..",
	L"",
	L"",
	L"$VICTIM$ does not like $CAUSE$'s appeal. What do you do?",
	L"You're just asking for trouble, right?",
	L"Guess you're not getting away that easy, $CAUSE$.",
	L"Don't be so flippant.",
	L"Who needs principles anyway?",
	L"Now that this is out of the way, perhaps we could continue?",
	L"Shut up, both of you!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_GOOD
	L"Alright alright. Jeez. I'm over it, okay?",
	L"",
	L"",
	L"Cut it, drama queen.",
	L"As long as it does not happen again.",
	L"",
	L"",
	L"$VICTIM$ was reined in by $CAUSE$. What do you do?",
	L"No reason to be so stiff about it.",
	L"Yeah, keep it down, will ya?",
	L"Pah. You're the one making all the fuss about it...",
	L"Yeah, drop that attitude, $VICTIM$.",
	L"*Sigh* More of this?",
	L"Why am I even listening to you people...",
	L"",
	L"",
	L"",

	// OPINIONEVENT_SOLVECONFLICT_AGGRESSIVE_BAD
	L"Who do you think you are, $CAUSE$? No, I won't be quiet about this!",
	L"",
	L"",
	L"I'm the one who tells you to shut up! I'm your superior, $VICTIM$!",
	L"The two of us are going to have real problem soon, $VICTIM$.",
	L"",
	L"",
	L"$VICTIM$ did not take $CAUSE$'s words of action well. What do you do?",
	L"Pfft. Don't make a fuss out of it.",
	L"Yeah, you won't boss us around anymore!",
	L"You are certainly nobodies superior!",
	L"Not sure about that, but yep!",
	L"Hey. Hey! Both of you, cut it out! What are you doing?",
	L"If anybody is superior here, then that's me... and I'm ordering you to stand down!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DISEASE_DISGUSTING
	L"Ewww! $CAUSE$ is sick! Get away from me, that looks disgusting!",
	L"",
	L"",
	L"Oh yeah? Back off, before I cough on you!",
	L"It really is. I... *cough* don't feel so well...",
	L"",
	L"",
	L"$VICTIM$ is offended by $CAUSE$ diseases. What do you do?",
	L"Stop behaving like a first grader. We need to get $CAUSE$ to a doctor!",
	L"This does look unhealthy. That better not be contagious!",
	L"Stop it!  We don't need more of whatever it is you have!",
	L"Yeah, there's nothing you can do against this stuff, right?",
	L"The important thing is to get $CAUSE$ to a doctor, and to make sure $CAUSE_GENDER$ doesn't infect anybody else.",
	L"Stop whining! $CAUSE$, get that treated, and the rest of you, back to business!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_DISEASE_TREATMENT
	L"Thanks, $CAUSE$. I'm already feeling better.",
	L"",
	L"",
	L"How did you get that in the first place? Did you forget to wash your hands again?",
	L"No problem, we can't have you running around coughing blood, right? Riiight?",
	L"",
	L"",
	L"$VICTIM$ has treated $CAUSE$'s diseases. What do you do?",
	L"Where did you get that stuff from in the first place?",
	L"Great. Are you sure it's fully treated now?",
	L"The important thing is that it's gone now... It is, right?",
	L"I told you people before... this country a dirty place, so beware of what you touch.",
	L"We have to be careful. The army isn't the only thing that wants us dead.",
	L"Great. Everything done? Then let's get back to shooting stuff!",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BRUTAL_GOOD
	L"Nice one, $CAUSE$!",
	L"",
	L"",
	L"Whoa. Are you really getting off on that?",
	L"Now THIS is action!",
	L"",
	L"",
	L"$VICTIM$ applauds $CAUSE$'s excessive violence. Does that seem good to you?",
	L"This isn't a video game, kid...",
	L"That was so wicked!",
	L"What are you apologizing for? That was awesome!",
	L"You are sick, $VICTIM$.",
	L"Killing them is enough. No need to make a show out of it.",
	L"Cross us, and this is what you get. Waste the rest of these guys.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BRUTAL_BAD
	L"Dammit, $CAUSE$, your supposed to kill them, not evaporate them!",
	L"",
	L"",
	L"Is there a difference?",
	L"Oops. This thing is powerful!",
	L"",
	L"",
	L"$VICTIM$ is appalled by $CAUSE$'s excessive violence. What do you think?",
	L"Don't tell me you've never seen blood before...",
	L"That was a bit excessive...",
	L"Does that mean you aren't even remotely familiar with your gun?",
	L"On the plus side, I doubt this guy is going to complain.",
	L"Don't waste ammunition, $CAUSE$.",
	L"They put up a fight, we put them in a bag. End of story.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_TEACHER
	L"Thanks for giving me a few pointers, $CAUSE$. I've learned a lot from you.",
	L"",
	L"",
	L"About that... you still have a lot to learn, $VICTIM$.",
	L"You're welcome!",
	L"",
	L"",
	L"$CAUSE$ is unimpressed by $VICTIM$'s progress. What do you think?",
	L"At some point you'll have to do on your own though.",
	L"Yeah, you better stick to $CAUSE$.",
	L"Nah, $VICTIM_GENDER$ is doing fine.",
	L"Yes, $VICTIM_GENDER$ still needs training.",
	L"This training is a good preparation, keep up the good work.",
	L"We need everybody at full capacity if we're going to win this campaign, so keep it up.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BESTCOMMANDEREVER
	L"Yeah! Take that, losers! That was a mighty fine strategy, $CAUSE$!",
	L"",
	L"",
	L"I couldn't have done it without you people.",
	L"Well, I do have my moments.",
	L"",
	L"",
	L"$CAUSE$ praises $VICTIM$'s leadership. What's your opinion?",
	L"Well... it's not like $CAUSE_GENDER$ pulled that all by $CAUSE_PRONOUN$self...",
	L"Agreed. $CAUSE$ is a fine squadleader.",
	L"It's alright. You deserve this.",
	L"You did everything right, $CAUSE$. Good work!",
	L"Yeah. We're turning into quite the outfit, aren't we?",
	L"We might have won this battle, but the war is far from over. We'll have to repeat victories like this.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_SAVIOUR
	L"Wow, that was close. Thanks, $CAUSE$, I owe you!",
	L"",
	L"",
	L"Don't mention it.",
	L"You'd do the same for me.",
	L"",
	L"",
	L"$CAUSE$ saved $VICTIM$'s life. What's your opinion?",
	L"Pff, that guy was dead anyway.",
	L"How bad is it, $VICTIM$? Can you still fight?",
	L"Then I will. Good job!",
	L"Yeah, I was worried there for a moment.",
	L"Good job, but let's focus on ending this first, okay?",
	L"When you're done hugging, could we go back to the issue at hand? You know, the guys shooting at us?",
	L"",
	L"",
	L"",

	// OPINIONEVENT_FRAGTHIEF
	L"Hey, I had him in my sights! That guy was MINE!",
	L"",
	L"",
	L"What? Are you nuts? We're in the middle of a firefight, and you set up a killcount?",
	L"What. Then where's my target?",
	L"",
	L"",
	L"$VICTIM$ is angry with $CAUSE$ for stealing their kill. What's your take on this?",
	L"This isn't some videogame, you moron. Nobody gives a shit about your godamn killcount.",
	L"Yeah, I hate it when people don't stick to their firing lane.",
	L"Stick to shooting whom you're supposed to, $CAUSE$.",
	L"Jeez. This feels like nineties videogaming. What's next, $VICTIM_GENDER$'ll accuse the enemy of camping?",
	L"You can sort this out later, but right now, everybody make sure we're not missing any angle.",
	L"Just stick to your firing sector, kill 'em all, and there'll be plenty of kills for everybody.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_ASSIST
	L"Boom! We sure took care of that guy.",
	L"",
	L"",
	L"Well, it was mostly me, but you did help too.",
	L"Yup. Ready for the next one.",
	L"",
	L"",
	L"$CAUSE$ and $VICTIM$ brought down an enemy. Any comment?",
	L"If you weren't such a bad shot, $CAUSE$ wouldn't have to finish your job.",
	L"It takes several people to take them down? Jeez, what kind of body armour do they have?",
	L"Blah blah, everybody look at me. $VICTIM$ is such a showoff.",
	L"Hehe, they've got no chance.",
	L"Hmm. We might need more firepower if it takes several of us to take those guys down, but good work anyway.",
	L"I guess you get to share what he had. $CAUSE$, you may draw first.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_BATTLE_TOOK_PRISONER
	L"Good thinking. We've already won, no need for more senseless slaughter.",
	L"",
	L"",
	L"We'll see about that... I won't hesitate to use force against them if they resist.",
	L"Yeah. Perhaps these guys have some intel we can use.",
	L"",
	L"",
	L"The rest of the enemy team surrendered to $CAUSE$. Now what?",
	L"No offense, but if you cannot handle death, $CAUSE$, perhaps this might not be the best job for you?",
	L"Good job, $CAUSE$. Makes our job hell of a lot easier.",
	L"Hey! Cut the crap. They already surrendered.",
	L"Yeah, you can't trust these guys, they're totally reckless.",
	L"We should move these guys to a prison ASAP. I'm sure they know what the army is up to.",
	L"Pah. I'd have preferred from wasting these losers. They'll just slow us down.",
	L"",
	L"",
	L"",

	// OPINIONEVENT_CIV_ATTACKER
	L"Watch it, $CAUSE$! They are on our side!",
	L"",
	L"",
	L"That's just a scratch, they'll live.",
	L"Oops.",
	L"",
	L"",
	L"$VICTIM$ is angry: $CAUSE$ injured a civilian. What do you do?",
	L"Well, this can happen. As long as they live it's okay.",
	L"This won't look good in the after-action report.",
	L"What are you doing? Watch it, $CAUSE$!",
	L"Don't worry. Happens to me too all the time.",
	L"Maintain fire discipline! After this is over, $VICTIM$ and $CAUSE$ will take care of the wounded.",
	L"If $CAUSE$ had intended it, they would be dead, so no worries here.",
	L"",
	L"",
	L"",
};


STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_DENY[] =
{
	L"What?!",
	L"No!",
	L"That is false!",
	L"That is not true!",

	L"Lies, lies, lies. Nothing but lies!",
	L"Liar!",
	L"Traitor!",
	L"You watch your mouth!",

	L"This is none of your business.",
	L"Who ever invited you?",
	L"Nobody asked for your opinion, $INTERJECTOR$.",
	L"You stay away from me.",

	L"Why are you all against me?",
	L"Why are you against me, $INTERJECTOR$?",
	L"I knew it! $VICTIM$ and $INTERJECTOR$ are in cahoots!",
	L"Not listening...!",

	L"I hate this psycho circus.",
	L"I hate this freak show.",
	L"Back off!",
	L"Lies, lies, lies...",

	L"No way!",
	L"So not true.",
	L"That is so not true.",
	L"I know what I saw.",

	L"I don't know what $INTERJECTOR_GENDER$ is talking off.",
	L"Don't listen to $INTERJECTOR_PRONOUN$!",
	L"Nope.",
	L"You are mistaken.",
};

STR16	szDynamicDialogueText_DOST_VICTIM_TO_INTERJECTOR_AGREE[] =
{
	L"I knew you'd back me, $INTERJECTOR$",
	L"I knew $INTERJECTOR$ would back me.",
	L"What $INTERJECTOR$ said.",
	L"What $INTERJECTOR_GENDER$ said.",

	L"Thanks, $INTERJECTOR$!",
	L"Once again I'm right!",
	L"See, $CAUSE$? I am right!",
	L"Once again $SPEAKER$ is right!",

	L"Aye.",
	L"Yup.",
	L"Yep",
	L"Yes.",

	L"Indeed.",
	L"True.",
	L"Ha!",
	L"See?",

	L"Exactly!",
};

STR16	szDynamicDialogueText_DOST_SIDEWITH_VICTIM[] =
{
	L"That's right!",
	L"Indeed!",
	L"Exactly that.",
	L"$CAUSE$ does that all the time.",

	L"$VICTIM$ is right!",
	L"I was gonna' point that out, too!",
	L"What $VICTIM$ said.",
	L"That's our $CAUSE$!",

	L"Yeah!",
	L"Now THIS is going to be interesting...",
	L"You tell'em, $VICTIM$!",
	L"Agreeing with $VICTIM$ here...",

	L"Classic $CAUSE$.",
	L"I couldn't have said it better myself.",
	L"That is exactly what happened.",
	L"Agreed!",

	L"Yup.",
	L"True.",
	L"Bingo.",
};

STR16	szDynamicDialogueText_DOST_SIDEWITH_CAUSE[] =
{
	L"Now wait a minute...",
	L"Wait a sec, that's not what right...",
	L"What? No.",
	L"That is not what happened.",

	L"Hey, stop blaming $CAUSE$!",
	L"Oh shut up, $VICTIM$!",
	L"Nonono, you got that wrong.",
	L"Whoa. Why so stiff all of a sudden, $VICTIM$?",

	L"And I suppose you never did, $VICTIM$?",
	L"Hmmmm... no.",
	L"Great. Let's have an argument. It's not like we have other things to do...",
	L"You are mistaken!",

	L"You are wrong!",
	L"Me and $CAUSE$ would never do such a thing.",
	L"Nah, can't be.",
	L"I don't think so.",

	L"Why bring that up now?",
	L"Really, $VICTIM$? Is this necessary?",
};

STR16	szDynamicDialogueText_DOST_INTERJECTOR_DIALOGUESELECTION_SHORTTEXT[] =
{
	L"Keep silent",
	L"Support $VICTIM$",
	L"Support $CAUSE$",
	L"Appeal to reason",
	L"Shut them up",
};

STR16	szDynamicDialogueText_GenderText[] =
{
	L"he",
	L"she",
	L"him",
	L"her",
};

STR16	szDiseaseText[] =
{
	L" %s%d%% agility stat\n",
	L" %s%d%% dexterity stat\n",
	L" %s%d%% strength stat\n",
	L" %s%d%% wisdom stat\n",
	L" %s%d%% effective level\n",

	L" %s%d%% APs\n",
	L" %s%d maximum breath\n",
	L" %s%d%% strength to carry items\n",
	L" %s%2.2f life regeneration/hour\n",
	L" %s%d need for sleep\n",
	L" %s%d%% water consumption\n",
	L" %s%d%% food consumption\n",

	L"%s was diagnosed with %s!",
	L"%s is cured of %s!",

	L"Diagnosis",
	L"Treatment",
	L"Burial",	// TODO.Translate
	L"Cancel",

	L"\n\n%s (undiagnosed) - %d / %d\n",	// TODO.Translate

	L"High amount of distress can cause a personality split\n",	// TODO.Translate
	L"Contaminated items found in %s' inventory.\n",
	L"Whenever we get this, a new disability is added.\n",	// TODO.Translate

	L"Only one hand can be used.\n",
	L"Only one hand can be used.\nA medical splint was applied to speed up the healing process.\n",
	L"Leg functionality severely limited.\n",
	L"Leg functionality severely limited.\nA medical splint was applied to speed up the healing process.\n",
};

STR16	szSpyText[] =
{
	L"Hide",			// TODO.Translate
	L"Get Intel",
};

STR16	szFoodText[] =
{
	L"\n\n|W|a|t|e|r: %d%%\n",
	L"\n\n|F|o|o|d: %d%%\n",

	L"max morale altered by %s%d\n",
	L" %s%d need for sleep\n",
	L" %s%d%% breath regeneration\n",
	L" %s%d%% assignment efficiency\n",
	L" %s%d%% chance to lose stats\n",
};

STR16 szIMPGearWebSiteText[] =	// TODO.Translate
{
	// IMP Gear Entrance
	L"How should gear be selected?",
	L"Old method: Random gear according to your choices",
	L"New method: Free selection of gear",
	L"Old method",
	L"New method",

	// IMP Gear Entrance
	L"I.M.P. Equipment",
	L"Additional Cost: %d$ (%d$ prepaid)",	// TODO.Translate
};

STR16 szIMPGearDropDownText[] =
{
	L"Select LBE vest",
	L"Select LBE combat pack",
	L"Select LBE backpack",
	L"Select LBE holster",
	L"Select LBE holster",

	L"Select main gun",
	L"Select ammo",
	L"Select sidearm",
	L"Select additional ammo",
	L"Select melee weapon",

	L"Select helmet",
	L"Select vest",
	L"Select pants",
	L"Select face gear",
	L"Select face gear",

	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
};

STR16 szIMPGearDropDownNoneText[] =
{
	L"No LBE vest",
	L"No LBE combat pack",
	L"No LBE backpack",
	L"No LBE holster",
	L"No LBE holster",

	L"No gun",
	L"No ammo",
	L"No gun",
	L"No ammo",
	L"No weapon",

	L"No helmet",
	L"No vest",
	L"No pants",
	L"No face gear",
	L"No face gear",

	L"No additional items",
	L"No additional items",
	L"No additional items",
	L"No additional items",
	L"No additional items",
	L"No additional items",
	L"No additional items",
};

STR16 szMilitiaStrategicMovementText[] =
{
	L"We cannot relay orders to this sector, militia command not possible.",
	L"Unassigned",
	L"Group No.",
	L"Next",

	L"ETA",
	L"Group %d (new)",
	L"Group %d",
	L"Final",

	L"Volunteers: %d (+%5.3f)",
};

STR16 szEnemyHeliText[] =	// TODO.Translate
{
	L"Enemy helicopter shot down in %s!",
	L"We... uhm... currently don't control that site, commander...",
	L"The SAM does not need maintenance at the moment.",
	L"We've already ordered the repair, this will take time.",

	L"We do not have enough resources to do that.",
	L"Repair SAM site? This will cost %d$ and take %d hours.",
	L"Enemy helicopter hit in %s.",
	L"%s fires %s at enemy helicopter in %s.",

	L"SAM in %s fires at enemy helicopter in %s.",
};

STR16 szFortificationText[] =
{
	L"No valid structure selected, nothing added to build plan.",
	L"No gridno found to create items in %s - created items are lost.",
	L"Structures could not be built in %s - people are in the way.",
	L"Structures could not be built in %s - the following items are required:",

	L"No fitting fortifications found for tileset %d: %s",
	L"Tileset %d: %s",
};

STR16	szMilitiaWebSite[] =
{
	// main page
	L"Militia",
	L"Militia Forces Overview",
};

STR16 szIndividualMilitiaBattleReportText[] =
{
	L"Took part in Operation %s",
	L"Recruited on Day %d, %d:%02d in %s",
	L"Promoted on Day %d, %d:%02d",
	L"KIA, Operation %s",

	L"Lightly wounded during Operation %s",
	L"Heavily wounded during Operation %s",
	L"Critically wounded during Operation %s",
	L"Valiantly fought in Operation %s",

	L"Hired from Kerberus on Day %d, %d:%02d in %s",
	L"Defected to us on  Day %d, %d:%02d in %s",
	L"Contract terminated on Day %d, %d:%02d",
};

STR16 szIndividualMilitiaTraitRequirements[] =
{
	L"HP",
	L"AGI",
	L"DEX",
	L"STR",

	L"LDR",
	L"MRK",
	L"MEC",
	L"EXP",

	L"MED",
	L"WIS",
	L"\nMust have regular or elite rank",
	L"\nMust have elite rank",

	L"\n\n|F|u|l|f|i|l|l|e|d |R|e|q|u|i|r|e|m|e|n|t|s",
	L"\n\n|F|a|i|l|e|d |R|e|q|u|i|r|e|m|e|n|t|s",
	L"\n%d %s",
	L"\nBasic version of trait",

	L" (Expert)"
};

STR16	szIdividualMilitiaWebsiteText[] =
{
	L"Operations",
	L"Are you sure you want to release %s from your service?",
	L"HP ratio: %3.0f %% Daily Wage: %3d$ Age: %d years",
	L"Daily Wage: %3d$ Age: %d years",

	L"Kills: %d Assists: %d",
	L"Status: Deceased",
	L"Status: Fired",
	L"Status: Active",

	L"Terminate Contract",
	L"Personal Data",
	L"Service Record",
	L"Inventory",

	L"Militia name",
	L"Sector name",
	L"Weapon",
	L"HP ratio: %3.1f%%%%%%%",

	L"%s is not active in the currently loaded sector.",
	L"%s has been promoted to regular militia",
	L"%s has been promoted to elite militia",
	L"Status: Deserted",	// TODO:Translate
};

STR16	szIdividualMilitiaWebsiteFilterText_Dead[] =
{
	L"All statuses",
	L"Deceased",
	L"Active",
	L"Fired",
};

STR16	szIdividualMilitiaWebsiteFilterText_Rank[] =
{
	L"All ranks",
	L"Green",
	L"Regular",
	L"Elite",
};

STR16	szIdividualMilitiaWebsiteFilterText_Origin[] =
{
	L"All origins",
	L"Rebel",
	L"PMC",
	L"Defector",
};

STR16	szIdividualMilitiaWebsiteFilterText_Sector[] =
{
	L"All sectors",
};

STR16	szNonProfileMerchantText[] =
{
	L"Merchant is hostile and does not want to trade.",
	L"Merchant is in no state to do business.",
	L"Merchant won't trade during combat.",
	L"Merchant refuses to interact with you.",
};

STR16	szWeatherTypeText[] =	// TODO.Translate
{
	L"normal",
	L"rain",
	L"thunderstorm",
	L"sandstorm",

	L"snow",
};

STR16	szSnakeText[] =
{
	L"%s evaded a snake attack!",
	L"%s was attacked by a snake!",
};

STR16	szSMilitiaResourceText[] =
{
	L"Converted %s into resources",
	L"Guns: ",
	L"Armour: ",
	L"Misc: ",

	L"There are no volunteers left for militia!",
	L"Not enough resources to train militia!",
};

STR16	szInteractiveActionText[] =
{
	L"%s starts hacking.",
	L"%s accesses the computer, but finds nothing of interest.",
	L"%s is not skilled enough to hack the computer.",
	L"%s reads the file, but learns nothing new.",

	L"%s can't make sense out of this.",
	L"%s couldn't use the watertap.",
	L"%s has bought a %s.",
	L"%s doesn't have enough money. That's just embarassing.",

	L"%s drank from water tap",
	L"This machine doesn't seem to be working.",	// TODO.Translate
};

STR16	szLaptopStatText[] =	// TODO.Translate
{
	L"threaten effectiveness %d\n",
	L"leadership %d\n",
	L"approach modifier %.2f\n",
	L"background modifier %.2f\n",

	L"+50 (other) for assertive\n",
	L"-50 (other) for malicious\n",
	L"Good Guy",
	L"%s eschews excessive violence and will refuse to attack non-hostiles.",

	L"Friendly approach",
	L"Direct approach",
	L"Threaten approach",
	L"Recruit approach",

	L"%s learns with normal speed.",	// TODO.Translate
	L"%s does not learn at all.",
	L"%s unlearns his skills.",
	L"%s learns with 3/4 speed.",

	L"%s learns with 1/2 speed.",
	L"%s learns with 1/4 speed.",
};

STR16	szGearTemplateText[] =	// TODO.Translate
{
	L"Enter Template Name",
	L"Not possible during combat.",
	L"Selected mercenary is not in this sector.",
	L"%s is not in that sector.",
	L"%s could not equip %s.",
	L"We cannot attach %s (item %d) as that might damage items.",	// TODO.Translate
};

STR16	szIntelWebsiteText[] =
{
	L"Recon Intelligence Services",
	L"Your need to know base",
	L"Information Requests",
	L"Information Verification",

	L"About us",
	L"You have %d Intel.",
	L"We currently have information on the following items, available in exchange for intel as usual:",
	L"There is currently no other information available.",

	L"%d Intel - %s",
	L"We can provide aerial reconnaissance of a map region. This will last until  %02d:00.",
	L"Buy data - 50 intel",
	L"Buy detailed data - 70 Intel",

	L"Select map region on which you want info on:",

	L"North-west",
	L"North-north-west",
	L"North-north-east",
	L"North-east",

	L"West-north-west",
	L"Center-north-west",
	L"Center-north-east",
	L"East-north-east",

	L"West-south-west",
	L"Center-south-west",
	L"Center-south-east",
	L"East-south-east",

	L"South-west",
	L"South-south-west",
	L"South-south-east",
	L"South-east",

	// about us
	L"On the 'Information Requests' page, you can buy information on various enemy targets for intel.",
	L"This includes information on enemy patrols & garrisons, noteworthy persons of interests, enemy aircraft etc..",
	L"Some of that information may be of temporary nature.",
	L"On the 'Information Verification' page, you can upload data you took of significant intelligence.",

	L"We will verify the data (this process usually takes several hours) and compensate you accordingly.",
	L"Note that you will reveive less intel if outside conditions have rendered the information less useful (e.g. the person in question having died since the data was acquired).",

	// sell info
	L"You can upload the following facts:",
	L"Previous",
	L"Next",
	L"Upload",

	L"You have already received compensation for the following:",
	L"You have nothing to upload.",
};

STR16	szIntelText[] =
{
	L"No more enemies present, %s is no longer in hiding!",
	L"%s has been discovered and goes into hiding for %d hours.",
	L"%s has been discovered, going to sector!",
	L"Enemy general present\n",

	L"Terrorist present\n",
	L"%s on %02d:%02d\n",
	L"No data found",
	L"Data no longer eligible.",

	L"Whereabouts of a high-ranking officer of the royal army.",
	L"Flight plans of an airforce helicopter.",
	L"Coordinates of a recently imprisoned member of your force.",
	L"Location of a high-value fugitive.",

	L"Information on possible bloodcat attacks against settlements.",
	L"Time and place of possible zombie attacks against settlements.",
	L"Information on planned bandit raids.",
};

STR16	szChatTextSpy[] =
{
	L"... so imagine my surprise when suddenly...",
	L"... and did I ever tell you the story of that one time...",
	L"... so, in conclusion, the colonel decided...",
	L"... tell you, they did not see that coming...",

	L"... so, without further consideration...",
	L"... but then SHE said...",
	L"... and, speaking of llamas...",
	L"... there I was, in the middle of the dustbowl, when...",

	L"... and let me tell, those things chafe...",
	L"... you should have seen his face...",
	L"... which wasn't the last of what we saw of them...",
	L"... which reminds me, my grandmother used to say...",

	L"... who, by the way, is a total berk...",
	L"... also, the roots were off by a margin...",
	L"... and I was like, 'Back off, heathen!'...",
	L"... at that point the vicars were in oben rebellion...",

	L"... not that I would've minded, you know, but...",
	L"... if not for that ridiculous hat...",
	L"... besides, it wasn't his favourite leg anyway...",
	L"... even though the ships were still watertight...",

	L"... aside from the fact that giraffes can't do that...",
	L"... totally wasted that fork, mind you...",
	L"... and no bakery in sight. After that...",
	L"... even though regulations are clear in that regard...",
};

STR16	szChatTextEnemy[] =
{
	L"Whoa. I had no idea!",
	L"Really?",
	L"Uhhhh....",
	L"Well... you see, uhh...",

	L"I am not entirely sure that...",
	L"I... well...",
	L"If I could just...",
	L"But...",

	L"I don't mean to intrude, but...",
	L"Really? I had no idea!",
	L"What? All of it?",
	L"No way!",

	L"Haha!",
	L"Whoa, the guys are not going to believe me!",
	L"... yeah, just...",
	L"That's just like the gypsy woman said!",

	L"... yeah, is that why...",
	L"... hehe, talk about refurbishing...",
	L"... yeah, I guess...",
	L"Wait. What?",

	L"... wouldn't have minded seeing that...",
	L"... now that you mention it...",
	L"... but where did all the chalk go...",
	L"... had never even considered that...",
};

STR16	szMilitiaText[] =
{
	L"Train new militia",
	L"Drill militia",
	L"Doctor militia",
	L"Cancel",
};

STR16	szFactoryText[] =	// TODO.Translate
{
	L"%s: Production of %s switched off as loyalty is too low.",
	L"%s: Production of %s switched off due to insufficient funds.",
	L"%s: Production of %s switched off as it requires a merc to staff the facility.",
	L"%s: Production of %s switched off due to required items missing.",
	L"Item to build",

	L"Preproducts",	// 5
	L"h/item",
};

STR16	szTurncoatText[] =
{
	L"%s now secretly works for us!",
	L"%s is not swayed by our offer. Suspicion against us rises...",
	L"Suspicion against us is high. We should stop trying to turn more soldiers to our side and lay low for a while.",
	L"Recruit approach (%d)",
	L"Use seduction (%d)",

	L"Bribe ($%d) (%d)",	// 5
	L"Offer %d intel (%d)",
	L"How to convince the soldier to join your forces?",
	L"Do it",
	L"%d turncoats present",
};

// rftr: better lbe tooltips
// TODO.Translate
STR16 gLbeStatsDesc[14] =
{
	L"MOLLE Space Available:",
	L"MOLLE Space Required:",
	L"MOLLE Small Slot Count:",
	L"MOLLE Medium Slot Count:",
	L"MOLLE Pouch Size: Small",
	L"MOLLE Pouch Size: Medium",
	L"MOLLE Pouch Size: Medium (Hydration)",
	L"Thigh Rig",
	L"Vest",
	L"Combat Pack",
	L"Backpack", // 10
	L"MOLLE Pouch",
	L"Compatible backpacks:",
	L"Compatible combat packs:",
};

STR16 szRebelCommandText[] = // TODO.Translate
{
	L"Arulco Rebel Command - National Overview",
	L"Arulco Rebel Command - Regional Overview",
	L"Switch to Regional Overview",
	L"Switch to National Overview",
	L"Supplies:",
	L"Incoming Supplies",
	L"/day",
	L"Current Directive",
	L"Improve Directive ($%d)",
	L"Improving the selected directive will cost $%d. Confirm expenditure?",
	L"Insufficient funds.",
	L"New directive will take effect at 00:00.",
	L"Militia Overview",
	L"Green:",
	L"Regular:",
	L"Elite:",
	L"Projected Daily Total:",
	L"Volunteer Pool:",
	L"Resources Available:",
	L"Guns:",
	L"Armour:",
	L"Misc:",
	L"Training Cost:",
	L"Upkeep Cost Per Soldier Per Day:",
	L"Training Speed Bonus:",
	L"Combat Bonuses:",
	L"Physical Stats Bonus:",
	L"Marksmanship Bonus:",
	L"Upgrade Stats ($%d)",
	L"Upgrading militia stats will cost $%d. Confirm expenditure?",
	L"Region:",
	L"Next",
	L"Previous",
	L"Administration Team:",
	L"None",
	L"Active",
	L"Inactive",
	L"Loyalty:",
	L"Maximum Loyalty:",
	L"Deploy Administration Team (%d supplies)",
	L"Reactivate Administration Team (%d supplies)",
	L"It is currently not safe to deploy an administration team to this region. You must establish a foothold by controlling at least one town sector first.",
	L"No regional actions available in Omerta.",
	L"Administration teams can be deployed to other regions once you capture at least one town sector. Once active, they will be able to expand your influence and power in the region. However, they will need supplies to operate and enact policies.",
	L"Be mindful of where you choose to direct supplies. Enacting regional policies will increase supply costs for other policies in the same region and nationally (to a lesser extent).",
	L"Administrative Actions:",
	L"Establish %s",
	L"Improve %s",
	L"Current Tier: %d",
	L"Taking any administrative action in this region will cost %d supplies.",
	L"Dead drop intel gain: %d",
	L"Smuggler supply gain: %d",
	L"A small group of militia from a nearby safehouse have joined the battle!",
	L"[A.R.C. WEBSITE AVAILABLE] With the delivery of food and basic supplies to Omerta, you have convinced the rebels that you're here to make an impact. You have been granted access to the command system they've been working on, which is now available through your laptop.",
	L"It is currently not safe to reactivate the administration team here. Recapture a town sector first.",
	L"Mine raid successful. Stole $%d.",
	L"Insufficient Intel to create turncoats!",
	L"Change Admin Action",
	L"Cancel",
	L"Confirm",
	L"<",
	L">",
	L"Changing this Admin Action will cost $%d and reset its tier. Confirm expenditure?",
};

STR16 szRebelCommandHelpText[] = // TODO.Translate
{
	L"|S|u|p|p|l|i|e|s\n \nFood, water, medical supplies, weapons, and anything else that\nthe rebels might find useful. Supplies are obtained automatically\nby the rebels.",
	L"|I|n|c|o|m|i|n|g |S|u|p|p|l|i|e|s\n \nEach day, the rebels will gather supplies on their own. As you\ntake over more towns, the amount of supplies they will be\nable to find per day will increase.",
	L"|C|u|r|r|e|n|t |D|i|r|e|c|t|i|v|e\n \nYou can choose how the rebels will prioritise their strategic\nobjectives. New directives will become available as you make\nprogress.",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |T|e|a|m\n \nOnce deployed, an admin team is responsible for handling the\nday-to-day affairs of the region. This includes supporting\nlocals, creating rebel propaganda, establishing regional\npolicies, and more.",
	L"|L|o|y|a|l|t|y\n \nThe effectiveness of many Administrative Actions depends on\nthe region's loyalty to your cause. It is in your best interest\nto raise loyalty as high as possible.",
	L"|M|a|x|i|m|u|m |L|o|y|a|l|t|y\n \nYou will need to convince the locals to fully trust you. This\ncan be done by creating a supply line to them, showing that\nyou intend to improve their quality of life.",
	L"|G|r|a|n|t |S|u|p|p|l|i|e|s\n \nSend supplies to the admin team here and allow them to use them\nas needed. This will increase the region's loyalty by a small amount\neach time you do this. However, doing this will slightly increase\nthe cost of enacting regional policies.",
	L"This Admin Action applies its bonus to town sectors only.", //TODO.Translate
	L"This Admin Action applies its bonus to town sectors, and\nsectors immediately adjacent to them.",
	L"This Admin Action applies its bonus to town sectors, one\nsector away at Tier 1, and up to two sectors away at Tier 2.",
	L"This Admin Action applies its bonus to town sectors, up to\ntwo sectors away at Tier 1, and up to three sectors away at Tier 2.",
};

// follows a specific format:
// x:	"Admin Action Button Text",
// x+1:	"Admin action description text",
STR16 szRebelCommandAdminActionsText[] = // TODO.Translate
{
	L"Supply Line",
	L"Distribute much-needed supplies amongst the local population. Increases maximum regional loyalty.",
	L"Rebel Radio",
	L"Begin broadcasting rebel public radio in the region. The town gains loyalty daily.",
	L"Safehouses",
	L"Construct rebel safehouses in the countryside, far from prying eyes. Bonus militia may join you in combat when operating in this region.",
	L"Supply Disruption",
	L"The rebels will try to disrupt enemy movements in the area by targetting their supplies. Enemies fighting in this region are weaker.",
	L"Scout Patrols",
	L"Start regular scout patrols to monitor hostile activity in the area. Enemy groups will be spotted further from town.",
	L"Dead Drops",
	L"Set up dead drops for rebel scouts and infiltrators to deliver their intel. Provides daily intel.",
	L"Smugglers",
	L"Enlist the aid of smugglers to bring in supplies for the rebels. Provides an unreliable daily supply boost.",
	L"Militia Warehouses",
	L"Construct warehouses in remote areas, allowing the rebels to stockpile weapons for the militia. Provides daily militia resources.",
	L"Regional Taxes",
	L"Collect money from the locals to assist your efforts. This is a permanent action. Increases daily income, but regional loyalty falls daily.",
	L"Civilian Aid",
	L"Assign some rebels to directly assist and support civilians in the area. Increases daily volunteer pool growth.",
	L"Merc Support",
	L"Set up facilities to directly support your mercs assigned in the town. Increases the effectiveness of merc assignments (doctoring, repairing, militia training, etc).",
	L"Mining Policy",
	L"Import better equipment and work with the town's miners to create more balanced and efficient shift schedules. Increases the town's mine income.",
	L"Pathfinders",
	L"The locals guide your teams through shortcuts in the area. Reduces on-foot travel time in the region.",
	L"Harriers",
	L"The rebels harass nearby enemy groups, significantly increasing their travel time in the region.",
	L"Fortifications",
	L"Set up killzones and defensive positions. Friendly forces are more effective when fighting in this town. Autoresolve only.",
};

// follows a specific format:
// x:	"Directive Name",
// x+1:	"Directive Bonus Description",
// x+2:	"Directive Help Text",
// x+3:	"Directive Improvement Button Description",
STR16 szRebelCommandDirectivesText[] = // TODO.Translate
{
	L"Gather Supplies",
	L"Gain an additional %.0f supplies per day.",
	L"Increase daily supply income by amassing supplies from\nsympathetic locals, and seeking aid from international aid groups.",
	L"Improving this directive will increase the amount of supplies that the rebels can gather daily.",
	L"Support Militia",
	L"Reduce militia daily upkeep costs. Militia daily upkeep cost modifier: %.2f.",
	L"The rebels will help out with logistics about the militia\nyou've trained, reducing the strain on your wallet.",
	L"Improving this directive will reduce the daily upkeep costs of your militia.",
	L"Train Militia",
	L"Reduce militia training costs and increase militia training speed. Militia training cost modifier: %.2f. Militia training speed modifier: %.2f.",
	L"The rebels will assist when you are training militia,\nincreasing the efficiency at which you can train them.",
	L"Improving this directive will further reduce training cost and increase training speed.",
	L"Propaganda Campaign",
	L"Town loyalty rises faster. Loyalty gain modifier: %.2f.",
	L"Your victories and feats will be embellished as news reaches\nthe locals.",
	L"Improving this directive will increase how quickly town loyalty rises.",
	L"Deploy Elites",
	L"%.0f elite militia appear in Omerta each day.",
	L"The rebels release a small number of their highly-trained forces to your command.",
	L"Improving this directive will increase the number of militia\nthat appear each day.",
	L"High Value Target Strikes",
	L"Enemy groups are less likely to have specialised soldiers.",
	L"Surgical strikes will be conducted against enemy groups. Officers,\nmedics, radio operators, and other specialists are targetted.",
	L"Improving this directive will make strikes more successful and effective.",
	L"Spotter Teams",
	L"When in combat, approximate enemy locations are revealed in the overhead map (press INSERT button in tactical).",
	L"A small team will be attached to each of your squads, providing\napproximate locations of enemies when in combat.",
	L"Improving this directive will make the locations of unspotted enemies more precise.",
	L"Raid Mines",
	L"Steal some income from mines not under your control. This directive becomes less useful as you claim mines.",
	L"Conduct smash-and-grab raids on hostile mines. While not always\nsuccessful, the raids that do succeed should provide a\nsmall income bump.",
	L"Improving this directive will increase the maximum value of stolen income.",
	L"Create Turncoats",
	L"Create %.0f turncoats in a random enemy group per day. Consumes %.1f Intel per day.",
	L"Convince enemy soldiers to betray their army and work for you\nthrough a combination of bribery, threats, and blackmail.",
	L"Improving this directive will increase the number of soldiers turned daily.",
	L"Draft Civilians",
	L"Gain %.0f volunteers each day. All towns lose some loyalty each day.",
	L"Draft civilians as recruits for militia. The general population\nprobably won't be too happy about it, though. Effectiveness\nincreases as you capture more towns.",
	L"Improving this directive will increase the number of volunteers gained per day.",
};

STR16 szRobotText[] = // TODO: Translate
{
	L"The robot's installed weapon cannot be changed.",
	L"It is not possible to add attachments to the robot's weapon.",
	L"Installed Weapon",
	L"Reserve Ammo",
	L"Targeting Upgrade",
	L"Chassis Upgrade",
	L"Utility Upgrade",
	L"Storage",
	L"No Bonus",
	L"The laser bonuses of this item are applied to the robot.",
	L"The night- and cave-vision bonuses of this item are applied to the robot.",
	L"This kit degrades instead of the robot's weapon.",
	L"The robot's cleaning kit was depleted!",
	L"Mines adjacent to the robot are automatically flagged.",
	L"Periodic X-Ray scans during combat. No batteries required.",
	L"The robot has activated an x-ray scan!",
	L"The robot can use the radio set.",
	L"The robot's chassis is strengthened, giving it better combat performance.",
	L"The camouflage bonuses of this item are applied to the robot.",
	L"The robot is tougher and takes less damage.",
	L"The robot's extra armour plating was destroyed!",
	L"The robot gains the benefit of the %s skill trait.",
};

#endif //ITALIAN
