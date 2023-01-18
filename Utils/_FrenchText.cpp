// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("FRENCH")

	#include "Language Defines.h"
	#ifdef FRENCH
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_FrenchText_public_symbol(void){;}

#ifdef FRENCH

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
	L"Lieux", // 0
	L"Personnages",
	L"Objets",
	L"Quêtes",
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
	L"Quitter",
};

STR16	pOtherButtonsText[] =
{
	L"Briefing", 
	L"Accepter",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Briefing",
	L"Accepter mission",
};


STR16	pLocationPageText[] =
{
	L"Page préc.",
	L"Photo", 
	L"Page suiv.",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"Accueil",
	L">>",
	L"Type : ",
	L"Vide",
	L"Pas de mission définie. Ajouter mission au fichier TableData\\BriefingRoom\\BriefingRoom.xml. La première mission doit être visible. Mettre la valeur Hidden = 0.",
	L"Salle de briefing. Appuyez sur la touche 'ENTRÉE'.",
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Inconnu",// 0 - unknown
	L"Ville", //1 - cities
	L"Site SAM", //2 - SAM Site
	L"Autre lieu", //3 - other location
	L"Mine", //4 - mines 
	L"Base militaire", //5 - military complex 
	L"Laboratoire",  //6 - laboratory complex 
	L"Entreprise", //7 - factory complex 
	L"Hôpital", //8 - hospital 
	L"Prison", //9 - prison
    L"Aéroport", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Voir tout",
	L"Voir AIM",
	L"Voir MERC",
	L"Voir PR",
	L"Voir PNJ",
	L"Voir Véhicule",
	L"Voir IMP",
	L"Voir PNJE",
	L"Filtre",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"Tout",
	L"AIM",
	L"MERC",
	L"PR",
	L"PNJ",
	L"Véh.",
	L"IMP",
	L"PNJE",
	L"Filtre",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Voir tout",
	L"Voir Ville",
	L"Voir Site SAM",
	L"Voir Autre lieu",
	L"Voir mine",
	L"Voir Base militaire",
	L"Voir Laboratoire",
	L"Voir Entreprise",
	L"Voir Hôpital",
	L"Voir prison",
	L"Voir Aéroport",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"Tout",
	L"Ville",
	L"SAM",
	L"Autre",
	L"Mine",
	L"Milit.",
	L"Labo.",
	L"Usine",
	L"Hôpit.",
	L"Prison",
	L"Aérop.",
};

STR16	pEncyclopediaFilterLocationText[] =
{//major location filter button text max 7 chars
//..L"------v"
	L"Tout",//0
	L"Ville",
	L"SAM",
	L"Mine",
	L"Aérop.",
	L"Nature",
	L"Ss-sol",
	L"Instal.",
	L"Autre",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Voir tout.",//facility index + 1
	L"Voir ville.",
	L"Voir site SAM.",
	L"Voir mine.",
	L"Voir aéroport.",
	L"Voir les extérieurs.",
	L"Voir les sous-sols.",
	L"Voir secteur avec installation(s)\n[|B|G] Suivant\n[|B|D] Réinitialise filtre.",
	L"Voir autre secteur.",
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
	L"Tout",//0
	L"AIM",
	L"MERC",
	L"PR",
	L"PNJ",
	L"IMP",
	L"Autre",//add new filter buttons before other
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Voir tout.",//Other index + 1
	L"Voir membre de l'AIM.",
	L"Voir membre de MERC.",
	L"Voir rebellle.",
	L"Voir Personnage Non Jouable.",
	L"Voir personnage IMP.",
	L"Voir autre personnage\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
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
	L"Véh.",
	L"PNJE",
	L"",//reserved. Insert new Other filters above!
};

STR16	pEncyclopediaFilterItemText[] = 
{//major item filter button text max 7 chars
//..L"------v"
	L"Tout",//0
	L"Arme",
	L"Munit.",
	L"Prot.",
	L"LBE",
	L"Acces.",
	L"Divers",//add new filter buttons before misc
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Tout voir",//misc index + 1
	L"Voir les armes\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
	L"Voir les munitions\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
	L"Voir les protections\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
	L"Voir l'équipement LBE\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
	L"Voir les accessoires\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
	L"Voir les objets\n[|B|G] Catégorie suivante\n[|B|D] Réinitialise filtre.",
};

STR16 pEncyclopediaSubFilterItemText[] =
{//item subfilter button text max 7 chars
//..L"------v"
//Guns......v"
	L"A/poing",
	L"PM",
	L"Mitra.",
	L"Fusil",
	L"Sniper",
	L"F/Ass.",
	L"FM",
	L"F/pompe",
	L"Arme/L",
	L"",//reserved. insert new gun filters above!
//Amunition.v"
	L"A/poing",
	L"PM",
	L"Mitra.",
	L"Fusil",
	L"Sniper",
	L"F/Ass.",
	L"FM",
	L"F/pompe",
	L"Arme/L",
	L"",//reserved. insert new ammo filters above!
//Armor.....v"
	L"Casque",
	L"Veste",
	L"Pant.",
	L"Blind.",
	L"",//reserved. insert new armor filters above!
//LBE.......v"
	L"Dédié",
	L"Veste",
	L"Sac",
	L"Sac/dos",
	L"Poche",
	L"Autre",
	L"",//reserved. insert new LBE filters above!
//Attachments"
	L"Optique",
	L"Attache",
	L"Bouche",
	L"Externe",
	L"Interne",
	L"Autre",
	L"",//reserved. insert new attachment filters above!
//Misc......v"
	L"A/bl.",
	L"A/Jet",
	L"Mêlée",
	L"Grenade",
	L"Explos.",
	L"Médical",
	L"Kit",
	L"Face",
	L"Autre",
	L"",//reserved. insert new misc filters above!
//add filters for a new button here
};

STR16 pEncyclopediaFilterQuestText[] =
{//major quest filter button text max 7 chars
//..L"------v"
	L"Tout",
	L"Activée",
	L"Achevée",
//filter button tooltip
//..L"---------------------------------------------------------------------v"
	L"Tout voir",//misc index + 1
	L"Voir les quêtes activées",
	L"Voir les quêtes achevées",
};

STR16 pEncyclopediaSubFilterQuestText[] =
{//Quest subfilter button text max 7 chars, not used, but needed if any subfilters are added
//..L"------v"
	L"",//reserved. insert new active quest subfilters above!
	L"",//reserved. insert new completed quest subfilters above!
};

STR16	pEncyclopediaShortInventoryText[] = 
{
	L"Tout", //0
	L"Arme",
	L"Mun.",
	L"LBE",
	L"Divers",
	
	L"Tout", //5
	L"Arme",
	L"Munition",
	L"Mat. LBE",
	L"Divers",
};

STR16			BoxFilter[] =
{
	// Guns
	L"A/Lourde",
	L"A/Poing",
	L"PM",
	L"Mitrail.",
	L"Fusil",
	L"Sniper",
	L"F/Assaut",
	L"FM",
	L"F/Pompe",

	// Ammo
	L"A/Poing",
	L"PM", //10
	L"Mitrail.",
	L"Fusil",
	L"Sniper",
	L"F/Assaut",
	L"FM",
	L"F/Pompe",

	// Used
	L"Arme",
	L"Protection",
	L"Mat. LBE",
	L"Divers", //20

	// Armour
	L"Casque",
	L"Veste",
	L"Pantalon",
	L"Blindage",

	// Misc
	L"A/blanche",
	L"Arme/Jet",
	L"Mêlée",
	L"Grenade",
	L"Explosif",
	L"Médical", //30
	L"Kit&Habit",
	L"Mat. Face",
	L"Mat. LBE",
	L"Divers", //34
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
	L"Supprimer",
	L"Article supprimer (|S|u|p|p|r)",
};

STR16 FaceDirs[8] = 
{ 
	L"NORD",
	L"NORD-Est",
	L"EST",
	L"SUD-EST",
	L"SUD",
	L"SUD-OUEST",
	L"OUEST",
	L"NORD-OUEST"
};

STR16 iEditorMercsToolbarText[] = 
{
 L"Activer l'affichage du joueur", //0
 L"Activer l'affichage des ennemis",
 L"Activer l'affichage des créatures",
 L"Activer l'affichage des rebelles",
 L"Activer l'affichage des civils",
 
 L"Joueur",
 L"Ennemi",
 L"Créature",
 L"Rebelle",
 L"Civil",
 
 L"PLACEMENT DÉTAILLÉ", //10
 L"Information général mode",
 L"Apparence physique mode",
 L"Attribut mode",
 L"Inventaire mode",
 L"ID profile mode",
 L"Calendrier mode",
 L"Calendrier mode",
 L"SUPPRIMER",
 L"Supprimer le mercenaire sélectionné (|S|u|p|p|r)",
 L"SUIVANT", //20
 L"Mercenaire suivant (|E|S|P|A|C|E)\nMercenaire précédente (|C|T|R|L+|E|S|P|A|C|E)",
 L"Changer l'existence prioritaire",
 L"Changer, si le placement a\naccès à toutes les portes",
 
 //Orders
 L"STATIONNAIRE",
 L"EN DÉFENSE",
 L"DE GARDE",
 L"CHERCHER LES ENNEMIS",
 L"PATROUILLE PROCHE",
 L"PATROUILLE LONTAINE",
 L"POINT DE RASSEMBLEMENT", //30
 L"TOUR DE PATROUILLE",
 
 //Attitudes
 L"DÉFENSE",
 L"SOLO BRAVE",
 L"SOUTIEN BRAVE",
 L"AGGRESIF",
 L"SOLO RUSÉ",
 L"SOUTIEN RUSÉ",
 
 L"Placez mercenaire face à %s",	// Max 30 characters
 
 L"Trouvé",
 L"MAUVAIS", //40
 L"FAIBLE",
 L"MOYEN",
 L"BON",
 L"EXCELLENT",
 
 L"MAUVAIS",
 L"FAIBLE",
 L"MOYEN",
 L"BON",
 L"EXCELLENT",
 
 L"Couleur précédente", //50
 L"Couleur suivante",
 
 L"Corps précédent",
 L"Corps suivant",
 
 L"Changer la variation de temps (+ ou - 15 minutes)",
 L"Changer la variation de temps (+ ou - 15 minutes)",
 L"Changer la variation de temps (+ ou - 15 minutes)",
 L"Changer la variation de temps (+ ou - 15 minutes)",
 
 L"Pas d'action",
 L"Pas d'action",
 L"Pas d'action", //60
 L"Pas d'action",
 
 L"Effacer le calendrier",
 
 L"trouver le mercenaire sélectionné",
};

STR16 iEditorBuildingsToolbarText[] =
{
	L"TOITS",  //0
	L"MURS",
	L"INFO PIÈCE",

	L"Placer les murs en utilisant la méthode de sélection",
	L"Placer les portes en utilisant la méthode de sélection",
	L"Placer les toits en utilisant la méthode de sélection",
	L"Placer les fenêtres en utilisant la méthode de sélection",
	L"Placer les murs endommagés en utilisant la méthode de sélection.",
	L"Placer les meubles en utilisant la méthode de sélection",
	L"Placer les décalcomanies murales en utilisant la méthode de sélection",
	L"Placer les étages en utilisant la méthode de sélection", //10
	L"Placer les meubles génériques en utilisant la méthode de sélection",
	L"Placer les murs avec la méthode rapide",
	L"Placer les portes avec la méthode rapide",
	L"Placer les fenêtres avec la méthode rapide",
	L"Placer les murs endommagés avec la méthode rapide",
	L"Verrouiller ou piéger les portes existantes",

	L"Ajouter une nouvelle salle",
	L"Éditer les murs de caverne.",
	L"Enlever un secteur de la construction existante.",
	L"Enlever une construction", //20
	L"Ajouter/remplacer le toit de la construction par un nouveau toit plat.",
	L"Copier une construction",
	L"Bouger une construction",
	L"Dessiner le numéro de pièce.\n(Maintenir |M|A|J pour réutiliser le numéro de pièce)",
	L"Supprimer le numéro de pièce",

	L"Activer le mode supprimer (|E)",
	L"Effacer le dernier changement (|R|e|t|o|u|r)",
	L"Taille du cycle (|A/|Z)",
	L"Toits (|H)",
	L"Murs (|W)", //30
	L"Info Pièce (|N)",
};

STR16 iEditorItemsToolbarText[] =
{
	L"Arm.", //0
	L"Mun.",
	L"Prot.",
	L"LBE",
	L"Exp",
	L"E1",
	L"E2",
	L"E3",
	L"Détentes",
	L"Clés",
	L"Cps",	//10
	L"Précédent (|,)",		// previous page
	L"Suivant (|.)",		// next page
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Ajouter une source de lumière ambiante", //0
	L"Basculer en fausse lumière ambiante.",
	L"Ajouter des réseaux de sortie (clic droit pour une requête existante).",
	L"Taille de cycle (|A/|Z)",
	L"Effacer le dernier changement (|R|e|t|o|u|r)",
	L"Basculer en mode supprimer (|E)",
	L"Spécifier un point au NORD pour valider le but.",
	L"Spécifier un point à l'OUEST pour valider le but.",
	L"Spécifier un point à l'EST pour valider le but.",
	L"Spécifier un point au SUD pour valider le but.",
	L"Spécifier un point du centre pour valider le but.", //10
	L"Spécifier un point isolé pour valider le but.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"Nouvelle carte",  //0
	L"Nouveau sous-sol",
	L"Nouveau niveau de caverne",
	L"Sauvegarder la carte (|C|T|R|L+|S)",
	L"Charger la carte (|C|T|R|L+|L)",
	L"Sélectionner un tileset",
	L"Quitter le mode éditeur",
	L"Quitter le jeu (|A|L|T+|X)",
	L"Créer un carte de radar",
	L"Une fois la carte vérifiée, elle sera sauvée sous le format original JA2. Les objets avec l'ID > 350 seront perdus.\nCette option est seulement valable sur les cartes de taille \"normale\" qui ne font pas référence aux nombre de réseaux (ex : réseau de sortie) > 25600.",
	L"Une fois la carte vérifiée et chargée, elle sera élargie automatiquement selon les rangées et colonnes choisies.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Dessiner des textures de sol (|G)", //0
	L"Sélectionner les textures du sol de la carte",
	L"Placer les rives et falaises (|C)",
	L"Dessiner les routes (|P)",
	L"Dessiner les débris (|D)",
	L"Placer les arbres & buissons (|T)",
	L"Placer les rochers (|R)",
	L"Placer barrils & autres camelotes (|O)",
	L"Remplir le secteur",
	L"Effacer le dernier changement (|R|e|t|o|u|r)",
	L"Basculer en mode supprimer (|E)", //10
	L"Taille du cycle (|A/|Z)",
	L"Augmenter la densité du pinceau (|])",
	L"Diminuer la densité du pinceau (|[)",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Terrain", //0
	L"Bâtiment",
	L"Objets",
	L"Mercenaires",
	L"Info carte",
	L"Options",
	L"|./|, : Cycle entre les dimensions 'largeur : xx'\n|P|g |U|p/|P|g |D|n : case précédente/suivante pour l(es)'objet(s) sélectionné(s)/en méthode intelligente", //Terrain fasthelp text
	L"|./|, : Cycle entre les dimensions 'largeur : xx'\n|P|g |U|p/|P|g |D|n : case précédente/suivante pour l(es)'objet(s) sélectionné(s)/en méthode intelligente", //Buildings fasthelp text
	L"|E|S|P|A|C|E : Sélectionne l'objet suivant\n|C|T|R|L+|E|S|P|A|C|E : Sélectionne l'objet précédent\n \n|/ : Place le même objet sous le curseur de la souris\n|C|T|R|L+|/ : Place le nouveau objet sous le curseur de la souris", //Items fasthelp text
	L"|1-|9 : Pose de waypoints\n|C|T|R|L+|C/|C|T|R|L+|V : Copie/colle mercenaire\n|P|g |U|p/|P|g |D|n: Toggle merc placement level", //Mercs fasthelp text	// TODO.Translate
	L"|C|T|R|L+|G : Va à la case\n|M|A|J : fait défiler la carte au-delà de ses limites\n \n(|t|i|l|d|e): Toggle cursor level\n|I : (dés)active la carte vue de dessus\n|J : (dés)active l'affichage des terrains élevés\n|K : (dés)active les marqueurs de terrain élevé\n|M|A|J+|L : (dés)active les points d'angle de la carte\n|M|A|J+|T : (dés)active les feuillages\n|U : (dés)active la montée du monde\n \n|./|, : Cycle entre les dimensions 'largeur : xx'", //Map Info fasthelp text	// TODO.Translate
	L"|C|T|R|L+|N : Crée une nouvelle carte\n \n|F|5 : Montre le résumé des informations/Carte du monde\n|F|1|0 : Retire toutes les lumières\n|F|1|1 : recule les horaires\n|F|1|2 : Efface les horaires\n \n|M|A|J+|R : (dés)active les placements aléatoires basés sur la quantité du/des objet(s) sélectionné(s)\n \nOptions en ligne de commande\n|-|F|A|I|R|E|C|A|R|T|E|S : Génère la carte de type radar\n|-|F|A|I|R|E|C|A|R|T|E|S|C|N|V : Génère la carte de type radar ainsi que le couvert pour la dernière version", //Options fasthelp text
};

//Editor Taskbar Utils.cpp

STR16 iRenderMapEntryPointsAndLightsText[] =
{
	L"Point d'entrée NORD", //0
	L"Point d'entrée OUEST",
	L"Point d'entrée EST",
	L"Point d'entrée SUD",
	L"Point d'entrée centre",
	L"Point d'entrée isolé",
	
	L"Principale",
	L"Nuit",
	L"24 heures",
};

STR16 iBuildTriggerNameText[] =
{
	L"Déclencheur de panique 1", //0
	L"Déclencheur de panique 2",
	L"Déclencheur de panique 3",
	L"Déclencheur %d",
	 
	L"Action de pression",
	L"Action de panique 1",
	L"Action de panique 2",
	L"Action de panique 3",
	L"Action %d",
};

STR16 iRenderDoorLockInfoText[]=
{
	L"Pas de verrouillage d'ID", //0
	L"Piège explosif",
	L"Piège électrique",
	L"Piège sonore",
	L"Alarme silencieuse",
	L"Super piège électrique", //5
	L"Piège sonore de maison close",
	L"Piège de niveau %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Enregistrer la carte au format vanilla JA2 (v1.12) (Version : 5.00/25)", //0
	L"Aucune carte n'est actuellement chargée.",
	L"Fichier :  %S, Tileset actuel :  %s",
	L"Élargir la carte au chargement",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"BASCULER", //0
	L"VUES",
	L"MODE DE SÉLECTION",
	L"MÉTHODE RAPIDE",
	L"MODE DE CONSTRUCTION",
	L"Pièce #", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Modification des attributs de verrouillage à l'index %d de la carte.",
	L"Verrouillage ID",
	L"Type de piège",
	L"Niveau du piège",
	L"Verrouillé",
};

//EditorItems.cpp

STR16 pInitEditorItemsInfoText[] = 
{
	L"Action de pression", //0
	L"Action de panique 1",
	L"Action de panique 2",
	L"Action de panique 3",
	L"Action %d",
	
	L"Déclencheur de panique 1", //5
	L"Déclencheur de panique 2",
	L"Déclencheur de panique 3",
	L"Déclencheur %d",
};

STR16 pDisplayItemStatisticsTex[] =
{
	L"Info Status Ligne 1",
	L"Info Status Ligne 2",
	L"Info Status Ligne 3",
	L"Info Status Ligne 4",
	L"Info Status Ligne 5",
};

//EditorMapInfo.cpp
STR16 pUpdateMapInfoText[] =
{
	L"R", //0
	L"G",
	L"B",
	
	L"Amorcer",
	L"Nuit",
	L"24 H", //5

	L"Rayon",

	L"Souterrain",
	L"Niveau de lumière",

	L"Extérieur",
	L"Sous-sol", //10
	L"Caves",

	L"Restreint",
	L"Défiler ID",

	L"Destination",
	L"Secteur", //15
	L"Destination",
	L"Niveau sous-sol",
	L"Dest.",
	L"Grille No",
};
//EditorMercs.cpp
CHAR16 gszScheduleActions[ 11 ][20] =
{
	L"Pas d'action",
	L"Porte verrouillée",
	L"Porte déverrouillée",
	L"Porte ouverte",
	L"Porte fermée",
	L"Aller à la No",
	L"Quitter le secteur",
	L"Entrer secteur",
	L"Rester secteur",
	L"Dormir",
	L"Ignorez cela !"
};

STR16 zDiffNames[5] = 
{ 
	L"Mauviette", 
	L"Simplet", 
	L"Moyen",
	L"Dur", 
	L"Utilisateurs de stéroïde seulement" 
};

STR16 EditMercStat[12] = 
{ 
	L"Santé max",
	L"Santé actuel",
	L"Force",
	L"Agilité",
	L"Dextérité",
	L"Charisme",
	L"Sagesse",
	L"Tir",
	L"Explosif",
	L"Médecine",
	L"Mécanique",
	L"Niveau exp", 
};


STR16 EditMercOrders[8] = 
{ 
	L"Stationnaire",
	L"En garde",
	L"Patrouille proche",
	L"Patrouille lointaine",
	L"Point de patrouille",
	L"Sur appel",
	L"Chercher ennemi", 
	L"Point de patrouille aléatoire",
};

STR16 EditMercAttitudes[6] = 
{ 
	L"Défensif",
	L"Solitaire courageux",
	L"Brave copain",
	L"Solitaire Rusé",
	L"Copain Rusé",
	L"Aggressif", 
};

STR16 pDisplayEditMercWindowText[] =
{
	L"Nom mercenaire :", //0
	L"Ordre :",
	L"Attitude de combat :",
};

STR16 pCreateEditMercWindowText[] = 
{
	L"Couleur mercenaire", //0
	L"Fait",
	
	L"Mercenaire précédent attendant les ordres",
	L"Mercenaire suivant attendant les ordres", 
	
	L"Mercenaire précédent pour l'attidude de combat",
	L"Mercenaire suivant pour l'attidude de combat",	//5
	
	L"Diminuer les stats du mercenaire",
	L"Augmenter les stats du mercenaire",
};

STR16 pDisplayBodyTypeInfoText[] =
{
	L"Aléatoire", //0
	L"Hme norm",
	L"Hme Grd",
	L"Hme trappu",
	L"Femme norm",
	L"Char NE", //5
	L"Char NO",
	L"Civil gros",
	L"H Civil",
	L"Minijupe",
	L"F Civil", //10
	L"Enf. chap.",
	L"Hummer",
	L"Eldorado",
	L"Camion glace",
	L"Jeep", //15
	L"Enf civil",
	L"Vache",
	L"Infirme",
	L"Robot désarmé",
	L"Larve", //20
	L"Jeune",
	L"Pt Monstre F",
	L"Pt Monstre M",
	L"Monstre Adt F",
	L"Monstre Adt M", //25
	L"Monstre Reine",
	L"Chat Sauvg",
	L"Humvee",		// TODO.Translate
};

STR16 pUpdateMercsInfoText[] = 
{
	L" --=ORDRES=-- ", //0
	L"--=ATTITUDE=--",
	
	L"ATTRIBUTS",
	L"RELATIF",
	
	L"ÉQUIPEMENT",
	L"RELATIF",
			
	L"ATTRIBUTS",
	L"RELATIF",
	
	L"Armée",
	L"Admin",
	L"Élite", //10
	
	L"Niveau d'exp.",
	L"Santé",
	L"Santé Max.",
	L"Tir",
	L"Force",
	L"Agilité",
	L"Dextérité",
	L"Sagesse",
	L"Commandement",
	L"Explosif", //20
	L"Médicine",
	L"Mécanique",
	L"Moral",
	
	L"Couleur cheveux :",
	L"Couleur peau :",
	L"Couleur veste :",
	L"Couleur pantalon :",
	
	L"ALÉATOIRE",
	L"ALÉATOIRE",
	L"ALÉATOIRE", //30
	L"ALÉATOIRE",
	
	L"En spécifiant un indice de profil, toutes les informations seront extraites du profil ",
	L"et remplacées par les valeurs que vous avez édité. Les fonctions d'édition seront aussi désactivées ",
	L"bien que vous soyez toujours en mesure de regarder les stats, etc. En appuyant sur ENTREE cela extraira ",
	L"automatiquement le numéro que vous avez tapé. Un champ vide effacera le profil. Le nombre ",
	L"actuel de profil allant de 0 à ",
	
	L"Profil actuel :  n/a              ",
	L"Profil actuel : %s",
	
	L"STATIONNAIRE",
	L"DE GARDE", //40
	L"EN DÉFENSE",
	L"CHERCHER LES ENNEMIS",
	L"PATROUILLE PROCHE",
	L"PATROUILLE LONTAINE",
	L"POINT DE RASSEMBLEMENT",
	L"TOUR DE PATROUILLE",

	L"Action",
	L"Temps",
	L"V",
	L"Grille No 1", //50
	L"Grille No 2",
	L"1)",
	L"2)",
	L"3)",
	L"4)",
	
	L"verrouillée",
	L"déverrouillée",
	L"ouverte",
	L"fermée",
	
	L"Cliquez sur la case adjacente à la porte que vous souhaitez %s.", //60
	L"Cliquez sur la case où vous voulez aller après avoir %s la porte.",
	L"Cliquez sur la case où vous voulez aller.",
	L"Cliquez sur la case où vous voulez dormir. Le personnage retournera automatiquement à sa position initiale lorsqu'il se lèvera.",
	L"Cliquez sur ESC pour annuler l'entrée de cette ligne dans le calendrier.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Emplacement #%d",
	L"Ordres de patrouille sans waypoints",
	L"Waypoints sans ordres de patrouille",
};

STR16 pClearCurrentScheduleText[] =
{
	L"Pas d'action",
};

STR16 pCopyMercPlacementText[] =
{
	L"Placement non copié car aucun placement sélectionné.",
	L"Placement copié.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Placement non collé car aucun placement n'a été enregistré.",
	L"Placement collé.",
	L"Placement non collé car le nombre maximum de placement pour cette équipe est dépassé.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Quitter l'éditeur ?",
};

STR16 pHandleKeyboardShortcutsText[] = 
{
	L"Êtes-vous sur de vouloir retirer toutes les lumières ?", //0
	L"Êtes-vous sur de vouloir inverser le calendrier ?",
	L"Êtes-vous sur de vouloir effacer tous les horaires ?",
	
	L"Cliquage de placement activé",
	L"Cliquage de placement désactivé",
	
	L"Dessin des hauteurs activés", //5
	L"Dessin des hauteurs désactivés",
	
	L"Nombre de points de bord : N=%d E=%d S=%d O=%d",
	
	L"Placement aléatoire activé",
	L"Placement aléatoire désactivé",
	
	L"Cacher la cîme des arbres", //10
	L"Montrer la cîme des arbres",
	
	L"Réinitialisation de l'aggrandissement de la carte",
	
	L"Ancienne méthode d'aggrandissement",
	L"Aggrandissement fait",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Création de carte radar pour %S", //0
	
	L"Supprimer la carte actuelle pour un nouveau niveau de sous-sol ?",
	L"Supprimer la carte actuelle pour un nouveau niveau de cave ?",
	L"Supprimer la carte actuelle pour un nouveau niveau extérieur ?",
	
	L" Essuyer les textures du sol ? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"Début", //0
	L"Activer l'éditeur de faux éclairage. Marche/arrêt",

	L"INSER",
	L"Activer le mode de remplissage. Marche/arrêt",

	L"RET.AR",
	L"Annuler la dernière modification",

	L"SUPR",
	L"Effacement rapide d'objet sous le curseur de la souris",

	L"ESC",
	L"Quitter l'éditeur",

	L"PHAUT/PBAS", //10
	L"Changement d'objets qui doivent être collé",

	L"F1",
	L"Écran d'aide",

	L"F10",
	L"Sauvegarder la carte actuelle",

	L"F11",
	L"Charger la carte",

	L"+/-",
	L"Changer l'obscurité de .01",

	L"MAJ +/-",  //20
	L"Changer l'obscurité de .05",

	L"0 - 9",
	L"Changer le nom de carte/tileset",

	L"b",
	L"Changer la taille du pinceau",

	L"d",
	L"Dessiner des débris",

	L"o",
	L"Dessiner des obstacles",

	L"r", //30
	L"Dessiner des rochers",

	L"t",
	L"Activer l'affichage des arbres. Marche/arrêt",

	L"g",
	L"Dessiner les textures du sol",

	L"w",
	L"Dessiner les murs des bâtiments",

	L"e",
	L"Activer le mode effacer. Marche/arrêt",

	L"h",  //40
	L"Activer les toits. Marche/arrêt",
};

STR16 pAutoLoadMapText[] =
{
	L"La carte vient d'être corrompue. N'enregistrez pas, ne quittez pas, demandez à parler à Kris ! S'il n'est pas là, sauvez la carte en utilisant un nom de fichier temporaire et documentez tout ce que vous venez de faire, surtout la dernière action !",
	L"Le calendrier vient d'être corrompu. N'enregistrez pas, ne quittez pas, demandez à parler à Kris ! S'il n'est pas là, sauvez la carte en utilisant un nom de fichier temporaire et documentez tout ce que vous venez de faire, surtout la dernière action !",
};

STR16 pShowHighGroundText[] =
{
	L"Afficher les marqueurs des textures de sol",
	L"Cacher les marqueurs des textures de sol",
};

//Item Statistics.cpp
CHAR16 gszActionItemDesc[ 34 ][ 30 ] =	// NUM_ACTIONITEMS = 34
{
	L"Mine klaxon",
	L"Mine Flash",
	L"Explosion lacrymo",
	L"Explosion Assourd.",
	L"Explosion Fumée",
	L"Gaz moutarde",
	L"Mine",
	L"Ouvrir porte",
	L"Fermer porte",
	L"Puit caché 3x3 ",
	L"Puit caché 5x5",
	L"Pt Explosion",
	L"Moy Explosion",
	L"Grd Explosion",
	L"Encl porte",
	L"Encl Action1s",
	L"Encl Action2s",
	L"Encl Action3s",
	L"Encl Action4s",
	L"Entrer ds bordel",
	L"Sortir du Bordel",
	L"Alarme sbire",
	L"Sexe avec pute",
	L"Montrer pièce",
	L"Alarme locale",
	L"Alarme globale",
	L"Bruit klaxon",
	L"Déver. porte",
	L"Encl verrou",
	L"Déminer porte",
	L"Encl Obj press",
	L"Alarme musée",
	L"Alarme chtsvg",
	L"Grd lacrymo",
};

//Item Statistics.cpp
/*
STR16 pUpdateItemStatsPanelText[] =
{
	L"Drapeaux M./A.", //0
	L"Aucun obj selec",
	L"Emp. dispo pour",
	L"Création aléatoire",
	L"Touche non modif",
	L"ID du proprio",
	L"Classe d'obj non incluse",
	L"Emp. vide verr",
	L"Statut",
	L"Balles",
	L"Niv piège", //10
	L"Quantité",
	L"Niv piège",
	L"Statut",
	L"Niv piège",
	L"Statut",
	L"Quantité",
	L"Niv piège",
	L"Dollars",
	L"Statut",
	L"Niv piège", //20
	L"Niv piège",
	L"Tolérance",
	L"Déclic alarme",
	L"Chance exist",// !!! Context
	L"B",// !!! Context
	L"R",
	L"S",
};
*/

STR16 pSetupGameTypeFlagsText[] =
{
	L"Obj apparaît en mode SF et réaliste. (|B)", //0
	L"Obj apparaît slt en mode |réaliste.",
	L"Obj apparaît slt en mode |SF.",
};

STR16 pSetupGunGUIText[] =
{
	L"SILENCIEUX", //0
	L"LNTTESNIPER",
	L"LNTTELASER",
	L"BIPIED",
	L"BCCANARD",
	L"LANCE-G", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"PLAQ CÉRAMIQUE", //0
};

STR16 pSetupExplosivesGUIText[] =
{
	L"DÉTONATEUR",
};

STR16 pSetupTriggersGUIText[] =
{
	L"Si le déclencheur de panique est une alarme,\nl'ennemi ne la sonnera pas\ns'il vous a déjà détecté.",
};

//Sector Summary.cpp

STR16 pCreateSummaryWindowText[]=
{
	L"Ok", //0
	L"A",
	L"G",
	L"B1",
	L"B2",
	L"B3", //5
	L"CHARGER",
	L"SAUVER",
	L"MISE À JOUR",
};

STR16 pRenderSectorInformationText[] =
{
	L"Tileset :  %s", //0
	L"Info. version :  Résumé :  1.%02d,  Carte :  %1.2f/%02d",
	L"Nombre d'objets :  %d", 
	L"Nombre de lumières :  %d",
	L"Nombre de points d'entrée :  %d",
	
	L"N",
	L"E",
	L"S",
	L"O",
	L"C",
	L"I", //10
	
	L"Nombre de pièces :  %d",
	L"Population totale :  %d",
	L"Ennemis :  %d",
	L"Admins :  %d",
	
	L"(%d detaillé, %d profil -- %d existe en priorité)",
	L"Troupes :  %d",
	
	L"(%d detaillé, %d profil -- %d existe en priorité)",
	L"Élites :  %d",
	
	L"(%d detaillé, %d profil -- %d existe en priorité)",
	L"Civils :  %d",  //20
	
	L"(%d detaillé, %d profil -- %d existe en priorité)",
	
	L"Humains :  %d",
	L"Vaches :  %d",
	L"Cht sauvg :  %d",
	
	L"Créatures :  %d",
	
	L"Monstres :  %d",
	L"Cht sauvg :  %d",
	
	L"Nombre de portes verr. et/ou piègées :  %d",
	L"Ver. :  %d",
	L"Piègées :  %d", //30
	L"Ver. & piègées :  %d",
	
	L"Civils prgrammés :  %d",
	
	L"Trop de destinations vers grille de sortie (plus de 4)...",
	L"GrilleSortie :  %d (%d vers destination longue dist)",
	L"GrilleSortie :  aucune",
	L"GrilleSortie :  1 destination utilisant %d GrilleSortie",
	L"GrilleSortie :  2 -- 1) Qté : %d, 2) Qté : %d",
	L"GrilleSortie :  3 -- 1) Qté : %d, 2) Qté : %d, 3) Qté : %d",
	L"GrilleSortie :  3 -- 1) Qté : %d, 2) Qté : %d, 3) Qté : %d, 4) Qté : %d",
	L"Car. relative ennemi :  %d mauvais, %d faible, %d norm, %d bon, %d super (%+d en tout)", //40
	L"Équipement relatif ennemi :  %d mauvais, %d faible, %d norm, %d bon, %d super (%+d en tout)",
	L"%d placements ont des ordres de patrouille, sans aucun waypoint défini.",
	L"%d placements ont des waypoints, mais sans aucun ordres de patrouille.",
	L"%d Numéro de grille ont un numéro de pièce étrange. Validez svp.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Ennemi",
	
	L"TROP D'OBJETS À L'ÉCRAN!",
	
	L"Panique1",
	L"Panique2",
	L"Panique3",
	L"Normal1",
	L"Normal2",
	L"Normal3",
	L"Normal4", //10
	L"Actions appuyer",
	
	L"TROP D'OBJETS À L'ÉCRAN !",
	
	L"OBJ LÂCHÉ PR ENMI PRIOR",
	L"Rien",
	
	L"TROP D'OBJETS À L'ÉCRAN !",
	L"OBJ LÂCHÉ PR ENMI NORM",
	L"TROP D'OBJETS À L'ÉCRAN !",
	L"Rien",
	L"TROP D'OBJETS À L'ÉCRAN !",
	L"ERREUR :  Imposs charg obj sur carte. Raison inconn.", //20	
};

STR16 pRenderSummaryWindowText[] =
{
	L"ÉDITEUR CAMPAGNE -- %s Version 1.%02d", //0
	L"(AUC MAP CHRGÉE).",
	L" %d de vos cartes sont obsolètes.",
	L"Plus de cartes à MAJ = plus de temps. Ça prend ",
	L"à peu près 4 min sur un P200MMX pour analyser 100 cartes, donc",
	L"la durée peu varier selon votre pc.",
	L"Voulez-vous MAJ TOUTES ces cartes (y/n) ?",
	
	L"Aucun secteur sélectionné.",
	
	L"Nom de fichier temp en conflit avec le format de l'éditeur de campagne",
	
	L"Il faut charger ou bien créer une carte afin d'entrer dans l'éditeur",
	L",ou bien vous pouvez quitter (ESC ou Alt+x).", //10

	L", RDC",					
	L", Sous-sol -1",	
	L", Sous-sol -2",	
	L", Sous-sol -3",	
	L", RDC alternatif",					
	L", Niv B1 alternatif",	
	L", Niv B2 alternatif",	
	L", Niv B3 alternatif",
	
	L"DÉTAILS OBJETS -- secteur %s",
	L"Les informations sommaires pour le secteur %s :", //20
	
	L"Les informations sommaires pour le secteur %s",
	L"n'existent pas.",
	
	L"Les informations sommaires pour le secteur %s",
	L"n'existent pas.",
	
	L"Pas d'informations existantes pour le secteur %s.",
	
	L"Pas d'informations existantes pour le secteur %s.",
	
	L"FICHIER :  %s",
	
	L"FICHIER :  %s",
	
	L"Outrepasser LECTURESEULE",//!!! Length limitation ? -> should be OK
	L"Écraser Fichier", //30
	
	L"Vous n'avez actuellement aucune donnée sommaire. En en créant un, vous pourrez garder la trace",
	L"des informations se rapportant à tous les secteurs que vous éditez et sauvez. La progression de la création",
	L"va analyser toutes les cartes contenu dans votre dossier \\MAPS, et en générer un nouveau. Cela peut prendre",
	L"quelques minutes, tout dépend de combien de carte valide vous avez. Les cartes valides sont",
	L"les cartes qui suivent la convention de nommage : a1.dat - p16.dat.  Les cartes souterraines",
	L"sont identifiées en ajoutant _b1 à _b3 avant le .dat (ex :  a9_b1.dat). ",
	
	L"Voulez-vous faire cela maintenant ?(o/n)?",
	
	L"Aucun renseignement sommaire. Création refusée.",
	
	L"Grille",
	L"Progression", //40
	L"Utilisez une carte alternative",
	
	L"Résumé",
	L"Objets",
};

STR16 pUpdateSectorSummaryText[] =
{
	L"Analyse de la carte :  %s...",
};

STR16 pSummaryLoadMapCallbackText[] =
{
	L"Chargement de la carte :  %s",
};

STR16 pReportErrorText[] =
{
	L"Pas de MAJ pour %s. probablement dû à des conflits tilesets",
};

STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Production d'informations sur la carte",
};

STR16 pSummaryUpdateCallbackText[] =
{
	L"Production de résumé de la carte",
};

STR16 pApologizeOverrideAndForceUpdateEverythingText[] =
{
	L"MISE À JOUR VERSION MAJEURE",
	L"Il y a %d cartes qui requièrent une mise à jour majeure.",
	L"Mise à jour de toutes les cartes obsolètes",
};

//selectwin.cpp
STR16 pDisplaySelectionWindowGraphicalInformationText[] =
{
	L"%S[%d] appartient à tileset def %s (%S)",
	L"Fichier :  %S, sousindex :  %d (%S)",
	L"Tileset utilisé :  %s",
};

