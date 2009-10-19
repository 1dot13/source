#pragma setlocale("ITALIAN")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25Italiantext.h"
#else
	#include "Language Defines.h"
	#ifdef ITALIAN
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_Ja25ItalianText_public_symbol(void){;}

#ifdef ITALIAN

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	//L"Distanza dal bersaglio: %d caselle, Luminosità: %d/%d",
	L"Colleghi il trasmettitore al tuo computer portatile.",
	L"Non puoi permetterti di ingaggiare %s",
	L"Per un periodo limitato, la tariffa qui sopra includerà i costi dell'intera missione, oltre all'equipaggiamento indicato sotto.",
	L"Assolda %s adesso e approfitta della nostra nuova tariffa 'tutto incluso'. Compreso in questa incredibile offerta anche l'equipaggiamento personale del mercenario, senza alcun costo aggiuntivo.",
	L"Tariffa",
	L"C'è qualcun altro nel settore...",
	//L"Gittata dell'arma: %d caselle, Probabilità di colpire: %d percent",
	L"Mostra nascondigli",
	L"Linea di Vista",
	L"Le nuove reclute non possono arrivare qui.",
	L"Poiché il tuo portatile non ha un trasmettitore, non potrai assoldare nuovi mercenari. Forse questo sarebbe un buon momento per caricare una partita salvata o ricominciare daccapo!",
	L"%s sente venire da sotto al corpo di Jerry il rumore di metallo che si accartoccia. E' un suono fastidioso, come se l'antenna del tuo portatile venisse schiacciata.",  //the %s is the name of a merc.
	L"Dopo aver dato un'occhiata al biglietto lasciato dal Vice Comandante Morris, %s vede una possibilità. La nota contiene le coordinate per il lancio di missili contro diverse città ad Arulco. C'è anche la locazione della base di lancio: la fabbrica di missili.",
	L"Guardando il pannello di controllo %s immagina che i numeri possano essere invertiti, cosicché il missile distrugga proprio questa fabbrica. %s deve trovare una via di fuga. L'ascensore sembra offrire la soluzione più rapida...",
	L"Questa è una partita a livello IRON MAN: non puoi salvare quando ci sono nemici nei dintorni.",
	L"(Non puoi salvare durante il combattimento.)",
	L"Il nome della campagna è più grande di 30 caratteri",
	L"Campagna non trovata.",
	L"Campagna: Default ( %S )",
	L"Campagna: %S",
	L"Hai selezionato la campagna %S. Questa campagna è una versione amatoriale della campagna originale di Unfinished Business. Sei sicuro di voler giocare la campagna %S?",
	L"Per usare l'editor, selezionare una campagna diversa da quella di default.",
};

//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"Scassinare",
	L"Lottare",
	L"Elettronica",
	L"Operazioni Notturne",
	L"Lanciare",
	L"Insegnare",
	L"Armi Pesanti",
	L"Armi Automatiche",
	L"Muoversi Silenziosamente",
	L"Ambidestro",
	L"Combattimento con il coltello",
	L"Cecchino",
	L"Mimetismo",
	L"Arti Marziali",

	L"Nessuna",
	L"Specialità I.M.P.",
};
//New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Hai scelto la modalità IRON MAN. Questa impostazione rende il gioco notevolmente più impegnativo, poiché non potrai salvare la partita in un settore occupato da nemici. Non potrai cambiare questa decisione nel corso della partita. Sei sicuro di voler giocare al livello IRON MAN?",
};

STR16 gzDisplayCoverText[]=
{
	L"Cover: %d/100 %s, Brightness: %d/100",
	L"Gun Range: %d/%d tiles, Chance to hit: %d/100",
	L"Disabling Cover Display",
	L"Showing Mercenary View",
	L"Showing Cover for Mercenary",
	L"Wood", // wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Urban",
	L"Desert",
	L"Snow", // NOT USED!!!
	L"Wood and Desert",
	L"" // yes empty for now
};


#endif
