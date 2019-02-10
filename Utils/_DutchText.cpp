// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("DUTCH")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( DUTCH )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_DutchText_public_symbol(void){;}

#ifdef DUTCH



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
	L"Enter",
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
//	L".38 kal",
//	L"9mm",
//	L".45 kal",
//	L".357 kal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm NAVO",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Raket",
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
//	L".38 kal",
//	L"9mm",
//	L".45 kal",
//	L".357 kal",
//	L"12 gauge",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm N.",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Raket",
//	L"", // dart
//};


CHAR16 WeaponType[][30] =
{
	L"Other",
	L"Pistol",
	L"Machine pistol",
	L"Machine Gun",
	L"Rifle",
	L"Sniper Rifle",
	L"Attack weapon",
	L"Light machine gun",
	L"Shotgun",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Beurt speler",
	L"Beurt opponent",
	L"Beurt beest",
	L"Beurt militie",
	L"Beurt burgers",
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

	L"%s geraakt in hoofd en verliest een intelligentiepunt!",
	L"%s geraakt in de schouder en verliest een handigheidspunt!",
	L"%s geraakt in de borst en verliest een krachtspunt!",
	L"%s geraakt in het benen en verliest een beweeglijkspunt!",
	L"%s geraakt in het hoofd en verliest %d wijsheidspunten!",
	L"%s geraakt in de schouder en verliest %d handigheidspunten!",
	L"%s geraakt in de borst en verliest %d krachtspunten!",
	L"%s geraakt in de benen en verliest %d beweeglijkheidspunten!",
	L"Storing!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"Je versterkingen zijn gearriveerd!",

	// In the following four lines, all %s's are merc names

	L"%s herlaad.",
	L"%s heeft niet genoeg actiepunten!",
	L"%s verricht eerste hulp. (Druk een toets om te stoppen.)",
	L"%s en %s verrichten eerste hulp. (Druk een toets om te stoppen.)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"reliable",
	L"unreliable",
	L"easy to repair",
	L"hard to repair",
	L"much damage",
	L"low damage",
	L"quick fire",
	L"slow fire",
	L"long range",
	L"short range",
	L"light",
	L"heavy",
	L"small",
	L"quick salvo",
	L"no salvo",
	L"large magazine",
	L"small magazine",

	// In the following two lines, all %s's are merc names

	L"%s's camouflage is verdwenen.",
	L"%s's camouflage is afgespoelt.",

	// The first %s is a merc name and the second %s is an item name

	L"Tweede wapen is leeg!",
	L"%s heeft %s gestolen.",

	// The %s is a merc name

	L"%s's wapen vuurt geen salvo.",

	L"Je hebt er al één van die vastgemaakt.",
	L"Samen voegen?",

	// Both %s's are item names

	L"Je verbindt %s niet met %s.",
	L"Geen",
	L"Eject ammo",
	L"Toebehoren",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"%s en %s zijn niet tegelijk te gebruiken.",

	L"Het item dat je aanwijst, kan vastgemaakt worden aan een bepaald item door het in een van de vier uitbreidingssloten te plaatsen.",
	L"Het item dat je aanwijst, kan vastgemaakt worden aan een bepaald item door het in een van de vier uitbreidingssloten te plaatsen. (Echter, het item is niet compatibel.)",
	L"Er zijn nog vijanden in de sector!",
	L"Je moet %s %s nog geven",
	L"kogel doorboorde %s in zijn hoofd!",
	L"Gevecht verlaten?",
	L"Dit samenvoegen is permanent. Verdergaan?",
	L"%s heeft meer energie!",
	L"%s is uitgegleden!",
	L"%s heeft %s niet gepakt!",
	L"%s repareert de %s",
	L"Stoppen voor ",
	L"Overgeven?",
	L"Deze persoon weigert je hulp.",
	L"Ik denk het NIET!",
	L"Chopper van Skyrider gebruiken? Eerst huurlingen TOEWIJZEN aan VOERTUIG/HELIKOPTER.",
	L"%s had tijd maar EEN geweer te herladen",
	L"Beurt bloodcats",
	L"automatic",
	L"no full auto",
	L"accurate",
	L"inaccurate",
	L"no semi auto",
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
	L"There's not enough space for this pocket.", //TODO.Translate

	L"%s has repaired the %s as much as possible.", // TODO.Translate
	L"%s has repaired %s's %s as much as possible.",

	L"%s has cleaned the %s.",	// TODO.Translate
	L"%s has cleaned %s's %s.",

	L"Assignment not possible at the moment",	// TODO.Translate
	L"No militia that can be drilled present.",
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
	L"Pause",
	L"Normal",
	L"5 min",
	L"30 min",
	L"60 min",
	L"6 uur", 
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Team 1",
	L"Team 2",
	L"Team 3",
	L"Team 4",
	L"Team 5",
	L"Team 6",
	L"Team 7",
	L"Team 8",
	L"Team 9",
	L"Team 10",
	L"Team 11",
	L"Team 12",
	L"Team 13",
	L"Team 14",
	L"Team 15",
	L"Team 16",
	L"Team 17",
	L"Team 18",
	L"Team 19",
	L"Team 20",
	L"Dienst", // on active duty
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Voertuig", // in a vehicle
	L"Onderweg", // in transit - abbreviated form
	L"Repareer", // repairing
	L"Radio Scan",	// scanning for nearby patrols	// TODO.Translate
	L"Oefenen", // training themselves  
	L"Militie", // training a town to revolt 
	L"M.Militia", //training moving militia units		// TODO.Translate
	L"Trainer", // training a teammate
	L"Student", // being trained by someone else 
	L"Get Item",	// get items	// TODO.Translate
	L"Staff", // operating a strategic facility			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.)	// TODO.Translate
	L"Rest", // Resting at a facility					// TODO.Translate
	L"Prison",		// Flugente: interrogate prisoners
	L"Dood", // dead
	L"Uitgesc.", // abbreviation for incapacitated
	L"POW", // Prisoner of war - captured
	L"Kliniek", // patient in a hospital 
	L"Leeg",	// Vehicle is empty
	L"Snitch",	// facility: undercover prisoner (snitch)	// TODO.Translate
	L"Propag.",	// facility: spread propaganda
	L"Propag.",	// facility: spread propaganda (globally)
	L"Rumours",	// facility: gather information
	L"Propag.",	// spread propaganda
	L"Rumours",	// gather information
	L"Command",	// militia movement orders
	L"Diagnose", // disease diagnosis	//TODO.Translate
	L"Treat D.", // treat disease among the population
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Repareer", // repairing
	L"Fortify",		// build structures according to external layout	// TODO.Translate
	L"Train W.",
	L"Hide",	// TODO.Translate
	L"GetIntel",
	L"DoctorM.",
	L"DMilitia",
	L"Burial",
};


STR16 pMilitiaString[] =
{
	L"Militie", // the title of the militia box
	L"Unassigned", //the number of unassigned militia troops
	L"Milities kunnen niet herplaatst worden als er nog vijanden in de buurt zijn!",
	L"Some militia were not assigned to a sector. Would you like to disband them?", // TODO.Translate
};


STR16 pMilitiaButtonString[] =
{
	L"Auto", // auto place the militia troops for the player
	L"OK", // done placing militia troops
	L"Disband", // HEADROCK HAM 3.6: Disband militia	// TODO.Translate
	L"Unassign All", // move all milita troops to unassigned pool	// TODO.Translate
};

STR16 pConditionStrings[] = 
{
	L"Excellent", //the state of a soldier .. excellent health
	L"Good", // good health
	L"Fair", // fair health     
	L"Wounded", // wounded health
	L"Tired", // tired
	L"Bleeding", // bleeding to death
	L"Knocked out", // knocked out
	L"Dying", // near death 
	L"Dead", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"On duty", // set merc on active duty
	L"Patient", // set as a patient to receive medical aid
	L"Vehicle", // tell merc to enter vehicle
	L"Alone", // let the escorted character go off on their own
	L"Close", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Team 1",
	L"Team 2",
	L"Team 3",
	L"Team 4",
	L"Team 5",
	L"Team 6",
	L"Team 7",
	L"Team 8",
	L"Team 9",
	L"Team 10",
	L"Team 11",
	L"Team 12",
	L"Team 13",
	L"Team 14",
	L"Team 15",
	L"Team 16",
	L"Team 17",
	L"Team 18",
	L"Team 19",
	L"Team 20",
	L"Dienst", // on active duty
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Voertuig", // in a vehicle
	L"Onderweg", // in transit - abbreviated form
	L"Repareer", // repairing
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Oefenen", // training themselves  
	L"Militie", // training a town to revolt 
	L"Training Mobile Militia",	// TODO.Translate
	L"Trainer", // training a teammate
	L"Student", // being trained by someone else
	L"Get Item",	// get items	// TODO.Translate
	L"Facility Staff",			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.)	// TODO.Translate
	L"Resting at Facility",		// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners	TODO.Translate
	L"Dood", // dead
	L"Uitgesc.", // abbreviation for incapacitated
	L"POW", // Prisoner of war - captured
	L"Kliniek", // patient in a hospital 
	L"Leeg",	// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gathering Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spreading Propaganda",// TODO.Translate	// spread propaganda
	L"Gathering Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Repareer", // repairing
	L"Fortify sector",		// build structures according to external layout	// TODO.Translate
	L"Train workers",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Team 1",
	L"Team 2",
	L"Team 3",
	L"Team 4",
	L"Team 5",
	L"Team 6",
	L"Team 7",
	L"Team 8",
	L"Team 9",
	L"Team 10",
	L"Team 11",
	L"Team 12",
	L"Team 13",
	L"Team 14",
	L"Team 15",
	L"Team 16",
	L"Team 17",
	L"Team 18",
	L"Team 19",
	L"Team 20",
	L"Dienst", // on active duty
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Voertuig", // in a vehicle
	L"Onderweg", // in transit - abbreviated form
	L"Repareer", // repairing
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Oefenen", // training themselves  
	L"Militie", // training a town to revolt
	L"Train Mobiles",		// TODO.Translate
	L"Trainer", // training a teammate
	L"Student", // being trained by someone else
	L"Get Item",	// get items	// TODO.Translate
	L"Staff Facility",		// TODO.Translate
	L"Rest at Facility",	// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners TODO.Translate
	L"Dood", // dead
	L"Uitgesc.", // abbreviation for incapacitated
	L"POW", // Prisoner of war - captured
	L"Kliniek", // patient in a hospital 
	L"Leeg",	// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gather Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spread Propaganda",// TODO.Translate	// spread propaganda
	L"Gather Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Dokter", // administering medical aid
	L"Patiënt", // getting medical aid
	L"Repareer", // repairing
	L"Fortify sector",		// build structures according to external layout	// TODO.Translate
	L"Train workers",
	L"Hide while disguised",	// TODO.Translate
	L"Get intel while disguised",
	L"Doctor wounded militia",
	L"Drill existing militia",
	L"Bury corpses",
};


// the contract options

STR16 pContractStrings[] =
{
	L"Contract Opties:", 
	L"", // a blank line, required
	L"Voor een dag", // offer merc a one day contract extension
	L"Voor een week", // 1 week
	L"Voor twee weken", // 2 week
	L"Ontslag", // end merc's contract
	L"Stop", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"POW",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
	L"KRACHT",
	L"HANDIGHEID",
	L"BEWEEGLIJKHEID",
	L"WIJSHEID",
	L"TREFZEKERHEID",
	L"MEDISCH",
	L"TECHNISCH",
	L"LEIDERSCHAP",
	L"EXPLOSIEVEN",
	L"NIVEAU",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Wapen", // the armor rating of the merc
	L"Gew.", // the weight the merc is carrying
	L"Camo", // the merc's camouflage rating
	L"Camouflage:",
	L"Protection:",
};

STR16 pShortAttributeStrings[] =
{
	L"Bew", // the abbreviated version of : agility
	L"Han", // dexterity
	L"Kra", // strength
	L"Ldr", // leadership
	L"Wij", // wisdom
	L"Niv", // experience level
	L"Tre", // marksmanship skill
	L"Tec", // mechanical skill
	L"Exp", // explosive skill
	L"Med", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Opdracht", // the mercs current assignment 
	L"Contract", // the contract info about the merc
	L"Gezond", // the health level of the current merc
	L"Moraal", // the morale of the current merc
	L"Cond.",	// the condition of the current vehicle
	L"Tank",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Oefen", // tell merc to train self 
	L"Militie", // tell merc to train town 
	L"Trainer", // tell merc to act as trainer
	L"Student", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Schietniveau:", // the allowable rate of fire for a merc who is guarding
	L" Agressief vuren", // the merc can be aggressive in their choice of fire rates
	L" Spaar Munitie", // conserve ammo 
	L" Afzien van Vuren", // fire only when the merc needs to
	L"Andere Opties:", // other options available to merc
	L" Kan Vluchten", // merc can retreat
	L" Kan Dekking Zoeken",  // merc is allowed to seek cover
	L" Kan Team Helpen", // merc can assist teammates
	L"OK", // done with this menu
	L"Stop", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Schietniveau:",
	L" *Agressief vuren*",
	L" *Spaar Munitie*",
	L" *Afzien van Vuren*",
	L"Andere Opties:",
	L" *Kan Vluchten*",
	L" *Kan Dekking Zoeken*",
	L" *Kan Team Helpen*",
	L"OK",
	L"Stop",
};

STR16 pAssignMenuStrings[] =
{
	L"On duty", // merc is on active duty
	L"Doctor", // the merc is acting as a doctor
	L"Disease", // merc is a doctor doing diagnosis TODO.Translate
	L"Patient", // the merc is receiving medical attention
	L"Vehicle", // the merc is in a vehicle     
	L"Repair", // the merc is repairing items
	L"Radio Scan", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Snitch", // TODO.Translate // anv: snitch actions
	L"Train", // the merc is training
	L"Militia",		// all things militia
	L"Get Item",	// get items	// TODO.Translate
	L"Fortify",		// fortify sector	// TODO.Translate
	L"Intel", // covert assignments	// TODO.Translate
	L"Facility", // the merc is using/staffing a facility	// TODO.Translate
	L"Stop", // cancel this menu
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

	// spy
	L"Hide assignment",		// TODO.Translate
	L"Get Intel assignment",

	// various
	L"Spotter",		// TODO.Translate
	L"Focus",
	L"Drag",
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

	// spy
	L"Assignment: hide among the population.",		// TODO.Translate
	L"Assignment: hide among the population and gather intel.",

	// various
	L"Observe an area, granting allied snipers a bonus to cth on anything you see.",	// TODO.Translate
	L"Increase interrupt modifier (penalty outside of area).",	// TODO.Translate
	L"Drag a person or corpse while you move.",
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
	L" - prone person or corpse next to merc\n",	// TODO.Translate
	L" - crouched position\n",
	L" - free main hand\n",
	L" - covert trait\n",
	L" - enemy occupied sector\n",
	L" - single merc\n",
	L" - no alarm raised\n",
	L" - civilian or soldier disguise\n",
};

STR16 pSkillMenuStrings[] =	// TODO.Translate
{
	L"Militia",
	L"Other Squads",
	L"Cancel",
	L"%d Militia",
	L"All Militia",

	L"More",
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
	L"Verw.Huurl.", // remove dead merc from current team
	L"Stop",
};

