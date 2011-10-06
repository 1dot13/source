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

// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Usuñ",
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
 L"Prze³¹cz wyœwietlanie graczy", //0
 L"Prze³¹cz wyœwietlanie wrogów",
 L"Prze³¹cz wyœwietlanie zwierz¹t",
 L"Prze³¹cz wyœwietlanie rebeliantów",
 L"Prze³¹cz wyœwietlanie cywili",
 
 L"Gracz",
 L"Wróg",
 L"Stworzenia",
 L"Rebelianci",
 L"Cywile",
 
 L"Szczegó³y", //10
 L"Tryb informacji ogólnych",
 L"Tryb fizyczny",
 L"Tryb atrybutów",
 L"Tryb wyposa¿enia",
 L"Tryb profilu",
 L"Tryb planowania",
 L"Tryb planowania",
 L"Usuñ",
 L"Usuñ zaznaczonego najemnika (DEL).",
 L"Kolejny", //20
 L"ZnajdŸ nastêpnego najemnika (SPACE).",
 L"W³¹cz priorytet egzystencji",
 L"Postaæ ma dostêp do wszystkich zamkniêtych drzwi.",
 
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
 L"S£ABE",
 L"ŒREDNIE",
 L"DOBRE",
 L"ŒWIETNE",
 
 L"MARNE",
 L"S£ABE",
 L"ŒREDNIE",
 L"DOBRE",
 L"ŒWIETNE",
 
 L"Poprzedni zbiór kolorów",//"Previous color set", //50
 L"Nastêpny zbiór kolorów",//"Next color set",
 
 L"Poprzednia budowa cia³a",//"Previous body type",
 L"Nastêpna budowa cia³a",//"Next body type",
 
 L"Ustaw niezgodnoœæ czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoœæ czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoœæ czasu (+ or - 15 minut)",
 L"Ustaw niezgodnoœæ czasu (+ or - 15 minut)",
 
 L"Brak akcji",
 L"Brak akcji",
 L"Brak akcji", //60
 L"Brak akcji",
 
 L"Czyœæ zadanie",
 
 L"Find selected merc",
};

STR16 iEditorBuildingsToolbarText[] =
{
	L"DACHY",  //0
	L"ŒCIANY",
	L"DANE POM.",

	L"Rozmieœæ œciany, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ drzwi, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ dachy, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ okna, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ uszkodzone œciany, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ meble, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ tekstury œcian, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ pod³ogi, u¿ywaj¹c metody wyboru", //10
	L"Rozmieœæ generyczne meble, u¿ywaj¹c metody wyboru",
	L"Rozmieœæ œciany, u¿ywaj¹c metody domyœlnej",
	L"Rozmieœæ drzwi, u¿ywaj¹c metody domyœlnej",
	L"Rozmieœæ okna, u¿ywaj¹c metody domyœlnej",
	L"Rozmieœæ uszkodzone œciany, u¿ywaj¹c metody domyœlnej",
	L"Zablokuj drzwi, lub umieœæ pu³apkê na drzwiach",

	L"Dodaj nowe pomieszczenie",
	L"Edytuj œciany jaskini.",
	L"Usuñ obszar z istniej¹cego budynku.",
	L"Usuñ budynek", //20
	L"Dodaj/zast¹p dach budynku nowym p³askim dachem.",
	L"kopiuj budynek",
	L"Przesuñ budynek",
	L"Rysuj numer pomieszczenia",
	L"Usuñ numer pomieszczenia",

	L"Prze³¹cz tryb wymazywania",
	L"Cofnij ostatni¹ zmianê",
	L"Wybierz rozmiar pêdzla",
};

STR16 iEditorItemsToolbarText[] =
{
	L"Broñ", //0
	L"Amun.",
	L"Pancerz",
	L"LBE",
	L"Mat.Wyb.",
	L"E1",
	L"E2",
	L"E3",
	L"W³¹czniki",
	L"Klucze",
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Dodaj Ÿród³o œwiat³a z otoczenia", //0
	L"Prze³¹cz fa³szywe œwiat³a z otoczenia.",
	L"Dodaj pola wyjœcia (p-klik, aby usun¹æ istniej¹ce).",
	L"Wybierz rozmiar pêdzla",
	L"Cofnij ostatni¹ zmianê",
	L"Prze³¹cz tryb wymazywania",
	L"Okreœl punkt pó³nocny dla celów potwierdzenia.",
	L"Okreœl punkt zachodu dla celów potwierdzenia.",
	L"Okreœl punkt wschodu dla celów potwierdzenia.",
	L"Okreœl punkt po³udnia dla celów potwierdzenia.",
	L"Okreœl punkt œrodka dla celów potwierdzenia.", //10
	L"Okreœl odosobniony punkt dla celów potwierdzenia.",
};

STR16 iEditorOptionsToolbarText[]=
{
	L"Nowa mapa",  //0
	L"Nowa piwnica",
	L"Nowy poziom jaskini",
	L"Zapisz mapê",
	L"Wczytaj mapê",
	L"Wybierz zestaw",
	L"WyjdŸ z trybu edycji do trybu gry",
	L"WyjdŸ z trybu edycji.",
	L"Utwórz mapê radaru",
	L"Kiedy zaznaczone, mapa bêdzie zapisana w oryginalnym formacie JA2.\nTa opcja jest wa¿na przy normalnych wielkoœciach map, których numery siatki nie s¹ (siatki wyjœcia) > 25600.",
	L"Kiedy zaznaczone, wczytana mapa lub nowa, bêdzie powiêkszona automatycznie do wybranych rozmiarów.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Rysuj tekstury terenu", //0
	L"Ustaw tekstury terenu mapy",
	L"Umieœæ brzegi i urwiska",
	L"Rysuj drogi",
	L"Rysuj gruzy",
	L"Umieœæ drzewa i krzewy",
	L"Umieœæ ska³y",
	L"Umieœæ beczki i inne œmieci",
	L"Wype³nij teren",
	L"Cofnij ostatni¹ zmianê",
	L"Prze³¹cz tryb wymazywania", //10
	L"Wybierz rozmiar pêdzla",
	L"Zwiêksz gêstoœæ pêdzla",
	L"Zmniejsz gêstoœæ pêdzla",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Teren", //0
	L"Budynki",
	L"Przedmioty",
	L"Najemnicy",
	L"Dane mapy",
	L"Opcje",
};

//Editor Taskbar Utils.cpp

STR16 iRenderMapEntryPointsAndLightsText[] =
{
	L"Pó³nocny pkt wejœcja", //0
	L"Zachodni pkt wejœcja",
	L"Wschodni pkt wejœcja",
	L"Po³udniowy pkt wejœcja",
	L"Œrodkowy pkt wejœcja",
	L"Odizolowany pkt wejœcja",
	
	L"Brzask",
	L"Noc",
	L"24h",
};

STR16 iBuildTriggerNameText[] =
{
	L"W³¹cznik Paniki1", //0
	L"W³¹cznik Paniki2",
	L"W³¹cznik Paniki3",
	L"W³¹cznik%d",
	 
	L"Akcja nacisku",
	L"Akcja Paniki1",
	L"Akcja Paniki2",
	L"Akcja Paniki3",
	L"Akcja%d",
};

STR16 iRenderDoorLockInfoText[]=
{
	L"Niezablokowane (ID)", //0
	L"Pu³apka eksploduj¹ca",
	L"Pu³apka elektryczna",
	L"Cicha pu³apka",
	L"Cichy alarm",
	L"Super-Elektryczna Pu³apka", //5
	L"Alarm domu publicznego",
	L"Poziom pu³apki %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Zapisz stary format mapy JA2 (v1.12) Version: 5.00 / 25", //0
	L"Brak mapy.",
	L"Plik:  %S, Aktualny zestaw:  %s",
	L"Powiêksz istniej¹c¹ mapê lub now¹.",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"PRZE£¥CZ", //0
	L"WIDOKI",
	L"METODA WYBORU",
	L"METODA DOMYŒLNA",
	L"METODA BUDOWANIA",
	L"Pomieszczenia", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Edytuj atrybuty zamka na mapie (siatka) %d.",
	L"Typ blokady (ID)",
	L"Typ pu³apki",
	L"Poziom pu³apki",
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
	
	L"W³¹cznik Paniki1", //5
	L"W³¹cznik Paniki2",
	L"W³¹cznik Paniki3",
	L"W³¹cznik%d",
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

	L"Promieñ",

	L"Pod ziemi¹",
	L"Poziom œwiat³a",

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
	L"Œiatka",
};
//EditorMercs.cpp
CHAR16 gszScheduleActions[ 11 ][20] =
{
	L"Brak akcji",
	L"Zablokuj drzwi",
	L"Odblokuj drzwi",
	L"Otwórz drzwi",
	L"Zamknij drzwi",
	L"IdŸ do siatki",
	L"Opóœæ sektor",
	L"WejdŸ do sektora",
	L"Pozostañ w sektorze",
	L"IdŸ spaæ",
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
	L"Si³a",
	L"Zwinnoœæ",
	L"Sprawnoœæ",
	L"Charyzma",
	L"M¹droœæ",
	L"Celnoœæ",
	L"Mat. Wybuchowe",
	L"Medycyna",
	L"Scientific",
	L"Poz. doœw.", 
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
	L"NE Czo³g", //5
	L"NW Czo³g",
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
	
	L"Poz. doœw.",
	L"Zdrowie",
	L"Akt. zdrowie",
	L"Celnoœæ",
	L"Si³a",
	L"Zwinnoœæ",
	L"Sprawnoœæ",
	L"Inteligencja",
	L"Zdol. dowodzenia",
	L"Mat. wybuchowe", //20
	L"Zdol. medyczne",
	L"Zdol. mechaniczne",
	L"Morale",
	
	L"Kolor w³osów:",
	L"Kolor skóry:",
	L"Kolor kamizelki:",
	L"Kolor spodni:",
	
	L"LOSOWY",
	L"LOSOWY",
	L"LOSOWY", //30
	L"LOSOWY",
	
	L"Podaj index profilu i naciœnij ENTER. ",
	L"Wszystkie informacje (statystyki, itd.) bêd¹ pobrane z pliku Prof.dat lub MercStartingGear.xml. ",
	L"Jeœli nie chcesz u¿yæ profilu, to zostaw pole puste i naciœnij ENTER. ",
	L"Nie podawaj wartoœci '200'! Wartoœæ '200' nie mo¿e byæ profilem! ",
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
	
	L"Kliknij na siatkê przylegaj¹c¹ do drzwi (%s).", //60
	L"Kliknij na siatkê, gdzie chcesz siê przemieœciæ gdy drzwi s¹ otwarte\\zamkniête (%s).",
	L"Kliknij na siatkê, gdzie chcia³byœ siê przemieœciæ.",
	L"Kliknij na siatkê, gdzie chcia³byœ spaæ. Postaæ po obudzeniu siê automatycznie wróci do oryginalnej pozycji.",
	L" Naciœnij ESC, by wyjœæ z trybu edycji planu.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
	L"Rozkaz patrolu bez punktów poœrednich",
	L"Waypoints with no patrol orders",
};

STR16 pClearCurrentScheduleText[] =
{
	L"Brak akcji",
};

STR16 pCopyMercPlacementText[] =
{
	L"Umiejscowienie nie zosta³o skopiowane, gdy¿ ¿adne nie zosta³o wybrane.",
	L"Umiejscowienie skopiowane.",
};

STR16 pPasteMercPlacementText[] = 
{
	L"Umiejscowienie nie zosta³o wklejone, gdy¿ ¿adne umiejscowienie nie jest zapisane w buforze.",
	L"Umiejscowienie wklejone.",
	L"Umiejscowienie nie zosta³o wklejone, gdy¿ maksymalna liczba umiejscowieñ dla tej dru¿yny jest ju¿ wykorzystana.",
};

//editscreen.cpp
STR16 pEditModeShutdownText[] = 
{
	L"Czy chcesz wyjœæ z trybu edytora do trybu gry ?",
	L"Czy chcesz zakoñczyæ pracê edytora ?",
};

STR16 pHandleKeyboardShortcutsText[] = 
{
	L"Czy jesteœ pewny, ¿e chcesz usun¹æ wszystkie œwiat³a?", //0
	L"Czy jesteœ pewny, ¿e chcesz cofn¹æ plany?",
	L"Czy jesteœ pewny, ¿e chcesz usun¹æ wszystkie plany?",
	
	L"W³¹czono rozmieszczanie elementów przez kilkniêcie",
	L"Wy³¹czono rozmieszczanie elementów przez kilkniêcie",
	
	L"W³¹czono rysowanie wysokiego pod³o¿a", //5
	L"Wy³¹czono rysowanie wysokiego pod³o¿a",
	
	L"Number of edge points: N=%d E=%d S=%d W=%d",
	
	L"W³¹czono losowe rozmieszczanie",
	L"Wy³¹czono losowe rozmieszczanie",
	
	L"Usuñ korony drzew", //10
	L"Poka¿ korony drzew",
	
	L"World Raise Reset",
	
	L"World Raise Set Old",
	L"World Raise Set",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Utworzono mape radaru dla %S", //0
	
	L"Usun¹æ aktualn¹ mapê i rozpocz¹æ nowy poziom piwnicy ?",
	L"Usun¹æ aktualn¹ mapê i rozpocz¹æ nowy poziom jaskini ?",
	L"Usun¹æ aktualn¹ mapê i rozpocz¹æ nowy poziom na wolnym powietrzu ?",
	
	L" Wipe out ground textures? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"HOME", //0
	L"Prze³¹cz fa³szywe œwiat³a z otoczenia ON/OFF",

	L"INSERT",
	L"Prze³¹cz tryb wype³nienia ON/OFF",

	L"BKSPC",
	L"Usuñ ostatni¹  zmianê",

	L"DEL",
	L"Quick erase object under mouse cursor",

	L"ESC",
	L"WyjdŸ z edytora",

	L"PGUP/PGDN", //10
	L"Change object to be pasted",

	L"F1",
	L"Wyœwietl ekran pomocy",

	L"F10",
	L"Zapisz mapê",

	L"F11",
	L"Wczytaj mapê",

	L"+/-",
	L"Change shadow darkness by .01",

	L"SHFT +/-",  //20
	L"Change shadow darkness by .05",

	L"0 - 9",
	L"Change map/tileset filename",

	L"b",
	L"Wybierz wielkoœæ pêdzla",

	L"d",
	L"Rysuj œmieci",

	L"o",
	L"Draw obstacle",

	L"r", //30
	L"Rysuj ska³y",

	L"t",
	L"Toggle trees display ON/OFF",

	L"g",
	L"Rysuj tekstury ziemi",

	L"w",
	L"Rysuj œciany budunków",

	L"e",
	L"Prze³¹cz tryb wymazywania ON/OFF",

	L"h",  //40
	L"Prze³¹cz tryb dachów ON/OFF",
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
	L"Mina klaksonowa",
	L"Mina oœwietlaj¹ca",
	L"Eksplozja gazu £zaw.",
	L"Eksplozja granatu Osza³am.",
	L"Eksplozja granatu dymnego",
	L"Gaz musztardowy",
	L"Mina przeciwpiechotna",
	L"Otwórz drzwi",
	L"Zamknij drzwi",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Ma³a eksplozja",
	L"Œrednia eksplozja",
	L"Du¿a eksplozja",
	L"Otwórz/Zamknij drzwi",
	L"Prze³¹cz wszystkie Akcje1",
	L"Prze³¹cz wszystkie Akcje2",
	L"Prze³¹cz wszystkie Akcje3",
	L"Prze³¹cz wszystkie Akcje4",
	L"Wejœcie do burdelu",
	L"Wyjœcie z burdelu",
	L"Alarm Kingpin'a",
	L"Seks z prostytutk¹",
	L"Poka¿ pokój",
	L"Alarm lokalny",
	L"Alarm globalny",
	L"DŸwiêk klaksonu",
	L"Odbezpiecz drzwi",
	L"Prze³¹cz blokadê (drzwi)",
	L"Usuñ pu³apkê (drzwi)",
	L"Tog pressure items",
	L"Alarm w Museum",
	L"Alarm dzikich kotów",
	L"Du¿y gaz ³zawi¹cy",
};

STR16 pUpdateItemStatsPanelText[] =
{
	L"Chowanie flagi", //0
	L"Brak wybranego przedmiotu.",
	L"Slot dostêpny dla",
	L"losowej generacji.",
	L"Nie mo¿na edytowaæ kluczy.",
	L"Profil identifikacyjny w³aœciciela",
	L"Item class not implemented.",
	L"Slot locked as empty.",
	L"Stan",
	L"Naboje",
	L"Poziom pu³apki", //10
	L"Iloœæ",
	L"Poziom pu³apki",
	L"Stan",
	L"Poziom pu³apki",
	L"Stan",
	L"Iloœæ",
	L"Poziom pu³apki",
	L"Dolary",
	L"Stan",
	L"Poziom pu³apki", //20
	L"Poziom pu³apki",
	L"Tolerancja",
	L"Wyzwalacz alarmu",
	L"Istn. szansa",
	L"B",
	L"R",
	L"S",
};

STR16 pSetupGameTypeFlagsText[] =
{
	L"Przedmiot bêdzie wyœwietlany w trybie Sci-Fi i realistycznym. (|B)", //0
	L"Przedmiot bêdzie wyœwietlany tylko |w trybie realistycznym.",
	L"Przedmiot bêdzie wyœwietlany tylko |w trybie Sci-Fi.",
};

STR16 pSetupGunGUIText[] =
{
	L"T£UMIK", //0
	L"CEL. SNAJP",
	L"CEL. LSER.",
	L"DWÓJNÓG",
	L"KACZY DZIÓB",
	L"GRANATNIK", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"P£YTKI CERAM.", //0
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
	L"Iloœæ przedmiotów:  %d", 
	L"Iloœæ œwiate³:  %d",
	L"Iloœæ punktów wyjœcia:  %d",
	
	L"N",
	L"E",
	L"S",
	L"W",
	L"C",
	L"I", //10
	
	L"Iloœæ pomieszczeñ:  %d",
	L"Ca³kowita populacja :  %d",
	L"Wróg:  %d",
	L"Admin.:  %d",
	
	L"(%d szczegó³owych, profile : %d  -- %d maj¹ priorytet egzystencji)",
	L"¯o³nierze:  %d",
	
	L"(%d szczegó³owych, profile : %d  -- %d maj¹ priorytet egzystencji)",
	L"Gwardia:  %d",
	
	L"(%d szczegó³owych, profile : %d  -- %d maj¹ priorytet egzystencji)",
	L"Cywile:  %d",  //20
	
	L"(%d szczegó³owych, profile : %d  -- %d maj¹ priorytet egzystencji)",
	
	L"Ludzie:  %d",
	L"Krowy:  %d",
	L"Dzikie koty:  %d",
	
	L"Zwierzêta:  %d",
	
	L"Stworzenia:  %d",
	L"Dzikie koty:  %d",
	
	L"Iloœæ zablokowanych drzwi oraz pu³apki zamontowane na drzwiach:  %d",
	L"Zablokowane:  %d",
	L"Pu³apki:  %d", //30
	L"Zablokowane i pu³apki:  %d",
	
	L"Cywile z planami:  %d",
	
	L"Zbyt wiele wyjœæ (siatki) (wiêcej ni¿ 4)...",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  %d (%d dalekie miejsce docelowe)",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  brak",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  poziom 1 u¿ywane miejsca %d siatki",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  poziom 2 -- 1) Qty: %d, 2) Qty: %d",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  poziom 3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d",
	L"Siatka wejœcia-wyjœcia (piwnice itd.) :  poziom 3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d, 4) Qty: %d",
	L"Enemy Relative Attributes:  %d marne, %d s³abe, %d œrednie, %d dobre, %d œwietne (%+d Ca³kowity)", //40
	L"Enemy Relative Equipment:  %d marne, %d s³abe, %d œrednie, %d dobre, %d œwietne (%+d Ca³kowity)",
	L"%d umiejscowienie maj¹ rozkazy patrolu bez ¿adnego zdefiniowanego punktu poœredniego.",
	L"%d umiejscowienia maj¹ punkty poœrednie, ale bez ¿adnych rozkazów.",
	L"%d siatki maj¹ niejasne numery pokoju.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Wróg",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŒWIETLENIA!",
	
	L"Paniki1",
	L"Paniki2",
	L"Paniki3",
	L"Norm1",
	L"Norm2",
	L"Norm3",
	L"Norm4", //10
	L"Akcje nacisku",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŒWIETLENIA!",
	
	L"PRIORITY ENEMY DROPPED ITEMS",
	L"Nic",
	
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŒWIETLENIA!",
	L"NORMAL ENEMY DROPPED ITEMS",
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŒWIETLENIA!",
	L"Nic",
	L"ZBYT WIELE PRZEDMIOTÓW DO WYŒWIETLENIA!",
	L"B£¥D:  Nie mo¿na wczytaæ przedmiotów dla tej mapy. Powód nieznany.", //20	
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
	
	L"SZCZEGÓ£Y PRZEDMIOTÓW -- sektor %s",
	L"Podsumowanie informacji dla sektora %s:", //20
	
	L"Podsumowanie informacji dla sektora %s",
	L"nie egzystuj¹.",
	
	L"Podsumowanie informacji dla sektora %s",
	L"nie egzystuj¹.",
	
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
	
	L"Czy chcesz to teraz zrobiæ (y/n)?",
	
	L"Brak informacji o podsumowaniu.  Anulowano tworzenie.",
	
	L"Siatka",
	L"Postêp", //40
	L"Use Alternate Maps",
	
	L"Podsumowanie",
	L"Przedmioty",
};

STR16 pUpdateSectorSummaryText[] =
{
	L"Analizujê mapê:  %s...",
};

STR16 pSummaryLoadMapCallbackText[] =
{
	L"Wczytujê mapê:  %s",
};

STR16 pReportErrorText[] =
{
	L"Skipping update for %s.  Probably due to tileset conflicts...",
};

STR16 pRegenerateSummaryInfoForAllOutdatedMapsText[] =
{
	L"Generuje informacjê o mapiê",
};

