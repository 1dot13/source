// WANNE: This pragma should not be needed anymore for Polish version, after we set the encoding to UTF8 
// WANNE: Yes we need this here exclusivly in Polish version, because we do not have a codepage in the code like for other versions.
//#pragma setlocale("POLISH")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#if defined( POLISH )
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_PolishText_public_symbol(void){;}

#ifdef POLISH

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

	Other examples are used multiple times, like the Esc key  or "|E|s|c" or Space -> (|S|p|a|c|j|a)

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
	L"Lokalizacje", //0
	L"Postacie",
	L"Przedmioty",
	L"Zadania",
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
	L"Odprawa",
	L"Akceptuj",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Odprawa",
	L"Akceptuj misje",
};


STR16	pLocationPageText[] =
{
	L"Poprzednia str.",
	L"Zdjęcia", 
	L"Następna str.",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"Strona główna",
	L">>",
	L"Typ: ",
	L"Brak danych",
	L"Brak zdefiniowanych misji. Dodaj misje do pliku TableData\\BriefingRoom\\BriefingRoom.xml. Pierwsza misja ma byæ widoczna. Ustaw Hidden = 0",
	L"Briefing Room. Please click the 'Enter' button.",		// TODO.Translate
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Nieznane",// 0 - unknown
	L"Miasto", //1 - cities
	L"Wyrzutnia rakiet", //2 - SAM Site
	L"Inna lokacja", //3 - other location
	L"Kopalnia", //4 - mines 
	L"Kompleks militarny", //5 - military complex 
	L"Laboratorium",  //6 - laboratory complex 
	L"Fabryka", //7 - factory complex 
	L"Szpital", //8 - hospital 
	L"Więzienie", //9 - prison
    L"Port lotniczy", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Pokaż wszystko",
	L"Pokaż AIM",
	L"Pokaż MERC",
	L"Pokaż RPC",
	L"Pokaż NPC",
	L"Pokaż Pojazd",
	L"Pokaż IMP",
	L"Pokaż EPC",
	L"Filtr",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"Wszy.",
	L"AIM",
	L"MERC",
	L"RPC",
	L"NPC",
	L"Pojazd",
	L"IMP",
	L"EPC",
	L"Filtr",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Pokaż wszystko",
	L"Pokaż miasta",
	L"Pokaż wyrzutnie rakiet",
	L"Pokaż inne lokacje",
	L"Pokaż kopalnie",
	L"Pokaż lokacje militarne",
	L"Pokaż laboratoria",
	L"Pokaż fabryki",
	L"Pokaż szpitale",
	L"Pokaż więzienie",
	L"Pokaż porty lotnicze",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"Wszy.",
	L"Miasta",
	L"W-R",
	L"Inne",
	L"Kop.",
	L"Kom.M.",
	L"Lab.",
	L"Fabry.",
	L"Szpit.",
	L"Więz.",
	L"Por.L.",
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
	L"Wszystko", //0
	L"Broń",
	L"Amu.",
	L"Oporz.",
	L"Różne",
	
	L"Wszystko", //5
	L"Broń",
	L"Amunicja",
	L"Oporz¹dzenie",
	L"Różne",
};


STR16			BoxFilter[] =
{
	// Guns
	L"Inny", //0
	L"Pist.",
	L"P.masz.",
	L"K.masz.",
	L"Kar.",
	L"K.snaj.",
	L"K.boj.",
	L"L.k.m.",
	L"Strz.", //8

	// Ammo
	L"Pist.",
	L"P.masz.",
	L"K.masz.",
	L"Kar.",
	L"K.snaj.",
	L"K.boj.",
	L"L.k.masz.",
	L"Strz.", //16

	// Used
	L"Broń",
	L"Panc.",
	L"Oporz.",
	L"Różne", //20

	// Armour
	L"Hełmy",
	L"Kami.",
	L"G.ochr.",
	L"P.cer.",

	// Misc
	L"Ostrza",
	L"N.do.rzu.",
	L"Punch.W.",
	L"Gr.",
	L"Bomby",
	L"Apt.",
	L"Ekwi.",
	L"N.twa.",
	L"Oporz.",	//LBE Gear
	L"Inne",
};




//-----------

// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Usuń",
	L"Przedmiot usuń (|D|e|l)",
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
 L"Przełącz wyœwietlanie graczy", //0
 L"Przełącz wyœwietlanie wrogów",
 L"Przełącz wyœwietlanie zwierząt",
 L"Przełącz wyœwietlanie rebeliantów",
 L"Przełącz wyœwietlanie cywili",
 
 L"Gracz",
 L"Wróg",
 L"Stworzenia",
 L"Rebelianci",
 L"Cywile",
 
 L"Szczegóły", //10
 L"Tryb informacji ogólnych",
 L"Tryb fizyczny",
 L"Tryb atrybutów",
 L"Tryb wyposażenia",
 L"Tryb profilu",
 L"Tryb planowania",
 L"Tryb planowania",
 L"Usuń",
 L"Usuń zaznaczonego najemnika (|D|e|l)",
 L"Kolejny", //20
 L"Znajdź następnego najemnika (|S|p|a|c|e)",
 L"Włącz priorytet egzystencji",
 L"Postać ma dostęp do wszystkich zamkniętych drzwi",
 
 //Orders
 L"STACJONARNY",
 L"CZUJNY", 
 L"NA STRA¯Y",
 L"SZUKAJ WROGA",
 L"BLISKI PATROL",
 L"DALEKI PATROL",
 L"PKT PATROL.",//30
 L"LOS PKT PATR.",
 
 //Attitudes
 L"OBRONA",
 L"DZIELNY SOLO",
 L"DZIELNY POMOC",
 L"AGRESYWNA",
 L"SPRYTNY SOLO",
 L"SPRYTNY POMOC",
 
 L"Set merc to face %s",
 
 L"Znaj",
 
 L"MARNE", //40
 L"SŁABE",
 L"ŚREDNIE",
 L"DOBRE",
 L"ŚWIETNE",
 
 L"MARNE",
 L"SŁABE",
 L"ŚREDNIE",
 L"DOBRE",
 L"ŚWIETNE",
 
 L"Poprzedni zbiór kolorów",//"Previous color set", //50
 L"Następny zbiór kolorów",//"Next color set",
 
 L"Poprzednia budowa ciała",//"Previous body type",
 L"Następna budowa ciała",//"Next body type",
 
 L"Ustaw niezgodnoŚć czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoŚć czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoŚć czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoŚć czasu (+ or - 15 minut)",
 
 L"Brak akcji",
 L"Brak akcji",
 L"Brak akcji", //60
 L"Brak akcji",
 
 L"CzyŚć zadanie",
 
 L"Find selected merc",
};

STR16 iEditorBuildingsToolbarText[] =
{
	L"DACHY",  //0
	L"ŚCIANY",
	L"DANE POM.",

	L"RozmieŚć Ściany, używając metody wyboru",
	L"RozmieŚć drzwi, używając metody wyboru",
	L"RozmieŚć dachy, używając metody wyboru",
	L"RozmieŚć okna, używając metody wyboru",
	L"RozmieŚć uszkodzone Ściany, używając metody wyboru",
	L"RozmieŚć meble, używając metody wyboru",
	L"RozmieŚć tekstury Ścian, używając metody wyboru",
	L"RozmieŚć podłogi, używając metody wyboru", //10
	L"RozmieŚć generyczne meble, używając metody wyboru",
	L"RozmieŚć Ściany, używając metody domyŚlnej",
	L"RozmieŚć drzwi, używając metody domyŚlnej",
	L"RozmieŚć okna, używając metody domyŚlnej",
	L"RozmieŚć uszkodzone Ściany, używając metody domyŚlnej",
	L"Zablokuj drzwi, lub umieŚć pułapkę na drzwiach",

	L"Dodaj nowe pomieszczenie",
	L"Edytuj Ściany jaskini.",
	L"Usuń obszar z istniejącego budynku.",
	L"Usuń budynek", //20
	L"Dodaj/zastąp dach budynku nowym płaskim dachem.",
	L"kopiuj budynek",
	L"Przesuń budynek",
	L"Rysuj numer pomieszczenia",
	L"Usuń numer pomieszczenia",

	L"Przełącz tryb wymazywania (|E)",
	L"Cofnij ostatnią zmianę (|B|a|c|k|s|p|a|c|e)",
	L"Wybierz rozmiar pędzla (|A/|Z)",
	L"Dachy (|H)",
	L"Ściany (|W)", //30
	L"Dane Pom. (|N)",
};

STR16 iEditorItemsToolbarText[] =
{
	L"Broń", //0
	L"Amun.",
	L"Pancerz",
	L"LBE",
	L"Mat.Wyb.",
	L"E1",
	L"E2",
	L"E3",
	L"Włączniki",
	L"Klucze",
	L"Rnd",	//10 // TODO.Translate
	L"Previous (|,)",		// previous page // TODO.Translate
	L"Next (|.)",		// next page // TODO.Translate
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Dodaj źródło Światła z otoczenia", //0
	L"Przełącz fałszywe Światła z otoczenia.",
	L"Dodaj pola wyjŚcia (p-klik, aby usunąć istniejące).",
	L"Wybierz rozmiar pędzla (|A/|Z)",
	L"Cofnij ostatnią zmianę (|B|a|c|k|s|p|a|c|e)",
	L"Przełącz tryb wymazywania (|E)",
	L"OkreŚl punkt północny dla celów potwierdzenia.",
	L"OkreŚl punkt zachodu dla celów potwierdzenia.",
	L"OkreŚl punkt wschodu dla celów potwierdzenia.",
	L"OkreŚl punkt południa dla celów potwierdzenia.",
	L"OkreŚl punkt Środka dla celów potwierdzenia.", //10
	L"OkreŚl odosobniony punkt dla celów potwierdzenia.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"Nowa na wolnym powietrzu",  //0
	L"Nowa piwnica",
	L"Nowy poziom jaskini",
	L"Zapisz mapę (|C|t|r|l+|S)",
	L"Wczytaj mapę (|C|t|r|l+|L)",
	L"Wybierz zestaw",
	L"Wyjdź z trybu edycji do trybu gry",
	L"Wyjdź z trybu edycji (|A|l|t+|X)",
	L"Utwórz mapę radaru",
	L"Kiedy zaznaczone, mapa będzie zapisana w oryginalnym formacie JA2.\nTa opcja jest ważna przy normalnych wielkoŚciach map, których numery siatki nie są (siatki wyjŚcia) > 25600.",
	L"Kiedy zaznaczone, wczytana mapa lub nowa, będzie powiększona automatycznie do wybranych rozmiarów.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Rysuj tekstury terenu (|G)", //0
	L"Ustaw tekstury terenu mapy",
	L"UmieŚć brzegi i urwiska (|C)",
	L"Rysuj drogi (|P)",
	L"Rysuj gruzy (|D)",
	L"UmieŚć drzewa i krzewy (|T)",
	L"UmieŚć skały (|R)",
	L"UmieŚć beczki i inne Śmieci (|O)",
	L"Wypełnij teren",
	L"Cofnij ostatnią zmianę (|B|a|c|k|s|p|a|c|e)",
	L"Przełącz tryb wymazywania (|E)", //10
	L"Wybierz rozmiar pędzla (|A/|Z)",
	L"Zwiększ gęstoŚć pędzla (|])",
	L"Zmniejsz gęstoŚć pędzla (|[)",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Teren", //0
	L"Budynki",
	L"Przedmioty",
	L"Najemnicy",
	L"Dane mapy",
	L"Opcje",
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Terrain fasthelp text // TODO.Translate
	L"|./|,: Cycle 'width: xx' dimensions\n|P|g |U|p/|P|g |D|n: Previous/Next tile for selected object(s)/in smart method", //Buildings fasthelp text // TODO.Translate
	L"|C|t|r|l+|/: Place new item under mouse cursor\n|/: Place same item under mouse cursor", //Items fasthelp text // TODO.Translate
	L"|1-|9: Set waypoints\n|C|t|r|l+|C/|C|t|r|l+|V: Copy/Paste merc", //Mercs fasthelp text // TODO.Translate
	L"|C|t|r|l+|G: Go to grid no\n \n|I: Toggle overhead map\n|J: Toggle draw high ground\n|K: Toggle high ground markers\n|S|h|i|f|t+|L: Toggle map edge points\n|S|h|i|f|t+|T: Toggle treetops\n|U: Toggle world raise\n \n|./|,: Cycle 'width: xx' dimensions", //Map Info fasthelp text // TODO.Translate
	L"|C|t|r|l+|N: Create new map\n \n|F|5: Show Summary Info/Country Map\n|F|1|0: Remove all lights\n|F|1|1: Reverse schedules\n|F|1|2: Clear schedules\n \n|S|h|i|f|t+|R: Toggle random placement based on quantity of selected object(s)", //Options fasthelp text // TODO.Translate
};

//Editor Taskbar Utils.cpp

STR16 iRenderMapEntryPointsAndLightsText[] =
{
	L"Północny pkt wejŚcja", //0
	L"Zachodni pkt wejŚcja",
	L"Wschodni pkt wejŚcja",
	L"Południowy pkt wejŚcja",
	L"Środkowy pkt wejŚcja",
	L"Odizolowany pkt wejŚcja",
	
	L"Brzask",
	L"Noc",
	L"24h",
};

STR16 iBuildTriggerNameText[] =
{
	L"Włącznik Paniki1", //0
	L"Włącznik Paniki2",
	L"Włącznik Paniki3",
	L"Włącznik%d",
	 
	L"Akcja nacisku",
	L"Akcja Paniki1",
	L"Akcja Paniki2",
	L"Akcja Paniki3",
	L"Akcja%d",
};

STR16 iRenderDoorLockInfoText[]=
{
	L"Niezablokowane (ID)", //0
	L"Pułapka eksplodująca",
	L"Pułapka elektryczna",
	L"Cicha pułapka",
	L"Cichy alarm",
	L"Super-Elektryczna Pułapka", //5
	L"Alarm domu publicznego",
	L"Poziom pułapki %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Zapisz stary format mapy JA2 (v1.12) Version: 5.00 / 25", //0
	L"Brak mapy.",
	L"Plik:  %S, Aktualny zestaw:  %s",
	L"Powiększ istniejącą mapę lub nową.",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"PRZEŁ¥CZ", //0
	L"WIDOKI",
	L"METODA WYBORU",
	L"METODA DOMYŚLNA",
	L"METODA BUDOWANIA",
	L"Pomieszczenia", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Edytuj atrybuty zamka na mapie (siatka) %d.",
	L"Typ blokady (ID)",
	L"Typ pułapki",
	L"Poziom pułapki",
	L"Zablokowane",
};

//EditorItems.cpp

STR16 pInitEditorItemsInfoText[] = 
{
	L"Akcja nacisku", //0
	L"Akcja Paniki1",
	L"Akcja Paniki2",
	L"Akcja Paniki3",
	L"Akcja%d",
	
	L"Włącznik Paniki1", //5
	L"Włącznik Paniki2",
	L"Włącznik Paniki3",
	L"Włącznik%d",
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
	
	L"Brzask",
	L"Noc",
	L"24h", //5

	L"Promień",

	L"Pod ziemią",
	L"Poziom Światła",

	L"Ter. Otw.",
	L"Piwnica", //10
	L"Jaskinia",

	L"Ograniczenie",
	L"Scroll ID",

	L"Cel",
	L"Sektor", //15
	L"Cel",
	L"Poziom piw.",
	L"Cel",
	L"Śiatka",
};
//EditorMercs.cpp
CHAR16 gszScheduleActions[ 11 ][20] =
{
	L"Brak akcji",
	L"Zablokuj drzwi",
	L"Odblokuj drzwi",
	L"Otwórz drzwi",
	L"Zamknij drzwi",
	L"Idź do siatki",
	L"OpóŚć sektor",
	L"Wejdź do sektora",
	L"Pozostań w sektorze",
	L"Idź spać",
	L"Zignoruj to!"
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
	L"Zdrowie",
	L"Akt. zdrowie",
	L"Siła",
	L"ZwinnoŚć",
	L"SprawnoŚć",
	L"Charyzma",
	L"MądroŚć",
	L"CelnoŚć",
	L"Mat. Wybuchowe",
	L"Medycyna",
	L"Scientific",
	L"Poz. doŚw.", 
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
	L"Nazwa najemnika:", //0
	L"Rozkaz:",
	L"Postawa walki:",
};

STR16 pCreateEditMercWindowText[] = 
{
	L"Kolor najemnika", //0
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
	L"Losowy", //0
	L"Reg Male",
	L"Big Male",
	L"Stocky Male",
	L"Reg Female",
	L"NE Czołg", //5
	L"NW Czołg",
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
	L"Nieuzbrojony robot",
	L"Larwa", //20
	L"Infant",
	L"Yng F Monster",
	L"Yng M Monster",
	L"Adt F Monster",
	L"Adt M Monster", //25
	L"Queen Monster",
	L"Dziki kot",
};

STR16 pUpdateMercsInfoText[] = 
{
	L" --=ROZKAZY=-- ", //0
	L"--=POSTAWA=--",
	
	L"RELATIVE",
	L"ATRYBUTY",
	
	L"RELATIVE",
	L"WYPOSA¯ENIA",
			
	L"RELATIVE",
	L"ATRYBUTY",
	
	L"Armia",
	L"Admin.",
	L"Gwardia", //10
	
	L"Poz. doŚw.",
	L"Zdrowie",
	L"Akt. zdrowie",
	L"CelnoŚć",
	L"Siła",
	L"ZwinnoŚć",
	L"SprawnoŚć",
	L"Inteligencja",
	L"Zdol. dowodzenia",
	L"Mat. wybuchowe", //20
	L"Zdol. medyczne",
	L"Zdol. mechaniczne",
	L"Morale",
	
	L"Kolor włosów:",
	L"Kolor skóry:",
	L"Kolor kamizelki:",
	L"Kolor spodni:",
	
	L"LOSOWY",
	L"LOSOWY",
	L"LOSOWY", //30
	L"LOSOWY",
	
	L"Podaj index profilu i naciŚnij ENTER. ",
	L"Wszystkie informacje (statystyki, itd.) będą pobrane z pliku Prof.dat lub MercStartingGear.xml. ",
	L"JeŚli nie chcesz użyć profilu, to zostaw pole puste i naciŚnij ENTER. ",
	L"Nie podawaj wartoŚci '200'! WartoŚć '200' nie może być profilem! ",
	L"Wybierz profil od 0 do ",
	
	L"Aktualny Profil:  brak              ",
	L"Aktualny Profil: %s",
	
	L"STACJONARNY",
	L"CZUJNY", //40
	L"NA STRA¯Y",
	L"SZUKAJ WROGA",
	L"BLISKI PATROL",
	L"DALEKI PATROL",
	L"PKT PATROL.",
	L"LOS PKT PATR.",

	L"Akcja",
	L"Czas",
	L"V",
	L"Siatka 1", //50
	L"Siatka 2",
	L"1)",
	L"2)",
	L"3)",
	L"4)",
	
	L"zablokuj",
	L"odblokuj",
	L"otwórz",
	L"zamknij",
	
	L"Kliknij na siatkę przylegającą do drzwi (%s).", //60
	L"Kliknij na siatkę, gdzie chcesz się przemieŚcić gdy drzwi są otwarte\\zamknięte (%s).",
	L"Kliknij na siatkę, gdzie chciałbyŚ się przemieŚcić.",
	L"Kliknij na siatkę, gdzie chciałbyŚ spać. Postać po obudzeniu się automatycznie wróci do oryginalnej pozycji.",
	L" NaciŚnij ESC, by wyjŚć z trybu edycji planu.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
	L"Rozkaz patrolu bez punktów poŚrednich",
	L"Waypoints with no patrol orders",
};

STR16 pClearCurrentScheduleText[] =
{
	L"Brak akcji",
};

STR16 pCopyMercPlacementText[] =
{
	L"Umiejscowienie nie zostało skopiowane, gdyż żadne nie zostało wybrane.",
	L"Umiejscowienie skopiowane.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Umiejscowienie nie zostało wklejone, gdyż żadne umiejscowienie nie jest zapisane w buforze.",
	L"Umiejscowienie wklejone.",
	L"Umiejscowienie nie zostało wklejone, gdyż maksymalna liczba umiejscowień dla tej drużyny jest już wykorzystana.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Czy chcesz wyjŚć z trybu edytora do trybu gry ?",
	L"Czy chcesz zakończyć pracę edytora ?",
};