STR16 pDisplaySelectionWindowButtonText[] =
{
	L"Confirmer les choix (|E|N|T|R|É|E)",
	L"Annuler les choix (|E|S|C)\nEffacer les choix (|E|S|P|A|C|E)",
	L"Faire défiler la fenêtre vers le haut (|H|A|U|T)",
	L"Faire défiler la fenêtre vers le bas (|B|A|S)",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Petit", 
 L"Moyen", 
 L"Large", 
 L"Très large", 
 L"Largeur : xx", 
 L"Secteur" 
 };

//---

CHAR16  gszAimPages[ 6 ][ 20 ] =
{
	L"Page 1/2", //0
	L"Page 2/2",
	
	L"Page 1/3",
	L"Page 2/3",
	L"Page 3/3",
	
	L"Page 1/1", //5
};

CHAR16 zGrod[][500] =
{
	L"Robot", //0    // Robot
};

STR16 pCreditsJA2113[] =
{
	L"@T,{;JA2 v1.13 Eq Développeurs",
	L"@T,C144,R134,{;Code",
	L"@T,C144,R134,{;Graphismes et Sons",
	L"@};(Autre mods!)",
	L"@T,C144,R134,{;Objets",
	L"@T,C144,R134,{;Autre contributeurs",
	L"@};(Tous les membres de la communauté qui ont contribué !)",
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
//	L"9mm",
//	L"cal .45",
//	L"cal .357",
//	L"cal 12",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm OTAN",
//	L"7.62mm PV",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monstre",
//	L"Roquette",
//	L"", // dart
//	L"", // flame
//	L".50 cal", // barrett
//	L"9mm Lrd", // Val silent
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
//	L"9mm",
//	L"cal .45",
//	L"cal .357",
//	L"cal 12",
//	L"CAWS",
//	L"5.45mm",
//	L"5.56mm",
//	L"7.62mm O.",
//	L"7.62mm PV",
//	L"4.7mm",
//	L"5.7mm",
//	L"Monstre",
//	L"Roquette",
//	L"", // dart
//	L"", // Lance-Flammes
//	L".50 cal", // barrett
//	L"9mm Lrd", // Val silent
//};


CHAR16 WeaponType[][30] =
{
	L"Divers",
	L"Pistolet",
	L"Pistolet-Mitrailleur",
	L"Mitraillette",
	L"Fusil",
	L"Fusil de précision",
	L"Fusil d'assaut",
	L"Fusil-mitrailleur",
	L"Fusil à pompe",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Tour du joueur", // player's turn
	L"Tour de l'ennemi",
	L"Tour des créatures",
	L"Tour de la milice",
	L"Tour des civils",
	L"Plan_joueur",// planning turn
	L"Client #1",//hayden
	L"Client #2",//hayden
	L"Client #3",//hayden
	L"Client #4",//hayden

};

CHAR16 Message[][STRING_LENGTH] =
{
	L"",

	// In the following 8 strings, the %s is the merc's name, and the %d (if any) is a number.

	L"%s est touché(e) à la tête et perd un point de sagesse !",
	L"%s est touché(e) à l'épaule et perd un point de dextérité !",
	L"%s est touché(e) à la poitrine et perd un point de force !",
	L"%s est touché(e) à la jambe et perd un point d'agilité !",
	L"%s est touché(e) à la tête et perd %d points de sagesse !",
	L"%s est touché(e) à l'épaule et perd %d points de dextérité !",
	L"%s est touché(e) à la poitrine et perd %d points de force !",
	L"%s est touché(e) à la jambe et perd %d points d'agilité !",
	L"Interruption !",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"Les renforts sont arrivés !",

	// In the following four lines, all %s's are merc names

	L"%s recharge.",
	L"%s n'a pas assez de points d'action !",
	L"%s applique les premiers soins (Appuyez sur une touche pour annuler).",
	L"%s et %s appliquent les premiers soins (Appuyez sur une touche pour annuler).",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"fiable",
	L"peu fiable",
	L"facile à entretenir",
	L"difficile à entretenir",
	L"puissant",
	L"peu puissant",
	L"cadence de tir élevée",
	L"faible cadence de tir",
	L"longue portée",
	L"courte portée",
	L"léger",
	L"lourd",
	L"petit",
	L"tir en rafale",
	L"pas de tir en rafale",
	L"grand chargeur",
	L"petit chargeur",

	// In the following two lines, all %s's are merc names

	L"Le camouflage de %s s'est effacé.",
	L"Le camouflage de %s est parti.",

	// The first %s is a merc name and the second %s is an item name

	L"La deuxième arme est vide !",
	L"%s a volé le/la %s.",

	// The %s is a merc name

	L"L'arme de %s ne peut pas tirer en rafale.",

	L"Vous avez déjà ajouté cet accessoire.",
	L"Combiner les objets ?",

	// Both %s's are item names

	L"Vous ne pouvez combiner un(e) %s avec un(e) %s.",

	L"Aucun",
	L"Retirer chargeur",
	L"Accessoire(s) ",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and your gas mask at the same time.
	L"Vous ne pouvez utiliser votre %s et votre %s simultanément.",

	L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre emplacements disponibles.",
	L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre emplacements disponibles (Ici, cet accessoire n'est pas compatible avec cet objet).",
	L"Ce secteur n'a pas été sécurisé !",
	L"Vous devez donner %s à %s",//inverted !! you still need to give the letter to X
	L"%s a été touché(e) à la tête !",
	L"Cesser le combat ?",
	L"Cet accessoire ne pourra plus être enlevé. Désirez-vous toujours le mettre ?",
	L"%s se sent beaucoup mieux !",
	L"%s a glissé sur des billes !",
	L"%s n'est pas parvenu(e) à ramasser le/la %s !",
	L"%s a réparé le/la %s",
	L"Interruption pour ",
	L"Voulez-vous vous rendre ?",
	L"Cette personne refuse votre aide.",
	L"JE NE CROIS PAS !",
	L"Pour utiliser l'hélicoptère de Skyrider, vous devez ASSIGNER vos mercenaires au VÉHICULE.",
	L"%s ne peut recharger qu'UNE arme",
	L"Tour des chats s.",
	L"automatique",
	L"Pas de tir auto",
	L"précis",
	L"peu précis",
	L"Pas de coup par coup",
	L"Plus rien à voler sur l'ennemi !",
	L"L'ennemi n'a rien dans les mains !",

	L"%s a perdu son camouflage désert.",
	L"%s a perdu son camouflage désert à cause de l'eau.",
	
	L"%s a perdu son camouflage forêt.",
	L"%s a perdu son camouflage forêt à cause de l'eau.",
	
	L"%s a perdu son camouflage urbain.",
	L"%s a perdu son camouflage urbain à cause de l'eau.",
	
	L"%s a perdu son camouflage neige.",
	L"%s a perdu son camouflage neige à cause de l'eau.",

	L"Vous ne pouvez pas attacher %s à cet emplacement.",
	L"%s n'ira dans aucun emplacement de libre.",
	L"Pas assez de place pour cette poche.",

	L"%s a réparé au mieux : %s.",
	L"%s a aidé %s pour réparer au mieux : %s.",

	L"%s has cleaned the %s.",	// TODO.Translate
	L"%s has cleaned %s's %s.",

	L"Assignment not possible at the moment",	// TODO.Translate
	L"No militia that can be drilled present.",

	L"%s has fully explored %s.",	// TODO.Translate
};

// the country and its noun in the game		// TODO.Translate
CHAR16 pCountryNames[][MAX_TOWN_NAME_LENGHT] =
{
#ifdef JA2UB
	L"Tracona",
	L"traconien(ne)",  // TODO.Translate //A voir fini (to see finished)
#else
	L"Arulco",
	L"arulcain(e)",  // TODO.Translate //A voir fini (to see finished)
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
	L"6 H", 
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Esc. 1",
	L"Esc. 2",
	L"Esc. 3",
	L"Esc. 4",
	L"Esc. 5",
	L"Esc. 6",
	L"Esc. 7",
	L"Esc. 8",
	L"Esc. 9",
	L"Esc. 10",
	L"Esc. 11",
	L"Esc. 12",
	L"Esc. 13",
	L"Esc. 14",
	L"Esc. 15",
	L"Esc. 16",
	L"Esc. 17",
	L"Esc. 18",
	L"Esc. 19",
	L"Esc. 20",
	L"Service", // on active duty
	L"Docteur", // administering medical aid
	L"Patient(e)", // getting medical aid
	L"Transport", // in a vehicle
	L"Transit", // in transit - abbreviated form
	L"Réparat.", // repairing
	L"Radio",	// scanning for nearby patrols
	L"Formation", // training themselves  
	L"Milice", // training a town to revolt
	L"Milice M.", //training moving militia units	
	L"Entraîneur", // training a teammate //!!! Too long ? (11 char) -> 11 chars is OK
	L"Élève", // being trained by someone else 
	L"Dépl obj.",	// move items
	L"Renseign.", // operating a strategic facility
	L"Mange",		// eating at a facility (cantina etc.)
	L"Repos", // Resting at a facility
	L"Prison",		// Flugente: interrogate prisoners
	L"Mort(e)", // dead
	L"Invalide", // abbreviation for incapacitated
	L"Capturé(e)", // Prisoner of war - captured
	L"Hôpital", // patient in a hospital 
	L"Vide",	// Vehicle is empty
	L"Infiltré",	// facility: undercover prisoner (snitch)
	L"Propag.",	// facility: spread propaganda
	L"Propag.",	// facility: spread propaganda (globally)
	L"Rumeur",	// facility: gather information
	L"Propag.",	// spread propaganda
	L"Rumeur",	// gather information
	L"Command.",	// militia movement orders
	L"Diagnose", // disease diagnosis	//TODO.Translate
	L"Treat D.", // treat disease among the population
	L"Docteur", // administering medical aid
	L"Patient(e)", // getting medical aid
	L"Réparat.", // repairing
	L"Fortify",		// build structures according to external layout	// TODO.Translate
	L"Train W.",
	L"Hide",	// TODO.Translate
	L"GetIntel",
	L"DoctorM.",
	L"DMilitia",
	L"Burial",
	L"Admin", // TODO.Translate
	L"Explore",	// TODO.Translate
	L"Event",// rftr: merc is on a mini event // TODO: translate
	L"Mission", // rftr: rebel command
};


STR16 pMilitiaString[] =
{
	L"Milice", // the title of the militia box
	L"Disponibles", //the number of unassigned militia troops
	L"Vous ne pouvez réorganiser la milice lors d'un combat !",
	L"Des milices ne sont pas assignées à un secteur. Voulez-vous les démobiliser ?",//!!! Too long ? (80 char) -> it is OK
};


STR16 pMilitiaButtonString[] =
{
	L"Auto", // auto place the militia troops for the player
	L"OK", // done placing militia troops
	L"Démobiliser", // HEADROCK HAM 3.6: Disband militia
	L"Tout ôter", // move all milita troops to unassigned pool
};

STR16 pConditionStrings[] = 
{
	L"Excellent", //the state of a soldier .. excellent health
	L"Bon", // good health
	L"En forme", // fair health
	L"Blessé(e)", // wounded health
	L"Fatigué(e)", // tired
	L"Épuisé(e)", // bleeding to death
	L"Inconscient(e)", // knocked out 
	L"Mourant(e)", // near death
	L"Mort(e)", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"Service", // set merc on active duty
	L"Patient(e)", // set as a patient to receive medical aid
	L"Transport", // tell merc to enter vehicle
	L"Laisser", // let the escorted character go off on their own
	L"Annuler", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Esc. 1",
	L"Esc. 2",
	L"Esc. 3",
	L"Esc. 4",
	L"Esc. 5",
	L"Esc. 6",
	L"Esc. 7",
	L"Esc. 8",
	L"Esc. 9",
	L"Esc. 10",
	L"Esc. 11",
	L"Esc. 12",
	L"Esc. 13",
	L"Esc. 14",
	L"Esc. 15",
	L"Esc. 16",
	L"Esc. 17",
	L"Esc. 18",
	L"Esc. 19",
	L"Esc. 20",
	L"Service",
	L"Docteur",
	L"Patient",
	L"Transport",
	L"Transit",
	L"Réparation",
	L"Radioécouteur",	// radio scan
	L"Formation", 
	L"Milice",
	L"Forme la milice mobile",//!!! Too long ? -> It is OK
	L"Entraîneur",
	L"Élève",
	L"Déplace les objets",	// move items
	L"Renseignement",	//!!! Idem ? -> The current translation is OK
	L"Mange",	 // eating at a facility (cantina etc.)
	L"Repos",
	L"Interroge prisonier(s)",	 // Flugente: interrogate prisoners
	L"Mort(e)",
	L"Invalide",
	L"Capturé(e)",
	L"Hôpital", 
	L"Vide",	// Vehicle is empty
	L"Infiltré",	// facility: undercover prisoner (snitch) // TODO.Translate //A voir fini (to see finished)
	L"Répand une propagande", // facility: spread propaganda // TODO.Translate //A voir fini (to see finished)
	L"Fait de la propagande", // facility: spread propaganda (globally) // TODO.Translate //A voir fini (to see finished)
	L"Collecte les rumeurs",	 // facility: gather rumours	 // TODO.Translate //A voir fini (to see finished)
	L"Propagande",	 // spread propaganda // TODO.Translate //A voir fini (to see finished)
	L"Rumeur",	 // gather information // TODO.Translate //A voir fini (to see finished) 
	L"Commande",	// militia movement orders
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Docteur",
	L"Patient",
	L"Réparation",
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
	L"Esc. 1",
	L"Esc. 2",
	L"Esc. 3",
	L"Esc. 4",
	L"Esc. 5",
	L"Esc. 6",
	L"Esc. 7",
	L"Esc. 8",
	L"Esc. 9",
	L"Esc. 10",
	L"Esc. 11",
	L"Esc. 12",
	L"Esc. 13",
	L"Esc. 14",
	L"Esc. 15",
	L"Esc. 16",
	L"Esc. 17",
	L"Esc. 18",
	L"Esc. 19",
	L"Esc. 20",
	L"Service",
	L"Docteur",
	L"Patient",
	L"Transport",
	L"Transit",
	L"Réparation",
	L"Radioécouteur",	// radio scan
	L"Formation", 
	L"Milice",
	L"Milice mobile",
	L"Entraîneur",
	L"Elève",
	L"Déplacer les objets",	// move items
	L"Renseignement",	//!!! Idem ? -> Current translation is OK
	L"Repos",
	L"Interroger captif(s)",	 // Flugente: interrogate prisoners
	L"Mort(e)",
	L"Invalide",
	L"Capturé(e)",
	L"Hôpital", // patient in a hospital 
	L"Vide",	// Vehicle is empty
	L"Infiltré", // facility: undercover prisoner (snitch) // TODO.Translate //A voir fini (to see finished)
	L"Répandre une propagande", // facility: spread propaganda // TODO.Translate //A voir fini (to see finished)
	L"Faire de la propagande", // facility: spread propaganda (globally) // TODO.Translate //A voir fini (to see finished)
	L"Récolter les rumeurs",	 // facility: gather rumours // TODO.Translate //A voir fini (to see finished)
	L"Propagande", // spread propaganda
	L"Rumeurs", // gather information 
	L"Commander",	// militia movement orders
	L"Diagnose", // disease diagnosis
	L"Treat Population disease", // treat disease among the population
	L"Docteur",
	L"Patient",
	L"Réparation",
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
	L"Options du contrat :", 
	L"", // a blank line, required
	L"Extension 1 jour", // offer merc a one day contract extension
	L"Extension 1 semaine", // 1 week
	L"Extension 2 semaines", // 2 week
	L"Renvoyer", // end merc's contract
	L"Annuler", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"Capturé(e)",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
	L"FORCE", 
	L"DEXTÉRITÉ",  
	L"AGILITÉ", 
	L"SAGESSE", 
	L"PRÉCISION",//!!! Accurate but not very good. Char limit ? -> 12 characters is the limit
	L"MÉDECINE",
	L"MÉCANIQUE",
	L"COMMANDEMENT",
	L"EXPLOSIFS",
	L"NIVEAU",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Protec.", // the armor rating of the merc
	L"Poids", // the weight the merc is carrying
	L"Cam.", // the merc's camouflage rating
	L"Camouflage :",
	L"Protection :",
};

STR16 pShortAttributeStrings[] =
{
	L"Agi", // the abbreviated version of : agilité
	L"Dex", // dextérité
	L"For", // strength
	L"Com", // leadership
	L"Sag", // sagesse
	L"Niv", // experience level
	L"Tir", // marksmanship skill
	L"Méc", // mechanical skill
	L"Exp", // explosive skill
	L"Méd", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Affectation", // the mercs current assignment 
	L"Contrat", // the contract info about the merc
	L"Santé", // the health level of the current merc
	L"Moral", // the morale of the current merc
	L"Cond.",	// the condition of the current vehicle
	L"Carb.",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Formation", // tell merc to train self 
	L"Milice", // tell merc to train town 
	L"Entraîneur", // tell merc to act as trainer
	L"Élève", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Cadence de tir :", // the allowable rate of fire for a merc who is guarding
	L"Feu à volonté", // the merc can be aggressive in their choice of fire rates
	L"Économiser munitions", // conserve ammo 
	L"Tir restreint", // fire only when the merc needs to 
	L"Autres Options :", // other options available to merc
	L"Retraite", // merc can retreat
	L"Abri",  // merc is allowed to seek cover
	L"Assistance", // merc can assist teammates
	L"OK", // done with this menu
	L"Annuler", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Cadence de tir :",
	L"*Feu à volonté*",
	L"*Économiser munitions*",
	L"*Tir restreint*",
	L"Autres Options :",
	L"*Retraite*",
	L"*Abri*",
	L"*Assistance*",
	L"OK",
	L"Annuler",
};

STR16 pAssignMenuStrings[] =
{
	L"Service", // merc is on active duty
	L"Docteur", // the merc is acting as a doctor
	L"Disease", // merc is a doctor doing diagnosis TODO.Translate
	L"Patient(e)", // the merc is receiving medical attention
	L"Transport", // the merc is in a vehicle
	L"Réparat.", // the merc is repairing items 
	L"Radio", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Infiltré",   // anv: snitch actions
	L"Formation", // the merc is training
	L"Militia",		// all things militia
	L"Dépl obj.",	// move items
	L"Fortify",		// fortify sector	// TODO.Translate
	L"Intel", // covert assignments	// TODO.Translate
	L"Administer",	// TODO.Translate
	L"Explore",	// TODO.Translate
	L"Affectat.", // the merc is using/staffing a facility
	L"Annuler", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Attaquez", // set militia to aggresive
	L"Tenez la position", // set militia to stationary
	L"Retraite", // retreat militia
	L"Rejoignez-moi", // retreat militia
	L"Couchez-vous", // retreat militia	
	L"Accroupi",
	L"À couvert !",
	L"Move to",	// TODO.Translate
	L"Tous : Attaquez", 
	L"Tous : Tenez la position",
	L"Tous : Retraite",
	L"Tous : Rejoignez-moi",
	L"Tous : Dispersez-vous",
	L"Tous : Couchez-vous",
	L"Tous : Accroupi",
	L"Tous : À couvert!",
	//L"All: Trouver matériel",
	L"Annuler", // cancel this menu
};

//Flugente
STR16 pTraitSkillsMenuStrings[] =
{
	// radio operator
	L"Tir d'artillerie",
	L"Brouiller les communications",
	L"Balayer les fréquences",
	L"Écouter les alentours",
	L"Appeler des renforts",
	L"Éteindre la radio",
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
	L"Guetteur",
	L"Focus",		// TODO.Translate
	L"Drag",
	L"Fill canteens",
};

//Flugente: short description of the above skills for the skill selection menu
STR16 pTraitSkillsMenuDescStrings[] =
{
	// radio operator
	L"Ordonner un tir d'artillerie au secteur...",
	L"Brouiller toutes les fréquences radios pour rendre les communications impossibles.",
	L"Pour trouver une fréquence émettrice.",
	L"Utiliser votre radio pour connaître les mouvements de l'ennemi.",
	L"Appeler des renforts des secteurs voisins.",
	L"Éteindre la radio.",
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
	L"Observer une zone avec un tireur d'élite donne un bonus de CDT sur tout ce que vous voyez.",
	L"Increase interrupt modifier (penalty outside of area).",	// TODO.Translate
	L"Drag a person, corpse or structure while you move.",
	L"Refill your squad's canteens with water from this sector.",
};

STR16 pTraitSkillsDenialStrings[] =
{
	L"Nécessite :\n",
	L" - %d PA\n",
	L" - %s\n",
	L" - %s ou plus\n",
	L" - %s ou plus ou",
	L" - %d minutes pour être prêt\n",
	L" - positions des mortiers dans les secteurs voisins\n",
	L" - %s |o|u %s |e|t %s ou %s ou plus\n",
	L" - possession par un démon",
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

STR16 pSkillMenuStrings[] =
{
	L"Milice",
	L"Autre escouade",
	L"Annuler",
	L"%d miliciens",
	L"Tous",

	L"More",	// TODO.Translate
	L"Corpse: %s",	// TODO.Translate
};

STR16 pSnitchMenuStrings[] =
{
	// snitch
	L"Infiltrer l'équipe",
	L"Infiltrer la ville",	
	L"Annuler",
};

STR16 pSnitchMenuDescStrings[] =
{
	// snitch
	L"Discuter du comportement du mouchard vis-à-vis de ses coéquipiers.",
	L"Prendre une mission dans ce secteur.",
	L"Annuler",
};

STR16 pSnitchToggleMenuStrings[] =
{
	// toggle snitching
	L"Rapporter les plaintes",
	L"Ne pas rapporter",
	L"Prévenir les mauvaises conduites",
	L"Ignorer les mauvaises conduites",
	L"Annuler",
};

STR16 pSnitchToggleMenuDescStrings[] =
{
	L"Signaler toute plainte des autres mercenaires à votre commandant.",
	L"Ne rien signaler.",
	L"Essayer d'empêcher les chapardages et la prise de produits addictifs des autres mercenaires.",
	L"Ne pas se soucier de ce que font les autres mercenaires.",
	L"Annuler",
};

STR16 pSnitchSectorMenuStrings[] =
{
	// sector assignments
	L"Fait de la propagande",   // TODO.Translate //A voir fini (to see finished)
	L"Récolte les rumeurs",   // TODO.Translate //A voir fini (to see finished)
	L"Annuler",
};

STR16 pSnitchSectorMenuDescStrings[] =
{
	L"Glorifier les actions des mercenaires pour accroître la fidélité de la ville et étouffer toute mauvaises nouvelles.",
	L"Prêter une oreille attentive aux rumeurs sur l'activité des forces ennemies.",
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
	L"%s s'est exposé(e) comme mouchard, mais s'en est rendu compte et a pu s'en sortir vivant.",
	L"%s s'est exposé(e) comme mouchard, mais a réussi à désamorcer la situation et a pu s'en sortir vivant.",
	L"%s s'est exposé(e) comme mouchard, mais a réussi à éviter la tentative d'assassinat.",
	L"%s s'est exposé(e) comme mouchard, mais les gardes ont empêché tout débordement violent.",

	L"%s s'est exposé(e) comme mouchard et a failli être noyé par les autres détenus avant que les gardes ne le/la sauvent.",
	L"%s s'est exposé(e) comme mouchard et a failli se faire battre à mort avant que les gardes ne le/la sauvent.",
	L"%s s'est exposé(e) comme mouchard et a failli se faire poignarder avant que les gardes ne le/la sauvent.",
	L"%s s'est exposé(e) comme mouchard et a failli se faire étrangler avant que les gardes ne le/la sauvent.",

	L"%s s'est exposé(e) comme mouchard et a été noyé(e) dans les toilettes par les autres détenus.",
	L"%s s'est exposé(e) comme mouchard et a été battu(e) à mort par les autres détenus.",
	L"%s s'est exposé(e) comme mouchard et les autres détenus l'ont poignardé(e).",
	L"%s s'est exposé(e) comme mouchard et les autres détenus l'ont étranglé(e).",
};

STR16 pSnitchGatheringRumoursResultStrings[] =
{
	L"%s a entendu des rumeurs sur une activité ennemie dans le secteur %d.",

};

STR16 pRemoveMercStrings[] =
{
	L"Enlever Merc", // remove dead merc from current team
	L"Annuler",
};

STR16 pAttributeMenuStrings[] =
{
	L"Santé",
	L"Agilité",
	L"Dextérité",
	L"Force",
	L"Commandement",
	L"Tir",
	L"Mécanique",
	L"Explosifs",
	L"Médecine",
	L"Annuler",
};

STR16 pTrainingMenuStrings[] =
{
	L"Formation", // train yourself 
	L"Train workers",	// TODO.Translate
	L"Entraîneur", // train your teammates 
	L"Élève",  // be trained by an instructor 
	L"Annuler", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Esc. 1",
	L"Esc. 2",
	L"Esc. 3",
	L"Esc. 4",
	L"Esc. 5",
	L"Esc. 6",
	L"Esc. 7",
	L"Esc. 8",
	L"Esc. 9",
	L"Esc. 10",
	L"Esc. 11",
	L"Esc. 12",
	L"Esc. 13",
	L"Esc. 14",
	L"Esc. 15",
	L"Esc. 16",
	L"Esc. 17",
	L"Esc. 18",
	L"Esc. 19",
	L"Esc. 20",
	L"Annuler",
};

STR16 pPersonnelTitle[] =
{
	L"Personnel", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Santé : ", // health of merc
	L"Agilité : ", 
	L"Dextérité : ",
	L"Force : ",
	L"Commandement : ",
	L"Sagesse : ",
	L"Niv. Exp. : ", // experience level
	L"Tir : ", 
	L"Mécanique : ",
	L"Explosifs : ",
	L"Médecine : ",
	L"Acompte méd. : ", // amount of medical deposit put down on the merc
	L"Contrat : ", // cost of current contract
	L"Tué : ", // number of kills by merc
	L"Participation : ", // number of assists on kills by merc //!!!ugly. Char limit ? -> 17 chars
	L"Coût/jour :", // daily cost of merc
	L"Coût total :", // total cost of merc
	L"Contrat :", // cost of current contract
	L"Service fait :", // total service rendered by merc
	L"Salaire :", // amount left on MERC merc to be paid
	L"Tir réussi :", // percentage of shots that hit target
	L"Bataille :", // number of battles fought
	L"Blessure :", // number of times merc has been wounded
	L"Spécialité :",
	L"Aucune spécialité",
	L"Réalisation :", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Soldat d'élite : %d\n",
	L"Soldat régulier : %d\n",
	L"Soldat d'administration : %d\n",
	L"Groupe hostile : %d\n",
	L"Créature : %d\n",
	L"Char : %d\n", 
	L"Autre : %d\n",

	L"Mercenaire : %d\n",
	L"Milice : %d\n",
	L"Autre : %d\n",

	L"Coup tiré : %d\n",
	L"Roquette tirée : %d\n",
	L"Grenade lancée : %d\n",
	L"Couteau lancé : %d\n",
	L"Attaque à l'arme blanche : %d\n",
	L"Attaque à mains nues : %d\n",
	L"Tir réussi : %d\n",

	L"Serrure crochetée : %d\n",
	L"Serrure fracturée : %d\n",
	L"Piège retiré : %d\n",
	L"Bombe explosée : %d\n",
	L"Objet réparé : %d\n",
	L"Objet combiné : %d\n",
	L"objet volé : %d\n",
	L"Milice entrainée : %d\n",
	L"Merc. soigné : %d\n",
	L"Chirurgie faite : %d\n",
	L"Personne rencontrée : %d\n",
	L"Secteur visité : %d\n",
	L"Embuscade empêchée : %d\n",
	L"Quête faite : %d\n",//!!!Ugly. Char limit ? -> Total: 28 chars

	L"Tactique de combat : %d\n",
	L"Combat auto-résolu : %d\n",
	L"Temps écoulé : %d\n",
	L"Embuscade expérimentée : %d\n",
	L"Meilleur combat : %d ennemis\n",//!!! Ennemies/Ennemis ? -> whatever you like

	L"Tir : %d\n",
	L"Arme blanche : %d\n",
	L"Bagarre : %d\n",
	L"Explosion : %d\n",
	L"Grave : %d\n",
	L"Chirurgie subie : %d\n",
	L"Accident : %d\n",

	L"Caractère :",
	L"Faiblesse :",

	L"Attitude :",	// WANNE: For old traits display instead of "Character:"!

	L"Zombi : %d\n",

	L"Passif :",
	L"Personnalité :",

	L"Prisoners interrogated: %d\n",	// TODO.Translate
	L"Diseases caught: %d\n",
	L"Total damage received: %d\n",
	L"Total damage caused: %d\n",
	L"Total healing: %d\n",
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Aucune spécialité",
	L"Crochetage",
	L"Combat à mains nues",
	L"Électronique",
	L"Opérations de nuit",
	L"Le lancer",
	L"Instructeur",
	L"Arme lourde",
	L"Arme automatique",
	L"Discrétion",
	L"Ambidextre",
	L"Voleur",
	L"Arts martiaux",
	L"Arme blanche",
	L"Tireur d'élite",
	L"Camouflage",					
	L"(Expert)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Aucune spécialité",
	L"Arme automatique",
	L"Arme lourde",
	L"Tireur d'élite",
	L"Reconnaissance",// Hunter
	L"Flingueur",
	L"Combat à mains nues",
	L"Meneur",
	L"Technicien",
	L"Médecin",
	// Minor traits
	L"Ambidextre",
	L"Mêlée",
	L"Le lancer",
	L"Opérations de nuit",
	L"Discrétion",
	L"Athlétique",
	L"Culturiste",
	L"Sabotage",
	L"Instructeur",
	L"Reconnaissance",
	// covert ops is a major trait that was added later
	L"Espionnage",		// 20

	// new minor traits
	L"Opérateur radio",	// 21
	L"Infiltré",	// 22
	L"Survival",

	// second names for major skills
	L"Mitrailleur",		// 24
	L"Bombardier",
	L"Sniper",
	L"Ranger",
	L"Combattant",
	L"Arts martiaux",
	L"Commandant",
	L"Ingénieur",
	L"Chirurgien",
	// placeholders for minor traits
	L"Placeholder",		// 33
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 37
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",
	L"Placeholder",		// 41
	L"Espion",			// 42
	L"Placeholder",		// for radio operator (minor trait)
	L"Placeholder",		// for snitch (minor trait)
	L"Placeholder",		// for survival (minor trait)
	L"Plus...",        	// 47
	L"Intel",			// for INTEL	// TODO.Translate
	L"Disguise",		// for DISGUISE
	L"divers",			// for VARIOUSSKILLS
	L"Bandage Mercs",	// for AUTOBANDAGESKILLS	//TODO.Translate
};
//////////////////////////////////////////////////////////

// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"Debout/Marcher (|S)",
	L"Accroupi/Avancer (|C)",
	L"Debout/Courir (|R)",
	L"À terre/Ramper (|P)",
	L"Regarder (|L)",
	L"Action",
	L"Parler",
	L"Examiner (|C|T|R|L)",

	// Pop up door menu
	L"Ouvrir à la main",
	L"Examen poussé",
	L"Crocheter",
	L"Enfoncer",
	L"Désamorcer",
	L"Verrouiller",
	L"Déverrouiller",
	L"Utiliser explosif",
	L"Utiliser pied de biche",
	L"Annuler (|E|C|H|A|P)",
	L"Fermer",
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"aucun piège",
	L"un piège explosif",
	L"un piège électrique",
	L"une alarme sonore",
	L"une alarme silencieuse",
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"jour",
	L"semaine",
	L"2 semaines",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Choix du personnage",
	L"Affectation",
	L"Destination",
	L"|Contrat du mercenaire", //!!! Char typo before "Contrat" ?
	L"Retirer mercenaire",
	L"Repos", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"FAIBLE",
	L"MOYEN",
	L"FORT",
	L"TRÈS FORT",
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"INCONNU",
	L"MOUVEMENT",
	L"GRINCEMENT",
	L"CLAPOTEMENT",
	L"IMPACT",
	L"COUP DE FEU",
	L"EXPLOSION",
	L"CRI",
	L"IMPACT",
	L"IMPACT",
	L"BRUIT",
	L"COLLISION",
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"au NORD-EST",
	L"à l'EST",
	L"au SUD-EST",
	L"au SUD",
	L"au SUD-OUEST",
	L"à l'OUEST",
	L"au NORD-OUEST",
	L"au NORD",
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Ville",
	L"Route",
	L"Plaine",
	L"Désert",
	L"Bois",
	L"Forêt",	 
	L"Marais",
	L"Eau",	
	L"Collines",
	L"Infranchissable",
	L"Rivière",	//river from north to south
	L"Rivière",	//river from east to west
	L"Pays étranger",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Tropical",
	L"Cultures",
	L"Plaine, route",
	L"Bois, route",
	L"Ferme, route",
	L"Tropical, route",
	L"Forêt, route",
	L"Route côtière",
	L"Montagne, route",
	L"Côte, route",
	L"Désert, route",
	L"Marais, route",
	L"Bois, site SAM",
	L"Désert, site SAM",
	L"Tropical, site SAM",
	L"Meduna, site SAM",
	
	//These are descriptions for special sectors
	L"Hôpital Cambria",
	L"Aéroport Drassen",
	L"Aéroport Meduna",
	L"Site SAM",
	L"Dépôt",
	L"Base rebelle", //The rebel base underground in sector A10
	L"Prison Tixa",	//The basement of the Tixa Prison (J9)
	L"Repaire créatures",	//Any mine sector with creatures in it
	L"Sous-sol d'Orta",	//The basement of Orta (K4)
	L"Tunnel",				//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Abri",				//The shelter underneath the queen's palace !!! "Bunker" is much better here. Char limit ?
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s détecté dans le secteur %c%d et une autre escouade est en route.",	//STR_DETECTED_SINGULAR
	L"%s détecté dans le secteur %c%d et d'autres escouades sont en route.",	//STR_DETECTED_PLURAL
	L"Voulez-vous coordonner une arrivée simultanée ?",													//STR_COORDINATE

	//Dialog strings for enemies.

	L"L'ennemi vous propose de vous rendre.",			//STR_ENEMY_SURRENDER_OFFER
	L"L'ennemi a capturé vos mercenaires inconscients.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Retraite", 		//The retreat button				//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"DÉFENSEURS",								//STR_AR_DEFEND_HEADER
	L"ATTAQUANTS",								//STR_AR_ATTACK_HEADER
	L"RENCONTRE",								//STR_AR_ENCOUNTER_HEADER
	L"Secteur",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"VICTOIRE !",								//STR_AR_OVER_VICTORY
	L"DÉFAITE !",								//STR_AR_OVER_DEFEAT
	L"RÉDDITION !",							//STR_AR_OVER_SURRENDERED
	L"CAPTURE !",								//STR_AR_OVER_CAPTURED
	L"RETRAITE !",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Milice",							//STR_AR_MILITIA_NAME,
	L"Élite",								//STR_AR_ELITE_NAME,
	L"Troupe",								//STR_AR_TROOP_NAME,
	L"Admin",								//STR_AR_ADMINISTRATOR_NAME,
	L"Créature",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Temps écoulé",							//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"SE RETIRE",								//STR_AR_MERC_RETREATED,
	L"EN RETRAITE",								//STR_AR_MERC_RETREATING,
	L"RETRAITE",								//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Auto.",							//STR_PB_AUTORESOLVE_BTN,
	L"Combat",							//STR_PB_GOTOSECTOR_BTN,
	L"Retraite",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"ENNEMIS REPÉRÉS",							//STR_PB_ENEMYENCOUNTER_HEADER,
	L"ATTAQUE ENNEMIE",							//STR_PB_ENEMYINVASION_HEADER, // 30
	L"EMBUSCADE !",								//STR_PB_ENEMYAMBUSH_HEADER
	L"VOUS PÉNÉTREZ EN SECTEUR ENNEMI",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"ATTAQUE DE CRÉATURES",							//STR_PB_CREATUREATTACK_HEADER
	L"AMBUSCADE DE CHATS SAUVAGES",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"VOUS ENTREZ DANS LE REPAIRE DES CHATS SAUVAGES",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER
	L"ENEMY AIRDROP",					//STR_PB_ENEMYINVASION_AIRDROP_HEADER	// TODO.Translate

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Lieu",
	L"Ennemis",
	L"Mercs",
	L"Milice",
	L"Créatures",
	L"Chats",
	L"Secteur",
	L"Aucun",		//If there are non uninvolved mercs in this fight.
	L"N/A",			//Acronym of Not Applicable
	L"j",			//One letter abbreviation of day
	L"h",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Annuler",
	L"Dispersé",
	L"Groupé",
	L"OK",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Annule le déploiement des mercenaires \net vous permet de les déployer vous-même. (|C)", 
	L"Disperse aléatoirement vos mercenaires \nà chaque fois. (|S)",
	L"Vous permet de placer votre groupe de mercenaires. (|G)",
	L"Cliquez sur ce bouton lorsque vous avez déployé \nvos mercenaires. (|E|N|T|R|É|E)",
	L"Vous devez déployer vos mercenaires \navant d'engager le combat.",

	//Various strings (translate word for word)

	L"Secteur",
	L"Définissez les points d'entrée",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Il semblerait que l'endroit soit inaccessible...",
	L"Déployez vos mercenaires dans la zone en surbrillance.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"est arrivé(e) dans le secteur", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Résolution automatique du combat\nsans charger la carte. (|A)",
	L"Résolution automatique impossible lorsque\nvous attaquez.",
	L"Pénétrez dans le secteur pour engager le combat. (|E)",
	L"Battre en retraite vers le secteur précédent. (|R)",				//singular version
	L"Battre en retraite vers les secteurs précédents. (|R)", //multiple groups with same previous sector !!! Changed "Faire" to "Battre en". Char limit ?

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"L'ennemi attaque votre milice dans le secteur %c%d.",
	//%c%d is the sector -- ex:  A9
	L"Les créatures attaquent votre milice dans le secteur %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Les créatures ont tué %d civils dans le secteur %s.",
	//%s is the sector location -- ex:  A9: Omerta
	L"L'ennemi attaque vos mercenaires dans le secteur %s. Aucun de vos hommes ne peut combattre !",
	//%s is the sector location -- ex:  A9: Omerta
	L"Les créatures attaquent vos mercenaires dans le secteur %s. Aucun de vos hommes ne peut combattre !",

	// Flugente: militia movement forbidden due to limited roaming
	L"La milice ne peut pas se déplacer (RESTRICT_ROAMING = TRUE).",
	L"La salle d'opérations n'est pas ouverte... Le mouvement de la milice a avorté !",

	L"Robot",								//STR_AR_ROBOT_NAME, // TODO: translate
	L"Tank",								//STR_AR_TANK_NAME,	// TODO.Translate
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
	L"Jour",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Secteur :",
	L"Jour :",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 20 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"État :",
	L"Poids :", 
	L"P. d'action",	
	L"Por. :",		// Range
	L"Dég. :",		// Damage
	L"Munition :", 		// Number of bullets left in a magazine
	L"PA :",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Précision :",	//9
	L"Portée :",		//10	
	L"Dégât :", 	//11
	L"Poids :",		//12
	L"Choc :",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Accessoire :",	//14
	L"AUTO/5 :",		//15
	L"Balle rest. :",		//16
	L"Par défaut :",	//17 //WarmSteel - So we can also display default attachments
	L"Encras. :",	// 18	//added by Flugente
	L"Place :", // 19 //space left on Molle items
	L"Spread Pattern:",	// 20	// TODO.Translate

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
STR16		gzWeaponStatsFasthelpTactical[ 33 ] =
{
	L"|P|o|r|t|é|e\n \nC'est la portée de l'arme. Un tir, au-delà de\ncette valeur, donnera des pénalités.\n \nValeur élevée recommandée.",
	L"|D|é|g|â|t\n \nC'est la valeur des dégâts. L'arme infligera\ngénéralement cette valeur (ou presque)\nà toutes cibles non protégées.\n \nValeur élevée recommandée.",
	L"|P|r|é|c|i|s|i|o|n\n \nC'est la conception particulière de\nchaque arme qui donne ce bonus/pénalité\naux chances de toucher.\n \nValeur élevée recommandée.",
	L"|V|i|s|é|e\n \nC'est le nombre maximum de clics pour viser.\n \nChaque clic donnera un tir plus précis.\n \nValeur élevée recommandée.",
	L"|V|i|s|é|e |m|o|d|i|f|i|é|e\n \nLes lunettes de visée améliorent l'efficacité\nde l'arme à chaque clic.\n \nValeur élevée recommandée.",
	L"|P|o|r|t|é|e |m|i|n|i|m|u|m |d|u |v|i|s|e|u|r\n \nC'est la portée minimum pour tirer\navec un viseur ou +.\n \nSi la cible est plus près que ce nombre de cases,\nles clics pour viser resteront à leur efficacité\npar défaut.\n \nValeur faible recommandée.",
	L"|F|a|c|t|e|u|r |d|e |d|é|g|â|t|s\n \nLa valeur des chances de toucher une\ncible avec les accessoires de l'arme.\n \nValeur élevée recommandée.",
	L"|P|o|r|t|é|e |o|p|t|i|m|a|l|e |d|u |l|a|s|e|r\n \nC'est la portée (en cases) pour\nune pleine efficacité du laser.\n \nSi la cible est au-delà de ce nombre, le laser\nfournira un faible bonus ou pas du tout.\n \nValeur élevée recommandée.",
	L"|C|a|c|h|e|-|f|l|a|m|m|e\n \nQuand cette icône apparaît, ça signifie que\nl'arme ne fait pas d'éclair lors du tir.\nAinsi, le tireur sera moins repérable.",
	L"|S|o|n|o|r|i|t|é\n \nDistance (en cases) de l'intensité sonore\nque fait l'arme lors d'un tir.\n \nValeur faible recommandée.\n \n(Sauf pour attirer les ennemis délibérément...).",
	L"|F|i|a|b|i|l|i|t|é\n \nCette valeur indique (en général) la\nrapidité avec laquelle cette arme va\nse détériorer lors des combats.\n \nValeur élevée recommandée.",
	L"|R|é|p|a|r|a|t|i|o|n\n \nCette valeur indique la rapidité avec\nlaquelle l'arme sera réparée. (Par un\nmercenaire affecté à cette tâche).\n \nValeur élevée recommandée.",
	L"",	//12
	L"PA pour mettre en joue",
	L"PA par tir",
	L"PA par rafale",
	L"PA pour tir auto.",
	L"PA pour recharger",
	L"PA pour recharger manuellement",
	L"Pénalité rafale (Valeur faible recommandée)",	//19
	L"Facteur de bipied",
	L"Nombre de tirs pour 5 PA",
	L"Pénalité auto (Valeur faible recommandée)",
	L"Pénalité rafale/auto (Valeur faible recommandée)",	//23
	L"PA pour jeter",
	L"PA pour lancer",
	L"PA pour poignarder",
	L"Pas de tir simple !",
	L"Pas de tir en rafale !",
	L"Pas de tir auto !",
	L"PA pour frapper",
    L"",
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n\n \nDétermine la difficulté à réparer une arme\net qui peut la réparer complètement.\n \nVert = N'importe qui peut la réparer.\n \nJaune = Seuls des PNJ spécialisés peuvent la\nréparer au-delà du seuil de réparation.\n \nRouge = L'arme ne peut pas être réparée.\n \nValeur élevée recommandée.",
};

STR16		gzMiscItemStatsFasthelp[] =
{
	L"Facteur d'encombrement (Valeur faible recommandée)", // 0
	L"Facteur de fiabilité",
	L"Facteur d'intensité sonore (Valeur faible recommandée)",
	L"Cache-flamme",
	L"Facteur de bipied",
	L"Facteur de portée", // 5
	L"Facteur de toucher",
	L"Portée optimum du laser",
	L"Facteur de bonus de visée",
	L"Facteur de longueur de rafale",
	L"Facteur de pénalité de rafale (Valeur élevée recommandée)", // 10
	L"Facteur de pénalité tir auto. (Valeur élevée recommandée)",
	L"Facteur de PA",
	L"Facteur de PA rafale (Valeur faible recommandée)",
	L"Facteur de PA tir auto (Valeur faible recommandée)",
	L"Facteur de PA mise en joue (Valeur faible recommandée)", // 15
	L"Facteur de PA recharger (Valeur faible recommandée)",
	L"Facteur de capacité chargeur",
	L"Facteur de PA attaque (Valeur faible recommandée)",
	L"Facteur de dégâts",
	L"Facteur de dégâts mêlée", // 20
	L"Camouflage bois",
	L"Camouflage urbain",
	L"Camouflage désert",
	L"Camouflage neige",
	L"Facteur de discrétion", // 25
	L"Facteur de portée auditive",
	L"Facteur de portée visuelle",
	L"Facteur de portée visuelle jour",
	L"Facteur de portée visuelle nuit",
	L"Facteur de portée visuelle faible lumière", //30
	L"Facteur de portée visuelle cave",
	L"Pourcentage d'effet tunnel (Valeur faible recommandée)",
	L"Portée minimale pour bonus de visée",
	L"Maintenez |C|T|R|L pour comparer les objets", // item compare help text
	L"Equipment weight: %4.1f kg",	// 35	// TODO.Translate
};

// HEADROCK: End new tooltip text

// HEADROCK HAM 4: New condition-based text similar to JA1.
STR16 gConditionDesc[] =
{
	L"En ",
	L"PARFAITE",
	L"EXCELLENTE",
	L"BONNE",
	L"MOYENNE",
	L"FAIBLE",
	L"MAUVAISE",
	L"PITEUSE",
	L" condition."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Montant ",
	L"restant :", //this is the overall balance
	L"Montant ",
	L"pris :", // the amount he wants to separate from the overall balance to get two piles of money

	L"Solde",
	L"actuel :",
	L"Montant",
	L" à retirer :",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"MOURANT",		//	>= 0
	L"CRITIQUE", 		//	>= 15
	L"FAIBLE",		//	>= 30
	L"BLESSÉ",    	//	>= 45
	L"EN FORME",    	//	>= 60
	L"BON",     	// 	>= 75
	L"EXCELLENT",		// 	>= 90
	L"EN PRISON",	// added by Flugente
};

STR16 gzHiddenHitCountStr[1] =
{
	L" ?",	
};

STR16	gzMoneyAmounts[6] = 
{ 
	L"1000",
	L"100",
	L"10",
	L"OK",
	L"Partager",
	L"Retirer",
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
	L"Pardon ?", 	//meaning "Repeat yourself" 
	L"Amical",		//approach in a friendly
	L"Direct",		//approach directly - let's get down to business
	L"Menaçant",		//approach threateningly - talk now, or I'll blow your face off
	L"Donner",		
	L"Recruter",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Acheter/Vendre",
	L"Acheter",
	L"Vendre",
	L"Réparer",
};

CHAR16 zDialogActions[1][ SMALL_STRING_LENGTH ] = 
{
	L"OK",
};


//These are vehicles in the game.

STR16 pVehicleStrings[] =
{
 L"Eldorado",
 L"Hummer", // a hummer jeep/truck -- military vehicle
 L"Camion de glaces",
 L"Jeep",
 L"Char",
 L"Hélicoptère",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Camion",
	L"Jeep",
	L"Char",
	L"Hélico", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Camion",			// Ice cream truck
	L"Jeep",
	L"Char",
	L"Hélico", 		//an abbreviation for Helicopter
};

STR16 pVehicleSeatsStrings[] =
{
	 L"You cannot shoot from this seat.",	// TODO.Translate
	 L"You cannot swap those two seats in combat without exiting vehicle first.",	// TODO.Translate
};

//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Raid aérien",
	L"Appliquer les premiers soins ?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s a remarqué qu'il manque des objets dans cet envoi.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"La serrure est piégée par %s.", 
	L"Pas de serrure.",
	L"Réussite !",
	L"Échec.",
	L"Réussite !",
	L"Échec.",
	L"La serrure n'est pas piégée.",
	L"Réussite !",
	// The %s is a merc name
	L"%s ne possède pas la bonne clé.",
	L"Le piège est désamorcé.",
	L"La serrure n'est pas piégée.",
	L"Verrouillée.",
	L"PORTE",
	L"PIÉGÉE",
	L"VERROUILLÉE",
	L"OUVERTE",
	L"ENFONCÉE",
	L"Un interrupteur. Voulez-vous l'actionner ?",
	L"Désamorcer le piège ?",
	L"Préc...",
	L"Suiv...",
	L"Plus...",

	// In the next 2 strings, %s is an item name

	L"%s posé(e) à terre.",
	L"%s donné(e) à %s.",

	// In the next 2 strings, %s is a name

	L"%s a été payé(e).",
	L"%d dus à %s.",
	L"Choisir la fréquence :",  	//in this case, frequency refers to a radio signal
	L"Nombre de tours avant explosion :",	//how much time, in turns, until the bomb blows
	L"Régler la fréquence :", 	//in this case, frequency refers to a radio signal
	L"Désamorcer le piège ?",
	L"Enlever le drapeau bleu ?",
	L"Poser un drapeau bleu ?",
	L"Fin du tour",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Voulez-vous vraiment attaquer %s ?",
	L"Les véhicules ne peuvent changer de position.",
	L"Le robot ne peut changer de position.",

	// In the next 3 strings, %s is a name

	L"%s ne peut adopter cette position ici.",
	L"%s ne peut recevoir de premiers soins ici.",	
	L"%s n'a pas besoin de premiers soins.",
	L"Impossible d'aller ici.",
	L"Votre escouade est au complet. Vous ne pouvez pas ajouter quelqu'un.",	//there's non room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s a été recruté(e).",

	// Here %s is a name and %d is a number

	L"Vous devez %d $ à %s.",

	// In the next string, %s is a name

	L"Escorter %s ?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"Engager %s à %s la journée ?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Voulez-vous engager le combat ?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"Acheter %s pour %s ?",

	// In the next string, %s is a name

	L"%s est escorté(e) par l'escouade %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"ENRAYÉ",					//weapon is jammed.
	L"Le robot a besoin de munitions calibre %s.",		//Robot is out of ammo
	L"Lancer ici ? Aucune chance.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Discrétion (|Z)",
	L"Carte (|M)",
	L"Fin du tour (|D)",
	L"Parler à",
	L"Muet",
	L"Se relever (|P|g|U|p)",
	L"Niveau du curseur (|T|A|B)",
	L"Escalader/Sauter (|J)",
	L"Se coucher (|P|g|D|n)",
	L"Examiner (|C|T|R|L)",
	L"Mercenaire précédent",
	L"Mercenaire suivant (|E|S|P|A|C|E)",
	L"Options (|O)",
	L"Rafale (|B)",
	L"Regarder/Pivoter (|L)",
	L"Santé : %d/%d\nÉnergie : %d/%d\nMoral : %s",
	L"Pardon ?",					//this means "what?" 
	L"Suite",					//an abbrieviation for "Continued" 
	L"Sourdine désactivée pour %s.",
	L"Sourdine activée pour %s.",
	L"État : %d/%d\nCarburant : %d/%d",
	L"Sortir du véhicule" ,
	L"Changer d'escouade (|M|A|J| |E|S|P|A|C|E)",
	L"Conduire",
	L"N/A",						//this is an acronym for "Not Applicable." 
	L"Utiliser (À mains nues)",
	L"Utiliser (Arme à feu)",
	L"Utiliser (Arme blanche)",
	L"Utiliser (Explosifs)",
	L"Utiliser (Trousse de soins)",
	L"(Prendre)",
	L"(Recharger)",
	L"(Donner)",
	L"%s part.",
	L"%s arrive.",
	L"%s n'a plus de points d'action.",
	L"%s n'est pas disponible.",
	L"%s est couvert de bandages.",
	L"%s n'a plus de bandages.",
	L"Ennemis dans le secteur !",
	L"Pas d'ennemi en vue.",
	L"Pas assez de points d'action.",
	L"Personne n'utilise une télécommande.",
	L"La rafale a vidé le chargeur !",
	L"SOLDAT",
	L"CRÉPITUS",
	L"MILICE",
	L"CIVIL",
	L"ZOMBI",
	L"PRISONNIER",
	L"Quitter Secteur",
	L"OK",
	L"Annuler",
	L"Mercenaire",
	L"Tous",
	L"GO",
	L"Carte",
	L"Vous ne pouvez pas quitter ce secteur par ce côté.",
	L"Vous ne pouvez pas quitter le mode tour par tour.",
	L"%s est trop loin.",
	L"Enlever cime des arbres",
	L"Afficher cime des arbres",
	L"CORBEAU",				//Crow, as in the large black bird
	L"COU",
	L"TÊTE",
	L"TORSE",
	L"JAMBES",
	L"Donner informations à la Reine ?",
	L"Acquisition de l'ID digitale",
	L"ID digitale refusée. Arme désactivée.",
	L"Cible acquise",
	L"Chemin bloqué",
	L"Dépôt/Retrait",		//Help text over the $ button on the Single Merc Panel 
	L"Personne n'a besoin de premiers soins.",
	L"Enra.",											// Short form of JAMMED, for small inv slots
	L"Impossible d'aller ici.",					// used ( now ) for when we click on a cliff
	L"Chemin bloqué. Voulez-vous changer de place avec cette personne ?",
	L"La personne refuse de bouger.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Êtes-vous d'accord pour payer %s ?",
	L"Acceptez-vous le traitement médical gratuit ?",
	L"Voulez-vous épouser %s ?", //Daryl
	L"Trousseau de clés",
	L"Vous ne pouvez pas faire ça avec ce personnage.",
	L"Épargner %s ?", //Krott
	L"Hors de portée",
	L"Mineur",
	L"Un véhicule ne peut rouler qu'entre des secteurs",
	L"Impossible d'apposer des bandages maintenant",
	L"Chemin bloqué pour %s",
	L"Vos mercenaires capturés par l'armée de %s sont emprisonnés ici !", //Deidranna
	L"Verrou touché",
	L"Verrou détruit",
	L"Quelqu'un d'autre veut essayer sur cette porte.",
	L"État : %d/%d\nCarburant : %d/%d",
	L"%s ne peut pas voir %s.",  // Cannot see person trying to talk to
	L"Accessoire retiré",
	L"Ne peut pas gagner un autre véhicule car vous en avez déjà 2",

	// added by Flugente for defusing/setting up trap networks
	L"Choisir la fréquence de la bombe (1-4) ou désamorçage (A-D) :",
	L"Régler la fréquence de désamorçage :",
	L"Régler la fréquence de détonation (1-4) et désamorçage (A-D) :",
	L"Régler le nombre de tours avant l'explosion (1-4) et désamorçage (A-D) :",
	L"Régler l'ordre des fils (1-4) et du réseau (A-D) :",
	
	// added by Flugente to display food status
	L"Santé : %d/%d\nÉnergie : %d/%d\nMoral : %s\nSoif : %d%s\nFaim : %d%s",

	// added by Flugente: selection of a function to call in tactical
	L"Que voulez-vous faire ?",
	L"Remplir les gourdes",
	L"Nettoyer votre arme",
	L"Nettoyer les armes",
	L"Retirer les habits",
	L"En faire sa tenue",
	L"Inspecter la milice",
	L"Rééquiper milice",
	L"Tester déguisement",
	L"Inutilisé",

	// added by Flugente: decide what to do with the corpses
	L"Que voulez-vous faire avec ce corps ?",
	L"Couper la tête",
	L"Dépecer",
	L"Voler les habits",
	L"Porter le corps",

	// Flugente: weapon cleaning
	L"%s a nettoyé %s",

	// added by Flugente: decide what to do with prisoners
	L"As we have no prison, a field interrogation is performed.",		// TODO.Translate
	L"Field interrogation",
	L"Où voulez-vous envoyer les %d prisonniers ?",
	L"Les relâcher",
	L"Voulez-vous proposer ?",
	L"Leur reddition",
	L"Votre reddition",
	L"Distract",	// TODO.Translate
	L"Parler",
	L"Recruit Turncoat", // TODO: translate

    // added by sevenfm: disarm messagebox options, messages when arming wrong bomb
    L"Désamorcer le piège",
    L"Vérifier le piège",
    L"Enlever le drapeau bleu",
    L"Faire exploser !",
    L"Activer fil piège",
    L"Désactiver fil piège",
    L"Montrer fil piège",
    L"Pas de détonateur ou détonateur à distance trouvé !",
    L"Cet explosif est déjà armé !",
    L"Sans danger",
    L"Quasiment sans danger",
    L"Risqué",
    L"Dangereux",
    L"Danger élevé !",

    L"Masque",
    L"LVN",
    L"Objet",       // TODO.Translate. A voir définition mot(to see finished, definition of word) 
        
    L"Cette option fonctionne uniquement avec nouveau le système d'inventaire",
    L"Aucun objet dans votre main principale",
    L"Nulle part où placer l'objet dans la main principale",
    L"Aucun objet défini pour cet emplacement",
    L"Aucune main libre pour un nouvel objet",
    L"Objet non trouvé",
    L"Vous ne pouvez pas prendre d'objet avec la main principale",

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
	L"Si vous cochez ce bouton, le secteur adjacent sera immédiatement chargé.",
	L"Si vous cochez ce bouton, vous arriverez directement dans l'écran de carte\nle temps que vos mercenaires arrivent.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Vous ne pouvez laisser vos mercenaires ici.\nVous devez d'abord nettoyer ce secteur.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Faites sortir vos derniers mercenaires du secteur\npour charger le secteur adjacent.",
	L"Faites sortir vos derniers mercenaires du secteur\npour aller dans l'écran de carte le temps que vos mercenaires fassent le voyage.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s doit être escorté(e) par vos mercenaires et ne peut quitter ce secteur tout(e) seul(e).",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s escorte %s, il ne peut quitter ce secteur seul.", //male singular
	L"%s escorte %s, elle ne peut quitter ce secteur seule.", //female singular
	L"%s escorte plusieurs personnages, il ne peut quitter ce secteur seul.", //male plural
	L"%s escorte plusieurs personnages, elle ne peut quitter ce secteur seule.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Tous vos mercenaires doivent être dans les environs\npour que l'escouade avance.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Si vous cochez ce bouton, %s voyagera seul et sera\nautomatiquement assigné à une nouvelle escouade.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Si vous cochez ce bouton, votre escouade\nquittera le secteur.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s est escorté par vos mercenaires et ne peut quitter ce secteur seul. Vos mercenaires doivent être à proximité.",
};



STR16 pRepairStrings[] = 
{
	L"Objets", 		// tell merc to repair items in inventory
	L"Site SAM", 		// tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Annuler", 		// cancel this menu
	L"Robot", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 

STR16 sPreStatBuildString[] =
{
	L"perd", 			// the merc has lost a statistic
	L"gagne", 		// the merc has gained a statistic
	L"point en",	// singular
	L"points en",	// plural
	L"niveau d'",	// singular
	L"niveaux d'",	// plural
};

STR16 sStatGainStrings[] =
{
	L"santé.",
	L"agilité.",
	L"dextérité.",
	L"sagesse.",
	L"compétence médicale.",
	L"compétence en explosifs.",
	L"compétence mécanique.",
	L"tir",
	L"expérience.",
	L"force.",
	L"commandement.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Distance totale :  ", 			// total distance for helicopter to travel
	L" Aller :  ", 			// distance to travel to destination
	L" Retour : ", 			// distance to return from destination to airport
	L"Coût : ", 		// total cost of trip by helicopter
	L"HPA :  ", 			// ETA is an acronym for "estimated time of arrival" 
	L"L'hélicoptère n'a plus de carburant et doit se poser en terrain ennemi !",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
	L"Passagers : ",
	L"Sélectionner Skyrider ou l'aire d'atterrissage ?",
	L"Skyrider",
	L"Arrivée",
	L"L'hélicoptère est trop endommagé et doit atterrir en territoire ennemi !",	// warning that the sector the helicopter is going to use for refueling is under enemy control ->
	L"L'hélicoptère va retourner directement à sa base, voulez-vous faire descendre les passagers ?",
	L"Carburant restant :",
	L"Ravitaillement :",
};

STR16 pHelicopterRepairRefuelStrings[]=
{
	// anv: Waldo The Mechanic - prompt and notifications
	L"Voulez-vous que %s commence les réparations ? Ça vous coûtera %d $ et l'hélicoptère ne sera pas disponible avant %d heure(s).",
	L"L'hélicoptère est actuellement démonté. Attendez jusqu'à ce que les réparations soient terminées.",
	L"Les réparations sont terminées. L'hélicoptère est à nouveau disponible.",
	L"L'hélicoptère est complètement ravitaillé.",

	L"Helicopter has exceeded maximum range!",	// TODO.Translate
};

STR16 sMapLevelString[] =
{
	L"Niveau souterrain :", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Loyauté", 			// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"Impossible de donner des ordres.", 
};

STR16 gsTimeStrings[] =
{
	L"h",				// hours abbreviation
	L"m",				// minutes abbreviation
	L"s",				// seconds abbreviation
	L"j",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"0",
	L"Hôpital",
	L"Usine",
	L"Prison",
	L"Militaire",
	L"Aéroport",
	L"Champ de tir",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Inventaire",
	L"Quitter",
	L"Repair",	// TODO.Translate
	L"Factories",	// TODO.Translate
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Taille ",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Contrôle ", 					// how much of town is controlled
	L"Aucune", 					// none of this town
	L"Mine associée ", 				// mine associated with this town
	L"Loyauté ",					// 5 // the loyalty level of this town
	L"Forces entraînées ", 					// the forces in the town trained by the player
	L"",
	L"Principale installation ", 				// main facilities in this town
	L"Niveau ", 					// the training level of civilians in this town
	L"Formation ",				// 10 // state of civilian training in town
	L"Milice ", 					// the state of the trained civilians in the town

	// Flugente: prisoner texts	// TODO.Translate
	L"Prisonnier ",
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
	L"Mine",						// 0
	L"Argent",
	L"Or",
	L"Production quotidienne",
	L"Production estimée",
	L"Abandonnée",				// 5
	L"Fermée",
	L"Épuisée",
	L"Production",
	L"État",
	L"Productivité",
	L"Ressource",				// 10
	L"Contrôle de la ville",
	L"Loyauté de la ville",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Forces ennemies ",
	L"Secteur ",
	L"Nombre d'objets ",
	L"Inconnu",

	L"Contrôlé ",
	L"Oui",
	L"Non",
	L"Status/Software status:",	// TODO.Translate

	L"Additional Intel", // TODO:Translate
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s n'est pas assez près.",	//Merc is in sector with item but not close enough
	L"Sélection impossible.",  //MARK CARTER
	L"%s n'est pas dans le bon secteur.",
	L"En combat, vous devez prendre les objets vous-même.",
	L"En combat, vous devez abandonner les objets vous-même.",
	L"%s n'est pas dans le bon secteur.",
	L"Pendant le combat, vous ne pouvez pas recharger avec une caisse de munitions.",
};

STR16 pMapInventoryStrings[] =
{
	L"Lieu", 			// sector these items are in
	L"Objets", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Cliquez sur la colonne assignation pour affecter un mercenaire à une nouvelle tâche",
	L"Cliquez sur la colonne destination pour ordonner à un mercenaire de se rendre dans un secteur",
	L"Utilisez la compression du temps pour que le voyage du mercenaire vous paraisse moins long.",
	L"Cliquez sur un secteur pour le sélectionner. Cliquez à nouveau pour donner un ordre de mouvement à un mercenaire ou effectuez un clic droit pour obtenir des informations sur le secteur.",
	L"Appuyez sur \"H\" pour afficher l'aide en ligne.",
	L"Test Texte",
	L"Test Texte",
	L"Test Texte",
	L"Test Texte",
	L"Cet écran ne vous est d'aucune utilité tant que vous n'êtes pas arrivé à Arulco. Une fois votre équipe constituée, cliquez sur le bouton de compression du temps en bas à droite. Le temps vous paraîtra moins long...",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Déplacement", 	// title for movement box 
	L"Route", 		// done with movement menu, start plotting movement
	L"Annuler", 		// cancel this menu
	L"Autre",		// title for group of mercs not on squads nor in vehicles
	L"Select all",		// Select all squads TODO: Translate
};


STR16 pUpdateMercStrings[] =
{
	L"Oups :", 			// an error has occured
	L"Expiration du contrat :", 	// this pop up came up due to a merc contract ending
	L"Tâches accomplies :", // this pop up....due to more than one merc finishing assignments
	L"Mercenaires disponibles :", // this pop up ....due to more than one merc waking up and returing to work
	L"Mercenaires au repos :", // this pop up ....due to more than one merc being tired and going to sleep
	L"Contrats arrivant à échéance :", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Villes (|W)",
	L"Mines (|M)",
	L"Escouades & Ennemis (|T)",
	L"Espace aérien (|A)",
	L"Objets (|I)",
	L"Milice & Ennemis (|Z)",
	L"Show |Disease Data",	// TODO.Translate
	L"Show Weathe|r",
	L"Show |Quests & Intel",
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Suivant (|.)",		// next page
	L"Précédent (|,)",		// previous page
	L"Quitter l'inventaire du secteur (|E|S|C)",	// exit sector inventory

	L"Zoom inventaire", // HEAROCK HAM 5: Inventory Zoom Button
	L"Empiler les mêmes objets", // HEADROCK HAM 5: Stack and Merge
	L"|C|l|i|c |G|. : Trier munitions par caisse\n|C|l|i|c |D|. : Trier munitions par boîte", // HEADROCK HAM 5: Sort ammo
	// 6 - 10
	L"|C|l|i|c |G|. : Ôter tous les accessoires des objets\n|C|l|i|c |D|. : empty LBE in sector", // HEADROCK HAM 5: Separate Attachments
	L"Décharger toutes les armes", //HEADROCK HAM 5: Eject Ammo
	L"|C|l|i|c |G|. : Voir tous les objets\n|C|l|i|c |D|. : Cacher tous les objets", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans armes\n|C|l|i|c |D|. : Voir que les armes", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans munitions\n|C|l|i|c |D|. : Voir que les munitions", // HEADROCK HAM 5: Filter Button
	// 11 - 15
	L"|C|l|i|c |G|. : Avec/sans explosifs\n|C|l|i|c |D|. : Voir que les explosifs", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans armes blanches\n|C|l|i|c |D|. : Voir que les armes blanches", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans protections\n|C|l|i|c |D|. : Voir que les protections", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans LBE\n|C|l|i|c |D|. : Voir que les LBE", // HEADROCK HAM 5: Filter Button
	L"|C|l|i|c |G|. : Avec/sans kits\n|C|l|i|c |D|. : Voir que les kits", // HEADROCK HAM 5: Filter Button
	// 16 - 20
	L"|C|l|i|c |G|. : Avec/sans objets divers\n|C|l|i|c |D|. : Voir que les objets divers", // HEADROCK HAM 5: Filter Button
	L"Pour afficher ou non les objets déplacés.", // Flugente: move item display
	L"Save Gear Template",	// TODO.Translate
	L"Load Gear Template...",
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"PC Portable (|L)",
	L"Tactique (|E|C|H|A|P)",
	L"Options (|O)",
	L"Compression du temps (|+)", 	// time compress more
	L"Compression du temps (|-)", 	// time compress less
	L"Message précédent (|H|A|U|T)\nPage précédente (|P|g|U|p)", 	// previous message in scrollable list
	L"Message suivant (|B|A|S)\nPage suivante (|P|g|D|n)", 	// next message in the scrollable list
	L"Arrêter/Reprendre (|E|S|P|A|C|E)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Solde actuel", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s est mort(e).",
};


STR16 pDayStrings[] =
{
	L"Jour",
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
	L"M.I.S. Assurance",	
	L"Bobby Ray",
	L"Kingpin",
	L"John Kulba",
	L"A.I.M.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"Précédent",
  L"Suivant",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Nouveaux messages...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Effacer message ?",
 L"Effacer message NON CONSULTÉ ?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
	L"De :",
	L"Sujet :",
	L"Date :",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
	L"Boîte mail",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Comptable Plus",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Crédit :", 				// credit (subtract from) to player's account
	L"Débit :", 				// debit (add to) to player's account
	L"Revenus (hier) :",
	L"Dépôts (hier) :",
	L"Dépenses (hier) :",
	L"Solde (fin de journée) :",
	L"Revenus (aujourd'hui) :",
	L"Dépôts (aujourd'hui) :",
	L"Dépenses (aujourd'hui) :",
	L"Solde actuel :",
	L"Revenus (prévision) :",
	L"Solde (prévision) :", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
	L"Jour", 				// the day column
	L"Crédit", 			// the credits column (to ADD money to your account)
	L"Débit",				// the debits column (to SUBTRACT money from your account)
	L"Transaction", // transaction type - see TransactionText below
	L"Solde", 		// balance at this point in time
	L"Page", 				// page number
	L"Jour(s)", 			// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Intérêts cumulés",			// interest the player has accumulated so far
	L"Dépôt anonyme",
	L"Commission", 
	L"Engagé", 				// Merc was hired
	L"Achats Bobby Ray", 		// Bobby Ray is the name of an arms dealer
	L"Règlement MERC",
	L"Acompte médical pour %s", 		// medical deposit for merc
	L"Analyse IMP", 		// IMP is the acronym for International Mercenary Profiling
	L"Assurance pour %s", 
	L"Réduction d'assurance pour %s",
	L"Extension d'assurance pour %s", 				// johnny contract extended
	L"Annulation d'assurance pour %s", 
	L"Indemnisation pour %s", 		// insurance claim for merc
	L"1 jour", 				// merc's contract extended for a day
	L"1 semaine", 				// merc's contract extended for a week
	L"2 semaines", 				// ... for 2 weeks
	L"Revenus des mines", 
	L"", //String nuked
	L"Achat de fleurs",
	L"Remboursement médical pour %s",
	L"Remb. médical partiel pour %s",
	L"Pas de remb. médical pour %s",
	L"Paiement à %s",		// %s is the name of the npc being paid
	L"Transfert de fonds pour %s", 			// transfer funds to a merc
	L"Transfert de fonds de %s", 		// transfer funds from a merc
	L"Coût milice de %s", // initial cost to equip a town's militia
	L"Achats à %s.",	//is used for the Shop keeper interface.  The dealers name will be appended to the en d of the string.
	L"Montant déposé par %s.",
	L"Matériel vendu à la population",
	L"Infrastucture utilisée", // HEADROCK HAM 3.6
	L"Entretien de la milice", // HEADROCK HAM 3.6
	L"Argent des prisonniers libérés",	// Flugente: prisoner system
	L"WHO data subscription",	// Flugente: disease TODO.Translate
	L"Payment to Kerberus",	// Flugente: PMC
	L"SAM site repair", // Flugente: SAM repair // TODO.Translate
	L"Trained workers", // Flugente: train workers
	L"Drill militia in %s",	// Flugente: drill militia	// TODO.Translate
	L"Mini event", // rftr: mini events // TODO: translate
	L"Funds transferred from rebel command", // rftr: rebel command
	L"Funds transferred to rebel command", // rftr: rebel command
	L"Bounty payout", // rftr: rebel command soldier bounties
};

STR16 pTransactionAlternateText[] =
{
	L"Assurance pour", 				// insurance for a merc
	L"Ext. contrat de %s (1 jour).", 				// entend mercs contract by a day
	L"Ext. contrat de %s (1 semaine).",
	L"Ext. contrat de %s (2 semaines).",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Skyrider a reçu %d $", 			// skyrider was paid an amount of money
	L"Skyrider attend toujours ses %d $", 		// skyrider is still owed an amount of money
	L"Skyrider a fait le plein",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider est prêt à redécoller.", // Skyrider was grounded but has been freed
	L"Skyrider n'a pas de passager. Si vous voulez envoyer des mercenaires dans un autre secteur, n'oubliez pas de les assigner à l'hélicoptère.",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
	L"Superbe", 
	L"Bon",
	L"Stable",
	L"Bas",
	L"Paniqué",
	L"Mauvais",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"L'équipement de %s est maintenant disponible à Omerta (A9).", 
	L"L'équipement de %s est maintenant disponible à Drassen (B13).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Santé",
	L"Énergie",
	L"Moral",
	L"État",	// the condition of the current vehicle (its "Santé")
	L"Carburant",	// the fuel level of the current vehicle (its "energy")
	L"Poison",
	L"Soif",		// drink level
	L"Faim",		// food level
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Mercenaire précédent (|G|a|u|c|h|e)", 			// previous merc in the list
	L"Mercenaire suivant (|D|r|o|i|t|e)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"HPA :", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Vous ne le reverrez jamais. Vous êtes sûr de vous ?", 	// do you want to continue and lose the item forever
	L"Cet objet a l'air VRAIMENT important. Vous êtes bien sûr (mais alors BIEN SÛR) de vouloir l'abandonner ?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"L'escouade ne peut se déplacer, si l'un de ses membres se repose.",

//1-5
	L"Déplacez d'abord votre escouade.",
	L"Des ordres de mouvement ? C'est un secteur hostile !",
	L"Les mercenaires doivent d'abord être assignés à une escouade ou un véhicule.",
	L"Vous n'avez plus aucun membre dans votre escouade.", 		// you have non members, can't do anything 
	L"Le mercenaire ne peut obéir.",			 		// merc can't comply with your order
//6-10
	L"doit être escorté. Mettez-le dans une escouade.", // merc can't move unescorted .. for a male
	L"doit être escortée. Mettez-la dans une escouade.", // for a female
	L"Ce mercenaire n'est pas encore arrivé à %s !",
	L"Il faudrait d'abord revoir les termes du contrat...",
	L"",
//11-15
	L"Des ordres de mouvement ? Vous êtes en plein combat !",
	L"Vous êtes tombé dans une embuscade de chats sauvages dans le secteur %s !",
	L"Vous venez d'entrer dans le repaire des chats sauvages (secteur %s) !", 
	L"", 
	L"Le site SAM en %s est sous contrôle ennemi.",
//16-20
	L"La mine en %s est sous contrôle ennemi. Votre revenu journalier est réduit à %s.",
	L"L'ennemi vient de prendre le contrôle du secteur %s.",
	L"L'un au moins de vos mercenaires ne peut effectuer cette tâche.",
	L"%s ne peut rejoindre %s (plein).",
	L"%s ne peut rejoindre %s (éloignement).",
//21-25
	L"La mine en %s a été reprise par les forces de Deidranna !",
	L"Les forces de Deidranna viennent d'envahir le site SAM en %s",
	L"Les forces de Deidranna viennent d'envahir %s",
	L"Les forces de Deidranna ont été repérées en %s.",
	L"Les forces de Deidranna viennent de prendre %s.",
//26-30
	L"L'un au moins de vos mercenaires n'est pas fatigué.",
	L"L'un au moins de vos mercenaires ne peut être réveillé.",
	L"La milice n'apparaît sur l'écran qu'une fois son entraînement achevé.",
	L"%s ne peut recevoir d'ordre de mouvement pour le moment.",
	L"Les miliciens qui ne se trouvent pas dans les limites d'une ville ne peuvent être déplacés.",
//31-35
	L"Vous ne pouvez pas entraîner de milice en %s.",
	L"Un véhicule ne peut se déplacer, s'il est vide !",
	L"L'état de santé de %s ne lui permet pas de voyager !",
	L"Vous devez d'abord quitter le musée !",
	L"%s est mort(e) !",
//36-40
	L"%s ne peut passer à %s (en mouvement)",
	L"%s ne peut pas pénétrer dans le véhicule de cette façon",
	L"%s ne peut rejoindre %s",
	L"Vous devez d'abord engager des mercenaires !",
	L"Ce véhicule ne peut circuler que sur les routes !",
//41-45
	L"Vous ne pouvez réaffecter des mercenaires qui sont en déplacement",
	L"Plus d'essence !",
	L"%s est trop fatigué(e) pour entreprendre ce voyage.",
	L"Personne n'est capable de conduire ce véhicule.",
	L"L'un au moins des membres de cette escouade ne peut se déplacer.",
//46-50
	L"L'un au moins des AUTRES mercenaires ne peut se déplacer.",
	L"Le véhicule est trop endommagé !",
	L"Deux mercenaires au plus peuvent être assignés à l'entraînement de la milice dans chaque secteur.",
	L"Le robot ne peut se déplacer sans son contrôleur. Mettez-les ensemble dans la même escouade.",
	L"Items cannot be moved to %s, as no valid dropoff point was found. Please enter map to resolve this issue.",	// TODO.Translate
// 51-55
	L"%d items moved from %s to %s",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Cliquez à nouveau sur votre destination pour la confirmer ou cliquez sur d'autres secteurs pour définir de nouvelles étapes.",
	L"Route confirmée.",
	L"Destination inchangée.",
	L"Route annulée.",
	L"Route raccourcie.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Cliquez sur la nouvelle destination de vos mercenaires.",
	L"OK. Les mercenaires arriveront en %s",
	L"Les mercenaires ne peuvent être déployés ici, l'espace aérien n'est pas sécurisé !",
	L"Annulé. Secteur d'arrivée inchangé.",
	L"L'espace aérien en %s n'est plus sûr ! Le secteur d'arrivée est maintenant %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Inventaire (|E|N|T|R|É|E)",
	L"Jeter objet",
	L"Quitter Inventaire (|E|N|T|R|É|E)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"%s doit-il abandonner son paquetage sur place (%s) ou à (%s) avant de quitter ?",
	L"%s est sur le point de partir et laissera son paquetage en %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"%s doit-elle abandonner son paquetage sur place (%s) ou à (%s) avant de quitter ?",
	L"%s est sur le point de partir et laissera son paquetage en %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"a rempli son contrat, il est rentré chez lui.", 		// merc's contract is over and has departed
	L"a rempli son contrat, elle est rentrée chez elle.", 		// merc's contract is over and has departed
	L"est parti, son contrat ayant été annulé.", 		// merc's contract has been terminated
	L"est partie, son contrat ayant été annulé.",		// merc's contract has been terminated
	L"Vous devez trop d'argent à la MERC, %s quitte Arulco.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"Code incorrect",
	L"Vous allez établir un nouveau profil. Êtes-vous sûr de vouloir recommencer ?",
	L"Veuillez entrer votre nom et votre sexe.",
	L"Vous n'avez pas les moyens de vous offrir une analyse de profil.",
	L"Option inaccessible pour le moment.",
	L"Pour que cette analyse soit efficace, il doit vous rester au moins une place dans votre escouade.",
	L"Profil déjà établi.",
	L"Impossible de charger le profil.",
	L"Vous avez déjà atteint le nombre maximum d'IMP.",
	L"Vous avez déjà trois IMP du même sexe dans l'escouade.",
	L"Vous n'avez pas les moyens.",	// 10
	L"Le nouvel IMP a rejoint votre escouade.",
	L"Vous avez déjà sélectionné le maximal de traits de caractères.",
	L"No voicesets found.",	// TODO.Translate
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"Nous", 			// about the IMP site
	L"COMMENCER", 			// begin profiling
	L"Personnalité", 		// personality section
	L"Caractéristiques", 		// personal stats/attributes section
	L"Apparence", 			// changed from portrait
	L"Voix %d", 			// the voice selection
	L"OK", 			// done profiling
	L"Recommencer", 		// start over profiling
	L"Oui, la réponse en surbrillance me convient.", 
	L"Oui", 
	L"Non",
	L"Terminé", 			// finished answering questions
	L"Préc.", 			// previous question..abbreviated form
	L"Suiv.", 			// next question
	L"OUI, JE SUIS SÛR.", 		// oui, I am certain 
	L"NON, JE VEUX RECOMMENCER.", // non, I want to start over the profiling process
	L"OUI",
	L"NON",
	L"Retour", 			// back one page
	L"Annuler", 			// cancel selection
	L"Oui, je suis sûr.",
	L"Non, je ne suis pas sûr.",
	L"Registre", 			// the IMP site registry..when name and gender is selected
	L"Analyse", 			// analyzing your profile results
	L"OK",
	L"Caractère", // Change from "Voice"
	L"Aucune",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed
	L"Vos traits de caractères étant choisis, il est temps de choisir vos compétences.",
	L"Pour compléter le processus, choisissez vos attributs.",
	L"Pour commencer votre profil réel, choisissez un portrait, une voix et vos couleurs",
	L"Maintenant que vous avez complété votre apparence, proccédons à l'analyse de votre personnage.",
};

STR16 pFilesTitle[] =
{
  L"Fichiers",
};

STR16 pFilesSenderList[] =
{
#ifdef JA2UB
L"Rapport Tracona", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
#else
L"Rapport Arulco", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
#endif
	L"Interception #1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"Interception #2",	   // second intercept file
	L"Interception #3",			 // third intercept file
	L"Interception #4", // fourth intercept file
	L"Interception #5", // fifth intercept file
	L"Interception #6", // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Historique",
};

STR16 pHistoryHeaders[] =
{
	L"Jour", 			// the day the history event occurred
	L"Page", 			// the current page in the history report we are in
	L"Jour", 			// the days the history report occurs over
	L"Lieu", 			// location (in sector) the event occurred
	L"Événement", 			// the event label
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
	L"",																						// leave this line blank
	//1-5
	L"%s engagé(e) sur le site AIM.", 										// merc was hired from the aim site
	L"%s engagé(e) sur le site MERC.", 									// merc was hired from the aim site
	L"%s est mort(e).", 															// merc was killed
	L"Versements MERC.",								// paid outstanding bills at MERC
	L"Ordre de mission de Chivaldori Enrico accepté",	
	//6-10
	L"IMP : Profil fait",
	L"Souscription d'un contrat d'assurance pour %s.", 				// insurance contract purchased
	L"Annulation du contrat d'assurance de %s.", 				// insurance contract canceled
	L"Indemnité pour %s.", 							// insurance claim payout for merc
	L"Extension du contrat de %s (1 jour).", 						// Extented "mercs name"'s for a day
	//11-15
	L"Extension du contrat de %s (1 semaine).", 					// Extented "mercs name"'s for a week
	L"Extension du contrat de %s (2 semaines).", 					// Extented "mercs name"'s 2 weeks
	L"%s a été renvoyé(e).", 													// "merc's name" was dismissed.
	L"%s a démissionné(e).", 																		// "merc's name" quit.
	L"quête commencée.", 															// a particular quest started
	//16-20
	L"quête achevée.",
	L"Entretien avec le chef des mineurs de %s",									// talked to head miner of town
	L"Libération de %s",
	L"Activation du mode triche",
	L"Le ravitaillement devrait arriver demain à Omerta",
	//21-25
	L"%s a quitté l'escouade pour épouser Hick Daryl",
	L"Expiration du contrat de %s.",
	L"Recrutement de %s.",
	L"Plainte d'Enrico pour manque de résultats",
	L"Victoire",
	//26-30
	L"La mine de %s commence à s'épuiser",
	L"La mine de %s est épuisée",
	L"La mine de %s a été fermée",
	L"La mine de %s a été réouverte",
	L"Une prison du nom de Tixa a été découverte.",
	//31-35
	L"Rumeurs sur une usine d'armes secrètes : Orta.",
	L"Les chercheurs d'Orta vous donnent des fusils à roquettes.",
	L"Deidranna fait des expériences sur les cadavres.",
	L"Frank parle de combats organisés à San Mona.",
	L"Un témoin a aperçu quelque chose dans les mines.",
	//36-40
	L"Rencontre avec Devin (vendeur d'explosifs).",
	L"Rencontre avec Mike, le fameux ex-mercenaire de l'AIM !",
	L"Rencontre avec Tony (vendeur d'armes).",
	L"Fusil à roquettes récupéré auprès du sergent Krott.",
	L"Acte de propriété du magasin d'Angel donné à Kyle.",
	//41-45
	L"Foulab propose de construire un robot.",
	L"Gabby fait des décoctions rendant invisible aux créatures.",
	L"Keith est hors-jeu.",
	L"Howard fournit du cyanure à la Reine Deidranna.",
	L"Rencontre avec Keith (vendeur à Cambria).",
	//46-50
	L"Rencontre avec Howard (pharmacien à Balime).",
	L"Rencontre avec Perko (réparateur en tous genres).",
	L"Rencontre avec Sam de Balime (vendeur de matériel).",
	L"Franz vend du matériel électronique.",
	L"Arnold tient un magasin de réparations à Grumm.",
	//51-55
	L"Fredo répare le matériel électronique à Grumm.",
	L"Don provenant d'un homme influent de Balime.",
	L"Rencontre avec Jake, vendeur de pièces détachées.",
	L"Clé électronique reçue.",
	L"Corruption de Walter pour ouvrir l'accès aux sous-sols.",
	//56-60
	L"Dave refait gratuitement le plein, s'il a du carburant.",
	L"Pot-de-vin donné à Pablo.",
	L"Caïd cache un trésor dans la mine de San Mona.",
	L"Victoire de %s dans le combat extrème",
	L"Défaite de %s dans le combat extrème",
	//61-65
	L"Disqualification de %s dans le combat extrème",
	L"Importante somme découverte dans la mine abandonnée.",
	L"Rencontre avec un tueur engagé par Caïd.",
	L"Perte du secteur",				//ENEMY_INVASION_CODE
	L"Secteur défendu",
	//66-70
	L"Défaite",							//ENEMY_ENCOUNTER_CODE
	L"Embuscade",						//ENEMY_AMBUSH_CODE
	L"Embuscade ennemie déjouée",
	L"Échec de l'attaque",			//ENTERING_ENEMY_SECTOR_CODE
	L"Réussite de l'attaque !",
	//71-75
	L"Attaque de créatures",			//CREATURE_ATTACK_CODE
	L"Ambuscade de chats sauvages",			//BLOODCAT_AMBUSH_CODE
	L"Élimination des chats sauvages",
	L"%s a été tué(e)",
	L"Tête de terroriste donnée à Carmen",
	//76-80
	L"Reste Slay",
	L"%s a été tué(e)",
	L"Rencontre avec Waldo, mécanicien aéronautique.",
	L"Réparations de l'hélico ont débuté, temps estimé : %d h.",
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
	L"Favoris",
	L"Finances",
	L"Personnel",
	L"Historique",
	L"Fichiers",
	L"Éteindre",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"AIM",
	L"Bobby Ray",
	L"IMP",
	L"MERC",
	L"Morgue",
	L"Fleuriste",
	L"Assurance",
	L"Annuler",
	L"Encyclopédie",
	L"Briefing",
	L"Comptes rendus",
	L"MeLoDY",
	L"WHO",
	L"Kerberus",
	L"Militia Overview",	// TODO.Translate
	L"R.I.S.",
	L"Factories",	// TODO.Translate
};

STR16 pBookmarkTitle[] =
{
	L"Favoris",
	L"Faites un clic droit pour accéder plus tard à ce menu.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Téléchargement",
	L"Chargement",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Prendre", 			// take money from merc
	L"Donner", 			// give money to merc
	L"Annuler", 			// cancel transaction
	L"Effacer", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Transférer $ ", 		// transfer money to merc -- short form
	L"Stats", 			// view stats of the merc
	L"Inventaire", 			// view the inventory of the merc
	L"Historique",
};

STR16 sATMText[ ]=
{
	L"Transférer les fonds ?", 		// transfer funds to merc?
	L"Ok ?", 			// are we certain?
	L"Entrer montant", 		// enter the amount you want to transfer to merc
	L"Choix du type", 		// select the type of transfer to merc
	L"Fonds insuffisants", 	// not enough money to transfer to merc
	L"Le montant doit être un multiple de 10 $", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Erreur",
	L"Le serveur ne trouve pas l'entrée DNS.",	
	L"Vérifiez l'adresse URL et essayez à nouveau.",
	L"OK",
	L"Connexion à l'hôte.",
};


STR16 pPersonnelString[] =
{
	L"Merc. :", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"AIM",
	L"Membres AIM",
	L"Galerie AIM",		// a mug shot is another name for a portrait
	L"Tri AIM",
	L"AIM",
	L"Anciens AIM",
	L"Règlement AIM",
	L"Historique AIM",
	L"Liens AIM",
	L"MERC",
	L"Comptes MERC",
	L"Enregistrement MERC",
	L"Index MERC",
	L"Bobby Ray",
	L"Bobby Ray : Armes",
	L"Bobby Ray : Munitions",
	L"Bobby Ray : Protections",
	L"Bobby Ray : Divers",							//misc is an abbreviation for miscellaneous				
	L"Bobby Ray : Occasions",
	L"Bobby Ray : Commande",
	L"IMP",
	L"IMP",
	L"Service des Fleuristes Associés",
	L"Service des Fleuristes Associés : Exposition",
	L"Service des Fleuristes Associés : Bon de commande",
	L"Service des Fleuristes Associés : Cartes",
	L"Malleus, Incus & Stapes Courtiers",
	L"Information",
	L"Contrat",
	L"Commentaires",
	L"Morgue McGillicutty",
	L"",
	L"URL introuvable.",
	L"%s, conseil de presse : Bilan du conflit",
	L"%s, conseil de presse : Rapports",
	L"%s, conseil de presse : Dernières nouvelles",
	L"%s, conseil de presse : À propos de nous",
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
	L"Militia Overview",			// TODO.Translate
	L"Recon Intelligence Services - Information Requests",		// TODO.Translate
	L"Recon Intelligence Services - Information Verification",
	L"Recon Intelligence Services - About us",
	L"Factory Overview",	// TODO.Translate
	L"Bobby Ray : Dernières commandes",
	L"Encyclopédie",
	L"Encyclopédie : Données",
	L"Salle de briefing",
	L"Salle de briefing : Données",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Help",
	L"Cliquez à nouveau pour accéder aux Favoris.",
};

STR16 pLaptopTitles[] =
{
	L"Boîte mail",
	L"Fichiers",
	L"Personnel",
	L"Comptable Plus",
	L"Historique",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Mort en mission",
	L"Parti(e)",
	L"Autre",
	L"Mariage",
	L"Contrat terminé",
	L"Quitter",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"Équipe actuelle",
	L"Départs",
	L"Coût quotidien :",
	L"Coût maximum :",
	L"Coût minimum :",
	L"Morts en mission :",
	L"Démissions :",
	L"Autres :",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Minimum",
	L"Moyenne",
	L"Maximum",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"SAN",
	L"AGI",
	L"DEX",
	L"FOR",
	L"COM",
	L"SAG",
	L"NIV",
	L"TIR",
	L"TECH",
	L"EXPL",
	L"MÉD",
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
	L"Contrat",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Continuer",
	L"Stop",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Vous avez été vaincu dans ce secteur !",
	L"L'ennemi, sans aucune compassion, ne fait pas de quartier !",
	L"Vos mercenaires inconscients ont été capturés !",
	L"Vos mercenaires ont été faits prisonniers.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Précédent",
	L"Suivant",
	L"Accepter",
	L"Annuler",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Précédent", 
	L"Suivant",
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Autoriser",
	L"Accueil",
	L"Compte :",
	L"Mercenaire",
	L"Jours",
	L"Taux",	//5
	L"Montant",
	L"Total :",
	L"Désirez-vous autoriser le versement de %s ?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
	L"%s (+gear)",	// TODO.Translate
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Précédent",
	L"Suivant",
};


//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"Santé",
	L"Agilité",
	L"Dextérité",
	L"Force",
	L"Commandement",
	L"Sagesse",
	L"Niveau",
	L"Tir",
	L"Mécanique",
	L"Explosifs",
	L"Médecine",

	L"Précédent",
	L"Engager",
	L"Suivant",
	L"Infos complémentaires",
	L"Accueil",
	L"Engagé(e)",
	L"Salaire :",
	L"Par jour",
	L"Paquetage :",
	L"Total :",
	L"Décédé(e)",

	L"Vous ne pouvez engager plus de 18 mercenaires.",
	L"Acheter paquetage ?",
	L"Indisponible",											
	L"Payez sa solde",
	L"Biographie",
	L"Inventaire",								
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Ouvrir compte",
	L"Annuler",
	L"Vous ne possédez pas de compte. Désirez-vous en ouvrir un ?",
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Kline Speck T., fondateur",
	L"Cliquez ici pour ouvrir un compte",
	L"Cliquez ici pour voir votre compte",
	L"Cliquez ici pour consulter les fichiers",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Le transfert a échoué. Aucun fonds disponible.",
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Morgue McGillicutty : À votre écoute depuis 1983.",
	L"McGillicutty Murray dit Pops, notre directeur bien aimé, est un ancien mercenaire de l'AIM. Sa spécialité : la mort des autres.",
	L"Pops l'a côtoyée pendant si longtemps qu'il est un expert de la mort, à tous points de vue.",
	L"La morgue McGillicutty vous offre un large éventail de services funéraires, depuis une écoute compréhensive jusqu'à la reconstitution des corps... dispersés.",
	L"Laissez donc la morgue McGillicutty vous aider, pour que votre compagnon repose enfin en paix.",

	// Text for the various links available at the bottom of the page
	L"ENVOYER FLEURS",
	L"CERCUEILS & URNES",
	L"CRÉMATION",
	L"SERVICES FUNÉRAIRES",
	L"NOTRE ÉTIQUETTE",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Le concepteur de ce site s'est malheureusement absenté pour cause de décès familial. Il reviendra dès que possible pour rendre ce service encore plus efficace.",
	L"Veuillez croire en nos sentiments les plus respectueux dans cette période qui doit vous être douloureuse.",
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Vitrine",

	//Address of United Florist

	L"\"Nous livrons partout dans le monde\"",
	L"0-800-SENTMOI",
	L"333 NoseGay Dr, Seedy City, CA USA 90210",
	L"http://www.sentmoi.com",

	// detail of the florist page

	L"Rapides et efficaces !",
	L"Livraison en 24 heures partout dans le monde (ou presque).",
	L"Les prix les plus bas (ou presque) !",
	L"Si vous trouvez moins cher, nous vous livrons gratuitement une douzaine de roses !",
	L"Flore, Faune & Fleurs depuis 1981.",
	L"Nos bombardiers (recyclés) vous livrent votre bouquet dans un rayon de 20 km (ou presque). N'importe quand... N'importe où !",
	L"Nous répondons à tous vos besoins (ou presque) !",
	L"Bruce, notre expert fleuriste-conseil, trouvera pour vous les plus belles fleurs et vous composera le plus beau bouquet que vous ayez vu !",
	L"Et n'oubliez pas que si nous ne l'avons pas, nous pouvons le faire pousser... et vite !",
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Retour",
	L"Envoi",
	L"Annuler",
	L"Galerie",

	L"Nom du bouquet :",
	L"Prix :",			//5
	L"Référence :",
	L"Date de livraison",
	L"jour suivant",
	L"dès que possible",
	L"Lieu de livraison",			//10
	L"Autres services",
	L"Pot Pourri (10 $)",
	L"Roses Noires (20 $)",
	L"Nature Morte (10 $)",
	L"Gâteau (si dispo)(10 $)",		//15
	L"Carte personnelle :",
	L"Veuillez écrire votre message en 75 caractères maximum...",
	L"...ou utiliser l'une de nos",

	L"CARTES STANDARDS",	
	L"Informations",//20

	//The text that goes beside the area where the user can enter their name

	L"Nom :",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Préc.",	//abbreviation for previous
	L"Suiv.",	//abbreviation for next

	L"Cliquez sur le bouquet que vous désirez commander.",
	L"Note : les bouquets \"pot pourri\" et \"nature morte\" vous seront facturés 10 $ supplémentaires.",

	//text on the button

	L"Accueil",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Faites votre choix",
	L"Retour",
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Commande",				//Title of the page
	L"Qté",					// The number of items ordered
	L"Poids (%s)",			// The weight of the item
	L"Description",				// The name of the item
	L"Prix unitaire",				// the item's weight
	L"Total",				//5	// The total price of all of items of the same type
	L"Sous-total",				// The sub total of all the item totals added
	L"Transport",		// S&H is an acronym for Shipping and Handling 
	L"Total",			// The grand total of all item totals + the shipping and handling
	L"Lieu de livraison",				
	L"Type d'envoi",			//10	// See below
	L"Coût (par %s.)",			// The cost to ship the items
	L"Du jour au lendemain",			// Gets deliverd the next day
	L"2 c'est mieux qu'un",			// Gets delivered in 2 days
	L"Jamais 2 sans 3",			// Gets delivered in 3 days
	L"Effacer commande",//15			// Clears the order page
	L"Confirmer commande",			// Accept the order
	L"Retour",				// text on the button that returns to the previous page
	L"Accueil",				// Text on the button that returns to the home page
	L"* Matériel d'occasion",		// Disclaimer stating that the item is used
	L"Vous n'avez pas les moyens.",		//20	// A popup message that to warn of not enough money
	L"<AUCUNE>",				// Gets displayed when there is non valid city selected
	L"Êtes-vous sûr de vouloir envoyer cette commande à %s ?",		// A popup that asks if the city selected is the correct one
	L"Poids total **",			// Displays the weight of the package
	L"** Pds Min.",				// Disclaimer states that there is a minimum weight for the package
	L"Envois",	
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"A/Poing",
	L"PM",
	L"Mitrail.",
	L"Fusil",
	L"Sniper",
	L"F/Assaut",
	L"FM",
	L"F/Pompe",
	L"A/Lourde",

	// Ammo
	L"A/Poing",
	L"PM",
	L"Mitrail.",
	L"Fusil",
	L"Sniper",
	L"F/Assaut",
	L"FM",
	L"F/Pompe",

	// Used
	L"Arme",
	L"Protection",
	L"Mat. LBE",
	L"Divers",

	// Armour
	L"Casque",
	L"Veste",
	L"Pantalon",
	L"Blindage",

	// Misc
	L"A/blanche",
	L"Arme/Jet",
	L"Mêlée",
	L"Grenade",
	L"Explosif",
	L"Médical",
	L"Kit&Habit",
	L"Mat. Face",
	L"Mat. LBE",
	L"Optique", // Madd: new BR filters	
	L"Pied&LAM",
	L"Bouche",
	L"Crosse",
	L"Mag&Dét.",
	L"Accessoire",
	L"Divers",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Pour commander",				// Title
	// instructions on how to order
	L"Cliquez sur les objets désirés. Cliquez à nouveau pour sélectionner plusieurs exemplaires d'un même objet. Effectuez un clic droit pour désélectionner un objet. Il ne vous reste plus qu'à passer commande.",			

	//Text on the buttons to go the various links

	L" PRÉCÉDENT",		// 
	L"Arme", 			//3
	L"Munition",			//4
	L"Protection",			//5
	L"Divers",			//6	//misc is an abbreviation for miscellaneous
	L"Occasion",			//7
	L"SUIVANT",
	L"COMMANDER",
	L"Accueil",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Votre escouade possède",//11
	L"arme(s) qui utilise(nt) ce type de munitions", //12

	//The following lines provide information on the items

	L"Poids :",		// Weight of all the items of the same type
	L"Cal :",			// the caliber of the gun
	L"Chgr :",			// number of rounds of ammo the Magazine can hold
	L"Portée :",			// The range of the gun
	L"Dégâts :",			// Damage of the weapon	
	L"CdT :",			// Weapon's Rate Of Fire, acronym ROF
	L"PA :",				// Weapon's Action Points, acronym AP
	L"Étourdis. :",	// Weapon's Stun Damage
	L"Prot. :",		// Armour's Protection
	L"Cam. :",			// Armour's Camouflage
	L"Armor Pen:",		// Ammo's Armour Piercing modifier (see AmmoTypes.xml - armourImpactReduction)
	L"Dmg Mod:",		// Ammo's Bullet Tumble modifier (see AmmoTypes.xml - afterArmourDamage)
	L"Projectiles:",	// Ammo's bullet count (for buckshot) (see AmmoTypes.xml - numberOfBullets)
	L"Prix :",			// Cost of the item
	L"Stock :",			// The number of items still in the store's inventory
	L"Quantité :",		// The number of items on order
	L"Endommagé",			// If the item is damaged
	L"Poids :",			// the Weight of the item
	L"Sous-total :",			// The total cost of all items on order
	L"* %% efficacité",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Pas de chance ! Vous ne pouvez commander que " ,//First part
	L" objets différents en une fois. Si vous désirez passer une commande plus importante, il vous faudra remplir un nouveau bon de commande.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Nous sommes navrés, mais notre stock est vide. N'hésitez pas à revenir plus tard !",

	//A popup that tells the user that the store is temporarily sold out

	L"Nous sommes navrés, mais nous n'en avons plus en rayon.",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Vous cherchez des armes et du matériel militaire ? Vous avez frappé à la bonne porte",
	L"Un seul credo : force de frappe !",
	L"Occasions et secondes mains",

	//Text for the various links to the sub pages

	L"Divers",
	L"ARMES",
	L"MUNITIONS",		//5
	L"PROTECTIONS",

	//Details on the web site

	L"Si nous n'en vendons pas, c'est que ça n'existe pas !",
	L"En construction",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"Membres AIM",				// Title
	// Title for the way to sort
	L"Tri par :",					

	// sort by...

	L"Prix",
	L"Expérience",
	L"Tir",
	L"Mécanique",
	L"Explosifs",
	L"Médecine",
	L"Santé",
	L"Agilité",
	L"Dextérité",
	L"Force",
	L"Commandement",
	L"Sagesse",
	L"Nom",

	//Text of the links to other AIM pages

	L"Afficher l'index de la galerie de portraits",
	L"Consulter le fichier individuel",
	L"Consulter la galerie des anciens de l'AIM",

	// text to display how the entries will be sorted

	L"Ascendant",
	L"Descendant",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Précédent",
	L"Accueil", 
	L"Index",
	L"Suivant",
	L"Je refuse",
	L"J'accepte",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Cliquez pour",
	L"contacter le mercenaire.",
	L"Clic droit pour",
	L"afficher l'index.",
};					

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Santé",
	L"Agilité",
	L"Dextérité",
	L"Force",
	L"Commandement",
	L"Sagesse",
	L"Niveau",
	L"Tir",
	L"Mécanique",
	L"Explosifs",
	L"Médecine",				//10

	// the contract expenses' area

	L"Tarif",							
	L"Contrat",				
	L"1 jour",					
	L"1 semaine",
	L"2 semaines",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Précédent",
	L"Contacter",
	L"Suivant",

	L"Info. complémentaires",				// Title for the additional info for the merc's bio
	L"Membres actifs",		//20		// Title of the page
	L"Matériel optionnel :",				// Displays the optional gear cost
	L"Paquetage",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"Dépôt médical",			// If the merc required a medical deposit, this is displayed
	L"Kit 1",				// Text on Starting Gear Selection Button 1
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
	L"Contrat :",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"1 jour",										
	L"1 semaine",
	L"2 semaines",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Pas de paquetage",
	L"Acheter paquetage",

	// Text on the Buttons

	L"TRANSFERT",			// to actually hire the merc
	L"Annuler",				// go back to the previous menu
	L"ENGAGER",				// go to menu in which you can hire the merc
	L"RACCROCHER",				// stops talking with the merc
	L"OK",									
	L"MESSAGE",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Conférence vidéo avec",		
	L"Connexion. . .",

	L"tout compris"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"TRANSFERT ACCEPTÉ",	// You hired the merc
	L"TRANSFERT REFUSÉ",		// Player doesn't have enough money, message 1
	L"FONDS INSUFFISANTS",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"En mission",											
	L"Veuillez laisser un message",
	L"Décédé(e)",

	//If you try to hire more mercs than game can support

	L"Équipe de mercenaires déjà au complet.",

	L"Message pré-enregistré",
	L"Message enregistré",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"Liens AIM",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"Historique AIM",					//Title

	// Text on the buttons at the bottom of the page

	L"Précédent",
	L"Accueil", 
	L"Anciens",
	L"Suivant",
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Prix",
	L"Expérience",
	L"Tir",
	L"Mécanique",
	L"Explosifs",
	L"Médecine",
	L"Santé",
	L"Agilité",
	L"Dextérité",
	L"Force",
	L"Commandement",
	L"Sagesse",
	L"Nom",

	// The title of the page, the above text gets added at the end of this text

	L"Tri ascendant des membres de l'AIM par %s",
	L"Tri descendant des membres de l'AIM par %s",

	// Instructions to the players on what to do

	L"Cliquez pour",
	L"sélectionner le mercenaire",			//10
	L"Clic droit pour",
	L"les options de tri",

	// Gets displayed on top of the merc's portrait if they are...

	L"Absent(e)",
	L"Décédé(e)",						//14
	L"En mission",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"PAGE 1",
	L"PAGE 2",
	L"PAGE 3",

	L"Anciens",	// Title of the page

	L"OK",			// Stops displaying information on selected merc
	L"Page suiv.",

};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"AIM et le logo A.I.M. sont des marques déposées dans la plupart des pays.",
	L"N'espérez même pas nous copier !",
	L"Copyright 2008-2009 A.I.M., Ltd. Tous droits réservés.",

	//Text for an advertisement that gets displayed on the AIM page

	L"Service des Fleuristes Associés",
	L"\"Nous livrons partout dans le monde\"",				//10
	L"Faites-le dans les règles de l'art",
	L"... la première fois",
	L"Si nous ne l'avons pas, c'est que vous n'en avez pas besoin.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"Accueil",
	L"Membres",
	L"Anciens",
	L"Règlement",
	L"Historique",
	L"Liens",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"MARCHANDISE EN STOCK",		//Header for the merchandise available
	L"PAGE",				//The current store inventory page being displayed
	L"COÛT TOTAL",				//The total cost of the the items in the Dealer inventory area
	L"VALEUR TOTALE",			//The total value of items player wishes to sell
	L"ÉVALUATION",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSACTION",			//Button text which completes the deal. Makes the transaction.
	L"OK",				//Text for the button which will leave the shopkeeper interface.
	L"COÛT RÉPARATION",			//The amount the dealer will charge to repair the merc's goods
	L"1 HEURE",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d HEURES",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"RÉPARÉ",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Plus d'emplacements libres.",	//Message box that tells the user there is non more room to put there stuff
	L"%d MINUTES",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Objet lâché à terre.",
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
	L"Prendre",					// Take money from the player
	L"Donner",					// Give money to the player
	L"Annuler",					// Cancel the transfer
	L"Effacer",					// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Choix",			// tells the user to select either to give or take from the merc
	L"Montant",			// Enter the amount to transfer
	L"Transfert de fonds au mercenaire",		// Giving money to the merc
	L"Transfert de fonds du mercenaire",		// Taking money from the merc
	L"Fonds insuffisants",			// Not enough money to transfer
	L"Solde",				// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Voulez-vous déduire %s de votre compte pour combler la différence ?",
	L"Pas assez d'argent. Il vous manque %s",
	L"Voulez-vous déduire %s de votre compte pour couvrir le coût ?",
	L"Demander au vendeur de lancer la transaction",
	L"Demander au vendeur de réparer les objets sélectionnés",
	L"Terminer l'entretien",
	L"Solde actuel",

	L"Do you want to transfer %s Intel to cover the difference?",	// TODO.Translate
	L"Do you want to transfer %s Intel to cover the cost?",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Sauvegarder",
	L"Charger",
	L"Quitter",
	L">>",
	L"<<",
	L"OK",
	L"1.13 Features",
	L"New in 1.13",
	L"Options",

	//Text above the slider bars
	L"Effets",
	L"Dialogue",
	L"Musique",

	//Confirmation pop when the user selects..
	L"Quitter la partie et revenir au menu principal ?",

	L"Activez le mode dialogue ou sous-titre.",
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
	L"Enregistrer",
	L"Charger partie",
	L"Annuler",
	L"Enregistrement",
	L"Chargement",

	L"Enregistrement réussi",
	L"ERREUR lors de la sauvegarde !",
	L"Chargement réussi",
	L"ERREUR lors du chargement !",

	L"La version de la sauvegarde est différente de celle du jeu. Désirez-vous continuer?",
	L"Les fichiers de sauvegarde sont peut-être altérés. Voulez-vous les effacer?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"La version de la sauvegarde a changé. Désirez-vous continuer ?",
#else
	L"Tentative de chargement d'une sauvegarde de version précédente. Voulez-vous effectuer une mise à jour ?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"La version de la sauvegarde a changé. Désirez-vous continuer?",
#else
	L"Tentative de chargement d'une sauvegarde de version précédente. Voulez-vous effectuer une mise à jour?",
#endif

	L"Êtes-vous sûr de vouloir écraser la sauvegarde #%d ?",
	L"Voulez-vous charger la sauvegarde #%d ?",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Vous risquez de manquer d'espace. Il ne vous reste que %d Mo de libre alors que le jeu nécessite %d Mo d'espace libre.",	

	L"Enregistrement",			//When saving a game, a message box with this string appears on the screen

	L"Quelques armes",
	L"Toutes armes",
	L"Style réaliste",
	L"Style SF",

	L"Difficulté",
	L"Platinum Mode", //Placeholder English

	L"Qualité de Bobby Ray",
	L"Bonne",
	L"Meilleure",
	L"Excellente",
	L"Superbe",

	L"Le nouvel inventaire (NIV) ne peut se lancer en 640x480. Changez de résolution.",
	L"Le nouvel inventaire (NIV) ne fonctionne pas depuis le dossier \"data\" original.",

	L"La taille de l'équipe de votre sauvegarde n'est pas supportée par la résolution actuelle de votre écran. Augmenter la résolution et ré-essayez.",
	L"Stock de Bobby Ray",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Niveau carte",
	L"Vous n'avez pas de milice : vous devez entraîner les habitants de la ville.",
	L"Revenu quotidien",
	L"Assurance vie", 
	L"%s n'est pas fatigué(e).", 
	L"%s est en mouvement et ne peut dormir.", 
	L"%s est trop fatigué(e) pour obéir.",
	L"%s conduit.",
	L"L'escouade ne peut progresser, si l'un de ses membres se repose.",

	// stuff for contracts
	L"Vous pouvez payer les honoraires de ce mercenaire, mais vous ne pouvez pas vous offrir son assurance.",
	L"La prime d'assurance de %s coûte %s pour %d jour(s) supplémentaire(s). Voulez-vous la payer ?",
	L"Inventaire du secteur",
	L"Le mercenaire a un dépôt médical.",
	
	// other items
	L"Docteurs", // people acting a field medics and bandaging wounded mercs 
	L"Patients", // people who are being bandaged by a medic 
	L"OK", // Continue on with the game after autobandage is complete 
	L"Stop", // Stop autobandaging of patients by medics now 
	L"Désolé. Cette option n'est pas disponible.", // informs player this option/button has been disabled in the demo 
	L"%s n'a pas de caisse à outils.",
	L"%s n'a pas de trousse de soins.",
	L"Il y a trop peu de volontaires pour l'entraînement.",
	L"%s ne peut pas former plus de miliciens.",
	L"Le mercenaire a un contrat déterminé.", 
	L"Ce mercenaire n'est pas assuré.",
	L"Écran carte",		// 24
	
	// Flugente: disease texts describing what a map view does TODO.Translate
	L"This view shows how many rotting corpses are in a sector. The white number are corpses, the green number is accumulated disease, the sector colour indicates how widespread it is. GREY= No disease known of. GREEN to RED = escalating levels of disease.",	// TODO.Translate

	// Flugente: weather texts describing what a map view does
	L"This view shows current weather. No colour=Sunny. CYAN=Rain. BLUE=Thunderstorm. ORANGE=Sandstorm. WHITE=Snow.",	// TODO.Translate

	// Flugente: describe what intel map view does
	L"This view shows which sectors relevant what ongoing quests. Some data bought with intel is also shown here.", // TODO.Translate
};