STR16 pSummaryUpdateCallbackText[] =
{
	L"Generujê podsumowanie mapy",
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
	L"%S[%d] pochodzi z domyœlnego zestawu %s (%S)",
	L"Plik:  %S, podindeks:  %d (%S)",
	L"Aktualny zestaw:  %s",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Ma³y", 
 L"Œredni", 
 L"Du¿y", 
 L"B.Du¿y", 
 L"Szerokoœæ: xx", 
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
	L"@T,C144,R134,{;Grafika i dŸwiêki",
	L"@};(Ró¿ne inne mody!)",
	L"@T,C144,R134,{;Przedmioty",
	L"@T,C144,R134,{;Pozostali autorzy",
	L"@};(Wszyscy pozostali cz³onkowie sceny JA którzy nas wsparli!)",
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
//	L"strza³ka", // dart
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
//	L"strza³ka", // dart
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
	L"Tura stworzeñ",
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

	L"%s dosta³(a) w g³owê i traci 1 punkt inteligencji!",
	L"%s dosta³(a) w ramiê i traci 1 punkt zrêcznoœci!",
	L"%s dosta³(a) w klatkê piersiow¹ i traci 1 punkt si³y!",
	L"%s dosta³(a) w nogi i traci 1 punkt zwinnoœci!",
	L"%s dosta³(a) w g³owê i traci %d pkt. inteligencji!",
	L"%s dosta³(a) w ramiê i traci %d pkt. zrêcznoœci!",
	L"%s dosta³(a) w klatkê piersiow¹ i traci %d pkt. si³y!",
	L"%s dosta³(a) w nogi i traci %d pkt. zwinnoœci!",
	L"Przerwanie!",

	// The first %s is a merc's name, the second is a string from pNoiseVolStr,
	// the third is a string from pNoiseTypeStr, and the last is a string from pDirectionStr

	L"", //OBSOLETE
	L"Dotar³y twoje posi³ki!",

	// In the following four lines, all %s's are merc names

	L"%s prze³adowuje.",
	L"%s posiada za ma³o Punktów Akcji!",
	L"%s udziela pierwszej pomocy. (Naciœnij dowolny klawisz aby przerwaæ.)",
	L"%s i %s udzielaj¹ pierwszej pomocy. (Naciœnij dowolny klawisz aby przerwaæ.)",
	// the following 17 strings are used to create lists of gun advantages and disadvantages
	// (separated by commas)
	L"niezawodna",
	L"zawodna",
	L"³atwa w naprawie",
	L"trudna do naprawy",
	L"solidna",
	L"niesolidna",
	L"szybkostrzelna",
	L"wolno strzelaj¹ca",
	L"daleki zasiêg",
	L"krótki zasiêg",
	L"ma³a waga",
	L"du¿a waga",
	L"niewielkie rozmiary",
	L"szybki ci¹g³y ogieñ",
	L"brak mo¿liwoœci strzelania seri¹",
	L"du¿y magazynek",
	L"ma³y magazynek",

	// In the following two lines, all %s's are merc names

	L"%s: kamufla¿ siê star³.",
	L"%s: kamufla¿ siê zmy³.",

	// The first %s is a merc name and the second %s is an item name

	L"Brak amunicji w dodatkowej broni!",
	L"%s ukrad³(a): %s.",

	// The %s is a merc name

	L"%s ma broñ bez funkcji ci¹g³ego ognia.",

	L"Ju¿ masz coœ takiego do³¹czone.",
	L"Po³¹czyæ przedmioty?",

	// Both %s's are item names

	L"%s i %s nie pasuj¹ do siebie.",

	L"Brak",
	L"Wyjmij amunicjê",
	L"Dodatki",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L" %s i %s nie mog¹ byæ u¿ywane jednoczeœnie.",

	L"Element, który masz na kursorze myszy mo¿e byæ do³¹czony do pewnych przedmiotów, poprzez umieszczenie go w jednym z czterech slotów.",
	L"Element, który masz na kursorze myszy mo¿e byæ do³¹czony do pewnych przedmiotów, poprzez umieszczenie go w jednym z czterech slotów. (Jednak w tym przypadku, przedmioty do siebie nie pasuj¹.)",
	L"Ten sektor nie zosta³ oczyszczony z wrogów!",
	L"Wci¹¿ musisz daæ %s %s", 
	L"%s dosta³(a) w g³owê!",
	L"Przerwaæ walkê?",
	L"Ta zmiana bêdzie trwa³a. Kontynuowaæ?",
	L"%s ma wiêcej energii!",
	L"%s poœlizgn¹³(nê³a) siê na kulkach!",
	L"%s nie chwyci³(a) - %s!",    //to by³o - L"%s nie zdo³a³ chwyciæ %s!",		teraz jest - L"%s nie zdo³a³ zabraæ %s z rêki wroga!",
	L"%s naprawi³(a) %s",
	L"Przerwanie dla: ",
	L"Poddaæ siê?",
	L"Ta osoba nie chce twojej pomocy.",
	L"NIE S¥DZÊ!",
  	L"Aby podró¿owaæ helikopterem Skyridera, musisz najpierw zmieniæ przydzia³ najemników na POJAZD/HELIKOPTER.",
	L"%s mia³(a) czas by prze³adowaæ tylko jedn¹ broñ",
	L"Tura dzikich kotów",
	L"ogieñ ci¹g³y",
	L"brak ognia ci¹g³ego",
	L"celna",
	L"niecelna",
	L"broñ samoczynna",
	L"Wróg nie ma przedmiotów, które mo¿na ukraœæ!",
	L"Wróg nie ma ¿adnego przedmiotu w rêce!",
	
	//add new camo string
	L"%s: kamufla¿ pustynny siê star³.",
	L"%s: kamufla¿ pustynny siê zmy³.",
	
	L"%s: kamufla¿ leœny siê star³.",
	L"%s: kamufla¿ leœny siê zmy³.",
	
	L"%s: kamufla¿ miejski siê star³.",
	L"%s: kamufla¿ miejski siê zmy³.",
	
	L"%s: kamufla¿ zimowy siê star³.",
	L"%s: kamufla¿ zimowy siê zmy³.",

	L"Niemo¿esz przydzieliæ %s do tego slotu.",
	L"The %s will not fit in any open slots.",
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
	L"S³u¿ba", // on active duty
	L"Lekarz", // administering medical aid
	L"Pacjent", // getting medical aid
	L"Pojazd", // in a vehicle
	L"Podró¿", // in transit - abbreviated form
	L"Naprawa", // repairing
	L"Praktyka", // training themselves  
	L"Samoobr.", // training a town to revolt 
	L"R.Samoobr.", //training moving militia units
	L"Instruk.", // training a teammate
	L"Uczeñ", // being trained by someone else 
	L"Staff", // operating a strategic facility			// TODO.Translate
	L"Rest", // Resting at a facility					// TODO.Translate
	L"Nie ¿yje", // dead
	L"Obezw³.", // abbreviation for incapacitated
	L"Jeniec", // Prisoner of war - captured
	L"Szpital", // patient in a hospital 
	L"Pusty",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"Samoobrona", // the title of the militia box
	L"Bez przydzia³u", //the number of unassigned militia troops
	L"Nie mo¿esz przemieszczaæ oddzia³ów samoobrony gdy nieprzyjaciel jest w sektorze!",
	L"Czêœæ samoobrony nie zosta³a przydzielona do sektoru. Czy chcesz ich rozwi¹zaæ?",
};


STR16 pMilitiaButtonString[] =
{
	L"Automatyczne", // auto place the militia troops for the player
	L"OK", // done placing militia troops
	L"Rozwi¹¿", // HEADROCK HAM 3.6: Disband militia
};

STR16 pConditionStrings[] = 
{
	L"Doskona³y", //the state of a soldier .. excellent health
	L"Dobry", // good health
	L"Doœæ dobry", // fair health
	L"Ranny", // wounded health	
	L"Zmêczony",//L"Wyczerpany", // tired
	L"Krwawi", // bleeding to death
	L"Nieprzyt.", // knocked out 
	L"Umieraj¹cy", // near death
	L"Nie ¿yje", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"S³u¿ba", // set merc on active duty
	L"Pacjent", // set as a patient to receive medical aid
	L"Pojazd", // tell merc to enter vehicle
	L"Wypuœæ", // let the escorted character go off on their own
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
	L"S³u¿ba",
	L"Lekarz",
	L"Pacjent",
	L"Pojazd",
	L"Podró¿",
	L"Naprawa",
	L"Praktyka",
	L"Trenuje samoobronê",
	L"Training Mobile Militia",	// TODO.Translate
	L"Instruktor",
	L"Uczeñ",
	L"Facility Staff",			// TODO.Translate
	L"Resting at Facility",		// TODO.Translate
	L"Nie ¿yje",
	L"Obezw³adniony",
	L"Jeniec",
	L"Szpital",
	L"Pusty",	// Vehicle is empty
};


// refer to above for comments

STR16 pLongAssignmentStrings[] =
{
	L"Oddzia³ 1",
	L"Oddzia³ 2",
	L"Oddzia³ 3",
	L"Oddzia³ 4",
	L"Oddzia³ 5",
	L"Oddzia³ 6",
	L"Oddzia³ 7",
	L"Oddzia³ 8",
	L"Oddzia³ 9",
	L"Oddzia³ 10",
	L"Oddzia³ 11",
	L"Oddzia³ 12",
	L"Oddzia³ 13",
	L"Oddzia³ 14",
	L"Oddzia³ 15",
	L"Oddzia³ 16",
	L"Oddzia³ 17",
	L"Oddzia³ 18",
	L"Oddzia³ 19",
	L"Oddzia³ 20",
	L"S³u¿ba",
	L"Lekarz",
	L"Pacjent",
	L"Pojazd",
	L"W podró¿y",
	L"Naprawa",
	L"Praktyka",
	L"Trenuj samoobronê",
	L"Train Mobiles",		// TODO.Translate
	L"Trenuj oddzia³",
	L"Uczeñ",
	L"Staff Facility",		// TODO.Translate
	L"Rest at Facility",	// TODO.Translate
	L"Nie ¿yje",
	L"Obezw³adniony",
	L"Jeniec",
	L"W szpitalu", // patient in a hospital 
	L"Pusty",	// Vehicle is empty
};


// the contract options

STR16 pContractStrings[] =
{
	L"Opcje kontraktu:", 
	L"", // a blank line, required
	L"Zaproponuj 1 dzieñ", // offer merc a one day contract extension
	L"Zaproponuj 1 tydzieñ", // 1 week
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
	L"SI£A", //The merc's strength attribute. Others below represent the other attributes.
	L"ZRÊCZNOŒÆ",  
	L"ZWINNOŒÆ", 
	L"INTELIGENCJA", 
	L"UMIEJÊTNOŒCI STRZELECKIE",
	L"WIEDZA MEDYCZNA",
	L"ZNAJOMOŒÆ MECHANIKI",
	L"UMIEJÊTNOŒÆ DOWODZENIA",
	L"ZNAJOMOŒÆ MATERIA£ÓW WYBUCHOWYCH",
	L"POZIOM DOŒWIADCZENIA",
};

STR16 pInvPanelTitleStrings[] =
{
	L"Os³ona", // the armor rating of the merc
	L"Ekwip.", // the weight the merc is carrying
	L"Kamuf.", // the merc's camouflage rating
	L"Kamufla¿:",
	L"Ochrona:",
};

STR16 pShortAttributeStrings[] =
{
	L"Zwn", // the abbreviated version of : agility
	L"Zrc", // dexterity
	L"Si³", // strength
	L"Dow", // leadership
	L"Int", // wisdom
	L"Doœ", // experience level
	L"Str", // marksmanship skill
	L"Wyb", // explosive skill
	L"Mec", // mechanical skill
	L"Med", // medical skill
};


STR16 pUpperLeftMapScreenStrings[] =
{
	L"Przydzia³", // the mercs current assignment 
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
	L"Uczeñ", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Limit ognia:", // the allowable rate of fire for a merc who is guarding
	L" Agresywny ogieñ", // the merc can be aggressive in their choice of fire rates
	L" Oszczêdzaj amunicjê", // conserve ammo 
	L" Strzelaj w ostatecznoœci", // fire only when the merc needs to 
	L"Inne opcje:", // other options available to merc
	L" Mo¿e siê wycofaæ", // merc can retreat
	L" Mo¿e szukaæ schronienia",  // merc is allowed to seek cover
	L" Mo¿e pomagaæ partnerom", // merc can assist teammates
	L"OK", // done with this menu
	L"Anuluj", // cancel this menu
};

// This string has the same comments as above, however the * denotes the option has been selected by the player

STR16 pOtherGuardMenuStrings[] =
{
	L"Limit ognia:",
	L" *Agresywny ogieñ*",
	L" *Oszczêdzaj amunicjê*",
	L" *Strzelaj w ostatecznoœci*",
	L"Inne opcje:",
	L" *Mo¿e siê wycofaæ*",
	L" *Mo¿e szukaæ schronienia*",
	L" *Mo¿e pomagaæ partnerom*",
	L"OK",
	L"Anuluj",
};

STR16 pAssignMenuStrings[] =
{
	L"S³u¿ba", // merc is on active duty
	L"Lekarz", // the merc is acting as a doctor
	L"Pacjent", // the merc is receiving medical attention
	L"Pojazd", // the merc is in a vehicle
	L"Naprawa", // the merc is repairing items 
	L"Szkolenie", // the merc is training
	L"Facility", // the merc is using/staffing a facility	// TODO.Translate
	L"Anuluj", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Atakuj", // set militia to aggresive
	L"Utrzymaj pozycjê", // set militia to stationary
	L"Wycofaj siê", // retreat militia
	L"ChodŸ do mnie", // retreat militia
	L"Padnij", // retreat militia	
	L"Kryj siê",
	L"Wszyscy: Atakujcie", 
	L"Wszyscy: Utrzymajcie pozycje",
	L"Wszyscy: Wycofajcie siê",
	L"Wszyscy: ChodŸcie do mnie",
	L"Wszyscy: Rozproszcie siê",
	L"Wszyscy: Padnijcie",
	L"Wszyscy: Kryjcie siê",
	//L"Wszyscy: Szukajcie przedmiotów",
	L"Anuluj", // cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"Atak", // set militia to aggresive
//	L"Utrzymaj pozycjê", // set militia to stationary
//	L"Odwrót", // retreat militia
//	L"Za mn¹", // retreat militia
//	L"Padnij", // retreat militia		 
//	L"Anuluj", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"Usuñ najemnika", // remove dead merc from current team
	L"Anuluj",
};

STR16 pAttributeMenuStrings[] =
{
	L"Si³a",
	L"Zrêcznoœæ",
	L"Zwinnoœæ",
	L"Zdrowie",
	L"Um. strzeleckie",
	L"Wiedza med.",
	L"Zn. mechaniki",
	L"Um. dowodzenia",
	L"Zn. mat. wyb.",
	L"Anuluj",
};

STR16 pTrainingMenuStrings[] =
{
 L"Praktyka", // train yourself 
 L"Samoobrona", // train the town 
 L"Mobile Militia",		// TODO.Translate
 L"Instruktor", // train your teammates 
 L"Uczeñ",  // be trained by an instructor 
 L"Anuluj", // cancel this menu
};


STR16 pSquadMenuStrings[] =
{
	L"Oddzia³  1",
	L"Oddzia³  2",
	L"Oddzia³  3",
	L"Oddzia³  4",
	L"Oddzia³  5",
	L"Oddzia³  6",
	L"Oddzia³  7",
	L"Oddzia³  8",
	L"Oddzia³  9",
	L"Oddzia³ 10",
	L"Oddzia³ 11",
	L"Oddzia³ 12",
	L"Oddzia³ 13",
	L"Oddzia³ 14",
	L"Oddzia³ 15",
	L"Oddzia³ 16",
	L"Oddzia³ 17",
	L"Oddzia³ 18",
	L"Oddzia³ 19",
	L"Oddzia³ 20",
	L"Anuluj",
};

STR16 pPersonnelTitle[] =
{
	L"Personel", // the title for the personnel screen/program application
};

STR16 pPersonnelScreenStrings[] =
{
	L"Zdrowie: ", // health of merc
	L"Zwinnoœæ: ", 
	L"Zrêcznoœæ: ",
 	L"Si³a: ",
 	L"Um. dowodzenia: ",
 	L"Inteligencja: ",
 	L"Poziom doœw.: ", // experience level
 	L"Um. strzeleckie: ", 
 	L"Zn. mechaniki: ",
 	L"Zn. mat. wybuchowych: ",
 	L"Wiedza medyczna: ",
 	L"Zastaw na ¿ycie: ", // amount of medical deposit put down on the merc
 	L"Bie¿¹cy kontrakt: ", // cost of current contract
 	L"Liczba zabójstw: ", // number of kills by merc
 	L"Liczba asyst: ", // number of assists on kills by merc
 	L"Dzienny koszt:", // daily cost of merc
 	L"Ogólny koszt:", // total cost of merc
 	L"Wartoœæ kontraktu:", // cost of current contract
 	L"Us³ugi ogó³em", // total service rendered by merc
 	L"Zaleg³a kwota", // amount left on MERC merc to be paid
 	L"Celnoœæ:", // percentage of shots that hit target
 	L"Iloœæ walk:", // number of battles fought
 	L"Ranny(a):", // number of times merc has been wounded
 	L"Umiejêtnoœci:",
 	L"Brak umiêjêtnoœci",
	L"Osi¹gniêcia:", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Elitarni: %d\n",
	L"Regularni: %d\n",
	L"Administratorzy: %d\n",
	L"Wrodzy Cywile: %d\n",
	L"Stworzenia: %d\n",
	L"Czo³gi: %d\n", 
	L"Inne: %d\n",

	L"Do najemników: %d\n",
	L"Milicja: %d\n",
	L"Inni: %d\n",

	L"Strza³ów: %d\n",
	L"Wystrzelonych Pocisków: %d\n",
	L"Rzuconych Granatów: %d\n",
	L"Rzuconych No¿y: %d\n",
	L"Ataków No¿em: %d\n",
	L"Ataków Wrêcz: %d\n",
	L"Udanych Trafieñ: %d\n",

	L"Zamki Otwarte Wytrychem: %d\n",
	L"Zamki Wy³amane: %d\n",
	L"Usuniête Pu³apka: %d\n",
	L"Zdetonowane £adunki: %d\n",
	L"Naprawione Przedmioty: %d\n",
	L"Po³¹czone Przedmioty: %d\n",
	L"Ukradzione Przedmioty: %d\n",
	L"Wytrenowana Milicja: %d\n",
	L"Zabanda¿owani Najemnicy: %d\n",
	L"Wykonane Operacje Chirurgiczne: %d\n",
	L"Spotkani NPC: %d\n",
	L"Odkryte Sektory: %d\n",
	L"Unikniête Zasadzki: %d\n",
	L"Wykonane Zadania: %d\n",

	L"Bitwy Taktyczne: %d\n",
	L"Bitwy Autorozstrzygniête: %d\n",
	L"Wykonane Odwroty: %d\n",
	L"Napotkanych Zasadzek: %d\n",
	L"Najwiêksza Walka: %d Wrogów\n",

	L"Postrzelony: %d\n",
	L"Ugodzony No¿em: %d\n",
	L"Uderzony: %d\n",
	L"Wysadzony W Powietrze: %d\n",
	L"Uszkodzonych Atrybutów: %d\n",
	L"Poddany Zabiegom Chirurgicznym: %d\n",
	L"Wypadków Przy Pracy: %d\n",

	L"Charakter:",
	L"Niepe³nosprawnoœæ:",

	L"Attitudes:",	// WANNE: For old traits display instead of "Character:"!
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Brak umiejêtnoœci",
	L"Otwieranie zamków",
	L"Walka wrêcz",		//JA25: modified
	L"Elektronika",
	L"Nocne operacje",			//JA25: modified
	L"Rzucanie",
	L"Szkolenie",
	L"Ciê¿ka broñ",
	L"Broñ automatyczna",
	L"Skradanie siê",
	L"Oburêcznoœæ",
	L"Kradzie¿",
	L"Sztuki walki",
	L"Broñ bia³a",
	L"Snajper",				//JA25: modified
	L"Kamufla¿",						//JA25: modified
	L"(Eksp.)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
// Major traits
L"Brak Umiejêtnoœci",
L"Broñ Automatyczna",
L"Broñ Ciê¿ka",
L"Strzelec Wyborowy",
L"Myœliwy",
L"Pistolero",
L"Walka Wrêcz",
L"Zastêpca Szeryfa",
L"Technik",
L"Paramedyk",
// Minor traits
L"Oburêcznoœæ",
L"Walka Wrêcz",
L"Rzucanie",
L"Operacje Nocne",
L"Ukradkowoœæ",
L"Atletyka",
L"Bodybuilding",
L"£adunki Wybuchowe",
L"Uczenie",
L"Zwiad",
// second names for major skills
L"Strzelec RKMu",
L"Bombardier",
L"Snajper",
L"Myœliwy",
L"Pistolero",
L"Sztuki Walki",
L"Dowódca Dru¿yny",
L"In¿ynier",
L"Doktor",
L"Wiêcej...",
};
//////////////////////////////////////////////////////////


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"W|stañ/IdŸ",
	L"S|chyl siê/IdŸ",
	L"Wstañ/Biegnij (|R)",
	L"|Padnij/Czo³gaj siê",
	L"Patrz (|L)",
	L"Akcja",
	L"Rozmawiaj",
	L"Zbadaj (|C|t|r|l)",

	// Pop up door menu
	L"Otwórz",
	L"Poszukaj pu³apek",
	L"U¿yj wytrychów",
	L"Wywa¿",
	L"Usuñ pu³apki",
	L"Zamknij na klucz",
	L"Otwórz kluczem",
	L"U¿yj ³adunku wybuchowego",
	L"U¿yj ³omu",
	L"Anuluj (|E|s|c)",
	L"Zamknij"
};

// Door Traps. When we examine a door, it could have a particular trap on it. These are the traps.

STR16 pDoorTrapStrings[] =
{
	L"nie posiada ¿adnych pu³apek",
	L"ma za³o¿ony ³adunek wybuchowy",
	L"jest pod napiêciem",
	L"posiada syrenê alarmow¹",
	L"posiada dyskretny alarm"
};

// Contract Extension. These are used for the contract extension with AIM mercenaries.

STR16 pContractExtendStrings[] =
{
	L"dzieñ",
	L"tydzieñ",
	L"dwa tygodnie",
};

// On the map screen, there are four columns. This text is popup help text that identifies the individual columns.

STR16 pMapScreenMouseRegionHelpText[] =
{
	L"Wybór postaci",
	L"Przydzia³ najemnika",
	L"Nanieœ trasê podró¿y",
	L"Kontrakt najemnika",
	L"Usuñ najemnika",
	L"Œpij", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"CICHY",
	L"WYRANY",
	L"G£OŒNY",
	L"BARDZO G£OŒNY"
};

// types of noises 

STR16 pNoiseTypeStr[] = // OBSOLETE
{
	L"NIEOKREŒLONY DWIÊK",
	L"ODG£OS RUCHU",
	L"ODG£OS SKRZYPNIÊCIA",
	L"PLUSK",
	L"ODG£OS UDERZENIA",
	L"STRZA£",
	L"WYBUCH",
	L"KRZYK",
	L"ODG£OS UDERZENIA",
	L"ODG£OS UDERZENIA",
	L"£OMOT",
	L"TRZASK"
};

// Directions that are used to report noises

STR16 pDirectionStr[] = 
{
	L"P£N-WSCH",
	L"WSCH",
	L"P£D-WSCH",
	L"P£D",
	L"P£D-ZACH",
	L"ZACH",
	L"P£N-ZACH",
	L"P£N"
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
	L"Wybrze¿e",
	L"Góry, droga",
	L"Wybrze¿e, droga",
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
	L"Gniazdo stworzeñ",	//Any mine sector with creatures in it
	L"Orta - Piwnica",	//The basement of Orta (K4)
	L"Tunel",				//The tunnel access from the maze garden in Meduna 
						//leading to the secret shelter underneath the palace
	L"Schron",				//The shelter underneath the queen's palace
	L"",							//Unused
};

STR16 gpStrategicString[] =
{
	L"",	//Unused
	L"%s wykryto w sektorze %c%d, a inny oddzia³ jest w drodze.",	//STR_DETECTED_SINGULAR
	L"%s wykryto w sektorze %c%d, a inne oddzia³y s¹ w drodze.",	//STR_DETECTED_PLURAL
	L"Chcesz skoordynowaæ jednoczesne przybycie?",			//STR_COORDINATE

	//Dialog strings for enemies.

	L"Wróg daje ci szansê siê poddaæ.",			//STR_ENEMY_SURRENDER_OFFER
	L"Wróg schwyta³ resztê twoich nieprzytomnych najemników.",	//STR_ENEMY_CAPTURED

	//The text that goes on the autoresolve buttons							

	L"Odwrót", 		//The retreat button				//STR_AR_RETREAT_BUTTON
	L"OK",		//The done button				//STR_AR_DONE_BUTTON

	//The headers are for the autoresolve type (MUST BE UPPERCASE)

	L"OBRONA",								//STR_AR_DEFEND_HEADER
	L"ATAK",								//STR_AR_ATTACK_HEADER
	L"STARCIE",								//STR_AR_ENCOUNTER_HEADER
	L"Sektor",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"ZWYCIÊSTWO!",								//STR_AR_OVER_VICTORY
	L"PORA¯KA!",								//STR_AR_OVER_DEFEAT
	L"KAPITULACJA!",							//STR_AR_OVER_SURRENDERED
	L"NIEWOLA!",								//STR_AR_OVER_CAPTURED
	L"ODWRÓT!",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Samoobrona",							//STR_AR_MILITIA_NAME,
	L"Elity",								//STR_AR_ELITE_NAME,
	L"¯o³nierze",								//STR_AR_TROOP_NAME,
	L"Administrator",							//STR_AR_ADMINISTRATOR_NAME,
	L"Stworzenie",								//STR_AR_CREATURE_NAME,

	//Label for the length of time the battle took

	L"Czas trwania",							//STR_AR_TIME_ELAPSED,

	//Labels for status of merc if retreating.  (UPPERCASE)			

	L"WYCOFA£(A) SIÊ",								//STR_AR_MERC_RETREATED,
	L"WYCOFUJE SIÊ",								//STR_AR_MERC_RETREATING,
	L"WYCOFAJ SIÊ",								//STR_AR_MERC_RETREAT,

	//PRE BATTLE INTERFACE STRINGS 
	//Goes on the three buttons in the prebattle interface.  The Auto resolve button represents
	//a system that automatically resolves the combat for the player without having to do anything.
	//These strings must be short (two lines -- 6-8 chars per line)

	L"Rozst. autom.",							//STR_PB_AUTORESOLVE_BTN,
	L"IdŸ do sektora",							//STR_PB_GOTOSECTOR_BTN,
	L"Wycof. ludzi",							//STR_PB_RETREATMERCS_BTN,

	//The different headers(titles) for the prebattle interface.
	L"STARCIE Z WROGIEM",							//STR_PB_ENEMYENCOUNTER_HEADER,
	L"INWAZJA WROGA",							//STR_PB_ENEMYINVASION_HEADER, // 30
	L"ZASADZKA WROGA",								//STR_PB_ENEMYAMBUSH_HEADER
	L"WEJŒCIE DO WROGIEGO SEKTORA",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"ATAK STWORÓW",							//STR_PB_CREATUREATTACK_HEADER
	L"ATAK DZIKICH KOTÓW",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"WEJŒCIE DO LEGOWISKA DZIKICH KOTÓW",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER

	//Various single words for direct translation.  The Civilians represent the civilian
	//militia occupying the sector being attacked.  Limited to 9-10 chars

	L"Po³o¿enie",
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

	L"Wyczyœæ",
	L"Rozprosz",
	L"Zgrupuj",
	L"OK",

	//The help text for the four buttons.  Use \n to denote new line (just like enter).

	L"Kasuje wszystkie pozy|cje najemników, \ni pozwala ponownie je wprowadziæ.", 
	L"Po ka¿dym naciœniêciu rozmie|szcza\nlosowo twoich najemników.",
	L"|Grupuje najemników w wybranym miejscu.",
	L"Kliknij ten klawisz gdy ju¿ rozmieœcisz \nswoich najemników. (|E|n|t|e|r)",
	L"Musisz rozmieœciæ wszystkich najemników \nzanim rozpoczniesz walkê.",

	//Various strings (translate word for word)

	L"Sektor",
	L"Wybierz pocz¹tkowe pozycje",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"To miejsce nie jest zbyt dobre. Jest niedostêpne. Spróbuj gdzie indziej.",
	L"Rozmieœæ swoich najemników na podœwietlonej czêœci mapy.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"przyby³(a) do sektora", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Automatycznie prowadzi walkê za ciebie, \nnie ³aduj¹c planszy. (|A)",
	L"Atakuj¹c sektor wroga \nnie mo¿na automatycznie rozstrzygn¹æ walki.",
	L"Wejœcie do sektora \nby nawi¹zaæ walkê z wrogiem. (|E)",
	L"Wycofuje oddzia³ \ndo s¹siedniego sektora. (|R)",				//singular version
	L"Wycofuje wszystkie oddzia³y \ndo s¹siedniego sektora. (|R)", //multiple groups with same previous sector

	//various popup messages for battle conditions.  

	//%c%d is the sector -- ex:  A9
	L"Nieprzyjaciel zatakowa³ oddzia³y samoobrony w sektorze %c%d.",
	//%c%d is the sector -- ex:  A9
	L"Stworzenia zaatakowa³y oddzia³y samoobrony w sektorze %c%d.",
	//1st %d refers to the number of civilians eaten by monsters,  %c%d is the sector -- ex:  A9
	//Note:  the minimum number of civilians eaten will be two.
	L"Stworzenia zatakowa³y i zabi³y %d cywili w sektorze %s.",
	//%c%d is the sector -- ex:  A9
	L"Nieprzyjaciel zatakowa³ twoich najemników w sektorze %s.  ¯aden z twoich najemników nie mo¿e walczyæ!",
	//%c%d is the sector -- ex:  A9
	L"Stworzenia zatakowa³y twoich najemników w sektorze %s.  ¯aden z twoich najemników nie mo¿e walczyæ!",

};

