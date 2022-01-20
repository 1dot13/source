// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("GERMAN")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#ifdef GERMAN
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_GermanText_public_symbol(void){;}

#ifdef GERMAN


/*
******************************************************************************************************
**																	IMPORTANT TRANSLATION NOTES																			**
******************************************************************************************************

GENERAL TOPWARE INSTRUCTIONS
- Always be aware that German strings should be of equal or shorter length than the English equivalent. 
	I know that this is difficult to do on many occasions due to the nature of the German language when 
	compared to English. By doing so, this will greatly reduce the amount of work on both sides. In 
	most cases (but not all), JA2 interfaces were designed with just enough space to fit the English word. 
	The general rule is if the string is very short (less than 10 characters), then it's short because of 
	interface limitations. On the other hand, full sentences commonly have little limitations for length. 
	Strings in between are a little dicey.
- Never translate a string to appear on multiple lines. All strings L"This is a really long string...", 
	must fit on a single line no matter how long the string is. All strings start with L" and end with ", 
- Never remove any extra spaces in strings. In addition, all strings containing multiple sentences only 
	have one space after a period, which is different than standard typing convention. Never modify sections 
	of strings contain combinations of % characters. These are special format characters and are always 
	used in conjunction with other characters. For example, %s means string, and is commonly used for names,
	locations, items, etc. %d is used for numbers. %c%d is a character and a number (such as A9). 
	%% is how a single % character is built. There are countless types, but strings containing these 
	special characters are usually commented to explain what they mean. If it isn't commented, then 
	if you can't figure out the context, then feel free to ask SirTech.
- Comments are always started with // Anything following these two characters on the same line are 
	considered to be comments. Do not translate comments. Comments are always applied to the following 
	string(s) on the next line(s), unless the comment is on the same line as a string. 
- All new comments made by SirTech will use "//@@@ comment" (without the quotes) notation. By searching 
	for @@@ everytime you recieve a new version, it will simplify your task and identify special instructions.
	Commonly, these types of comments will be used to ask you to abbreviate a string. Please leave the 
	comments intact, and SirTech will remove them once the translation for that particular area is resolved.
- If you have a problem or question with translating certain strings, please use "//!!! comment" 
	(without the quotes). The syntax is important, and should be identical to the comments used with @@@ 
	symbols. SirTech will search for !!! to look for Topware problems and questions. This is a more 
	efficient method than detailing questions in email, so try to do this whenever possible.


	
FAST HELP TEXT -- Explains how the syntax of fast help text works.
**************

1) BOLDED LETTERS
	The popup help text system supports special characters to specify the hot key(s) for a button. 
	Anytime you see a '|' symbol within the help text string, that means the following key is assigned
	to activate the action which is usually a button. 

	EX: L"|Map Screen" 

	This means the 'M' is the hotkey. In the game, when somebody hits the 'M' key, it activates that
	button. When translating the text to another language, it is best to attempt to choose a word that
	uses 'M'. If you can't always find a match, then the best thing to do is append the 'M' at the end
	of the string in this format:

	EX: L"Ecran De Carte (|M)" (this is the French translation)

	Other examples are used multiple times, like the Esc key or "|E|s|c" or Space -> (|S|p|a|c|e)

2) NEWLINE
	Any place you see a \n within the string, you are looking at another string that is part of the fast help
	text system. \n notation doesn't need to be precisely placed within that string, but whereever you wish 
	to start a new line. 

	EX: L"Clears all the mercs' positions,\nand allows you to re-enter them manually." 

	Would appear as:

				Clears all the mercs' positions,
				and allows you to re-enter them manually.

	NOTE: It is important that you don't pad the characters adjacent to the \n with spaces. If we did this
				in the above example, we would see

	WRONG WAY -- spaces before and after the \n
	EX: L"Clears all the mercs' positions, \n and allows you to re-enter them manually." 

	Would appear as: (the second line is moved in a character)
		
				Clears all the mercs' positions,
 				 and allows you to re-enter them manually.


@@@ NOTATION
************

	Throughout the text files, you'll find an assortment of comments. Comments are used to describe the
	text to make translation easier, but comments don't need to be translated. A good thing is to search for
	"@@@" after receiving new version of the text file, and address the special notes in this manner. 

!!! NOTATION
************

	As described above, the "!!!" notation should be used by Topware to ask questions and address problems as
	SirTech uses the "@@@" notation.

*/

/*
LOOTF - Foot note. I've rewritten a whole lot of stuff and only marked specific lines and blocks.
	That's where I'm either

		- not sure about the character limit (might not be mentioned but causes trouble when displaying texts?)
		- not sure about the meaning
		- not sure if people will like it (this concerns German speakers)
		- not as creative as to find a perfect replacement

I have also changed stuff people might have found okay, which only troubled me.
This includes
<GERMAN> "Zurückziehen". Klingt einfach nicht. Hört sich an wie sich zur Nachtruhe begeben.
"Zurückgezogen" ist ein Waldschrat. Geändert auf "ausgewichen".
Ich hoffe nur, dass nicht irgendjemand dumm rumschwätzt wegen Kugeln ausweichen oder so.</GERMAN>

Anything else is a-ok and can be filtered out by comparing this cpp with the old version.
I have also added tabs and removed some where I thought it was appropriate (format-wise).
My comments are marked using LOOTF.
Comments for SANDRO are marked using LOOTF - SANDRO.
Remove any LOOTF comment that has been checked, except maybe for "alt." (alternative) stuff or stuff of that sort.

07/2010 LootFragg
*/

CHAR16	XMLTacticalMessages[1000][MAX_MESSAGE_NAMES_CHARS] = 
{
	L"",
};

//Encyclopedia

STR16 pMenuStrings[] =
{
	//Encyclopedia
	L"Orte", // 0
	L"Personen",
	L"Gegenstände",
	L"Aufträge",
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
	L"Eintreten",
};

STR16	pOtherButtonsText[] =
{
	L"Auftrag", 
	L"Akzep.",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Einsatzbesprechung",
	L"Auftrag annehmen",
};


STR16	pLocationPageText[] =
{
	L"Vorherige Seite",
	L"Foto", 
	L"Nächste Seite",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"Hauptseite",
	L">>",
	L"Typ: ",
	L"Keine Daten",
	L"Es gibt keine Missionen. Fügen Sie Missionen zu der Datei TableData\\BriefingRoom\\BriefingRoom.xml hinzu. Die erste Mission muss SICHTBAR sein. Setzen Sie den Wert Hidden = 0.",
	L"Besprechungszimmer. Bitte drücken sie auf 'Eintreten'.",
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Unbekannt",// 0 - unknown
	L"Stadt", //1 - cities
	L"Luftwaffen Stützpunkt", //2 - SAM Site
	L"Andere Örtlichkeiten", //3 - other location
	L"Minen", //4 - mines 
	L"Militärstützpunkt", //5 - military complex 
	L"Labor",  //6 - laboratory complex 
	L"Fabrik", //7 - factory complex 
	L"Spital", //8 - hospital 
	L"Gefängnis", //9 - prison
    L"Flughafen", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Alle anzeigen",
	L"AIM anzeigen",
	L"MERC anzeigen",
	L"RPC anzeigen",
	L"NPC anzeigen",
	L"Fahrzeuge anzeigen",
	L"BSE anzeigen",
	L"EPC anzeigen",
	L"Filter",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"Alle",
	L"AIM",
	L"MERC",
	L"RPC",
	L"NPC",
	L"Veh.",
	L"BSE",
	L"EPC",
	L"Filter",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Alles anzeigen",
	L"Städte anzeigen",
	L"Luftwaffenstützpunkte anzeigen",
	L"Andere Örtlichkeiten anzeigen",
	L"Minen anzeigen",
	L"Militärstützpunkte anzeigen",
	L"Labor-Komplexe anzeigen",
	L"Fabriken anzeigen",
	L"Spitäler anzeigen",
	L"Gefängnisse anzeigen",
	L"Flughäfen anzeigen",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"Alle",
	L"Stadt",
	L"SAM",
	L"Andere",
	L"Mine",
	L"Mil.",
	L"Lab.",
	L"Fabr.",
	L"Spit.",
	L"Gefän.",
	L"Flugh.",
};

STR16	pEncyclopediaFilterLocationText[] =
{//major location filter button text max 7 chars
//..L"------v"
	L"Alle",//0
	L"Stadt",
	L"SAM",
	L"Mine",
	L"Flugh.",
	L"Wildn.",
	L"Unterg.",
	L"Gebäude",
	L"andere",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Zeige Alle",//facility index + 1
	L"Zeige Stadtsektoren",
	L"Zeige SAM's ",
	L"Zeige Mine",
	L"Zeige Flughäfen",
	L"Zeige Sektoren in der Wildniss",
	L"Zeige Untergrund",
	L"Zeige wichtige Gebäude\n[|L|MT] ändere Filter\n[|R|MT] F. zurücksetzen",
	L"Zeige andere Sektoren",
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

STR16	pEncyclopediaFilterCharText[] =
{//major char filter button text
//..L"------v"
	L"Alle",//0
	L"A.I.M.",
	L"MERC",
	L"RPC",
	L"NPC",
	L"IMP",
	L"Andere",//add new filter buttons before other
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Zeige Alle",//Other index + 1
	L"Zeige A.I.M. Mitarbeiter",
	L"Zeige M.E.R.C Mitarbeiter",
	L"Zeige Rebellen",
	L"Zeige Nichtspieler Charaktere",
	L"Zeige Spieler Charaktere",
	L"Zeige Andere\n[|L|MT] ändere Filter\n[|R|MT] F. zurücksetzen",
};

STR16	pEncyclopediaSubFilterCharText[] =
{//item subfilter button text
//..L"------v"
	L"",//reserved. Insert new AIM filters above!
	L"",//reserved. Insert new MERC filters above!
	L"",//reserved. Insert new RPC filters above!
	L"",//reserved. Insert new NPC filters above!
	L"",//reserved. Insert new IMP filters above!
//Other-----v"
	L"Fahrz.",
	L"EPC",
	L"",//reserved. Insert new Other filters above!
};

STR16	pEncyclopediaFilterItemText[] = 
{//major item filter button text max 7 chars
//..L"------v"
	L"Alle",//0
	L"Hand.W.",
	L"Muni.",
	L"Rüstung",
	L"LBE",
	L"Zubeh.",
	L"Versch.",//add new filter buttons before misc
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Zeige Alle",//misc index + 1
	L"Zeige Handfeuer Waffen\n[|L|MT] ändere Filter\n[|R|MT] F. zurücksetzen",
	L"Zeige Munition\n[|L|MT] ändere Filter\n[|R|MT] Filter zurücksetzen",
	L"Zeige Rüstung\n[|L|MT] ändere Filter\n[|R|MT] Filter zurücksetzen",
	L"Zeige LBE-Gepäck\n[|L|MT] ändere Filter\n[|R|MT] Filter zurücksetzen",
	L"Zeige Zubehör\n[|L|MT] ändere Filter\n[|R|MT] Filter zurücksetzen",
	L"Zeige Verschiedenes\n[|L|MT] ändere Filter\n[|R|MT] F. zurücksetzen",
};

STR16 pEncyclopediaSubFilterItemText[] =
{//item subfilter button text max 7 chars
//..L"------v"
//Guns......v"
	L"Pistole",
	L"M.Pist.",
	L"lei. MG",
	L"Gewehr",
	L"Scharfs",
	L"Sturm G",
	L"MG",
	L"Schrot.",
	L"Schwere",
	L"",//reserved. insert new gun filters above!
//Amunition.v"
	L"Pistole",
	L"M.Pist.",
	L"lei. MG",
	L"Gewehr",
	L"Scharfs",
	L"Sturm G",
	L"MG",
	L"Schrot.",
	L"Schwere",
	L"",//reserved. insert new ammo filters above!
//Armor.....v"
	L"Helm",
	L"Weste",
	L"Hose",
	L"Platte",
	L"",//reserved. insert new armor filters above!
//LBE.......v"
	L"Gürtel",
	L"Weste",
	L"Kampfg.",
	L"Marsch.",
	L"Tasche",
	L"Andere",
	L"",//reserved. insert new LBE filters above!
//Attachments"
	L"Optik",
	L"Seite",
	L"Lauf",
	L"Extern",
	L"Intern",
	L"Andere",
	L"",//reserved. insert new attachment filters above!
//Misc......v"
	L"Messer",
	L"Wurf M.",
	L"Schlag",
	L"Kranate",
	L"Sprengs",
	L"Medikit",
	L"Kit",
	L"Kopf",
	L"Andere",
	L"",//reserved. insert new misc filters above!
//add filters for a new button here
};

STR16 pEncyclopediaFilterQuestText[] =
{//major quest filter button text max 7 chars
//..L"------v"
	L"Alle",
	L"Aktiv",
	L"Abges.",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Zeige Alle",//misc index + 1
	L"Zeige Aktive Quests",
	L"Zeige Abgeschlossene Quests",
};

STR16 pEncyclopediaSubFilterQuestText[] =
{//Quest subfilter button text max 7 chars, not used, but needed if any subfilters are added
//..L"------v"
	L"",//reserved. insert new active quest subfilters above!
	L"",//reserved. insert new completed quest subfilters above!
};

STR16	pEncyclopediaShortInventoryText[] = 
{
	L"Alle", //0
	L"Waffen",
	L"Mun.",
	L"LBE",
	L"Sonst.",
	
	L"Alle", //5
	L"Waffen",
	L"Munition",
	L"LBE Gegenstände",
	L"Sonstige",
};

STR16			BoxFilter[] =
{
	// Guns
	L"Schwer",
	L"Pistole",
	L"M. Pist.",
	L"SMG",
	L"Gewehr",
	L"S.Gew.",
	L"A.Gew.",
	L"MG",
	L"Schrot.",

	// Ammo
	L"Pistol",
	L"M. Pist.", //10
	L"SMG",
	L"Gewehr",
	L"S.Gew",
	L"A.Gew.",
	L"MG",
	L"Schrot.",

	// Used
	L"Waffen",
	L"Panz.",
	L"LBE Ausr.",
	L"Sonst.", //20

	// Armour
	L"Helme",
	L"Westen",
	L"Hosen",
	L"Platten",

	// Misc
	L"Klingen",
	L"Wurfm.",
	L"Nah.",
	L"Gran.",
	L"Bomb.",
	L"Med.", //30
	L"Kits",
	L"Gesicht",
	L"LBE",
	L"Sonst.", //34
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
	L"Omerta befreit",
	L"Drassen befreit",
	L"Sanmona befreit",
	L"Cambria befreit",
	L"Alma befreit",
	L"Grumm befreit",
	L"Tixa befreit",
	L"Chitzena befreit",
	L"Estoni befreit",
	L"Balime befreit",

	L"Orta befreit",					//10
	L"Meduna befreit",
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

// TODO.Translate
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
	L"Humvee",		
};

// TODO.Translate
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

// TODO.Translate
CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
	L"Patrol orders with no waypoints",
	L"Waypoints with no patrol orders",
};

STR16 pClearCurrentScheduleText[] =
{
	L"Keine Aktion",
};

STR16 pCopyMercPlacementText[] =
{
	L"Platzierung wurde nicht kopiert, weil keine Platzierung ausgewählt wurde.",
	L"Platzierung kopiert.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Platzierung wurde nicht eingefügt, weil keine Platzierung in den Speicher kopiert wurde.",
	L"Platzierung eingefügt.",
	L"Platzierung wurde nicht eingefügt, weil die Maximalanzahl für die Platzierungen des Teams erreicht wurde.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Editor beenden?",
};

// TODO.Translate
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

// TODO.Translate
STR16 pPerformSelectedActionText[] = 
{
	L"Creating radar map for %S", //0
	
	L"Delete current map and start a new basement level?",
	L"Delete current map and start a new cave level?",
	L"Delete current map and start a new outdoor level?",
	
	L" Wipe out ground textures? ",
};

// TODO.Translate
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

// TODO.Translate
STR16 pAutoLoadMapText[] =
{
	L"Map data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
	L"Schedule data has just been corrupted.  Don't save, don't quit, get Kris!  If he's not here, save the map using a temp filename and document everything you just did, especially your last action!",
};

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16 pSetupGameTypeFlagsText[] =
{
	L"Item appears in both Sci-Fi and Realistic modes", //0
	L"Item appears in Realistic mode only",
	L"Item appears in Sci-Fi mode only",
};

// TODO.Translate
STR16 pSetupGunGUIText[] =
{
	L"SILENCER", //0
	L"SNIPERSCOPE",
	L"LASERSCOPE",
	L"BIPOD",
	L"DUCKBILL",
	L"G-LAUNCHER", //5
};

// TODO.Translate
STR16 pSetupArmourGUIText[] =
{
	L"CERAMIC PLATES", //0
};

// TODO.Translate
STR16 pSetupExplosivesGUIText[] =
{
	L"DETONATOR",
};

// TODO.Translate
STR16 pSetupTriggersGUIText[] =
{
	L"If the panic trigger is an alarm trigger,\nenemies won't attempt to use it if they\nare already aware of your presence.",
};

//Sector Summary.cpp
// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16 pUpdateSectorSummaryText[] =
{
	L"Analyzing map:  %s...",
};

// TODO.Translate
STR16 pSummaryLoadMapCallbackText[] =
{
	L"Loading map:  %s",
};

// TODO.Translate
STR16 pReportErrorText[] =
{
	L"Skipping update for %s.  Probably due to tileset conflicts...",
};

// TODO.Translate
STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Generating map information",
};

// TODO.Translate
STR16 pSummaryUpdateCallbackText[] =
{
	L"Generating map summary",
};

// TODO.Translate
STR16 pApologizeOverrideAndForceUpdateEverythingText[] =
{
	L"MAJOR VERSION UPDATE",
	L"There are %d maps requiring a major version update.",
	L"Updating all outdated maps",
};

// TODO.Translate
//selectwin.cpp
STR16 pDisplaySelectionWindowGraphicalInformationText[] =
{
	L"%S[%d] from default tileset %s (%d, %S)",
	L"File: %S, subindex: %d (%d, %S)",
	L"Tileset:  %s",
};

// TODO.Translate
STR16 pDisplaySelectionWindowButtonText[] =
{
	L"Accept selections (|E|n|t|e|r)",
	L"Cancel selections (|E|s|c)\nClear selections (|S|p|a|c|e)",
	L"Scroll window up (|U|p)",
	L"Scroll window down (|D|o|w|n)",
};

// TODO.Translate
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
	L"Seite 1/2", //0
	L"Seite 2/2",
	
	L"Seite 1/3",
	L"Seite 2/3",
	L"Seite 3/3",
	
	L"Seite 1/1", //5
};

// by Jazz
CHAR16 zGrod[][500] =
{
	L"Roboter", //0    // Robot
};