STR16 pAttributeMenuStrings[] =
{
	L"Gezondheid",
	L"Lenigheid",
	L"Behendigheid",
	L"Kracht",
	L"Leiderschap",
	L"Scherpschutterskunst",
	L"Mechanisch",
	L"Explosief",
	L"Medisch",
	L"Annuleren",
};

STR16 pTrainingMenuStrings[] =
{
	L"Oefenen", // train yourself 
	L"Train workers",	// TODO.Translate
	L"Trainer", // train your teammates 
	L"Student",  // be trained by an instructor 
	L"Stop", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Team  1",
	L"Team  2",
	L"Team  3",
	L"Team  4",
	L"Team  5",
	L"Team  6",
	L"Team  7",
	L"Team  8",
	L"Team  9",
	L"Team 10",
	L"Team 11",
	L"Team 12",
	L"Team 13",
	L"Team 14",
	L"Team 15",
	L"Team 16",
	L"Team 17",
	L"Team 18",
	L"Team 19",
	L"Team 20",
	L"Stop",
};

STR16 pPersonnelTitle[] =
{
	L"Dossiers", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Gezondheid: ", // health of merc
	L"Beweeglijkheid: ", 
	L"Handigheid: ",
	L"Kracht: ",
	L"Leiderschap; ",
	L"Wijsheid: ",
	L"Erv. Niv.: ", // experience level
	L"Trefzekerheid: ",
	L"Techniek: ",
	L"Explosieven: ",
	L"Medisch: ",
	L"Med. Kosten: ", // amount of medical deposit put down on the merc
	L"Rest Contract: ", // cost of current contract
	L"Doden: ", // number of kills by merc
	L"Hulp: ", // number of assists on kills by merc
	L"Dag. Kosten:", // daily cost of merc
	L"Huidige Tot. Kosten:", // total cost of merc
	L"Huidige Tot. Service:", // total service rendered by merc
	L"Salaris Tegoed:", // amount left on MERC merc to be paid
	L"Trefzekerheid:", // percentage of shots that hit target
	L"Gevechten:", // number of battles fought
	L"Keren Gewond:", // number of times merc has been wounded
	L"Vaardigheden:",
	L"Vaardigheden:",
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
	L"No Skill",
	L"Forceer slot",
	L"Man-tot-man",
	L"Elektronica",
	L"Nachtops",
	L"Werpen",
	L"Lesgeven",
	L"Zware Wapens",
	L"Auto Wapens",
	L"Sluipen",
	L"Handig",
	L"Dief",
	L"Vechtkunsten",
	L"Mesworp",
	L"Sniper",
	L"Camouflaged",					
	L"(Expert)",
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
	L"Placeholder",		// 38
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
	L"various",			// for VARIOUSSKILLS
};
//////////////////////////////////////////////////////////


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"|Staan/Lopen",
	L"Hurken/Gehurkt lopen (|C)",
	L"Staan/|Rennen",
	L"Liggen/Kruipen (|P)",
	L"Kijk (|L)",
	L"Actie",
	L"Praat",
	L"Bekijk (|C|t|r|l)",

	// Pop up door menu
	L"Handm. openen",
	L"Zoek boobytraps",
	L"Forceer",
	L"Met geweld",
	L"Verwijder boobytrap",
	L"Sluiten",
	L"Maak open",
	L"Gebruik explosief",
	L"Gebruik breekijzer",
	L"Stoppen (|E|s|c)",
	L"Stop",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"geen val",
	L"een explosie",
	L"een elektrische val",
	L"alarm",
	L"stil alarm",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"dag",
	L"week",
	L"twee weken",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Selecteer Karakter",
	L"Contracteer huurling",
	L"Plan Route",
	L"Huurling |Contract",
	L"Verwijder Huurling",
	L"Slaap",
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"VAAG",
	L"ZEKER",
	L"HARD",
	L"ERG HARD",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"ONBEKEND",
	L"geluid van BEWEGING",
	L"GEKRAAK",
	L"PLONZEN",
	L"INSLAG",
	L"SCHOT",
	L"EXPLOSIE",
	L"GEGIL",
	L"INSLAG",
	L"INSLAG",
	L"BARSTEN",
	L"DREUN",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"het NOORDOOSTEN",
	L"het OOSTEN",
	L"het ZUIDOOSTEN",
	L"het ZUIDEN",
	L"het ZUIDWESTEN",
	L"het WESTEN",
	L"het NOORDWESTEN",
	L"het NOORDEN",
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Stad",
	L"Weg",
	L"Vlaktes",
	L"Woestijn",
	L"Bossen",
	L"Woud",
	L"Moeras",
	L"Water",
	L"Heuvels",
	L"Onbegaanbaar",
	L"Rivier",	//river from north to south
	L"Rivier",	//river from east to west
	L"Buitenland",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Tropisch",
	L"Landbouwgrond",
	L"Vlaktes, weg",
	L"Bossen, weg",
	L"Boerderij, weg",
	L"Tropisch, weg",
	L"Woud, weg",
	L"Kustlijn",
	L"Bergen, weg",
	L"Kust-, weg",
	L"Woestijn, weg",
	L"Moeras, weg",
	L"Bossen, SAM-stelling",
	L"Woestijn, SAM-stelling",
	L"Tropisch, SAM-stelling",
	L"Meduna, SAM-stelling",
	
	//These are descriptions for special sectors
	L"Cambria Ziekenhuis",
	L"Drassen Vliegveld",
	L"Meduna Vliegveld",
	L"SAM-stelling",
	L"Refuel site", // TODO.Translate
	L"Schuilplaats Rebellen",	//The rebel base underground in sector A10
	L"Tixa Kerker",			//The basement of the Tixa Prison (J9)
	L"Hol Beest",			//Any mine sector with creatures in it
	L"Orta Basis",			//The basement of Orta (K4)
	L"Tunnel",				//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Schuilplaats",			//The shelter underneath the queen's palace
	L"",					//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s zijn ontdekt in sector %c%d en een ander team arriveert binnenkort.",	//STR_DETECTED_SINGULAR
	L"%s zijn ontdekt in sector %c%d en andere teams arriveren binnenkort.",	//STR_DETECTED_PLURAL
	L"Wil je een gezamenlijke aankomst coördineren?",					//STR_COORDINATE

	//Dialog strings for enemies.

	L"De vijand geeft je de kans om je over te geven.",			//STR_ENEMY_SURRENDER_OFFER
	L"De vijand heeft je overgebleven bewusteloze huurlingen gevangen.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Vluchten", 	//The retreat button			//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"VERDEDIGEN",								//STR_AR_DEFEND_HEADER
	L"AANVALLEN",								//STR_AR_ATTACK_HEADER
	L"ONTDEKKEN",								//STR_AR_ENCOUNTER_HEADER
	L"Sector",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"VICTORIE!",								//STR_AR_OVER_VICTORY
	L"NEDERLAAG!",								//STR_AR_OVER_DEFEAT
	L"OVERGEGEVEN!",							//STR_AR_OVER_SURRENDERED
	L"GEVANGEN!",								//STR_AR_OVER_CAPTURED
	L"GEVLUCHT!",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Militie",							//STR_AR_MILITIA_NAME,
	L"Elite",							//STR_AR_ELITE_NAME,
	L"Troep",							//STR_AR_TROOP_NAME,
	L"Admin",							//STR_AR_ADMINISTRATOR_NAME,
	L"Wezen",							//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Tijd verstreken",							//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"GEVLUCHT",							//STR_AR_MERC_RETREATED,
	L"VLUCHTEN",							//STR_AR_MERC_RETREATING,
	L"VLUCHT",								//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Autom. Opl.",			//!!! 1			//STR_PB_AUTORESOLVE_BTN,
	L"Naar Sector",						//STR_PB_GOTOSECTOR_BTN,
	L"Terug- trekken",			//!!! 2		//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"VIJAND ONTDEKT",						//STR_PB_ENEMYENCOUNTER_HEADER,
	L"INVASIE VIJAND",						//STR_PB_ENEMYINVASION_HEADER, // 30
	L"HINDERLAAG VIJAND",						//STR_PB_ENEMYAMBUSH_HEADER
	L"BINNENGAAN VIJANDIGE SECTOR",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"AANVAL BEEST",							//STR_PB_CREATUREATTACK_HEADER
	L"BLOODCAT VAL",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"BINNENGAAN HOL BLOODCAT",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER
	L"ENEMY AIRDROP",					//STR_PB_ENEMYINVASION_AIRDROP_HEADER	// TODO.Translate

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Locatie",
	L"Vijanden",
	L"Huurlingen",
	L"Milities",
	L"Beesten",
	L"Bloodcats",
	L"Sector",
	L"Geen",		//If there are no uninvolved mercs in this fight.
	L"NVT",			//Acronym of Not Applicable
	L"d",			//One letter abbreviation of day
	L"u",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Weggaan",
	L"Verspreid",
	L"Groeperen",
	L"OK",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Maakt posities van huurlingen vrij en\nmaakt handmatig herinvoer mogelijk. (|C)",
	L"Ver|spreidt willekeurig je huurlingen\nelke keer als je de toets indrukt.",
	L"Hiermee is het mogelijk de huurlingen te |groeperen.",
	L"Druk op deze toets als je klaar bent met\nhet positioneren van je huurlingen. (|E|n|t|e|r)",
	L"Je moet al je huurlingen positioneren\nvoor je het gevecht kunt starten.",

	//Various strings (translate word for word)

	L"Sector",
	L"Kies posities binnenkomst",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Ziet er hier niet goed uit. Het is onbegaanbaar. Probeer een andere locatie.",
	L"Plaats je huurlingen in de gemarkeerde sectie van de kaart.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"is gearriveerd in sector", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Lost het gevecht |Automatisch\nop zonder de kaart te laden.",
	L"Automatisch oplossen niet\nmogelijk als de speler aanvalt.",
	L"Ga sector binnen om tegen\nde vijand te strijden. (|E)",
	L"T|rek groep terug en ga naar de vorige sector.",			//singular version
	L"T|rek alle groepen terug en\nga naar hun vorige sectors.", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"Vijanden vallen je militie aan in sector %c%d.",
	//%c%d is the sector -- ex:  A9
	L"Beesten vallen je militie aan in sector %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Beesten vallen aan en doden %d burgers in sector %s.",
	//%s is the sector location -- ex:  A9: Omerta
	L"Vijand valt je huurlingen aan in sector %s. Geen enkele huurling kan vechten!",
	//%s is the sector location -- ex:  A9: Omerta
	L"Beesten vallen je huurlingen aan in sector %s. Geen enkele huurling kan vechten!",

	// Flugente: militia movement forbidden due to limited roaming	// TODO.Translate
	L"Militia cannot move here (RESTRICT_ROAMING = TRUE).",
	L"War room isn't staffed - militia move aborted!",

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
	L"Dag",
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Sector gevonden:",
	L"Dag gevonden:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 20 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Status:",
	L"Gewicht:",
	L"AP Costs",	
	L"Afst:",		// Range
	L"Sch:",		// Damage
	L"Munitie:", 		// Number of bullets left in a magazine
	L"AP:",			// abbreviation for Action Points
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
	L"Bedrag",
	L"Restbedrag:", //this is the overall balance
	L"Bedrag",
	L"Splitsen:", // the amount he wants to separate from the overall balance to get two piles of money

	L"Huidig",
	L"Saldo:",
	L"Bedrag",
	L"naar Opnemen:",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"STERVEND",	//	>= 0
	L"KRITIEK", 	//	>= 15
	L"SLECHT",		//	>= 30
	L"GEWOND",    	//	>= 45
	L"GEZOND",    	//	>= 60
	L"STERK",     	// 	>= 75
	L"EXCELLENT",	// 	>= 90
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
	L"OK",
	L"Splitsen",
	L"Opnemen",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons." 
CHAR16		gzProsLabel[10] = 
{
	L"Voor:",
};

CHAR16		gzConsLabel[10] = 
{
	L"Tegen:",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Wat?", 		//meaning "Repeat yourself" 
	L"Aardig",		//approach in a friendly
	L"Direct",		//approach directly - let's get down to business
	L"Dreigen",		//approach threateningly - talk now, or I'll blow your face off
	L"Geef",		
	L"Rekruut",		//recruit
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Koop/Verkoop",	//Buy/Sell
	L"Koop",		//Buy
	L"Verkoop",		//Sell
	L"Repareer",		//Repair
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"OK",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"Eldorado",
 L"Hummer",			// a hummer jeep/truck -- military vehicle
 L"Koeltruck",		// Icecream Truck
 L"Jeep",
 L"Tank",
 L"Helikopter",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Truck",
	L"Jeep",
	L"Tank",
	L"Heli", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Truck",			// Ice cream truck
	L"Jeep",
	L"Tank",
	L"Heli", 		//an abbreviation for Helicopter
};

STR16 pVehicleSeatsStrings[] =
{
	 L"You cannot shoot from this seat.",	// TODO.Translate
	 L"You cannot swap those two seats in combat without exiting vehicle first.",	// TODO.Translate
};