STR16 gpGameClockString[] = 
{
	//This is the day represented in the game clock.  Must be very short, 4 characters max.
	L"Dzieñ",	
};

//When the merc finds a key, they can get a description of it which 
//tells them where and when they found it.
STR16 sKeyDescriptionStrings[2] =
{
	L"Zn. w sektorze:",
	L"Zn. w dniu:",
};

//The headers used to describe various weapon statistics.

CHAR16		gWeaponStatsDesc[][ 17 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Stan:",
	L"Waga:", 
	L"PA Koszty",	
	L"Zas.:",		// Range
	L"Si³a:",		// Damage
	L"Iloœæ:", 		// Number of bullets left in a magazine
    L"PA:",                 // abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Celnoœæ:",	//9
	L"Zasiêg:",		//10	
	L"Si³a:",		//11
	L"Waga:",		//12
	L"Og³uszenie:",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Attachments:",	//14	// TODO.Translate
	L"AUTO/5:",		//15
	L"Remaining ammo:",		//16	// TODO.Translate

	L"Domyœlne:",	//17 //WarmSteel - So we can also display default attachments

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
// Please note, several of these are artificially inflated to 19 entries to help fix a complication with
// changing item type while watching its description box
STR16		gzWeaponStatsFasthelp[ 32 ] =
{
	L"Celnoœæ",
	L"Obra¿.",
	L"Zas.",
	L"Aiming Levels",
	L"Modf. celowania",
	L"Œredni max zasg. lasera",
	L"T³umienie b³ysku",
	L"G³oœnoœæ (mniej - lepiej)",
	L"Stan",
	L"Trud napraw",
	L"Min. zasg. dla bonusu cel.",
	L"Modyf. trafieñ",
	L"",	//12
	L"AP/przygot.",
	L"AP za 1 strza³",
	L"AP za seriê",
	L"AP za Auto",
	L"AP/prze³aduj",
	L"AP/prze³aduj rêcznie",
	L"",	//19
	L"Modf. dwójnogu",
	L"Auto/5AP",
	L"PA: (mniej - lepiej)",	//22
	L"AP za rzut",
	L"AP za strza³",
	L"AP/cios-nó¿",
	L"Wy³. 1 strza³!",
	L"Wy³. seriê!",
	L"Wy³. auto!",
	L"AP/cios-³om",
	L"Autofire Penalty (Lower is better)",
    L"Burst Penalty (Lower is better)",
};

STR16		gzWeaponStatsFasthelpTactical[ 32 ] =
{
	L"Celnoœæ",
	L"Obra¿.",
	L"Zas.",
	L"Aiming Levels",
	L"Modf. celowania",
	L"Œredni max zasg. lasera",
	L"T³umienie b³ysku",
	L"G³oœnoœæ (mniej - lepiej)",
	L"Stan",
	L"Trud napraw",
	L"Min. zasg. dla bonusu cel.",
	L"Modyf. trafieñ",
	L"",	//12
	L"AP/przygot.",
	L"AP za 1 strza³",
	L"AP za seriê",
	L"AP za Auto",
	L"AP/prze³aduj",
	L"AP/prze³aduj rêcznie",
	L"Burst Penalty (Lower is better)",	//19
	L"Modf. dwójnogu",
	L"Auto/5AP",
	L"Autofire Penalty (Lower is better)",
	L"PA: (mniej - lepiej)",	//23
	L"AP za rzut",
	L"AP za strza³",
	L"AP/cios-nó¿",
	L"Wy³. 1 strza³!",
	L"Wy³. seriê!",
	L"Wy³. auto!",
	L"AP/cios-³om",
    L"",
};

STR16		gzAmmoStatsFasthelp[ 20 ] =
{
	L"Penetracja os³on (mniej – lepiej)",
	L"Moc obalaj¹ca (wiêcej – lepiej)",
	L"Eksplozja przed celem (wiêcej – lepiej)",
	L"Efekt smugowy",
	L"P. czo³g.",
	L"Nisc zamka",
	L"Pomija os³onê",
	L"Korozj",
	L"Modyf. zasg.",
	L"Modyf. obra¿eñ",
	L"Modyf. trafieñ",
	L"Autofire Penalty Modifier (Higher is better)",
	L"Modyf. kary za seriê (wiêcej – lepiej)",
	L"Modyf. sprawnoœci",
	L"Modyf. g³oœnoœci (mniej - lepiej)",
	L"",
	L"",
	L"",
	L"",
	L"",
};

STR16		gzArmorStatsFasthelp[ 20 ] =
{
	L"Os³ona",
	L"Pokrycie (wiêcej - lepiej)",
	L"Zu¿ycie (mniej - lepiej)",
	L"Modyf. AP",
	L"Modyf. trafieñ",
	L"Kam leœny",
	L"Kam miasto",
	L"Kam pustyn.",
	L"Kam œnieg",
	L"Modyf. skradania",
	L"Modyf. zasg. wzroku",
	L"Modyf. zasg. wzroku/dzieñ",
	L"Modyf. zasg. wzroku/noc",
	L"Modyf. zasg. wzroku/jasne œwiat³o",
	L"Modyf. zasg. wzr./jaskinia",
	L"Widzenie tunelowe (w %)",
	L"Modyf. zasg. s³uchu",
	L"",
	L"",
	L"",
};

STR16		gzExplosiveStatsFasthelp[ 20 ] =
{
	L"Obra¿.",
	L"Og³uszanie",
	L"G³oœn. wybuchu (mniej - lepiej)",
	L"Niestabilnoœæ! (mniej - lepiej)",
	L"Zsg. wybuchu",
	L"Zsg efektu pocz¹tk.",
	L"Zsg. efektu koñcow.",
	L"Trwanie efektu",
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
	L"Modyf. rozmiaru (mniej - lepiej)", // 0
	L"Modyf. sprawnoœci",
	L"Modyf. g³oœnoœci (mniej - lepiej)",
	L"Ukrywa b³ysk",
	L"Modf. dwójnogu",
	L"Modyf. zasiêgu", // 5
	L"Modyf. trafieñ",
	L"Max zasg. lasera",
	L"Modf bonusu celowania",
	L"Modyf. d³ug. serii",
	L"Modyf. kary za seriê (wiêcej - lepiej)", // 10
	L"Modyf. kary za ogieñ auto (wiêcej - lepiej)",
	L"Modyf. AP",
	L"Modyf. AP za seriê (mniej - lepiej)",
	L"Modyf. AP za ogieñ auto (mniej - lepiej)",
	L"Modf AP/przygotwanie (mniej - lepiej)", // 15
	L"Modf AP/prze³adowanie (mniej - lepiej)",
	L"Modyf. wlk. magazynka",
	L"Modyf. AP/atak (mniej - lepiej)",
	L"Modyf. obra¿eñ",
	L"Modf obr. walki wrêcz", // 20
	L"Kam leœny",
	L"Kam miasto",
	L"Kam pustyn.",
	L"Kam œnieg",
	L"Modyf. skradania", // 25
	L"Modyf. zasg. s³uchu",
	L"Modyf. zasg. wzroku",
	L"Modyf. zasg. wzroku/dzieñ",
	L"Modyf. zasg. wzroku/noc",
	L"Modyf. zasg. wzroku/jasne œwiat³o", //30
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

CHAR16 gMoneyStatsDesc[][ 13 ] =
{
	L"Kwota",
	L"Pozosta³o:", //this is the overall balance
	L"Kwota",
	L"Wydzieliæ:", // the amount he wants to separate from the overall balance to get two piles of money

	L"Bie¿¹ce",
	L"Saldo",
	L"Kwota do",
	L"podjêcia",
};

//The health of various creatures, enemies, characters in the game. The numbers following each are for comment
//only, but represent the precentage of points remaining. 

CHAR16 zHealthStr[][13] =
{
	L"UMIERAJ¥CY",		//	>= 0
	L"KRYTYCZNY", 		//	>= 15
	L"KIEPSKI",		//	>= 30
	L"RANNY",    	//	>= 45
	L"ZDROWY",    	//	>= 60
	L"SILNY",     	// 	>= 75
	L"DOSKONA£Y",		// 	>= 90
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
	L"PrzyjaŸnie",		//approach in a friendly
	L"Bezpoœrednio",		//approach directly - let's get down to business
	L"GroŸnie",		//approach threateningly - talk now, or I'll blow your face off
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
 L"Czo³g",
 L"Helikopter",
};

STR16 pShortVehicleStrings[] =
{
	L"Eldor.",
	L"Hummer",			// the HMVV
	L"Furg.",
	L"Jeep",
	L"Czo³g",
	L"Heli.", 				// the helicopter
};

STR16	zVehicleName[] =
{
	L"Eldorado",
	L"Hummer",		//a military jeep. This is a brand name.
	L"Furg.",			// Ice cream truck
	L"Jeep",
	L"Czo³g",
	L"Heli.", 		//an abbreviation for Helicopter
};


//These are messages Used in the Tactical Screen

CHAR16 TacticalStr[][ MED_STRING_LENGTH ] =
{
	L"Nalot",
	L"Udzieliæ automatycznie pierwszej pomocy?",
	
	// CAMFIELD NUKE THIS and add quote #66.
	
	L"%s zauwa¿y³(a) ¿e dostawa jest niekompletna.",
	
	// The %s is a string from pDoorTrapStrings
	
	L"Zamek %s.", 
	L"Brak zamka.",
	L"Sukces!",
	L"Niepowodzenie.",
	L"Sukces!",
	L"Niepowodzenie.",
	L"Zamek nie ma pu³apek.",
	L"Sukces!",
	// The %s is a merc name
	L"%s nie posiada odpowiedniego klucza.",
	L"Zamek zosta³ rozbrojony.",
	L"Zamek nie ma pu³apek.",
	L"Zamkniête.",
	L"DRZWI",
	L"ZABEZP.",
	L"ZAMKNIÊTE",
	L"OTWARTE",
	L"ROZWALONE",
	L"Tu jest prze³¹cznik. W³¹czyæ go?",
	L"Rozbroiæ pu³apkê?",
	L"Poprz...",
	L"Nast...",
	L"Wiêcej...",

	// In the next 2 strings, %s is an item name

	L"%s - po³o¿ono na ziemi.",
	L"%s - przekazano do - %s.",

	// In the next 2 strings, %s is a name

	L"%s otrzyma³(a) ca³¹ zap³atê.",
	L"%s - nale¿noœæ wobec niej/niego wynosi jeszcze %d.",
	L"Wybierz czêstotliwoœæ sygna³u detonuj¹cego:",  	//in this case, frequency refers to a radio signal
	L"Ile tur do eksplozji:",	//how much time, in turns, until the bomb blows
	L"Ustaw czêstotliwoœæ zdalnego detonatora:", 	//in this case, frequency refers to a radio signal
	L"Rozbroiæ pu³apkê?",
	L"Usun¹æ niebiesk¹ flagê?",
	L"Umieœciæ tutaj niebiesk¹ flagê?",
	L"Koñcz¹ca tura",

	// In the next string, %s is a name. Stance refers to way they are standing.

	L"Na pewno chcesz zaatakowaæ - %s?",
	L"Pojazdy nie mog¹ zmieniaæ pozycji.",
	L"Robot nie mo¿e zmieniaæ pozycji.",

	// In the next 3 strings, %s is a name

	L"%s nie mo¿e zmieniæ pozycji w tym miejscu.",
	L"%s nie mo¿e tu otrzymaæ pierwszej pomocy.",	
	L"%s nie potrzebuje pierwszej pomocy.",
	L"Nie mo¿na ruszyæ w to miejsce.",
	L"Oddzia³ jest ju¿ kompletny. Nie ma miejsca dla nowych rekrutów.",	//there's no room for a recruit on the player's team

	// In the next string, %s is a name

	L"%s pracuje ju¿ dla ciebie.",

	// Here %s is a name and %d is a number

	L"%s - nale¿noœæ wobec niej/niego wynosi %d$.",

	// In the next string, %s is a name

	L"%s - Eskortowaæ t¹ osobê?",

	// In the next string, the first %s is a name and the second %s is an amount of money (including $ sign)

	L"%s - Zatrudniæ t¹ osobê za %s dziennie?",

	// This line is used repeatedly to ask player if they wish to participate in a boxing match. 

	L"Chcesz walczyæ?",

	// In the next string, the first %s is an item name and the 
	// second %s is an amount of money (including $ sign)

	L"%s - Kupiæ to za %s?",

	// In the next string, %s is a name

	L"%s jest pod eskort¹ oddzia³u %d.",

	// These messages are displayed during play to alert the player to a particular situation

	L"ZACIÊTA",					//weapon is jammed.
	L"Robot potrzebuje amunicji kaliber %s.",		//Robot is out of ammo
	L"Rzuciæ tam? To niemo¿liwe.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Skradanie siê (|Z)",
	L"Ekran |Mapy",
	L"Koniec tury (|D)",
	L"Rozmowa",
	L"Wycisz",
	L"Pozycja do góry (|P|g|U|p)",
	L"Poziom kursora (|T|a|b)",
	L"Wspinaj siê / Zeskocz (|J)",
	L"Pozycja w dó³ (|P|g|D|n)",
	L"Badaæ (|C|t|r|l)",
	L"Poprzedni najemnik",
	L"Nastêpny najemnik (|S|p|a|c|j|a)",
	L"|Opcje",
	L"Ci¹g³y ogieñ (|B)",
	L"Spójrz/Obróæ siê (|L)",
	L"Zdrowie: %d/%d\nEnergia: %d/%d\nMorale: %s",
	L"Co?",					//this means "what?"
	L"Kont",					//an abbrieviation for "Continued"
	L"%s ma w³¹czone potwierdzenia g³osowe.",
	L"%s ma wy³¹czone potwierdzenia g³osowe.",
	L"Stan: %d/%d\nPaliwo: %d/%d",
	L"Wysi¹dŸ z pojazdu" ,
	L"Zmieñ oddzia³ ( |S|h|i|f|t |S|p|a|c|j|a )",
	L"ProwadŸ",
	L"N/D",						//this is an acronym for "Not Applicable."
	L"U¿yj ( Walka wrêcz )",
	L"U¿yj ( Broni palnej )",
	L"U¿yj ( Broni bia³ej )",
	L"U¿yj ( Mat. wybuchowych )",
	L"U¿yj ( Apteczki )",
	L"(£ap)",
	L"(Prze³aduj)",
	L"(Daj)",
	L"%s - pu³apka zosta³a uruchomiona.",
	L"%s przyby³(a) na miejsce.",
	L"%s straci³(a) wszystkie Punkty Akcji.",
	L"%s jest nieosi¹galny(na).",
	L"%s ma ju¿ za³o¿one opatrunki.",
	L"%s nie ma banda¿y.",
	L"Wróg w sektorze!",
	L"Nie ma wroga w zasiêgu wzroku.",
	L"Zbyt ma³o Punktów Akcji.",
	L"Nikt nie u¿ywa zdalnego sterowania.",
	L"Ci¹g³y ogieñ opró¿ni³ magazynek!",
	L"¯O£NIERZ",
	L"STWÓR",
	L"SAMOOBRONA",
	L"CYWIL",
	L"Wyjœcie z sektora",
	L"OK",
	L"Anuluj",
	L"Wybrany najemnik",
	L"Wszyscy najemnicy w oddziale",
	L"IdŸ do sektora",
	L"Otwórz mapê",
	L"Nie mo¿na opuœciæ sektora z tej strony.",
	L"%s jest zbyt daleko.",
	L"Usuñ korony drzew",
	L"Poka¿ korony drzew",
	L"WRONA",				//Crow, as in the large black bird
	L"SZYJA",
	L"G£OWA",
	L"TU£ÓW",
	L"NOGI",
	L"Powiedzieæ królowej to, co chce wiedzieæ?",
	L"Wzór odcisku palca pobrany",
	L"Niew³aœciwy wzór odcisku palca. Broñ bezu¿yteczna.",
	L"Cel osi¹gniêty",
	L"Droga zablokowana",
	L"Wp³ata/Podjêcie pieniêdzy",		//Help text over the $ button on the Single Merc Panel 
	L"Nikt nie potrzebuje pierwszej pomocy.",
	L"Zac.",						// Short form of JAMMED, for small inv slots
	L"Nie mo¿na siê tam dostaæ.",					// used ( now ) for when we click on a cliff
	L"Przejœcie zablokowane. Czy chcesz zamieniæ siê miejscami z t¹ osob¹?",
	L"Osoba nie chce siê przesun¹æ.",
	// In the following message, '%s' would be replaced with a quantity of money (e.g. $200)
	L"Zgadzasz siê zap³aciæ %s?",
	L"Zgadzasz siê na darmowe leczenie?",
	L"Zgadasz siê na ma³¿eñstwo z Darylem?",
	L"Kó³ko na klucze",
	L"Nie mo¿esz tego zrobiæ z eskortowan¹ osob¹.",
	L"Oszczêdziæ Krotta?",
	L"Poza zasiêgiem broni",
	L"Górnik",
	L"Pojazdem mo¿na podró¿owaæ tylko pomiêdzy sektorami",
	L"Teraz nie mo¿na automatycznie udzieliæ pierwszej pomocy",
	L"Przejœcie zablokowane dla - %s",
	L"Twoi najemnicy, schwytani przez ¿o³nierzy Deidranny, s¹ tutaj uwiêzieni!",
	L"Zamek zosta³ trafiony",
	L"Zamek zosta³ zniszczony",
	L"Ktoœ inny majstruje przy tych drzwiach.",
	L"Stan: %d/%d\nPaliwo: %d/%d",
	L"%s nie widzi - %s.",  // Cannot see person trying to talk to
	L"Dodatek usuniêty",
	L"Nie mo¿esz zdobyæ kolejnego pojazdu, poniewa¿ posiadasz ju¿ 2",
};

//Varying helptext explains (for the "Go to Sector/Map" checkbox) what will happen given different circumstances in the "exiting sector" interface.
STR16 pExitingSectorHelpText[] =
{
	//Helptext for the "Go to Sector" checkbox button, that explains what will happen when the box is checked.
	L"Jeœli zaznaczysz tê opcjê, to s¹siedni sektor zostanie natychmiast za³adowany.",
	L"Jeœli zaznaczysz tê opcjê, to na czas podró¿y pojawi siê automatycznie ekran mapy.",

	//If you attempt to leave a sector when you have multiple squads in a hostile sector.
	L"Ten sektor jest okupowany przez wroga i nie mo¿esz tu zostawiæ najemników.\nMusisz uporaæ siê z t¹ sytuacj¹ zanim za³adujesz inny sektor.",

	//Because you only have one squad in the sector, and the "move all" option is checked, the "go to sector" option is locked to on. 
	//The helptext explains why it is locked.
	L"Gdy wyprowadzisz swoich pozosta³ych najemników z tego sektora,\ns¹siedni sektor zostanie automatycznie za³adowany.",
	L"Gdy wyprowadzisz swoich pozosta³ych najemników z tego sektora,\nto na czas podró¿y pojawi siê automatycznie ekran mapy.",

	//If an EPC is the selected merc, it won't allow the merc to leave alone as the merc is being escorted.  The "single" button is disabled.
	L"%s jest pod eskort¹ twoich najemników i nie mo¿e bez nich opuœciæ tego sektora.",

	//If only one conscious merc is left and is selected, and there are EPCs in the squad, the merc will be prohibited from leaving alone.
	//There are several strings depending on the gender of the merc and how many EPCs are in the squad.
	//DO NOT USE THE NEWLINE HERE AS IT IS USED FOR BOTH HELPTEXT AND SCREEN MESSAGES!
	L"%s nie mo¿e sam opuœciæ tego sektora, gdy¿ %s jest pod jego eskort¹.", //male singular
	L"%s nie mo¿e sama opuœciæ tego sektora, gdy¿ %s jest pod jej eskort¹.", //female singular
	L"%s nie mo¿e sam opuœciæ tego sektora, gdy¿ eskortuje inne osoby.", //male plural
	L"%s nie mo¿e sama opuœciæ tego sektora, gdy¿ eskortuje inne osoby.", //female plural

	//If one or more of your mercs in the selected squad aren't in range of the traversal area, then the  "move all" option is disabled,
	//and this helptext explains why.
	L"Wszyscy twoi najemnicy musz¹ byæ w pobli¿u,\naby oddzia³ móg³ siê przemieszczaæ.",

	L"", //UNUSED

	//Standard helptext for single movement.  Explains what will happen (splitting the squad)
	L"Jeœli zaznaczysz tê opcjê, %s bêdzie podró¿owaæ w pojedynkê\ni automatycznie znajdzie siê w osobnym oddziale.",

	//Standard helptext for all movement.  Explains what will happen (moving the squad)
	L"Jeœli zaznaczysz tê opcjê, aktualnie\nwybrany oddzia³ opuœci ten sektor.",

	//This strings is used BEFORE the "exiting sector" interface is created.  If you have an EPC selected and you attempt to tactically
	//traverse the EPC while the escorting mercs aren't near enough (or dead, dying, or unconscious), this message will appear and the
	//"exiting sector" interface will not appear.  This is just like the situation where
	//This string is special, as it is not used as helptext.  Do not use the special newline character (\n) for this string.
	L"%s jest pod eskort¹ twoich najemników i nie mo¿e bez nich opuœciæ tego sektora. Aby opuœciæ sektor twoi najemnicy musz¹ byæ w pobli¿u.",
};



STR16 pRepairStrings[] = 
{
	L"Wyposa¿enie", 		// tell merc to repair items in inventory
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
	L"zwinnoœci.",
	L"zrêcznoœci.",
	L"inteligencji.",
	L"umiejêtnoœci medycznych.",
	L"umiejêtnoœci w dziedzinie materia³ów wybuchowych.",
	L"umiejêtnoœci w dziedzinie mechaniki.",
	L"umiejêtnoœci strzeleckich.",
	L"doœwiadczenia.",
	L"si³y.",
	L"umiejêtnoœci dowodzenia.",
};


STR16 pHelicopterEtaStrings[] =
{
	L"Ca³kowita trasa:  ",// total distance for helicopter to travel
	L" Bezp.:   ", 			// distance to travel to destination
	L" Niebezp.:", 			// distance to return from destination to airport
	L"Ca³kowity koszt: ", 		// total cost of trip by helicopter
	L"PCP:  ", 			// ETA is an acronym for "estimated time of arrival" 
	L"Helikopter ma ma³o paliwa i musi wyl¹dowaæ na terenie wroga.",	// warning that the sector the helicopter is going to use for refueling is under enemy control -> 
  L"Pasa¿erowie: ",
  L"Wybór Skyridera lub pasa¿erów?",
  L"Skyrider",
  L"Pasa¿erowie",
};

STR16 sMapLevelString[] =
{
	L"Poziom:", 			// what level below the ground is the player viewing in mapscreen
};

STR16 gsLoyalString[] =
{
	L"Lojalnoœci", 			// the loyalty rating of a town ie : Loyal 53%
};


// error message for when player is trying to give a merc a travel order while he's underground.

STR16 gsUndergroundString[] =
{
	L"nie mo¿na wydawaæ rozkazów podró¿y pod ziemi¹.", 
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
	L"Wiêzienie",
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
	L"Pod kontrol¹", 					// how much of town is controlled
	L"Brak", 					// none of this town
	L"Przynale¿na kopalnia", 				// mine associated with this town
	L"Lojalnoœæ",					// 5 // the loyalty level of this town
	L"Wyszkolonych", 					// the forces in the town trained by the player
	L"",
	L"G³ówne obiekty", 				// main facilities in this town
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
	L"Z³oto",
	L"Dzienna produkcja",
	L"Mo¿liwa produkcja",
	L"Opuszczona",				// 5
	L"Zamkniêta",
	L"Na wyczerpaniu",
	L"Produkuje",
	L"Stan",
	L"Tempo produkcji",
	L"Typ z³o¿a",				// 10
	L"Kontrola miasta",
	L"Lojalnoœæ miasta",
//	L"Górnicy",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Si³y wroga",
	L"Sektor",
	L"Przedmiotów",
	L"Nieznane",
	L"Pod kontrol¹",
	L"Tak",
	L"Nie",
};

// error strings for inventory

STR16 pMapInventoryErrorString[] =
{
	L"%s jest zbyt daleko.",	//Merc is in sector with item but not close enough
	L"Nie mo¿na wybraæ tego najemnika.",  //MARK CARTER
	L"%s nie mo¿e st¹d zabraæ tego przedmiotu, gdy¿ nie jest w tym sektorze.",
	L"Podczas walki nie mo¿na korzystaæ z tego panelu.",
	L"Podczas walki nie mo¿na korzystaæ z tego panelu.",
	L"%s nie mo¿e tu zostawiæ tego przedmiotu, gdy¿ nie jest w tym sektorze.",
	L"W trakcie walki nie mo¿esz do³adowywaæ magazynka.",
};

STR16 pMapInventoryStrings[] =
{
	L"Po³o¿enie", 			// sector these items are in
	L"Razem przedmiotów", 		// total number of items in sector
};


// help text for the user

STR16 pMapScreenFastHelpTextList[] =
{
	L"Kliknij w kolumnie 'Przydz.', aby przydzieliæ najemnika do innego oddzia³u lub wybranego zadania.",
	L"Aby wyznaczyæ najemnikowi cel w innym sektorze, kliknij pole w kolumnie 'Cel'.",
	L"Gdy najemnicy otrzymaj¹ ju¿ rozkaz przemieszczenia siê, kompresja czasu pozwala im szybciej dotrzeæ na miejsce.",
	L"Kliknij lewym klawiszem aby wybraæ sektor. Kliknij ponownie aby wydaæ najemnikom rozkazy przemieszczenia, lub kliknij prawym klawiszem by uzyskaæ informacje o sektorze.",
	L"Naciœnij w dowolnym momencie klawisz 'H' by wyœwietliæ okienko pomocy.",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Próbny tekst",
	L"Niewiele mo¿esz tu zrobiæ, dopóki najemnicy nie przylec¹ do Arulco. Gdy ju¿ zbierzesz swój oddzia³, kliknij przycisk Kompresji Czasu, w prawym dolnym rogu. W ten sposób twoi najemnicy szybciej dotr¹ na miejsce.",
};

// movement menu text

STR16 pMovementMenuStrings[] =
{
	L"Przemieœæ najemników", 	// title for movement box 
	L"Nanieœ trasê podró¿y", 		// done with movement menu, start plotting movement
	L"Anuluj", 		// cancel this menu
	L"Inni",		// title for group of mercs not on squads nor in vehicles
};


STR16 pUpdateMercStrings[] =
{
	L"Oj:", 			// an error has occured
	L"Wygas³ kontrakt najemników:", 	// this pop up came up due to a merc contract ending
	L"Najemnicy wype³nili zadanie:", // this pop up....due to more than one merc finishing assignments
	L"Najemnicy wrócili do pracy:", // this pop up ....due to more than one merc waking up and returing to work
	L"Odpoczywaj¹cy najemnicy:", // this pop up ....due to more than one merc being tired and going to sleep
	L"Wkrótce wygasn¹ kontrakty:", 	// this pop up came up due to a merc contract ending
};

// map screen map border buttons help text

STR16 pMapScreenBorderButtonHelpText[] =
{
	L"Poka¿ miasta (|W)",
	L"Poka¿ kopalnie (|M)",
	L"Poka¿ oddzia³y i wrogów (|T)",
	L"Poka¿ przestrzeñ powietrzn¹ (|A)",
	L"Poka¿ przedmioty (|I)",
	L"Poka¿ samoobronê i wrogów (|Z)",
	L"Show Mobile Militia |Restrictions", // HEADROCK HAM 4: Mobile Restrictions Button // TODO.Translate
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Next (|.)",		// next page // TODO.Translate
	L"Previous (|,)",		// previous page // TODO.Translate
	L"Exit Sector Inventory (|E|s|c)",	// exit sector inventory // TODO.Translate
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"|Laptop",
	L"Ekran taktyczny (|E|s|c)",
	L"|Opcje",
	L"Kompresja czasu (|+)", 	// time compress more
	L"Kompresja czasu (|-)", 	// time compress less
	L"Poprzedni komunikat (|S|t|r|z|a|³|k|a |w |g|ó|r|ê)\nPoprzednia strona (|P|g|U|p)", 	// previous message in scrollable list
	L"Nastêpny komunikat (|S|t|r|z|a|³|k|a |w |d|ó|³)\nNastêpna strona (|P|g|D|n)", 	// next message in the scrollable list
	L"W³¹cz/Wy³¹cz kompresjê czasu (|S|p|a|c|j|a)",	// start/stop time compression
};

STR16 pMapScreenBottomText[] =
{
	L"Saldo dostêpne", 	// current balance in player bank account
};

STR16 pMercDeadString[] =
{
	L"%s nie ¿yje.",
};


STR16 pDayStrings[] =
{
	L"Dzieñ",
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
  L"Nastêpny",
};

// new mail notify string 

STR16 pNewMailStrings[] =
{
 L"Masz now¹ pocztê...",
};


// confirm player's intent to delete messages

STR16 pDeleteMailStrings[] =
{
 L"Usun¹æ wiadomoœæ?",
 L"Usun¹æ wiadomoœæ?",
};


// the sort header strings

STR16 pEmailHeaders[] =
{
 	L"Od:",
 	L"Temat:",
 	L"Dzieñ:",
};

// email titlebar text

STR16 pEmailTitleText[] =
{
 	L"Skrzynka odbiorcza",
};


// the financial screen strings
STR16 pFinanceTitle[] =
{
	L"Ksiêgowy Plus",		//the name we made up for the financial program in the game
};

STR16 pFinanceSummary[] =
{
	L"Wyp³ata:", 				// credit (subtract from) to player's account
	L"Wp³ata:", 				// debit (add to) to player's account
	L"Wczorajsze wp³ywy:",
	L"Wczorajsze dodatkowe wp³ywy:",
	L"Wczorajsze wydatki:",
	L"Saldo na koniec dnia:",
	L"Dzisiejsze wp³ywy:",
	L"Dzisiejsze dodatkowe wp³ywy:",
	L"Dzisiejsze wydatki:",
	L"Saldo dostêpne:",
	L"Przewidywane wp³ywy:",
	L"Przewidywane saldo:", 		// projected balance for player for tommorow
};


// headers to each list in financial screen

STR16 pFinanceHeaders[] =
{
	L"Dzieñ", 					// the day column
	L"Ma", 				// the credits column (to ADD money to your account)
	L"Winien",				// the debits column (to SUBTRACT money from your account)
	L"Transakcja", 			// transaction type - see TransactionText below
	L"Saldo", 				// balance at this point in time
	L"Strona", 				// page number
	L"Dzieñ (dni)", 		// the day(s) of transactions this page displays 
};


STR16 pTransactionText[] =
{
	L"Naros³e odsetki",			// interest the player has accumulated so far
	L"Anonimowa wp³ata",
	L"Koszt transakcji", 
	L"Wynajêto -", 				// Merc was hired
	L"Zakupy u Bobby'ego Ray'a", 		// Bobby Ray is the name of an arms dealer
	L"Uregulowanie rachunków w M.E.R.C.",
	L"Zastaw na ¿ycie dla - %s", 		// medical deposit for merc
	L"Analiza profilu w IMP", 		// IMP is the acronym for International Mercenary Profiling
	L"Ubezpieczneie dla - %s", 
	L"Redukcja ubezp. dla - %s",
	L"Przed³. ubezp. dla - %s", 				// johnny contract extended
	L"Anulowano ubezp. dla - %s", 
	L"Odszkodowanie za - %s", 		// insurance claim for merc
	L"1 dzieñ", 				// merc's contract extended for a day
	L"1 tydzieñ", 				// merc's contract extended for a week
	L"2 tygodnie", 				// ... for 2 weeks
	L"Przychód z kopalni", 
	L"", //String nuked
	L"Zakup kwiatów",
	L"Pe³ny zwrot zastawu za - %s",
	L"Czêœciowy zwrot zastawu za - %s",
	L"Brak zwrotu zastawu za - %s",
	L"Zap³ata dla - %s",		// %s is the name of the npc being paid
	L"Transfer funduszy do - %s", 			// transfer funds to a merc
	L"Transfer funduszy od - %s", 		// transfer funds from a merc
	L"Samoobrona w - %s", // initial cost to equip a town's militia
	L"Zakupy u - %s.",	//is used for the Shop keeper interface.  The dealers name will be appended to the end of the string.
	L"%s wp³aci³(a) pieni¹dze.",
	L"Sprzedano rzecz(y) miejscowym",
	L"Wykorzystanie Placówki", // HEADROCK HAM 3.6	
	L"Utrzymanie Samoobr.", // HEADROCK HAM 3.6
};

STR16 pTransactionAlternateText[] =
{
	L"Ubezpieczenie dla -", 				// insurance for a merc
	L"Przed³. kontrakt z - %s o 1 dzieñ.", 				// entend mercs contract by a day
	L"Przed³. kontrakt z - %s o 1 tydzieñ.",
	L"Przed³. kontrakt z - %s o 2 tygodnie.",
};

// helicopter pilot payment 

STR16 pSkyriderText[] =
{
	L"Skyriderowi zap³acono %d$", 			// skyrider was paid an amount of money
	L"Skyriderowi trzeba jeszcze zap³aciæ %d$", 		// skyrider is still owed an amount of money
	L"Skyrider zatankowa³",	// skyrider has finished refueling
	L"",//unused
	L"",//unused
	L"Skyrider jest gotów do kolejnego lotu.", // Skyrider was grounded but has been freed
	L"Skyrider nie ma pasa¿erów. Jeœli chcesz przetransportowaæ najemników, zmieñ ich przydzia³ na POJAZD/HELIKOPTER.",
};


// strings for different levels of merc morale

STR16 pMoralStrings[] =
{
 L"Œwietne", 
 L"Dobre",
 L"Stabilne",
 L"S³abe",
 L"Panika",
 L"Z³e",
};

// Mercs equipment has now arrived and is now available in Omerta or Drassen.

STR16 pLeftEquipmentString[] =
{ 
	L"%s - jego/jej sprzêt jest ju¿ w Omercie( A9 ).", 
	L"%s - jego/jej sprzêt jest ju¿ w Drassen( B13 ).",
};

// Status that appears on the Map Screen

STR16 pMapScreenStatusStrings[] =
{
	L"Zdrowie",
	L"Energia",
	L"Morale",
	L"Stan",	// the condition of the current vehicle (its "health")
	L"Paliwo",	// the fuel level of the current vehicle (its "energy")
};


STR16 pMapScreenPrevNextCharButtonHelpText[] =
{
	L"Poprzedni najemnik (|S|t|r|z|a|³|k|a |w |l|e|w|o)", 			// previous merc in the list
	L"Nastêpny najemnik (|S|t|r|z|a|³|k|a |w |p|r|a|w|o)", 				// next merc in the list
};


STR16 pEtaString[] =
{
	L"PCP:", 				// eta is an acronym for Estimated Time of Arrival
};

STR16 pTrashItemText[] =
{
	L"Wiêcej tego nie zobaczysz. Czy na pewno chcesz to zrobiæ?", 	// do you want to continue and lose the item forever
	L"To wygl¹da na coœ NAPRAWDÊ wa¿nego. Czy NA PEWNO chcesz to zniszczyæ?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"Oddzia³ nie mo¿e siê przemieszczaæ, jeœli któryœ z najemników œpi.",

//1-5
	L"Najpierw wyprowadŸ oddzia³ na powierzchniê.",
	L"Rozkazy przemieszczenia? To jest sektor wroga!",
	L"Aby podró¿owaæ najemnicy musz¹ byæ przydzieleni do oddzia³u lub pojazdu.",
	L"Nie masz jeszcze ludzi.", 		// you have no members, can't do anything 
	L"Najemnik nie mo¿e wype³niæ tego rozkazu.",			 		// merc can't comply with your order
//6-10
	L"musi mieæ eskortê, aby siê przemieszczaæ. Umieœæ go w oddziale z eskort¹.", // merc can't move unescorted .. for a male
	L"musi mieæ eskortê, aby siê przemieszczaæ. Umieœæ j¹ w oddziale z eskort¹.", // for a female
	L"Najemnik nie przyby³ jeszcze do Arulco!",
	L"Wygl¹da na to, ¿e trzeba wpierw uregulowaæ sprawy kontraktu.",
	L"Nie mo¿na przemieœciæ najemnika. Trwa nalot powietrzny.",
//11-15
	L"Rozkazy przemieszczenia? Trwa walka!",
	L"Zaatakowa³y ciê dzikie koty, w sektorze %s!",
	L"W sektorze %s znajduje siê coœ, co wygl¹da na legowisko dzikich kotów!", 
	L"", 
	L"Baza rakiet Ziemia-Powietrze zosta³a przejêta.",
//16-20
	L"%s - kopalnia zosta³a przejêta. Twój dzienny przychód zosta³ zredukowany do %s.",
	L"Nieprzyjaciel bezkonfliktowo przej¹³ sektor %s.",
	L"Przynajmniej jeden z twoich najemników nie zosta³ do tego przydzielony.",
	L"%s nie mo¿e siê przy³¹czyæ, poniewa¿ %s jest pe³ny",
	L"%s nie mo¿e siê przy³¹czyæ, poniewa¿ %s jest zbyt daleko.",
//21-25
	L"%s - kopalnia zosta³a przejêta przez si³y Deidranny!",
	L"Si³y wroga w³aœnie zaatakowa³y bazê rakiet Ziemia-Powietrze w - %s.",
	L"Si³y wroga w³aœnie zaatakowa³y - %s.",
	L"W³aœnie zauwa¿ono si³y wroga w - %s.",
	L"Si³y wroga w³aœnie przejê³y - %s.",
//26-30
	L"Przynajmniej jeden z twoich najemników nie móg³ siê po³o¿yæ spaæ.",
	L"Przynajmniej jeden z twoich najemników nie móg³ wstaæ.",
	L"Oddzia³y samoobrony nie pojawi¹ siê dopóki nie zostan¹ wyszkolone.",
	L"%s nie mo¿e siê w tej chwili przemieszczaæ.",
	L"¯o³nierze samoobrony, którzy znajduj¹ siê poza granicami miasta, nie mog¹ byæ przeniesieni do innego sektora.",
//31-35
	L"Nie mo¿esz trenowaæ samoobrony w - %s.",
	L"Pusty pojazd nie mo¿e siê poruszaæ!",
	L"%s ma zbyt wiele ran by podró¿owaæ!",
	L"Musisz wpierw opuœciæ muzeum!",
	L"%s nie ¿yje!",
//36-40
	L"%s nie mo¿e siê zamieniæ z - %s, poniewa¿ siê porusza",
	L"%s nie mo¿e w ten sposób wejœc do pojazdu",
	L"%s nie mo¿e siê do³¹czyæ do - %s",
	L"Nie mo¿esz kompresowaæ czasu dopóki nie zatrudnisz sobie kilku nowych najemników!",
	L"Ten pojazd mo¿e siê poruszaæ tylko po drodze!",
//41-45
	L"Nie mo¿na zmieniaæ przydzia³u najemników, którzy s¹ w drodze",
	L"Pojazd nie ma paliwa!",
	L"%s jest zbyt zmêczony(na) by podró¿owaæ.",
	L"¯aden z pasa¿erów nie jest w stanie kierowaæ tym pojazdem.",
	L"Jeden lub wiêcej cz³onków tego oddzia³u nie mo¿e siê w tej chwili przemieszczaæ.",
//46-50
	L"Jeden lub wiêcej INNYCH cz³onków tego oddzia³u nie mo¿e siê w tej chwili przemieszczaæ.",
	L"Pojazd jest uszkodzony!",
	L"Pamiêtaj, ¿e w jednym sektorze tylko dwóch najemników mo¿e trenowaæ ¿o³nierzy samoobrony.",
	L"Robot nie mo¿e siê poruszaæ bez operatora. Umieœæ ich razem w jednym oddziale.",
};


// help text used during strategic route plotting
STR16 pMapPlotStrings[] =
{
	L"Kliknij ponownie sektor docelowy, aby zatwierdziæ trasê podró¿y, lub kliknij inny sektor, aby j¹ wyd³u¿yæ.",
	L"Trasa podró¿y zatwierdzona.",
	L"Cel podró¿y nie zosta³ zmieniony.",
	L"Trasa podró¿y zosta³a anulowana.",
	L"Trasa podró¿y zosta³a skrócona.",
};


// help text used when moving the merc arrival sector
STR16 pBullseyeStrings[] =
{
	L"Kliknij sektor, do którego maj¹ przylatywaæ najemnicy.",
	L"Dobrze. Przylatuj¹cy najemnicy bêd¹ zrzucani w %s",
	L"Najemnicy nie mog¹ tu przylatywaæ. Przestrzeñ powietrzna nie jest zabezpieczona!",
	L"Anulowano. Sektor zrzutu nie zosta³ zmieniony.",
	L"Przestrzeñ powietrzna nad %s nie jest ju¿ bezpieczna! Sektor zrzutu zosta³ przesuniêty do %s.",
};


// help text for mouse regions

STR16 pMiscMapScreenMouseRegionHelpText[] =
{
	L"Otwórz wyposa¿enie (|E|n|t|e|r)",
	L"Zniszcz przedmiot",
	L"Zamknij wyposa¿enie (|E|n|t|e|r)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"Czy %s ma zostawiæ swój sprzêt w sektorze, w którym siê obecnie znajduje (%s), czy w Dressen (B13), sk¹d odlatuje? ",
	L"Czy %s ma zostawiæ swój sprzêt w sektorze, w którym siê obecnie znajduje (%s), czy w Omercie (A9), sk¹d odlatuje?",
	L"wkrótce odchodzi i zostawi swój sprzêt w Omercie (A9).",
	L"wkrótce odchodzi i zostawi swój sprzêt w Drassen (B13).",
	L"%s wkrótce odchodzi i zostawi swój sprzêt w %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"Czy %s ma zostawiæ swój sprzêt w sektorze, w którym siê obecnie znajduje (%s), czy w Dressen (B13), sk¹d odlatuje? ",
	L"Czy %s ma zostawiæ swój sprzêt w sektorze, w którym siê obecnie znajduje (%s), czy w Omercie (A9), sk¹d odlatuje?",
	L"wkrótce odchodzi i zostawi swój sprzêt w Omercie (A9).",
	L"wkrótce odchodzi i zostawi swój sprzêt w Drassen (B13).",
	L"%s wkrótce odchodzi i zostawi swój sprzêt w %s.",
};


STR16 pMercContractOverStrings[] =
{
	L" zakoñczy³ kontrakt wiêc wyjecha³.", 		// merc's contract is over and has departed
	L" zakoñczy³a kontrakt wiêc wyjecha³a.", 		// merc's contract is over and has departed
	L" - jego kontrakt zosta³ zerwany wiêc odszed³.", 		// merc's contract has been terminated
	L" - jej kontrakt zosta³ zerwany wiêc odesz³a.",		// merc's contract has been terminated
	L"Masz za du¿y d³ug wobec M.E.R.C. wiêc %s odchodzi.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"Nieprawid³owy kod dostêpu",
	L"Czy na pewno chcesz wznowiæ proces okreœlenia profilu?",
	L"WprowadŸ nazwisko oraz p³eæ",
	L"Wstêpna kontrola stanu twoich finansów wykaza³a, ¿e nie staæ ciê na analizê profilu.",
	L"Opcja tym razem nieaktywna.",
	L"Aby wykonaæ profil, musisz mieæ miejsce dla przynajmniej jednego cz³onka za³ogi.",
	L"Profil zosta³ ju¿ wykonany.",
	L"Nie mo¿na za³adowaæ postaci I.M.P. z dysku.",
	L"Wykorzysta³eœ ju¿ maksymaln¹ liczbê postaci I.M.P.",
	L"Masz ju¿ w oddziale trzy postacie I.M.P. tej samej p³ci.",					//L"You have already the maximum number of I.M.P characters with that gender on your team.",  BY£o ->>L"You have already three I.M.P characters with the same gender on your team.",
	L"Nie staæ ciê na postaæ I.M.P.",
	L"Nowa postaæ I.M.P. do³¹czy³a do oddzia³u.",
};


// button labels used on the IMP site

STR16 pImpButtonText[] =
{
	L"O Nas", 			// about the IMP site
	L"ZACZNIJ", 			// begin profiling
	L"Umiejêtnoœci", 		// personality section
	L"Atrybuty", 		// personal stats/attributes section
	L"Appearance", 			// changed from portrait
	L"G³os %d", 			// the voice selection
	L"Gotowe", 			// done profiling
	L"Zacznij od pocz¹tku", 		// start over profiling
	L"Tak, wybieram t¹ odpowiedŸ.", 
	L"Tak", 
	L"Nie",
	L"Skoñczone", 			// finished answering questions
	L"Poprz.", 			// previous question..abbreviated form
	L"Nast.", 			// next question
	L"TAK, JESTEM.", 		// yes, I am certain 
	L"NIE, CHCÊ ZACZ¥Æ OD NOWA.", // no, I want to start over the profiling process
	L"TAK",
	L"NIE",
	L"Wstecz", 			// back one page
	L"Anuluj", 			// cancel selection
	L"Tak.",
	L"Nie, Chcê spojrzeæ jeszcze raz.",
	L"Rejestr", 			// the IMP site registry..when name and gender is selected
	L"Analizujê...", 			// analyzing your profile results
	L"OK",
	L"Postaæ", // Change from "Voice"
};

STR16 pExtraIMPStrings[] =
{
	// These texts have been also slightly changed 
	L"Po wybraniu twoich cech pora wybraæ twoje umiejêtnoœci.",
	L"Wybierz twoje atrybuty.",
	L"Aby dokonaæ prawdziwego profilowania wybie¿ portret, g³os i kolory.",
	L"Teraz, po wybraniu wygl¹du, przejdŸ do analizy postaci.",
};

STR16 pFilesTitle[] =
{
	L"Przegl¹darka plików",
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
	L"Dzieñ", 			// the day the history event occurred
	L"Strona", 			// the current page in the history report we are in
	L"Dzieñ", 			// the days the history report occurs over
	L"Po³o¿enie", 			// location (in sector) the event occurred
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
	L"%s najêty(ta) w A.I.M.", 										// merc was hired from the aim site
	L"%s najêty(ta) w M.E.R.C.", 									// merc was hired from the aim site
	L"%s ginie.", 															// merc was killed
	L"Uregulowano rachunki w M.E.R.C.",								// paid outstanding bills at MERC
	L"Przyjêto zlecenie od Enrico Chivaldori",	
	//6-10
	L"Profil IMP wygenerowany",
	L"Podpisano umowê ubezpieczeniow¹ dla %s.", 				// insurance contract purchased
	L"Anulowano umowê ubezpieczeniow¹ dla %s.", 				// insurance contract canceled
	L"Wyp³ata ubezpieczenia za %s.", 							// insurance claim payout for merc
	L"Przed³u¿ono kontrakt z: %s o 1 dzieñ.", 						// Extented "mercs name"'s for a day
	//11-15
	L"Przed³u¿ono kontrakt z: %s o 1 tydzieñ.", 					// Extented "mercs name"'s for a week
	L"Przed³u¿ono kontrakt z: %s o 2 tygodnie.", 					// Extented "mercs name"'s 2 weeks
	L"%s zwolniony(na).", 													// "merc's name" was dismissed.
	L"%s odchodzi.", 																		// "merc's name" quit.
	L"przyjêto zadanie.", 															// a particular quest started
	//16-20
	L"zadanie wykonane.",
	L"Rozmawiano szefem kopalni %s",									// talked to head miner of town
	L"Wyzwolono - %s",
	L"U¿yto kodu Cheat",
	L"¯ywnoœæ powinna byæ jutro w Omercie",
	//21-25
	L"%s odchodzi, aby wzi¹æ œlub z Darylem Hickiem",
	L"Wygas³ kontrakt z - %s.",
	L"%s zrekrutowany(na).",
	L"Enrico narzeka na brak postêpów",
	L"Walka wygrana",
	//26-30
	L"%s - w kopalni koñczy siê ruda",
	L"%s - w kopalni skoñczy³a siê ruda",
	L"%s - kopalnia zosta³a zamkniêta",
	L"%s - kopalnia zosta³a otwarta",
	L"Informacja o wiêzieniu zwanym Tixa.",
	//31-35
	L"Informacja o tajnej fabryce broni zwanej Orta.",
	L"Naukowiec w Orcie ofiarowa³ kilka karabinów rakietowych.",
	L"Królowa Deidranna robi u¿ytek ze zw³ok.",
	L"Frank opowiedzia³ o walkach w San Monie.",
	L"Pewien pacjent twierdzi, ¿e widzia³ coœ w kopalni.",
	//36-40
	L"Goœæ o imieniu Devin sprzedaje materia³y wybuchowe.",
	L"Spotkanie ze s³awynm eks-najemnikiem A.I.M. - Mike'iem!",
	L"Tony handluje broni¹.",
	L"Otrzymano karabin rakietowy od sier¿anta Krotta.",
	L"Dano Kyle'owi akt w³asnoœci sklepu Angela.",
	//41-45
	L"Madlab zaoferowa³ siê zbudowaæ robota.",
	L"Gabby potrafi zrobiæ miksturê chroni¹c¹ przed robakami.",
	L"Keith wypad³ z interesu.",
	L"Howard dostarcza³ cyjanek królowej Deidrannie.",
	L"Spotkanie z handlarzem Keithem w Cambrii.",
	//46-50
	L"Spotkanie z aptekarzem Howardem w Balime",
	L"Spotkanie z Perko, prowadz¹cym ma³y warsztat.",
	L"Spotkanie z Samem z Balime - prowadzi sklep z narzêdziami.",
	L"Franz handluje sprzêtem elektronicznym.",
	L"Arnold prowadzi warsztat w Grumm.",
	//51-55
	L"Fredo naprawia sprzêt elektroniczny w Grumm.",
	L"Otrzymano darowiznê od bogatego goœcia w Balime.",
	L"Spotkano Jake'a, który prowadzi z³omowisko.",
	L"Jakiœ w³óczêga da³ nam elektroniczn¹ kartê dostêpu.",
	L"Przekupiono Waltera, aby otworzy³ drzwi do piwnicy.",
	//56-60
	L"Dave oferuje darmowe tankowania, jeœli bêdzie mia³ paliwo.",
	L"Greased Pablo's palms.",
	L"Kingpin trzyma pieni¹dze w kopalni w San Mona.",
	L"%s wygra³(a) walkê",
	L"%s przegra³(a) walkê",
	//61-65
	L"%s zdyskwalifikowany(na) podczas walki",
	L"Znaleziono du¿o pieniêdzy w opuszczonej kopalni.",
	L"Spotkano zabójcê nas³anego przez Kingpina.",
	L"Utrata kontroli nad sektorem",				//ENEMY_INVASION_CODE
	L"Sektor obroniony",
	//66-70
	L"Przegrana bitwa",							//ENEMY_ENCOUNTER_CODE
	L"Fatalna zasadzka",						//ENEMY_AMBUSH_CODE
	L"Usunieto zasadzkê wroga",
	L"Nieudany atak",			//ENTERING_ENEMY_SECTOR_CODE
	L"Udany atak!",
	//71-75
	L"Stworzenia zaatakowa³y",			//CREATURE_ATTACK_CODE
	L"Zabity(ta) przez dzikie koty",			//BLOODCAT_AMBUSH_CODE
	L"Wyr¿niêto dzikie koty",
	L"%s zabity(ta)",
	L"Przekazano Carmenowi g³owê terrorysty",
	L"Slay odszed³",
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
	L"Sieæ",
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
};

STR16 pBookmarkTitle[] =
{
	L"Ulubione",
	L"Aby w przysz³oœci otworzyæ to menu, kliknij prawym klawiszem myszy.",
};

// When loading or download a web page

STR16 pDownloadString[] =
{
	L"£adowanie strony...",
	L"Otwieranie strony...",
};

//This is the text used on the bank machines, here called ATMs for Automatic Teller Machine

STR16 gsAtmSideButtonText[] =
{
	L"OK",
	L"WeŸ", 			// take money from merc
	L"Daj", 			// give money to merc
	L"Anuluj", 			// cancel transaction
	L"Skasuj", 			// clear amount being displayed on the screen
};

STR16 gsAtmStartButtonText[] =
{
	L"Transfer $", 		// transfer money to merc -- short form
	L"Atrybuty", 			// view stats of the merc
	L"Wyposa¿enie", 			// view the inventory of the merc
	L"Zatrudnienie",
};

STR16 sATMText[ ]=
{
	L"Przes³aæ fundusze?", 		// transfer funds to merc?
	L"OK?", 			// are we certain?
	L"WprowadŸ kwotê", 		// enter the amount you want to transfer to merc
	L"Wybierz typ", 		// select the type of transfer to merc
	L"Brak œrodków", 	// not enough money to transfer to merc
	L"Kwota musi byæ podzielna przez $10", // transfer amount must be a multiple of $10
};

// Web error messages. Please use foreign language equivilant for these messages. 
// DNS is the acronym for Domain Name Server
// URL is the acronym for Uniform Resource Locator

STR16 pErrorStrings[] =
{
	L"B³¹d",
	L"Serwer nie posiada DNS.",	
	L"SprawdŸ adres URL i spróbuj ponownie.",
	L"OK",
	L"Niestabilne po³¹czenie z Hostem. Transfer mo¿e trwaæ d³u¿ej.",
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
	L"A.I.M. Cz³onkowie",
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
	L"Bobby Ray's - Broñ",
	L"Bobby Ray's - Amunicja",
	L"Bobby Ray's - Pancerz",
	L"Bobby Ray's - Ró¿ne",							//misc is an abbreviation for miscellaneous				
	L"Bobby Ray's - U¿ywane",
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
	L"McGillicutty - Zak³ad pogrzebowy",
	L"",
	L"Nie odnaleziono URL.",
	L"Bobby Ray's - Ostatnie dostawy",
	L"",
	L"",
};

STR16 pShowBookmarkString[] =
{
	L"Sir-Pomoc",
	L"Kliknij ponownie Sieæ by otworzyæ menu Ulubione.",
};

STR16 pLaptopTitles[] =
{
	L"Poczta",
	L"Przegl¹darka plików",
	L"Personel",
	L"Ksiêgowy Plus",
	L"Historia",
};

STR16 pPersonnelDepartedStateStrings[] =
{
	//reasons why a merc has left.
	L"Œmieræ w akcji",
	L"Zwolnienie",
	L"Inny",
	L"Ma³¿eñstwo",
	L"Koniec kontraktu",
	L"Rezygnacja",
};
// personnel strings appearing in the Personnel Manager on the laptop

STR16 pPersonelTeamStrings[] =
{
	L"Bie¿¹cy oddzia³",
	L"Wyjazdy",
	L"Koszt dzienny:",
	L"Najwy¿szy koszt:",
	L"Najni¿szy koszt:",
	L"Œmieræ w akcji:",
	L"Zwolnienie:",
	L"Inny:",
};


STR16 pPersonnelCurrentTeamStatsStrings[] =
{
	L"Najni¿szy",
	L"Œredni",
	L"Najwy¿szy",
};


STR16 pPersonnelTeamStatsStrings[] =
{
	L"ZDR",
	L"ZWN",
	L"ZRCZ",
	L"SI£A",
	L"DOW",
	L"INT",
	L"DOŒW",
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
	L"Pokonano ciê w tym sektorze!",
	L"Wróg nie zna litoœci i po¿era was wszystkich!",
	L"Nieprzytomni cz³onkowie twojego oddzia³u zostali pojmani!",
	L"Cz³onkowie twojego oddzia³u zostali uwiêzieni.",
};


//Insurance Contract.c
//The text on the buttons at the bottom of the screen.

STR16			InsContractText[] = 
{ 
	L"Wstecz",
	L"Dalej",
	L"Akceptujê",
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
	L"Strona g³ówna",
	L"Konto #:",
	L"Najemnik",
	L"Dni",
	L"Stawka",	//5
	L"Op³ata",
	L"Razem:",
	L"Czy na pewno chcesz zatwierdziæ p³atnoœæ: %s?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
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
	L"Zwinnoœæ",
	L"Sprawnoœæ",
	L"Si³a",
	L"Um. dowodz.",
	L"Inteligencja",
	L"Poz. doœwiadczenia",
	L"Um. strzeleckie",
	L"Zn. mechaniki",
	L"Mat. wybuchowe",
	L"Wiedza medyczna",

	L"Poprzedni",
	L"Najmij",
	L"Nastêpny",
	L"Dodatkowe informacje",
	L"Strona g³ówna",
	L"Najêty",
	L"Koszt:",
	L"Dziennie",
	L"Nie ¿yje",

	L"Wygl¹da na to, ¿e chcesz wynaj¹æ zbyt wielu najemników. Limit wynosi 18.",
	L"Niedostêpny",
};



// For use at the M.E.R.C. web site. Text relating to opening an account with MERC

STR16			MercNoAccountText[] =
{
	//Text on the buttons at the bottom of the screen
	L"Otwórz konto",
	L"Anuluj",
	L"Nie posiadasz konta. Czy chcesz sobie za³o¿yæ?"
};



// For use at the M.E.R.C. web site. MERC Homepage

STR16			MercHomePageText[] =
{
	//Description of various parts on the MERC page
	L"Speck T. Kline, za³o¿yciel i w³aœciciel",
	L"Aby otworzyæ konto naciœnij tu",
	L"Aby zobaczyæ konto naciœnij tu",
	L"Aby obejrzeæ akta naciœnij tu",
	// The version number on the video conferencing system that pops up when Speck is talking
	L"Speck Com v3.2",
	L"Transfer failed. No funds available.",	// TODO.Translate
};

// For use at MiGillicutty's Web Page.

STR16			sFuneralString[] =
{
	L"Zak³ad pogrzebowy McGillicutty, pomaga rodzinom pogr¹¿onym w smutku od 1983.",
	L"Kierownik, by³y najemnik A.I.M. Murray \'Pops\' McGillicutty jest doœwiadczonym pracownikiem zak³adu pogrzebowego.",
	L"Przez ca³e ¿ycie obcowa³ ze œmierci¹, 'Pops' wie jak trudne s¹ te chwile.",
	L"Zak³ad pogrzebowy McGillicutty oferuje szeroki zakres us³ug, od duchowego wsparcia po rekonstrukcjê silnie zniekszta³conych zw³ok.",
	L"Pozwól by McGillicutty ci pomóg³ a twój ukochany bêdzie spoczywa³ w pokoju.",

	// Text for the various links available at the bottom of the page
	L"WYŒLIJ KWIATY",
	L"KOLEKCJA TRUMIEN I URN",
	L"US£UGI KREMA- CYJNE",
	L"US£UGI PLANOWANIA POGRZEBU",
	L"KARTKI POGRZE- BOWE",

	// The text that comes up when you click on any of the links ( except for send flowers ).
	L"Niestety, z powodu œmierci w rodzinie, nie dzia³aj¹ jeszcze wszystkie elementy tej strony.",
	L"Przepraszamy za powy¿sze uniedogodnienie."
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

	L"Dzia³amy szybko i sprawnie!",	
	L"Gwarantujemy dostawê w dowolny punkt na Ziemi, nastêpnego dnia po z³o¿eniu zamówienia!",
	L"Oferujemy najni¿sze ceny na œwiecie!",	
	L"Poka¿ nam ofertê z ni¿sz¹ cen¹, a dostaniesz w nagrodê tuzin ró¿, za darmo!",
	L"Lataj¹ca flora, fauna i kwiaty od 1981.",
	L"Nasz ozdobiony bombowiec zrzuci twój bukiet w promieniu co najwy¿ej dziesiêciu mil od ¿¹danego miejsca. Kiedy tylko zechcesz!",
	L"Pozwól nam zaspokoiæ twoje kwieciste fantazje.",
	L"Bruce, nasz œwiatowej renomy projektant bukietów, zerwie dla ciebie najœwie¿sze i najwspanialsze kwiaty z naszej szklarni.",
	L"I pamiêtaj, jeœli czegoœ nie mamy, mo¿emy to szybko zasadziæ!"
};



//Florist OrderForm

STR16			sOrderFormText[] = 
{
	//Text on the buttons

	L"Powrót",
	L"Wyœlij",
	L"Skasuj",
	L"Galeria",

	L"Nazwa bukietu:",
	L"Cena:",			//5
	L"Zamówienie numer:",
	L"Czas dostawy",
	L"nast. dnia",
	L"dostawa gdy to bêdzie mo¿liwe",
	L"Miejsce dostawy",			//10
	L"Dodatkowe us³ugi",
	L"Zgnieciony bukiet($10)",
	L"Czarne Ró¿e($20)",
	L"Zwiêdniêty bukiet($10)",
	L"Ciasto owocowe (je¿eli bêdzie)($10)",		//15
	L"Osobiste kondolencje:",
	L"Ze wzglêdu na rozmiar karteczek, tekst nie mo¿e zawieraæ wiêcej ni¿ 75 znaków.",
	L"...mo¿esz te¿ przejrzeæ nasze",

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

	L"Kliknij wybran¹ pozycjê aby z³o¿yæ zamówienie.",
	L"Uwaga: $10 dodatkowej op³aty za zwiêdniêty lub zgnieciony bukiet.",

	//text on the button

	L"G³ówna",
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
	L"Iloœæ",					// The number of items ordered
	L"Waga (%s)",			// The weight of the item
	L"Nazwa",				// The name of the item
	L"Cena",				// the item's weight
	L"Wartoœæ",				//5	// The total price of all of items of the same type
	L"W sumie",				// The sub total of all the item totals added
	L"Transport",		// S&H is an acronym for Shipping and Handling 
	L"Razem",			// The grand total of all item totals + the shipping and handling
	L"Miejsce dostawy",	
	L"Czas dostawy",			//10	// See below
	L"Koszt (za %s.)",			// The cost to ship the items
	L"Ekspres - 24h",			// Gets deliverd the next day
	L"2 dni robocze",			// Gets delivered in 2 days
	L"Standardowa dostawa",			// Gets delivered in 3 days
	L" Wyczyœæ",//15			// Clears the order page
	L" Akceptujê",			// Accept the order
	L"Wstecz",				// text on the button that returns to the previous page
	L"Strona g³ówna",				// Text on the button that returns to the home page
	L"* oznacza u¿ywane rzeczy",		// Disclaimer stating that the item is used
	L"Nie staæ ciê na to.",		//20	// A popup message that to warn of not enough money
	L"<BRAK>",				// Gets displayed when there is no valid city selected
	L"Miejsce docelowe przesy³ki: %s. Potwierdzasz?",		// A popup that asks if the city selected is the correct one
	L"Waga przesy³ki*",			// Displays the weight of the package
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
	L"Broñ",
	L"Pancerz",
	L"Oporz¹dzenie",
	L"Ró¿ne",

	// Armour
	L"He³my",
	L"Kamizelki",
	L"Getry ochronne",
	L"P³ytki ceramiczne",

	// Misc
	L"Ostrza",
	L"No¿e do rzucania",
	L"Blunt W.", // TODO.Translate
	L"Granaty",
	L"Bomby",
	L"Apteczki",
	L"Ekwipunek",
	L"Na twarz",
	L"Oporz¹dzenie",	//LBE Gear
	L"Inne",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Zamów",				// Title
	// instructions on how to order
	L"Kliknij wybrane towary. Lewym klawiszem zwiêkszasz iloœæ towaru, a prawym zmniejszasz. Gdy ju¿ skompletujesz swoje zakupy przejdŸ do formularza zamówienia.",			

	//Text on the buttons to go the various links

	L"Poprzednia",		// 
	L"Broñ", 			//3
	L"Amunicja",			//4
	L"Ochraniacze",			//5
	L"Ró¿ne",			//6	//misc is an abbreviation for miscellaneous
	L"U¿ywane",			//7
	L"Nastêpna",
	L"FORMULARZ",
	L"Strona g³ówna",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Twój zespó³ posiada",//11
	L"szt. broni do której pasuje amunicja tego typu", //12

	//The following lines provide information on the items

	L"Waga:",			// Weight of all the items of the same type
	L"Kal:",			// the caliber of the gun
	L"Mag:",			// number of rounds of ammo the Magazine can hold
	L"Zas:",				// The range of the gun
	L"Si³a:",				// Damage of the weapon	
	L"CS:",			// Weapon's Rate Of Fire, acronym ROF
	L"Koszt:",			// Cost of the item
	L"Na stanie:",			// The number of items still in the store's inventory
	L"Iloœæ na zamów.:",		// The number of items on order
	L"Uszkodz.",			// If the item is damaged
	L"Waga:",			// the Weight of the item
	L"Razem:",			// The total cost of all items on order
	L"* Stan: %%",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Przepraszamy za to utrudnienie, ale na jednym zamówieniu mo¿e siê znajdowaæ tylko 10 pozycji! Jeœli potrzebujesz wiêcej, z³ó¿ kolejne zamówienie.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Przykro nam. Chwilowo nie mamy tego wiêcej na magazynie. Proszê spróbowaæ póŸniej.",

	//A popup that tells the user that the store is temporarily sold out

	L"Przykro nam, ale chwilowo nie mamy tego towaru na magazynie",

};


// Text for Bobby Ray's Home Page

STR16			BobbyRaysFrontText[] =
{
	//Details on the web site

	L"Tu znajdziesz nowoœci z dziedziny broni i osprzêtu wojskowego",
	L"Zaspokoimy wszystkie twoje potrzeby w dziedzinie materia³ów wybuchowych",
	L"U¯YWANE RZECZY",

	//Text for the various links to the sub pages

	L"RÓ¯NE",
	L"BROÑ",
	L"AMUNICJA",		//5
	L"OCHRANIACZE",

	//Details on the web site

	L"Jeœli MY tego nie mamy, to znaczy, ¿e nigdzie tego nie dostaniesz!",
	L"W trakcie budowy",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"Cz³onkowie A.I.M.",				// Title
	// Title for the way to sort
	L"Sortuj wg:",					

	// sort by...

	L"Ceny",											
	L"Doœwiadczenia",
	L"Um. strzeleckich",
	L"Um. med.",
	L"Zn. mat. wyb.",
	L"Zn. mechaniki",

	//Text of the links to other AIM pages

	L"Portrety najemników",
	L"Akta najemnika",
	L"Poka¿ galeriê by³ych cz³onków A.I.M.",

	// text to display how the entries will be sorted

	L"Rosn¹co",
	L"Malej¹co",
};


//Aim Policies.c
//The page in which the AIM policies and regulations are displayed

STR16		AimPolicyText[] =
{
	// The text on the buttons at the bottom of the page

	L"Poprzednia str.",
	L"Strona g³ówna", 
	L"Przepisy",
	L"Nastêpna str.",
	L"Rezygnujê",
	L"Akceptujê",
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
	L"Zwinnoœæ",
	L"Sprawnoœæ",
	L"Si³a",
	L"Um. dowodzenia",
	L"Inteligencja",
	L"Poziom doœw.",
	L"Um. strzeleckie",
	L"Zn. mechaniki",
	L"Zn. mat. wyb.",
	L"Wiedza med.",				//10

	// the contract expenses' area

	L"Zap³ata",
	L"Czas",
	L"1 dzieñ",
	L"1 tydzieñ",
	L"2 tygodnie",

	// text for the buttons that either go to the previous merc, 
	// start talking to the merc, or go to the next merc

	L"Poprzedni",
	L"Kontakt",
	L"Nastêpny",

	L"Dodatkowe informacje",				// Title for the additional info for the merc's bio
	L"Aktywni cz³onkowie",		//20		// Title of the page
	L"Opcjonalne wyposa¿enie:",				// Displays the optional gear cost
	L"gear",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"Wymagany jest zastaw na ¿ycie",			// If the merc required a medical deposit, this is displayed
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
	L"Wartoœæ kontraktu:",				//Title beside the cost of hiring the merc

	//Text on the buttons to select the length of time the merc can be hired

	L"Jeden dzieñ",										
	L"Jeden tydzieñ",
	L"Dwa tygodnie",

	//Text on the buttons to determine if you want the merc to come with the equipment 

	L"Bez sprzêtu",
	L"WeŸ sprzêt",

	// Text on the Buttons

	L"TRANSFER",			// to actually hire the merc
	L"ANULUJ",				// go back to the previous menu
	L"WYNAJMIJ",				// go to menu in which you can hire the merc
	L"ROZ£¥CZ",				// stops talking with the merc
	L"OK",									
	L"NAGRAJ SIÊ",			// if the merc is not there, you can leave a message 

	//Text on the top of the video conference popup

	L"Wideo konferencja z - ",		
	L"£¹czê. . .",

	L"z zastawem"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"TRANSFER ZAKOÑCZONY POMYŒLNIE",	// You hired the merc
	L"PRZEPROWADZENIE TRANSFERU NIE MO¯LIWE",		// Player doesn't have enough money, message 1
	L"BRAK ŒRODKÓW",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"Wynajêto",											
	L"Proszê zostaw wiadomoœæ",
	L"Nie ¿yje",

	//If you try to hire more mercs than game can support

	L"You have a full team of mercs already.",

	L"Nagrana wiadomoœæ",
	L"Wiadomoœæ zapisana",
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
	L"Strona g³ówna", 
	L"Byli cz³onkowie",
	L"Nastêpna str."
};


//Aim Mug Shot Index

//The page in which all the AIM members' portraits are displayed in the order selected by the AIM sort page.

STR16			AimFiText[] =
{
	// displays the way in which the mercs were sorted

	L"ceny",
	L"doœwiadczenia",
	L"um. strzeleckich",
	L"um. medycznych",
	L"zn. materia³ów wyb.",
	L"zn. mechaniki",

	// The title of the page, the above text gets added at the end of this text

	L"Cz³onkowie A.I.M. posortowani rosn¹co wg %s",
	L"Cz³onkowie A.I.M. posortowani malej¹co wg %s",

	// Instructions to the players on what to do

	L"Lewy klawisz",
	L"Wybór najemnika",			//10
	L"Prawy klawisz",
	L"Opcje sortowania",

	// Gets displayed on top of the merc's portrait if they are...

	L"Wyjecha³(a)",
	L"Nie ¿yje",						//14
	L"Wynajêto",
};



//AimArchives.
// The page that displays information about the older AIM alumni merc... mercs who are no longer with AIM

STR16			AimAlumniText[] =
{
	// Text of the buttons

	L"STRONA 1",
	L"STRONA 2",
	L"STRONA 3",

	L"Byli cz³onkowie A.I.M.",	// Title of the page


	L"OK"			// Stops displaying information on selected merc
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"Znaki A.I.M. i logo A.I.M. s¹ prawnie chronione w wiêkszoœci krajów.",
	L"Wiêc nawet nie myœl o próbie ich podrobienia.",
	L"Wszelkie prawa zastrze¿one A.I.M. 2005, Ltd.",

	//Text for an advertisement that gets displayed on the AIM page

	L"United Floral Service",
	L"\"Zrzucamy gdziekolwiek\"",				//10
	L"Zrób to jak nale¿y...",
	L"...za pierwszym razem",
	L"Broñ i akcesoria, jeœli czegoœ nie mamy, to tego nie potrzebujesz.",
};


//Aim Home Page

STR16			AimBottomMenuText[] =
{
	//Text for the links at the bottom of all AIM pages
	L"Strona g³ówna",
	L"Cz³onkowie",
	L"Byli cz³onkowie",
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
	L"KOSZT OGÓ£EM",				//The total cost of the the items in the Dealer inventory area
	L"WARTOŒÆ OGÓ£EM",			//The total value of items player wishes to sell
	L"WYCENA",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSAKCJA",			//Button text which completes the deal. Makes the transaction.
	L"OK",				//Text for the button which will leave the shopkeeper interface.
	L"KOSZT NAPRAWY",			//The amount the dealer will charge to repair the merc's goods
	L"1 GODZINA",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d GODZIN(Y)",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"NAPRAWIONO",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Brak miejsca by zaoferowaæ wiêcej rzeczy.",	//Message box that tells the user there is no more room to put there stuff
	L"%d MINUT(Y)",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Upuœæ przedmiot na ziemiê.",
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
	L"WeŸ",					// Take money from the player
	L"Daj",					// Give money to the player
	L"Anuluj",					// Cancel the transfer
	L"Skasuj",					// Clear the money display
};


//Shopkeeper Interface
STR16	gzSkiAtmText[] = 
{	

	// Text on the bank machine panel that....
	L"Wybierz",			// tells the user to select either to give or take from the merc
	L"WprowadŸ kwotê",			// Enter the amount to transfer
	L"Transfer gotówki do najemnika",		// Giving money to the merc
	L"Transfer gotówki od najemnika",		// Taking money from the merc
	L"Brak œrodków",			// Not enough money to transfer
	L"Saldo",				// Display the amount of money the player currently has
};


STR16	SkiMessageBoxText[] =
{
	L"Czy chcesz do³o¿yæ %s ze swojego konta, aby pokryæ ró¿nicê?",
	L"Brak œrodków. Brakuje ci %s",
	L"Czy chcesz przeznaczyæ %s ze swojego konta, aby pokryæ koszty?",
	L"Poproœ o rozpoczêcie transakscji",
	L"Poproœ o naprawê wybranych przedmiotów",
	L"Zakoñcz rozmowê",
	L"Saldo dostêpne",
};


//OptionScreen.c

STR16	zOptionsText[] = 
{
	//button Text
	L"Zapisz grê",
	L"Odczytaj grê",
	L"Wyjœcie",
	L">>",
	L"<<",
	L"OK",

	//Text above the slider bars
	L"Efekty",
	L"Dialogi",
	L"Muzyka",

	//Confirmation pop when the user selects..
	L"Zakoñczyæ grê i wróciæ do g³ównego menu?",

	L"Musisz w³¹czyæ opcjê dialogów lub napisów.",
};


//SaveLoadScreen 
STR16			zSaveLoadText[] = 
{
	L"Zapisz grê",
	L"Odczytaj grê",
	L"Anuluj",
	L"Zapisz wybran¹",
	L"Odczytaj wybran¹",

	L"Gra zosta³a pomyœlnie zapisana",
	L"B£¥D podczas zapisu gry!",
	L"Gra zosta³a pomyœlnie odczytana",
	L"B£¥D podczas odczytu gry!",

	L"Wersja gry w zapisanym pliku ró¿ni siê od bie¿¹cej. Prawdopodobnie mo¿na bezpiecznie kontynuowaæ. Kontynuowaæ?",
	L"Zapisane pliki gier mog¹ byæ uszkodzone. Czy chcesz je usun¹æ?",

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.	
#ifdef JA2BETAVERSION
	L"Nieprawid³owa wersja zapisu gry. W razie problemów prosimy o raport. Kontynuowaæ?",
#else
	L"Próba odczytu starszej wersji zapisu gry. Zaktualizowaæ ten zapis i odczytaæ grê?",
#endif

	//Translators, the next two strings are for the same thing.  The first one is for beta version releases and the second one
	//is used for the final version.  Please don't modify the "#ifdef JA2BETAVERSION" or the "#else" or the "#endif" as they are
	//used by the compiler and will cause program errors if modified/removed.  It's okay to translate the strings though.
#ifdef JA2BETAVERSION
	L"Nieprawid³owa wersja zapisu gry. W razie problemów prosimy o raport. Kontynuowaæ?",
#else
	L"Próba odczytu starszej wersji zapisu gry. Zaktualizowaæ ten zapis i odczytaæ grê?",
#endif

	L"Czy na pewno chcesz nadpisaæ grê na pozycji %d?",
	L"Chcesz odczytaæ grê z pozycji",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Brak miejsca na dysku twardym.  Na dysku wolne jest %d MB, a wymagane jest przynajmniej %d MB.",	

	L"Zapisujê...",			//When saving a game, a message box with this string appears on the screen

	L"Standardowe uzbrojenie",
	L"Ca³e mnóstwo broni",
	L"Realistyczna gra",
	L"Elementy S-F",

	L"Stopieñ trudnoœci",
	L"Platynowy tryb", //Placeholder English

	L"Wyposa¿enie Bobby Ray's",
	L"Normalne",
	L"Œwietne",
	L"Wyœmienite",
	L"Niewiarygodne",

	L"Nowy inwentarz nie dzia³a w rozdzielczoœci 640x480. Aby z niego korzystaæ zmieñ rozdzielczoœæ i spróbuj ponownie.",
	L"Nowy inwentarz nie korzysta z domyœlnego folderu 'Data'.",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Poziom mapy",
	L"Nie masz jeszcze ¿o³nierzy samoobrony.  Musisz najpierw wytrenowaæ mieszkañców miast.",
	L"Dzienny przychód",
	L"Najmemnik ma polisê ubezpieczeniow¹", 
    L"%s nie potrzebuje snu.", 
	L"%s jest w drodze i nie mo¿e spaæ", 
	L"%s jest zbyt zmêczony(na), spróbuj trochê póŸniej.",
	L"%s prowadzi.",
	L"Oddzia³ nie mo¿e siê poruszaæ je¿eli jeden z najemników œpi.",

	// stuff for contracts
	L"Mimo, ¿e mo¿esz op³aciæ kontrakt, to jednak nie masz gotówki by op³aciæ sk³adkê ubezpieczeniow¹ za najemnika.",
	L"%s - sk³adka ubezpieczeniowa najemnika bêdzie kosztowaæ %s za %d dzieñ(dni). Czy chcesz j¹ op³aciæ?",
	L"Inwentarz sektora",
	L"Najemnik posiada zastaw na ¿ycie.", 
	
	// other items
	L"Lekarze", // people acting a field medics and bandaging wounded mercs 
	L"Pacjenci", // people who are being bandaged by a medic 
	L"Gotowe", // Continue on with the game after autobandage is complete 
	L"Przerwij", // Stop autobandaging of patients by medics now 
	L"Przykro nam, ale ta opcja jest wy³¹czona w wersji demo.", // informs player this option/button has been disabled in the demo 
	L"%s nie ma zestawu narzêdzi.",
	L"%s nie ma apteczki.",
	L"Brak chêtnych ludzi do szkolenia, w tej chwili.",
	L"%s posiada ju¿ maksymaln¹ liczbê oddzia³ów samoobrony.",
	L"Najemnik ma kontrakt na okreœlony czas.",
	L"Kontrakt najemnika nie jest ubezpieczony",
	L"Mapa",		// 24
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"You currently have no Mobile Militia. Return to this view mode once you've recruited some.",
	L"This view shows where your Mobile Militia can and cannot go. GREY = Mobile Militia refuse to go here. RED = Mobile Militia can go here, but you've told them not to. YELLOW = Mobile Militia can enter this sector, but not leave. GREEN = Mobile Militia can go here freely. Right click a Green/Yellow sector to cycle its behavior.",
};


STR16 pLandMarkInSectorString[] =
{
	L"Oddzia³ %d zauwa¿y³ kogoœ w sektorze %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"Szkolenie oddzia³u samoobrony bêdzie kosztowa³o $", // telling player how much it will cost
	L"Zatwierdzasz wydatek?", // asking player if they wish to pay the amount requested
	L"Nie staæ ciê na to.", // telling the player they can't afford to train this town
	L"Kontynuowaæ szkolenie samoobrony w - %s (%s %d)?", // continue training this town?
	L"Koszt $", // the cost in dollars to train militia
	L"( T/N )",   // abbreviated yes/no
	L"",	// unused
	L"Szkolenie samoobrony w %d sektorach bêdzie kosztowa³o $ %d. %s", // cost to train sveral sectors at once
	L"Nie masz %d$, aby wyszkoliæ samoobronê w tym mieœcie.",
	L"%s musi mieæ %d% lojalnoœci, aby mo¿na by³o kontynuowaæ szkolenie samoobrony.",
	L"Nie mo¿esz ju¿ d³u¿ej szkoliæ samoobrony w mieœcie %s.",
	L"Nie masz %d$ i nie mo¿esz trenowaæ tutaj ruchomej samoobrony.", // HEADROCK HAM 3.6: Mobile Militia
	L"Kontynuuj trening ruchomej samoobrony w %s (%s %d)?", // HEADROCK HAM 3.6: Mobile Militia
	L"Trenowanie ruchomej samoobrony w %d sektorach bêdzie kosztowaæ %d$. %s", // HEADROCK HAM 3.6: Mobile Militia
	L"Trenowanie ruchomej samoobrony bêdzie kosztowaæ $", // HEADROCK HAM 3.6: Mobile Militia
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"Jednorazowo mo¿esz wyp³aciæ do 20,000$.",
	L"Czy na pewno chcesz wp³aciæ %s na swoje konto?",
};

STR16	gzCopyrightText[] = 
{
	L"Prawa autorskie nale¿¹ do (C) 1999 Sir-tech Canada Ltd. Wszelkie prawa zastrze¿one.",
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
	L"Pokazuj trasê ruchu",
	L"Pokazuj chybione strza³y",
	L"Potwierdzenia Real-Time",
	L"Najemnik œpi/budzi siê",
	L"U¿ywaj systemu metrycznego",
	L"Oœwietlenie podczas ruchu",
	L"Przyci¹gaj kursor do najemników",
	L"Przyci¹gaj kursor do drzwi",
	L"Pulsuj¹ce przedmioty",
	L"Pokazuj korony drzew",
	L"Pokazuj siatkê",
	L"Pokazuj kursor 3D",
	L"Pokazuj szansê na trafienie",
	L"Zamiana kursora granatnika",
	L"Pozwól na przechwa³ki wrogów", // Changed from "Enemies Drop all Items" - SANDRO
	L"Wysoki k¹t strza³ów z granatnika",
	L"Pozwól na skradanie siê w czasie rzeczywistym", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"Spacja = nastêpny oddzia³",
	L"Pokazuj cienie przedmiotów",
	L"Pokazuj zasiêg broni w polach",
	L"Efekt smugowy dla poj. strza³u",
	L"Odg³osy padaj¹cego deszczu",
	L"Pokazuj wrony",
	L"Show Soldier Tooltips",
	L"Automatyczny zapis",
	L"Cichy Skyrider",
	//L"Niskie obci¹¿enie procesora",
	L"Rozszerzone Okno Opisu (EDB)",	//Enhanced Description Box
	L"Wymuœ tryb turowy",					// add forced turn mode
	L"Stat Progress Bars",					// Show progress towards stat increase		// TODO.Translate
	L"Alternate Strategy-Map Colors",		// Change color scheme of Strategic Map
	L"Alternate bullet graphics",			// Show alternate bullet graphics (tracers) // TODO.Translate
	L"Activate New CTH system",				// use NCTH
	L"Show Face gear graphics",				// TODO.Translate
	L"Show Face gear icons",
	L"Disable Cursor Swap",		            // Disable Cursor Swap		// TODO.Translate
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
	L"Jeœli W£¥CZONE, w grze pojawiaæ siê bêd¹ dialogi.",

	//Mute Confirmation
	L"Jeœli W£¥CZONE, g³osowe potwierdzenia postaci zostan¹ wyciszone.",

	//Subtitles
	L"Jeœli W£¥CZONE, pojawiaæ siê bêd¹ napisy podczas rozmów z innymi postaciami.",

	//Key to advance speech
	L"Jeœli W£¥CZONE, napisy pojawiaj¹ce siê podczas dialogów bêd¹ znika³y dopiero po klikniêciu.",

	//Toggle smoke animation
	L"Jeœli W£¥CZONE, dym z granatów bêdzie animowany. Mo¿e spowolniæ dzia³anie gry.",

	//Blood n Gore
	L"Jeœli W£¥CZONE, pokazywane bêd¹ bardzo drastyczne sceny.",

	//Never move my mouse
	L"Jeœli W£¥CZONE, kursor nie bêdzie automatycznie ustawia³ siê nad pojawiaj¹cymi siê okienkami dialogowymi.",

	//Old selection method
	L"Jeœli W£¥CZONE, wybór postaci bêdzie dzia³a³ tak jak w poprzednich czêœciach gry.",

	//Show movement path
	L"Jeœli W£¥CZONE, bêdziesz widzia³ trasê ruchu w trybie Real-Time.",

	//show misses
	L"Jeœli W£¥CZONE, bêdzie móg³ obserwowaæ w co trafiaj¹ twoje kule gdy spud³ujesz.",
	
	//Real Time Confirmation
	L"Jeœli W£¥CZONE, ka¿dy ruch najemnika w trybie Real-Time bêdzie wymaga³ dodatkowego, potwierdzaj¹cego klikniêcia.",

	//Sleep/Wake notification
	L"Jeœli W£¥CZONE, wyœwietlana bêdzie informacja, ¿e najemnik po³o¿y³ siê spaæ lub wsta³ i wróci³ do pracy.",

	//Use the metric system
	L"Jeœli W£¥CZONE, gra bêdzie u¿ywa³a systemu metrycznego.",

	//Merc Lighted movement
	L"Jeœli W£¥CZONE, teren wokó³ najemnika bêdzie oœwietlony podczas ruchu. Mo¿e spowolniæ dzia³anie gry.",

	//Smart cursor
	L"Jeœli W£¥CZONE, kursor bêdzie automatycznie ustawia³ siê na najemnikach gdy znajdzie siê w ich pobli¿u.",

	//snap cursor to the door
	L"Jeœli W£¥CZONE, kursor bêdzie automatycznie ustawia³ siê na drzwiach gdy znajdzie siê w ich pobli¿u.",

	//glow items 
	L"Jeœli W£¥CZONE, przedmioty bêd¹ pulsowaæ. ( |I )",

	//toggle tree tops
	L"Jeœli W£¥CZONE, wyœwietlane bêd¹ korony drzew. ( |T )",

	//toggle wireframe
	L"Jeœli W£¥CZONE, wyœwietlane bêd¹ zarysy niewidocznych œcian. ( |W )",

	L"Jeœli W£¥CZONE, kursor ruchu wyœwietlany bêdzie w 3D. ( |Home )",

	// Options for 1.13
	L"Jeœli W£¥CZONE, kursor bêdzie pokazywa³ szansê na trafienie.",
	L"Jeœli W£¥CZONE, seria z granatnika bêdzie u¿ywa³a kursora serii z broni palnej.",
	L"Je¿eli w³¹czone, to wrogowie bêd¹ czasami komentowaæ pewne akcje.", // Changed from Enemies Drop All Items - SANDRO
	L"Jeœli W£¥CZONE, granatniki bêd¹ strzela³y pod wysokim k¹tem. ( |Q )",
	L"Jeœli W£¥CZONE, zapobiega przejœciu do trybu turowego po zauwa¿eniu wroga podczas skradania. Aby wymusiæ tryb turowy z t¹ opcj¹ aktywn¹ naciœnij |C|t|r+|X.", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Jeœli W£¥CZONE, |S|p|a|c|j|a wybiera kolejny oddzia³.",
	L"Jeœli W£¥CZONE, pokazywane bêd¹ cienie przedmiotów.",
	L"Jeœli W£¥CZONE, zasiêg broni bêdzie wyœwietlany w polach.",
	L"Jeœli W£¥CZONE, pojedynczy strza³ bêdzie z efektem pocisku smugowego",
	L"Jeœli W£¥CZONE, bêdziesz s³ysza³ padaj¹cy deszcz.",
	L"Jeœli W£¥CZONE, w grze pojawiaæ siê bêd¹ wrony.",
	L"Gdy ta opcja jest AKTYWNA, wskazanie postaci wroga kursorem i naciœniêcie A|l|t uka¿e okienko informacji dodatkowych.",
	L"Jeœli W£¥CZONE, gra bêdzie zapisywana ka¿dorazowo po zakoñczeniu tury gracza.",
	L"Jeœli W£¥CZONE, Skyrider nie bêdzie nic mówi³.",
	L"Jeœli W£¥CZONE, gra bêdzie obci¹¿a³a procesor w mniejszym stopniu.",
	//L"Jeœli W£¥CZONE, rozszerzone opisy bêd¹ pokazane dla przedmiotów i broni.",
	L"Jeœli W£¥CZONE i wróg jest obecny, \ntryb turowy jest w³¹czony, \ndopóki sektor nie zostanie oczyszczony (|C|T|R|L+|S|H|I|F|T+|A|L|T+|T).",	// add forced turn mode
	L"Gdy W£¥CZONE, pokazuje postêp w doœwiadczeniu postaci.",
	L"When ON, the Strategic Map will be colored differently based on exploration.",
	L"Gdy W£¥CZONE, zastêpuje star¹ animacjê pocisku now¹.",
	L"When ON, New CTH system and cursor is used.",
	L"When ON, you will see the equipped face gear on the merc portraits.",	// TODO.Translate
	L"When ON, you will see icons for the equipped face gear on the merc portraits in the lower right corner.",
	L"When ON, the cursor will not toggle between exchange position and other actions. Press |x to initiate quick exchange.",	// TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Wymuœ wszystkie oczekiwane dostawy od Bobby Ray's.",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: When ON, will report the distance each bullet deviates from the\ncenter of the target, taking all NCTH factors into account.",
	L"Kliknij by naprawiæ b³êdy w ustawieniach gry.",							// failsafe show/hide option to reset all options
	L"Kliknij by naprawiæ b³êdy w ustawieniach gry.",							// a do once and reset self option (button like effect)
	L"Udostêpnia tryb debugowania w edytorze map oraz wersji koñcowej.",				// allow debugging in release or mapeditor
	L"Prze³¹cz na tryb wyœwietlania/ukrycia opcji renderowania debugowego.",						// an example option that will show/hide other options
	L"Wyœwietl wymiary wokó³ kursora myszy.",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};

STR16	gzGIOScreenText[] =
{
	L"POCZ¥TKOWE USTAWIENIA GRY",
	L"Styl gry",
	L"Realistyczny",
	L"S-F",
	L"Platynowy", //Placeholder English
	L"Opcje broni",
	L"Mnóstwo",
	L"Standardowe",
	L"Stopieñ trudnoœci",
	L"Nowicjusz",
	L"Doœwiadczony",
	L"Ekspert",
	L"SZALONY",
	L"Start",		// TODO.Translate
	L"Anuluj",
	L"Zapis gry",
	L"Zawsze",
	L"W czasie pokoju",
	L"Nie dzia³a w wersji demo",
	L"Wyposa¿enie Bobby Ray's",
	L"Normalne",
	L"Œwietne",
	L"Wyœmienite",
	L"Niewiarygodne",
	L"Ekwipunek / Dodatki",
	L"Nie u¿ywany",
	L"Nie u¿ywany",
	L"Wczytaj grê MP",
	L"POCZ¥TKOWE USTAWIENIA GRY (Only the server settings take effect)",
	// Added by SANDRO
	L"Cechy umiejêtnoœci",
	L"Stare",
	L"Nowe",
	L"Maks. liczba IMP",
	L"Jeden",
	L"Dwa",
	L"Trzy",
	L"Cztery",
	L"Piêæ",
	L"Szeœæ",
	L"Wrogowie upuszczaj¹ ca³oœæ ekwipunku",
	L"Wy³¹czone",
	L"W³¹czone",
	L"Liczba terrorystów",
	L"Losowe",
	L"Wszystkie",
	L"Tajne sk³ady proni",
	L"Losowe",
	L"Wszystkie",
	L"Szybkoœæ zwiêkszania dostêpnoœci przedmiotów",
	L"Bardzo wolna",
	L"Wolna",
	L"Normala",
	L"Szybka",
	L"Bardzo szybkia",

	L"Stare / Stare",
	L"Nowe / Stare",
	L"Nowe / Nowe",
};

STR16	gzMPJScreenText[] =
{
	L"MULTIPLAYER",
	L"Do³¹cz",
	L"Uruchom serwer",
	L"Anuluj",
	L"Odœwie¿",
	L"Nazwa gracza",
	L"IP Serwera",
	L"Port",
	L"Nazwa serwera",
	L"# Plrs",			// ?? if plrs=players then "# graczy"
	L"Wersja",
	L"Typ rozgrywki",
	L"Ping",
	L"Musisz podaæ nazwê gracza",
	L"Musisz podaæ odpowiedni numer IP serwera. (np. 84.114.195.239).",
	L"Musisz podaæ odpowiedni port serwera pomiêdzy 1 i 65535.",
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
	L"Pary¿",				//Paris, France
	L"Trypolis",			//Tripoli, Libya (eastern Mediterranean)
	L"Tokio",				//Tokyo, Japan
	L"Vancouver",		//Vancouver, British Columbia, Canada (west coast near US border)
};

STR16 pSkillAtZeroWarning[] =
{ //This string is used in the IMP character generation.  It is possible to select 0 ability
	//in a skill meaning you can't use it.  This text is confirmation to the player.
	L"Na pewno? Wartoœæ zero oznacza brak jakichkolwiek umiejêtnoœci w tej dziedzinie.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( Maks. 8 znaków )",
};

STR16 pIMPFinishButtonText[ 1 ]=
{
	L"Analizujê",
};

STR16 pIMPFinishStrings[ ]=
{
	L"Dziêkujemy, %s", //%s is the name of the merc
};

// the strings for imp voices screen
STR16 pIMPVoicesStrings[] =
{
	L"G³os", 
};

STR16 pDepartedMercPortraitStrings[ ]=
{
	L"Œmieræ w akcji",
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
	L"Wznów grê (|P|a|u|s|e)",
	L"Wstrzymaj grê (|P|a|u|s|e)",
};


STR16 pMessageStrings[] =
{
	L"Zakoñczyæ grê?",
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
	L"Dzieñ",
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
	L"Strona g³ówna",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"N/D",					//Lowercase acronym for not applicable.
	L"Tymczasem",		//Meanwhile
	L"%s przyby³(a) do sektora %s%s", //30	Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Wersja",
	L"Wolna pozycja na szybki zapis",
	L"Ta pozycja zarezerwowana jest na szybkie zapisy wykonywane podczas gry kombinacj¹ klawiszy ALT+S.",
	L"Otwarte",
	L"Zamkniête",
	L"Brak miejsca na dysku twardym.  Na dysku wolne jest %s MB, a wymagane jest przynajmniej %s MB.",
	L"Najêto - %s z A.I.M.", 
	L"%s z³apa³(a) %s",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s zaaplikowa³(a) sobie lekarstwo", //'Merc name' has taken the drug
	L"%s nie posiada wiedzy medycznej",//40	'Merc name' has no medical skill.

	//CDRom errors (such as ejecting CD while attempting to read the CD)
	L"Integralnoœæ gry zosta³a nara¿ona na szwank.",
	L"B£¥D: Wyjêto p³ytê CD",

	//When firing heavier weapons in close quarters, you may not have enough room to do so.
	L"Nie ma miejsca, ¿eby st¹d oddaæ strza³.",
	
	//Can't change stance due to objects in the way...
	L"Nie mo¿na zmieniæ pozycji w tej chwili.",

	//Simple text indications that appear in the game, when the merc can do one of these things.
	L"Upuœæ",
	L"Rzuæ",
	L"Podaj",

	L"%s przekazano do - %s.", //"Item" passed to "merc".  Please try to keep the item %s before the merc %s, otherwise,
											 //must notify SirTech.
	L"Brak wolnego miejsca, by przekazaæ %s do - %s.", //pass "item" to "merc".  Same instructions as above.

	//A list of attachments appear after the items.  Ex:  Kevlar vest ( Ceramic Plate 'Attached )'
	L" do³¹czono]",			// 50

	//Cheat modes
	L"Pierwszy poziom lamerskich zagrywek osi¹gniêty",
	L"Drugi poziom lamerskich zagrywek osi¹gniêty",

	//Toggling various stealth modes
	L"Oddzia³ ma w³¹czony tryb skradania siê.",
	L"Oddzia³ ma wy³¹czony tryb skradania siê.",
	L"%s ma w³¹czony tryb skradania siê.",
	L"%s ma wy³¹czony tryb skradania siê.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Dodatkowe siatki w³¹czone.",
	L"Dodatkowe siatki wy³¹czone.",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Nie mo¿na wyjœæ do góry z tego poziomu...",
	L"Nie ma ju¿ ni¿szych poziomów...",		// 60
	L"Wejœcie na %d poziom pod ziemi¹...",
	L"Wyjœcie z podziemii...",

	L"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Automatyczne centrowanie ekranu wy³¹czone.",
	L"Automatyczne centrowanie ekranu w³¹czone.",
	L"Kursor 3D wy³¹czony.",
	L"Kursor 3D w³¹czony.",
	L"Oddzia³ %d aktywny.",
	L"%s - Nie staæ ciê by wyp³aciæ jej/jemu dzienn¹ pensjê w wysokoœci %s.",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Pomiñ",				// 70
	L"%s nie mo¿e odejœæ sam(a).",
	L"Utworzono zapis gry o nazwie SaveGame99.sav. W razie potrzeby zmieñ jego nazwê na SaveGame01..10. Wtedy bêdzie mo¿na go odczytaæ ze standardowego okna odczytu gry.",
	L"%s wypi³(a) trochê - %s",
	L"Przesy³ka dotar³a do Drassen.",
 	L"%s przybêdzie do wyznaczonego punktu zrzutu (sektor %s) w dniu %d, oko³o godziny %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival	
	L"Lista historii zaktualizowana.",
	L"Seria z granatnika u¿ywa kursora celowania (dostêpny ogieñ rozproszony)",		//L"Grenade Bursts use Targeting Cursor (Spread fire enabled)", BY£O -->>L"Grenade Bursts - Using Targeting Cursor (Spread fire enabled)",
	L"Seria z granatnika u¿ywa kursora trajektorii (dostêpny ogieñ rozproszony)",		//L"Grenade Bursts use Trajectory Cursor (Spread fire disabled)", BY£O -->L"Grenade Bursts - Using Trajectory Cursor (Spread fire disabled)",
	L"Enabled Soldier Tooltips", // Changed from Drop All On - SANDRO
	L"Disabled Soldier Tooltips", // Changed from Drop All Off - SANDRO
	L"Granatniki strzelaj¹ pod standardowymi k¹tami",					//L"Grenade Launchers fire at standard angles", BY£o->>L"Grenade Launchers - Fire at standard angles",
	L"Granatniki strzelaj¹ pod wysokimi k¹tami",						//L"Grenade Launchers fire at higher angles", BY£o-->>L"Grenade Launchers - Fire at high angles",
	// forced turn mode strings
    L"Forced Turn Mode",
	L"Normal turn mode",
	L"Exit combat mode",
	L"Forced Turn Mode Active, Entering Combat",
#ifdef JA2BETAVERSION
	L"Automatyczny zapis zosta³ pomyœlnie wykonany.",
#endif
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",

	L"Nie mo¿esz u¿ywaæ nowego trybu wyposa¿enia i nowego systemu dodatków jednoczeœnie.",
};


CHAR16 ItemPickupHelpPopup[][40] =
{
	L"OK",
	L"W górê",
	L"Wybierz wszystko",
	L"W dó³",
	L"Anuluj",
};

STR16 pDoctorWarningString[] =
{
	L"%s jest za daleko, aby poddaæ siê leczeniu.",
	L"Lekarze nie mogli opatrzyæ wszystkich rannych.",
};

STR16 pMilitiaButtonsHelpText[] =
{
	L"Podnieœ(Prawy klawisz myszy)/upuœæ(Lewy klawisz myszy) Zielonych ¿o³nierzy", // button help text informing player they can pick up or drop militia with this button
	L"Podnieœ(Prawy klawisz myszy)/upuœæ(Lewy klawisz myszy) Doœwiadczonych ¿o³nierzy",
	L"Podnieœ(Prawy klawisz myszy)/upuœæ(Lewy klawisz myszy) Weteranów",
	L"Umieszcza jednakow¹ iloœæ ¿o³nierzy samoobrony w ka¿dym sektorze.",
};

STR16 pMapScreenJustStartedHelpText[] =
{
	L"Zajrzyj do A.I.M. i zatrudnij kilku najemników (*Wskazówka* musisz otworzyæ laptopa)", // to inform the player to hired some mercs to get things going
	L"Jeœli chcesz ju¿ udaæ siê do Arulco, kliknij przycisk kompresji czasu, w prawym dolnym rogu ekranu.", // to inform the player to hit time compression to get the game underway
};

STR16 pAntiHackerString[] = 
{
	L"B³¹d. Brakuje pliku, lub jest on uszkodzony. Gra zostanie przerwana.",
};


STR16 gzLaptopHelpText[] =
{
	//Buttons:
	L"Przegl¹danie poczty",
	L"Przegl¹danie stron internetowych",
	L"Przegl¹danie plików i za³¹czników pocztowych",
	L"Rejestr zdarzeñ",
	L"Informacje o cz³onkach oddzia³u",
	L"Finanse i rejestr transakcji",
	L"Koniec pracy z laptopem",

	//Bottom task bar icons (if they exist):
	L"Masz now¹ pocztê",
	L"Masz nowe pliki",

	//Bookmarks:
	L"Miêdzynarodowe Stowarzyszenie Najemników",
	L"Bobby Ray's - Internetowy sklep z broni¹",
	L"Instytut Badañ Najemników",
	L"Bardziej Ekonomiczne Centrum Rekrutacyjne",
	L"McGillicutty's - Zak³ad pogrzebowy",
	L"United Floral Service",
	L"Brokerzy ubezpieczeniowi",
};


STR16	gzHelpScreenText[] =
{
	L"Zamknij okno pomocy",	
};

STR16 gzNonPersistantPBIText[] =
{
	L"Trwa walka. Najemników mo¿na wycofaæ tylko na ekranie taktycznym.",
	L"W|ejdŸ do sektora, aby kontynuowaæ walkê.",
	L"|Automatycznie rozstrzyga walkê.",
	L"Nie mo¿na automatycznie rozstrzygn¹æ walki, gdy atakujesz.",
	L"Nie mo¿na automatycznie rozstrzygn¹æ walki, gdy wpadasz w pu³apkê.",
	L"Nie mo¿na automatycznie rozstrzygn¹æ walki, gdy walczysz ze stworzeniami w kopalni.",
	L"Nie mo¿na automatycznie rozstrzygn¹æ walki, gdy w sektorze s¹ wrodzy cywile.",
	L"Nie mo¿na automatycznie rozstrzygn¹æ walki, gdy w sektorze s¹ dzikie koty.",
	L"TRWA WALKA",
	L"W tym momencie nie mo¿esz siê wycofaæ.",
};

STR16 gzMiscString[] =
{
	L"¯o³nierze samoobrony kontynuuj¹ walkê bez pomocy twoich najemników...",
	L"W tym momencie tankowanie nie jest konieczne.",
	L"W baku jest %d%% paliwa.",
	L"¯o³nierze Deidranny przejêli ca³kowit¹ kontrolê nad - %s.",
	L"Nie masz ju¿ gdzie zatankowaæ.",
};

STR16	gzIntroScreen[] = 
{
	L"Nie odnaleziono filmu wprowadzaj¹cego",
};

// These strings are combined with a merc name, a volume string (from pNoiseVolStr),
// and a direction (either "above", "below", or a string from pDirectionStr) to 
// report a noise.
// e.g. "Sidney hears a loud sound of MOVEMENT coming from the SOUTH."
STR16 pNewNoiseStr[] =
{
	L"%s s³yszy %s DWIÊK dochodz¹cy z %s.",
	L"%s s³yszy %s ODG£OS RUCHU dochodz¹cy z %s.",
	L"%s s³yszy %s ODG£OS SKRZYPNIÊCIA dochodz¹cy z %s.",
	L"%s s³yszy %s PLUSK dochodz¹cy z %s.",
	L"%s s³yszy %s ODG£OS UDERZENIA dochodz¹cy z %s.",
	L"%s s³yszy %s WYBUCH dochodz¹cy z %s.",
	L"%s s³yszy %s KRZYK dochodz¹cy z %s.",
	L"%s s³yszy %s ODG£OS UDERZENIA dochodz¹cy z %s.",
	L"%s s³yszy %s ODG£OS UDERZENIA dochodz¹cy z %s.",
	L"%s s³yszy %s £OMOT dochodz¹cy z %s.",
	L"%s s³yszy %s TRZASK dochodz¹cy z %s.",
};

STR16 wMapScreenSortButtonHelpText[] =
{
	L"Sortuj wed³ug kolumny Imiê (|F|1)",
	L"Sortuj wed³ug kolumny Przydzia³ (|F|2)",
	L"Sortuj wed³ug kolumny Sen (|F|3)",
	L"Sortuj wed³ug kolumny Lokalizacja (|F|4)",
	L"Sortuj wed³ug kolumny Cel podró¿y (|F|5)",
	L"Sortuj wed³ug kolumny Wyjazd (|F|6)",
};



STR16		BrokenLinkText[] = 
{
	L"B³¹d 404",
	L"Nie odnaleziono strony.",
};


STR16 gzBobbyRShipmentText[] = 
{
	L"Ostatnie dostawy",
	L"Zamówienie nr ",
	L"Iloœæ przedmiotów",
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
	L"(wci¹¿ uczy siê interpunkcji)",					// Shaun Lyng
	L"(\"Skoñczone, tylko to posk³adam\")",	//Kris \"The Cow Rape Man\" Marnes
	L"(robiê siê na to za stary)",				// Ian Currie
	L"(i pracuje nad Wizardry 8)",						// Linda Currie
	L"(zmuszony pod broni¹ do koñcowych testów jakoœci produktu)",			// Eric \"WTF\" Cheng
	L"(Opuœci³ nas dla Stowarzyszenia na Rzecz Rozs¹dnych Wynagrodzeñ. Ciekawe czemu... )",	// Lynn Holowka
	L"",																			// Norman \"NRG\" Olsen
	L"",																			// George Brooks
	L"(mi³oœnik zespo³u Dead Head i jazzu)",						// Andrew Stacey
	L"(tak naprawdê na imiê ma Robert)",							// Scot Loving
	L"(jedyna odpowiedzialna osoba)",					// Andrew \"Big Cheese Doddle\" Emmons
	L"(teraz mo¿e wróciæ do motocrossu)",	// Dave French
	L"(ukradziony z projektu Wizardry 8)",							// Alex Meduna
	L"(zrobi³ przedmioty i ekrany wczytywania!!)",	// Joey \"Joeker\" Whelan",
};

STR16 sRepairsDoneString[] =
{
	L"%s skoñczy³(a) naprawiaæ w³asne wyposa¿enie",
	L"%s skoñczy³(a) naprawiaæ broñ i ochraniacze wszystkich cz³onków oddzia³u",
	L"%s skoñczy³(a) naprawiaæ wyposa¿enie wszystkich cz³onków oddzia³u",
	L"%s skoñczy³(a) naprawiaæ ciê¿kie wyposa¿enie wszystkich cz³onków oddzia³u",
	L"%s skoñczy³(a) naprawiaæ œrednie wyposa¿enie wszystkich cz³onków oddzia³u",
	L"%s skoñczy³(a) naprawiaæ lekkie wyposa¿enie wszystkich cz³onków oddzia³u",
	L"%s skoñczy³(a) naprawiaæ strój LBE wszystkich cz³onków oddzia³u",
};

STR16 zGioDifConfirmText[]=
{
	L"Wybrano opcjê NOWICJUSZ. Opcja ta jest przeznaczona dla niedoœwiadczonych graczy, lub dla tych, którzy nie maj¹ ochoty na d³ugie i ciê¿kie walki. Pamiêtaj, ¿e opcja ta ma wp³yw na przebieg ca³ej gry. Czy na pewno chcesz graæ w trybie Nowicjusz?",
	L"Wybrano opcjê DOŒWIADCZONY. Opcja ta jest przenaczona dla graczy posiadaj¹cych ju¿ pewne doœwiadczenie w grach tego typu. Pamiêtaj, ¿e opcja ta ma wp³yw na przebieg ca³ej gry. Czy na pewno chcesz graæ w trybie Doœwiadczony?",
	L"Wybrano opcjê EKSPERT. Jakby co, to ostrzegaliœmy ciê. Nie obwiniaj nas, jeœli wrócisz w plastikowym worku. Pamiêtaj, ¿e opcja ta ma wp³yw na przebieg ca³ej gry. Czy na pewno chcesz graæ w trybie Ekspert?",
	L"Wybrano opcjê SZALONY. OSTRZE¯ENIE: Nie obwiniaj nas, jeœli wrócisz w malutkich kawa³eczkach... Deidranna NAPRAWDÊ skopie ci ty³ek. Mocno. Pamiêtaj, ¿e opcja ta ma wp³yw na przebieg ca³ej gry. Czy na pewno chcesz graæ w trybie SZALONY?",
};

STR16 gzLateLocalizedString[] =
{
	L"%S - nie odnaleziono pliku...",

	//1-5
	L"Robot nie mo¿e opuœciæ sektora bez operatora.",

	//This message comes up if you have pending bombs waiting to explode in tactical.
	L"Nie mo¿na teraz kompresowaæ czasu.  Poczekaj na fajerwerki!",  

	//'Name' refuses to move.
	L"%s nie chce siê przesun¹æ.",

	//%s a merc name
	L"%s ma zbyt ma³o energii, aby zmieniæ pozycjê.",

	//A message that pops up when a vehicle runs out of gas.
	L"%s nie ma paliwa i stoi w sektorze %c%d.",

	//6-10

	// the following two strings are combined with the pNewNoise[] strings above to report noises
	// heard above or below the merc
	L"GÓRY",
	L"DO£U",

	//The following strings are used in autoresolve for autobandaging related feedback.
	L"¯aden z twoich najemników nie posiada wiedzy medycznej.",
	L"Brak œrodków medycznych, aby za³o¿yæ rannym opatrunki.",
	L"Zabrak³o œrodków medycznych, aby za³o¿yæ wszystkim rannym opatrunki.",
	L"¯aden z twoich najemników nie potrzebuje pomocy medycznej.",
	L"Automatyczne zak³adanie opatrunków rannym najemnikom.",
	L"Wszystkim twoim najemnikom za³o¿ono opatrunki.",

	//14
	L"Arulco",

  L"(dach)",

	L"Zdrowie: %d/%d",

	//In autoresolve if there were 5 mercs fighting 8 enemies the text would be "5 vs. 8"
	//"vs." is the abbreviation of versus.
	L"%d vs. %d",
	
	L"%s - brak wolnych miejsc!",  //(ex "The ice cream truck is full")

  L"%s nie potrzebuje pierwszej pomocy lecz opieki lekarza lub d³u¿szego odpoczynku.",

	//20
	//Happens when you get shot in the legs, and you fall down.
	L"%s dosta³(a) w nogi i upad³(a)!",
	//Name can't speak right now.
	L"%s nie mo¿e teraz mówiæ.",

	//22-24 plural versions 
	L"%d zielonych ¿o³nierzy samoobrony awansowa³o na weteranów.",
	L"%d zielonych ¿o³nierzy samoobrony awansowa³o na regularnych ¿o³nierzy.",
	L"%d regularnych ¿o³nierzy samoobrony awansowa³o na weteranów.",

	//25
	L"Prze³¹cznik",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s dostaje œwira!",

	//27-28
	//Messages why a player can't time compress.
	L"Niebezpiecznie jest kompresowaæ teraz czas, gdy¿ masz najemników w sektorze %s.",
	L"Niebezpiecznie jest kompresowaæ teraz czas, gdy¿ masz najemników w kopalni zaatakowanej przez robale.",

	//29-31 singular versions 
	L"1 zielony ¿o³nierz samoobrony awansowa³ na weterana.",
	L"1 zielony ¿o³nierz samoobrony awansowa³ na regularnego ¿o³nierza.",
	L"1 regularny ¿o³nierz samoobrony awansowa³ na weterana.",

	//32-34
	L"%s nic nie mówi.",
	L"Wyjœæ na powierzchniê?",
	L"(Oddzia³ %d)",

	//35
	//Ex: "Red has repaired Scope's MP5K".  Careful to maintain the proper order (Red before Scope, Scope before MP5K)
	L"%s naprawi³(a) najemnikowi - %s, jego/jej - %s",

	//36
	L"DZIKI KOT",

	//37-38 "Name trips and falls"
	L"%s potyka siê i upada",
	L"Nie mo¿na st¹d podnieœæ tego przedmiotu.",

	//39
	L"¯aden z twoich najemników nie jest w stanie walczyæ.  ¯o³nierze samoobrony sami bêd¹ walczyæ z robalami.",

	//40-43
	//%s is the name of merc.
	L"%s nie ma œrodków medycznych!",
	L"%s nie posiada odpowiedniej wiedzy, aby kogokolwiek wyleczyæ!",
	L"%s nie ma narzêdzi!",
	L"%s nie posiada odpowiedniej wiedzy, aby cokolwiek naprawiæ!",

	//44-45
	L"Czas naprawy",
	L"%s nie widzi tej osoby.",

	//46-48
	L"%s - przed³u¿ka lufy jego/jej broni odpada!",
	L"W tym sektorze mo¿e byæ maks. %d instruktorów szkol¹cych samoobronê.",
  	L"Na pewno?",

	//49-50
	L"Kompresja czasu",
	L"Pojazd ma pe³ny zbiornik paliwa.",

	//51-52 Fast help text in mapscreen.
	L"Kontynuuj kompresjê czasu (|S|p|a|c|j|a)",
	L"Zatrzymaj kompresjê czasu (|E|s|c)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s odblokowa³(a) - %s",
	L"%s odblokowa³(a) najemnikowi - %s, jego/jej - %s",

	//55 
	L"Nie mo¿na kompresowaæ czasu, gdy otwarty jest inwentarz sektora.",

	L"Nie odnaleziono p³yty nr 2 Jagged Alliance 2.",

	L"Przedmioty zosta³y pomyœlnie po³¹czone.",
	
	//58
	//Displayed with the version information when cheats are enabled.
	L"Bie¿¹cy/Maks. postêp: %d%%/%d%%",

	L"Eskortowaæ Johna i Mary?",
	
	// 60
	L"Prze³¹cznik aktywowany.",

	L"%s: dodatki do pancerza zosta³y zniszczone!",
	L"%s wystrzeli³(a) %d pocisk(ów) wiêcej ni¿ to by³o zamierzone!",
	L"%s wystrzeli³(a) 1 pocisk(ów) wiêcej ni¿ to by³o zamierzone!",
};

STR16 gzCWStrings[] = 
{
	L"Wezwaæ posi³ki do %s z przyleg³ych sektorów?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Miejsce: %d\n",
	L"%s|Jasnoœæ: %d / %d\n",
	L"%s|Odleg³oœæ do |Celu: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Rozkazy: %d\n",
	L"%s|Postawa: %d\n",
	L"%s|Aktualne |P|A: %d\n",
	L"%s|Aktualne |Zdrowie: %d\n",
	// Full info
	L"%s|He³m: %s\n",
	L"%s|Kamizelka: %s\n",
	L"%s|Getry ochronne: %s\n",
	// Limited, Basic
	L"|Pancerz: ",
	L"he³m ",
	L"kamizelka ",
	L"getry ochronne",
	L"u¿ywane",
	L"brak pancerza",
	L"%s|N|V|G: %s\n",
	L"brak NVG",
	L"%s|Maska |Gazowa: %s\n",
	L"brak maski gazowej",
	L"%s|Pozycja |G³owy |1: %s\n",
	L"%s|Pozycja |G³ów |2: %s\n",
	L"\n(w plecaku) ",
	L"%s|Broñ: %s ",
	L"brak broni",
	L"rewolwer",
	L"SMG",
	L"karabin",
	L"MG",
	L"strzelba",
	L"nó¿",
	L"broñ Ciê¿ka",
	L"brak he³mu",
	L"brak kamizelki",
	L"brak getrów ochronnych",
	L"|Pancerz: %s\n", 
	 // Added - SANDRO
	L"%s|Umiejêtnoœæ 1: %s\n",
	L"%s|Umiejêtnoœæ 2: %s\n", 
	L"%s|Umiejêtnoœæ 3: %s\n",
};

STR16 New113Message[] =
{
	L"Nadesz³a burza.",
	L"Burza skoñczy³a siê.",
	L"Rozpada³ siê deszcz.",
	L"Deszcz przesta³ padaæ.",
	L"Uwa¿aj na snajperów...",
	L"Ogieñ d³awi¹cy!",	
	L"BRST",
	L"AUTO",
	L"GL",
	L"GL BRST",
	L"GL AUTO",
	L"Snajper!",
	L"Nie mo¿na podzieliæ pieniêdzy z powodu przedmiotu na kursorze.",
	L"Przybycie nowych rekrutów zosta³o przekierowane do sektora %s , z uwagi na to, ¿e poprzedni punkt zrzutu w sektorze %s jest zajêty przez wroga.",
	L"Przedmiot usuniêty",
	L"Usuniêto wszystkie przedmioty tego typu",
	L"Przedmiot sprzedany",
	L"Wszystkie przedmioty tego typu sprzedane",
	L"SprawdŸ swoje gogle",
	// Real Time Mode messages
	L"W trakcie walki",
	L"Brak wrogów w zasiêgu wzroku",
	L"Wy³¹czone skradanie w czasie rzeczywistym",
	L"W³¹czone skradanie w czasie rzeczywistym",
	//L"Spotkano wroga! (Ctrl + x by uruchomiæ tryb turowy)",
	L"Spotkano wroga!", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s dokona³(a) udanej kradzie¿y!",
	L"%s nie posiada wystarczaj¹cej liczby PA by ukraœæ wszystkie zaznaczone przedmioty.",
	L"Chcesz operowaæ %s zanim u¿yjesz banda¿y? (mo¿esz przywróciæ oko³o %i punktów zdrowia.)",
	L"Chcesz operowaæ %s zanim u¿yjesz banda¿y? (mo¿esz przywróciæ oko³o %i punktów zdrowia.)",
	L"Czy chcesz najpierw operowaæ przed u¿yciem banda¿y? (%i pacjent(ów))",
	L"Czy chcesz najpierw operowaæ pacjenta?",
	L"Czy podczas udzielania pierwszej pomocy wykonywaæ niezbêdne operacje?",
	L"operacja na %s zakoñczona.",
	L"%s otrzymuje trafienie w korpus i traci punkt maksymalnego zdrowia!",
	L"%s otrzymuje trafienie w korpus i traci %d punktów maksymalnego zdrowia!",
	L"%s odzyska³(a) utracony punkt %s",
	L"%s odzyska³(a) %d utraconych punktów %s",
	L"Twoja umiejêtnoœæ zwiadowcy uchroni³a ciê przed zasadzk¹!",
	L"Twoja umiejêtnoœæ zwiadowcy pozwoli³a ci omin¹æ stado krwawych kotów!",
	L"%s zosta³ trafiony w pachwinê i pada na ziemiê!",
	//////////////////////////////////////////////////////////////////////////////
	L"Warning: enemy corpse found!!!",
	L"%s [%d rnds]\n%s %1.1f %s",

};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s kuli siê ze strachu!",
	L"%s zosta³ przyparty do muru!",
	L"%s odda³(a) wiêcej strza³ów ni¿ zamierza³(a)!",
	L"Nie mo¿esz szkoliæ samoobrony w tym sektorze.",
	L"Samoobrona znajduje %s.",
	L"Nie mo¿na szkoliæ samoobrony, gdy wróg jest w sektorze!", 
	// 6 - 10
	L"%s Brak odpowiednich umiejêtnoœci dowodzenia do szkolenia samoobrony.",
	L"W tym sektorze jest dozwolonych nie wiêcej ni¿ %d instruktorów samoobrony.",
	L"W %s oraz okolicy nie ma miejsca dla nowych oddzia³ów samoobrony!",
	L"Musisz mieæ conajmniej %d jednostek samoobrony w ka¿dym z %s wyzwolonych sektorów zanim bêdziesz móg³ szkoliæ tu nowe oddzia³y.",
	L"Nie mo¿esz obsadziæ fabryki dopóki w okolicy s¹ wrogie jednostki!",  // not sure
	// 11 - 15
	L"%s zbyt niska inteligencja do obsadzenia tej fabryki.",
	L"%s posiada ju¿ maksymaln¹ liczbê personelu.",
	L"Obsadzenie tej fabryki bêdzie kosztowaæ $%d za godzinê. Chcesz kontynuowaæ?",
	L"Nie posiadasz funduszy potrzebnych na op³acenie wszystkich fabryk. Zap³acono $%d, ale wci¹¿ pozosta³o $%d do zap³aty. Tubylcy s¹ zaniepokojeni.",
	L"Nie posiadasz funduszy potrzebnych na op³acenie wszystkich fabryk. Pozosta³o $%d do zap³aty. Tubylcy s¹ zaniepokojeni.",
	// 16 - 20
	L"Zalegasz $%d dla Fabryki i nie posiadasz funduszy umo¿liwiaj¹cych sp³atê d³ugu!",
	L"Zalegasz $%d dla Fabryki!. Nie mo¿esz przydzieliæ tego najemnika do fabryki do momentu sp³aty d³ugu.",
	L"Zalegasz $%d dla Fabryki!. Czy chcesz sp³aciæ ten d³ug?",
	L"Niedostêpny w tym sektorze",
	L"Dzienne wydatki",
	// 21 - 25
	L"Brak funduszy do op³acenia wykazanych jednostek samoobrony! %d jednostek opuszcza oddzia³y i wraca do domu.",
};

// WANNE: This are the email texts, when one of the 4 new 1.13 MERC mercs have levelled up, that Speck sends
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16	New113MERCMercMailTexts[] =
{
	// Gaston: Text from Line 39 in Email.edt
	L"Niniejszym informujê, i¿ w zwi¹zku z dotychczasowymi osi¹gniêciami Gastona, op³ata za jego us³ugi zosta³a podniesiona. Osobiœcie, nie jestem tymfaktem zaskoczony.  ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Informujemy, i¿ od chwili obecnej cena za us³ugi œwiadczone przez pana Stoggiego wzros³a w zwi¹zku ze wzrostem jego umiejêtnoœci. ± ± Speck T. Kline ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Informujemy, i¿ nabyte przez Texa doœwiadczenie upowa¿nia go do wy¿szego wynagrodzenia, z tego wzglêdu jego wynagrodzenie zosta³o zwiêkszone w celu lepszego odzwierciedlenia jego wartoœci. ± ± Speck T. Kline ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Proszê o zwrócenie uwagi, i¿ w zwi¹zku ze wzrostem jakoœci us³ug œwiadczonych przez pana Biggens`a jego pensja tak¿e uleg³a podwy¿szeniu. ± ± Speck T. Kline ± ",
};

// TODO.Translate
// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"PD z Serwera AIM: Wiadomoœæ od - Victor Kolesnikov",
	L"Hej, tu Monk. Dosta³em Twoj¹ wiadomoœæ. Ju¿ jestem z powrotem  - jeœli chcesz siê skontaktowaæ. ± ± Zadzwoñ. ±",

	// Brain
	L"PD z Serwera AIM: Wiadomoœæ od - Janno Allik",
	L"Jestem ju¿ gotów do przyjêcia nowego zadania. Wszystko musi mieæ swoj¹ porê. ± ± Janno Allik ±",

	// Scream
	L"PD z Serwera AIM: Wiadomoœæ od - Lennart Vilde",
	L"Lennart Vilde jest obecnie dostêpny! ±",

	// Henning
	L"PD z Serwera AIM: Wiadomoœæ od - Henning von Branitz",
	L"Otrzyma³em Twoj¹ wiadomoœæ, dziêkujê. Jeœli chcesz omówiæ mo¿liwoœæ zatrudnienia, skontaktuj siê ze mn¹ za poœrednictwem strony AIM . ± ± Na razie! ± ± Henning von Branitz ±",

	// Luc
	L"PD z Serwera AIM: Wiadomoœæ od - Luc Fabre",
	L"Otrzyma³em wiadomoœæ, merci! Chêtnie rozwa¿ê Twoje propozycje. Wiesz, gdzie mnie znaleŸæ. ± ± Oczekujê odpowiedzi ±",

	// Laura
	L"PD z Serwera AIM: Wiadomoœæ od - Laura Colin",
	L"Pozdrowienia! Dobrze, ¿e zostawi³eœ wiadomoœæ.  Sprawa wygl¹da interesuj¹co. ± ± Zajrzyj jeszcze raz do AIM. Chêtnie poznam wiêcej informacji. ± ± Wyrazy szacunku ± ± Dr Laura Colin ±",

	// Grace
	L"PD z Serwera AIM: Wiadomoœæ od - Graziella Girelli",
	L"Chcia³eœ siê ze mn¹ skontaktowaæ, ale Ci siê nie uda³o.± ± Mia³am spotkanie rodzinne, na pewno to rozumiesz. Teraz mam ju¿ doœæ swojej rodziny i bardzo siê ucieszê, jeœli skontaktujesz siê ze mn¹ ponownie za poœrednictwem witryny AIM . ± ± Ciao! ±",

	// Rudolf
	L"PD z Serwera AIM: Wiadomoœæ od - Rudolf Steiger",
	L"Wiesz, ile telefonów odbieram ka¿dego dnia? Co drugi baran myœli, ¿e mo¿e do mnie wydzwaniaæ.  ± ± W ka¿dym razie ju¿ jestem, jeœli masz dla mnie coœ ciekawego. ±",

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
	L"Snajper: Sokole oczy! Mo¿esz odstrzeliæ skrzyd³a muszce ze stu jardów. ± ",
	// Camouflage
	L"Kamufla¿: Przy tobie nawet krzaki wygl¹daj¹ na sztuczne! ± ",	
	// SANDRO - new strings for new traits added
	// Ranger
	L"Stra¿nik: To ty jesteœ tym z pustyni Teksasu! ± ",	
	// Gunslinger
	L"Rewolwerowiec: Z pistoletem lub dwoma, mo¿esz byæ tak œmiercionoœny jak Billy Kid! ± ",
	// Squadleader
	L"Przywódca: Urodzony dowódca i szef, jesteœ naprawdê grub¹ ryb¹ bez kitu! ± ",
	// Technician
	L"Mechanik: Naprawa sprzêtu, rozbrajanie pu³apek, podk³adanie bomb, to twoja dzia³ka! ± ",
	// Doctor
	L"Chirurg: Mo¿esz przeprowadziæ skomplikowan¹ operacjê przy u¿yciu scyzoryka i gumy do ¿ucia! ± ",
	// Athletics
	L"Atleta: Kondycji móg³by ci pozazdroœciæ niejeden maratoñczyk! ± ",
	// Bodybuilding
	L"Budowa cia³a: Ta ogromna muskularna postaæ, której nie sposób przeoczyæ, to w rzeczy samej ty! ± ",
	// Demolitions
	L"Materia³y wybuchowe: Potrafisz wysadziæ miasto przy u¿yciu standardowego wyposa¿enia kuchni! ± ",
	// Scouting
	L"Zwiadowca: Nic nie umknie twej uwadze! ± ",
};

