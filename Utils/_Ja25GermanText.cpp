// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("GERMAN")

	#include "Language Defines.h"
	#ifdef GERMAN
		#include "text.h"
		#include "Fileman.h"
	#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_Ja25GermanText_public_symbol(void){;}

#ifdef GERMAN

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

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
	L"Autom. Waffen",
	L"Schleichen",
	L"Beidhändig geschickt",
	L"Messer",
	L"Dach-Treffer Bonus",
	L"Getarnt",
	L"Kampfsport",

	L"Keine",
	L"B.S.E. - Spezialisierungen",
	L"(Experte)",
};

//added another set of skill texts for new major traits
//Da es mittlerweile einen beruflichen Background für Söldner gibt, habe ich die "Tätigkeitsbezeichnungen" der Traits wieder an das ursprüngliche System angeglichen.
//Die Traits sagen nun aus, in welchem Bereich ein Söldner besondere Fertigkeiten erworben hat. Sie stellen kein Berufsbild dar. Das bleibt dem Background überlassen.
//Vorschläge in alten Kommentaren berücksichtigt. [Leonidas]
STR16 gzIMPSkillTraitsTextNewMajor[]=
{
	L"Automatische Waffen",			//
	L"Granatwaffen",			// Vanilla: Schwere Waffen/Heavy Weapons [Leonidas]
	L"Scharfschützengewehre",		// (Schlechte) Alternative: Präzisionsgewehre [Leonidas]
	L"Jagd",				// Alternative: Überlebenstraining [Leonidas] – Die Mischung aus/von Flintenboni, Bewegungsboni und Tarnboni ist ungünstig [Leonidas]
	L"Pistolen",				//
	L"Faustkampf",				//
	L"Teamführung",				//
	L"Feinmechanik",			//
	L"Erste Hilfe",				// Alternativen: Sanitätsausbildung; Medizin. Versorgung [Leonidas]
	L"Verdeckte Operationen",		//

	L"Nichts",
	L"B.S.E. Hauptfertigkeiten",

	// second names
	L"Automatische Waffen\n(Experte)",	// (Schlechte) Alternative: Maschinengewehre [Leonidas]
	L"Granatwaffen (Experte)",		//
	L"Scharfschützengewehre\n(Experte)",	//
	L"Jagd (Experte)",			// Alternative: Jagd – Die Mischung aus/von Flintenboni, Bewegungsboni und Tarnboni ist ungünstig [Leonidas]
	L"Pistolen (Experte)",			//
	L"Kampfsport",				//
	L"Truppenführung",			//
	L"Feinwerktechnik",			// Alternative: Mechatronik [Leonidas]
	L"Notfallmedizin",			//
	L"Geheimoperationen",			//
};

//added another set of skill texts for new minor traits
STR16 gzIMPSkillTraitsTextNewMinor[]=
{
	L"Beidhändigkeit",			// alt. "Beidhändig geschickt"
	L"Bewaffneter Nahkampf",		// alt. "Hieb- und Stichwaffen"
	L"Wurfwaffen",				// alt. "Wurfwaffen"
	L"Nachteinsätze",			// alt. "Nachteinsatz"
	L"Schleichen",				// alt. "Schleichen"
	L"Leichtathletik",			// alt. "Athletisch"
	L"Kraftsport",				// alt. "Bodybuilding"
	L"Sprengtechnik",			// alt. "Kampfmittel"
	L"Ausbilden",				// alt. "Lehren"
	L"Aufklären",				// alt. "Spähen"
	L"Funktechnik",				// Alternative: Funk, Funkkommunikation [Leonidas]
	L"Survival",	//TODO.Translate

	L"Keine",
	L"B.S.E. Nebenfertigkeiten",
};

//these texts are for help popup windows, describing trait properties
STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[]=
{
	L"+%d%s Trefferchance mit Sturmgewehren\n",
	L"+%d%s Trefferchance mit Maschinenpistolen\n",
	L"+%d%s Trefferchance mit Maschinengewehren\n",
	L"-%d%s APs benötigt für MG-Feuerstöße (Burst/Auto) abzugeben\n",
	L"-%d%s APs benötigt um Maschinengewehre auszurichten\n",
	L"Trefferratenabzug bei Feuerstößen um %d%s reduziert\n",
	L"Geringere Wahrscheinlichkeit bei Feuerstößen ungewollt mehr Schüsse abzugeben\n",
};

STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[]=
{
	L"-%d%s APs benötigt um Granatwerfer abzufeuern\n",
	L"-%d%s APs benötigt um Raketenwaffen abzufeuern\n",
	L"+%d%s Trefferchance mit Granatwerfern\n",
	L"+%d%s Trefferchance mit Raketenwaffen\n",
	L"-%d%s APs für den Abschuss von Mörsergranaten benötigt\n",
	L"Trefferchancenreduktion für Mörser gesenkt um %d%s\n",
	L"+%d%s Schaden an Panzern mit Granatwaffen, Granaten und Bomben\n",
	L"+%d%s Schaden an allen anderen Zielen mit Granatwaffen\n",
};

STR16 gzIMPMajorTraitsHelpTextsSniper[]=
{
	L"+%d%s Trefferchance mit Büchsen\n",
	L"+%d%s Trefferchance mit Scharfschützengewehren\n",
	L"-%d%s effektive Reichweite zum Ziel mit allen Waffen\n",
	L"+%d%s Zielbonus pro Zielerfassungs-Klick (außer für Faustfeuerwaffen)\n",
	L"+%d%s Schaden pro Schuss",
	L" plus",
	L" für jeden Zielerfassungs-Klick",
	L" nach dem ersten",
	L" nach dem zweiten",
	L" nach dem dritten",
	L" nach dem vierten",
	L" nach dem fünften",
	L" nach dem sechsten",
	L" nach dem siebenten",
	L"-%d%s APs benötigt um ein Repetiergewehr erneut fertigzuladen.\n",
	L"Gibt einen weiteren Ziel-Klick für gewehrartige Waffen\n",
	L"Gibt weitere %d Ziel-Klicks für gewehrartige Waffen\n",
	L"Schnelleres Zielen mit Gewehren bei genau einem Zielgenauigkeit-Klick\n",
	L"Schnelleres Zielen mit Gewehren bei %d Zielgenauigkeit-Klicks\n",
	L"Focus skill: +%d interrupt modifier in marked area\n",	// TODO.Translate

};
STR16 gzIMPMajorTraitsHelpTextsRanger[]=
{
	L"+%d%s Trefferchance mit Gewehren\n",
	L"+%d%s Trefferchance mit Schrotflinten\n",
	L"-%d%s APs benötigt, um Schrotflinten zu repetieren\n",
	L"-%d%s APs benötigt, um Schrotflinten abzufeuern\n",
	L"Gibt Schrotflinten einen weiteren Zielklick\n",
	L"%d weitere Zielklicks für Schrotflinten\n",
	L"+%d%s effektive Reichweite mit Schrotflinten\n",
	L"-%d%s APs benötigt, um einzelne Schrotpatronen zu laden\n",
	L"Gibt Gewehren einen weiteren Zielklick\n",
	L"%d weitere Zielklicks für Gewehre\n",
};

STR16 gzIMPMajorTraitsHelpTextsGunslinger[]=
{
	L"-%d%s APs benötigt um mit Pistolen oder Revolvern zu schießen\n",
	L"+%d%s effektive Reichweite mit Pistolen und Revolvern\n",
	L"+%d%s Trefferchance mit  mit halbautomatischen Pistolen und Revolvern\n",
	L"+%d%s Trefferchance mit vollautomatischen Pistolen",
	L" (nur bei Einzelfeuer)",
	L"+%d%s Zielbonus pro Klick mit halb- und vollautomatischen Pistolen sowie Revolvern\n",
	L"-%d%s APs benötigt um Pistolen und Revolver in Vorhalte zu bringe\n",
	L"-%d%s APs benötigt um halb- und vollautomatische Pistolen sowie Revolver nachzuladen\n",
	L"Gibt für halb- und vollautomatische Pistolen sowie Revolver einen weiteren Zielklick\n",
	L"%d weiteren Zielklick für halb- und vollautomatische Pistolen sowie Revolver\n",
	L"Can fan the hammer with revolvers\n",	// TODO.Translate
};