STR16 pHandleKeyboardShortcutsText[] = 
{
	L"Czy jesteŚ pewny, że chcesz usunąć wszystkie Światła?", //0
	L"Czy jesteŚ pewny, że chcesz cofnąć plany?",
	L"Czy jesteŚ pewny, że chcesz usunąć wszystkie plany?",
	
	L"Włączono rozmieszczanie elementów przez kilknięcie",
	L"Wyłączono rozmieszczanie elementów przez kilknięcie",
	
	L"Włączono rysowanie wysokiego podłoża", //5
	L"Wyłączono rysowanie wysokiego podłoża",
	
	L"Number of edge points: N=%d E=%d S=%d W=%d",
	
	L"Włączono losowe rozmieszczanie",
	L"Wyłączono losowe rozmieszczanie",
	
	L"Usuń korony drzew", //10
	L"Pokaż korony drzew",
	
	L"World Raise Reset",
	
	L"World Raise Set Old",
	L"World Raise Set",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Utworzono mape radaru dla %S", //0
	
	L"Usunąć aktualną mapę i rozpocząć nowy poziom piwnicy ?",
	L"Usunąć aktualną mapę i rozpocząć nowy poziom jaskini ?",
	L"Usunąć aktualną mapę i rozpocząć nowy poziom na wolnym powietrzu ?",
	
	L" Wipe out ground textures? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"HOME", //0
	L"Przełącz fałszywe Światła z otoczenia ON/OFF",

	L"INSERT",
	L"Przełącz tryb wypełnienia ON/OFF",

	L"BKSPC",
	L"Usuń ostatnią  zmianę",

	L"DEL",
	L"Quick erase object under mouse cursor",

	L"ESC",
	L"Wyjdź z edytora",

	L"PGUP/PGDN", //10
	L"Change object to be pasted",

	L"F1",
	L"WyŚwietl ekran pomocy",

	L"F10",
	L"Zapisz mapę",

	L"F11",
	L"Wczytaj mapę",

	L"+/-",
	L"Change shadow darkness by .01",

	L"SHFT +/-",  //20
	L"Change shadow darkness by .05",

	L"0 - 9",
	L"Change map/tileset filename",

	L"b",
	L"Wybierz wielkoŚć pędzla",

	L"d",
	L"Rysuj Śmieci",

	L"o",
	L"Draw obstacle",

	L"r", //30
	L"Rysuj skały",

	L"t",
	L"Toggle trees display ON/OFF",

	L"g",
	L"Rysuj tekstury ziemi",

	L"w",
	L"Rysuj Ściany budunków",

	L"e",
	L"Przełącz tryb wymazywania ON/OFF",

	L"h",  //40
	L"Przełącz tryb dachów ON/OFF",
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
CHAR16 gszActionItemDesc[ 34 ][ 30 ] =
{
	L"Mina klaksonowa",
	L"Mina oświetlająca",
	L"Eksplozja gazu Łzaw.",
	L"Eksplozja granatu Oszałam.",
	L"Eksplozja granatu dymnego",
	L"Gaz musztardowy",
	L"Mina przeciwpiechotna",
	L"Otwórz drzwi",
	L"Zamknij drzwi",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Mała eksplozja",
	L"średnia eksplozja",
	L"Duża eksplozja",
	L"Otwórz/Zamknij drzwi",
	L"Przełącz wszystkie Akcje1",
	L"Przełącz wszystkie Akcje2",
	L"Przełącz wszystkie Akcje3",
	L"Przełącz wszystkie Akcje4",
	L"Wejście do burdelu",
	L"Wyjście z burdelu",
	L"Alarm Kingpin'a",
	L"Seks z prostytutką",
	L"Pokaż pokój",
	L"Alarm lokalny",
	L"Alarm globalny",
	L"Dźwięk klaksonu",
	L"Odbezpiecz drzwi",
	L"Przełącz blokadę (drzwi)",
	L"Usuń pułapkę (drzwi)",
	L"Tog pressure items",
	L"Alarm w Museum",
	L"Alarm dzikich kotów",
	L"Duży gaz łzawiący",
};
*/
STR16 pUpdateItemStatsPanelText[] =
{
	L"Chowanie flagi", //0
	L"Brak wybranego przedmiotu.",
	L"Slot dostępny dla",
	L"losowej generacji.",
	L"Nie można edytować kluczy.",
	L"Profil identifikacyjny właŚciciela",
	L"Item class not implemented.",
	L"Slot locked as empty.",
	L"Stan",
	L"Naboje",
	L"Poziom pułapki", //10
	L"IloŚć",
	L"Poziom pułapki",
	L"Stan",
	L"Poziom pułapki",
	L"Stan",
	L"IloŚć",
	L"Poziom pułapki",
	L"Dolary",
	L"Stan",
	L"Poziom pułapki", //20
	L"Poziom pułapki",
	L"Tolerancja",
	L"Wyzwalacz alarmu",
	L"Istn. szansa",
	L"B",
	L"R",
	L"S",
};

STR16 pSetupGameTypeFlagsText[] =
{
	L"Przedmiot będzie wyŚwietlany w trybie Sci-Fi i realistycznym", //0
	L"Przedmiot będzie wyŚwietlany tylko w trybie realistycznym",
	L"Przedmiot będzie wyŚwietlany tylko w trybie Sci-Fi",
};

STR16 pSetupGunGUIText[] =
{
	L"TŁUMIK", //0
	L"CEL. SNAJP",
	L"CEL. LSER.",
	L"DWÓJNÓG",
	L"KACZY DZIÓB",
	L"GRANATNIK", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"PŁYTKI CERAM.", //0
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
	L"Ok", //0
	L"A",
	L"G",
	L"B1",
	L"B2",
	L"B3", //5
	L"WCZYTAJ",
	L"ZAPISZ",
	L"Aktual.",
};

STR16 pRenderSectorInformationText[] =
{
	L"Zestaw:  %s", //0
	L"Wersja:  Podsumowanie:  1.%02d,  Map:  %1.2f / %02d",
	L"IloŚć przedmiotów:  %d", 
	L"IloŚć Świateł:  %d",
	L"IloŚć punktów wyjŚcia:  %d",
	
	L"N",
	L"E",
	L"S",
	L"W",
	L"C",
	L"I", //10
	
	L"IloŚć pomieszczeń:  %d",
	L"Całkowita populacja :  %d",
	L"Wróg:  %d",
	L"Admin.:  %d",
	
	L"(%d szczegółowych, profile : %d  -- %d mają priorytet egzystencji)",
	L"¯ołnierze:  %d",
	
	L"(%d szczegółowych, profile : %d  -- %d mają priorytet egzystencji)",
	L"Gwardia:  %d",
	
	L"(%d szczegółowych, profile : %d  -- %d mają priorytet egzystencji)",
	L"Cywile:  %d",  //20
	
	L"(%d szczegółowych, profile : %d  -- %d mają priorytet egzystencji)",
	
	L"Ludzie:  %d",
	L"Krowy:  %d",
	L"Dzikie koty:  %d",
	
	L"Zwierzęta:  %d",
	
	L"Stworzenia:  %d",
	L"Dzikie koty:  %d",
	
	L"IloŚć zablokowanych drzwi oraz pułapki zamontowane na drzwiach:  %d",
	L"Zablokowane:  %d",
	L"Pułapki:  %d", //30
	L"Zablokowane i pułapki:  %d",
	
	L"Cywile z planami:  %d",
	
	L"Zbyt wiele wyjŚć (siatki) (więcej niż 4)...",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  %d (%d dalekie miejsce docelowe)",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  brak",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  poziom 1 używane miejsca %d siatki",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  poziom 2 -- 1) Qty: %d, 2) Qty: %d",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  poziom 3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d",
	L"Siatka wejŚcia-wyjŚcia (piwnice itd.) :  poziom 3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d, 4) Qty: %d",
	L"Enemy Relative Attributes:  %d marne, %d słabe, %d Średnie, %d dobre, %d Świetne (%+d Całkowity)", //40
	L"Enemy Relative Equipment:  %d marne, %d słabe, %d Średnie, %d dobre, %d Świetne (%+d Całkowity)",
	L"%d umiejscowienie mają rozkazy patrolu bez żadnego zdefiniowanego punktu poŚredniego.",
	L"%d umiejscowienia mają punkty poŚrednie, ale bez żadnych rozkazów.",
	L"%d siatki mają niejasne numery pokoju.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Wróg",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŚWIETLENIA!",
	
	L"Paniki1",
	L"Paniki2",
	L"Paniki3",
	L"Norm1",
	L"Norm2",
	L"Norm3",
	L"Norm4", //10
	L"Akcje nacisku",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŚWIETLENIA!",
	
	L"PRIORITY ENEMY DROPPED ITEMS",
	L"Nic",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŚWIETLENIA!",
	L"NORMAL ENEMY DROPPED ITEMS",
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŚWIETLENIA!",
	L"Nic",
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŚWIETLENIA!",
	L"BŁ¥D:  Nie można wczytać przedmiotów dla tej mapy. Powód nieznany.", //20	
};

STR16 pRenderSummaryWindowText[] =
{
	L"EDYTOR KAMPANII -- %s Version 1.%02d", //0
	L"(NIE WCZYTANO MAPY).",
	L"You currently have %d outdated maps.",
	L"The more maps that need to be updated, the longer it takes.  It'll take ",
	L"approximately 4 minutes on a P200MMX to analyse 100 maps, so",
	L"depending on your computer, it may vary.",
	L"Do you wish to regenerate info for ALL these maps at this time (y/n)?",
	
	L"Aktualnie nie ma wybranego sektora.",
	
	L"Entering a temp file name that doesn't follow campaign editor conventions...",
	
	L"You need to either load an existing map or create a new map before being",
	L"able to enter the editor, or you can quit (ESC or Alt+x).", //10

	L", poziom na powietrzu",					
	L", podziemny poziom 1",	
	L", podziemny poziom 2",	
	L", podziemny poziom 3",	
	L", alternatywny poziom G",					
	L", alternatywny poziom 1",	
	L", alternatywny poziom 2",	
	L", alternatywny poziom 3",
	
	L"SZCZEGÓŁY PRZEDMIOTÓW -- sektor %s",
	L"Podsumowanie informacji dla sektora %s:", //20
	
	L"Podsumowanie informacji dla sektora %s",
	L"nie egzystują.",
	
	L"Podsumowanie informacji dla sektora %s",
	L"nie egzystują.",
	
	L"Brak informacji o egzystencji dla sektora %s.",
	
	L"Brak informacji o egzystencji dla sektora %s.",
	
	L"PLIK:  %s",
	
	L"PLIK:  %s",
	
	L"Override READONLY",
	L"Nadpisz plik", //30
	
	L"You currently have no summary data.  By creating one, you will be able to keep track",
	L"of information pertaining to all of the sectors you edit and save.  The creation process",
	L"will analyse all maps in your \\MAPS directory, and generate a new one.  This could",
	L"take a few minutes depending on how many valid maps you have.  Valid maps are",
	L"maps following the proper naming convention from a1.dat - p16.dat.  Underground maps", 
	L"are signified by appending _b1 to _b3 before the .dat (ex:  a9_b1.dat). ",
	
	L"Czy chcesz to teraz zrobić (y/n)?",
	
	L"Brak informacji o podsumowaniu.  Anulowano tworzenie.",
	
	L"Siatka",
	L"Postęp", //40
	L"Use Alternate Maps",
	
	L"Podsumowanie",
	L"Przedmioty",
};

STR16 pUpdateSectorSummaryText[] =
{
	L"Analizuję mapę:  %s...",
};

STR16 pSummaryLoadMapCallbackText[] =
{
	L"Wczytuję mapę:  %s",
};

STR16 pReportErrorText[] =
{
	L"Skipping update for %s.  Probably due to tileset conflicts...",
};

STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Generuje informację o mapię",
};

STR16 pSummaryUpdateCallbackText[] =
{
	L"Generuję podsumowanie mapy",
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
	L"%S[%d] z domyŚlnego zestawu %s (%d, %S)",
	L"Plik: %S, podindeks: %d (%d, %S)",
	L"Zestaw:  %s",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Mały", 
 L"Średni", 
 L"Duży", 
 L"B.Duży", 
 L"SzerokoŚć: xx", 
 L"Obszar" 
 };
 
//---

CHAR16  gszAimPages[ 6 ][ 20 ] =
{
	L"Str. 1/2", //0
	L"Str. 2/2",
	
	L"Str. 1/3",
	L"Str. 2/3",
	L"Str. 3/3",
	
	L"Str. 1/1", //5
};

// by Jazz: TODO.Translate
CHAR16 zGrod[][500] =
{
	L"Robot", //0    // Robot
};

STR16 pCreditsJA2113[] =
{
	L"@T,{;JA2 v1.13 Development Team",
	L"@T,C144,R134,{;Kodowanie",
	L"@T,C144,R134,{;Grafika i dźwięki",
	L"@};(Różne inne mody!)",
	L"@T,C144,R134,{;Przedmioty",
	L"@T,C144,R134,{;Pozostali autorzy",
	L"@};(Wszyscy pozostali członkowie sceny JA którzy nas wsparli!)",
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
//	L"Monstrum",
//	L"Rakiety",
//	L"strzałka", // dart
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
//	L"Monstrum",
//	L"Rakiety",
//	L"strzałka", // dart
//	L"", // flamethrower
//	L".50 cal", // barrett
//	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[MAXITEMS][30] =
{
	L"Inny",
	L"Pistolet",
	L"Pistolet maszynowy",
	L"Karabin maszynowy",
	L"Karabin",
	L"Karabin snajperski",
	L"Karabin bojowy",
	L"Lekki karabin maszynowy",
	L"Strzelba"
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Tura gracza", // player's turn
	L"Tura przeciwnika",
	L"Tura stworzeń",
	L"Tura samoobrony",
	L"Tura cywili"
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

	L"%s dostał(a) w głowę i traci 1 punkt inteligencji!",
	L"%s dostał(a) w ramię i traci 1 punkt zręczności!",
	L"%s dostał(a) w klatkę piersiową i traci 1 punkt siły!",
	L"%s dostał(a) w nogi i traci 1 punkt zwinności!",
	L"%s dostał(a) w głowę i traci %d pkt. inteligencji!",
	L"%s dostał(a) w ramię i traci %d pkt. zręczności!",
	L"%s dostał(a) w klatkę piersiową i traci %d pkt. siły!",
	L"%s dostał(a) w nogi i traci %d pkt. zwinności!",
	L"Przerwanie!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"Dotarły twoje posiłki!",

	// In the following four lines, all %s's are merc names

	L"%s przeładowuje.",
	L"%s posiada za mało Punktów Akcji!",
	L"%s udziela pierwszej pomocy. (Naciśnij dowolny klawisz aby przerwać.)",
	L"%s i %s udzielają pierwszej pomocy. (Naciśnij dowolny klawisz aby przerwać.)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"niezawodna",
	L"zawodna",
	L"łatwa w naprawie",
	L"trudna do naprawy",
	L"solidna",
	L"niesolidna",
	L"szybkostrzelna",
	L"wolno strzelająca",
	L"daleki zasięg",
	L"krótki zasięg",
	L"mała waga",
	L"duża waga",
	L"niewielkie rozmiary",
	L"szybki ciągły ogień",
	L"brak możliwości strzelania serią",
	L"duży magazynek",
	L"mały magazynek",

	// In the following two lines, all %s's are merc names

	L"%s: kamuflaż się starł.",
	L"%s: kamuflaż się zmył.",

	// The first %s is a merc name and the second %s is an item name

	L"Brak amunicji w dodatkowej broni!",
	L"%s ukradł(a): %s.",

	// The %s is a merc name

	L"%s ma broń bez funkcji ciągłego ognia.",

	L"Już masz coś takiego dołączone.",
	L"Połączyć przedmioty?",

	// Both %s's are item names

	L"%s i %s nie pasują do siebie.",

	L"Brak",
	L"Wyjmij amunicję",
	L"Dodatki",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L" %s i %s nie mogą być używane jednocześnie.",

	L"Element, który masz na kursorze myszy może być dołączony do pewnych przedmiotów, poprzez umieszczenie go w jednym z czterech slotów.",
	L"Element, który masz na kursorze myszy może być dołączony do pewnych przedmiotów, poprzez umieszczenie go w jednym z czterech slotów. (Jednak w tym przypadku, przedmioty do siebie nie pasują.)",
	L"Ten sektor nie został oczyszczony z wrogów!",
	L"Wciąż musisz dać %s %s", 
	L"%s dostał(a) w głowę!",
	L"Przerwać walkę?",
	L"Ta zmiana będzie trwała. Kontynuować?",
	L"%s ma więcej energii!",
	L"%s poślizgnął(nęła) się na kulkach!",
	L"%s nie chwycił(a) - %s!",
	L"%s naprawił(a) %s",
	L"Przerwanie dla: ",
	L"Poddać się?",
	L"Ta osoba nie chce twojej pomocy.",
	L"NIE SĄDZĘ!",
  	L"Aby podróżować helikopterem Skyridera, musisz najpierw zmienić przydział najemników na POJAZD/HELIKOPTER.",
	L"%s miał(a) czas by przeładować tylko jedną broń",
	L"Tura dzikich kotów",
	L"ogień ciągły",
	L"brak ognia ciągłego",
	L"celna",
	L"niecelna",
	L"broń samoczynna",
	L"Wróg nie ma przedmiotów, które można ukraść!",
	L"Wróg nie ma żadnego przedmiotu w ręce!",
	
	//add new camo string
	L"%s: kamuflaż pustynny się starł.",
	L"%s: kamuflaż pustynny się zmył.",
	
	L"%s: kamuflaż leśny się starł.",
	L"%s: kamuflaż leśny się zmył.",
	
	L"%s: kamuflaż miejski się starł.",
	L"%s: kamuflaż miejski się zmył.",
	
	L"%s: kamuflaż zimowy się starł.",
	L"%s: kamuflaż zimowy się zmył.",

	L"Niemożesz przydzielić %s do tego slotu.",
	L"The %s will not fit in any open slots.",
	L"There's not enough space for this pocket.", //TODO:Translate
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
	L"Pauza",
	L"Normalna",
	L"5 min.",
	L"30 min.",
	L"60 min.",
	L"6 godz.", //NEW
};


// Assignment Strings: what assignment does the merc  have right now? For example, are they on a squad, training, 
// administering medical aid (doctor) or training a town. All are abbreviated. 8 letters is the longest it can be.

STR16 pAssignmentStrings[] =
{
	L"Oddz. 1",
	L"Oddz. 2",
	L"Oddz. 3",
	L"Oddz. 4",
	L"Oddz. 5",
	L"Oddz. 6",
	L"Oddz. 7",
	L"Oddz. 8",
	L"Oddz. 9",
	L"Oddz. 10",
	L"Oddz. 11",
	L"Oddz. 12",
	L"Oddz. 13",
	L"Oddz. 14",
	L"Oddz. 15",
	L"Oddz. 16",
	L"Oddz. 17",
	L"Oddz. 18",
	L"Oddz. 19",
	L"Oddz. 20",
	L"Służba", // on active duty
	L"Lekarz", // administering medical aid
	L"Pacjent", // getting medical aid
	L"Pojazd", // in a vehicle
	L"Podróż", // in transit - abbreviated form
	L"Naprawa", // repairing
	L"Radio Scan",	// scanning for nearby patrols	// TODO.Translate
	L"Praktyka", // training themselves  
	L"Samoobr.", // training a town to revolt 
	L"R.Samoobr.", //training moving militia units
	L"Instruk.", // training a teammate
	L"Uczeń", // being trained by someone else 
	L"Move Item",	// move items // TODO.Translate
	L"Staff", // operating a strategic facility			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Rest", // Resting at a facility					// TODO.Translate
	L"Prison",		// Flugente: interrogate prisoners
	L"Nie żyje", // dead
	L"Obezwł.", // abbreviation for incapacitated
	L"Jeniec", // Prisoner of war - captured
	L"Szpital", // patient in a hospital 
	L"Pusty",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"Samoobrona", // the title of the militia box
	L"Bez przydziału", //the number of unassigned militia troops
	L"Nie możesz przemieszczać oddziałów samoobrony gdy nieprzyjaciel jest w sektorze!",
	L"Część samoobrony nie została przydzielona do sektoru. Czy chcesz ich rozwiązać?",
};


STR16 pMilitiaButtonString[] =
{
	L"Automatyczne", // auto place the militia troops for the player
	L"OK", // done placing militia troops
	L"Rozwiąż", // HEADROCK HAM 3.6: Disband militia
	L"Unassign All", // move all milita troops to unassigned pool	// TODO.Translate
};

STR16 pConditionStrings[] = 
{
	L"Doskonały", //the state of a soldier .. excellent health
	L"Dobry", // good health
	L"Dość dobry", // fair health
	L"Ranny", // wounded health	
	L"Zmęczony",//L"Wyczerpany", // tired
	L"Krwawi", // bleeding to death
	L"Nieprzyt.", // knocked out 
	L"Umierający", // near death
	L"Nie żyje", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"Służba", // set merc on active duty
	L"Pacjent", // set as a patient to receive medical aid
	L"Pojazd", // tell merc to enter vehicle
	L"Wypuść", // let the escorted character go off on their own
	L"Anuluj", // close this menu
};


// look at pAssignmentString above for comments

STR16 pPersonnelAssignmentStrings[] =
{
	L"Oddz. 1",
	L"Oddz. 2",
	L"Oddz. 3",
	L"Oddz. 4",
	L"Oddz. 5",
	L"Oddz. 6",
	L"Oddz. 7",
	L"Oddz. 8",
	L"Oddz. 9",
	L"Oddz. 10",
	L"Oddz. 11",
	L"Oddz. 12",
	L"Oddz. 13",
	L"Oddz. 14",
	L"Oddz. 15",
	L"Oddz. 16",
	L"Oddz. 17",
	L"Oddz. 18",
	L"Oddz. 19",
	L"Oddz. 20",
	L"Służba",
	L"Lekarz",
	L"Pacjent",
	L"Pojazd",
	L"Podróż",
	L"Naprawa",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Praktyka",
	L"Trenuje samoobronę",
	L"Training Mobile Militia",	// TODO.Translate
	L"Instruktor",
	L"Uczeń",
	L"Move item",	// move items // TODO.Translate
	L"Facility Staff",			// TODO.Translate
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Resting at Facility",		// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners	TODO.Translate
	L"Nie żyje",
	L"Obezwładniony",
	L"Jeniec",
	L"Szpital",
	L"Pusty",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Oddział 1",
	L"Oddział 2",
	L"Oddział 3",
	L"Oddział 4",
	L"Oddział 5",
	L"Oddział 6",
	L"Oddział 7",
	L"Oddział 8",
	L"Oddział 9",
	L"Oddział 10",
	L"Oddział 11",
	L"Oddział 12",
	L"Oddział 13",
	L"Oddział 14",
	L"Oddział 15",
	L"Oddział 16",
	L"Oddział 17",
	L"Oddział 18",
	L"Oddział 19",
	L"Oddział 20",
	L"Służba",
	L"Lekarz",
	L"Pacjent",
	L"Pojazd",
	L"W podróży",
	L"Naprawa",
	L"Radio Scan",	// radio scan	// TODO.Translate
	L"Praktyka",
	L"Trenuj samoobronę",
	L"Train Mobiles",		// TODO.Translate
	L"Trenuj oddział",
	L"Uczeń",
	L"Move Item",	// move items // TODO.Translate
	L"Staff Facility",		// TODO.Translate
	L"Rest at Facility",	// TODO.Translate
	L"Interrogate prisoners",		// Flugente: interrogate prisoners TODO.Translate
	L"Nie żyje",
	L"Obezwładniony",
	L"Jeniec",
	L"W szpitalu", // patient in a hospital 
	L"Pusty",	// Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"Opcje kontraktu:", 
	L"", // a blank line, required
	L"Zaproponuj 1 dzień", // offer merc a one day contract extension
	L"Zaproponuj 1 tydzień", // 1 week
	L"Zaproponuj 2 tygodnie", // 2 week
	L"Zwolnij", // end merc's contract
	L"Anuluj", // stop showing this menu
};

STR16 pPOWStrings[] =
{
	L"Jeniec",  //an acronym for Prisoner of War
	L"??",
};

STR16 pLongAttributeStrings[] =
{
	L"SIŁA", //The merc's strength attribute. Others below represent the other attributes.
	L"ZRĘCZNOŚĆ",  
	L"ZWINNOŚĆ", 
	L"INTELIGENCJA", 
	L"UMIEJĘTNOŚCI STRZELECKIE",
	L"WIEDZA MEDYCZNA",
	L"ZNAJOMOŚĆ MECHANIKI",
	L"UMIEJĘTNOŚĆ DOWODZENIA",
	L"ZNAJOMOŚĆ MATERIAŁÓW WYBUCHOWYCH",
	L"POZIOM DOŚWIADCZENIA",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Osłona", // the armor rating of the merc
	L"Ekwip.", // the weight the merc is carrying
	L"Kamuf.", // the merc's camouflage rating
	L"Kamuflaż:",
	L"Ochrona:",
};

STR16 pShortAttributeStrings[] =
{
	L"Zwn", // the abbreviated version of : agility
	L"Zrc", // dexterity
	L"Sił", // strength
	L"Dow", // leadership
	L"Int", // wisdom
	L"Doś", // experience level
	L"Str", // marksmanship skill
	L"Mec", // mechanical skill
	L"Wyb", // explosive skill
	L"Med", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Przydział", // the mercs current assignment 
	L"Kontrakt", // the contract info about the merc
	L"Zdrowie", // the health level of the current merc
	L"Morale", // the morale of the current merc
	L"Stan",	// the condition of the current vehicle
	L"Paliwo",	// the fuel level of the current vehicle
};

STR16 pTrainingStrings[] =
{
	L"Praktyka", // tell merc to train self 
	L"Samoobrona", // tell merc to train town 
	L"Instruktor", // tell merc to act as trainer
	L"Uczeń", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Limit ognia:", // the allowable rate of fire for a merc who is guarding
	L" Agresywny ogień", // the merc can be aggressive in their choice of fire rates
	L" Oszczędzaj amunicję", // conserve ammo 
	L" Strzelaj w ostateczności", // fire only when the merc needs to 
	L"Inne opcje:", // other options available to merc
	L" Może się wycofać", // merc can retreat
	L" Może szukać schronienia",  // merc is allowed to seek cover
	L" Może pomagać partnerom", // merc can assist teammates
	L"OK", // done with this menu
	L"Anuluj", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Limit ognia:",
	L" *Agresywny ogień*",
	L" *Oszczędzaj amunicję*",
	L" *Strzelaj w ostateczności*",
	L"Inne opcje:",
	L" *Może się wycofać*",
	L" *Może szukać schronienia*",
	L" *Może pomagać partnerom*",
	L"OK",
	L"Anuluj",
};

STR16 pAssignMenuStrings[] =
{
	L"Służba", // merc is on active duty
	L"Lekarz", // the merc is acting as a doctor
	L"Pacjent", // the merc is receiving medical attention
	L"Pojazd", // the merc is in a vehicle
	L"Naprawa", // the merc is repairing items 
	L"Radio Scan", // Flugente: the merc is scanning for patrols in neighbouring sectors
	L"Szkolenie", // the merc is training
	L"Move Item",	// move items	// TODO.Translate
	L"Facility", // the merc is using/staffing a facility	// TODO.Translate
	L"Anuluj", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Atakuj", // set militia to aggresive
	L"Utrzymaj pozycję", // set militia to stationary
	L"Wycofaj się", // retreat militia
	L"Chodź do mnie", // retreat militia
	L"Padnij", // retreat militia	
	L"Crouch",	// TODO.Translate
	L"Kryj się",
	L"Wszyscy: Atakujcie", 
	L"Wszyscy: Utrzymajcie pozycje",
	L"Wszyscy: Wycofajcie się",
	L"Wszyscy: Chodźcie do mnie",
	L"Wszyscy: Rozproszcie się",
	L"Wszyscy: Padnijcie",
	L"All: Crouch",	// TODO.Translate
	L"Wszyscy: Kryjcie się",
	//L"Wszyscy: Szukajcie przedmiotów",
	L"Anuluj", // cancel this menu
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
//	L"Atak", // set militia to aggresive
//	L"Utrzymaj pozycję", // set militia to stationary
//	L"Odwrót", // retreat militia
//	L"Za mną", // retreat militia
//	L"Padnij", // retreat militia		 
//	L"Anuluj", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"Usuń najemnika", // remove dead merc from current team
	L"Anuluj",
};

STR16 pAttributeMenuStrings[] =
{
	L"Zdrowie",
	L"Zwinność",
	L"Zręczność",
	L"Siła",
	L"Um. dowodzenia",
	L"Um. strzeleckie",
	L"Zn. mechaniki",
	L"Zn. mat. wyb.",
	L"Wiedza med.",
	L"Anuluj",
};

STR16 pTrainingMenuStrings[] =
{
 L"Praktyka", // train yourself 
 L"Samoobrona", // train the town 
 L"Mobile Militia",		// TODO.Translate
 L"Instruktor", // train your teammates 
 L"Uczeń",  // be trained by an instructor 
 L"Anuluj", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Oddział  1",
	L"Oddział  2",
	L"Oddział  3",
	L"Oddział  4",
	L"Oddział  5",
	L"Oddział  6",
	L"Oddział  7",
	L"Oddział  8",
	L"Oddział  9",
	L"Oddział 10",
	L"Oddział 11",
	L"Oddział 12",
	L"Oddział 13",
	L"Oddział 14",
	L"Oddział 15",
	L"Oddział 16",
	L"Oddział 17",
	L"Oddział 18",
	L"Oddział 19",
	L"Oddział 20",
	L"Anuluj",
};

STR16 pPersonnelTitle[] =
{
	L"Personel", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Zdrowie: ", // health of merc
	L"Zwinność: ", 
	L"Zręczność: ",
 	L"Siła: ",
 	L"Um. dowodzenia: ",
 	L"Inteligencja: ",
 	L"Poziom dośw.: ", // experience level
 	L"Um. strzeleckie: ", 
 	L"Zn. mechaniki: ",
 	L"Zn. mat. wybuchowych: ",
 	L"Wiedza medyczna: ",
 	L"Zastaw na życie: ", // amount of medical deposit put down on the merc
 	L"Bieżący kontrakt: ", // cost of current contract
 	L"Liczba zabójstw: ", // number of kills by merc
 	L"Liczba asyst: ", // number of assists on kills by merc
 	L"Dzienny koszt:", // daily cost of merc
 	L"Ogólny koszt:", // total cost of merc
 	L"Wartość kontraktu:", // cost of current contract
 	L"Usługi ogółem", // total service rendered by merc
 	L"Zaległa kwota", // amount left on MERC merc to be paid
 	L"Celność:", // percentage of shots that hit target
 	L"Ilość walk:", // number of battles fought
 	L"Ranny(a):", // number of times merc has been wounded
 	L"Umiejętności:",
 	L"Brak umięjętności",
	L"Osiągnięcia:", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Elitarni: %d\n",
	L"Regularni: %d\n",
	L"Administratorzy: %d\n",
	L"Wrodzy Cywile: %d\n",
	L"Stworzenia: %d\n",
	L"Czołgi: %d\n", 
	L"Inne: %d\n",

	L"Do najemników: %d\n",
	L"Milicja: %d\n",
	L"Inni: %d\n",

	L"Strzałów: %d\n",
	L"Wystrzelonych Pocisków: %d\n",
	L"Rzuconych Granatów: %d\n",
	L"Rzuconych Noży: %d\n",
	L"Ataków Nożem: %d\n",
	L"Ataków Wręcz: %d\n",
	L"Udanych Trafień: %d\n",

	L"Zamki Otwarte Wytrychem: %d\n",
	L"Zamki Wyłamane: %d\n",
	L"Usunięte Pułapka: %d\n",
	L"Zdetonowane Ładunki: %d\n",
	L"Naprawione Przedmioty: %d\n",
	L"Połączone Przedmioty: %d\n",
	L"Ukradzione Przedmioty: %d\n",
	L"Wytrenowana Milicja: %d\n",
	L"Zabandażowani Najemnicy: %d\n",
	L"Wykonane Operacje Chirurgiczne: %d\n",
	L"Spotkani NPC: %d\n",
	L"Odkryte Sektory: %d\n",
	L"Uniknięte Zasadzki: %d\n",
	L"Wykonane Zadania: %d\n",

	L"Bitwy Taktyczne: %d\n",
	L"Bitwy Autorozstrzygnięte: %d\n",
	L"Wykonane Odwroty: %d\n",
	L"Napotkanych Zasadzek: %d\n",
	L"Największa Walka: %d Wrogów\n",

	L"Postrzelony: %d\n",
	L"Ugodzony Nożem: %d\n",
	L"Uderzony: %d\n",
	L"Wysadzony W Powietrze: %d\n",
	L"Uszkodzonych Atrybutów: %d\n",
	L"Poddany Zabiegom Chirurgicznym: %d\n",
	L"Wypadków Przy Pracy: %d\n",

	L"Charakter:",
	L"Niepełnosprawność:",

	L"Attitudes:",	// WANNE: For old traits display instead of "Character:"!

	L"Zombies: %d\n", // TODO.Translate

	L"Background:", // TODO.Translate
	L"Personality:", // TODO.Translate
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Brak umiejętności",
	L"Otwieranie zamków",
	L"Walka wręcz",		//JA25: modified
	L"Elektronika",
	L"Nocne operacje",			//JA25: modified
	L"Rzucanie",
	L"Szkolenie",
	L"Ciężka broń",
	L"Broń automatyczna",
	L"Skradanie się",
	L"Oburęczność",
	L"Kradzież",
	L"Sztuki walki",
	L"Broń biała",
	L"Snajper",				//JA25: modified
	L"Kamuflaż",						//JA25: modified
	L"(Eksp.)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Brak Umiejętności",
	L"Broń Automatyczna",
	L"Broń Ciężka",
	L"Strzelec Wyborowy",
	L"Myśliwy",
	L"Pistolero",
	L"Walka Wręcz",
	L"Zastępca Szeryfa",
	L"Technik",
	L"Paramedyk",	
	// Minor traits
	L"Oburęczność",
	L"Walka Wręcz",
	L"Rzucanie",
	L"Operacje Nocne",
	L"Ukradkowość",
	L"Atletyka",
	L"Bodybuilding",
	L"Ładunki Wybuchowe",
	L"Uczenie",
	L"Zwiad",
	// covert ops is a major trait that was added later
	L"Covert Ops",		// 20	// TODO.Translate
	// new minor traits
	L"Radio Operator",	// 21
	// second names for major skills
	L"Strzelec RKMu",
	L"Bombardier",
	L"Snajper",
	L"Myśliwy",
	L"Pistolero",
	L"Sztuki Walki",
	L"Dowódca Drużyny",
	L"Inżynier",
	L"Doktor",
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
	L"Spy",				// 41	// TODO.Translate
	L"Placeholder",		// for radio operator (minor trait)
	L"Więcej...",
};
//////////////////////////////////////////////////////////


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"W|stań/Idź",
	L"S|chyl się/Idź",
	L"Wstań/Biegnij (|R)",
	L"|Padnij/Czołgaj się",
	L"Patrz (|L)",
	L"Akcja",
	L"Rozmawiaj",
	L"Zbadaj (|C|t|r|l)",

	// Pop up door menu
	L"Otwórz",
	L"Poszukaj pułapek",
	L"Użyj wytrychów",
	L"Wyważ",
	L"Usuń pułapki",
	L"Zamknij na klucz",
	L"Otwórz kluczem",
	L"Użyj ładunku wybuchowego",
	L"Użyj łomu",
	L"Anuluj (|E|s|c)",
	L"Zamknij"
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"nie posiada żadnych pułapek",
	L"ma założony ładunek wybuchowy",
	L"jest pod napięciem",
	L"posiada syrenę alarmową",
	L"posiada dyskretny alarm"
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"dzień",
	L"tydzień",
	L"dwa tygodnie",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Wybór postaci",
	L"Przydział najemnika",
	L"Nanieś trasę podróży",
	L"Kontrakt najemnika",
	L"Usuń najemnika",
	L"Śpij", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"CICHY",
	L"WYRAŹNY",
	L"GŁOŚNY",
	L"BARDZO GŁOŚNY"
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"NIEOKREŚLONY DŹWIĘK",
	L"ODGŁOS RUCHU",
	L"ODGŁOS SKRZYPNIĘCIA",
	L"PLUSK",
	L"ODGŁOS UDERZENIA",
	L"STRZAŁ",
	L"WYBUCH",
	L"KRZYK",
	L"ODGŁOS UDERZENIA",
	L"ODGŁOS UDERZENIA",
	L"ŁOMOT",
	L"TRZASK"
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"PŁN-WSCH",
	L"WSCH",
	L"PŁD-WSCH",
	L"PŁD",
	L"PŁD-ZACH",
	L"ZACH",
	L"PŁN-ZACH",
	L"PŁN"
};