STR16 NewInvMessage[] = 
{
	L"Nie mo¿esz teraz podnieœæ plecaka.",
	L"Nie ma miejsca, aby po³o¿yæ tutaj plecak",
	L"Nie znaleziono plecaka",
	L"Zamek b³yskawiczny dzia³a tylko podczas walki.",
	L"Nie mo¿esz siê przemieszczaæ, gdy zamek plecaka jest aktywny.",
	L"Na pewno chcesz sprzedaæ wszystkie przedmioty z tego sektora?",
	L"Na pewno chcesz skasowaæ wszystkie przedmioty z tego sektora?",
	L"Nie mo¿na wspinaæ siê z plecakiem",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Inicjacja serwera RakNet...",
	L"Serwer w³., oczekiwanie na po³¹czenie",
	L"Musisz teraz po³¹czyæ swojego klienta z serwerem, wciskaj¹c 2",
	L"Serwer ju¿ dzia³a",
	L"W³. nie powiod³o siê. Przerwanie.",
	// 5
	L"%d/%d klientów gotowych na tryb realtime.",
	L"Serwer roz³¹czony i wy³.",
	L"Serwer nie dzia³a",
	L"£adowanie klientów, czekaj.",
	L"Nie mo¿na zmieniaæ miejsc zrzutu po starcie serwera.",
	// 10
	L"Wys³ano '%S' pliku - 100/100",
	L"Zakoñczono wysy³anie plików do '%S'.",
	L"Rozpoczêto wysy³anie plików do '%S'.",
};