STR16 gzIMPMajorTraitsHelpTextsMartialArts[]=
{
	L"-%d%s AP-Kosten für den Nahkampf (bloße Hände oder mit Schlagring)\n",
	L"+%d%s Trefferchance im Nahkampf mit bloßen Händen\n",
	L"+%d%s Trefferchance im Nahkampf mit dem Schlagring\n",
	L"+%d%s Schaden im Nahkampf (bloße Hände oder mit Schlagring)\n",
	L"+%d%s Ausdauerschaden im Nahkampf (bloße Hände oder mit Schlagring)\n",
	L"Ein im Nahkampf niedergestreckter Gegner braucht etwas länger um sich zu erholen\n",
	L"Ein im Nahkampf niedergestreckter Gegner braucht länger um sich zu erholen\n",
	L"Ein im Nahkampf niedergestreckter Gegner braucht deutlich länger um sich zu erholen\n",
	L"Ein im Nahkampf niedergestreckter Gegner braucht viel länger um sich zu erholen\n",
	L"Ein im Nahkampf niedergestreckter Gegner braucht sehr viel länger um sich zu erholen\n",
	L"Ein im Nahkampf niedergestreckter Gegner schläft wie ein Baby bevor er sich erholt\n",
	L"Ein im Nahkampf niedergestreckter Gegner steht vermutlich erstmal gar nicht mehr auf\n",
	L"Ein gezielter Schlag richtet +%d%s mehr Schaden an\n",
	L"Ein gezielter Tritt richtet +%d%s mehr Schaden an\n",
	L"+%d%s Chance, Schlägen und Tritten auszuweichen\n",
	L"Dazu +%d%s Chance mit freien Händen",
	L" oder nur mit Schlagring",
	L" (+%d%s mit Schlagring)",
	L"+Dazu %d%s Chance, Schlägen und Tritten mit ausgerüstetem Schlagring auszuweichen\n",
	L"+%d%s Chance einem Angriff mit einer beliebigen Nahkampfwaffe auszuweichen\n",
	L"-%d%s APs benötigt um einen Gegner zu entwaffnen\n",
	L"-%d%s APs benötigt um die Körperhaltung zu ändern, sich umzudrehen, auf oder von Dächern zu klettern und Hindernisse zu überspringen\n",
	L"-%d%s APs benötigt um die Körperhaltung zu ändern (stehen, ducken, liegen)\n",
	L"-%d%s APs benötigt um sich umzudrehen\n",
	L"-%d%s APs benötigt um auf oder von Dächern zu klettern und Hindernisse zu überspringen\n",
	L"+%d%s Chance eine Tür erfolgreich einzutreten\n",
	L"Besondere Animationen für den Nahkampf\n",
	L"-%d%s Wahrscheinlichkeit bei einem Nahkampfangriff unterbrochen zu werden\n",
};

STR16 gzIMPMajorTraitsHelpTextsSquadleader[]=
{
	L"+%d%s APs pro Runde für andere Söldner im Einflussbereich\n",
	L"+%d Erfahrung für umgebende Söldner im Einflussbereich mit weniger Erfahrung als der %s\n",
	L"+%d Erfahrung beim Berechnen des Gruppeneffekts auf Unterdrückungsfeuer\n",
	L"+%d%s Resistenz gegen Unterdrückungsfeuer für den %s und andere Söldner im Einflussbereich\n",
	L"+%d Moralgewinn für andere Söldner im Einflussbereich\n",
	L"-%d Moralverlust für andere Söldner im Einflussbereich\n",
	L"Der Einflussbereich hat einen Radius von %d Feldern",
	L"(%d Felder mit Kopfhörer-Funkgerät)",
	L"(Gleichzeitig wirksame Boni für einen Söldner begrenzt auf: %d)\n",
	L"+%d%s Widerstand gegen Furcht für den %s\n",
	L"Nachteil: %dx Moralverlust bei Tod des %ss für alle anderen Söldner\n",
	L"+%d%s Wahrscheinlichkeit für gemeinsame Unterbrechungen\n",
};

STR16 gzIMPMajorTraitsHelpTextsTechnician[]=
{
	L"+%d%s schnellere Reparaturen\n",
	L"+%d%s mehr Erfolg beim Knacken normaler und elektronischer Schlösser\n",
	L"+%d%s mehr Erfolg beim Entschärfen elektronischer Fallen\n",
	L"+%d%s mehr Erfolg beim Anbringen besonderer Gegenstände und beim Zusammenbau von Einzelteilen\n",
	L"+%d%s mehr Erfolg beim Beheben von Waffenstörungen im Gefecht\n",
	L"Der Malus beim Reparieren elektronischer Gegenstände wird um %d%s gesenkt\n",
	L"Erhöhte Chance, Fallen und Minen zu entdecken (+%d zum Erkennungslevel)\n",
	L"+%d%s Trefferchance des Roboters, wenn vom %s gesteuert\n",
	L"Der %s kann den Roboter reparieren\n",
	L"%d%s Reduzierung des Geschwindigkeitsabzugs beim Reparieren des Roboters\n",
	L"Able to restore item threshold to 100%% during repair\n",	// TODO.Translate
};