STR16 pLandMarkInSectorString[] =
{
	L"L'escouade %d a remarqué quelque chose dans le secteur %s",
	L"L'escouade %s a remarqué quelque chose dans le secteur %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"L'entraînement de la milice vous coûtera $ ", // telling player how much it will cost
	L"Êtes-vous d'accord ?", // asking player if they wish to pay the amount requested
	L"Vous n'en avez pas les moyens.", // telling the player they can't afford to train this town
	L"Voulez-vous poursuivre l'entraînement de la milice à %s (%s %d) ?", // continue training this town?

	L"Coût $ ", // the cost in dollars to train militia
	L"(O/N)",   // abbreviated oui/non
	L"",	// unused
	L"L'entraînement des milices dans %d secteurs vous coûtera %d $. %s", // cost to train sveral sectors at once

	L"Vous ne pouvez pas payer les %d $ nécessaires à l'entraînement.",
	L"Vous ne pouvez poursuivre l'entraînement de la milice à %s que si cette ville est à niveau de loyauté de %d pour cent.",
	L"Vous ne pouvez plus entraîner de milice à %s.",
	L"libérer plus de secteurs d'une ville",

	L"libérer de nouveaux secteurs d'une ville",
	L"libérer plus de villes",
	L"reprendre vos secteurs perdus",
	L"progresser dans votre avancée",

	L"recruter plus de rebelles",
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"Vous ne pouvez retirer que 20 000 $ à la fois.",
	L"Êtes-vous sûr de vouloir déposer %s sur votre compte ?",
};