STR16 MPClientMessage[] =
{
	// 0
	L"Inicjacja klienta RakNet…",
	L"£¹czenie z IP: %S ...",
	L"Otrzymano ustawienia:",
	L"Jesteœ ju¿ po³¹czony.",
	L"Jesteœ ju¿ w trakcie ³¹czenia",
	// 5
	L"Klient #%d - '%S' wynaj¹³ '%s'.",
	L"Klient #%d - '%S' has hired another merc.",
	L"Gotowy! Wszystkich gotowych - %d/%d.",
	L"Nie jesteœ ju¿ gotowy. Gotowych - %d/%d.",
	L"Pocz¹tek bitwy...",
	// 10
	L"Klient #%d - '%S' jest gotów. Gotowych - %d/%d.",
	L"Klient #%d - '%S' nie jest ju¿ gotowy. Gotowych - %d/%d",
	L"Jesteœ gotów. Czekanie na pozosta³ych… Naciœnij OK., jeœli ju¿ nie jesteœ gotów.",
	L"Zaczynajmy ju¿!",
	L"Klient A musi dzia³aæ, by zacz¹æ grê.",
	// 15
	L"Nie mo¿na zacz¹æ. Brak najemników.",
	L"Czekaj na zgodê serwera, by odblokowaæ laptopa…",
	L"Przerwano",
	L"Koniec przerwania",
	L"Po³o¿enie siatki myszy:",
	// 20
	L"X: %d, Y: %d",
	L"Numer siatki %d",
	L"W³aœciwoœci serwera",
	L"Ustaw rêcznie stopieñ nadrzêdnoœci serwera: ‘1’ – w³.laptop/rekrut.; ‘2’- w³./³aduj poziom; ‘3’ – odblok. UI; ‘4’ – koñczy rozmieszczanie",	
	L"Sektor=%s, MaxKlientów=%d, Max Najem=%d, Tryb_Gry=%d, TenSamNaj=%d, Mno¿nik obra¿.=%f, TuryCzas=%d, Sek/ruch=%d, Dis BobbyRay=%d, Wy³ Aim/Merc Ekwip=%d, Wy³ morale=%d, Test=%d",
	// 25
	L"",
	L"Nowe po³¹czenie Client #%d - '%S'.",
	L"Dru¿yna: %d.",//not used any more
	L"'%s' (klient %d - '%S') zabity przez '%s' (client %d - '%S')",
	L"Wyrzucono #%d - '%S'",
	// 30
	L"Zacz¹æ turê dla klientów. #1: <Anulować>, #2: %S, #3: %S, #4: %S",
	L"Pocz¹tek tury dla #%d",
	L"¯¹danie trybu realtime…",
	L"Zmieniono w tryb realtime.",
	L"B³¹d. Coœ posz³o nie tak przy prze³¹czaniu.",
	// 35
	L"Odblokowaæ laptopy? (Czy gracze s¹ ju¿ pod³¹czeni?)",
	L"Serwer odblokowa³ laptopa. Zaczynaj rekrutowaæ!",
	L"Przerywaj¹cy",
	L"Nie mo¿esz zmieniaæ strefy zrzutu, jeœli nie  jesteœ serwerem gry.",
	L"Odrzuci³eœ ofertê poddania siê, gdy¿ grasz w trybie Multiplayer.",
	// 40
	L"Wszyscy twoi ludzie s¹ martwi!",
	L"Tryb obserwatora w³..",
	L"Zosta³eœ pokonany!",
	L"Wspinanie wy³¹czone w MP",
	L"Wynajêto '%s'",
	// 45
	L"Nie mo¿esz zmieniæ mapy w trakcie dokonywania zakupu",
	L"Mapa zmieniona na '%s'",
	L"Klient '%s' roz³¹czony, usuwanie z gry",
	L"Zosta³eœ roz³¹czony, powrót do g³ównego menu",
	L"Próba po³¹czenia zakoñczona niepowodzeniem, kolejna poróba za 5 sekund, %i prób pozosta³o...",
	//50
	L"Próba po³¹czenia zakoñczona niepowodzeniem, rezygnacja z kolejnych prób...",
	L"Nie mo¿esz rozpocz¹æ gry dopóki nie s¹ pod³¹czeni inni gracze",
	L"%s : %s",
	L"Wyœlij do wszystkich",
	L"Wyœlij do sprzymierzeñców",
	// 55
	L"Nie mo¿na do³¹czyæ do gry. Ta gra ju¿ siê rozpoczê³a",
	L"%s (dru¿yna): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Pobrano wszystkie pliki z serwera.",
	L"Pobrano '%S' z serwera",
	L"Rozpoczêto pobieranie '%s' z serwera",
	L"Nie mo¿na rozpocz¹æ gry dopóki wszyscy nie zakoñcz¹ pobierania plików z serwera",
	L"Ten serwer przed rozpoczêciem gry wymaga pobrania zmodyfikowanych plików, czy chcesz kontynuowaæ",
	// 65
	L"Naciœnij 'Gotowe' aby przejœæ do ekranu taktycznego",
	L"Nie mo¿na siê po³¹czyæ poniewa¿ twoja wersja %S ró¿ni siê od wersji %S na serwerze.",
	L"Zabi³eœ wrog¹ jednostkê",
	L"Nie mo¿na do³¹czyæ do gry. Ta gra ju¿ siê rozpoczê³a",
	L"The server has choosen New Inventory (NIV), but your screen resolution does not support NIV.",
	// 70		// TODO.Translate
	L"Could not save received file '%S'",
	L"%s's bomb was disarmed by &s",
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
	L"Pn", //- Pó³noc
	L"Pd", //- Po³udnie
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
	L"Nie mo¿esz zmieniæ pocz¹tkowego wierzcho³ka dopóki laptop jest odblokowany.",
	L"Nie mo¿esz zmieniaæ dru¿yn dopóki laptop jest odblokowany.",
	L"Losowi najemnicy: ",
	L"T", //if "Y" means Yes here
	L"Poziom trudnoœci:",
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
	L"Strza³y chybione",
	L"Celnoœæ",
	L"Obra¿enia zadane",
	L"Obra¿enia otrzymane",
	L"Czekaj na po³¹czenie z serwerem aby nacisn¹æ 'Kontynuuj'",
};