STR16 gzIMPMajorTraitsHelpTextsDoctor[]=
{
	L"Kann Patienten operieren (bei Verwendung eines Arztkoffers)\n",
	L"Die Operation stellt sofort %d%s der verlorenen Lebenspunkte wieder her.",
	L" (Dieser Vorgang verbraucht einen Großteil des Arztkoffers.)",
	L"Kann verlorene Attributpunkte (durch kritische Treffer) mittels",
	L" einer sofortigen Operation oder",
	L" der Einteilung als Arzt (Sektorübersicht) wiederherstellen.\n",
	L"+%d%s bessere Heilungsrate der Patienten\n",
	L"+%d%s schnelleres Anlegen von Wundverbänden\n",
	L"+%d%s natürliche Regenerationsrate aller Söldner im selben Sektor",
	L" (maximal %d Instanzen dieses Bonus pro Sektor)",
	L"Returned health can be boosted an additional %d%s by using blood bags.\n",	// TODO.Translate
};

STR16 gzIMPMajorTraitsHelpTextsCovertOps[]=
{
	L"Kann sich als Zivilist oder feindlicher Soldat ausgeben, um hinter die feindlichen Linien zu gelangen\n",
	L"Wird bei verdächtigen Aktionen, verdächtiger Ausrüstung oder in der Nähe frischer Leichen immer erkannt\n",
	L"Wird in feindlicher Uniform erkannt, wenn der Feind näher als %d Felder entfernt ist\n",
	L"Wird in feindlicher Uniform erkannt, wenn eine frische Leiche näher als %d Felder entfernt ist.\n",
	L"+%d%s Trefferwahrscheinlichkeit mit verdeckten Nahkampfwaffen (z.B. Garotte)\n",
	L"+%d%s Wahrscheinlichkeit für einen tödlichen Angriff mit verdeckten Nahkampfwaffen (z.B. Garotte)\n",
	L"Um %d%s verringerte AP-Kosten zum Verkleiden\n",
	L"Kann feindliche Soldaten zum Überlaufen überreden.\n",
};

STR16 gzIMPMajorTraitsHelpTextsRadioOperator[]=
{
	L"Kann Funkausrüstung nutzen\n", 
	L"Kann Artillerieunterstützung von Verbündeten in Nachbarsektoren anfordern\n",
	L"Kann Funkfrequenzen abhören und feindliche Truppen aufspüren\n",
	L"Kann den Funkverkehr im ganzen Sektor stören\n",
	L"Kann nach feindlichen Störsendern suchen, wenn solche aktiv sind\n"
	L"Kann Unterstützung durch Milizen aus Nachbarsektoren anfordern\n",
};

STR16 gzIMPMajorTraitsHelpTextsNone[]=
{
	L"Keine Boni",
};

STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[]=
{
	L"Reduced penalty to shoot if offhand item is equipped by %d%s\n",	// TODO.Translate
	L"+%d%s schnelleres Nachladen mit Magazinen\n",
	L"+%d%s schnelleres Nachladen mit einzelnen Patronen\n",
	L"-%d%s APs benötigt um Gegenstände aufzuheben\n",
	L"-%d%s APs benötigt für die Handhabe des Rucksacks\n",
	L"-%d%s APs benötigt um mit Türen zu interagieren\n",
	L"-%d%s APs benötigt um Bomben und Minen zu legen oder zu entschärfen\n",
	L"-%d%s APs needed to attach items\n",
};