// These are the different terrain types. 

STR16 pLandTypeStrings[] =
{
	L"Miasto",
	L"Droga",
	L"Otwarty teren",
	L"Pustynia",
	L"Las",
	L"Las",	 
	L"Bagno",
	L"Woda",	
	L"Wzgórza",
	L"Teren nieprzejezdny",
	L"Rzeka",	//river from north to south
	L"Rzeka",	//river from east to west
	L"Terytorium innego kraju",
	//NONE of the following are used for directional travel, just for the sector description.
	L"Tropiki",
	L"Pola uprawne",
	L"Otwarty teren, droga",
	L"Las, droga",
	L"Las, droga",
	L"Tropiki, droga",
	L"Las, droga",
	L"Wybrzeże",
	L"Góry, droga",
	L"Wybrzeże, droga",
	L"Pustynia, droga",
	L"Bagno, droga",
	L"Las, Rakiety Z-P",
	L"Pustynia, Rakiety Z-P",
	L"Tropiki, Rakiety Z-P",
	L"Meduna, Rakiety Z-P",
	
	//These are descriptions for special sectors
	L"Szpital w Cambrii",
	L"Lotnisko w Drassen",
	L"Lotnisko w Medunie",
	L"Rakiety Z-P",
	L"Kryjówka rebeliantów", //The rebel base underground in sector A10
	L"Tixa - Lochy",	//The basement of the Tixa Prison (J9)
	L"Gniazdo stworzeń",	//Any mine sector with creatures in it
	L"Orta - Piwnica",	//The basement of Orta (K4)
	L"Tunel",				//The tunnel access from the maze garden in Meduna 
						//leading to the secret shelter underneath the palace
	L"Schron",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s wykryto w sektorze %c%d, a inny oddział jest w drodze.",	//STR_DETECTED_SINGULAR
	L"%s wykryto w sektorze %c%d, a inne oddziały są w drodze.",	//STR_DETECTED_PLURAL
	L"Chcesz skoordynować jednoczesne przybycie?",			//STR_COORDINATE

	//Dialog strings for enemies.

	L"Wróg daje ci szansę się poddać.",			//STR_ENEMY_SURRENDER_OFFER
	L"Wróg schwytał resztę twoich nieprzytomnych najemników.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Odwrót", 		//The retreat button				//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"OBRONA",								//STR_AR_DEFEND_HEADER
	L"ATAK",								//STR_AR_ATTACK_HEADER
	L"STARCIE",								//STR_AR_ENCOUNTER_HEADER
	L"Sektor",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"ZWYCIĘSTWO!",								//STR_AR_OVER_VICTORY
	L"PORAŻKA!",								//STR_AR_OVER_DEFEAT
	L"KAPITULACJA!",							//STR_AR_OVER_SURRENDERED
	L"NIEWOLA!",								//STR_AR_OVER_CAPTURED
	L"ODWRÓT!",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Samoobrona",							//STR_AR_MILITIA_NAME,
	L"Elity",								//STR_AR_ELITE_NAME,
	L"Żołnierze",								//STR_AR_TROOP_NAME,
	L"Administrator",							//STR_AR_ADMINISTRATOR_NAME,
	L"Stworzenie",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Czas trwania",							//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"WYCOFAŁ(A) SIĘ",								//STR_AR_MERC_RETREATED,
	L"WYCOFUJE SIĘ",								//STR_AR_MERC_RETREATING,
	L"WYCOFAJ SIĘ",								//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Rozst. autom.",							//STR_PB_AUTORESOLVE_BTN,
	L"Idź do sektora",							//STR_PB_GOTOSECTOR_BTN,
	L"Wycof. ludzi",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"STARCIE Z WROGIEM",							//STR_PB_ENEMYENCOUNTER_HEADER,
	L"INWAZJA WROGA",							//STR_PB_ENEMYINVASION_HEADER, // 30
	L"ZASADZKA WROGA",								//STR_PB_ENEMYAMBUSH_HEADER
	L"WEJŚCIE DO WROGIEGO SEKTORA",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"ATAK STWORÓW",							//STR_PB_CREATUREATTACK_HEADER
	L"ATAK DZIKICH KOTÓW",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"WEJŚCIE DO LEGOWISKA DZIKICH KOTÓW",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Położenie",
	L"Wrogowie",
	L"Najemnicy",
	L"Samoobrona",
	L"Stwory",
	L"Dzikie koty",
	L"Sektor",
	L"Brak",		//If there are no uninvolved mercs in this fight.
	L"N/D",			//Acronym of Not Applicable
	L"d",			//One letter abbreviation of day
	L"g",			//One letter abbreviation of hour

	//TACTICAL PLACEMENT USER INTERFACE STRINGS
	//The four buttons 

	L"Wyczyść",
	L"Rozprosz",
	L"Zgrupuj",
	L"OK",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Kasuje wszystkie pozy|cje najemników, \ni pozwala ponownie je wprowadzić.", 
	L"Po każdym naciśnięciu rozmie|szcza\nlosowo twoich najemników.",
	L"|Grupuje najemników w wybranym miejscu.",
	L"Kliknij ten klawisz gdy już rozmieścisz \nswoich najemników. (|E|n|t|e|r)",
	L"Musisz rozmieścić wszystkich najemników \nzanim rozpoczniesz walkę.",

	//Various strings (translate word for word)

	L"Sektor",
	L"Wybierz początkowe pozycje",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"To miejsce nie jest zbyt dobre. Jest niedostępne. Spróbuj gdzie indziej.",
	L"Rozmieść swoich najemników na podświetlonej części mapy.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"przybył(a) do sektora", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Automatycznie prowadzi walkę za ciebie, \nnie ładując planszy. (|A)",
	L"Atakując sektor wroga \nnie można automatycznie rozstrzygnąć walki.",
	L"Wejście do sektora \nby nawiązać walkę z wrogiem. (|E)",
	L"Wycofuje oddział \ndo sąsiedniego sektora. (|R)",				//singular version
	L"Wycofuje wszystkie oddziały \ndo sąsiedniego sektora. (|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"Nieprzyjaciel zatakował oddziały samoobrony w sektorze %c%d.",
	//%c%d is the sector -- ex:  A9
	L"Stworzenia zaatakowały oddziały samoobrony w sektorze %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Stworzenia zatakowały i zabiły %d cywili w sektorze %s.",
	//%c%d is the sector -- ex:  A9
	L"Nieprzyjaciel zatakował twoich najemników w sektorze %s.  Żaden z twoich najemników nie może walczyć!",
	//%c%d is the sector -- ex:  A9
	L"Stworzenia zatakowały twoich najemników w sektorze %s.  Żaden z twoich najemników nie może walczyć!",

};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"Dzień",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Zn. w sektorze:",
	L"Zn. w dniu:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 19 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Stan:",
	L"Waga:", 
	L"PA Koszty",	
	L"Zas.:",		// Range
	L"Siła:",		// Damage
	L"Ilość:", 		// Number of bullets left in a magazine
    L"PA:",                 // abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Celność:",	//9
	L"Zasięg:",		//10	
	L"Siła:",		//11
	L"Waga:",		//12
	L"Ogłuszenie:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Attachments:",	//14	// TODO.Translate
	L"AUTO/5:",		//15
	L"Remaining ammo:",		//16	// TODO.Translate

	L"Domyślne:",	//17 //WarmSteel - So we can also display default attachments
	L"Dirt:",	// 18	//added by Flugente	// TODO.Translate
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
	L"AP/przygot.",
	L"AP za 1 strzał",
	L"AP za serię",
	L"AP za Auto",
	L"AP/przeładuj",
	L"AP/przeładuj ręcznie",
	L"Burst Penalty (Lower is better)",	//19
	L"Modf. dwójnogu",
	L"Auto/5AP",
	L"Autofire Penalty (Lower is better)",
	L"PA: (mniej - lepiej)",	//23
	L"AP za rzut",
	L"AP za strzał",
	L"AP/cios-nóż",
	L"Wył. 1 strzał!",
	L"Wył. serię!",
	L"Wył. auto!",
	L"AP/cios-łom",
    L"",
	L"|R|e|p|a|i|r |E|a|s|e\n \nDetermines how difficult it is to repair\nthis weapon and who can fully repair it.\n \ngreen = Anybody can repair it.\n \nyellow = Only special NPCs can\nrepair it beyond repair threshold.\n \nred = This item can't be repaired.\n \nHigher is better.",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
{
	L"Modyf. rozmiaru (mniej - lepiej)", // 0
	L"Modyf. sprawności",
	L"Modyf. głośności (mniej - lepiej)",
	L"Ukrywa błysk",
	L"Modf. dwójnogu",
	L"Modyf. zasięgu", // 5
	L"Modyf. trafień",
	L"Max zasg. lasera",
	L"Modf bonusu celowania",
	L"Modyf. dług. serii",
	L"Modyf. kary za serię (więcej - lepiej)", // 10
	L"Modyf. kary za ogień auto (więcej - lepiej)",
	L"Modyf. AP",
	L"Modyf. AP za serię (mniej - lepiej)",
	L"Modyf. AP za ogień auto (mniej - lepiej)",
	L"Modf AP/przygotwanie (mniej - lepiej)", // 15
	L"Modf AP/przeładowanie (mniej - lepiej)",
	L"Modyf. wlk. magazynka",
	L"Modyf. AP/atak (mniej - lepiej)",
	L"Modyf. obrażeń",
	L"Modf obr. walki wręcz", // 20
	L"Kam leśny",
	L"Kam miasto",
	L"Kam pustyn.",
	L"Kam śnieg",
	L"Modyf. skradania", // 25
	L"Modyf. zasg. słuchu",
	L"Modyf. zasg. wzroku",
	L"Modyf. zasg. wzroku/dzień",
	L"Modyf. zasg. wzroku/noc",
	L"Modyf. zasg. wzroku/jasne światło", //30
	L"Modyf. zasg. wzr./jaskinia",
	L"Widzenie tunelowe w % (mniej - lepiej)",
	L"Min. zasg. dla bonusu cel.",
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
	L"Kwota",
	L"Pozostało:", //this is the overall balance
	L"Kwota",
	L"Wydzielić:", // the amount he wants to separate from the overall balance to get two piles of money

	L"Bieżące",
	L"Saldo:",
	L"Kwota",
	L"do podjęcia:",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"UMIERAJĄCY",		//	>= 0
	L"KRYTYCZNY", 		//	>= 15
	L"KIEPSKI",		//	>= 30
	L"RANNY",    	//	>= 45
	L"ZDROWY",    	//	>= 60
	L"SILNY",     	// 	>= 75
	L"DOSKONAŁY",		// 	>= 90
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
	L"Wydziel",
	L"Podejmij",
};

// short words meaning "Advantages" for "Pros" and "Disadvantages" for "Cons." 
CHAR16		gzProsLabel[10] = 
{
	L"Zalety:",
};

CHAR16		gzConsLabel[10] = 
{
	L"Wady:",
};

//Conversation options a player has when encountering an NPC
CHAR16 zTalkMenuStrings[6][ SMALL_STRING_LENGTH ] =
{
	L"Powtórz", 	//meaning "Repeat yourself"
	L"Przyjaźnie",		//approach in a friendly
	L"Bezpośrednio",		//approach directly - let's get down to business
	L"Groźnie",		//approach threateningly - talk now, or I'll blow your face off
	L"Daj",		
	L"Rekrutuj",
};

//Some NPCs buy, sell or repair items. These different options are available for those NPCs as well.
CHAR16 zDealerStrings[4][ SMALL_STRING_LENGTH ]=
{
	L"Kup/Sprzedaj",
	L"Kup",
	L"Sprzedaj",
	L"Napraw",
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
 L"Furgonetka z lodami",
 L"Jeep",
 L"Czołg",
 L"Helikopter",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Furg.",
	L"Jeep",
	L"Czołg",
	L"Heli.", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Furg.",			// Ice cream truck
	L"Jeep",
	L"Czołg",
	L"Heli.", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Nalot",
	L"Udzielić automatycznie pierwszej pomocy?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s zauważył(a) że dostawa jest niekompletna.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Zamek %s.", 
	L"Brak zamka.",
	L"Sukces!",
	L"Niepowodzenie.",
	L"Sukces!",
	L"Niepowodzenie.",
	L"Zamek nie ma pułapek.",
	L"Sukces!",
	// The %s is a merc name
	L"%s nie posiada odpowiedniego klucza.",
	L"Zamek został rozbrojony.",
	L"Zamek nie ma pułapek.",
	L"Zamknięte.",
	L"DRZWI",
	L"ZABEZP.",
	L"ZAMKNIĘTE",
	L"OTWARTE",
	L"ROZWALONE",
	L"Tu jest przełącznik. Włączyć go?",
	L"Rozbroić pułapkę?",
	L"Poprz...",
	L"Nast...",
	L"Więcej...",

	// In the next 2 strings, %s is an item name

	L"%s - położono na ziemi.",
	L"%s - przekazano do - %s.",

	// In the next 2 strings, %s is a name

	L"%s otrzymał(a) całą zapłatę.",
	L"%s - należność wobec niej/niego wynosi jeszcze %d.",
	L"Wybierz częstotliwość sygnału detonującego:",  	//in this case, frequency refers to a radio signal
	L"Ile tur do eksplozji:",	//how much time, in turns, until the bomb blows
	L"Ustaw częstotliwość zdalnego detonatora:", 	//in this case, frequency refers to a radio signal
	L"Rozbroić pułapkę?",
	L"Usunąć niebieską flagę?",
	L"Umieścić tutaj niebieską flagę?",
	L"Kończąca tura",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Na pewno chcesz zaatakować - %s?",
	L"Pojazdy nie mogą zmieniać pozycji.",
	L"Robot nie może zmieniać pozycji.",

	// In the next 3 strings, %s is a name

	L"%s nie może zmienić pozycji w tym miejscu.",
	L"%s nie może tu otrzymać pierwszej pomocy.",	
	L"%s nie potrzebuje pierwszej pomocy.",
	L"Nie można ruszyć w to miejsce.",
	L"Oddział jest już kompletny. Nie ma miejsca dla nowych rekrutów.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s pracuje już dla ciebie.",

	// Here %s is a name and %d is a number

	L"%s - należność wobec niej/niego wynosi %d$.",

	// In the next string, %s is a name

	L"%s - Eskortować tą osobę?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"%s - Zatrudnić tą osobę za %s dziennie?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Chcesz walczyć?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"%s - Kupić to za %s?",

	// In the next string, %s is a name

	L"%s jest pod eskortą oddziału %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"ZACIĘTA",					//weapon is jammed.
	L"Robot potrzebuje amunicji kaliber %s.",		//Robot is out of ammo
	L"Rzucić tam? To niemożliwe.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Skradanie się (|Z)",
	L"Ekran |Mapy",
	L"Koniec tury (|D)",
	L"Rozmowa",
	L"Wycisz",
	L"Pozycja do góry (|P|g|U|p)",
	L"Poziom kursora (|T|a|b)",
	L"Wspinaj się / Zeskocz (|J)",
	L"Pozycja w dół (|P|g|D|n)",
	L"Badać (|C|t|r|l)",
	L"Poprzedni najemnik",
	L"Następny najemnik (|S|p|a|c|j|a)",
	L"|Opcje",
	L"Ciągły ogień (|B)",
	L"Spójrz/Obróć się (|L)",
	L"Zdrowie: %d/%d\nEnergia: %d/%d\nMorale: %s",
	L"Co?",					//this means "what?"
	L"Kont",					//an abbrieviation for "Continued"
	L"%s ma włączone potwierdzenia głosowe.",
	L"%s ma wyłączone potwierdzenia głosowe.",
	L"Stan: %d/%d\nPaliwo: %d/%d",
	L"Wysiądź z pojazdu" ,
	L"Zmień oddział ( |S|h|i|f|t |S|p|a|c|j|a )",
	L"Prowadź",
	L"N/D",						//this is an acronym for "Not Applicable."
	L"Użyj ( Walka wręcz )",
	L"Użyj ( Broni palnej )",
	L"Użyj ( Broni białej )",
	L"Użyj ( Mat. wybuchowych )",
	L"Użyj ( Apteczki )",
	L"(Łap)",
	L"(Przeładuj)",
	L"(Daj)",
	L"%s - pułapka została uruchomiona.",
	L"%s przybył(a) na miejsce.",
	L"%s stracił(a) wszystkie Punkty Akcji.",
	L"%s jest nieosiągalny(na).",
	L"%s ma już założone opatrunki.",
	L"%s nie ma bandaży.",
	L"Wróg w sektorze!",
	L"Nie ma wroga w zasięgu wzroku.",
	L"Zbyt mało Punktów Akcji.",
	L"Nikt nie używa zdalnego sterowania.",
	L"Ciągły ogień opróżnił magazynek!",
	L"ŻOŁNIERZ",
	L"STWÓR",
	L"SAMOOBRONA",
	L"CYWIL",
	L"ZOMBIE",		// TODO.Translate
	L"PRISONER",//TODO.Translate
	L"Wyjście z sektora",
	L"OK",
	L"Anuluj",
	L"Wybrany najemnik",
	L"Wszyscy najemnicy w oddziale",
	L"Idź do sektora",
	L"Otwórz mapę",
	L"Nie można opuścić sektora z tej strony.",
	L"You can't leave in turn based mode.",		// TODO.Translate
	L"%s jest zbyt daleko.",
	L"Usuń korony drzew",
	L"Pokaż korony drzew",
	L"WRONA",				//Crow, as in the large black bird
	L"SZYJA",
	L"GŁOWA",
	L"TUŁÓW",
	L"NOGI",
	L"Powiedzieć królowej to, co chce wiedzieć?",
	L"Wzór odcisku palca pobrany",
	L"Niewłaściwy wzór odcisku palca. Broń bezużyteczna.",
	L"Cel osiągnięty",
	L"Droga zablokowana",
	L"Wpłata/Podjęcie pieniędzy",		//Help text over the $ button on the Single Merc Panel 
	L"Nikt nie potrzebuje pierwszej pomocy.",
	L"Zac.",						// Short form of JAMMED, for small inv slots
	L"Nie można się tam dostać.",					// used ( now ) for when we click on a cliff
	L"Przejście zablokowane. Czy chcesz zamienić się miejscami z tą osobą?",
	L"Osoba nie chce się przesunąć.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Zgadzasz się zapłacić %s?",
	L"Zgadzasz się na darmowe leczenie?",
	L"Zgadasz się na małżeństwo z %s?", //Darylem
	L"Kółko na klucze",
	L"Nie możesz tego zrobić z eskortowaną osobą.",
	L"Oszczędzić %s?", //Krotta
	L"Poza zasięgiem broni",
	L"Górnik",
	L"Pojazdem można podróżować tylko pomiędzy sektorami",
	L"Teraz nie można automatycznie udzielić pierwszej pomocy",
	L"Przejście zablokowane dla - %s",
	L"Twoi najemnicy, schwytani przez żołnierzy %s, są tutaj uwięzieni!", //Deidranny
	L"Zamek został trafiony",
	L"Zamek został zniszczony",
	L"Ktoś inny majstruje przy tych drzwiach.",
	L"Stan: %d/%d\nPaliwo: %d/%d",
	L"%s nie widzi - %s.",  // Cannot see person trying to talk to
	L"Dodatek usunięty",
	L"Nie możesz zdobyć kolejnego pojazdu, ponieważ posiadasz już 2",

	// added by Flugente for defusing/setting up trap networks	// TODO.Translate
	L"Choose detonation frequency (1 - 4) or defuse frequency (A - D):",
	L"Set defusing frequency:",
	L"Set detonation frequency (1 - 4) and defusing frequency (A - D):",
	L"Set detonation time in turns (1 - 4) and defusing frequency (A - D):",
	L"Select tripwire hierarchy (1 - 4) and network (A - D):",

	// added by Flugente to display health and poisoning		// TODO.Translate
	L"Health: %d/%d\nPoison: %d/%d\nEnergy: %d/%d\nMorale: %s",

	// added by Flugente to display food status
	L"Health: %d/%d\nEnergy: %d/%d\nMorale: %s\nWater: %d%s\nFood: %d%s",
	L"Health: %d/%d\nPoison: %d/%d\nEnergy: %d/%d\nMorale: %s\nWater: %d%s\nFood: %d%s",

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
	L"You have no prison for these prisoners, you have to let them go",
	L"Where do you want to send the prisoners?",
	L"Let them go",
	L"What do you want to do?",
	L"Demand surrender",
	L"Offer surrender",
	L"Talk",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"Jeśli zaznaczysz tę opcję, to sąsiedni sektor zostanie natychmiast załadowany.",
	L"Jeśli zaznaczysz tę opcję, to na czas podróży pojawi się automatycznie ekran mapy.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Ten sektor jest okupowany przez wroga i nie możesz tu zostawić najemników.\nMusisz uporać się z tą sytuacją zanim załadujesz inny sektor.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Gdy wyprowadzisz swoich pozostałych najemników z tego sektora,\nsąsiedni sektor zostanie automatycznie załadowany.",
	L"Gdy wyprowadzisz swoich pozostałych najemników z tego sektora,\nto na czas podróży pojawi się automatycznie ekran mapy.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s jest pod eskortą twoich najemników i nie może bez nich opuścić tego sektora.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s nie może sam opuścić tego sektora, gdyż %s jest pod jego eskortą.", //male singular
	L"%s nie może sama opuścić tego sektora, gdyż %s jest pod jej eskortą.", //female singular
	L"%s nie może sam opuścić tego sektora, gdyż eskortuje inne osoby.", //male plural
	L"%s nie może sama opuścić tego sektora, gdyż eskortuje inne osoby.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Wszyscy twoi najemnicy muszą być w pobliżu,\naby oddział mógł się przemieszczać.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Jeśli zaznaczysz tę opcję, %s będzie podróżować w pojedynkę\ni automatycznie znajdzie się w osobnym oddziale.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Jeśli zaznaczysz tę opcję, aktualnie\nwybrany oddział opuści ten sektor.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s jest pod eskortą twoich najemników i nie może bez nich opuścić tego sektora. Aby opuścić sektor twoi najemnicy muszą być w pobliżu.",
};