STR16	gzCopyrightText[] = 
{
	L"Copyright (C) 1999 Sir-tech Canada Ltd. Tous droits réservés.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Dialogue",
	L"Confirmations muettes",
	L"Sous-titres",
	L"Pause des dialogues",
	L"Animation fumée",
	L"Du sang et des tripes",
	L"Ne pas toucher à ma souris !",
	L"Ancienne méthode de sélection",
	L"Afficher chemin",
	L"Afficher tirs manqués",
	L"Confirmation temps réel",
	L"Notifications sommeil/réveil",
	L"Système métrique",
	L"Mettez en surbrillance les mercenaires",
	L"Figer curseur sur mercenaires",
	L"Figer curseur sur les portes",
	L"Objets en surbrillance",
	L"Afficher cimes",
	L"Smart Tree Tops", // TODO. Translate
	L"Affichage fil de fer",
	L"Curseur toit",
	L"Afficher chance de toucher",
	L"Curseur raf. pour raf. lance G.",
	L"Remarques des ennemis", // Changed from "Enemies Drop all Items" - SANDRO
	L"Lance-grenades grand angle",
	L"Autori. déplcmt silenci. tps réel", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"Espace pour escouade suivante",
	L"Ombres objets",
	L"Afficher portée armes en cases",
	L"Balle traçante pour tir simple",
	L"Son de pluie",
	L"Afficher corbeaux",
	L"Afficher infobulle soldat",
	L"Sauvegarde auto",
	L"Silence Skyrider !",
	L"EDB (mod rajoutant info utiles)",
	L"Mode tour par tour forcé",			// add forced turn mode
	L"Couleur alternative carte",		// Change color scheme of Strategic Map
	L"Montrer tirs alternatifs",			// Show alternate bullet graphics (tracers)
	L"Logical Bodytypes",
	L"Afficher grade du mercenaire",						// shows mercs ranks
	L"Afficher équip. sur portrait",
	L"Afficher icônes sur portrait",
	L"Désactiver échange curseur",		            // Disable Cursor Swap
	L"Entraîner en silence",						// Madd: mercs don't say quotes while training
	L"Réparer en silence",						// Madd: mercs don't say quotes while repairing
	L"Soigner en silence",						// Madd: mercs don't say quotes while doctoring
	L"Accélérer les tours ennemis",			// Automatic fast forward through AI turns
	L"Avec zombis",						// Flugente Zombies
	L"Propose objet/inventaire",				// the_bob : enable popups for picking items from sector inv
	L"Situer ennemi restant",				
	L"Afficher contenu LBE/DESC.",				
	L"Inverser molette/souris",
	L"Déplacement tactique",				// when multiple mercs are selected, they will try to keep their relative distances
	L"Show enemy location",					// show locator on last known enemy location
	L"--Options mode triche--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Forcer envois Bobby Ray",				// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--OPTIONS DE DEBUG--",					// an example options screen options header (pure text)
	L"Afficher déviation balle",					// Screen messages showing amount and direction of shot deviation.	
	L"Réinitialiser TOUTES les options du jeu",				// failsafe show/hide option to reset all options
	L"Voulez-vous vraiment réinitialiser ?",		// a do once and reset self option (button like effect)
	L"Autres Options de débug",		// allow debugging in release or mapeditor
	L"Afficher les options de débug",			// an example option that will show/hide other options
	L"Afficher zones souris activables",				// an example of a DEBUG build option
	L"-----------------",					// an example options screen options divider (pure text)

	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"THE_LAST_OPTION",
};