STR16 gzIMPMinorTraitsHelpTextsMelee[]=
{
	L"-%d%s APs benötigt für den Angriff mit Klingenwaffen\n",
	L"+%d%s Trefferchance mit Klingenwaffen\n",
	L"+%d%s Trefferchance mit Schlagwaffen\n",
	L"+%d%s Schaden mit Klingenwaffen\n",
	L"+%d%s Schaden mit Schlagwaffen\n",
	L"Angriffe mit Nahkampfwaffen richten %d%s mehr Schaden an\n",
	L"+%d%s Chance Angriffen durch Klingenwaffen auszuweichen\n",
	L"Dazu +%d%s Chance Klingenwaffen auszuweichen wenn man selber eine in der Hand hat\n",
	L"+%d%s Chance Angriffen durch Schlagwaffen auszuweichen\n",
	L"Dazu +%d%s Chance Schlagwaffen auszuweichen wenn man eine Klingenwaffe führt\n",
};

STR16 gzIMPMinorTraitsHelpTextsThrowing[]=
{
	L"-%d%s Basis-APs benötigt für den Angriff mit Wurfwaffen\n",
	L"+%d%s maximale Reichweite beim Einsatz von Wurfwaffen\n",
	L"+%d%s Trefferchance mit Wurfwaffen\n",
	L"+%d%s Trefferchance mit Wurfwaffen für jeden Ziel-Klick\n",
	L"+%d%s Schaden geworfener Klingen\n",
	L"+%d%s Schaden geworfener Klingen für jeden Ziel-Klick\n",
	L"+%d%s Chance auf kritischen Treffer beim Angriff mit Wurfwaffen, falls das Ziel den Werfer nicht bemerkt hat\n",
	L"+%d Multiplikator für kritische Treffer durch Wurfwaffen\n",
	L"Gibt einen weiteren Zielklick beim Einsatz von Wurfwaffen\n",
	L"Gibt %d weitere Zielklicks beim Einsatz von Wurfwaffen\n",
	L"-%d%s APs benötigt um Handgranaten (und ähnliche Objekte) zu werfen\n",
	L"+%d%s höhere Reichweite beim Werfen von Handgranaten (und ähnliche Objekten)\n",
	L"+%d%s höhere Trefferwahrscheinlichkeit beim Werfen von Handgranaten (und ähnlichen Objekten)\n",
};

STR16 gzIMPMinorTraitsHelpTextsNightOps[]=
{
	L"+%d zur effektiven Sichtweite im Dunkeln\n",
	L"+%d zum allgemeinen effektiven Hörweite\n",
	L"Dazu +%d zum effektive Hörweite in der Dunkelheit\n",
	L"+%d zum Unterbrechungs-Modifikator in der Dunkelheit\n",
	L"-%d weniger Schlafbedarf\n",
};

STR16 gzIMPMinorTraitsHelpTextsStealthy[]=
{
	L"-%d%s APs zum Schleichen nötig\n",
	L"+%d%s Chance beim Schleichen kein Geräusch zu erzeugen zu sein\n",
	L"+%d%s Chance, 'unsichtbar' zu sein wenn man sich nicht verrät (schleichen)\n",
	L"Der Abzug der berechneten Sichtdeckung beim Bewegen ist %d%s geringer\n",
	L"-%d%s Wahrscheinlichkeit um unterbrochen zu werden\n",
};

STR16 gzIMPMinorTraitsHelpTextsAthletics[]=
{
	L"-%d%s APs benötigt für Bewegung (rennen, aufrecht oder geduckt gehen, gleiten, schwimmen, usw.)\n",
	L"-%d%s weniger Ausdauerverbrauch für für Bewegung, Dachklettern, Hindernisse Überwinden, usw.\n",
};

STR16 gzIMPMinorTraitsHelpTextsBodybuilding[]=
{
	L"Hat eine Schadensresistenz von %d%s\n",
	L"+%d%s effektive Stärke für das Berechnen der maximalen Traglast\n",
	L"%d%s weniger Energieverlust beim Erleiden von Schlägen und Tritten\n",
	L"Fällt bei Beintreffern weniger leicht um durch um %d%s erhöhte Schadenstoleranz\n",
};

STR16 gzIMPMinorTraitsHelpTextsDemolitions[]=
{
	L"+%d%s höherer Schaden für gelegte Bomben und Minen\n",
	L"+%d%s mehr Erfolg beim Anbringen von Zündern\n",
	L"+%d%s mehr Erfolg beim Legen und Entschärfen von Bomben\n",
	L"Geringere Wahrscheinlichkeit, dass Gegner gelegte Bomben und Minen entdecken (%d zum Bombenlevel)\n",
	L"Höhere Wahrscheinlichkeit Türen mit einer Sprengladung öffnen zu können (Schaden wird mit %d multipliziert)\n",
};