STR16 pRepairStrings[] = 
{
	L"Wyposażenie", 		// tell merc to repair items in inventory
	L"Baza rakiet Z-P", // tell merc to repair SAM site - SAM is an acronym for Surface to Air Missile
	L"Anuluj", 		// cancel this menu
	L"Robot", 		// repair the robot
};


// NOTE: combine prestatbuildstring with statgain to get a line like the example below.
// "John has gained 3 points of marksmanship skill." 

STR16 sPreStatBuildString[] =
{
	L"traci", 		// the merc has lost a statistic
	L"zyskuje", 		// the merc has gained a statistic
	L"pkt.",	// singular
	L"pkt.",	// plural
	L"pkt.",	// singular
	L"pkt.",	// plural
};

STR16 sStatGainStrings[] =
{
	L"zdrowia.",
	L"zwinności.",
	L"zręczności.",
	L"inteligencji.",
	L"umiejętności medycznych.",
	L"umiejętności w dziedzinie materiałów wybuchowych.",
	L"umiejętności w dziedzinie mechaniki.",
	L"umiejętności strzeleckich.",
	L"doświadczenia.",
	L"siły.",
	L"umiejętności dowodzenia.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Całkowita trasa:  ",// total distance for helicopter to travel
	L" Bezp.:   ", 			// distance to travel to destination
	L" Niebezp.:", 			// distance to return from destination to airport
	L"Całkowity koszt: ", 		// total cost of trip by helicopter
	L"PCP:  ", 			// ETA is an acronym for "estimated time of arrival" 
	L"Helikopter ma mało paliwa i musi wylądować na terenie wroga.",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
  L"Pasażerowie: ",
  L"Wybór Skyridera lub pasażerów?",
  L"Skyrider",
  L"Pasażerowie",
};

STR16 sMapLevelString[] =
{
	L"Poziom:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Lojalności", 			// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"nie można wydawać rozkazów podróży pod ziemią.", 
};

STR16 gsTimeStrings[] =
{
	L"g",				// hours abbreviation
	L"m",				// minutes abbreviation
	L"s",				// seconds abbreviation
	L"d",				// days abbreviation
};

// text for the various facilities in the sector

STR16 sFacilitiesStrings[] =
{
	L"Brak", 		
	L"Szpital",  		 
	L"Factory",	// TODO.Translate
	L"Więzienie",
	L"Baza wojskowa",
	L"Lotnisko",
	L"Strzelnica",		// a field for soldiers to practise their shooting skills
};

// text for inventory pop up button

STR16 pMapPopUpInventoryText[] =
{
	L"Inwentarz",
	L"Zamknij",
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Rozmiar",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Pod kontrolą", 					// how much of town is controlled
	L"Brak", 					// none of this town
	L"Przynależna kopalnia", 				// mine associated with this town
	L"Lojalność",					// 5 // the loyalty level of this town
	L"Wyszkolonych", 					// the forces in the town trained by the player
	L"",
	L"Główne obiekty", 				// main facilities in this town
	L"Poziom", 					// the training level of civilians in this town
	L"Szkolenie cywili",				// 10 // state of civilian training in town
	L"Samoobrona", 					// the state of the trained civilians in the town
	L"Mobile Training",			// HEADROCK HAM 3.6: The stat of Mobile militia training in town	// TODO.Translate
};

// Mine strings

STR16 pwMineStrings[] =
{
	L"Kopalnia",						// 0
	L"Srebro",
	L"Złoto",
	L"Dzienna produkcja",
	L"Możliwa produkcja",
	L"Opuszczona",				// 5
	L"Zamknięta",
	L"Na wyczerpaniu",
	L"Produkuje",
	L"Stan",
	L"Tempo produkcji",
	L"Resource",				// 10	L"Typ złoża",	// TODO.Translate
	L"Kontrola miasta",
	L"Lojalność miasta",
//	L"Górnicy",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Siły wroga",
	L"Sektor",
	L"Przedmiotów",
	L"Nieznane",
	L"Pod kontrolą",
	L"Tak",
	L"Nie",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s jest zbyt daleko.",	//Merc is in sector with item but not close enough
	L"Nie można wybrać tego najemnika.",  //MARK CARTER
	L"%s nie może stąd zabrać tego przedmiotu, gdyż nie jest w tym sektorze.",
	L"Podczas walki nie można korzystać z tego panelu.",
	L"Podczas walki nie można korzystać z tego panelu.",
	L"%s nie może tu zostawić tego przedmiotu, gdyż nie jest w tym sektorze.",
	L"W trakcie walki nie możesz doładowywać magazynka.",
};

STR16 pMapInventoryStrings[] =
{
	L"Położenie", 			// sector these items are in
	L"Razem przedmiotów", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Kliknij w kolumnie 'Przydz.', aby przydzielić najemnika do innego oddziału lub wybranego zadania.",
	L"Aby wyznaczyć najemnikowi cel w innym sektorze, kliknij pole w kolumnie 'Cel'.",
	L"Gdy najemnicy otrzymają już rozkaz przemieszczenia się, kompresja czasu pozwala im szybciej dotrzeć na miejsce.",
	L"Kliknij lewym klawiszem aby wybrać sektor. Kliknij ponownie aby wydać najemnikom rozkazy przemieszczenia, lub kliknij prawym klawiszem by uzyskać informacje o sektorze.",
	L"Naciśnij w dowolnym momencie klawisz 'H' by wyświetlić okienko pomocy.",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Niewiele możesz tu zrobić, dopóki najemnicy nie przylecą do Arulco. Gdy już zbierzesz swój oddział, kliknij przycisk Kompresji Czasu, w prawym dolnym rogu. W ten sposób twoi najemnicy szybciej dotrą na miejsce.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Przemieść najemników", 	// title for movement box 
	L"Nanieś trasę podróży", 		// done with movement menu, start plotting movement
	L"Anuluj", 		// cancel this menu
	L"Inni",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"Oj:", 			// an error has occured
	L"Wygasł kontrakt najemników:", 	// this pop up came up due to a merc contract ending
	L"Najemnicy wypełnili zadanie:", // this pop up....due to more than one merc finishing assignments
	L"Najemnicy wrócili do pracy:", // this pop up ....due to more than one merc waking up and returing to work
	L"Odpoczywający najemnicy:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Wkrótce wygasną kontrakty:", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Pokaż miasta (|W)",
	L"Pokaż kopalnie (|M)",
	L"Pokaż oddziały i wrogów (|T)",
	L"Pokaż przestrzeń powietrzną (|A)",
	L"Pokaż przedmioty (|I)",
	L"Pokaż samoobronę i wrogów (|Z)",
	L"Show Mobile Militia |Restrictions", // HEADROCK HAM 4: Mobile Restrictions Button // TODO.Translate
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
	L"Remove all item attachments", // HEADROCK HAM 5: Separate Attachments
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
	L"|L|e|f|t |C|l|i|c|k: Toggle Move Item Display", // Flugente: move item display	// TODO.Translate
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"|Laptop",
	L"Ekran taktyczny (|E|s|c)",
	L"|Opcje",
	L"Kompresja czasu (|+)", 	// time compress more
	L"Kompresja czasu (|-)", 	// time compress less
	L"Poprzedni komunikat (|S|t|r|z|a|ł|k|a |w |g|ó|r|ę)\nPoprzednia strona (|P|g|U|p)", 	// previous message in scrollable list
	L"Następny komunikat (|S|t|r|z|a|ł|k|a |w |d|ó|ł)\nNastępna strona (|P|g|D|n)", 	// next message in the scrollable list
	L"Włącz/Wyłącz kompresję czasu (|S|p|a|c|j|a)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Saldo dostępne", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s nie żyje.",
};


STR16 pDayStrings[] =
{
	L"Dzień",
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
	L"Pomoc",
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
	L"M.I.S. Ubezpieczenia",	
	L"Bobby Rays",
	L"Kingpin",
	L"John Kulba",
	L"A.I.M.",
};
*/

// next/prev strings

STR16 pTraverseStrings[] =
{
  L"Poprzedni",
  L"Następny",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Masz nową pocztę...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Usunąć wiadomość?",
 L"Usunąć wiadomość?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
 	L"Od:",
 	L"Temat:",
 	L"Dzień:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
 	L"Skrzynka odbiorcza",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Księgowy Plus",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Wypłata:", 				// credit (subtract from) to player's account
	L"Wpłata:", 				// debit (add to) to player's account
	L"Wczorajsze wpływy:",
	L"Wczorajsze dodatkowe wpływy:",
	L"Wczorajsze wydatki:",
	L"Saldo na koniec dnia:",
	L"Dzisiejsze wpływy:",
	L"Dzisiejsze dodatkowe wpływy:",
	L"Dzisiejsze wydatki:",
	L"Saldo dostępne:",
	L"Przewidywane wpływy:",
	L"Przewidywane saldo:", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
	L"Dzień", 					// the day column
	L"Ma", 				// the credits column
	L"Winien",				// the debits column
	L"Transakcja", 			// transaction type - see TransactionText below
	L"Saldo", 				// balance at this point in time
	L"Strona", 				// page number
	L"Dzień (dni)", 		// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Narosłe odsetki",			// interest the player has accumulated so far
	L"Anonimowa wpłata",
	L"Koszt transakcji", 
	L"Wynajęto -", 				// Merc was hired
	L"Zakupy u Bobby'ego Ray'a", 		// Bobby Ray is the name of an arms dealer
	L"Uregulowanie rachunków w M.E.R.C.",
	L"Zastaw na życie dla - %s", 		// medical deposit for merc
	L"Analiza profilu w IMP", 		// IMP is the acronym for International Mercenary Profiling
	L"Ubezpieczneie dla - %s", 
	L"Redukcja ubezp. dla - %s",
	L"Przedł. ubezp. dla - %s", 				// johnny contract extended
	L"Anulowano ubezp. dla - %s", 
	L"Odszkodowanie za - %s", 		// insurance claim for merc
	L"1 dzień", 				// merc's contract extended for a day
	L"1 tydzień", 				// merc's contract extended for a week
	L"2 tygodnie", 				// ... for 2 weeks
	L"Przychód z kopalni", 
	L"", //String nuked
	L"Zakup kwiatów",
	L"Pełny zwrot zastawu za - %s",
	L"Częściowy zwrot zastawu za - %s",
	L"Brak zwrotu zastawu za - %s",
	L"Zapłata dla - %s",		// %s is the name of the npc being paid
	L"Transfer funduszy do - %s", 			// transfer funds to a merc
	L"Transfer funduszy od - %s", 		// transfer funds from a merc
	L"Samoobrona w - %s", // initial cost to equip a town's militia
	L"Zakupy u - %s.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s wpłacił(a) pieniądze.",
	L"Sprzedano rzecz(y) miejscowym",
	L"Wykorzystanie Placówki", // HEADROCK HAM 3.6	
	L"Utrzymanie Samoobr.", // HEADROCK HAM 3.6
	L"Ransom for released prisoners",	// Flugente: prisoner system TODO.Translate
};

STR16 pTransactionAlternateText[] =
{
	L"Ubezpieczenie dla -", 				// insurance for a merc
	L"Przedł. kontrakt z - %s o 1 dzień.", 				// entend mercs contract by a day
	L"Przedł. kontrakt z - %s o 1 tydzień.",
	L"Przedł. kontrakt z - %s o 2 tygodnie.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Skyriderowi zapłacono %d$", 			// skyrider was paid an amount of money
	L"Skyriderowi trzeba jeszcze zapłacić %d$", 		// skyrider is still owed an amount of money
	L"Skyrider zatankował",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider jest gotów do kolejnego lotu.", // Skyrider was grounded but has been freed
	L"Skyrider nie ma pasażerów. Jeśli chcesz przetransportować najemników, zmień ich przydział na POJAZD/HELIKOPTER.",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
 L"Świetne", 
 L"Dobre",
 L"Stabilne",
 L"Słabe",
 L"Panika",
 L"Złe",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s - jego/jej sprzęt jest już w Omercie( A9 ).", 
	L"%s - jego/jej sprzęt jest już w Drassen( B13 ).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Zdrowie",
	L"Energia",
	L"Morale",
	L"Stan",	// the condition of the current vehicle (its "health")
	L"Paliwo",	// the fuel level of the current vehicle (its "energy")
	L"Poison",	// TODO.Translate
	L"Water",		// drink level
	L"Food",		// food level
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Poprzedni najemnik (|S|t|r|z|a|ł|k|a |w |l|e|w|o)", 			// previous merc in the list
	L"Następny najemnik (|S|t|r|z|a|ł|k|a |w |p|r|a|w|o)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"PCP:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Więcej tego nie zobaczysz. Czy na pewno chcesz to zrobić?", 	// do you want to continue and lose the item forever
	L"To wygląda na coś NAPRAWDĘ ważnego. Czy NA PEWNO chcesz to zniszczyć?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"Oddział nie może się przemieszczać, jeśli któryś z najemników śpi.",

//1-5
	L"Najpierw wyprowadź oddział na powierzchnię.",
	L"Rozkazy przemieszczenia? To jest sektor wroga!",
	L"Aby podróżować najemnicy muszą być przydzieleni do oddziału lub pojazdu.",
	L"Nie masz jeszcze ludzi.", 		// you have no members, can't do anything 
	L"Najemnik nie może wypełnić tego rozkazu.",			 		// merc can't comply with your order
//6-10
	L"musi mieć eskortę, aby się przemieszczać. Umieść go w oddziale z eskortą.", // merc can't move unescorted .. for a male
	L"musi mieć eskortę, aby się przemieszczać. Umieść ją w oddziale z eskortą.", // for a female
#ifdef JA2UB
	L"Najemnik nie przybył jeszcze do Tracony!",
#else
	L"Najemnik nie przybył jeszcze do Arulco!",
#endif
	L"Wygląda na to, że trzeba wpierw uregulować sprawy kontraktu.",
	L"Nie można przemieścić najemnika. Trwa nalot powietrzny.",
//11-15
	L"Rozkazy przemieszczenia? Trwa walka!",
	L"Zaatakowały cię dzikie koty, w sektorze %s!",
	L"W sektorze %s znajduje się coś, co wygląda na legowisko dzikich kotów!", 
	L"", 
	L"Baza rakiet Ziemia-Powietrze została przejęta.",
//16-20
	L"%s - kopalnia została przejęta. Twój dzienny przychód został zredukowany do %s.",
	L"Nieprzyjaciel bezkonfliktowo przejął sektor %s.",
	L"Przynajmniej jeden z twoich najemników nie został do tego przydzielony.",
	L"%s nie może się przyłączyć, ponieważ %s jest pełny",
	L"%s nie może się przyłączyć, ponieważ %s jest zbyt daleko.",
//21-25
	L"%s - kopalnia została przejęta przez siły Deidranny!",
	L"Siły wroga właśnie zaatakowały bazę rakiet Ziemia-Powietrze w - %s.",
	L"Siły wroga właśnie zaatakowały - %s.",
	L"Właśnie zauważono siły wroga w - %s.",
	L"Siły wroga właśnie przejęły - %s.",
//26-30
	L"Przynajmniej jeden z twoich najemników nie mógł się położyć spać.",
	L"Przynajmniej jeden z twoich najemników nie mógł wstać.",
	L"Oddziały samoobrony nie pojawią się dopóki nie zostaną wyszkolone.",
	L"%s nie może się w tej chwili przemieszczać.",
	L"Żołnierze samoobrony, którzy znajdują się poza granicami miasta, nie mogą być przeniesieni do innego sektora.",
//31-35
	L"Nie możesz trenować samoobrony w - %s.",
	L"Pusty pojazd nie może się poruszać!",
	L"%s ma zbyt wiele ran by podróżować!",
	L"Musisz wpierw opuścić muzeum!",
	L"%s nie żyje!",
//36-40
	L"%s nie może się zamienić z - %s, ponieważ się porusza",
	L"%s nie może w ten sposób wejśc do pojazdu",
	L"%s nie może się dołączyć do - %s",
	L"Nie możesz kompresować czasu dopóki nie zatrudnisz sobie kilku nowych najemników!",
	L"Ten pojazd może się poruszać tylko po drodze!",
//41-45
	L"Nie można zmieniać przydziału najemników, którzy są w drodze",
	L"Pojazd nie ma paliwa!",
	L"%s jest zbyt zmęczony(na) by podróżować.",
	L"Żaden z pasażerów nie jest w stanie kierować tym pojazdem.",
	L"Jeden lub więcej członków tego oddziału nie może się w tej chwili przemieszczać.",
//46-50
	L"Jeden lub więcej INNYCH członków tego oddziału nie może się w tej chwili przemieszczać.",
	L"Pojazd jest uszkodzony!",
	L"Pamiętaj, że w jednym sektorze tylko dwóch najemników może trenować żołnierzy samoobrony.",
	L"Robot nie może się poruszać bez operatora. Umieść ich razem w jednym oddziale.",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Kliknij ponownie sektor docelowy, aby zatwierdzić trasę podróży, lub kliknij inny sektor, aby ją wydłużyć.",
	L"Trasa podróży zatwierdzona.",
	L"Cel podróży nie został zmieniony.",
	L"Trasa podróży została anulowana.",
	L"Trasa podróży została skrócona.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Kliknij sektor, do którego mają przylatywać najemnicy.",
	L"Dobrze. Przylatujący najemnicy będą zrzucani w %s",
	L"Najemnicy nie mogą tu przylatywać. Przestrzeń powietrzna nie jest zabezpieczona!",
	L"Anulowano. Sektor zrzutu nie został zmieniony.",
	L"Przestrzeń powietrzna nad %s nie jest już bezpieczna! Sektor zrzutu został przesunięty do %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Otwórz wyposażenie (|E|n|t|e|r)",
	L"Zniszcz przedmiot",
	L"Zamknij wyposażenie (|E|n|t|e|r)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Czy %s ma zostawić swój sprzęt w sektorze, w którym się obecnie znajduje (%s), czy w (%s), skąd odlatuje?",
	L"%s wkrótce odchodzi i zostawi swój sprzęt w (%s).",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Czy %s ma zostawić swój sprzęt w sektorze, w którym się obecnie znajduje (%s), czy w (%s), skąd odlatuje? ",
	L"%s wkrótce odchodzi i zostawi swój sprzęt w (%s.",
};


STR16 pMercContractOverStrings[] =
{
	L" zakończył kontrakt więc wyjechał.", 		// merc's contract is over and has departed
	L" zakończyła kontrakt więc wyjechała.", 		// merc's contract is over and has departed
	L" - jego kontrakt został zerwany więc odszedł.", 		// merc's contract has been terminated
	L" - jej kontrakt został zerwany więc odeszła.",		// merc's contract has been terminated
	L"Masz za duży dług wobec M.E.R.C. więc %s odchodzi.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"Nieprawidłowy kod dostępu",
	L"Czy na pewno chcesz wznowić proces określenia profilu?",
	L"Wprowadź nazwisko oraz płeć",
	L"Wstępna kontrola stanu twoich finansów wykazała, że nie stać cię na analizę profilu.",
	L"Opcja tym razem nieaktywna.",
	L"Aby wykonać profil, musisz mieć miejsce dla przynajmniej jednego członka załogi.",
	L"Profil został już wykonany.",
	L"Nie można załadować postaci I.M.P. z dysku.",
	L"Wykorzystałeś już maksymalną liczbę postaci I.M.P.",
	L"Masz już w oddziale trzy postacie I.M.P. tej samej płci.",					//L"You have already the maximum number of I.M.P characters with that gender on your team.",  BYŁo ->>L"You have already three I.M.P characters with the same gender on your team.",
	L"Nie stać cię na postać I.M.P.",
	L"Nowa postać I.M.P. dołączyła do oddziału.",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"O Nas", 			// about the IMP site
	L"ZACZNIJ", 			// begin profiling
	L"Umiejętności", 		// personality section
	L"Atrybuty", 		// personal stats/attributes section
	L"Appearance", 			// changed from portrait
	L"Głos %d", 			// the voice selection
	L"Gotowe", 			// done profiling
	L"Zacznij od początku", 		// start over profiling
	L"Tak, wybieram tą odpowiedź.", 
	L"Tak", 
	L"Nie",
	L"Skończone", 			// finished answering questions
	L"Poprz.", 			// previous question..abbreviated form
	L"Nast.", 			// next question
	L"TAK, JESTEM.", 		// yes, I am certain 
	L"NIE, CHCĘ ZACZĄĆ OD NOWA.", // no, I want to start over the profiling process
	L"TAK",
	L"NIE",
	L"Wstecz", 			// back one page
	L"Anuluj", 			// cancel selection
	L"Tak.",
	L"Nie, Chcę spojrzeć jeszcze raz.",
	L"Rejestr", 			// the IMP site registry..when name and gender is selected
	L"Analizuję...", 			// analyzing your profile results
	L"OK",
	L"Postać", // Change from "Voice"
	L"Brak",
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed 
	L"Po wybraniu twoich cech pora wybrać twoje umiejętności.",
	L"Wybierz twoje atrybuty.",
	L"Aby dokonać prawdziwego profilowania wybież portret, głos i kolory.",
	L"Teraz, po wybraniu wyglądu, przejdź do analizy postaci.",
};

STR16 pFilesTitle[] =
{
	L"Przeglądarka plików",
};

STR16 pFilesSenderList[] =
{
	L"Raport Rozp.", 		// the recon report sent to the player. Recon is an abbreviation for reconissance
	L"Intercept #1", 		// first intercept file .. Intercept is the title of the organization sending the file...similar in function to INTERPOL/CIA/KGB..refer to fist record in files.txt for the translated title
	L"Intercept #2",	   // second intercept file
	L"Intercept #3",			 // third intercept file
	L"Intercept #4", // fourth intercept file
	L"Intercept #5", // fifth intercept file
	L"Intercept #6", // sixth intercept file
};

// Text having to do with the History Log

STR16 pHistoryTitle[] =
{
	L"Historia",
};