STR16 pCreditsJA2113[] =
{
	L"@T,{;JA2 v1.13 Entwicklungsteam",
	L"@T,C144,R134,{;Programmierung",
	L"@T,C144,R134,{;Grafiken und Sounds",
	L"@};(Verschiedene weitere Mods!)",
	L"@T,C144,R134,{;Gegenstände",
	L"@T,C144,R134,{;Weitere Mitwirkende",
	L"@};(Alle weiteren Community-Mitglieder die Ideen und Feedback eingebracht haben!)",
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
//	L".38 Kal",
//	L"9mm",
//	L".45 Kal",
//	L".357 Kal",
//	L"12 Kal",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm NATO",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rakete",
//	L"",
//	L"",
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
//	L".38 Kal",
//	L"9mm",
//	L".45 Kal",
//	L".357 Kal",
//	L"12 Kal",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm N.",
//	L"7.62mm WP",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monster",
//	L"Rakete",
//	L"", // dart
//};

CHAR16 WeaponType[][30] =
{
	L"Andere",
	L"Pistole",
	L"MP",
	L"Schwere MP",
	L"Gewehr",
	L"SSG",
	L"SG",
	L"LMG",
	L"Schrotflinte",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Spielzug Spieler",
	L"Spielzug Gegner",
	L"Spielzug Monster",
	L"Spielzug Miliz",
	L"Spielzug Zivilisten",
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

	L"%s am Kopf getroffen, verliert einen Weisheitspunkt!",
	L"%s an der Schulter getroffen, verliert Geschicklichkeitspunkt!",
	L"%s an der Brust getroffen, verliert einen Kraftpunkt!",
	L"%s an den Beinen getroffen, verliert einen Beweglichkeitspunkt!",
	L"%s am Kopf getroffen, verliert %d Weisheitspunkte!",
	L"%s an der Schulter getroffen, verliert %d Geschicklichkeitspunkte!",
	L"%s an der Brust getroffen, verliert %d Kraftpunkte!",
	L"%s an den Beinen getroffen, verliert %d Beweglichkeitspunkte!",
	L"Unterbrechung!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //obsolete
	L"Verstärkung ist angekommen!",

	// In the following four lines, all %s's are merc names

	L"%s lädt nach.",
	L"%s hat nicht genug Action-Punkte!",
	L"%s leistet Erste Hilfe. (Rückgängig mit beliebiger Taste.)",
	L"%s und %s leisten Erste Hilfe. (Rückgängig mit beliebiger Taste.)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"zuverlässig",
	L"unzuverlässig",
	L"Reparatur leicht",
	L"Reparatur schwer",
	L"große Durchschlagskraft",
	L"kleine Durchschlagskraft",
	L"feuert schnell",
	L"feuert langsam",
	L"große Reichweite",
	L"kurze Reichweite",
	L"leicht",
	L"schwer",
	L"klein",
	L"schneller Feuerstoß",
	L"kein Feuerstoß",
	L"großes Magazin",
	L"kleines Magazin",

	// In the following two lines, all %s's are merc names

	L"%ss Tarnung hat sich abgenutzt.",
	L"%ss Tarnung ist weggewaschen.",

	// The first %s is a merc name and the second %s is an item name

	L"Zweite Waffe hat keine Munition!",
	L"%s hat %s gestohlen.",

	// The %s is a merc name

	L"%ss Waffe kann keinen Feuerstoß abgeben.",

	L"Sie haben schon eines davon angebracht.", 
	L"Gegenstände zusammenfügen?",

	// Both %s's are item names

	L"Sie können %s mit %s nicht kombinieren",

	L"Keine",
	L"Waffen entladen",		// = Removing ammo from weapons on the ground on pressing Shift + F if set in options.
	L"Modifikationen",

	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex: You cannot use sun goggles and your gas mask at the same time.
	L"Sie können %s nicht zusammen mit %s benutzen.",	//

	L"Der Gegenstand in Ihrem Cursor kann mit anderen Gegenständen verbunden werden, indem Sie ihn in einer der vier Einbaustellen platzieren.",
	L"Der Gegenstand in Ihrem Cursor kann mit anderen Gegenständen verbunden werden, indem Sie ihn in einer der vier Einbaustellen platzieren. (Aber in diesem Fall sind die Gegenstände nicht kompatibel.)",
	L"Es sind noch Feinde im Sektor!",
	L"Geben Sie %s %s", 
	L"%s am Kopf getroffen!",
	L"Kampf abbrechen?",
	L"Die Modifikation ist permanent. Weitermachen?",
	L"%s fühlt sich frischer!",
	L"%s ist auf Murmeln ausgerutscht!",
	L"%s konnte %s nicht aus der Hand des Feindes stehlen!",
	L"%s hat %s repariert",
	L"Unterbrechung für ",
	L"Ergeben?",
	L"Diese Person will keine Hilfe.", 
	L"Lieber NICHT!",
	L"Wenn Sie zu Skyriders Heli wollen, müssen Sie Söldner einem FAHRZEUG/HELIKOPTER ZUWEISEN.",
	L"%s hat nur Zeit, EINE Waffe zu laden",
	L"Spielzug Bloodcats", 
	L"Dauerfeuer",
	L"kein Dauerfeuer",
	L"genau",
	L"ungenau",
	L"kein Einzelschuss",
	L"Der Feind besitzt keine Gegenstände mehr zum Stehlen!",
	L"Der Feind hat keinen Gegenstand in seiner Hand!",

	L"%s's Wüstentarnung ist nicht mehr effektiv.",
	L"%s's Wüstentarnung wurde herunter gewaschen.",
	
	L"%s's Waldtarnung ist nicht mehr effektiv.",
	L"%s's Waldtarnung wurde herunter gewaschen.",
	
	L"%s's Stadttarnung ist nicht mehr effektiv.",
	L"%s's Stadttarnung wurde herunter gewaschen.",
	
	L"%s's Schneetarnung ist nicht mehr effektiv.",
	L"%s's Schneetarnung wurde herunter gewaschen.",
	L"Sie können %s nicht an dieser Einbaustelle anbringen.",
	L"%s passt in keine freie Einbaustelle.",
	L"Für diese Tasche ist nicht mehr genug Platz.",

	L"%s hat %s so gut wie möglich repariert.",
	L"%s hat %s's %s so gut wie möglich repariert.",

	L"%s hat %s gereinigt.",
	L"%s hat %s's %s gereinigt.",

	L"Assignment not possible at the moment",	// TODO.Translate
	L"No militia that can be drilled present.",

	L"%s hat %s vollständig erkundet.",
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
	L"5 Min",
	L"30 Min",
	L"60 Min",
	L"6 Std", 
};

// Assignment Strings: what assignment does the merc have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.
STR16 pAssignmentStrings[] =
{
	L"Trupp 1",
	L"Trupp 2",
	L"Trupp 3",
	L"Trupp 4",
	L"Trupp 5",
	L"Trupp 6",
	L"Trupp 7",
	L"Trupp 8",
	L"Trupp 9",
	L"Trupp 10",
	L"Trupp 11",
	L"Trupp 12",
	L"Trupp 13",
	L"Trupp 14",
	L"Trupp 15",
	L"Trupp 16",
	L"Trupp 17",
	L"Trupp 18",
	L"Trupp 19",
	L"Trupp 20",
	L"Dienst", // on active duty
	L"Doktor", // administering medical aid	
	L"Patient", // getting medical aid
	L"Fahrzeug", // in a vehicle
	L"Transit", // in transit - abbreviated form
	L"Repar.", // repairing
	L"Radio Scan",	// scanning for nearby patrols
	L"Üben", // training themselves
	L"Miliz", // training a town to revolt 
	L"M.Miliz", //training moving militia units		
	L"Trainer", // training a teammate
	L"Rekrut", // being trained by someone else 
	L"Umzug",	// move items
	L"Betrieb", // operating a strategic facility
	L"Essen",		// eating at a facility (cantina etc.)
	L"Pause", // Resting at a facility
	L"Verhör",		// Flugente: interrogate prisoners
	L"Tot", // dead
	L"Koma", // abbreviation for incapacitated	//LOOTF - "Unfähig" klingt schlimm. Geändert auf Koma. Vorschläge?
	L"Gefangen", // Prisoner of war - captured
	L"Hospital", // patient in a hospital		
	L"Leer",	//Vehicle is empty
	L"Snitch",	// facility: undercover prisoner (snitch)	// TODO.Translate
	L"Propag.",	// facility: spread propaganda
	L"Propag.",	// facility: spread propaganda (globally)
	L"Rumours",	// facility: gather information
	L"Propag.",	// spread propaganda
	L"Rumours",	// gather information
	L"Command",	// militia movement orders
	L"Diagnose", // disease diagnosis	//TODO.Translate
	L"Treat D.", // treat disease among the population
	L"Doktor", // administering medical aid	
	L"Patient", // getting medical aid
	L"Repar.", // repairing
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
	L"Miliz", // the title of the militia box
	L"Ohne Aufgabe", //the number of unassigned militia troops
	L"Mit Feinden im Sektor können Sie keine Miliz einsetzen!",
	L"Einige Milizen wurden keinem Sektor zugewiesen. Möchten Sie diese Einheiten auflösen?",
};

STR16 pMilitiaButtonString[] = 
{
	L"Autom.", // auto place the militia troops for the player
	L"Fertig", // done placing militia troops
	L"Auflösen", // HEADROCK HAM 3.6: Disband militia
	L"Zuordnungen aufh.", // move all milita troops to unassigned pool
};

STR16 pConditionStrings[] = 
{
	L"Sehr gut", //the state of a soldier .. excellent health
	L"Gut", // good health
	L"Mittel", // fair health
	L"Verwundet", // wounded health
	L"Erschöpft", // tired
	L"Verblutend", // bleeding to death
	L"Bewusstlos", // knocked out 
	L"Stirbt", // near death
	L"Tot", // dead
};

STR16 pEpcMenuStrings[] = 
{
	L"Dienst", // set merc on active duty
	L"Patient", // set as a patient to receive medical aid
	L"Fahrzeug", // tell merc to enter vehicle
	L"Unbewacht", // let the escorted character go off on their own
	L"Abbrechen", // close this menu
};

// look at pAssignmentString above for comments
STR16 pPersonnelAssignmentStrings[] =
{
	L"Trupp 1",
	L"Trupp 2",
	L"Trupp 3",
	L"Trupp 4",
	L"Trupp 5",
	L"Trupp 6",
	L"Trupp 7",
	L"Trupp 8",
	L"Trupp 9",
	L"Trupp 10",
	L"Trupp 11",
	L"Trupp 12",
	L"Trupp 13",
	L"Trupp 14",
	L"Trupp 15",
	L"Trupp 16",
	L"Trupp 17",
	L"Trupp 18",
	L"Trupp 19",
	L"Trupp 20",
	L"Dienst",
	L"Doktor",
	L"Patient",
	L"Fahrzeug",
	L"Transit",
	L"Reparieren",
	L"Radio Scan",	// radio scan
	L"Üben", 
	L"Miliz",
	L"Trainiere Mobile Miliz",
	L"Trainer",
	L"Rekrut",
	L"Gegenstand verschieben",
	L"Betriebspersonal",
	L"Essen",		// eating at a facility (cantina etc.)
	L"Betriebspause",
	L"Gefangene verhören",		// Flugente: interrogate prisoners
	L"Tot",
	L"Koma",			//LOOTF - s.o.
	L"Gefangen",
	L"Hospital", 
	L"Leer",			// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spreading Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gathering Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spreading Propaganda",// TODO.Translate	// spread propaganda
	L"Gathering Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Doktor",
	L"Patient",
	L"Reparieren",
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
	L"Trupp 1",
	L"Trupp 2",
	L"Trupp 3",
	L"Trupp 4",
	L"Trupp 5",
	L"Trupp 6",
	L"Trupp 7",
	L"Trupp 8",
	L"Trupp 9",
	L"Trupp 10",
	L"Trupp 11",
	L"Trupp 12",
	L"Trupp 13",
	L"Trupp 14",
	L"Trupp 15",
	L"Trupp 16",
	L"Trupp 17",
	L"Trupp 18",
	L"Trupp 19",
	L"Trupp 20",
	L"Dienst",
	L"Doktor",
	L"Patient",
	L"Fahrzeug",
	L"Transit",
	L"Reparieren",
	L"Radio Scan",	// radio scan
	L"Üben", 
	L"Miliz",
	L"Trainiere Mobile",
	L"Trainer",
	L"Rekrut",
	L"Umzug",	// move items
	L"Betriebspersonal",
	L"Betriebspause",
	L"Gefangene verhören",		// Flugente: interrogate prisoners
	L"Tot",
	L"Unfähig",
	L"Gefangen",
	L"Hospital", // patient in a hospital 
	L"Leer",	// Vehicle is empty
	L"Undercover Snitch",	// TODO.Translate	// facility: undercover prisoner (snitch)
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda
	L"Spread Propaganda",// TODO.Translate	// facility: spread propaganda (globally)
	L"Gather Rumours",// TODO.Translate			// facility: gather rumours	
	L"Spread Propaganda",// TODO.Translate	// spread propaganda
	L"Gather Rumours",// TODO.Translate			// gather information
	L"Commanding Militia",	// militia movement orders	// TODO.Translate
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Doktor",
	L"Patient",
	L"Reparieren",
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
	L"Vertragsoptionen:", 
	L"", // a blank line, required
	L"Einen Tag anbieten", // offer merc a one day contract extension
	L"Eine Woche anbieten", // 1 week
	L"Zwei Wochen anbieten", // 2 week
	L"Entlassen", //end merc's contract (used to be "Terminate")
	L"Abbrechen", // stop showing this menu
};

STR16 pPOWStrings[] = 
{
	L"gefangen", //an acronym for Prisoner of War 
	L"??",
};

STR16 pLongAttributeStrings[] =
{
	L"KRAFT", //The merc's strength attribute. Others below represent the other attributes.
	L"GESCHICKLICHKEIT",
	L"BEWEGLICHKEIT", 
	L"WEISHEIT", 
	L"TREFFSICHERHEIT",
	L"MEDIZIN",
	L"TECHNIK",
	L"FÜHRUNGSQUALITÄT",
	L"SPRENGSTOFFE",
	L"ERFAHRUNGSSTUFE",
};

STR16 pInvPanelTitleStrings[] = 
{
	L"Rüstung", // the armor rating of the merc
	L"Gew.", // the weight the merc is carrying
	L"Tarn.", // the merc's camouflage rating
	L"Tarnung:",
	L"Rüstung:",
};

STR16 pShortAttributeStrings[] =
{
	L"Bew", // the abbreviated version of : agility
	L"Ges", // dexterity
	L"Krf", // strength
	L"Fhr", // leadership
	L"Wsh", // wisdom
	L"Erf", // experience level
	L"Trf", // marksmanship skill
	L"Tec", // mechanical skill
	L"Spr", // explosive skill
	L"Med", // medical skill
};

STR16 pUpperLeftMapScreenStrings[] =
{
	L"Aufgabe", // the mercs current assignment 
	L"Vertrag", // the contract info about the merc
	L"Gesundh.", // the health level of the current merc 
	L"Moral", // the morale of the current merc
	L"Zustand",	// the condition of the current vehicle
	L"Tank",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Üben", // tell merc to train self 
	L"Miliz", // tell merc to train town // 
	L"Trainer", // tell merc to act as trainer
	L"Rekrut", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] = 
{
	L"Schussrate:", // the allowable rate of fire for a merc who is guarding
	L" Aggressiv feuern", // the merc can be aggressive in their choice of fire rates
	L" Munition sparen", // conserve ammo 
	L" Nur bei Bedarf feuern", // fire only when the merc needs to 
	L"Andere Optionen:", // other options available to merc
	L" Rückzug möglich", // merc can retreat
	L" Deckung möglich", // merc is allowed to seek cover
	L" Kann Kameraden helfen", // merc can assist teammates
	L"Fertig", // done with this menu
	L"Abbruch", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player
STR16 pOtherGuardMenuStrings[] = 
{
	L"Schussrate:",
	L" *Aggressiv feuern*",
	L" *Munition sparen*",
	L" *Nur bei Bedarf feuern*",
	L"Andere Optionen:",
	L" *Rückzug möglich*",
	L" *Deckung möglich*",
	L" *Kann Kameraden helfen*",
	L"Fertig",
	L"Abbruch",
};

STR16 pAssignMenuStrings[] =
{
	L"Dienst", // merc is on active duty
	L"Doktor", // the merc is acting as a doctor
	L"Disease", // merc is a doctor doing diagnosis TODO.Translate
	L"Patient", // the merc is receiving medical attention
	L"Fahrzeug", // the merc is in a vehicle
	L"Repar.", // the merc is repairing items 
	L"Radio Scan", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Snitch", // TODO.Translate // anv: snitch actions
	L"Training", // the merc is training
	L"Miliz",		// all things militia
	L"Umzug",	// move items
	L"Fortify",		// fortify sector	// TODO.Translate
	L"Intel", // covert assignments	// TODO.Translate
	L"Administer",	// TODO.Translate
	L"Explore",	// TODO.Translate
	L"Betrieb", // the merc is using/staffing a facility
	L"Abbrechen", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Angreifen",		// set militia to aggresive
	L"Position halten",	// set militia to stationary
	L"Rückzug",		// retreat militia
	L"An meine Position",	// retreat militia
	L"Auf den Boden",	// retreat militia	
	L"Ducken",
	L"In Deckung gehen",
	L"Move to",	// TODO.Translate
	L"Alle: Angreifen", 
	L"Alle: Position halten",
	L"Alle: Rückzug",
	L"Alle: An meine Position",
	L"Alle: Ausschwärmen",
	L"Alle: Auf den Boden",
	L"Alle: Ducken",
	L"Alle: In Deckung gehen",
	//L"All: Find items",
	L"Abbrechen",		// cancel this menu
};

//Flugente
STR16 pTraitSkillsMenuStrings[] =
{
	// radio operator
	L"Artillerie befehligen",
	L"Kommunikation stören",
	L"Frequenzen scannen",
	L"Abhöraktion starten",
	L"Verstärkung rufen",
	L"Radiogerät ausschalten",

	// spy
	L"Hide assignment",		// TODO.Translate
	L"Get Intel assignment",

	// various
	L"Spotter",
	L"Fokus",
	L"Greifen",
};

//Flugente: short description of the above skills for the skill selection menu
STR16 pTraitSkillsMenuDescStrings[] =
{
	// radio operator
	L"Artillerieschlag befehligen von einem Sektor...",
	L"Alle Funkfrequenzen mit weißem Rauschen füllen, sodass eine Kommunikation nicht mehr möglich ist.",
	L"Nach Störsignalen scannen.",
	L"Das Radiogerät verwenden, um feindliche Bewegungen zu orten.",
	L"Verstärkung aus dem Nachbarsektor anfordern.",
	L"Radiogerät ausschalten.",

	// spy
	L"Assignment: hide among the population.",		// TODO.Translate
	L"Assignment: hide among the population and gather intel.",

	// various
	L"Bestimmtes Gebiet beobachten, damit Scharfschützen einen Bonus auf deren Treffsicherheit erhalten.",
	L"Increase interrupt modifier (penalty outside of area).",	// TODO.Translate
	L"Drag a person, corpse or structure while you move.",
};

STR16 pTraitSkillsDenialStrings[] =
{
	L"Benötigt:\n",
	L" - %d AP\n",
	L" - %s\n",
	L" - %s oder höher\n",
	L" - %s oder höher oder\n",
	L" - %d Minuten um fertig zu sein\n",
	L" - Mörser Positionen in Nachbarsektoren\n",
	L" - %s |o|d|e|r %s |u|n|d %s oder %s oder höher\n",
	L" - besessen von einem Dämon",
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

STR16 pRemoveMercStrings[] ={
	L"Söldner entfernen", // remove dead merc from current team
	L"Abbrechen",
};

STR16 pAttributeMenuStrings[] =
{
	L"Gesundheit",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Kraft",
	L"Führungsqualität",
	L"Treffsicherheit",
	L"Technik",
	L"Sprengstoffe",
	L"Medizin",
	L"Abbrechen",
};

STR16 pTrainingMenuStrings[] =
{
	L"Üben", // train yourself 
	L"Train workers",	// TODO.Translate
	L"Trainer", // train your teammates 
	L"Rekrut", // be trained by an instructor 
	L"Abbrechen", // cancel this menu
};

STR16 pSquadMenuStrings[] =
{
	L"Trupp  1",
	L"Trupp  2",
	L"Trupp  3",
	L"Trupp  4",
	L"Trupp  5",
	L"Trupp  6",
	L"Trupp  7",
	L"Trupp  8",
	L"Trupp  9",
	L"Trupp 10",
	L"Trupp 11",
	L"Trupp 12",
	L"Trupp 13",
	L"Trupp 14",
	L"Trupp 15",
	L"Trupp 16",
	L"Trupp 17",
	L"Trupp 18",
	L"Trupp 19",
	L"Trupp 20",
	L"Abbrechen",
};

STR16 pPersonnelTitle[] =
{
	L"Personal", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Gesundheit: ",	// Stat: Health of merc
	L"Beweglichkeit: ",	// Stat: Agility
	L"Geschicklichkeit: ",	// Stat: Dexterity
	L"Kraft: ",		// Stat: Strength
	L"Führungsqualität: ",	// Stat: Leadership
	L"Weisheit: ",		// Stat: Wisdom
	L"Erfahrungsstufe: ",	// Stat: Experience level
	L"Treffsicherheit: ", 	// Stat: Marksmanship
	L"Technik: ",		// Stat: Mechanical
	L"Sprengstoffe: ",	// Stat: Explosives
	L"Medizin: ",		// Stat: Medical
	L"Med. Vorsorge: ",	// amount of medical deposit put down on the merc
	L"Laufzeit: ",		// time remaining on current contract
	L"Getötet: ",		// number of kills by merc
	L"Mithilfe: ",		// number of assists on kills by merc
	L"Tgl. Kosten:",	// daily cost of merc
	L"Gesamtkosten:",	// total cost of merc
	L"Vertrag:",		// cost of current contract
	L"Diensttage:",		// total service rendered by merc
	L"Schulden:",		// amount left on MERC merc to be paid
	L"Trefferquote:",	// percentage of shots that hit target
	L"Einsätze:",		// number of battles fought
	L"Verwundungen:",	// number of times merc has been wounded
	L"Fähigkeiten:", 	// Traits
	L"Keine Fähigkeiten:",	// No traits
	L"Aktivitäten:", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	// GETÖTET
	// 0
	L"Elite Soldaten: %d\n",
	L"Reguläre Soldaten: %d\n",
	L"Admin Soldaten: %d\n",
	L"Feindliche Gruppen: %d\n",
	L"Monster: %d\n",	
	L"Panzer: %d\n", 
	L"Andere: %d\n",

	// MITHILFE
	// 7
	L"Söldner: %d\n",
	L"Miliz: %d\n",
	L"Andere: %d\n",

	// TREFFERQUOTE
	// 10
	L"Schüsse gefeuert: %d\n",
	L"Raketen gefeuert: %d\n",
	L"Granaten geworfen: %d\n",
	L"Messer geworfen: %d\n",
	L"Klinge attakiert: %d\n",
	L"Nahkampf attakiert: %d\n",
	L"Gelandete Treffer: %d\n",

	// AKTIVITÄTEN
	// 17
	L"Schlösser geknackt: %d\n",
	L"Schlösser gebrochen: %d\n",
	L"Fallen entschärft: %d\n",
	L"Sprenstoffe entzündet: %d\n",
	L"Gegenstände repariert: %d\n",
	L"Gegenstände kombiniert: %d\n",
	L"Gegenstände gestohlen: %d\n",
	L"Miliz trainiert: %d\n",
	L"Soldaten verbunden: %d\n",
	L"Operation gemacht: %d\n",
	L"Personen bekanntgemacht: %d\n",
	L"Sektoren erkundet: %d\n",
	L"Hinterhalte vermieden: %d\n",
	L"Aufträge erledigt: %d\n",

	// EINSÄTZE
	// 31
	L"Taktische Kämpfe: %d\n",
	L"Automatische Kämpfe: %d\n",
	L"Fluchtversuche: %d\n",
	L"Erfolgreiche Hinterhalte: %d\n",
	L"Schwerster Kampf: %d Feinde\n",

	// VERWUNDUNGEN
	// 36
	L"Angeschossen: %d\n",
	L"Angestochen: %d\n",
	L"Geschlagen: %d\n",
	L"Explosionsverletzungen: %d\n",
	L"Schaden erlitten in Anlagen: %d\n",
	L"Operationen ertragen: %d\n",
	L"Unfälle in Anlagen: %d\n",

	// 43
	L"Charakter:",
	L"Schwächen:",

	L"Persönlichkeit:",

	L"Zombies: %d\n",

	L"Werdegang:",
	L"Personalität:",

	L"Prisoners interrogated: %d\n",	// TODO.Translate
	L"Diseases caught: %d\n",
	L"Total damage received: %d\n",
	L"Total damage caused: %d\n",
	L"Total healing: %d\n",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Keine Fähigkeiten",
	L"Schlösser knacken",
	L"Nahkampf",
	L"Elektronik",
	L"Nachteinsatz",
	L"Werfen",
	L"Lehren",
	L"Schwere Waffen",
	L"Autom. Waffen",
	L"Schleichen",
	L"Geschickt",
	L"Dieb",
	L"Kampfsport",
	L"Messer",
	L"Scharfschütze",
	L"Getarnt",
	L"(Experte)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Keine Fertigkeit",

	L"MG-Schütze",
	L"Grenadier",
	L"Präzisionsschütze",
	L"Pfadfinder",
	L"Pistolenschütze",
	L"Faustkämpfer",
	L"Gruppenführer",
	L"Mechaniker",
	L"Sanitäter",	
	// Minor traits
	L"Beidhänder",
	L"Messerkämpfer",
	L"Messerwerfer",
	L"Nachtmensch",
	L"Schleicher",
	L"Läufer",
	L"Kraftsportler",
	L"Sprengmeister",
	L"Ausbilder",
	L"Aufklärer",
	// covert ops is a major trait that was added later
	L"Geheimagent",
	// new minor traits
	L"Funker",	// 21
	L"Snitch",	// 22	// TODO.Translate
	L"Survival",

	// second names for major skills
	L"MG-Veteran",		// 24
	L"Artillerist",
	L"Scharfschütze",
	L"Jäger",
	L"Revolverheld",
	L"Kampfsportler",
	L"Zugführer",
	L"Ingenieur",
	L"Arzt",
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
	L"Spion",			// 43
	L"Placeholder",		// for radio operator (minor trait)
	L"Placeholder",		// for snitch(minor trait)
	L"Placeholder",		// for survival (minor trait)
	L"Mehr...",			// 47
	L"Intel",			// for INTEL	// TODO.Translate
	L"diverse",			// for VARIOUSSKILLS
};

// This is pop up help text for the options that are available to the merc
STR16 pTacticalPopupButtonStrings[] =
{
	L"|Stehen/Gehen",
	L"Kauern/Kauernd bewegen (|C)",
	L"Stehen/|Rennen",
	L"Hinlegen/Kriechen (|P)",
	L"B|licken",
	L"Aktion",
	L"Reden",
	L"Untersuchen (|C|t|r|l)",

	//Pop up door menu
	L"Manuell öffnen",
	L"Auf Fallen untersuchen",
	L"Dietrich",
	L"Mit Gewalt öffnen",
	L"Falle entschärfen",
	L"Abschließen",
	L"Aufschließen",
	L"Schloss aufsprengen",
	L"Brecheisen benutzen",
	L"Rückgängig (|E|s|c)",
	L"Schließen",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.
STR16 pDoorTrapStrings[] =
{
	L"keine Falle",
	L"eine Sprengstofffalle",
	L"eine elektrische Falle",
	L"eine Falle mit Sirene",
	L"eine Falle mit stummem Alarm",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.
STR16 pContractExtendStrings[] =
{
	L"1 Tag",
	L"1 Woche",
	L"2 Wochen",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.
STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Charakter auswählen",
	L"Söldner einteilen",
	L"Marschroute",

	//The new 'c' key activates this option. Either reword this string to include a 'c' in it, or leave as is.
	L"Vertrag für Söldner (|c)",

	L"Söldner entfernen",
	L"Schlafen", 
};

// volumes of noises 
STR16 pNoiseVolStr[] =
{
	L"LEISE",
	L"DEUTLICH",
	L"LAUT",
	L"SEHR LAUT",
};

// types of noises 
STR16 pNoiseTypeStr[] =
{
	L"EIN UNBEKANNTES GERÄUSCH",
	L"EINE BEWEGUNG",
	L"EIN KNARREN",
	L"EIN KLATSCHEN",
	L"EINEN AUFSCHLAG",
	L"EINEN SCHUSS",
	L"EINE EXPLOSION",
	L"EINEN SCHREI",
	L"EINEN AUFSCHLAG",
	L"EINEN AUFSCHLAG",
	L"EIN ZERBRECHEN",
	L"EIN ZERSCHMETTERN",
};

// Directions that are used throughout the code for identification.
STR16 pDirectionStr[] = 
{
	L"NORDOSTEN",
	L"OSTEN",
	L"SÜDOSTEN",
	L"SÜDEN",
	L"SÜDWESTEN",
	L"WESTEN",
	L"NORDWESTEN",
	L"NORDEN",
};

// These are the different terrain types. 
STR16 pLandTypeStrings[] =
{
	L"Stadt",
	L"Straße",
	L"Ebene",
	L"Wüste",
	L"Lichter Wald",
	L"Dichter Wald",	 
	L"Sumpf",
	L"See/Ozean",	
	L"Hügel",
	L"Unpassierbar",
	L"Fluss",		//river from north to south
	L"Fluss",		//river from east to west
	L"Fremdes Land",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Tropen",
	L"Farmland",
	L"Ebene, Straße",
	L"Wald, Straße", 
	L"Farm, Straße",
	L"Tropen, Straße",
	L"Wald, Straße", 
	L"Küste",
	L"Berge, Straße",
	L"Küste, Straße",
	L"Wüste, Straße",
	L"Sumpf, Straße",
	L"Wald, Raketen",
	L"Wüste, Raketen",
	L"Tropen, Raketen",
	L"Meduna, Raketen",

	//These are descriptions for special sectors
	L"Cambria Hospital",
	L"Drassen Flugplatz", 
	L"Meduna Flugplatz",
	L"Raketen",
	L"Tankstelle",	// refuel site
	L"Rebellenlager", //The rebel base underground in sector A10
	L"Tixa, Keller",	//The basement of the Tixa Prison (J9)
	L"Monsterhöhle",	//Any mine sector with creatures in it
	L"Orta, Keller",	//The basement of Orta (K4)
	L"Tunnel",				//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Bunker",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	//     The first %s can either be bloodcats or enemies.
	L"",	//Unused
	L"%s wurden entdeckt in Sektor %c%d und ein weiterer Trupp wird gleich ankommen.",	//STR_DETECTED_SINGULAR
	L"%s wurden entdeckt in Sektor %c%d und weitere Trupps werden gleich ankommen.",	//STR_DETECTED_PLURAL
	L"Gleichzeitige Ankunft koordinieren?",							//STR_COORDINATE

	//Dialog strings for enemies.

	L"Feind bietet die Chance zum Aufgeben an.",						//STR_ENEMY_SURRENDER_OFFER
	L"Feind hat restliche bewusstlose Söldner gefangen genommen.",				//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Rückzug", 		//The retreat button						//STR_AR_RETREAT_BUTTON
	L"Fertig",		//The done button						//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"VERTEIDIGUNG",									//STR_AR_DEFEND_HEADER
	L"ANGRIFF",										//STR_AR_ATTACK_HEADER
	L"BEGEGNUNG",										//STR_AR_ENCOUNTER_HEADER
	L"Sektor",		//The Sector A9 part of the header				//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"SIEG!",										//STR_AR_OVER_VICTORY
	L"NIEDERLAGE!",										//STR_AR_OVER_DEFEAT
	L"AUFGEGEBEN!",										//STR_AR_OVER_SURRENDERED
	L"GEFANGENGENOMMEN!",									//STR_AR_OVER_CAPTURED
	L"AUSGEWICHEN!",									//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Miliz",									//STR_AR_MILITIA_NAME,
	L"Elite",									//STR_AR_ELITE_NAME,
	L"Soldat",									//STR_AR_TROOP_NAME,
	L"Admin.",									//STR_AR_ADMINISTRATOR_NAME,
	L"Monster",									//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Zeit verstrichen",								//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating. (UPPERCASE)			

	L"IST AUSGEWICHEN",								//STR_AR_MERC_RETREATED,
	L"WEICHT AUS",									//STR_AR_MERC_RETREATING,
	L"RÜCKZUG",									//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS
	//Goes on the three buttons in the prebattle interface. The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Autom. Kampf",									//STR_PB_AUTORESOLVE_BTN,
	L"Gehe zu Sektor",								//STR_PB_GOTOSECTOR_BTN,
	L"Rückzug",									//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"FEINDBEGEGNUNG",							
	L"FEINDLICHE INVASION",					
	L"FEINDLICHER HINTERHALT",
	L"FEINDLICHEN SEKTOR BETRETEN",				
	L"MONSTERANGRIFF",
	L"BLOODCAT-HINTERHALT",							
	L"BLOODCAT-HÖHLE BETRETEN",
	L"ENEMY AIRDROP",					//STR_PB_ENEMYINVASION_AIRDROP_HEADER	// TODO.Translate

	//Various single words for direct translation. The Civilians represent the civilian
	//militia occupying the sector being attacked. Limited to 9-10 chars

	L"Ort",
	L"Feinde",
	L"Söldner",
	L"Miliz",
	L"Monster",
	L"Bloodcats",
	L"Sektor",
	L"Keine",			//If there are no uninvolved mercs in this fight.
	L"n.a.",			//Acronym of Not Applicable
	L"T",				//One letter abbreviation of day
	L"h",				//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Räumen",
	L"Verteilen",
	L"Gruppieren",
	L"Fertig",

	//The help text for the four buttons. Use \n to denote new line (just like enter).

	L"Söldner räumen ihre Positionen\n und können manuell neu platziert werden. (|C)", 
	L"Söldner |schwärmen in alle Richtungen aus\n wenn der Button gedrückt wird.",
	L"Mit diesem Button können Sie wählen, wo die Söldner |gruppiert werden sollen.",
	L"Klicken Sie auf diesen Button, wenn Sie die\n Positionen der Söldner gewählt haben. (|E|n|t|e|r)",
	L"Sie müssen alle Söldner positionieren\n bevor die Schlacht beginnt.",

	//Various strings (translate word for word)

	L"Sektor",
	L"Eintrittspunkte wählen",

	//Strings used for various popup message boxes. Can be as long as desired.

	L"Das sieht nicht gut aus. Gelände ist unzugänglich. Versuchen Sie es an einer anderen Stelle.",
	L"Platzieren Sie Ihre Söldner in den markierten Sektor auf der Karte.",

	//This message is for mercs arriving in sectors. Ex: Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"ist angekommen im Sektor", 

	//These entries are for button popup help text for the prebattle interface. All popup help
	//text supports the use of \n to denote new line. Do not use spaces before or after the \n.
	L"Entscheidet Schlacht |automatisch für Sie\nohne Karte zu laden.",
	L"Sie können den PC-Kampf-Modus nicht benutzen, während Sie\neinen vom Feind verteidigten Ort angreifen.",
	L"Sektor b|etreten und Feind in Kampf verwickeln.",
	L"Gruppe zum vorigen Sektor zu|rückziehen.",				//singular version
	L"Alle Gruppen zum vorigen Sektor zu|rückziehen.", //multiple groups with same previous sector

	//various popup messages for battle conditions. 

	//%c%d is the sector -- ex: A9
	L"Feinde attackieren Ihre Miliz im Sektor %c%d.",
	//%c%d is the sector -- ex: A9
	L"Monster attackieren Ihre Miliz im Sektor %c%d.",
	//1st %d refers to the number of civilians eaten by monsters, %c%d is the sector -- ex: A9
	//Note: the minimum number of civilians eaten will be two.
	L"Monster attackieren und töten %d Zivilisten im Sektor %s.",
	//%s is the sector -- ex: A9
	L"Feinde attackieren Ihre Söldner im Sektor %s. Alle Söldner sind bewusstlos!",
	//%s is the sector -- ex: A9
	L"Monster attackieren Ihre Söldner im Sektor %s. Alle Söldner sind bewusstlos!",

	// Flugente: militia movement forbidden due to limited roaming	// TODO.Translate
	L"Militia cannot move here (RESTRICT_ROAMING = TRUE).",
	L"War room isn't staffed - militia move aborted!",

	L"Robot",								//STR_AR_ROBOT_NAME, // TODO: translate
	L"Panzer",								//STR_AR_TANK_NAME,
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
	//This is the day represented in the game clock. Must be very short, 4 characters max.
	L"Tag",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2]=
{
	L"gefunden im Sektor:",
	L"gefunden am:",
};

//The headers used to describe various weapon statistics.
CHAR16		gWeaponStatsDesc[][ 20 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Status:",
	L"Gew.:", //weight
	L"AP Kosten",	
	L"Reichw.:",		// Range
	L"Schaden:",		
	L"Anzahl:", 		// Number of bullets left in a magazine
	L"AP:",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Genauigkeit:",	//9
	L"Reichweite:",		//10	
	L"Schaden:", 		//11
	L"Gewicht:",		//12
	L"Bet. Schaden:",	//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Zubehör:",	//14	// Attachments
	L"AUTO/5:",		//15
	L"Verf. Munition:",		//16

	L"Standard:",	//17 //WarmSteel - So we can also display default attachments
	L"Schmutz:",	// 18	//added by Flugente
	L"Platz:", // 19 //space left on Molle items
	L"Streumuster:",	// 20

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box

STR16		gzWeaponStatsFasthelpTactical[ 33 ] =
{	
	L"|R|e|i|c|h|w|e|i|t|e\n \nDie effektive Reichweite dieser Waffe\nAngriffe jenseits dieser Reichweite führt zu drastischen Genauigkeitseinbußen.\n \nHöher ist besser.",
	L"|S|c|h|a|d|e|n\n \nDas Schadenspotential der Waffe.\nDie Waffe wird in der Regel diesen \n(oder ähnlichen) Schaden an ungeschützten Zielen verursachen.\n \nHöher ist besser.",
	L"|G|e|n|a|u|i|g|k|e|i|t\n \nDieser Wert gibt die Exaktheit der Waffe\n(Trefferwahrscheinlichkeit) an, je nachdem,\nwie gut oder schlecht das Design der Waffe ist.\n \nHöher ist besser.",
	L"|Z|i|e|l|g|e|n|a|u|i|g|k|e|i|t\n \nThis is the maximum number of aiming clicks allowed\nwhen using this gun.\n \nEach aiming-click will make an attack more\naccurate.\n \nHöher ist besser.",
	L"|Z|i|e|l|e|n|-|M|o|d|i|f|i|k|a|t|o|r\n \nEin Modifikator, welcher die Wirksamkeit\nder Waffe bei jedem Zielgenauigkeits-Klick verändert\n \nHöher ist besser.",
	L"|M|i|n|. |R|e|i|c|h|w|e|i|t|e |f|ü|r |Z|i|e|l|e|n|-|B|o|n|u|s\n \nDer minimale Bereich zum Ziel welcher erforderlich ist,\num den Zielen-Modifikator verwenden zu können.\n \nNiedriger ist besser.",
	L"|T|r|e|f|f|e|r|-|M|o|d|i|f|i|k|a|t|o|r\n \nEin Modifikator zur Trefferwahrscheinlichkeit\nbei jedem Schuss der mit dieser Waffe abgefeuert wird.\n \nHöher ist besser.",
	L"|O|p|t|i|m|a|l|e |L|a|s|e|r |R|e|i|c|h|w|e|i|t|e\n \nDie Entfernung (in Felder) bei der die angebrachte Lasermarkierung\nauf der Waffe die beste Effektivität erreicht.\n \nHöher ist besser.",
	L"|M|ü|n|d|u|n|g|s|f|e|u|e|r |U|n|t|e|r|d|r|ü|c|k|u|n|g\n \nWenn dieses Symbol erscheint, bedeutet dies, dass die Waffe\nkein Mündungsfeuer verursacht, wenn geschossen wird.",
	L"|L|a|u|t|s|t|ä|r|k|e\n \nAngriffe mit dieser Waffe können bis zu der\nangezeigten Distanz (in Felder) gehört werden.\n \nNiedriger ist besser.",
	L"|Z|u|v|e|r|l|ä|s|s|i|g|k|e|i|t\n \nDieser Wert zeigt an, wie schnell die Waffe\nim Kampf bei Benützung schadhaft werden kann.\n \nHöher ist besser.",
	L"|R|e|p|a|r|a|t|u|r|l|e|i|c|h|t|i|g|k|e|i|t\n \nBestimmt, wie schwierig es ist, die Waffe\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur Techniker und spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
	L"",	//12
	L"APs zum Anlegen",
	L"APs für Einzelschuss",
	L"APs für Feuerstoß",
	L"APs für Autofeuer",
	L"APs zum Nachladen",
	L"APs zum manuellen Nachladen",
	L"Feuerstoß-Streuung (Niedriger ist besser)",	//19
	L"Zweibein-Modifikator",
	L"Autofeuer: Schüsse je 5 AP",
	L"Autofeuer-Streuung (Niedriger ist besser)",
	L"Burst/Auto-Streuung (Niedriger ist besser)",	//23
	L"APs zum Werfen",
	L"APs zum Abschießen",
	L"APs zum Stechen",
	L"Kein Einzelschuss!",
	L"Kein Feuerstoß!",
	L"Kein Autofeuer!",
	L"APs zum Schlagen",
	L"",
	L"|R|e|p|a|r|a|t|u|r|l|e|i|c|h|t|i|g|k|e|i|t\n \nBestimmt, wie schwierig es ist, die Waffe\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
};

STR16		gzMiscItemStatsFasthelp[] =
{
	L"Gegenstandsgrößen-Modifikator (Niedriger ist besser)",
	L"Zuverlässigkeits-Modifikator",
	L"Schalldämpfung (Niedriger ist besser)",
	L"Mündungsfeuerdämpfung",
	L"Zweibein-Modifikator",
	L"Reichweiten-Modifikator",
	L"Treffer-Modifikator",
	L"Beste Laser-Reichweite",
	L"Zielen-Bonus-Modifikator",
	L"Schusszahlmodifikator Feuerstoß",			//LOOTF - geändert von "Feuerstoßgrößen-Modifikator"
	L"Feuerstoßstreuungs-Modifikator",
	L"Dauerfeuerstreuungs-Modifikator",
	L"AP-Modifikator",
	L"AP-Modifikator Feuerstoß (Niedriger ist besser)",	//LOOTF - geändert von "AP für Feuerstoß Modifikator.."
	L"AP-Modifikator Dauerfeuer (Niedriger ist besser)",	//LOOTF - geändert von "AP für Autofeuer Modifikator.."
	L"AP-Modifikator Waffenvorhalt",				//LOOTF - geändert von "AP für Anlegen Modifikator"
	L"AP-Modifikator Nachladen",				//LOOTF - geändert von "AP für Nachladen Mofifikator"
	L"Magazingrößen-Modifikator",
	L"AP-Modifikator für Angriff",				//LOOTF - geändert von "AP für Angriff Modifikator"
	L"Schaden-Modifikator",
	L"Nahkampf-Modifikator",
	L"Waldtarnung",
	L"Stadt-Tarnung",
	L"Wüstentarnung",
	L"Schneetarnung",
	L"Anschleichen-Modifikator",
	L"Hörweiten-Modifikator",
	L"Sichtweiten-Modifikator",
	L"Tagsichtweiten-Modifikator",
	L"Nachtsichtweiten-Modifikator",
	L"Grelles-Licht-Modifikator",
	L"Höhlensicht-Modifikator",
	L"Tunnelblick-Modifikator (Niedriger ist besser)",
	L"Minimale Reichweite für Zielbonus",
	L"Hold |C|t|r|l to compare items", // item compare help text // TODO.Translate
	L"Gesamtgewicht: %4.1f kg",	// 35
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"In ",
	L"PERFEKTEM",
	L"EXZELLENTEM",
	L"GUTEM",
	L"NORMALEM",
	L"NICHT GUTEM",
	L"SCHLECHTEM",
	L"SCHRECKLICHEM",
	L" Zustand."
};

//The headers used for the merc's money.
CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Betrag",
	L"verbleibend:", //this is the overall balance
	L"Betrag",
	L"teilen:",	// the amount he wants to separate from the overall balance to get two piles of money

	L"Konto",	
	L"Saldo:",		
	L"Betrag",	
	L"abheben:",	
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 
CHAR16 zHealthStr[][13] =	//used to be 10
{
	L"STIRBT",	//	>= 0
	L"KRITISCH", 	//	>= 15
	L"SCHLECHT",	//	>= 30
	L"VERWUNDET",	//	>= 45
	L"GESUND",	//	>= 60
	L"STARK",	// 	>= 75
	L"SEHR GUT",	// 	>= 90
	L"GEFANGEN",	// added by Flugente
};

STR16 gzHiddenHitCountStr[1] =
{
	L"?",	
};

STR16 gzMoneyAmounts[6] = 
{ 
	L"$1000",
	L"$100",
	L"$10",
	L"OK", 
	L"Abheben", 	// Money from pile
	L"Abheben",	// Money from account
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons." 
CHAR16 gzProsLabel[10] = 
{
	L"Pro:",
};

CHAR16 gzConsLabel[10] = 
{
	L"Kont:",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Wie bitte?", 		//meaning "Repeat yourself" 
	L"Freundlich",		//approach in a friendly
	L"Direkt",		//approach directly - let's get down to business
	L"Drohen",		//approach threateningly - talk now, or I'll blow your face off
	L"Geben",		
	L"Rekrutieren",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Handeln",	
	L"Kaufen",
	L"Verkaufen",
	L"Reparieren",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"Fertig",
};

STR16 pVehicleStrings[] =
{
	L"Eldorado",
	L"Hummer",		// a hummer jeep/truck -- military vehicle
	L"Eisverkaufswagen",
	L"Jeep",
	L"Panzer",
	L"Helikopter",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Laster",
	L"Jeep",
	L"Tank",
	L"Heli", 			// the helicopter
};

STR16 zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Laster",		// Ice cream truck
	L"Jeep",
	L"Panzer",
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
	L"Luftangriff",
	L"Automatisch Erste Hilfe leisten?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s bemerkt, dass Teile aus der Lieferung fehlen.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Das Schloss hat %s.", 
	L"Es gibt kein Schloss.",
	L"Erfolg!",
	L"Fehlschlag.",
	L"Erfolg!",
	L"Fehlschlag.",
	L"Das Schloss hat keine Falle.",
	L"Erfolg!",
	// The %s is a merc name
	L"%s hat nicht den richtigen Schlüssel.",
	L"Die Falle am Schloss ist entschärft.",
	L"Das Schloss hat keine Falle.",
	L"Geschl.",
	L"TÜR",
	L"FALLE AN",
	L"Geschl.",
	L"GEÖFFNET",
	L"EINGETRETEN",
	L"Hier ist ein Schalter. Betätigen?",
	L"Falle entschärfen?",
	L"Zurück...",
	L"Weiter...",
	L"Mehr...",

	// In the next 2 strings, %s is an item name

	L"%s liegt jetzt auf dem Boden.",
	L"%s ist jetzt bei %s.",

	// In the next 2 strings, %s is a name

	L"%s hat den vollen Betrag erhalten.",
	L"%s bekommt noch %d.",
	L"Detonationsfrequenz auswählen:",		//in this case, frequency refers to a radio signal
	L"Wie viele Züge bis zur Explosion:",		//how much time, in turns, until the bomb blows
	L"Ferngesteuerte Zündung einstellen:",		//in this case, frequency refers to a radio signal
	L"Falle entschärfen?",
	L"Blaue Flagge wegnehmen?",
	L"Blaue Flagge hier aufstellen?",
	L"Zug beenden",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Wollen Sie %s wirklich angreifen?",
	L"Fahrzeuge können ihre Haltung nicht ändern.",
	L"Der Roboter kann seine Haltung nicht ändern.",

	// In the next 3 strings, %s is a name

	//%s can't change to that stance here
	L"%s kann die Haltung hier nicht ändern.",

	L"%s kann hier nicht versorgt werden.",	
	L"%s braucht keine Erste Hilfe.",
	L"Kann nicht dorthin gehen.",
	L"Ihr Team ist komplett. Kein Platz mehr für Rekruten.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s wird rekrutiert.",

	// Here %s is a name and %d is a number

	L"%s bekommt noch %d $.",

	// In the next string, %s is a name

	L"%s eskortieren?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"%s für %s pro Tag anheuern?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Kämpfen?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"%s für %s kaufen?",

	// In the next string, %s is a name

	L"%s wird von Trupp %d eskortiert.",

	// These messages are displayed during play to alert the player to a particular situation

	L"KLEMMT",						//weapon is jammed.
	L"Roboter braucht Munition vom Kaliber %s.",		//Robot is out of ammo
	L"Dorthin werfen? Unmöglich.",				//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Schleichen (|Z)",
	L"Kartenbildschir|m",
	L"Spielzug been|den",
	L"Sprechen",
	L"Stumm",
	L"Aufrichten (|P|g|U|p)",
	L"Ebene wechseln (|T|a|b)",
	L"Klettern / Springen (|J)",
	L"Ducken (|P|g|D|n)",
	L"Untersuchen (|C|t|r|l)",
	L"Voriger Söldner",
	L"Nächster Söldner (|S|p|a|c|e)",
	L"|Optionen",
	L"Feuermodus (|B)",
	L"B|lickrichtung",
	L"Gesundheit: %d/%d\nEnergie: %d/%d\nMoral: %s",
	L"Was?",					//this means "what?" 
	L"Weiter",					//an abbrieviation for "Continued" (displayed on merc portrait)
	L"Schleichen aus für %s.",
	L"Schleichen an für %s.",
	L"Fahrer",
	L"Fahrzeug verlassen",
	L"Trupp wechseln",
	L"Fahren",
	L"n.a.",					//this is an acronym for "Not Applicable." 
	L"Benutzen ( Faustkampf )",
	L"Benutzen ( Feuerwaffe )",
	L"Benutzen ( Hieb-/Stichwaffe )",
	L"Benutzen ( Sprengstoff )",
	L"Benutzen ( Verbandskasten )",
	L"(Fangen)",
	L"(Nachladen)",
	L"(Geben)",
	L"%s Falle wurde ausgelöst.", 
	L"%s ist angekommen.",
	L"%s hat keine Action-Punkte mehr.",
	L"%s ist nicht verfügbar.",
	L"%s ist fertig verbunden.",
	L"%s sind die Verbände ausgegangen.",
	L"Feind im Sektor!",
	L"Keine Feinde in Sicht.",
	L"Nicht genug Action-Punkte.",
	L"Niemand bedient die Fernbedienung.",
	L"Feuerstoß hat Magazin geleert!",
	L"SOLDAT",
	L"MONSTER", 
	L"MILIZ",
	L"ZIVILIST",
	L"ZOMBIE",
	L"PRISONER",
	L"Sektor verlassen",
	L"OK",
	L"Abbruch",
	L"Gewählter Söldner",
	L"Ganzer Trupp",
	L"Gehe zu Sektor",

	L"Gehe zu Karte", 
	
	L"Sie können den Sektor von dieser Seite aus nicht verlassen.",
	L"Sie können den Sektor nicht verlassen im Rundenmodus.",
	L"%s ist zu weit weg.",
	L"Baumkronen entfernen",
	L"Baumkronen zeigen",
	L"KRÄHE",				//Crow, as in the large black bird
	L"NACKEN",
	L"KOPF",
	L"TORSO",
	L"BEINE",
	L"Der Herrin sagen, was sie wissen will?",
	L"Fingerabdruck-ID gespeichert",
	L"Falsche Fingerabdruck-ID. Waffe außer Betrieb",
	L"Ziel erfasst",
	L"Weg blockiert",
	L"Geld einzahlen/abheben",		//Help text over the $ button on the Single Merc Panel 
	L"Niemand braucht Erste Hilfe.",
	L"Klemmt.",				//Short form of JAMMED, for small inv slots
	L"Kann da nicht hin.",			// used ( now ) for when we click on a cliff
	L"Weg ist blockiert. Mit dieser Person den Platz tauschen?",
	L"Person will sich nicht bewegen",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Mit der Zahlung von %s einverstanden?",
	L"Gratisbehandlung akzeptieren?",
	L"%s heiraten?", //Daryl
	L"Schlüsselring",
	L"Das ist mit einem EPC nicht möglich.",
	L"%s verschonen?", //Krott
	L"Außer Reichweite",
	L"Arbeiter",	//People that work in mines to extract precious metals
	L"Fahrzeug kann nur zwischen Sektoren fahren",
	L"Automatische Erste Hilfe nicht möglich",
	L"Weg blockiert für %s",
	L"Ihre von %s Truppe gefangenen Soldaten sind hier inhaftiert", //Deidrannas
	L"Schloss getroffen",
	L"Schloss zerstört",
	L"Noch jemand an der Tür.",
	L"Gesundh.: %d/%d\nTank: %d/%d",
	L"%s kann %s nicht sehen.",		// Cannot see person trying to talk to
	L"Anbringung entfernt",
	L"Sie können kein weiteres Fahrzeug mehr verwenden, da Sie bereits 2 haben",

	// added by Flugente for defusing/setting up trap networks
	L"Detonations-Frequenz (1 - 4) oder Entschärfungs-Frequenz (A - D):",
	L"Entschärfungs-Frequenz:",
	L"Detonations-Frequenz (1 - 4) und die Entschärfungs-Frequenz (A - D):",
	L"Detonations-Zeit (in Züge) (1 - 4) und die Entschärfungs-Frequenz (A - D):",
	L"Stolperdraht-Hierarchie (1 - 4) und das Netzwerk (A - D):",

	// added by Flugente to display food status
	L"Gesundheit: %d/%d\nAusdauer: %d/%d\nMoral: %s\nWasser: %d%s\nEssen: %d%s",

	// added by Flugente: selection of a function to call in tactical
	L"Was möchten Sie tun?",
	L"Feldflasche auffüllen",
	L"Waffen reinigen (Merc)",
	L"Waffen reinigen (Team)",
	L"Kleidung ausziehen",
	L"Verkleidung loswerden",
	L"Miliz inspizieren",
	L"Miliz ausrüsten",
	L"Verkleidung testen",
	L"unused",

	// added by Flugente: decide what to do with the corpses
	L"Was möchten Sie mit der Leiche tun?",
	L"Enthaupten",
	L"Ausweiden",
	L"Kleidung nehmen",
	L"Leiche nehmen",

	// Flugente: weapon cleaning
	L"%s reinigte %s",

	// added by Flugente: decide what to do with prisoners
	L"As we have no prison, a field interrogation is performed.",		// TODO.Translate
	L"Field interrogation",
	L"Wohin mit den %d Gefangenen?",
	L"Freilassen",
	L"Was möchten Sie tun?",
	L"Kapitulation fordern",
	L"Kapitulation anbieten",
	L"Ablenken",
	L"Sprechen",
	L"Recruit Turncoat", // TODO: translate

    // added by sevenfm: disarm messagebox options, messages when arming wrong bomb
    L"Falle entschärfen",
    L"Falle untersuchen",
    L"Blaue Flagge entfernen",
    L"Sprengen!",
    L"Stolperdraht aktivieren",
    L"Stolperdraht deaktivieren",
    L"Stolperdraht freilegen",
    L"Kein Zünder/Fernzünder gefunden!",
    L"Diese Bombe ist bereits scharf!",
    L"Sicher",
    L"Fast sicher",
    L"Riskant",
    L"Gefährlich",
    L"Höchst gefährlich!",

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
	L"Der nächste Sektor wird sofort geladen, wenn Sie das Kästchen aktivieren.",
	L"Sie kommen sofort zum Kartenbildschirm, wenn Sie das Kästchen aktivieren\nweil die Reise Zeit braucht.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Der Sektor ist von Feinden besetzt. Sie können keine Söldner hier lassen.\nRegeln Sie das, bevor Sie neue Sektoren laden.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Wenn die restlichen Söldner den Sektor verlassen,\nwird sofort der nächste Sektor geladen.",
	L"Wenn die restlichen Söldner den Sektor verlassen,\nkommen Sie sofort zum Kartenbildschirm\nweil die Reise Zeit braucht.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted. The "single" button is disabled.
	L"%s kann den Sektor nicht ohne Eskorte verlassen.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s kann den Sektor nicht verlassen, weil er %s eskortiert.", //male singular
	L"%s kann den Sektor nicht verlassen, weil sie %s eskortiert.", //female singular
	L"%s kann den Sektor nicht verlassen, weil er mehrere Personen eskortiert.", //male plural
	L"%s kann den Sektor nicht verlassen, weil sie mehrere Personen eskortiert.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the "move all" option is disabled,
	//and this helptext explains why.
	L"Alle Söldner müssen in der Nähe sein,\ndamit der Trupp weiterreisen kann.",

	L"", //UNUSED

	//Standard helptext for single movement. Explains what will happen (splitting the squad)
	L"Bei aktiviertem Kästchen reist %s alleine und\nbildet automatisch wieder einen Trupp.",

	//Standard helptext for all movement. Explains what will happen (moving the squad)
	L"Bei aktiviertem Kästchen reist der ausgewählte Trupp\nweiter und verlässt den Sektor.",

	//This strings is used BEFORE the "exiting sector" interface is created. If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear. This is just like the situation where
	//This string is special, as it is not used as helptext. Do not use the special newline character (\n) for this string.
	L"%s wird von Söldnern eskortiert und kann den Sektor nicht alleine verlassen. Die anderen Söldner müssen in der Nähe sein.",
};

STR16 pRepairStrings[] =
{
	L"Gegenstände", 		// tell merc to repair items in inventory
	L"Raketenstützpunkt", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Abbruch", 			// cancel this menu
	L"Roboter", 			// repair the robot
};

// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 
STR16 sPreStatBuildString[] =
{
	L"verliert", 		// the merc has lost a statistic
	L"gewinnt", 		// the merc has gained a statistic
	L"Punkt",	// singular
	L"Punkte",	// plural
	L"Level",	// singular
	L"Level",	// plural
};

STR16 sStatGainStrings[] =
{
	L"Gesundheit.",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Weisheit.",
	L"an Medizinkenntnis.",
	L"an Sprengstoffkenntnis.",
	L"an Technikfähigkeit.",
	L"an Treffsicherheit.",
	L"Erfahrungsstufe(n).",
	L"an Kraft.",
	L"an Führungsqualität.",
};

STR16 pHelicopterEtaStrings[] =
{
	L"Gesamt: ", 			// total distance for helicopter to travel
	L" Sicher: ", 			// Number of safe sectors
	L" Unsicher:", 			// Number of unsafe sectors
	L"Gesamtkosten: ",		// total cost of trip by helicopter
	L"Ank.: ", 			// ETA is an acronym for "estimated time of arrival" 
	
	// warning that the sector the helicopter is going to use for refueling is under enemy control
	L"Helikopter hat fast keinen Sprit mehr und muss im feindlichen Gebiet landen.",	
	L"Passagiere: ",
	L"Skyrider oder Absprungsort auswählen?",
	L"Skyrider",
	L"Absprung",	//make sure length doesn't exceed 8 characters (used to be "Absprungsort")
	L"Helicopter is seriously damaged and must land in hostile territory!",	// warning that the sector the helicopter is going to use for refueling is under enemy control ->	// TODO.Translate
	L"Helicopter will now return straight to base, do you want to drop down passengers before?",			// TODO.Translate
	L"Remaining Fuel:",			// TODO.Translate
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
	L"Ebene:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Loyalität ", 			// the loyalty rating of a town ie : Loyal 53%
};

// error message for when player is trying to give a merc a travel order while he's underground.
STR16 gsUndergroundString[] =
{
	L"Ich kann unter der Erde keinen Marschbefehl empfangen.", 
};

STR16 gsTimeStrings[] =
{
	L"h",				// hours abbreviation
	L"m",				// minutes abbreviation
	L"s",				// seconds abbreviation
	L"T",				// days abbreviation
};

// text for the various facilities in the sector
STR16 sFacilitiesStrings[] =
{
	L"Keine",
	L"Krankenhaus",
	L"Fabrik",		// Factory
	L"Gefängnis",
	L"Militär",
	L"Flughafen",
	L"Schießanlage",	// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button
STR16 pMapPopUpInventoryText[] =
{
	L"Inventar",
	L"Exit",
	L"Repair",	// TODO.Translate
	L"Factories",	// TODO.Translate
};

// town strings
STR16 pwTownInfoStrings[] =
{
	L"Größe",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"unter Kontrolle", 				// how much of town is controlled
	L"Keine", 					// none of this town
	L"Mine", 					// mine associated with this town
	L"Loyalität",					// 5 // the loyalty level of this town
	L"Trainiert", 					// the forces in the town trained by the player
	L"",
	L"Wichtigste Gebäude", 				// main facilities in this town
	L"Level", 					// the training level of civilians in this town
	L"Zivilistentraining",				// 10 // state of civilian training in town
	L"Miliz", 					// the state of the trained civilians in the town
	
	// Flugente: prisoner texts
	L"Gefangene",
	L"%d (max. %d)",
	L"%d Hilfssoldaten",
	L"%d Truppen",
	L"%d Elite",
	L"%d Offiziere",
	L"%d Generele",
	L"%d Zivilisten",
	L"%d Special1",
	L"%d Special2",
};

// Mine strings
STR16 pwMineStrings[] =
{
	L"Mine",						// 0
	L"Silber",
	L"Gold",
	L"Tagesproduktion",
	L"Maximale Produktion",
	L"Aufgegeben",				// 5
	L"Geschlossen",
	L"Fast erschöpft",
	L"Produziert",
	L"Status",
	L"Produktionsrate",
	L"Rohstoff",				// 10	L"Erzart",
	L"Kontrolle über Stadt",
	L"Loyalität der Stadt",
};

// blank sector strings
STR16 pwMiscSectorStrings[] =
{
	L"Feindliche Verbände",
	L"Sektor",
	L"# der Gegenstände",
	L"Unbekannt",

	L"Kontrolliert",
	L"Ja",
	L"Nein",
	L"Status/Software Status:",

	L"Additional Intel", // TODO:Translate
};

// error strings for inventory
STR16 pMapInventoryErrorString[] =
{
	L"%s ist nicht nah genug.",	//Merc is in sector with item but not close enough
	L"Diesen Söldner können Sie nicht auswählen.", 
	L"%s ist nicht im Sektor.",
	L"Während einer Schlacht müssen Sie Gegenstände manuell nehmen.",
	L"Während einer Schlacht müssen Sie Gegenstände manuell fallenlassen.",
	L"%s ist nicht im Sektor und kann Gegenstand nicht fallen lassen.",
	L"Während des Kampfes können Sie die Munitionskiste nicht zum Nachladen verwenden.",
};

STR16 pMapInventoryStrings[] =
{
	L"Ort", 				// sector these items are in
	L"Zahl der Gegenstände", 		// total number of items in sector
};

// help text for the user
STR16 pMapScreenFastHelpTextList[] =
{
	L"Um die Aufgabe eines Söldners zu ändern und ihn einem anderen Trupp, einem Reparatur- oder Ärzteteam zuzuweisen, klicken Sie in die 'Aufträge'-Spalte.",
	L"Um einen Söldner an einen anderen Bestimmungsort zu versetzen, klicken Sie in die 'Aufträge'-Spalte.",
	L"Wenn ein Söldner seinen Marschbefehl erhalten hat, kann er sich mit dem Zeitraffer schneller bewegen.",
	L"Die linke Maustaste wählt den Sektor aus. Zweiter Klick auf die linke Maustaste erteilt Marschbefehl an Söldner. Mit der rechten Maustaste erhalten Sie Kurzinfos über den Sektor.",
	L"Hilfe aufrufen mit Taste 'h'.",
	L"Test-Text",
	L"Test-Text",
	L"Test-Text",
	L"Test-Text",
	L"In diesem Bildschirm können Sie nicht viel machen, bevor Sie in Arulco ankommen. Wenn Sie Ihr Team zusammengestellt haben, klicken Sie auf den Zeitraffer-Button unten rechts. Dadurch vergeht die Zeit schneller, bis Ihr Team in Arulco ankommt.",
};

// movement menu text
STR16 pMovementMenuStrings[] =
{
	L"Söldner in Sektor bewegen", 	// title for movement box 
	L"Route planen", 		// done with movement menu, start plotting movement
	L"Abbruch",			// cancel this menu
	L"Andere",			// title for group of mercs not on squads nor in vehicles
	L"Select all",		// Select all squads TODO: Translate
};

STR16 pUpdateMercStrings[] =
{
	L"Ups:", 			// an error has occured
	L"Vertrag ist abgelaufen:", 	// this pop up came up due to a merc contract ending
	L"Auftrag wurde ausgeführt:",	// this pop up....due to more than one merc finishing assignments
	L"Diese Söldner arbeiten wieder:", // this pop up ....due to more than one merc waking up and returing to work
	L"Diese Söldner schlafen:",	// this pop up ....due to more than one merc being tired and going to sleep
	L"Vertrag bald abgelaufen:", 	//this pop up came up due to a merc contract ending
};

// map screen map border buttons help text
STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Städte zeigen (|W)",
	L"|Minen zeigen",
	L"|Teams & Feinde zeigen",
	L"Luftr|aum zeigen",
	L"Gegenstände zeigen (|I)",
	L"Mili|z & Feinde zeigen",
	L"Krankheitsdaten zeigen (|D)",
	L"Wette|r zeigen",
	L"Aufträge & Intel zeigen (|Q)",
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Nächste (|.)",		// next page
	L"Vorherige (|,)",		// previous page
	L"Sektor Inventar schließen (|E|s|c)",	// exit sector inventory
	
	L"Inventar zoomen", // HEAROCK HAM 5: Inventory Zoom Button
	L"Gegenstände stapeln und verbinden", // HEADROCK HAM 5: Stack and Merge
	L"|L|i|n|k|e|r |K|l|i|c|k: Munition in Kisten sortieren\n|R|e|c|h|t|e|r |K|l|i|c|k: Munition in Boxen sortieren", // HEADROCK HAM 5: Sort ammo
	// 6 - 10
	L"|L|i|n|k|e|r |K|l|i|c|k: Alle Gegenstandsanbauten entfernen\n|R|e|c|h|t|e|r |K|l|i|c|k: empty LBE in sector", // HEADROCK HAM 5: Separate Attachments
	L"Munition aus allen Waffen entfernen", //HEADROCK HAM 5: Eject Ammo
	L"|L|i|n|k|e|r |K|l|i|c|k: Alle Gegenstände anzeigen\n|R|e|c|h|t|e|r |K|l|i|c|k: Alle Gegenstände ausblenden", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Waffen\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Waffen anzeigen", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Munition\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Munition anzeigen", // HEADROCK HAM 5: Filter Button
	// 11 - 15
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Sprengstoffe\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Sprengstoffe anzeigen", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Nahkampfwaffen\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Nahkampfwaffen anzeigen", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Körperpanzerungen\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Körperpanzerungen anzeigen", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von LBEs\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur LBEs anzeigen", // HEADROCK HAM 5: Filter Button
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von Ausrüstung\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur Ausrüstung anzeigen", // HEADROCK HAM 5: Filter Button
	// 16 - 20
	L"|L|i|n|k|e|r |K|l|i|c|k: Ein-/Ausblenden von anderen Gegenständen\n|R|e|c|h|t|e|r |K|l|i|c|k: Nur andere Gegenstände anzeigen", // HEADROCK HAM 5: Filter Button
	L"Ein-/Ausblenden von zu bewegenden Gegenständen", // Flugente: move item display
	L"Speichere Ausrüstungsvorlage",
	L"Lade Ausrüstungsvorlage...",
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"|Laptop",
	L"Taktik (|E|s|c)",
	L"|Optionen",
	L"Zeitraffer (|+)", 	// time compress more
	L"Zeitraffer (|-)", 	// time compress less
	L"Vorige Nachricht (|U|p)\nSeite zurück (|P|g|U|p)",		// previous message in scrollable list
	L"Nächste Nachricht (|D|o|w|n)\nNächste Seite (|P|g|D|n)", 	// next message in the scrollable list
	L"Zeit Start/Stop (|S|p|a|c|e)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Kontostand", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s ist tot.",
};

STR16 pDayStrings[] =
{
	L"Tag",
};

// the list of email sender names
CHAR16 pSenderNameList[500][128] =
{
	L"",
};
/*
{
	L"Enrico",
	L"Psych Pro Inc.",
	L"Online-Hilfe",
	L"Psych Pro Inc.",
	L"Speck",
	L"R.I.S.",
	L"Barry",
	L"Blood",
	L"Lynx",
	L"Grizzly",
	L"Vicki",
	L"Trevor",
	L"Grunty",
	L"Ivan",
	L"Steroid",
	L"Igor",
	L"Shadow",
	L"Red",
	L"Reaper",
	L"Fidel",
	L"Fox",
	L"Sidney",
	L"Gus",
	L"Buns",
	L"Ice",
	L"Spider",
	L"Cliff",
	L"Bull",
	L"Hitman",
	L"Buzz",
	L"Raider",
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
	L"H, A & S Versicherung",	
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
	L"Nächste",
};

// new mail notify string 
STR16 pNewMailStrings[] =
{
	L"Sie haben neue Mails...",
};

// confirm player's intent to delete messages
STR16 pDeleteMailStrings[] =
{
	L"Mail löschen?",
	L"UNGELESENE Mail löschen?",
};

// the sort header strings
STR16 pEmailHeaders[] =
{
	L"Absender:",
	L"Betreff:",
	L"Datum:",
};

// email titlebar text
STR16 pEmailTitleText[] =
{
	L"Mailbox",		
};

// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Buchhalter Plus",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Haben:", 				//the credits column (to ADD money to your account)
	L"Soll:", 				//the debits column (to SUBTRACT money from your account)
	L"Einkünfte vom Vortag:",
	L"Sonstige Einzahlungen vom Vortag:",
	L"Haben vom Vortag:",
	L"Kontostand Ende des Tages:",
	L"Tagessatz:",
	L"Sonstige Einzahlungen von heute:",
	L"Haben von heute:",
	L"Kontostand:",
	L"Voraussichtliche Einkünfte:",
	L"Prognostizierter Kontostand:", 		// projected balance for player for tommorow
};

// headers to each list in financial screen
STR16 pFinanceHeaders[] =
{
	L"Tag", 					// the day column
	L"Haben", 				//the credits column (to ADD money to your account)
	L"Soll", 					//the debits column (to SUBTRACT money from your account)
	L"Kontobewegungen", 			// transaction type - see TransactionText below
	L"Kontostand", 				// balance at this point in time
	L"Seite", 				// page number
	L"Tag(e)", 				// the day(s) of transactions this page displays 
};

STR16 pTransactionText[] =
{
	L"Aufgelaufene Zinsen",			// interest the player has accumulated so far
	L"Anonyme Einzahlung",
	L"Bearbeitungsgebühr", 
	L"Angeheuert", 				// Merc was hired
	L"Kauf bei Bobby Rays",	 		// Bobby Ray is the name of an arms dealer
	L"Ausgeglichene Konten bei M.E.R.C.",
	L"Krankenversicherung für %s", 		// medical deposit for merc 
	L"B.S.E.-Profilanalyse", 		// IMP is the acronym for International Mercenary Profiling
	L"Versicherung für %s abgeschlossen", 
	L"Versicherung für %s verringert",
	L"Versicherung für %s verlängert", 	// johnny contract extended
	L"Versicherung für %s gekündigt", 
	L"Versicherungsanspruch für %s", 	// insurance claim for merc
	L"1 Tag", 				// merc's contract extended for a day
	L"1 Woche", 				// merc's contract extended for a week
	L"2 Wochen", 				// ... for 2 weeks
	L"Minenertrag", 
	L"", 
	L"Blumen kaufen",
	L"Volle Rückzahlung für %s",
	L"Teilw. Rückzahlung für %s",
	L"Keine Rückzahlung für %s",
	L"Zahlung an %s",			// %s is the name of the npc being paid
	L"Überweisen an %s", 			// transfer funds to a merc
	L"Überweisen von %s", 			// transfer funds from a merc
	L"Miliz in %s ausbilden",		// initial cost to equip a town's militia
	L"Gegenstände von %s gekauft.",	//is used for the Shop keeper interface. The dealers name will be appended to the end of the string.
	L"%s hat Geld angelegt.",
	L"Gegenstände an Bevölkerung verkauft",
	L"Betriebskosten",			// HEADROCK HAM 3.6
	L"Unterhaltskosten für Miliz",		// HEADROCK HAM 3.6
	L"Lösegeld erpresst",	// Flugente: prisoner system
	L"WHO Daten abonnieren",	// Flugente: disease
	L"Zahlung an Kerberus",	// Flugente: PMC
	L"SAM reparieren", // Flugente: SAM repair
	L"Arbeiter trainiert", // Flugente: train workers
	L"Miliz in %s ausbilden",	// Flugente: drill militia
	L"Mini event", // rftr: mini events // TODO: translate
};

STR16 pTransactionAlternateText[] =
{
	L"Versicherung für", 				// insurance for a merc
	L"%ss Vertrag verl. um 1 Tag", 				// entend mercs contract by a day
	L"%ss Vertrag verl. um 1 Woche",
	L"%ss Vertrag verl. um 2 Wochen",
};

// helicopter pilot payment 
STR16 pSkyriderText[] =
{
	L"Skyrider wurden $%d gezahlt", 	// skyrider was paid an amount of money
	L"Skyrider bekommt noch $%d", 		// skyrider is still owed an amount of money
	L"Skyrider hat aufgetankt",		// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider ist bereit für weiteren Flug.",	// Skyrider was grounded but has been freed
	L"Skyrider hat keine Passagiere. Wenn Sie Söldner in den Sektor transportieren wollen, weisen Sie sie einem Fahrzeug/Helikopter zu.",
};

// strings for different levels of merc morale
STR16 pMoralStrings[] =
{
	L"Super", 
	L"Gut",
	L"Stabil",
	L"Schlecht",
	L"Panik",
	L"Mies",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.
STR16 pLeftEquipmentString[] =
{ 
	L"%ss Ausrüstung ist in Omerta angekommen (A9).", 
	L"%ss Ausrüstung ist in Drassen angekommen (B13).",
};

// Status that appears on the Map Screen
STR16 pMapScreenStatusStrings[] =
{
	L"Gesundheit",
	L"Energie",
	L"Moral",
	L"Zustand",	// the condition of the current vehicle (its "health")
	L"Tank",	// the fuel level of the current vehicle (its "energy")
	L"Gift",
	L"Wasser",		// drink level
	L"Essen",		// food level
};

STR16 pMapScreenPrevNextCharButtonHelpText[] =
{ 
	L"Voriger Söldner (|L|e|f|t)", 		// previous merc in the list
	L"Nächster Söldner (|R|i|g|h|t)", 	// next merc in the list
};

STR16 pEtaString[] =
{
	L"Ank.:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Sie werden das Ding nie wiedersehen. Trotzdem wegwerfen?", 	// do you want to continue and lose the item forever
	L"Dieser Gegenstand sieht SEHR wichtig aus. Sind Sie GANZ SICHER, dass Sie ihn wegwerfen wollen?", // does the user REALLY want to trash this item
};

STR16 pMapErrorString[] =
{
	L"Trupp kann nicht reisen, wenn einer schläft.",

//1-5
	L"Wir müssen erst an die Oberfläche.",
	L"Marschbefehl? Wir sind in einem feindlichen Sektor!",
	L"Wenn Söldner reisen sollen, müssen sie einem Trupp oder Fahrzeug zugewiesen werden.",
	L"Sie haben noch keine Teammitglieder.", 		// you have no members, can't do anything 
	L"Söldner kann Befehl nicht ausführen.",		// merc can't comply with your order
//6-10
	L"braucht eine Eskorte. Platzieren Sie ihn in einem Trupp mit Eskorte.", // merc can't move unescorted .. for a male
	L"braucht eine Eskorte. Platzieren Sie sie in einem Trupp mit Eskorte.", // for a female
	L"Söldner ist noch nicht in %s!",
	L"Erst mal Vertrag aushandeln!",
	L"Marschbefehl ist nicht möglich. Luftangriffe finden statt.",
//11-15
	L"Marschbefehl? Hier tobt ein Kampf!",
	L"Sie sind von Bloodcats umstellt in Sektor %s!",
	L"Sie haben gerade eine Bloodcat-Höhle betreten in Sektor %s!", 
	L"", 
	L"Raketenstützpunkt in %s wurde erobert.",
//16-20
	L"Mine in %s wurde erobert. Ihre Tageseinnahmen wurden reduziert auf %s.",
	L"Feind hat Sektor %s ohne Gegenwehr erobert.",
	L"Mindestens ein Söldner konnte nicht eingeteilt werden.",
	L"%s konnte sich nicht anschließen, weil %s voll ist", 
	L"%s konnte sich %s nicht anschließen, weil er zu weit weg ist.", 
//21-25
	L"Die Mine in %s ist von Deidrannas Truppen erobert worden!",
	L"Deidrannas Truppen sind gerade in den Raketenstützpunkt in %s eingedrungen",
	L"Deidrannas Truppen sind gerade in %s eingedrungen",
	L"Deidrannas Truppen wurden gerade in %s gesichtet.",
	L"Deidrannas Truppen haben gerade %s erobert.",
//26-30
	L"Mindestens ein Söldner kann nicht schlafen.",
	L"Mindestens ein Söldner ist noch nicht wach.",
	L"Die Miliz kommt erst, wenn das Training beendet ist.",
	L"%s kann im Moment keine Marschbefehle erhalten.",
	L"Milizen außerhalb der Stadtgrenzen können nicht in andere Sektoren reisen.",
//31-35
	L"Sie können keine Milizen in %s haben.",
	L"Leere Fahrzeuge fahren nicht!",
	L"%s ist nicht transportfähig!",
	L"Sie müssen erst das Museum verlassen!",
	L"%s ist tot!",
//36-40
	L"%s kann nicht zu %s wechseln, weil der sich bewegt", 
	L"%s kann so nicht einsteigen",
	L"%s kann sich %s nicht anschließen", 
	L"Sie können den Zeitraffer erst mit neuen Söldnern benutzen!",
	L"Dieses Fahrzeug kann nur auf Straßen fahren!",
//41-45
	L"Reisenden Söldnern können Sie keine Aufträge erteilen.",
	L"Kein Benzin mehr!",
	L"%s ist zu müde.",
	L"Keiner kann das Fahrzeug steuern.",
	L"Ein oder mehrere Söldner dieses Trupps können sich jetzt nicht bewegen.",
//46-50
	L"Ein oder mehrere Söldner des ANDEREN Trupps können sich gerade nicht bewegen.",
	L"Fahrzeug zu stark beschädigt!",
	L"Nur zwei Söldner pro Sektor können Milizen trainieren.",
	L"Roboter muss von jemandem bedient werden. Beide im selben Trupp platzieren.",
	L"Items cannot be moved to %s, as no valid dropoff point was found. Please enter map to resolve this issue.",	// TODO.Translate
// 51-55
	L"%d Gegenstände von %s nach %s bewegt",
};

// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Klicken Sie noch einmal auf das Ziel, um die Route zu bestätigen. Klicken Sie auf andere Sektoren, um die Route zu ändern.",
	L"Route bestätigt.",
	L"Ziel unverändert.",
	L"Route geändert.",
	L"Route verkürzt.",
};

// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Klicken Sie auf den Sektor, in dem die Söldner stattdessen ankommen sollen.",
	L"OK. Söldner werden in %s abgesetzt.",
	L"Söldner können nicht dorthin fliegen. Luftraum nicht gesichert!",
	L"Abbruch. Ankunftssektor unverändert,",
	L"Luftraum über %s ist nicht mehr sicher! Ankunftssektor jetzt in %s.",
};

// help text for mouse regions
STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Ins Inventar gehen (|E|n|t|e|r)",
	L"Gegenstand wegwerfen",
	L"Inventar verlassen (|E|n|t|e|r)",
};

// male version of where equipment is left
STR16 pMercHeLeaveString[] = 
{
	L"Soll %s seine Ausrüstung hier lassen (%s) oder in (%s), wenn er verlässt?",
	L"%s geht bald und lässt seine Ausrüstung in %s.",
};

// female version
STR16 pMercSheLeaveString[] =
{
	L"Soll %s ihre Ausrüstung hier lassen (%s) oder in (%s), bevor sie verlässt?",
	L"%s geht bald und lässt ihre Ausrüstung in %s.",
};

STR16 pMercContractOverStrings[] =
{
	L"s Vertrag war abgelaufen, und er ist nach Hause gegangen.", 		// merc's contract is over and has departed
	L"s Vertrag war abgelaufen, und sie ist nach Hause gegangen.", 		// merc's contract is over and has departed
	L"s Vertrag wurde gekündigt, und er ist weggegangen.", 			// merc's contract has been terminated
	L"s Vertrag wurde gekündigt, und sie ist weggegangen.",			// merc's contract has been terminated
	L"Sie schulden M.E.R.C. zu viel, also ist %s gegangen.", 		// Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages
STR16 pImpPopUpStrings[] =
{
	L"Ungültiger Code",
	L"Sie wollen gerade den ganzen Evaluierungsprozess von vorn beginnen. Sind Sie sicher?",
	L"Bitte Ihren vollen Namen und Ihr Geschlecht eingeben",
	L"Die Überprüfung Ihrer finanziellen Mittel hat ergeben, dass Sie sich keine Evaluierung leisten können.",
	L"Option zur Zeit nicht gültig.",
	L"Um eine genaue Evaluierung durchzuführen, müssen Sie mindestens noch ein Teammitglied aufnehmen können.",
	L"Evaluierung bereits durchgeführt.",
	L"Fehler beim Laden des B.S.E.-Charakters.",
	L"Sie haben bereits die maximale Anzahl an B.S.E.-Charakteren.",
	L"Sie haben bereits drei B.S.E.-Charaktere mit dem gleichen Geschlecht.",
	L"Sie können sich den B.S.E.-Charakter nicht leisten.",	// 10
	L"Der neue B.S.E.-Charakter ist nun in ihrem Team.",
	L"You have already selected the maximum number of traits.",	// TODO.Translate
	L"No voicesets found.",	// TODO.Translate
};

// button labels used on the IMP site
STR16 pImpButtonText[] =
{
	L"Wir über uns", 		// about the IMP site
	L"BEGINNEN", 			// begin profiling
	L"Persönlichkeiten", 		// personality section
	L"Eigenschaften", 		// personal stats/attributes section
	L"Aussehen", 			// changed from portrait
	L"Stimme %d", 			// the voice selection
	L"Fertig", 			// done profiling
	L"Von vorne anfangen",	 	// start over profiling
	L"Ja, die Antwort passt!", 
	L"Ja", 
	L"Nein",
	L"Fertig", 			// finished answering questions
	L"Zurück", 			// previous question..abbreviated form
	L"Weiter", 			// next question
	L"JA",		 		// yes, I am certain 
	L"NEIN, ICH MÖCHTE VON VORNE ANFANGEN.", // no, I want to start over the profiling process
	L"JA",
	L"NEIN",
	L"Zurück", 			// back one page
	L"Abbruch", 			// cancel selection
	L"Ja",
	L"Nein, ich möchte es mir nochmal ansehen.",
	L"Registrieren", 			// the IMP site registry..when name and gender is selected
	L"Analyse wird durchgeführt",	// analyzing your profile results
	L"OK",
	L"Charakter", // Change from "Voice"
	L"Keine",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed
	L"Nach Festlegung Ihres Charakters können Sie Ihre Fertigkeit(en) auswählen.",
	L"Um die Evaluierung erfolgreich abzuschließen, bestimmen Sie Ihre Eigenschaften.",
	L"Um Ihr Profil zu erstellen, wählen Sie ein Portrait und eine Stimme aus und definieren Ihre äußere Erscheinung.",
	L"Jetzt, da Sie Ihr Aussehen bestimmt haben, fahren wir mit der Charakter-Analyse fort.",
};

STR16 pFilesTitle[] =
{
	L"Akten einsehen",
};

STR16 pFilesSenderList[] =
{
	L"Aufklärungsbericht", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
	L"Abschnitt #1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"Abschnitt #2",		// second intercept file
	L"Abschnitt #3",		// third intercept file
	L"Abschnitt #4",		// fourth intercept file
	L"Abschnitt #5",		// fifth intercept file
	L"Abschnitt #6",		// sixth intercept file
};

// Text having to do with the History Log
STR16 pHistoryTitle[] =
{
	L"Logbuch",
};

STR16 pHistoryHeaders[] =
{
	L"Tag",	 			// the day the history event occurred
	L"Seite", 			// the current page in the history report we are in
	L"Tag", 			// the days the history report occurs over
	L"Ort",	 			// location (in sector) the event occurred
	L"Ereignis", 			// the event label
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
	L"%s von A.I.M. angeheuert.",				// merc was hired from the aim site
	L"%s von M.E.R.C. angeheuert.", 			// merc was hired from the aim site
	L"%s ist tot.", 					// merc was killed
	L"Rechnung an M.E.R.C. bezahlt",			// paid outstanding bills at MERC
	L"Enrico Chivaldoris Auftrag akzeptiert",	
	//6-10
	L"B.S.E.-Profil erstellt",
	L"Versicherung abgeschlossen für %s.", 			// insurance contract purchased
	L"Versicherung gekündigt für %s.", 			// insurance contract canceled
	L"Versicherung ausgezahlt für %s.", 			// insurance claim payout for merc
	L"%ss Vertrag um 1 Tag verlängert.",			// Extented "mercs name"'s for a day
	//11-15
	L"%ss Vertrag um 1 Woche verlängert.", 			// Extented "mercs name"'s for a week
	L"%ss Vertrag um 2 Wochen verlängert.", 		// Extented "mercs name"'s 2 weeks
	L"%s entlassen.", 					// "merc's name" was dismissed.
	L"%s geht.", 						// "merc's name" quit.
	L"Quest begonnen.", 					// a particular quest started
	//16-20
	L"Quest gelöst.",
	L"Mit Vorarbeiter in %s geredet",			// talked to head miner of town
	L"%s befreit",
	L"Cheat benutzt",
	L"Essen ist morgen in Omerta",
	//21-25
	L"%s heiratet Daryl Hick",
	L"%ss Vertrag abgelaufen.",
	L"%s rekrutiert.",
	L"Enrico sieht kaum Fortschritte",
	L"Schlacht gewonnen",
	//26-30
	L"Mine in %s produziert weniger",
	L"Mine in %s leer",
	L"Mine in %s geschlossen",
	L"Mine in %s wieder offen",
	L"Etwas über Gefängnis in Tixa erfahren.",
	//31-35
	L"Von Waffenfabrik in Orta gehört.",
	L"Forscher in Orta gab uns viele Raketengewehre.", 
	L"Deidranna verfüttert Leichen.",
	L"Frank erzählte von Kämpfen in San Mona.",
	L"Patient denkt, er hat in den Minen etwas gesehen.",
	//36-40
	L"Devin getroffen - verkauft Sprengstoff",
	L"Berühmten Ex-AIM-Mann Mike getroffen!",
	L"Tony getroffen - verkauft Waffen.",
	L"Sergeant Krott gab mir Raketengewehr.",
	L"Kyle die Urkunde für Angels Laden gegeben.",
	//41-45
	L"Madlab will Roboter bauen.",
	L"Gabby kann Tinktur gegen Käfer machen.",
	L"Keith nicht mehr im Geschäft.",
	L"Howard lieferte Gift an Deidranna.",
	L"Keith getroffen - verkauft alles in Cambria.",
	//46-50
	L"Howard getroffen - Apotheker in Balime",
	L"Perko getroffen - hat kleinen Reparaturladen.",
	L"Sam aus Balime getroffen - hat Computerladen.",
	L"Franz hat Elektronik und andere Sachen.",
	L"Arnold repariert Sachen in Grumm.",
	//51-55
	L"Fredo repariert Elektronik in Grumm.",
	L"Spende von Reichem aus Balime bekommen.",
	L"Schrotthändler Jake getroffen.",
	L"Ein Depp hat uns eine Codekarte gegeben.",
	L"Walter bestochen, damit er Keller öffnet.",
	//56-60
	L"Wenn Dave Sprit hat, bekommen wir's gratis.",
	L"Pablo bestochen.",
	L"Kingpin hat Geld in San Mona-Mine.",
	L"%s gewinnt Extremkampf",
	L"%s verliert Extremkampf",
	//61-65
	L"%s beim Extremkampf disqualifiziert",
	L"Viel Geld in verlassener Mine gefunden.",
	L"Von Kingpin geschickten Mörder getroffen", 
	L"Kontrolle über Sektor verloren",				
	L"Sektor verteidigt",		
	//66-70
	L"Schlacht verloren",						//ENEMY_ENCOUNTER_CODE
	L"Tödlicher Hinterhalt",					//ENEMY_AMBUSH_CODE
	L"Hinterhalt ausgehoben",
	L"Angriff fehlgeschlagen",					//ENTERING_ENEMY_SECTOR_CODE
	L"Angriff erfolgreich",
	//71-75
	L"Monster angegriffen",						//CREATURE_ATTACK_CODE
	L"Von Bloodcats getötet",					//BLOODCAT_AMBUSH_CODE
	L"Bloodcats getötet",
	L"%s wurde getötet",	
	L"Carmen den Kopf eines Terroristen gegeben",
	//76-80
	L"Slay ist gegangen",						//Slay is a merc and has left the team
	L"%s getötet",							//History log for when a merc kills an NPC or PC
	L"Met Waldo - aircraft mechanic.",
	L"Helicopter repairs started. Estimated time: %d hour(s).",
};
*/

STR16 pHistoryLocations[] =
{
	L"n.a.",							// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop
STR16 pLaptopIcons[] =
{
	L"E-mail",
	L"Web",
	L"Finanzen",
	L"Personal",
	L"Logbuch",	
	L"Dateien",
	L"Schließen",
	L"sir-FER 4.0",		// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added
STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Rays",
	L"B.S.E.",
	L"M.E.R.C.",
	L"Bestatter",
	L"Florist",
	L"Versicherung",
	L"Abbruch",
	L"Enzyklopädie",
	L"Besprechung",
	L"Geschichte",
	L"MeLoDY",
	L"WHO",
	L"Kerberus",
	L"Militia Overview",	// TODO.Translate
	L"R.I.S.",
	L"Factories",	// TODO.Translate
};

STR16 pBookmarkTitle[] =
{
	 L"Lesezeichen",
	 L"Rechts klicken, um in Zukunft in dieses Menü zu gelangen.",
};

// When loading or download a web page
STR16 pDownloadString[] = {
	L"Download läuft",
	L"Neuladen läuft",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine
STR16 gsAtmSideButtonText[] = 
{
	L"OK",
	L"Nehmen", 		// take money from merc
	L"Geben", 		// give money to merc
	L"Rückgängig", 		// cancel transaction
	L"Löschen", 		// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] = 
{
	L"Überw $", 		// transfer money to merc -- short form
	L"Statistik", 		// view stats of the merc
	L"Inventar", 		// view the inventory of the merc
	L"Anstellung",
};

STR16 sATMText[] = 
{
	L"Geld überw.?", 	// transfer funds to merc?
	L"Ok?", 		// are we certain?
	L"Betrag eingeben", 	// enter the amount you want to transfer to merc
	L"Art auswählen", 	// select the type of transfer to merc
	L"Nicht genug Geld", 	// not enough money to transfer to merc
	L"Betrag muss durch $10 teilbar sein", // transfer amount must be a multiple of $10
};

// Web error messages. Please use German equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator
STR16 pErrorStrings[] = 
{
	L"Fehler",
	L"Server hat keinen DNS-Eintrag.",	
	L"URL-Adresse überprüfen und nochmal versuchen.",
	L"OK",
	L"Verbindung zum Host wird dauernd unterbrochen. Mit längeren Übertragungszeiten ist zu rechnen.",
};

STR16 pPersonnelString[] = 
{
	L"Söldner:", 		// mercs we have
};

STR16 pWebTitle[ ] = 
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};

// The titles for the web program title bar, for each page loaded
STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. Mitglieder",
	L"A.I.M. Bilder",		// a mug shot is another name for a portrait
	L"A.I.M. Sortierfunktion",
	L"A.I.M.",
	L"A.I.M. Veteranen",
	L"A.I.M. Politik",
	L"A.I.M. Geschichte",
	L"A.I.M. Links",
	L"M.E.R.C.",
	L"M.E.R.C. Konten",
	L"M.E.R.C. Registrierung",
	L"M.E.R.C. Index",
	L"Bobby Rays",
	L"Bobby Rays - Waffen",
	L"Bobby Rays - Munition",
	L"Bobby Rays - Rüstungen",
	L"Bobby Rays - Sonstige",	//misc is an abbreviation for miscellaneous				
	L"Bobby Rays - Gebraucht",
	L"Bobby Rays - Versandauftrag",
	L"B.S.E.",
	L"B.S.E.",
	L"Fleuropa",
	L"Fleuropa - Gestecke",
	L"Fleuropa - Bestellformular",
	L"Fleuropa - Karten",
	L"Hammer, Amboss & Steigbügel Versicherungsmakler",
	L"Information",
	L"Vertrag",
	L"Bemerkungen",
	L"McGillicuttys Bestattungen",
	L"",
	L"URL nicht gefunden.",
	L"%s Presse Rat - Konflikt-Zusammenfassungen",
	L"%s Presse Rat - Kampfberichte",
	L"%s Presse Rat - Aktuellste Neuigkeiten",
	L"%s Presse Rat - Über uns",
	L"Mercs Love or Dislike You - About us",				// TODO.Translate
	L"Mercs Love or Dislike You - Analyze a team",
	L"Mercs Love or Dislike You - Pairwise comparison",
	L"Mercs Love or Dislike You - Personality",
	L"WHO - About WHO",
	L"WHO - Disease in Arulco",
	L"WHO - Helpful Tips",
	L"Kerberus - About Us",
	L"Kerberus - Hire a Team",
	L"Kerberus - Individual Contracts",
	L"Miliz - Übersicht",
	L"Recon Intelligence Services - Information Requests",		// TODO.Translate
	L"Recon Intelligence Services - Information Verification",
	L"Recon Intelligence Services - About us",
	L"Fabriken - Übersicht",
	L"Bobby Rays - Letzte Lieferungen",
	L"Enzyklopädie",
	L"Enzyklopädie - Daten",
	L"Einsatzbesprechung",
	L"Einsatzbesprechung - Daten",
};

STR16 pShowBookmarkString[] = 
{
	L"Sir-Help",
	L"Erneut auf Web klicken für Lesezeichen.",
};

STR16 pLaptopTitles[] = 
{
	L"E-Mail",
	L"Dateien",
	L"Söldner-Manager",
	L"Buchhalter Plus",
	L"Logbuch",
};

STR16 pPersonnelDepartedStateStrings[] =
{ //(careful not to exceed 18 characters total including spaces)
	//reasons why a merc has left.
	L"Getötet",
	L"Entlassen",
	L"Sonstiges",
	L"Heirat",
	L"Vertrag zu Ende",
	L"Aufgehört",			//LOOTF - Englisch "quit", welcher Kontext? = Slay Ruttwen?
};

// personnel strings appearing in the Personnel Manager on the laptop
STR16 pPersonelTeamStrings[] = 
{
	L"Aktuelles Team",
	L"Ausgeschieden",
	L"Tgl. Kosten:",
	L"Höchste Kosten:",
	L"Niedrigste Kosten:",
	L"Im Kampf getötet:",
	L"Entlassen:",
	L"Sonstiges:",
};

STR16 pPersonnelCurrentTeamStatsStrings[] = 
{
	L"Schlechteste",
	L"Durchsch.",	
	L"Beste",
};

STR16 pPersonnelTeamStatsStrings[] = 
{
	L"GSND",
	L"BEW",
	L"GES",
	L"KRF",
	L"FHR",
	L"WSH",
	L"ERF",
	L"TRF",
	L"TEC",
	L"SPR",
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
	L"Vertrag",
};

// text that appears on the update panel buttons
STR16 pUpdatePanelButtons[] = 
{
	L"Weiter",
	L"Stop",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle
CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Sie sind in diesem Sektor geschlagen worden!",
	L"Der Feind hat kein Erbarmen mit den Seelen Ihrer Teammitglieder und verschlingt jeden einzelnen.",	//LOOTF - Auch im Englischen Kannibalismus. Was zum Henker?
	L"Ihre bewusstlosen Teammitglieder wurden gefangen genommen!",
	L"Ihre Teammitglieder wurden vom Feind gefangen genommen.",
};

//Insurance Contract.c
//The text on the buttons at the bottom of the screen.
STR16 InsContractText[] = 
{ 
	L"Zurück",
	L"Vor",
	L"OK",
	L"Löschen",
};

//Insurance Info
// Text on the buttons on the bottom of the screen
STR16 InsInfoText[] = 
{ 
	L"Zurück", 
	L"Vor",
};

//For use at the M.E.R.C. web site. Text relating to the player's account with MERC
STR16 MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Befugnis ert.",
	L"Startseite",
	L"Konto #:",
	L"Söldner",
	L"Tage",
	L"Tagessatz",		//5			//LOOTF - "Rate" geändert auf "Tagessatz", ändern wenn Probleme, alt. "Sold"
	L"Belasten",
	L"Gesamt:",
	L"Zahlung von %s wirklich genehmigen?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
	L"%s (+gear)",	// TODO.Translate
};

// Merc Account Page buttons
STR16 MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Zurück",
	L"Weiter",
};

//For use at the M.E.R.C. web site. Text relating a MERC mercenary
STR16 MercInfo[] =
{
	L"Gesundheit",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Kraft",
	L"Führungsqualität",
	L"Weisheit",
	L"Erfahrungsstufe",
	L"Treffsicherheit",
	L"Technik",
	L"Sprengstoffe",
	L"Medizin",

	L"Zurück",
	L"Anheuern",
	L"Weiter",
	L"Zusatzinfo",
	L"Startseite",
	L"Angestellt",	
	L"Sold:",
	L"Pro Tag",
	L"Ausr.:",
	L"Gesamt:",
	L"Verstorben",	

	L"Sie haben bereits ein vollständiges Team.",
	L"Ausrüstung kaufen?",
	L"nicht da",
	L"Offene Beträge",
	L"Bio",
	L"Inv",								
};

// For use at the M.E.R.C. web site. Text relating to opening an account with MERC
STR16 MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Konto eröffnen",
	L"Rückgängig",
	L"Sie haben kein Konto. Möchten Sie eins eröffnen?",
};

// For use at the M.E.R.C. web site. MERC Homepage
STR16 MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, Gründer und Besitzer",
	L"Hier klicken, um ein Konto zu eröffnen",
	L"Hier klicken, um das Konto einzusehen",
	L"Hier klicken, um Dateien einzusehen.",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer fehlgeschlagen. Kein Geld vorhanden.",
};

// For use at MiGillicutty's Web Page.
STR16 sFuneralString[] =
{
	L"McGillicuttys Bestattungen: Wir trösten trauernde Familien seit 1983.",
	L"Der Bestattungsunternehmer und frühere A.I.M.-Söldner Murray \"Pops\" McGillicutty ist ein ebenso versierter wie erfahrener Bestatter.",
	L"Pops hat sein ganzes Leben mit Todes- und Trauerfällen verbracht. Deshalb weiß er aus erster Hand, wie schwierig das sein kann.",
	L"Das Bestattungsunternehmen McGillicutty bietet Ihnen einen umfassenden Service, angefangen bei der Schulter zum Ausweinen bis hin zur kosmetischen Aufbereitung entstellter Körperteile.",
	L"McGillicuttys Bestattungen - und Ihre Lieben ruhen in Frieden.",

	// Text for the various links available at the bottom of the page
	L"BLUMEN",
	L"SÄRGE UND URNEN",
	L"FEUERBEST.",	
	L"GRÄBER", 
	L"PIETÄT",	

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Leider ist diese Site aufgrund eines Todesfalles in der Familie noch nicht fertiggestellt. Sobald das Testament eröffnet worden und die Verteilung des Erbes geklärt ist, wird diese Site fertiggestellt.",
	L"Unser Mitgefühl gilt trotzdem all jenen, die es diesmal versucht haben. Bis später.",
};

// Text for the florist Home page
STR16 sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Galerie",

	//Address of United Florist

	L"\"Wir werfen überall per Fallschirm ab\"",
	L"1-555-SCHNUPPER-MAL",
	L"333 Duftmarke Dr, Aroma City, CA USA 90210",
	L"http://www.schnupper-mal.com",

	// detail of the florist page

	L"Wir arbeiten schnell und effizient",
	L"Lieferung am darauf folgenden Tag, in fast jedes Land der Welt. Ausnahmen sind möglich. ",
	L"Wir haben die garantiert niedrigsten Preise weltweit!",
	L"Wenn Sie anderswo einen niedrigeren Preis für irgend ein Arrangement sehen, bekommen Sie von uns ein Dutzend Rosen umsonst!",
	L"Fliegende Flora, Fauna & Blumen seit 1981.",
	L"Unsere hochdekorierten Ex-Bomber-Piloten werfen das Bouquet in einem Radius von zehn Meilen rund um den Bestimmungsort ab. Jederzeit!",
	L"Mit uns werden Ihre blumigsten Fantasien wahr",
	L"Bruce, unser weltberühmter Designer-Florist, verwendet nur die frischesten handverlesenen Blumen aus unserem eigenen Gewächshaus.",
	L"Und denken Sie daran: Was wir nicht haben, pflanzen wir für Sie - und zwar schnell!",
};

//Florist OrderForm
STR16 sOrderFormText[] = 
{

	//Text on the buttons

	L"Zurück",
	L"Senden",
	L"Löschen",
	L"Galerie",

	L"Name des Gestecks:",
	L"Preis:",			//5
	L"Bestellnr.:",
	L"Liefertermin",
	L"Morgen",
	L"Egal",
	L"Bestimmungsort",			//10
	L"Extraservice",
	L"Kaputtes Gesteck($10)",
	L"Schwarze Rosen($20)",
	L"Welkes Gesteck($10)",
	L"Früchtekuchen (falls vorrätig)($10)",		//15
	L"Persönliche Worte:",
	L"Aufgrund der Kartengröße darf Ihre Botschaft nicht länger sein als 75 Zeichen.",
	L"...oder wählen Sie eine unserer",

	L"STANDARD-KARTEN",	
	L"Rechnung für",//20

	//The text that goes beside the area where the user can enter their name

	L"Name:",
};

//Florist Gallery.c
STR16 sFloristGalleryText[] =
{
	//text on the buttons
	L"Zurück",	//abbreviation for previous
	L"Weiter",	//abbreviation for next
	L"Klicken Sie auf das Gesteck Ihrer Wahl",
	L"Bitte beachten Sie, dass wir für jedes kaputte oder verwelkte Gesteck einen Aufpreis von $10 berechnen.",
	L"Home",
};

STR16 sFloristCards[] = 
{
	L"Klicken Sie auf das Gesteck Ihrer Wahl",
	L"Zurück",
};

// Text for Bobby Ray's Mail Order Site
STR16 BobbyROrderFormText[] = 
{
	L"Bestellformular",			//Title of the page
	L"St.",					// The number of items ordered
	L"Gew. (%s)",				// The weight of the item
	L"Artikel",				// The name of the item
	L"Preis",				// the item's weight
	L"Summe",			//5	// The total price of all of items of the same type
	L"Zwischensumme",			// The sub total of all the item totals added
	L"Versandkosten (vgl. Bestimmungsort)",	// S&H is an acronym for Shipping and Handling 
	L"Endbetrag",				// The grand total of all item totals + the shipping and handling
	L"Bestimmungsort",			
	L"Liefergeschwindigkeit",	//10	// See below
	L"$ (pro %s)",				// The cost to ship the items
	L"Übernacht-Express",			// Gets deliverd the next day
	L"2 Arbeitstage",			// Gets delivered in 2 days
	L"Standard-Service",			// Gets delivered in 3 days
	L"Löschen",			//15	// Clears the order page
	L"Bestellen",				// Accept the order
	L"Zurück",				// text on the button that returns to the previous page
	L"Home",				// Text on the button that returns to the home page
	L"* Gebrauchter Gegenstand",		// Disclaimer stating that the item is used
	L"Sie haben nicht genug Geld.",	//20	// A popup message that to warn of not enough money
	L"<KEINER>",				// Gets displayed when there is no valid city selected
	L"Wollen Sie Ihre Bestellung wirklich nach %s schicken?",		// A popup that asks if the city selected is the correct one
	L"Packungsgewicht**",			// Displays the weight of the package
	L"** Mindestgewicht",			// Disclaimer states that there is a minimum weight for the package
	L"Lieferungen",	
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Pistolen",
	L"MPs",
	L"SMGs",
	L"Gewehre",
	L"SSGs",
	L"Sturmgew.",
	L"MGs",
	L"Schrotfl.",
	L"Schwere W.",

	// Ammo
	L"Pistole",
	L"M.-Pistole",
	L"SMG",
	L"Gewehr",
	L"SS-Gewehr",
	L"Sturmgew.",
	L"MG",
	L"Schrotfl.",

	// Used
	L"Feuerwfn.",
	L"Rüstungen",
	L"Trageausr.",
	L"Sonstiges",

	// Armour
	L"Helme",
	L"Westen",
	L"Hosen",
	L"Platten",

	// Misc
	L"Klingen",
	L"Wurfmesser",
	L"Schlagwaf.",
	L"Granaten",
	L"Bomben",
	L"Verbandsk.",
	L"Taschen",
	L"Kopfausr.",
	L"Trageausr.",
	L"Optik", // Madd: new BR filters
	L"Gri/Las",
	L"Mündung",
	L"Schaft",
	L"Mag/Abz.",
	L"Andere An.",
	L"Sonstiges",
};

// This text is used when on the various Bobby Ray Web site pages that sell items
STR16 BobbyRText[] = 
{
	L"Bestellen",				// Title
	L"Klicken Sie auf den gewünschten Gegenstand. Weiteres Klicken erhöht die Stückzahl. Rechte Maustaste verringert Stückzahl. Wenn Sie fertig sind, weiter mit dem Bestellformular.",			// instructions on how to order

	//Text on the buttons to go the various links

	L"Zurück",		// 
	L"Feuerwfn.", 			//3
	L"Munition",			//4
	L"Rüstung",			//5
	L"Sonstiges",			//6	//misc is an abbreviation for miscellaneous
	L"Gebraucht",			//7
	L"Vor",
	L"BESTELLEN",
	L"Startseite",			//10

	//The following 2 lines are used on the Ammunition page. 
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Ihr Team hat",				//11
	L"Waffe(n), die diese Munition verschießen",	//12

	//The following lines provide information on the items

	L"Gewicht:",			// Weight of all the items of the same type
	L"Kal:",			// the caliber of the gun
	L"Mag:",			// number of rounds of ammo the Magazine can hold
	L"Reichw.:",			// The range of the gun
	L"Schaden:",			// Damage of the weapon	
	L"Kadenz:",			// Weapon's Rate Of Fire, acroymn ROF
	L"AP:",				// Weapon's Action Points, acronym AP
	L"Bet.:",			// Weapon's Stun Damage
	L"Rüstung:",		// Armour's Protection
	L"Tarn.:",			// Armour's Camouflage
	L"Armor Pen:",		// Ammo's Armour Piercing modifier (see AmmoTypes.xml - armourImpactReduction)
	L"Dmg Mod:",		// Ammo's Bullet Tumble modifier (see AmmoTypes.xml - afterArmourDamage)
	L"Projectiles:",	// Ammo's bullet count (for buckshot) (see AmmoTypes.xml - numberOfBullets)
	L"Preis:",			// Cost of the item
	L"Vorrätig:",			// The number of items still in the store's inventory
	L"Bestellt:",			// The number of items on order
	L"Beschädigt",			// If the item is damaged
	L"Gew.:",			// the Weight of the item
	L"Summe:",			// The total cost of all items on order
	L"* %% funktionstüchtig",	// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	L"Mist! Mit diesem Formular können Sie nur " ,//First part
	L" Sachen bestellen. Wenn Sie mehr wollen (was wir sehr hoffen), füllen Sie bitte noch ein Formular aus.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Sorry. Davon haben wir leider im Moment nichts mehr auf Lager. Versuchen Sie es später noch einmal.",

	//A popup that tells the user that the store is temporarily sold out

	L"Es tut uns sehr leid, aber im Moment sind diese Sachen total ausverkauft.",

};

// Text for Bobby Ray's Home Page
STR16 BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Dies ist die heißeste Seite für Waffen und militärische Ausrüstung aller Art",
	L"Welchen Sprengstoff Sie auch immer brauchen - wir haben ihn.",
	L"SECOND HAND",

	//Text for the various links to the sub pages

	L"SONSTIGES",
	L"FEUERWAFFEN",
	L"MUNITION",		//5
	L"RÜSTUNG",

	//Details on the web site

	L"Was wir nicht haben, das hat auch kein anderer",
	L"In Arbeit",
};

// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page
STR16 AimSortText[] =
{
	L"A.I.M. Mitglieder",				// Title
	L"Sortieren:",					// Title for the way to sort

	// sort by...

	L"Preis",
	L"Erfahrung",
	L"Treffsicherheit",
	L"Technik",
	L"Sprengstoff",
	L"Medizin",
	L"Gesundheit",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Kraft",
	L"Führungsqualität",
	L"Weisheit",
	L"Name",

	//Text of the links to other AIM pages

	L"Den Söldner-Kurzindex ansehen",
	L"Personalakte der Söldner ansehen",
	L"Die AIM-Veteranengalerie ansehen",

	// text to display how the entries will be sorted

	L"Aufsteigend",
	L"Absteigend",
};

//Aim Policies.c
//The page in which the AIM policies and regulations are displayed
STR16 AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Zurück",
	L"AIM HomePage", 
	L"Regel-Index",
	L"Nächste Seite",
	L"Ablehnen",	
	L"Zustimmen",
};

