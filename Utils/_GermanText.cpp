//#pragma setlocale("GERMAN")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#ifdef GERMAN
		#include "text.h"
		#include "Fileman.h"
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
	L"@};(Verschiedene weiter Mods!)",
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

	L"Der Gegenstand in Ihrem Cursor kann mit anderen Gegenständen verbunden werden, indem Sie ihn in einem der vier Slots platzieren.",
	L"Der Gegenstand in Ihrem Cursor kann mit anderen Gegenständen verbunden werden, indem Sie ihn in einem der vier Attachment-Slots platzieren. (Aber in diesem Fall sind die Gegenstände nicht kompatibel.)",
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
	L"%s's Wüstentarnung wurde heruntergewaschen.",
	
	L"%s's Waldtarnung ist nicht mehr effektiv.",
	L"%s's Waldtarnung wurde heruntergewaschen.",
	
	L"%s's Stadttarnung ist nicht mehr effektiv.",
	L"%s's Stadttarnung wurde heruntergewaschen.",
	
	L"%s's Schneetarnung ist nicht mehr effektiv.",
	L"%s's Schneetarnung wurde heruntergewaschen.",
	L"Sie können %s nicht an diesem Slot anbringen.",
};

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
	L"Üben", // training themselves
	L"Miliz", // training a town to revolt 
	L"M.Miliz", //training moving militia units		
	L"Trainer", // training a teammate
	L"Rekrut", // being trained by someone else 
	L"Betrieb", // operating a strategic facility
	L"Pause", // Resting at a facility
	L"Tot", // dead
	L"Koma", // abbreviation for incapacitated	//LOOTF - "Unfähig" klingt schlimm. Geändert auf Koma. Vorschläge?
	L"Gefangen", // Prisoner of war - captured
	L"Hospital", // patient in a hospital		
	L"Leer",	//Vehicle is empty
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
	L"Üben", 
	L"Miliz",
	L"Trainiere Mobile Miliz",
	L"Trainer",
	L"Rekrut",
	L"Betriebspersonal",
	L"Betriebspause",
	L"Tot",
	L"Koma",			//LOOTF - s.o.
	L"Gefangen",
	L"Hospital", 
	L"Leer",			// Vehicle is empty
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
	L"Üben", 
	L"Miliz",
	L"Trainiere Mobile",
	L"Trainer",
	L"Rekrut",
	L"Betriebspersonal",
	L"Betriebspause",
	L"Tot",
	L"Unfähig",
	L"Gefangen",
	L"Hospital", // patient in a hospital 
	L"Leer",	// Vehicle is empty
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
	L"Gewicht", // the weight the merc is carrying
	L"Tarnung", // the merc's camouflage rating
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
	L"Spr", // explosive skill
	L"Tec", // mechanical skill
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
	L"Patient", // the merc is receiving medical attention
	L"Fahrzeug", // the merc is in a vehicle
	L"Repar.", // the merc is repairing items 
	L"Training", // the merc is training
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
	L"In Deckung gehen",
	L"Alle: Angreifen", 
	L"Alle: Position halten",
	L"Alle: Rückzug",
	L"Alle: An meine Position",
	L"Alle: Ausschwärmen",
	L"Alle: Auf den Boden",
	L"Alle: In Deckung gehen",
	//L"All: Find items",
	L"Abbrechen",		// cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"Attack",		// set militia to aggresive
//	L"Hold Position",	// set militia to stationary
//	L"Retreat",		// retreat militia
//	L"Come to me",		// retreat militia
//	L"Get down",		// retreat militia		 
//	L"Cancel",		// cancel this menu
//};

STR16 pRemoveMercStrings[] ={
	L"Söldner entfernen", // remove dead merc from current team
	L"Abbrechen",
};

STR16 pAttributeMenuStrings[] =
{
	L"Kraft",
	L"Geschicklichkeit",
	L"Beweglichkeit",
	L"Gesundheit",
	L"Treffsicherheit",
	L"Medizin",
	L"Technik",
	L"Führungsqualität",
	L"Sprengstoffe",
	L"Abbrechen",
};