STR16 pHistoryHeaders[] =
{
	L"Dzień", 			// the day the history event occurred
	L"Strona", 			// the current page in the history report we are in
	L"Dzień", 			// the days the history report occurs over
	L"Położenie", 			// location (in sector) the event occurred
	L"Zdarzenie", 			// the event label
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
	L"%s najęty(ta) w A.I.M.", 										// merc was hired from the aim site
	L"%s najęty(ta) w M.E.R.C.", 									// merc was hired from the aim site
	L"%s ginie.", 															// merc was killed
	L"Uregulowano rachunki w M.E.R.C.",								// paid outstanding bills at MERC
	L"Przyjęto zlecenie od Enrico Chivaldori",	
	//6-10
	L"Profil IMP wygenerowany",
	L"Podpisano umowę ubezpieczeniową dla %s.", 				// insurance contract purchased
	L"Anulowano umowę ubezpieczeniową dla %s.", 				// insurance contract canceled
	L"Wypłata ubezpieczenia za %s.", 							// insurance claim payout for merc
	L"Przedłużono kontrakt z: %s o 1 dzień.", 						// Extented "mercs name"'s for a day
	//11-15
	L"Przedłużono kontrakt z: %s o 1 tydzień.", 					// Extented "mercs name"'s for a week
	L"Przedłużono kontrakt z: %s o 2 tygodnie.", 					// Extented "mercs name"'s 2 weeks
	L"%s zwolniony(na).", 													// "merc's name" was dismissed.
	L"%s odchodzi.", 																		// "merc's name" quit.
	L"przyjęto zadanie.", 															// a particular quest started
	//16-20
	L"zadanie wykonane.",
	L"Rozmawiano szefem kopalni %s",									// talked to head miner of town
	L"Wyzwolono - %s",
	L"Użyto kodu Cheat",
	L"Żywność powinna być jutro w Omercie",
	//21-25
	L"%s odchodzi, aby wziąć ślub z Darylem Hickiem",
	L"Wygasł kontrakt z - %s.",
	L"%s zrekrutowany(na).",
	L"Enrico narzeka na brak postępów",
	L"Walka wygrana",
	//26-30
	L"%s - w kopalni kończy się ruda",
	L"%s - w kopalni skończyła się ruda",
	L"%s - kopalnia została zamknięta",
	L"%s - kopalnia została otwarta",
	L"Informacja o więzieniu zwanym Tixa.",
	//31-35
	L"Informacja o tajnej fabryce broni zwanej Orta.",
	L"Naukowiec w Orcie ofiarował kilka karabinów rakietowych.",
	L"Królowa Deidranna robi użytek ze zwłok.",
	L"Frank opowiedział o walkach w San Monie.",
	L"Pewien pacjent twierdzi, że widział coś w kopalni.",
	//36-40
	L"Gość o imieniu Devin sprzedaje materiały wybuchowe.",
	L"Spotkanie ze sławynm eks-najemnikiem A.I.M. - Mike'iem!",
	L"Tony handluje bronią.",
	L"Otrzymano karabin rakietowy od sierżanta Krotta.",
	L"Dano Kyle'owi akt własności sklepu Angela.",
	//41-45
	L"Madlab zaoferował się zbudować robota.",
	L"Gabby potrafi zrobić miksturę chroniącą przed robakami.",
	L"Keith wypadł z interesu.",
	L"Howard dostarczał cyjanek królowej Deidrannie.",
	L"Spotkanie z handlarzem Keithem w Cambrii.",
	//46-50
	L"Spotkanie z aptekarzem Howardem w Balime",
	L"Spotkanie z Perko, prowadzącym mały warsztat.",
	L"Spotkanie z Samem z Balime - prowadzi sklep z narzędziami.",
	L"Franz handluje sprzętem elektronicznym.",
	L"Arnold prowadzi warsztat w Grumm.",
	//51-55
	L"Fredo naprawia sprzęt elektroniczny w Grumm.",
	L"Otrzymano darowiznę od bogatego gościa w Balime.",
	L"Spotkano Jake'a, który prowadzi złomowisko.",
	L"Jakiś włóczęga dał nam elektroniczną kartę dostępu.",
	L"Przekupiono Waltera, aby otworzył drzwi do piwnicy.",
	//56-60
	L"Dave oferuje darmowe tankowania, jeśli będzie miał paliwo.",
	L"Greased Pablo's palms.",
	L"Kingpin trzyma pieniądze w kopalni w San Mona.",
	L"%s wygrał(a) walkę",
	L"%s przegrał(a) walkę",
	//61-65
	L"%s zdyskwalifikowany(na) podczas walki",
	L"Znaleziono dużo pieniędzy w opuszczonej kopalni.",
	L"Spotkano zabójcę nasłanego przez Kingpina.",
	L"Utrata kontroli nad sektorem",				//ENEMY_INVASION_CODE
	L"Sektor obroniony",
	//66-70
	L"Przegrana bitwa",							//ENEMY_ENCOUNTER_CODE
	L"Fatalna zasadzka",						//ENEMY_AMBUSH_CODE
	L"Usunieto zasadzkę wroga",
	L"Nieudany atak",			//ENTERING_ENEMY_SECTOR_CODE
	L"Udany atak!",
	//71-75
	L"Stworzenia zaatakowały",			//CREATURE_ATTACK_CODE
	L"Zabity(ta) przez dzikie koty",			//BLOODCAT_AMBUSH_CODE
	L"Wyrżnięto dzikie koty",
	L"%s zabity(ta)",
	L"Przekazano Carmenowi głowę terrorysty",
	L"Slay odszedł",
	L"Zabito: %s",
};

STR16 pHistoryLocations[] =
{
	L"N/D",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"E-mail",
	L"Sieć",
	L"Finanse",
	L"Personel",
	L"Historia",
	L"Pliki",
	L"Zamknij",
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
	L"Pogrzeby",
	L"Kwiaty",
	L"Ubezpieczenia",
	L"Anuluj",
	L"Encyclopedia",
	L"Briefing Room",
};

STR16 pBookmarkTitle[] =
{
	L"Ulubione",
	L"Aby w przyszłości otworzyć to menu, kliknij prawym klawiszem myszy.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"Ładowanie strony...",
	L"Otwieranie strony...",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"Weź", 			// take money from merc
	L"Daj", 			// give money to merc
	L"Anuluj", 			// cancel transaction
	L"Skasuj", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Transfer $", 		// transfer money to merc -- short form
	L"Atrybuty", 			// view stats of the merc
	L"Wyposażenie", 			// view the inventory of the merc
	L"Zatrudnienie",
};

STR16 sATMText[ ]=
{
	L"Przesłać fundusze?", 		// transfer funds to merc?
	L"OK?", 			// are we certain?
	L"Wprowadź kwotę", 		// enter the amount you want to transfer to merc
	L"Wybierz typ", 		// select the type of transfer to merc
	L"Brak środków", 	// not enough money to transfer to merc
	L"Kwota musi być podzielna przez $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"Błąd",
	L"Serwer nie posiada DNS.",	
	L"Sprawdź adres URL i spróbuj ponownie.",
	L"OK",
	L"Niestabilne połączenie z Hostem. Transfer może trwać dłużej.",
};


STR16 pPersonnelString[] =
{
	L"Najemnicy:", 			// mercs we have
};


STR16 pWebTitle[ ]=
{
	L"sir-FER 4.0",		// our name for the version of the browser, play on company name
};


// The titles for the web program title bar, for each page loaded

STR16 pWebPagesTitles[] =
{
	L"A.I.M.",
	L"A.I.M. Członkowie",
	L"A.I.M. Portrety",		// a mug shot is another name for a portrait
	L"A.I.M. Lista",
	L"A.I.M.",
	L"A.I.M. Weterani",
	L"A.I.M. Polisy",
	L"A.I.M. Historia",
	L"A.I.M. Linki",
	L"M.E.R.C.",
	L"M.E.R.C. Konta",
	L"M.E.R.C. Rejestracja",
	L"M.E.R.C. Indeks",
	L"Bobby Ray's",
	L"Bobby Ray's - Broń",
	L"Bobby Ray's - Amunicja",
	L"Bobby Ray's - Pancerz",
	L"Bobby Ray's - Różne",							//misc is an abbreviation for miscellaneous				
	L"Bobby Ray's - Używane",
	L"Bobby Ray's - Zamówienie pocztowe",
	L"I.M.P.",
	L"I.M.P.",
	L"United Floral Service",
	L"United Floral Service - Galeria",
	L"United Floral Service - Zamówienie",
	L"United Floral Service - Galeria kartek",
	L"Malleus, Incus & Stapes - Brokerzy ubezpieczeniowi",
	L"Informacja",
	L"Kontrakt",
	L"Uwagi",
	L"McGillicutty - Zakład pogrzebowy",
	L"",
	L"Nie odnaleziono URL.",
	L"Bobby Ray's - Ostatnie dostawy",
	L"Encyclopedia",
	L"Encyclopedia - Dane",
	L"Briefing Room",
	L"Briefing Room - Dane",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Pomoc",
	L"Kliknij ponownie Sieć by otworzyć menu Ulubione.",
};

STR16 pLaptopTitles[] =
{
	L"Poczta",
	L"Przeglądarka plików",
	L"Personel",
	L"Księgowy Plus",
	L"Historia",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Śmierć w akcji",
	L"Zwolnienie",
	L"Inny",
	L"Małżeństwo",
	L"Koniec kontraktu",
	L"Rezygnacja",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"Bieżący oddział",
	L"Wyjazdy",
	L"Koszt dzienny:",
	L"Najwyższy koszt:",
	L"Najniższy koszt:",
	L"Śmierć w akcji:",
	L"Zwolnienie:",
	L"Inny:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Najniższy",
	L"Średni",
	L"Najwyższy",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"ZDR",
	L"ZWN",
	L"ZRCZ",
	L"SIŁA",
	L"DOW",
	L"INT",
	L"DOŚW",
	L"STRZ",
	L"MECH",
	L"WYB",
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
	L"Kontrakt",
};

// text that appears on the update panel buttons

STR16 pUpdatePanelButtons[] =
{
	L"Dalej",
	L"Stop",
};

// Text which appears when everyone on your team is incapacitated and incapable of battle

CHAR16 LargeTacticalStr[][ LARGE_STRING_LENGTH ] =
{
	L"Pokonano cię w tym sektorze!",
	L"Wróg nie zna litości i pożera was wszystkich!",
	L"Nieprzytomni członkowie twojego oddziału zostali pojmani!",
	L"Członkowie twojego oddziału zostali uwięzieni.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Wstecz",
	L"Dalej",
	L"Akceptuję",
	L"Skasuj",
};



//Insurance Info
// Text on the buttons on the bottom of the screen

STR16		InsInfoText[] = 
{ 
	L"Wstecz", 
	L"Dalej"
};



//For use at the M.E.R.C. web site. Text relating to the player's account with MERC

STR16			MercAccountText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Autoryzacja",
	L"Strona główna",
	L"Konto #:",
	L"Najemnik",
	L"Dni",
	L"Stawka",	//5
	L"Opłata",
	L"Razem:",
	L"Czy na pewno chcesz zatwierdzić płatność: %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Wstecz",
	L"Dalej",
};

//For use at the M.E.R.C. web site. Text relating a MERC mercenary


STR16			MercInfo[] =
{
	L"Zdrowie",
	L"Zwinność",
	L"Sprawność",
	L"Siła",
	L"Um. dowodz.",
	L"Inteligencja",
	L"Poz. doświadczenia",
	L"Um. strzeleckie",
	L"Zn. mechaniki",
	L"Mat. wybuchowe",
	L"Wiedza medyczna",

	L"Poprzedni",
	L"Najmij",
	L"Następny",
	L"Dodatkowe informacje",
	L"Strona główna",
	L"Najęty",
	L"Koszt:",
	L"Dziennie",
	L"Gear:", //TODO.Translate
	L"Razem:",
	L"Nie żyje",

	L"You have a full team of mercs already.", //TODO.Translate
	L"Weź sprzęt?",
	L"Niedostępny",
	L"Unsettled Bills", //TODO.Translate
	L"Bio", //TODO.Translate
	L"Inv",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Otwórz konto",
	L"Anuluj",
	L"Nie posiadasz konta. Czy chcesz sobie założyć?"
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, założyciel i właściciel",
	L"Aby otworzyć konto naciśnij tu",
	L"Aby zobaczyć konto naciśnij tu",
	L"Aby obejrzeć akta naciśnij tu",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer failed. No funds available.",	// TODO.Translate
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Zakład pogrzebowy McGillicutty, pomaga rodzinom pogrążonym w smutku od 1983.",
	L"Kierownik, były najemnik A.I.M. Murray \'Pops\' McGillicutty jest doświadczonym pracownikiem zakładu pogrzebowego.",
	L"Przez całe życie obcował ze śmiercią, 'Pops' wie jak trudne są te chwile.",
	L"Zakład pogrzebowy McGillicutty oferuje szeroki zakres usług, od duchowego wsparcia po rekonstrukcję silnie zniekształconych zwłok.",
	L"Pozwól by McGillicutty ci pomógł a twój ukochany będzie spoczywał w pokoju.",

	// Text for the various links available at the bottom of the page
	L"WYŚLIJ KWIATY",
	L"KOLEKCJA TRUMIEN I URN",
	L"USŁUGI KREMA- CYJNE",
	L"USŁUGI PLANOWANIA POGRZEBU",
	L"KARTKI POGRZE- BOWE",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Niestety, z powodu śmierci w rodzinie, nie działają jeszcze wszystkie elementy tej strony.",
	L"Przepraszamy za powyższe uniedogodnienie."
};

// Text for the florist Home page

STR16			sFloristText[] = 
{
	//Text on the button on the bottom of the page

	L"Galeria",

	//Address of United Florist

	L"\"Zrzucamy z samolotu w dowolnym miejscu\"",
	L"1-555-POCZUJ-MNIE",
	L"333 Dr Nos, Miasto Nasion, CA USA 90210",
	L"http://www.poczuj-mnie.com",

	// detail of the florist page

	L"Działamy szybko i sprawnie!",	
	L"Gwarantujemy dostawę w dowolny punkt na Ziemi, następnego dnia po złożeniu zamówienia!",
	L"Oferujemy najniższe ceny na świecie!",	
	L"Pokaż nam ofertę z niższą ceną, a dostaniesz w nagrodę tuzin róż, za darmo!",
	L"Latająca flora, fauna i kwiaty od 1981.",
	L"Nasz ozdobiony bombowiec zrzuci twój bukiet w promieniu co najwyżej dziesięciu mil od żądanego miejsca. Kiedy tylko zechcesz!",
	L"Pozwól nam zaspokoić twoje kwieciste fantazje.",
	L"Bruce, nasz światowej renomy projektant bukietów, zerwie dla ciebie najświeższe i najwspanialsze kwiaty z naszej szklarni.",
	L"I pamiętaj, jeśli czegoś nie mamy, możemy to szybko zasadzić!"
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Powrót",
	L"Wyślij",
	L"Skasuj",
	L"Galeria",

	L"Nazwa bukietu:",
	L"Cena:",			//5
	L"Zamówienie numer:",
	L"Czas dostawy",
	L"nast. dnia",
	L"dostawa gdy to będzie możliwe",
	L"Miejsce dostawy",			//10
	L"Dodatkowe usługi",
	L"Zgnieciony bukiet($10)",
	L"Czarne Róże($20)",
	L"Zwiędnięty bukiet($10)",
	L"Ciasto owocowe (jeżeli będzie)($10)",		//15
	L"Osobiste kondolencje:",
	L"Ze względu na rozmiar karteczek, tekst nie może zawierać więcej niż 75 znaków.",
	L"...możesz też przejrzeć nasze",

	L"STANDARDOWE KARTKI",	
	L"Informacja o rachunku",//20

	//The text that goes beside the area where the user can enter their name

	L"Nazwisko:",
};




//Florist Gallery.c

STR16		sFloristGalleryText[] =
{
	//text on the buttons

	L"Poprz.",	//abbreviation for previous
	L"Nast.",	//abbreviation for next

	L"Kliknij wybraną pozycję aby złożyć zamówienie.",
	L"Uwaga: $10 dodatkowej opłaty za zwiędnięty lub zgnieciony bukiet.",

	//text on the button

	L"Główna",
};

//Florist Cards

STR16			sFloristCards[] =
{
	L"Kliknij swój wybór",
	L"Wstecz"
};



// Text for Bobby Ray's Mail Order Site

STR16			BobbyROrderFormText[] = 
{
	L"Formularz zamówienia",				//Title of the page
	L"Ilość",					// The number of items ordered
	L"Waga (%s)",			// The weight of the item
	L"Nazwa",				// The name of the item
	L"Cena",				// the item's weight
	L"Wartość",				//5	// The total price of all of items of the same type
	L"W sumie",				// The sub total of all the item totals added
	L"Transport",		// S&H is an acronym for Shipping and Handling 
	L"Razem",			// The grand total of all item totals + the shipping and handling
	L"Miejsce dostawy",	
	L"Czas dostawy",			//10	// See below
	L"Koszt (za %s.)",			// The cost to ship the items
	L"Ekspres - 24h",			// Gets deliverd the next day
	L"2 dni robocze",			// Gets delivered in 2 days
	L"Standardowa dostawa",			// Gets delivered in 3 days
	L" Wyczyść",//15			// Clears the order page
	L" Akceptuję",			// Accept the order
	L"Wstecz",				// text on the button that returns to the previous page
	L"Strona główna",				// Text on the button that returns to the home page
	L"* oznacza używane rzeczy",		// Disclaimer stating that the item is used
	L"Nie stać cię na to.",		//20	// A popup message that to warn of not enough money
	L"<BRAK>",				// Gets displayed when there is no valid city selected
	L"Miejsce docelowe przesyłki: %s. Potwierdzasz?",		// A popup that asks if the city selected is the correct one
	L"Waga przesyłki*",			// Displays the weight of the package
	L"* Min. Waga",				// Disclaimer states that there is a minimum weight for the package
	L"Dostawy",
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Pistolet",
	L"Pistolet maszynowy",
	L"Karabin maszynowy",
	L"Karabin",
	L"Karabin snajperski",
	L"Karabin bojowy",
	L"Lekki karabin maszynowy",
	L"Strzelba",
	L"Inny",

	// Ammo
	L"Pistolet",
	L"Pistolet maszynowy",
	L"Karabin maszynowy",
	L"Karabin",
	L"Karabin snajperski",
	L"Karabin bojowy",
	L"Lekki karabin maszynowy",
	L"Strzelba",

	// Used
	L"Broń",
	L"Pancerz",
	L"Oporządzenie",
	L"Różne",

	// Armour
	L"Hełmy",
	L"Kamizelki",
	L"Getry ochronne",
	L"Płytki ceramiczne",

	// Misc
	L"Ostrza",
	L"Noże do rzucania",
	L"Blunt W.", // TODO.Translate
	L"Granaty",
	L"Bomby",
	L"Apteczki",
	L"Ekwipunek",
	L"Na twarz",
	L"Oporządzenie",	//LBE Gear
	L"Optics", // Madd: new BR filters		// TODO.Translate
	L"Grip/LAM",
	L"Muzzle",
	L"Stock",
	L"Mag/Trig.",
	L"Other Att.",
	L"Inne",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Zamów",				// Title
	// instructions on how to order
	L"Kliknij wybrane towary. Lewym klawiszem zwiększasz ilość towaru, a prawym zmniejszasz. Gdy już skompletujesz swoje zakupy przejdź do formularza zamówienia.",			

	//Text on the buttons to go the various links

	L"Poprzednia",		// 
	L"Broń", 			//3
	L"Amunicja",			//4
	L"Ochraniacze",			//5
	L"Różne",			//6	//misc is an abbreviation for miscellaneous
	L"Używane",			//7
	L"Następna",
	L"FORMULARZ",
	L"Strona główna",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Twój zespół posiada",//11
	L"szt. broni do której pasuje amunicja tego typu", //12

	//The following lines provide information on the items

	L"Waga:",			// Weight of all the items of the same type
	L"Kal:",			// the caliber of the gun
	L"Mag:",			// number of rounds of ammo the Magazine can hold
	L"Zas:",				// The range of the gun
	L"Siła:",				// Damage of the weapon	
	L"CS:",			// Weapon's Rate Of Fire, acronym ROF
	L"PA:",				// Weapon's Action Points, acronym AP
	L"Ogłuszenie:",			// Weapon's Stun Damage
	L"Ochrona:",		// Armour's Protection
	L"Kamuf.:",			// Armour's Camouflage
	L"Koszt:",			// Cost of the item
	L"Na stanie:",			// The number of items still in the store's inventory
	L"Ilość na zamów.:",		// The number of items on order
	L"Uszkodz.",			// If the item is damaged
	L"Waga:",			// the Weight of the item
	L"Razem:",			// The total cost of all items on order
	L"* Stan: %%",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Przepraszamy za to utrudnienie, ale na jednym zamówieniu może się znajdować tylko " ,//First part
	L" pozycji! Jeśli potrzebujesz więcej, złóż kolejne zamówienie.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Przykro nam. Chwilowo nie mamy tego więcej na magazynie. Proszę spróbować później.",

	//A popup that tells the user that the store is temporarily sold out

	L"Przykro nam, ale chwilowo nie mamy tego towaru na magazynie",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Tu znajdziesz nowości z dziedziny broni i osprzętu wojskowego",
	L"Zaspokoimy wszystkie twoje potrzeby w dziedzinie materiałów wybuchowych",
	L"UŻYWANE RZECZY",

	//Text for the various links to the sub pages

	L"RÓŻNE",
	L"BROŃ",
	L"AMUNICJA",		//5
	L"OCHRANIACZE",

	//Details on the web site

	L"Jeśli MY tego nie mamy, to znaczy, że nigdzie tego nie dostaniesz!",
	L"W trakcie budowy",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"Członkowie A.I.M.",				// Title
	// Title for the way to sort
	L"Sortuj wg:",					

	// sort by...

	L"Ceny",
	L"Doświadczenia",
	L"Um. strzeleckich",
	L"Zn. mechaniki",
	L"Zn. mat. wyb.",
	L"Um. med.",
	L"Zdrowie",
	L"Zwinność",
	L"Sprawność",
	L"Siła",
	L"Um. dowodzenia",
	L"Inteligencja",
	L"Nazwisko",

	//Text of the links to other AIM pages

	L"Portrety najemników",
	L"Akta najemnika",
	L"Pokaż galerię byłych członków A.I.M.",

	// text to display how the entries will be sorted

	L"Rosnąco",
	L"Malejąco",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Poprzednia str.",
	L"Strona główna", 
	L"Przepisy",
	L"Następna str.",
	L"Rezygnuję",
	L"Akceptuję",
};



//Aim Member.c
//The page in which the players hires AIM mercenaries

// Instructions to the user to either start video conferencing with the merc, or to go the mug shot index

STR16			AimMemberText[] =
{
	L"Lewy klawisz myszy",
	L"kontakt z najemnikiem",
	L"Prawy klawisz myszy",
	L"lista portretów",
};

//Aim Member.c
//The page in which the players hires AIM mercenaries

STR16			CharacterInfo[] =
{
	// The various attributes of the merc

	L"Zdrowie",
	L"Zwinność",
	L"Sprawność",
	L"Siła",
	L"Um. dowodzenia",
	L"Inteligencja",
	L"Poziom dośw.",
	L"Um. strzeleckie",
	L"Zn. mechaniki",
	L"Zn. mat. wyb.",
	L"Wiedza med.",				//10

	// the contract expenses' area

	L"Zapłata",
	L"Czas",
	L"1 dzień",
	L"1 tydzień",
	L"2 tygodnie",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Poprzedni",
	L"Kontakt",
	L"Następny",

	L"Dodatkowe informacje",				// Title for the additional info for the merc's bio
	L"Aktywni członkowie",		//20		// Title of the page
	L"Opcjonalne wyposażenie:",				// Displays the optional gear cost
	L"gear",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's //TODO.Translate
	L"Wymagany jest zastaw na życie",			// If the merc required a medical deposit, this is displayed
	L"Zestaw nr 1",				// Text on Starting Gear Selection Button 1
	L"Zestaw nr 2",				// Text on Starting Gear Selection Button 2
	L"Zestaw nr 3",				// Text on Starting Gear Selection Button 3
	L"Zestaw nr 4",				// Text on Starting Gear Selection Button 4
	L"Zestaw nr 5",				// Text on Starting Gear Selection Button 5
};


//Aim Member.c
//The page in which the player's hires AIM mercenaries

//The following text is used with the video conference popup

STR16			VideoConfercingText[] =
{
	L"Wartość kontraktu:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"Jeden dzień",										
	L"Jeden tydzień",
	L"Dwa tygodnie",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Bez sprzętu",
	L"Weź sprzęt",

	// Text on the Buttons

	L"TRANSFER",			// to actually hire the merc
	L"ANULUJ",				// go back to the previous menu
	L"WYNAJMIJ",				// go to menu in which you can hire the merc
	L"ROZŁĄCZ",				// stops talking with the merc
	L"OK",									
	L"NAGRAJ SIĘ",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Wideo konferencja z - ",		
	L"Łączę. . .",

	L"z zastawem"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"TRANSFER ZAKOŃCZONY POMYŚLNIE",	// You hired the merc
	L"PRZEPROWADZENIE TRANSFERU NIE MOŻLIWE",		// Player doesn't have enough money, message 1
	L"BRAK ŚRODKÓW",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"Wynajęto",											
	L"Proszę zostaw wiadomość",
	L"Nie żyje",

	//If you try to hire more mercs than game can support

	L"You have a full team of mercs already.", //TODO.Translate

	L"Nagrana wiadomość",
	L"Wiadomość zapisana",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"A.I.M. Linki",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"A.I.M. Historia",					//Title

	// Text on the buttons at the bottom of the page

	L"Poprzednia str.",
	L"Strona główna", 
	L"Byli członkowie",
	L"Następna str."
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"Ceny",
	L"Doświadczenia",
	L"Um. strzeleckich",
	L"Zn. mechaniki",
	L"Zn. mat. wyb.",
	L"Um. med.",
	L"Zdrowie",
	L"Zwinność",
	L"Sprawność",
	L"Siła",
	L"Um. dowodzenia",
	L"Inteligencja",
	L"Nazwisko",

	// The title of the page, the above text gets added at the end of this text

	L"Członkowie A.I.M. posortowani rosnąco wg %s",
	L"Członkowie A.I.M. posortowani malejąco wg %s",

	// Instructions to the players on what to do

	L"Lewy klawisz",
	L"Wybór najemnika",			//10
	L"Prawy klawisz",
	L"Opcje sortowania",

	// Gets displayed on top of the merc's portrait if they are...

	L"Wyjechał(a)",
	L"Nie żyje",						//14
	L"Wynajęto",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"STRONA 1",
	L"STRONA 2",
	L"STRONA 3",

	L"Byli członkowie A.I.M.",	// Title of the page


	L"OK"			// Stops displaying information on selected merc
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"Znaki A.I.M. i logo A.I.M. są prawnie chronione w większości krajów.",
	L"Więc nawet nie myśl o próbie ich podrobienia.",
	L"Copyright 1998-1999 A.I.M., Ltd. All rights reserved.",

	//Text for an advertisement that gets displayed on the AIM page

	L"United Floral Service",
	L"\"Zrzucamy gdziekolwiek\"",				//10
	L"Zrób to jak należy...",
	L"...za pierwszym razem",
	L"Broń i akcesoria, jeśli czegoś nie mamy, to tego nie potrzebujesz.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"Strona główna",
	L"Członkowie",
	L"Byli członkowie",
	L"Przepisy",
	L"Historia",
	L"Linki",
};



//ShopKeeper Interface
// The shopkeeper interface is displayed when the merc wants to interact with 
// the various store clerks scattered through out the game.

