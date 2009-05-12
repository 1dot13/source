#pragma setlocale("GERMAN")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25GermanText.h"
#else
	#include "Language Defines.h"
	#include "text.h"
	#include "Fileman.h"
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_Ja25GermanText_public_symbol(void){;}

#ifdef GERMAN

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	L"Entfernung zum Ziel: %d Felder, Helligkeit: %d/%d",
	L"Verbinden Sie den Transmitter mit Ihrem Laptop-Computer.",
	L"Sie haben nicht genug Geld, um %s anzuheuern",
	L"Das obenstehende Honorar deckt für einen begrenzten Zeitraum die Kosten der Gesamtmission, und schließt untenstehendes Equipment mit ein.",
	L"Engagieren Sie %s jetzt und nutzen Sie den Vorteil unseres beispiellosen 'Ein Betrag für alles'-Honorars. Das persönliche Equipment des Söldners ist gratis in diesem Preis mit inbegriffen.",
	L"Honorar",
	L"Da ist noch jemand im Sektor...",
	L"Waffen-Rchwt.: %d Felder, Trefferwahrsch.: %d Prozent",
	L"Deckung anzeigen",
	L"Sichtfeld",
	L"Neue Rekruten können dort nicht hinkommen.",
	L"Da Ihr Laptop keinen Transmitter besitzt, können Sie keine neuen Teammitglieder anheuern. Vielleicht ist dies eine guter Zeitpunkt, ein gespeichertes  Spiel zu laden oder ein neues zu starten!",
	L"%s hört das Geräusch knirschenden Metalls unter Jerry hervordringen. Es klingt grässlich - die Antenne ihres Laptop-Computers ist  zerstört.",  //the %s is the name of a merc.  @@@  Modified
	L"Nach Ansehen des Hinweises, den Commander Morris hinterließ, erkennt %s eine einmalige Gelegenheit. Der Hinweis enthält Koordinaten für den Start von Raketen gegen verschiedene Städte in Arulco. Aber er enthält auch die Koordinaten des Startpunktes - der Raketenanlage.",
	L"Das Kontroll-Board studierend, entdeckt %s, dass die Zahlen umgedreht werden könnten, so dass die Raketen diese Anlage selbst zerstören. %s muss nun einen Fluchtweg finden. Der Aufzug scheint die schnellstmögliche Route zu bieten...",         //!!! The original reads:	L"Noticing the control panel %s, figures the numbers can be reversed..." That sounds odd for me, but I think the comma is placed one word too late... (correct?)
	L"Dies ist ein IRON MAN-Spiel, und es kann nicht gespeichert werden, wenn sich Gegner in der Nähe befinden.",		
	L"(Kann während Kampf nicht speichern)", 
	L"Der Name der aktuellen Kampagne enthält mehr als 30 Buchstaben.",						
	L"Die aktuelle Kampagne kann nicht gefunden werden.",																	
	L"Kampagne: Standard ( %S )",																							
	L"Kampagne: %S",																													
	L"Sie haben die Kampagne %S gewählt. Diese ist eine vom Spieler modifizierte Version der Originalkampagne von JA2UB. Möchten Sie die Kampagne %S spielen?",			
	L"Um den Editor zu benutzen, müssen Sie eine andere als die Standardkampgane auswählen.",		
};

//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"Schlösser knacken",
	L"Nahkampf",
	L"Elektronik",
	L"Nachteinsatz",
	L"Werfen",
	L"Lehren",
	L"Schwere Waffen",
	L"Automatische Waffen",
	L"Schleichen",
	L"Beidhändig geschickt",
	L"Messer",
	L"Dach-Treffer Bonus",
	L"Getarnt",
	L"Kampfsport",

	L"Keine",
	L"B.S.E. Persönlichkeiten",
};

//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Sie haben sich für den Ironman- Modus entschieden. Mit dieser Einstellung können Sie das Spiel nicht speichern, wenn Feinde im Sektor sind. Sind Sie sicher, dass Sie im Ironman- Modus spielen wollen?",
};


#endif