//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Luchtaanval",
	L"Automatisch EHBO toepassen?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s ziet dat er items missen van de lading.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Het slot heeft %s.",
	L"Er is geen slot.",
	L"Gelukt!",
	L"Mislukt.",
	L"Gelukt!",
	L"Mislukt.",
	L"Geen boobytrap op het slot.",
	L"Gelukt!",
	// The %s is a merc name
	L"%s heeft niet de juiste sleutel.",
	L"Val weggehaald van slot.",
	L"Slot heeft geen boobytrap.",
	L"Op slot.",
	L"DEUR",
	L"VAL",
	L"OP SLOT",
	L"OPEN",
	L"KAPOT",
	L"Hier zit een schakelaar. Activeren?",
	L"Boobytrap ontmantelen?",
	L"Vorige...",
	L"Volgende...",
	L"Meer...",

	// In the next 2 strings, %s is an item name

	L"%s is op de grond geplaatst.",
	L"%s is gegeven aan %s.",

	// In the next 2 strings, %s is a name

	L"%s is helemaal betaald.",
	L"%s heeft tegoed nog %d.",
	L"Kies detonatie frequentie:",  	//in this case, frequency refers to a radio signal
	L"Aantal beurten tot ontploffing:",	//how much time, in turns, until the bomb blows
	L"Stel frequentie in van ontsteking:", 	//in this case, frequency refers to a radio signal
	L"Boobytrap ontmantelen?",
	L"Blauwe vlag weghalen?",
	L"Blauwe vlag hier neerzetten?",
	L"Laatste beurt",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Zeker weten dat je %s wil aanvallen?",
	L"Ah, voertuigen kunnen plaats niet veranderen.",
	L"De robot kan niet van plaats veranderen.",

	// In the next 3 strings, %s is a name

	L"%s kan niet naar die plaats gaan.",
	L"%s kan hier geen EHBO krijgen.",
	L"%s heeft geen EHBO nodig.",
	L"Kan daar niet heen.",
	L"Je team is vol. Geen ruimte voor rekruut.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s is gerekruteerd.",

	// Here %s is a name and %d is a number

	L"%s ontvangt $%d.",

	// In the next string, %s is a name

	L"%s begeleiden?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"%s inhuren voor %s per dag?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Wil je vechten?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"%s kopen voor %s?",

	// In the next string, %s is a name

	L"%s wordt begeleid door team %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"GEBLOKKEERD",					//weapon is jammed.
	L"Robot heeft %s kal. munitie nodig.",		//Robot is out of ammo
	L"Hier gooien? Kan niet.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Sluipmodus (|Z)",			// L"Stealth Mode (|Z)",
	L"Landkaart (|M)",			// L"|Map Screen",
	L"OK (Ein|de)",				// L"|Done (End Turn)",
	L"Praat",					// L"Talk",
	L"Stil",					// L"Mute",
	L"Omhoog (|P|g|U|p)",			// L"Stance Up (|P|g|U|p)",
	L"Cursor Niveau (|T|a|b)",		// L"Cursor Level (|T|a|b)",
	L"Klim / Spring",				// L"Climb / Jump",
	L"Omlaag (|P|g|D|n)",			// L"Stance Down (|P|g|D|n)",
	L"Bekijk (|C|t|r|l)",			// L"Examine (|C|t|r|l)",
	L"Vorige huurling",			// L"Previous Merc",
	L"Volgende huurling (|S|p|a|c|e)",		// L"Next Merc (|S|p|a|c|e)",
	L"|Opties",					// L"|Options",
	L"Salvo's (|B)",				// L"|Burst Mode",
	L"Kijk/draai (|L)",			// L"|Look/Turn",
	L"Gezond: %d/%d\nKracht: %d/%d\nMoraal: %s",		// L"Health: %d/%d\nEnergy: %d/%d\nMorale: %s",
	L"Hé?",					//this means "what?" 
	L"Door",					//an abbrieviation for "Continued" 
	L"%s is praat weer.",			// L"Mute off for %s.",
	L"%s is stil.",				// L"Mute on for %s.",
	L"Gezond: %d/%d\nBrandst: %d/%d",	// L"Health: %d/%d\nFuel: %d/%d",
	L"Stap uit voertuig",			// L"Exit Vehicle" ,
	L"Wissel Team ( |S|h|i|f|t |S|p|a|c|e )",			// L"Change Squad ( |S|h|i|f|t |S|p|a|c|e )",
	L"Rijden",					// L"Drive",
	L"Nvt",						//this is an acronym for "Not Applicable." 
	L"Actie ( Man-tot-man )",		// L"Use ( Hand To Hand )",
	L"Actie ( Firearm )",			// L"Use ( Firearm )",
	L"Actie ( Mes )",				// L"Use ( Blade )",
	L"Actie ( Explosieven )",		// L"Use ( Explosive )",
	L"Actie ( EHBO )",			// L"Use ( Medkit )",
	L"(Vang)",					// L"(Catch)",
	L"(Herlaad)",				// L"(Reload)",
	L"(Geef)",					// L"(Give)",
	L"%s is afgezet.",			// L"%s has been set off.",
	L"%s is gearriveerd.",			// L"%s has arrived.",
	L"%s heeft geen Actie Punten.",	// L"%s ran out of Action Points.",
	L"%s is niet beschikbaar.",		// L"%s isn't available.",
	L"%s zit onder het verband.",		// L"%s is all bandaged.",
	L"Verband van %s is op.",		// L"%s is out of bandages.",
	L"Vijand in de sector!",		// L"Enemy in sector!",
	L"Geen vijanden in zicht.",		// L"No enemies in sight.",
	L"Niet genoeg Actie Punten.",		// L"Not enough Action Points.",
	L"Niemand gebruikt afstandb.",	// L"Nobody's using the remote.",
	L"Magazijn leeg door salvovuur!",	// L"Burst fire emptied the clip!",
	L"SOLDAAT",					// L"SOLDIER",
	L"CREPITUS",				// L"CREPITUS",
	L"MILITIE",					// L"MILITIA",
	L"BURGER",					// L"CIVILIAN",
	L"ZOMBIE",		// TODO.Translate
	L"PRISONER",// TODO.Translate
	L"Verlaten Sector",			// L"Exiting Sector",
	L"OK",
	L"Stoppen",					// L"Cancel",
	L"Huurling gesel.",			// L"Selected Merc",
	L"Alle huurl. in team",			// L"All Mercs in Squad",
	L"Naar Sector",				// L"Go to Sector",
	L"Naar Landk.",				// L"Go to Map",
	L"Vanaf deze kant kun je de sector niet verlaten.",	// L"You can't leave the sector from this side.",
	L"You can't leave in turn based mode.",		// TODO.Translate
	L"%s is te ver weg.",			// L"%s is too far away.",
	L"Verwijder Boomtoppen",		// L"Removing Treetops",
	L"Tonen Boomtoppen",			// L"Showing Treetops",
	L"KRAAI",				//Crow, as in the large black bird
	L"NEK",
	L"HOOFD",
	L"TORSO",
	L"BENEN",
	L"De Koningin vertellen wat ze wil weten?",		// L"Tell the Queen what she wants to know?",
	L"Vingerafdruk-ID nodig",					// L"Fingerprint ID aquired",
	L"Vingerafdruk-ID ongeldig. Wapen funct. niet",		// L"Invalid fingerprint ID. Weapon non-functional",
	L"Doelwit nodig",					// L"Target aquired",
	L"Pad geblokkeerd",				// L"Path Blocked",
	L"Geld Storten/Opnemen",		//Help text over the $ button on the Single Merc Panel ("Deposit/Withdraw Money")
	L"Niemand heeft EHBO nodig.",			// L"No one needs first aid.",
	L"Vast.",						// Short form of JAMMED, for small inv slots
	L"Kan daar niet heen.",				// used ( now ) for when we click on a cliff
	L"Pad is geblokkeerd. Wil je met deze persoon van plaats wisselen?",
	L"Persoon weigert weg te gaan.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Ben je het eens met %s?",					// L"Do you agree to pay %s?",
	L"Wil je kostenloze medische hulp?",			// L"Accept free medical treatment?",
	L"Wil je trouwen met %s?",					// L"Agree to marry %s?", Daryl
	L"Slot Ring Paneel",						// L"Key Ring Panel",
	L"Dat kan niet met een EPC.",					// L"You cannot do that with an EPC.",
	L"%s sparen?",							// L"Spare Krott?", Krott
	L"Buiten wapenbereik",						// L"Out of weapon range",
	L"Mijnwerker",							// L"Miner",
	L"Voertuig kan alleen tussen sectors reizen",		// L"Vehicle can only travel between sectors",
	L"Nu geen Auto-EHBO mogelijk",				// L"Can't autobandage right now",
	L"Pad Geblokkeerd voor %s",					// L"Path Blocked for %s",
	L"Je huurlingen, gevangen door %s's leger, zitten hier opgesloten!", //Deidranna
	L"Slot geraakt",							// L"Lock hit",
	L"Slot vernielt",							// L"Lock destroyed",
	L"Iemand anders probeert deze deur te gebruiken.",		// L"Somebody else is trying to use this door.",
	L"Gezondheid: %d/%d\nBrandstof: %d/%d",				//L"Health: %d/%d\nFuel: %d/%d",
	L"%s kan %s niet zien.",					// Cannot see person trying to talk to
	L"Attachment removed",
	L"Kan niet een ander voertuig bereiken aangezien u reeds 2 hebt",

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
	L"Als aangekruist, dan wordt de aanliggende sector meteen geladen.",
	L"Als aangekruist, dan worden de huurlingen automatisch op de\nkaart geplaatst rekening houdend met reistijden.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Deze sector is door de vijand bezet en huurlingen kun je niet achterlaten.\nJe moet deze situatie oplossen voor het laden van andere sectors.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Als de overgebleven huurlingen uit deze sector trekken,\nwordt de aanliggende sector onmiddellijk geladen.",
	L"Als de overgebleven huurlingen uit deze sector trekken,\nwordt je automatisch in het landkaartscherm geplaatst,\nrekening houdend met de reistijd van je huurlingen.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s moet geëscorteerd worden door jouw huurlingen\nen kan de sector niet alleen verlaten.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s kan de sector niet alleen verlaten omdat hij %s escorteert.",	//male singular
	L"%s kan de sector niet alleen verlaten omdat zij %s escorteert.",	//female singular
	L"%s kan de sector niet alleen verlaten omdat hij meerdere karakters escorteert.",	//male plural
	L"%s kan de sector niet alleen verlaten omdat zij meerdere karakters escorteert.",	//female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Al je huurlingen moeten in de buurt zijn om het team te laten reizen.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Als aangekruist, dan zal %s alleen verder reizen\nen automatisch bij een uniek team gevoegd worden.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Als aangekruist, dan zal je geselecteerde\nteam verder reizen, de sector verlatend.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s wordt geëscorteerd door jouw huurlingen en kan de sector niet alleen verlaten. Je huurlingen moeten eerst in de buurt zijn.",
};