//This is the help text associated with the above toggles.
STR16	zOptionsScreenHelpText[] =
{
	// HEADROCK HAM 4: Added more tooltip text to some toggles, in order to explain them better.

	//speech
	L"Activez cette option pour entendre vos mercenaires lorsqu'ils parlent.",

	//Mute Confirmation
	L"Active/désactive les confirmations des mercenaires.",

	//Subtitles
	L"Affichage des sous-titres à l'écran.",

	//Key to advance speech
	L"Si les sous-titres s'affichent à l'écran,\ncette option vous permet de prendre le temps de les lire.",

	//Toggle smoke animation
	L"Désactivez cette option, si votre machine n'est pas suffisamment puissante.",

	//Blood n Gore
	L"Désactivez cette option, si le jeu vous paraît trop violent.",

	//Never move my mouse
	L"Activez cette option pour que le curseur ne se place pas automatiquement sur les boutons qui s'affichent.",

	//Old selection method
	L"Activez cette option pour retrouver vos automatismes de la version précédente.",

	//Show movement path
	L"Activez cette option pour afficher le chemin suivi par les mercenaires.\nVous pouvez la désactiver et utiliser la touche |M|A|J en cours de jeu.",

	//show misses
	L"Activez cette option pour voir où atterrissent tous vos tirs.",
	
	//Real Time Confirmation
	L"Activez cette option pour afficher une confirmation de mouvement en temps réel.",

	//Sleep/Wake notification
	L"Activez cette option pour être mis au courant de l'état de veille de vos mercenaires.",

	//Use the metric system
	L"Activez cette option pour que le jeu utilise le système métrique.",

	//Highlight Mercs
	L"Lorsqu'il est activé, le mercenaire est mis en surbrillance (invisible pour les ennemis).\nBasculer dans le jeu avec (|G)",

	//Smart cursor
	L"Activez cette option pour que le curseur se positionne directement sur un mercenaire quand il est à proximité.",

	//snap cursor to the door
	L"Activez cette option pour que le curseur se positionne directement sur une porte quand il est à proximité.",

	//glow items 
	L"Activez cette option pour mettre les objets en évidence. (|C|T|R|L+|A|L|T+||I)",

	//toggle tree tops
	L"Activez cette option pour afficher la cime des arbres. (|T)",
	
	//smart tree tops
	L"When ON, hides tree tops near visible mercs and cursor position.",	// TODO.Translate

	//toggle wireframe
	L"Activez cette option pour afficher les murs en fil de fer. (|C|T|R|L+|A|L|T+||W)",

	L"Activez cette option pour afficher le curseur toit. (|Début)",

	// Options for 1.13
	L"Si activé, affiche une barre de probabilités de succès sur le curseur.",
	L"Si activé, les rafales de lance-grenades ont un curseur de rafale.",
	L"Si activé, les ennemis feront de temps en temps des remarques sur certaines actions.", // Changed from Enemies Drop All Items - SANDRO
	L"Si activé, les grenades des lance-grenades ont un grand angle (|A|l|t+|Q).",
	L"Si activé, le mode tour par tour ne sera pas actif, si vous n'êtes pas vu ou entendu par l'ennemi à moins d'appuyer sur |C|t|r+|X.", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Si activé, |E|S|P|A|C|E sélectionne l'escouade suivante.",
	L"Si activé, les ombres d'objets sont affichées.",
	L"Si activé, la portée des armes est affichée en nombres de cases.",
	L"Si activé, les effets de traçantes sont affichés pour les tirs simples.",
	L"Si activé, le son de pluie est audible quand il pleut.",
	L"Si activé, les corbeaux sont présents dans le jeu.",
	L"Si activé, une fenêtre info-bulle apparaît lorsque vous appuyez sur |A|L|T et que le curseur est sur un ennemi.",
	L"Si activé, le jeu est sauvegardé à chaque nouveau tour du joueur.",
	L"Si activé, les confirmations insistantes de Skyrider cessent.",
	L"Si activé, l'EDB sera affiché pour les armes et objets.",
	L"Si cette option est activée et que des ennemis sont présents,\nle mode tour par tour est actif tant qu'il reste \ndes ennemis dans le secteur (|C|T|R|L+|T).",	// add forced turn mode
	L"Si activé, la carte stratégique sera colorée différemment selon l'exploration.",
	L"Si activé, le graphisme des tirs alternatifs sera affiché quand vous tirerez.",
	L"When ON, mercenary body graphic can change along with equipped gear.",	// TODO.Translate
	L"Si activée, le grade sera affiché devant le nom du merc. dans la carte stratégique.",
	L"Si activé, vous verrez l'équipement du mercenaire à travers son portrait.",
	L"Si activé, vous verrez les icônes correspondant à l'équipement porté en bas à droite du portrait.",
	L"Si activé, le curseur ne basculera pas entre changer de position et une autre action. Appuyez sur |x pour initier un échange rapide.",
	L"Si activé, les mercenaires ne feront pas de compte rendu des progrès pendant la formation.",
	L"Si activé, les mercenaires ne feront pas de compte rendu des progrès sur les réparations.",
	L"Si activé, les mercenaires ne feront pas de compte rendu des progrès sur les soins.",
	L"Si activé, les tours de l'IA seront plus rapides.",
	L"Si activé, les zombis seront présents. Soyez au courant !",							// allow zombies
	L"Si activé, ouvrir inventaire d'un mercenaire et celui du secteur.\nClic gauche sur un emplacement vide inv/obj/arme et une fenêtre \nproposera des choix.",
	L"Si activé, la zone où se trouve le reste des ennemis dans le secteur, est mis en évidence.",
	L"Si activé, montre le contenu d'un élément LBE quand la fenêtre de description est ouverte.",
	L"Si activé, inverse le sens de la molette de la souris.",
	L"When ON and multiple mercs are selected, they will try to keep their relative distances while moving.\n(press |C|t|r|l+|A|l|t+|G to toggle mode or |S|h|i|f|t + click to move in formation)", //TODO.Translate
	L"When ON, shows last known enemy location.",	//TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Forcer tous les envois en attente de Bobby Ray",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g : Si activé, annoncera la distance de déviation de chaque tir à partir\ndu centre de la cible, en prenant en compte tous les facteurs du NSCDT.",
	L"Cliquer ici pour corriger les paramètres de jeu corrompus",							// failsafe show/hide option to reset all options
	L"Cliquer ici pour corriger les paramètres de jeu corrompus",							// a do once and reset self option (button like effect)
	L"Autoriser les options de debug dans les releases ou les mapeditor",				// allow debugging in release or mapeditor
	L"Activer pour afficher les options de debug",						// an example option that will show/hide other options
	L"Essaye de rendre visible les zones souris en les encadrant",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};


STR16	gzGIOScreenText[] =
{
	L"CONFIGURATION DU JEU",
#ifdef JA2UB
	L"Texte de Manuel aléatoire",
	L"Non",
	L"Oui",
#else
	L"Style de jeu",
	L"Réaliste",
	L"S-F",
#endif	
	L"Platinum", //Placeholder English
	L"Armes disponibles",
	L"Toutes",
	L"Quelques-unes",
	L"Difficulté",
	L"Novice",
	L"Expérimenté",
	L"Expert",
	L"INCROYABLE",
	L"Commencer",
	L"Annuler",
	L"En combat",
	L"Sauv. à volonté",
	L"Iron Man",
	L"Désactivé pour la démo",
	L"Qualité de Bobby Ray",
	L"Bonne",
	L"Meilleure",
	L"Excellente",
	L"Superbe",
	L"Inventaire/Accessoire",
	L"NON UTILISÉ",
	L"NON UTILISÉ",
	L"Charge jeu multi",
	L"CONFIGURATION DU JEU (Les paramètres serveur seulement prennent effet)",
	// Added by SANDRO
	L"Compétences (Traits)",
	L"Anciennnes",
	L"Nouvelles",
	L"Nombre max. de merc IMP",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Objets lâchés par l'ennemi",
	L"Non",
	L"Oui",
#ifdef JA2UB
	L"John et Tex",
	L"Aléatoire",
	L"Les deux",
#else
	L"Nombre de terroristes",
	L"Aléatoire",
	L"Tous",
#endif	
	L"Cachettes d'armes secrètes",
	L"Aléatoire",
	L"Toutes",
	L"Progression des objets",
	L"Très lente",
	L"Lente",
	L"Normal",
	L"Rapide",
	L"Très rapide",
	L"Ancien/Ancien",
	L"Nouveau/Ancien",
	L"Nouveau/Nouveau",

	// Squad Size
	L"Taille max. de l'escouade",
	L"6",
	L"8",
	L"10",
	//L"Faster Bobby Ray Shipments",
	L"Coût de l'inventaire en PA",
	
	L"Nouv. syst. chance de toucher",
	L"Syst. amélioré d'interruption",
	L"Passif des mercenaires",
	L"Système alimentaire",
	L"Stock de Bobby Ray",

	// anv: extra iron man modes
	L"Soft Iron Man",	// TODO.Translate
	L"Extreme Iron Man",	// TODO.Translate
};

STR16	gzMPJScreenText[] =
{
	L"MULTIJOUEURS",
	L"Rejoindre",
	L"Héberger",
	L"Annuler",
	L"Rafraichir",
	L"Nom du joueur",
	L"IP du serveur",
	L"Port",
	L"nom du serveur",
	L"# Plrs",
	L"Version",
	L"Type de jeu",
	L"Ping",
	L"Vous devez entrer un nom de joueur",
	L"Vous devez entrer une adresse IP de serveur valide. Par exemple : 84.114.195.239",
	L"Vous devez entrer un port de serveur valide entre 1 et 65535.",
};


STR16 gzMPJHelpText[] =
{
	L"Visiter http://webchat.quakenet.org/?channels=ja2-multiplayer pour trouver d'autres joueurs.",
	L"Vous pouvez appuyer sur 'y' pour ouvrir la fenêtre de chat ingame, après avoir été connecté au serveur.",

	L"HÉBERGER",
	L"Entrer '127.0.0.1' pour l'IP et un nombre plus grand que 60000 pour le port.",
	L"Assurez vous que les ports (UDP, TCP) sont ouverts sur votre routeur. Pour plus d'informations visitez : http://portforward.com",
	L"Vous devez envoyer (via IRC, MSN, ICQ, etc.) votre IP externe (http://www.whatismyip.com) et votre numéro de port aux autres joueurs.",
	L"Cliquez sur \"Héberger\" pour héberger une nouvelle partie en multijoueurs.",
	
	L"REJOINDRE",
	L"L'hébergeur doit vous envoyer (via IRC, MSN, ICQ, etc.) son IP externe ansi que son numéro de port.",
	L"Entrez l'IP externe ainsi que le port du serveur.",
	L"Cliquer sur \"Rejoindre\" pour rejoindre une partie multijoueurs déjà existante.",
};

STR16	gzMPHScreenText[] =
{
	L"HÉBERGER",
	L"Commencer",
	L"Annuler",
	L"Nom du serveur",
	L"Type de jeu",
	L"À mort",
	L"À mort/Équipe",
	L"Coopératif",
	L"Joueurs max.",
	L"Mercs max.",
	L"Sélection mercenaire",
	L"Mercenaire embauché",
	L"Embauché par les joueurs",
	L"Départ avec argent",
	L"Autoriser l'embauche d'un même mercenaire",
	L"Reporter les mercenaires embauchés",
	L"Bobby Ray",
	L"Bord de départ",
	L"Vous devez entrer un nom de serveur",
	L"",
	L"",
	L"Départs",
	L"",
	L"",
	L"Dégâts des armes",
	L"",
	L"Tounures prévues",
	L"",
	L"Activer les civils en CO-OP",
	L"",
	L"Maximum d'ennemis en CO-OP",
	L"Synchroniser le répertoire du jeu",
	L"MP Sync. Directory",
	L"Vous devez entrer un répertoire de transfert de fichier.",
	L"(Utilisez '/' au lieu '\\' pour délimiter les dossiers.)",
	L"Le répertoire de synchronisation indiqué n'existe pas.",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	// Max. Enemies / Report Hired Merc / Enable Civs in CO-OP
	L"Oui",
	L"Non",
	// Starting Time
	L"Matin",
	L"Après-midi",
	L"Nuit",
	// Starting Cash
	L"Faible",
	L"Moyen",
	L"Haut",
	L"Illimité",
	// Time Turns
	L"Jamais",
	L"Lent",
	L"Moyen",
	L"Rapide",
	// Weapon Damage
	L"Très lent",
	L"Lent",
	L"Normal",
	// Merc Hire
	L"Aléatoire",
	L"Normal",
	// Sector Edge
	L"Aléatoire",
	L"Sélectionnable",
	// Bobby Ray / Hire same merc
	L"Désactiver",
	L"Autoriser",
};

STR16 pDeliveryLocationStrings[] =
{
	L"Austin",			//Austin, Texas, USA	
	L"Bagdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Hong Kong",		//Hong Kong, Hong Kong
	L"Beyrouth",			//Beirut, Lebanon	(Middle East)
	L"Londres",			//London, England
	L"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Métavira",		//The island of Metavira was the fictional location used by JA1
	L"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"Moscou",			//Moscow, USSR
	L"New-York",		//New York, New York, USA
	L"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Paris",				//Paris, France
	L"Tripoli",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokyo",				//Tokyo, Japan
	L"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Êtes-vous sûr de vous ? Une valeur de ZÉRO signifie que vous serez INCAPABLE d'utiliser cette compétence.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"(8 Caractères Max)",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Analyse",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Nous vous remercions, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Voix", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Mort(e)",
	L"Renvoyé(e)",
	L"Autre",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Personnel",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Pause",
	L"Reprendre (|P|a|u|s|e)",
	L"Pause (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Quitter la partie ?",
	L"OK",
	L"OUI",
	L"NON",
	L"Annuler",
	L"CONTRAT",
	L"MENT",
	L"Sans description", //Save slots that don't have a description.
	L"Partie sauvegardée.",
	L"Partie sauvegardée.",
	L"QuickSave", //The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"Jour",
	L"Mercs",
	L"Vide", //An empty save game slot
	L"Démo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"Version",			//Release build for JA2
	L"CpM",					//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"min",					//Abbreviation for minute.
	L"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"cart.",				//Abbreviation for rounds (# of bullets)
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Accueil",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"n/a",					//Lowercase acronym for not applicable.
	L"Entre-temps",		//Meanwhile
	L"%s est arrivée dans le secteur %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Version",
	L"Emplacement de sauvegarde rapide vide",
	L"Cet emplacement est réservé aux sauvegardes rapides effectuées depuis l'écran tactique (ALT+S).",
	L"Ouverte",
	L"Fermée",
	L"Espace disque insuffisant. Il ne vous reste que %s Mo de libre et Jagged Alliance 2 nécessite %s Mo.",
	L"%s embauché(e) sur le site AIM", 
	L"%s prend %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s a pris %s.", //'Merc name' has taken the drug
	L"%s n'a aucune compétence médicale.",//'Merc name' has non medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"L'intégrité du jeu n'est plus assurée.",
	L"ERREUR : CD-ROM manquant",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Pas assez de place !",
	
	//Can't change stance due to objects in the way...
	L"Impossible de changer de position ici.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Lâcher",
	L"Lancer",
	L"Donner",

	L"%s donné à %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Impossible de donner %s à %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" combiné(s)]",

	//Cheat modes
	L"Triche niveau 1",
	L"Triche niveau 2",

	//Toggling various stealth modes
	L"Escouade en mode discrétion.",
	L"Escouade en mode normal.",
	L"%s en mode discrétion.",
	L"%s en mode normal.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Fil de fer activé.",
	L"Fil de fer désactivé.",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Impossible de remonter...",
	L"Pas de niveau inférieur...",
	L"Entrer dans le sous-sol %d...",
	L"Sortir du sous-sol...",

	L"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Mode poursuite désactivé.",
	L"Mode poursuite activé.",
	L"Curseur toit désactivé.",
	L"Curseur toit activé.",
	L"Escouade %d active.",
	L"Vous ne pouvez pas payer le salaire de %s qui se monte à %s.",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Passer",
	L"%s ne peut sortir seul.",
	L"Une sauvegarde a été crée (Partie249.sav). Renommez-la (Partie01 - Partie10) pour pouvoir la charger ultérieurement.",
	L"%s a bu %s.",
	L"Un colis vient d'arriver à Drassen.",
 	L"%s devrait arriver au point d'entrée (secteur %s) au jour %d vers %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"Historique mis à jour.",
	L"Curseur de visée pour raf. gre. (Dispersion activée).",
	L"Curseur de trajectoire raf. gre. (Dispersion desact.).",
	L"Infobulle soldat activée", // Changed from Drop All On - SANDRO
	L"Infobulle soldat désactivée", 	// 80	// Changed from Drop All Off - SANDRO
	L"Petit angle pour lance-grenades",
	L"Grand angle pour lance-grenades",
	// forced turn mode strings
    L"Mode tour par tour forcé",
	L"Mode tour par tour normal",
	L"Mode de combat quitté",
	L"Mode tour par tour forcé activé, mode de combat activé",
	L"Partie enregistrée dans l'emplacement de sauvegarde automatique.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",
	
	L"Vous ne pouvez pas utiliser l'ancien système d'inventaire et le nouveau système d'accessoire en même temps.",
	
	L"Auto Save #", //91		// Text des Auto Saves im Load Screen mit ID
	L"Cet emplacement est réservé pour les sauvegardes automatiques et peut être activé/désactivé dans ja2_options.ini (AUTO_SAVE_EVERY_N_HOURS).", //92	// The text, when the user clicks on the save screen on an auto save
	L"Emplacement vide auto #", //93	// The text, when the auto save slot (1 - 5) is empty (not saved yet)
	L"AutoSaveJeu",		// 94	// The filename of the auto save, such as AutoSaveGame01 - AutoSaveGame05
	L"Save fin-tour #",	// 95	// The text for the tactical end turn auto save
	L"Enregistrement auto #",	// 96	// The message box, when doing auto save
	L"Enregistrement",	// 97	// The message box, when doing end turn auto save
	L"Emplacement fin-tour vide #",	// 98	// The message box, when doing auto save
	L"Cet emplacement est réservé pour les sauvegardes fin-tour et peut être activé/désactivé dans l'écran option.", //99	// The text, when the user clicks on the save screen on an auto save
	// Mouse tooltips
	L"QuickSave.sav",	// 100
	L"AutoSaveGame%02d.sav",	// 101
	L"Auto%02d.sav",	// 102
	L"SaveGame%02d.sav", //103
	// Lock / release mouse in windowed mode (window boundary)
	L"Verrouiller le curseur pour qu'il reste dans la fenêtre.",			// 104
	L"Libérer le curseur pour qu'il se déplace hors de la fenêtre.",			// 105
	L"Déplacement tactique activé",
	L"Déplacement tactique désactivé",
	L"Éclairage du mercenaire activé",
	L"Éclairage du mercenaire désactivé",
	L"Squad %s active.",	//TODO.Translate
	L"%s smoked %s.",
	L"Activate cheats?",
	L"Deactivate cheats?",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"OK",
	L"Défilement haut",
	L"Tout sélectionner",
	L"Défilement bas",
	L"Annuler",
};

STR16 pDoctorWarningString[] =
{
	L"%s est trop loin pour être soigné.",
	L"Impossible de soigner tout le monde.",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"Prendre (Clic droit)/poser (Clic gauche) Miliciens", // button help text informing player they can pick up or drop militia with this button
	L"Prendre (Clic droit)/poser (Clic gauche) Soldats",
	L"Prendre (Clic droit)/poser (Clic gauche) Vétérans",
	L"Répartition automatique",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"Allez sur le site de l'AIM et engagez des mercenaires (*Conseil* allez voir dans le Poste de travail)", // to inform the player to hired some mercs to get things going
#ifdef JA2UB
	L"Cliquez sur le bouton de compression du temps pour faire avancer votre escouade sur le terrain.", // to inform the player to hit time compression to get the game underway
#else
	L"Cliquez sur le bouton de compression du temps pour faire avancer votre escouade sur le terrain.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Erreur. Fichier manquant ou corrompu. L'application va s'arrêter.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Voir messages",
	L"Consulter les sites Internet",
	L"Consulter les documents attachés",
	L"Lire le compte-rendu",
	L"Afficher les infos de l'escouade",
	L"Afficher les états financiers",
	L"Fermer le Poste de travail",

	//Bottom task bar icons (if they exist):
	L"Vous avez de nouveaux messages",
	L"Vous avez reçu de nouveaux fichiers",

	//Bookmarks:
	L"Association Internationale des Mercenaires",
	L"Bobby Ray : Petits et Gros Calibres",
	L"Institut des Mercenaires Professionnels",
	L"Mouvement pour l'Entraînement et le Recrutement des Commandos",
	L"Morgue McGillicutty",
	L"Service des Fleuristes Associés",
	L"Courtiers d'Assurance des Mercenaires de l'AIM",
	//New Bookmarks
	L"",
	L"Encyclopédie",
	L"Salle de briefing",
	L"Comptes rendus",
	L"Mercenaries Love or Dislike You",	// TODO.Translate
	L"World Health Organization",
	L"Kerberus - Experience In Security",
	L"Militia Overview",	// TODO.Translate
	L"Recon Intelligence Services",	// TODO.Translate
	L"Controlled factories",	// TODO.Translate
};


STR16 gzHelpScreenText[] =
{
	L"Quitter l'écran d'aide",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Vous êtes en plein combat. Vous pouvez donner l'ordre de retraite depuis l'écran tactique.",
	L"Pénétrez dans le secteur pour reprendre le cours du combat. (|E)",
	L"Résolution automatique du combat. (|A)",
	L"Résolution automatique impossible lorsque vous êtes l'attaquant.",
	L"Résolution automatique impossible lorsque vous êtes pris en embuscade.",
	L"Résolution automatique impossible lorsque vous combattez des créatures dans les mines.",
	L"Résolution automatique impossible en présence de civils hostiles.",
	L"Résolution automatique impossible en présence de chats sauvages.",
	L"COMBAT EN COURS",
	L"Retraite impossible.",
};

STR16 gzMiscString[] =
{
	L"Votre milice continue le combat sans vos mercenaires...",
	L"Ce véhicule n'a plus besoin de carburant pour le moment.",
	L"Le réservoir est plein à %d%%.",
	L"L'armée de Deidranna a repris le contrôle de %s.",
	L"Vous avez perdu un site de ravitaillement.",
};

STR16	gzIntroScreen[] = 
{
	L"Vidéo d'introduction introuvable",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s entend un bruit de %s %s.",
	L"%s entend un bruit %s de MOUVEMENT %s.",
	L"%s entend un GRINCEMENT %s %s.",
	L"%s entend un CLAPOTIS %s %s.",
	L"%s entend un IMPACT %s %s.",
	L"%s entend un COUP DE FEU %s %s.", // anv: without this, all further noise notifications were off by 1!
	L"%s entend une EXPLOSION %s %s.",
	L"%s entend un CRI %s %s.",
	L"%s entend un IMPACT %s %s.",
	L"%s entend un IMPACT %s %s.",
	L"%s entend un BRUIT %s %s.",
	L"%s entend un BRUIT %s %s.",
	L"",											 // anv: placeholder for silent alarm
	L"%s entend une VOIX %s %s.", // anv: report enemy taunt to player
};


STR16 pTauntUnknownVoice[] =
{
	L"Voix inconnue",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Tri par nom (|F|1)",
	L"Tri par affectation (|F|2)",
	L"Tri par état de veille (|F|3)",
	L"Tri par lieu (|F|4)",
	L"Tri par destination (|F|5)",
	L"Tri par date de départ (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Erreur 404",
	L"Site introuvable.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Derniers envois",
	L"Commande #",
	L"Quantité d'objets",
	L"Commandé",
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
	L"Programmeur", 			// Chris Camfield
	L"Co-designer/Écrivain",							// Shaun Lyng
	L"Systèmes stratégiques & Programmeur",					//Kris Marnes
	L"Producteur/Co-designer",						// Ian Currie
	L"Co-designer/Conception des cartes",				// Linda Currie
	L"Artiste",													// Eric \"WTF\" Cheng
	L"Coordination, Assistance",				// Lynn Holowka
	L"Artiste Extraordinaire",						// Norman \"NRG\" Olsen
	L"Gourou du son",											// George Brooks
	L"Conception écrans/Artiste",					// Andrew Stacey
	L"Artiste en chef/Animateur",						// Scot Loving
	L"Programmeur en chef",									// Andrew \"Big Cheese Doddle\" Emmons
	L"Programmeur",											// Dave French
	L"Systèmes stratégiques & Programmeur",					// Alex Meduna
	L"Portraits",								// Joey \"Joeker\" Whelan",
};

STR16	gzCreditNameFunny[]=
{
	L"", 																			// Chris Camfield
	L"(ah, la ponctuation...)",					// Shaun Lyng
	L"(\"C'est bon, trois fois rien\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(j'ai passé l'âge)",				// Ian Currie
	L"(et en plus je bosse sur Wizardry 8)",						// Linda Currie
	L"(on m'a forcé !)",			// Eric \"WTF\" Cheng
	L"(partie en cours de route...)",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(Tête de mort et fou de jazz)",						// Andrew Stacey
	L"(en fait il s'appelle Robert)",							// Scot Loving
	L"(la seule personne un peu responsable de l'équipe)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(bon, je vais pouvoir réparer ma moto)",	// Dave French
	L"(piqué à l'équipe de Wizardry 8)",							// Alex Meduna
	L"(conception des objets et des écrans de chargement !)",	// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s a terminé la réparation de ses objets.",
	L"%s a terminé la réparation des armes & protections.",
	L"%s a terminé la réparation des objets portés.",
	L"%s a fini de réparer les grands objets portés par chacun.",
	L"%s a fini de réparer les moyens objets portés par chacun.",
	L"%s a fini de réparer les petits objets portés par chacun.",
	L"%s a fini de réparer le mécanisme LBE de chacun.",
	L"%s finished cleaning everyone's guns.",	// TODO.Translate
};

STR16 zGioDifConfirmText[]=
{
	L"Vous avez choisi le mode de difficulté NOVICE. Ce mode de jeu est conseillé pour les joueurs qui découvrent Jagged Alliance, qui n'ont pas l'habitude de jouer à des jeux de stratégie ou qui souhaitent que les combats ne durent pas trop longtemps. Ce choix influe sur de nombreux paramètres du jeu. Êtes-vous certain de vouloir jouer en mode Novice ?",
	L"Vous avez choisi le mode de difficulté EXPÉRIMENTE. Ce mode de jeu est conseillé pour les joueurs qui ont déjà joué à Jagged Alliance ou des jeux de stratégie. Ce choix influe sur de nombreux paramètres du jeu. Êtes-vous certain de vouloir jouer en mode Expérimenté ?",
	L"Vous avez choisi le mode de difficulté EXPERT. Vous aurez été prévenu. Ne venez pas vous plaindre, si vos mercenaires quittent Arulco dans un cerceuil. Ce choix influe sur de nombreux paramètres du jeu. Êtes-vous certain de vouloir jouer en mode Expert ?",
	L"Vous avez choisi le mode de difficulté INCROYABLE. ATTENTION : Ne venez pas vous plaindre, si vos mercenaires quittent Arulco en petits morceaux... Deidranna va vous tuer. À coup sûr.  Ce choix influe sur de nombreux paramètres du jeu. Êtes-vous certain de vouloir jouer en mode INCROYABLE ?",
};

STR16 gzLateLocalizedString[] =
{
	L"Données de l'écran de chargement de %S introuvables...",

	//1-5
	L"Le robot ne peut quitter ce secteur par lui-même.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Compression du temps impossible. C'est bientôt le feu d'artifice !",  

	//'Name' refuses to move.
	L"%s refuse d'avancer.",

	//%s a merc name
	L"%s n'a pas assez d'énergie pour changer de position.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s n'a plus de carburant ; le véhicule est bloqué à %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"au-dessus",
	L"en-dessous",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Aucun de vos mercenaires n'a de compétence médicale.",
	L"Plus de bandages !",
	L"Pas assez de bandages pour soigner tout le monde.",
	L"Aucun de vos mercenaires n'a besoin de soins.",
	L"Soins automatiques.",
	L"Tous vos mercenaires ont été soignés.",

	//14
#ifdef JA2UB
	L"Tracona",
#else
	L"Arulco",
#endif
    L"(toit)",

	L"Santé : %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d contre %d",
	
	L"Plus de place dans le %s !",  //(ex "The ice cream truck is full")

    L"%s requiert des soins bien plus importants et/ou du repos.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s a été touché aux jambes ! Il ne peut plus se tenir debout !",
	//Name can't speak right now.
	L"%s ne peut pas parler pour le moment.",

	//22-24 plural versions 
	L"%d miliciens ont été promus vétérans.",
	L"%d miliciens ont été promus soldats.",
	L"%d soldats ont été promus vétérans.",

	//25
	L"Échanger",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s est devenu dingue !",

	//27-28
	//Messages why a player can't time compress.
	L"Nous vous déconseillons d'utiliser la compression du temps ; vous avez des mercenaires dans le secteur %s.",
	L"Nous vous déconseillons d'utiliser la compression du temps lorsque vos mercenaires se trouvent dans des mines infestées de créatures.",

	//29-31 singular versions 
	L"1 milicien a été promu vétéran.",
	L"1 milicien a été promu soldat.",
	L"1 soldat a été promu vétéran.",

	//32-34
	L"%s ne dit rien.",
	L"Revenir à la surface ?",
	L"(Escouade %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s a réparé pour %s : %s",//inverted order !!! Red has repaired the MP5 of Scope

	//36
	L"Chat",	// Max. 9 Characters. Should be "bloodcat".

	//37-38 "Name trips and falls"
	L"%s trébuche et tombe",
	L"Cet objet ne peut être pris d'ici.",

	//39
	L"Il ne vous reste aucun mercenaire en état de se battre. La milice combattra les créatures seule.",

	//40-43
	//%s is the name of merc.
	L"%s n'a plus de trousse de soins !",
	L"%s n'a aucune compétence médicale !",
	L"%s n'a plus de caisse à outils !",
	L"%s n'a aucune compétence en mécanique !",

	//44-45
	L"Temps de réparation",
	L"%s ne peut pas voir cette personne.",

	//46-48
	L"Le prolongateur de %s est tombé !",
	L"Seulement %d personnes sont autorisées dans ce secteur pour former la milice mobile.",	
    L"Êtes-vous sûr ?",

	//49-50
	L"Compression du temps",
	L"Le réservoir est plein.",

	//51-52 Fast help text in mapscreen.
	L"Compression du temps (|E|S|P|A|C|E)",
	L"Arrêt de la compression du temps (|E|C|H|A|P)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s a désenrayé : %s",
	L"%s a désenrayé pour %s : %s",//inverted !!! magic has unjammed the g11 of raven

	//55 
	L"Compression du temps impossible dans l'écran d'inventaire.",

	L"Le CD Play de Jagged Alliance 2 est introuvable. L'application va se terminer.",

	L"Objets associés.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Actuel/Maximum : %d%%/%d%%",

	L"Escorter John et Mary ?",

	//60	
	L"Interrupteur activé.",

	L"%s : attachement de protection détruit !",
	L"%s tire %d fois de plus que prévu !",
	L"%s tire 1 fois de plus que prévu !",

	L"Vous devez d'abord fermer la fenêtre de description !",

	L"Compression du temps impossible avec des civils hostiles et/ou des chats sauvages dans ce secteur. ", // 65
};

STR16 gzCWStrings[] = 
{
	L"Faut-il appelez des renforts pour %s dans les secteurs adjacents ?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Emplacement : %d\n",
	L"%s|Luminosité : %d/%d\n",
	L"%s|Distance de la |Cible : %d\n",
	L"%s|I|D : %d\n",
	L"%s|Ordres : %d\n",
	L"%s|Attitude : %d\n",
	L"%s|P|A |Actuel : %d\n",
	L"%s|Santé |Actuelle : %d\n",
	L"%s|Énergie |Actuelle : %d\n",
	L"%s|Moral |Actuel : %d\n",
	L"%s|C|hoc |Actuel : %d\n",
	L"%s|V|aleur de |S|uppression |Actuelle : %d\n",
	// Full info
	L"%s|Casque : %s\n",
	L"%s|Veste : %s\n",
	L"%s|Pantalon : %s\n",
	// Limited, Basic
	L"|Protection : ",
	L"casque",
	L"veste",
	L"pantalon",
	L"oui",
	L"pas de protection",
	L"%s|L|V|N : %s\n",
	L"Pas de lunette de vision de nuit",
	L"%s|Masque à |Gaz : %s\n",
	L"pas de masque à gaz",
	L"%s|Emplacement |1 |tête : %s\n",
	L"%s|Emplacement |2 |tête : %s\n",
	L"\n(dans le sac de transport) ",
	L"%s|Arme : %s ",
	L"pas d'arme",
	L"Pistolet",
	L"PM",
	L"Fusil",
	L"FM",
	L"Fusil à pompe",
	L"Arme blanche",
	L"Armes lourdes",
	L"pas de casque",
	L"pas de veste",
	L"pas de pantalon",
	L"|Protection : %s\n", 
	 // Added - SANDRO
	L"%s|Compétence 1 : %s\n",
	L"%s|Compétence 2 : %s\n", 
	L"%s|Compétence 3 : %s\n",
    // Additional suppression effects - sevenfm	
    L"%s|P|A perdu(s) en raison de |S|uppression : %d\n",
    L"%s|Tolérance de |Suppression : %d\n",
    L"%s|Niveau |Effectif du |C|hoc : %d\n",
    L"%s|Moral de l'|I|A : %d\n",
};

STR16 New113Message[] =
{
	L"La tempête débute.",
	L"La tempête est finie.",
	L"Il commence à pleuvoir.",
	L"La pluie cesse.",
	L"Attention aux tireurs isolés...",
	L"Tir de couverture !",	
	L"RAF.",
	L"AUTO",
	L"LG",
	L"RAF. LG",
	L"LG AUTO",
	L"S/CA",
	L"S/CA R",
	L"S/CA A",
	L"BAÏONNETTE",
	L"Tireur embusqué !",
	L"Impossible de partager l'argent avec un objet sélectionné.",
	L"Arrivée de nouvelles recrues est déroutée au secteur %s, car le point d'arrivée prévu %s est sous contrôle ennemi.",
	L"Article supprimé",
	L"A supprimé tous les articles de ce type",
	L"Article vendu",
	L"A vendu tous les articles de ce type",
	L"Vous devriez vérifier si votre accessoire de vision convient bien à ce type de lieu",
	// Real Time Mode messages
	L"Encore en combat",
	L"pas d'ennemi en vue",
	L"Mode discrétion en temps réel désactivé",
	L"Mode discrétion en temps réel activé",
	L"Ennemis repérés !", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s a réussi son vol !",
	L"%s n'avait pas assez de points d'action pour voler tous les articles choisis.",
	L"Voulez-vous faire de la chirurgie sur %s avant de le bander ? (Vous pouvez lui guérir %i santé.)",
	L"Voulez-vous faire de la chirurgie sur %s ? (Vous pouvez lui guérir %i santé.)",
	L"Voulez-vous lui faire les premiers soins d'abord ? (%i patient(s))",
	L"Voulez-vous faire les premiers soins sur ce patient d'abord ?",
	L"Appliquez les premiers soins automatiquement avec chirurgie ou sans ?",
	L"Do you want to perform surgery on %s before bandaging? (You can heal about %i health, *: %i by blood bag use.)",	// TODO.Translate
	L"Do you want to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"Do you wish to perform surgery on %s? (You can heal about %i Health, *: %i by blood bag use.)",
	L"La chirurgie sur %s est finie.",
	L"%s est touché(e) au torse et perd un maximum de points de vie !",
	L"%s est touché(e) au torse et perd %d points de vie !",
	L"%s est devenu(e) aveugle par le souffle de l'explosion !",
	L"%s a regagné 1 point sur les %s perdus",
	L"%s a regagné %d points sur les %s perdus",
	L"Vos compétences de reconnaissance vous ont empêchés d'être pris en embuscade par l'ennemi !",
	L"Grâce à vos compétences de reconnaissance vous avez réussi à éviter un groupe de félins !",
	L"%s est frappé à l'aine et tombe de douleur !",
	//////////////////////////////////////////////////////////////////////////////////////
	L"Attention : Cadavre ennemi trouvé !!!",
	L"%s [%d cart]\n%s %1.1f %s",
	L"PA insuffisant ! Coût %d et vous avez %d.",
	L"Astuce : %s",
	L"Moral du joueur : %d - Moral de l'ennemi : %6.0f",		// Surrender values to be printed, if DISPLAY_SURRENDER_VALUES = TRUE

	L"Compétence inutilisable dans ces conditions !",
	L"Impossible de construire pendant que des ennemis sont dans le secteur !",
	L"Impossible de faire un repérage radio à cet endroit !",
	L"Numéro de grille incorrect pour un tir d'artillerie !",
	L"Les fréquences radio sont brouillées. Pas de communications possibles !",
	L"L'action radio a échoué !",
	L"Pas assez d'obus de mortier dans le secteur pour un tir de barrage !",
	L"Aucun obus éclairant trouvé dans Items.xml !",
	L"Aucun mortier trouvé, tir de barrage impossible !",
	L"Brouillage radio déjà en cours, inutile d'en lancer un autre !",
	L"Écoute des sons alentour déjà en cours, inutile d'en lancer une autre !",
	L"Repérage radio déjà en cours, inutile d'en lancer un autre !",
	L"Balayage des fréquences déjà en cours, inutile d'en lancer un autre !",
	L"%s n'a pas pu appliquer %s à %s.",
	L"%s demande des renforts depuis %s.",
	L"%s a épuisé la batterie de sa radio.",
	L"une radio",
	L"des jumelles",
	L"de la patience",
	L"%s's shield has been destroyed!",	// TODO.Translate
	L" DELAY",	// TODO.Translate
	L"Yes*",
	L"Yes",
	L"No",
	L"%s applied %s to %s.", // TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s sombre dans la peur !",
	L"%s est cloué(e) au sol !",
	L"%s tire plus de fois que désiré !",
	L"Vous ne pouvez pas former de milice dans ce secteur.",
	L"La milice prend %s.",
	L"Vous ne pouvez pas former de milice, alors que des ennemis sont présents !", 
	// 6 - 10
	L"%s n'a pas assez de points en commandement pour former la milice.",
	L"Seulement %d personnes sont autorisées dans ce secteur pour former la milice mobile.",
	L"Aucune case de libre à %s ou autour pour de nouvelles milices mobiles !",
	L"Vous devez avoir %d villes de milice dans chaque secteur libéré de %s pour pouvoir former une milice mobile.",
	L"Aucune affectation ne peut être faite tant que les ennemis sont présents !",
	// 11 - 15
	L"%s n'a pas assez en sagesse pour être affecté(e) à cette installation.",
	L"L'installation : %s est déjà entièrement pourvue en personnel.",
	L"Cela va coûter %d $ par heure pour cette affectation. Voulez-vous continuer ?",
	L"À ce jour, vous n'avez pas assez d'argent pour payer toutes vos dépenses. Vos %d $ ont déjà été versés, mais vous devez encore %d $. Les habitants ne sont pas très patients...",
	L"À ce jour, vous n'avez pas assez d'argent pour payer toutes vos dépenses. Vous devez %d $. Les habitants ne sont pas très patients...",
	// 16 - 20
	L"Vous avez une dette échue de %d $ et pas d'argent pour la régler !",
	L"Vous avez une dette échue de %d $. Vous ne pouvez pas donner cette affectation avant que vous n'ayez assez d'argent pour régler la dette entière.",
	L"Vous avez une dette échue de %d $. Voulez-vous payer ?",
	L"N/A à ce secteur",
	L"Coût quotidien",
	// 21 - 25
	L"Fonds insuffisants pour payer toute la milice enrôlée ! %d membres de la milice sont partis pour rentrer chez eux.",
	L"Pour voir la description d'un objet pendant un combat, vous devez d'abord le prendre vous-même.", // HAM 5
	L"Pour attacher un objet à un autre pendant un combat, vous devez d'abord les prendre vous-même.", // HAM 5
	L"Pour combiner deux objets pendant un combat, vous devez d'abord les prendre vous-même.", // HAM 5
};

// HEADROCK HAM 5: Text dealing exclusively with Item Transformations.
STR16 gzTransformationMessage[] = 
{
	L"Aucun choix disponible",
	L"%s a été séparé(e) en plusieurs morceaux.",
	L"%s a été séparé(e) en plusieurs morceaux. %s les a récupérés dans son inventaire.",
	L"Il n'y avait pas suffisamment de place dans l'inventaire après la transformation, %s a dû poser des objets au sol.",
	L"%s a été séparé(e) en plusieurs morceaux. Il n'y avait pas suffisamment de place dans l'inventaire, %s a dû poser des objets au sol.",
	L"Voulez-vous transformer les %d objets dans ce tas ? (Pour transformer un seul objet, retirez-le du tas d'abord)",
	// 6 - 10
	L"Compléter l'inventaire",
	L"Conditionner en chargeurs %d coups",
	L"%s a été conditionné en %d chargeurs %d coups.",
	L"%s a été réparti dans l'inventaire de : %s.",
	L"%s n'a plus de place dans son inventaire pour des chargeurs de ce calibre !",
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
	L"Nous vous informons que de par ses perfomances passées, Gaston voit ses honoraires augmentés. Personellement, je ne suis pas surpris. ± ± Kline Speck T ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Soyez informé qu'à paritr de maintenant, les honoraires de Stogie ont augmenté en accord avec ses compétences. ± ± Kline Speck T. ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Sachez que l'expérience de Tex lui autorise une promotion. Son salaire a donc été ajusté pour refléter sa vraie valeur. ± ± Kline Speck T. ± ",
	// Biggins: Text from Line 49 in Email.edt
	L"Prenez note. De par ses performances accrues, Biggins voit le prix de ses services augmentés. ± ± Kline Speck T. ± ",
};

// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"TR du serveur AIM : Message de Kolesnikov Victor",
	L"Salut. Ici Monk. Message reçu. Je suis disponible si vous voulez me voir. ± ± J’attends votre appel. ±",

	// Brain
	L"TR du serveur AIM : Message de Allik Janno",
	L"Je suis prêt à considérer votre offre. Il y a un temps et un lieu pour tout. ± ± Allik Janno ±",

	// Scream
	L"TR du serveur AIM : Message de Vilde Lennart",
	L"Vilde Lennart est maintenant disponible! ±",

	// Henning
	L"TR du serveur AIM : Message de von Branitz Henning",
	L"J’ai reçu votre message, merci. Pour parler d’embauche, contactez-moi sur le site web de l’AIM. ± ± Von Branitz Henning ±",

	// Luc
	L"TR du serveur AIM : Message de Fabre Luc",
	L"Message reçu, merci ! Je suis heureux de considérer votre proposition. Vous savez où me trouver. ± ± Au plaisir de vous entendre. ±",

	// Laura
	L"TR du serveur AIM : Message du Dr Colin Laura",
	L"Salutations ! Merci pour votre message, il semble intéressant. ± ± Visiter l’AIM à nouveau, je serais ravie d’en entendre plus. ± ± Cordialement ! ± ± Dr Colin Laura ±",

	// Grace
	L"TR du serveur AIM : Message de Girelli Graziella",
	L"Vous vouliez me contacter, mais vous n’avez pas réussi. ± ± Une réunion de famille. Je suis sûr que vous comprenez ? J’en ai maintenant assez de la famille et serais très heureuse si vous voulez me contacter de nouveau sur le site AIM. ± ± Ciao ! ±",

	// Rudolf
	L"TR du serveur AIM : Message de Steiger Rudolf",
	L"Vous savez combien j’ai d’appel par jour ? Tous les branleurs pensent pouvoir m’appeler. ± ± Mais je suis de retour, si vous avez quelque chose d’intéressant pour moi. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"TR du serveur AIM : Message des disponibilités des mercs",
	L"J'ai reçu votre message. J'attends votre appel. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Tireur d'élite : Des yeux de faucon, vous pouvez tirer les ailes d'une mouche à cent mètres ! ± ",
	// Camouflage
	L"Camouflage : Sans compter qu'à côté de vous, les buissons semblent synthétiques ! ± ",	
	// SANDRO - new strings for new traits added
	// Ranger
	L"Ranger : Vous êtes celui du désert du Texas, n'est-ce pas ! ± ",	
	// Gunslinger
	L"Bandit : Avec un pistolet ou deux, vous pouvez être aussi mortel que Billy the Kid ! ± ",
	// Squadleader
	L"Commandant : Naturel leader et commandant, vous êtes le gros calibre, sans blague ! ± ",
	// Technician
	L"Technicien : Fixer des objets, retirer des pièges, poser des bombes, c'est ça votre boulot ! ± ",
	// Doctor
	L"Docteur :  Vous pouvez faire une intervention chirurgicale avec un couteau suisse et un chewing gum et cela n'importe où ! ± ",
	// Athletics
	L"Athlétique : Votre vitesse et votre vitalité sont au top des possibilités actuelles ! ± ",
	// Bodybuilding
	L"Culturiste : Cette grande figure musclée qui ne peut pas être dominée, est en faite vous en réalité ! ± ",
	// Demolitions
	L"Sabotage : Vous pouvez réduire à néant toute une ville rien qu'avec des produits ménagers ! ± ",
	// Scouting
	L"Reconnaissance : Rien n'échappe à votre vigilance ! ± ",
	// Covert ops
	L"Déguisement : Vous ferez passer 007 pour un amateur ! ± ",
	// Radio Operator
	L"Opérateur radio : Votre maitrise des appareils de communication élargit le champs des compétences tactiques et stratégiques de votre équipe. ± ",
	// Survival
	L"Survival: Nature is a second home to you. ± ",	// TODO.Translate
};

STR16 NewInvMessage[] = 
{
	L"Le sac à dos ne peut être ramassé pour le moment",
	L"Pas de place pour le sac à dos",
	L"Sac à dos non trouvé",
	L"La fermeture éclair fonctionne seulement en combat",
	L"Ne peut se déplacer, si la fermeture éclair est ouverte",
	L"Êtes-vous sûr de vouloir vendre tous les articles du secteur ?",
	L"Êtes-vous sûr de vouloir supprimer tous les articles du secteur ?",
	L"Ne peut pas escalader avec un sac à dos",
	L"Tous les sacs à dos sont posés à terre",
	L"Tous les sacs à dos sont ramassés",
	L"%s drops backpack",
	L"%s picks up backpack",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Initialisation du serveur RakNet...",
	L"Le serveur a démarré, en attente de connexion...",
	L"Vous devez maintenant vous connecter avec votre client sur le serveur en pressant '2'.",
	L"Le serveur est déjà démarré.",
	L"Le serveur n'a pas pu démarré. Terminé.",
	// 5
	L"%d/%d clients sont déjà en mode realtime.",
	L"Le serveur s'est déconnecté et s'est éteint.",
	L"Le serveur n'est pas démarré.",
	L"Les clients sont en cours de chargement, veuillez patienter...",
	L"Vous ne pouvez pas changer de dropzone alors que le serveur vient de démarrer.",
	// 10
	L"Fichier envoyé '%S' - 100/100",
	L"Envoie de fichier fini pour '%S'.",
	L"Départ d'envoie de fichier pour '%S'.",
	L"Utilisez la vue aérienne pour sélectionner la carte que vous voulez jouer. Si vous voulez changer de carte, vous devez le faire avant de cliquer sur \"Démarrer la partie\".",
};

STR16 MPClientMessage[] =
{
	// 0
	L"Initialisation du client RakNet...",		
	L"Connexion à l'IP : %S ...",
	L"Réception des optiosn de jeu :",
	L"Vous êtes déjà connecté.",
	L"Vous êtes déjà connecté...",
	// 5
	L"Client #%d - '%S' a engagé '%s'.",
	L"Client #%d - '%S' a engagé un autre mercenaire.",
	L"Vous êtes prêt - Total prêts = %d/%d.",
	L"Vous n'êtes pas encore prêts - Total prêt = %d/%d.",
	L"Départ de bataille...",
	// 10
	L"Client #%d - '%S' est prêt - Total prêts = %d/%d.",
	L"Client #%d - '%S' n'est pas encore prêt - Total prêts = %d/%d",
	L"Vous êtes prêt. En attente des autres clients... Cliquez sur 'OK', si vous n'êtes plus prêt.",
	L"Laissez-nous, la bataille commence !",
	L"Un client doit poser sa candidature pour démarrer la partie.",
	// 15
	L"Le jeu ne peut démarrer. Aucun mercenaire n'a été engagé...",
	L"En attente de 'OK' de la part du serveur pour ouvrir le portable...",
	L"Interrompu",
	L"Fin de l'interromption",
	L"Coordonnées de réseau de souris :",
	// 20
	L"X : %d, Y : %d",
	L"Réseau numéro : %d",
	L"Le serveur figure seulement",
	L"Choissez les étapes à ignorer :  ('1' - Activer portable/l'embauche)  ('2' - lancer/charger level)  ('3' - Unlock UI)  ('4' - placement de finition)",	
	L"Secteur=%s, Clients max.=%d, Mercs max.=%d, Game_Mode=%d, Same Merc=%d, Multiplicateur de Dégâts=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Équip=%d, Dis Moral=%d, Testing=%d",
	// 25
	L"",
	L"Nouvelle conncetion : Client #%d - '%S'.",
	L"Équipe : %d.",//not used any more
	L"'%s' (client %d - '%S') a été tué par '%s' (client %d - '%S')",
	L"Client kické #%d - '%S'",
	// 30
	L"Début de manche pour les numéros de clients : #1: <Annuler>, #2: %S, #3: %S, #4: %S",
	L"Début de manche pour le client #%d",
	L"Requête pour le realtime...",
	L"Commutation en mode realtime.",
	L"Erreur lors de la commutation.",
	// 35
	L"Dévérouiller le portable pour l'embauche ? (Tous les clients sont connectés ?)",
	L"Le serveur a déverrouillé le portable pour l'embauche. Vous pouvez commencez a embauché !",
	L"Interruption.",
	L"Vous ne pouvez pas changer la dropzone, si vous êtes seulement un client et pas le gérant du serveur.",
	L"Vous avez décliné l'offre de vous rendre, car vous êtes dans une partie multijoueur.",
	// 40
	L"Tous vos mercenaires sont morts !",
	L"Mode spectateur activé.",
	L"Vous avez été vaincu !",
	L"Désolé, escalader sur les toits est interdit en multijoueur.",
	L"Vous avez embauché '%s'",
	// 45
	L"Vous ne pouvez pas changer la carte une fois que l'achat a commencé",
	L"Changement de carte : '%s'",
	L"Le client '%s' s'est déconnecté, il a été retiré du jeu",
	L"Vous avez été déconnecté du jeu, retourner au menu principal",
	L"Connexion échouée, reconnexion dans 5 s. Encore %i tentatives...",
	//50
	L"Connexion échouée, abandon de l'opération...",
	L"Vous ne pouvez pas démarrer la partie tant qu'un autre joueur ne s'est pas connecté",
	L"%s : %s",
	L"Envoyer à tous",
	L"Alliés seulement",
	// 55
	L"Vous ne pouvez pas rejoindre cette partie car elle a déjà commencé.",
	L"%s (équipe) : %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Réception de tous les fichiers depuis le serveur.",
	L"'%S' a fini de télécharger depuis le serveur.",
	L"'%S' a commencé à télécharger depuis le serveur.",
	L"Vous ne pouvez pas démarrer le jeu tant que tous les joueurs n'ont pas fini de recevoir les fichiers",
	L"Ce serveur requiert des fichiers modifiés pour pouvoir jouer, voulez-vous continuer ?",
	// 65
	L"Cliquez sur 'Ready' pour aller à l'écran tactique.",
	L"Vous ne pouvez pas vous connecter car votre version %S est différente de celle du serveur %S.",
	L"Vous avez tué un soldat ennemi.",
	L"Vous ne pouvez pas commencer la partie car toutes les équipes sont les mêmes.",
	L"Le serveur a choisi l'option du Nouvel Inventaire (NI), mais la résolution de votre écran ne le supporte pas.",
	// 70
	L"Impossible de sauver les fichiers reçus '%S'",
	L"La bombe de %s a été désamorcé par %s",
	L"Vous avez perdu, quel honte !",	// All over red rover
	L"Mode spectateur désactivé",
	L"Choisir le numéro du client a kické :",
	// 75
	L"La team %s a été anéantie.",
	L"Le client n'a pas réussi à démarrer. Terminé.",
	L"Le client s'est déconnecté et s'est fermé.",
	L"Le client n'est pas démarré.",
	L"INFO : Si le jeu est bloqué (la barre de progression des adversaires ne se déplace pas), notifier le au serveur en appuyant sur ALT + E pour aller directement à votre tour de jeu !",
	// 80
	L"Tour de l'IA - %d restant(s)",
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"E",
	L"S",
	L"O",
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
	L"Type de jeu : ",
	L"Joueurs : ",
	L"Merc pris : ",
	L"Vous ne pouvez pas changer le bord de départ tant que le portable est ouvert.",
	L"Vous ne pouvez pas changer d'équipe tant que le portable est ouvert.",
	L"Merc aléatoire : ",
	L"O",
	L"Difficulté : ",
	L"Version Serveur : ",
};

STR16 gzMPSScreenText[] =
{
	L"Tableaux des scores",
	L"Continue",
	L"Annulé",
	L"Joueurs",
	L"Tués",
	L"Morts",
	L"Armée de la Reine",
	L"Touchés",
	L"Ratés",
	L"Précision",
	L"Dégâts faits",
	L"Dégâts reçus",
	L"Attendez le serveur avant d'appuyer sur \"Continue\"."
};

STR16 gzMPCScreenText[] =
{
	L"Annulé",
	L"Connexion au serveur",
	L"Obtention des options du serveur",
	L"Téléchargement des fichiers modifiés",
	L"Appuyer sur \"ESC\" pour annulé ou \"Y\" pour parler",
	L"Appuyer sur \"ESC\" pour annulé",
	L"Prêt"
};

STR16 gzMPChatToggleText[] =
{
	L"Envoyer à tous",
	L"Envoyer aux alliés seulement",
};

STR16 gzMPChatboxText[] =
{
	L"Chat multijoueurs",
	L"\"ENTRÉE\" pour envoyer, \"ESC\" pour annuler",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"À la page suivante, vous allez choisir vos traits de compétence selon votre spécialisation professionnel comme un mercenaire. Pas plus de deux traits différents ou un trait expert peuvent être choisis.",
	L"Vous pouvez aussi choisir seulement un ou même aucun trait, ce qui vous donnera un bonus à vos points d'attributs, une sorte de compensation. Notez que les compétences : mécanique, ambidextre et camouflage ne peuvent pas être prises aux niveaux experts.",
	// For new major/minor traits
	L"L'étape suivante est le choix de vos traits de compétences. À la première page vous pouvez choisir jusqu'à deux traits principaux qui représentent surtout votre rôle dans une escouade. Tandis qu'à la deuxième page, c'est la liste de vos traits mineurs qui représentent des exploits personnels.",
	L"Pas plus de trois choix au total sont possibles. Ce qui signifie que si vous ne choisissez aucun trait principal, vous pourrez alors choisir trois traits secondaires. Si vous choisissez deux traits principaux (ou un en expert), vous pourrez alors choisir qu'un seul trait secondaire...",
};

STR16 sgAttributeSelectionText[] = 
{
	L"Ajustez, s'il vous plaît, vos attributs physiques selon vos vraies capacités. Vous ne pouvez pas augmenter les scores au-dessus de",
	L"IMP : Examen des attributs et compétences",
	L"Points bonus :",
	L"Départ au niveau",
	// New strings for new traits
	L"À la page suivante vous allez spécifier vos attributs physiques comme : la santé, la dextérité, l'agilité, la force et la sagesse. Les attributs ne peuvent pas aller plus bas que %d.",
	L"Le reste est appelé \"habilités\" et à la différence des attributs, ils peuvent être mis à zéro signifiant que vous serez un incapable dans cette habilité !",
	L"Tous les scores sont mis à un minimum au début. Notez que certains attributs sont mis a des valeurs spécifiques correspondant aux traits de compétence que vous avez choisis. Vous ne pouvez pas mettre ces attributs plus bas.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"IMP : Analyse du cacractère",
	L"L'analyse de votre personnage est la prochaine étape. À la première page, on vous montrera une liste d'attitudes à choisir. Nous imaginons bien que vous pourriez vous identifier à plusieurs d'entre elles, mais vous ne pourrez en choisir qu'une seule. Choisissez celle qui vous correspond le plus.",
	L"La deuxième page montre des handicaps que vous pourriez avoir. Si vous souffrez de n'importe lequel de ces handicaps, choisissez le (un seul choix est possible). Soyez honnête, pensez que c'est un entretien d'embauche et qu'il est toujours important de faire connaitre votre vraie personnalité.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Normal",
	L"Amical",
	L"Solitaire",
	L"Optimiste",
	L"Péssimiste",
	L"Aggressif",
	L"Arrogant",
	L"Gros tireur",
	L"Trou du cul",
	L"Lâche",
	L"IMP : Attitudes",
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Normal",
	L"Sociable",
	L"Solitaire",
	L"Optimiste",
	L"Assuré",
	L"Intellectuel",
	L"Primitif",
	L"Aggressif",
	L"Flegmatique",
	L"Intrépide",
	L"Pacifiste",
	L"Malicieux",
	L"Frimeur",
	L"Coward",	// TODO.Translate
	L"IMP : Traits de caractère",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"IMP : Teint et musculature",
	L"IMP : Couleurs",
	L"Choisissez les couleurs respectives de votre peau, vos cheveux et vos habits. Ainsi que votre physionomie (traits physiques).",
	L"Choisissez les couleurs respectives de votre peau, vos cheveux et vos habits.",
	L"Cocher ici pour utiliser une prise en main alternative du fusil.",
	L"\n(Attention : vous devez avoir une grande force pour l'utiliser.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Cheveux",
	L"Teint",
	L"T-shirt",
	L"Pantalon",
	L"Corps normal",
	L"Corps musclé",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Pas d'handicap",
	L"Déteste la chaleur",
	L"Nerveux",
	L"Claustrophobe",
	L"Mauvais nageur",
	L"Peur des insectes",
	L"Distrait",
	L"Psychotique",
	L"Mauvaise audition",
	L"Mauvaise vue",
	L"Hemophiliac",		// TODO.Translate
	L"Fear of Heights",
	L"Self-Harming",
	L"IMP : Handicaps",
};

STR16 gzIMPDisabilityTraitEmailTextDeaf[] =
{
	L"Nous gageons que vous êtes heureux que ceci ne soit pas un message audio.",
	L"Vous avez peut-être fréquenté trop de discothèques dans votre jeunesse ou vous avez été pris sous un bombardement... Ou vous êtes tout simplement vieux. Dans tous les cas, votre équipe ferait bien d'apprendre le langage des signes.",
};

STR16 gzIMPDisabilityTraitEmailTextShortSighted[] =	
{
	L"Vous êtes foutu si vous perdez vos lunettes.",
	L"C'est ce qui arrive lorsque l'on passe ses journées devant un écran. Vous auriez dû manger plus de carottes. Avez-vous déjà vu un lapin à lunettes ? Improbable, hein ?",
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
	L"%s n'a pas assez de force pour accomplir cette tâche.",
	L"%s n'a pas assez de dextérité pour accomplir cette tâche.",
	L"%s n'est pas assez agile pour accomplir cette tâche.",
	L"%s n'est pas assez en bonne santé pour accomplir cette tâche.",
	L"%s n'a pas assez de sagesse pour accomplir cette tâche.",
	L"%s n'est pas assez bon tireur pour accomplir cette tâche.",
	// 6 - 10
	L"%s n'est pas assez bon médecin pour accomplir cette tâche.",
	L"%s n'est pas assez bon en mécanique pour accomplir cette tâche.",
	L"%s n'est pas assez bon en commandement pour accomplir cette tâche.",
	L"%s n'est pas assez bon en explosif pour accomplir cette tâche.",
	L"%s n'a pas assez d'expérience pour accomplir cette tâche.",
	// 11 - 15
	L"%s n'a pas assez de moral pour accomplir cette tâche.",
	L"%s est trop épuisé pour effectuer cette tâche.",
	L"Loyauté insuffisante à %s. Les habitants refusent de vous permettre de faire cette tâche.",
	L"Il n'y a plus d'affectation possible pour : %s.",
	L"Il n'y a plus d'affectation possible pour : %s.",
	// 16 - 20
	L"%s n'a pas trouvé d'objets à réparer.",
	L"%s a perdu %s, alors qu'il travaillait dans le secteur %s !",
	L"%s a perdu %s, alors qu'il travaillait sur %s à %s !",
	L"%s a été blessé, alors qu'il travaillait dans le secteur %s et nécessite des soins médicaux immédiats !",
	L"%s a été blessé, alors qu'il travaillait %s à %s et nécessite des soins médicaux immédiats !",
	// 21 - 25
	L"%s a été blessé, alors qu'il travaillait dans le secteur %s. Il ne semble pas être en trop mauvais état.",
	L"%s a été blessé, alors qu'il travaillait sur %s à %s. Il ne semble pas être en trop mauvais état.",
	L"Les résidents de %s semblent être excédés par la présence de %s.",
	L"Les résidents de %s semblent être excédés par le travail de %s sur %s.",
	L"Les actions de %s dans le secteur %s ont causé une perte de loyauté à travers la région !",
	// 26 - 30
	L"Les actions de %s sur %s à %s ont causé une perte de loyauté à travers la région !",
	L"%s est ivre.", // <--- This is a log message string.
	L"%s est devenu gravement malade dans le secteur %s et commence à manquer à son devoir.",
	L"%s est devenu gravement malade et ne peut continuer son travail sur %s à %s.",
	L"%s a été blessé dans le secteur %s.", // <--- This is a log message string.
	// 31 - 35
	L"%s a été gravement blessé dans le secteur %s.", //<--- This is a log message string.
	L"Il y a actuellement des prisonniers qui ont connaissance de l'identité de : %s",
	L"%s est actuellement trop connu(e) comme indic. Attendez au moins %d heures.",


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Force",
	L"Agilité",
	L"Dextérité",
	L"Sagesse",
	L"Santé",
	L"Tir",
	// 5-10
	L"Commandant",
	L"Mécanique",
	L"Médecin",
	L"Explosif",
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"AMBIENT",
	L"Collecter renseignements",
	L"Manger",
	L"Repos",
	L"Réparer les objets",
	L"Réparer %s", // Vehicle name inserted here
	L"Réparer le robot",
	// 6-10
	L"Docteur",
	L"Patient",
	L"Apprendre Force",
	L"Apprendre Dextérité",
	L"Apprendre Agilité",
	L"Apprendre Santé",
	// 11-15
	L"Apprendre Tir",
	L"Apprendre Médecin",
	L"Apprendre Mécanique",
	L"Apprendre Commandement",
	L"Apprendre Explosif",
	// 16-20
	L"Élève Force",
	L"Élève Dextérité",
	L"Élève Agilité",
	L"Élève Santé",
	L"Élève Tir",
	// 21-25
	L"Élève Médecin",
	L"Élève Mécanique",
	L"Élève Commandement",
	L"Élève Explosif",
	L"Entraîneur Force",
	// 26-30
	L"Entraîneur Dextérité",
	L"Entraîneur Agilité",
	L"Entraîneur Santé",
	L"Entraîneur Tir",
	L"Entraîneur Médecin",
	// 30-35
	L"Entraîneur Mécanique",
	L"Entraîneur Commandement",
	L"Entraîneur Explosif",
	L"Interroger prisonnier",	// added by Flugente
	L"Infiltré",
	// 36-40
	L"Répand une propagande",
	L"Fait de la propagande",	// spread propaganda (globally)
	L"Collecte les rumeurs",
	L"Dirige la Milice",	// militia movement orders
};
STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 mode fenêtré exige une profondeur de couleur de 16 bit.",
	L"Jagged Alliance 2 v1.13 fullscreen mode (%d x %d) is not supported by your primary screen.\nPlease either change the game resolution or use 16bpp windowed mode.",	// TODO.Translate
	L"Erreur interne en lisant %s emplacements depuis la sauvegarde : Le nombre d'emplacements dans la sauvegarde (%d) diffère des emplacements définis dans les paramètres de ja2_options.ini (%d)",
	// WANNE: Savegame slots validation against INI file
	L"Mercenaires (MAX_NUMBER_PLAYER_MERCS) / Véhicule (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Ennemis (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Créatures (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Milices (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civils (MAX_NUMBER_CIVS_IN_TACTICAL)",
};

// SANDRO - Taunts (here for now, xml for future, I hope)
STR16 sEnemyTauntsFireGun[]=
{
	L"Suce-moi ça !",
	L"Dis bonjour à ma pétoire !",
	L"Viens par là !",
	L"T'es à moi !",
	L"Meurs !",
	L"T'as peur enfoiré ?",
	L"Ça va faire mal !",
	L"Viens-là bâtard !",
	L"Allez viens ! Je n'ai pas toute la vie !",
	L"Viens voir papa !",
	L"Je vais t'envoyer à six pieds sous terre dans peu de temps !",
	L"Retourne chez ta mère, looserr !",
	L"Hé, tu veux jouer ?",
	L"T'aurais dû rester chez toi, salope !",
	L"Enculé(e) !",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"C'est l'heure du barbecue !",
	L"J'ai un cadeau pour toi !",
	L"Paf !",
	L"Souris, l'oiseau va sortir !",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Attrape !",
	L"Et c'est parti !",
	L"Prends-toi ça !",
	L"Et un pour toi !",
	L"Mouhahaha !",
	L"Attrape ça sale porc !",
	L"Ça va faire mal !",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"J'vais te trépaner !",
	L"Viens voir papa !",
	L"Montre-moi tes couilles !",
	L"Je vais te découper en rondelles !",
	L"Hannibal Lecter, ppfft tu ne me connais pas !",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"On est vraiment dans une grosse merde...",
	L"Ils disent de rejoindre l'armée. Mais pas pour cette merde !",
	L"J'en ai plein le cul !",
	L"Oh mon Dieu !",
	L"On n'est pas assez payé pour ce foutoir !",
	L"C'est vraiment trop pour moi.",
	L"Je vais chercher quelques potes !",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"T'as entendu !",
	L"Qui est là ?",
	L"Qu'est-ce que c'est ?",
	L"Eh ! C'est quoi ce bordel ?",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Ils sont là !",
	L"Yeah, la partie peut commencer !",
	L"J'espérais que ça n'arriverait jamais...",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Aaaïe !",
	L"Pouah !",
	L"Ça... Ça fait mal !",
	L"Enfoiré !",
	L"Tu vas regret... uhh... ter ça.",
	L"C'est quoi ce bordel ?",
	L"Maintenant vous... m'avez énervé(e) !",

};


STR16 sEnemyTauntsNoticedMerc[]=
{
	L"Da'ffff... !",
	L"Oh mon Dieu !",
	L"Oh putain !",
	L"Ennemi !!!",
	L"Alerte ! Alerte !",
	L"Ils sont là !",
	L"Attaque !",

};

//////////////////////////////////////////////////////
// HEADROCK HAM 4: Begin new UDB texts and tooltips
//////////////////////////////////////////////////////
STR16 gzItemDescTabButtonText[] =
{
	L"Description",
	L"Général",
	L"Avancés",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Desc",
	L"Gén",
	L"Ava",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Primaire",
	L"Secondaire",
	L"Coût PA",
	L"Rafale/auto",
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
	L"|P|a|g|e |d|e |d|e|s|c|r|i|p|t|i|o|n :\n \nMontre les informations textuelles de base sur cet objet.",
	L"|P|r|o|p|r|i|é|t|é|s |g|é|n|é|r|a|l|e|s :\n \nMontre les données spécifiques de cet objet.\n \nArmes : Cliquez à nouveau pour voir la deuxième page.",
	L"|P|r|o|p|r|i|é|t|é|s |a|v|a|n|c|é|e|s :\n \nMontre les bonus donnés par cet objet.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|P|r|o|p|r|i|é|t|é|s |p|r|i|m|a|i|r|e|s :\n \nPropriétés et données liées à la classe de cet objet\n(Armes/Armures/ etc.).",
	L"|P|r|o|p|r|i|é|t|é|s |s|e|c|o|n|d|a|i|r|e|s :\n \nLes caractéristiques supplémentaires de cet objet,\net/ou capacités secondaires possibles.",
	L"|C|o|û|t |e|n |P|A :\n \nCoût en PA pour tirer ou manipuler cette arme.",
	L"|R|a|f|a|l|e|/|A|u|t|o|m|a|t|i|q|u|e :\n \nDonnées liées au tir de cette arme en mode rafale ou auto.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|P|r|o|p|r|i|é|t|é |i|c|ô|n|e\n \nSurvol avec la souris pour révéler le nom de la propriété.",
	L"|V|a|l|e|u|r |b|a|s|i|q|u|e\n \nValeurs de base données par cet objet, excluant\nles bonus ou pénalités liés aux accessoires ou munitions.",
	L"|B|o|n|u|s |d|e|s |a|c|c|e|s|s|o|i|r|e|s\n \nBonus ou pénalités donnés par les munitions ou accessoires.",
	L"|V|a|l|e|u|r |f|i|n|a|l|e\n \nValeur finale donnée par cette objet, incluant les\nbonus/pénalités des accessoires et munitions.",
};

STR16 gzUDBAdvIndexTooltipText[]=
{
	L"Propriété icône (survoler avec la souris pour voir le nom).",
	L"Bonus/pénalité donné |d|e|b|o|u|t.",
	L"Bonus/pénalité donné |a|c|c|r|o|u|p|i.",
	L"Bonus/pénalité donné |c|o|u|c|h|é.",
	L"Bonus/pénalité donné",
};

STR16 szUDBGenWeaponsStatsTooltipText[]=
{
	L"|P|r|é|c|i|s|i|o|n",
	L"|D|é|g|â|t",
	L"|P|o|r|t|é|e",
	L"|D|i|f|f|i|c|u|l|t|é |d|e |p|r|i|s|e |e|n |m|a|i|n",
	L"|N|i|v|e|a|u |d|e |v|i|s|é|e",
	L"|G|r|o|s|s|i|s|s|e|m|e|n|t |d|e |l|a |l|u|n|e|t|t|e",
	L"|F|a|c|t|e|u|r |d|e |p|r|o|j|e|c|t|i|o|n",
	L"|C|a|c|h|e|-|f|l|a|m|m|e",
	L"|I|n|t|e|n|s|i|t|é",
	L"|F|i|a|b|i|l|i|t|é",
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n",
	L"|P|o|r|t|é|e |m|i|n|i|m|u|m |p|o|u|r |b|o|n|u|s |d|e |v|i|s|é|e",
	L"|F|a|c|t|e|u|r |d|e |d|é|g|â|t|s",	
	L"|N|o|m|b|r|e |d|e |P|A |p|o|u|r |m|e|t|t|r|e |e|n |j|o|u|e",
	L"|N|o|m|b|r|e |d|e |P|A |p|o|u|r |t|i|r|e|r",
	L"|N|o|m|b|r|e |d|e |P|A |t|i|r |e|n |r|a|f|a|l|e",
	L"|N|o|m|b|r|e |d|e |P|A |t|i|r |e|n |a|u|t|o|m|a|t|i|q|u|e",
	L"|N|o|m|b|r|e |d|e |P|A |p|o|u|r |r|e|c|h|a|r|g|e|r",
	L"|N|o|m|b|r|e |d|e |P|A |p|o|u|r |r|e|c|h|a|r|g|e|r |m|a|n|u|e|l|l|e|m|e|n|t",
	L"",	// No longer used!
	L"|R|e|c|u|l| |t|o|t|a|l",
	L"|T|i|r |a|u|t|o|m|a|t|i|q|u|e |p|o|u|r |5 |P|A",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nDétermine si des balles tirées par cette arme dévieront\nloin de l'impact d'origine.\n \nÉchelle : 0-100.\nValeur élevée recommandée.",
	L"\n \nDétermine la quantité moyenne de dégâts faits par\ndes balles tirées de cette arme, avant\n de tenir compte de l'armure et de la pénétration d'armure.\n \nValeur élevée recommandée.",
	L"\n \nDistance maximale (en cases) que vont parcourir les balles\ntirées par cette arme avant de redescendre vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nDétermine la difficulté pour tenir cette arme au tir. Une difficulté de manipulation élevée réduit les chances de toucher en visant, mais particulièrement sans viser. Valeur faible recommandée.",
	L"\n \nCeci est le nombre de niveau de visée supplémentaire que\nvous pouvez ajouter en visant avec cette arme.\n \nRéduire le nombre de niveau de visée signifie que chaque\nniveau ajoute proportionnellement plus de précision au tir.\nPar conséquent, avoir peu de niveaux de visée vous\npermettra de garder une bonne précision avec une vitesse\nde mise en joue élevée !\n \nValeur faible recommandée.",
	L"\n \nUne valeur plus grande de *1.0, réduit proportionnellement\nles erreurs de visée à distance.\n \nN'oubliez pas qu'un trop gros zoom sur une cible\nproche vous pénalisera !\n \nLa valeur de 1.0 signifie qu'aucune lunette est installée.",
	L"\n \nRéduit proportionnellement les erreurs de visée à distance.\n \nCes effets ne sont valables qu'à une distance donnée,\net se dissipent ou disparaissent\nà une longue distance.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette propriété est en vigueur, l'arme\nne produit pas d'éclair lors du tir.\n \nLes ennemis ne seront plus en mesure de vous repérer\nà cause de la flamme (mais ils\npourront toujours vous entendre !).",
	L"\n \nDistance (en cases) de l'intensité sonore que fait votre arme\nlorsque vous tirez avec.\n \nLes ennemis placés en deçà de cette distance entendront\nvotre tir.\n \nValeur faible recommandée.",
	L"\n \nDétermine la vitesse de déterioration de cette arme\nà l'usage.\n \nValeur élevée recommandée.",
	L"\n \nDétermine la difficulté de réparation de cette arme.\n \nValeur élevée recommandée.",
	L"\n \nPortée minimum où la lunette de visée fournit un bonus de visée.",
	L"\n \nFacteur de chance de toucher accordé par un laser.",	
	L"\n \nLe nombre de PA requis pour mettre en joue.\n \nQuand cette arme est prête, vous pouvez tirez plusieurs fois\nsans avoir de coût supplémentaire.\n \nAnnule automatiquement cette opération, si vous faîtes des\nactions autre que pivoter ou tirer.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour effectuer\nune attaque simple avec cette arme.\n \nPour les fusils, c'est le coût pour un tir\nsimple sans niveau de visée.\n \nSi cette icône est grisée, les tirs simples\nne sont pas possible.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour tirer une Rafale.\n \nle nombre de balles tirées pour chaque rafale est\ndéterminé par l'arme elle-même, et indiqué\npar le nombre de balles sur cette icône.\n \nSi cette icône est grisée, le mode rafale\nn'est pas possible avec cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour tirer en Automatique.\n \nSi vous voulez tirez plus de balles,\ncela coûtera plus de PA.\n \nSi cette icône est grisée, le mode Auto\nn'est pas possible avec cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour recharger cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour recharger cette arme\nentre chaque tir.\n \nValeur faible recommandée.",
	L"",	// No longer used!
	L"\n \nLa distance totale de la bouche de cette arme qui se\ndéplacera entre chaque balle en rafale ou en\nautomatique, si aucune force inverse n'est appliquée.\n \nValeur faible recommandée.", // HEADROCK HAM 5: Altered to reflect unified number.
	L"\n \nIndique le nombre de balles qui seront ajoutées\nau mode auto tous les 5 PA que vous dépensez.\n \nValeur élevée recommandée.",
	L"\n \nDétermine la difficulté à réparer une arme\net qui peut la réparer complètement.\n \nVert = N'importe qui peut la réparer.\n \nJaune = Seuls des PNJ spécialisés peuvent la\nréparer au-delà du seuil de réparation.\n \nRouge = L'arme ne peut pas être réparée.\n \nValeur élevée recommandée.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|V|a|l|e|u|r |d|e |p|r|o|t|e|c|t|i|o|n",
	L"|C|o|u|v|e|r|t|u|r|e",
	L"|T|a|u|x |d|e |d|é|g|r|a|d|a|t|i|o|n",
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nCette propriété d'armure définit de combien elle\nabsorbe les dégâts de chaque attaque.\n \nN'oubliez pas que les attaques perforantes et\ndivers facteurs aléatoires peuvent altérer\nla réduction final des dégâts.\n \nValeur élevée recommandée.",
	L"\n \nDétermine la protection de l'armure\nsur votre corps.\n \nSi la protection est inférieure à 100%, les attaques\nont une certaine chance de passer à travers l'armure\nen causant un maximum de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nIndique à quelle vitesse les conditions de l'armure\nvont chuter et qui est proportionnelle aux\ndégâts subis.\n \nValeur faible recommandée.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|T|a|u|x |d|e |p|é|n|é|t|r|a|t|i|o|n",
	L"|É|c|r|a|s|e|m|e|n|t |d|e |l|a |b|a|l|l|e",
	L"|E|x|p|l|o|s|i|o|n |a|v|a|n|t |i|m|p|a|c|t",
	L"|T|e|m|p|é|r|a|t|u|r|e |d|u |t|i|r",
	L"|T|a|u|x |d|'|e|m|p|o|i|s|o|n|n|e|m|e|n|t",
	L"|E|n|c|r|a|s|s|e|m|e|n|t",
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nCeci est la capacité de la balle à pénétrer\nl'armure de la cible.\n \nAvec une valeur supérieure à 1.0, la balle réduiera fortement\nla valeur de protection de l'armure touchée.\n \nValeur élevée recommandée.",
	L"\n \nDétermine le potentiel de la balle à faire des dégâts\nsur le corps après avoir traversée l'armure.\n \nAvec une valeur supérieure à 1.0, la balle fera de lourds dégâts\naprès pénétration\nAvec une valeur inférieure à 1.0, la balle fera des dégâts moindre\naprès pénétration.\n \nValeur élevée recommandée.",
	L"\n \nMultiplicateur de potentiel de dégâts juste avant\nl'impact de la balle.\n \nAvec une valeur supérieure à 1.0, la balle fera de lourds dégâts.\nUne valeur inférieure à 1.0 fera des dégâts moindre.\n \nValeur élevée recommandée.",
	L"\n \nTempérature additionnelle générée par ces munitions.\n \nValeur faible recommandée",
	L"\n \nDétermine le pourcentage de dégâts d'une balle empoisonnée.\n \nValeur élevée recommandée.",
	L"\n \nEncrassement additionnel généré par ces munitions.\n \nValeur faible recommandée",
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|D|é|g|â|t|s",
	L"|D|é|g|â|t|s |é|t|o|u|r|d|i|s|s|a|n|t",
	L"|E|x|p|l|o|s|i|o|n |à |l|'|i|m|p|a|c|t",		// HEADROCK HAM 5
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n",
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n |é|t|o|u|r|d|i|s|a|n|t|e",
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n |s|o|n|o|r|e",
	L"|D|é|b|u|t |r|a|y|o|n |g|a|z |l|a|c|r|y|m|o|g|è|n|e",
	L"|D|é|b|u|t |r|a|y|o|n |g|a|z |m|o|u|t|a|r|d|e",
	L"|D|é|b|u|t |r|a|y|o|n |f|l|a|s|h |l|u|m|i|n|e|u|x",
	L"|D|é|b|u|t |r|a|y|o|n |f|u|m|é|e",
	L"|D|é|b|u|t |r|a|y|o|n |i|n|c|e|n|d|i|e",
	L"|F|i|n |r|a|y|o|n |g|a|z |l|a|c|r|y|m|o|g|è|n|e",
	L"|F|i|n |r|a|y|o|n |g|a|z |m|o|u|t|a|r|d|e",
	L"|F|i|n |r|a|y|o|n |f|l|a|s|h |l|u|m|i|n|e|u|x",
	L"|F|i|n |r|a|y|o|n |f|u|m|é|e",
	L"|F|i|n |r|a|y|o|n |i|n|c|e|n|d|i|e",
	L"|D|u|r|é|e |d|e |l|'|e|f|f|e|t",
	// HEADROCK HAM 5: Fragmentation
	L"|N|o|m|b|r|e |d|e |s|h|r|a|p|n|e|l|s",
	L"|D|é|g|â|t|s |p|a|r |s|h|r|a|p|n|e|l",
	L"|P|o|r|t|é|e |d|e|s |s|h|r|a|p|n|e|l|s",
	// HEADROCK HAM 5: End Fragmentations
	L"|I|n|t|e|n|s|i|t|é |s|o|n|o|r|e",
	L"|V|o|l|a|t|i|l|i|t|é",
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nLa quantité de dégâts causés par cet explosif.\n \nNotez que les explosifs de type \"explosion\" livrent des dégâts\nseulement une fois (quand ils explosent), tandis que les\nexplosifs à effets prolongés livrent cette quantité de dégâts\nà chaque tour jusqu'à ce que l'effet se dissipe.\n \nValeur élevée recommandée.",
	L"\n \nLa quantité de dégâts non mortels (étourdissant) causés\npar cet explosif.\n \nNotez que les explosifs de type \"explosion\" livrent des\ndégâts seulement une fois (quand ils explosent), tandis\nque les explosifs à effets prolongés livrent cette\nquantité de dégâts d'étourdissement à chaque tour jusqu'à\nce que l'effet se dissipe.\n \nValeur élevée recommandée.",
	L"\n \nCet explosif ne rebondit pas. Il explose dès qu'il touche un obstacle.",	// HEADROCK HAM 5
	L"\n \nRayon de l'explosion causé par cet objet.\n \nPlus les ennemis seront loin du centre de l'explosion\nmoins ils subiront de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nRayon de l'onde de choc causé par cet objet.\n \nPlus les ennemis seront loin du centre de l'explosion\nmoins ils subiront de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nDistance du parcours du bruit causé par ce piège.\n Les ennemis placés en deçà de cette distance entendront\n votre piège et sonneront l'alerte.\n \nValeur faible recommandée.",
	L"\n \nRayon de départ libéré par la lacrymogène.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nÀ noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ libéré par le gaz moutarde.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nÀ noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ émis par le flash lumineux.\n \nLes cases autours du centre de l'effet deviendront très\nlumineuses, quand celles autours ne seront que\nlégèrement plus lumineuse que la normale.\n \nÀ noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nÀ noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ libéré par la fumée.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz. plus important,\nquiconque se trouvant à l'intérieur de la fumée aura des difficultés à se repérer,\net perdra aussi sa visibilité.\n \nÀ noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ causés par les flammes.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour.\n \nÀ noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par la lacrymogène avant\nqu'il ne se dissipe.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nÀ noter également le début du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par le gaz moutarde avant\nqu'il ne se dissipe.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nÀ noter également le début du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin émis par le flash lumineux.\n \nLes cases autours du centre de l'effet deviendront très\nlumineuses, quand celles autours ne seront que\nlégèrement plus lumineuse que la normale.\n \nÀ noter également la fin du rayon et la durée de\nl'effet.\n \nÀ noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par la fumée.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz. plus important,\nquiconque se trouvant à l'intérieur de la fumée aura des difficultés à se repérer,\net perdra aussi sa visibilité.\n \nÀ noter également la fin du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ causés par les flammes.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour.\n \nÀ noter également la fin du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nDurée des effets de l'explosion.\n \nChaque tour, le rayon s'aggrandit d'une case dans\n toutes les directions, avant d'atteindre\nla valeur de fin de rayon indiquée.\n \nQuand la durée a été atteinte, les effets se\ndissipent complètement.\n \nÀ noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	// HEADROCK HAM 5: Fragmentation
	L"\n \nC'est le nombre de shrapnels qui seront\néjectés lors de l'explosion.\n \nLes shrapnels sont comme des balles et\ntoucheront toutes les personnes qui\nsont assez proche de l'explosion.\n \nValeur élevée recommandée.",
	L"\n \nLe potentiel des dégâts causés par chaque shrapnel.\n \nValeur élevée recommandée.",
	L"\n \nC'est la portée moyenne des shrapnels. Ils peuvent\ncouvrir plus ou moins loin de cette distance.\n \nValeur élevée recommandée.",
	// HEADROCK HAM 5: End Fragmentations
	L"\n \nDistance (en cases) en deçà de laquelle chaque\nsoldat et mercenaire peuvent entendre l'explosion.\n \nLes ennemis qui entendent cette explosion peuvent alerter de\nvotre présence.\n \nValeur faible recommandée.",
	L"\n \nCette valeur représente la chance (sur 100) que cette\nexplosif explose spontanément chaque fois qu'il est endommagé\n(Par exemple, quand il y a d'autres explosions à proximité).\n \nTransporter des explosifs hautement volatiles en combat\nest donc extrêmement risqué et devrait être évitée.\n \nÉchelle : 0-100.\nValeur faible recommandée.",
	L"\n \nDétermine la difficulté à réparer complètement un explosif.\n \nVert = N'importe qui peut le réparer.\n \nRouge = Il ne peut pas être réparé.\n \nValeur élevée recommandée.",
};