STR16 gzMPCScreenText[] =
{
	L"Anuluj",
	L"£¹czenie z serwerem",
	L"Pobieranie ustawieñ serwera",
	L"Pobieranie plików",
	L"Naciœnij 'ESC' aby anulowaæ lub 'Y' aby wejœæ na chat",
	L"Naciœnij 'ESC'aby anuowaæ",
	L"Gotowe",
};

STR16 gzMPChatToggleText[] =
{
	L"Wyœlij do wszystkich",
	L"Wyœlij do sprzymierzeñców",
};

STR16 gzMPChatboxText[] =
{
	L"Multiplayer Chat",
	L"Chat: Naciœnij 'ENTER' aby wys³aæ lub 'ESC' by anulowaæ",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// Odnoœnie starych umiejêtnoœci
	L"Na nastêpnej stronie, wybierzesz umiejêtnoœci dotycz¹ce twojej specjalizacji jako najemnika. Nie mo¿na wybraæ wiêcej ni¿ dwóch ró¿nych umiejêtnoœci, albo jednej w stopniu eksperta.",
	
	//L"Mo¿esz tak¿e wybraæ jedn¹, albo nawet nie wybraæ ¿adnej umiejêtnoœci, co da ci bonus do liczby punktów atrybutów. Zauwa¿, ¿e elektronika i oburêcznoœæ nie mog¹ byæ wybrane w stopniu eksperta.",

	// TODO.Translate (added Camouflage)
	L"You can also choose only one or even no traits, which will give you a bonus to your attribute points as a compensation. Note that Electronics, Ambidextrous and Camouflage traits cannot be achieved at expert levels.",

	// Odnoœnie nowych/pomniejszych umiejêtnoœci
	L"Nastêpny etap dotyczy wybierania umiejêtnoœci dotycz¹cych twojej specjalizacji jako najemnika. Na pierwszej stronie, mo¿esz wybraæ do dwóch g³ównych umiejêtnoœci, które reprezentuj¹ twoj¹ rolê w zespole. Druga strona to lista pomniejszych cech, które reprezentuj¹ twoj¹ osobowoœæ.",
	L"Wybranie wiêcej ni¿ trzech jednoczeœnie jest niemo¿liwe. Oznacza to, ¿e je¿eli nie wybierzesz ¿adnych g³ównych umiejêtnoœci, mo¿esz wybraæ trzy pomniejsze cechy. Jeœli wybierzesz 2 g³ówne umiejêtnoœci (albo jedn¹ zaawansowan¹), mo¿esz wybraæ tylko jedn¹ cechê dodatkow¹...",
};