//Aim Member.c
//The page in which the players hires AIM mercenaries
// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index
STR16 AimMemberText[] =
{
	L"Linksklick",
	L"zum Kontaktieren.",
	L"Rechtsklick",
	L"zum Foto-Index.",
//	L"Linksklick zum Kontaktieren. \nRechtsklick zum Foto-Index.",
};					

//Aim Member.c
//The page in which the players hires AIM mercenaries
STR16 CharacterInfo[] =
{
	// The various attributes of the merc

	L"Gesundheit",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Kraft",
	L"Führungsqualität",
	L"Weisheit",
	L"Erfahrungsstufe",
	L"Treffsicherheit",
	L"Technik",
	L"Sprengstoff",
	L"Medizin",				//10

	
	// the contract expenses' area

	L"Preis",							
	L"Vertrag",				
	L"1 Tag",					
	L"1 Woche",
	L"2 Wochen",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Zurück",
	L"Kontakt",
	L"Weiter",
	L"Zusatzinfo",				// Title for the additional info for the merc's bio
	L"Aktive Mitglieder",		//20	// Title of the page
	L"Zusätzl. Ausrüst:",			// Displays the optional gear cost
	L"Ausr.",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"VERSICHERUNG erforderlich",			// If the merc required a medical deposit, this is displayed
	L"Kit 1",				// Text on Starting Gear Selection Button 1
	L"Kit 2",				// Text on Starting Gear Selection Button 2
	L"Kit 3",				// Text on Starting Gear Selection Button 3
	L"Kit 4",				// Text on Starting Gear Selection Button 4
	L"Kit 5",				// Text on Starting Gear Selection Button 5
};