STR16 gzIMPMinorTraitsHelpTextsTeaching[]=
{
	L"Bei der Ausbildung von Milizen +%d%s schneller\n",
	L"Bei der Ausbildung von Milizen +%d%s Bonus zur effektiven Führungsfähigkeit\n",
	L"Beim Ausbilden von Söldnern +%d%s schneller\n",
	L"Beim Ausbilden von Söldnern +d% zum effektiven Fähigkeitslevel des Ausbilders\n",
	L"Beim eigenständigen Lernen +%d%s schneller\n",
};

STR16 gzIMPMinorTraitsHelpTextsScouting[]=
{
	L"+%d%% zur effektiven Sichtweite mit Zielfernrohren an Waffen\n",
	L"+%d%% zur effektiven Sichtweite mit Doppelfernrohren und losen Zielfernrohren\n",
	L"-%d%% Tunnelblick mit Doppelfernrohren und losen Zielfernrohren\n",
	L"Auf der Weltkarte wird in angrenzenden Sektoren die genaue Feindstärke (Anzahl) bestimmt\n",
	L"Auf der Weltkarte wird in angrenzenden Sektoren die Präsenz von vorhandenem Feind enthüllt\n",
	L"Verhindert, dass der Feind die Gruppe in den Hinterhalt lockt\n",
	L"Verhindert, das Umzingeln der Gruppe durch Bloodcats\n",
};

STR16 gzIMPMinorTraitsHelpTextsSnitch[]=
{
	L"Informiert täglich über die Ansichten der Teammitglieder\n",
	L"Verhindert mögliches Fehlverhalten der Teammitglieder (Alkohol- und Drogenkonsum, Diebstahl)\n",
	L"Kann in Städten Propaganda verbreiten\n",
	L"Kann in Städten Gerüchten nachgehen\n",
	L"Kann als Informant in Gefängnisse eingeschleust werden\n",
	L"Kann bei guter Moral den allgemeinen Ruf jeden Tag um %d verbessern\n",
	L"Erhält +%d auf die Hörweite",
};

STR16 gzIMPMajorTraitsHelpTextsSurvival[] =
{
	L"-%d%s Reisezeit der Gruppe zwischen Sektoren zu Fuß\n",
	L"-%d%s Reisezeit der Gruppe zwischen Sektoren bei Benutzung von Fahrzeugen (außer dem Helikopter)\n",
	L"-%d%s weniger Energieverlust beim Reisen zwischen Sektoren\n",
	L"-%d%s Einfluss durch schlechtes Wetter\n",
	L"-%d%s Abnutzung von Tarnfarbe durch Wasser oder Zeit\n",
	L"Kann Spuren bis zu %d Felder weit erkennen\n",

	L"%s%d%% disease resistance\n",	//TODO.Translate
	L"%s%d%% food consumption\n",
	L"%s%d%% water consumption\n",
	L"+%d%% snake evasion\n",	// TODO.Translate
	L"+%d%% Tarnungs-Effektivität\n",
};

STR16 gzIMPMinorTraitsHelpTextsNone[]=
{
	L"Keine Boni",
};

