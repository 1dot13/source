*************************************************
* Deadly Games German Translated Mercs für 1.13 *
********************by firebat*******************

Dank RoWa21 ist dieses Projekt ein fester Bestandteil des JA2 1.13 (DE) Patches.



Status:
-------
Abgeschlossen!



Bekannte Probleme:
------------------
Problem: 	Mikes Stimmenwechsel von NPC zu PC	
Lösung: 	Mike nicht anheuern.

Problem: 	Stimmenwechsel bei Speck als NPC und PC
Lösung: 	Keine. Stimmenwechsel wurde bereits auf ein Minimum reduziert.

Problem: 	Stimmenwechsel bei Tex
Lösung: 	Keine. Doch sind nur zwei Sätze betroffen und der Unterschied kaum hörbar.

Problem: 	Stimmwechsel bei Buzz (wenn als Spitzel)
Lösung: 	Das Spitzelverhalten von Buzz deaktivieren. Suche in JA2/Data-1.13/Tabledata/mercprofiles.xml nach <uiIndex>23</uiIndex>
			Ersetze in Buzzs Tabellenbaum die Zeile <bNewSkillTrait4>22</bNewSkillTrait4> mit <bNewSkillTrait4>0</bNewSkillTrait4>

Problem: 	Stimmenwechsel bei jeweils Hitman, Len, Magic, Scully, Larry, Vicki, Fidel, Fox, Cliff, Wolf, Biff (nur Beziehungsverhalten)
Lösung: 	Deaktivieren der Freund- Feindbeziehung zwischen o.g. JA2 zu DG Söldnern; via mercprofiles.xml
			Für Anleitung siehe Link: https://www.dropbox.com/s/j288c8l7a5hin7l/mercprofiles-disabled.txt?dl=0



Anmerkung:
----------
Für Feedback sowie Fehlerberichtserstattung, schreib mir eine Mitteilung über das Bear's Pit Forum.
http://thepit.ja-galaxy-forum.com/index.php?t=usrinfo&id=12495&



Changelog:
----------
2016-08-29:
- Diese ReadMe in das Deutsche übersetzt
- updates für viele Söldner


2016-07-07:
- alle: Wassereffekte für ertrinkende Söldner hinzugefügt
- updates für Boss
- updates für Samuel
- updates für Gary
- updates für Moses
- updates für Needle
- updates für Juan
- updates für Wahan
- updates für Weasel
- updates für Speck
- updates für Skitz
- updates für Wally
- updates für Carp
- updates für Dyno
- updates für Leon


2016-06-22:
- updates für Wally
- updates für Eli
- updates für Postie
- updates für Bernie
- updates für Dyno
- updates für Screw
- updates für Sparky
- updates für Pops
- updates für Skitz
- updates für Vinny
- updates für Ears
- updates für Doc
- updates für Juan
- updates für Speck
- updates für Mike



Genutzte Tools:
---------------
u.a.
JA1 DG Speech File Extractor
EDT Mega Editor v2 von Bugmonster
Gap-file generator von Trailblazer
Audacity