STR16 pRepairStrings[] = 
{
	L"Items", 		// tell merc to repair items in inventory
	L"SAM-Stelling", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Stop", 		// cancel this menu
	L"Robot", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 

STR16 sPreStatBuildString[] =
{
	L"verliest", 	// the merc has lost a statistic
	L"krijgt", 		// the merc has gained a statistic
	L"punt voor",	// singular
	L"punten voor",	// plural
	L"niveau voor",	// singular
	L"niveaus voor",	// plural
};

STR16 sStatGainStrings[] =
{
	L"gezondheid.",
	L"beweeglijkheid.",
	L"handigheid.",
	L"wijsheid.",
	L"medisch kunnen.",
	L"explosieven.",
	L"technisch kunnen.",
	L"trefzekerheid.",
	L"ervaring.",
	L"kracht.",
	L"leiderschap.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Totale Afstand: ", 			// total distance for helicopter to travel
	L" Veilig: ", 			// distance to travel to destination
	L" Onveilig:", 			// distance to return from destination to airport
	L"Totale Kosten: ", 		// total cost of trip by helicopter
	L"Aank: ", 				// ETA is an acronym for "estimated time of arrival" 
	L"Helikopter heeft weinig brandstof en moet landen in vijandelijk gebied!",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
	L"Passagiers: ",
	L"Selecteer Skyrider of Aanvoer Drop-plaats?",			// L"Select Skyrider or the Arrivals Drop-off?",
	L"Skyrider",
	L"Aanvoer",						// L"Arrivals",
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
	L"Subniv.:", 			// what level below the ground is the player viewing in mapscreen ("Sublevel:")
};

STR16 gsLoyalString[] =
{
	L"Loyaal", 			// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"kan geen reisorders ondergronds ontvangen.",				// L"can't get travel orders underground.",
};

STR16 gsTimeStrings[] =
{
	L"u",				// hours abbreviation
	L"m",				// minutes abbreviation
	L"s",				// seconds abbreviation
	L"d",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"Geen",
	L"Ziekenhuis",
	L"Factory",	// TODO.Translate
	L"Gevangenis",
	L"Krijgsmacht",
	L"Vliegveld",
	L"Schietterrein",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Inventaris",
	L"OK",
	L"Repair",	// TODO.Translate
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Grootte",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Gezag", 					// how much of town is controlled
	L"Geen", 					// none of this town
	L"Verboden Mijn", 				// mine associated with this town
	L"Loyaliteit",					// 5 // the loyalty level of this town
	L"Getraind", 					// the forces in the town trained by the player
	L"",
	L"Voorzieningen", 				// main facilities in this town
	L"Niveau", 					// the training level of civilians in this town
	L"Training Burgers",				// 10 // state of civilian training in town
	L"Militie", 					// the state of the trained civilians in the town
	
	// Flugente: prisoner texts
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
	L"Mijn",					// 0
	L"Zilver",
	L"Goud",
	L"Dagelijkse prod.",
	L"Mogelijke prod.",
	L"Verlaten",				// 5
	L"Gesloten",
	L"Raakt Op",
	L"Produceert",
	L"Status",
	L"Prod. Tempo",
	L"Resource",				// 10	L"Ertstype",	// TODO.Translate
	L"Gezag Dorp",
	L"Loyaliteit Dorp",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Vijandelijke troepen",
	L"Sector",
	L"# Items",
	L"Onbekend",

	L"Gecontrolleerd",
	L"Ja",
	L"Nee",
	L"Status/Software status:",	// TODO.Translate

	L"Additional Intel", // TODO:Translate
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s is niet dichtbij genoeg.",		//Merc is in sector with item but not close enough
	L"Kan huurling niet selecteren.",		//MARK CARTER
	L"%s is niet in de sector om dat item te pakken.",
	L"Tijdens gevechten moet je items handmatig oppakken.",
	L"Tijdens gevechten moet je items handmatig neerleggen.",
	L"%s is niet in de sector om dat item neer te leggen.",
	L"Tijdens gevecht, kunt u met een munitiekrat herladen niet.",
};

STR16 pMapInventoryStrings[] =
{
	L"Locatie", 		// sector these items are in
	L"Aantal Items", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Om de taken van een huurling te veranderen, zoals team, dokter of repareren, klik dan in de 'Toewijzen'-kolom",
	L"Om een huurling een ander doel te geven, klik dan in de 'Doel'-kolom",
	L"Op het moment dat een huurling een reis-order gekregen heeft, kan deze met de tijd-versneller in beweging worden gezet.",
	L"Links-klikken selecteert de sector. Nogmaals links-klikken geeft de huurling een reisorder. Rechts-klikken geeft sector-informatie.",
	L"Druk op een willekeurig moment op 'h'om deze helptekst te krijgen.",
	L"Test Tekst",
	L"Test Tekst",
	L"Test Tekst",
	L"Test Tekst",
	L"Totdat je arriveert in Arulco is er niet veel te doen bij dit scherm. Als je klaar bent met het samenstellen van je team, klik dan op de Tijd-Versnel-knop rechtsonder. Zo verstrijkt de tijd totdat je team in Arulco aankomt.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Huurlingen in Sector", 	// title for movement box 
	L"Teken Reisroute", 		// done with movement menu, start plotting movement
	L"Stop", 		// cancel this menu
	L"Anders",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"Oeps:", 			// an error has occured
	L"Contract Huurling verlopen:", 	// this pop up came up due to a merc contract ending
	L"Huurling Taak Volbracht:", // this pop up....due to more than one merc finishing assignments
	L"Huurling weer aan het Werk:", // this pop up ....due to more than one merc waking up and returing to work
	L"Huurling zegt Zzzzzzz:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Contract Loopt Bijna Af:", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Toon Dorpen (|W)",
	L"Toon |Mijnen",
	L"Toon |Teams & Vijanden",
	L"Toon Luchtruim (|A)",
	L"Toon |Items",
	L"Toon Milities & Vijanden (|Z)",
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
	L"|Laptop",
	L"Tactisch (|E|s|c)",
	L"|Opties",
	L"TijdVersneller (|+)", 	// time compress more
	L"TijdVersneller (|-)", 	// time compress less
	L"Vorig Bericht (|U|p)\nVorige Pagina (|P|g|U|p)", 	// previous message in scrollable list
	L"Volgend Bericht (|D|o|w|n)\nVolgende pagina (|P|g|D|n)", 	// next message in the scrollable list
	L"Start/Stop Tijd (|S|p|a|c|e)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Huidig Saldo", 		// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s is dood.",
};


STR16 pDayStrings[] =
{
	L"Dag",
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
	L"Vicki",		//10
	L"Trevor",
	L"Grunty",
	L"Ivan",
	L"Steroid",
	L"Igor",		//15
	L"Shadow",
	L"Red",
	L"Reaper",
	L"Fidel",
	L"Fox",		//20
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
	L"M.I.S. Verzekeringen",	
	L"Bobby Rays",
	L"Kingpin",
	L"John Kulba",
	L"A.I.M.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"Vorige",
  L"Volgende",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Je hebt nieuwe berichten...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Bericht verwijderen?",
 L"ONGELEZEN bericht(en) verwijderen?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"Van:",
	L"Subject:",
	L"Dag:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"Postvak",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Account Plus",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Credit:", 				// credit (subtract from) to player's account
	L"Debet:", 				// debit (add to) to player's account
	L"Saldo Gisteren:",
	L"Stortingen Gisteren:",
	L"Uitgaven Gisteren:",
	L"Saldo Eind van de Dag:",
	L"Saldo Vandaag:",
	L"Stortingen Vandaag:",
	L"Uitgaven Vandaag:",
	L"Huidig Saldo:",
	L"Voorspelde Inkomen:",
	L"Geschat Saldo:", 	// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
	L"Dag", 			// the day column
	L"Credit", 			// the credits column (to ADD money to your account)
	L"Debet",			// the debits column (to SUBTRACT money from your account)
	L"Transactie", 		// transaction type - see TransactionText below
	L"Saldo", 			// balance at this point in time
	L"Pag.", 			// page number
	L"Dag(en)", 		// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Toegenomen Interest",			// interest the player has accumulated so far
	L"Anonieme Storting",
	L"Transactiekosten", 
	L"Gehuurd", 					// Merc was hired
	L"Bobby Ray's Wapenhandel", 		// Bobby Ray is the name of an arms dealer
	L"Rekeningen Voldaan bij M.E.R.C.",
	L"Medische Storting voor %s", 		// medical deposit for merc
	L"IMP Profiel Analyse", 		// IMP is the acronym for International Mercenary Profiling
	L"Verzekering Afgesloten voor %s", 
	L"Verzekering Verminderd voor %s",
	L"Verzekering Verlengd voor %s", 				// johnny contract extended
	L"Verzekering Afgebroken voor %s", 
	L"Verzekeringsclaim voor %s", 		// insurance claim for merc
	L"een dag", 				// merc's contract extended for a day
	L"1 week", 					// merc's contract extended for a week
	L"2 weken", 				// ... for 2 weeks
	L"Inkomen Mijn", 
	L"",						 //String nuked
	L"Gekochte Bloemen",
	L"Volledige Medische Vergoeding voor %s",
	L"Gedeeltelijke Medische Vergoeding voor %s",
	L"Geen Medische Vergoeding voor %s",
	L"Betaling aan %s",				// %s is the name of the npc being paid
	L"Maak Geld over aan %s", 		// transfer funds to a merc
	L"Maak Geld over van %s", 		// transfer funds from a merc
	L"Rust militie uit in %s",			// initial cost to equip a town's militia
	L"Items gekocht van %s.",		//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s heeft geld gestort.",
	L"Sold Item(s) to the Locals",
	L"Facility Use", // HEADROCK HAM 3.6		// TODO.Translate
	L"Militia upkeep", // HEADROCK HAM 3.6		// TODO.Translate
	L"Ransom for released prisoners",	// Flugente: prisoner system TODO.Translate
	L"WHO data subscription",	// Flugente: disease TODO.Translate
	L"Payment to Kerberus",	// Flugente: PMC
	L"SAM site repair", // Flugente: SAM repair // TODO.Translate
	L"Trained workers", // Flugente: train workers
	L"Drill militia in %s",	// Flugente: drill militia	// TODO.Translate
};

STR16 pTransactionAlternateText[] =
{
	L"Verzekering voor", 				// insurance for a merc
	L"Contract %s verl. met 1 dag.", 		// entend mercs contract by a day
	L"Contract %s verl. met 1 week.",
	L"Contract %s verl. met 2 weken.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Skyrider is $%d betaald.", 			// skyrider was paid an amount of money
	L"Skyrider heeft $%d tegoed.", 		// skyrider is still owed an amount of money
	L"Skyrider is klaar met tanken",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider is klaar om weer te vliegen.", // Skyrider was grounded but has been freed
	L"Skyrider heeft geen passagiers. Als je huurlingen in deze sector wil vervoeren, wijs ze dan eerst toe aan Voertuig/Helikopter.",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"Super", 
	L"Goed",
	L"Stabiel",
	L"Mager",
	L"Paniek",
	L"Slecht",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s's uitrusting is nu beschikbaar in Omerta (A9).", 
	L"%s's uitrusting is nu beschikbaar in Drassen (B13).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Gezondheid",
	L"Energie",
	L"Moraal",
	L"Conditie",	// the condition of the current vehicle (its "health")
	L"Brandstof",	// the fuel level of the current vehicle (its "energy")
	L"Posion",	// TODO.Translate
	L"Water",		// drink level
	L"Food",		// food level
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Vorige Huurling (|L|e|f|t)", 			// previous merc in the list
	L"Volgende Huurling (|R|i|g|h|t)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"aank:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Je bent het voor altijd kwijt. Zeker weten?", 	// do you want to continue and lose the item forever
	L"Dit item ziet er HEEL belangrijk uit. Weet je HEEL, HEEL zeker dat je het wil weggooien?", // does the user REALLY want to trash this item

};


STR16 pMapErrorString[] = 
{
	L"Team kan niet verder reizen met een slapende huurling.",

//1-5
	L"Verplaats het team eerst bovengronds.",
	L"Reisorders? Het is vijandig gebied!",
	L"Om te verplaatsen moeten huurlingen eerst toegewezen worden aan een team of voertuig.",
	L"Je hebt nog geen team-leden.", 			// you have no members, can't do anything
	L"Huurling kan order niet opvolgen.",			 		// merc can't comply with your order
//6-10
	L"heeft een escorte nodig. Plaats hem in een team.", // merc can't move unescorted .. for a male
	L"heeft een escorte nodig. Plaats haar in een team.", // for a female
	L"Huurling is nog niet in %s aangekomen!",
	L"Het lijkt erop dat er eerst nog contractbesprekingen gehouden moeten worden.",
	L"Cannot give a movement order. Air raid is going on.",
//11-15
	L"Reisorders? Er is daar een gevecht gaande!",
	L"Je bent in een hinderlaag gelokt van Bloodcats in sector %s!",
	L"Je bent in sector %s iets binnengelopen dat lijkt op het hol van een bloodcat!",
	L"", 
	L"De SAM-stelling in %s is overgenomen.",
//16-20
	L"De mijn in %s is overgenomen. Je dagelijkse inkomen is gereduceerd tot %s per dag.",
	L"De vijand heeft sector %s onbetwist overgenomen.",
	L"Tenminste een van je huurlingen kan niet meedoen met deze opdracht.",
	L"%s kon niet meedoen met %s omdat het al vol is",
	L"%s kon niet meedoen met %s omdat het te ver weg is.",
//21-25
	L"De mijn in %s is buitgemaakt door Deidranna's troepen!",
	L"Deidranna's troepen zijn net de SAM-stelling in %s binnengevallen",
	L"Deidranna's troepen zijn net %s binnengevallen",
	L"Deidranna's troepen zijn gezien in %s.",
	L"Deidranna's troepen hebben zojuist %s overgenomen.",
//26-30
	L"Tenminste één huurling kon niet tot slapen gebracht worden.",
	L"Tenminste één huurling kon niet wakker gemaakt worden.",
	L"De Militie verschijnt niet totdat hun training voorbij is.",
	L"%s kan geen reisorders gegeven worden op dit moment.",
	L"Milities niet binnen de stadsgrenzen kunnen niet verplaatst worden naar een andere sector.",
//31-35
	L"Je kunt geen militie in %s hebben.",
	L"Een voertuig kan niet leeg rijden!",
	L"%s is te gewond om te reizen!",
	L"Je moet het museum eerst verlaten!",
	L"%s is dood!",
//36-40
	L"%s kan niet wisselen naar %s omdat het onderweg is",
	L"%s kan het voertuig op die manier niet in",
	L"%s kan zich niet aansluiten bij %s",
	L"Totdat je nieuwe huurlingen in dienst neemt, kan de tijd niet versneld worden!",
	L"Dit voertuig kan alleen over wegen rijden!",
//41-45
	L"Je kunt geen reizende huurlingen opnieuw toewijzen",
	L"Voertuig zit zonder brandstof!",
	L"%s is te moe om te reizen.",
	L"Niemand aan boord is in staat om het voertuig te besturen.",
	L"Eén of meer teamleden kunnen zich op dit moment niet verplaatsen.",
//46-50
	L"Eén of meer leden van de ANDERE huurlingen kunnen zich op dit moment niet verplaatsen.",
	L"Voertuig is te beschadigd!",
	L"Let op dat maar twee huurlingen milities in een sector mogen trainen.",
	L"De robot kan zich zonder bediening niet verplaatsen. Plaats ze in hetzelfde team.",
	L"Items cannot be moved to %s, as no valid dropoff point was found. Please enter map to resolve this issue.",	// TODO.Translate
// 51-55
	L"%d items moved from %s to %s",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Klik nogmaals op de bestemming om de route te bevestigen, of klik op een andere sector om meer routepunten te plaatsen.",
	L"Route bevestigd.",
	L"Bestemming onveranderd.",
	L"Reis afgebroken.",
	L"Reis verkort.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Klik op de sector waar de huurlingen in plaats daarvan moeten arriveren.",
	L"OK. Arriverende huurlingen worden afgezet in %s",
	L"Huurlingen kunnen hier niet ingevlogen worden, het luchtruim is onveilig!",
	L"Afgebroken. Aankomst-sector onveranderd",
	L"Luchtruim boven %s is niet langer veilig! Aankomst-sector is verplaatst naar %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Naar Inventaris (|E|n|t|e|r)",
	L"Gooi Item Weg",
	L"Verlaat Inventaris (|E|n|t|e|r)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Laat %s zijn uitrusting achterlaten waar hij nu is (%s) of in (%s) bij het nemen van de vlucht?",
	L"%s gaat binnenkort weg en laat zijn uitrusting achter in %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Laat %s haar uitrusting achterlaten waar ze nu is (%s) of in (%s) bij het nemen van de vlucht?",
	L"%s gaat binnenkort weg en laat haar uitrusting achter in %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"'s contract is geëindigd, hij is dus naar huis.",	// merc's contract is over and has departed
	L"'s contract is geëindigd, ze is dus naar huis.", 		// merc's contract is over and has departed
	L"'s contract is opgezegd, hij is dus weg.", 		// merc's contract has been terminated
	L"'s contract is opgezegd, ze is dus weg.",		// merc's contract has been terminated
	L"M.E.R.C. krijgt nog teveel geld van je, %s is dus weggegaan.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

STR16 pImpPopUpStrings[] =
{
	L"Ongeldige Autorisatiecode",
	L"Je wil het gehele persoonlijkheidsonderzoek te herstarten. Zeker weten?",
	L"Vul alsjeblieft de volledige naam en geslacht in",
	L"Voortijdig onderzoek van je financiële status wijst uit dat je een persoonlijksheidsonderzoek niet kunt betalen.",
	L"Geen geldige optie op dit moment.",
	L"Om een nauwkeurig profiel te maken, moet je ruimte hebben voor tenminste één teamlid.",
	L"Profiel is al gemaakt.",
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
	L"Info", 			// about the IMP site ("About Us")
	L"BEGIN", 			// begin profiling ("BEGIN")
	L"Persoonlijkheid", 		// personality section ("Personality")
	L"Eigenschappen", 		// personal stats/attributes section ("Attributes")
	L"Appearance", 			// changed from portrait
	L"Stem %d", 			// the voice selection ("Voice %d")
	L"OK", 			// done profiling ("Done")
	L"Opnieuw", 		// start over profiling ("Start Over")
	L"Ja, ik kies het geselecteerde antwoord.",		// ("Yes, I choose the highlighted answer.")
	L"Ja", 
	L"Nee",
	L"OK", 			// finished answering questions
	L"Vor.", 			// previous question..abbreviated form
	L"Vol.", 			// next question
	L"JA ZEKER.", 		// yes, I am certain ("YES, I AM.")
	L"NEE, IK WIL OPNIEUW BEGINNEN.",	// no, I want to start over the profiling process ("NO, I WANT TO START OVER.")
	L"JA, ZEKER.",		// ("YES, I DO.")
	L"NEE",
	L"Terug", 					// back one page
	L"Stop", 					// cancel selection
	L"Ja, zeker weten.",			// ("Yes, I am certain.")
	L"Nee, laat me nog eens kijken.",	// ("No, let me have another look.")
	L"Registratie", 			// the IMP site registry..when name and gender is selected
	L"Analyseren", 			// analyzing your profile results
	L"OK",
	L"Character", // Change from "Voice"
	L"None",
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
	L"Bestanden Bekijken",		// ("File Viewer")
};

STR16 pFilesSenderList[] =
{
#ifdef JA2UB
	L"Int. Verslag",		// the recon report sent to the player. Recon is an abbreviation for reconissance
#else
	L"Int. Verslag",		// the recon report sent to the player. Recon is an abbreviation for reconissance
#endif
	L"Intercept.#1",		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"Intercept.#2",		// second intercept file
	L"Intercept.#3",		// third intercept file
	L"Intercept.#4",		// fourth intercept file ("Intercept #4")
	L"Intercept.#5",		// fifth intercept file
	L"Intercept.#6",		// sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Geschiedenis",
};

STR16 pHistoryHeaders[] =
{
	L"Dag", 			// the day the history event occurred
	L"Pag.", 			// the current page in the history report we are in
	L"Dag", 			// the days the history report occurs over
	L"Locatie", 		// location (in sector) the event occurred
	L"Geb.", 			// the event label
};

/*
// Externalized to "TableData\History.xml"
// various history events
// THESE STRINGS ARE "HISTORY LOG" STRINGS AND THEIR LENGTH IS VERY LIMITED.
// PLEASE BE MINDFUL OF THE LENGTH OF THESE STRINGS. ONE WAY TO "TEST" THIS
// IS TO TURN "CHEAT MODE" ON AND USE CONTROL-R IN THE TACTICAL SCREEN, THEN
// GO INTO THE LAPTOP/HISTORY LOG AND CHECK OUT THE STRINGS. CONTROL-R INSERTS
// MANY (NOT ALL) OF THE STRINGS IN THE FOLLOWING LIST INTO THE GAME.
STR16 pHistoryStrings[] =
{
	L"",										// leave this line blank
	//1-5
	L"%s ingehuurd via A.I.M.",						// merc was hired from the aim site
	L"%s ingehuurd via M.E.R.C.",						// merc was hired from the merc site
	L"%s gedood.", 								// merc was killed
	L"Facturen betaald bij M.E.R.C.",					// paid outstanding bills at MERC
	L"Opdracht van Enrico Chivaldori geaccepteerd.",		// ("Accepted Assignment From Enrico Chivaldori")
	//6-10
	L"IMP Profiel Klaar",							// ("IMP Profile Generated")
	L"Verzekeringspolis gekocht voor %s.",				// insurance contract purchased
	L"Verzekeringspolis afgebroken van %s.",				// insurance contract canceled
	L"Uitbetaling Verzekeringspolis %s.",				// insurance claim payout for merc
	L"%s's contract verlengd met 1 dag.",				// Extented "mercs name"'s for a day
	//11-15
	L"%s's contract verlengd met 1 week.",				// Extented "mercs name"'s for a week
	L"%s's contract verlengd met 2 weken.",				// Extented "mercs name"'s 2 weeks
	L"%s is ontslagen.",							// "merc's name" was dismissed.
	L"%s gestopt.",								// "merc's name" quit.
	L"zoektocht gestart.",							// a particular quest started
	//16-20
	L"zoektocht afgesloten.",						// ("quest completed.")
	L"Gepraat met hoofdmijnwerker van %s",				// talked to head miner of town
	L"%s bevrijd",								// ("Liberated %s")
	L"Vals gespeeld",								// ("Cheat Used")
	L"Voedsel zou morgen in Omerta moeten zijn",			// ("Food should be in Omerta by tomorrow")
	//21-25
	L"%s weggegaan, wordt Daryl Hick's vrouw",			// ("%s left team to become Daryl Hick's wife")
	L"%s's contract afgelopen.",						// ("%s's contract expired.")
	L"%s aangenomen.",							// ("%s was recruited.")
	L"Enrico klaagde over de voortgang",				// ("Enrico complained about lack of progress")
	L"Strijd gewonnen",							// ("Battle won")
	//26-30
	L"%s mijn raakt uitgeput",						// ("%s mine started running out of ore")
	L"%s mijn is uitgeput",							// ("%s mine ran out of ore")
	L"%s mijn is gesloten",							// ("%s mine was shut down")
	L"%s mijn heropend",							// ("%s mine was reopened")
	L"Info verkregen over gevangenis Tixa.",				// ("Found out about a prison called Tixa.")
	//31-35
	L"Van geheime wapenfabriek gehoord, Orta genaamd.",		// ("Heard about a secret weapons plant called Orta.")
	L"Onderzoeker in Orta geeft wat raketwerpers.",			// ("Scientist in Orta donated a slew of rocket rifles.")
	L"Koningin Deidranna kickt op lijken.",				// ("Queen Deidranna has a use for dead bodies.")
	L"Frank vertelde over knokwedstrijden in San Mona.",		// ("Frank talked about fighting matches in San Mona.")
	L"Een patiënt dacht dat ie iets in de mijnen zag.",		// ("A patient thinks he saw something in the mines.")
	//36-40
	L"Pers. ontmoet; Devin - verkoopt explosieven.",		// ("Met someone named Devin - he sells explosives.")
	L"Beroemde ex-AIM huurling Mike ontmoet!",			// ("Ran into the famous ex-AIM merc Mike!")
	L"Tony ontmoet - handelt in wapens.",				// ("Met Tony - he deals in arms.")
	L"Raketwerper gekregen van Serg. Krott.",				// ("Got a rocket rifle from Sergeant Krott.")
	L"Kyle akte gegeven van Angel's leerwinkel.",			// ("Gave Kyle the deed to Angel's leather shop.")
	//41-45
	L"Madlab bood aan robot te bouwen.",				// ("Madlab offered to build a robot.")
	L"Gabby maakt superbrouwsel tegen beesten.",			// ("Gabby can make stealth concoction for bugs.")
	L"Keith is er mee opgehouden.",					// ("Keith is out of business.")
	L"Howard geeft Koningin Deidranna cyanide.",			// ("Howard provided cyanide to Queen Deidranna.")
	L"Keith ontmoet - handelaar in Cambria.",				// ("Met Keith - all purpose dealer in Cambria.")
	//46-50
	L"Howard ontmoet - medicijnendealer in Balime",			// ("Met Howard - deals pharmaceuticals in Balime")
	L"Perko ontmoet - heeft reparatiebedrijfje.",			// ("Met Perko - runs a small repair business.")
	L"Sam van Balime ontmoet - verkoopt ijzerwaren.",		// ("Met Sam of Balime - runs a hardware shop.")
	L"Franz verkoopt elektronica en andere dingen.",		// ("Franz deals in electronics and other goods.")
	L"Arnold runt reparatiezaak in Grumm.",				// ("Arnold runs a repair shop in Grumm.")
	//51-55
	L"Fredo repareert elektronica in Grumm.",				// ("Fredo repairs electronics in Grumm.")
	L"Van rijke vent in Balime donatie gekregen.",			// ("Received donation from rich guy in Balime.")
	L"Schroothandelaar Jake ontmoet.",					// ("Met a junkyard dealer named Jake.")
	L"Vaag iemand gaf ons elektronische sleutelkaart.",		// ("Some bum gave us an electronic keycard.")
	L"Walter omgekocht om kelderdeur open te maken.",		// ("Bribed Walter to unlock the door to the basement.")
	//56-60
	L"Als Dave gas heeft, geeft hij deze weg.",			// ("If Dave has gas, he'll provide free fillups.")
	L"Geslijmd met Pablo.",							// ("Greased Pablo's palms.")
	L"Kingpin bewaard geld in San Mona mine.",			// ("Kingpin keeps money in San Mona mine.")
	L"%s heeft Extreme Fighting gewonnen",				// ("%s won Extreme Fighting match")
	L"%s heeft Extreme Fighting verloren",				// ("%s lost Extreme Fighting match")
	//61-65
	L"%s gediskwalificeerd v. Extreme Fighting",			// ("%s was disqualified in Extreme Fighting")
	L"Veel geld gevonden in een verlaten mijn.",			// ("Found a lot of money stashed in the abandoned mine.")
	L"Huurmoordenaar van Kingpin ontdekt.",				// ("Encountered assassin sent by Kingpin.")
	L"Controle over sector verloren",					//ENEMY_INVASION_CODE ("Lost control of sector")
	L"Sector verdedigd",							// ("Defended sector")
	//66-70
	L"Strijd verloren",					//ENEMY_ENCOUNTER_CODE ("Lost battle")
	L"Fatale val",						//ENEMY_AMBUSH_CODE ("Fatal ambush")
	L"Vijandige val weggevaagd",				// ("Wiped out enemy ambush")
	L"Aanval niet gelukt",					//ENTERING_ENEMY_SECTOR_CODE ("Unsuccessful attack")
	L"Aanval gelukt!",					// ("Successful attack!")
	//71-75
	L"Beesten vielen aan",					//CREATURE_ATTACK_CODE ("Creatures attacked")
	L"Gedood door bloodcats",				//BLOODCAT_AMBUSH_CODE ("Killed by bloodcats")
	L"Afgeslacht door bloodcats",				// ("Slaughtered bloodcats")
	L"%s was gedood",						// ("%s was killed")
	L"Carmen kop v.e. terrorist gegeven",		// ("Gave Carmen a terrorist's head")
	//76-80
	L"Slay vertrok",						// ("Slay left")
	L"%s vermoord",						// ("Killed %s")
	L"Met Waldo - aircraft mechanic.",
	L"Helicopter repairs started. Estimated time: %d hour(s).",
};
*/

STR16 pHistoryLocations[] =
{
	L"Nvt",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"E-mail",
	L"Web",
	L"Financieel",
	L"Dossiers",
	L"Historie",
	L"Bestanden",
	L"Afsluiten",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Ray's",
	L"I.M.P",
	L"M.E.R.C.",
	L"Mortuarium",
	L"Bloemist",
	L"Verzekering",
	L"Stop",
	L"Encyclopedia",
	L"Briefing Room",
	L"Campaign History",	// TODO.Translate
	L"MeLoDY",
	L"WHO",
	L"Kerberus",
	L"Militia Overview",	// TODO.Translate
	L"R.I.S.",
};

STR16 pBookmarkTitle[] =
{
	L"Bladwijzer",
	L"Rechter muisklik om dit menu op te roepen.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Laden",
	L"Herladen",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Neem", 			// take money from merc
	L"Geef", 			// give money to merc
	L"Stop", 			// cancel transaction
	L"Leeg", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Maak over $", 		// transfer money to merc -- short form
	L"Info", 			// view stats of the merc
	L"Inventaris", 			// view the inventory of the merc
	L"Werk",
};

STR16 sATMText[ ]=
{
	L"Overmaken geld?", 		// transfer funds to merc?
	L"Ok?", 			// are we certain?
	L"Geef bedrag", 		// enter the amount you want to transfer to merc
	L"Geef type", 		// select the type of transfer to merc
	L"Onvoldoende saldo", 	// not enough money to transfer to merc
	L"Bedrag moet veelvoud zijn van $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Fout",
	L"Server heeft geen DNS ingang.",	
	L"Controleer URL adres en probeer opnieuw.",
	L"OK",
	L"Periodieke verbinding met host. Houdt rekening met lange wachttijden.",
};


STR16 pPersonnelString[] =
{
	L"Huurlingen:", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. Leden",
	L"A.I.M. Portretten",		// a mug shot is another name for a portrait
	L"A.I.M. Sorteer",
	L"A.I.M.",
	L"A.I.M. Veteranen",
	L"A.I.M. Regelement",
	L"A.I.M. Geschiedenis",
	L"A.I.M. Links",
	L"M.E.R.C.",
	L"M.E.R.C. Rekeningen",
	L"M.E.R.C. Registratie",
	L"M.E.R.C. Index",
	L"Bobby Ray's",
	L"Bobby Ray's - Wapens",
	L"Bobby Ray's - Munitie",
	L"Bobby Ray's - Pantsering",
	L"Bobby Ray's - Diversen",					//misc is an abbreviation for miscellaneous				
	L"Bobby Ray's - Gebruikt",
	L"Bobby Ray's - Mail Order",
	L"I.M.P.",
	L"I.M.P.",
	L"United Floral Service",
	L"United Floral Service - Etalage",
	L"United Floral Service - Bestelformulier",
	L"United Floral Service - Kaart Etalage",
	L"Malleus, Incus & Stapes Verzekeringen",
	L"Informatie",
	L"Contract",
	L"Opmerkingen",
	L"McGillicutty's Mortuarium",
	L"",
	L"URL niet gevonden.",
	L"%s Press Council - Conflict Summary",	// TODO.Translate
	L"%s Press Council - Battle Reports",
	L"%s Press Council - Latest News",
	L"%s Press Council - About us",
	L"Mercs Love or Dislike You - About us",
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
	L"Bobby Ray's - Recent Shipments",
	L"Encyclopedia",
	L"Encyclopedia - Data",
	L"Briefing Room",
	L"Briefing Room - Data",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Help",
	L"Klik opnieuw voor Bookmarks.",
};

STR16 pLaptopTitles[] =
{
	L"E-Mail",
	L"Bestanden bekijken",
	L"Persoonlijk",
	L"Boekhouder Plus",
	L"Geschiedenis",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Omgekomen tijdens gevechten",
	L"Weggestuurd",
	L"Anders",
	L"Getrouwd",
	L"Contract Afgelopen",
	L"Gestopt",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"Huidig Team",
	L"Vertrekken",
	L"Dag. Kosten:",
	L"Hoogste Kosten:",
	L"Laagste Kosten:",
	L"Omgekomen tijdens gevechten:",
	L"Weggestuurd:",
	L"Anders:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Laagste",
	L"Gemiddeld",
	L"Hoogste",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"GZND",
	L"BEW",
	L"HAN",
	L"KRA",
	L"LDR",
	L"WIJ",
	L"NIV",
	L"TREF",
	L"MECH",
	L"EXPL",
	L"MED",
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
	L"Contract",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Doorgaan",
	L"Stop",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Je bent verslagen in deze sector!",
	L"De vijand, geen genade kennende, slacht ieder teamlid af!",
	L"Je bewusteloze teamleden zijn gevangen genomen!",
	L"Je teamleden zijn gevangen genomen door de vijand.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Vorige",
	L"Volgende",
	L"OK",
	L"Leeg",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Vorige", 
	L"Volgende",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Autoriseer",
	L"Thuis",
	L"Rekening#:",
	L"Huurl.",
	L"Dagen",
	L"Tarief",	//5
	L"Prijs",
	L"Totaal:",
	L"Weet je zeker de betaling van %s te autoriseren?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
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
	L"Gezondheid",
	L"Beweeglijkheid",
	L"Handigheid",
	L"Kracht",
	L"Leiderschap",
	L"Wijsheid",
	L"Ervaringsniveau",
	L"Trefzekerheid",
	L"Technisch",
	L"Explosieven",
	L"Medisch",

	L"Vorige",
	L"Huur",
	L"Volgende",
	L"Extra Info",
	L"Thuis",
	L"Ingehuurd",
	L"Salaris:",
	L"Per Dag",
	L"Gear:", // TODO.Translate
	L"Totaal:",
	L"Overleden",

	L"Je team bestaat al uit huurlingen.",
	L"Koop Uitrusting?",
	L"Niet beschikbaar",
	L"Unsettled Bills", // TODO.Translate
	L"Bio", // TODO.Translate
	L"Inv",							
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Open Rekening",
	L"Afbreken",
	L"Je hebt geen rekening. Wil je er één openen?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, oprichter en bezitter",
	L"Om een rekening te open, klik hier",
	L"Klik hier om rekening te bekijken",
	L"Klik hier om bestanden in te zien",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer failed. No funds available.",	// TODO.Translate
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"McGillicutty's Mortuarium: Helpt families rouwen sinds 1983.",
	L"Begrafenisondernemer en voormalig A.I.M. huurling Murray \"Pops\" McGillicutty is een kundig en ervaren begrafenisondernemer.",
	L"Pops weet hoe moeilijk de dood kan zijn, in heel zijn leven heeft hij te maken gehad met de dood en sterfgevallen.",
	L"McGillicutty's Mortuarium biedt een breed scala aan stervensbegeleiding, van een schouder om uit te huilen tot recontructie van misvormde overblijfselen.",
	L"Laat McGillicutty's Mortuarium u helpen en laat uw dierbaren zacht rusten.",

	// Text for the various links available at the bottom of the page
	L"STUUR BLOEMEN",
	L"DOODSKIST & URN COLLECTIE",
	L"CREMATIE SERVICE",
	L"SERVICES",
	L"ETIQUETTE",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Helaas is deze pagina nog niet voltooid door een sterfgeval in de familie. Afhankelijk van de laatste wil en uitbetaling van de beschikbare activa wordt de pagina zo snel mogelijk voltooid.",
	L"Ons medeleven gaat uit naar jou, tijdens deze probeerperiode. Kom nog eens langs.",
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Etalage",

	//Address of United Florist

	L"\"We brengen overal langs\"",
	L"1-555-SCENT-ME",
	L"333 NoseGay Dr, Seedy City, CA USA 90210",
	L"http://www.scent-me.com",

	// detail of the florist page

	L"We zijn snel en efficiënt!",
	L"Volgende dag gebracht, wereldwijd, gegarandeerd. Enkele beperkingen zijn van toepassing.",
	L"Laagste prijs in de wereld, gegarandeerd!",
	L"Toon ons een lagere geadverteerde prijs voor een regeling en ontvang gratis een dozijn rozen.",
	L"Flora, Fauna & Bloemen sinds 1981.",
	L"Onze onderscheiden ex-bommenwerperpiloten droppen je boeket binnen een tien kilometer radius van de gevraagde locatie. Altijd!",
	L"Laat ons al je bloemenfantasieën waarmaken.",
	L"Laat Bruce, onze wereldberoemde bloemist, de verste bloemen met de hoogste kwaliteit uit onze eigen kassen uitzoeken.",
	L"En onthoudt, als we het niet hebben, kunnen we het kweken - Snel!",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Terug",
	L"Verstuur",
	L"Leeg",
	L"Etalage",

	L"Naam vh Boeket:",
	L"Prijs:",			//5
	L"Ordernummer:",
	L"Bezorgingsdatum",
	L"volgende dag",
	L"komt wanneer het komt",
	L"Locatie Bezorging",			//10
	L"Extra Service",
	L"Geplet Boeket($10)",
	L"Zwarte Rozen ($20)",
	L"Verlept Boeket($10)",
	L"Fruitcake (indien beschikbaar)($10)",		//15
	L"Persoonlijk Bericht:",
	L"Wegens de grootte kaarten, mogen je berichten niet langer zijn dan 75 karakters.",
	L"...of selecteer er één van de onze",

	L"STANDAARDKAARTEN",
	L"Factuurinformatie",	//20

	//The text that goes beside the area where the user can enter their name

	L"Naam:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Back",	//abbreviation for previous
	L"Next",	//abbreviation for next

	L"Klik op de selectie die je wil bestellen.",	
	L"Let op: er geldt een extra tarief van $10 voor geplette en verlepte boeketten.",

	//text on the button

	L"Home",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Klik op je selectie",
	L"Terug",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Bestelformulier",				//Title of the page
	L"Hvl",					// The number of items ordered
	L"Gewicht(%s)",			// The weight of the item
	L"Itemnaam",				// The name of the item
	L"Prijs unit",				// the item's weight
	L"Totaal",				//5	// The total price of all of items of the same type
	L"Sub-Totaal",				// The sub total of all the item totals added
	L"Porto (Zie Bezorgloc.)",		// S&H is an acronym for Shipping and Handling 
	L"Eindtotaal",			// The grand total of all item totals + the shipping and handling
	L"Bezorglocatie",				
	L"Verzendingssnelheid",			//10	// See below
	L"Kosten (per %s.)",			// The cost to ship the items
	L"Nacht-Express",			// Gets deliverd the next day
	L"2 Werkdagen",			// Gets delivered in 2 days
	L"Standaard Service",			// Gets delivered in 3 days
	L"Order Leegmaken",//15			// Clears the order page
	L"Accept. Order",			// Accept the order
	L"Terug",				// text on the button that returns to the previous page
	L"Home",				// Text on the button that returns to the home page
	L"* Duidt op Gebruikte Items",		// Disclaimer stating that the item is used
	L"Je kunt dit niet betalen.",		//20	// A popup message that to warn of not enough money
	L"<GEEN>",				// Gets displayed when there is no valid city selected
	L"Weet je zeker dat je de bestelling wil sturen naar %s?",		// A popup that asks if the city selected is the correct one
	L"Gewicht Pakket**",			// Displays the weight of the package
	L"** Min. Gew.",				// Disclaimer states that there is a minimum weight for the package
	L"Zendingen",	
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
	L"Bestelling",				// Title
	// instructions on how to order
	L"Klik op de item(s). Voor meer dan één, blijf dan klikken. Rechtsklikken voor minder. Als je alles geselecteerd hebt, dat je wil bestellen, ga dan naar het bestelformulier.",

	//Text on the buttons to go the various links

	L"Vorige Items",		// 
	L"Wapens", 			//3
	L"Munitie",			//4
	L"Pantser",			//5
	L"Diversen",			//6	//misc is an abbreviation for miscellaneous
	L"Gebruikt",			//7
	L"Meer Items",
	L"BESTELFORMULIER",
	L"Home",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Je team heeft", //11
	L"wapen(s) gebruik makende van deze munitie", //12

	//The following lines provide information on the items

	L"Gewicht:",		// Weight of all the items of the same type
	L"Kal:",			// the caliber of the gun
	L"Mag:",			// number of rounds of ammo the Magazine can hold
	L"Afs:",			// The range of the gun
	L"Sch:",			// Damage of the weapon	
	L"ROF:",			// Weapon's Rate Of Fire, acronym ROF
	L"AP:",				// Weapon's Action Points, acronym AP
	L"Stun:",			// Weapon's Stun Damage
	L"Protect:",		// Armour's Protection
	L"Camo:",			// Armour's Camouflage
	L"Kost:",			// Cost of the item
	L"Aanwezig:",			// The number of items still in the store's inventory
	L"# Besteld:",		// The number of items on order
	L"Beschadigd",			// If the item is damaged
	L"Gewicht:",			// the Weight of the item
	L"SubTotaal:",			// The total cost of all items on order
	L"* %% Functioneel",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Verdraaid! Dit on-line bestelformulier accepteert maar " ,//First part
	L" items per keer. Als je meer wil bestellen (en dat hopen we), plaats dan afzonderlijke orders en accepteer onze excuses.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Sorry. We hebben niet meer van die zaken in het magazijn. Probeer het later nog eens.",

	//A popup that tells the user that the store is temporarily sold out

	L"Sorry, alle items van dat type zijn nu uitverkocht.",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Hier moet je zijn voor de nieuwste en beste wapens en militaire goederen",
	L"We kunnen de perfecte oplossing vinden voor elke explosiebehoefte",
	L"Gebruikte en opgeknapte items",

	//Text for the various links to the sub pages

	L"Diversen",
	L"WAPENS",
	L"MUNITIE",		//5
	L"PANTSER",

	//Details on the web site

	L"Als wij het niet verkopen, dan kun je het nergens krijgen!",
	L"Under construction",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"A.I.M. Leden",				// Title
	// Title for the way to sort
	L"Sort. op:",					

	// sort by...

	L"Prijs",
	L"Ervaring",
	L"Trefzekerheid",
	L"Technisch",
	L"Explosieven",
	L"Medisch",
	L"Gezondheid",
	L"Beweeglijkheid",
	L"Handigheid",
	L"Kracht",
	L"Leiderschap",
	L"Wijsheid",
	L"Naam",

	//Text of the links to other AIM pages

	L"Bekijk portretfotoindex van huurlingen",
	L"Bekijk het huurlingendossier",
	L"Bekijk de A.I.M. Veteranen",

	// text to display how the entries will be sorted

	L"Oplopend",
	L"Aflopend",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Previous",
	L"AIM HomePage", 
	L"Index Regels",
	L"Next",
	L"Oneens",
	L"Mee eens",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Klik Links",
	L"voor Verbinding met Huurl.",
	L"Klik Rechts",
	L"voor Portretfotoindex.",
};					

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Gezondheid",										
	L"Beweeglijkheid",										
	L"Handigheid",									
	L"Kracht",
	L"Leiderschap",
	L"Wijsheid",
	L"Ervaringsniveau",
	L"Trefzekerheid",
	L"Technisch",
	L"Explosieven",
	L"Medisch",				//10

	// the contract expenses' area

	L"Tarief",							
	L"Contract",				
	L"een dag",					
	L"een week",
	L"twee weken",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Previous",
	L"Contact",
	L"Next",

	L"Extra Info",				// Title for the additional info for the merc's bio
	L"Actieve Leden",		//20		// Title of the page
	L"Aanv. Uitrusting:",				// Displays the optional gear cost
	L"gear",			//"gear",		//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's // TODO.Translate
	L"MEDISCHE aanbetaling nodig",			// If the merc required a medical deposit, this is displayed
	L"Uitrusting 1",				// Text on Starting Gear Selection Button 1  // TODO.Translate
	L"Uitrusting 2",				// Text on Starting Gear Selection Button 2
	L"Uitrusting 3",				// Text on Starting Gear Selection Button 3
	L"Uitrusting 4",				// Text on Starting Gear Selection Button 4
	L"Uitrusting 5",				// Text on Starting Gear Selection Button 5
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"Contractkosten:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"Een Dag",										
	L"Een Week",
	L"Twee Weken",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Geen Uitrusting",
	L"Koop Uitrusting",

	// Text on the Buttons

	L"HUUR IN",			// to actually hire the merc
	L"STOP",				// go back to the previous menu
	L"VOORWAARDEN",				// go to menu in which you can hire the merc
	L"OPHANGEN",			// stops talking with the merc
	L"OK",									
	L"STUUR BERICHT",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Video Conference met",		
	L"Verbinding maken. . .",

	L"+ med. depo",			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"BEDRAG OVERGEBOEKT",	// You hired the merc
	L"OVERMAKEN NIET MOGELIJK",			// Player doesn't have enough money, message 1
	L"ONVOLDOENDE GELD",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"Op missie",											
	L"Laat a.u.b. bericht achter",
	L"Overleden",

	//If you try to hire more mercs than game can support

	L"Je team bestaat al uit huurlingen.",

	L"Opgenomen bericht",
	L"Bericht opgenomen",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M. Links",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M. Geschiedenis",					//Title

	// Text on the buttons at the bottom of the page

	L"Previous",
	L"Home", 
	L"A.I.M. Veteranen",
	L"Next",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Prijs",
	L"Ervaring",
	L"Trefzekerheid",
	L"Technisch",
	L"Explosieven",
	L"Medisch",
	L"Gezondheid",
	L"Beweeglijkheid",
	L"Handigheid",
	L"Kracht",
	L"Leiderschap",
	L"Wijsheid",
	L"Naam",

	// The title of the page, the above text gets added at the end of this text

	L"A.I.M. Leden Oplopend Gesorteerd op %s",
	L"A.I.M. Leden Aflopend Gesorteerd op %s",

	// Instructions to the players on what to do

	L"Klik Links",
	L"om Huurling te Selecteren",			//10
	L"Klik Rechts",
	L"voor Sorteeropties",

	// Gets displayed on top of the merc's portrait if they are...

	L"Afwezig",
	L"Overleden",						//14
	L"Op missie",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"PAG. 1",
	L"PAG. 2",
	L"PAG. 3",

	L"A.I.M. Veteranen",	// Title of the page

	L"OK",			// Stops displaying information on selected merc
	L"Next page",	// TODO.Translate
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers

	L"A.I.M. en A.I.M.-logo zijn geregistreerde handelsmerken in de meeste landen.",
	L"Dus denk er niet aan om ons te kopiëren.",
	L"Copyright 1998-1999 A.I.M., Ltd.  All rights reserved.",

	//Text for an advertisement that gets displayed on the AIM page

	L"United Floral Service",
	L"\"We droppen overal\"",				//10
	L"Doe het goed",
	L"... de eerste keer",
	L"Wapens en zo, als we het niet hebben, dan heb je het ook niet nodig.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"Home",
	L"Leden",
	L"Veteranen",
	L"Regels",
	L"Geschiedenis",
	L"Links",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"HANDELSWAAR OP VOORRAAD",		//Header for the merchandise available
	L"PAG.",				//The current store inventory page being displayed
	L"TOTALE KOSTEN",				//The total cost of the the items in the Dealer inventory area
	L"TOTALE WAARDE",			//The total value of items player wishes to sell
	L"EVALUEER",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSACTIE",			//Button text which completes the deal. Makes the transaction.
	L"OK",				//Text for the button which will leave the shopkeeper interface.
	L"REP. KOSTEN",			//The amount the dealer will charge to repair the merc's goods
	L"1 UUR",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d UREN",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"GEREPAREERD",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Er is geen ruimte meer.",	//Message box that tells the user there is no more room to put there stuff
	L"%d MINUTEN",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Drop Item op Grond.",
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
	L"Neem",					// Take money from the player
	L"Geef",					// Give money to the player
	L"Stop",					// Cancel the transfer
	L"Leeg",					// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Selecteer Type",			// tells the user to select either to give or take from the merc
	L"Voer Bedrag In",			// Enter the amount to transfer
	L"Maak Geld over naar Huurl.",		// Giving money to the merc
	L"Maak Geld over van Huurl.",		// Taking money from the merc
	L"Onvoldoende geld",			// Not enough money to transfer
	L"Saldo",				// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Wil je %s aftrekken van je hoofdrekening om het verschil op te vangen?",
	L"Niet genoeg geld. Je komt %s tekort",
	L"Wil je %s aftrekken van je hoofdrekening om de kosten te dekken?",
	L"Vraag de dealer om de transactie te starten",
	L"Vraag de dealer om de gesel. items te repareren",
	L"Einde conversatie",
	L"Huidige Saldo",

	L"Do you want to transfer %s Intel to cover the difference?",	// TODO.Translate
	L"Do you want to transfer %s Intel to cover the cost?",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Spel Bewaren",
	L"Spel Laden",
	L"Stop",
	L"Next",
	L"Prev",
	L"OK",

	//Text above the slider bars
	L"Effecten",
	L"Spraak",
	L"Muziek",

	//Confirmation pop when the user selects..
	L"Spel verlaten en terugkeren naar hoofdmenu?",

	L"Je hebt of de Spraakoptie nodig of de ondertiteling.",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Spel Bewaren",
	L"Spel Laden",
	L"Stop",
	L"Bewaren Gesel.",
	L"Laden Gesel.",

	L"Spel Bewaren voltooid",
	L"FOUT bij bewaren spel!",
	L"Spel laden succesvol",
	L"FOUT bij laden spel!",

	L"De spelversie van het bewaarde spel verschilt van de huidige versie. Waarschijnlijk is het veilig om door te gaan. Doorgaan?",
	L"De bewaarde spelen zijn waarschijnlijk ongeldig. Deze verwijderen?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Save version has changed. Please report if there any problems. Continue?",
#else
	L"Attempting to load an older version save. Automatically update and load the save?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Save version and game version have changed. Please report if there are any problems. Continue?",
#else
	L"Attempting to load an older version save. Automatically update and load the save?",
#endif

	L"Weet je zeker dat je het spel in slot #%d wil overschrijven?",
	L"Wil je het spel laden van slot #",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Er is te weinig ruimte op de harde schijf. Er is maar %d MB vrij en Jagged heeft tenminste %d MB nodig.",

	L"Bewaren",			//When saving a game, a message box with this string appears on the screen

	L"Normale Wapens",
	L"Stapels Wapens",
	L"Realistische stijl",
	L"SF stijl",

	L"Moeilijkheid",
	L"Platinum Mode", //Placeholder English

	L"Bobby Ray Quality",
	L"Good",
	L"Great",
	L"Excellent",
	L"Awesome",

	L"New Inventory does not work in 640x480 screen resolution. Please increase the screen resolution and try again.",
	L"New Inventory does not work from the default 'Data' folder.",

	L"The squad size from the savegame is not supported by the current screen resolution. Please increase the screen resolution and try again.",
	L"Bobby Ray Quantity",// TODO.Translate
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Kaartniveau",
	L"Je hebt geen militie. Je moet stadsburgers trainen om een stadsmilitie te krijgen.",
	L"Dagelijks Inkomen",
	L"Huurling heeft levensverzekering",
	L"%s is niet moe.",
	L"%s is bezig en kan niet slapen",
	L"%s is te moe, probeer het later nog eens.",
	L"%s is aan het rijden.",
	L"Team kan niet reizen met een slapende huurling.",

	// stuff for contracts	
	L"Je kunt wel het contract betalen, maar je hebt geen geld meer om de levensverzekering van de huurling te betalen.",
	L"%s verzekeringspremie kost %s voor %d extra dag(en). Wil je betalen?",
	L"Inventaris Sector",
	L"Huurling heeft medische kosten.",
	
	// other items	
	L"Medici",		// people acting a field medics and bandaging wounded mercs 
	L"Patiënten", // people who are being bandaged by a medic 
	L"OK", // Continue on with the game after autobandage is complete 
	L"Stop", // Stop autobandaging of patients by medics now 
	L"Sorry. Optie niet mogelijk in deze demo.", // informs player this option/button has been disabled in the demo 
	L"%s heeft geen reparatie-kit.",
	L"%s heeft geen medische kit.",
	L"Er zijn nu niet genoeg mensen die getraind willen worden.",
	L"%s is vol met milities.",
	L"Huurling heeft eindig contract.",
	L"Contract Huurling is niet verzekerd",
	L"Map Overview",		// 24
	
	// Flugente: disease texts describing what a map view does TODO.Translate
	L"This view shows how many rotting corpses are in a sector. The white number are corpses, the green number is accumulated disease, the sector colour indicates how widespread it is. GREY= No disease known of. GREEN to RED = escalating levels of disease.",	// TODO.Translate

	// Flugente: weather texts describing what a map view does
	L"This view shows current weather. No colour=Sunny. CYAN=Rain. BLUE=Thunderstorm. ORANGE=Sandstorm. WHITE=Snow.",	// TODO.Translate

	// Flugente: describe what intel map view does
	L"This view shows which sectors relevant what ongoing quests. Some data bought with intel is also shown here.", // TODO.Translate
};


STR16 pLandMarkInSectorString[] =
{
	L"Team %d is heeft iemand ontdekt in sector %s",
	L"Team %s is heeft iemand ontdekt in sector %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Een stadsmilitie trainen kost $",		// telling player how much it will cost
	L"Uitgave goedkeuren?", // asking player if they wish to pay the amount requested
	L"je kunt dit niet betalen.", // telling the player they can't afford to train this town
	L"Doorgaan met militie trainen %s (%s %d)?", // continue training this town?

	L"Kosten $", // the cost in dollars to train militia
	L"( J/N )",   // abbreviated yes/no
	L"",	// unused
	L"Stadsmilities trainen in %d sectors kost $ %d. %s", // cost to train sveral sectors at once

	L"Je kunt de $%d niet betalen om de stadsmilitie hier te trainen.",
	L"%s heeft een loyaliteit nodig van %d procent om door te gaan met milities trainen.",
	L"Je kunt de militie in %s niet meer trainen.",
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
	L"Je kunt maximaal $20.000 in één keer opnemen.",
	L"Weet je zeker dat je %s wil storten op je rekening?",
};

STR16	gzCopyrightText[] = 
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd.  All rights reserved.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Spraak",
	L"Bevestigingen uit",
	L"Ondertitels",
	L"Wacht bij tekst-dialogen",
	L"Rook Animeren",
	L"Bloedsporen Tonen",
	L"Cursor Niet Bewegen",
	L"Oude Selectiemethode",
	L"Toon reisroute",
	L"Toon Missers",
	L"Bevestiging Real-Time",
	L"Slaap/wakker-berichten",
	L"Metrieke Stelsel",
	L"Huurling Oplichten",
	L"Auto-Cursor naar Huurling",
	L"Auto-Cursor naar Deuren",
	L"Items Oplichten",
	L"Toon Boomtoppen",
	L"Toon Draadmodellen",
	L"Toon 3D Cursor",
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
	L"Stat Progress Bars",					// Show progress towards stat increase		// TODO.Translate
	L"Alternate Strategy-Map Colors",		// Change color scheme of Strategic Map
	L"Alternate bullet graphics",			// Show alternate bullet graphics (tracers) // TODO.Translate
	L"Show Merc Ranks",						// shows mercs ranks		// TODO.Translate
	L"Show Face gear graphics",				// TODO.Translate
	L"Show Face gear icons",
	L"Uit te schakelen Cursor Swap",		// Disable Cursor Swap
	L"Quiet Training",						// Madd: mercs don't say quotes while training // TODO.Translate
	L"Quiet Repairing",						// Madd: mercs don't say quotes while repairing // TODO.Translate
	L"Quiet Doctoring",						// Madd: mercs don't say quotes while doctoring // TODO.Translate
	L"Auto Fast Forward AI Turns",			// Automatic fast forward through AI turns		// TODO.Translate
	L"Allow Zombies",				// TODO.Translate
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
	L"Schakel deze optie IN als je de karakter-dialogen wil horen.",

	//Mute Confirmation
	L"Schakelt verbale bevestigingen v.d. karakters in of uit.",

	//Subtitles	
	L"Stelt in of dialoogteksten op het scherm worden getoond.",

	//Key to advance speech
	L"Als ondertitels AANstaan, schakel dit ook in om tijd te hebben de NPC-dialogen te lezen.",

	//Toggle smoke animation
	L"Schakel deze optie uit als rookanimaties het spel vertragen.",

	//Blood n Gore
	L"Schakel deze optie UIT als je bloed aanstootgevend vindt.",

	//Never move my mouse
	L"Schakel deze optie UIT als je wil dat de muis automatisch gepositioneerd wordt bij bevestigingsdialogen.",

	//Old selection method
	L"Schakel deze optie IN als je karakters wil selecteren zoals in de vorige JAGGED ALLIANCE (methode is tegengesteld dus).",

	//Show movement path
	L"Schakel deze optie IN om bewegingspaden te tonen in real-time (schakel het uit en gebruik dan de |S|h|i|f|t-toets om paden te tonen).",

	//show misses
	L"Schakel IN om het spel de plaats van inslag van je kogels te tonen wanneer je \"mist\".",
	
	//Real Time Confirmation
	L"Als INGESCHAKELD, een extra \"veiligheids\"-klik is nodig om in real-time te bewegen.",

	//Sleep/Wake notification
	L"INGESCHAKELD zorgt voor berichten of huurlingen op een \"missie\" slapen of werken.",

	//Use the metric system
	L"Wanneer INGESCHAKELD wordt het metrieke stelsel gebruikt, anders het Imperiale stelsel.",

	//Merc Lighted movement
	L"Wanneer INGESCHAKELD, de huurling verlicht de grond tijdens het lopen. Schakel UIT voor sneller spelen.\nToggle artificial merc light. (|C|t|r|l+|A|l|t+|G)",	// TODO.Translate

	//Smart cursor
	L"Wanneer INGESCHAKELD zullen huurlingen dichtbij de cursor automatisch oplichten.",

	//snap cursor to the door
	L"Wanneer INGESCHAKELD zal de cursor dichtbij een deur automatisch boven de deur gepositioneerd worden.",

	//glow items 
	L"Wanneer INGESCHAKELD lichten items altijd op. (|C|t|r|l+|A|l|t+|I)",

	//toggle tree tops
	L"Wanneer INGESCHAKELD worden Boom|toppen getoond.", 

	//toggle wireframe
	L"Wanneer INGESCHAKELD worden Draadmodellen van niet-zichtbare muren getoond. (|C|t|r|l+|A|l|t+|W)",

	L"Wanneer INGESCHAKELD wordt de cursor in 3D getoond. (|H|o|m|e)",

	// Options for 1.13
	L"When ON, the chance to hit is shown on the cursor.",
	L"When ON, GL burst uses burst cursor.",
	L"When ON, enemies will occasionally comment certain actions.", // Changed from Enemies Drop All Items - SANDRO
	L"When ON, grenade launchers fire grenades at higher angles. (|Q)",
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
	L"When ON, shows character progress towards gaining levels.",	// TODO.Translate
	L"When ON, the Strategic Map will be colored differently based on exploration.",
	L"When ON, alternate bullet graphics will be shown when you shoot.", // TODO.Translate
	L"When ON, ranks will be displayed before merc names in the strategic view.",		// TODO.Translate
	L"When ON, you will see the equipped face gear on the merc portraits.",	// TODO.Translate
	L"When ON, you will see icons for the equipped face gear on the merc portraits in the lower right corner.",
	L"Wanneer ingeschakeld, zal de cursor niet te schakelen tussen uitwisseling positie en andere acties. Druk op |x om snelle uitwisseling te starten.",
	L"When ON, mercs will not report progress during training.", // TODO.Translate
	L"When ON, mercs will not report progress during repairing.",	// TODO.Translate
	L"When ON, mercs will not report progress during doctoring.",	// TODO.Translate
	L"When ON, AI turns will be much faster.",		// TODO.Translate

	L"When ON, zombies will spawn. Beware!",							// allow zombies	// TODO.Translate
	L"When ON, enables popup boxes that appear when you left click on empty merc inventory slots while viewing sector inventory in mapscreen.",	// TODO.Translate
	L"When ON, approximate locations of the last enemies in the sector are highlighted.",  // TODO.Translate
	L"When ON, show the contents of an LBE item, otherwise show the regular NAS interface.", // TODO.Translate
	L"When ON, inverts mouse wheel directions.",		// TODO.Translate
	L"When ON and multiple mercs are selected, they will try to keep their relative distances while moving. (|G)",	// TODO.Translate
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
	L"SPEL-INSTELLINGEN",
#ifdef JA2UB
	L"Random Manuel texts ",
	L"Off",
	L"On",
#else
	L"Speelstijl",
	L"Realistisch",
	L"SF",
#endif	
	L"Platinum", //Placeholder English
	L"Wapenopties",
	L"Extra wapens",
	L"Normaal",
	L"Moeilijksheidsgraad",
	L"Beginneling",
	L"Ervaren",
	L"Expert",
	L"INSANE",
	L"Start",	// TODO.Translate
	L"Stop",
	L"Extra Moeilijk",
	L"Save Anytime",
	L"Iron Man",
	L"Niet mogelijk bij Demo",
	L"Bobby Ray Quality",
	L"Good",
	L"Great",
	L"Excellent",
	L"Awesome",
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
	L"Bobby Ray Quantity",// TODO.Translate

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
	L"London",			//London, England
	L"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
	L"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"Moscow",			//Moscow, USSR
	L"New York",		//New York, New York, USA
	L"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Paris",				//Paris, France
	L"Tripoli",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokyo",				//Tokyo, Japan
	L"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Are you sure? A value of zero means NO ability in this skill.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( 8 Karakters Max )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Analiseren",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Bedankt, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Stem", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Gedood tijdens gevecht",
	L"Ontslagen",
	L"Anders",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Personeelsmanager",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Spel Gepauzeerd",
	L"Doorgaan (|P|a|u|s|e)",
	L"Pauze Spel (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Spel verlaten?",
	L"OK",
	L"JA",
	L"NEE",
	L"STOPPEN",
	L"WEER AANNEMEN",
	L"LEUGEN",
	L"Geen beschrijving", //Save slots that don't have a description.
	L"Spel opgeslagen.",
	L"Spel opgeslagen.",
	L"SnelBewaren", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"Dag",
	L"Huurl",
	L"Leeg Slot",		//An empty save game slot
	L"Demo",			//Demo of JA2
	L"Debug",			//State of development of a project (JA2) that is a debug build
	L"Release",			//Release build for JA2
	L"rpm",				//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"min",				//Abbreviation for minute.
	L"m",					//One character abbreviation for meter (metric distance measurement unit).
	L"rnds",				//Abbreviation for rounds (# of bullets)
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Home",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"nvt",					//Lowercase acronym for not applicable.
	L"Intussen",		//Meanwhile
	L"%s is gearriveerd in sector %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Versie",
	L"Leeg SnelBewaarSlot",
	L"Dit slot is gereserveerd voor SnelBewaren tijdens tactische en kaartoverzichten m.b.v. ALT+S.",
	L"Geopend",
	L"Gesloten",
	L"Schijfruimte raakt op. Er is slects %s MB vrij en Jagged Alliance 2 v1.13 heeft %s MB nodig.",
	L"%s ingehuurd van AIM",
	L"%s heeft %s gevangen.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s heeft %s genomen.",		//'Merc name' has taken the drug
	L"%s heeft geen medische kennis",	//'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"De integriteit van het spel is aangetast.",
	L"FOUT: CD-ROM geopend",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Er is geen plaats om vanaf hier te schieten.",
	
	//Can't change stance due to objects in the way...
	L"Kan op dit moment geen standpunt wisselen.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Drop",
	L"Gooi",
	L"Geef",

	L"%s gegeven aan %s.",	//"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Geen plaats om %s aan %s te geven.",	//pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" eraan vastgemaakt )",

	//Cheat modes
	L"Vals spel niveau EEN",
	L"Vals spel niveau TWEE",

	//Toggling various stealth modes
	L"Team op sluipmodus.",
	L"Team niet op sluipmodus.",
	L"%s op sluipmodus.",
	L"%s niet op sluipmodus.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Extra Draadmodellen Aan",
	L"Extra Draadmodellen Uit",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Kan niet naar boven vanaf dit niveau...",
	L"Er zijn geen lagere niveaus...",
	L"Betreden basisniveau %d...",
	L"Verlaten basisniveau...",

	L"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Volgmodus UIT.",
	L"Volgmodus AAN.",
	L"3D Cursor UIT.",
	L"3D Cursor AAN.",
	L"Team %d actief.",
	L"Je kunt %s's dagelijkse salaris van %s niet betalen",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Overslaan",
	L"%s kan niet alleen weggaan.",
	L"Een spel is bewaard onder de naam SaveGame249.sav. Indien nodig, hernoem het naar SaveGame10 zodat je het kan aanroepen in het Laden-scherm.",
	L"%s dronk wat %s",
	L"Een pakket is in Drassen gearriveerd.",
	L"%s zou moeten arriveren op het aangewezen punt (sector %s) op dag %d, om ongeveer %s.",
	L"Geschiedenisverslag bijgewerkt.",
	L"Grenade Bursts use Targeting Cursor (Spread fire enabled)",
	L"Grenade Bursts use Trajectory Cursor (Spread fire disabled)",
	L"Enabled Soldier Tooltips", // Changed from Drop All On - SANDRO
	L"Disabled Soldier Tooltips", 	// 80	// Changed from Drop All Off - SANDRO
	L"Granade Launchers fire at standard angles",
	L"Grenade Launchers fire at higher angles",
	// forced turn mode strings
    L"Forced Turn Mode",
	L"Normal turn mode",
	L"Exit combat mode",
	L"Forced Turn Mode Active, Entering Combat",
	L"Spel succesvol bewaard in de Einde Beurt Auto Bewaar Slot.",
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
	L"Scroll Omhoog",
	L"Selecteer Alles",
	L"Scroll Omlaag",
	L"Stop",
};