STR16 szUDBGenCommonStatsTooltipText[]=
{
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n",
	L"|A|v|a|i|l|a|b|l|e |V|o|l|u|m|e",	// TODO.Translate
	L"|V|o|l|u|m|e",	// TODO.Translate
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nDétermine la difficulté à réparer complètement un objet.\n \nVert = N'importe qui peut le réparer.\n \nRouge = Il ne peut pas être réparé.\n \nValeur élevée recommandée.",
	L"\n \nDetermines how much space is available on this MOLLE carrier.\n \nHigher is better.",	// TODO.Translate
	L"\n \nDetermines how much space this MOLLE pocket will occupy.\n \nLower is better.",	// TODO.Translate
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|B|a|l|l|e|s |t|r|a|ç|a|n|t|e|s",
	L"|M|u|n|i|t|i|o|n|s |a|n|t|i|-|c|h|a|r",
	L"|I|g|n|o|r|e |l|'|a|r|m|u|r|e",
	L"|M|u|n|i|t|i|o|n|s |a|c|i|d|e|s",
	L"|M|u|n|i|t|i|o|n|s |c|a|s|s|a|n|t |s|e|r|r|u|r|e",
	L"|R|é|s|i|s|t|a|n|t |a|u|x |e|x|p|l|o|s|i|f|s",
	L"|É|t|a|n|c|h|é|i|t|é",
	L"|É|l|e|c|t|r|o|n|i|q|u|e",
	L"|M|a|s|q|u|e |à |g|a|z",
	L"|B|e|s|o|i|n |d|e |p|i|l|e|s",
	L"|P|e|u|t |c|r|o|c|h|e|t|e|r |l|e|s |s|e|r|r|u|r|e|s",
	L"|P|e|u|t |c|o|u|p|e|r |d|e|s |f|i|l|s",
	L"|P|e|u|t |c|a|s|s|e|r |l|e|s |v|e|r|r|o|u|s",
	L"|D|é|t|e|c|t|e|u|r |d|e |m|é|t|a|l",
	L"|D|é|c|l|e|n|c|h|e|u|r |à |d|i|s|t|a|n|c|e",
	L"|D|é|t|o|n|a|t|e|u|r |à |d|i|s|t|a|n|c|e",
	L"|M|i|n|u|t|e|r|i|e |d|e| |d|é|t|o|n|a|t|e|u|r",
	L"|C|o|n|t|i|e|n|t |d|e |l|'|e|s|s|e|n|c|e",
	L"|C|a|i|s|s|e |à |o|u|t|i|l|s",
	L"|O|p|t|i|q|u|e|s |t|h|e|r|m|i|q|u|e|s",
	L"|D|i|s|p|o|s|i|t|i|f |à |r|a|y|o|n|s |X",
	L"|C|o|n|t|i|e|n|t |d|e |l|'|e|a|u |p|o|t|a|b|l|e",
	L"|C|o|n|t|i|e|n|t |d|e |l|'|a|l|c|o|o|l",
	L"|T|r|o|u|s|s|e |d|e |1|e|r |s|o|i|n|s",
	L"|T|r|o|u|s|s|e |m|é|d|i|c|a|l|e",
	L"|B|o|m|b|e |p|o|u|r |s|e|r|r|u|r|e |d|e |p|o|r|t|e",
	L"|B|o|i|s|s|o|n",
	L"|R|e|p|a|s",
	L"|B|a|n|d|e|s |d|e |m|u|n|i|t|i|o|n|s",
	L"|H|a|r|n|a|i|s |à |m|u|n|i|t|i|o|n|s",
	L"|K|i|t |d|e |d|é|s|a|m|o|r|ç|a|g|e",
	L"|O|b|j|e|t |d|i|s|s|i|m|u|l|a|b|l|e",
	L"|N|e |p|e|u|t |ê|t|r|e |e|n|d|o|m|m|a|g|é",
	L"|F|a|i|t |e|n |m|é|t|a|l",
	L"|N|e |f|l|o|t|t|e |p|a|s",
	L"|À |d|e|u|x |m|a|i|n|s",
	L"|B|l|o|q|u|e |l|e |v|i|s|e|u|r",
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
	L"\n \nCes munitions, en mode Auto ou rafale, ont la propriété d'être\ndes des balles traçantes.\n \nLa lumière qu'apporte les balles traçantes lors d'une rafale\npermet d'avoir une meilleur précision et d'être ainsi plus\nmortel malgré le recul de l'arme.\n \nDe plus, ces balles créent un halo lumineux permettant de\nrévéler l'ennemi pendant la nuit. Cependant, elles révèlent\naussi la position du tireur à l'ennemi !\n \nLes balles traçantes désactive automatiquement le\ncache-flamme installé sur l'arme utilisé.",
	L"\n \nCes munitions peuvent faire des dégâts aux chars.\n \nLes munitions SANS cette propriété ne feront aucun dégât quel que\nsoit le char.\n \nMême avec cette propriété, n'oubliez pas que la plupart des armes\nne feront que peu de dégâts, donc n'en abusez pas.",
	L"\n \nCes munitions ignorent complètement l'armure.\n \nQuand vous tirez sur un ennemi avec une armure, cela sera comme s'il\nn'en avait pas, permettant ainsi de faire un maximum de dégâts !",
	L"\n \nLorsque cette munition frappe une cible avec une armure,\ncette dernière se dégradera très rapidement.\n \nCeci peut potentiellement retirer l'armure de la cible !",
	L"\n \nCette munition est exceptionnelle pour casser les serrures.\n \nTirez directement sur la serrure de la porte ou du\ncoffre pour faire de lourds dégâts sur le mécanisme.",
	L"\n \nCette armure est trois fois plus résistante contre les\nexplosifs que sa valeur indiquée.\n \nQuand une explosion heurte cette armure, la valeur de\nsa protection est considérée comme trois fois plus\nélevée que celle indiquée.",
	L"\n \nCet objet est imperméable à l'eau. Il ne recevra pas de\ndégâts causés par l'eau.\n \nLes objets SANS cette propriété vont progressivement se\ndétériorer, si la personne nage avec.",
	L"\n \nCet objet est de nature électronique et contient des\ncircuits complexes.\n \nLes objets électroniques sont intrinsèquement plus\ndifficiles à réparer, d'autant plus si vous n'avez pas\nles compétences nécessaires.",
	L"\n \nLorsque cet objet est porté sur le visage, il le protègera\nde tous les gaz nocifs.\n \nNotez que certains gaz sont corrosifs et pourrait bien\npénétrer à travers le masque...",
	L"\n \nCet objet requière des piles. Sans les piles, vous ne pouvez pas\nactiver ces principales caractéristiques.\n \nPour utiliser un jeu de piles, attachez-les à cette objet\ncomme si vous m'étiez une lunette de visée à votre arme.",
	L"\n \nCet objet peut être utilisé pour crocheter des portes\nou des containers verrouillés.\n \nLe crochetage est silencieux, mais nécessite des\ncompétences en mécanique. Il améliore\nla chance au crochetage de ", //JMich_SkillsModifiers: needs to be followed by a number",
	L"\n \nCet objet peut être utilisé pour couper les clôtures de fil.\n \nCela autorise le mercenaire a passé à travers des zones\nsécurisées, pour éventuellement surprendre l'ennemi !",
	L"\n \nCet objet peut être utilisé pour forcer des portes\nou des coffres verrouillés.\n \nForcer des serrures, requière une grande force,\ngénère beaucoup de bruits et peut facilement\nvous épuisez. Cependant, c'est une bonne façon\nd'ouvrir une serrure sans avoir des talents en\nmécanique ou des outils adéquates. Il améliore\nvotre chance de ", //JMich_SkillsModifiers: needs to be followed by a number
	L"\n \nCet objet peut être utilisé pour détecter des objets métalliques\nenfouis sous terre.\n \nNaturellement, sa fonction première est de détecter les mines\nsans que vous ayez les compétences nécessaires pour les repérer\nà l'œil nu.\n \nPeut-être trouverez-vous certains trésors cachés...",
	L"\n \nCet objet peut être utilisé pour faire exploser une bombe\nqui aura été amorcée par un détonateur.\n \nPlantez la bombe en 1er, puis utilisez votre déclencheur\nà distance pour l'activer quand c'est le bon moment.",
	L"\n \nQuand il est attaché à un dispositif explosif et positionné\ndans le bon sens, ce détonateur peut être déclenché par un\ndétonateur (séparé) à distance.\n \nLes détonateurs à distance sont excellents pour faire des\npièges, car ils se déclenchent quand vous le souhaitez.\n \nDe plus, vous avez tout le temps pour déguerpir !",
	L"\n \nQuand il est attaché à un dispositif explosif et positionné\ndans le bon sens, ce détonateur va lancer un compte à\nrebours défini et explosera quand le temps sera écoulé.\n \nCes détonateurs avec minuterie sont pas chers et faciles à\ninstaller, mais vous aurez besoin de temps pour pouvoir\ndéguerpir de là !",
	L"\n \nCet objet contient de l'essence.\n \nIl pourrait arriver à point nommé, si vous aviez besoin\nde remplir un réservoir d'essence...",
	L"\n \nCet objet contient divers outils qui peuvent être utilisés\npour réparer d'autres objets.\n \nUne caisse à outils est toujours nécessaire lorsque vous\nêtes en mission de réparation. Il améliore\nl'efficacité en réparation de ", //JMich_SkillsModifiers: need to be followed by a number
	L"\n \nQuand équipé sur le visage, cet objet donne la capacité de\nrepérer les ennemis à travers les murs, grâce à leur\nsignature thermique.",
	L"\n \nCe merveilleux dispositif peut être utilisé pour repérer\nles ennemis en utilisant les rayons X.\n \nCela révélera tous les ennemis à une certaine distance\npour une courte période de temps.\n \nGardez cela loin de vos organes reproductifs !",
	L"\n \nCet objet contient de l'eau potable bien fraiche.\nÀ boire lorsque vous êtes assoiffé.",
	L"\n \nCet objet contient du digestif, gnôle, eau-de-vie, liqueur,\nqu'importe comment vous appelez cela.\n \nÀ prendre avec modération. Boire ou conduire !\nPeut causer une cirrhose du foie.",
	L"\n \nIl s'agit d'un kit médical basic, contenant les ustensiles\nrequis pour faire une intervention médicale basic.\n \nIl peut être utilisé pour soigner un mercenaire blessé et\nempêcher le saignement.\n \nPour une guérison optimale, utilisez une véritable trousse\nde soins et/ou beaucoup de repos.",
	L"\n \nIl s'agit d'un kit médical complet, qui peut être utilisé\npour une opération chirurgicale ou autre cas sérieux.\n \nUne trousse de soins est toujours nécessaire lorsque vous\nêtes en mission de docteur.",
	L"\n \nCet objet peut être utilisé pour faire sauter les portes\nou coffres verrouillés.\n \nDes compétences en explosion sont nécessaires pour éviter\nune explosion prématurée.\n \nExploser les serrures, est relativement facile et rapide\nà faire. Cependant, c'est très bruyant et dangereux pour\nla plupart des mercenaires.",
	L"\n \nCette boisson étanchera votre soif\nsi vous la buvez.",
	L"\n \nCeci vous nourrira\nsi vous l'ingurgitez.",
	L"\n \nVous alimenterez une mitrailleuse\navec ces bandes de munitions.",
	L"\n \nVous nourrirez une mitrailleuse\navec ces munitions stockées dans\nce harnais.",
	L"\n \nCet objet améliore votre chance de désamorçage de ",
	L"\n \nCet objet est dissimulable,\ny compris ses accessoires.",
	L"\n \nCet objet ne peut pas être endommagé.",
	L"\n \nCet objet est en métal.\nIl prend moins de dégâts que les autres.",
	L"\n \nCet objet coule dans l'eau.",
	L"\n \nCet objet exige les deux mains pour être utilisé.",
	L"\n \nCet Objet bloquera votre viseur\nde ce fait vous ne pouvez pas l'utiliser.",
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
	L"|F|a|c|t|e|u|r |d|e |p|r|é|c|i|s|i|o|n",
	L"|F|a|c|t|e|u|r |n|e|t |d|e |p|r|é|c|i|s|i|o|n |s|u|r |t|o|u|t |t|i|r",
	L"|F|a|c|t|e|u|r |e|n |p|o|u|r|c|e|n|t|a|g|e |s|u|r |t|o|u|t |t|i|r",
	L"|F|a|c|t|e|u|r |n|e|t |s|u|r |l|a |v|i|s|é|e",
	L"|F|a|c|t|e|u|r |p|o|u|r|c|e|n|t|a|g|e |d|e |v|i|s|é|e",
	L"|F|a|c|t|e|u|r |n|i|v|e|a|u |d|e |v|i|s|é|e |a|u|t|o|r|i|s|é|e",
	L"|F|a|c|t|e|u|r |d|e |p|r|é|c|i|s|i|o|n |m|a|x|i|m|a|l|e",
	L"|F|a|c|t|e|u|r |d|e |p|r|i|s|e |e|n |m|a|i|n |d|e |l|'|a|r|m|e",
	L"|F|a|c|t|e|u|r |c|o|m|p|e|n|s|a|t|i|o|n |d|e |c|h|u|t|e",
	L"|F|a|c|t|e|u|r |p|o|u|r|s|u|i|t|e |c|i|b|l|e",
	L"|F|a|c|t|e|u|r |d|e |d|é|g|â|t|s",
	L"|F|a|c|t|e|u|r |d|e |d|é|g|â|t|s |d|e |m|ê|l|é|e",
	L"|F|a|c|t|e|u|r |d|e |d|i|s|t|a|n|c|e",
	L"|F|a|c|t|e|u|r |a|g|g|r|a|n|d|i|s|e|m|e|n|t |d|e |l|a |p|o|r|t|é|e",
	L"|F|a|c|t|e|u|r |d|e |p|r|o|j|e|c|t|i|o|n",
	L"|F|a|c|t|e|u|r |d|e |r|e|c|u|l |h|o|r|i|z|o|n|t|a|l",
	L"|F|a|c|t|e|u|r |d|e |r|e|c|u|l |v|e|r|t|i|c|a|l",
	L"|F|a|c|t|e|u|r |d|e |c|o|n|t|r|e|-|f|o|r|c|e |m|a|x|i|m|u|m",
	L"|F|a|c|t|e|u|r |d|e |p|r|é|c|i|s|i|o|n |d|e |c|o|n|t|r|e|-|f|o|r|c|e",
	L"|F|a|c|t|e|u|r |d|e |f|r|é|q|u|e|n|c|e |d|e |c|o|n|t|r|e|-|f|o|r|c|e",
	L"|F|a|c|t|e|u|r |d|e |P|A |t|o|t|a|l",
	L"|F|a|c|t|e|u|r |d|e |P|A |m|i|s|e |e|n |j|o|u|e",
	L"|F|a|c|t|e|u|r |d|e |P|A |e|n |a|t|t|a|q|u|e |s|i|m|p|l|e",
	L"|F|a|c|t|e|u|r |d|e |P|A |e|n |r|a|f|a|l|e",
	L"|F|a|c|t|e|u|r |d|e |P|A |e|n |a|u|t|o",
	L"|F|a|c|t|e|u|r |d|e |P|A |p|o|u|r |r|e|c|h|a|r|g|e|r",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |m|u|n|i|t|i|o|n",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |r|a|f|a|l|e",
	L"|C|a|c|h|e|-|f|l|a|m|m|e",
	L"|F|a|c|t|e|u|r |i|n|t|e|n|s|i|t|é |s|o|n|o|r|e",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |o|b|j|e|t",
	L"|F|a|c|t|e|u|r |d|e |f|i|a|b|i|l|i|t|é",
	L"|C|a|m|o|u|f|l|a|g|e |f|o|r|ê|t",
	L"|C|a|m|o|u|f|l|a|g|e |u|r|b|a|i|n ",
	L"|C|a|m|o|u|f|l|a|g|e |d|é|s|e|r|t",
	L"|C|a|m|o|u|f|l|a|g|e |n|e|i|g|e",
	L"|F|a|c|t|e|u|r |d|e |d|i|s|c|r|é|t|i|o|n",
	L"|F|a|c|t|e|u|r |d|i|s|t|a|n|c|e |a|u|d|i|t|i|v|e",
	L"|F|a|c|t|e|u|r |v|i|s|i|o|n |g|é|n|é|r|a|l|e",
	L"|F|a|c|t|e|u|r |v|i|s|i|o|n |n|o|c|t|u|r|n|e",
	L"|F|a|c|t|e|u|r |v|i|s|i|o|n |d|e |j|o|u|r",
	L"|F|a|c|t|e|u|r |v|i|s|i|o|n |l|u|m|i|è|r|e |i|n|t|e|n|s|e",
	L"|F|a|c|t|e|u|r |v|i|s|i|o|n |s|o|u|s|-|s|o|l",
	L"|V|i|s|i|o|n |e|n |t|u|n|n|e|l ",
	L"|C|o|n|t|r|e|-|f|o|r|c|e |m|a|x|i|m|u|m",
	L"|F|r|é|q|u|e|n|c|e |C|o|n|t|r|e|-|f|o|r|c|e",
	L"|B|o|n|u|s |c|h|a|n|c|e |d|e |t|o|u|c|h|e|r",
	L"|B|o|n|u|s |d|e |v|i|s|é|e",
	L"|T|e|m|p|é|r|a|t|u|r|e |t|i|r |u|n|i|q|u|e",
	L"|T|a|u|x |d|e |R|e|f|r|o|i|d|i|s|s|e|m|e|n|t",
	L"|S|e|u|i|l |d|'|e|n|r|a|y|e|m|e|n|t",
	L"|S|e|u|i|l |d|é|t|é|r|i|o|r|a|t|i|o|n",
	L"|F|a|c|t|e|u|r |d|e |t|e|m|p|é|r|a|t|u|r|e",
	L"|F|a|c|t|e|u|r |d|e |r|e|f|r|o|i|d|i|s|s|e|m|e|n|t",
	L"|F|a|c|t|e|u|r |d|u |S|e|u|i|l |d|'|e|n|r|a|y|e|m|e|n|t",
	L"|F|a|c|t|e|u|r |d|u |S|e|u|i|l |d|é|t|é|r|i|o|r|a|t|i|o|n",
	L"|T|a|u|x |d|e |p|o|i|s|o|n",
	L"|F|a|c|t|e|u|r |d|'|e|n|c|r|a|s|s|e|m|e|n|t",
	L"|F|a|c|t|e|u|r |d|e |P|o|i|s|o|n",
	L"|V|a|l|e|u|r |d|'|A|l|i|m|e|n|t|a|t|i|o|n",
	L"|V|a|l|e|u|r |d|'|H|y|d|r|a|t|a|t|i|o|n",
	L"|T|a|i|l|l|e |d|e|s |Po|r|t|i|o|n|s",
	L"|F|a|c|t|e|u|r |d|e |M|o|r|a|l",
	L"|F|a|c|t|e|u|r |d|e |P|é|r|e|m|p|t|i|o|n",
	L"|P|o|r|t|é|e |O|p|t|i|m|a|l|e |d|u |L|a|s|e|r",
	L"|F|a|c|t|e|u|r |d|u |R|e|c|u|l |P|o|u|r |c|e|n|t",	// 65	
	L"|F|a|n |t|h|e |H|a|m|m|e|r",	// TODO.Translate
	L"|B|a|r|r|e|l |C|o|n|f|i|g|u|r|a|t|i|o|n|s",
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular.
STR16 szUDBAdvStatsExplanationsTooltipText[]=
{
	L"\n \nLorsque attaché à une arme de distance, cet objet modifie la\nvaleur de sa précision.\n \nLe gain en précision permet à l'arme de pouvoir toucher une\ncible à des distances plus élevées et plus souvent.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie la précision du tireur pour n'importe quel tir avec\nune arme de distance de la valeur suivante.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie la précision du tireur pour n'importe quel tir avec\nune arme de distance avec un pourcentage calculé à partir\nde sa précision initiale.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie le gain de précision, pris à chaque\nniveau de visée supplémentaire, de la valeur suivante.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie le gain de précision, pris à chaque\nniveau de visée, d'un pourcentage calculé à partir\nde sa précision initiale.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie le nombre de niveau de visée que cette arme\npeut avoir.\n \nRéduire le nombre de niveau de visée signifie que chaque\nniveau ajoute proportionnellement plus de précision au tir.\nPar conséquent, même les bas niveaux de visée vous\npermettrons de garder une bonne précision avec une vitesse\nélevée pour viser !\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la précision maximale du tireur équipé d'une\narme de distance, avec un pourcentage basé sur sa précision\ninitiale.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet modifie sa\ndifficulté de manipulation.\n \nUne meilleure prise en main permet une meilleure précision\nde l'arme, avec ou sans niveaux de visée supplémentaires.\n \nNotez que c'est basé sur le facteur de prise en main des\narmes, qui est plus élevé pour les fusils et armes lourdes\nque les pistolets et armes légères.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la difficulté des tirs hors de la portée de l'arme.\n \nUne valeur élevée peut augmenter la portée maximale de l'arme de\nquelques cases.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la difficulté de toucher une cible en mouvement\navec une arme de distance.\n \nUne valeur élevée peut vous aider à toucher une cible en\nmouvement, même à distance.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la puissance d'impact de votre arme\nde la valeur suivante.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la puissance d'impact de votre arme de mêlée\nde la valeur suivante.\n \nCeci s'applique uniquement aux armes de mêlée, tranchantes\nou contondantes.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie sa portée effective.\n \nLa portée maximale dicte essentiellement dans quelle mesure une balle\ntirée par l'arme peut voler avant de commencer à tomber\nbrusquement vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nfournit un grossissement supplémentaire, réussissant des coups\nplus facilement que la normale.\n \nNotez qu'un facteur de grossissement trop élevé est préjudiciable\nquand la cible est plus PROCHE que la distance optimale.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nprojette un point sur la cible, rendant le tir plus facile.\n \nCette projection est seulement utile à une certaine distance,\nau-delà elle diminue puis éventuellement disparaît.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\ndes modes rafale ou auto, cet objet modifie\nle recul horizontal de l'arme par le\npourcentage suivant.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\ndes modes rafale ou auto, cet objet modifie\nle recul vertical de l'arme par le\npourcentage suivant.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à faire\nface au recul durant une salve en mode rafale ou auto.\n \nUne valeur élevée permet d'aider le tireur à contrôler une arme\navec un fort recul, même s'il a peu de force.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à compenser\nle recul durant une salve en mode rafale ou auto.\n \nUne valeur élevée permet de corriger le recul pour garder le canon\nsur la cible, même à longue distance, rendant ainsi la salve\nplus précise.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à adapter\nà chaque fréquence l'effort de compensation du recul durant une\nsalve en mode rafale ou auto.\n \nUne fréquence élevée de compensation permet une salve très précise\net ainsi permettre des tirs en rafale et auto à très longues portées.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie directement le nombre de PA\nque le mercenaire a durant chaque début de tour.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le coût en PA pour mettre en joue.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme, cet objet\nmodifie le coût en PA pour faire une attaque simple.\n \nNotez que pour les armes ayant un mode auto/rafale, le\ncoût est directement influencé par ce facteur !\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\nun mode rafale, cet objet modifie le coût en PA d'un tir en rafale.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\nun mode auto, cet objet modifie le coût en PA d'un tir en auto.\n \nNotez que cela ne modifie pas le coût supplémentaire\npour ajouter des balles à la salve, mais seulement\nle coût initiale d'une salve.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le coût en PA pour recharger.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nchange la taille des munitions qui peuvent être\nchargées dans l'arme.\n \nCette arme peut maintenant accepter des tailles plus ou moins grandes de munitions\nayant un même calibre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le nombre de balles tiré\npar cette arme en rafale.\n \nSi cette arme n'a pas de mode rafale et que la\nvaleur est positive, alors cet objet donnera à l'arme la possibilité\nde tirer en mode rafale.\n \nInversement, s'il y a un mode rafale\net une valeur négative, cela peut retirer le mode rafale.\n \nValeur élevée généralement recommandée. Gardez bien à l'esprit\nque le mode rafale est là pour conserver les munitions...",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nva cacher le flash du canon.\n \nCela permettra au tireur de ne pas se faire repérer\net de rester à couvert, s'il l'est.\nChose importante de nuit...",
	L"\n \nLorsque attaché à une arme, cet objet\nmodifie la distance à laquelle un tir sera entendu par les\nennemis et les mercenaires.\n \nSi ce facteur modifie l'intensité sonore de l'arme à 0\n, elle deviendra alors indétectable.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la taille de n'importe quel objet\npouvant être attaché.\n \nLa taille est importante dans le nouveau système d'inventaire,\noù les poches n'acceptent qu'une taille et des formes spécifiques.\n \nAugmenter la taille d'un objet peut le rendre trop gros pour des poches.\n \nInversement, réduire sa taille peut permettre de l'insérer dans plus de poches\net les poches seront à même de contenir plus d'objets.\n \nValeur faible généralement recommandée.",
	L"\n \nLorsque attaché à une arme, cet objet modifie la valeur\nde fiabilité de l'arme.\n \nSi positive, l'état de l'arme se détériore moins\nrapidement au combat. Mais hors combat elle se détériore\nplus rapidement.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone forestière du porteur.\n \nPour avoir un facteur de camouflage efficace en forêt,\nvous devez être près d'arbres ou d'herbes hautes.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone urbaine du porteur.\n \nPour avoir un facteur de camouflage efficace en zone urbaine,\nvous devez être près des bâtîments.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone désertique du porteur.\n \nPour avoir un facteur de camouflage efficace en zone désertique,\nvous devez être près du sable ou d'une végétation désertique.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone enneigée du porteur.\n \nPour avoir un facteur de camouflage efficace en zone enneigée,\nvous devez être près de cases enneigées.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie la discrétion du porteur en rendant le mercenaire\nplus difficile à entendre lorsqu'il se déplace en mode discrétion.\n \nNotez que cela ne change en rien sur la visibilité du mercenaire,\nmais seulement la quantité de sons émis lors d'un déplacement en silence.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie l'audition du porteur du pourcentage suivant.\n \nUne valeur positive rend possible l'écoute de sons\nprovenant de longues distances.\n \nInversement, une valeur négative détériore l'audition du porteur.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision dans toutes les conditions.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsqu'il y a peu de lumière ambiante.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque l'intensité de la lumière\nest normale ou forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque l'intensité de la lumière est très forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque vous êtes dans un sous-sol sombre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nle champ de vision du porteur.\n \nRéduisant le champ de vision de chaque côté.\n \nValeur faible recommandée.",
	L"\n \nHabilité du tireur à faire face au recul\nlors d'un tir en mode rafale ou auto.\n \nValeur élevée recommandée.",
	L"\n \nFréquence de recalcule du tireur pour ajuster la force\nqu'il doit mettre pour contrer le recul de l'arme, lors d'un tir\nen mode rafale ou auto.\n \nUne fréquence faible rend la salve plus précise en supposant que\nle tireur puisse surmonter le recul correctement.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie sa chance de toucher la cible (CDT).\n \nAugmenter son CDT permet de toucher plus souvent\nune cible, en supposant que le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie ses bonus de visée.\n \nAugmenter les bonus de visée, permet de toucher\nune cible à longue distance plus souvent, en supposant\nque le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nUn tir augmente la température de l'arme de cette valeur.\nLes munitions et certains accessoires peuvent\ninfluer sur cette valeur.\n \nValeur faible recommandée.",
	L"\n \nÀ chaque tour, la température de l'arme diminue\nde cette valeur.\n \nValeur élevée recommandée.",
	L"\n \nSi la température d'une arme dépasse cette valeur,\nelle s'enrayera plus souvent.\n \nValeur élevée recommandée.",
	L"\n \nSi la température d'un objet dépasse cette valeur,\nil se détériorera plus facilement.\n \nValeur élevée recommandée.",
	L"\n \nLa température d'un tir,\nest augmentée par ce pourcentage.\n \nValeur faible recommandée.",
	L"\n \nLe facteur de refroidissement d'une arme,\nest augmenté par ce pourcentage.\n \nValeur élevée recommandée.",
	L"\n \nLe seuil d'enrayement d'une arme,\nest augmenté par ce pourcentage.\n \nValeur élevée recommandée.",
	L"\n \nLe seuil de détérioration d'une arme,\nest augmenté par ce pourcentage.\n \nValeur élevée recommandée.",
	L"\n \nUn tir salit l'arme de cette valeur. Le type\ndes munitions et les accessoires peuvent\ninfluer sur cette valeur.\n \nValeur faible recommandée.",
	L"\n \nLorsque cet aliment est mangé, il provoque un empoisonnement.\n \nValeur faible recommandée.",
	L"\n \nValeur énergétique en kcal.\n \nValeur élevée recommandée.",
	L"\n \nQuantité d'eau en litres.\n \nValeur élevée recommandée.",
	L"\n \nLe pourcentage d'aliment consommé par utilisation.\n \nValeur faible recommandée.",
	L"\n \nLe moral est modifié de cette valeur.\n \nValeur élevée recommandée.",
	L"\n \nCet aliment pourrit au fil du temps.\nAu-dessus de 50 pour cent, il devient\nun poison. Il s'agit de la vitesse à\nlaquelle la moisissure est générée.\n \nValeur faible recommandée.",
	L"",
	L"\n \nLorsqu'il est attaché à une arme qui a le mode\nrafale ou auto, cet objet modifie le recul de\nl'arme par le pourcentage indiqué. Réduire le\nrecul permet de mieux garder la bouche de l'arme\npointée sur la cible lors d'une rafale.\n \nValeur faible recommandée.", // 65
	L"\n \nIf a gunslinger wields this gun two-handed,\nthey can burst in hipfire.",	// TODO.Translate
	L"\n \nToggling firemodes also toggles how many\nbarrels you can fire at the same time.",
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \nLa précision de cette arme a été modifiée\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAugmenter la précision permet de toucher une cible\nà longue distance plus souvent.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la précision du tireur,\nde n'importe quel mode de tir, de la valeur suivante.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la précision du tireur,\nde n'importe quel mode de tir, du pourcentage suivant.\nPourcentage basé sur la précision initiale du tireur.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la quantité de précision gagnée\nà chaque niveau de visée de la valeur suivante.\n \nÉchelle : -100 à +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la quantité de précision gagnée\nà chaque niveau de visée du pourcentage suivant.\nPourcentage basé sur la précision initiale du tireur.\n \nValeur élevée recommandée.",
	L"\n \nLe nombre de niveaux de visée supplémentaires permis par\ncette arme, a été modifié par une munition, un accessoire\n ou bien intégré dans les attributs.\nSi le nombre de niveaux de visée a baissé, c'est que l'arme est\nplus rapide à viser sans perdre en précision.\n \nInversement, si le nombre de visée a augmenté, l'arme sera\nplus lente à viser sans perdre en précision.\n \nValeur faible recommandée.",
	L"\n \nCette arme modifie la précision maximum du tireur\nbasé sur un pourcentage de la précision initiale maximale.\n \nValeur élevée recommandée.",
	L"\n \nLes accessoires ou les caractéristiques de l'arme modifient\nsa difficulté de prise en main.\n \nUne meilleure prise en main, l'arme sera plus précise\navec ou sans niveaux de visée supplémentaires.\n \nNotez que c'est basé sur le facteur de prise en main des armes,\nqui est plus élevé pour les fusils et armes lourdes que\nles pistolets et armes légères.\n \nValeur faible recommandée.",
	L"\n \nL'habilité de l'arme à compenser les tirs\nqui sont hors de portée est modifié par un\naccessoire ou les caractéristiques de l'arme.\n \nUne valeur élevée peut augmenter la portée maximale\nde l'arme de quelques cases.\n \nValeur élevée recommandée.",
	L"\n \nL'habilité de l'arme à toucher une cible en mouvement\nà distance a été modifiée par un accessoire ou\nun attribut inhérent à l'arme.\n \nUne valeur élevée peut vous aider à toucher\nune cible en mouvement, même à distance.\n \nValeur élevée recommandée.",
	L"\n \nLa puissance d'impact de votre arme a été modifiée\npar une munition, un accessoire ou attribut inhérent à l'arme.\n \nValeur élevée recommandée.",
	L"\n \nLa puissance d'impact de votre arme de mêlée a été modifiée\npar un accessoire ou attribut inhérent à l'arme.\n \nCeci s'applique uniquement aux armes de mêlée, tranchantes\nou contondantes.\n \nValeur élevée recommandée.",
	L"\n \nLa portée maximum de votre arme a été augmentée ou diminuée\ngrâce à une munition, un accessoire ou attribut inhérent à l'arme.\n \nLa portée maximale dicte essentiellement dans quelle mesure une balle\ntirée par l'arme peut voler avant de commencer à tomber\nbrusquement vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nCette arme est équipée d'une visée optique,\nrendant les tirs à distance plus facile à réaliser.\n \nNotez qu'un facteur de grossissement trop élevée est préjudiciable\nquand la cible est plus PROCHE que la distance optimale.\n \nValeur élevée recommandée.",
	L"\n \nCette arme est équipée d'un système de projection\n(tel qu'un laser), qui projette un point sur la\ncible, rendant le tir plus facile.\n \nCette projection est seulement utile à une certaine distance\n, au-delà elle diminue puis éventuellement disparaît.\n \nValeur élevée recommandée.",
	L"\n \nLe recul horizontal de cette arme a été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAucun effet, si l'arme ne possède pas de mode auto et/ou rafale.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nLe recul vertical de cette arme a été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAucun effet, si l'arme ne possède pas de mode auto et/ou rafale.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à faire face\nau recul durant une salve en mode auto ou rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme.\n \nUne valeur élevée permet d'aider le tireur a contrôler une arme\navec un fort recul, même s'il a peu de force.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à compenser\nle recul durant une salve en mode auto ou rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme.\n \nUne valeur élevée permet de corriger le recul pour garder le canon\nsur la cible, même à longue distance,\nrendant ainsi la salve plus précise.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à adapter à chaque\nà chaque fréquence l'effort de compensation du recul durant\nune salve en mode auto ou rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme.\n \nUne fréquence élevée de compensation permet une salve très précise\net ainsi permettre des tirs en rafale et auto à très longues portées,\nen supposant que le tireur puisse couvrir le recul correctement.\n \nValeur élevée recommandée.",
	L"\n \nLorsque tenue en main, cette arme modifie la quantité de\nPA que le mercenaire a au début de chaque tour.\n \nValeur élevée recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour mettre en joue avec cette arme, a été\nmodifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une attaque simple avec cette arme,\na été modifié.\n \nNotez que les modes auto et rafale de l'arme,\nont leur coût directement influencé par ce facteur.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une rafale avec cette arme,\na été modifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une salve en auto avec cette arme,\na été modifié.\n \nNotez que cela ne modifie pas le coût supplémentaire en PA\nlorsque vous ajoutez des balles à la salve, mais\nseulement son coût initial.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour recharger cette arme,\na été modifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla taille des munitions qui peuvent être chargées sur cette arme,\na été modifiée.\n \nCette arme peut maintenant accepter des tailles plus ou moins grandes de munitions\nayant un même calibre.\n \nValeur élevée recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla quantité de balles tirées par cette arme en mode rafale,\na été modifiée.\n \nSi cette arme n'a pas de mode rafale et que la\nvaleur est positive, alors cet objet donnera à l'arme la possibilité\nde tirer en mode rafale.\n \nInversement, s'il y a un mode rafale\net une valeur négative, cela peut retirer le mode rafale.\n \nValeur élevée généralement recommandée. Gardez bien à l'esprit\nque le mode rafale est là pour conserver les munitions...",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\ncette arme ne produira pas de flash lors du tir.\n \nCela permettra au tireur de ne pas se faire repérer\net de rester à couvert, s'il l'est.\nChose importante de nuit...",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle bruit généré par l'arme, a été modifié. La distance\nà laquelle les ennemis et mercenaires peuvent entendre votre tir, a changé.\n \nSi ce facteur de l'intensité sonore de l'arme a 0\n, elle deviendra alors indédectable.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla catégorie de la taille de cette arme a changé.\n \nLa taille est importante dans le nouveau système d'inventaire,\noù les poches n'acceptent qu'une taille et des formes spécifiques.\n \nAugmenter la taille d'un objet peut le rendre trop gros pour des poches.\n \nInversement, réduire sa taille peut permettre de l'insérer dans plus de poches\net les poches seront à même de contenir plus d'objets.\n \nValeur faible généralement recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla fiabilité de cette arme a changé.\n \nSi positive, l'état de l'arme se détériore\nmoins rapidement, si utilisé en combat. Mais hors combat\nelle se détériore plus rapidement.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone forestière du porteur.\n \nPour avoir un facteur de camouflage efficace en forêt,\nvous devez être près d'arbres ou d'herbes hautes.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone urbaine du porteur.\n \nPour avoir un facteur de camouflage efficace en zone urbaine,\nvous devez être près de bâtîments.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone désertique du porteur.\n \nPour avoir un facteur de camouflage efficace en zone désertique,\nvous devez être près du sable ou d'une végétation désertique.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone enneigée du porteur.\n \nPour avoir un facteur de camouflage efficace en zone enneigée,\nvous devez être près de cases enneigés.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nla discrétion du porteur en rendant le mercenaire\nplus difficile à entendre lorsqu'il se déplace en mode discrétion.\n \nNotez que cela ne change en rien sur la visibilité du mercenaire,\nmais seulement la quantité de sons émis lors d'un déplacement en silence.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nl'audition du porteur du pourcentage suivant.\n \nUne valeur positive rend possible l'écoute de sons\nprovenant de longues distances.\n \nInversement, une valeur négative détériore l'audition du porteur.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision dans toutes les conditions.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision de nuit lorsqu'il y a peu de lumière ambiante.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision de jour lorsque l'intensité de la lumière\nest normale ou forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision lorsque l'intensité de la lumière est très forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision lorsque vous êtes dans un sous-sol sombre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie le champ de vision du porteur.\n \nRéduisant le champ de vision de chaque côté.\n \nValeur faible recommandée.",
	L"\n \nHabilité du tireur à faire face au recul\nlors d'un tir en mode rafale ou auto.\n \nValeur élevée recommandée.",
	L"\n \nFréquence de recalcule du tireur pour ajuster la force\nqu'il doit mettre pour contrer le recul de l'arme, lors d'un tir\nen mode rafale ou auto.\n \nUne fréquence faible rend la salve plus précise en supposant que\nle tireur puisse surmonter le recul correctement.\n \nValeur faible recommandée.",
	L"\n \nLa chance de toucher la cible avec cette arme,\na été modifiée par une munition, un accessoire ou\nun attribut inhérent à l'arme.\n \nAugmenter la chance de toucher permet de toucher plus souvent\nune cible, en supposant que le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nLes bonus de visée de cette arme, ont été modifiés\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAugmenter les bonus de visée, permet de toucher\nune cible à longue distance plus souvent, en supposant\nque le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nUn tir augmente la température de l'arme de cette valeur.\nLes munitions peuvent influer sur cette valeur.\n \nValeur faible recommandée.",
	L"\n \nÀ chaque tour, la température de l'arme diminue\nde cette valeur.\nLes accessoires des armes peuvent influer\nsur cette valeur.\n \nValeur élevée recommandée.",
	L"\n \nSi la température d'une arme dépasse cette valeur,\nelle s'enrayera plus souvent.",
	L"\n \nSi la température d'une arme dépasse cette valeur,\nelle se détériorera plus facilement.",
	L"\n \nLa force de recul de cette arme est modifiée par cette\nvaleur en pourcentage par ses munitions, ses objets attachés\nou ses caractéristiques. N'a aucun effet si l'arme n'a pas\nde mode rafale ni de mode automatique. Réduire le recul\npermet de mieux garder la bouche de l'arme pointée sur\nla cible lors d'une rafale.\n \nValeur faible recommandée.",
};

// HEADROCK HAM 4: Text for the new CTH indicator.
STR16 gzNCTHlabels[]=
{
	L"SIMPLE",
	L"PA",
};
//////////////////////////////////////////////////////
// HEADROCK HAM 4: End new UDB texts and tooltips
//////////////////////////////////////////////////////

// HEADROCK HAM 5: Screen messages for sector inventory sorting reports.
STR16 gzMapInventorySortingMessage[] =
{
	L"Fin du tri des munitions par caisses/boîtes au secteur %c%d.",
	L"Fin du démontage de tous les accessoires au secteur %c%d.",
	L"Fin du déchargement des armes au secteur %c%d.",
	L"Fin du classement et de l'empilage par type au secteur %c%d.",
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
	L"Tout voir",
	L"Armes",
	L"Munitions",
	L"Explosifs",
	L"Armes blanches",
	L"Protections",
	L"LBE",
	L"Kits",
	L"Objets divers",
	L"Cacher tout",
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
STR16 gTemperatureDesc[] =
{
	L"Température ",
	L"très basse",
	L"basse",
	L"moyenne",
	L"haute",
	L"très haute",
	L"dangereuse",
	L"CRITIQUE",
	L"EXTRÊME",
	L"inconnue",
	L"."
};

// Flugente: food condition texts
STR16 gFoodDesc[] =
{
	L"C'est ",
	L"frais",
	L"assez frais",
	L"consommable",
	L"périmé",
	L"malsain",
	L"nocif",
	L"."
};

CHAR16* ranks[] = 
{	L"",			//ExpLevel 0
	L"Rc. ",		//ExpLevel 1
	L"Sdt ",		//ExpLevel 2
	L"Cpl ",		//ExpLevel 3
	L"Sgt ",		//ExpLevel 4
	L"Maj ",		//ExpLevel 5
	L"Lt ",		//ExpLevel 6
	L"Cne ",		//ExpLevel 7
	L"Cdt ",	//ExpLevel 8
	L"Col ",		//ExpLevel 9
	L"Gal "		//ExpLevel 10
};

STR16	gzNewLaptopMessages[]=
{
	L"Renseignez-vous sur notre offre spéciale !",
	L"Temporairement indisponible",
	L"Un bref aperçu sur Jagged Alliance 2 : Unfinished Businessn, il contient six secteurs de la carte. La version finale du jeu proposera beaucoup plus. Lisez le fichier readme inclus pour plus de détails.",
};

STR16	zNewTacticalMessages[]=
{
	//L"Distance cible: %d tiles, Brightness: %d/%d",
	L"Vous reliez l'antenne de ce portable au vôtre.",
	L"Vous n'avez pas les moyens d'engager %s",
	L"Pour une durée limitée, les frais ci-dessus couvrent la mission entière, paquetage ci-dessous compris.",
	L"Engagez %s et découvrez dès à présent notre prix \"tout compris\".  Aussi inclus dans cette incroyable offre, le paquetage personnel du mercenaire sans frais supplémentaires.",
	L"Frais",
	L"Il y a quelqu'un d'autre dans le secteur...",
	//L"Portée arme: %d tiles, de chances: %d pourcent",
	L"Afficher couverture",
	L"Champs de vision",
	L"Les nouvelles recrues ne peuvent arriver ici.",
	L"Comme votre portable n'a pas d'antenne, vous ne pouvez pas engager de nouvelles recrues. Revenez à une sauvegarde précédente et réessayez.",
	L"%s entend le son de métal broyé provenant d'en dessous du corps de Jerry. On dirait que l'antenne de votre portable ne sert plus à rien.",  //the %s is the name of a merc.  @@@  Modified
	L"Apres avoir scanné la note laissée par le commandant adjoint Morris, %s sent une oppurtinité. La note contient les coordonnées pour le lancement de missiles sur Arulco. Elle contient aussi l'emplacement de l'usine d'où les missiles proviennent.",
	L"En examinant le panneau de contrôle, %s s'aperçoît que les chiffres peuvent être inversés pour que les missiles détruisent cette même usine.  %s a besoin de trouver un chemin pour s'enfuir. L'ascenseur semble être la solution la plus rapide...",
	L"Ceci est un jeu IRON MAN et vous ne pouvez pas sauvegarder, s'il y a des ennemis dans les parages.",	//	@@@  new text
	L"(ne peut pas sauvegarder en plein combat)", //@@@@ new text
	L"Le nom de la campagne actuelle est supérieur à 30 lettres.",							// @@@ new text
	L"La campagne actuelle est introuvable.",																	// @@@ new text
	L"Campagne : Par défaut ( %S )",																							// @@@ new text
	L"Campagne : %S",																													// @@@ new text
	L"Vous avez choisi la campagne %S. Cette campagne est un mod d'Unfinished Business. Êtes-vous sûr de vouloir jouer la campagne %S ?",			// @@@ new text
	L"Pour pouvoir utiliser l'éditeur, veuillez choisir une autre campagne que celle par défaut.",		///@@new
	// anv: extra iron man modes
	L"This is a SOFT IRON MAN game and you cannot save during turn-based combat.",	// TODO.Translate	
	L"This is an EXTREME IRON MAN game and you can only save once per day, at %02d:00.",	// TODO.Translate
};

// The_bob : pocket popup text defs
STR16	gszPocketPopupText[]=
{
	L"Lance-grenades",	// POCKET_POPUP_GRENADE_LAUNCHERS,
	L"Lance-roquettes",	// POCKET_POPUP_ROCKET_LAUNCHERS
	L"Armes blanches",	// POCKET_POPUP_MEELE_AND_THROWN
	L"-Aucune munition-",	//POCKET_POPUP_NO_AMMO
	L"-Pas d'arme-",	//POCKET_POPUP_NO_GUNS
	L"plus...",		//POCKET_POPUP_MOAR
};

// Flugente: externalised texts for some features
STR16	szCovertTextStr[]=
{
	L"%s a du camouflage !",
	L"%s a un sac à dos !",
	L"%s est vu(e) en train de porter un corps !",
	L"%s a un(e) %s suspect(e) !",
	L"%s a un(e) %s considéré(e) comme du matériel militaire !",
	L"%s transporte trop d'armes !",
	L"%s a un(e) %s trop avancé(e) pour un soldat %s !",
	L"%s a un(e) %s avec trop d'accessoires !",
	L"%s a été repéré(e) en train de commettre des activités douteuses !",
	L"%s ne ressemble pas à un civil !",
	L"Le sang de %s, a été repéré !",
	L"%s est trop ivre pour se comporter comme un soldat !",
	L"Le déguisement de %s, ne tiendra pas la route à une inspection !",
	L"%s n'est pas supposé(e) être là !",
	L"%s n'est pas supposé(e) se trouver là à cette heure !",
	L"%s a été trouvé(e) près d'un cadavre !",
	L"L'équipement de %s, soulève quelques suspicions !",
	L"%s est vu(e) en train de viser %s !",
	L"%s a percé le déguisement : %s !",
	L"Aucun habit trouvé dans Items.xml !",
	L"Ça ne fonctionne pas avec l'ancien système de compétences !",
	L"Pas assez de PA !",
	L"Mauvaise palette trouvée !",
	L"Vous avez besoin de la compétence \"Déguisement\" ou \"Espion\" pour le faire !",
	L"Pas d'uniforme trouvé !",
	L"%s est maintenant déguisé(e) en civil.",
	L"%s est maintenant déguisé(e) en soldat.",
	L"%s porte un uniforme dépareillé !",
	L"En y repensant, demander une reddition avec un déguisement n'était pas la meilleure des idées...",
	L"%%s a été découvert(e) !",
	L"Le déguisement de %s a l'air d'aller...",
	L"Le déguisement de %s ne marchera pas...",
	L"%s a été pris en train de voler !",
	L"%s a essayé d'accéder à l'inventaire de %s.",
	L"An elite soldier did not recognize %s!",	// TODO.Translate
	L"A officer knew %s was unfamiliar!",
};

STR16	szCorpseTextStr[]=
{
	L"Aucune tête trouvée dans items.xml !",
	L"Ce corps n'a plus de tête :",
	L"Aucune nourriture trouvée dans Items.xml !",
	L"Impossible, vous êtes malade ou une personne tordue !",
	L"Aucun habit à prendre !",
	L"%s ne peut pas prendre les habits du corps !",
	L"Ce corps ne peut être pris !",
	L"Pas de main libre pour le corps !",
	L"Aucun corps trouvé dans Items.xml !",
	L"Identité du corps invalide !",
};

STR16	szFoodTextStr[]=
{
	L"%s ne veut pas manger : %s",
	L"%s ne veut pas boire : %s",
	L"%s mange : %s",
	L"%s boit : %s",
	L"%s a sa force pénalisée par cause de suralimentation !",
	L"%s a sa force pénalisée par cause de sous-alimentation !",
	L"%s a sa santé pénalisée par cause de suralimentation !",
	L"%s a sa santé pénalisée par cause de sous-alimentation !",
	L"%s a sa force pénalisée par cause de hyperhydratation !",
	L"%s a sa force pénalisée par cause de déshydratation !",
	L"%s a sa santé pénalisée par cause de hyperhydratation !",
	L"%s a sa santé pénalisée par cause de déshydratation !",
	L"Le remplissage des gourdes n'est pas possible, si le système alimentaire n'est pas activé !"
};

STR16	szPrisonerTextStr[]=
{
	L"%d officers, %d elites, %d regulars, %d admins, %d generals and %d civilians were interrogated.",	// TODO.Translate
	L"Gained $%d as ransom money.",		// TODO.Translate
	L"Prisonnier(s) ayant révélé les positions ennemies : %d.",
	L"Prisonnier(s) ayant rejoint notre cause : %d officiers, %d élite(s), %d régulier(s) et %d administratif(s).",
	L"Prisonnier(s) ayant commencé une émeute en %s !",
	L"%d prisonnier(s) envoyé(s) en %s !",
	L"Prisonnier(s) libéré(s) !",
	L"L'armée a délivré la prison en %s et les prisonniers ont été libérés !",
	L"L'ennemi refuse de se rendre !",
	L"L'ennemi refuse votre reddition... Ils veulent vos têtes !",
	L"Ce comportement est désactivé dans vos fichiers ini.",
	L"%s a libéré %s !",
	L"A high-ranking army officer in %s has been revealed!",	// TODO.Translate
	L"The enemy leader refuses to even consider surrender!",
	L"%d prisoners volunteered to join our forces.",
	L"Some of your mercs managed to escape the enemy capture!",
	L"No possible escape is seen, it's a fight to the death!"
};

STR16	szMTATextStr[]=
{
	L"rien, en fait.",
	L"la construction d'une fortification",
	L"le retrait d'une fortification",
	L"hacking",	// TODO.Translate
	L"%s a dû arrêter... %s",
	L"Cette sorte de barricade ne peut pas être construite dans ce secteur",
};

STR16	szInventoryArmTextStr[]=
{
	L"Faire exploser (%d PA)",
	L"Faire exploser",
	L"Armer (%d PA)",
	L"Armer",
	L"Désamorcer (%d PA)",
	L"Désamorcer",
};


STR16		szBackgroundText_Flags[]=
{
	L" peut consommer des drogues se trouvant dans son inventaire\n",
	L" ne tient pas compte des autres passifs\n",
	L" +1 Niveau dans les souterrains\n",
	L" steals money from the locals sometimes\n", // TODO.Translate

	L" +1 Niveau pour poser des pièges\n",
	L" répand la corruption aux mercenaires proches\n",
	L" femme seulement",							// won't show up, text exists for compatibility reasons
	L" homme seulement",								// won't show up, text exists for compatibility reasons

	L" huge loyality penalty in all towns if we die\n",		// TODO.Translate

	L" refuses to attack animals\n",	// TODO.Translate
	L" refuses to attack members of the same group\n",	// TODO.Translate
};


STR16	szBackgroundText_Value[]=
{
	L" %s%d%% de PA dans les secteurs polaires\n",
	L" %s%d%% de PA dans les secteurs désertiques\n",	
	L" %s%d%% de PA dans les secteurs marécageux\n",
	L" %s%d%% de PA dans les secteurs urbains\n",
	L" %s%d%% APs in forest sectors\n",	// TODO.Translate
	L" %s%d%% APs in plain sectors\n",
	L" %s%d%% de PA dans les secteurs fluviaux\n",
	L" %s%d%% de PA dans les secteurs tropicaux\n",
	L" %s%d%% de PA dans les secteurs côtiers\n",
	L" %s%d%% de PA dans les secteurs montagneux\n",

	L" %s%d%% en agilité\n",
	L" %s%d%% en dextérité\n",
	L" %s%d%% en force\n",
	L" %s%d%% en commandement\n",
	L" %s%d%% au tir\n",
	L" %s%d%% en mécanique\n",	
	L" %s%d%% en explosifs\n",
	L" %s%d%% en médecine\n",
	L" %s%d%% en sagesse\n",

	L" %s%d%% de PA sur les toits\n",
	L" %s%d%% de PA nécessaire pour nager\n",
	L" %s%d%% de PA nécessaire pour les actions de fortification\n",
	L" %s%d%% de PA nécessaire pour utiliser un mortier\n",
	L" %s%d%% de PA nécessaire pour utiliser l'inventaire\n",
	L" toujours opérationnel après un parachutage\n %s%d%% de PA après un parachutage\n",
	L" %s%d%% de PA au premier tour lors d'un assaut d'un secteur\n",
		
	L" %s%d%% de vitesse dans les voyages à pied\n",
	L" %s%d%% de vitesse dans les voyages en véhicule\n",
	L" %s%d%% de vitesse dans les voyages aériens\n",
	L" %s%d%% de vitesse dans les voyages sur l'eau\n",

	L" %s%d%% de résistance à la peur\n",
	L" %s%d%% de résistance au tir de couverture\n",
	L" %s%d%% de résistance physique\n",
	L" %s%d%% de résistance à l'alcool\n",
	L" %s%d%% disease resistance\n",	// TODO.Translate

	L" %s%d%% d'efficacité dans les interrogatoires\n",
	L" %s%d%% d'efficacité comme gardien de prison\n",
	L" %s%d%% meilleurs prix au marchandage d'armes et de munitions\n",
	L" %s%d%% meilleurs prix au marchandage d'armures, de LBE, d'armes blanches, kits etc.\n",
	L" %s%d%% à la force de capitulation de l'équipe, si nous menons les négociations\n",
	L" %s%d%% plus rapide à la marche\n",
	L" %s%d%% de vitesse de bandage\n",
	L" %s%d%% breath regeneration\n",	// TODO.Translate
	L" %s%d%% de force pour porter des objets\n",
	L" %s%d%% de besoins énergétiques (nourriture)\n",
	L" %s%d%% de réhydratation nécessaire (eau)\n",
	L" %s%d de besoin de sommeil\n",
	L" %s%d%% de dégâts avec une arme de mêlée\n",	
	L" %s%d%% de chance de toucher avec des armes blanches\n",
	L" %s%d%% d'efficacité dans le camouflage\n",
	L" %s%d%% en discrétion\n",
	L" %s%d%% de chance de toucher maximum\n",
	L" %s%d en audition pendant la nuit\n",
	L" %s%d en audition pendant la journée\n",
	L" %s%d d'efficacité à désamorcer les pièges\n",
	L" %s%d%% CTH with SAMs\n",	// TODO.Translate

	L" %s%d%% d'efficacité dans une approche amicale\n",
	L" %s%d%% d'efficacité dans une approche directe\n",
	L" %s%d%% d'efficacité dans une approche menaçante\n",
	L" %s%d%% d'efficacité dans une approche de recrutement\n",

	L" %s%d%% de chance de succès avec les explosifs d'ouverture de porte\n",
	L" %s%d%% de CDT avec des armes à feu contre les créatures\n",
	L" %s%d%% du coût de l'assurance\n",
	L" %s%d%% d'efficacité comme guetteur pour vos tireurs d'élite\n",
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

STR16		szBackgroundTitleText[] =
{
	L"IMP : Passif",
};

// Flugente: personality
STR16		szPersonalityTitleText[] =
{
	L"IMP : Préjugés",
};

STR16		szPersonalityDisplayText[]=
{
	L"Vous êtes",
	L"et l'apparence est",
	L"importante pour vous.",
	L"Vos",
	L"sont",
	L"essentielles.",
	L"Vous êtes",
	L"vous haïssez tout",
	L".",
	L"raciste envers les non-",
	L".",
};

// texts showing up when hovering over the box, used to explain what a selection does. Do not use more than 200 characters!
STR16		szPersonalityHelpText[]=
{
	L"Comment vous voyez-vous ?",
	L"Quelle importance accordez-vous\nau regard des autres ?",
	L"Quels sont vos manières ?",
	L"Quelle est l'importance des manières des autres pour vous ?",
	L"Quelle est votre nationalité ?",
	L"Vous haïssez quelle nationalité ?",
	L"À quel point les haïssez-vous ?",
	L"À quel point êtes-vous raciste ?",
	L"Quelle est votre race ? Et vous serez\nraciste contre toutes les autres.",
	L"À quel point êtes-vous sexiste ?",
};

STR16		szRaceText[]=
{
	L"Blancs",
	L"Noirs",
	L"Asiatiques",
	L"Esquimaux",
	L"Hispaniques",
};

STR16		szAppearanceText[]=
{
	L"quelconque",
	L"laid",
	L"ordinaire",
	L"attirant",
	L"très beau",
};

STR16		szRefinementText[]=
{
	L"manières banales",
	L"manières de plouc",
	L"manières de snob",
};

STR16		szRefinementTextTypes[] =	// TODO.Translate
{
	L"normal people",
	L"slobs",
	L"snobs",
};

STR16		szNationalityText[]=
{
	L"Américain",		// 0
	L"Arabe",
	L"Australien",
	L"Anglais",
	L"Canadien",
	L"Cubain",			// 5
	L"Danois",
	L"Français",
	L"Russe",
	L"Nigérian",
	L"Suisse",			// 10
	L"Jamaïcain",
	L"Polonais",
	L"Chinois",
	L"Irlandais",
	L"Sud Africain",	// 15
	L"Hongrois",
	L"Écossais",
	L"Arulcain",
	L"Allemand",
	L"Africain",			// 20
	L"Italien",
	L"Néerlandais",
	L"Roumain",
	L"Métavirien",

	// newly added from here on
	L"Grec",			// 25
	L"Estonien",
	L"Vénézuélien",
	L"Japonais",
	L"Turc",
	L"Indien",			// 30
	L"Mexicain",
	L"Norvégien",
	L"Espagnol",
	L"Brésilien",
	L"Finlandais",			// 35
	L"Iranien",
	L"Israélien",
	L"Bulgare",
	L"Suédois",
	L"Irakien",			// 40
	L"Syrien",
	L"Belge",
	L"Portugais",
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
	L"et n'haïssez aucune autre nationalité.",		// used in personnel.cpp
	L"apatride",		// used in IMP generation
};

STR16		szCareLevelText[]=
{
	L"non",
	L"vaguement",
	L"bigrement",
};

STR16		szRacistText[]=
{
	L"pas",
	L"un peu",
	L"très",
};

STR16		szSexistText[]=
{
	L"pas sexiste",
	L"un peu sexiste",
	L"très sexiste",
	L"un gentleman",
};

// Flugente: power pack texts
STR16 gPowerPackDesc[] =
{
	L"État : ",
	L"Chargée",
	L"Bonne",
	L"À moitié chargée",
	L"Faible",
	L"Morte",
	L"."
};

// WANNE: Special characters like % or someting else should go here
// We can't put them directly in the CPP code files, because they need special encoding (UTF8) for some languages (e.g: Chinese)
STR16 sSpecialCharacters[] =
{
	L"%",		// Percentage character
};

STR16	szSoldierClassName[]=
{
	L"Mercenaire",
	L"Milicien",
	L"Soldat",
	L"Vétéran",

	L"Civil",

	L"Administratif",
	L"Soldat régulier",
	L"Soldat d'élite",
	L"Char",

	L"Animal",
	L"Zombi",
};

STR16	szCampaignHistoryWebSite[]=
{
	L"%s : Conseil de presse",
	L"Ministère de l'information %s",
	L"Mouvement révolutionnaire à %s",
	L"The Times International",
	L"International Times",
	L"RIS (Renseignements Internationaux Spécialisés)",

	L"Recueille les articles de presse sur %s",
	L"Nous sommes une source d'information neutre. Nous collectons différents articles d'actualité venant d'%s. Nous ne jugeons pas ces sources, nous nous contentons de les publier, pour que vous puissiez vous faire votre avis. Nous faisons paraitre des articles de différentes sources, venant :",
	
	L"Bilan du conflit",
	L"Rapports",
	L"News",
	L"À propos de nous",
};

STR16	szCampaignHistoryDetail[]=
{
	L"%s, %s %s %s en %s.",

	L"la guérilla",
	L"l'armée",

	L"a attaqué",
	L"a pris en embuscade",
	L"héliportée a attaqué",

	L"L'attaque est venue %s.",
	L"%s a eu des renforts venant %s.",
	L"L'attaque est venue %s. %s a eu des renforts venant %s.",
	L"du nord",
	L"de l'est",
	L"du sud",
	L"de l'ouest",
	L"et",
	L"d'une direction inconnue",

	L"Des bâtiments ont été endommagés.",
	L"Dans les combats, des bâtiments ont été endommagés. Il y a eu %d civil(s) tué(s) et %d blessé(s).",
	L"Pendant l'attaque, %s et %s ont appelé des renforts.",
	L"Pendant l'attaque, %s a appelé des renforts.",
	L"Les témoins rapportent l'utilisation d'armes chimiques par les deux camps.",
	L"Des armes chimiques ont été utilisées par %s.",
	L"L'escalade du conflit s'aggrave ; les deux camps ont déployés des chars.",
	L"Il y avait %d chars pour renforcer %s. %d d'entre eux ont été détruits dans des combats acharnés.",
	L"Les deux camps avaient des tireurs d'élite.",
	L"Des sources non vérifiées indiquent que des tireurs d'élite de %s ont été impliqués dans le combat.",
	L"Ce secteur a une très grande importance stratégique, car il abrite l'une des rares batteries de missiles sol-air que l'armée %s possède. Des photographies aériennes montrent les dégâts du centre de commande. Ça laissera l'espace aérien %s sans défense pour le moment.",  // TODO.Translate //A voir fini (to see finished)
	L"La situation sur le terrain est devenue encore plus confuse, car il semble que le combat des rebelles a pris un nouveau virage. On a maintenant la confirmation qu'une milice rebelle s'est engagée activement avec les mercenaires étrangers.",
	L"La position des royalistes semble plus précaire qu'on ne le pensait. Des rapports d'une scission au sein de l'armée ont fait surface, impliquant des échanges de feu au sein même du personnel militaire.",
};

STR16	szCampaignHistoryTimeString[]=
{
	L"Tard dans la nuit",		// 23 - 3
	L"À l'aube",					// 3 - 6
	L"Tôt ce matin",	// 6 - 8
	L"Dans la matinée",	// 8 - 11
	L"À midi",					// 11 - 14
	L"Dans l'après-midi",		// 14 - 18
	L"Dans la soirée",			// 18 - 21
	L"Dans la nuit",		// 21 - 23
};

STR16	szCampaignHistoryMoneyTypeString[]=
{
	L"Fond initial",
	L"Revenu des mines",
	L"Commerce",
	L"Autres",
};

STR16	szCampaignHistoryConsumptionTypeString[]=
{
	L"Munition",
	L"Explosifs",
	L"Nourriture",
	L"Matériel médical",
	L"Maintenance",
};

STR16	szCampaignHistoryResultString[]=
{
	L"Dans une bataille extrêmement meurtrière et inégale, l'armée a été anéantie sans trop de résistance.",

	L"Les rebelles ont facilement vaincu l'armée, infligeant de lourdes pertes.",
	L"Sans trop d'effort, les rebelles ont infligé de lourdes pertes à l'armée et ont fait plusieurs prisonniers.",

	L"Dans un combat meurtrier, les rebelles ont réussi à écraser la partie adversaire. L'armée a subi des pertes sévères.",
	L"Les rebelles ont subi des pertes, mais ont vaincu les royalistes. Des sources non vérifiées disent que plusieurs soldats auraient été faits prisonniers.",

	L"Dans une victoire à la Pyrrhus, les rebelles ont vaincu les royalistes, mais ils ont subi de lourdes pertes. Il n'est pour l'instant pas possible de dire s'ils arriveront à tenir position face à des assauts répétés.",
	
	L"La supériorité numérique de l'armée a été l'élément déterminant de ce combat. Les rebelles n'avaient aucune chance et ont dû se replier pour ne pas être tués ou capturés.",
	L"Malgré le nombre élevé de rebelles dans ce secteur, l'armée les a facilement repoussés.",

	L"Les rebelles n'étaient clairement pas préparés à affronter la supériorité numérique de l'armée, ni son niveau d'équipement. Ils ont été aisément vaincus.",
	L"Même si les rebelles étaient plus nombreux sur le terrain, l'armée était mieux équipée. Les rebelles ont évidemment perdu.",

	L"La violence des combats a fait des pertes considérables dans les deux camps, mais à la fin, la supériorité numérique l'armée a fait pencher la balance en sa faveur. La force rebelle a été anéantie. Il pourrait y avoir des survivants, mais nous ne pouvons pas confirmer cette source pour le moment.",
	L"Lors d'une fusillade intense, l'entraînement supérieur de l'armée a fait pencher la balance en sa faveur. Les rebelles ont dû battre en retraite.",
	
	L"Aucun des deux camps n'était prêt à se soumettre. Alors que l'armée a finalement écarté la menace rebelle de la zone, leurs pertes conséquentes ont conduit l'unité à continuer d'exister uniquement de nom. Mais il est clair que les rebelles vont rapidement être à court d'hommes et de femmes si l'armée continue ce taux d'attrition.",
};

STR16	szCampaignHistoryImportanceString[]=
{
	L"Hors sujet",
	L"Fait mineur",
	L"Fait notable",
	L"Fait marquant",
	L"Fait significatif",
	L"Fait intéressant",
	L"Fait important",
	L"Fait très important",
	L"Fait grave",
	L"Fait majeur",
	L"Fait historique",
};

STR16	szCampaignHistoryWebpageString[]=
{
	L"Tué",
	L"Blessé",
	L"Prisonnier",
	L"Tir",

	L"Compte",
	L"Logistique",
	L"Pertes",
	L"Participant",

	L"Promotion",
	L"Bilan",
	L"Récit",
	L"Précédent",

	L"Suivant",
	L" :",
	L"Jour",
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
        L"Manipulations de l'inventaire",

        L"LVN",
        L"Recharger",
        L"Réunir o.",
        L"",

        L"Trier",
        L"Fusionner",
        L"Séparer",
        L"Classer",

        L"Caisses",
        L"Boîtes",
        L"Poser S/D",
		L"Mett. S/D",

        L"",
        L"",
        L"",
        L"",
};

STR16   szTacticalCoverDialogString[]=
{
        L"Afficher couverture",

        L"Fermer",
        L"Ennemi",
        L"Merc.",
        L"",

        L"Roles",	// TODO.Translate
		L"Fortification",	// TODO.Translate
		L"Tracker",
		L"CTH mode",

        L"Pièges",
        L"Réseau",
        L"Détecteur",
        L"",

        L"Réseau A",	    
        L"Réseau B",	    
        L"Réseau C",	    
        L"Réseau D",	    
};

STR16   szTacticalCoverDialogPrintString[]=
{
        
        L"Désactivation affichage couverture/pièges",
        L"Afficher les zones dangereuses",
        L"Afficher la vue du mercenaire",
        L"",

		L"Display enemy role symbols",	// TODO.Translate
		L"Display planned fortifications",
		L"Display enemy tracks",
        L"",

        L"Afficher réseau (piège)",
        L"Afficher les réseaux (pièges) par couleur",
        L"Afficher les pièges à proximité",
        L"",

        L"Afficher le réseau A",
        L"Afficher le réseau B",
        L"Afficher le réseau C",
        L"Afficher le réseau D",
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

STR16 szIMPGearPocketText[] =
{
	L"Select helmet",
	L"Select vest",
	L"Select pants",
	L"Select face gear",
	L"Select face gear",

	L"Select main gun",
	L"Select sidearm",

	L"Select LBE vest",
	L"Select left LBE holster",
	L"Select right LBE holster",
	L"Select LBE combat pack",
	L"Select LBE backpack",

	L"Select launcher / rifle",
	L"Select melee weapon",

	L"Select additional items", //BIGPOCK1POS
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select medkit", //MEDPOCK1POS
	L"Select medkit",
	L"Select medkit",
	L"Select medkit",
	L"Select main gun ammo", //SMALLPOCK1POS
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select main gun ammo",
	L"Select launcher / rifle ammo", //SMALLPOCK6POS
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select launcher / rifle ammo",
	L"Select sidearm ammo", //SMALLPOCK11POS
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select sidearm ammo",
	L"Select additional items", //SMALLPOCK19POS
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items",
	L"Select additional items", //SMALLPOCK30POS
	L"Left click to select item / Right click to close window",
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
	L"National Overview",
	L"Regional Overview",
	L"Mission Overview",
	L"Select View:",
	L"Regional (2)",
	L"National (1)",
	L"Mission (3)",
	L"Supplies:",
	L"Incoming Supplies",
	L"Intel:",
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
	L"Insufficient supplies! Admin Actions have been DISABLED.",
	L"New missions will be available every %d hours.",
	L"New missions are available at the A.R.C. website.",
	L"Mission preparations in progress.",
	L"Mission duration: %d days",
	L"Chance of success: %d%s",
	L"[REDACTED]",
	L"Name: %s",
	L"Location: %s",
	L"Assignment: %s",
	L"Contract: %d days",
	L"Contract: %d hours",
	L"Contract: ---",
	L"Agent bonus:",
	L"Chance of success +%d%s (%s)",
	L"Deployment range +%d (%s)",
	L"ASD Income -%2.0f%s (%s)",
	L"Steal fuel; send to %s (%s)",
	L"Destroy reserve units (%s)",
	L"Time +%2.0f%s (%s)",
	L"Vision -%2.0f%s (%s)",
	L"Gear quality -%d (%s)",
	L"Overall stats -%d (%s)",
	L"Max trainers: %d (%s)",
	L"Payout +%2.0f%s (%s)",
	L"Payout limit increased to $%d (%s)",
	L"Bonus for officers (%s)",
	L"Bonus for vehicles (%s)",
	L"Duration +%d hours (%s)",
	L"Agent not in town",
	L"Town loyalty too low",
	L"Agent unavailable",
	L"Agent contract expiring",
	L"Can't use rebel agent",
	L"Battle in progress",
	L"Prepare Mission (%d supplies)",
	L"View active mission effects",
	L"View available mission list",
	L"You are able to prepare one of the two missions presented. Once an agent is dispatched, they will be unavailable for approximately %d hours before becoming available again. A popup will notify you when preparations are complete. If preparations succeed, the mission's effects become active.",
	L"A rebel agent can be sent to prepare a mission, but your mercenaries will be far more effective. Their experience level and skills can provide additional bonuses to missions.",
	L"The cost of preparing a mission increases based on the number other missions either active or being prepared.",
	L"New missions will be available on Day %d at 00:00.",
	L"Active missions:",
	L"%s - Preparing - Ready on Day %d, %02d:%02d",
	L"%s - Active - Expires on Day %d, %02d:%02d",
	L"[%s (%d supplies)]",
	L"%s Send a rebel agent to prepare this mission?",
	L"%s Send %s to prepare this mission? He will return in approximately %d hours.",
	L"%s Send %s to prepare this mission? She will return in approximately %d hours.",
	L"Mission \"%s\" is now in effect.",
	L"Preparations for mission \"%s\" failed.",
	L"Mission \"%s\" has expired and is no longer in effect.",
};

STR16 szRebelCommandHelpText[] = // TODO.Translate
{
	L"|S|u|p|p|l|i|e|s\n \nFood, water, medical supplies, weapons, and anything else that\nthe rebels might find useful. Supplies are obtained automatically\nby the rebels.",
	L"|I|n|c|o|m|i|n|g |S|u|p|p|l|i|e|s\n \nEach day, the rebels will gather supplies on their own. As you\ntake over more towns, the amount of supplies they will be\nable to find per day will increase.\n \n+%d (Base income)",
	L"|C|u|r|r|e|n|t |D|i|r|e|c|t|i|v|e\n \nYou can choose how the rebels will prioritise their strategic\nobjectives. New directives will become available as you make\nprogress.",
	L"|A|d|m|i|n|i|s|t|r|a|t|i|o|n |T|e|a|m\n \nOnce deployed, an admin team is responsible for handling the\nday-to-day affairs of the region. This includes supporting\nlocals, creating rebel propaganda, establishing regional\npolicies, and more.",
	L"|L|o|y|a|l|t|y\n \nThe effectiveness of many Administrative Actions depends on\nthe region's loyalty to your cause. It is in your best interest\nto raise loyalty as high as possible.",
	L"|M|a|x|i|m|u|m |L|o|y|a|l|t|y\n \nYou will need to convince the locals to fully trust you. This\ncan be done by creating a supply line to them, showing that\nyou intend to improve their quality of life.",
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
	L"Collect money from the locals to assist your efforts. Increases daily income, but regional loyalty falls daily.",
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

STR16 szRebelCommandAgentMissionsText[] =
{
	L"Deep Deployment",
	L"Coordinate efforts to find ways to sneak up on the enemy, but be careful: it's equally possible to put yourself in a disadvantaged deployment area. When attacking enemy forces, the deployment area is much larger.",
	L"Disrupt ASD",
	L"Wreak havoc on the day-to-day operations of the Arulco Special Division. Temporarily prevent the ASD from deploying additional mechanised units, and drastically reduce their daily income.",
	L"Strategic Intel",
	L"Intercept plans and discover where enemies intend to strike. When viewing teams on the strategic map, sectors prioritised by the enemy will be marked in red.",
	L"Improve Local Shops",
	L"Set up ways for merchants across the country to acquire better goods more easily. Shopkeepers will have better than usual inventories.",
	L"Slow Strategic Decisions",
	L"Sow confusion and misdirection at the highest levels of enemy command. The enemy takes longer to make decisions at a strategic level.",
	L"Lower Readiness",
	L"Trick enemy soldiers into letting their guard down. Enemy soldiers have reduced vision range until they are alerted to your mercs' presence.",
	L"Sabotage Equipment",
	L"Disrupt enemy supply chains and prevent the enemy from maintaining their gear properly. Enemy soldiers use equipment that is worse than normal.",
	L"Sabotage Vehicles",
	L"Sabotage vehicle maintenance hubs to reduce their combat effectiveness and readiness. Enemy vehicles encountered have reduced stats.",
	L"Send Supplies",
	L"Temporarily increase direct support to this town. Town loyalty passively increases for the duration of the mission.",
	L"Soldier Bounties (Kingpin)",
	L"Get a payout for enemy kills. Negotiate with Kingpin, who feels he can use your presence here to indirectly weaken the Queen's power. Bounties are deposited into your account at midnight and are limited to $%d per day.",
	L"Train Militia Anywhere",
	L"Create training areas in the wilderness that can be quickly set up and torn down. Militia can be trained in uncontested sectors outside of town.",
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

#endif //FRENCH