STR16 gzIMPOldSkillTraitsHelpTexts[]=
{
	L"+%d%s Bonus zum Schlösser Knacken\n",		// 0
	L"+%d%s Trefferchance im Faustkampf\n",
	L"+%d%s Schaden im Faustkampf\n",
	L"+%d%s Chance Schlägen auszuweichen\n",
	L"Bei der Reparatur und Bedienung von Elektrotechnik\n(Schlösser, Fallen, Fernzünder, Roboter...) kein Abzug\n",
	L"+%d zur effektiven Sichtweite im Dunkeln\n",
	L"+%d zur allgemeinen effektiven Hörweite\n",
	L"Dazu +%d zur effektiven Hörweite in der Dunkelheit\n",
	L"+%d zum Unterbrechungsmodifikator in der Dunkelheit\n",
	L"-%d weniger Schlafbedarf\n",
	L"+%d%s maximale Reichweite beim Werfen\n",	// 10
	L"+%d%s Trefferchance beim Werfen\n",
	L"+%d%s Chance auf sofortige Tötung mit Wurfmesser wenn unbemerkt\n",
	L"+%d%s Bonus zum Trainieren von Milizen und anderen Söldnern\n",
	L"+%d%s effektive Führungsfertigkeit beim Ausbilden von Milizen\n",
	L"+%d%s Trefferchance mit Raketen-/Granatwerfern und Mörsern\n",
	L"Trefferchancenabzug bei Dauerfeuer und Feuerstoß wird durch %d geteilt\n",
	L"Das Verschießen von zu viel Munition bei Dauerfeuer wird unwahrscheinlicher\n",
	L"+%d%s Chance sich leise zu bewegen\n",
	L"+%d%s stealth (unsichtbar sein, wenn man sich nicht verrät)\n",
	L"Beim Schießen mit zwei Waffen mit jeder so präzise wie mit nur einer\n",	// 20
	L"+%d%s Trefferchance mit Stichwaffen\n",
	L"+%d%s Chance, Stichwaffen auszuweichen, wenn man selber eine führt\n",
	L"+%d%s Chance, Stichwaffen auszuweichen, wenn man etwas anderes in der Hand hat\n",
	L"+%d%s Chance Schlägen auszuweichen, wenn man eine Stichwaffe hält\n",
	L"-%d%s effektive Reichweite zum Ziel mit allen Waffen\n",
	L"+%d%s Bonus zum Zielen pro Mausklick\n",
	L"Immer vollständig getarnt sein\n",
	L"+%d%s Trefferchance im Faustkampf\n",
	L"+%d%s Schaden im Faustkampf\n",
	L"+%d%s Chance, Schläge mit leeren Händen zu blocken\n",	// 30
	L"+%d%s Chance, Schläge mit etwas in der Hand zu blocken\n",
	L"+%d%s Chance, Stichwaffenangriffen auszuweichen\n",
	L"Kann angeschlagenen Gegnern einen Tornadotritt verpassen, der doppelten Schaden anrichtet\n",
	L"Sie erhalten besondere Animationen für den Faustkampf (etwas fernöstlicher)\n",
	L"Keine Boni",
};

STR16 gzIMPNewCharacterTraitsHelpTexts[]=
{
	L"V: Keine Vorteile.\nN: Keine Nachteile.",
	L"V: Ist leistungsfähiger, wenn andere Söldner in der Nähe sind.\nN: Erhält keinen Moralzuwachs, wenn niemand in der Nähe ist.",
	L"V: Ist leistungsfähiger, wenn niemand in der Nähe ist.\nN: Erhält keinen Moralzuwachs, wenn andere Söldner in der Nähe sind.",
	L"V: Die Moral sinkt etwas langsamer und steigt schneller als normal.\nN: Hat eine geringere Chance, Fallen und Minen zu entdecken.",
	L"V: Erhält Boni beim Ausbilden von Milizen und kann besser mit Menschen reden.\nN: Erhält keinen Moralzuwachs für Aktionen anderer Söldner.",
	L"V: Lernt durch eigenes Training und Ausbildung etwas schneller.\nN: Hat weniger Unterdrückungs- und Angstresistenz.",
	L"V: Verbraucht etwas weniger Energie, außer bei medizinischen und technischen Aufgaben sowie einer anspruchsvollen Ausbildung.\nN: Weisheit, Führungskraft, Sprengstoff-, Mechanik- und Medizinkenntnisse entwickeln sich langsamer.",
	L"V: Hat eine leicht erhöhte Trefferchance bei Feuerstößen und richtet etwas mehr Schaden im Nahkampf an.\n   Erhält ein wenig mehr Moralzuwachs beim Töten.\nN: Erledigt Aufgaben, die Geduld erfordern schlechter (Reparatur, Schlösser knacken, Fallen entschärfen, Patientenversorgung und Milizausbildung).",
	L"V: Erhält Boni für Aufgaben, die Geduld erfordern (Reparatur, Schlösser knacken, Fallen entschärfen, Patientenversorgung und Milizausbildung).\nN: Erhält seltener Unterbrechungen im Kampf.",
	L"V: Erhöhte Resistenz gegenüber Unterdrückungsfeuer und Panikanfällen.\nVerliert weniger Moral durch Verwundung oder Todesfälle im Team.\nN: Wird leichter getroffen, und kann seltener Feindfeuer durch schnelle Bewegung ausweichen.",
	L"V: Erhält Moralzuwachs für nichtkämpferische Tätigkeiten (Außnahme: Milizausbildung).\nN: Erhält keinen Moralzuwachs für das Töten von Gegnern.",
	L"V: Hat eine höhere Chance, Statusschäden anzurichten, und kann besonders fiese Wunden verursachen.\n   Erhält mehr Moral für erfolgreiche Statusschäden.\nN: Kann schlechter mit Leuten reden. Die Moral sinkt ohne Kampf schneller.",
	L"V: Ist leistungsfähiger, wenn Söldner des anderen Geschlechts in der Nähe sind.\nN: Die Moral anderer naher Söldner des gleichen Geschlechts steigt langsamer.",
	L"A: Gains morale when retreating.\nD: Loses morale when encountering numerically superior enemy forces.", // TODO.Translate
};

