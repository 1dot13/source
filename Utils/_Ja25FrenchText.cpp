#pragma setlocale("FRENCH")
#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "_Ja25Frenchtext.h"
#else
	#include "Language Defines.h"
	#ifdef FRENCH
		#include "text.h"
		#include "Fileman.h"
	#endif
#endif

//suppress : warning LNK4221: no public symbols found; archive member will be inaccessible
void this_is_the_Ja25FrenchText_public_symbol(void){;}

#ifdef FRENCH

// VERY TRUNCATED FILE COPIED FROM JA2.5 FOR ITS FEATURES FOR JA2 GOLD

STR16	zNewTacticalMessages[]=
{
	//L"Distance cible: %d tiles, Brightness: %d/%d",
	L"Attache le transpondeur à votre PC portable.",
	L"Vous n'avez pas les moyens d'engager %s",
	L"Pour une durée limitée, les frais ci-dessus couvrent la mission entière, équipement ci-dessous compris.",
	L"Engagez %s et découvrez dès à présent notre prix \"tout compris\".  Aussi inclus dans cette incroyable offerte l'équipement personnel du mercenaire sans frais supplémentaires.",
	L"Frais",
	L"Il y a quelqu'un d'autre dans le secteur...",
	//L"Portée arme: %d tiles, Probabilités: %d pourcent",
	L"Afficher couverture",
	L"Ligne de vision",
	L"Les nouvelles recrues ne peuvent arriver ici.",
	L"Comme votre PC n'a pas de transpondeur, vous ne pouvez engager de nouvelles recrues. Revenez à une sauvegarde précédente et réessayez.",
	L"%s entend le son de métal broyé provenant d'en dessous du corps de Jerry.  On dirait que l'antenne de votre PC ne sers plus à rien.",  //the %s is the name of a merc.  @@@  Modified
	L"Apres avoir scanné la note laissée par le Deputy Commander Morris, %s sent une oppurtinité. La note contient les coordonnées pour le lancement de missiles sur Arulco. Elle contient aussi l'emplacement de l'usine d'où les missiles proviennent.",
	L"En examinant le panneau de contrôle, %s s'aperçoît que less chiffres peuvent être inversés, pour que les missiles détruisent cette même usine.  %s a besoin de trouver un chemin pour s'enfuir.  L'ascenseur semble être la solution la plus rapide...",
	L"Ceci est un jeu IRON MAN et vous ne pouvez sauvegarder s'il ya des ennemis dans les parages.",	//	@@@  new text
	L"(ne peut sauvegarder en plein combat)", //@@@@ new text
	L"Le nom de la campagne actuelle est supérieur à 30 lettres.",							// @@@ new text
	L"La campagne actuelle est introuvable.",																	// @@@ new text
	L"Campaign: Defaut ( %S )",																							// @@@ new text
	L"Campaign: %S",																													// @@@ new text
	L"Vous avez choisi la campagne %S. Cette campagne est un mod d'unfinished business. Etes vous sûr de vouloir jouer la campagne %S ?",			// @@@ new text
	L"Pour pouvoir utiliser l'éditeur, veuillez choisir une autre campagne que celle par defaut.",		///@@new
};

//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"crochetage",
	L"combat à mains nues",
	L"Electronique",
	L"Operations de nuit",
	L"Lancer",
	L"Enseigner",
	L"Armes lourdes",
	L"Armes automatiques",
	L"Discretion",
	L"Ambidextre",
	L"Couteau",
	L"Tireur isolé",
	L"Camouflage",
	L"Arts martiaux",

	L"aucune",
	L"Spécialtés I.M.P.",
};
//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Vous avez choisi le mode IRON MAN. La difficultée du jeu s'en trouveras considerablement augmentée du fait de l'imposssiblité de sauvegarder en territoire ennemi. Ce paramètre prendra effet tout au long de la partie.  Etes vous vraiment sûr de vouloir jouer en mode IRON MAN ?",
};

STR16 gzDisplayCoverText[]=
{
	L"Cover: %d/100 %s, Brightness: %d/100",
	L"Gun Range: %d/%d tiles, Chance to hit: %d/100",
	L"Disabling cover display",
	L"Showing mercenary view",
	L"Showing danger zones for mercenary",
	L"Wood", // wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Urban",
	L"Desert",
	L"Snow", // NOT USED!!!
	L"Wood and Desert",
	L"" // yes empty for now
};


#endif