STR16 SKI_Text[ ] = 
{
	L"TOWARY NA STANIE",		//Header for the merchandise available
	L"STRONA",				//The current store inventory page being displayed
	L"KOSZT OGÓŁEM",				//The total cost of the the items in the Dealer inventory area
	L"WARTOŚĆ OGÓŁEM",			//The total value of items player wishes to sell
	L"WYCENA",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSAKCJA",			//Button text which completes the deal. Makes the transaction.
	L"OK",				//Text for the button which will leave the shopkeeper interface.
	L"KOSZT NAPRAWY",			//The amount the dealer will charge to repair the merc's goods
	L"1 GODZINA",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d GODZIN(Y)",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"NAPRAWIONO",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Brak miejsca by zaoferować więcej rzeczy.",	//Message box that tells the user there is no more room to put there stuff
	L"%d MINUT(Y)",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Upuść przedmiot na ziemię.",
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
	L"Weź",					// Take money from the player
	L"Daj",					// Give money to the player
	L"Anuluj",					// Cancel the transfer
	L"Skasuj",					// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Wybierz",			// tells the user to select either to give or take from the merc
	L"Wprowadź kwotę",			// Enter the amount to transfer
	L"Transfer gotówki do najemnika",		// Giving money to the merc
	L"Transfer gotówki od najemnika",		// Taking money from the merc
	L"Brak środków",			// Not enough money to transfer
	L"Saldo",				// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Czy chcesz dołożyć %s ze swojego konta, aby pokryć różnicę?",
	L"Brak środków. Brakuje ci %s",
	L"Czy chcesz przeznaczyć %s ze swojego konta, aby pokryć koszty?",
	L"Poproś o rozpoczęcie transakscji",
	L"Poproś o naprawę wybranych przedmiotów",
	L"Zakończ rozmowę",
	L"Saldo dostępne",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Zapisz grę",
	L"Odczytaj grę",
	L"Wyjście",
	L">>",
	L"<<",
	L"OK",

	//Text above the slider bars
	L"Efekty",
	L"Dialogi",
	L"Muzyka",

	//Confirmation pop when the user selects..
	L"Zakończyć grę i wrócić do głównego menu?",

	L"Musisz włączyć opcję dialogów lub napisów.",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Zapisz grę",
	L"Odczytaj grę",
	L"Anuluj",
	L"Zapisz wybraną",
	L"Odczytaj wybraną",

	L"Gra została pomyślnie zapisana",
	L"BŁĄD podczas zapisu gry!",
	L"Gra została pomyślnie odczytana",
	L"BŁĄD podczas odczytu gry!",

	L"Wersja gry w zapisanym pliku różni się od bieżącej. Prawdopodobnie można bezpiecznie kontynuować. Kontynuować?",
	L"Zapisane pliki gier mogą być uszkodzone. Czy chcesz je usunąć?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.	
#ifdef JA2BETAVERSION
	L"Nieprawidłowa wersja zapisu gry. W razie problemów prosimy o raport. Kontynuować?",
#else
	L"Próba odczytu starszej wersji zapisu gry. Zaktualizować ten zapis i odczytać grę?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Nieprawidłowa wersja zapisu gry. W razie problemów prosimy o raport. Kontynuować?",
#else
	L"Próba odczytu starszej wersji zapisu gry. Zaktualizować ten zapis i odczytać grę?",
#endif

	L"Czy na pewno chcesz nadpisać grę na pozycji %d?",
	L"Chcesz odczytać grę z pozycji",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Brak miejsca na dysku twardym.  Na dysku wolne jest %d MB, a wymagane jest przynajmniej %d MB.",	

	L"Zapisuję",			//When saving a game, a message box with this string appears on the screen

	L"Standardowe uzbrojenie",
	L"Całe mnóstwo broni",
	L"Realistyczna gra",
	L"Elementy S-F",

	L"Stopień trudności",
	L"Platynowy tryb", //Placeholder English

	L"Bobby Ray Quality",//TODO.Translate
	L"Normalne",
	L"Świetne",
	L"Wyśmienite",
	L"Niewiarygodne",

	L"Nowy inwentarz nie działa w rozdzielczości 640x480. Aby z niego korzystać zmień rozdzielczość i spróbuj ponownie.",
	L"Nowy inwentarz nie korzysta z domyślnego folderu 'Data'.",

	L"The squad size from the savegame is not supported by the current screen resolution. Please increase the screen resolution and try again.", // TODO.Translate
	L"Bobby Ray Quantity", //TODO.Translate
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Poziom mapy",
	L"Nie masz jeszcze żołnierzy samoobrony.  Musisz najpierw wytrenować mieszkańców miast.",
	L"Dzienny przychód",
	L"Najmemnik ma polisę ubezpieczeniową", 
    L"%s nie potrzebuje snu.", 
	L"%s jest w drodze i nie może spać", 
	L"%s jest zbyt zmęczony(na), spróbuj trochę później.",
	L"%s prowadzi.",
	L"Oddział nie może się poruszać jeżeli jeden z najemników śpi.",

	// stuff for contracts
	L"Mimo, że możesz opłacić kontrakt, to jednak nie masz gotówki by opłacić składkę ubezpieczeniową za najemnika.",
	L"%s - składka ubezpieczeniowa najemnika będzie kosztować %s za %d dzień(dni). Czy chcesz ją opłacić?",
	L"Inwentarz sektora",
	L"Najemnik posiada zastaw na życie.", 
	
	// other items
	L"Lekarze", // people acting a field medics and bandaging wounded mercs 
	L"Pacjenci", // people who are being bandaged by a medic 
	L"Gotowe", // Continue on with the game after autobandage is complete 
	L"Przerwij", // Stop autobandaging of patients by medics now 
	L"Przykro nam, ale ta opcja jest wyłączona w wersji demo.", // informs player this option/button has been disabled in the demo 
	L"%s nie ma zestawu narzędzi.",
	L"%s nie ma apteczki.",
	L"Brak chętnych ludzi do szkolenia, w tej chwili.",
	L"%s posiada już maksymalną liczbę oddziałów samoobrony.",
	L"Najemnik ma kontrakt na określony czas.",
	L"Kontrakt najemnika nie jest ubezpieczony",
	L"Mapa",		// 24
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"You currently have no Mobile Militia. Return to this view mode once you've recruited some.",
	L"This view shows where your Mobile Militia can and cannot go. GREY = Mobile Militia refuse to go here. RED = Mobile Militia can go here, but you've told them not to. YELLOW = Mobile Militia can enter this sector, but not leave. GREEN = Mobile Militia can go here freely. Right click a Green/Yellow sector to cycle its behavior.",
};


STR16 pLandMarkInSectorString[] =
{
	L"Oddział %d zauważył kogoś w sektorze %s",
	L"Oddział %s zauważył kogoś w sektorze %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Szkolenie oddziału samoobrony będzie kosztowało $", // telling player how much it will cost
	L"Zatwierdzasz wydatek?", // asking player if they wish to pay the amount requested
	L"Nie stać cię na to.", // telling the player they can't afford to train this town
	L"Kontynuować szkolenie samoobrony w - %s (%s %d)?", // continue training this town?
	L"Koszt $", // the cost in dollars to train militia
	L"( T/N )",   // abbreviated yes/no
	L"",	// unused
	L"Szkolenie samoobrony w %d sektorach będzie kosztowało $ %d. %s", // cost to train sveral sectors at once
	L"Nie masz %d$, aby wyszkolić samoobronę w tym mieście.",
	L"%s musi mieć %d% lojalności, aby można było kontynuować szkolenie samoobrony.",
	L"Nie możesz już dłużej szkolić samoobrony w mieście %s.",
	L"Nie masz %d$ i nie możesz trenować tutaj ruchomej samoobrony.", // HEADROCK HAM 3.6: Mobile Militia
	L"Kontynuuj trening ruchomej samoobrony w %s (%s %d)?", // HEADROCK HAM 3.6: Mobile Militia
	L"Trenowanie ruchomej samoobrony w %d sektorach będzie kosztować %d$. %s", // HEADROCK HAM 3.6: Mobile Militia
	L"Trenowanie ruchomej samoobrony będzie kosztować $", // HEADROCK HAM 3.6: Mobile Militia
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
	L"Jednorazowo możesz wypłacić do 20,000$.",
	L"Czy na pewno chcesz wpłacić %s na swoje konto?",
};

STR16	gzCopyrightText[] = 
{
	L"Prawa autorskie należą do (C) 1999 Sir-tech Canada Ltd. Wszelkie prawa zastrzeżone.",
};

//option Text
STR16		zOptionsToggleText[] = 
{
	L"Dialogi",
	L"Wycisz potwierdzenia",
	L"Napisy",
	L"Wstrzymuj napisy",
	L"Animowany dym",
	L"Drastyczne sceny",
	L"Nigdy nie ruszaj mojej myszki!",
	L"Stara metoda wyboru",
	L"Pokazuj trasę ruchu",
	L"Pokazuj chybione strzały",
	L"Potwierdzenia Real-Time",
	L"Najemnik śpi/budzi się",
	L"Używaj systemu metrycznego",
	L"Oświetlenie podczas ruchu",
	L"Przyciągaj kursor do najemników",
	L"Przyciągaj kursor do drzwi",
	L"Pulsujące przedmioty",
	L"Pokazuj korony drzew",
	L"Pokazuj siatkę",
	L"Pokazuj kursor 3D",
	L"Pokazuj szansę na trafienie",
	L"Zamiana kursora granatnika",
	L"Pozwól na przechwałki wrogów", // Changed from "Enemies Drop all Items" - SANDRO
	L"Wysoki kąt strzałów z granatnika",
	L"Pozwól na skradanie się w czasie rzeczywistym", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"Spacja = następny oddział",
	L"Pokazuj cienie przedmiotów",
	L"Pokazuj zasięg broni w polach",
	L"Efekt smugowy dla poj. strzału",
	L"Odgłosy padającego deszczu",
	L"Pokazuj wrony",
	L"Show Soldier Tooltips",
	L"Automatyczny zapis",
	L"Cichy Skyrider",
	L"Rozszerzone Okno Opisu (EDB)",	//Enhanced Description Box
	L"Wymuś tryb turowy",					// add forced turn mode
	L"Stat Progress Bars",					// Show progress towards stat increase		// TODO.Translate
	L"Alternate Strategy-Map Colors",		// Change color scheme of Strategic Map
	L"Alternate bullet graphics",			// Show alternate bullet graphics (tracers) // TODO.Translate
	L"Show Merc Ranks",						// shows mercs ranks		// TODO.Translate
	L"Show Face gear graphics",				// TODO.Translate
	L"Show Face gear icons",
	L"Disable Cursor Swap",		            // Disable Cursor Swap		// TODO.Translate
	L"Quiet Training",						// Madd: mercs don't say quotes while training // TODO.Translate
	L"Quiet Repairing",						// Madd: mercs don't say quotes while repairing //TODO.Translate
	L"Quiet Doctoring",						// Madd: mercs don't say quotes while doctoring //TODO.Translate
	L"Auto Fast Forward AI Turns",			// Automatic fast forward through AI turns		// TODO.Translate

#ifdef ENABLE_ZOMBIES
	L"Allow Zombies",						// Flugente Zombies 1.0
#endif
	L"Enable inventory popups",				// the_bob : enable popups for picking items from sector inv	// TODO.Translate
	L"Mark Remaining Hostiles",				//TODO.Translate
	L"Show LBE Content",					//TODO.Translate
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
	L"Jeśli WŁĄCZONE, w grze pojawiać się będą dialogi.",

	//Mute Confirmation
	L"Jeśli WŁĄCZONE, głosowe potwierdzenia postaci zostaną wyciszone.",

	//Subtitles
	L"Jeśli WŁĄCZONE, pojawiać się będą napisy podczas rozmów z innymi postaciami.",

	//Key to advance speech
	L"Jeśli WŁĄCZONE, napisy pojawiające się podczas dialogów będą znikały dopiero po kliknięciu.",

	//Toggle smoke animation
	L"Jeśli WŁĄCZONE, dym z granatów będzie animowany. Może spowolnić działanie gry.",

	//Blood n Gore
	L"Jeśli WŁĄCZONE, pokazywane będą bardzo drastyczne sceny.",

	//Never move my mouse
	L"Jeśli WŁĄCZONE, kursor nie będzie automatycznie ustawiał się nad pojawiającymi się okienkami dialogowymi.",

	//Old selection method
	L"Jeśli WŁĄCZONE, wybór postaci będzie działał tak jak w poprzednich częściach gry.",

	//Show movement path
	L"Jeśli WŁĄCZONE, będziesz widział trasę ruchu w trybie Real-Time.",

	//show misses
	L"Jeśli WŁĄCZONE, będzie mógł obserwować w co trafiają twoje kule gdy spudłujesz.",
	
	//Real Time Confirmation
	L"Jeśli WŁĄCZONE, każdy ruch najemnika w trybie Real-Time będzie wymagał dodatkowego, potwierdzającego kliknięcia.",

	//Sleep/Wake notification
	L"Jeśli WŁĄCZONE, wyświetlana będzie informacja, że najemnik położył się spać lub wstał i wrócił do pracy.",

	//Use the metric system
	L"Jeśli WŁĄCZONE, gra będzie używała systemu metrycznego.",

	//Merc Lighted movement
	L"Jeśli WŁĄCZONE, teren wokół najemnika będzie oświetlony podczas ruchu. Może spowolnić działanie gry.",

	//Smart cursor
	L"Jeśli WŁĄCZONE, kursor będzie automatycznie ustawiał się na najemnikach gdy znajdzie się w ich pobliżu.",

	//snap cursor to the door
	L"Jeśli WŁĄCZONE, kursor będzie automatycznie ustawiał się na drzwiach gdy znajdzie się w ich pobliżu.",

	//glow items 
	L"Jeśli WŁĄCZONE, przedmioty będą pulsować. ( |I )",

	//toggle tree tops
	L"Jeśli WŁĄCZONE, wyświetlane będą korony drzew. ( |T )",

	//toggle wireframe
	L"Jeśli WŁĄCZONE, wyświetlane będą zarysy niewidocznych ścian. ( |W )",

	L"Jeśli WŁĄCZONE, kursor ruchu wyświetlany będzie w 3D. (|H|o|m|e)",

	// Options for 1.13
	L"Jeśli WŁĄCZONE, kursor będzie pokazywał szansę na trafienie.",
	L"Jeśli WŁĄCZONE, seria z granatnika będzie używała kursora serii z broni palnej.",
	L"Jeśli WŁĄCZONE, to wrogowie będą czasami komentować pewne akcje.", // Changed from Enemies Drop All Items - SANDRO
	L"Jeśli WŁĄCZONE, granatniki będą strzelały pod wysokim kątem. ( |Q )",
	L"Jeśli WŁĄCZONE, zapobiega przejściu do trybu turowego po zauważeniu wroga podczas skradania. Aby wymusić tryb turowy z tą opcją aktywną naciśnij |C|t|r|l+|X. (|C|t|r|l+|S|h|i|f|t+|X)", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Jeśli WŁĄCZONE, |S|p|a|c|j|a wybiera kolejny oddział.",
	L"Jeśli WŁĄCZONE, pokazywane będą cienie przedmiotów.",
	L"Jeśli WŁĄCZONE, zasięg broni będzie wyświetlany w polach.",
	L"Jeśli WŁĄCZONE, pojedynczy strzał będzie z efektem pocisku smugowego",
	L"Jeśli WŁĄCZONE, będziesz słyszał padający deszcz.",
	L"Jeśli WŁĄCZONE, w grze pojawiać się będą wrony.",
	L"Jeśli WŁĄCZONE, wskazanie postaci wroga kursorem i naciśnięcie A|l|t ukaże okienko informacji dodatkowych.",
	L"Jeśli WŁĄCZONE, gra będzie zapisywana każdorazowo po zakończeniu tury gracza.",
	L"Jeśli WŁĄCZONE, Skyrider nie będzie nic mówił.",
	L"Jeśli WŁĄCZONE, gra będzie obciążała procesor w mniejszym stopniu.",
	L"Jeśli WŁĄCZONE i wróg jest obecny, \ntryb turowy jest włączony, \ndopóki sektor nie zostanie oczyszczony (|C|t|r|l+|S|h|i|f|t+|A|l|t+|T).",	// add forced turn mode
	L"Jeśli WŁĄCZONE, pokazuje postęp w doświadczeniu postaci.",
	L"When ON, the Strategic Map will be colored differently based on exploration.",
	L"Jeśli WŁĄCZONE, zastępuje starą animację pocisku nową.",
	L"When ON, ranks will be displayed before merc names in the strategic view.",			// TODO.Translate
	L"When ON, you will see the equipped face gear on the merc portraits.",	// TODO.Translate
	L"When ON, you will see icons for the equipped face gear on the merc portraits in the lower right corner.",
	L"When ON, the cursor will not toggle between exchange position and other actions. Press |x to initiate quick exchange.",	// TODO.Translate
	L"When ON, mercs will not report progress during training.",
	L"When ON, mercs will not report progress during repairing.",	// TODO.Translate
	L"When ON, mercs will not report progress during doctoring.",	// TODO.Translate
	L"When ON, AI turns will be much faster.",		// TODO.Translate

#ifdef ENABLE_ZOMBIES
	L"When ON, zombies will spawn. Beware!",							// allow zombies	// TODO.Translate
#endif
	L"When ON, enables popup boxes that appear when you left click on empty merc inventory slots while viewing sector inventory in mapscreen.",	// TODO.Translate
	L"When ON, approximate locations of the last enemies in the sector are highlighted.",  //TODO.Translate
	L"When ON, show the contents of an LBE item, otherwise show the regular NAS interface.", //TODO.Translate
	L"When ON, inverts mouse wheel directions.",		// TODO.Translate
	L"When multiple mercs are selected, they will try to keep their relative distances while moving. (|C|t|r|l+|S|h|i|f|t+|G)",	// TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Wymuś wszystkie oczekiwane dostawy od Bobby Ray's.",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: When ON, will report the distance each bullet deviates from the\ncenter of the target, taking all NCTH factors into account.",
	L"Kliknij by naprawić błędy w ustawieniach gry.",							// failsafe show/hide option to reset all options
	L"Kliknij by naprawić błędy w ustawieniach gry.",							// a do once and reset self option (button like effect)
	L"Udostępnia tryb debugowania w edytorze map oraz wersji końcowej.",				// allow debugging in release or mapeditor
	L"Przełącz na tryb wyświetlania/ukrycia opcji renderowania debugowego.",						// an example option that will show/hide other options
	L"Wyświetl wymiary wokół kursora myszy.",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};

STR16	gzGIOScreenText[] =
{
	L"POCZĄTKOWE USTAWIENIA GRY",
#ifdef JA2UB
	L"Dialogi Manuel'a",
	L"Off",
	L"On",
#else
	L"Styl gry",
	L"Realistyczny",
	L"S-F",
#endif	
	L"Platynowy", //Placeholder English
	L"Opcje broni",
	L"Mnóstwo",
	L"Standardowe",
	L"Stopień trudności",
	L"Nowicjusz",
	L"Doświadczony",
	L"Ekspert",
	L"SZALONY",
	L"Start",		// TODO.Translate
	L"Anuluj",
	L"Zapis gry",
	L"Zawsze",
	L"W czasie pokoju",
	L"Nie działa w wersji demo",
	L"Bobby Ray Quality", //TODO.Translate
	L"Normalne",
	L"Świetne",
	L"Wyśmienite",
	L"Niewiarygodne",
	L"Ekwipunek / Dodatki",
	L"Nie używany",
	L"Nie używany",
	L"Wczytaj grę MP",
	L"POCZĄTKOWE USTAWIENIA GRY (Only the server settings take effect)",
	// Added by SANDRO
	L"Cechy umiejętności",
	L"Stare",
	L"Nowe",
	L"Maks. liczba IMP",
	L"Jeden",
	L"Dwa",
	L"Trzy",
	L"Cztery",
	L"Pięć",
	L"Sześć",
	L"Wrogowie upuszczają całość ekwipunku",
	L"Wyłączone",
	L"Włączone",
#ifdef JA2UB
	L"Tex and John",
	L"Random",
	L"All",
#else
	L"Liczba terrorystów",
	L"Losowe",
	L"Wszystkie",
#endif	
	L"Tajne składy proni",
	L"Losowe",
	L"Wszystkie",
	L"Szybkość zwiększania dostępności przedmiotów",
	L"Bardzo wolna",
	L"Wolna",
	L"Normala",
	L"Szybka",
	L"Bardzo szybkia",

	L"Stare / Stare",
	L"Nowe / Stare",
	L"Nowe / Nowe",

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
	L"Food System",//TODO.Translate
	L"Bobby Ray Quantity", //TODO.Translate
};

STR16	gzMPJScreenText[] =
{
	L"MULTIPLAYER",
	L"Dołącz",
	L"Uruchom serwer",
	L"Anuluj",
	L"Odśwież",
	L"Nazwa gracza",
	L"IP Serwera",
	L"Port",
	L"Nazwa serwera",
	L"# Plrs",			// ?? if plrs=players then "# graczy"
	L"Wersja",
	L"Typ rozgrywki",
	L"Ping",
	L"Musisz podać nazwę gracza",
	L"Musisz podać odpowiedni numer IP serwera. (np. 84.114.195.239).",
	L"Musisz podać odpowiedni port serwera pomiędzy 1 i 65535.",
};

// TODO.Translate
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
	L"Bagdad",			//Baghdad, Iraq (Suddam Hussein's home)
	L"Drassen",			//The main place in JA2 that you can receive items.  The other towns are dummy names...
	L"Hong Kong",		//Hong Kong, Hong Kong
	L"Bejrut",			//Beirut, Lebanon	(Middle East)
	L"Londyn",			//London, England
	L"Los Angeles",	//Los Angeles, California, USA (SW corner of USA)
	L"Meduna",			//Meduna -- the other airport in JA2 that you can receive items.
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
	L"Miami",				//Miami, Florida, USA (SE corner of USA)
	L"Moskwa",			//Moscow, USSR
	L"Nowy Jork",		//New York, New York, USA
	L"Ottawa",			//Ottawa, Ontario, Canada -- where JA2 was made!
	L"Paryż",				//Paris, France
	L"Trypolis",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokio",				//Tokyo, Japan
	L"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Na pewno? Wartość zero oznacza brak jakichkolwiek umiejętności w tej dziedzinie.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( Maks. 8 znaków )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Analizuję",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Dziękujemy, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"Głos", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Śmierć w akcji",
	L"Zwolnienie",
	L"Inny",
};

// title for program
STR16 pPersTitleText[] =
{
	L"Personel",
};

// paused game strings
STR16 pPausedGameText[] =
{
	L"Gra wstrzymana",
	L"Wznów grę (|P|a|u|s|e)",
	L"Wstrzymaj grę (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Zakończyć grę?",
	L"OK",
	L"TAK",
	L"NIE",
	L"ANULUJ",
	L"NAJMIJ",
	L"LIE",
	L"Brak opisu", //Save slots that don't have a description.
	L"Gra zapisana.",
	L"Gra zapisana.",
	L"Szybki zapis", //10	The name of the quicksave file (filename, text reference)
	L"SaveGame",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"Dzień",
	L"Najemn.",
	L"Wolna pozycja", //An empty save game slot
	L"Demo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"",			//Release build for JA2
	L"strz/min",					//20	Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"min",					//Abbreviation for minute.
	L"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"kul",				//Abbreviation for rounds (# of bullets)
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Strona główna",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"N/D",					//Lowercase acronym for not applicable.
	L"Tymczasem",		//Meanwhile
	L"%s przybył(a) do sektora %s%s", //30	Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Wersja",
	L"Wolna pozycja na szybki zapis",
	L"Ta pozycja zarezerwowana jest na szybkie zapisy wykonywane podczas gry kombinacją klawiszy ALT+S.",
	L"Otwarte",
	L"Zamknięte",
	L"Brak miejsca na dysku twardym.  Na dysku wolne jest %s MB, a wymagane jest przynajmniej %s MB.",
	L"Najęto - %s z A.I.M.", 
	L"%s złapał(a) %s",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s zaaplikował(a) sobie lekarstwo", //'Merc name' has taken the drug
	L"%s nie posiada wiedzy medycznej",//40	'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"Integralność gry została narażona na szwank.",
	L"BŁĄD: Wyjęto płytę CD",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Nie ma miejsca, żeby stąd oddać strzał.",
	
	//Can't change stance due to objects in the way...
	L"Nie można zmienić pozycji w tej chwili.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Upuść",
	L"Rzuć",
	L"Podaj",

	L"%s przekazano do - %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Brak wolnego miejsca, by przekazać %s do - %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" dołączono]",			// 50

	//Cheat modes
	L"Pierwszy poziom lamerskich zagrywek osiągnięty",
	L"Drugi poziom lamerskich zagrywek osiągnięty",

	//Toggling various stealth modes
	L"Oddział ma włączony tryb skradania się.",
	L"Oddział ma wyłączony tryb skradania się.",
	L"%s ma włączony tryb skradania się.",
	L"%s ma wyłączony tryb skradania się.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Dodatkowe siatki włączone.",
	L"Dodatkowe siatki wyłączone.",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Nie można wyjść do góry z tego poziomu...",
	L"Nie ma już niższych poziomów...",		// 60
	L"Wejście na %d poziom pod ziemią...",
	L"Wyjście z podziemii...",

	L"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Automatyczne centrowanie ekranu wyłączone.",
	L"Automatyczne centrowanie ekranu włączone.",
	L"Kursor 3D wyłączony.",
	L"Kursor 3D włączony.",
	L"Oddział %d aktywny.",
	L"%s - Nie stać cię by wypłacić jej/jemu dzienną pensję w wysokości %s.",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Pomiń",				// 70
	L"%s nie może odejść sam(a).",
	L"Utworzono zapis gry o nazwie SaveGame249.sav. W razie potrzeby zmień jego nazwę na SaveGame01..10. Wtedy będzie można go odczytać ze standardowego okna odczytu gry.",
	L"%s wypił(a) trochę - %s",
	L"Przesyłka dotarła do Drassen.",
 	L"%s przybędzie do wyznaczonego punktu zrzutu (sektor %s) w dniu %d, około godziny %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival	
	L"Lista historii zaktualizowana.",
	L"Seria z granatnika używa kursora celowania (dostępny ogień rozproszony)",		//L"Grenade Bursts use Targeting Cursor (Spread fire enabled)", BYŁO -->>L"Grenade Bursts - Using Targeting Cursor (Spread fire enabled)",
	L"Seria z granatnika używa kursora trajektorii (dostępny ogień rozproszony)",		//L"Grenade Bursts use Trajectory Cursor (Spread fire disabled)", BYŁO -->L"Grenade Bursts - Using Trajectory Cursor (Spread fire disabled)",
	L"Enabled Soldier Tooltips", // Changed from Drop All On - SANDRO
	L"Disabled Soldier Tooltips", // Changed from Drop All Off - SANDRO
	L"Granatniki strzelają pod standardowymi kątami",					//L"Grenade Launchers fire at standard angles", BYŁo->>L"Grenade Launchers - Fire at standard angles",
	L"Granatniki strzelają pod wysokimi kątami",						//L"Grenade Launchers fire at higher angles", BYŁo-->>L"Grenade Launchers - Fire at high angles",
	// forced turn mode strings
    L"Forced Turn Mode",
	L"Normal turn mode",
	L"Exit combat mode",
	L"Forced Turn Mode Active, Entering Combat",
	L"Automatyczny zapis został pomyślnie wykonany.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",

	L"Nie możesz używać nowego trybu wyposażenia i nowego systemu dodatków jednocześnie.",
	
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
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"OK",
	L"W górę",
	L"Wybierz wszystko",
	L"W dół",
	L"Anuluj",
};