STR16 sgAttributeSelectionText[] = 
{
	L"Proszê, wybierz swoje atrybuty fizyczne zgodnie z twoimi prawdziwymi umiejêtnoœciami. Nie mo¿esz podnieœæ ¿adnego z powy¿szych wyników.",
	L"Przegl¹d atrybutów i umiejêtnoœci I.M.P..",
	L"Punkty bonusowe.:",
	L"Poziom startowy",
	// New strings for new traits
	L"Na nastêpnej stronie wybierzesz swoje atrybuty fizyczne i umiejêtnoœci. 'Atrybuty' to: zdrowie, zwinnoœæ, zrêcznoœæ, si³a oraz inteligencja. Atrybuty nie mog¹ byæ ni¿sze ni¿ %d.",
	L"Reszta to 'umiejêtnoœci', w przeciwieñstwie do atrybutów, mog¹ byæ ustawione na zero, co oznacza, ¿e nie masz w nich kompletnie ¿adnej wprawy.",
	L"Wszystkie punkty s¹ na pocz¹tku ustawione na minimum. Zauwa¿, ¿e niektóre atrybuty s¹ ustawione na wartoœciach, co ma zwi¹zek z wczeœniej wybranymi umiejêtnoœciami. Nie mo¿esz ustawiæ ich ni¿ej.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"Analiza charakteru I.M.P.",
	L"Analiza charakteru to nastêpny krok w tworzeniu twojego profile. Na pierwszej stronie zostanie ci przedstawiona lista cech charakteru do wybrania. Domyœlamy siê, ¿e mo¿esz identyfikowaæ siê z wiêksz¹ ich iloœci¹, jednak bêdziesz móg³ wybraæ tylko jedn¹. Wybierz tak¹, z któr¹ czujesz siê najbardziej zwi¹zany.",
	L"Druga strona przedstawia listê niepe³nosprawnoœci, na które mo¿esz cierpieæ. Jeœli cierpisz na jedn¹ z nich, wybierz j¹ (wierzymy, ¿e ka¿dy ma tylko jedn¹ tak¹ niepe³nosprawnoœæ). B¹dŸ szczery, poniewa¿ to wa¿ne, by poinformowaæ potencjalnych pracodawców o twoim faktycznym stanie zdrowia.",
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
	L"Podstêpny",
	L"Gwiazdor",
	L"Cechy charakteru I.M.P.-a",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"Kolory i sylwetka cia³a I.M.P.-a",
	L"Kolory I.M.P.",
	L"Proszê wybraæ odpowiednie kolory skóry, w³osów i ubrañ oraz swoj¹ sylwetkê cia³a.",
	L" Proszê wybraæ odpowiednie kolory skóry, w³osów i ubrañ.",
	L"Zaznacz, by u¿ywaæ alternatywnego sposobu trzymania broni.",
	L"\n(Uwaga: bêdziesz potrzebowaæ do tego du¿ej si³y.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Kolor w³osów",
	L"Kolor skóry",
	L"Kolor koszulki",
	L"Kolor spodni",
	L"Normalne cia³o",
	L"Du¿e cia³o",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Bez niepe³nosprawnoœci",
	L"Nie lubi ciep³a",
	L"Nerwowy",
	L"Klaustrofobik",
	L"Nie umie p³ywaæ",
	L"Boi siê owadów",
	L"Zapominalski",
	L"Psychol",
	L"Niepe³nosprawnoœci I.M.P.-a",
};

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s nie ma wystarczaj¹cej si³y, by tego dokonaæ",
	L"%s nie ma wystarczaj¹cej zrêcznoœci, by tego dokonaæ",
	L"%s nie ma wystarczaj¹cej zwinnoœci, by tego dokonaæ",
	L"%s nie ma doœæ zdrowia, by tego dokonaæ",
	L"%s nie ma wystarczaj¹cej inteligencji, by tego dokonaæ",
	L"%s nie ma wystarczaj¹cej celnoœci, by tego dokonaæ",
	// 6 - 10
	L"%s nie ma wystarczaj¹cych um. medycznych, by tego dokonaæ.",
	L"%s nie ma wystarczaj¹cych um. mechaniki, by tego dokonaæ.",
	L"%s nie ma wystarczaj¹cych um. dowodzenia, by tego dokonaæ.",
	L"%s nie ma wystarczaj¹cych um. mat. wyb., by tego dokonaæ.",
	L"%s nie ma wystarczaj¹cego doœwiadczenia, by tego dokonaæ.",
	// 11 - 15
	L"%s ma za ma³e morale, by tego dokonaæ",
	L"%s nie ma wystarczaj¹co du¿o energii, by tego dokonaæ",
	L"W %s jest zbyt ma³a lojalnoœæ. Mieszkañcy nie pozwol¹ ci tego zrobiæ.",
	L"W %s pracuje ju¿ zbyt wiele osób.",
	L"W %s zbyt wiele osób wykonuje ju¿ to polecenie.",
	// 16 - 20
	L"%s nie znajduje przedmiotów do naprawy.",
	L"%s traci nieco %s, pracuj¹c w sektorze %s",
	L"%s traci nieco %s, pracuj¹c w %s w sektorze %s",
	L"%s odnosi rany, pracuj¹c w sektorze %s i wymaga natychmiastowej pomocy medycznej!",
	L"%s odnosi rany, pracuj¹c w %s w sektorze %s i wymaga natychmiastowej pomocy medycznej!",
	// 21 - 25
	L"%s odnosi rany, pracuj¹c w sektorze %s. Nie wygl¹da to jednak bardzo powa¿nie.",
	L"%s odnosi rany, pracuj¹c w %s w sektorze %s. Nie wygl¹da to jednak bardzo powa¿nie.",
	L"Mieszkañcy miasta %s wydaj¹ siê poirytowani tym, ¿e %s przebywa w ich okolicy.",
	L"Mieszkañcy miasta %s wydaj¹ siê poirytowani tym, ¿e %s pracuje w %s.",
	L"%s po swych dzia³aniach w sektorze %s powoduje spadek poparcia w regionie.",
	// 26 - 30
	L"%s swymi dzia³aniami w %s w %s powoduje spadek poparcia w regionie.",
	L"%s jest w stanie upojenia alkoholowego",
	L"%s ciê¿ko choruje w sektorze %s i przechodzi w stan spoczynku",
	L"%s ciêzko choruje i nie mo¿e dalej pracowaæ w %s w %s",
	L"%s doznaje ciê¿kich obra¿eñ w sektorze %s",	//	%s was injured in sector %s. // <--- This is a log message string.
	// 31 - 35
	L"%s doznaje ciê¿kich obra¿eñ w sektorze %s", //<--- This is a log message string.


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Si³a",
	L"Zwinnoœæ",
	L"Zrêcznoœæ",
	L"Inteligencja",
	L"Zdrowie",
	L"Umiejêtnoœci strzeleckie",   //short: "Um. strzeleckie"
	// 5-10
	L"Umiejêtnoœæ dowodzenia",   //short: "Um. dowodzenia"
	L"Znajomoœæ mechaniki",   //short: "Zn. mechaniki"
	L"Wiedza medyczna",
	L"Znajomoœæ materia³ów wybuchowych",   //short: "Zn. mat. wybuchowych"
};