//Aim Member.c
//The page in which the player's hires AIM mercenaries
//The following text is used with the video conference popup
STR16 VideoConfercingText[] =
{
	L"Vertragskosten:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"1 Tag",										
	L"1 Woche",
	L"2 Wochen",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Keine Ausrüstung",
	L"Ausrüstung kaufen",

	// Text on the Buttons

	L"GELD ÜBERWEISEN",			// to actually hire the merc
	L"ABBRECHEN",				// go back to the previous menu
	L"ANHEUERN",				// go to menu in which you can hire the merc
	L"AUFLEGEN",				// stops talking with the merc
	L"OK",									
	L"NACHRICHT",				// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Videokonferenz mit",		
	L"Verbinde. . .",

	L"versichert",				// Displays if you are hiring the merc with the medical deposit
												
};

//Aim Member.c
//The page in which the player hires AIM mercenaries
// The text that pops up when you select the TRANSFER FUNDS button
STR16 AimPopUpText[] =
{
	L"ELEKTRONISCHE ÜBERWEISUNG AUSGEFÜHRT",	// You hired the merc
	L"ÜBERWEISUNG KANN NICHT BEARBEITET WERDEN",		// Player doesn't have enough money, message 1
	L"NICHT GENUG GELD",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"Im Einsatz",											
	L"Bitte Nachricht hinterlassen",
	L"Verstorben",

	//If you try to hire more mercs than game can support

	L"Sie haben bereits ein vollständiges Team.",

	L"Mailbox",
	L"Nachricht aufgenommen",
};

//AIM Link.c
STR16 AimLinkText[] =
{
	L"A.I.M. Links",	//The title of the AIM links page
};

//Aim History
// This page displays the history of AIM
STR16 AimHistoryText[] =
{
	L"Die Geschichte von A.I.M.",					//Title

	// Text on the buttons at the bottom of the page

	L"Zurück",
	L"Startseite", 
	L"Veteranen",
	L"Weiter",
};

//Aim Mug Shot Index
//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.
STR16 AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Preis",
	L"Erfahrung",
	L"Treffsicherheit",
	L"Technik",
	L"Sprengstoff",
	L"Medizin",
	L"Gesundheit",
	L"Beweglichkeit",
	L"Geschicklichkeit",
	L"Kraft",
	L"Führungsqualität",
	L"Weisheit",
	L"Name",

	// The title of the page, the above text gets added at the end of this text
	L"A.I.M.-Mitglieder ansteigend sortiert nach %s",
	L"A.I.M.-Mitglieder absteigend sortiert nach %s",

	// Instructions to the players on what to do

	L"Linke Maustaste",
	L"um Söldner auszuwählen",			//10
	L"Rechte Maustaste",
	L"um Optionen einzustellen",

	// Gets displayed on top of the merc's portrait if they are...

	//Please be careful not to increase the size of strings for following three
	L"Abwesend",
	L"Verstorben",						//14
	L"Im Dienst",
};

//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM
STR16 AimAlumniText[] =
{
	// Text of the buttons
	L"SEITE 1",
	L"SEITE 2",
	L"SEITE 3",
	L"A.I.M.-Veteranen",	// Title of the page
	L"ENDE",		// Stops displaying information on selected merc
	L"Nächste",	
};

//AIM Home Page
STR16 AimScreenText[] =
{
	// AIM disclaimers

	L"A.I.M. und das A.I.M.-Logo sind in den meisten Ländern eingetragene Warenzeichen.",
	L"Also denken Sie nicht mal daran, uns nachzumachen.",
	L"Copyright 1998-1999 A.I.M., Ltd. Alle Rechte vorbehalten.", 

	//Text for an advertisement that gets displayed on the AIM page

	L"Fleuropa",
	L"\"Wir werfen überall per Fallschirm ab\"",				//10
	L"Treffen Sie gleich zu Anfang",
	L"... die richtige Wahl.",
	L"Was wir nicht haben, das brauchen Sie auch nicht.",
};

//Aim Home Page
STR16 AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages

	L"Home",
	L"Mitglieder",
	L"Veteranen",
	L"Regeln",
	L"Geschichte",
	L"Links",
};

//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.
STR16 SKI_Text[] = 
{
	L"WAREN VORRÄTIG",	//Header for the merchandise available
	L"SEITE",		//The current store inventory page being displayed
	L"KOSTEN",		//The total cost of the the items in the Dealer inventory area
	L"WERT",		//The total value of items player wishes to sell
	L"SCHÄTZUNG",		//Button text for dealer to evaluate items the player wants to sell
	L"TRANSAKTION",		//Button text which completes the deal. Makes the transaction.
	L"FERTIG",		//Text for the button which will leave the shopkeeper interface.
	L"KOSTEN",		//The amount the dealer will charge to repair the merc's goods
	L"1 STUNDE",		// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d STUNDEN",		// PLURAL! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"REPARIERT",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Es ist kein Platz mehr, um Sachen anzubieten.",	//Message box that tells the user there is no more room to put there stuff
	L"%d MINUTEN",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Gegenstand fallenlassen.",
	L"BUDGET",
};

//ShopKeeper Interface
//for the bank machine panels. Referenced here is the acronym ATM, which means Automatic Teller Machine
STR16 SkiAtmText[] =
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
	L"Nehmen",					// Take money from the player
	L"Geben",					// Give money to the player
	L"Abbruch",					// Cancel the transfer
	L"Löschen",					// Clear the money display
};

//Shopkeeper Interface
STR16 gzSkiAtmText[] = 
{	
	// Text on the bank machine panel that....
	L"Vorgang auswählen",			// tells the user to select either to give or take from the merc
	L"Betrag eingeben",			// Enter the amount to transfer
	L"Geld an Söldner überweisen",		// Giving money to the merc
	L"Geld von Söldner überweisen",		// Taking money from the merc
	L"Nicht genug Geld",			// Not enough money to transfer
	L"Kontostand",				// Display the amount of money the player currently has
};

STR16 SkiMessageBoxText[] =
{
	L"Möchten Sie %s von Ihrem Konto abbuchen, um die Differenz zu begleichen?",
	L"Nicht genug Geld. Ihnen fehlen %s.",
	L"Möchten Sie %s von Ihrem Konto abbuchen, um die Kosten zu decken?",
	L"Händler bitten, mit der Überweisung zu beginnen.",
	L"Händler bitten, Gegenstände zu reparieren",
	L"Unterhaltung beenden",
	L"Kontostand",

	L"Do you want to transfer %s Intel to cover the difference?",	// TODO.Translate
	L"Do you want to transfer %s Intel to cover the cost?",
};

