#pragma setlocale("POLISH")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25Polishtext.h"
#else
	#include "Language Defines.h"
	#ifdef POLISH
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

#ifdef POLISH

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	L"Odleg³oœæ od celu (w polach): %d, Jasnoœæ = %d/%d",
	L"Nadajnik zosta³ pod³¹czony do twojego laptopa.",
	L"Nie mo¿esz zatrudniæ %s(a)",
	L"Na okreœlony czas, poni¿sze honorarium pokryje koszt ca³ej misji razem z wyposa¿eniem zamieszonym poni¿ej.",
	L"Zatrudnij %s(a) ju¿ teraz i weŸ udzia³ naszej promocji 'jedno honorarium pokrywa wszystko'.  Ponadto w tej niewiarygodnej ofercie ca³y ekwipunek najemnika dostaniesz za darmo.",
	L"Honorarium",
	L"Ktoœ jest w sektorze...",
	L"Zasiêg broni (w polach): %d, Szansa na trafienie: %d procent",
	L"Poka¿ os³onê",
	L"Zasiêg wzroku",
	L"Nowi rekruci nie mog¹ tam przybyæ.",
	L"Dopóki twój laprop bêdzie bez nadajnika, nie bêdziesz móg³ zatrudniaæ nowych cz³onków zespo³u.  Mo¿liwe, ¿e to odpowiedni moment ¿eby odczytaæ zapisany stan gry lub zacz¹æ graæ od nowa!",
	L"%s s³yszy dŸwiêk zgniatanego metalu dochodz¹cy spod cia³a Jerrego.  To niestety zabrzmia³o jak dŸwiêk zgniatanej anteny od twojego laptopa.",  //the %s is the name of a merc.  @@@  Modified
	L"After scanning the note left behind by Deputy Commander Morris, %s senses an oppurtinity.  The note contains the coordinates for launching missiles against different towns in Arulco.  It also gives the coodinates of the origin - the missile facility.",
	L"Noticing the control panel, %s figures the numbers can be reveresed, so that the missile might destroy this very facility.  %s needs to find an escape route.  The elevator appears to offer the fastest solution...",
	L"To jest tryb CZ£OWIEK ZE STALI i nie mo¿esz zapisywaæ gry gdy wróg jest w sektorze.",	//	@@@  new text
	L"(Nie mo¿na zapisywaæ gry podczas walki)", //@@@@ new text
	L"Kampania ma wiêcej ni¿ 30 postaci.",							// @@@ new text
	L"Nie mo¿na odnaleŸæ kampanii.",																	// @@@ new text
	L"Kampania: Standardowa ( %S )",																							// @@@ new text
	L"Kampania: %S",																													// @@@ new text
	L"Wybra³eœ kampaniê %S. Ta kampania zosta³a stworzona przez fanów gry. Czy jesteœ pewien, ¿e chcesz w ni¹ zagraæ?",			// @@@ new text
	L"¯eby u¿yæ edytora powinieneœ wczeœniej wybraæ kampaniê inn¹ ni¿ standardowa.",		///@@new
};

//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"Otwieranie zamków",
	L"Walka wrêcz",
	L"Elektronika",
	L"Operacje nocne",
	L"Rzucanie",
	L"Szkolenie",
	L"Broñ ciê¿ka",
	L"Broñ automatyczna",
	L"Skradanie siê",
	L"Oburêcznoœæ",
	L"Broñ bia³a",
	L"Snajper",
	L"Kamufla¿",
	L"Sztuki walki",

	L"Brak",
	L"Umiejêtnoœci",
};
//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Wybra³eœ tryb CZ£OWIEK ZE STALI. Opcja ta sprawi, ¿e gra bêdzie du¿o trudniejsza poniewa¿ nie bêdzie mo¿liwoœci zapisywania gry podczas walki. Bêdzie to mia³o wp³yw na ca³y przebieg rozgrywki. Czy na pewno chcesz graæ w trybie CZ£OWIEK ZE STALI?",
};


#endif