STR16 pDoctorWarningString[] =
{
	L"%s jest za daleko, aby poddać się leczeniu.",
	L"Lekarze nie mogli opatrzyć wszystkich rannych.",
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
	L"Zajrzyj do A.I.M. i zatrudnij kilku najemników (*Wskazówka* musisz otworzyć laptopa)", // to inform the player to hired some mercs to get things going
#ifdef JA2UB
	L"Jeśli chcesz już udać się do Tracony, kliknij przycisk kompresji czasu, w prawym dolnym rogu ekranu.", // to inform the player to hit time compression to get the game underway
#else
	L"Jeśli chcesz już udać się do Arulco, kliknij przycisk kompresji czasu, w prawym dolnym rogu ekranu.", // to inform the player to hit time compression to get the game underway
#endif
};

STR16 pAntiHackerString[] = 
{
	L"Błąd. Brakuje pliku, lub jest on uszkodzony. Gra zostanie przerwana.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Przeglądanie poczty",
	L"Przeglądanie stron internetowych",
	L"Przeglądanie plików i załączników pocztowych",
	L"Rejestr zdarzeń",
	L"Informacje o członkach oddziału",
	L"Finanse i rejestr transakcji",
	L"Koniec pracy z laptopem",

	//Bottom task bar icons (if they exist):
	L"Masz nową pocztę",
	L"Masz nowe pliki",

	//Bookmarks:
	L"Międzynarodowe Stowarzyszenie Najemników",
	L"Bobby Ray's - Internetowy sklep z bronią",
	L"Instytut Badań Najemników",
	L"Bardziej Ekonomiczne Centrum Rekrutacyjne",
	L"McGillicutty's - Zakład pogrzebowy",
	L"United Floral Service",
	L"Brokerzy ubezpieczeniowi",
	//New Bookmarks
	L"",
	L"Encyklopedia",
	L"Briefing Room",
};


STR16	gzHelpScreenText[] =
{
	L"Zamknij okno pomocy",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Trwa walka. Najemników można wycofać tylko na ekranie taktycznym.",
	L"W|ejdź do sektora, aby kontynuować walkę.",
	L"|Automatycznie rozstrzyga walkę.",
	L"Nie można automatycznie rozstrzygnąć walki, gdy atakujesz.",
	L"Nie można automatycznie rozstrzygnąć walki, gdy wpadasz w pułapkę.",
	L"Nie można automatycznie rozstrzygnąć walki, gdy walczysz ze stworzeniami w kopalni.",
	L"Nie można automatycznie rozstrzygnąć walki, gdy w sektorze są wrodzy cywile.",
	L"Nie można automatycznie rozstrzygnąć walki, gdy w sektorze są dzikie koty.",
	L"TRWA WALKA",
	L"W tym momencie nie możesz się wycofać.",
};

STR16 gzMiscString[] =
{
	L"Żołnierze samoobrony kontynuują walkę bez pomocy twoich najemników...",
	L"W tym momencie tankowanie nie jest konieczne.",
	L"W baku jest %d%% paliwa.",
	L"Żołnierze Deidranny przejęli całkowitą kontrolę nad - %s.",
	L"Nie masz już gdzie zatankować.",
};

STR16	gzIntroScreen[] = 
{
	L"Nie odnaleziono filmu wprowadzającego",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s słyszy %s DŹWIĘK dochodzący z %s.",
	L"%s słyszy %s ODGŁOS RUCHU dochodzący z %s.",
	L"%s słyszy %s ODGŁOS SKRZYPNIĘCIA dochodzący z %s.",
	L"%s słyszy %s PLUSK dochodzący z %s.",
	L"%s słyszy %s ODGŁOS UDERZENIA dochodzący z %s.",
	L"%s słyszy %s ODGŁOS STRZAŁU dochodzący z %s.", // anv: without this, all further noise notifications were off by 1!		// TODO.Translate
	L"%s słyszy %s WYBUCH dochodzący z %s.",
	L"%s słyszy %s KRZYK dochodzący z %s.",
	L"%s słyszy %s ODGŁOS UDERZENIA dochodzący z %s.",
	L"%s słyszy %s ODGŁOS UDERZENIA dochodzący z %s.",
	L"%s słyszy %s ŁOMOT dochodzący z %s.",
	L"%s słyszy %s TRZASK dochodzący z %s.",
	L"",											 // anv: placeholder for silent alarm		// TODO.Translate
	L"%s słyszy czyjś %s GŁOS dochodzący z %s.", // anv: report enemy taunt to player		// TODO.Translate
};

// TODO.Translate
STR16 pTauntUnknownVoice[] =
{
	L"Nieznany głos",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Sortuj według kolumny Imię (|F|1)",
	L"Sortuj według kolumny Przydział (|F|2)",
	L"Sortuj według kolumny Sen (|F|3)",
	L"Sortuj według kolumny Lokalizacja (|F|4)",
	L"Sortuj według kolumny Cel podróży (|F|5)",
	L"Sortuj według kolumny Wyjazd (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"Błąd 404",
	L"Nie odnaleziono strony.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Ostatnie dostawy",
	L"Zamówienie nr ",
	L"Ilość przedmiotów",
	L"Zamówiono:",
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
	L"(wciąż uczy się interpunkcji)",					// Shaun Lyng
	L"(\"Skończone, tylko to poskładam\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(robię się na to za stary)",				// Ian Currie
	L"(i pracuje nad Wizardry 8)",						// Linda Currie
	L"(zmuszony pod bronią do końcowych testów jakości produktu)",			// Eric \"WTF\" Cheng
	L"(Opuścił nas dla Stowarzyszenia na Rzecz Rozsądnych Wynagrodzeń. Ciekawe czemu... )",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(miłośnik zespołu Dead Head i jazzu)",						// Andrew Stacey
	L"(tak naprawdę na imię ma Robert)",							// Scot Loving
	L"(jedyna odpowiedzialna osoba)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(teraz może wrócić do motocrossu)",	// Dave French
	L"(ukradziony z projektu Wizardry 8)",							// Alex Meduna
	L"(zrobił przedmioty i ekrany wczytywania!!)",	// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s skończył(a) naprawiać własne wyposażenie",
	L"%s skończył(a) naprawiać broń i ochraniacze wszystkich członków oddziału",
	L"%s skończył(a) naprawiać wyposażenie wszystkich członków oddziału",
	L"%s skończył(a) naprawiać ciężkie wyposażenie wszystkich członków oddziału",
	L"%s skończył(a) naprawiać średnie wyposażenie wszystkich członków oddziału",
	L"%s skończył(a) naprawiać lekkie wyposażenie wszystkich członków oddziału",
	L"%s skończył(a) naprawiać strój LBE wszystkich członków oddziału",
};

STR16 zGioDifConfirmText[]=
{
	L"Wybrano opcję NOWICJUSZ. Opcja ta jest przeznaczona dla niedoświadczonych graczy, lub dla tych, którzy nie mają ochoty na długie i ciężkie walki. Pamiętaj, że opcja ta ma wpływ na przebieg całej gry. Czy na pewno chcesz grać w trybie Nowicjusz?",
	L"Wybrano opcję DOŚWIADCZONY. Opcja ta jest przenaczona dla graczy posiadających już pewne doświadczenie w grach tego typu. Pamiętaj, że opcja ta ma wpływ na przebieg całej gry. Czy na pewno chcesz grać w trybie Doświadczony?",
	L"Wybrano opcję EKSPERT. Jakby co, to ostrzegaliśmy cię. Nie obwiniaj nas, jeśli wrócisz w plastikowym worku. Pamiętaj, że opcja ta ma wpływ na przebieg całej gry. Czy na pewno chcesz grać w trybie Ekspert?",
	L"Wybrano opcję SZALONY. OSTRZEŻENIE: Nie obwiniaj nas, jeśli wrócisz w malutkich kawałeczkach... Deidranna NAPRAWDę skopie ci tyłek. Mocno. Pamiętaj, że opcja ta ma wpływ na przebieg całej gry. Czy na pewno chcesz grać w trybie SZALONY?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S - nie odnaleziono pliku...",

	//1-5
	L"Robot nie może opuścić sektora bez operatora.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Nie można teraz kompresować czasu.  Poczekaj na fajerwerki!",  

	//'Name' refuses to move.
	L"%s nie chce się przesunąć.",

	//%s a merc name
	L"%s ma zbyt mało energii, aby zmienić pozycję.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s nie ma paliwa i stoi w sektorze %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"GÓRY",
	L"DOŁU",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"Żaden z twoich najemników nie posiada wiedzy medycznej.",
	L"Brak środków medycznych, aby założyć rannym opatrunki.",
	L"Zabrakło środków medycznych, aby założyć wszystkim rannym opatrunki.",
	L"Żaden z twoich najemników nie potrzebuje pomocy medycznej.",
	L"Automatyczne zakładanie opatrunków rannym najemnikom.",
	L"Wszystkim twoim najemnikom założono opatrunki.",

	//14
#ifdef JA2UB
	L"Tracona",
#else
	L"Arulco",
#endif

  L"(dach)",

	L"Zdrowie: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d vs. %d",
	
	L"%s - brak wolnych miejsc!",  //(ex "The ice cream truck is full")

  L"%s nie potrzebuje pierwszej pomocy lecz opieki lekarza lub dłuższego odpoczynku.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s dostał(a) w nogi i upadł(a)!",
	//Name can't speak right now.
	L"%s nie może teraz mówić.",

	//22-24 plural versions 
	L"%d zielonych żołnierzy samoobrony awansowało na weteranów.",
	L"%d zielonych żołnierzy samoobrony awansowało na regularnych żołnierzy.",
	L"%d regularnych żołnierzy samoobrony awansowało na weteranów.",

	//25
	L"Przełącznik",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s dostaje świra!",

	//27-28
	//Messages why a player can't time compress.
	L"Niebezpiecznie jest kompresować teraz czas, gdyż masz najemników w sektorze %s.",
	L"Niebezpiecznie jest kompresować teraz czas, gdyż masz najemników w kopalni zaatakowanej przez robale.",

	//29-31 singular versions 
	L"1 zielony żołnierz samoobrony awansował na weterana.",
	L"1 zielony żołnierz samoobrony awansował na regularnego żołnierza.",
	L"1 regularny żołnierz samoobrony awansował na weterana.",

	//32-34
	L"%s nic nie mówi.",
	L"Wyjść na powierzchnię?",
	L"(Oddział %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s naprawił(a) najemnikowi - %s, jego/jej - %s",

	//36
	L"DZIKI KOT",

	//37-38 "Name trips and falls"
	L"%s potyka się i upada",
	L"Nie można stąd podnieść tego przedmiotu.",

	//39
	L"Żaden z twoich najemników nie jest w stanie walczyć.  Żołnierze samoobrony sami będą walczyć z robalami.",

	//40-43
	//%s is the name of merc.
	L"%s nie ma środków medycznych!",
	L"%s nie posiada odpowiedniej wiedzy, aby kogokolwiek wyleczyć!",
	L"%s nie ma narzędzi!",
	L"%s nie posiada odpowiedniej wiedzy, aby cokolwiek naprawić!",

	//44-45
	L"Czas naprawy",
	L"%s nie widzi tej osoby.",

	//46-48
	L"%s - przedłużka lufy jego/jej broni odpada!",
	L"W tym sektorze może być maks. %d instruktorów szkolących samoobronę.",
  	L"Na pewno?",

	//49-50
	L"Kompresja czasu",
	L"Pojazd ma pełny zbiornik paliwa.",

	//51-52 Fast help text in mapscreen.
	L"Kontynuuj kompresję czasu (|S|p|a|c|j|a)",
	L"Zatrzymaj kompresję czasu (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s odblokował(a) - %s",
	L"%s odblokował(a) najemnikowi - %s, jego/jej - %s",

	//55 
	L"Nie można kompresować czasu, gdy otwarty jest inwentarz sektora.",

	L"Nie odnaleziono płyty nr 2 Jagged Alliance 2.",

	L"Przedmioty zostały pomyślnie połączone.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Bieżący/Maks. postęp: %d%%/%d%%",

	L"Eskortować Johna i Mary?",
	
	// 60
	L"Przełącznik aktywowany.",

	L"%s: dodatki do pancerza zostały zniszczone!",
	L"%s wystrzelił(a) %d pocisk(ów) więcej niż to było zamierzone!",
	L"%s wystrzelił(a) 1 pocisk(ów) więcej niż to było zamierzone!",

	L"You need to close the item description box first!",	// TODO.Translate

	L"Cannot compress time - hostile civilians and/or bloodcats are in this sector.", // 65	// TODO.Translate
};

STR16 gzCWStrings[] = 
{
	L"Wezwać posiłki do %s z przyległych sektorów?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Miejsce: %d\n",
	L"%s|Jasność: %d / %d\n",
	L"%s|Odległość do |Celu: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Rozkazy: %d\n",
	L"%s|Postawa: %d\n",
	L"%s|Aktualne |P|A: %d\n",
	L"%s|Aktualne |Zdrowie: %d\n",
	L"%s|Current |Breath: %d\n",	// TODO.Translate
	L"%s|Current |Morale: %d\n",
	L"%s|Current |S|hock: %d\n",//TODO.Translate
	L"%s|Current |S|uppression Points: %d\n",//TODO.Translate
	// Full info
	L"%s|Hełm: %s\n",
	L"%s|Kamizelka: %s\n",
	L"%s|Getry ochronne: %s\n",
	// Limited, Basic
	L"|Pancerz: ",
	L"hełm ",
	L"kamizelka ",
	L"getry ochronne",
	L"używane",
	L"brak pancerza",
	L"%s|N|V|G: %s\n",
	L"brak NVG",
	L"%s|Maska |Gazowa: %s\n",
	L"brak maski gazowej",
	L"%s|Pozycja |Głowy |1: %s\n",
	L"%s|Pozycja |Głów |2: %s\n",
	L"\n(w plecaku) ",
	L"%s|Broń: %s ",
	L"brak broni",
	L"rewolwer",
	L"SMG",
	L"karabin",
	L"MG",
	L"strzelba",
	L"nóż",
	L"broń Ciężka",
	L"brak hełmu",
	L"brak kamizelki",
	L"brak getrów ochronnych",
	L"|Pancerz: %s\n", 
	 // Added - SANDRO
	L"%s|Umiejętność 1: %s\n",
	L"%s|Umiejętność 2: %s\n", 
	L"%s|Umiejętność 3: %s\n",
};