//OptionScreen.c
STR16 zOptionsText[] = 
{
	//button Text
	L"Spiel sichern",
	L"Spiel laden",
	L"Beenden",
	L"Nächste",
	L"Vorherige",
	L"Fertig",
	L"1.13 Features",
	L"New in 1.13",
	L"Options",
	//Text above the slider bars
	L"Effekte",
	L"Sprache",
	L"Musik",
	//Confirmation pop when the user selects..
	L"Spiel verlassen und zurück zum Hauptmenü?",
	L"Sprachoption oder Untertitel müssen aktiviert sein.",
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
STR16 zSaveLoadText[] = 
{
	L"Spiel sichern",
	L"Spiel laden",
	L"Abbrechen",
	L"Auswahl speichern",
	L"Auswahl laden",

	L"Spiel erfolgreich gespeichert",
	L"FEHLER beim Speichern des Spiels!",
	L"Spiel erfolgreich geladen",
	L"FEHLER beim Laden des Spiels!",


	L"Der gespeicherte Spielstand unterscheidet sich vom aktuellen Spielstand. Es kann wahrscheinlich nichts passieren. Weiter?",
	L"Die gespeicherten Spielstände sind evtl. beschädigt. Wollen Sie sie alle löschen?", 

	
	//Translators, the next two strings are for the same thing. The first one is for beta version releases and the second one
	//is used for the final version. Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed. It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Gespeicherte Version wurde geändert. Bitte melden Sie etwaige Probleme. Weiter?",
#else
	L"Versuche, älteren Spielstand zu laden. Laden und automatisch aktualisieren?",
#endif


	//Translators, the next two strings are for the same thing. The first one is for beta version releases and the second one
	//is used for the final version. Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed. It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Spielstand und Spieleversion wurden geändert. Bitte melden Sie etwaige Probleme. Weiter?",
#else
	L"Versuche, älteren Spielstand zu laden. Laden und automatisch aktualisieren?",
#endif
	
	L"Gespeichertes Spiel in Position #%d wirklich überschreiben?",
	L"Wollen Sie das Spiel aus Position # speichern?",

	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	//
	L"Sie haben zu wenig Festplattenspeicher. Sie haben nur %d MB frei und JA2 benötigt mindestens %d MB.",
	
										
	L"Speichere",	//While the game is saving this message appears.
	
	L"Normale Waffen",
	L"Zusatzwaffen",
	L"Real-Stil",
	L"SciFi-Stil",
	L"Schwierigkeit",
	L"Platinum Mode",	//Placeholder English
	L"Bobby Ray Qualität",
	L"Normale Auswahl",
	L"Große Auswahl",
	L"Ausgezeichnete Auswahl",
	L"Fantastische Auswahl",

	L"Neues Inventar funktioniert nicht in 640x480 Bildschirmauflösung. Wählen Sie eine höhere Bildschirmauflösung und versuchen Sie es erneut.",
	L"Neues Inventar funktioniert nicht mit dem ausgewählten 'Data' Ordner.",	

	L"Die gespeicherte Truppengröße im Spielstand wird nicht unterstützt bei der aktullen Bildschirmauflösung. Wählen Sie eine höhere Bildschirmauflösung und versuchen Sie es erneut.",
	L"Bobby Ray Auswahl",
};

//MapScreen
STR16 zMarksMapScreenText[] =
{
	L"Map-Level",
	L"Sie haben gar keine Miliz. Sie müssen Bewohner der Stadt trainieren, wenn Sie dort eine Miliz aufstellen wollen.",
	L"Tägl. Einkommen", 
	L"Söldner hat Lebensversicherung", 
	L"%s ist nicht müde.", 
	L"%s ist unterwegs und kann nicht schlafen.", 
	L"%s ist zu müde. Versuchen Sie es ein bisschen später noch mal.",
	L"%s fährt.", 
	L"Der Trupp kann nicht weiter, wenn einer der Söldner pennt.",


	// stuff for contracts
	L"Sie können zwar den Vertrag bezahlen, haben aber kein Geld für die Lebensversicherung.", 
	L"%s Lebensversicherungsprämien kosten %s pro %d Zusatztag(en). Wollen Sie das bezahlen?",
	L"Gegenstände im Sektor", 

	L"Söldner hat Krankenversicherung.", 
	
	
	// other items
	L"Sanitäter", // people acting a field medics and bandaging wounded mercs 
	L"Patienten", // people who are being bandaged by a medic 
	L"Fertig", // Continue on with the game after autobandage is complete 
	L"Stop", // Stop autobandaging of patients by medics now 
	L"Sorry. Diese Option gibt es in der Demo nicht.", // informs player this option/button has been disabled in the demo 
	
	L"%s hat kein Werkzeug.",
	L"%s hat kein Verbandszeug.",
	L"Es sind nicht genug Leute zum Training bereit.",
	L"%s ist voller Milizen.",
	L"Söldner hat begrenzten Vertrag.", 
	L"Vertrag des Söldners beinhaltet keine Versicherung.",
	L"Kartenübersicht",		// 24
	
	// Flugente: disease texts describing what a map view does TODO.Translate
	L"This view shows how many rotting corpses are in a sector. The white number are corpses, the green number is accumulated disease, the sector colour indicates how widespread it is. GREY= No disease known of. GREEN to RED = escalating levels of disease.",	// TODO.Translate

	// Flugente: weather texts describing what a map view does
	L"This view shows current weather. No colour=Sunny. CYAN=Rain. BLUE=Thunderstorm. ORANGE=Sandstorm. WHITE=Snow.",	// TODO.Translate

	// Flugente: describe what intel map view does
	L"This view shows which sectors relevant what ongoing quests. Some data bought with intel is also shown here.", // TODO.Translate
};

STR16 pLandMarkInSectorString[] =
{
	L"Trupp %d hat in Sektor %s jemanden bemerkt.",
	L"Trupp %s hat in Sektor %s jemanden bemerkt.",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] = 
{
	L"Eine Milizeinheit für diese Stadt zu trainieren kostet $", // telling player how much it will cost
	L"Ausgabe genehmigen?", // asking player if they wish to pay the amount requested
	L"Sie haben nicht genug Geld.", // telling the player they can't afford to train this town
	L"Miliz in %s (%s %d) weitertrainieren?", // continue training this town?

	L"Preis $", // the cost in dollars to train militia
	L"( J/N )", // abbreviated yes/no
	L"Miliz auf dem Raketenstützpunkt im Sektor %s (%s %d) weitertrainieren?", // continue trainign militia in SAM site sector
	L"Milizen in %d Sektoren zu trainieren kostet $ %d. %s", // cost to train sveral sectors at once

	L"Sie können sich keine $%d für die Miliz hier leisten.",
	L"%s benötigt eine Loyalität von %d Prozent, um mit dem Milizen-Training fortzufahren.",
	L"Sie können die Miliz in %s nicht mehr trainieren.", 
	L"weitere Stadtteile befreien",

	L"neue Stadtteile befreien",
	L"mehr Städte erobern",
	L"den verlorenen Fortschritt wieder aufholen",
	L"weiter fortschreiten",

	L"mehr Rebellen rekrutieren",
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16 gzMoneyWithdrawMessageText[] = 
{
	L"Sie können nur max. 20.000$ abheben.",
	L"Wollen Sie wirklich %s auf Ihr Konto einzahlen?",
};

STR16 gzCopyrightText[] = 
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd. Alle Rechte vorbehalten.", //
};

//option Text
STR16 zOptionsToggleText[] = 
{
	L"Sprache",
	L"Stumme Bestätigungen",
	L"Untertitel",
	L"Dialoge Pause",
	L"Rauch animieren", 
	L"Blut zeigen",
	L"Cursor nicht bewegen",
	L"Alte Auswahlmethode",
	L"Weg vorzeichnen",
	L"Fehlschüsse anzeigen",
	L"Bestätigung bei Echtzeit",
	L"Schlaf-/Wachmeldung anzeigen", 
	L"Metrisches System benutzen",
	L"Boden beleuchten",
	L"Cursor autom. auf Söldner",
	L"Cursor autom. auf Türen",
	L"Gegenstände leuchten",
	L"Baumkronen zeigen",
	L"Smart Tree Tops", // TODO. Translate
	L"Drahtgitter zeigen",
	L"3D Cursor zeigen",
	L"Trefferchance anzeigen",
	L"GL Burst mit Burst Cursor",
	L"Gegner-Spott aktiveren",		// Changed from "Enemies Drop all Items" - SANDRO
	L"Hohe Granatwerfer-Flugbahn",
	L"Echtzeit-Schleichen aktivieren",	// Changed from "Restrict extra Aim Levels" - SANDRO
	L"Nächste Gruppe selektieren",
	L"Gegenstände mit Schatten",
	L"Waffenreichweite in Felder",
	L"Leuchtspur für Einzelschüsse",
	L"Regengeräusche",
	L"Krähen erlauben",
	L"Tooltips über Gegner",
	L"Automatisch speichern",
	L"Stummer Skyrider",
	L"Erw. Gegenstandsinfo",
	L"Erzwungener Runden-Modus",			// add forced turn mode
	L"Status Trainingsfortschritt",			// Show progress towards stat increase
	L"Alternatives Kartenfarbschema",		// Change color scheme of Strategic Map
	L"Alternative Projektil-Grafik",			// Show alternate bullet graphics (tracers)
	L"Logical Bodytypes",
	L"Söldnerrang anzeigen.",					// shows mercs ranks
	L"Gesichtsequipment-Grafiken",			
	L"Gesichtsequipment-Icons",
	L"Cursor-Wechsel deaktivieren",		    // Disable Cursor Swap
	L"Stummes Trainieren",					// Madd: mercs don't say quotes while training
	L"Stummes Reparieren",					// Madd: mercs don't say quotes while repairing
	L"Stumme Behandlung",					// Madd: mercs don't say quotes while doctoring
	L"Autom. schnelle Gegner-Züge",			// Automatic fast forward through AI turns
	L"Zombies erlauben",					// Flugente Zombies
	L"Inventar Popup-Menüs",				// the_bob : enable popups for picking items from sector inv
	L"Übrige Feinde markieren",				
	L"Tascheninhalt anzeigen",			
	L"Mausradrichtung umkehren",
	L"Bewegung in Formation",				// when multiple mercs are selected, they will try to keep their relative distances
	L"--Cheat Mode Options--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Erzwinge BR Lieferung",				// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--DEBUG OPTIONS--",					// an example options screen options header (pure text)
	L"Report Miss Offsets",					// Screen messages showing amount and direction of shot deviation.
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
	L"Wenn diese Funktion aktiviert ist, werden in Dialogen Stimmen wiedergegeben. Anderenfalls wird nur der Text angezeigt.",

	//Mute Confirmation
	L"Schalten Sie mit dieser Funktion die gesprochenen Bestätigungen (wie \"Okay\" oder \"Bin dran\") aus, wenn sie stören.",

	//Subtitles
	L"Wenn diese Funktion aktiviert ist, wird in Dialogen der entsprechende Text angezeigt.",

	//Key to advance speech
	L"Schalten Sie diese Funktion AN, wenn Sie Dialoge von NPCs ganz in Ruhe lesen wollen. Untertitel müssen dazu AN sein.",

	//Toggle smoke animation
	L"Schalten Sie diese Option ab, wenn animierter Rauch Ihre Bildwiederholrate verlangsamt.",

	//Blood n Gore
	L"Diese Option abschalten, wenn Sie kein Blut sehen können.",

	//Never move my mouse
	L"Schalten Sie diese Option ab, wenn Sie nicht möchten, dass Ihr Mauszeiger automatisch auf Pop-Up-Fenster springt.",

	//Old selection method
	L"Mit dieser Option funktioniert die Auswahl der Söldner so wie in früheren JAGGED ALLIANCE-Spielen (also genau andersherum als jetzt).",

	//Show movement path
	L"Diese Funktion anschalten, um die geplanten Wege der Söldner zum Cursor anzuzeigen\n(oder abgeschaltet lassen und bei gewünschter Anzeige die |S|h|i|f|t-Taste drücken).",

	//show misses
	L"Wenn diese Funktion aktiviert ist, folgt die Spielkamera im Rundenmodus der Geschossflugbahn bis zu ihrem Ende. Ausschalten um das Spiel zu beschleunigen.",

	//Real Time Confirmation
	L"Wenn diese Funktion aktiviert ist, wird für jede Aktion im Echtzeit-Modus ein zusätzlicher \"Sicherheits\"-Klick verlangt um versehentliche Befehle zu vermeiden.",

	//Sleep/Wake notification
	L"Schalten Sie diese Option aus, wenn Sie kein Popup erhalten wollen, sobald zu einem Dienst eingeteilte Söldner schlafen gehen oder die Arbeit wieder aufnehmen.",

	//Use the metric system
	L"Mit dieser Option wird im Spiel das metrische anstelle des imperialen Maßsystems verwendet (z.B. Meter und Kilogramm).",

	//Merc Lighted movement
	L"Diese Funktion beleuchtet für den Spieler die Umgebung des Söldners - auch beim Bewegen. AUSgeschaltet erhöht sich die Bildwiederholrate.\nToggle artificial merc light. (|G)",	// TODO.Translate

	//Smart cursor
	L"Wenn diese Funktion aktiviert ist, springt der Cursor immer automatisch auf Söldner in seiner direkten Nähe.",

	//snap cursor to the door
	L"Wenn diese Funktion aktiviert ist, springt der Cursor automatisch auf Türen in direkter Nähe des Mauszeigers.",

	//glow items
	L"Wenn diese Funktion aktiviert ist, haben Gegenstände am Boden zur besseren Sichtbarkeit einen pulsierenden Rahmen (|C|t|r|l+|A|l|t+|I).",

	//toggle tree tops
	L"Mit der Deaktivierung dieser Funktion lassen sich Baumkronen ausblenden um bessere Sicht auf das Geschehen zu ermöglichen (|T).",

	//smart tree tops
	L"When ON, hides tree tops near visible mercs and cursor position.",	// TODO.Translate

	//toggle wireframe
	L"Wenn diese Funktion aktiviert ist, werden Drahtgitter verborgener Wände gezeigt um z.B. perspektivisch verdeckte Fenster zu erkennen (|C|t|r|l+|A|l|t+|W).",

	L"Wenn diese Funktion aktiviert ist, wird der Bewegungs-Cursor in 3D angezeigt (|H|o|m|e).",
	
	// Options for 1.13
	L"Wenn diese Funktion aktiviert ist, wird die Trefferwahrscheinlichkeit am Cursor angezeigt.",
	L"Mit dieser Funktion lässt sich der Zielcursor für Granatwerfer-Feuerstöße umschalten. Der Burst-Cursor (wenn AN) ermöglicht den Beschuss einer größeren Fläche.",
	L"Wenn diese Funktion aktiviert ist, beschimpfen Gegner den Spieler oder kommentieren ihre Situation mittels kleiner Pop-Ups.", // Changed from Enemies Drop All Items - SANDRO
	L"Wenn diese Funktion aktiviert ist, können Granatwerfer Granaten in höherem Winkel abfeuern und so ihre volle Reichweite ausnutzen (|A|l|t+|Q).",
	L"Wenn diese Funktion aktiviert ist, schaltet das Spiel für unbemerkt schleichende Söldner nicht automatisch in den Rundenmodus sobald Gegner in Sicht geraten, außer Sie drücken |C|t|r|l+|X. (|C|t|r|l+|S|h|i|f|t+|X)", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Wenn diese Funktion aktiviert ist, selektiert |S|p|a|c|e automatisch die nächste Gruppe statt den nächsten Söldner der Gruppe.",
	L"Wenn diese Funktion aktiviert ist, werfen Gegenstände einen Schatten.",
	L"Wenn diese Funktion aktiviert ist, werden Waffenreichweiten in Feldern angezeigt statt in z.B. Metern.",
	L"Wenn diese Funktion aktiviert ist, wird auch für Einzelschüsse mit Leuchtspurmunition der grafische Effekt dazu angezeigt.",
	L"Wenn diese Funktion aktiviert ist, werden Regengeräusche hörbar, sobald es regnet.",
	L"Wenn diese Funktion aktiviert ist, sind Krähen im Spiel vorhanden und hacken lautstark an manchen Leichen herum, haben aber sonst keine großen Auswirkungen auf das Spiel.",
	L"Wenn diese Funktion aktiviert ist, werden mit Druck auf |A|l|t Informationen über den Gegner eingeblendet, auf dem sich der Maus-Cursor befindet.",
	L"Wenn diese Funktion aktiviert ist, wird nach jeder Runde automatisch abwechselnd in zwei speziellen Autosave-Spielständen gespeichert.",
	L"Wenn diese Funktion aktiviert ist, wird Skyrider nichts mehr sagen. Verwenden Sie diese Option, wenn er Ihnen auf die Nüsse geht.",
	L"Wenn diese Funktion aktiviert ist, werden erweiterte Beschreibungen und Werte zu den Waffen und Gegenständen angezeigt.",
	L"Wenn diese Funktion aktiviert ist und noch Gegner im Sektor sind, bleibt das Spiel im Runden-Modus, bis alle Feinde tot sind (|C|t|r|l+|T).",
	L"Wenn diese Funktion aktiviert ist, werden die Söldnerwerte visuell mit ihrem Trainingsfortschritt unterlegt.",
	L"Wenn diese Funktion aktiviert ist, wird die Strategische Karte entsprechend Ihres Erkundungsfortschrittes unterschiedlich eingefärbt.",
	L"Wenn diese Funktion aktiviert ist, werden geschossene Projektile visuell mit Tracer-Effekten dargestellt.",
	L"When ON, mercenary body graphic can change along with equipped gear.",	// TODO.Translate
	L"Wenn diese Funktion aktiviert ist, werden die Ränge der Söldner in der Strategischen Karte vor dem Namen angezeigt.",
	L"Wenn diese Funktion aktiviert ist, sehen sie das Gesichtsequipment Ihrer Söldner direkt auf dem Portrait.",
	L"Wenn diese Funktion aktiviert ist, sehen sie Icons für das Gesichtsequipment in der rechten unteren Ecke des Portraits.",
	L"Wenn diese Funktion aktiviert ist, wird der Mauscursor nicht automatisch wechseln zwischen Personen-Positionswechsel und weiteren Aktionen.\nFür manuellen Positionswechsel drücken Sie |x.",
	L"Wenn diese Funktion aktiviert ist, werden die Söldner über ihren Fortschritt während des Trainings nicht mehr berichten.",
	L"Wenn diese Funktion aktiviert ist, werden die Söldner über ihren Reperaturfortschritt nicht mehr berichten.",
	L"Wenn diese Funktion aktiviert ist, werden die Söldner über den ärztlichen Fortschritt nicht mehr berichten.",
	L"Wenn diese Funktion aktiviert ist, werden gegnerische Züge schneller durchgeführt.",

	L"Wenn diese Funktion aktiviert ist, können Tote als Zombies wieder auferstehen. Seien Sie auf der Hut!",
	L"Wenn diese Funktion aktiviert ist, und Sie mit der linken Maustaste auf einen freien Söldner-Inventarplatz klicken (während das Sektor-Inventar angezeigt wird), wird ein hilfreiches Popup-Menü eingeblendet.",
	L"Wenn diese Funktion aktiviert ist, wird die ungefähre Postion der verbleibenden Feinde auf der Übersichtskarte schraffiert",
	L"Wenn diese Funktion aktiviert ist, wird in der erweiterten Beschreibung von Taschen statt den Anbauteilen deren Inhalt angezeigt.",
	L"Wenn diese Funktion aktiviert ist, wird die Mausradrichtung umgekehrt",
	L"Wenn diese Funktion aktiviert ist und mehrere Sölnder ausgewählt sind, werden sich diese in Formation (in relativen Abständen zueinander) bewegen. (|C|t|r|l+|A|l|t+|G)",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Force all pending Bobby Ray shipments",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: Wenn diese Funktion aktiviert ist, wird der Abstand den jede die Kugel vom Zielmittelpunkt abweicht, unter Berücksichtigung aller CTH-Faktoren, ausgegeben.",
	L"Hier klicken, um fehlerhafte Optionseinstellungen zu beheben.",	// failsafe show/hide option to reset all options
	L"Hier klicken, um fehlerhafte Optionseinstellungen zu beheben.",	// a do once and reset self option (button like effect)
	L"Allows debug options in release or mapeditor builds",				// allow debugging in release or mapeditor
	L"Toggle to display debugging render options",						// an example option that will show/hide other options
	L"Attempts to display slash-rects around mouse regions",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};

STR16 gzGIOScreenText[] =
{
	L"GRUNDEINSTELLUNGEN",
#ifdef JA2UB
	L"Random Manuel texts",
	L"Off",
	L"On",
#else
	L"Spielmodus",
	L"Realistisch",
	L"SciFi",
#endif	
	L"Platinum", //Placeholder English
	L"Waffen",													
	L"Zus. Waffen",
	L"Normal",
	L"Schwierigkeitsgrad",
	L"Einsteiger",				
	L"Profi",
	L"Alter Hase",
	L"WAHNSINNIG",
	L"Starten",
	L"Abbrechen",
	L"Extra schwer",
	L"Jederzeit speichern",
	L"IRONMAN",
	L"Option nicht verfügbar", 
	L"Bobby Ray Qualität",
	L"Normal",
	L"Groß",
	L"Ausgezeichnet",
	L"Fantastisch",
	L"Inventar / Attachments",
	L"NOT USED",			// Alt (Original)
	L"NOT USED",	// Neu - mit Trageausr.
	L"Lade MP Spiel",
	L"GRUNDEINSTELLUNGEN (Nur Servereinstellungen werden verwendet)",
	// Added by SANDRO
	L"Fertigkeiten",
	L"Alte",
	L"Neue",
	L"Max. BSE-Charaktere",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Tote Gegner lassen alles fallen",
	L"Aus",
	L"An",
#ifdef JA2UB
	L"Tex and John",
	L"Zufällig",
	L"Alle vorhanden",
#else
	L"Anzahl der Terroristen",
	L"Zufällig",
	L"Alle vorhanden",
#endif	
	L"Geheime Waffenlager",
	L"Zufällig",
	L"Alle vorhanden",
	L"Fortschritt Waffenwahl",
	L"Sehr langsam",
	L"Langsam",
	L"Normal",
	L"Schnell",
	L"Sehr schnell",
	L"Alt / Alt",
	L"Neu / Alt",
	L"Neu / Neu",

	// Squad Size
	L"Max. Truppengröße",
	L"6",
	L"8",
	L"10",
	//L"Schneller Bobby Ray Lieferungen",
	L"Inventarzugriff kostet APs",	
	L"Neues Zielsystem",
	L"Verbesserte Unterbrechungen",
	L"Söldner-Hintergrundgeschichten",
	L"Nahrungssystem",
	L"Bobby Ray Auswahl",

	// anv: extra iron man modes
	L"SOFT IRONMAN",
	L"EXTREME IRONMAN",
};

STR16	gzMPJScreenText[] =
{
	L"MEHRSPIELER",
	L"Teilnehmen",
	L"Eröffnen",
	L"Abbrechen",
	L"Aktualisieren",
	L"Spielername",
	L"Server-IP",
	L"Port",
	L"Servername",
	L"# Spieler",
	L"Version",
	L"Spieltyp",
	L"Ping",
	L"Sie müssen einen Spielernamen eingeben.",
	L"Sie müssen eine gültie Server-IP-Adresse eingeben. Zum Beispiel: 84.114.195.239",
	L"Sie müssen eine gültige Server-Portnummer zwischen 1 und 65535 eingeben.",
};

STR16 gzMPJHelpText[] =
{
	L"Besuchen Sie http://webchat.quakenet.org/?channels=ja2-multiplayer um sich mit anderen Spielern zu treffen.",
	L"Drücken Sie 'y' um das Chat-Fenster im Spiel zu öffnen, nachdem Sie mit dem Server verbunden sind.",

	L"ERÖFFNEN",
	L"Geben Sie '127.0.0.1' für die IP Adresse ein. Die Port Nummer sollte größer als 60000 sein.",
	L"Vergewissern Sie sich, dass das Port (UDP, TCP) auf dem Router weitergeleitet wird. Siehe: http://portforward.com",
	L"Sie müssen Ihre externe IP (http://www.whatismyip.com) und die Port Nummer an die anderen Spieler schicken (via IRC, ICQ, etc.).",
	L"Klicken Sie auf 'Eröffnen', um ein neues Spiel zu eröffnen.",
	
	L"TEILNEHMEN",
	L"Der Host muss Ihnen die externe IP Adresse und die Port Nummer schicken (via IRC, ICQ, etc.).",
	L"Geben Sie die externe IP und die Port Nummer des Hosts ein.",
	L"Klicken Sie auf 'Teilnehmen', um an einem bereits eröffneten Spiel teilzunehmen.",
};

STR16	gzMPHScreenText[] =
{
	L"ERÖFFNE SPIEL",
	L"Starten",
	L"Abbrechen",
	L"Servername",
	L"Spieltyp",
	L"Deathmatch",
	L"Team-Deathmatch",
	L"Kooperativ",
	L"Maximale Spieler",
	L"Maximale Söldner",
	L"Söldnerauswahl",
	L"Söldnerrekrutierung",
	L"Söldner selbst wählen",
	L"Startkapital",
	L"Gleiche Söldner erlaubt",
	L"Angeheuerte Söldner anzeigen",
	L"Bobby Ray",
	L"Sektor Startzone",
	L"Sie müssen einen Servernamen eingeben.",
	L"",
	L"",
	L"Tageszeit",
	L"",
	L"" ,
	L"Waffenschaden",
	L"",
	L"Rundenzeitbegrenzung",
	L"",
	L"Erlaube Zivilisten in CO-OP",
	L"",
	L"Maximale KI-Gegner in CO-OP",
	L"Synchronisiere Verzeichnisse",
	L"Synchronisationsverzeichnis",
	L"Sie müssen ein gültiges MP-Synchronisationsverzeichnis eingeben.",
	L"(Benutzen Sie '/' anstelle von '\\' als Verzeichnistrennzeichen.)",
	L"Das angegebene MP-Synch.-Verzeichnis existiert nicht.",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	// Max. Enemies / Report Hired Merc / Enable Civs in CO-OP
	L"Ja",
	L"Nein",
	// Starting Time
	L"Morgen",
	L"Nachmittag",
	L"Nacht",
	// Starting Cash
	L"Wenig",
	L"Mittel",
	L"Viel",
	L"Unendlich",
	// Time Turns
	L"Niemals",
	L"Langsam",
	L"Mittel",
	L"Schnell",
	// Weapon Damage
	L"Sehr gering",
	L"Gering",
	L"Normal",
	// Merc Hire
	L"Zufällig",
	L"Normal",
	// Sector Edge
	L"Zufällig",
	L"Wählbar",
	// Bobby Ray / Hire same merc
	L"Verbieten",
	L"Erlauben",
};

STR16 pDeliveryLocationStrings[] =
{
	L"Austin",			//Austin, Texas, USA	
	L"Bagdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items. The other towns except Meduna are dummy names...
	L"Hong Kong",			//Hong Kong, Hong Kong
	L"Beirut",			//Beirut, Lebanon	(Middle East)
	L"London",			//London, England
	L"Los Angeles",			//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",			//The island of Metavira was the fictional location used by JA1
	L"Miami",			//Miami, Florida, USA (SE corner of USA)
	L"Moskau",			//Moscow, USSR
	L"New York",			//New York, New York, USA
	L"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Paris",			//Paris, France
	L"Tripolis",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokio",			//Tokyo, Japan
	L"Vancouver",			//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation. It is possible to select 0 ability
	//in a skill meaning you can't use it. This text is confirmation to the player.
	L"Sind Sie sicher? Ein Wert von 0 bedeutet, dass der Charakter diese Fähigkeit nicht nutzen kann.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"(max. 8 Buchstaben)",
};

STR16 pIMPFinishButtonText[] =
{
	L"Analyse wird durchgeführt",
};

STR16 pIMPFinishStrings[] =
{
	L"Danke, %s",	//%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] = 
{
	L"Stimme", 
};

STR16 pDepartedMercPortraitStrings[] = 
{
	L"Im Einsatz getötet",
	L"Entlassen",
	L"Sonstiges",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Söldner-Manager",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Pause",
	L"Zurück zum Spiel (|P|a|u|s|e)",
	L"Pause (|P|a|u|s|e)",
};

STR16 pMessageStrings[] =
{
	L"Spiel beenden?",
	L"OK",
	L"JA",
	L"NEIN",
	L"ABBRECHEN",
	L"ZURÜCK",
	L"LÜGEN",
	L"Keine Beschreibung",			//Save slots that don't have a description.
	L"Spiel gespeichert",	
	L"Spiel gespeichert",	
	L"QuickSave",		//10		//The name of the quicksave file (filename, text reference)
	L"SaveGame",				//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",					//The 3 character dos extension (represents sav)
	L"..\\SavedGames",			//The name of the directory where games are saved.
	L"Tag",
	L"Söldner",
	L"Leere Spiel Position",				//An empty save game slot
	L"Demo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"Veröffentlichung",			//Release build for JA2
	L"RpM",			//20		//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.	//LOOTF - KpM macht Augenkrebs, KpM gibt es einfach nicht.
	L"min",					//Abbreviation for minute.
	L"m",					//One character abbreviation for meter (metric distance measurement unit).
	L"Kgln",				//Abbreviation for rounds (# of bullets)		//LOOTF - character limit? Kugeln = kacke, will ändern!
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"Pfd",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Home",				//Home as in homepage on the internet.
	L"US$",					//Abbreviation for US Dollars
	L"n.a.",				//Lowercase acronym for not applicable.
	L"Inzwischen",				//Meanwhile
	L"%s ist im Sektor %s%s angekommen", //30 //Name/Squad has arrived in sector A9. Order must not change without notifying SirTech
	L"Version", 
	L"Leere Quick-Save Position",
	L"Diese Position ist für Quick-Saves aus dem Karten- oder Taktik-Bildschirm reserviert. Speichern mit ALT+S.",
	L"offen",
	L"zu",
	L"Ihr Festplattenspeicher ist knapp. Sie haben lediglich %sMB frei und Jagged Alliance 2 v1.13 benötigt %sMB.",
	L"%s von AIM angeheuert", 
	L"%s hat %s gefangen.",			//'Merc name' has caught 'item' -- let SirTech know if name comes after item.

	L"%s hat %s eingenommen.",			//'Merc name' has taken 'item name'
	L"%s hat keine medizinischen Fähigkeiten",//40	//'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"Die Integrität des Spieles wurde beschädigt.", //The integrity of the game has been compromised
	L"FEHLER: CD-ROM-Laufwerk schließen",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Kein Platz, um von hier aus zu feuern.",
	
	//Can't change stance due to objects in the way...
	L"Kann seine Position hier nicht ändern.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Ablegen",
	L"Werfen",
	L"Weitergeben",

	L"%s weitergegeben an %s.", //"Item" passed to "merc". Please try to keep the item %s before the merc %s, otherwise you must notify SirTech.
	L"Kein Platz, um %s an %s weiterzugeben.", //pass "item" to "merc". Same instructions as above.

	//A list of attachments appear after the items. Ex: Kevlar vest ( Ceramic Plate 'Attached )'
	L" angebracht )",			// 50

	//Cheat modes
	L"Cheat-Level EINS erreicht",
	L"Cheat-Level ZWEI erreicht",

	//Toggling various stealth modes
	L"Schleichbewegung für Trupp ein.",
	L"Schleichbewegung für Trupp aus.",
	L"Schleichbewegung für %s ein.",
	L"Schleichbewegung für %s aus.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine. You can toggle this mode freely in the game.
	L"Drahtgitter ein",
	L"Drahtgitter aus",

	//These are used in the cheat modes for changing levels in the game. Going from a basement level to
	//an upper level, etc. 
	L"Von dieser Ebene geht es nicht nach oben...",
	L"Noch tiefere Ebenen gibt es nicht...",		// 60
	L"Gewölbeebene %d betreten...",
	L"Gewölbe verlassen...",

	L"s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Autoscrolling AUS.",
	L"Autoscrolling AN.",
	L"3D-Cursor AUS.",
	L"3D-Cursor AN.",
	L"Trupp %d aktiv.",
	L"Sie können %ss Tagessold von %s nicht zahlen",	//first %s is the mercs name, the second is a string containing the salary
	L"Abbruch",			// 70
	L"%s kann alleine nicht gehen.",
	L"Spielstand namens SaveGame249.sav kreiert. Wenn nötig, in SaveGame01 - SaveGame10 umbennen und über die Option 'Laden' aufrufen.",
	L"%s hat %s getrunken.",
	L"Paket in Drassen angekommen.",
	L"%s kommt am %d. um ca. %s am Zielort an (Sektor %s).", //first %s is mercs name(OK), next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival       !!!7 It should be like this: first one is merc (OK), next is day of arrival (OK) , next is time of the day for ex. 07:00 (not OK, now it is still sector), next should be sector (not OK, now it is still time of the day)	//LOOTF - is this still valid? I assume it's not.
	L"Logbuch aktualisiert.",
	L"Granatenwerfer-Feuerstöße verwenden Ziel-Cursor (Sperrfeuer aktiviert).",
	L"Granatenwerfer-Feuerstöße verwenden Flugbahn-Cursor (Sperrfeuer deaktiviert).",
	L"Soldaten-Kurzinfo (\"Tooltips\") aktiviert",		// Changed from Drop All On - SANDRO
	L"Soldaten-Kurzinfo (\"Tooltips\") deaktiviert", 	// 80	// Changed from Drop All Off - SANDRO
	L"Granatwerfer schießen in flachem Winkel.",
	L"Granatwerfer schießen in steilem Winkel.",
	// forced turn mode strings
	L"Erzwungener Rundenmodus",
	L"Normaler Rundenmodus",
	L"Verlasse Kampfmodus",
	L"Erzwungener Rundenmodus ist aktiv, gehe in Kampfmodus",
	L"Spiel erfolgreich in Position End Turn Auto Save gespeichert.",
	L"..\\SavedGames\\MP_SavedGames",			//The name of the directory where games are saved.
	L"Client",
	
	L"Sie können nicht altes Inventar und neues Attachment System gleichzeitig verwenden.",
	L"Automatischer Spielstandspeicherung #", //91		// Text des Auto Saves im Load Screen mit ID
	L"Dieser Platz ist reserviert für Spielstände die automatisch gespeichert werden. Dies kann ein/ausgeschaltet werden in der Datei ja2_options.ini (AUTO_SAVE_EVERY_N_HOURS).", //92	// The text, when the user clicks on the save screen on an auto save
	L"Leerer Platz für automatische Spielstandspeicherung #", //93	// The text, when the auto save slot (1 - 5) is empty (not saved yet)
	L"AutoSpielstand",		// 94	// The filename of the auto save, such as AutoSaveGame01 - AutoSaveGame05
	L"Zugende Spielstand #",	// 95	// The text for the tactical end turn auto save
	L"Speichere Automatischen Spielstand #",	// 96	// The message box, when doing auto save
	L"Speichere",	// 97	// The message box, when doing end turn auto save
	L"Leere Platz für Spieler-Zugende Spielstandspeicherung #",	// 98	// The message box, when doing auto save
	L"Dieser Platz ist reserviert für Spielstände am Ende eines Spieler Zuges. Dies kann ein/ausgeschaltet werden in den Spieleinstellungen.", //99	// The text, when the user clicks on the save screen on an auto save
	// Mouse tooltips
	L"QuickSave.sav",	// 100
	L"AutoSaveGame%02d.sav",	// 101
	L"Auto%02d.sav",	// 102
	L"SaveGame%02d.sav", //103
	// Lock / release mouse in windowed mode (window boundary)
	L"Mausberech begrenzen, damit Mauscursor innerhalb des Spielfensters bleibt.",	// 104
	L"Mausbereich wieder freigeben, um uneingeschränkte Mausbewebung zu erhalten.",	// 105
	L"In Formation bewegen - EINGESCHALTET",
	L"In Formation bewegen - AUSGESCHALTET",
	L"Artificial Merc Light ON",	// TODO.Translate
	L"Artificial Merc Light OFF",
	L"Gruppe %s aktiv.",
	L"%s hat %s geraucht.",
	L"Cheats aktivieren?",
	L"Cheats deaktivieren?",
};

CHAR16 ItemPickupHelpPopup[][40] =
{
	L"OK",
	L"Hochscrollen",
	L"Alle auswählen",
	L"Runterscrollen",
	L"Abbrechen",
};

STR16 pDoctorWarningString[] =
{
	L"%s ist nicht nahe genug, um geheilt zu werden",
	L"Ihre Mediziner haben noch nicht alle verbinden können.",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"Zuordnung auflösen (|R|e|c|h|t|s |K|l|i|c|k)\nZuordnen (|L|i|n|k|s |K|l|i|c|k)\nGrüne Miliz", // button help text informing player they can pick up or drop militia with this button
	L"Zuordnung auflösen (|R|e|c|h|t|s |K|l|i|c|k)\nZuordnen (|L|i|n|k|s |K|l|i|c|k)\nReguläre Miliz",
	L"Zuordnung auflösen (|R|e|c|h|t|s |K|l|i|c|k)\nZuordnen (|L|i|n|k|s |K|l|i|c|k)\nElite Miliz",
	L"Verteile Miliz gleichwertig über alle Sektoren",
};

STR16 pMapScreenJustStartedHelpText[] = 
{
	L"Zu AIM gehen und Söldner anheuern ( *Tip*: Befindet sich im Laptop )", // to inform the player to hire some mercs to get things going
#ifdef JA2UB
	L"Sobald Sie für die Reise nach Tracona bereit sind, klicken Sie auf den Zeitraffer-Button unten rechts auf dem Bildschirm.", // to inform the player to hit time compression to get the game underway
#else
	L"Sobald Sie für die Reise nach Arulco bereit sind, klicken Sie auf den Zeitraffer-Button unten rechts auf dem Bildschirm.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Fehler. Fehlende oder fehlerhafte Datei(en). Spiel wird beendet.",
};

STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"E-Mail einsehen",
	L"Websites durchblättern",
	L"Dateien und Anlagen einsehen",
	L"Logbuch lesen",
	L"Team-Info einsehen",
	L"Finanzen und Notizen einsehen", 

	L"Laptop schließen",

	//Bottom task bar icons (if they exist):
	L"Sie haben neue Mail",
	L"Sie haben neue Dateien",

	//Bookmarks:
	L"Association of International Mercenaries",
	L"Bobby Rays Online-Waffenversand",
	L"Bundesinstitut für Söldnerevaluierung",
	L"More Economic Recruiting Center",
	L"McGillicuttys Bestattungen",
	L"Fleuropa",
	L"Versicherungsmakler für A.I.M.-Verträge",
	//New Bookmarks
	L"",
	L"Enzyklopädie",
	L"Einsatzbesprechung",
	L"Geschichte",
	L"Mercenaries Love or Dislike You",	// TODO.Translate
	L"World Health Organization",
	L"Kerberus - Experience In Security",
	L"Militia Overview",	// TODO.Translate
	L"Recon Intelligence Services",	// TODO.Translate
	L"Controlled factories",	// TODO.Translate
};

STR16 gzHelpScreenText[] =
{
	L"Helpscreen verlassen",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Es tobt eine Schlacht. Sie können sich nur im Taktik-Bildschirm zurückziehen.",
	L"Sektor betreten und Kampf fortsetzen (|E).",
	L"Kampf durch PC entscheiden (|A).",
	L"Sie können den Kampf nicht vom PC entscheiden lassen, wenn Sie angreifen.",
	L"Sie können den Kampf nicht vom PC entscheiden lassen, wenn Sie in einem Hinterhalt sind.",
	L"Sie können den Kampf nicht vom PC entscheiden lassen, wenn Sie gegen Monster kämpfen.",
	L"Sie können den Kampf nicht vom PC entscheiden lassen, wenn feindliche Zivilisten da sind.",
	L"Sie können einen Kampf nicht vom PC entscheiden lassen, wenn Bloodcats da sind.",
	L"KAMPF IN GANGE",
	L"Sie können sich nicht zurückziehen, wenn Sie in einem Hinterhalt sind.",
};

STR16 gzMiscString[] =
{
	L"Ihre Milizen kämpfen ohne die Hilfe der Söldner weiter...",
	L"Das Fahrzeug muss nicht mehr aufgetankt werden.",
	L"Der Tank ist %d%% voll.",
	L"Deidrannas Armee hat wieder volle Kontrolle über %s.",
	L"Sie haben ein Tanklager verloren.",
};


STR16 gzIntroScreen[] = 
{
	L"Kann Introvideo nicht finden",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH." 
STR16 pNewNoiseStr[] =
{ 
	//There really isn't any difference between using "coming from" or "to".
	//For the explosion case the string in English could be either:
	//	L"Gus hears a loud EXPLOSION 'to' the north.",
	//	L"Gus hears a loud EXPLOSION 'coming from' the north.",
	//For certain idioms, it sounds better to use one over the other. It is a matter of preference.
	L"%s hört %s aus dem %s.",
	L"%s hört eine BEWEGUNG (%s) von %s.", 
	L"%s hört ein KNARREN (%s) von %s.",
	L"%s hört ein KLATSCHEN (%s) von %s.",
	L"%s hört einen AUFSCHLAG (%s) von %s.",
	L"%s hört ein %s GESCHÜTZFEUER von %s.", // anv: without this, all further noise notifications were off by 1!
	L"%s hört eine EXPLOSION (%s) von %s.", 
	L"%s hört einen SCHREI (%s) von %s.",
	L"%s hört einen AUFSCHLAG (%s) von %s.",
	L"%s hört einen AUFSCHLAG (%s) von %s.",
	L"%s hört ein ZERBRECHEN (%s) von %s.",
	L"%s hört ein ZERSCHMETTERN (%s) von %s.",
	L"",											// anv: placeholder for silent alarm
	L"%s hört irgendeine %s STIMME von %s.",		// anv: report enemy taunt to player
};

STR16 pTauntUnknownVoice[] =
{
	L"Unbekannte Stimme",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Sort. nach Name (|F|1)",
	L"Sort. nach Auftrag (|F|2)",
	L"Sort. nach wach/schlafend (|F|3)",
	L"Sort. nach Ort (|F|4)",
	L"Sort. nach Ziel (|F|5)",
	L"Sort. nach Vertragsende (|F|6)",
};

STR16		BrokenLinkText[] = 
{
	L"Error 404",
	L"Seite nicht gefunden.",
};

STR16 gzBobbyRShipmentText[] = 
{
	L"Letzte Lieferungen",
	L"Bestellung #",
	L"Artikelanzahl",
	L"Bestellt am",
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
	L"Game Internals Programmer", 			// Chris Camfield
	L"Co-designer/Writer",				// Shaun Lyng
	L"Strategic Systems & Editor Programmer",	// Kris \"The Cow Rape Man\" Marnes
	L"Producer/Co-designer",			// Ian Currie
	L"Co-designer/Map Designer",			// Linda Currie
	L"Artist",					// Eric \"WTF\" Cheng
	L"Beta Coordinator, Support",			// Lynn Holowka
	L"Artist Extraordinaire",			// Norman \"NRG\" Olsen
	L"Sound Guru",					// George Brooks
	L"Screen Designer/Artist",			// Andrew Stacey
	L"Lead Artist/Animator",			// Scot Loving
	L"Lead Programmer",				// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmer",					// Dave French
	L"Strategic Systems & Game Balance Programmer",	// Alex Meduna
	L"Portraits Artist",				// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 						// Chris Camfield
	L"(still learning punctuation)",		// Shaun Lyng
	L"(\"It's done. I'm just fixing it\")",		//Kris \"The Cow Rape Man\" Marnes
	L"(getting much too old for this)",		// Ian Currie
	L"(and working on Wizardry 8)",			// Linda Currie
	L"(forced at gunpoint to also do QA)",		// Eric \"WTF\" Cheng
	L"(Left us for the CFSA - go figure...)",	// Lynn Holowka
	L"",						// Norman \"NRG\" Olsen
	L"",						// George Brooks
	L"(Dead Head and jazz lover)",			// Andrew Stacey
	L"(his real name is Robert)",			// Scot Loving
	L"(the only responsible person)",		// Andrew \"Big Cheese Doddle\" Emmons
	L"(can now get back to motocrossing)",		// Dave French
	L"(stolen from Wizardry 8)",			// Alex Meduna
	L"(did items and loading screens too!)",	// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s hat seine eigenen Gegenstände repariert.",
	L"%s hat die Waffen und Rüstungen aller Teammitglieder repariert.",
	L"%s hat die aktivierten Gegenstände aller Teammitglieder repariert.",
	L"%s hat die großen mitgeführten Gegenstände aller Teammitglieder repariert.",
	L"%s hat die mittelgroßen mitgeführten Gegenstände aller Teammitglieder repariert.",
	L"%s hat die kleinen mitgeführten Gegenstände aller Teammitglieder repariert.",
	L"%s hat die Trageausrüstung aller Teammitglieder repariert.",
	L"%s hat die Waffen aller Teammitglieder gereinigt.",
};

STR16 zGioDifConfirmText[]=
{
	L"Sie haben sich für den Einsteiger-Modus entschieden. Dies ist die passende Einstellung für Spieler, die noch nie zuvor Jagged Alliance oder ähnliche Spiele gespielt haben oder für Spieler, die sich ganz einfach kürzere Schlachten wünschen. Ihre Wahl wird den Verlauf des ganzen Spiels beeinflussen. Treffen Sie also eine sorgfältige Wahl. Sind Sie ganz sicher, dass Sie im Einsteiger-Modus spielen wollen?",
	L"Sie haben sich für den Profi-Modus entschieden. Dies ist die passende Einstellung für Spieler, die bereits Erfahrung mit Jagged Alliance oder ähnlichen Spielen haben. Ihre Wahl wird den Verlauf des ganzen Spiels beeinflussen. Treffen Sie also eine sorgfältige Wahl. Sind Sie ganz sicher, dass Sie im Profi-Modus spielen wollen?",
	L"Sie haben sich für den Alter Hase-Modus entschieden. Na gut, wir haben Sie gewarnt. Machen Sie hinterher bloß nicht uns dafür verantwortlich, wenn Sie im Sarg nach Hause kommen. Ihre Wahl wird den Verlauf des ganzen Spiels beeinflussen. Treffen Sie also eine sorgfältige Wahl. Sind Sie ganz sicher, dass Sie im Alter Hase-Modus spielen wollen?",
	L"Sie haben sich für den WAHNSINNIG-Modus entschieden. WARNUNG: Beschweren Sie sich nicht, wenn Sie in kleinen Stücken zurückkommen ... Deidranna wird Sie in den Allerwertesten treten und das schmerzhaft. Ihre Wahl wird den Verlauf des ganzen Spiels beeinflussen. Treffen Sie also eine sorgfältige Wahl. Sind Sie ganz sicher, dass Sie im WAHNSINNIG-Modus spielen wollen?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S Loadscreen-Daten nicht gefunden...",

	//1-5
	L"Der Roboter kann diesen Sektor nicht verlassen, wenn niemand die Fernbedienung benutzt.",

	L"Sie können den Zeitraffer jetzt nicht benutzen. Warten Sie das Feuerwerk ab!",
	L"%s will sich nicht bewegen.",
	L"%s hat nicht genug Energie, um die Position zu ändern.",
	L"%s hat kein Benzin mehr und steckt in %c%d fest.",

	//6-10 

	// the following two strings are combined with the strings below to report noises
	// heard above or below the merc
	L"oben",
	L"unten",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Keiner der Söldner hat medizinische Fähigkeiten.",
	L"Sie haben kein Verbandszeug.",
	L"Sie haben nicht genug Verbandszeug, um alle zu verarzten.",
	L"Keiner der Söldner muss verbunden werden.",
	L"Söldner automatisch verbinden.",
	L"Alle Söldner verarztet.",

	//14-16
#ifdef JA2UB
	L"Tracona",
#else
	L"Arulco",
#endif
	L"(Dach)",
	L"Gesundheit: %d/%d",

	//17
	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8" 
	//"vs." is the abbreviation of versus.
	L"%d gegen %d",

	//18-19
	L"%s ist voll!", //(ex "The ice cream truck is full")
	L"%s braucht nicht eine schnelle Erste Hilfe, sondern eine richtige medizinische Betreuung und/oder Erholung.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s ist am Bein getroffen und hingefallen!",
	//Name can't speak right now.
	L"%s kann gerade nicht sprechen.",

	//22-24 plural versions
	L"%d grüne Milizen wurden zu Elitemilizen befördert.",
	L"%d grüne Milizen wurden zu regulären Milizen befördert.",
	L"%d reguläre Milizen wurden zu Elitemilizen befördert.",

	//25 
	L"Schalter",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s dreht durch!",

	//27-28
	//Messages why a player can't time compress.
	L"Es ist momentan gefährlich den Zeitraffer zu betätigen, da Sie noch Söldner in Sektor %s haben.",
	L"Es ist gefährlich den Zeitraffer zu betätigen, wenn Sie noch Söldner in den von Monstern verseuchten Minen haben.",

	//29-31 singular versions
	L"1 grüne Miliz wurde zur Elitemiliz befördert.",
	L"1 grüne Miliz wurde zur regulären Miliz befördert.",
	L"1 reguläre Miliz wurde zur Elitemiliz befördert.",

	//32-34
	L"%s sagt überhaupt nichts.",
	L"Zur Oberfläche gehen?",
	L"(Trupp %d)",

	//35
	L"%s reparierte %ss %s", 

	//36
	L"BLOODCAT", 

	//37-38 "Name trips and falls" 
	L"%s stolpert und stürzt",
	L"Dieser Gegenstand kann von hier aus nicht aufgehoben werden.",

	//39 
	L"Keiner Ihrer übrigen Söldner ist in der Lage zu kämpfen. Die Miliz wird die Monster alleine bekämpfen",

	//40-43
	//%s is the name of merc.
	L"%s hat keinen Erste-Hilfe-Kasten mehr!",
	L"%s hat nicht das geringste Talent, jemanden zu verarzten!",
	L"%s hat keinen Werkzeugkasten mehr!",
	L"%s ist absolut unfähig dazu, irgendetwas zu reparieren!",

	//44
	L"Repar. Zeit",
	L"%s kann diese Person nicht sehen.", 

	//46-48
	L"%ss Gewehrlauf-Verlängerung fällt ab!",
	L"Es sind nicht mehr als %d Miliz-Ausbilder in diesem Sektor erlaubt.",
	L"Sind Sie sicher?", // 

	//49-50
	L"Zeitraffer", //time compression
	L"Der Fahrzeugtank ist jetzt voll.",

	//51-52 Fast help text in mapscreen.
	L"Zeitraffer fortsetzen (|S|p|a|c|e)",
	L"Zeitraffer anhalten (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s hat die Ladehemmung der %s behoben",
	L"%s hat die Ladehemmung von %ss %s behoben",

	//55
	L"Die Zeit kann nicht komprimiert werden, während das Sektorinventar eingesehen wird.",

	L"Die Jagged Alliance 2 v1.13 PLAY CD wurde nicht gefunden. Das Programm wird jetzt beendet.",

	//L"Im Sektor sind Feinde entdeckt worden",		//STR_DETECTED_SIMULTANEOUS_ARRIVAL

	L"Die Gegenstände wurden erfolgreich miteinander kombiniert.",

	//58
	//Displayed with the version information when cheats are enabled.
	L"Aktueller/Max. Fortschritt: %d%%/%d%%",

	//59
	L"John und Mary eskortieren?",

	L"Schalter aktiviert.",

	L"%s's Rüstungsverstärkung wurde zertrümmert!",
	L"%s feuert %d Schüsse mehr als beabsichtigt!",
	L"%s feuert einen Schuss mehr als beabsichtigt!",

	L"Sie müssen zuerst das Gegenstandsbeschreibungsfenster schließen!",

	L"Zeitraffer kann nicht betätigt werden - Feindliche Zivilisten/Bloodcats sind im Sektor.", // 65
};

STR16 gzCWStrings[] = 
{
	L"Verstärkung aus benachbarten Sektoren nach %s rufen?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Ort: %d\n",
	L"%s|Helligkeit: %d / %d\n",
	L"%s|Entfernung zum |Ziel: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Befehle: %d\n",
	L"%s|Gesinnung: %d\n",
	L"%s|Aktuelle |A|Ps: %d\n",
	L"%s|Aktuelle |Gesundheit: %d\n",
	L"%s|Aktueller |Atem: %d\n",
	L"%s|Aktuelle |Moral: %d\n",
	L"%s|Aktueller |Schock: %d\n",
	L"%s|Aktuelle |S|perrfeuer P.: %d\n",
	// Full info
	L"%s|Helm: %s\n",
	L"%s|Weste: %s\n",
	L"%s|Hose: %s\n",
	// Limited, Basic
	L"|Rüstung: ",
	L"Helm",
	L"Weste",
	L"Hose",
	L"getragen",
	L"keine Rüstung",
	L"%s|N|V|G: %s\n",
	L"kein NVG",
	L"%s|Gasmaske: %s\n",
	L"keine Gasmaske",
	L"%s|Kopf |Position |1: %s\n",
	L"%s|Kopf |Position |2: %s\n",
	L"\n(im Rucksack) ",
	L"%s|Waffe: %s ",
	L"keine Waffe",
	L"Pistole",
	L"SMG",
	L"Gewehr",
	L"MG",
	L"Schrotflinte",
	L"Messer",
	L"Schwere Waffe",
	L"kein Helm",
	L"keine Weste",
	L"keine Hose",
	L"|Rüstung: %s\n", 
	 // Added - SANDRO
	L"%s|Fertigkeit 1: %s\n",
	L"%s|Fertigkeit 2: %s\n", 
	L"%s|Fertigkeit 3: %s\n",
    // Additional suppression effects - sevenfm
    L"%s|A|Ps verloren aufgrund von |U|nterdrückung: %d\n",
    L"%s|Unterdrückungs-|Toleranz: %d\n",
    L"%s|Effektive |S|chock |Stufe: %d\n",
    L"%s|K|I |Moral: %d\n",
};

STR16 New113Message[] =
{
	L"Sturm startet.",
	L"Sturm endet.",
	L"Regen startet.",
	L"Regen endet.",
	L"Vorsicht vor Scharfschützen...",
	L"Unterdrückungsfeuer!",	
	L"BRST",
	L"AUTO",
	L"GL",
	L"GL BRST",
	L"GL AUTO",
	L"UB",
	L"UBRST",
	L"UAUTO",
	L"BAYONET",
	L"Scharfschütze!",
	L"Geld kann nicht aufgeteilt werden, weil ein Gegenstand am Cursor ist.",
	L"Ankunft der neuen Söldner wurde in den Sektor %s verlegt, weil der geplante Sektor %s von Feinden belagert ist.",
	L"Gegenstand gelöscht.",
	L"Alle Gegenstände dieses Typs gelöscht.",
	L"Gegenstand verkauft.",
	L"Alle Gegenstände dieses Typs verkauft.",
	L"Überprüfen Sie die Sichtgeräte Ihrer Söldner!",
	// Real Time Mode messages
	L"Sie sind bereits im Kampfmodus",
	L"Keine Gegner in Sicht",
	L"Echtzeit-Schleichmodus AUS",
	L"Echtzeit-Schleichmodus AN",
	//L"Gegner gesichtet! (Ctrl + x für Rundenmodus)",
	L"Gegner gesichtet!",	// this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO 
	L"%s hatte Erfolg beim Stehlen!",
	L"%s hatte nicht genug Aktionspunkte um alles zu stehlen.",
	L"Möchten Sie %s vor dem Bandagieren operativ behandeln? (Sie können etwa %i Lebenspunkte wiederherstellen.)",
	L"Möchten Sie %s operativ behandeln? (Sie können etwa %i Lebenspunkte wiederherstellen.)",
	L"Möchten Sie zuerst Operationen durchführen? (%i Patient(en))",
	L"Möchten Sie an diesem Patienten zuerst eine Operation durchführen?",
	L"Erste Hilfe automatisch mit entsprechender operativer Behandlung durchführen oder ohne?",
	L"Do you want to perform surgery on %s before bandaging? (You can heal about %i health, *: %i by blood bag use.)",	// TODO.Translate
	L"Do you want to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"Do you wish to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"%s wurde erfolgreich operiert.",
	L"%s ist am Torso getroffen und verliert einen Punkt maximaler Gesundheit!",
	L"%s ist am Torso getroffen und verliert %d Punkte maximaler Gesundheit!",
	L"%s is blinded by the blast!",	// TODO.Translate
	L"%s hat einen Punkt an %s wiedergewonnen.",
	L"%s hat %d Punkte an %s wiedergewonnen.",
	L"Ihre Späher-Fertigkeit hat Sie davor bewahrt, vom Gegner in einen Hinterhalt gelockt zu werden.",
	L"Dank Ihrer Späher-Fertigkeit haben Sie erfolgreich ein Rudel Bloodcats umgangen.",
	L"%s wurde in die Leiste getroffen und windet sich in Schmerzen!",
	//////////////////////////////////////////////////////////////////////////////////////
	L"Warning: enemy corpse found!!!",
	L"%s [%d rnds]\n%s %1.1f %s",
	L"Zu wenig APs! Es werden %d APs benötigt, Sie haben aber nur %d APs.",
	L"Tipp: %s",
	L"Spieler Stärke: %d - Gegner Stärke: %6.0f",		// Surrender values to be printed, if DISPLAY_SURRENDER_VALUES = TRUE

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
	L"Ja*",
	L"Ja",
	L"Nein",
	L"%s applied %s to %s.", // TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s zittert vor Angst!",
	L"%s ist festgenagelt!",
	L"%s feuert mehr Schüsse als beabsichtigt!",
	L"Sie können keine Miliz in diesem Sektor ausbilden.",
	L"Miliz hebt %s auf.",
	L"Wenn Feinde im Sektor sind können Sie keine Miliz ausbilden!",
	// 6 - 10
	L"%s hat nicht genug Führungsqualität um Milizen auszubilden.",	
	L"Pro Sektor sind nicht mehr als %d Milizausbilder erlaubt.",	
	L"Kein Platz für mobile Milizen in oder rund um %s!",
	L"Sie benötigen %d Stadtmilizen in jedem von %ss befreiten Sektoren bevor Sie hier mobile Milizen ausbilden können.",
	L"Anlage nicht nutzbar wenn Feinde in der Gegend sind!",
	// 11 - 15
	L"%s hat nicht genügend Weisheit um diese Anlage betreiben zu können.",
	L"%s ist schon voll besetzt.",
	L"Diese Anlage zu betreiben kostet $%d pro Stunde. Weitermachen?",
	L"Sie haben nicht genug Geld um alle heutigen Betriebskosten zu zahlen. $%d wurden bezahlt, $%d fehlen noch. Die Einwohner sind nicht erfreut.",
	L"Sie haben nicht genug Geld um alle heutigen Betriebskosten zu zahlen. Es stehen $%d aus. Die Einwohner sind nicht erfreut.",
	// 16 - 20
	L"Sie haben eine ausstehende Forderung von $%d für Betriebskosen und kein Geld um zu bezahlen!",
	L"Sie haben eine ausstehende Forderung von $%d für Betriebskosen. Dieser Anlage können Sie keinen Söldner zuweisen bis Sie Ihre gesamten Schulden beglichen haben.",
	L"Sie haben eine ausstehende Forderung von $%d für Betriebskosen. Möchten Sie diese Schuld begleichen?",
	L"Nicht möglich in diesem Sektor",
	L"Tagesausgaben",
	// 21 - 25
	L"Nicht genug Geld für alle angeworbenen Milzen! %d Milzen wurden entlassen und sind heimgekehrt.",
	L"Um sich den Status eines Gegenstandes während des Kampfes anzuschauen, müssen Sie den Gegenstand vorher aufheben.", // HAM 5
	L"Um einen Gegenstand an einen anderen anbringen zu können, müssen Sie beide Gegenstände vorher aufheben.", // HAM 5
	L"Um zwei Gegenstände miteinander zu verbinden, müssen Sie beide Gegenstände vorher aufheben.", // HAM 5
};

// HEADROCK HAM 5: Text dealing exclusively with Item Transformations.
STR16 gzTransformationMessage[] = 
{
	L"Keine Umgestaltungsmöglichkeit vorhanden",
	L"%s wurde in mehrere Teile umgewandelt.",
	L"%s wurde in mehrere Teile umgewandelt. Prüfen Sie %s's Inventar für die daraus entstandenen Teile.",
	L"Aufgrund des fehlendes Platzes im Inventar nach der Umgestaltung wurden einige von %s's Gegenstände auf den Boden abgelegt.",
	L"%s wurde in mehrere Teile umgewandelt. Durch den Platzmangel im Inventar hat %s ein paar Gegenstände auf den Boden abgelegt.",
	L"Möchsten Sie alle %d Gegenstände im Stapel umwandeln? (Um nur einen Gegenstand umzuwandeln, entferenen Sie diesen zuerst vom Stapel)",
	// 6 - 10
	L"Aufteilen des Kisteninhaltes ins Inventar",
	L"Aufteilen in %d-Schuss Magazine",
	L"%s wurde aufgeteilt in %d Magazine, wobei jedes davon %d Schuss enthält.",
	L"%s wurde aufgeteilt in %s's Inventar.",
	L"Es ist nicht genügend Platz in %s's Inventar um die Magazine dieses Kalibers abzulegen!",
	L"Sofortmodus",
	L"Verzögerter Modus",
	L"Sofortmodus (%d AP)",
	L"Verzögerter Modus (%d AP)",
};

// WANNE: These are the email texts Speck sends when one of the 4 new 1.13 MERC mercs have levelled up
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Hiermit geben wir zur Kenntnis, dass aufgrund von Gastons guten Leistungen in der Vergangenheit sein Sold erhöht wurde. Ich persönlich bin darüber nicht überrascht. ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Bitte nehmen Sie zur Kenntnis, dass Stogies Bezüge für seine geleisteten Dienste mit sofortiger Wirkung erhöht werden in Anpassung an seine verbesserten Fähigkeiten. ± ± Speck T. Kline ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Bitte nehmen Sie zur Kenntnis, dass Tex aufgrund seiner Erfahrung Anspruch auf eine angemessenere Entlohnung hat. Seine Bezüge werden daher ab sofort seinem Wert entsprechend erhöht. ± ± Speck T. Kline ± ",
	// Biggins: Text from Line 49 in Email.edt
	L"Zur Kenntnisnahme. Aufgrund seiner verbesserten Leistungen wurden Colonel Biggins' Dienstbezüge erhöht. ± ± Speck T. Kline ± ",
};

// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"Weitergeleitet von AIM-Server: Nachricht von Victor Kolesnikov",
	L"Vielen Dank für Nachricht auf Anrufbeantworter. Nun stehe ich zur Verfügung. ± ± Allerdings bin ich wählerisch mit meine Komamandanten. Ich werde mich noch über Sie erkundigen.± ± V.K. ±",

	// Brain
	L"Weitergeleitet von AIM-Server: Nachricht von Janno Allik",
	L"Jetzt wäre ich bereit für einen Auftrag. Sie wissen schon, alles zu seiner Zeit. ± ± Janno Allik ±",

	// Scream
	L"Weitergeleitet von AIM-Server: Nachricht von Lennart Vilde",
	L"Vielen Dank für Ihre Kontaktaufnahme. Sagen Sie mir Bescheid, wenn die nächste Party steigen kann. Ab sofort erreichen Sie mich über die AIM page. ± ± Lennart Vilde.",

	// Henning
	L"Weitergeleitet von AIM-Server: Nachricht von Henning von Branitz",
	L"Ihre Nachricht hat mich erreicht, vielen Dank. Falls Sie mich engagieren möchten, kontaktieren Sie mich über die AIM Website. ± ± Bis die Tage! ± ± Henning von Branitz ±",

	// Luc
	L"Weitergeleitet von AIM-Server: Nachricht von Luc Fabre",
	L"Ich habe Ihre Nachricht erhalten, merci. Zur Zeit könnte ich gerne einen Auftrag annehmen. Sie wissen ja, wo Sie mich erreichen. ± ± Sicher hören wir bald von einander. ±",

	// Laura
	L"Weitergeleitet von AIM-Server: Nachricht von Dr. Laura Colin",
	L"Ich grüße Sie! Schön, dass Sie mir eine Nachricht hinterlassen haben. Es hörte sich interessant an. ± ± Wenn Sie wieder bei AIM vorbeischauen, würde ich mich freuen, von Ihnen zu hören. ± ± Noch viel Erfolg! ± ± Dr. Laura Colin ±",

	// Grace
	L"Weitergeleitet von AIM-Server: Nachricht von Graziella Girelli",
	L"Sie wollten mich kontaktieren, aber ich war leider nicht zu erreichen.± ± Ein Familientreffen. Sie kennen das ja sicher... Jetzt hab' ich erst mal wieder genug von Familie.± ± Jedenfalls freue ich mich, wenn Sie sich auf der AIM Site mit mir in Verbindung setzen. Ciao! ±",

	// Rudolf
	L"Weitergeleitet von AIM-Server: Nachricht von Rudolf Steiger",
	L"Wissen Sie eigentlich, wieviel Anrufe ich jeden Tag kriege? Jeder Pisser meint, er müsste hier anrufen.  ± ± Aber gut, ich bin jetzt wieder da. Falls Sie einen interessanten Auftrag haben. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"Weitergeleitet von AIM-Server: Nachricht über Söldner Verfügbarkeit",
	L"Ich habe Ihre Nachricht erhalten und warte auf Ihren Rückruf. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper	
	L"Scharfschütze: Sie haben Augen wie ein Falke. Dadurch können sie sogar auf die Flügel einer Fliege aus hunderten von Metern schießen! ± ",
	// Camouflage	
	L"Tarnung: Neben Ihnen schauen sogar Büsche synthetisch aus. ± ",	
	// Ranger
	L"Jäger: Sie haben eine bemerkenswerte Affinität zu schwer passierbarem Gelände und Ihre unermüdlichen Beine tragen Sie im Handumdrehen über Stock und Stein. ± ",	
	// Gunslinger
	L"Revolverheld: Sie beweisen enormes Talent im Umgang mit Pistolen und Revolvern aller Art. John Wayne lässt grüßen. ± ",
	// Squadleader
	L"Zugführer: Ihre Rhetorik hat uns ganz schön beeindruckt und Ihre generelle Erscheinung motiviert einfach. In Ihrer Nähe kann eigentlich nichts schiefgehen. ± ",
	// Technician
	L"Ingenieur: Sie können mit Hufeisen und altem Garn so gut wie alles reparieren, MacGyver würde vor Neid erblassen. ± ",
	// Doctor
	L"Arzt: Ärzte wie Sie braucht das Land! Sie können Kranke heilen wie ein junger Jesus. ± ",
	// Athletics
	L"Läufer: So schnell und ausdauernd wie Sie rennen, möchte ich annehmen, Sie sind mit dem Wort Marathon vertraut. Einholen wird Sie bestimmt keiner. ± ",
	// Bodybuilding
	L"Kraftsportler: Arnie? Was für ein Weichei! Sie könnten ihn selbst mit einer gebrochenen Hand zu Boden befördern. ± ",
	// Demolitions
	L"Sprengmeister: Nutzen Sie Ihre Begeisterung für alles, was mit mehrfacher Schallgeschwindigkeit expandiert um sich im Training mit Granaten und Sprengstoffen hervorzutun. ± ",
	// Scouting
	L"Aufklärer: Sie sind über die Maßen aufmerksam, haben ein sehr reges Auge und einen nimmermüden Geist. ± ",
	// Covert ops
	L"Geheimagent: Neben Ihnen schaut 007 wie der reinste Amateur aus! ± ",
	// Radio Operator
	L"Radio Operator: Your usage of communication devices broaden your team's tactical and strategic skills. ± ",	// TOO.Translate
	// Survival
	L"Survival: Nature is a second home to you. ± ",	// TODO.Translate
};