STR16 pDoctorWarningString[] =
{
	L"%s is niet dichtbij genoeg om te worden genezen.",
	L"Je medici waren niet in staat om iedereen te verbinden.",
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
	L"Ga naar AIM en huur wat huurlingen in ( *Hint* dat kan bij Laptop )",
#ifdef JA2UB
	L"Als je klaar bent om naar Tracona te gaan, klik dan op TijdVersneller onder rechts op het scherm.",	// to inform the player to hit time compression to get the game underway
#else
	L"Als je klaar bent om naar Arulco te gaan, klik dan op TijdVersneller onder rechts op het scherm.",	// to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Fout. Bestanden missen of zijn beschadigd. Spel wordt beëindigd.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Lees E-mail",
	L"Bekijk web-pagina's",
	L"Bekijk bestanden en e-mail attachments",
	L"Lees verslag van gebeurtenissen",
	L"Bekijk team-info",
	L"Bekijk financieel overzicht",
	L"Sluit laptop",

	//Bottom task bar icons (if they exist):
	L"Je hebt nieuwe berichten",
	L"Je hebt nieuwe bestanden",

	//Bookmarks:
	L"Association of International Mercenaries",
	L"Bobby Ray's online weapon mail order",
	L"Institute of Mercenary Profiling",
	L"More Economic Recruiting Center",
	L"McGillicutty's Mortuarium",
	L"United Floral Service",
	L"Verzekeringsagenten voor A.I.M. contracten",
	//New Bookmarks
	L"",
	L"Encyclopedia",
	L"Briefing Room",
	L"Campaign History",	// TODO.Translate
	L"Mercenaries Love or Dislike You",	// TODO.Translate
	L"World Health Organization",
	L"Kerberus - Excellence In Security",
	L"Militia Overview",	// TODO.Translate
	L"Recon Intelligence Services",	// TODO.Translate
};