STR16 gzFacilityAssignmentStrings[]=
{
	L"AMBIENT",
	L"Staff",
	L"Odpoczywa"
	L"Naprawa ekwipunku",
	L"Naprawa %s",
	L"Naprawa Robota",
	// 6-10
	L"Lekarz",
	L"Pacjent",
	L"Trening si³y",
	L"Trening zrêcznoœci",
	L"Trening zwinnoœci",
	L"Trening zdrowia",
	// 11-15
	L"Trening um. strzeleckich",
	L"Trening wiedzy medycznej",
	L"Trening zn. mechaniki",
	L"Trening dowodzenia",
	L"Trening zn. mat. wybuchowych",
	// 16-20
	L"Uczeñ si³a",
	L"Uczeñ zrêcznoœæ",
	L"Uczeñ zwinnoœæ",
	L"Uczeñ zdrowie",
	L"Uczeñ um. strzeleckie",
	// 21-25
	L"Uczeñ wiedza medyczna",
	L"Uczeñ zn. mechaniki",
	L"Uczeñ um. dowodzenia",
	L"Uczeñ zn. mat. wybuchowych",
	L"Instruktor si³a",
	// 26-30
	L"Instruktor zrêcznoœæ",
	L"Instruktor zwinnoœæ",
	L"Instruktor zdrowie",
	L"Instruktor um. strzeleckie",
	L"Instruktor wiedza medyczna",
	// 30-35
	L"Instruktor zn. mechaniki",
	L"Instruktor um. dowodzenia",
	L"Instruktor zn. mat. wybuchowych",
};

STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 trybie okienkowym wymaga g³êbi koloru 16-bitowego lub mniej.",

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
	L"Na¿ryj siê!",
	L"Jesteœcie moi!",
	L"Zdychaj!",
	L"Boisz siê kurwo?",
	L"To dopiero zaboli!",
	L"Dawaj skurwielu!",
	L"Dawaj! Nie mam ca³ego dnia!",
	L"ChodŸ do tatusia!",
	L"Zaraz pójdziesz do piachu!",
	L"Wracasz do domu w dêbowej jesionce frajerze!",
	L"Chcesz siê zabawiæ?"
	L"Trzeba by³o zostaæ w domu kurwo."
	L"Ciota!",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"Urz¹dzamy grilla.",
	L"Mam dla ciebie prezent.",
	L"Bum!",
	L"Uœmiech!",
};

STR16 sEnemyTauntsThrow[]=
{
	L"£ap!",
	L"A masz!",
	L"Przysz³a kryska na Matyska!",
	L"To dla ciebie!",
	L"Hahahaha!",
	L"£ap œwinio!",
	L"Uwielbiam to.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"Zedrê ci skalp!",
	L"ChodŸ do tatusia.",
	L"Pochwal siê flakami!",
	L"Por¿nê ciê na kawa³ki!",
	L"Skurwysyny!",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"Jesteœmy po uszy w gównie...",
	L"IdŸ do wojska -mówili. Nie ma chuja, nie po to!",
	L"Mam ju¿ doœæ.",
	L"O mój Bo¿e.",
	L"Za to mi nie p³ac¹.",
	L"Nie wytrzymam tego!",
	L"Wrócê z kumplami.",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"S³ysza³em to!",
	L"Kto tam?",
	L"Co to by³o?",
	L"Hej! Co do...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"S¹ tutaj!",
	L"Czas zacz¹æ zabawê."
	L"Liczy³em na to, ¿e to siê nie stanie.",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Au!",
	L"U³...",
	L"To... boli!",
	L"Skurwysyny!",
	L"Po¿a³ujecie... tego.",
	L"Co do..!",
	L"Teraz siê... wkurwi³em.",

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
	L"|M|u|s|t|a|r|d |G|a|s |E|n|d |R|a|d|i|u|s",
	L"|L|i|g|h|t |E|n|d |R|a|d|i|u|s",
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
	L"|M|a|x|i|m|u|m |C|o|u|n|t|e|r|-|F|o|r|c|e",
	L"|C|o|u|n|t|e|r|-|F|o|r|c|e |F|r|e|q|u|e|n|c|y",
	L"|T|o|-|H|i|t |B|o|n|u|s",
	L"|A|i|m |B|o|n|u|s",
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
	L"\n \nWhen this weapon is raised to the shooting position,\nit changes the wearer's field-of-view.\n \nNarrowing the field of view shortens sightrange to\neither side.\n \nLower is better.",
	L"\n \nThis is the shooter's ability to\ncope with recoil during Burst or Autofire volleys.\n \nHigher is better.",
	L"\n \nThis is the shooter's ability to\nfrequently reasses how much counter-force they\nneed to apply against a gun's recoil.\n \nNaturally, this has no effect if the weapon lacks\nboth Burst and Auto-Fire modes.\n \nLower frequency makes volleys more accurate on the whole,\nand also makes longer volleys more accurate assuming\nthe shooter can overcome recoil correctly.\n \nLower is better.",
	L"\n \nThis weapon's to-hit is being modified by\nan ammo, attachment, or built-in attributes.\n \nIncreased To-Hit allows the gun to hit targets\nmore often, assuming it is also well-aimed.\n \nHigher is better.",
	L"\n \nThis weapon's Aim Bonus is being modified by\nan ammo, attachment, or built-in attributes.\n \nIncreased Aim Bonus allows the gun to hit\ntargets at longer ranges more often, assuming\nit is also well-aimed.\n \nHigher is better.",
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

#endif //POLISH