STR16 NewInvMessage[] = 
{
	L"Rucksack kann zur Zeit nicht aufgehoben werden",
	L"Kein Platz zum Ablegen des Rucksacks",
	L"Rucksack nicht gefunden",
	L"Reißverschluss funktioniert nur im Kampf",
	L"Bewegung nicht möglich, während Reißverschluss des Rucksacks offen ist",
	L"Sind Sie sicher, dass Sie alle Gegenstände im Sektor verkaufen wollen?",
	L"Sind Sie sicher, dass Sie alle Gegenstände im Sektor löschen wollen?",
	L"Kann nicht beim Tragen eines Rucksacks klettern",
	L"All backpacks dropped",	// TODO.Translate
	L"All owned backpacks picked up",
	L"%s drops backpack",
	L"%s picks up backpack",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Initialisiere RakNet Server...",
	L"Server gestartet, warte auf Client-Verbindungen...",
	L"Sie müssen sich nun als Client durch drücken von '2' mit dem Server verbinden.",
	L"Server läuft bereits.",
	L"Starten des Servers ist fehlgeschlagen. Abbruch.",
	// 5	
	L"%d/%d Clients sind bereit für Echtzeitmodus.",
	L"Verbindung zum Server ist unterbrochen, wird heruntergefahren.",
	L"Server läuft nicht.",
	L"Clients sind noch am laden, bitte warten...",
	L"Sie können die Absprungzone nicht ändern, wenn der Server bereits gestartet wurde.",
	// 10
	L"Datei '%S' gesendet - 100/100",
	L"Alle Dateien wurden an '%S' gesendet.",
	L"Starte mit dem versenden der Dateien an '%S'.",
	L"Verwenden Sie die Anzeige für die Absprungzone wenn Sie den Startsektor ändern möchten. Änderungen sind nur möglich, bevor Sie auf 'Starte Spiel' geklickt haben.",
};

STR16 MPClientMessage[] =
{
	// 0
	L"Initialisiere RakNet Client...",
	L"Verbinde zur ausgewählten Server-IP...",
	L"Erhalte Spieleinstellungen:",
	L"Sie sind bereits verbunden.",
	L"Sie verbinden sich bereits...",
	// 5
	L"Client #%d - '%S' hat '%s' angeheuert.",
	L"Client #%d - '%S' hat einen weiteren Söldner angeheuert.",
	L"Sie sind bereit - Gesamt bereit = %d/%d",
	L"Sie sind nicht mehr bereit - Gesamt bereit = %d/%d",
	L"Starte Gefecht...",
	// 10
	L"Client #%d - '%S' ist bereit - Gesamt bereit = %d/%d",
	L"Client #%d - '%S' ist nicht mehr bereit - Gesamt bereit = %d/%d",
	L"Sie sind bereit. Warte auf die anderen Clients... Drücken Sie 'OK' wenn Sie doch noch nicht bereit sind.",
	L"Lass uns das Gefecht beginnen!",
	L"Ein Client muss laufen, um das Spiel beginnen zu können.",		//LOOTF - Hintergrund? Wenn kein Client aktiv ist, gibt es doch auch niemanden, der eine Aufforderung zum Spielstart setzt? oO
	// 15
	L"Spiel kann nicht gestartet werden. Es sind noch keine Söldner angeheuert.",
	L"Erwarte Freigabe vom Server für den Laptop...",
	L"Unterbrochen",
	L"Unterbrechung beendet",
	L"Maus-Raster-Koordinaten:",
	// 20
	L"X: %d, Y: %d",
	L"Raster-Nummer: %d",
	L"Aktion kann nur der Server durchführen.",
	L"Wähle exklusive Server-Aktion:  ('1' - Laptop freischalten/anheuern)  ('2' - Gefecht starten/Sektor laden)  ('3' - Interface freischalten )  ('4' - Söldner Platzierung abschließen) ",
	L"Sektor=%s, Max. Clients=%d, Teamgröße=%d, Spieltyp=%d, Gleiche Söldner=%d, Schaden-Mult.=%f, Rundenzeitbeschr.=%d, Seks/Tik=%d, Kein Bobby Ray=%d, Keine Aim/Merc-Ausrüstung=%d, Keine Moral=%d, Testen=%d",		//LOOTF - Was ist Seks/Tik? Lol. Sextick. Finde gut. Englisch = Secs/Tic, aber das sagt mir auch nix.
	// 25
	L"Testmodus und Cheat-Funktion mit '9' ist freigeschaltet.",
	L"Neue Verbindung: Client #%d - '%S'.",
	L"Team: %d.",
	L"'%s' (Client #%d - '%S') wurde getötet von '%s' (Client #%d - '%S')",
	L"Werfe Client #%d - '%S' aus dem Spiel.",
	// 30
	L"Starte neuen Spielzug für gewählten Client. #1: <Abbrechen>, #2: %S, #3: %S, #4: %S",
	L"Starte Spielzug für Client #%d",
	L"Anfrage auf Echtzeit-Modus...",
	L"In Echtzeit-Modus gewechselt.",
	L"Fehler: Es ist ein Fehler beim Zurückwechseln in den Echtzeit-Modus aufgetreten",
	// 35
	L"Laptop freischalten um Söldner anzuheuern? (Sind alle Clients bereits verbunden?)",
	L"Server hat den Laptop freigeschaltet. Söldner anheuern!",
	L"Unterbrechung.",
	L"Sie können die Absprungzone nicht ändern, wenn Sie nur der Client und nicht zusätzlich der Server sind.",
	L"Sie haben das Angebot zur Kampfaufgabe abgelehnt.",
	// 40
	L"Alle Ihre Söldner wurden getötet!",
	L"Überwachungsmodus wurde eingeschaltet.",
	L"Sie wurden besiegt!",
	L"Auf Dächer klettern ist in einem Mehrspieler-Spiel nicht erlaubt.",
	L"Sie haben '%s' angeheuert.",
	// 45
	L"Sie können den Sektor nicht ändern, wenn bereits Einkäufe begonnen haben",
	L"Sektor gewechselt zu '%s'",
	L"Verbindung zu Client '%s' abgebrochen, entferne Client vom Spiel.",
	L"Ihre Verbindung zum Spiel wurde unterbrochen, gehe zurück zum Hauptmenü.",
	L"Verbindung fehlgeschlagen. Wiederholung des Verbindungsaufbaus in 5 Sekunden. %i Versuche übrig...",
	//50
	L"Verbindung fehlgeschlagen, Abbruch...",
	L"Sie können das Spiel nicht beginnen, solange sich noch kein weiterer Spieler verbunden hat.",
	L"%s : %s",
	L"Sende an alle",
	L"Nur Verbündete",
	// 55
	L"Spielbeitritt nicht möglich. Das Spiel ist bereits gestartet.",
	L"%s (Team): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Alle Dateien vom Server erhalten.",
	L"'%S' hat alle Dateien vom Server heruntergeladen.",
	L"'%S' startet mit dem Download der Dateien.",
	L"Starten des Spiels ist nicht möglich solange die Clients noch nicht alle Dateien erhalten haben.",
	L"Dieser Server erfordert, dass sie modifizierte Dateien für das Spiel herunterladen. Möchten Sie fortfahren?",
	// 65
	L"Drücken Sie 'Bereit' um in den taktischen Bildschirm zu gelangen.",
	L"Kann keine Verbindung herstellen, weil Ihre Version %S unterschiedlich zur Server Version %S ist.",
	L"Sie haben einen gegnerischen Soldaten getötet.",
	L"Spiel kann nicht gestartet werden, weil es keine unterschiedlichen Teams gibt.",
	L"Die Spieleinstellungen erfordern Neues Inventar (NIV), aber NIV ist aufgrund der Spielauflösung nicht verwendbar.",
	// 70
	L"Kann erhaltene Datei '%S' nicht speichern",
	L"%s's Sprengstoff wurde von %s entschärft",
	L"Sie haben verloren. Was für eine Schande",	// All over red rover
	L"Überwachungsmodus wurde ausgeschaltet",
	L"Wählen Sie den Client, der gekickt werden soll. #1: <Abbruch>, #2: %S, #3: %S, #4: %S",
	// 75
	L"Team %s wurde vernichtet",
	L"Client konnte nicht gestartet werden. Beendigung.",
	L"Client Verbindung aufgelöst und heruntergefahren.",
	L"Client läuft nicht.",
	L"INFO: Falls das Spiel hängen bleibt (die Statusanzeige beim Gegnerischen Zug bewegt sich nicht), informieren Sie den Server, dass dieser ALT + E drücken soll, um Ihnen den Spielzug wieder zu geben!",
	// 80
	L"Gegnerischer Spielzug - %d übrig",
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"O",
	L"S",
	L"W",
	L"M",	// "C"enter
};

STR16 gszMPTeamNames[] =
{
	L"Foxtrot",
	L"Bravo",
	L"Delta",
	L"Charlie",
	L"n.a.",		// Acronym of Not Applicable
};

STR16 gszMPMapscreenText[] =
{
	L"Spieltyp: ",
	L"Spieler: ",
	L"Teamgröße: ",
	L"Sie können die Startpositionen nicht mehr ändern, sobald der Laptop freigeschaltet ist.",
	L"Sie können die Teams nicht mehr ändern, sobald der Laptop freigeschaltet ist.",
	L"Zuf. Söldner: ",
	L"J",
	L"Schwierigkeit:",
	L"Server Version:",
};

STR16 gzMPSScreenText[] =
{
	L"Kampfstatistik",
	L"Weiter",
	L"Abbrechen",
	L"Spieler",
	L"Tötungen",
	L"Tote",
	L"Gegnerische Armee",
	L"Treffer",
	L"Fehlschüsse",
	L"Treffgenauigkeit",
	L"Schaden verursacht",
	L"Schaden erhalten",
	L"Bitte warten Sie bis der Server auf 'Weiter' geklickt hat."
};

STR16 gzMPCScreenText[] =
{
	L"Abbrechen",
	L"Verbindungsaufbau zum Server",
	L"Erhalte Server Einstellungen",
	L"Herunterladen von Dateien",
	L"Drücke 'ESC' zum Verlassen oder 'Y' zum Chatten",
	L"Drücke 'ESC' zum Verlassen",
	L"Fertig"
};

STR16 gzMPChatToggleText[] =
{
	L"Sende an alle",
	L"Sende nur an Verbündete",
};

STR16 gzMPChatboxText[] =
{
	L"Mehrspieler Chat",
	L"Senden mit 'ENTER', Abbrechen mit 'ESC'",
};

// Following strings added - SANDRO
// Translated by Scheinworld
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"Auf der nächsten Seite können Sie Ihre Fertigkeiten entsprechend Ihrer Spezialisierung als Söldner festlegen. Es können nicht mehr als zwei verschiedene Fertigkeiten oder eine Expertenfertigkeit gewählt werden.",
	L"Sie können auch nur eine oder gar keine Fertigkeit auswählen. Sie erhalten dafür einen Bonus zu Ihren Attributpunkten als Gegenleistung. Beachten Sie, dass die Fertigkeiten 'Elektronik', 'Beidhändig geschickt' und 'Getarnt' keine Experten-Spezialisierung erhalten.",
	// For new major/minor traits
	L"Auf der nächsten Seite können Sie Ihre Fertigkeiten entsprechend Ihrer Spezialisierung festlegen. Auf der ersten Seite können Sie bis zu %d Hauptfertigkeiten auswählen, die Ihre Rolle in einem Team repräsentieren, während Sie auf der zweiten Seite eine Liste der möglichen Nebenfertigkeiten finden.",
	L"Es können nicht mehr als insgesamt %d Fertigkeiten gewählt werden. Wenn Sie keine Hauptfertigkeiten nutzen wollen, können Sie dafür %d Nebenfertigkeiten wählen. Selektieren Sie zwei Hauptfertigkeiten (oder eine Spezialisierung), ist/sind noch %d Nebenfertigkeit(en) wählbar.",
};

STR16 sgAttributeSelectionText[] =
{
	L"Bitte verteilen Sie nun Ihre Bonuspunkte auf die gewünschten Attribute. Der Wert kann dabei nicht höher sein als",
	L"B.S.E. Eigenschaften und Fähigkeiten.",
	L"Bonus Pkt.:",
	L"Anfangs-Level",
	// New strings for new traits - SANDRO
	L"Im nächsten Schritt können Sie Ihre Attribute und Fähigkeiten festlegen. Attribute sind Gesundheit, Geschicklichkeit, Beweglichkeit, Stärke und Weisheit. Sie können nicht weniger als %d Punkte verteilen.",
	L"Der Rest sind Ihre Fähigkeiten, die Sie auch auf 0 setzen können.",
	L"Beachten Sie, dass bestimmte Attribute auf spezifische Minimalwerte gesetzt werden, die den Voraussetzungen der Fertigkeiten entsprechen. Sie können diese Werte nicht weiter senken.",	
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"B.S.E. Charakter-Analyse",
	L"Die Charakter-Analyse ist der nächste Schritt bei der Erstellung Ihres Profils. Auf der nun folgenden Seite steht eine Vielzahl von Charaktereigenschaften zur Auswahl. Wir können uns vorstellen, dass Sie sich mit mehreren verbunden fühlen, entscheiden Sie sich daher bitte für die zutreffendste. ",
	L"Die zweite Seite dient der Erfassung Ihrer Unzulänglichkeiten, die Sie möglicherweise haben (wir glauben, dass jeder Mensch nur eine große Schwäche hat). Bitte seien Sie dabei ehrlich, damit potentielle Arbeitgeber über Ihr zukünftiges Einsatzfeld informiert werden können.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Neutral",
	L"Freundlich",
	L"Einzelgänger",
	L"Optimist",
	L"Pessimist",
	L"Aggressiv",
	L"Arrogant",
	L"Bonze",
	L"Arschloch",
	L"Feigling",
	L"B.S.E. - Persönlichkeit",
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Neutral",
	L"Umgänglich",		//LOOTF - alt. "Extrovertiert"
	L"Einzelgängerisch",
	L"Optimistisch",
	L"Selbstsicher",
	L"Lernbegeistert",
	L"Primitiv",
	L"Aggressiv",
	L"Phlegmatisch",
	L"Tollkühn",
	L"Pazifistisch",
	L"Sadistisch",
	L"Machohaft",		//LOOTF - alt. "Angeberisch"
	L"Feigling",
	L"B.S.E. - Charakterzüge",
};

STR16 gzIMPColorChoosingText[] =
{
	L"Erscheinung und Äußeres",
	L"Hautfarbe",
	L"Bitte geben Sie Ihre Haar- und Hautfarbe, Ihre Statur, sowie Ihre bevorzugten Kleidungsfarben an.",
	L"Bitte geben Sie Ihre Haar- und Hautfarbe, sowie Ihre bevorzugten Kleidungsfarben an.",
	L"Eingeschaltet, wird ein Gewehr einhändig abgefeuert.",
	L"\n(Warnung: Sie werden eine Menge Stärke dafür benötigen.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Haarfarbe",
	L"Hautfarbe",
	L"Hemdfarbe",
	L"Hosenfarbe",
	L"Normale Statur",
	L"Kräftige Statur",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Keine Schwäche",
	L"Hitzeempfindlichkeit",
	L"Nervosität",
	L"Klaustrophobie",
	L"Nichtschwimmer",
	L"Angst vor Insekten",
	L"Vergesslichkeit",
	L"Psychopath",
	L"Schwerhörigkeit",
	L"Kurzsichtigkeit",
	L"Bluter",
	L"Höhenangst"
	L"Selbstverletzend",
	L"Ihre größte Schwäche",
};

STR16 gzIMPDisabilityTraitEmailTextDeaf[] =
{
	L"Sie sind bestimmt froh, dass wir ihnen das hier nicht auf die Mailbox sprechen.",
	L"Sie haben entweder in ihrer Jugend zuviele Diskos besucht, oder zu viele Bombardierungen von nahem erlebt. Oder sie sind einfach alt. Ihr Team sollte jedenfalls Gebärdensprache lernen.",
};

STR16 gzIMPDisabilityTraitEmailTextShortSighted[] =
{
	L"Ohne ihre Brille sind sie aufgeschmissen.",
	L"Das passiert wenn man dauernd nur vor der Glotze rumhängt. Sie hätten mehr Karotten essen sollen. Schon mal einen Hasen mit Brille gesehen? Aha.",
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

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s hat nicht genug Kraft um diese Aufgabe zu erledigen.",
	L"%s ist nicht geschickt genug um diese Aufgabe zu erledigen.",
	L"%s ist nicht beweglich genug um diese Aufgabe zu erledigen.",
	L"%s hat keine ausreichende Gesundheit um diese Aufgabe zu erledigen.",
	L"%s mangelt es an ausreichender Weisheit um diese Aufgabe zu erledigen.",
	L"%s mangelt es an ausreichender Treffsicherheit um diese Aufgabe zu erledigen.",
	// 6 - 10
	L"%s hat nicht genug Medizinkenntnis um diese Aufgabe zu erledigen.",
	L"%s hat zu wenig technisches Verständnis um diese Aufgabe zu erledigen.",
	L"%s mangelt es an ausreichender Führungsqualität um diese Aufgabe zu erledigen.",
	L"%s hat nicht genug Sprengstoffkenntnis um diese Aufgabe zu erledigen.",
	L"%s mangelt es an ausreichender Erfahrung um diese Aufgabe zu erledigen.",
	// 11 - 15
	L"%s hat nicht genug Moral um diese Aufgabe zu erledigen.",
	L"%s ist zu erschöpft um diese Aufgabe zu erledigen.",
	L"Zu wenig Loyalität in %s. Die Einwohner lassen Sie diese Aufgabe nicht verrichten.",
	L"Es arbeiten bereits zu viele Personen in %s.",
	L"Zu viele Personen verrichten diese Aufgabe schon in %s.",
	// 16 - 20
	L"%s findet nichts mehr zum Reparieren.",
	L"%s verliert %s beim Arbeiten in %s.",
	L"%s hat ein paar %s verloren beim Arbeiten in der %s in %s !",
	L"%s wurde verletzt beim Arbeiten in Sektor %s und benötigt dringend medizinische Versorgung!",
	L"%s wurde verletzt beim Arbeiten in der %s in %s und benötigt dringend medizinische Versorgung!",
	// 21 - 25
	L"%s wurde verletzt beim Arbeiten in Sektor %s. Es scheint aber nichts Ernstes zu sein.",
	L"%s wurde verletzt beim Arbeiten in der %s in %s. Es scheint aber nichts Ernstes zu sein.",
	L"Die Einwohner von %s scheinen sich über die Anwesenheit von %s aufzuregen."
	L"Die Einwohner von %s scheinen sich über die Arbeit von %s in der %s aufzuregen."
	L"%ss Handeln im Sektor %s hat einen Loyalitätsverlust in der gesamten Region bewirkt!",
	// 26 - 30
	L"%ss Handeln in der %s in %s hat einen Loyalitätsverlust in der gesamten Region bewirkt!",
	L"%s ist betrunken.", // <--- This is a log message string.
	L"%s ist ernsthaft krank geworden in Sektor %s und wurde vom Dienst freigestellt.",
	L"%s ist ernsthaft krank geworden und kann keine seine Arbeiten in der %s in %s fortsetzen.",
	L"%s wurde verletzt in Sektor %s.", // <--- This is a log message string.
	// 31 - 35
	L"%s wurde ernsthaft im Sektor %s verletzt.", //<--- This is a log message string.
	L"There are currently prisoners here who are aware of %s's identity.", // TODO.Translate
	L"%s is currently well known as a mercenary snitch. Wait at least %d more hours.", // TODO.Translate


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Kraft",
	L"Beweglichkeit", 
	L"Geschicklichkeit",
	L"Weisheit", 
	L"Gesundheit",
	L"Treffsicherheit",
	// 5-10
	L"Führungsqualität",
	L"Technik",
	L"Medizin",
	L"Sprengstoffe",
};

STR16 gzFacilityAssignmentStrings[]=
{

	L"UMGEBUNG",
	L"Betrieb",
	L"Essen",
	L"Pause",
	L"Repariere Gegenstände",
	L"Repariere %s",
	L"Repariere Roboter",
	// 6-10
	L"Arzt",
	L"Patient",
	L"Üben Kraft",
	L"Üben Geschicklichkeit",
	L"Üben Beweglichkeit",
	L"Üben Gesundheit",
	// 11-15
	L"Üben Treffsicherheit",
	L"Üben Medizin",
	L"Üben Technik",
	L"Üben Führungsqualität",
	L"Üben Sprengstoff",
	// 16-20
	L"Rekrut Kraft",
	L"Rekrut Geschicklichkeit",
	L"Rekrut Beweglichkeit",
	L"Rekrut Gesundheit",
	L"Rekrut Treffsicherheit",
	// 21-25
	L"Rekrut Medizin",
	L"Rekrut Technik",
	L"Rekrut Führungsqualität.",
	L"Rekrut Sprengstoff",
	L"Trainer Kraft",
	// 26-30
	L"Trainer Geschicklichkeit",
	L"Trainer Beweglichkeit",
	L"Trainer Gesundheit",
	L"Trainer Treffsicherheit",
	L"Trainer Medizin",
	// 30-35
	L"Trainer Technik",
	L"Trainer Führungsqualität",
	L"Trainer Sprengstoff",
	L"Gefangene verhören",	// added by Flugente
	L"Undercover Snitch",	// TODO.Translate
	// 36-40
	L"Spread Propaganda",
	L"Spread Propaganda",	// spread propaganda (globally)
	L"Gather Rumours",
	L"Command Militia",	// militia movement orders
};

STR16 Additional113Text[]=
{
	L"Für die korrekte Arbeit im Fenster-Modus benötigt Jagged Alliance 2 v1.13 16-bit Farbmodus.", //Jagged Alliance 2 v1.13 windowed mode requires a color depth of 16bpp or less.
	L"Jagged Alliance 2 v1.13 fullscreen mode (%d x %d) is not supported by your primary screen.\nPlease either change the game resolution or use 16bpp windowed mode.",	// TODO.Translate
	L"Interner Fehler beim Auslesen der %s Slots des zu ladenden Spielstandes: Die Anzahl der Slots im Spielstand (%d) unterscheidet sich mit den definierten Slots in der Datei ja2_options.ini (%d)",
	// WANNE: Savegame slots validation against INI file
	L"Söldner (MAX_NUMBER_PLAYER_MERCS) / Fahrzeuge (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Gegner (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Monster (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Miliz (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Zivilisten (MAX_NUMBER_CIVS_IN_TACTICAL)",
};

// SANDRO - Taunts (here for now, xml for future, I hope)
STR16 sEnemyTauntsFireGun[]=
{
	L"Friss Blei!",
	L"Duck dich!",
	L"Komm und hol mich!",
	L"Du gehörst mir!",
	L"Stirb!",
	L"Zeit zu sterben.",
	L"Vorsicht, Kugel!",
	L"Komm her, du Mistkerl!",
	L"Rrraaaaaah!",
	L"Komm zu Papa.",
	L"Du kommst unter die Erde.",
	L"Du kommst hier nicht lebend raus!",
	L"Aufs Maul!",
	L"Du hättest daheim bleiben sollen!",
	L"Stirb doch!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"Wird Zeit den Ballermann aus dem Sack zu holen.",
	L"Überraschung.",
	L"Beenden wir das schmerzhaft.",
	L"Bitte lächeln!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Hier, fang!",
	L"Die ist für dich.",
	L"Spiel doch damit!",
	L"Hab da was fallen gelassen.",
	L"Hahaha.",
	L"Viel Spaß damit!",
	L"Hrrmmgh... uff!",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"Ich hol mir deinen Skalp.",
	L"Komma her, du.",
	L"Zeig mir dein Innerstes.",
	L"Ich schneid dich in Streifen.",
	L"Hjaaaaah!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"Wir sitzen in der Scheiße! Raus hier!",
	L"Komm zur Armee, haben sie gesagt... Tze!",
	L"Aufklärung erfolgreich, Feind überlegen, weg hier!",
	L"Ohmeingottohmeingottohmeingott.",
	L"Ganze Kampfgruppe - Rückzug!",
	L"Rückzug! Alle Mann Rückzug!",
	L"Weg von hier, die machen uns platt!",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"Ich hab da was gehört.",
	L"Wer ist da? Ist da einer?",
	L"Was war das eben?",
	L"Paul, bist du das? Alles in Ordnung?",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Alarm! Feindkontakt!",
	L"Es geht los! Da sind sie!",
	L"Ich seh einen von ihnen hier drüben!",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Argh!",
	L"Hnngh!",
	L"Der saß...",
	L"Au! Du Penner!",
	L"Das wirst du bereuen!",
	L"Sanitäter!",
	L"Ich hab doch gar nichts gemacht!",
};

STR16 sEnemyTauntsNoticedMerc[]=
{
	L"Da'ffff...!",
	L"Oh mein Gott!",
	L"Heilige Scheiße!",
	L"Gegner!!!",
	L"Alarm! Alarm!",
	L"Hier ist einer!",
	L"Angriff!",
};


//////////////////////////////////////////////////////
// HEADROCK HAM 4: Begin new UDB texts and tooltips
//////////////////////////////////////////////////////
STR16 gzItemDescTabButtonText[] =
{
	L"Beschreibung",
	L"Allgemein",
	L"Erweitert",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Bes.",
	L"Allg.",
	L"Erw.",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Primär",
	L"Sekundär",
	L"AP Kosten",
	L"Feuerstoß/Autofeuer",
};

STR16 gzItemDescGenIndexes[] =
{
	L"Eigensch.",
	L"0",
	L"+/-",
	L"=",
};