STR16 gzIMPDisabilitiesHelpTexts[]=
{
	L"Keine besonderen Einschränkungen.",
	L"Leidet an Atemproblemen und zeigt allgemein schlechtere Leistung in tropischen und Wüstensektoren.",
	L"Erleidet allein in schwierigen Situationen Panikattacken.",
	L"Zeigt schlechtere Leistung unter Tage.",
	L"Kann bei Schwimmversuchen leicht ertrinken.",
	L"Erträgt den Anblick großer Insekten nicht und\nzeigt etwas schlechtere Leistung in Wüsten- oder tropischen oder Sektoren.",
	L"Vergisst manchmal seine Befehle und verliert dadurch im Kampf einen Teil seiner APs.",
	L"Dreht manchmal durch und schießt dabei hin und wieder wild um sich.\nVerliert Moral, wenn das mit der ausgerüsteten Waffe nicht möglich ist.",
	L"Deutlich geringere Hörweite.",
	L"Geringere Sichtweite.",
	L"Drastically increased bleeding.",	// TODO.Translate
	L"Performance suffers while on a rooftop.",	// TODO.Translate
	L"Occasionally harms self.",
};

STR16 gzIMPProfileCostText[]=
{
	L"Ein Profil kostet $%d. Genehmigen Sie die Zahlung?",
};

STR16 zGioNewTraitsImpossibleText[]=
{
	L"Sie können das neue Fertigkeitensystem nicht ohne aktiviertem PROFEX-Utility benutzen. Suchen Sie in Ihrer ja2_options.ini den Eintrag: READ_PROFILE_DATA_FROM_XML.",
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Sie haben sich für den IRONMAN-Modus entschieden. Mit dieser Einstellung können Sie das Spiel nicht speichern, wenn Feinde im Sektor sind. Sind Sie sicher, dass Sie im IRONMAN-Modus spielen wollen?",
	L"Sie haben sich für den SOFT IRONMAN-Modus entschieden. Mit dieser Einstellung können Sie das Spiel nicht speichern, wenn der Rundenmodus im taktischen Kampf aktiv ist. Sind Sie sicher, dass Sie im SOFT IRONMAN-Modus spielen wollen?",
	L"Sie haben sich für den EXTREME IRONMAN-Modus entschieden. Mit dieser Einstellung können Sie das Spiel nur einmal am Tag speichern, um %02d:00. Sind Sie sicher, dass Sie im EXTREME IRONMAN-Modus spielen wollen?",
};

STR16 gzDisplayCoverText[]=
{
	L"Deckung: %d/100 %s, Helligkeit: %d/100",
	L"Waffen-Reichweite: %d/%d Felder, Trefferwahrscheinlichkeit: %d/100",
	L"Waffen-Reichweite: %d/%d Felder, Lauf-Genauigkeit: %d/100",
	L"Deckungsanzeige ausschalten",
	L"Zeige Ansicht für Söldner",
	L"Zeige Gefahrenbereich für Söldner",
	L"Wald", // wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Stadt",
	L"Wüste",
	L"Schnee", // NOT USED!!!
	L"Wald und Wüste",
	L"Wald und Stadt",
	L"Wald und Schnee",
	L"Wüste und Stadt",
	L"Wüste und Schnee",
	L"Stadt und Schnee",
	L"-", // yes empty for now
	L"Deckung: %d/100, Helligkeit: %d/100",			
	L"Footstep volume",				// TODO.Translate			
	L"Stealth difficulty",
	L"Trap level",
};


#endif