STR16 pTrainingMenuStrings[] =
{
 L"Üben", // train yourself 
 L"Miliz", // train the town 
 L"Mobile Miliz",
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
	L"Getarnt (Stadt)",						
	L"Getarnt (Wüste)",						
	L"Getarnt (Schnee)",						
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

	// second names for major skills
	L"MG-Veteran",
	L"Artillerist",
	L"Scharfschütze",
	L"Jäger",
	L"Revolverheld",
	L"Kampfsportler",
	L"Zugführer",
	L"Ingenieur",
	L"Arzt",
	
	L"Mehr...",
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

	L"PC-Kampf",									//STR_PB_AUTORESOLVE_BTN,
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
CHAR16		gWeaponStatsDesc[][ 17 ] =
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

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
// Please note, several of these are artificially inflated to 19 entries to help fix a complication with
// changing item type while watching its description box
STR16		gzWeaponStatsFasthelp[ 32 ] =
{
	L"Genauigkeit",
	L"Schaden",
	L"Reichweite",
	L"Zielgenauigkeit",
	L"Zielen-Modifikator",
	L"Durchschn. beste Laser-Reichweite",
	L"Mündungsfeuerdämpfung",
	L"Lautstärke (Niedriger ist besser)",
	L"Zuverlässigkeit",
	L"Reparaturleichtigkeit",
	L"Min. Reichweite für Zielen-Bonus",
	L"Treffer-Modifikator",
	L"",	//12
	L"APs zum Anlegen",
	L"APs für Einzelschuss",
	L"APs für Feuerstoß",
	L"APs für Autofeuer",
	L"APs zum Nachladen",
	L"APs zum manuellen Nachladen",
	L"",	//19
	L"Zweibein-Modifikator",
	L"Autofeuer: Schüsse je 5 AP",
	L"Burst/Auto-Streuung (Niedriger ist besser)",	//22
	L"APs zum Werfen",
	L"APs zum Abschießen",
	L"APs zum Stechen",
	L"Kein Einzelschuss!",
	L"Kein Feuerstoß!",
	L"Kein Autofeuer!",
	L"APs zum Schlagen",
	L"Autofeuer-Streuung (Niedriger ist besser)",
	L"Feuerstoß-Streuung (Niedriger ist besser)",
};

STR16		gzWeaponStatsFasthelpTactical[ 32 ] =
{
	L"Genauigkeit",
	L"Schaden",
	L"Reichweite",
	L"Zielgenauigkeit",
	L"Zielen-Modifikator",
	L"Durchschn. beste Laser-Reichweite",
	L"Mündungsfeuerdämpfung",
	L"Lautstärke (Niedriger ist besser)",
	L"Zuverlässigkeit",
	L"Reparaturleichtigkeit",
	L"Min. Reichweite für Zielen-Bonus",
	L"Treffer-Modifikator",
	L"",	//12
	L"APs zum Anlegen",
	L"APs für Einzelschuss",
	L"APs für Feuerstoß",
	L"APs für Autofeuer",
	L"APs zum Nachladen",
	L"APs zum manuellen Nachladen",
	L"",	//19
	L"Zweibein-Modifikator",
	L"Autofeuer: Schüsse je 5 AP",
	L"Burst/Auto-Streuung (Niedriger ist besser)",	//22
	L"APs zum Werfen",
	L"APs zum Abschießen",
	L"APs zum Stechen",
	L"Kein Einzelschuss!",
	L"Kein Feuerstoß!",
	L"Kein Autofeuer!",
	L"APs zum Schlagen",
	L"Autofeuer-Streuung (Niedriger ist besser)",
	L"Feuerstoß-Streuung (Niedriger ist besser)",
};

STR16		gzAmmoStatsFasthelp[ 20 ] =
{
	L"Rüstungsdurchschlag (Niedriger ist besser)",
	L"Schaden im Ziel (Höher ist besser)",
	L"Nahbereichs-Explosion (Höher ist besser)",
	L"Leuchtspur-Effekt",
	L"Panzerbrechend",
	L"Schlossbrecher",
	L"Ignoriert Rüstung",
	L"Säurehaltig",
	L"Reichweiten-Modifikator",
	L"Schaden-Modifikator",
	L"Treffer-Modifikator",
	L"Dauerfeuer-Streuungs-Modifikator (Höher ist besser)",
	L"Feuerstoß-Streuungs-Modifikator (Höher ist besser)",
	L"Zuverlässigkeits-Modifikator",
	L"Schalldämpfung (Niedriger ist besser)",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzArmorStatsFasthelp[ 20 ] =
{
	L"Schutz",
	L"Bedeckung (Höher ist besser)",
	L"Abnutzungsfaktor (Niedriger ist besser) ",
	L"AP-Modifikator",
	L"Treffer-Modifikator",
	L"Waldtarnung",
	L"Stadt-Tarnung",
	L"Wüstentarnung",
	L"Schneetarnung",
	L"Anschleichen-Modifikator",
	L"Sichtweiten-Modifikator",
	L"Tagsichtweiten-Modifikator",
	L"Nachtsichtweiten-Modifikator",
	L"Grelles-Licht-Modifikator",
	L"Höhlen-Sicht-Modifikator",
	L"Tunnelblick-Modifikator",
	L"Hörweiten-Modifikator",
	L"",
	L"",
	L"",
};

STR16		gzExplosiveStatsFasthelp[ 20 ] =
{
	L"Schaden",
	L"Betäubungsschaden",
	L"Detonationsgeräusch (Niedriger ist besser)",
	L"Flüchtigkeit!!! (Niedriger ist besser)",
	L"Detonationsbereich",
	L"Startausdehnung des Wirkbereichs",
	L"Endausdehnung des wirkbereichs",
	L"Wirkungsdauer",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
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
CHAR16 gMoneyStatsDesc[][ 13 ] =
{
	L"Betrag",
	L"Verbleibend:", //this is the overall balance
	L"Betrag",
	L"Abheben:",	// the amount he wants to separate from the overall balance to get two piles of money
	L"Konto",	
	L"Saldo",		
	L"Betrag",	
	L"abheben",	
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
	L"Kontra:",
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
	L"Eiscreme-Wagen",
	L"Jeep",
	L"Panzer",
	L"Helikopter",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Truck",
	L"Jeep",
	L"Tank",
	L"Heli", 			// the helicopter
};

STR16 zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Truck",		// Ice cream truck
	L"Jeep",
	L"Panzer",
	L"Heli", 		//an abbreviation for Helicopter
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

	L"%s wird eskortiert von Trupp %d.",

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
	L"Sektor verlassen",
	L"OK",
	L"Abbruch",
	L"Gewählter Söldner",
	L"Ganzer Trupp",
	L"Gehe zu Sektor",

	L"Gehe zu Karte", 
	
	L"Sie können den Sektor von dieser Seite aus nicht verlassen.",
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
	L"Daryl heiraten?",
	L"Schlüsselring",
	L"Das ist mit einem EPC nicht möglich.",
	L"Krott verschonen?",
	L"Außer Reichweite",
	L"Arbeiter",	//People that work in mines to extract precious metals
	L"Fahrzeug kann nur zwischen Sektoren fahren",
	L"Automatische Erste Hilfe nicht möglich",
	L"Weg blockiert für %s",
	L"Ihre von Deidrannas Truppe gefangenen Soldaten sind hier inhaftiert", 		
	L"Schloss getroffen",
	L"Schloss zerstört",
	L"Noch jemand an der Tür.",
	L"Gesundh.: %d/%d\nTank: %d/%d",
	L"%s kann %s nicht sehen.",		// Cannot see person trying to talk to
	L"Anbringung entfernt",
	L"Sie können kein weiteres Fahrzeug mehr verwenden, da Sie bereits 2 haben",
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
	L"Mobile M. Training",				// HEADROCK HAM 3.6: The stat of Mobile militia training in town
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
	L"Erzart",				// 10
	L"Kontrolle über Stadt",
	L"Loyalität der Stadt",
//	L"Minenarbeiter",
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
	L"Zeige Mobile Miliz Begrenzungen", // HEADROCK HAM 4: Mobile Restrictions Button
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
	L"Betriebskosten",			// HEADROCK HAM 3.6
	L"Unterhaltskosten für Miliz",		// HEADROCK HAM 3.6
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
	L"Söldner ist noch nicht in Arulco!",
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
	L"Soll %s seine Ausrüstung hier lassen (%s) oder in Drassen (B13), wenn er Arulco verlässt?",
	L"Soll %s seine Ausrüstung hier lassen (%s) oder in Omerta (A9), wenn er Arulco verlässt?",
	L"geht bald und lässt seine Ausrüstung in Omerta (A9).",
	L"geht bald und lässt seine Ausrüstung in Drassen (B13).",
	L"%s geht bald und lässt seine Ausrüstung in %s.",
};

// female version
STR16 pMercSheLeaveString[] =
{
	L"Soll %s ihre Ausrüstung hier lassen (%s) oder in Drassen (B13), bevor sie Arulco verlässt?",
	L"Soll %s ihre Ausrüstung hier lassen (%s) oder in Omerta (A9), bevor sie Arulco verlässt?",
	L"geht bald und lässt ihre Ausrüstung in Omerta (A9).",
	L"geht bald und lässt ihre Ausrüstung in Drassen (B13).",
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
	L"Sie können sich den B.S.E.-Charakter nicht leisten.",
	L"Der neue B.S.E.-Charakter ist nun in ihrem Team.",
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
	L"Slay ist gegangen",						//Slay is a merc and has left the team
	L"%s getötet",							//History log for when a merc kills an NPC or PC
};

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
	L"Bobby Rays - Letzte Lieferungen",
	L"",
	L"",
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
	L"Verstorben",	

	L"Sie versuchen, zu viele Söldner anzuheuern. Sie haben die Maximalanzahl erreicht.",
	L"nicht da",											
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
	L"Schwere W.",
	L"Pistolen",
	L"MPs",
	L"SMGs",
	L"Gewehre",
	L"SSGs",
	L"Sturmgew.",
	L"MGs",
	L"Schrotfl.",

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
	L"Preis:",			// Cost of the item
	L"Vorrätig:",			// The number of items still in the store's inventory
	L"Bestellt:",			// The number of items on order
	L"Beschädigt",			// If the item is damaged
	L"Gew.:",			// the Weight of the item
	L"Summe:",			// The total cost of all items on order
	L"* %% funktionstüchtig",	// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	L"Mist! Mit diesem Formular können Sie nur 10 Sachen bestellen. Wenn Sie mehr wollen (was wir sehr hoffen), füllen Sie bitte noch ein Formular aus.",

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
	L"Medizin",
	L"Sprengstoff",
	L"Technik",

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
	L"Medizin",
	L"Sprengstoff",
	L"Technik",

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
	//Text above the slider bars
	L"Effekte",
	L"Sprache",
	L"Musik",
	//Confirmation pop when the user selects..
	L"Spiel verlassen und zurück zum Hauptmenü?",
	L"Sprachoption oder Untertitel müssen aktiviert sein.",
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

	
	L"Gespeichertes Spiel in Slot #%d wirklich überschreiben?",
	L"Wollen Sie das Spiel aus Slot # speichern?",

	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	//
	L"Sie haben zu wenig Festplattenspeicher. Sie haben nur %d MB frei und JA2 benötigt mindestens %d MB.",
	
										
	L"Speichere...",	//While the game is saving this message appears.
	
	L"Normale Waffen",
	L"Zusatzwaffen",
	L"Real-Stil",
	L"SciFi-Stil",
	L"Schwierigkeit",
	L"Platinum Mode",	//Placeholder English
	L"Bobby Ray's",
	L"Normale Auswahl",
	L"Große Auswahl",
	L"Ausgezeichnete Auswahl",
	L"Fantastische Auswahl",

	L"Neues Inventar funktioniert nicht in 640x480 Auflösung. Wählen Sie eine andere Auflösung und versuchen Sie es erneut.",
	L"Neues Inventar funktioniert nicht mit dem ausgewählten 'Data' Ordner.",	
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
	L"Inventar auswählen.", 

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
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"Sie haben derzeit keine Mobile Miliz. Rekrutieren Sie einige Milizen und kehren Sie zurück zu dieser Ansicht.",
	L"Diese Ansicht zeigt Ihnen, wo Ihre Mobile Miliz ins feindliche Gebiet vorrücken kann. GRAU = Mobile Miliz verweigern hier vorzurücken. ROT = Mobile Miliz kann hier vorrücken, aber Sie haben den Befehl nicht gegeben. GELB = Mobile Miliz kann in den Sektor vorrücken, aber nicht verlassen. GRÜN = Mobile Miliz kann sich hier frei bewegen. Rechtsklick auf einen GRÜNEN / GELBEN Bereich um das Verhalten der Miliz zu ändern.",
};

STR16 pLandMarkInSectorString[] =
{
	L"Trupp %d hat in Sektor %s jemanden bemerkt.",
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
	L"Sie können sich keine $%d für mobile Miliz hier leisten.", // HEADROCK HAM 3.6: Mobile Militia
	L"Mobile Milizen in %s (%s %d) weitertrainieren?", // HEADROCK HAM 3.6: Mobile Militia
	L"Mobile Milizen in %d Sektoren zu trainieren kostet $ %d. %s", // HEADROCK HAM 3.6: Mobile Militia
	L"Eine mobile Milizeinheit für diese Stadt zu trainieren kostet $", // HEADROCK HAM 3.6: Mobile Militia
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
	L"Meinen Cursor nicht bewegen!",
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
	L"Drahtgitter zeigen",
	L"3D Cursor zeigen",
	L"Trefferchance anzeigen",
	L"GL Burst mit Burst Cursor",
	L"Gegner-Spott aktiveren",		// Changed from "Enemies Drop all Items" - SANDRO
	L"Hohe Granatwerferflugbahn",
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
	L"Niedrige CPU Belastung",
	L"Erw. Gegenstandsinfo (EDB)",
	L"Erzwungener Runden-Modus",			// add forced turn mode
	L"Status Trainingsfortschritt",			// Show progress towards stat increase
	L"Alternatives Kartenfarbschema",		// Change color scheme of Strategic Map
	L"Alternative Projektilgrafik",			// Show alternate bullet graphics (tracers)
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
STR16 zOptionsScreenHelpText[] =
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
	L"Diese Funktion ANschalten, um die geplanten Wege der Söldner zum Cursor anzuzeigen\n(oder abgeschaltet lassen und bei gewünschter Anzeige die |S|H|I|F|T-Taste drücken).",

	//show misses
	L"Wenn diese Funktion aktiviert ist, folgt die Spielkamera im Rundenmodus der Geschossflugbahn bis zu ihrem Ende. Ausschalten um das Spiel zu beschleunigen.",

	//Real Time Confirmation
	L"Wenn diese Funktion aktiviert ist, wird für jede Aktion im Echtzeit-Modus ein zusätzlicher \"Sicherheits\"-Klick verlangt um versehentliche Befehle zu vermeiden.",

	//Sleep/Wake notification
	L"Schalten Sie diese Option aus, wenn Sie kein Popup erhalten wollen, sobald zu einem Dienst eingeteilte Söldner schlafen gehen oder die Arbeit wieder aufnehmen.",

	//Use the metric system
	L"Mit dieser Option wird im Spiel das metrische anstelle des imperialen Maßsystems verwendet (z.B. Meter und Kilogramm).",

	//Merc Lighted movement
	L"Diese Funktion beleuchtet für den Spieler die Umgebung des Söldners - auch beim Bewegen. AUSgeschaltet erhöht sich die Bildwiederholrate.",

	//Smart cursor
	L"Wenn diese Funktion aktiviert ist, springt der Cursor immer automatisch auf Söldner in seiner direkten Nähe.",

	//snap cursor to the door
	L"Wenn diese Funktion aktiviert ist, springt der Cursor automatisch auf Türen in direkter Nähe des Mauszeigers.",

	//glow items
	L"Wenn diese Funktion aktiviert ist, haben Gegenstände am Boden zur besseren Sichtbarkeit einen pulsierenden Rahmen (|I).",

	//toggle tree tops
	L"Mit der Deaktivierung dieser Funktion lassen sich Baumkronen ausblenden um bessere Sicht auf das Geschehen zu ermöglichen (|T).",

	//toggle wireframe
	L"Wenn diese Funktion aktiviert ist, werden Drahtgitter verborgener Wände gezeigt um z.B. perspektivisch verdeckte Fenster zu erkennen (|W).",

	L"Wenn diese Funktion aktiviert ist, wird der Bewegungs-Cursor in 3D angezeigt (|Home).",
	
	// Options for 1.13
	L"Wenn diese Funktion aktiviert ist, wird die Trefferwahrscheinlichkeit am Cursor angezeigt.",
	L"Mit dieser Funktion lässt sich der Zielcursor für Granatwerfer-Feuerstöße umschalten. Der Burst-Cursor (wenn AN) ermöglicht den Beschuss einer größeren Fläche.",
	L"Wenn diese Funktion aktiviert ist, beschimpfen Gegner den Spieler oder kommentieren ihre Situation mittels kleiner Pop-Ups.", // Changed from Enemies Drop All Items - SANDRO
	L"Wenn diese Funktion aktiviert ist, können Granatwerfer Granaten in höherem Winkel abfeuern und so ihre volle Reichweite ausnutzen (|Q).",
	L"Wenn diese Funktion aktiviert ist, schaltet das Spiel für unbemerkt schleichende Söldner nicht automatisch in den Rundenmodus sobald Gegner in Sicht geraten, außer Sie drücken |S|t|r|g+|X.", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Wenn diese Funktion aktiviert ist, selektiert |S|p|a|c|e automatisch die nächste Gruppe statt den nächsten Söldner der Gruppe.",
	L"Wenn diese Funktion aktiviert ist, werfen Gegenstände einen Schatten.",
	L"Wenn diese Funktion aktiviert ist, werden Waffenreichweiten in Feldern angezeigt statt in z.B. Metern.",
	L"Wenn diese Funktion aktiviert ist, wird auch für Einzelschüsse mit Leuchtspurmunition der grafische Effekt dazu angezeigt.",
	L"Wenn diese Funktion aktiviert ist, werden Regengeräusche hörbar, sobald es regnet.",
	L"Wenn diese Funktion aktiviert ist, sind Krähen im Spiel vorhanden und hacken lautstark an manchen Leichen herum, haben aber sonst keine großen Auswirkungen auf das Spiel.",
	L"Wenn diese Funktion aktiviert ist, werden mit Druck auf |A|l|t Informationen über den Gegner eingeblendet, der unter dem Mauszeiger steht. Hinweis: Wenn nicht anders eingestellt, muss dieser in guter Sichtweite eigener Truppen sein.",
	L"Wenn diese Funktion aktiviert ist, wird nach jeder Runde automatisch in einen speziellen Autosave-Spielstand gespeichert.",
	L"Wenn diese Funktion aktiviert ist, wird Skyrider nichts mehr sagen. Verwenden Sie diese Option, wenn er Ihnen auf die Nüsse geht.",
	L"Aktivieren Sie diese Funktion, wenn Ihr Rechner steinalt ist und anderenfalls Probleme auftreten würden.",
	L"Wenn diese Funktion aktiviert ist, werden erweiterte Beschreibungen und Werte zu den Waffen und Gegenständen angezeigt.",
	L"Wenn diese Funktion aktiviert ist und noch Gegner im Sektor sind, bleibt das Spiel im Runden-Modus, bis alle Feinde tot sind (|C|T|R|L+|S|H|I|F|T+|A|L|T+|T).",
	L"Wenn diese Funktion aktiviert ist, werden die Söldnerwerte visuell mit ihrem Trainingsfortschritt unterlegt.",
	L"Wenn diese Funktion aktiviert ist, wird die Strategische Karte entsprechend Ihres Erkundungsfortschrittes unterschiedlich eingefärbt.",
	L"Wenn diese Funktion aktiviert ist, werden geschossene Projektile visuell mit Tracereffekten dargestellt.",
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
	L"Spielmodus",
	L"Realistisch",
	L"SciFi",
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
	L"Extraschwer (Speichern erlauben)",
	L"Jederzeit",
	L"Nicht im Kampf",
	L"Option nicht verfügbar", 
	L"Bobby Ray's Auswahl",
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
	L"Anzahl der Terroristen",
	L"Zufällig",
	L"Alle vorhanden",
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
	L"Sie müssen eine gültie Server-IP-Adresse eingeben.\n (Bsp: 84.114.195.239).",
	L"Sie müssen eine gültige Server-Portnummer zwischen 1 und 65535 eingeben.",
};

STR16 gzMPJHelpText[] =
{
	L"Besuchen Sie http://webchat.quakenet.org/?channels=ja2-multiplayer um sich mit anderen Spielern zu treffen.",

	L"ERÖFFNEN",
	L"Geben Sie '127.0.0.1' für die IP Adresse ein. Die Port Nummer sollte größer als 60000 sein.",
	L"Vergewissern Sie sich, dass das Port (UDP, TCP) auf dem Router weitergeleitet wird. Siehe: http://portforward.com",
	L"Sie müssen Ihre externe IP (http://www.whatismyip.com) und die Port Nummmer an die anderen Spieler schicken (via IRC, ICQ, etc.).",
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
	L"Kooperativ (CO-OP)",
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
	L"Spielstand",				//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",					//The 3 character dos extension (represents sav)
	L"..\\Spielstände",			//The name of the directory where games are saved.
	L"Tag",
	L"Söldner",
	L"Leerer Slot",				//An empty save game slot
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
	L"Leerer Quick-Save-Slot",
	L"Dieser Slot ist für Quick-Saves aus dem Karten- oder Taktik-Bildschirm reserviert. Speichern mit ALT+S.",
	L"offen",
	L"zu",
	L"Ihr Festplattenspeicher ist knapp. Sie haben lediglich %sMB frei und Jagged Alliance 2 v1.13 benötigt %sMB.",
	L"%s von AIM angeheuert", 
	L"%s hat %s gefangen.",			//'Merc name' has caught 'item' -- let SirTech know if name comes after item.

	L"%s hat die Droge genommen.",			//'Merc name' has taken the drug
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
	L"Spielstand namens Spielstand99.sav kreiert. Wenn nötig, in Spielstand01 - Spielstand10 umbennen und über die Option 'Laden' aufrufen.",
	L"%s hat %s getrunken.",
	L"Paket in Drassen angekommen.",
	L"%s kommt am %d. um ca. %s am Zielort an (Sektor %s).", //first %s is mercs name(OK), next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival       !!!7 It should be like this: first one is merc (OK), next is day of arrival (OK) , next is time of the day for ex. 07:00 (not OK, now it is still sector), next should be sector (not OK, now it is still time of the day)	//LOOTF - is this still valid? I assume it's not.
	L"Logbuch aktualisiert.",
	L"Granatenwerfer-Feuerstöße verwenden Ziel-Cursor (Sperrfeuer aktiviert).",
	L"Granatenwerfer-Feuerstöße verwenden Flugbahn-Cursor (Sperrfeuer deaktiviert).",
	L"Soldaten-Kurzinfo (\"Tooltips\") aktiviert",		// Changed from Drop All On - SANDRO
	L"Soldaten-Kurzinfo (\"Tooltips\") deaktiviert",	// Changed from Drop All Off - SANDRO
	L"Granatwerfer schießen in flachem Winkel.",
	L"Granatwerfer schießen in steilem Winkel.",
	// forced turn mode strings
	L"Erzwungener Rundenmodus",
	L"Normaler Rundenmodus",
	L"Verlasse Kampfmodus",
	L"Erzwungener Rundenmodus ist aktiv, gehe in Kampfmodus",
#ifdef JA2BETAVERSION
	L"Spiel erfolgreich im Slot End Turn Auto Save gespeichert.",	// 83
#endif
	L"..\\Spielstände\\MP_Spielstände",			//The name of the directory where games are saved.
	L"Client",
	
	L"Sie können nicht altes Inventar und neues Attachment System gleichzeitig verwenden.",
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
	L"Grüne Miliz aufnehmen(Rechtsklick)/absetzen(Linksklick)", // button help text informing player they can pick up or drop militia with this button
	L"Reguläre Milizen aufnehmen(Rechtsklick)/absetzen(Linksklick)",
	L"Elitemilizen aufnehmen(Rechtsklick)/absetzen(Linksklick)",
	L"Milizen gleichmäßig über alle Sektoren verteilen",
};

STR16 pMapScreenJustStartedHelpText[] = 
{
	L"Zu AIM gehen und Söldner anheuern ( *Tip*: Befindet sich im Laptop )", // to inform the player to hire some mercs to get things going
	L"Sobald Sie für die Reise nach Arulco bereit sind, klicken Sie auf den Zeitraffer-Button unten rechts auf dem Bildschirm.", // to inform the player to hit time compression to get the game underway
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
};

STR16 gzHelpScreenText[] =
{
	L"Helpscreen verlassen",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Es tobt eine Schlacht. Sie können sich nur im Taktikbildschirm zurückziehen.",
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
	L"%s hört eine EXPLOSION (%s) von %s.", 
	L"%s hört einen SCHREI (%s) von %s.",
	L"%s hört einen AUFSCHLAG (%s) von %s.",
	L"%s hört einen AUFSCHLAG (%s) von %s.",
	L"%s hört ein ZERBRECHEN (%s) von %s.",
	L"%s hört ein ZERSCHMETTERN (%s) von %s.",
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
	L"%s hat seine eigenen Gegenstände repariert",
	L"%s hat die Waffen und Rüstungen aller Teammitglieder repariert",
	L"%s hat die aktivierten Gegenstände aller Teammitglieder repariert",
	L"%s hat die großen mitgeführten Gegenstände aller Teammitglieder repariert",
	L"%s hat die mittelgroßen mitgeführten Gegenstände aller Teammitglieder repariert",
	L"%s hat die kleinen mitgeführten Gegenstände aller Teammitglieder repariert",
	L"%s hat die Trageausrüstung aller Teammitglieder repariert",
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
	L"Arulco",
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
	L"%s ist absolut unfähig dazu, irgend etwas zu reparieren!",

	//44
	L"Repar. Zeit",
	L"%s kann diese Person nicht sehen.", 

	//46-48
	L"%ss Gewehrlauf-Verlängerung fällt ab!",
	L"Es sind nicht mehr als %d Milizenausbilder in diesem Sektor erlaubt.",
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
	// Full info
	L"%s|Helm: %s\n",
	L"%s|Weste: %s\n",
	L"%s|Hose: %s\n",
	// Limited, Basic
	L"|Rüstung: ",
	L"Helm ",
	L"Weste ",
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
	L"%s wurde erfolgreich operiert.",
	L"%s ist am Torso getroffen und verliert einen Punkt maximaler Gesundheit!",
	L"%s ist am Torso getroffen und verliert %d Punkte maximaler Gesundheit!",
	L"%s hat einen Punkt an %s wiedergewonnen."
	L"%s hat %d Punkte an %s wiedergewonnen.",
	L"Ihre Späher-Fertigkeit hat Sie davor bewahrt, vom Gegner in einen Hinterhalt gelockt zu werden.",
	L"Dank Ihrer Späher-Fertigkeit haben Sie erfolgreich ein Rudel Bloodcats umgangen.",
	L"%s wurde in die Leiste getroffen und windet sich in Schmerzen!",
	//////////////////////////////////////////////////////////////////////////////////////
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
	L"Vielen Dank für Ihre Kontaktaufnahme. Sagen Sie mir bescheid, wenn die nächste Party steigen kann. Ab sofort erreichen Sie mich über die AIM page. ± ± Lennart Vilde.",

	// Henning
	L"Weitergeleitet von AIM-Server: Nachricht von Henning von Branitz",
	L"Ihre Nachricht hat mich erreicht, vielen Dank. Falls Sie mich engagieren möchten, kontaktieren Sie mich über die AIM Website. ± ± Bis die Tage! ± ± Henning von Branitz ±",

	// Luc
	L"Weitergeleitet von AIM-Server: Nachricht von Luc Fabre",
	L"Ich habe Ihre Nachricht erhalten, merci. Zur Zeit könnte ich gerne einen Auftrag annehmen. Sie wissen ja, wo Sie mich erreichen. ± ± Sicher hören wir bald von einander ±",

	// Laura
	L"Weitergeleitet von AIM-Server: Nachricht von Dr. Laura Colin",
	L"Ich grüße Sie! Schön, dass Sie mir eine Nachricht hinterlassen haben. Es hörte sich interessant an. ± ± Wenn Sie wieder bei AIM vorbeischauen, würde ich mich freuen, von Ihnen zu hören. ± ± Noch viel Erfolg! ± ± Dr. Laura Colin ±",

	// Grace
	L"Weitergeleitet von AIM-Server: Nachricht von Graziella Girelli",
	L"Sie wollten mich kontaktieren, aber ich war leider nicht zu erreichen.± ± Ein Familientreffen. Sie kennen das ja sicher... Jetzt hab' ich erst mal wieder genug von Familie.± ± Jedenfalls freue ich mich, wenn Sie sich auf der AIM Site mit mir in Verbindung setzen. Ciao! ±",

	// Rudolf
	L"Weitergeleitet von AIM-Server: Nachricht von Rudolf Steiger",
	L"Wissen Sie eigentlich, wieviel Anrufe ich jeden Tag kriege? Jeder Pisser meint, er müsste hier anrufen.  ± ± Aber gut, ich bin jetzt wieder da. Falls Sie einen interessanten Auftrag haben. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	//L"Scharfschütze: Sie haben Augen wie ein Falke. Dadurch können sie sogar auf die Flügel einer Fliege aus hunderten von Metern schießen! ± ",
	//LOOTF - ersetzt mit:
	L"Scharfschütze: Sie haben einen scharfen Blick und eine ruhige Hand, neben Ihrem guten Gespür für die Windrichtung die perfekten Voraussetzungen für das Präzisionsschießen auf große Entfernung oder eine Partie 'Jenga'. Gut so, bleiben Sie auf Distanz, so ersparen Sie sich spätere medizinische Betreuung. ± "

	// Camouflage
	//L"Tarnung: Neben Ihnen schauen sogar Büsche synthetisch aus! ± ",
	//LOOTF - ersetzt mit:
	L"Tarnung: Sie haben das Überkleistern des eigenen Gesichts mit Tarnfarbe zu einer Kunstform erhoben und erschaffen aus Zwang durchweg wundervolle Illusionen der umgebenden Flora auf Ihrem eher langweiligen Antlitz. Dadurch wirken Sie im örtlichen ALDI wie ein Wunderling, sind aber als Heckenschütze im Dschungel nahezu unsichtbar. ± "

	// SANDRO - new strings for new traits added
	// Ranger
	L"Jäger: Vermutlich lesen Sie diese Nachricht auf einem tragbaren Gerät von einem Dschungel eines Dritte-Welt-Lands aus und ich ahne, dass Sie dabei nicht einmal aufrecht sitzen. Sie haben eine bemerkenswerte Affinität zu schwer passierbarem Gelände und Ihre unermüdlichen Beine tragen Sie im Handumdrehen über Stock und Stein. Sie scheuen sich auch nicht vor Dreck und so passen Sie sich mehr als alle anderen an Ihre Umgebung an. Setzen Sie diese Qualitäten als Führer ein um Ihr Team auf bestem Wege ans Ziel zu bringen! ± ",	
	// Gunslinger
	L"Revolverheld: Ob es an ihrer handlichen Effizienz im Nahkampf liegt oder einfach daran, dass sonst niemand sonst so gut mit ihnen umgehen kann, aber Sie lieben Faustfeuerwaffen, das ist klar. Sie beweisen enormes Talent im Umgang mit Pistolen und Revolvern aller Art und in Ihrem Beruf ist das nicht das Schlechteste. Passen Sie nur auf, dass Sie neben all dem Pistolenrumwirbeln und den coolen Magazintricks auch noch an Ihre Nächsten denken, falls nach der Waffe noch jemand kommt.  ± ",
	// Squadleader
	L"Zugführer: Normalerweise würde ich Sie ja wie einen gewöhnlichen (wenn auch den bisherig besten) Kunden behandeln, aber ich sehe ein wenig zu Ihnen auf. Die Rhetorik in Ihrem Fragebogen hat einige hier ganz schön beeindruckt und Ihre generelle Erscheinung motiviert einfach. Das enorme Wissen über die Kunst der Kriegsführung, das Sie sich angeeignet haben, kommt den Ihnen unterstellten Männern und Frauen sicher sehr zugute. In Ihrer Nähe kann eigentlich nichts schiefgehen. ± ",
	// Technician
	L"Ingenieur: Im Rahmen der Söldnerevaluierung führen wir einen generellen Schnelltest auf handwerkliche und technische Befähigung durch und können dabei offenbar nicht die ganze Palette menschlichen Könnens abdecken. Die Bilder des von Ihnen gebauten reaktorbetriebenen, dreibeinigen Rasenmähers, die Sie auf unsere Server geladen haben, fanden wir sehr beeindruckend. Wir wissen also, dass Sie mit Hufeisen und altem Garn so gut wie alles reparieren können, aber bitte nutzen Sie demnächst unsere Webpräsenz, statt sich direkt in unser System zu hacken. ± ",
	// Doctor
	L"Arzt: Ärzte wie Sie braucht das Land! Es sind immer die besten, die sich in den Krieg stürzen, aber womöglich können Sie in rauchenden Trümmern mehr bewirken als in einem heruntergewirtschafteten und von fetter Dekadenz gezeichneten Gesundheitssystem. Gibt man Ihnen die Ausrüstung, können Sie auf einem Strohbett Schusswunden operieren und Kranke heilen wie ein junger Jesus. Sollten Sie zurückkehren, es wartet immer eine Beschäftigung auf Sie! ± ",
	// Athletics
	L"Läufer: Am Anfang waren wir besorgt, Sie wären der erste beinlose Kunde, der sich ins Ausland wagt, doch offenbar waren die Testwerte einfach nur außerhalb des Berechnungsrahmens, also Glückwunsch, hier sind Sie Rekordhalter. So schnell und ausdauernd wie Sie rennen, möchte ich annehmen, Sie sind mit dem Wort Marathon vertraut. Ich schlage vor, wenn es Ärger gibt, laufen Sie einfach! Einholen wird Sie eh niemand. ± ",
	// Bodybuilding
	L"Kraftsportler: Manche meinen ja, es ist eine abscheuliche Form der Körperkunst, aber Sie scheinen damit recht gut leben zu können. Durch Ihr intensives Muskeltraining fällt Ihnen das Schleppen schwerer Gerätschaften garantiert leicht und so einen Typen wie Sie kriegt man vermutlich nicht einmal mit dem Brecheisen auf die Knie gezwungen. Und hey, ganz so schlimm wie die EU-Verfassung sehen Sie nun auch nicht aus. ± ",
	// Demolitions
	L"Sprengmeister: Unsere fachmännischen Psychoanalysten meinen, Sie sind als Kind nicht oft genug oder gar nicht von Ihren Eltern daran erinnert worden, dass Messer, Gabel, Plastiksprengstoff und Licht für kleine Kinder nicht sind. Machen Sie aus Ihrem verschobenen Weltbild einfach das Beste und nutzen Sie Ihre Begeisterung für alles, was mit mehrfacher Schallgeschwindigkeit expandiert um sich im Training mit Granaten und Sprengstoffen hervorzutun. ± ",
	// Scouting
	L"Aufklärer: Sie sind über die Maßen aufmerksam, haben ein sehr reges Auge und einen nimmermüden Geist. Ihnen entgeht nicht das geringste Detail, vor allem nicht, wenn Sie mit Ihrem alten Feldstecher unterwegs sind. Ihre Präsenz zahlt sich aus, denn durch die Anzeichen nahender Bedrohung, die nur Sie erkennen, können Sie Ihre Liebsten oder Ihre Kameraden vor Hinterhalten bewahren. Waren Sie mal bei der Stasi?  ± ",	
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
	L"Starte Spielzug für Client Nummer: #1 - '%S' | #2 - '%S' | #3 - '%S' | #4 - '%S'",
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
	L"%s's Sprengstoff wurde von &s entschärft",
	L"Sie haben verloren. Was für eine Schande",	// All over red rover
	L"Überwachungsmodus wurde ausgeschaltet",
	L"Welcher Client soll vom Spiel entfernt werden:",
	// 75
	L"Team #%d wurde vernichtet",
	L"Client konnte nicht gestartet werden. Beendigung.",
	L"Client Verbindung aufgelöst und heruntergefahren.",
	L"Client läuft nicht.",
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"S",
	L"O",
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
	L"Chat: Drücke 'ENTER' zum Senden oder 'ESC' zum Verlassen.",
};

// Following strings added - SANDRO
// Translated by Scheinworld
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"Auf der nächsten Seite können Sie Ihre Fertigkeiten entsprechend Ihrer Spezialisierung als Söldner festlegen. Es können nicht mehr als zwei verschiedene Fertigkeiten oder eine Expertenfertigkeit gewählt werden.",
	L"Sie können auch nur eine oder gar keine Fertigkeit auswählen. Sie erhalten dafür einen Bonus zu Ihren Attributpunkten als Gegenleistung. Beachten Sie, dass die Fertigkeiten 'Elektronik', 'Beidhändig geschickt' und 'Getarnt' keine Experten-Spezialisierung erhalten.",
	// For new major/minor traits
	L"Auf der nächsten Seite können Sie Ihre Fertigkeiten entsprechend Ihrer Spezialisierung festlegen. Auf der ersten Seite können Sie bis zu zwei Hauptfertigkeiten auswählen, die Ihre Rolle in einem Team repräsentieren, während Sie auf der zweiten Seite eine Liste der möglichen Nebenfertigkeiten finden.",
	L"Es können nicht mehr als insgesamt drei Fertigkeiten gewählt werden, was bedeutet, dass, wenn Sie keine Hauptfertigkeiten angeben, dafür drei Nebenfertigkeiten verfügbar sind.",
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
	L"Ihre größte Schwäche",
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
	L"%s wurde veletzt in Sektor %s.", // <--- This is a log message string.
	// 31 - 35
	L"%s wurde ernsthaft im Sektor %s verletzt.", //<--- This is a log message string.


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
};