STR16 gzUDBButtonTooltipText[]=
{
	L"|B|e|s|c|h|r|e|i|b|u|n|g:\n \nZeigt allgemeine Informationen über den Gegenstand.",
	L"|A|l|l|g|e|m|e|i|n|e |E|i|g|e|n|s|c|h|a|f|t|e|n:\n \nZeigt typische Daten über den Gegenstand.\n \nWaffen: Nochmals klicken um zweite Seite anzuzeigen.",
	L"|E|r|w|e|i|t|e|r|t|e |E|i|g|e|n|s|c|h|a|f|t|e|n:\n \nZeigt Vor-/Nachteile des Gegenstandes.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|P|r|i|m|ä|r|e |E|i|g|e|n|s|c|h|a|f|t|e|n:\n \nEigenschaften und Daten in Bezug auf die Gegenstandsklasse\n(Waffen / Rüstungen / usw.).",
	L"|S|e|k|u|n|d|ä|r|e |E|i|g|e|n|s|c|h|a|f|t|e|n:\n \nZusätzliche Eigenschaften des Gegenstands,\nund/oder mögliche sekundäre Fähigkeiten.",
	L"|A|P |K|o|s|t|e|n:\n \nDiverse AP Kosten in Bezug auf Abfeuern\noder Handhabung der Waffe.",
	L"|F|e|u|e|r|s|t|o|ß|/|A|u|t|o|f|e|u|e|r |E|i|g|e|n|s|c|h|a|f|t|e|n:\n \nMit dem Abfeuern dieser Waffe verbundene Daten für\nFeuerstoß-/Autofeuermodus.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|E|i|g|e|n|s|c|h|a|f|t |S|y|m|b|o|l\n \nMaus-darüber um den Namen der Eigenschaft zu erfahren.",
	L"|G|r|u|n|d|w|e|r|t\n \nDer normale Wert des Gegenstandes ausschließlich aller\nVor-/Nachteile von Erweiterungen oder Munition.",
	L"|E|r|w|e|i|t|e|r|u|n|g|s|b|o|n|u|s\n \nVor-/Nachteile von Munition, Erweiterungen, \noder schlechtem Zustand des Gegenstandes.",
	L"|E|n|d|w|e|r|t\n \nDer endgültige Wert des Gegenstandes, einschließlich aller \nVor-/Nachteile von Erweiterungen oder Munition.",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"Eigenschaft Symbol (Maus-darüber zeigt den Namen).",
	L"Vor-/Nachteil wenn |s|t|e|h|e|n|d.",
	L"Vor-/Nachteil wenn |h|o|c|k|e|n|d.",
	L"Vor-/Nachteil wenn |l|i|e|g|e|n|d.",
	L"Gegebener Vor-/Nachteil",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|G|e|n|a|u|i|g|k|e|i|t",
	L"|S|c|h|a|d|e|n",
	L"|R|e|i|c|h|w|e|i|t|e",
	L"|H|a|n|d|h|a|b|u|n|g|s|p|r|o|b|l|e|m|a|t|i|k",
	L"|E|r|l|a|u|b|t|e |Z|i|e|l|s|t|u|f|e|n",
	L"|V|e|r|g|r|ö|ß|e|r|u|n|g|s|f|a|k|t|o|r",
	L"|P|r|o|j|e|k|t|i|o|n|s|f|a|k|t|o|r",
	L"|U|n|t|e|r|b|u|n|d|e|n|e|s M|ü|n|d|u|n|g|s|f|e|u|e|r",
	L"|L|a|u|t|s|t|ä|r|k|e",
	L"|Z|u|v|e|r|l|ä|s|s|i|g|k|e|i|t",
	L"|R|e|p|a|r|i|e|r|f|ä|h|i|g|k|e|i|t",
	L"|M|i|n|d|e|s|t |R|e|i|c|h|w|e|i|t|e |f|ü|r |Z|i|e|l|v|o|r|t|e|i|l",
	L"|T|r|e|f|f|e|r |M|o|d|i|f|i|k|a|t|o|r",	
	L"|A|P|s |f|ü|r |A|n|l|e|g|e|n",
	L"|A|P|s |f|ü|r |S|c|h|u|s|s",
	L"|A|P|s |f|ü|r |Fe|u|e|r|s|t|o|ß ",
	L"|A|P|s |f|ü|r |A|u|t|o|f|e|u|e|r",
	L"|A|P|s |f|ü|r |N|a|c|h|l|a|d|e|n",
	L"|A|P|s |f|ü|r |R|e|p|e|t|i|e|r|e|n",
	L"",	// No longer used!
	L"|G|e|s|a|m|t|e|r| |R|ü|c|k|s|t|o|ß",
	L"|A|u|t|o|f|e|u|e|r |p|r|o |5 |A|P|s",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nBestimmt ob Kugeln, welche von dieser Waffe gefeuert werden, vom\nZiel abweichen.\n \nMaßstab: 0-100.\nHöher ist besser.",
	L"\n \nBestimmt den durchschnittlichen Schaden den von dieser Waffe gefeuerte Kugeln machen,\nbevor Berücksichtigung von Rüstung oder Rüstungsdurchdringen.\n \nHöher ist besser.",
	L"\n \nDie gößte Entfernung (in Felder) die von dieser Waffe gefeuerte Kugel\nzurücklegen, bevor sie zu Boden fallen.\n \nHöher ist besser.",
	L"\n \nBestimmt die Schwierigkeit für das Halten und Feuern der Waffe.\nEin höhere Wert resultiert in einer niedrigeren Trefferwahrscheinlichkeit.\n \nNiedriger ist besser.",
	L"\n \nDas ist die Anzahl von extra Ziellevel welche Sie erhalten,\nwenn Sie mit der Waffe zielen.\n \nJe weniger Ziellevel erlaubt sind desto mehr\nLevel erhalten Sie. Deshalb, weniger Level zu haben,\nmacht die Waffe schneller ohne an Genauigkeit\nzu verlieren.\n \nNiedriger ist besser.",
	L"\n \nWenn größer als 1.0, werden Zielfehler\nproportional zur Entfernung reduziert.\n \nZur Erinnernung hohe Zielfernrohrvergrößerungen sind schädlich wenn das Ziel zu nahe ist!\n \n Der Wert von 1.0 bedeutet kein Zielfernrohr wird benutzt.",
	L"\n \nReduziert Zielfehler proportional zur Entfernung.\n \nDieser Effekt wirkt bis zu einer gegebenen Entfernung,\ndann löst er sich langsam auf und verschwindet evtl. bei ausreichender Entfernung.\n \nHöher ist besser.",
	L"\n \nWenn diese Eigenschaft in Kraft ist, dann produziert die Waffe kein sichtbares Mündungsfeuer\nwenn abgefeuert.\n \nFeinde werden Sie nicht bloß beim Mündungsfeuer ausfindig machen können\n(aber Sie können Sie dennoch hören).",
	L"\n \nBestimmt die Entfernung (in Felder) der erzeugten Lautstärke,\nwenn die Waffe geschossen wird.\n \nFeinde innerhalb dieser Entfernung hören den Schuss, Feinde außerhalb nicht.\n \nNiedriger ist besser.",
	L"\n \nBestimmt, wie schnell sich diese Waffe bei Gebrauch abnutzt.\n \nHöher ist besser.",
	L"\n \nBestimmt, wie schwierig es ist, die Waffe\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur Techniker und spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
	L"\n \nDie minimale Entfernung um einen Zielvorteil zu erhalten.",
	L"\n \nTreffermodifikator den eine Laservorrichtung gewährleistet.",	
	L"\n \nDie Anzahl von APs nötig um die Waffe anzulegen.\n \nSobald die Waffe angelegt ist, können Sie wiederholt feuern ohne diese Kosten erneut zu bezahlen.\n \nEine Waffe ist automatisch abgelegt wenn der Anwender irgendeine andere Aktivität ausübt,\nmit der Ausnahme von schießen oder ausrichten.\n \nNiedriger ist besser.",
	L"\n \nDie Anzahl von APs nötig um einen einzelnen Angriff mit dieser Waffe durchzuführen.\n \nFür Schusswaffen ist dies der Aufwand für einen Einzelschuss ohne extra Zielen.\n \nWenn das Symbol 'grau' erscheint sind Einzelschüsse nicht möglich.\n \nNiedriger ist besser.",
	L"\n \nDie Anzahl von APs die für einen Feuerstoß benötigt werden.\n \nDie Anzahl der Geschosse welche mit jedem Feuerstoß abgefeuert werden hängt von der Waffe selbst ab,\nund ist angedeutet bei der Anzahl der Kugeln neben dem Symbol.\n \nWenn das Symbol 'grau' erscheint ist ein Feuerstoss nicht möglich.\n \nNiedriger ist besser.",
	L"\n \nDie Anzahl von APs die für eine Autofeuer Salve von genau 3 Kugeln benötigt werden.\nWenn das Symbol 'grau' erscheint ist Autofeuer nicht möglich.\n \nNiedriger ist besser.",
	L"\n \nDie Anzahl von APs die für das Nachladen benötigt werden.\n \nNiedriger ist besser.",
	L"\n \nDie Anzahl von APs die für das repetieren der Waffe benötigt werden.\n \nNiedriger ist besser.",
	L"",	// No longer used!
	L"\n \nDie absolute Reichweite mit der sich das Mündungsfeuer\nausbreitet für jede Kugel die geschossen wird,\nwenn keine Gegenmaßnahme angewendet wird.\n \nNiedriger ist besser.", // HEADROCK HAM 5: Altered to reflect unified number.
	L"\n \nZeigt die Anzahl der Kugeln an, welche zu einer Autofeuer Salve für jeweils 5 investierte AP addiert werden.\n \nHöher ist besser.",
	L"\n \nBestimmt, wie schwierig es ist, die Waffe\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|S|c|h|u|t|z |W|e|r|t",
	L"|F|l|ä|c|h|e|n|d|e|c|k|u|n|g",
	L"|Z|e|r|f|a|l|l |R|a|t|e",
	L"|R|e|p|a|r|i|e|r|f|ä|h|i|g|k|e|i|t",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nDiese grundlegende Rüstungseigenschaft bestimmt wie viel\nSchaden abgefangen wird.\nZur Erinnerung: Schutzdurchschlagende Angriffe und einige\nzufällige Faktoren können die Schadensreduzierung verändern.\n \nHöher ist besser.",
	L"\n \nBestimmt wie viel des geschützten\nKörperteils durch die Rüstung abgedeckt wird.\n \nWenn weniger als 100% verdeckt wird, haben Angriffe\neine gewisse Chance die Rüstung schlichtweg\nzu umgehen, und höchsten Schaden\nauf das verdeckte Körperteil auszuüben.\n \nHöher ist besser.",
	L"\n \nBestimmt wie schnell der Zustand der Rüstung abfällt,\nwenn sie getroffen wird, im Verhältnis zum\nSchaden durch einen Angriff.\n \nNiedriger ist besser.",
	L"\n \nBestimmt, wie schwierig es ist, die Rüstung\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur Techniker und spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
	L"\n \nBestimmt, wie schwierig es ist, die Rüstung\nzu reparieren und wer sie vollständig reparieren kann.\n \ngrün = Jeder kann sie reparieren.\n \ngelb = Nur spezielle NPCs können\nsie über die Reparaturschwelle hinaus reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|R|ü|s|t|u|n|g|s|d|u|r|c|h|s|c|h|l|a|g",
	L"|K|u|g|e|l|s|t|u|r|z",
	L"|E|x|p|l|o|s|i|o|n |v|o|r |E|i|n|s|c|h|l|a|g",
	L"|T|e|m|p|e|r|a|t|u|r |M|o|d|i|f|i|k|a|t|o|r",
	L"|G|i|f|t|-|I|n|d|i|k|a|t|o|r",
	L"|S|c|h|m|u|t|z |M|o|d|i|f|i|k|a|t|o|r",
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nDas ist die Fähigkeit der Kugel, in die Rüstung\neines Ziels einzudringen.\n \nWenn der Wert kleiner als 1.0 ist, reduziert die Kugel \nverhältnismäßig den Schutz jeglicher Rüstung auf die sie eintrifft.\n \nIst der Wert grösser als 1.0, tritt die Kugel weniger tief in die Rüstung des Ziels ein.\n \nKleiner ist besser.",
	L"\n \nBestimmt eine verhältnismäßige Zunahme des Schadenspotentials,\nsobald die Kugel die Rüstung des Ziels\ndurchbricht und den Körper dahinter trifft.\n \nWerte über 1.0 erhöhen, Werte unter 1.0 reduzieren das Schadenspotential\nder durchbrochenen Kugel.\n \nHöher ist besser.",
	L"\n \nEin Multiplikator zum Schadenspotential der Kugel,\nder vor dem Treffen des Zieles angewandt wird.\n \nWerte über 1.0 erhöhen, Werte unter 1.0 reduzieren den Schaden.\n \nHöher ist besser.",
	L"\n \nProzentuale zusätzliche Hitze\ndurch diese Munitionsart.\n \nNiedriger ist besser.",
	L"\n \nGibt die Anzahl in Prozent an,\nob der eingetretene Schaden einer Kugel auch eine Vergiftung verursacht.",
	L"\n \nZusätzlicher Schmutz der entsteht durch diese Munition.\n \nNiedriger ist besser.",
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|S|c|h|a|d|e|n",
	L"|B|e|t|ä|u|b|u|n|g|s|s|c|h|a|d|e|n",
	L"|E|x|p|l|o|d|i|e|r|t |b|e|i|m |A|u|f|p|r|a|l|l",		// HEADROCK HAM 5
	L"|R|a|d|i|u|s|-|D|r|u|c|k|w|e|l|l|e",
	L"|R|a|d|i|u|s|-|B|e|t|ä|u|b|u|n|g|s|s|c|h|a|d|e|n",
	L"|R|a|d|i|u|s|-|G|e|r|ä|u|s|c|h",
	L"|S|t|a|r|t|r|a|d|i|u|s|-|T|r|ä|n|e|n|g|a|s",
	L"|S|t|a|r|t|r|a|d|i|u|s|-|S|e|n|f|g|a|s",
	L"|S|t|a|r|t|r|a|d|i|u|s|-|L|i|c|h|t",
	L"|S|t|a|r|t|r|a|d|i|u|s|-|R|a|u|c|h",
	L"|S|t|a|r|t|r|a|d|i|u|s|-|F|e|u|e|r",
	L"|E|n|d|r|a|d|i|u|s|-|T|r|ä|n|e|n|g|a|s",
	L"|E|n|d|r|a|d|i|u|s|-|S|e|n|f|g|a|s",
	L"|E|n|d|r|a|d|i|u|s|-|L|i|c|h|t",
	L"|E|n|d|r|a|d|i|u|s|-|R|a|u|c|h",
	L"|E|n|d|r|a|d|i|u|s|-|F|e|u|e|r ",
	L"|Z|e|i|t|d|a|u|e|r",
	// HEADROCK HAM 5: Fragmentation
	L"|A|n|z|a|h|l |a|n |F|r|a|g|m|e|n|t|e|n",
	L"|S|c|h|a|d|e|n |p|r|o |F|r|a|g|m|e|n|t",
	L"|F|r|a|g|m|e|n|t |R|e|i|c|h|w|e|i|t|e",
	// HEADROCK HAM 5: End Fragmentations
	L"|L|a|u|t|s|t|ä|r|k|e",
	L"|U|n|b|e|s|t|ä|n|d|i|g|k|e|i|t",
	L"|R|e|p|a|r|i|e|r|f|ä|h|i|g|k|e|i|t",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nDer Schaden der durch diesen Sprengstoff\nverursacht wird.\n \nAnmerkung: Sprengstoffe die in einer Druckwelle explodieren liefern\nnur einmal Schaden (dann wenn Sie explodieren), während anhaltend wirkende\nSprengstoffe rundenübergreifend Schaden bis die Wirkung nachlässt.\n \nHöher ist besser.",
	L"\n \nDer Betäubungschaden (nicht tödlich) der durch diesen\nSprengstoff verursacht wird.\n \nAnmerkung: Sprengstoffe die in einer Druckwelle explodieren liefern\nnur einmal Schaden (dann wenn Sie explodieren), während anhaltend wirkende\nSprengstoffe rundenübergreifend Schaden bis die Wirkung nachlässt.\n \nHöher ist besser.",
	L"\n \nDieser Sprengstoff wird sobald er ein Hindernis\ntrifft explodieren (und nicht vorher noch abprallen).",	// HEADROCK HAM 5
	L"\n \nDas ist der Radius der Explosionswelle den dieser\nSprengstoff hervorruft.\n \nZiele werden weniger Schaden erleiden desto weiter entfernt\nsie von der Mitte der Explosion sind.\n \nHöher ist besser.",
	L"\n \nDas ist der Radius des Betäubungsschlags den dieser\nSprengstoff hervorruft.\n \nZiele werden weniger Schaden erleiden desto weiter entfernt\nsie von der Mitte der Explosion sind.\n \nHöher ist besser.",
	L"\n \nDie Entfernung die das Geräusch der Explosion\nzurücklegen wird. Soldaten innerhalb der Entfernung \nsind fähig das Geräusch zu hören und werden gewarnt.\n \nHöher ist besser.",
	L"\n \nDas ist der Startradius des Tränengas\nder durch diesen Sprengstoff freigesetzt wird.\n \nFeinde die innerhalb des Radius erfasst werden erleiden\nden angegebenen Betäubungsschaden jeden Zug,\nbis Sie Gasmasken tragen.\n \nAchte auf Endradius und Dauer der Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Startradius des Senfgas\nder durch diesen Sprengstoff freigesetzt wird.\n \nFeinde die innerhalb des Radius erfasst werden erleiden\nden angegebenen Schaden jeden Zug,\nbis Sie Gasmasken tragen.\n \nAchte auf Endradius und Dauer der Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Startradius des Lichts\nder durch den Sprengstoff freigesetzt wird.\n \nFelder in der Nähe der Wirkung leuchten \nsehr hell, Felder nahe zum Rand\nsind nur ein weniger heller als normal.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nZur Erinnerung: Im Unterschied zu anderen Sprengstoffen mit\nzietlich festgelegter Wirkung, wird die Wirkung des Lichts nach einiger\nZeit weniger, bis es verschwindet.\n \nHöher ist besser.",
	L"\n \nDas ist der Startradius des Rauchs\nder durch diesen Sprengstoff freigesetzt wird.\n \nJeder innerhalb der Rauchwolke wird sehr schwer zu erkennen,\nund verliert ein ganzes Stück Sichtweite.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Startradius der Flammen\ndie durch den Sprengstoff freigesetzt werden.\n \nFeinde die innerhalb des Radius erfasst werden erleiden\nden angegebenen Schaden jeden Zug.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Endradius den das Tränengas durch den\nSprengstoff vor dem Verschwinden freisetzt.\n \nFeinde die innerhalb des Radius erfasst werden erleiden\nden angegebenen Betäubungsschaden jeden Zug,\nbis Sie Gasmasken tragen.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Endradius den das Senfgas durch den\nSprengstoff vor dem Verschwinden freisetzt.\n \nFeinde die innerhalb des Radius erfasst werden erleiden\nden angegebenen Schaden jeden Zug,\nbis Sie Gasmasken tragen.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Endradius des Lichts der durch\nden Sprengstoff freigesetzt wird bevor er verschwindet.\n \nFelder in der Nähe der Wirkung leuchten \nsehr hell, Felder nah zum Rand\nsind nur ein weniger heller als normal.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nZur Erinnerung: Im Unterschied zu anderen Sprengstoffen mit\nzietlich festgelegter Wirkung, wird die Wirkung des Lichts nach einiger\nZeit weniger, bis es verschwindet.\n \nHöher ist besser.",
	L"\n \nDas ist der Endradius den das Rauchs durch den\nSprengstoff vor dem Verschwinden freisetzt.\n \nJeder innerhalb der Rauchwolke ist sehr schwer zu erkennen,\nund verliert ein ganzes Stück Sichtweite.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist der Endradius den die Flammen dieses\nSprengstoffs einnehmen, bevor sie verschwinden.\n \nFeinde, die innerhalb des Radius erfasst werden erleiden\nden angegebenen Schaden jede Runde.\n \nAchte auf Endradius und Dauer\nder Wirkung.\n \nHöher ist besser.",
	L"\n \nDas ist die Wirkungsdauer des Sprengstoffs.\n \nJeden Zug wird der Wirkungsradius wachsen, \nein Feld in jede Richtung, bis\nder angegebene Endradius erreicht ist.\n \nWird die maximale Dauer erreicht, verschwindet\ndie Wirkung vollständig.\n \nLicht freigesetzt durch Sprengstoffe\nnimmt ab, im Unterschied zu anderen Wirkungen.\n \nHöher ist besser.",
	// HEADROCK HAM 5: Fragmentation
	L"\n \nDies ist die Anzahl an Fragementen die von der\nExplosion ausgestoßen werden.\n \nDie Fragmente verhalten sich ähnlich wie Kugeln und können jeden treffen\nder sich nahe genug an der Explosion aufhält.\n \nHöher ist besser.",
	L"\n \nDer potenzielle Schaden welcher durch die\nExplosion der ausgestoßenen Fragemente verursacht wird.\n \nHöher ist besser.",
	L"\n \nDas ist die durchschnittliche Reichweite welche\ndie Fragmente einer Explosion fliegen können.\n \nEinige dieser Fragmente können jedoch weiter oder weniger weit fliegen.\n \nHöher ist besser.",
	// HEADROCK HAM 5: End Fragmentations
	L"\n \nDie Reichweite in Feldern\nin der Feinde und Söldner die Explosion wahrnehmen.\n \nFeinde die die Explosion hören werden alarmiert.\n \nNiedriger ist besser.",
	L"\n \nDieser Wert (außerhalb von 100) stellt eine Möglichkeit für den\nSprengstoff dar, spontan zu explodieren wenn er Schaden nimmt\n(z.B. durch Explosionen in der Nähe).\n \nDas Mitführen empfindlicher Sprengstoffe innerhalb des Kampfs\nist deshalb extrem riskant und sollte vermieden werden.\n \nSkala: 0-100.\nNiedriger ist besser.",
	L"\n \nBestimmt, wie schwierig es ist, diesen Sprengsatz zu reparieren.\n \ngrün = Jeder kann ihn reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
};