STR16 New113Message[] =
{
	L"Nadeszła burza.",
	L"Burza skończyła się.",
	L"Rozpadał się deszcz.",
	L"Deszcz przestał padać.",
	L"Uważaj na snajperów...",
	L"Ogień dławiący!",	
	L"BRST",
	L"AUTO",
	L"GL",
	L"GL BRST",
	L"GL AUTO",
	L"UB",			// TODO.Translate
	L"UBRST",
	L"UAUTO",
	L"BAYONET",
	L"Snajper!",
	L"Nie można podzielić pieniędzy z powodu przedmiotu na kursorze.",
	L"Przybycie nowych rekrutów zostało przekierowane do sektora %s , z uwagi na to, że poprzedni punkt zrzutu w sektorze %s jest zajęty przez wroga.",
	L"Przedmiot usunięty",
	L"Usunięto wszystkie przedmioty tego typu",
	L"Przedmiot sprzedany",
	L"Wszystkie przedmioty tego typu sprzedane",
	L"Sprawdź swoje gogle",
	// Real Time Mode messages
	L"W trakcie walki",
	L"Brak wrogów w zasięgu wzroku",
	L"Wyłączone skradanie w czasie rzeczywistym",
	L"Włączone skradanie w czasie rzeczywistym",
	//L"Spotkano wroga! (Ctrl + x by uruchomić tryb turowy)",
	L"Spotkano wroga!", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s dokonał(a) udanej kradzieży!",
	L"%s nie posiada wystarczającej liczby PA by ukraść wszystkie zaznaczone przedmioty.",
	L"Chcesz operować %s zanim użyjesz bandaży? (możesz przywrócić około %i punktów zdrowia.)",
	L"Chcesz operować %s zanim użyjesz bandaży? (możesz przywrócić około %i punktów zdrowia.)",
	L"Czy chcesz najpierw operować przed użyciem bandaży? (%i pacjent(ów))",
	L"Czy chcesz najpierw operować pacjenta?",
	L"Czy podczas udzielania pierwszej pomocy wykonywać niezbędne operacje?",
	L"operacja na %s zakończona.",
	L"%s otrzymuje trafienie w korpus i traci punkt maksymalnego zdrowia!",
	L"%s otrzymuje trafienie w korpus i traci %d punktów maksymalnego zdrowia!",
	L"%s odzyskał(a) utracony punkt %s",
	L"%s odzyskał(a) %d utraconych punktów %s",
	L"Twoja umiejętność zwiadowcy uchroniła cię przed zasadzką!",
	L"Twoja umiejętność zwiadowcy pozwoliła ci ominąć stado krwawych kotów!",
	L"%s został trafiony w pachwinę i pada na ziemię!",
	//////////////////////////////////////////////////////////////////////////////
	L"Warning: enemy corpse found!!!",
	L"%s [%d rnds]\n%s %1.1f %s",
	L"Insufficient AP Points! Cost %d, you have %d.",	// TODO.Translate
	L"Hint: %s",	// TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s kuli się ze strachu!",
	L"%s został przyparty do muru!",
	L"%s oddał(a) więcej strzałów niż zamierzał(a)!",
	L"Nie możesz szkolić samoobrony w tym sektorze.",
	L"Samoobrona znajduje %s.",
	L"Nie można szkolić samoobrony, gdy wróg jest w sektorze!", 
	// 6 - 10
	L"%s Brak odpowiednich umiejętności dowodzenia do szkolenia samoobrony.",
	L"W tym sektorze jest dozwolonych nie więcej niż %d instruktorów samoobrony.",
	L"W %s oraz okolicy nie ma miejsca dla nowych oddziałów samoobrony!",
	L"Musisz mieć conajmniej %d jednostek samoobrony w każdym z %s wyzwolonych sektorów zanim będziesz mógł szkolić tu nowe oddziały.",
	L"Nie możesz obsadzić fabryki dopóki w okolicy są wrogie jednostki!",  // not sure
	// 11 - 15
	L"%s zbyt niska inteligencja do obsadzenia tej fabryki.",
	L"%s posiada już maksymalną liczbę personelu.",
	L"Obsadzenie tej fabryki będzie kosztować $%d za godzinę. Chcesz kontynuować?",
	L"Nie posiadasz funduszy potrzebnych na opłacenie wszystkich fabryk. Zapłacono $%d, ale wciąż pozostało $%d do zapłaty. Tubylcy są zaniepokojeni.",
	L"Nie posiadasz funduszy potrzebnych na opłacenie wszystkich fabryk. Pozostało $%d do zapłaty. Tubylcy są zaniepokojeni.",
	// 16 - 20
	L"Zalegasz $%d dla Fabryki i nie posiadasz funduszy umożliwiających spłatę długu!",
	L"Zalegasz $%d dla Fabryki!. Nie możesz przydzielić tego najemnika do fabryki do momentu spłaty długu.",
	L"Zalegasz $%d dla Fabryki!. Czy chcesz spłacić ten dług?",
	L"Niedostępny w tym sektorze",
	L"Dzienne wydatki",
	// 21 - 25
	L"Brak funduszy do opłacenia wykazanych jednostek samoobrony! %d jednostek opuszcza oddziały i wraca do domu.",

	// TODO.Translate
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
};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Niniejszym informuję, iż w związku z dotychczasowymi osiągnięciami Gastona, opłata za jego usługi została podniesiona. Osobiście, nie jestem tymfaktem zaskoczony.  ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Informujemy, iż od chwili obecnej cena za usługi świadczone przez pana Stoggiego wzrosła w związku ze wzrostem jego umiejętności. ± ± Speck T. Kline ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Informujemy, iż nabyte przez Texa doświadczenie upoważnia go do wyższego wynagrodzenia, z tego względu jego wynagrodzenie zostało zwiększone w celu lepszego odzwierciedlenia jego wartości. ± ± Speck T. Kline ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Proszę o zwrócenie uwagi, iż w związku ze wzrostem jakości usług świadczonych przez pana Biggens`a jego pensja także uległa podwyższeniu. ± ± Speck T. Kline ± ",
};

// TODO.Translate
// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"PD z Serwera AIM: Wiadomość od - Victor Kolesnikov",
	L"Hej, tu Monk. Dostałem Twoją wiadomość. Już jestem z powrotem  - jeśli chcesz się skontaktować. ± ± Zadzwoń. ±",

	// Brain
	L"PD z Serwera AIM: Wiadomość od - Janno Allik",
	L"Jestem już gotów do przyjęcia nowego zadania. Wszystko musi mieć swoją porę. ± ± Janno Allik ±",

	// Scream
	L"PD z Serwera AIM: Wiadomość od - Lennart Vilde",
	L"Lennart Vilde jest obecnie dostępny! ±",

	// Henning
	L"PD z Serwera AIM: Wiadomość od - Henning von Branitz",
	L"Otrzymałem Twoją wiadomość, dziękuję. Jeśli chcesz omówić możliwość zatrudnienia, skontaktuj się ze mną za pośrednictwem strony AIM . ± ± Na razie! ± ± Henning von Branitz ±",

	// Luc
	L"PD z Serwera AIM: Wiadomość od - Luc Fabre",
	L"Otrzymałem wiadomość, merci! Chętnie rozważę Twoje propozycje. Wiesz, gdzie mnie znaleźć. ± ± Oczekuję odpowiedzi ±",

	// Laura
	L"PD z Serwera AIM: Wiadomość od - Laura Colin",
	L"Pozdrowienia! Dobrze, że zostawiłeś wiadomość.  Sprawa wygląda interesująco. ± ± Zajrzyj jeszcze raz do AIM. Chętnie poznam więcej informacji. ± ± Wyrazy szacunku ± ± Dr Laura Colin ±",

	// Grace
	L"PD z Serwera AIM: Wiadomość od - Graziella Girelli",
	L"Chciałeś się ze mną skontaktować, ale Ci się nie udało.± ± Miałam spotkanie rodzinne, na pewno to rozumiesz. Teraz mam już dość swojej rodziny i bardzo się ucieszę, jeśli skontaktujesz się ze mną ponownie za pośrednictwem witryny AIM . ± ± Ciao! ±",

	// Rudolf
	L"PD z Serwera AIM: Wiadomość od - Rudolf Steiger",
	L"Wiesz, ile telefonów odbieram każdego dnia? Co drugi baran myśli, że może do mnie wydzwaniać.  ± ± W każdym razie już jestem, jeśli masz dla mnie coś ciekawego. ±",

	// TODO.Translate
	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"FW from AIM Server: Message about merc availability",
	L"I got your message. Waiting for your call. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Snajper: Sokole oczy! Możesz odstrzelić skrzydła muszce ze stu jardów. ± ",
	// Camouflage
	L"Kamuflaż: Przy tobie nawet krzaki wyglądają na sztuczne! ± ",	
	// SANDRO - new strings for new traits added
	// Ranger
	L"Strażnik: To ty jesteś tym z pustyni Teksasu! ± ",	
	// Gunslinger
	L"Rewolwerowiec: Z pistoletem lub dwoma, możesz być tak śmiercionośny jak Billy Kid! ± ",
	// Squadleader
	L"Przywódca: Urodzony dowódca i szef, jesteś naprawdę grubą rybą bez kitu! ± ",
	// Technician
	L"Mechanik: Naprawa sprzętu, rozbrajanie pułapek, podkładanie bomb, to twoja działka! ± ",
	// Doctor
	L"Chirurg: Możesz przeprowadzić skomplikowaną operację przy użyciu scyzoryka i gumy do żucia! ± ",
	// Athletics
	L"Atleta: Kondycji mógłby ci pozazdrościć niejeden maratończyk! ± ",
	// Bodybuilding
	L"Budowa ciała: Ta ogromna muskularna postać, której nie sposób przeoczyć, to w rzeczy samej ty! ± ",
	// Demolitions
	L"Materiały wybuchowe: Potrafisz wysadzić miasto przy użyciu standardowego wyposażenia kuchni! ± ",
	// Scouting
	L"Zwiadowca: Nic nie umknie twej uwadze! ± ",
	// Covert ops
	L"Covert Operations: You make 007 look like an amateur! ± ",		// TODO.Translate
	// Radio Operator
	L"Radio Operator: Your usage of communication devices broaden your team's tactical and strategic skills. ± ",	// TOO.Translate
};

STR16 NewInvMessage[] = 
{
	L"Nie możesz teraz podnieść plecaka.",
	L"Nie ma miejsca, aby położyć tutaj plecak",
	L"Nie znaleziono plecaka",
	L"Zamek błyskawiczny działa tylko podczas walki.",
	L"Nie możesz się przemieszczać, gdy zamek plecaka jest aktywny.",
	L"Na pewno chcesz sprzedać wszystkie przedmioty z tego sektora?",
	L"Na pewno chcesz skasować wszystkie przedmioty z tego sektora?",
	L"Nie można wspinać się z plecakiem",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Inicjacja serwera RakNet...",
	L"Serwer wł., oczekiwanie na połączenie",
	L"Musisz teraz połączyć swojego klienta z serwerem, wciskając 2",
	L"Serwer już działa",
	L"Wł. nie powiodło się. Przerwanie.",
	// 5
	L"%d/%d klientów gotowych na tryb realtime.",
	L"Serwer rozłączony i wył.",
	L"Serwer nie działa",
	L"Ładowanie klientów, czekaj.",
	L"Nie można zmieniać miejsc zrzutu po starcie serwera.",
	// 10
	L"Wysłano '%S' pliku - 100/100",
	L"Zakończono wysyłanie plików do '%S'.",
	L"Rozpoczęto wysyłanie plików do '%S'.",
	L"Use the Airspace view to select a map you wish to play. If you want to change the map you have to do this before clicking the 'Start Game' button.",	// TODO.Translate
};

STR16 MPClientMessage[] =
{
	// 0
	L"Inicjacja klienta RakNet…",
	L"Łączenie z IP: %S ...",
	L"Otrzymano ustawienia:",
	L"Jesteś już połączony.",
	L"Jesteś już w trakcie łączenia",
	// 5
	L"Klient #%d - '%S' wynajął '%s'.",
	L"Klient #%d - '%S' has hired another merc.",
	L"Gotowy! Wszystkich gotowych - %d/%d.",
	L"Nie jesteś już gotowy. Gotowych - %d/%d.",
	L"Początek bitwy...",
	// 10
	L"Klient #%d - '%S' jest gotów. Gotowych - %d/%d.",
	L"Klient #%d - '%S' nie jest już gotowy. Gotowych - %d/%d",
	L"Jesteś gotów. Czekanie na pozostałych… Naciśnij OK., jeśli już nie jesteś gotów.",
	L"Zaczynajmy już!",
	L"Klient A musi działać, by zacząć grę.",
	// 15
	L"Nie można zacząć. Brak najemników.",
	L"Czekaj na zgodę serwera, by odblokować laptopa…",
	L"Przerwano",
	L"Koniec przerwania",
	L"Położenie siatki myszy:",
	// 20
	L"X: %d, Y: %d",
	L"Numer siatki %d",
	L"Właściwości serwera",
	L"Ustaw ręcznie stopień nadrzędności serwera: ‘1’ – wł.laptop/rekrut.; ‘2’- wł./ładuj poziom; ‘3’ – odblok. UI; ‘4’ – kończy rozmieszczanie",	
	L"Sektor=%s, MaxKlientów=%d, Max Najem=%d, Tryb_Gry=%d, TenSamNaj=%d, Mnożnik obraż.=%f, TuryCzas=%d, Sek/ruch=%d, Dis BobbyRay=%d, Wył Aim/Merc Ekwip=%d, Wył morale=%d, Test=%d",
	// 25
	L"",
	L"Nowe połączenie Client #%d - '%S'.",
	L"Drużyna: %d.",//not used any more
	L"'%s' (klient %d - '%S') zabity przez '%s' (client %d - '%S')",
	L"Wyrzucono #%d - '%S'",
	// 30
	L"Zacząć turę dla klientów. #1: <Anulować>, #2: %S, #3: %S, #4: %S",
	L"Początek tury dla #%d",
	L"Żądanie trybu realtime…",
	L"Zmieniono w tryb realtime.",
	L"Błąd. Coś poszło nie tak przy przełączaniu.",
	// 35
	L"Odblokować laptopy? (Czy gracze są już podłączeni?)",
	L"Serwer odblokował laptopa. Zaczynaj rekrutować!",
	L"Przerywający",
	L"Nie możesz zmieniać strefy zrzutu, jeśli nie  jesteś serwerem gry.",
	L"Odrzuciłeś ofertę poddania się, gdyż grasz w trybie Multiplayer.",
	// 40
	L"Wszyscy twoi ludzie są martwi!",
	L"Tryb obserwatora wł..",
	L"Zostałeś pokonany!",
	L"Wspinanie wyłączone w MP",
	L"Wynajęto '%s'",
	// 45
	L"Nie możesz zmienić mapy w trakcie dokonywania zakupu",
	L"Mapa zmieniona na '%s'",
	L"Klient '%s' rozłączony, usuwanie z gry",
	L"Zostałeś rozłączony, powrót do głównego menu",
	L"Próba połączenia zakończona niepowodzeniem, kolejna poróba za 5 sekund, %i prób pozostało...",
	//50
	L"Próba połączenia zakończona niepowodzeniem, rezygnacja z kolejnych prób...",
	L"Nie możesz rozpocząć gry dopóki nie są podłączeni inni gracze",
	L"%s : %s",
	L"Wyślij do wszystkich",
	L"Wyślij do sprzymierzeńców",
	// 55
	L"Nie można dołączyć do gry. Ta gra już się rozpoczęła",
	L"%s (drużyna): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Pobrano wszystkie pliki z serwera.",
	L"Pobrano '%S' z serwera",
	L"Rozpoczęto pobieranie '%s' z serwera",
	L"Nie można rozpocząć gry dopóki wszyscy nie zakończą pobierania plików z serwera",
	L"Ten serwer przed rozpoczęciem gry wymaga pobrania zmodyfikowanych plików, czy chcesz kontynuować",
	// 65
	L"Naciśnij 'Gotowe' aby przejść do ekranu taktycznego",
	L"Nie można się połączyć ponieważ twoja wersja %S różni się od wersji %S na serwerze.",
	L"Zabiłeś wrogą jednostkę",
	L"Nie można dołączyć do gry. Ta gra już się rozpoczęła",
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
	L"Pn", //- Północ
	L"Pd", //- Południe
	L"W", //- Wschód
	L"Z", //- Zachód
	L"C",   // "C" - Centralny
};

STR16 gszMPTeamNames[] =
{
	L"Foxtrot",
	L"Bravo",
	L"Delta",
	L"Charlie",
	L"N/D",         // Acronym of Not Applicable
};

STR16 gszMPMapscreenText[] =
{
	L"Tryb gry: ",
	L"Gracze: ",
	L"Mercs each: ",
	L"Nie możesz zmienić początkowego wierzchołka dopóki laptop jest odblokowany.",
	L"Nie możesz zmieniać drużyn dopóki laptop jest odblokowany.",
	L"Losowi najemnicy: ",
	L"T", //if "Y" means Yes here
	L"Poziom trudności:",
	L"Wersja serwera:",
};

STR16 gzMPSScreenText[] =
{
	L"Tabela punktów",
	L"Kontynuuj",
	L"Anuluj",
	L"Gracz",
	L"Zabitych",
	L"Liczba zgonów",
	L"Armia królowej",
	L"Trafienia",
	L"Strzały chybione",
	L"Celność",
	L"Obrażenia zadane",
	L"Obrażenia otrzymane",
	L"Czekaj na połączenie z serwerem aby nacisnąć 'Kontynuuj'",
};

STR16 gzMPCScreenText[] =
{
	L"Anuluj",
	L"Łączenie z serwerem",
	L"Pobieranie ustawień serwera",
	L"Pobieranie plików",
	L"Naciśnij 'ESC' aby anulować lub 'Y' aby wejść na chat",
	L"Naciśnij 'ESC'aby anuować",
	L"Gotowe",
};

STR16 gzMPChatToggleText[] =
{
	L"Wyślij do wszystkich",
	L"Wyślij do sprzymierzeńców",
};

STR16 gzMPChatboxText[] =
{
	L"Multiplayer Chat",
	L"Chat: Naciśnij 'ENTER' aby wysłać lub 'ESC' by anulować",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// Odnośnie starych umiejętności
	L"Na następnej stronie, wybierzesz umiejętności dotyczące twojej specjalizacji jako najemnika. Nie można wybrać więcej niż dwóch różnych umiejętności, albo jednej w stopniu eksperta.",
	
	//L"Możesz także wybrać jedną, albo nawet nie wybrać żadnej umiejętności, co da ci bonus do liczby punktów atrybutów. Zauważ, że elektronika i oburęczność nie mogą być wybrane w stopniu eksperta.",

	// TODO.Translate (added Camouflage)
	L"You can also choose only one or even no traits, which will give you a bonus to your attribute points as a compensation. Note that Electronics, Ambidextrous and Camouflage traits cannot be achieved at expert levels.",

	// Odnośnie nowych/pomniejszych umiejętności
	L"Następny etap dotyczy wybierania umiejętności dotyczących twojej specjalizacji jako najemnika. Na pierwszej stronie, możesz wybrać do %d głównych umiejętności, które reprezentują twoją rolę w zespole. Druga strona to lista pomniejszych cech, które reprezentują twoją osobowość.",
	L"Wybranie więcej niż %d jednocześnie jest niemożliwe. Oznacza to, że jeżeli nie wybierzesz żadnych głównych umiejętności, możesz wybrać %d pomniejsze cechy. Jeśli wybierzesz 2 główne umiejętności (albo jedną zaawansowaną), możesz wybrać tylko %d cechę dodatkową...",
};

STR16 sgAttributeSelectionText[] = 
{
	L"Proszę, wybierz swoje atrybuty fizyczne zgodnie z twoimi prawdziwymi umiejętnościami. Nie możesz podnieść żadnego z powyższych wyników.",
	L"Przegląd atrybutów i umiejętności I.M.P..",
	L"Punkty bonusowe.:",
	L"Poziom startowy",
	// New strings for new traits
	L"Na następnej stronie wybierzesz swoje atrybuty fizyczne i umiejętności. 'Atrybuty' to: zdrowie, zwinność, zręczność, siła oraz inteligencja. Atrybuty nie mogą być niższe niż %d.",
	L"Reszta to 'umiejętności', w przeciwieństwie do atrybutów, mogą być ustawione na zero, co oznacza, że nie masz w nich kompletnie żadnej wprawy.",
	L"Wszystkie punkty są na początku ustawione na minimum. Zauważ, że niektóre atrybuty są ustawione na wartościach, co ma związek z wcześniej wybranymi umiejętnościami. Nie możesz ustawić ich niżej.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"Analiza charakteru I.M.P.",
	L"Analiza charakteru to następny krok w tworzeniu twojego profile. Na pierwszej stronie zostanie ci przedstawiona lista cech charakteru do wybrania. Domyślamy się, że możesz identyfikować się z większą ich ilością, jednak będziesz mógł wybrać tylko jedną. Wybierz taką, z którą czujesz się najbardziej związany.",
	L"Druga strona przedstawia listę niepełnosprawności, na które możesz cierpieć. Jeśli cierpisz na jedną z nich, wybierz ją (wierzymy, że każdy ma tylko jedną taką niepełnosprawność). Bądź szczery, ponieważ to ważne, by poinformować potencjalnych pracodawców o twoim faktycznym stanie zdrowia.",
};

STR16 gzIMPAttitudesText[]=
{
	L"Normalny",
	L"Przyjazny",
	L"Samotnik",
	L"Optymista",
	L"Pesymista",
	L"Agresywny",
	L"Arogancki",
	L"Gruba ryba",
	L"Dupek",
	L"Tchórz",
	L"Nastawienie I.M.P.-a",
};

STR16 gzIMPCharacterTraitText[]=
{
	L"Normalny",
	L"Towarzyski",
	L"Samotnik",
	L"Optymista",
	L"Asertywny",
	L"Intelektualista",
	L"Prymityw",
	L"Agresywny",
	L"Flegmatyk",
	L"Nieustraszony",
	L"Pacyfista",
	L"Podstępny",
	L"Gwiazdor",
	L"Cechy charakteru I.M.P.-a",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"Kolory i sylwetka ciała I.M.P.-a",
	L"Kolory I.M.P.",
	L"Proszę wybrać odpowiednie kolory skóry, włosów i ubrań oraz swoją sylwetkę ciała.",
	L" Proszę wybrać odpowiednie kolory skóry, włosów i ubrań.",
	L"Zaznacz, by używać alternatywnego sposobu trzymania broni.",
	L"\n(Uwaga: będziesz potrzebować do tego dużej siły.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Kolor włosów",
	L"Kolor skóry",
	L"Kolor koszulki",
	L"Kolor spodni",
	L"Normalne ciało",
	L"Duże ciało",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Bez niepełnosprawności",
	L"Nie lubi ciepła",
	L"Nerwowy",
	L"Klaustrofobik",
	L"Nie umie pływać",
	L"Boi się owadów",
	L"Zapominalski",
	L"Psychol",
	L"Deaf",
	L"Shortsighted",
	L"Niepełnosprawności I.M.P.-a",
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
	L"%s nie ma wystarczającej siły, by tego dokonać",
	L"%s nie ma wystarczającej zręczności, by tego dokonać",
	L"%s nie ma wystarczającej zwinności, by tego dokonać",
	L"%s nie ma dość zdrowia, by tego dokonać",
	L"%s nie ma wystarczającej inteligencji, by tego dokonać",
	L"%s nie ma wystarczającej celności, by tego dokonać",
	// 6 - 10
	L"%s nie ma wystarczających um. medycznych, by tego dokonać.",
	L"%s nie ma wystarczających um. mechaniki, by tego dokonać.",
	L"%s nie ma wystarczających um. dowodzenia, by tego dokonać.",
	L"%s nie ma wystarczających um. mat. wyb., by tego dokonać.",
	L"%s nie ma wystarczającego doświadczenia, by tego dokonać.",
	// 11 - 15
	L"%s ma za małe morale, by tego dokonać",
	L"%s nie ma wystarczająco dużo energii, by tego dokonać",
	L"W %s jest zbyt mała lojalność. Mieszkańcy nie pozwolą ci tego zrobić.",
	L"W %s pracuje już zbyt wiele osób.",
	L"W %s zbyt wiele osób wykonuje już to polecenie.",
	// 16 - 20
	L"%s nie znajduje przedmiotów do naprawy.",
	L"%s traci nieco %s, pracując w sektorze %s",
	L"%s traci nieco %s, pracując w %s w sektorze %s",
	L"%s odnosi rany, pracując w sektorze %s i wymaga natychmiastowej pomocy medycznej!",
	L"%s odnosi rany, pracując w %s w sektorze %s i wymaga natychmiastowej pomocy medycznej!",
	// 21 - 25
	L"%s odnosi rany, pracując w sektorze %s. Nie wygląda to jednak bardzo poważnie.",
	L"%s odnosi rany, pracując w %s w sektorze %s. Nie wygląda to jednak bardzo poważnie.",
	L"Mieszkańcy miasta %s wydają się poirytowani tym, że %s przebywa w ich okolicy.",
	L"Mieszkańcy miasta %s wydają się poirytowani tym, że %s pracuje w %s.",
	L"%s po swych działaniach w sektorze %s powoduje spadek poparcia w regionie.",
	// 26 - 30
	L"%s swymi działaniami w %s w %s powoduje spadek poparcia w regionie.",
	L"%s jest w stanie upojenia alkoholowego",
	L"%s ciężko choruje w sektorze %s i przechodzi w stan spoczynku",
	L"%s cięzko choruje i nie może dalej pracować w %s w %s",
	L"%s doznaje ciężkich obrażeń w sektorze %s",	//	%s was injured in sector %s. // <--- This is a log message string.
	// 31 - 35
	L"%s doznaje ciężkich obrażeń w sektorze %s", //<--- This is a log message string.


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Siła",
	L"Zwinność",
	L"Zręczność",
	L"Inteligencja",
	L"Zdrowie",
	L"Umiejętności strzeleckie",   //short: "Um. strzeleckie"
	// 5-10
	L"Umiejętność dowodzenia",   //short: "Um. dowodzenia"
	L"Znajomość mechaniki",   //short: "Zn. mechaniki"
	L"Wiedza medyczna",
	L"Znajomość materiałów wybuchowych",   //short: "Zn. mat. wybuchowych"
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"AMBIENT",
	L"Staff",
	L"Eat",// TODO.Translate
	L"Odpoczywa"
	L"Naprawa ekwipunku",
	L"Naprawa %s",
	L"Naprawa Robota",
	// 6-10
	L"Lekarz",
	L"Pacjent",
	L"Trening siły",
	L"Trening zręczności",
	L"Trening zwinności",
	L"Trening zdrowia",
	// 11-15
	L"Trening um. strzeleckich",
	L"Trening wiedzy medycznej",
	L"Trening zn. mechaniki",
	L"Trening dowodzenia",
	L"Trening zn. mat. wybuchowych",
	// 16-20
	L"Uczeń siła",
	L"Uczeń zręczność",
	L"Uczeń zwinność",
	L"Uczeń zdrowie",
	L"Uczeń um. strzeleckie",
	// 21-25
	L"Uczeń wiedza medyczna",
	L"Uczeń zn. mechaniki",
	L"Uczeń um. dowodzenia",
	L"Uczeń zn. mat. wybuchowych",
	L"Instruktor siła",
	// 26-30
	L"Instruktor zręczność",
	L"Instruktor zwinność",
	L"Instruktor zdrowie",
	L"Instruktor um. strzeleckie",
	L"Instruktor wiedza medyczna",
	// 30-35
	L"Instruktor zn. mechaniki",
	L"Instruktor um. dowodzenia",
	L"Instruktor zn. mat. wybuchowych",
	L"Interrogate Prisoners",	// added by Flugente	TODO.Translate
};

STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 trybie okienkowym wymaga głębi koloru 16-bitowego lub mniej.",

	// TODO.Translate
	// WANNE: Savegame slots validation against INI file
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	L"Mercenary (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Enemy (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Creature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Militia (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civilian (MAX_NUMBER_CIVS_IN_TACTICAL)",
};

// SANDRO - Taunts (here for now, xml for future, I hope)
STR16 sEnemyTauntsFireGun[]=
{
	L"Masz cwelu!",
	L"A masz!",
	L"Nażryj się!",
	L"Jesteście moi!",
	L"Zdychaj!",
	L"Boisz się kurwo?",
	L"To dopiero zaboli!",
	L"Dawaj skurwielu!",
	L"Dawaj! Nie mam całego dnia!",
	L"Chodź do tatusia!",
	L"Zaraz pójdziesz do piachu!",
	L"Wracasz do domu w dębowej jesionce frajerze!",
	L"Chcesz się zabawić?"
	L"Trzeba było zostać w domu kurwo."
	L"Ciota!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"Urządzamy grilla.",
	L"Mam dla ciebie prezent.",
	L"Bum!",
	L"Uśmiech!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Łap!",
	L"A masz!",
	L"Przyszła kryska na Matyska!",
	L"To dla ciebie!",
	L"Hahahaha!",
	L"Łap świnio!",
	L"Uwielbiam to.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"Zedrę ci skalp!",
	L"Chodź do tatusia.",
	L"Pochwal się flakami!",
	L"Porżnę cię na kawałki!",
	L"Skurwysyny!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"Jesteśmy po uszy w gównie...",
	L"Idź do wojska -mówili. Nie ma chuja, nie po to!",
	L"Mam już dość.",
	L"O mój Boże.",
	L"Za to mi nie płacą.",
	L"Nie wytrzymam tego!",
	L"Wrócę z kumplami.",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"Słyszałem to!",
	L"Kto tam?",
	L"Co to było?",
	L"Hej! Co do...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Są tutaj!",
	L"Czas zacząć zabawę."
	L"Liczyłem na to, że to się nie stanie.",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Au!",
	L"Uł...",
	L"To... boli!",
	L"Skurwysyny!",
	L"Pożałujecie... tego.",
	L"Co do..!",
	L"Teraz się... wkurwiłem.",

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
};

STR16 szUDBGenCommonStatsExplanationsTooltipText[]=
{
	L"\n \nDetermines how difficult it is to repair this item.\n \ngreen = Anybody can repair it.\n \nred = This item can't be repaired.\n \nHigher is better.",
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
	L"|P|o|i|s|o|n |M|o|d|i|f|i|e|r",//TODO.translate
	L"|F|o|o|d| |P|o|i|n|t|s",//TODO.translate
	L"|D|r|i|n|k |P|o|i|n|t|s",//TODO.translate
	L"|P|o|r|t|i|o|n |S|i|z|e",//TODO.translate
	L"|M|o|r|a|l|e |M|o|d|i|f|i|e|r",//TODO.translate
	L"|D|e|c|a|y |M|o|d|i|f|i|e|r",//TODO.translate
	L"|B|e|s|t |L|a|s|e|r |R|a|n|g|e",//TODO.translate
	L"|P|e|r|c|e|n|t |R|e|c|o|i|l |M|o|d|i|f|i|e|r",	// 65
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
	L"\n \nThis is the percentage of damage dealt\nby this item that will be poisonous.\n\nUsefulness depends on wether enemy\nhas poison resistance or absorption.",	// TODO.Translate
	L"\n \nA single shot causes this much dirt.\nAmmunition types and attachments can\naffect this value.\n \nLower is better.",	// TODO.Translate
	L"\n \nWhen this item is eaten\nit causes that much poison.\n \nLower is better.",	// TODO.TRANSLATE
	L"\n \nAmount of energy in kcal.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nAmount of water in liter.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nThe percentage of the item\nthat will be eaten at once.\n \nLower is better.",	// TODO.TRANSLATE
	L"\n \nMorale is adjusted by this amount.\n \nHigher is better.",	// TODO.TRANSLATE
	L"\n \nThis item becomes stale over time.\nIf more then 50% is molded it becomes poisoneous.\nThis is the rate at which mold is generated.\nLower is better.",	// TODO.TRANSLATE
	L"\n \nWhen attached to a ranged weapon capable\nof Burst or Autofire modes, this item modifies\nthe weapon's Recoil by the listed percentage.\n \nReducing recoil makes it easier to keep the gun's\nmuzzle pointed at the target during a volley.\n \nLower is better.",
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

//Ja25 UB

STR16	gzNewLaptopMessages[]=
{
	L"Zapytaj o naszą specjalną ofertę",
	L"Temporarily Unavailable",
	L"This special press preview of Jagged Alliance 2: Unfinished Business contains the only first 6 sector maps. The final version of the game will feature many more - please see the included readme file for details.",
};

STR16	zNewTacticalMessages[]=
{
	//L"Odległość od celu (w polach): %d, Jasność = %d/%d",
	L"Nadajnik został podłączony do twojego laptopa.",
	L"Nie mo¿esz zatrudnić %s(a)",
	L"Na określony czas, poni¿sze honorarium pokryje koszt całej misji razem z wyposażeniem zamieszonym poniżej.",
	L"Zatrudnij %s(a) już teraz i weź udział naszej promocji 'jedno honorarium pokrywa wszystko'.  Ponadto w tej niewiarygodnej ofercie cały ekwipunek najemnika dostaniesz za darmo.",
	L"Honorarium",
	L"Ktoś jest w sektorze...",
	//L"Zasięg broni (w polach): %d, Szansa na trafienie: %d procent",
	L"Pokaż osłonę",
	L"Zasięg wzroku",
	L"Nowi rekruci nie mogą tam przybyć.",
	L"Dopóki twój laprop będzie bez nadajnika, nie będziesz mógł zatrudniać nowych członków zespołu.  Możliwe, że to odpowiedni moment żeby odczytać zapisany stan gry lub zacząć grać od nowa!",
	L"%s słyszy dźwięk zgniatanego metalu dochodzący spod ciała Jerrego.  To niestety zabrzmiało jak dźwięk zgniatanej anteny od twojego laptopa.",  //the %s is the name of a merc.  @@@  Modified
	L"Po przejżeniu notatki zostawionej przez zastępce dowódcy Morris'a, %s zauważa pewną możliwość.  Notatka zawiera koordynaty do wystrzelenia pocisków w dwa miasteczka w Arulco.  Są na niej również koordynaty z których te pociski zostaną wystrzelone - wojskowej placówki.",
	L"Przyglądając się panelowi kontrolnemu, %s zauważa, że cyfry można odwrócić, tak, że pociski mogą zniszczyć tą placówkę.  %s musi znaleźć drogę ucieczki. Wydaje się, że winda jest najszybszym rozwiązaniem...",
	L"To jest tryb CZ£OWIEK ZE STALI i nie możesz zapisywać gry gdy wróg jest w sektorze.",	//	@@@  new text
	L"(Nie można zapisywać gry podczas walki)", //@@@@ new text
	L"Kampania ma więcej niż 30 postaci.",							// @@@ new text
	L"Nie można odnaleźć kampanii.",																	// @@@ new text
	L"Kampania: Standardowa ( %S )",																							// @@@ new text
	L"Kampania: %S",																													// @@@ new text
	L"Wybrałeś kampanię %S. Ta kampania została stworzona przez fanów gry. Czy jesteś pewien, że chcesz w nią zagrać?",			// @@@ new text
	L"Żeby użyć edytora powinieneś wcześniej wybrać kampanię inną niż standardowa.",		///@@new
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
	L"%d elites, %d regulars and %d amins were interrogated.",
	L"%d prisoners paid ransom money.",
	L"%d prisoners revealed enemy positions.",
	L"%d elites, %d regulars and %d amins joined our cause.",
	L"Prisoners start a massive riot in %s!",
	L"%d prisoners were sent to %s!",
	L"Prisoners have been released!",
	L"The army now occupies the prison in %s, the prisoners were freed!",
	L"The enemy refuses to surrender!",
	L"The enemy refuses to take you as prisoners - they prefer you dead!",	// TODO.Translate
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

#endif //POLISH