STR16 gzHelpScreenText[] =
{
	L"Verlaat help-scherm",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Er is een gevecht gaande. Je kan alleen terugtrekken m.b.v. het tactische scherm.",
	L"B|etreedt sector om door te gaan met het huidige gevecht.",
	L"Los huidige gevecht |automatisch op.",
	L"Gevecht kan niet automatisch opgelost worden als je de aanvaller bent.",
	L"Gevecht kan niet automatisch opgelost worden als je in een hinderlaag ligt.",
	L"Gevecht kan niet automatisch opgelost worden als je vecht met beesten in de mijnen.",
	L"Gevecht kan niet automatisch opgelost worden als er vijandige burgers zijn.",
	L"Gevecht kan niet automatisch opgelost worden als er nog bloodcats zijn.",
	L"GEVECHT GAANDE",
	L"je kan je op dit moment niet terugtrekken.",
};

STR16 gzMiscString[] =
{
	L"Je militie vecht door zonder hulp van je huurlingen...",
	L"Het voertuig heeft geen brandstof meer nodig.",
	L"De brandstoftank is voor %d%% gevuld.",
	L"Het leger van Deidranna heeft totale controle verkregen over %s.",
	L"Je hebt een tankplaats verloren.",
};

STR16	gzIntroScreen[] = 
{
	L"Kan intro video niet vinden",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH." 
STR16 pNewNoiseStr[] =
{
	L"%s hoort een %s geluid uit %s.",
	L"%s hoort een %s geluid van BEWEGING uit %s.",
	L"%s hoort een %s KRAKEND geluid uit %s.",
	L"%s hoort een %s SPETTEREND geluid uit %s.",
	L"%s hoort een %s INSLAG uit %s.",
	L"%s hears a %s GUNFIRE coming from %s.", // anv: without this, all further noise notifications were off by 1!		// TODO.Translate
	L"%s hoort een %s EXPLOSIE naar %s.",
	L"%s hoort een %s SCHREEUW naar %s.",
	L"%s hoort een %s INSLAG naar %s.",
	L"%s hoort een %s INSLAG naar %s.",
	L"%s hoort een %s VERSPLINTEREN uit %s.",
	L"%s hoort een %s KLAP uit %s.",
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
	L"Sorteer op Naam (|F|1)",
	L"Sorteer op Taak (|F|2)",
	L"Sorteer op Slaapstatus (|F|3)",
	L"Sorteer op locatie (|F|4)",
	L"Sorteer op Bestemming (|F|5)",
	L"Sorteer op Vertrektijd (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Fout 404",
	L"Site niet gevonden.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Recentelijke ladingen",
	L"Order #",
	L"Aantal Items",
	L"Besteld op",
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
	L"Spel Programmeur", 					// Chris Camfield "Game Internals Programmer" 
	L"Co-ontwerper/Schrijver",				// Shaun Lyng "Co-designer/Writer" 
	L"Strategische Systemen & Programmeur",		//Kris Marnes "Strategic Systems & Editor Programmer" 
	L"Producer/Co-ontwerper",				// Ian Currie "Producer/Co-designer" 
	L"Co-ontwerper/Kaartontwerp",				// Linda Currie "Co-designer/Map Designer" 
	L"Artiest",							// Eric \"WTF\" Cheng "Artist" 
	L"Beta Coördinator, Ondersteuning",			// Lynn Holowka
	L"Artiest Extraordinaire",				// Norman \"NRG\" Olsen
	L"Geluidsgoeroe",						// George Brooks
	L"Schermontwerp/Artiest",				// Andrew Stacey
	L"Hoofd-Artiest/Animator",				// Scot Loving
	L"Hoofd-Programmeur",					// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmeur",							// Dave French
	L"Strategische Systemen & Spelbalans Programmeur",	// Alex Meduna
	L"Portret-Artiest",						// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(leert nog steeds interpunctie)",			// Shaun Lyng
	L"(\"Het is klaar. Ben er mee bezig\")",		//Kris \"The Cow Rape Man\" Marnes
	L"(wordt veel te oud voor dit)",			// Ian Currie
	L"(en werkt aan Wizardry 8)",				// Linda Currie
	L"(moets onder bedreiging ook QA doen)",		// Eric \"WTF\" Cheng
	L"(Verliet ons voor CFSA - dus...)",		// Lynn Holowka
	L"",								// Norman \"NRG\" Olsen
	L"",								// George Brooks
	L"(Dead Head en jazz liefhebber)",			// Andrew Stacey
	L"(in het echt heet hij Robert)",			// Scot Loving
	L"(de enige verantwoordelijke persoon)",		// Andrew \"Big Cheese Doddle\" Emmons
	L"(kan nu weer motorcrossen)",			// Dave French
	L"(gestolen van Wizardry 8)",				// Alex Meduna
	L"(deed items en schermen-laden ook!)",		// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s is klaar met reparatie van eigen items.",
	L"%s is klaar met reparatie van ieders wapens en bepantering.",
	L"%s is klaar met reparatie van ieders uitrusting.",
	L"%s finished repairing everyone's large carried items.",
	L"%s finished repairing everyone's medium carried items.",
	L"%s finished repairing everyone's small carried items.",
	L"%s finished repairing everyone's LBE gear.",
	L"%s finished cleaning everyone's guns.",	// TODO.Translate
};

STR16 zGioDifConfirmText[]=
{
	L"Je hebt de NOVICE-modus geselecteerd. Deze instelling is geschikt voor diegenen die Jagged Alliance voor de eerste keer spelen, voor diegenen die nog niet zo bekend zijn met strategy games, of voor diegenen die kortere gevechten in de game willen hebben.", //Je keuze beïnvloedt dingen in het hele verloop van de game, dus weet wat je doet. Weet je zeker dat je in de Novice-modus wilt spelen?",
	L"Je hebt de EXPERIENCED-modus geselecteerd. Deze instelling is geschikt voor diegenen die al bekend zijn met Jagged Alliance of dergelijke games. Je keuze beïnvloedt dingen in het hele verloop van de game, dus weet wat je doet. Weet je zeker dat je in de Experienced-modus wilt spelen ?",
	L"Je hebt de EXPERT-modus geselecteerd. We hebben je gewaarschuwd. Geef ons niet de schuld als je in een kist terugkomt. Je keuze beïnvloedt dingen in het hele verloop van de game, dus weet wat je doet. Weet je zeker dat je in de Expert-modus wilt spelen?",
	L"You have chosen INSANE mode. WARNING: Don't blame us if you get shipped back in little pieces... Deidranna WILL kick your ass.  Hard.  Your choice will affect things throughout the entire course of the game, so choose wisely. Are you sure you want to play in INSANE mode?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S laadscherm-data niet gevonden...",

	//1-5
	L"De robot kan de sector niet verlaten als niemand de besturing gebruikt.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Je kan de tijd niet versnellen, Wacht op het vuurwerk!",

	//'Name' refuses to move.
	L"%s weigert zich te verplaatsen.",

	//%s a merc name
	L"%s heeft niet genoeg energie om standpunt te wisselen.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s heeft geen brandstof en is gestrand in %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"boven",
	L"onder",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Niemand van je huurlingen heeft medische kennis.",
	L"Er zijn geen medische hulpmiddelen om mensen te verbinden.",
	L"Er waren niet genoeg medische hulpmiddelen om iedereen te verbinden.",
	L"Geen enkele huurling heeft medische hulp nodig.",
	L"Verbindt huurlingen automatisch.",
	L"Al je huurlingen zijn verbonden.",

	//14
#ifdef JA2UB
	L"Tracona",
#else
	L"Arulco",
#endif
	L"(dak)",

	L"Gezondheid: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8" 
	//"vs." is the abbreviation of versus.
	L"%d vs. %d",
	
	L"%s is vol!",  //(ex "The ice cream truck is full")

	L"%s heeft geen eerste hulp nodig, maar échte medische hulp of iets dergelijks.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s is geraakt in het been en valt om!",
	//Name can't speak right now.
	L"%s kan nu niet praten.",

	//22-24 plural versions 
	L"%d groene milities zijn gepromoveerd tot veteranenmilitie.",
	L"%d groene milities zijn gepromoveerd tot reguliere militie.",
	L"%d reguliere milities zijn gepromoveerd tot veteranenmilitie.",

	//25
	L"Schakelaar",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s wordt gek!",

	//27-28
	//Messages why a player can't time compress.
	L"Het is nu onveilig om de tijd te versnellen omdat je huurlingen hebt in sector %s.",
	L"Het is nu onveilig om de tijd te versnellen als er huurlingen zijn in de mijnen met beesten.",

	//29-31 singular versions 
	L"1 groene militie is gepromoveerd tot veteranenmilitie.",
	L"1 groene militie is gepromoveerd tot reguliere militie.",
	L"1 reguliere militie is gepromoveerd tot veteranenmilitie.",

	//32-34
	L"%s zegt helemaal niets.",
	L"Naar oppervlakte reizen?",
	L"(Team %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s heeft %s's %s gerepareerd",

	//36
	L"BLOODCAT",

	//37-38 "Name trips and falls" 
	L"%s ups en downs",
	L"Dit item kan vanaf hier niet opgepakt worden.",

	//39
	L"Geen enkele huurling van je is in staat om te vechten. De militie zal zelf tegen de beesten vechten.",

	//40-43
	//%s is the name of merc.
	L"%s heeft geen medische kits meer!",
	L"%s heeft geen medische kennis om iemand te verzorgen!",
	L"%s heeft geen gereedschapkits meer!",
	L"%s heeft geen technische kennis om iets te repareren!",

	//44-45
	L"Reparatietijd",
	L"%s kan deze persoon niet zien.",

	//46-48
	L"%s's pistoolloopverlenger valt eraf!",
	L"No more than %d militia trainers are permitted in this sector.",		// TODO.Translate
	L"Zeker weten?",

	//49-50
	L"Tijdversneller",
	L"De tank van het voertuig is nu vol.",

	//51-52 Fast help text in mapscreen.
	L"Doorgaan met Tijdversnelling (|S|p|a|c|e)",
	L"Stop Tijdversnelling (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s heeft de %s gedeblokkeerd",
	L"%s heeft %s's %s gedeblokkeerd",

	//55 
	L"Kan tijd niet versneller tijdens bekijken van sector inventaris.",

	L"Kan de Jagged Alliance 2 v1.13 SPEL CD niet vinden. Programma wordt afgesloten.",

	L"Items succesvol gecombineerd.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Huidig/Max Voortgang: %d%%/%d%%",

	L"John en Mary escorteren?",

	//60	
	L"Schakelaar geactiveerd.",

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
	L"Geschrapt punt",
	L"Schrapte alle punten van dit type",
	L"Verkocht punt",
	L"Verkocht alle punten van dit type",
	L"You should check your goggles",
	// Real Time Mode messages
	L"In combat already",
	L"No enemies in sight",
	L"Real-time sneaking OFF",
	L"Real-time sneaking ON",
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
	L"Sniper: De ogen van een havik, u kunnen de vleugels van een vlieg bij honderd werven ontspruiten! ± ",
	// Camouflage
	L"Camouflage: Naast u ringt synthetische zelfs blik! ± ",	
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
	L"Kan op dit moment niet bestelwagenrugzak",
	L"Geen plaats om rugzak te zetten",
	L"Gevonden niet rugzak",
	L"De ritssluiting werkt slechts in gevecht",
	L"Kan niet me bewegen terwijl actieve rugzakritssluiting",
	L"Bent zeker u u wilt alle sectorpunten verkopen?",
	L"Bent zeker u u wilt alle sectorpunten schr?",
	L"Kan beklimmen niet terwijl het dragen van een rugzak",
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
	// 70	// TODO.Translate
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
	L"Nvt",		// Acronym of Not Applicable
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
	L"Jagged Alliance 2 v1.13 windowed modus vereist een kleurdiepte van 16 bits per pixel.",
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
	L"|D|e|f|u|s|a|l |K|i|t", // TODO.Translate
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
	L"\n \nThis item can be used to pick open locked\ndoors or containers.\n \nLockpicking is silent, although it requires\nsubstantial mechanical skill to pick anything\nbut the simplest locks. This item modifies\nthe lockpicking chance by ", //JMich_SkillsModifiers: needs to be followed by a number",		// TODO.Translate
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
	//L"Range to target: %d tiles, Brightness: %d/%d",
	L"Attaching the transmitter to your laptop computer.",
	L"You cannot afford to hire %s",
	L"For a limited time, the above fee covers the cost of the entire mission and includes the equipment listed below.",
	L"Hire %s now and take advantage of our unprecedented 'one fee covers all' pricing.  Also included in this unbelievable offer is the mercenary's personal equipment at no charge.",
	L"Fee",
	L"There is someone else in the sector...",
	//L"Gun Range: %d tiles, Chance to hit: %d percent",
	L"Display Cover",
	L"Line of Sight",
	L"New Recruits cannot arrive there.",
	L"Since your laptop has no transmitter, you won't be able to hire new team members.  Perhaps this would be a good time to load a saved game or start over!",
	L"%s hears the sound of crumpling metal coming from underneath Jerry's body.  It sounds disturbingly like your laptop antenna being crushed.",  //the %s is the name of a merc.  @@@  Modified
	L"After scanning the note left behind by Deputy Commander Morris, %s senses an oppurtinity.  The note contains the coordinates for launching missiles against different towns in Arulco.  It also gives the coodinates of the origin - the missile facility.",
	L"Noticing the control panel, %s figures the numbers can be reversed, so that the missile might destroy this very facility.  %s needs to find an escape route.  The elevator appears to offer the fastest solution...",
	L"This is an IRON MAN game and you cannot save when enemies are around.",	//	@@@  new text
	L"(Cannot save during combat)", //@@@@ new text
	L"The current campaign name is greater than 30 characters.",							// @@@ new text
	L"The current campaign cannot be found.",																	// @@@ new text
	L"Campaign: Default ( %S )",																							// @@@ new text
	L"Campaign: %S",																													// @@@ new text
	L"You have selected the campaign %S. This campaign is a player-modified version of the original Unfinished Business campaign. Are you sure you wish to play the %S campaign?",			// @@@ new text
	L"In order to use the editor, please select a campaign other than the default.",		///@@new
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
	L" might steal items\n",

	L" +1 traplevel to planted bombs\n",
	L" spreads corruption to nearby mercs\n",
	L" female only",							// won't show up, text exists for compatibility reasons
	L" male only",								// won't show up, text exists for compatibility reasons

	L" huge loyality penalty in all towns if we die\n",		// TODO.Translate
};

// TODO.Translate
STR16	szBackgroundText_Value[]=
{
	L" %s%d%% APs in polar sectors\n",
	L" %s%d%% APs in desert sectors\n",	
	L" %s%d%% APs in swamp sectors\n",
	L" %s%d%% APs in urban sectors\n",
	L" %s%d%% APs in forest sectors\n",	// TODO.Translate
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
	L"Contaminated items found in %s' inventory.",
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
	L"Equipment cost: %d$",
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

#endif //DUTCH