STR16 szUDBGenCommonStatsTooltipText[]=
{
	L"|R|e|p|a|r|i|e|r|f|ä|h|i|g|k|e|i|t",
	L"|V|e|r|f|ü|g|b|a|r|e|r |P|l|a|t|z",
	L"|P|l|a|t|z|b|e|d|a|r|f",
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nBestimmt, wie schwierig es ist, diesen Gegenstand zu reparieren.\n \ngrün = Jeder kann ihn reparieren.\n \nrot = Dieser Gegenstand kann nicht repariert werden.\n \nHöher ist besser.",
	L"\n \nBestimmt, wieviel Platz dieser MOLLE Träger für Taschen bietet.\n \nHöher ist besser.",
	L"\n \nBestimmt, wieviel Platz diese Tasche auf einem MOLLE Träger einnimmt.\n \nNiedriger ist besser.",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|L|e|u|c|h|t|s|p|u|r|m|u|n|i|t|i|o|n",
	L"|A|n|t|i|-|P|a|n|z|e|r M|u|n|i|t|i|o|n",
	L"|I|g|n|o|r|i|e|r|t |S|c|h|u|t|z",
	L"|S|ä|u|r|e|h|a|l|t|i|g|e |M|u|n|i|t|i|o|n",
	L"|T|ü|r|z|e|r|s|c|h|l|a|g|e|n|d|e |M|u|n|i|t|i|o|n",
	L"|W|i|d|e|r|s|t|a|n|d|s|f|ä|h|i|g|k|e|i|t |g|e|g|e|n |S|p|r|e|n|g|s|t|o|f|f|e",
	L"|W|a|s|s|e|r|s|i|c|h|e|r",
	L"|E|le|k|t|r|o|n|i|k",
	L"|G|a|s|m|a|s|k|e",
	L"|B|e|n|ö|t|i|g|t |B|a|t|t|e|r|i|e|n",
	L"|K|a|n|n |S|c|h|l|ö|s|s|e|r |K|n|a|c|k|e|n",
	L"|K|a|n|n |D|r|a|h|t |d|u|r|c|h|t|r|e|n|n|e|n",
	L"|K|a|n|n |S|c|h|l|ö|s|s|e|r |z|e|r|b|r|e|c|h|e|n",
	L"|M|e|t|a|l|l|d|e|t|e|k|t|o|r",
	L"|F|e|r|n|a|u|s|l|ö|s|e|r",
	L"|F|e|r|n|z|ü|n|d|e|r",
	L"|Z|e|i|t|z|ü|n|d|e|r",
	L"|E|n|t|h|ä|l|t |B|e|n|z|i|n",
	L"|W|e|r|k|z|e|u|g|k|a|s|t|e|n",
	L"|W|ä|r|m|e|s|i|c|h|t",
	L"|R|ö|n|t|g|e|n|g|e|r|ä|t",
	L"|E|n|t|h|ä|l|t |D|r|i|n|k|w|a|s|s|e|r",
	L"|E|n|t|h|ä|l|t |A|l|k|o|h|o|l",
	L"|E|r|s|t|e|-|H|i|l|f|e|-|K|a|s|t|e|n",
	L"|A|r|z|t|t|a|s|c|h|e",
	L"|T|ü|r|s|p|r|e|n|g|s|a|t|z",
	L"|W|a|s|s|e|r",
	L"|N|a|h|r|u|n|g",
	L"|M|u|n|i|t|i|o|n|s|g|u|r|t",
	L"|M|u|n|i|t|i|o|n|s|w|e|s|t|e",
	L"|E|n|t|s|c|h|ä|r|f|u|n|g|s|a|u|s|r|ü|s|t|u|n|g",
	L"|V|e|r|d|e|c|k|t|e |G|e|g|e|n|s|t|a|n|d",
	L"|K|a|n|n |n|i|c|h|t |b|e|s|c|h|ä|d|i|g|t |w|e|r|d|e|n",
	L"|M|a|d|e |o|f |M|e|t|a|l",
	L"|S|i|n|k|s",
	L"|T|w|o|-|H|a|n|d|e|d",
	L"|B|l|o|c|k|s |I|r|o|n |S|i|g|h|t|s",
	L"|A|n|t|i|-|M|a|t|e|r|i|a|l |A|m|m|o",	// TODO.Translate
	L"|F|a|c|e |P|r|o|t|e|c|t|i|o|n",
	L"|I|n|f|e|c|t|i|o|n |P|r|o|t|e|c|t|i|o|n",	// 39
	L"|S|c|h|i||l|d",
	L"|K|a|m|e|r|a",
	L"|B|u|r|i|a|l |M|o|d|i|f|i|e|r",
	L"|B|l|u|t|b|e|u|t|e|l |(|l|e|e|r|)",
	L"|B|l|u|t|b|e|u|t|e|l",						// 44
	L"|W|i|d|e|r|s|t|a|n|d|s|f|ä|h|i|g|k|e|i|t |g|e|g|e|n |F|e|u|e|r",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |M|o|d|i|f|i|e|r",
	L"|H|a|c|k|i|n|g |M|o|d|i|f|i|e|r",
	L"|M|e|d|i|c|a|l |S|p|l|i|n|t", // TODO.Translate
	L"|F|i|r|e |R|e|t|a|r|d|a|n|t |A|m|m|o",				// 49	TODO.Translate
	L"|I|n|c|e|n|d|i|a|r|y |A|m|m|o",
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \nDiese Munition lässt eine Leuchtwirkung entstehen wenn sie abgefeuert wird.\n \nLeuchtfeuer hilft Salven genauer zu kontrollieren.\n \nLeuchtpatronen geben die Position bei Tag/Nacht preis\n \nund deaktivieren Gegenstände\ndie Mündungsfeuer unterdrücken.",
	L"\n \nDiese Munition kann einen Panzer beschädigen.\n \nMunition ohne diese Eigenschaft bewirkt keinen Schaden\nan Panzern.",
	L"\n \nDiese Munition ignoriert die Rüstung vollständig.\n \nDas bewirkt das eine geschützte Person\nbehandelt wird als hätte sie keine Rüstung!",
	L"\n \nWenn diese Munition auf ein Ziel trifft,\nwird dessen Schutz dadurch schnell verschlechtert.\n \nMöglicherweise wird ein Ziel dadurch durch seinen eigenen Schutz getrennt!",
	L"\n \nDiese Munitionstyp ist ausnahmslos im zerstören von Schlössern.\n \nDirekt auf eine verschlossene Tür oder einen Behälter\ngefeuert richtet er enormen Schaden an.",
	L"\n \nDieser Schutz ist dreifach resistent\ngegen Sprengstoffe als es durch seinen gegebenen\nSchutzwert ersichtlich ist.\n \nWenn ein Sprengstoff den Schutz trifft, wird sein Schutzwert verdreifach.",
	L"\n \nDieser Gegenstand ist gegen Wasser immun. Nicht aber,\nwenn er Schaden nimmt und untertaucht.\n \nGegenstände ohne diese Eigenschaft verschlechtern sich allmählich,\nwenn die Person sie zum Schwimmen mitnimmt.",
	L"\n \nDieser Gegenstand ist Elektronik pur und beinhaltet\nkomplexe Schaltungen.\n \nElektronische Gegenstände sind von Natur aus schwer\nzu reparieren noch schwieriger ohne elektronische Fähigkeiten.",
	L"\n \nWenn dieser Gegenstand am Gesicht der Person getragen wird,\nwird Sie von allen schädlichen Gasen beschützt.\n \nEinige Gase sind ätzend und können sich\ndurch die Gasmaske fressen!",
	L"\n \nDieser Gegenstand benötigt Batterien. Ohne Batterien,\nkönnen seine primären Fähigkeiten nicht aktiviert werden.\n \nUm seine Fähigkeiten zu aktivieren\nbefestige Batterien an ihm.",
	L"\n \nDieser Gegenstand kann kann dazu benutzt werden verschlossene\nTüren oder Behälter zu öffnen.\n \nSchlösser knacken ist leise, doch es benötigt\nreichlich technische Fähigkeiten \nauch für die einfachsten Schlösser. Dieser Gegenstand ändert\ndie Fähigkeit Schlösser zu knacken um ", //JMich_SkillsModifiers: needs to be followed by a number
	L"\n \nDieser Gegenstand kann dazu benutzt werden Zäune zu zertrennen.\n \nDas ermöglicht einer Person schnell in abgezäuntes Gebiet\neinzudringen und den Feind möglicherweise zu überraschen!",
	L"\n \nDieser Gegenstand kann kann zum zerschlagen von verschlossenen\nTüren oder Behälter benutzt werden.\n \nDas Zerschlagen von Schlössern benötigt reichlich Stärke,\nerzeugt eine Menge Lärm und bringt\neine Person schnell zur Ermüdung. Jedoch ist eine guter\nWeg hinter verschlossenes zu gelagen ohne\nhöhere Fertigkeiten oder\nkomplizierte Werkzeuge zu besitzen. Dieser Gegenstand steigert \ndie Wahrscheinlichkeit um ", //JMich_SkillsModifiers: needs to be followed by a number
	L"\n \nDieser Gegenstand kann metallische Objekte\nim Boden aufspüren.\n \nSeine Funktion ist das aufspüren\nvon Minen ohne die nötigen Fähigkeiten zu besitzen diese\nmit blosen Auge zu erkennen.\n \nVielleicht finden Sie aber auch vergrabene Schätze.",
	L"\n \nDieser Gegenstand kann dazu benutzt werden eine Bombe\nwelche mit einem Fernzünder versehen wurde zu zünden.\n \nZu erst setzt die Bombe, dann benutze den\nFernauslöer um sie zu zünden wenn\ndie richtige Zeit ist.",
	L"\n \nWenn an einer Sprengstoffeinheit angebracht und eingestellt\n kann der Zünder durch eine\n separate Fernsteuerung ausgelöst werden.\n \nFernzünder sind bestens geeignet um Fallen zu stellen,\nweil Sie erst dann explodieren wenn sie es sollen.\n \nDarüber hinaus hat man genüge Zeit in Deckung zu gehen!",
	L"\n \nWenn an einer Sprengstoffeinheit angebracht und eingestellt\n zählt der Zünder von der eingegeben\nMenge an Zeit nach unten und explodiert \nwenn die Zeit abgelaufen ist.\n \nZeitzünder sind billig und einfach zu Installieren,\naber sie müssen so eingestellt werden umd\nsich selbst rechtzeitig in Deckung zu bringen!",
	L"\n \nDieser Gegenstand enthält Benzin.\n \nEs könnte praktisch sein\nwenn man einen Benzintank findet.",
	L"\n \nDieser Gegenstand enthält verschiedene Gerätschaften die dazu\nbenutzt werden können andere Gegenstände zu reparieren.\n \nEin Werkzeugkasten wird dafür benötigt, wenn \neinem Söldner die Aufgabe Reparieren zugewiesen wird. Dieser Gegenstand ändert\ndie Effektivität der Reperatur um ", //JMich_SkillsModifiers: need to be followed by a number
	L"\n \nWenn ans Gesicht angebracht, bietet der Gegenstand die Möglichkeit\nPersonen durch Wände zu sehen,\ndank ihrer Wärmestrahlung.",
	L"\n \nDieses mächtige Gerät kann dazu benutzt werden\nnach Personen zu suchen.\n \nEs zeigt alle Personen innerhalb eines bestimmten Radius\nfür einen kurzen Zeitraum.\n \nVon Geschlechtsorganen fernhalten!",
	L"\n \nDieser Gegenstand enthält frisches Wasser.\nGebrauchen Sie es wenn Sie durstig sind.",
	L"\n \nDieser Gegenstand enthält Spirituosen, Alkohol, Schnaps\noder was immer Sie sich einbilden.\n \nMit Vorsicht zu genießen! Kein Alkohol am Steuer!\nKann ihrer Leber schaden!",
	L"\n \nDas ist ein Erste-Hilfe-Kasten der\nGegenstände enthält die einfache medizinische Hilfe bieten.\n \nEr kann dazu benutzt werden verwundetet Personen zu bandagieren\nund Blutungen zu stoppen.\n \nFür richtige Heilung, benutze eine Arzttasche\nund/oder reichlich Ruhe.",
	L"\n \nDas ist eine Arzttasche, die für\nOperationen und andere gravierende medizinische\nZwecke genutzt werden kann.\n \nEine Arzttasche wird dazu benötigt\neinem Söldner die Aufgabe Doktor zu geben.",
	L"\n \nDieser Gegenstand kann dazu benutzt werden verschlossene\nTüren/Behälter zu sprengen.\n \nExplosionsfertigkeit ist erforderlich um\nvorzeitige Detonationen zu vermeiden.\n \nSchlösser zu sprengen ist der einfache Weg um schnell\ndurch verschlossene Türen/Behälter zu kommen. Wie auch immer,\nes ist laut und für die meisten Personen gefährlich.",
	L"\n \nMan kann das trinken.",
	L"\n \nMan kann das essen.",
	L"\n \nEin externer Munitionsgurt für MGs.",
	L"\n \nIn diese Weste passen auch Munitionsgurte.",
	L"\n \nDieser Gegenstand verbessert die Chance Fallen zu entschärfen um ",
	L"\n \nDieser Gegenstand und alles was daran angebracht/enthalten ist\nwird von neugierigen Augen verborgen bleiben.",
	L"\n \nDieser Gegenstand kann nicht beschädigt werden.",
	L"\n \nDieser Gegenstand ist aus Metall.\nEr nimmt weniger Schaden als andere Gegenstände.",
	L"\n \nDieser Gegenstand versinkt in Wasser.",
	L"\n \nDieser Gegenstand muß mit beiden Händen benutzt werden.",
	L"\n \nDieser Gegenstand verhindert die Verwendung von Kimme und Korn.",
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
	L"|M|o|d|i|f|i|k|a|t|o|r|-|G|e|n|a|u|i|g|k|e|i|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|M|o|m|e|n|t|a|u|f|n|a|h|m|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|M|o|m|e|n|t|a|u|f|n|a|h|m|e|-|P|r|o|z|e|n|t|u|e|l|l",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|Z|i|e|l|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|Z|i|e|l|e|n|-|P|r|o|z|e|n|t|u|e|l|l",
	L"|M|o|d|i|f|i|k|a|t|o|r |- |E|r|l|a|u|b|t|e |Z|i|e|l|g|e|n|a|u|i|g|k|e|i|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|Z|i|e|l|k|a|p|a|z|i|t|ä|t",
	L"|M|o|d|i|f|i|k|a|t|o|r |- |W|a|f|f|e|n|h|a|n|d|h|a|b|u|n|g",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|b|s|e|n|k|u|n|g|s|k|o|m|p|e|n|s|i|e|r|u|n|g",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|Z|i|e|l|a|u|f|s|p|ü|r|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|c|h|a|d|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|M|e|s|s|e|r|s|c|h|a|d|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|E|n|t|f|e|r|nu|n|g",
	L"|F|a|k|t|o|r|-|V|e|r|g|ö|ß|e|r|u|n|g",
	L"|F|a|k|t|o|r|-|V|e|r|b|r|e|i|t|e|r|u|n|g",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|R|ü|c|k|s|t|o|ß|-|H|o|r|i|z|o|n|t|a|l",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|R|ü|c|k|s|t|o|ß |- |V|e|r|t|i|k|a|l",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|G|e|g|e|n|w|i|r|k|e|n|-|M|a|x|i|m|u|m",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|G|e|g|e|n|w|i|r|k|e|n|-|G|e|n|a|u|i|g|k|e|i|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|G|e|g|e|n|w|i|r|k|e|n|-|H|ä|u|f|i|g|k|e|i|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|T|o|t|a|l",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|A|n|l|e|g|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|E|i|n|z|e|l|s|c|h|u|s|s",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|F|e|u|e|r|s|t|o|ß",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|A|u|t|o|f|e|u|e|r",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|P|-|N|a|c|h|l|a|d|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|M|a|g|a|z|i|n|g|r|ö|ß|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|F|e|u|e|r|s|t|o|ß|g|r|ö|ß|e",
	L"|U|n|t|e|r|d|r|ü|c|k|t|e|s |M|ü|n|d|u|n|g|s|f|e|u|e|r",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|L|a|u|t|s|t|ä|r|k|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|G|e|g|e|n|s|t|a|n|d|g|r|ö|ß|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|Z|u|v|e|r|l|ä|s|s|i|g|k|e|i|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|T|a|r|n|u|n|g|-|W|a|l|d",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|T|a|r|n|u|n|g|-|S|t|a|d|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|T|a|r|n|u|n|g|-|W|ü|s|t|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|T|a|r|n|u|n|g|-|S|c|h|n|e|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|c|h|l|e|i|c|h|e|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|H|ö|r|w|e|i|t|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|i|c|h|t|w|e|i|t|e|-|A|l|l|g|e|m|e|i|n",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|i|c|h|t|w|e|i|t|e|-|N|a|c|h|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|i|c|h|t|w|e|i|t|e|-|T|a|g",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|i|c|h|t|w|e|i|t|e|-|H|e|l|l|e|s |L|i|c|h|t",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|S|i|c|h|t|w|e|i|t|e|-|U|n|t|e|r|g|r|u|n|d",
	L"|T|u|n|n|e|l|s|i|c|h|t",
	L"|G|e|g|e|n|w|i|r|k|e|n|-|M|a|x|i|m|u|m",
	L"|G|e|g|e|n|w|i|r|k|e|n|-|H|ä|u|f|i|g|k|e|i|t",
	L"|T|r|e|f|f|e|r|b|o|n|u|s",
	L"|Z|i|e|l|b|o|n|u|s",
	L"|E|r|z|e|u|g|t|e |H|i|t|z|e",
	L"|A|b|k|ü|h|l|f|a|k|t|o|r",
	L"|L|a|d|e|h|e|m|m|u|n|g|s|s|c|h|r|a|n|k|e",
	L"|H|i|t|z|e|s|c|h|a|d|e|n|s|s|c|h|r|a|n|k|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|E|r|z|e|u|g|t|e |H|i|t|z|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|A|b|k|ü|h|l|f|a|k|t|o|r",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|L|a|d|e|h|e|m|m|u|n|g|s|s|c|h|r|a|n|k|e",
	L"|M|o|d|i|f|i|k|a|t|o|r|-|H|i|t|z|e|s|c|h|a|d|e|n|s|s|c|h|r|a|n|k|e",
	L"|G|i|f|t|-|I|n|d|i|k|a|t|o|r",
	L"|S|c|h|m|u|t|z |M|o|d|i|f|i|k|a|t|o|r",
	L"|G|i|f|t|i|g|k|e|i|t",
	L"|N|a|h|r|u|n|g|s |P|u|n|k|t|e",
	L"|T|r|i|n|k |P|u|n|k|t|e",
	L"|P|o|r|t|i|o|n|s |G|r|ö|s|s|e",
	L"|M|o|r|a|l |M|o|d|i|f|i|k|a|t|o|r",
	L"|S|c|h|i|m|m|e|l |R|a|t|e",
	L"|B|e|s|t|e |L|a|s|e|r |R|e|i|c|h|w|e|i|t|e",
	L"|P|r|o|z|e|n|t|u|a|l|e|r |R|ü|c|k|s|t|o|ß |M|o|d|i|f|i|k|a|t|o|r",	// 65
	L"|F|a|n |t|h|e |H|a|m|m|e|r",							// TODO.Translate
	L"|B|a|r|r|e|l |C|o|n|f|i|g|u|r|a|t|i|o|n|s",			// TODO.Translate
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular. 
STR16 szUDBAdvStatsExplanationsTooltipText[] =
{
	L"\n \nWenn befestigt an einer Fernwaffe dieser Gegenstand verändert den Genauigkeitswert.\n \nEine erhöhte Genauigkeit lässt die Schusswaffe Ziele auf eine größere Entfernung öfter treffen,\nangenommen es wird auch gut gezielt.\n \nMaßstab: -100 bis +100.\nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Genauigkeit\nfür jede Art von Feuermodus für diese Fernwaffe um den angegebenen Wert.\n \nMaßstab: -100 bis +100.\nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Genauigkeit\nfür jede Art von Feuermodus für diese Fernwaffe um den angegebenen Prozentsatz mit Bezug auf die ursprünglichen Genauigkeit.\n \nMaßstab: -100 bis +100.\nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Genauigkeit,\nwelche die Waffe mit jedem zusätzlichen Punkt durch genaueres Zielen erhält um den genannten Wert.\n \nMaßstab: -100 bis +100.\nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Genauigkeit,\ndie die Waffe mit jedem zusätzlichen Punkt durch genaueres Zielen erhält um den genannten Prozentsatz der ursprünglichen Genauigkeit.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Anzahl der möglichen zusätzlichen Zielpunkte dieser Waffe. \n \nEine niedrigere Anzahl von möglichen zusätzlichen Zielpunkten bedeutet,\n daß jeder Zielpunkt proportional mehr Genauigkeit zum Schuss beiträgt.\n\nDies bedeutet, das weniger mögliche zusätzliche Zielpunkte ein schnelleres Zielen mit dieser Waffen erlauben, ohne die Genauigkeit zu reduzieren!\n \nNiedriger ist besser.",
	L"\n \nDieser Gengenstand verändert den Höchstwert für Genauigkeit des Schützen mit Fernwaffen\num den angegebenen Prozentsatz seiner ursprünglichen maximalen Genauigkeit.\n \nHöher ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe verändert dieser Gegenstand die Schwierigkeit der Handhabung der Waffe.\nEine bessere Handhabung macht die Waffe genauer zu feuern; mit oder ohne zusätzliches Zielen.\n Der Wert bezieht sich dabei auf den ursprüngliche Handhabungs-Wert der Waffe,\nwelcher höher ist für Gewehre und schwere Waffen,\nund niedriger bei Pistolen und leichten Waffen.\n \nNiedriger ist besser",
	L"\n \nDieser Gegenstand verändert die Schwierigkeit,\nmit einer Waffe auch jenseits ihrer effektiven Reichweite zu treffen.\n \nEin hoher Wert kann durchaus\ndie tatsächliche effektive Reichweite dieser Waffe\num einige Felder verlängern.\nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Schwierigkeit,\nein sich bewegendes Ziel zu treffen.\nEin hoher Wert kann es einfacher machen,\nsich bewegende Ziele auch in großer Entfernung zu treffen.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert den Schaden,\nwelchen diese Waffe anrichtet.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert den Schaden,\nden eine Handwaffe anrichtet, um den angegebenen Wert.\n \n Dies bezieht sich sowohl auf Klingen als auch auf Schlagwaffen.\n \nHöher ist besser",
	L"\n \nWenn befestigt an einer Fernwaffe verändert\ndieser Gegenstand die größte effektive Reichweite.\n \nDie größte effektive Reichweite bestimmt,\nwie weit eine Kugel fliegt, bevor sie schnell in Richtung Boden fällt.\n \nHöher ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe bietet\ndieser Gegenstand eine zusätzlich Zielvergrößerung an,\nwas Schüsse auf große Entfernung vergleichsweise einfacher macht.\n \nEin hoher Vergrößerungsfaktor wirkt sich allerdings nachteilig aus\nbei Zielen, die näher als die optimale Entfernung sind.\n\nHöher ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe blendet\ndieser Gegenstand einen Punkt über das Ziel ein,\nwas das Treffen deutlich vereinfacht.\n\nDieser Effekt ist nur bis zu einer bestimmten Entfernung nützlich, darüber hinaus vermindert und schließlich verschwindet der Effekt.\n \nHöher ist besser.",
	L"\n \nWenn befestigt an Feuerstoß oder Dauerfeuer fähigen Fernwaffen\nverändert dieser Gegenstand den horizontalen Rückstoß um den genannten Wert.\n\n Ein geringerer Rückstoß macht es einfacher, die Waffe im Dauerfeuer auf das Ziel gerichtet zu halten.\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an Feuerstoß oder Dauerfeuer fähigen Fernwaffen\nverändert dieser Gegenstand den vertikalen Rückstoß um den genannten Wert.\n\n Ein geringerer Rückstoß macht es einfacher, die Waffe im Dauerfeuer auf das Ziel gerichtet zu halten.\n \nNiedriger ist besser.",
	L"\n \nDieser Gegenstand verändert die Fähigkeit des Schützen,\nseine Waffe im Feuerstoß oder Dauerfeuer unter Kontrolle zu behalten.\n\n Ein hoher Wert kann auch körperlich schwachen Schützen helfen, Waffen mit starkem Rückstoß zu kontrollieren. \n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Fähigkeit des Schützen,\ndem Rückstoß der Waffe im Feuerstoß oder Dauerfeuer wirksam entgegenzuwirken.\n\n Ein hoher Wert erlaubt dem Schützen,\n die Waffe besser auf das Ziel auszurichten,\n was im Ergebnis die Genauigkeit von Feuerstößen erhöht. \n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Fähigkeit des Schützen,\ndie Kraft, welche er benötigt, um dem Rückstoß im Feuerstoß oder Dauerfeuer entgegenzuwirken, öfter anzupassen.\n\n Ein höherer Wert macht Feuerstöße generell genauer,\n insbesondere lange Feuerstöße,\nvorausgesetzt, der Schütze kann dem Rückstoß wirksam entgegenwirken. \n \nHöher ist besser.",
	L"\n \nDieser Gegenstand verändert die Anzahl der Aktionspunkte,\nwelche der Charakter zu Beginn jeder Runde bekommt. \n \nHöher ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe verändert dieser Gegenstand die Anzahl der Aktionspunkte,\ndie benötigt werden,\num die Waffe in Anschlag zu bringen.\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an irgendeiner Waffe verändert dieser Gegenstand die Anzahl der Aktionspunkte,\ndie für einen Angriff mit der Waffe benötigt werden.\n\n Dies bezieht sich auch auf Feuerstoß oder Dauerfeuer fähige Waffen.\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an Feuerstoß oder Dauerfeuer fähigen Fernwaffen\nverändert dieser Gegenstand die Anzahl der Aktionspunkte,\ndie für die Abgabe eines Feuerstoßes benötigt werden.\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an Feuerstoß oder Dauerfeuer fähigen Fernwaffen\nverändert dieser Gegenstand die Anzahl der Aktionspunkte,\ndie für das Schießen im Dauerfeuer Modus benötigt werden.\n\n Dies bezieht sich allerdings nicht auf die AP, die benötigt werden,\num zusätzliche Schüsse im Feuerstoß abzugeben. .\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe verändert dieser Gegenstand die Anzahl der Aktionspunkte,\ndie benötigt werden, um die Waffe zu laden.\n \nNiedriger ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe verändert dieser Gegenstand\ndie Magazinkapazität von Magazinen welche in die Waffe geladen werden können. \n \nHöher ist besser.",
	L"\n \nWenn befestigt an einer Fernwaffe verändert dieser Gegenstand die Anzahl der Kugeln,\ndie pro Feuerstoß abgegeben werden.\n\n Wenn die Waffe ursprünglich nicht für Feuerstöße angelegt war,\nkann ein positiver Faktor die Abgabe von Feuerstößen ermöglichen.\n\nIm Gegensatz kann eine Waffe mit einer solchen Funktion diese bei einem entsprechend negativem Faktor verlieren. \n \nHöher ist - gewöhnlich – besser. Allerdings ist Munitionssparen ein Grund für kontrollierte Feuerstöße.",
	L"\n \nAn die Waffe angebracht beeinflusst der Gegenstand\nein Verbergen des Mündungsfeuers.\n \nDadruch wird es schwieriger den Schützen zu entdecken.",
	L"\n \nAn die Waffe angebracht beeinflusst der Gegenstand\ndie Lautstärke der Waffe.\n \nNiedriger ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Größe des Gegenstands\nan den er angebracht ist.\n \nDie Größe des Gegenstands bestimmt wie oft und worin er\nbefördert werden kann.\n \nNiedriger ist besser.",
	L"\n \nAn die Waffe angebracht beeinflusst der Gegenstand\nden Zuverlässigkeitswert.\n \nEin positiver Wert bewirkt ein verlangsamte,\nein negativer Wert eine verschnellert Verschlechterung.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Tarnung\nin Waldgebieten, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Tarnung\nin Stadtgebieten, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Tarnung\nin Wüstenebieten, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Tarnung\nin Schneegebieten, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst das Schleichen\n, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Hörweite\num die angegebenen Prozent.\n \nEin positiver Bonus ermöglicht das Hören von Geräuschen\nauf größere Distanzen.\n \nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser generelle Sichtweitenmodifikator funktioniert bei allen Lichtverhältnissen.\nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Nacht-Sichtweitenmodifikator funktioniert nur\nwenn die Umgebungsbeleuchtung niedrig ist.\n \nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Tag-Sichtweitenmodifikator funktioniert nur\nwenn die Umgebungsbeleuchtung überdurchschnittlich hoch ist.\n \nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Helligkeits-Sichtweitenmodifikator funktioniert nur\nwenn die Umgebungsbeleuchtung extrem hoch und grell ist.\n \nHöher ist besser.",
	L"\n \nWenn dieser Gegenstand getragen, oder an einem getragenen\nGegenstand angebracht, verändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Höhlen-Sichtweitenmodifikator funktioniert nur\nim Dunkeln und auch nur unter Tage.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Sichtbreite,\nwenn er getragen oder\nan einen getragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDie Fähigkeit des Schützen Rückstoß\nwährend Feuerstoß-/Autofeuersalven zu bewältigen.\n \nHöher ist besser.",
	L"\n \nDie Fähigkeit des Schützen einzuschätzen\nwieviel Kraft er während Feuerstoß-/Autofeuersalven\nbenötigt um den Rückstoß auszugleichen.\n \nNiedriger ist besser.",
	L"\n \nDieser Gegenstand beeinflusst die Trefferchance\n, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nDieser Gegenstand beeinflusst den Zielbonus\n, wenn er getragen oder an einen\ngetragenen Gegenstand angebracht wird.\n \nHöher ist besser.",
	L"\n \nEin Schuss erzeugt soviel Hitze. Munitionstypen und Anbauten können diesen Wert verändern.\n \nNiedriger ist besser.",
	L"\n \nIn jedem Zug wird die Temperatur um diesen Wert gesenkt.\n \nHöher ist besser.",
	L"\n \nWenn die Temperatur diesen Wert überschreitet, kommt es leichter zu Ladehemmungen.\n \nHöher ist besser.",
	L"\n \nWenn die Temperatur diesen Wert überschreitet, wird der Gegenstand leichter beschädigt.\n \nHöher ist besser.",
	L"\n \nErzeugte Hitze wird um diesen Prozentsatz erhöht.\n \nNiedriger ist besser.",
	L"\n \nAbkühlung wird um diesen Prozentsatz erhöht.\n \nHöher ist besser.",
	L"\n \nLadehemmungsschranke wird um diesen Prozentsatz erhöht.\n \nHöher ist besser.",
	L"\n \nHitzeschadensschranke wird um diesen Prozentsatz erhöht.\n \nHöher is besser.",
	L"\n \nDies ist der prozentuelle Schaden der durch\nden vergifteten Gegenstand verursacht wird.\n\nDieser Schaden hängt natürlich davon ab,\noder der Feind eine Gift-Resistenz hat oder nicht.",
	L"\n \nEin einziger Schuss verursacht diesen Schmutz.\nUnterschiedliche Munition und Waffen-Anbauten\nkönnen diesen Wert verändern.\n \nNiedriger ist besser.",
	L"\n \nDies zu Essen verursacht soviel Vergiftung.\n \nNiedriger ist besser.",
	L"\n \nMenge an Energy in kcal.\n \nHöher ist besser.",
	L"\n \nWasseranteil in Litern.\n \nHöher ist besser.",
	L"\n \nProzentsatz des Gegenstandes der mit einem\n Bissen aufgebraucht wird.\n \nWeniger ist besser.",
	L"\n \nDie Moral wird um diese Zahl modifiziert.\n \nHöher ist besser.",
	L"\n \nDer Gegenstand wird mit der Zeit schlecht.\nIst mehr als 50% verschimmelt wird er giftig.\nDies ist die Rate in der Schimmel entsteht.\n \nNiedriger ist besser.",
	L"",
	L"\n \nWenn befestigt an Feuerstoß oder Dauerfeuer fähigen Fernwaffen\nverändert dieser Gegenstand den Rückstoß um den genannten Prozentwert.\n\n Ein geringerer Rückstoß macht es einfacher, die Waffe im Dauerfeuer auf das Ziel gerichtet zu halten.\n \nNiedriger ist besser.", // 65
	L"\n \nIf a gunslinger wields this gun two-handed,\nthey can burst in hipfire.",				// TODO.Translate
	L"\n \nToggling firemodes also toggles how many\nbarrels you can fire at the same time.",		// TODO.Translate
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \nDie Genauigkeit wurde verändert durch\nMunition, Erweiterungen oder inhärenter Fähigkeiten.\n \nErhöhte Genauigkeit erlaubt es Ziele die weiter entfernt sind\nöfter zu treffen, sofern \ngut gezielt wird.\n \nSkala: -100 bis +100.\nHöher ist besser.",
	L"\n \nDie Waffe verändert die Genauigkeit des Schützen\nmit jedem Schuss durch den angegebenen Wert.\n \nSkala: -100 bis +100.\nHöher ist besser.",
	L"\n \nDie Waffe verändert die Genauigkeit des Schützen\nmit jedem Schuss prozentual zur \nursprünglichen Genauigkeit des Schützen.\n \nHöher ist besser.",
	L"\n \nDie Waffe verändert den Betrag an Genauigkeit\ndurch jedes zusätzlich investierte Ziellevel.\n \nSkala: -100 bis +100.\nHöher ist besser.",
	L"\n \nDie Waffe verändert den Betrag an Genauigkeit\ndurch jedes zusätzlich investierte Ziellevel\nprozentual zur\nursprünglichen Genauigkeit des Schützen.\n \nHöher ist besser.",
	L"\n \nDie Anzahl an zusätzlich erlaubter Ziellevel\nfür diese Waffe wurden geändert durch Munition,\nErweiterungen oder inhärenter Fähigkeiten.\nWird die Anzahl an Ziellevel reduziert, is das Zielen mit\nder Waffe schneller bei gleicher Genauigkeit.\n \nGleiches gilt für die Erhöhung der Anzahl an Ziellevel.\n \nNiedriger ist besser.",
	L"\n \nDie Waffe ändert die maximale Genauigkeit des Schützen\nprozentual zu der ursprünglichen Genauigkeit des Schützen.\n \nHöher ist besser.",
	L"\n \nErweiterungen oder inhärente Fähigkeiten der Waffe\nverändern ihr Handhabung.\n \nLeichtere Handhabung lässt die Waffe genauer werden.\n \nNiedriger ist besser.",
	L"\n \nDie Fähigkeit Schüsse außerhalb\nder maximalen Reichweite zu gewährleisten wurde durch Erweiterungen\n oder inhärenter Fähigkeiten geändert.\n \nHöher ist besser.",
	L"\n \nDie Fähigkeit entfernte, bewegende Ziele zu treffen\nwurde durch Erweiterungen\noder inhärenter Fähigkeiten geändert.\n \nHöher ist besser.",
	L"\n \nDas Schadensergebnis wurde durch\nMunition oder Erweiterungen geändert.\n \nHöher ist besser.",
	L"\n \nDer Messerschaden wurde durch\nErweiterungen oder inhärente Fähigkeiten geändert.\n \nHöher ist besser.",
	L"\n \nDie maximale Reichweite wurde verbessert/verschlechtert durch Munition,\nErweiterungen oder inhärenter Fähigkeiten.\n \nHöher ist besser.",
	L"\n \nWurde durch optische Vergrößerungen ausgerüstet,\num entfernte Ziele leichter zu treffen.\n \nZielen mit hoher Vergrößerungen außerhalb der optimalen Entfernung ist schädlich.\n \nHöher ist besser.",
	L"\n \nMit einem Projektionsgerät ausgestattet\ndas einen Punkt (innerhalb der Reichweite) auf das Ziel projiziert\nund somit das Treffen erleichtert.\n \nHöher ist besser.",
	L"\n \nDer horizontale Rückstoß wurde geändert\ndurch Munition, Erweiterungen oder inhärenter\nFähigkeiten.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nEin reduzierter Rückstoß erleichtert es bei einer Salve die\nMündung auf das Ziel zu richten!\n \nNiedriger ist besser.",
	L"\n \nDer vertikale Rückstoß wurde geändert\ndurch Munition, Erweiterungen oder inhärenter\nFähigkeiten.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nEin reduzierter Rückstoß erleichtert es bei einer Salve die\nMündung auf das Ziel zu richten!.\n \nNiedriger ist besser.",
	L"\n \nÄndert die Fertigkeit des Schützen, Rückstoß bei\nFeuerstoß-/Autofeuer zu bewältigen.\n \nEin hoher Wert hilft dem Schützen seine Waffe bei starkem Rückstoß\nzu kontrollieren selbst dann, wenn er wenig Kraft besitzt.\n \nHöher ist besser.",
	L"\n \nÄndert die Fertigkeit des Schützen\nRückstöße genauer auszugleichen.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nEin hoher Wert hilft dem Schützen die Mündung der Waffe\nbei Salven genauer auf das Ziel zu richten.\n \nHöher ist besser.",
	L"\n \nÄndert die Fertigkeit des Schützen\neinzuschätzen wie viel Kraft zum ausgleichen des Rückstoßes benötigt wird.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nHöhere Frequenzen machen Salven im ganzen genauer,\nlängere Salven werden genauer sofern\nder Schütze den Rückstoß bewältigen kann.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, ändert die Waffe die Menge an AP\ndie der Soldat zu Anfang einer Runde bekommt.\n \nHöher ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurden die AP-Kosten zum Anlegen der Waffe\ngeändert.\n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurden die AP-Kosten für einen Einzelangriff\ngeändert.\n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurden die AP-Kosten für einen Feuerstoß\ngeändert.\n \nDies hat keine Wirkung bei Einzelschüssen!.\n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterung oder inhärenter Fähigkeiten,\nwurden die AP-Kosten für eine Salve geändert.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurden die AP-Kosten für Nachladen geändert.\n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurde die größe der fassbaren Magazine geändert.\n \nHöher ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurde die Anzahl der Patronen die abgefeuert werden\nkönnen geändert.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nentsteht kein Mündungsfeuer.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurde die Lautstärke geändert. \n \nNiedriger ist besser.",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurde die Waffengröße geändert. Die größe des Gegenstands bestimmt wie oft und worin er\nbefördert werden kann",
	L"\n \nDurch Munition, Erweiterungen oder inhärenter Fähigkeiten,\nwurde die Zuverlässigkeit geändert.\n \nEin positiver Wert bewirkt ein verlangsamte,\nein negativer Wert eine vergröbert Verschlechterung.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Tarnung des Soldaten in Waldgebieten.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Tarnung des Soldaten in Stadtgebieten.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Tarnung des Soldaten in Wüstengebieten.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Tarnung des Soldaten in Schneegebieten.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Fähigkeit des Soldaten sich leise zu bewegen.\n \nHöher ist besser.",
	L"\n \nWenn in der Hand gehalten, verändert sich\ndie Hörfähigkeit (in Felder) des Soldaten.\n \nHöher ist besser.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser allgemeine Modifikator wirkt unter allen Bedingungen.\n \nHöher ist besser.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Nachsicht-Modifikator wirk nur, wenn die Beleuchtung sehr niedrig ist.\n \nHigher is better.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Tagsicht-Modifikator wird nur, wenn die Beleuchtung durchchnittlich öder höher ist.\n \nHöher ist besser.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Hellighekits-Modifikator funktioniert nur, wenn die Beleuchtung extrem hell ist wie zum Beispiel bei Leuchtstäben.\n \nHöher ist besser.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite\num die angegebenen Prozent.\n \nDieser Höhlen-Modifikator wirkt nur im Dunkeln und unterirdisch.\n \nHöher ist besser.",
	L"\n \nWenn diese Waffe zum Schießen angelegt wird,\nverändert dies die Sichtweite.\n \nDie Seitensicht wird durch eine Art Tunnelblick eingegrenzt.\n \nHöher ist besser.",
	L"\n \nDas ist die Fähigkeit des Schützen Rückstoß\nwährend der Feuerstoß-/Autofeuersalven zu bewältigen.\n \nHöher ist besser.",
	L"\n \nDas ist die Fähigkeit des Schützen Rückstöße\ngenauer auszugleichen.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nEin hoher Wert hilft dem Schützen die Mündung der Waffe\nbei Salven genauer auf das Ziel zu richten.\n \nNiedriger ist besser.",
	L"\n \nDer Treffer Modifikator wird verändert durch\nMunition, Erweiterungen oder eingebauter Attribute.\n \nEin erhöhter Treffer Modifikator erlaubt es entfernte Ziele\n öfter zu treffen sofern\nanständig gezielt wird.\n \nHöher ist besser.",
	L"\n \nDer Zielbonus wird verändert durch\nMunition, Erweiterungen oder eingebauter Attribute.\n \nEin erhöhter Zielbonus erlaubt es entfernte Ziele\n öfter zu treffen sofern\nanständig gezielt wird.\n \nHöher ist besser.",
	L"\n \nEin einziger Schuss dieser Waffe\nkann diese Hitze erzeugen.\nUnterschiedliche Munition kann diesen Wert beeinflussen.\n \nNiedriger ist besser.",
	L"\n \nIn jeder Runde kühlt die Temperatur um diesen Wert ab.\nAnbauten an der Waffe können diesen Wert beinflussen.\n \nHöher ist besser.",
	L"\n \nWenn die Temperatur der Waffe über diese Wert steigt,\nkann die Waffe leicht blockieren.",
	L"\n \nWenn die Temperatur der Waffe über diesen Wert steigt,\nkann die Waffe Schaden davon tragen.",
	L"\n \nDer horizontale Rückstoß wurde prozentual geändert\ndurch Munition, Erweiterungen oder inhärenter\nFähigkeiten.\n \nDies hat keine Wirkung bei Einzelschüssen!\n \nEin reduzierter Rückstoß erleichtert es bei einer Salve die\nMündung auf das Ziel zu richten!\n \nNiedriger ist besser.",
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"EINZEL",
	L"AP",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

// HEADROCK HAM 5: Screen messages for sector inventory sorting reports.
// TODO.Translate
STR16 gzMapInventorySortingMessage[] =
{
	L"Das Sortieren der Munition in Kisten im Sektor %c%d wurde fertiggestellt.",
	L"Das Entfernen von Gegenstandsanbauten im Sektor %c%d wurde fertiggestellt.",
	L"Das Entfernen von Munition der Waffen im Sektor %c%d wurde fertiggestellt.",
	L"Das Stapeln und Zusammenführen von Gegenständen im Sektor %c%d wurde fertiggestellt.",
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
	L"Alle anzeigen",
	L"Waffen",
	L"Munition",
	L"Sprengstoffe",
	L"Nahkampfwaffen",
	L"Rüstung",
	L"LBE",
	L"Ausrüstung",
	L"Andere Gegenstände",
	L"Alle ausblenden",
};

// TODO.Translate
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
STR16 gTemperatureDesc[] =
{
	L"Temperatur ist ",
	L"sehr niedrig",
	L"niedrig",
	L"mittel",
	L"hoch",
	L"sehr hoch",
	L"gefährlich",
	L"KRITISCH",
	L"DRAMATISCH",
	L"unbekannt",
	L"."
};

// Flugente: food condition texts
STR16 gFoodDesc[] =
{
	L"Nahrung ist ",
	L"frisch",
	L"gut",
	L"in Ordnung",
	L"alt",
	L"ranzig",
	L"verdorben",
	L"."
};

CHAR16* ranks[] = 
{	L"",			//ExpLevel 0
	L"Rekr. ",		//ExpLevel 1
	L"Gfr. ",		//ExpLevel 2
	L"Kpl. ",		//ExpLevel 3
	L"Zgf. ",		//ExpLevel 4
	L"Lt. ",		//ExpLevel 5
	L"Hptm. ",		//ExpLevel 6
	L"Mjr. ",		//ExpLevel 7
	L"Bgdr. ",		//ExpLevel 8
	L"GenLt. ",		//ExpLevel 9
	L"Gen. "		//ExpLevel 10
};

// TODO.Translate
STR16	gzNewLaptopMessages[]=
{
	L"Ask about our special offer!",
	L"Temporarily Unavailable",
	L"This special press preview of Jagged Alliance 2: Unfinished Business contains the only first 6 sector maps. The final version of the game will feature many more - please see the included readme file for details.",
};

STR16	zNewTacticalMessages[]=
{
	//L"Entfernung zum Ziel: %d Felder, Helligkeit: %d/%d",
	L"Verbinden Sie den Transmitter mit Ihrem Laptop-Computer.",
	L"Sie haben nicht genug Geld, um %s anzuheuern",
	L"Das obenstehende Honorar deckt für einen begrenzten Zeitraum die Kosten der Gesamtmission, und schließt untenstehendes Equipment mit ein.",
	L"Engagieren Sie %s jetzt und nutzen Sie den Vorteil unseres beispiellosen 'Ein Betrag für alles'-Honorars. Das persönliche Equipment des Söldners ist gratis in diesem Preis mit inbegriffen.",
	L"Honorar",
	L"Da ist noch jemand im Sektor...",
	//L"Waffen-Rchwt.: %d Felder, Trefferwahrsch.: %d Prozent",
	L"Deckung anzeigen",
	L"Sichtfeld",
	L"Neue Rekruten können dort nicht hinkommen.",
	L"Da Ihr Laptop keinen Transmitter besitzt, können Sie keine neuen Teammitglieder anheuern. Vielleicht ist dies eine guter Zeitpunkt, ein gespeichertes  Spiel zu laden oder ein neues zu starten!",
	L"%s hört das Geräusch knirschenden Metalls unter Jerry hervordringen. Es klingt grässlich - die Antenne ihres Laptop-Computers ist  zerstört.",  //the %s is the name of a merc.  @@@  Modified
	L"Nach Ansehen des Hinweises, den Commander Morris hinterließ, erkennt %s eine einmalige Gelegenheit. Der Hinweis enthält Koordinaten für den Start von Raketen gegen verschiedene Städte in Arulco. Aber er enthält auch die Koordinaten des Startpunktes - der Raketenanlage.",
	L"Das Kontroll-Board studierend, entdeckt %s, dass die Zahlen umgedreht werden könnten, so dass die Raketen diese Anlage selbst zerstören. %s muss nun einen Fluchtweg finden. Der Aufzug scheint die schnellstmögliche Route zu bieten...",         //!!! The original reads:	L"Noticing the control panel %s, figures the numbers can be reversed..." That sounds odd for me, but I think the comma is placed one word too late... (correct?)
	L"Dies ist der IRONMAN-Modus und es kann nicht gespeichert werden, wenn sich Gegner in der Nähe befinden.",		
	L"(Kann während Kampf nicht speichern)", 
	L"Der Name der aktuellen Kampagne enthält mehr als 30 Buchstaben.",						
	L"Die aktuelle Kampagne kann nicht gefunden werden.",																	
	L"Kampagne: Standard ( %S )",																							
	L"Kampagne: %S",																													
	L"Sie haben die Kampagne %S gewählt. Diese ist eine vom Spieler modifizierte Version der Originalkampagne von JA2UB. Möchten Sie die Kampagne %S spielen?",			
	L"Um den Editor zu benutzen, müssen Sie eine andere als die Standardkampgane auswählen.",		
	// anv: extra iron man modes
	L"Das ist der SOFT IRONMAN-Modus und es kann während des taktischen Rundenkampfes nicht gespeichert werden.",	
	L"Das ist der EXTREME IRONMAN-Modus und es kann nur einmal am Tag gespeichert werden, um %02d:00.",
};

// The_bob : pocket popup text defs
STR16	gszPocketPopupText[]=
{
	L"Granatwerfer",	// POCKET_POPUP_GRENADE_LAUNCHERS,
	L"Raketenwerfer",	// POCKET_POPUP_ROCKET_LAUNCHERS
	L"Hand- & Wurfwaffen",	// POCKET_POPUP_MEELE_AND_THROWN
	L"- keine passende Munition -",	//POCKET_POPUP_NO_AMMO
	L"- keine Waffen im Inventar -",	//POCKET_POPUP_NO_GUNS
	L"weitere...",		//POCKET_POPUP_MOAR
};

// Flugente: externalised texts for some features
STR16	szCovertTextStr[]=
{
	L"%s hat Feldtarnung!",
	L"%s hat einen Rucksack!",
	L"%s wurde gesichtet beim Tragen einer Leiche!",
	L"%s's %s ist verdächtig!",
	L"%s's %s ist eine militärische Ausrüstung!",
	L"%s trägt zu viele Waffen!",
	L"%s's %s ist zu fortgeschritten für einen %s Soldat!",
	L"%s's %s hat zu viele Anbauten!",
	L"%s wurde gesichtet bei verdächtigen Handlungen!",
	L"%s schaut nicht wie ein Zivilist aus!",
	L"%s Blutung wurde entdeckt!",
	L"%s ist betrunken und verhält sich deshalb nicht wie ein Soldat!",
	L"Bei genauerer Betrachtungweise ist %s's Verkleidung nicht wirksam!",
	L"%s sollte nicht hier sein!",
	L"%s sollte um diese Uhrzeit nicht hier sein!",
	L"%s wurde in der Nähe einer frischen Leiche gesichtet!",
	L"%s Ausrüstung sorgt für Erstaunen!",
	L"%s visiert %s an!",
	L"%s hat durch %s's Verkleidung gesehen!",
	L"In der Items.xml wurde keine Kleidung gefunden!",
	L"Dies funktioniert nicht mit dem alten Fertigkeitensystem!",
	L"Zu wenig Bewegungspunkte!",
	L"Fehlerhafte Farbpalette!",
	L"Sie brauchen die Geheimagent-Fertigkeit um dies zu tun!",
	L"Keine Uniform gefunden!",
	L"%s ist jetzt verkleidet als ein Zivilist.",
	L"%s ist jetzt verkleidet als ein Soldat.",
	L"%s trägt keine korrekte Uniform!",
	L"Verkleidet zur Kapitulation aufzufordern, war im Nachhinein nicht ganz so klug ...",
	L"%s wurde enttarnt!",
	L"%s's Verkleidung sollte überzeugen",
	L"%s's Verkleidung wird auffliegen!",
	L"%s wurde beim Stehlen erwischt!",
	L"%s hat versucht, %s's Azsrüstung zu verändern.",
	L"Einem Elitesoldat schien %s verdächtig!",
	L"Ein Offizier hat %s erkannt!",
};

STR16	szCorpseTextStr[]=
{
	L"Kein Kopf-Gegenstand in der Datei Items.xml!",
	L"Leiche kann nicht enthauptet werden!",
	L"Kein Fleisch-Gegenstand in der Datei Items.xml!",
	L"Nicht möglich, sie krankes Individuum!",
	L"Keine Kleidung vorhanden zum Nehmen!",
	L"%s kann der Leiche nicht die Kleidung entwenden!",
	L"Diese Leiche kann nicht genommen werden!",
	L"Keine freie Hand vorhanden um die Leiche zu nehmen!",
	L"Kein Leiche-Gegenstand vorhanden in der Datei Items.xml!",
	L"Ungültige Leiche-ID!",
};

STR16	szFoodTextStr[]=
{
	L"%s möchte nicht %s essen",
	L"%s möchte nicht %s trinken",
	L"%s hat %s gegessen",
	L"%s hat %s getrunken",
	L"%s's Kraft wurde weniger, weil überfüttert!",
	L"%s's Kraft wurde weniger, wegen Nahrungsmangel!",
	L"%s's Gesundheit wurde angegriffen, weil überfüttert!",
	L"%s's Gesundheit wurde angegriffen, wegen Nahrungsmangel!",
	L"%s's Kraft wurde weniger, durch exzessives Trinken!",
	L"%s's Kraft wurde weniger, durch Mangel an Wasser!",
	L"%s's Gesundeheit wurde angegriffen, durch exzessives Trinken!",
	L"%s's Gesundeheit wurde angegriffen, durch Mangel an Wasser!",
	L"Sektorübergreifendes Befüllen der Feldflaschen ist nicht möglich, weil Nahrungssystem nicht aktiv ist!"
};

STR16	szPrisonerTextStr[]=
{
	L"%d Offiziere, %d Elite-, %d Reguläre, %d Hilfssoldaten, %d Generäle und %d Zivilisten wurden verhört.",
	L"$%d als Lösegeld erhalten.",
	L"%d Gefangene haben uns Truppenstandorte verraten.",
	L"%d Offiziere, %d Elite-, %d Reguläre und %d Hilfssoldaten laufen zu uns über.",
	L"Gefangenenaufstand in %s!",
	L"%d Gefangene wurden nach %s geschickt!",
	L"Gefangene freigelassen!",
	L"Die Armee hat das Gefängnis in %s besetzt, die Gefangenen wurden befreit!",
	L"Der Gegner weigert sich aufzugeben!",
	L"Der Feind weigert sich, Sie als Gefangenen zu nehmen - Er möchte Sie tod sehen!",
	L"Dieses Verhalten ist ausgeschaltet in der ja2_options.ini Datei.",
	L"%s hat %s befreit!",
	L"Ein ranghoher Offizier in %s wurde enttarnt!",
	L"Der feindliche Anführer denkt nicht mal an Kapitulation!",
	L"%d Gefangene sind uns als Freiwillige beigetreten.",
};

STR16	szMTATextStr[]=
{
	L"nichts",
	L"baue eine Befestigung",
	L"entferne eine Befestigung",
	L"hacking",
	L"%s musste %s stoppen.",
	L"Die gewählte Barrikade kann in diesem Sektor nicht gebaut werden",
};

STR16	szInventoryArmTextStr[]=
{
	L"Sprengen (%d AP)",
	L"Sprengen",
	L"Scharf machen (%d AP)",
	L"Scharf machen",
	L"Entschärfen (%d AP)",
	L"Entschärfen",
};

// TODO.Translate
STR16		szBackgroundText_Flags[]=
{
	L" might consume drugs in inventory\n",
	L" disregard for all other backgrounds\n",
	L" +1 level in underground sectors\n",
	L" steals money from the locals sometimes\n",

	L" +1 traplevel to planted bombs\n",
	L" spreads corruption to nearby mercs\n",
	L" female only",							// won't show up, text exists for compatibility reasons
	L" male only",								// won't show up, text exists for compatibility reasons

	L" huge loyality penalty in all towns if we die\n",

	L" refuses to attack animals\n",
	L" refuses to attack members of the same group\n",
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
	L" %s%d%% disease resistance\n",

	L" %s%d%% interrogation effectiveness\n",
	L" %s%d%% prison guard strength\n",
	L" %s%d%% better prices when trading guns and ammo\n",
	L" %s%d%% better prices when trading armour, lbe, blades, kits etc.\n",
	L" %s%d%% team capitulation strength if we lead negotiations\n",
	L" %s%d%% faster running\n",
	L" %s%d%% bandaging speed\n",
	L" %s%d%% breath regeneration\n",
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
	L" %s%d effectivity at disarming traps\n",
	L" %s%d%% CTH with SAMs\n",

	L" %s%d%% effectiveness to friendly approach\n",
	L" %s%d%% effectiveness to direct approach\n",
	L" %s%d%% effectiveness to threaten approach\n",
	L" %s%d%% effectiveness to recruit approach\n",

	L" %s%d%% chance of success with door breaching charges\n",
	L" %s%d%% cth with firearms against creatures\n",
	L" %s%d%% insurance cost\n",
	L" %s%d%% effectiveness as spotter for fellow snipers\n",
	L" %s%d%% effectiveness at diagnosing diseases\n",
	L" %s%d%% effectiveness at treating population against diseases\n",
	L"Can spot tracks up to %d tiles away\n",
	L" %s%d%% initial distance to enemy in ambush\n",
	L" %s%d%% chance to evade snake attacks\n",

	L" dislikes some other backgrounds\n",
	L"Smoker",
	L"Nonsmoker",
	L" %s%d%% enemy CTH if crouched against thick cover in their direction\n",
	L" %s%d%% building speed\n",
	L" hacking skill: %s%d ",
	L" %s%d%% burial speed\n",
	L" %s%d%% administration effectiveness\n",
	L" %s%d%% exploration effectiveness\n",
};

// TODO.Translate
STR16		szBackgroundTitleText[] =
{
	L"I.M.P. Background",
};

// Flugente: personality
// TODO.Translate
STR16		szPersonalityTitleText[] =
{
	L"I.M.P. Prejudices",
};

// TODO.Translate
STR16		szPersonalityDisplayText[]=
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
// TODO.Translate
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

// TODO.Translate
STR16		szRaceText[]=
{
	L"white",
	L"black",
	L"asian",
	L"eskimo",
	L"hispanic",
};

// TODO.Translate
STR16		szAppearanceText[]=
{
	L"average",
	L"ugly",
	L"homely",
	L"attractive",
	L"like a babe",
};

// TODO.Translate
STR16		szRefinementText[]=
{
	L"average manners",
	L"manners of a slob",
	L"manners of a snob",
};

// TODO.Translate
STR16		szRefinementTextTypes[] =
{
	L"normal people",
	L"slobs",
	L"snobs",
};

// TODO.Translate
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

// TODO.Translate
STR16		szNationalityTextAdjective[] =
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
// TODO.Translate
STR16		szNationalityText_Special[]=
{
	L"and do not hate any other nationality.",		// used in personnel.cpp
	L"of no origin",		// used in IMP generation
};

// TODO.Translate
STR16		szCareLevelText[]=
{
	L"not",
	L"somewhat",
	L"extremely",
};

// TODO.Translate
STR16		szRacistText[]=
{
	L"not",
	L"somewhat",
	L"very",
};

// TODO.Translate
STR16		szSexistText[]=
{
	L"no sexist",
	L"somewhat sexist",
	L"very sexist",
	L"a Gentleman",
};

// Flugente: power pack texts
// TODO.Translate
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

// TODO.Translate
STR16	szSoldierClassName[]=
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16	szCampaignHistoryMoneyTypeString[]=
{
	L"Initial funding",
	L"Mine income",
	L"Trade",
	L"Other sources",
};

// TODO.Translate
STR16	szCampaignHistoryConsumptionTypeString[]=
{
	L"Ammunition",
	L"Explosives",
	L"Food",
	L"Medical gear",
	L"Item maintenance",
};

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16	szCampaignStatsOperationPrefix[] =
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
	L"Narcoleptic %s",
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

// TODO.Translate
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
	L"Uniform",
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

// TODO.Translate
STR16	szMercCompareWebSite[] =
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

// TODO.Translate
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

// TODO.Translate
STR16	szPMCWebSite[] =
{
	// main page
	L"Kerberus",
	L"Erfahrung in Sicherheit",

	// links to other pages
	L"Was ist Kerberus?",
	L"Team Verträge",
	L"Individuelle Verträge",

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

// TODO.Translate
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

// TODO.Translate
STR16   szTacticalCoverDialogString[]=
{
        L"Cover Display Mode",

        L"Off",
        L"Enemy",
        L"Merc",
        L"",

        L"Roles",
		L"Fortification",
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

// TODO.Translate
STR16   szTacticalCoverDialogPrintString[]=
{
        
        L"Turning off cover/traps display",
        L"Showing danger zones",
        L"Showing merc view",
        L"",

		L"Display enemy role symbols",
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
STR16	szDynamicDialogueText[40][17] =
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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
	L"er",
	L"sie",
	L"seine",
	L"ihre",
};

// TODO.Translate
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
	L"Burial",
	L"Cancel",

	L"\n\n%s (undiagnosed) - %d / %d\n",

	L"High amount of distress can cause a personality split\n",
	L"Contaminated items found in %s' inventory.\n",
	L"Whenever we get this, a new disability is added.\n",

	L"Only one hand can be used.\n",
	L"Only one hand can be used.\nA medical splint was applied to speed up the healing process.\n",
	L"Leg functionality severely limited.\n",
	L"Leg functionality severely limited.\nA medical splint was applied to speed up the healing process.\n",
};

// TODO.Translate
STR16	szSpyText[] =
{
	L"Hide",
	L"Get Intel",
};

// TODO.Translate
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

// TODO.Translate
STR16 szIMPGearWebSiteText[] =
{
	// IMP Gear Entrance
	L"How should gear be selected?",
	L"Old method: Random gear according to your choices",
	L"New method: Free selection of gear",
	L"Old method",
	L"New method",

	// IMP Gear Entrance
	L"I.M.P. Equipment",
	L"Additional Cost: %d$ (%d$ prepaid)",
};

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16 szEnemyHeliText[] =
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

// TODO.Translate
STR16 szFortificationText[] =
{
	L"No valid structure selected, nothing added to build plan.",
	L"No gridno found to create items in %s - created items are lost.",
	L"Structures could not be built in %s - people are in the way.",
	L"Structures could not be built in %s - the following items are required:",

	L"No fitting fortifications found for tileset %d: %s",
	L"Tileset %d: %s",
};

// TODO.Translate
STR16	szMilitiaWebSite[] =
{
	// main page
	L"Militia",
	L"Militia Forces Overview",
};

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16	szIdividualMilitiaWebsiteFilterText_Dead[] =
{
	L"All statuses",
	L"Deceased",
	L"Active",
	L"Fired",
};

// TODO.Translate
STR16	szIdividualMilitiaWebsiteFilterText_Rank[] =
{
	L"All ranks",
	L"Green",
	L"Regular",
	L"Elite",
};

// TODO.Translate
STR16	szIdividualMilitiaWebsiteFilterText_Origin[] =
{
	L"All origins",
	L"Rebel",
	L"PMC",
	L"Defector",
};

STR16	szIdividualMilitiaWebsiteFilterText_Sector[] =
{
	L"Alle Sektoren",
};

// TODO.Translate
STR16	szNonProfileMerchantText[] =
{
	L"Merchant is hostile and does not want to trade.",
	L"Merchant is in no state to do business.",
	L"Merchant won't trade during combat.",
	L"Merchant refuses to interact with you.",
};

STR16	szWeatherTypeText[] =
{
	L"Normal",
	L"Regen",
	L"Gewitter",
	L"Sandsturm",
	L"Schnee",
};

STR16	szSnakeText[] =
{
	L"%s weicht Schlangenangriff aus!",
	L"%s wurde von Schlange angegriffen!",
};

// TODO.Translate
STR16	szSMilitiaResourceText[] =
{
	L"Converted %s into resources",
	L"Guns: ",
	L"Armour: ",
	L"Misc: ",

	L"There are no volunteers left for militia!",
	L"Not enough resources to train militia!",
};

// TODO.Translate
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
	L"This machine doesn't seem to be working.",
};

// TODO.Translate
STR16	szLaptopStatText[] =
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

// TODO.Translate
STR16	szGearTemplateText[] =	// TODO.Translate
{
	L"Enter Template Name",
	L"Not possible during combat.",
	L"Selected mercenary is not in this sector.",
	L"%s is not in that sector.",
	L"%s could not equip %s.",
	L"We cannot attach %s (item %d) as that might damage items.",	// TODO.Translate
};

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
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

// TODO.Translate
STR16	szMilitiaText[] =
{
	L"Train new militia",
	L"Drill militia",
	L"Doctor militia",
	L"Cancel",
};

// TODO.Translate
STR16	szFactoryText[] =
{
	L"%s: Production of %s switched off as loyalty is too low.",
	L"%s: Production of %s switched off due to insufficient funds.",
	L"%s: Production of %s switched off as it requires a merc to staff the facility.",
	L"%s: Production of %s switched off due to required items missing.",
	L"Item to build",

	L"Preproducts",	// 5
	L"h/item",
};

// TODO.Translate
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

#endif //GERMAN