STR16 Additional113Text[]=
{
	L"Für die korrekte Arbeit im Fenster-Modus benötigt Jagged Alliance 2 v1.13 16-bit Farbmodus.", //Jagged Alliance 2 v1.13 windowed mode requires a color depth of 16bpp or less.
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
	L"|G|e|n|e|r|a|l |P|r|o|p|e|r|t|i|e|s |P|a|g|e:\n \nShows specific data about this item.",
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
	L"|A|l|l|o|w|e|d |A|i|m|i|n|g |L|e|v|e|l|s",
	L"|S|c|o|p|e |M|a|g|n|i|f|i|c|a|t|i|o|n |F|a|c|t|o|r",
	L"|P|r|o|j|e|c|t|i|o|n |F|a|c|t|o|r",
	L"|H|i|d|d|e|n |M|u|z|z|l|e |F|l|a|s|h",
	L"|L|o|u|d|n|e|s|s",
	L"|R|e|l|i|a|b|i|l|i|t|y",
	L"|R|e|p|a|i|r |E|a|s|e",
	L"|M|i|n|. |R|a|n|g|e |f|o|r |A|i|m|i|n|g |B|o|n|u|s",
	L"|T|o|-|H|i|t |M|o|d|i|f|i|e|r",
	L"", // (12)
	L"|A|P|s |t|o |R|e|a|d|y",
	L"|A|P|s |t|o |A|t|t|a|c|k",
	L"|A|P|s |t|o |B|u|r|s|t",
	L"|A|P|s |t|o |A|u|t|o|f|i|r|e",
	L"|A|P|s |t|o |R|e|l|o|a|d",
	L"|A|P|s |t|o |R|e|c|h|a|m|b|e|r",
	L"|L|a|t|e|r|a|l |R|e|c|o|i|l",
	L"|V|e|r|t|i|c|a|l |R|e|c|o|i|l",
	L"|A|u|t|o|f|i|r|e |B|u|l|l|e|t|s |p|e|r |5 |A|P|s",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines whether bullets fired by\nthis gun will stray far from where\nit is pointed.\n \nScale: 0-100.\nHigher is better.",
	L"\n \nDetermines the average amount of damage done\nby bullets fired from this weapon, before\ntaking into account armor or armor-penetration.\n \nHigher is better.",
	L"\n \nThe maximum distance (in tiles) that\nbullets fired from this gun will travel\nbefore they begin dropping towards the\nground.\n \nHigher is better.",
	L"\n \nThis is the number of Extra Aiming\nLevels you can add when aiming this gun.\n \nThe FEWER aiming levels are allowed, the MORE\nbonus each aiming level gives you. Therefore,\nhaving FEWER levels makes the gun faster to aim,\nwithout making it any less accurate.\n \nLower is better.",
	L"\n \nWhen greater than 1.0, will proportionally reduce\naiming errors at a distance.\n \nRemember that high scope magnification is detrimental\nwhen the target is too close!\n \nA value of 1.0 means no scope is installed.",
	L"\n \nProportionally reduces aiming errors at a distance.\n \nThis effect works up to a given distance,\nthen begins to dissipate and eventually\ndisappears at sufficient range.\n \nHigher is better.",
	L"\n \nWhen this property is in effect, the weapon\nproduces no visible flash when firing.\n \nEnemies will not be able to spot you\njust by your muzzle flash (but they\nmight still HEAR you).",
	L"\n \nWhen firing this weapon, Loudness is the\ndistance (in tiles) that the sound of\ngunfire will travel.\n \nEnemies within this distance will probably\nhear the shot.\n \nLower is better.",
	L"\n \nDetermines how quickly this weapon will degrade\nwith use.\n \nHigher is better.",
	L"\n \nDetermines how difficult it is to repair this weapon.\n \nHigher is better.",
	L"\n \nThe minimum range at which a scope can provide it's aimBonus.",
	L"\n \nTo hit modifier granted by laser sights.",
	L"", // (12)
	L"\n \nThe number of APs required to bring this\nweapon up to firing stance.\n \nOnce the weapon is raised, you may fire repeatedly\nwithout paying this cost again.\n \nA weapon is automatically 'Unreadied' if its\nwielder performs any action other than\nfiring or turning.\n \nLower is better.",
	L"\n \nThe number of APs required to perform\na single attack with this weapon.\n \nFor guns, this is the cost of firing\na single shot without extra aiming.\n \nIf this icon is greyed-out, single-shots\n are not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\na burst.\n \nThe number of bullets fired in each burst is\ndetermined by the weapon itself, and indicated\nby the number of bullets shown on this icon.\n \nIf this icon is greyed-out, burst fire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to fire\nan Autofire Volley of three bullets.\n \nIf you wish to fire more than 3 bullets,\nyou will need to pay extra APs.\n \nIf this icon is greyed-out, autofire\nis not possible with this weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to reload\nthis weapon.\n \nLower is better.",
	L"\n \nThe number of APs required to rechamber this weapon\nbetween each and every shot fired.\n \nLower is better.",
	L"\n \nThe distance this weapon's muzzle will shift\nhorizontally between each and every bullet in a\nburst or autofire volley.\n \nPositive numbers indicate shifting to the right.\nNegative numbers indicate shifting to the left.\n \nCloser to 0 is better.",
	L"\n \nThe distance this weapon's muzzle will shift\nvertically between each and every bullet in a\nburst or autofire volley.\n \nPositive numbers indicate shifting upwards.\nNegative numbers indicate shifting downwards.\n \nCloser to 0 is better.",
	L"\n \nIndicates the number of bullets that will be added\nto an autofire volley for every extra 5 APs\nyou spend.\n \nHigher is better.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|P|r|o|t|e|c|t|i|o|n |V|a|l|u|e",
	L"|C|o|v|e|r|a|g|e",
	L"|D|e|g|r|a|d|e |R|a|t|e",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nThis primary armor property defines how much\ndamage the armor will absorb from any attack.\n \nRemember that armor-piercing attacks and\nvarious randomal factors may alter the\nfinal damage reduction.\n \nHigher is better.",
	L"\n \nDetermines how much of the protected\nbodypart is covered by the armor.\n \nIf coverage is below 100%, attacks have\na certain chance of bypassing the armor\ncompletely, causing maximum damage\nto the protected bodypart.\n \nHigher is better.",
	L"\n \nIndicates how quickly this armor's condition\ndrops when it is struck, proportional to\nthe damage caused by the attack.\n \nLower is better.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|A|r|m|o|r |P|i|e|r|c|i|n|g",
	L"|B|u|l|l|e|t |T|u|m|b|l|e",
	L"|P|r|e|-|I|m|p|a|c|t |E|x|p|l|o|s|i|o|n",
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nThis is the bullet's ability to penetrate\na target's armor.\n \nWhen above 1.0, the bullet proportionally\nreduces the Protection value of any\narmor it hits.\n \nWhen below 1.0, the bullet increases the\nprotection value of the armor instead.\n \nHigher is better.",
	L"\n \nDetermines a proportional increase of damage\npotential once the bullet gets through the\ntarget's armor and hits the bodypart behind it.\n \nWhen above 1.0, the bullet's damage\nincreases after penetrating the armor.\n \nWhen below 1.0, the bullet's damage\npotential decreases after passing through armor.\n \nHigher is better.",
	L"\n \nA multiplier to the bullet's damage potential\nthat is applied immediately before hitting the\ntarget.\n \nValues above 1.0 indicate an increase in damage,\nvalues below 1.0 indicate a decrease.\n \nHigher is better.",
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|D|a|m|a|g|e",
	L"|S|t|u|n |D|a|m|a|g|e",
	L"|B|l|a|s|t |R|a|d|i|u|s",
	L"|S|t|u|n |B|l|a|s|t |R|a|d|i|u|s",
	L"|N|o|i|s|e |B|l|a|s|t |R|a|d|i|u|s",
	L"|T|e|a|r|g|a|s |S|t|a|r|t |R|a|d|i|u|s",
	L"|M|u|s|t|a|r|d |G|a|s |S|t|a|r|t |R|a|d|i|u|s",
	L"|L|i|g|h|t |S|t|a|r|t |R|a|d|i|u|s",
	L"|S|m|o|k|e |S|t|a|r|t |R|a|d|i|u|s",
	L"|I|n|c|e|n|d|i|a|r|y |S|t|a|r|t |R|a|d|i|u|s",
	L"|T|e|a|r|g|a|s |E|n|d |R|a|d|i|u|s",
	L"|L|i|g|h|t |E|n|d |R|a|d|i|u|s",
	L"|M|u|s|t|a|r|d |G|a|s |E|n|d |R|a|d|i|u|s",
	L"|S|m|o|k|e |E|n|d |R|a|d|i|u|s",
	L"|I|n|c|e|n|d|i|a|r|y |E|n|d |R|a|d|i|u|s",
	L"|E|f|f|e|c|t |D|u|r|a|t|i|o|n",
	L"|L|o|u|d|n|e|s|s",
	L"|V|o|l|a|t|i|l|i|t|y",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nThe amount of damage caused by this explosive.\n \nNote that blast-type explosives deliver this damage\nonly once (when they go off), while prolonged effect\nexplosives deliver this amount of damage every turn until the\neffect dissipates.\n \nHigher is better.",
	L"\n \nThe amount of non-lethal (stun) damage caused\nby this explosive.\n \nNote that blast-type explosives deliver their damage\nonly once (when they go off), while prolonged effect\nexplosives deliver this amount of stun damage every\nturn until the effect dissipates.\n \nHigher is better.",
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
	L"\n \nThis is the distance (in Tiles) within which\nsoldiers and mercs will hear the explosion when\nit goes off.\n \nEnemies hearing the explosion will be alerted to your\npresence.\n \nLower is better.",
	L"\n \nThis value represents a chance (out of 100) for this\nexplosive to spontaneously explode whenever it is damaged\n(for instance, when other explosions go off nearby).\n \nCarrying highly-volatile explosives into combat\nis therefore extremely risky and should be avoided.\n \nScale: 0-100.\nLower is better.",
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
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \nThis ammo creates a tracer effect when firedin\nfull-auto or burst mode.\n \nTracer fire helps keep the volley accurate\nand thus deadly despite the gun's recoil.\n \nAlso, tracer bullets create paths of light that\ncan reveal a target in darkness. However, they\nalso reveal the shooter to the enemy!\n \nTracer Bullets automatically disable any\nMuzzle Flash Suppression items installed on the\nsame weapon.",
	L"\n \nThis ammo can damage the armor on a tank.\n \nAmmo WITHOUT this property will do no damage\nat all to tanks.\n \nEven with this property, remember that most guns\ndon't cause enough damage anyway, so don't\nexpect too much.",
	L"\n \nThis ammo ignores armor completely.\n \nWhen fired at an armored target, it will behave\nas though the target is completely unarmored,\nand thus transfer all its damage potential to the target!",
	L"\n \nWhen this ammo strikes the armor on a target,\n \nit will cause that armor to degrade rapidly.\n \nThis can potentially strip a target of its\narmor!",
	L"\n \nThis type of ammo is exceptional at breaking locks.\n \nFire it directly at a locked door or container\nto cause massive damage to the lock.",
	L"\n \nThis armor is three times more resistant\nagainst explosives than it should be, given\nits Protection value.\n \nWhen an explosion hits the armor, its Protection\nvalue is considered three times higher than\nthe listed value.",
	L"\n \nThis item is imprevious to water. It does not\nreceive damage from being submerged.\n \nItems WITHOUT this property will gradually deteriorate\nif the person carrying them goes for a swim.",
	L"\n \nThis item is electronic in nature, and contains\ncomplex circuitry.\n \nElectronic items are inherently more difficult\nto repair, at least without the ELECTRONICS skill.",
	L"\n \nWhen this item is worn on a character's face,\nit will protect them from all sorts of noxious gasses.\n \nNote that some gasses are corrosive, and might eat\nright through the mask...",
	L"\n \nThis item requires batteries. Without batteries,\nyou cannot activate its primary abilities.\n \nTo use a set of batteries, attach them to\nthis item as you would a scope to a rifle.",
	L"\n \nThis item can be used to pick open locked\ndoors or containers.\n \nLockpicking is silent, although it requires\nsubstantial mechanical skill to pick anything\nbut the simplest locks.",
	L"\n \nThis item can be used to cut through wire fences.\n \nThis allows a character to rapidly move through\nfenced areas, possibly outflanking the enemy!",
	L"\n \nThis item can be used to smash open locked\ndoors or containers.\n \nLock-smashing requires substantial strength,\ngenerates a lot of noise, and can easily\ntire a character out. However, it is a good\nway to get through locks without superior skills or\ncomplicated tools.",
	L"\n \nThis item can be used to detect metallic objects\nunder the ground.\n \nNaturally, its primary function is to detect\nmines without the necessary skills to spot them\nwith the naked eye.\n \nMaybe you'll find some buried treasure too.",
	L"\n \nThis item can be used to detonate a bomb\nwhich has been set with a remote detonator.\n \nPlant the bomb first, then use the\nRemote Trigger item to set it off when the\ntime is right.",
	L"\n \nWhen attached to an explosive device and set up\nin the right position, this detonator can be triggered\nby a (separate) remote device.\n \nRemote Detonators are great for setting traps,\nbecause they only go off when you tell them to.\n \nAlso, you have plenty of time to get away!",
	L"\n \nWhen attached to an explosive device and set up\nin the right position, this detonator will count down\nfrom the set amount of time, and explode once the\ntimer expires.\n \nTimer Detonators are cheap and easy to install,\nbut you'll need to time them just right to give\nyourself enough chance to get away!",
	L"\n \nThis item contains gasoline (fuel).\n \nIt might come in handy if you ever\nneed to fill up a gas tank...",
	L"\n \nThis item contains various tools that can\nbe used to repair other items.\n \nA toolkit item is always required when setting\na character to repair duty.",
	L"\n \nWhen worn in a face-slot, this item provides\nthe ability to spot enemies through walls,\nthanks to their heat signature.",
	L"\n \nThis powerful device can be used to scan\nfor enemies using X-rays.\n \nIt will reveal all enemies within a certain radius\nfor a short period of time.\n \nKeep away from reproductive organs!",
	L"\n \nThis item contains fresh drinking water.\nUse when thirsty.",
	L"\n \nThis item contains liquor, alcohol, booze,\nwhatever you fancy calling it.\n \nUse with caution. Do not drink and drive.\nMay cause cirrhosis of the liver.",
	L"\n \nThis is a basic field medical kit, containing\nitems required to provide basic medical aid.\n \nIt can be used to bandage wounded characters\nand prevent bleeding.\n \nFor actual healing, use a proper Medical Kit\nand/or plenty of rest.",
	L"\n \nThis is a proper medical kit, which can\nbe used in surgery and other serious medicinal\npurposes.\n \nMedical Kits are always required when setting\na character to Doctoring duty.",
	L"\n \nThis item can be used to blast open locked\ndoors and containers.\n \nExplosives skill is required to avoid\npremature detonation.\n \nBlowing locks is a relatively easy way of quickly\ngetting through locked doors. However,\nit is very loud, and dangerous to most characters.",
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
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Hearing Range by the\nlisted number of tiles.\n \nA positive bonus makes it possible to hear noises\nfrom a greater distance.\n \nConversely, a negative modifier impairs the wearer's hearing.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted number of tiles.\n \nThis General modifier works in all conditions.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted number of tiles.\n \nThis Night-Vision modifier works only when light\nlevels are sufficiently low.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted number of tiles.\n \nThis Day-Vision modifier works only when light\nlevels are average or higher.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted number of tiles.\n \nThis Bright-Vision modifier works only when light\nlevels are very high, for example when looking\ninto tiles lit by Break-Lights or at high noon.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it modifies the wearer's Vision Range by the\nlisted number of tiles.\n \nThis Cave-Vision modifier works only in the dark\nand only underground.\n \nHigher is better.",
	L"\n \nWhen this item is worn, or attached to a worn\nitem, it changes the wearer's field-of-view.\n \nNarrowing the field of view shortens sightrange to\neither side.\n \nLower is better.",
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
	L"\n \nWhen this weapon is held in hand, it modifies the\nsoldier's Hearing Range by the listed number of tiles.\n \nA positive bonus makes it possible to hear noises\nfrom a greater distance.\n \nConversely, a negative modifier impairs the wearer's hearing.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted number of tiles, thanks to attachments or\ninherent properties of the weapon.\n \nThis General modifier works in all conditions.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted number of tiles, thanks to attachments or\ninherent properties of the weapon.\n \nThis Night-Vision modifier works only when light\nlevels are sufficiently low.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted number of tiles, thanks to attachments or\ninherent properties of the weapon.\n \nThis Day-Vision modifier works only when light\nlevels are average or higher.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted number of tiles, thanks to attachments or\ninherent properties of the weapon.\n \nThis Bright-Vision modifier works only when light\nlevels are very high, for example when looking\ninto tiles lit by Break-Lights or at high noon.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit modifies the wearer's Vision Range by the\nlisted number of tiles, thanks to attachments or\ninherent properties of the weapon.\n \nThis Cave-Vision modifier works only in the dark\nand only underground.\n \nHigher is better.",
	L"\n \nWhen this weapon is raised to the shooting position,\nit changes the wearer's field-of-view.\n \nNarrowing the field of view shortens sightrange to\neither side.\n \nLower is better.",
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

#endif //GERMAN
