// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("FRENCH")

#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
#else
	#include "Language Defines.h"
	#ifdef FRENCH
		#include "text.h"
		#include "Fileman.h"
		#include "Scheduling.h"
		#include "EditorMercs.h"
		#include "Item Statistics.h"
		#include "Encyclopedia.h"
	#endif
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
	L"Locations", // 0
	L"Characters",
	L"Items",
	L"Quests",
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
	L"Exit",
};

STR16	pOtherButtonsText[] =
{
	L"Briefing", 
	L"Accept",
};

STR16	pOtherButtonsHelpText[] =
{
	L"Briefing",
	L"Accept missions",
};


STR16	pLocationPageText[] =
{
	L"Prev page",
	L"Photo", 
	L"Next page",
};

STR16	pSectorPageText[] =
{
	L"<<",
	L"Main page",
	L">>",
	L"Type: ",
	L"Empty data",
	L"Missing of defined missions. Add missions to the file TableData\\BriefingRoom\\BriefingRoom.xml. First mission has to be visible. Put value Hidden = 0.",
};

STR16	pEncyclopediaTypeText[] = 
{
	L"Unknown",// 0 - unknown
	L"City", //1 - cities
	L"SAM Site", //2 - SAM Site
	L"Other location", //3 - other location
	L"Mines", //4 - mines 
	L"Military complex", //5 - military complex 
	L"Laboratory complex",  //6 - laboratory complex 
	L"Factory complex", //7 - factory complex 
	L"Hospital", //8 - hospital 
	L"Prison", //9 - prison
    L"Airport", //10 - air port 
};

STR16	pEncyclopediaHelpCharacterText[] = 
{
	L"Show all",
	L"Show AIM",
	L"Show MERC",
	L"Show RPC",
	L"Show NPC",
	L"Show Pojazd",
	L"Show IMP",
	L"Show EPC",
	L"Filter",
};

STR16	pEncyclopediaShortCharacterText[] = 
{
	L"All",
	L"AIM",
	L"MERC",
	L"RPC",
	L"NPC",
	L"Veh.",
	L"IMP",
	L"EPC",
	L"Filter",
};

STR16	pEncyclopediaHelpText[] = 
{
	L"Show all",
	L"Show cities",
	L"Show SAM Sites",
	L"Show other location",
	L"Show mines",
	L"Show military complex",
	L"Show laboratory complex",
	L"Show Factory complex",
	L"Show hospital",
	L"Show prison",
	L"Show air port",
};

STR16	pEncyclopediaSkrotyText[] = 
{
	L"All",
	L"City",
	L"SAM",
	L"Other",
	L"Mine",
	L"Mil.",
	L"Lab.",
	L"Fact.",
	L"Hosp.",
	L"Prison",
	L"Air.",
};

STR16	pEncyclopediaShortInventoryText[] = 
{
	L"All", //0
	L"Gun",
	L"Ammo",
	L"LBE",
	L"Misc",
	
	L"All", //5
	L"Gun",
	L"Ammo",
	L"LBE Gear",
	L"Misc",
};

STR16			BoxFilter[] =
{
	// Guns
	L"Heavy",
	L"Pistol",
	L"M. Pist.",
	L"SMG",
	L"Rifle",
	L"S. Rifle",
	L"A. Rifle",
	L"MG",
	L"Shotgun",

	// Ammo
	L"Pistol",
	L"M. Pist.", //10
	L"SMG",
	L"Rifle",
	L"S. Rifle",
	L"A. Rifle",
	L"MG",
	L"Shotgun",

	// Used
	L"Guns",
	L"Armor",
	L"LBE Gear",
	L"Misc", //20

	// Armour
	L"Helmets",
	L"Vests",
	L"Leggings",
	L"Plates",

	// Misc
	L"Blades",
	L"Th. Knife",
	L"Melee",
	L"Grenades",
	L"Bombs",
	L"Med.", //30
	L"Kits",
	L"Face",
	L"LBE",
	L"Misc.", //34
};
//-----------

// Editor
//Editor Taskbar Creation.cpp
STR16 iEditorItemStatsButtonsText[] =
{
	L"Supprimer",
};

STR16 FaceDirs[8] = 
{ 
	L"NORD",
	L"NORD-est",
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
 
 L"PLACEMENT DETAILLE", //10
 L"Information général mode",
 L"Apparence physique mode",
 L"Attribut mode",
 L"Inventaire mode",
 L"ID profile mode",
 L"Calendrier mode",
 L"Calendrier mode",
 L"SUPPRIMER",
 L"Supprimer le mercenaire sélectionné (SUPPR).",
 L"SUIVANT", //20
 L"Mercenaire suivant (ESPACE).",
 L"Changer l'existance prioritaire",
 L"Changer si le placement a/naccès à toutes les portes.",
 
 //Orders
 L"STATIONNAIRE",
 L"EN DEFENSE",
 L"AU TELEPHONE",
 L"CHERCHER LES ENNEMIS",
 L"PATROUILLE RAPPROCHE",
 L"PATROUILLE ELOIGNE",
 L"POINT DE RASSEMBLEMENT", //30
 L"TOUR DE PATROUILLE",
 
 //Attitudes
 L"DEFENSE",
 L"SOLO BRAVE",
 L"SOUTIEN BRAVE",
 L"AGGRESIF",
 L"SOLO RUSE",
 L"SOUTIEN RUSE",
 
 L"Sélectionné le mercenaire pour faire face à %s",
 
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
	L"INFO PIECE",

	L"Placer les murs en utilisant la méthode de sélection",
	L"Placer les portes en utilisant la méthode de sélection",
	L"Placer les toits en utilisant la méthode de sélection",
	L"Placer les fenêtres en utilisant la méthode de sélection",
	L"Placer les murs endommagés en utilisant la méthode de sélection.",
	L"Placer les meubles en utilisant la méthode de sélection",
	L"Placer les décalcomanies murales en utilisant la méthode de sélection",
	L"Placer les étages en utilisant la méthode de sélection", //10
	L"Placer les meubles génériques en utilisant la méthode de sélection",
	L"Placer les murs avec la méthode courte",
	L"Placer les portes avec la méthode courte",
	L"Placer les fenêtres avec la méthode courte",
	L"Placer les murs endommagés avec la méthode courte",
	L"Vérouiller ou piéger les porte existantes",

	L"Ajouter une nouvelle salle",
	L"Editer les mûrs de caverne.",
	L"Enlevez un secteur de la construction existante.",
	L"Enlevez une construction", //20
	L"Ajoutez/remplacez le toit de la construction par un nouveau toit plat.",
	L"Copier une construction",
	L"Bouger une construction",
	L"Dessiner le numéro de pièce",
	L"Supprimer le numéro de pièce",

	L"Activer le mode supprimer",
	L"Effacer le dernier changement",
	L"Taille du cycle",

};

STR16 iEditorItemsToolbarText[] =
{
	L"Arm.", //0
	L"Munitions",
	L"Armures",
	L"LBE",
	L"Exp",
	L"E1",
	L"E2",
	L"E3",
	L"Détentes",
	L"Clés",
	L"Rnd",	// TODO.Translate
};

STR16 iEditorMapInfoToolbarText[] =
{
	L"Ajouter une source de lumière ambiante", //0
	L"Basculer en fausse lumière ambiante.",
	L"Ajouter des réseaux de sortie (clic droit pour une requête existante).",
	L"Taille de cycle",
	L"Effacer le dernier changement",
	L"Basculer en mode supprimer",
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
	L"Sauvegarder la carte",
	L"Charger la carte",
	L"Séléctionner un tileset",
	L"Quitter le mode éditeur",
	L"Quitter le jeu.",
	L"Créer un carte de radar",
	L"La carte vérifiée, elle sera sauvée sur le format original JA2.\nCette option est seulement valable sur les cartes de taille 'normales' qui ne font pas référence aux nombres de réseau (ex: réseau de sortie) > 25600.",
	L"La carte vérifiée et chargée, elle sera élargie automatiquement selon les rangées et colonnes choisies.",
};

STR16 iEditorTerrainToolbarText[] =
{
	L"Dessiner des textures de sol", //0
	L"Sélectionner les textures du sol de la carte",
	L"Placer les rives et falaises",
	L"Dessiner les routes",
	L"Déssiner les débris",
	L"Placer les arbres & buissons",
	L"Placer les rochers",
	L"Placer barrils & autres camelotes",
	L"Remplisser le secteur",
	L"Effacer le dernier changement",
	L"Basculer en mode supprimer", //10
	L"Taille du cycle",
	L"Augmenter la densité de la brosse",
	L"Diminuer la densité de la brosse",
};

STR16 iEditorTaskbarInternalText[]=
{
	L"Terrain", //0
	L"Bâtiment",
	L"Objets",
	L"Mercenaires",
	L"Map Info",
	L"Options",
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
	L"Piège de level %d",
};

STR16 iRenderEditorInfoText[]=
{
	L"Enregistrer la map au format vanilla JA2 (v1.12) (Version: 5.00 / 25)", //0
	L"Aucune carte n'est actuellement chargée.",
	L"Fichier:  %S, Tileset actuel:  %s",
	L"Elargir la map au chargement",
};
//EditorBuildings.cpp
STR16 iUpdateBuildingsInfoText[] =
{
	L"BASCULER", //0
	L"VUES",
	L"MODE DE SELECTION",
	L"METHODE COURTE",
	L"MODE DE CONSTRUCTION",
	L"Pièce #", //5
};

STR16 iRenderDoorEditingWindowText[] =
{
	L"Modification des attributs de verrouillage à l'index %d de la carte.",
	L"Vérrouillage ID",
	L"Type de piège",
	L"Niveau du piège",
	L"Vérrouillé",
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
	L"24 Hrs", //5

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
	L"Porte vérrouillé",
	L"Porte dévérouillé",
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
	L"Sur Appel",
	L"Chercher ennemi", 
	L"Point de patrouille aléatoire",
};

STR16 EditMercAttitudes[6] = 
{ 
	L"Defensif",
	L"Solitaire courageux",
	L"Brave copain",
	L"Solitaire Rusé",
	L"Copain Rusé",
	L"Aggressif", 
};

STR16 pDisplayEditMercWindowText[] =
{
	L"Nom mercenaire :", //0
	L"Ordres:",
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
	L"Reg Male",
	L"Big Male",
	L"Stocky Male",
	L"Reg Female",
	L"NE Tank", //5
	L"NW Tank",
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
	L"Unarmed Robot",
	L"Larvae", //20
	L"Infant",
	L"Yng F Monster",
	L"Yng M Monster",
	L"Adt F Monster",
	L"Adt M Monster", //25
	L"Queen Monster",
	L"Bloodcat",
};

STR16 pUpdateMercsInfoText[] = 
{
	L" --=ORDRES=-- ", //0
	L"--=ATTITUDE=--",
	
	L"ATTRIBUS",
	L"RELATIF",
	
	L"EQUIPEMENT",
	L"RELATIF",
			
	L"ATTRIBUS",
	L"RELATIF",
	
	L"Armée",
	L"Admin",
	L"Elite", //10
	
	L"Niveau d'exp.",
	L"Sante",
	L"Sante Max.",
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
	
	L"ALEATOIRE",
	L"ALEATOIRE",
	L"ALEATOIRE", //30
	L"ALEATOIRE",
	
	L"En spécifiant un indice de profil, toutes les informations seront extraites du profil ",
	L"et remplacer par les valeurs que vous avez édité. Les fonctions d'édition seront aussi désactivé ",
	L"bien, que vous serez toujours en mesure de regarder les stats, etc. En appuyant sur ENTREE cela extraira ",
	L"automatiquement le numéro que vous avez tapé.  Un champ vide effacera le profil. Le nombre",
	L"actuel de profil allant de 0 à ",
	
	L"Profil actuel:  n/a              ",
	L"Profil actuel : %s",
	
	L"STATIONNAIRE",
	L"EN APPEL", //40
	L"MIS EN GARDE",
	L"CHERCHEZ ENNEMIE",
	L"PATROUILLE PROCHE",
	L"PATROUILLE LONTAINE",
	L"PATROUILLE DE POINT",
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
	
	L"vérouillé",
	L"dévérouillé",
	L"ouvert",
	L"fermer",
	
	L"Cliquez sur la grille no adjacente à la porte que vous souhaitez %s.", //60
	L"Cliquez sur la grille no où vous voulez aller avoir %s la porte.",
	L"Cliquez sur la grille no où vous voulez aller.",
	L"Cliquez sur la grille no où vous voulez dormir.	La personne retournera automatiquement à sa position initiale lorsqu'elle se réveillera.",
	L" Cliquez sur la ESC pour annuler l'entrée de cette ligne dans le calendrier.",
};

CHAR16 pRenderMercStringsText[][100] =
{
	L"Slot #%d",
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
	L"Etes-vous sur de vouloir retirer toute les lumières ?", //0
	L"Etes-vous sûr de vouloir renverser le calendrier ?",
	L"Etes-vous sûr de vouloir effacer tous les horaires ?",
	
	L"Cliquage de placement activé",
	L"Cliquage de placement désactivé",
	
	L"Dessin des hauteur activé", //5
	L"Dessin des hauteur désactivé",
	
	L"Nombre de points de bord: N=%d E=%d S=%d O=%d",
	
	L"Placement aléatoire activé",
	L"Placement aléatoire désactivé",
	
	L"Retirer la cîme des arbres", //10
	L"Montrer la cîme des arbres",
	
	L"Réinitialisation de l'aggrandissement de la carte",
	
	L"Ancienne méthode d'aggrandissement",
	L"Aggrandissment fait",
};

STR16 pPerformSelectedActionText[] = 
{
	L"Création du carte radar pour %S", //0
	
	L"Supprimer la carte actuelle et commencer un nouveau niveau de sous-sol ?",
	L"Supprimer la carte actuelle et commencer un nouveau niveau de cave ?",
	L"Supprimer la carte actuelle et commencer un nouveau niveau extérieur ?",
	
	L" Essuyer les textures du sol ? ",
};

STR16 pWaitForHelpScreenResponseText[] = 
{
	L"HOME", //0
	L"Activer l'éditeur de faux éclairage ON/OFF",

	L"INSER",
	L"Activer le mode de remplissage ON/OFF",

	L"BKSPC",
	L"Annuler la dernière modification",

	L"DEL",
	L"Effacement rapide d'objet sous le curseur de la souris",

	L"ESC",
	L"Quitter l'éditeur",

	L"PGUP/PGDN", //10
	L"Changement d'objets qui doivent être collé",

	L"F1",
	L"Ecran d'aide",

	L"F10",
	L"Sauvegarder la carte actuelle",

	L"F11",
	L"Charger la carte",

	L"+/-",
	L"Changement l'obscurité de .01",

	L"SHFT +/-",  //20
	L"Changement l'obscurité de .05",

	L"0 - 9",
	L"Changer le nom de carte/tileset",

	L"b",
	L"Changer la taille du pinceau",

	L"d",
	L"Dessinez des débris",

	L"o",
	L"Dessinez des obstacles",

	L"r", //30
	L"Dessinez des rochers",

	L"t",
	L"Acriver l'affichage des arbres ON/OFF",

	L"g",
	L"Dessinez les textures du sol",

	L"w",
	L"Dessinez les murs des bâtiments",

	L"e",
	L"Activer le mode effacer ON/OFF",

	L"h",  //40
	L"Activer les toits ON/OFF",
};

STR16 pAutoLoadMapText[] =
{
	L"La carte vient d'être corrompu. N'enregistrer pas, ne quittez pas, demander à parler à Kris ! Si il n'est pas là, sauver la carte en utilisant un nom de fichier temporaire et documenter tout ce que vous venez de faire, surtout la dernière action !",
	L"Le calendrier vient d'être corrompu.  N'enregistrer pas, ne quittez pas, demander à parler à Kris ! Si il n'est pas là, sauver la carte en utilisant un nom de fichier temporaire et documenter tout ce que vous venez de faire, surtout la dernière action !",
};

STR16 pShowHighGroundText[] =
{
	L"Afficher les marqueurs des textures de sol",
	L"Cacher les marqueurs des textures de sol",
};

//Item Statistics.cpp
CHAR16 gszActionItemDesc[ 34 ][ 30 ] =	// NUM_ACTIONITEMS = 34
{
	L"Klaxon Mine",
	L"Flare Mine",
	L"Teargas Explosion",
	L"Stun Explosion",
	L"Smoke Explosion",
	L"Mustard Gas",
	L"Land Mine",
	L"Open Door",
	L"Close Door",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Small Explosion",
	L"Medium Explosion",
	L"Large Explosion",
	L"Toggle Door",
	L"Toggle Action1s",
	L"Toggle Action2s",
	L"Toggle Action3s",
	L"Toggle Action4s",
	L"Enter Brothel",
	L"Exit Brothel",
	L"Kingpin Alarm",
	L"Sex with Prostitute",
	L"Reveal Room",
	L"Local Alarm",
	L"Global Alarm",
	L"Klaxon Sound",
	L"Unlock door",
	L"Toggle lock",
	L"Untrap door",
	L"Tog pressure items",
	L"Museum alarm",
	L"Bloodcat alarm",
	L"Big teargas",
};

STR16 pUpdateItemStatsPanelText[] =
{
	L"Toggle hide flag", //0
	L"No item selected.",
	L"Slot available for",
	L"random generation.",
	L"Keys not editable.",
	L"ProfileID of owner",
	L"Item class not implemented.",
	L"Slot locked as empty.",
	L"Status",
	L"Rounds",
	L"Trap Level", //10
	L"Quantity",
	L"Trap Level",
	L"Status",
	L"Trap Level",
	L"Status",
	L"Quantity",
	L"Trap Level",
	L"Dollars",
	L"Status",
	L"Trap Level", //20
	L"Trap Level",
	L"Tolerance",
	L"Alarm Trigger",
	L"Exist Chance",
	L"B",
	L"R",
	L"S",
};

STR16 pSetupGameTypeFlagsText[] =
{
	L"Item appears in both Sci-Fi and Realistic modes. (|B)", //0
	L"Item appears in |Realistic mode only.",
	L"Item appears in |Sci-Fi mode only.",
};

STR16 pSetupGunGUIText[] =
{
	L"SILENCER", //0
	L"SNIPERSCOPE",
	L"LASERSCOPE",
	L"BIPOD",
	L"DUCKBILL",
	L"G-LAUNCHER", //5
};

STR16 pSetupArmourGUIText[] =
{
	L"CERAMIC PLATES", //0
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
	L"Okay", //0
	L"A",
	L"G",
	L"B1",
	L"B2",
	L"B3", //5
	L"CHARGER",
	L"SAUVER",
	L"MISE A JOUR",
};

STR16 pRenderSectorInformationText[] =
{
	L"Tileset:  %s", //0
	L"Info. version:  Résumé:  1.%02d,  Carte:  %1.2f / %02d",
	L"Nombre d'objets:  %d", 
	L"Nombre de lumière:  %d",
	L"Nombre de points d'entré:  %d",
	
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
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Troops:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Elites:  %d",
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	L"Civilians:  %d",  //20
	
	L"(%d detailed, %d profile -- %d have priority existance)",
	
	L"Humans:  %d",
	L"Cows:  %d",
	L"Bloodcats:  %d",
	
	L"Creatures:  %d",
	
	L"Monsters:  %d",
	L"Bloodcats:  %d",
	
	L"Number of locked and/or trapped doors:  %d",
	L"Locked:  %d",
	L"Trapped:  %d", //30
	L"Locked & Trapped:  %d",
	
	L"Civilians with schedules:  %d",
	
	L"Too many exit grid destinations (more than 4)...",
	L"ExitGrids:  %d (%d with a long distance destination)",
	L"ExitGrids:  none",
	L"ExitGrids:  1 destination using %d exitgrids",
	L"ExitGrids:  2 -- 1) Qty: %d, 2) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d",
	L"ExitGrids:  3 -- 1) Qty: %d, 2) Qty: %d, 3) Qty: %d, 4) Qty: %d",
	L"Enemy Relative Attributes:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)", //40
	L"Enemy Relative Equipment:  %d bad, %d poor, %d norm, %d good, %d great (%+d Overall)",
	L"%d placements have patrol orders without any waypoints defined.",
	L"%d placements have waypoints, but without any patrol orders.",
	L"%d gridnos have questionable room numbers.  Please validate.",
	
};

STR16 pRenderItemDetailsText[] =
{
	L"R",  //0
	L"S",
	L"Enemy",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"Panic1",
	L"Panic2",
	L"Panic3",
	L"Norm1",
	L"Norm2",
	L"Norm3",
	L"Norm4", //10
	L"Pressure Actions",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	
	L"PRIORITY ENEMY DROPPED ITEMS",
	L"None",
	
	L"TOO MANY ITEMS TO DISPLAY!",
	L"NORMAL ENEMY DROPPED ITEMS",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"None",
	L"TOO MANY ITEMS TO DISPLAY!",
	L"ERROR:  Can't load the items for this map.  Reason unknown.", //20	
};

STR16 pRenderSummaryWindowText[] =
{
	L"CAMPAIGN EDITOR -- %s Version 1.%02d", //0
	L"(NO MAP LOADED).",
	L"You currently have %d outdated maps.",
	L"The more maps that need to be updated, the longer it takes.  It'll take ",
	L"approximately 4 minutes on a P200MMX to analyse 100 maps, so",
	L"depending on your computer, it may vary.",
	L"Do you wish to regenerate info for ALL these maps at this time (y/n)?",
	
	L"There is no sector currently selected.",
	
	L"Entering a temp file name that doesn't follow campaign editor conventions...",
	
	L"You need to either load an existing map or create a new map before being",
	L"able to enter the editor, or you can quit (ESC or Alt+x).", //10

	L", ground level",					
	L", underground level 1",	
	L", underground level 2",	
	L", underground level 3",	
	L", alternate G level",					
	L", alternate B1 level",	
	L", alternate B2 level",	
	L", alternate B3 level",
	
	L"DETAILS OBJETS -- secteur %s",
	L"Les informations sommaires pour le secteur %s :", //20
	
	L"Les informations sommaires pour le secteur %s",
	L"n'existent pas.",
	
	L"Les informations sommaires pour le secteur %s",
	L"n'existent pas.",
	
	L"Pas d'informations existantes pour le secteur %s.",
	
	L"Pas d'informations existantes pour le secteur %s.",
	
	L"FICHIER:  %s",
	
	L"FICHIER:  %s",
	
	L"Override READONLY",
	L"Écrasez Fichier", //30
	
	L"Vous n'avez actuellement aucune donnée sommaire.  En en créant un, vous pourrez garder la trace",
	L"des informations se rapportant à tous les secteurs que vous éditez et sauvez.  La progression de la création",
	L"va analyser toutes les cartes contenu dans votre dossier \\MAPS, et en générer un nouveau. Cela peut prendre",
	L"quelques minutes, tout dépend de combien de carte valide vous avez. Les cartes valides sont",
	L"les maps qui suivent la convention de nommage : a1.dat - p16.dat.  Les cartes souterraines",
	L"sont identifiées en ajoutant _b1 à _b3 avant le .dat (ex:  a9_b1.dat). ",
	
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
	L"Skipping update for %s.  Probably due to tileset conflicts...",
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
	L"MISE A JOUR VERSION MAJEURE",
	L"Il y a %d de cartes qui requièrent une mise à jour majeure.",
	L"Mise à jour de toutes les cartes périmées",
};

//selectwin.cpp
STR16 pDisplaySelectionWindowGraphicalInformationText[] =
{
	L"%S[%d] is from default tileset %s (%S)",
	L"File:  %S, subindex:  %d (%S)",
	L"Current Tileset:  %s",
};

//Cursor Modes.cpp
STR16 wszSelType[6] = {
 L"Petit", 
 L"Moyen", 
 L"Large", 
 L"Très large", 
 L"Largeur: xx", 
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
	L"@T,{;JA2 v1.13 Development Team",
	L"@T,C144,R134,{;Coding",
	L"@T,C144,R134,{;Graphics and Sounds",
	L"@};(Various other mods!)",
	L"@T,C144,R134,{;Items",
	L"@T,C144,R134,{;Other Contributors",
	L"@};(All other community members who contributed input and feedback!)",
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
//	L"9mm Hvy", // Val silent
//};


CHAR16 WeaponType[][30] =
{
	L"Divers",
	L"Pistolet",
	L"Pistolet-mitrailleur",
	L"Mitraillette",
	L"Fusil",
	L"Fusil de précision",
	L"Fusil d'assaut",
	L"Mitrailleuse légère",
	L"Fusil à pompe",
};

CHAR16 TeamTurnString[][STRING_LENGTH] =
{
	L"Tour du joueur", // player's turn
	L"Tour de l'ennemi",
	L"Tour des créatures",
	L"Tour de la milice",
	L"Tour des civils",
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
	L"%s n'a pas assez de Points d'Action !",
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
	L"Accessoires",
	
	//You cannot use "item(s)" and your "other item" at the same time.
	//Ex:  You cannot use sun goggles and you gas mask at the same time.
	L"Vous ne pouvez utiliser votre %s et votre %s simultanément.",

	L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre emplacements disponibles.",
	L"Vous pouvez combiner cet accessoire avec certains objets en le mettant dans l'un des quatre emplacements disponibles (Ici, cet accessoire n'est pas compatible avec cet objet).",
	L"Ce secteur n'a pas été sécurisé !",
	L"Vous devez donner %s à %s",//inverted !! you still need to give the letter to X
	L"%s a été touché à la tête !",
	L"Rompre le combat ?",
	L"Cet accessoire ne pourra plus être enlevé. Désirez-vous toujours le mettre ?",
	L"%s se sent beaucoup mieux !",
	L"%s a glissé sur des billes !",
	L"%s n'est pas parvenu à ramasser le/la %s !",
	L"%s a réparé le %s",
	L"Interruption pour ",
	L"Voulez-vous vous rendre ?",
	L"Cette personne refuse votre aide.",
	L"JE NE CROIS PAS !",
	L"Pour utiliser l'hélicoptère de Skyrider, vous devez ASSIGNER vos mercenaires au VEHICULE.",
	L"%s ne peut recharger qu'UNE arme",
	L"Tour des chats sauvages",
	L"automatique",
	L"Pas de tir auto",
	L"précis",
	L"peu précis",
	L"Pas de coup par coup",
	L"L'ennemi n'as plus rien à voler!",
	L"L'ennemi n'a rien dans les mains!",

	L"%s a perdu son camouflage de désert.",
	L"%s a perdu son camouflage de désert à cause de l'eau.",
	
	L"%s a perdu son camouflage de bois.",
	L"%s a perdu son camouflage de bois à cause de l'eau",
	
	L"%s a perdu son camouflage urbain.",
	L"%s a perdu son camouflage urbain à cause de l'eau.",
	
	L"%s a perdu son camouflage de neige.",
	L"%s a perdu son camouflage de neige à cause de l'eau.",

	L"Vous ne pouvez pas attacher %s à cette emplacement.",
	L"%s n'ira dans aucun emplacement de libre.",
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
	L"Pause",
	L"Normal",
	L"5 min",
	L"30 min",
	L"60 min",
	L"6 hrs", 
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
	L"Patient", // getting medical aid
	L"Transport", // in a vehicle
	L"Transit", // in transit - abbreviated form
	L"Réparation", // repairing
	L"Formation", // training themselves  
	L"Milice", // training a town to revolt
	L"Milice M.", //training moving militia units	
	L"Entraîneur", // training a teammate
	L"Elève", // being trained by someone else 
	L"Exploitation", // operating a strategic facility
	L"Eat",		// eating at a facility (cantina etc.)		// TODO.Translate
	L"Repos", // Resting at a facility
	L"Prison",		// Flugente: interrogate prisoners
	L"Mort(e)", // dead
	L"Incap.", // abbreviation for incapacitated
	L"Capturé", // Prisoner of war - captured
	L"Hôpital", // patient in a hospital 
	L"Vide",	// Vehicle is empty
};


STR16 pMilitiaString[] =
{
	L"Milice", // the title of the militia box
	L"Disponibles", //the number of unassigned militia troops
	L"Vous ne pouvez réorganiser la milice lors d'un combat !",
	L"Des milices ne sont pas assignées à un secteur. Voulez-vous les dissoudre ?",
};


STR16 pMilitiaButtonString[] =
{
	L"Auto", // auto place the militia troops for the player
	L"OK", // done placing militia troops
	L"Dissoudre", // HEADROCK HAM 3.6: Disband militia
};

STR16 pConditionStrings[] = 
{
	L"Excellent", //the state of a soldier .. excellent health
	L"Bon", // good health
	L"En forme", // fair health
	L"Blessé", // wounded health
	L"Fatigué", // tired
	L"Epuisé", // bleeding to death
	L"Inconscient", // knocked out 
	L"Mourant", // near death
	L"Mort", // dead
};

STR16 pEpcMenuStrings[] =
{
	L"Service", // set merc on active duty
	L"Patient", // set as a patient to receive medical aid
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
	L"Formation", 
	L"Milice",
	L"Forme la milice mobile",
	L"Entraîneur",
	L"Elève",
	L"Exploitation infras.",	
	L"Eat",		// eating at a facility (cantina etc.) // TODO.Translate
	L"Repos infras.",
	L"Interrogate prisoners",		// Flugente: interrogate prisoners	TODO.Translate
	L"Mort(e)",
	L"Incap.",
	L"Capturé(e)",
	L"Hôpital", 
	L"Vide",	// Vehicle is empty
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
	L"Formation", 
	L"Milice",
	L"Milice mobile",
	L"Entraîneur",
	L"Elève",
	L"Exploitation infras.",	
	L"Repos infras.",
	L"Interrogate prisoners",		// Flugente: interrogate prisoners TODO.Translate
	L"Mort(e)",
	L"Incap.",
	L"Capturé(e)",
	L"Hôpital", // patient in a hospital 
	L"Vide",	// Vehicle is empty
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
	L"DEXTERITE",  
	L"AGILITE", 
	L"SAGESSE", 
	L"PRECISION",
	L"MEDECINE",
	L"MECANIQUE",
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
	L"Exp", // explosive skill
	L"Méc", // mechanical skill
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
	L"Elève", // tell merc to be train by other 
};

STR16 pGuardMenuStrings[] =
{
	L"Cadence de tir :", // the allowable rate of fire for a merc who is guarding
	L"Feu à volonté", // the merc can be aggressive in their choice of fire rates
	L"Economiser munitions", // conserve ammo 
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
	L"*Economiser munitions*",
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
	L"Patient", // the merc is receiving medical attention
	L"Transport", // the merc is in a vehicle
	L"Réparation", // the merc is repairing items 
	L"Formation", // the merc is training
	L"Construction", // the merc is using/staffing a facility
	L"Annuler", // cancel this menu
};

//lal
STR16 pMilitiaControlMenuStrings[] =
{
	L"Attaquez", // set militia to aggresive
	L"Tenez la Position", // set militia to stationary
	L"Retraite", // retreat militia
	L"Rejoignez moi", // retreat militia
	L"Couchez vous", // retreat militia	
	L"Crouch",	// TODO.Translate
	L"A couvert!",
	L"All: Attaquez", 
	L"All: Tenez la Position",
	L"All: Retraite",
	L"All: Rejoignez-moi",
	L"All: Dispersez-vous",
	L"All: Couchez-vous",
	L"All: Crouch",	// TODO.Translate
	L"All: A couvert!",
	//L"All: Trouver materiel",
	L"Annuler", // cancel this menu
};

//STR16 pTalkToAllMenuStrings[] =
//{
//	L"Attaquez", // set militia to aggresive
//	L"Tenez la Position", // set militia to stationary
//	L"Retraite", // retreat militia
//	L"Rejoignez-moi", // retreat militia
//	L"Couchez-vous", // retreat militia		 
//	L"Annuler", // cancel this menu
//};

STR16 pRemoveMercStrings[] =
{
	L"Enlever Merc", // remove dead merc from current team
	L"Annuler",
};

STR16 pAttributeMenuStrings[] =
{
	L"Force",
	L"Dextérité",
	L"Agilité",
	L"Santé",
	L"Tir",
	L"Médecine",
	L"Mécanique",
	L"Commandement",
	L"Explosifs",
	L"Annuler",
};

STR16 pTrainingMenuStrings[] =
{
	L"Formation", // train yourself 
	L"Milice", // train the town 
	L"Milice mobile",
	L"Entraîneur", // train your teammates 
	L"Elève",  // be trained by an instructor 
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
	L"Tués : ", // number of kills by merc
	L"Participation : ", // number of assists on kills by merc
	L"Coût/jour :", // daily cost of merc
	L"Coût total :", // total cost of merc
	L"Contrat :", // cost of current contract
	L"Services rendus :", // total service rendered by merc
	L"Salaires dus :", // amount left on MERC merc to be paid
	L"Tir réussis :", // percentage of shots that hit target
	L"Batailles :", // number of battles fought
	L"Blessures :", // number of times merc has been wounded
	L"Spécialités :",
	L"Aucune spécialité",
	L"Réalisation :", // added by SANDRO
};

// SANDRO - helptexts for merc records
STR16 pPersonnelRecordsHelpTexts[] =
{
	L"Soldats d'élite : %d\n",
	L"Soldats réguliers : %d\n",
	L"Soldats d'administrations : %d\n",
	L"Groupe hostile : %d\n",
	L"Créatures : %d\n",
	L"Tanks : %d\n", 
	L"Autres : %d\n",

	L"Mercenaires : %d\n",
	L"Milices : %d\n",
	L"Autres : %d\n",

	L"Coups tirés : %d\n",
	L"Missiles tirés : %d\n",
	L"Grenades lancés : %d\n",
	L"Couteaus lancés : %d\n",
	L"Attaques au couteau : %d\n",
	L"Attaques au corps-à-corps : %d\n",
	L"Tirs réussis : %d\n",

	L"Serrures crochetées : %d\n",
	L"Serrures fracturées : %d\n",
	L"Pièges retirés : %d\n",
	L"Bombes explosées : %d\n",
	L"Objets réparés : %d\n",
	L"Objets combinés : %d\n",
	L"objets volés : %d\n",
	L"Milices entrainées : %d\n",
	L"Mercs soignés : %d\n",
	L"Chirurgies faites : %d\n",
	L"Personnes rencontrées : %d\n",
	L"Secteurs découverts : %d\n",
	L"Embuscades empêchées : %d\n",
	L"Quêtes faîtes : %d\n",

	L"Tactiques de combats : %d\n",
	L"Combats autorésolus : %d\n",
	L"Temps écoulés : %d\n",
	L"Embuscades expérimentées : %d\n",
	L"Meilleurs combats : %d Ennemies\n",

	L"Tirs : %d\n",
	L"Poignardés : %d\n",
	L"Poings : %d\n",
	L"Détruit : %d\n",
	L"Dégâts subit en installant : %d\n",
	L"Chirurgies subies : %d\n",
	L"Facility accidents : %d\n",

	L"Caractère :",
	L"Faiblesse :",

	L"Attitudes :",	// WANNE: For old traits display instead of "Character:"!

	L"Zombies: %d\n", // TODO.Translate
};


//These string correspond to enums used in by the SkillTrait enums in SoldierProfileType.h
STR16 gzMercSkillText[] = 
{
	L"Aucune spécialité",
	L"Crochetage",
	L"Combat à mains nues",
	L"Electronique",
	L"Opérations de nuit",
	L"Lancer",
	L"Enseigner",
	L"Armes lourdes",
	L"Armes automatiques",
	L"Discretion",
	L"Ambidextre",
	L"Voleur",
	L"Arts martiaux",
	L"Couteau",
	L"Tireur isolé",
	L"Camouflage",					
	L"(Expert)",
};

//////////////////////////////////////////////////////////
// SANDRO - added this
STR16 gzMercSkillTextNew[] = 
{
	// Major traits
	L"Aucune spécialité",

	L"Armes automatiques",
	L"Armes lourdes",
	L"Tireur d'élite",
	L"Scout",
	L"Bandit",
	L"Corps-à-corps",
	L"Manager",
	L"Technicien",
	L"Médecin",
	L"Covert Ops",		// 10	// TODO.Translate
	// Minor traits
	L"Ambidextre",
	L"Mêlée",
	L"Lancer",
	L"Opérations de nuit",
	L"Discrétion",
	L"Athlétic",
	L"Masse musculaire",
	L"Démolition",
	L"Enseigner",
	L"Reconnaissance",
	// second names for major skills
	L"Mitrailleur",
	L"Bombardier",
	L"Sniper",
	L"Chasseur",
	L"Combattant",
	L"Arts martiaux",
	L"Commandant",
	L"Ingénieur",
	L"Chirurgien",
	L"Spy",				// 30	// TODO.Translate
	L"Plus...",
};
//////////////////////////////////////////////////////////


// This is pop up help text for the options that are available to the merc

STR16 pTacticalPopupButtonStrings[] =
{
	L"Debout/Marcher (|S)",
	L"Accroupi/Avancer (|C)",
	L"Debout/Courir (|R)",
	L"A terre/Ramper (|P)",
	L"Regarder (|L)",
	L"Action",
	L"Parler",
	L"Examiner (|C|t|r|l)",

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
	L"Annuler (|E|c|h|a|p)",
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
	L"|Contrat du mercenaire",
	L"Retirer mercenaire",
	L"Repos", 
};

// volumes of noises 

STR16 pNoiseVolStr[] =
{
	L"FAIBLE",
	L"MOYEN",
	L"FORT",
	L"TRES FORT",
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
	L"Plaines, route",
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
	L"Hôpital de Cambria",
	L"Aéroport de Drassen",
	L"Aéroport de Meduna",
	L"Site SAM",
	L"Base rebelle", //The rebel base underground in sector A10
	L"Prison de Tixa",	//The basement of the Tixa Prison (J9)
	L"Repaire de créatures",	//Any mine sector with creatures in it
	L"Sous-sols d'Orta",	//The basement of Orta (K4)
	L"Tunnel",				//The tunnel access from the maze garden in Meduna 
										//leading to the secret shelter underneath the palace
	L"Abri",				//The shelter underneath the queen's palace
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

	L"DEFENSEURS",								//STR_AR_DEFEND_HEADER
	L"ATTAQUANTS",								//STR_AR_ATTACK_HEADER
	L"RENCONTRE",								//STR_AR_ENCOUNTER_HEADER
	L"Secteur",		//The Sector A9 part of the header		//STR_AR_SECTOR_HEADER

	//The battle ending conditions						

	L"VICTOIRE !",								//STR_AR_OVER_VICTORY
	L"DEFAITE !",								//STR_AR_OVER_DEFEAT
	L"REDDITION !",							//STR_AR_OVER_SURRENDERED
	L"CAPTURE !",								//STR_AR_OVER_CAPTURED
	L"RETRAITE !",								//STR_AR_OVER_RETREATED

	//These are the labels for the different types of enemies we fight in autoresolve.

	L"Milice",							//STR_AR_MILITIA_NAME,
	L"Elite",								//STR_AR_ELITE_NAME,
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
	L"ENNEMI REPERE",							//STR_PB_ENEMYENCOUNTER_HEADER,
	L"ATTAQUE ENNEMIE",							//STR_PB_ENEMYINVASION_HEADER, // 30
	L"EMBUSCADE !",								//STR_PB_ENEMYAMBUSH_HEADER
	L"VOUS PENETREZ EN SECTEUR ENNEMI",				//STR_PB_ENTERINGENEMYSECTOR_HEADER
	L"ATTAQUE DE CREATURES",							//STR_PB_CREATUREATTACK_HEADER
	L"AMBUSCADE DE CHATS SAUVAGES",							//STR_PB_BLOODCATAMBUSH_HEADER
	L"VOUS ENTREZ DANS LE REPAIRE DES CHATS SAUVAGES",			//STR_PB_ENTERINGBLOODCATLAIR_HEADER

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
	L"Cliquez sur ce bouton lorsque vous avez déployé \nvos mercenaires. (|E|n|t|r|é|e)",
	L"Vous devez déployer vos mercenaires \navant d'engager le combat.",

	//Various strings (translate word for word)

	L"Secteur",
	L"Définissez les points d'entrée",

	//Strings used for various popup message boxes.  Can be as long as desired.

	L"Il semblerait que l'endroit soit inaccessible...",
	L"Déployez vos mercenaires dans la zone en surbrillance.",

	//This message is for mercs arriving in sectors.  Ex:  Red has arrived in sector A9.
	//Don't uppercase first character, or add spaces on either end.

	L"est arrivé dans le secteur", 

	//These entries are for button popup help text for the prebattle interface.  All popup help
	//text supports the use of \n to denote new line.  Do not use spaces before or after the \n.
	L"Résolution automatique du combat\nsans charger la carte. (|A)",
	L"Résolution automatique impossible lorsque\nvous attaquez.",
	L"Pénétrez dans le secteur pour engager le combat. (|E)",
	L"Faire retraite vers le secteur précédent. (|R)",				//singular version
	L"Faire retraite vers les secteurs précédents. (|R)", //multiple groups with same previous sector

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

CHAR16		gWeaponStatsDesc[][ 19 ] =
{
	// HEADROCK: Changed this for Extended Description project
	L"Etat :",
	L"Poids :", 
	L"P. d'action",	
	L"Por. :",		// Range
	L"Dég. :",		// Damage
	L"Munitions :", 		// Number of bullets left in a magazine
	L"PA :",			// abbreviation for Action Points
	L"=",
	L"=",
						//Lal: additional strings for tooltips
	L"Precision :",	//9
	L"Portée :",		//10	
	L"Dégats :", 	//11
	L"Poids :",		//12
	L"Etourdissement :",//13
	// HEADROCK: Added new strings for extended description ** REDUNDANT **
	L"Accessoire :",	//14
	L"AUTO/5:",		//15
	L"Munit. rest. :",		//16
	L"Par défaut :",	//17 //WarmSteel - So we can also display default attachments
	L"Dirt:",	// 18	//added by Flugente	// TODO.Translate
	L"Space:", // 19 //space left on Molle items //TODO.Translate

};

// HEADROCK: Several arrays of tooltip text for new Extended Description Box
STR16		gzWeaponStatsFasthelpTactical[ 32 ] =
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
	L"|R|e|p|a|i|r |E|a|s|e\n \nThis value indicates how quickly this weapon can\nbe repaired (by a mercenary set to Repair duty).\n \nHigher is better.",
	L"",	//12
	L"PA pour mettre en joue",
	L"PA par tir",
	L"PA par rafale",
	L"PA pour tir auto.",
	L"PA pour recharger",
	L"PA pour recharger manuellement",
	L"Pénalité Rafale (Moins c'est mieux)",	//19
	L"Facteur de bipied",
	L"Nombre de tirs pour 5 PA",
	L"Pénalité Auto (Moins c'est mieux)",
	L"Pénalité Rafale/Auto (Moins c'est mieux)",	//23
	L"PA pour jeter",
	L"PA pour lancer",
	L"PA pour poignarder",
	L"Pas de tir simple!",
	L"Pas de tir en rafale!",
	L"Pas de tir auto.!",
	L"PA pour frapper",
    L"",
};

STR16		gzMiscItemStatsFasthelp[ 34 ] =
{
	L"Facteur d'encombrement (Valeur faible recommandée)", // 0
	L"Facteur de fiabilité",
	L"Facteur d'intensité sonore (Valeur faible recommandée)",
	L"Cache-flammes",
	L"Facteur de bipied",
	L"Facteur de portée", // 5
	L"Facteur de toucher",
	L"Portée laser optimum",
	L"Facteur de bonus de visée",
	L"Facteur de longueur de rafale",
	L"Facteur de pénalité de rafale (Valeur élevée recommandée)", // 10
	L"Facteur de pénalité tir auto. (Valeur élevée recommandée)",
	L"Facteur de PA",
	L"Facteur de PA rafale (Valeur faible recommandée)",
	L"Facteur de PA tir auto (Valeur faible recommandée)",
	L"Facteur de PA mise en joue (Valeur faible recommandéer)", // 15
	L"Facteur de PA recharger (Valeur faible recommandée)",
	L"Facteur de capacité chargeur",
	L"Facteur de PA attaque (Valeur faible recommandée)",
	L"Facteur de dégats",
	L"Facteur de dégats Mélée", // 20
	L"Camouflage bois",
	L"Camouflage urbain",
	L"Camouflage désert",
	L"Camouflage neige",
	L"Facteur de discrétion", // 25
	L"Facteur de portée auditive",
	L"Facteur de portée visuelle",
	L"Facteur de portée visuelle jour",
	L"Facteur de portée visuelle nuit",
	L"Facteur de portée visuelle faible lumiere", //30
	L"Facteur de portée visuelle Cave",
	L"Pourcentage d'effet tunnel (Valeur faible recommandée)",
	L"Portée minimale pour bonus de visée",
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
	L"TERRIBLE",
	L" condition."
};

//The headers used for the merc's money.

CHAR16 gMoneyStatsDesc[][ 14 ] =
{
	L"Montant",
	L"Restant :", //this is the overall balance
	L"Montant",
	L"Partager :", // the amount he wants to separate from the overall balance to get two piles of money

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
	L"BLESSE",    	//	>= 45
	L"EN FORME",    	//	>= 60
	L"BON",     	// 	>= 75
	L"EXCELLENT",		// 	>= 90
	L"CAPTURED",	// added by Flugente TODO.Translate
};

STR16 gzHiddenHitCountStr[1] =
{
	L"?",	
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
	L"Plus : ",
};

CHAR16		gzConsLabel[10] = 
{
	L"Moins : ",
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
	L"Echec.",
	L"Réussite !",
	L"Echec.",
	L"La serrure n'est pas piégée.",
	L"Réussite !",
	// The %s is a merc name
	L"%s ne possède pas la bonne clé.",
	L"Le piège est désamorcé.",
	L"La serrure n'est pas piégée.",
	L"Verrouillée.",
	L"PORTE",
	L"PIEGEE",
	L"VERROUILLEE",
	L"OUVERTE",
	L"ENFONCEE",
	L"Un interrupteur. Voulez-vous l'actionner ?",
	L"Désamorcer le piège ?",
	L"Préc...",
	L"Suiv...",
	L"Plus...",

	// In the next 2 strings, %s is an item name

	L"%s posé(e) à terre.",
	L"%s donné(e) à %s.",

	// In the next 2 strings, %s is a name

	L"%s a été payé.",
	L"%d dus à %s.",
	L"Choisir la fréquence de détonation :",  	//in this case, frequency refers to a radio signal
	L"Nombre de tours avant explosion :",	//how much time, in turns, until the bomb blows
	L"Définir la fréquence radio :", 	//in this case, frequency refers to a radio signal
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

	L"ENRAYE",					//weapon is jammed.
	L"Le robot a besoin de munitions calibre %s.",		//Robot is out of ammo
	L"Lancer ici ? Aucune chance.",		//Merc can't throw to the destination he selected

	// These are different buttons that the player can turn on and off.

	L"Discretion (|Z)",
	L"Carte (|M)",
	L"Fin du tour (|D)",
	L"Parler à",
	L"Muet",
	L"Se relever (|P|g|U|p)",
	L"Niveau du curseur (|T|a|b)",
	L"Escalader/Sauter (|J)",
	L"Se coucher (|P|g|D|n)",
	L"Examiner (|C|t|r|l)",
	L"Mercenaire précédent",
	L"Mercenaire suivant (|E|s|p|a|c|e)",
	L"Options (|O)",
	L"Rafale (|B)",
	L"Regarder/Pivoter (|L)",
	L"Santé : %d/%d\nEnergie : %d/%d\nMoral : %s",
	L"Pardon ?",					//this means "what?" 
	L"Suite",					//an abbrieviation for "Continued" 
	L"Sourdine désactivée pour %s.",
	L"Sourdine activée pour %s.",
	L"Etat : %d/%d\nCarburant : %d/%d",
	L"Sortir du véhicule" ,
	L"Changer d'escouade ( |M|a|j| |E|s|p|a|c|e )",
	L"Conduire",
	L"N/A",						//this is an acronym for "Not Applicable." 
	L"Utiliser (Mains nues)",
	L"Utiliser (Arme à feu)",
	L"Utiliser (Couteau)",
	L"Utiliser (Explosifs)",
	L"Utiliser (Trousse de soins)",
	L"(Prendre)",
	L"(Recharger)",
	L"(Donner)",
	L"%s part.",
	L"%s arrive.",
	L"%s n'a plus de Points d'Action.",
	L"%s n'est pas disponible.",
	L"%s est couvert de bandages.",
	L"%s n'a plus de bandages.",
	L"Ennemi dans le secteur !",
	L"Pas d'ennemi en vue.",
	L"Pas assez de Points d'Action.",
	L"Télécommande inutilisée.",
	L"La rafale a vidé le chargeur !",
	L"SOLDAT",
	L"CREPITUS",
	L"Milice",
	L"CIVIL",
	L"ZOMBIE",		// TODO.Translate
	L"Quitter Secteur",
	L"OK",
	L"Annuler",
	L"Mercenaire",
	L"Tous",
	L"GO",
	L"Carte",
	L"Vous ne pouvez pas quitter ce secteur par ce côté.",
	L"You can't leave in turn based mode.",		// TODO.Translate
	L"%s est trop loin.",
	L"Enlever cime des arbres",
	L"Afficher cime des arbres",
	L"CORBEAU",				//Crow, as in the large black bird
	L"COU",
	L"TETE",
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
	L"Etes-vous d'accord pour payer %s ?",
	L"Acceptez-vous le traitement médical gratuit ?",
	L"Voulez-vous épouser Daryl ?",
	L"Trousseau de Clés",
	L"Vous ne pouvez pas faire ça avec ce personnage.",
	L"Epargner Krott ?",
	L"Hors de portée",
	L"Mineur",
	L"Un véhicule ne peut rouler qu'entre des secteurs",
	L"Impossible d'apposer des bandages maintenant",
	L"Chemin bloqué pour %s",
	L"Vos mercenaires capturés par l'armée de Deidranna sont emprisonnés ici !",
	L"Verrou touché",
	L"Verrou détruit",
	L"Quelqu'un d'autre veut essayer sur cette porte.",
	L"Etat : %d/%d\nCarburant : %d/%d",
	L"%s ne peut pas voir %s.",  // Cannot see person trying to talk to
	L"Accessoire retiré",
	L"Ne peut pas gagner un autre véhicule car vous en avez déjà 2",

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
	L"1 - Fill Canteens 2 - Clean one gun 3 - Clean all guns 4 - Take off clothes",	// TODO.Translate

	// added by Flugente: decide what to do with the corpses
	L"1 - Decapitate 2 - Gut 3 - Take Clothes 4 - Take Body",	// TODO.Translate

	// Flugente: weapon cleaning	// TODO.Translate
	L"%s cleaned %s",

	// added by Flugente: decide what to do with prisoners	// TODO.Translate
	L"You have no prison for these prisoners, you have to let them go",
	L"Yes - Send prisoners to jail      No - Let them go",
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
	L"%s doit être escorté(e) par vos mercenaires et ne peut quitter ce secteur tout seul.",

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
	L"Aucun",
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
};

// town strings

STR16 pwTownInfoStrings[] =
{
	L"Taille",					// 0 // size of the town in sectors
	L"", 						// blank line, required
	L"Contrôle", 					// how much of town is controlled
	L"Aucune", 					// none of this town
	L"Mine associée", 				// mine associated with this town
	L"Loyauté",					// 5 // the loyalty level of this town
	L"Forces entraînées", 					// the forces in the town trained by the player
	L"",
	L"Principales installations", 				// main facilities in this town
	L"Niveau", 					// the training level of civilians in this town
	L"Formation",				// 10 // state of civilian training in town
	L"Milice", 					// the state of the trained civilians in the town
	L"Formation mobile",			// HEADROCK HAM 3.6: The stat of Mobile militia training in town
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
	L"Epuisée",
	L"Production",
	L"Etat",
	L"Productivité",
	L"Type de minerai",				// 10
	L"Contrôle de la ville",
	L"Loyauté de la ville",
//	L"Mineurs au travail",
};

// blank sector strings

STR16 pwMiscSectorStrings[] =
{
	L"Forces ennemies",
	L"Secteur",
	L"# d'objets",
	L"Inconnu",
	L"Contrôlé",
	L"Oui",
	L"Non",
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
	L"Cliquez sur la colonne Affectation pour assigner un mercenaire à une nouvelle tâche",
	L"Cliquez sur la colonne Destination pour ordonner à un mercenaire de se rendre dans un secteur",
	L"Utilisez la compression du temps pour que le voyage du mercenaire vous paraisse moins long.",
	L"Cliquez sur un secteur pour le sélectionner. Cliquez à nouveau pour donner un ordre de mouvement à un mercenaire ou effectuez un clic droit pour obtenir des informations sur le secteur.",
	L"Appuyez sur 'H' pour afficher l'aide en ligne.",
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
	L"Montrer la milice mobile |Restrictions", // HEADROCK HAM 4: Mobile Restrictions Button
};

STR16 pMapScreenInvenButtonHelpText[] =
{
	L"Suivant (|.)",		// next page
	L"Précédent (|,)",		// previous page
	L"Quitter l'inventaire du secteur (|E|s|c)",	// exit sector inventory

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
};

STR16 pMapScreenBottomFastHelp[] =
{
	L"PC Portable (|L)",
	L"Tactique (|E|c|h|a|p)",
	L"Options (|O)",
	L"Compression du temps (|+)", 	// time compress more
	L"Compression du temps (|-)", 	// time compress less
	L"Message précédent (|U|p)\nPage précédente (|P|g|U|p)", 	// previous message in scrollable list
	L"Message suivant (|D|o|w|n)\nPage suivante (|P|g|D|n)", 	// next message in the scrollable list
	L"Interrompre/Reprendre (|S|p|a|c|e)",	// start/stop time compression
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
 L"Effacer message NON CONSULTE ?",
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
	L"Bookkeeper Plus",		//the name we made up for the financial program in the game
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
	L"Règlement M.E.R.C.",
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
	L"Materiel vendu à la population",
	L"Infrastucture utilisée", // HEADROCK HAM 3.6
	L"Entretien de la milice", // HEADROCK HAM 3.6
	L"Ransom for released prisoners",	// Flugente: prisoner system TODO.Translate
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
	L"Skyrider n'a pas de passagers. Si vous voulez envoyer des mercenaires dans ce secteur, n'oubliez pas de les assigner à l'hélicoptère.",
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
	L"Energie",
	L"Moral",
	L"Etat",	// the condition of the current vehicle (its "Santé")
	L"Carburant",	// the fuel level of the current vehicle (its "energy")
	L"Poison",	// TODO.Translate
	L"Water",		// drink level
	L"Food",		// food level
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
	L"Cet objet a l'air VRAIMENT important. Vous êtes bien sûr (mais alors BIEN SUR) de vouloir l'abandonner ?", // does the user REALLY want to trash this item
};


STR16 pMapErrorString[] = 
{
	L"L'escouade ne peut se déplacer si l'un de ses membres se repose.",

//1-5
	L"Déplacez d'abord votre escouade.",
	L"Des ordres de mouvement ? C'est un secteur hostile !",
	L"Les mercenaires doivent d'abord être assignés à une escouade ou un vehicule.",
	L"Vous n'avez plus aucun membre dans votre escouade.", 		// you have non members, can't do anything 
	L"Le mercenaire ne peut obéir.",			 		// merc can't comply with your order
//6-10
	L"doit être escorté. Mettez-le dans une escouade.", // merc can't move unescorted .. for a male
	L"doit être escortée. Mettez-la dans une escouade.", // for a female
	L"Ce mercenaire n'est pas encore arrivé !",
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
	L"Les forces de Deidranna ont été aperçues en %s.",
	L"Les forces de Deidranna viennent de prendre %s.",
//26-30
	L"L'un au moins de vos mercenaires n'est pas fatigué.",
	L"L'un au moins de vos mercenaires ne peut être réveillé.",
	L"La milice n'apparaît sur l'écran qu'une fois son entraînement achevé.",
	L"%s ne peut recevoir d'ordre de mouvement pour le moment.",
	L"Les miliciens qui ne se trouvent pas dans les limites d'une ville ne peuvent être déplacés.",
//31-35
	L"Vous ne pouvez pas entraîner de milice en %s.",
	L"Un véhicule ne peut se déplacer s'il est vide !",
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
	L"Inventaire (|E|n|t|r|é|e)",
	L"Lancer objet",
	L"Quitter Inventaire (|E|n|t|r|é|e)",
};



// male version of where equipment is left
STR16 pMercHeLeaveString[] =
{
	L"%s doit-il abandonner son équipement sur place (%s) ou à Drassen (B13) avant de quitter Arulco ?",
	L"%s doit-il abandonner son équipement sur place (%s) ou à Omerta (A9) avant de quitter Arulco ?",
	L"est sur le point de partir et laissera son équipement à Omerta (A9).",
	L"est sur le point de partir et laissera son équipement à Drassen (B13).",
	L"%s est sur le point de partir et laissera son équipement en %s.",
};


// female version
STR16 pMercSheLeaveString[] =
{
	L"%s doit-elle abandonner son équipement sur place (%s) ou à Drassen (B13) avant de quitter Arulco ?",
	L"%s doit-elle abandonner son équipement sur place (%s) ou à Omerta (A9) avant de quitter Arulco ?",
	L"est sur le point de partir et laissera son équipement à Omerta (A9).",
	L"est sur le point de partir et laissera son équipement à Drassen (B13).",
	L"%s est sur le point de partir et laissera son équipement en %s.",
};


STR16 pMercContractOverStrings[] =
{
	L"a rempli son contrat, il est rentré chez lui.", 		// merc's contract is over and has departed
	L"a rempli son contrat, elle est rentrée chez elle.", 		// merc's contract is over and has departed
	L"est parti, son contrat ayant été annulé.", 		// merc's contract has been terminated
	L"est partie, son contrat ayant été annulé.",		// merc's contract has been terminated
	L"Vous devez trop d'argent à la M.E.R.C., %s quitte Arulco.", // Your M.E.R.C. account is invalid so merc left
};

// Text used on IMP Web Pages

// WDS: Allow flexible numbers of IMPs of each sex
//    note: I only updated the English text to remove "three" below
STR16 pImpPopUpStrings[] =
{
	L"Code Incorrect",
	L"Vous allez établir un nouveau profil. Etes-vous sûr de vouloir recommencer ?",
	L"Veuillez entrer votre nom et votre sexe.",
	L"Vous n'avez pas les moyens de vous offrir une analyse de profil.",
	L"Option inaccessible pour le moment.",
	L"Pour que cette analyse soit efficace, il doit vous rester au moins une place dans votre escouade.",
	L"Profil déjà établi.",
	L"Impossible de charger le profil.",
	L"Vous avez déjà atteint le nombre maximum d'IMP.",
	L"Vous avez déjà trois IMP du même sexe dans l'équipe.",
	L"Vous n'avez pas les moyens.",
	L"Le nouvel IMP a rejoint votre équipe.",
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
	L"OUI, JE SUIS SUR.", 		// oui, I am certain 
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
	L"Evénement", 			// the event label
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
	L"%s engagé(e) sur le site de l'A.I.M.", 										// merc was hired from the aim site
	L"%s engagé(e) sur le site M.E.R.C.", 									// merc was hired from the aim site
	L"%s meurt.", 															// merc was killed
	L"Versements M.E.R.C.",								// paid outstanding bills at MERC
	L"Ordre de mission d'Enrico Chivaldori accepté",	
	//6-10
	L"Profil IMP",
	L"Souscription d'un contrat d'assurance pour %s.", 				// insurance contract purchased
	L"Annulation du contrat d'assurance de %s.", 				// insurance contract canceled
	L"Indemnité pour %s.", 							// insurance claim payout for merc
	L"Extension du contrat de %s (1 jour).", 						// Extented "mercs name"'s for a day
	//11-15
	L"Extension du contrat de %s (1 semaine).", 					// Extented "mercs name"'s for a week
	L"Extension du contrat de %s (2 semaines).", 					// Extented "mercs name"'s 2 weeks
	L"%s a été renvoyé(e).", 													// "merc's name" was dismissed.
	L"%s a démissionné.", 																		// "merc's name" quit.
	L"quête commencée.", 															// a particular quest started
	//16-20
	L"quête achevée.",
	L"Entretien avec le chef des mineurs de %s",									// talked to head miner of town
	L"Libération de %s",
	L"Activation du mode triche",
	L"Le ravitaillement devrait arriver demain à Omerta",
	//21-25
	L"%s a quitté l'escouade pour épouser Daryl Hick",
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
	L"Un témoin pense avoir aperçu quelque chose dans les mines.",
	//36-40
	L"Rencontre avec Devin - vend des explosifs.",
	L"Rencontre avec Mike, le fameux ex-mercenaire de l'AIM !",
	L"Rencontre avec Tony - vend des armes.",
	L"Fusil à roquettes récupéré auprès du Sergent Krott.",
	L"Acte de propriété du magasin d'Angel donné à Kyle.",
	//41-45
	L"Madlab propose de construire un robot.",
	L"Gabby fait des décoctions rendant invisible aux créatures.",
	L"Keith est hors-jeu.",
	L"Howard fournit du cyanure à la Reine Deidranna.",
	L"Rencontre avec Keith - vendeur à Cambria.",
	//46-50
	L"Rencontre avec Howard - pharmacien à Balime",
	L"Rencontre avec Perko - réparateur en tous genres.",
	L"Rencontre avec Sam de Balime - vendeur de matériel.",
	L"Franz vend du matériel électronique.",
	L"Arnold tient un magasin de réparations à Grumm.",
	//51-55
	L"Fredo répare le matériel électronique à Grumm.",
	L"Don provenant d'un homme influent de Balime.",
	L"Rencontre avec Jake, vendeur de pièces détachées.",
	L"Clé électronique reçue.",
	L"Corruption de Walter pour ouvrir l'accès aux sous-sols.",
	//56-60
	L"Dave refait gratuitement le plein s'il a du carburant.",
	L"Pot-de-vin donné à Pablo.",
	L"Kingpin cache un trésor dans la mine de San Mona.",
	L"Victoire de %s dans l'Extreme Fighting",
	L"Défaite de %s dans l'Extreme Fighting",
	//61-65
	L"Disqualification de %s dans l'Extreme Fighting",
	L"Importante somme découverte dans la mine abandonnée.",
	L"Rencontre avec un tueur engagé par Kingpin.",
	L"Perte du secteur",				//ENEMY_INVASION_CODE
	L"Secteur défendu",
	//66-70
	L"Défaite",							//ENEMY_ENCOUNTER_CODE
	L"Embuscade",						//ENEMY_AMBUSH_CODE
	L"Embuscade ennemie déjouée",
	L"Echec de l'attaque",			//ENTERING_ENEMY_SECTOR_CODE
	L"Réussite de l'attaque !",
	//71-75
	L"Attaque de créatures",			//CREATURE_ATTACK_CODE
	L"Ambuscade de chats sauvages",			//BLOODCAT_AMBUSH_CODE
	L"Elimination des chats sauvages",
	L"%s a été tué(e)",
	L"Tête de terroriste donnée à Carmen",
	L"Reste Slay",
	L"%s a été tué(e)",
};

STR16 pHistoryLocations[] =
{
	L"N/A",						// N/A is an acronym for Not Applicable
};

// icon text strings that appear on the laptop

STR16 pLaptopIcons[] =
{
	L"E-mail",
	L"Internet",
	L"Finances",
	L"Personnel",
	L"Historique",
	L"Fichiers",
	L"Eteindre",
	L"sir-FER 4.0",			// our play on the company name (Sirtech) and web surFER
};

// bookmarks for different websites
// IMPORTANT make sure you move down the Cancel string as bookmarks are being added

STR16 pBookMarkStrings[] =
{
	L"A.I.M.",
	L"Bobby Ray",
	L"I.M.P",
	L"M.E.R.C.",
	L"Morgue",
	L"Fleuriste",
	L"Assurance",
	L"Annuler",
	L"Encyclopedia",
	L"Briefing Room",
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
	L"Transférer $", 		// transfer money to merc -- short form
	L"Stats", 			// view stats of the merc
	L"Inventaire", 			// view the inventory of the merc
	L"Tâche",
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
	L"A.I.M.",
	L"Membres A.I.M.",
	L"Galerie A.I.M.",		// a mug shot is another name for a portrait
	L"Tri A.I.M.",
	L"A.I.M.",
	L"Anciens A.I.M.",
	L"Règlement A.I.M.",
	L"Historique A.I.M.",
	L"Liens A.I.M.",
	L"M.E.R.C.",
	L"Comptes M.E.R.C.",
	L"Enregistrement M.E.R.C.",
	L"Index M.E.R.C.",
	L"Bobby Ray",
	L"Bobby Ray - Armes",
	L"Bobby Ray - Munitions",
	L"Bobby Ray - Protections",
	L"Bobby Ray - Divers",							//misc is an abbreviation for miscellaneous				
	L"Bobby Ray - Occasions",
	L"Bobby Ray - Commande",
	L"I.M.P.",
	L"I.M.P.",
	L"Service des Fleuristes Associés",
	L"Service des Fleuristes Associés - Exposition",
	L"Service des Fleuristes Associés - Bon de commande",
	L"Service des Fleuristes Associés - Cartes",
	L"Malleus, Incus & Stapes Courtiers",
	L"Information",
	L"Contrat",
	L"Commentaires",
	L"Morgue McGillicutty",
	L"",
	L"URL introuvable.",
	L"Bobby Ray - Dernières commandes",
	L"Encyclopedia",
	L"Encyclopedia - Data",
	L"Briefing Room",
	L"Briefing Room - Data",
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
	L"Bookkeeper Plus",
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
	L"Equipe actuelle",
	L"Départs",
	L"Coût quotidien :",
	L"Coût maximum :",
	L"Coût minimum :",
	L"Morts en mission :",
	L"Démissionés :",
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
	L"Home",
	L"Compte # :",
	L"Mercenaire",
	L"Jours",
	L"Taux",	//5
	L"Montant",
	L"Total :",
	L"Désirez-vous autoriser le versement de %s ?",		//the %s is a string that contains the dollar amount ( ex. "$150" )
};

// Merc Account Page buttons
STR16			MercAccountPageText[] = 
{
	// Text on the buttons on the bottom of the screen
	L"Previous",
  L"Next",
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
	L"Home",
	L"Engagé",
	L"Salaire :",
	L"Par jour",
	L"Décédé(e)",

	L"Vous ne pouvez engager plus de 18 mercenaires.",
	L"Indisponible",											
	L"Bio", //TODO.Translate
	L"Inv",								
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
	L"Speck T. Kline, fondateur",
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
	L"Morgue McGillicutty : A votre écoute depuis 1983.",
	L"Murray \"Pops\" McGillicutty, notre directeur bien aimé, est un ancien mercenaire de l'AIM. Sa spécialité : la mort des autres.",
	L"Pops l'a côtoyée pendant si longtemps qu'il est un expert de la mort, à tous points de vue.",
	L"La morgue McGillicutty vous offre un large éventail de services funéraires, depuis une écoute compréhensive jusqu'à la reconstitution des corps... dispersés.",
	L"Laissez donc la morgue McGillicutty vous aider, pour que votre compagnon repose enfin en paix.",

	// Text for the various links available at the bottom of the page
	L"ENVOYER FLEURS",
	L"CERCUEILS & URNES",
	L"CREMATION",
	L"SERVICES FUNERAIRES",
	L"NOTRE ETIQUETTE",

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
	L"Nos bombardiers (recyclés) vous livrent votre bouquet dans un rayon de 20 km (ou presque). N'importe quand - N'importe où !",
	L"Nous répondons à tous vos besoins (ou presque) !",
	L"Bruce, notre expert fleuriste-conseil, trouvera pour vous les plus belles fleurs et vous composera le plus beau bouquet que vous ayez vu !",
	L"Et n'oubliez pas que si nous ne l'avons pas, nous pouvons le faire pousser - et vite !",
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
	L"Pot Pourri (10$)",
	L"Roses Noires (20$)",
	L"Nature Morte (10$)",
	L"Gâteau (si dispo)(10$)",		//15
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
	L"Note : les bouquets \"pot pourri\" et \"nature morte\" vous seront facturés 10$ supplémentaires.",

	//text on the button

	L"Home",
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
	L"Home",				// Text on the button that returns to the home page
	L"* Matériel d'occasion",		// Disclaimer stating that the item is used
	L"Vous n'avez pas les moyens.",		//20	// A popup message that to warn of not enough money
	L"<AUCUNE>",				// Gets displayed when there is non valid city selected
	L"Etes-vous sûr de vouloir envoyer cette commande à %s ?",		// A popup that asks if the city selected is the correct one
	L"Poids total **",			// Displays the weight of the package
	L"** Pds Min.",				// Disclaimer states that there is a minimum weight for the package
	L"Envois",	
};

STR16			BobbyRFilter[] =
{
	// Guns
	L"Arm.poing",
	L"Pist. M.",
	L"Mitraill.",
	L"Fusil",
	L"Sniper",
	L"F.d'assa.",
	L"M. légère",
	L"F.à pompe",
	L"Arm.Lourd",

	// Ammo
	L"Arm.poing",
	L"Pist. M.",
	L"Mitraill.",
	L"Fusil",
	L"Sniper",
	L"F.d'assa.",
	L"M. légère",
	L"F.à pompe",

	// Used
	L"Armes",
	L"Protec.",
	L"Mat. LBE",
	L"Divers",

	// Armour
	L"Casques",
	L"Vestes",
	L"Pant.",
	L"Blindage",

	// Misc
	L"Couteau",
	L"Cout de L.",
	L"Mêlée",
	L"Grenades",
	L"Explosifs",
	L"Kits Med.",
	L"Kits",
	L"Mat. Face",
	L"Mat. LBE",
	L"Optics", // Madd: new BR filters		// TODO.Translate
	L"Grip/LAM",
	L"Muzzle",
	L"Stock",
	L"Mag/Trig.",
	L"Other Att.",
	L"Divers",
};


// This text is used when on the various Bobby Ray Web site pages that sell items

STR16			BobbyRText[] = 
{
	L"Pour commander",				// Title
	// instructions on how to order
	L"Cliquez sur les objets désirés. Cliquez à nouveau pour sélectionner plusieurs exemplaires d'un même objet. Effectuez un clic droit pour désélectionner un objet. Il ne vous reste plus qu'à passer commande.",			

	//Text on the buttons to go the various links

	L"Objets précédents",		// 
	L"Armes", 			//3
	L"Munitions",			//4
	L"Protection",			//5
	L"Divers",			//6	//misc is an abbreviation for miscellaneous
	L"Occasion",			//7
	L"Autres objets",
	L"BON DE COMMANDE",
	L"Home",			//10

	//The following 2 lines are used on the Ammunition page.  
	//They are used for help text to display how many items the player's merc has
	//that can use this type of ammo

	L"Votre équipe possède",//11
	L"arme(s) qui utilise(nt) ce type de munitions", //12

	//The following lines provide information on the items

	L"Poids :",		// Weight of all the items of the same type
	L"Cal :",			// the caliber of the gun
	L"Chrg :",			// number of rounds of ammo the Magazine can hold
	L"Portée:",			// The range of the gun
	L"Dégats:",			// Damage of the weapon	
	L"CdT :",			// Weapon's Rate Of Fire, acronym ROF
	L"Prix :",			// Cost of the item
	L"En réserve :",			// The number of items still in the store's inventory
	L"Qté commandée :",		// The number of items on order
	L"Endommagé",			// If the item is damaged
	L"Poids :",			// the Weight of the item
	L"Sous-total :",			// The total cost of all items on order
	L"* %% efficacité",		// if the item is damaged, displays the percent function of the item

	//Popup that tells the player that they can only order 10 items at a time
	
	L"Pas de chance ! Vous ne pouvez commander que " ,//First part
	L" objets à la fois. Si vous désirez passer une commande plus importante, il vous faudra remplir un nouveau bon de commande.",

	// A popup that tells the user that they are trying to order more items then the store has in stock

	L"Nous sommes navrés, mais nos stocks sont vides. N'hésitez pas à revenir plus tard !",

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
	L"En Construction",
};



// Text for the AIM page.
// This is the text used when the user selects the way to sort the aim mercanaries on the AIM mug shot page

STR16			AimSortText[] =
{
	L"Membres A.I.M.",				// Title
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
	L"Consulter la galerie des anciens de l'A.I.M.",

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
	L"Home AIM", 
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

	L"Paie",							
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
	L"gear",			//"gear",	//tais: Displays the optional gear cost in nsgi, this moved and can have only a small room, so just make it "gear" without extra's
	L"Dépôt Médical",			// If the merc required a medical deposit, this is displayed
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

	L"Pas d'équipement",
	L"Acheter équipement",

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

	L"dépôt compris"			// Displays if you are hiring the merc with the medical deposit
};



//Aim Member.c
//The page in which the player hires AIM mercenaries

// The text that pops up when you select the TRANSFER FUNDS button

STR16			AimPopUpText[] =
{
	L"TRANSFERT ACCEPTE",	// You hired the merc
	L"TRANSFERT REFUSE",		// Player doesn't have enough money, message 1
	L"FONDS INSUFFISANTS",				// Player doesn't have enough money, message 2

	// if the merc is not available, one of the following is displayed over the merc's face

	L"En mission",											
	L"Veuillez laisser un message",
	L"Décédé(e)",

	//If you try to hire more mercs than game can support

	L"Equipe de mercenaires déjà au complet.",

	L"Message pré-enregistré",
	L"Message enregistré",
};


//AIM Link.c

STR16			AimLinkText[] =
{
	L"Liens A.I.M.",	//The title of the AIM links page
};



//Aim History

// This page displays the history of AIM

STR16			AimHistoryText[] =
{
	L"Historique A.I.M.",					//Title

	// Text on the buttons at the bottom of the page

	L"Précédent",
	L"Home", 
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

	L"Tri ascendant des membres de l'A.I.M. par %s",
	L"Tri descendant des membres de l'A.I.M. par %s",

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

	L"OK"			// Stops displaying information on selected merc
};






//AIM Home Page

STR16			AimScreenText[] =
{
	// AIM disclaimers
	
	L"A.I.M. et le logo A.I.M. sont des marques déposées dans la plupart des pays.",
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
	L"COUT TOTAL",				//The total cost of the the items in the Dealer inventory area
	L"VALEUR TOTALE",			//The total value of items player wishes to sell
	L"EVALUATION",				//Button text for dealer to evaluate items the player wants to sell
	L"TRANSACTION",			//Button text which completes the deal. Makes the transaction.
	L"OK",				//Text for the button which will leave the shopkeeper interface.
	L"COUT REPARATION",			//The amount the dealer will charge to repair the merc's goods
	L"1 HEURE",			// SINGULAR! The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"%d HEURES",		// PLURAL!   The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"REPARE",		// Text appearing over an item that has just been repaired by a NPC repairman dealer
	L"Plus d'emplacements libres.",	//Message box that tells the user there is non more room to put there stuff
	L"%d MINUTES",		// The text underneath the inventory slot when an item is given to the dealer to be repaired
	L"Objet lâché à terre.",
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

	//Text above the slider bars
	L"Effets",
	L"Dialogue",
	L"Musique",

	//Confirmation pop when the user selects..
	L"Quitter la partie et revenir au menu principal ?",

	L"Activez le mode Dialogue ou Sous-titre.",
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

	L"Etes-vous sûr de vouloir écraser la sauvegarde #%d ?",
	L"Voulez-vous charger la sauvegarde #%d ?",


	//The first %d is a number that contains the amount of free space on the users hard drive,
	//the second is the recommended amount of free space.
	L"Votre risquez de manquer d'espace disque. Il ne vous reste que %d Mo de libre alors que le jeu nécessite %d Mo d'espace libre.",	

	L"Enregistrement",			//When saving a game, a message box with this string appears on the screen

	L"Quelques Armes",
	L"Toutes Armes",
	L"Style réaliste",
	L"Style SF",

	L"Difficulté",
	L"Platinum Mode", //Placeholder English

	L"Bobby Ray Qualité",
	L"Bonne",
	L"Meilleure",
	L"Excellente",
	L"Superbe",

	L"Le nouvel inventaire (NIV) ne peut se lancer en 640x480. Changez de résolution.",
	L"Le nouvel inventaire (NIV) ne fonctionne pas depuis le dossier \"data\" original.",

	L"The squad size from the savegame is not supported by the current screen resolution. Please increase the screen resolution and try again.",	// TODO.Translate
	L"Bobby Ray Quantité",
};



//MapScreen
STR16		zMarksMapScreenText[] =
{
	L"Niveau carte",
	L"Vous n'avez pas de milice : vous devez entraîner les habitants de la ville.",
	L"Revenu quotidien",
	L"Assurance vie", 
	L"%s n'est pas fatigué.", 
	L"%s est en mouvement et ne peut dormir.", 
	L"%s est trop fatigué pour obéir.",
	L"%s conduit.",
	L"L'escouade ne peut progresser si l'un de ses membres se repose.",

	// stuff for contracts
	L"Vous pouvez payer les honoraires de ce mercenaire, mais vous ne pouvez pas vous offrir son assurance.",
	L"La prime d'assurance de %s coûte %s pour %d jour(s) supplémentaire(s). Voulez-vous les payer ?",
	L"Inventaire du Secteur",
	L"Le mercenaire a un dépôt médical.",
	
	// other items
	L"Docteurs", // people acting a field medics and bandaging wounded mercs 
	L"Patients", // people who are being bandaged by a medic 
	L"OK", // Continue on with the game after autobandage is complete 
	L"Stop", // Stop autobandaging of patients by medics now 
	L"Désolé. Cette option n'est pas disponible.", // informs player this option/button has been disabled in the demo 
	L"%s n'a pas de trousse à outil.",
	L"%s n'a pas de trousse de soins.",
	L"Il y a trop peu de volontaires pour l'entraînement.",
	L"%s ne peut pas former plus de miliciens.",
	L"Le mercenaire a un contrat déterminé.", 
	L"Ce mercenaire n'est pas assuré.",
	L"Ecran carte",		// 24
	// HEADROCK HAM 4: Prompt messages when turning on Mobile Militia Restrictions view.
	L"Vous n'avez actuellement aucune Milice Mobile. Retournez à ce mode du vue quand vous en aurez recruté.",
	L"Cette vue montre où votre Milice Mobile peut et ne peut pas aller. Gris = La Milice Mobile refuse d'aller ici. ROUGE = La Milice Mobile peut aller ici, mais vous avez dit de ne pas y aller. JAUNE = La Milice Mobile peut entrer dans ce secteur, mais ne peut en sortir. VERT = la Milice Mobile peut aller ici librement. Right click a Green/Yellow sector to cycle its behavior.",
};


STR16 pLandMarkInSectorString[] =
{
	L"L'escouade %d a remarqué quelque chose dans le secteur %s",
};

// confirm the player wants to pay X dollars to build a militia force in town
STR16 pMilitiaConfirmStrings[] =
{
	L"L'entraînement de la milice vous coûtera $", // telling player how much it will cost
	L"Etes-vous d'accord ?", // asking player if they wish to pay the amount requested
	L"Vous n'en avez pas les moyens.", // telling the player they can't afford to train this town
	L"Voulez-vous poursuivre l'entraînement de la milice à %s (%s %d) ?", // continue training this town?
	L"Coût $", // the cost in dollars to train militia
	L"(O/N)",   // abbreviated oui/non
	L"",	// unused
	L"L'entraînement des milices dans %d secteurs vous coûtera %d $. %s", // cost to train sveral sectors at once
	L"Vous ne pouvez pas payer les %d $ nécessaires à l'entraînement.",
	L"Vous ne pouvez poursuivre l'entraînement de la milice à %s que si cette ville est à niveau de loyauté de %d pour-cent.",
	L"Vous ne pouvez plus entraîner de milice à %s.",
	L"Yous ne pouvez pas payer les %d $ nécessaires à l'entraînement de la milice mobile.", // HEADROCK HAM 3.6: Mobile Militia
	L"Voulez-vous poursuivre l'entraînement de la milice mobile à %s (%s %d)?", // HEADROCK HAM 3.6: Mobile Militia	
	L"L'entraînement des milices mobiles dans %d secteurs vous coûtera %d $. %s", // HEADROCK HAM 3.6: Mobile Militia
	L"L'entraînement de la milice mobile vous coûtera $", // HEADROCK HAM 3.6: Mobile Militia
};

//Strings used in the popup box when withdrawing, or depositing money from the $ sign at the bottom of the single merc panel
STR16	gzMoneyWithdrawMessageText[] = 
{
	L"Vous ne pouvez retirer que 20 000 $ à la fois.",
	L"Etes-vous sûr de vouloir déposer %s sur votre compte ?",
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
	L"Mouvemts mercenaires éclairés",
	L"Figer curseur sur mercenaires",
	L"Figer curseur sur les portes",
	L"Objets en surbrillance",
	L"Afficher cimes",
	L"Affichage fil de fer",
	L"Curseur toît",
	L"Afficher chance de toucher",
	L"Curseur raf. pour raf. lance G.",
	L"Remarques des ennemis", // Changed from "Enemies Drop all Items" - SANDRO
	L"Lancemt Grenades grand angle",
	L"Autori. déplcmt silenci. tps réel", // Changed from "Restrict extra Aim Levels" - SANDRO
	L"Espace pour escouade suivante",
	L"Ombres objets",
	L"Afficher portée armes en cases",
	L"Balle Traçante pour tir simple",
	L"Son de pluie",
	L"Afficher corbeaux",
	L"Afficher infobulle soldat",
	L"Sauvegarde auto",
	L"Silence Skyrider !",
	L"EDB (mod rajoutant info utiles)",
	L"Mode tour par tour forcé",			// add forced turn mode
	L"Afficher deviation balle",					// Screen messages showing amount and direction of shot deviation.
	L"Couleur alternative map",		// Change color scheme of Strategic Map
	L"Montrer tirs alternatifs",			// Show alternate bullet graphics (tracers)
	L"Show Merc Ranks",						// shows mercs ranks		// TODO.Translate
	L"Afficher équipments portrait",
	L"Afficher icones portraits",
	L"Désactiver échange curseur",		            // Disable Cursor Swap
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
	L"--Cheat Mode Options--",				// TOPTION_CHEAT_MODE_OPTIONS_HEADER,
	L"Forcer envois Bobby Ray",			// force all pending Bobby Ray shipments
	L"-----------------",					// TOPTION_CHEAT_MODE_OPTIONS_END
	L"--DEBUG OPTIONS--",					// an example options screen options header (pure text)
	L"Barre de progression des stats",					// Show progress towards stat increase		
	L"Réinitialiser TOUTES les options du jeu",				// failsafe show/hide option to reset all options
	L"Voulez-vous vraiment réinitialiser ?",		// a do once and reset self option (button like effect)
	L"Debug Options dans les autres releases",		// allow debugging in release or mapeditor
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
	L"Activez cette option pour entendre vos mercenaires lorsqu'ils parlent.",

	//Mute Confirmation
	L"Active/désactive les confirmations des mercenaires.",

	//Subtitles
	L"Affichage des sous-titres à l'écran.",

	//Key to advance speech
	L"Si les sous-titres s'affichent à l'écran, \ncette option vous permet de prendre le temps de les lire.",

	//Toggle smoke animation
	L"Désactivez cette option si votre machine n'est pas suffisamment puissante.",

	//Blood n Gore
	L"Désactivez cette option si le jeu vous paraît trop violent.",

	//Never move my mouse
	L"Activez cette option pour que le curseur ne se place pas automatiquement sur les boutons qui s'affichent.",

	//Old selection method
	L"Activez cette option pour retrouver vos automatismes de la version précédente.",

	//Show movement path
	L"Activez cette option pour afficher le chemin suivi par les mercenaires. \nVous pouvez la désactiver et utiliser la touche |M|A|J en cours de jeu.",

	//show misses
	L"Activez cette option pour voir où atterrissent tous vos tirs.",
	
	//Real Time Confirmation
	L"Activez cette option pour afficher une confirmation de mouvement en temps réel.",

	//Sleep/Wake notification
	L"Activez cette option pour être mis au courant de l'état de veille de vos mercenaires.",

	//Use the metric system
	L"Activez cette option pour que le jeu utilise le système métrique.",

	//Merc Lighted movement
	L"Activez cette option pour éclairer les environs des mercenaires. (|G) \nDésactivez-le si votre machine n'est pas suffisamment puissante.",

	//Smart cursor
	L"Activez cette option pour que le curseur se positionne directement sur un mercenaire quand il est à proximité.",

	//snap cursor to the door
	L"Activez cette option pour que le curseur se positionne directement sur une porte quand il est à proximité.",

	//glow items 
	L"Activez cette option pour mettre les objets en évidence. (|I)",

	//toggle tree tops
	L"Activez cette option pour afficher le cime des arbres. (|T)",

	//toggle wireframe
	L"Activez cette option pour afficher les murs en fil de fer. (|W)",

	L"Activez cette option pour afficher le curseur toît. (|Home)",

	// Options for 1.13
	L"Si activé, affiche une barre de probabilités de succès sur le curseur.",
	L"Si activé, les rafales de lance grenades ont un curseur de rafale.",
	L"Si activé, les ennemis feront de temps en temps des remarques sur certaines actions.", // Changed from Enemies Drop All Items - SANDRO
	L"Si activé, les grenades lances grenades ont un grand angle (|Q).",
	L"Si activé, le mode tour par tour ne sera pas actif si vous n'êtes pas vu ou entendu par l'ennemie à moins d'appuyer sur |C|t|r+|X.", // Changed from Restrict Extra Aim Levels - SANDRO
	L"Si activé, |E|s|p|a|c|e selectionne l'escouade suivante.",
	L"Si activé, les ombres d'objets sont affichées.",
	L"Si activé, la portée des armes est affichée en nombres de cases.",
	L"Si activé, les effets de traçantes sont affichés pour les tir simples.",
	L"Si activé, le son de pluie est audible quand il pleut.",
	L"Si activé, les corbeaux sont présents dans le jeu.",
	L"Si activé, une fenêtre info-bulle apparaît lorsque vous appuyez sur |A|l|t et que le curseur est au dessus d'un ennemi.",
	L"Si activé, le jeu est sauvegardé à chaque nouveau tour du joueur.",
	L"Si activé, les confirmations insistantes de Skyrider cessent.",
	L"Si activé, l'EDB sera affiché pour les armes et objets.",
	L"Si cette option est activée et que des ennemis sont présents, \nle mode tour par tour est actif tant qu'il reste \ndes ennemis dans le secteur (|C|T|R|L+|S|H|I|F|T+|A|L|T+|T).",	// add forced turn mode
	L"Si activé, montre la progression des caractères à travers le gain de niveaux.",
	L"Si activé, la carte stratégique sera coloré différemment selon l'exploration.",
	L"Si activé, le graphisme des tirs alternatifs sera affiché quand vous tirerez.",
	L"When ON, ranks will be displayed before merc names in the strategic view.",		// TODO.Translate
	L"Si activé, vous verrez l'équipement du mercenaire à travers son portrait.",
	L"Si activé, vous verrez les icones correspondant à l'équipement porté en bas à droite du portrait.",
	L"Si activé, le curseur ne basculera pas entre changer de position et une autre action. Appuyez sur |x pour initier un échange rapide.",
	L"When ON, mercs will not report progress during training.",
	L"When ON, mercs will not report progress during repairing.",	// TODO.Translate
	L"When ON, mercs will not report progress during doctoring.",	// TODO.Translate
	L"When ON, AI turns will be much faster.",		// TODO.Translate
#ifdef ENABLE_ZOMBIES
	L"When ON, zombies will span. Be aware!",							// allow zombies	// TODO.Translate
#endif
	L"When ON, enables popup boxes that appear when you left click on empty merc inventory slots while viewing sector inventory in mapscreen.",	// TODO.Translate
	L"When ON, approximate locations of the last enemies in the sector are highlighted.",  //TODO.Translate
	L"When ON, show the contents of an LBE item, otherwise show the regular NAS interface.", //TODO.Translate
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_HEADER",
	L"Forcer tous les envois en attente de Bobby Ray",
	L"(text not rendered)TOPTION_CHEAT_MODE_OPTIONS_END",
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_HEADER",			// an example options screen options header (pure text)
	L"|H|A|M |4 |D|e|b|u|g: Si activé, annoncera la distance de déviation de chaque tir à partir\ndu centre de la cible, en prenant en compte tous les facteurs du NCTH.",
	L"Click me to fix corrupt game settings",							// failsafe show/hide option to reset all options
	L"Click me to fix corrupt game settings",							// a do once and reset self option (button like effect)
	L"Autoriser les options de debug dans les releases ou les mapeditor",				// allow debugging in release or mapeditor
	L"Toggle to display debugging render options",						// an example option that will show/hide other options
	L"Attempts to display slash-rects around mouse regions",			// an example of a DEBUG build option
	L"(text not rendered)TOPTION_DEBUG_MODE_OPTIONS_END",				// an example options screen options divider (pure text)


	// this is THE LAST option that exists (debug the options screen, doesnt do anything, except exist)
	L"TOPTION_LAST_OPTION",
};


STR16	gzGIOScreenText[] =
{
	L"CONFIGURATION DU JEU",
#ifdef JA2UB
	L"Random Manuel texts ",
	L"Off",
	L"On",
#else
	L"Style de jeu",
	L"Réaliste",
	L"S-F",
#endif	
	L"Platinum", //Placeholder English
	L"Armes disponibles",
	L"Toutes",
	L"Quelques unes",
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
	L"Inventaire / Accessoire",
	L"NON UTILISE",
	L"NON UTILISE",
	L"Charge jeu multi",
	L"CONFIGURATION DU JEU (Les paramètres serveur seulement prennent effet)",
	// Added by SANDRO
	L"Skill Traits",
	L"Ancien",
	L"Nouveau",
	L"Nombre max de merc I.M.P",
	L"1",
	L"2",
	L"3",
	L"4",
	L"5",
	L"6",
	L"Objets lachés par les ennemis",
	L"Off",
	L"On",
#ifdef JA2UB
	L"Tex and John",
	L"Random",
	L"All",
#else
	L"Nombre de terroristes",
	L"Aléatoire",
	L"Tout",
#endif	
	L"Cachettes d'armes secrètes",
	L"Aléatoire",
	L"Toutes",
	L"Progression des objets choisis",
	L"Très lente",
	L"Lente",
	L"Normal",
	L"Rapide",
	L"Très rapide",
	L"Ancien / Ancien",
	L"Nouveau / Ancien",
	L"Nouveau / Nouveau",

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
	L"Weapon Overheating",
	L"Food System",//TODO.Translate
	L"Quantité de Bobby Ray",//TODO.Translate
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

	L"HEBERGER",
	L"Entrer '127.0.0.1' pour l'IP et un nombre plus grand que 60000 pour le port.",
	L"Assurez vous que les ports (UDP, TCP) sont ouverts sur votre routeur. Pour plus d'informations visitez : http://portforward.com",
	L"Vous devez envoyer (via IRC, MSN, ICQ, etc) votre IP externe (http://www.whatismyip.com) et votre numéro de port aux autres joueurs.",
	L"Cliquez sur 'Héberger' pour héberger une nouvelle partie en multijoueurs.",
	
	L"REJOINDRE",
	L"L'hébergeur doit vous envoyer (via IRC, MSN, ICQ, etc) son IP externe ansi que son numéro de port.",
	L"Entrez l'IP externe ainsi que le port du serveur.",
	L"Cliquer sur 'Rejoindre' pour rejoindre une partie multijoueurs déjà existante.",
};

STR16	gzMPHScreenText[] =
{
	L"HOST GAME",
	L"Commencer",
	L"Annuler",
	L"Nom du serveur",
	L"Type de jeu",
	L"Deathmatch",
	L"Team-Deathmatch",
	L"Co-Operative",
	L"Joueurs max.",
	L"Mercs max.",
	L"Sélection mercenaire",
	L"Mercenaire embauché",
	L"Embauché par les joueurs",
	L"Départ avec argent",
	L"Autoriser l'embauche d'un même mercenaire",
	L"Reporter les mercenaires embauchés",
	L"Bobby Rays",
	L"Sector Starting Edge",
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
	L"Maximum d'ennemies en CO-OP",
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
	L"Metavira",		//The island of Metavira was the fictional location used by JA1
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
	L"Etes-vous sûr de vous ? Une valeur de ZERO signifie que vous serez INCAPABLE d'utiliser cette compétence.",
};

STR16 pIMPBeginScreenStrings[] =
{
	L"( 8 Caractères Max )",
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
	L"Sauvegarde rapide", //The name of the quicksave file (filename, text reference)
	L"Partie",	//The name of the normal savegame file, such as SaveGame01, SaveGame02, etc.
	L"sav",				//The 3 character dos extension (represents sav)
	L"..\\SavedGames", //The name of the directory where games are saved.
	L"Jour",
	L"Mercs",
	L"Vide", //An empty save game slot
	L"Démo",				//Demo of JA2
	L"Debug",				//State of development of a project (JA2) that is a debug build
	L"Version",			//Release build for JA2
	L"bpm",					//Abbreviation for Rounds per minute -- the potential # of bullets fired in a minute.
	L"min",					//Abbreviation for minute.
	L"m",						//One character abbreviation for meter (metric distance measurement unit).
	L"balles",				//Abbreviation for rounds (# of bullets)
	L"kg",					//Abbreviation for kilogram (metric weight measurement unit)
	L"lb",					//Abbreviation for pounds (Imperial weight measurement unit)
	L"Accueil",				//Home as in homepage on the internet.
	L"USD",					//Abbreviation to US dollars
	L"n/a",					//Lowercase acronym for not applicable.
	L"Entre-temps",		//Meanwhile
	L"%s est arrivé dans le secteur %s%s", //Name/Squad has arrived in sector A9.  Order must not change without notifying
																		//SirTech
	L"Version",
	L"Emplacement de sauvegarde rapide vide",
	L"Cet emplacement est réservé aux sauvegardes rapides effectuées depuis l'écran tactique (ALT+S).",
	L"Ouverte",
	L"Fermée",
	L"Espace disque insuffisant. Il ne vous reste que %s Mo de libre et Jagged Alliance 2 nécessite %s Mo.",
	L"%s embauché(e) sur le site AIM", 
	L"%s prend %s.",		//'Merc name' has caught 'item' -- let SirTech know if name comes after item.
	L"%s a pris la drogue.", //'Merc name' has taken the drug
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
	L" combiné]",

	//Cheat modes
	L"Triche niveau 1",
	L"Triche niveau 2",

	//Toggling various stealth modes
	L"Escouade en mode discretion.",
	L"Escouade en mode normal.",
	L"%s en mode discretion.",
	L"%s en mode normal.",

	//Wireframes are shown through buildings to reveal doors and windows that can't otherwise be seen in 
	//an isometric engine.  You can toggle this mode freely in the game.
	L"Fil de fer activé",
	L"Fil de fer désactivé",

	//These are used in the cheat modes for changing levels in the game.  Going from a basement level to
	//an upper level, etc.  
	L"Impossible de remonter...",
	L"Pas de niveau inférieur...",
	L"Entrée dans le sous-sol %d...",
	L"Sortie du sous-sol...",

	L"'s",		// used in the shop keeper inteface to mark the ownership of the item eg Red's gun
	L"Mode poursuite désactivé.",
	L"Mode poursuite activé.",
	L"Curseur Toît désactivé.",
	L"Curseur Toît activé.",
	L"Escouade %d active.",
	L"Vous ne pouvez pas payer le salaire de %s qui se monte à %s",	//first %s is the mercs name, the seconds is a string containing the salary
	L"Passer",
	L"%s ne peut sortir seul.",
	L"Une sauvegarde a été crée (Partie249.sav). Renommez-la (Partie01 - Partie10) pour pouvoir la charger ultérieurement.",
	L"%s a bu %s",
	L"Un colis vient d'arriver à Drassen.",
 	L"%s devrait arriver au point d'entrée (secteur %s) au jour %d vers %s.",		//first %s is mercs name, next is the sector location and name where they will be arriving in, lastely is the day an the time of arrival
	L"Historique mis à jour.",
	L"Curseur de visée pour raf.Gre. (Dispersion activée)",
	L"Curseur de trajectoire raf.gre. (Dispersion desact.)",
	L"Enabled Soldier Tooltips", // Changed from Drop All On - SANDRO
	L"Disabled Soldier Tooltips", // Changed from Drop All Off - SANDRO
	L"Angles standards pour lance-grenades",
	L"Lance-grenades grands angles",
	// forced turn mode strings
    L"Mode tour par tour forcé",
	L"Mode tour par tour normal",
	L"Mode de combat quitté",
	L"Mode tour par tour forcé activé, mode de combat activé",
	L"Partie enregistrée dans l'emplacement de sauvegarde automatique.",
	L"..\\SavedGames\\MP_SavedGames", //The name of the directory where games are saved.
	L"Client",
	
	L"Vous ne pouvez pas utiliser l'ancien système d'inventaire et le nouveau système d'accessoire en même temps.",
	
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
	L"Locking mouse cursor to stay within window boundary.",			// 104
	L"Releasing mouse cursor to move outside window boundary.",			// 105
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
	L"Allez sur le site de l'AIM et engagez des mercenaires ( *Truc* allez voir dans le Poste de travail)", // to inform the player to hired some mercs to get things going
	L"Cliquez sur le bouton de Compression du temps pour faire avancer votre équipe sur le terrain.", // to inform the player to hit time compression to get the game underway
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
	L"Afficher les infos de l'équipe",
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
	L"Courtiers d'Assurance des Mercenaires de l'A.I.M.",
	//New Bookmarks
	L"",
	L"Encyclopedia",
	L"Briefing Room",
};


STR16	gzHelpScreenText[] =
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
	L"%s entend une EXPLOSION %s %s.",
	L"%s entend un CRI %s %s.",
	L"%s entend un IMPACT %s %s.",
	L"%s entend un IMPACT %s %s.",
	L"%s entend un BRUIT %s %s.",
	L"%s entend un BRUIT %s %s.",
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
	L"Co-designer/Ecrivain",							// Shaun Lyng
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
	L"%s a terminé la réparation de ses objets",
	L"%s a terminé la réparation des armes & protections",
	L"%s a terminé la réparation des objets portés",
	L"%s a fini de réparer les grands objets portés par chacun",
	L"%s a fini de réparer les moyens objets portés par chacun",
	L"%s a fini de réparer les petits objets portés par chacun",
	L"%s a fini de réparer le mécanisme LBE de chacun",
};

STR16 zGioDifConfirmText[]=
{
	L"Vous avez choisi le mode de difficulté NOVICE. Ce mode de jeu est conseillé pour les joueurs qui découvrent Jagged Alliance, qui n'ont pas l'habitude de jouer à des jeux de stratégie ou qui souhaitent que les combats ne durent pas trop longtemps. Ce choix influe sur de nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode Novice ?",
	L"Vous avez choisi le mode de difficulté EXPERIMENTE. Ce mode de jeu est conseillé pour les joueurs qui ont déjà joué à Jagged Alliance ou des jeux de stratégie. Ce choix influe sur de nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode Expérimenté ?",
	L"Vous avez choisi le mode de difficulté EXPERT. Vous aurez été prévenu. Ne venez pas vous plaindre si vos mercenaires quittent Arulco dans un cerceuil. Ce choix influe sur de nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode Expert ?",
	L"Vous avez choisi le mode de difficulté INCROYABLE. ATTENTION: Ne venez pas vous plaindre si vos mercenaires quittent Arulco en petits morceaux... Deidranna va vous tuer. A coup sûr.  Ce choix influe sur de nombreux paramètres du jeu. Etes-vous certain de vouloir jouer en mode INCROYABLE ?",
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
	L"Le %s n'a plus de carburant ; le véhicule est bloqué à %c%d.",

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
	L"%s a été touché aux jambes ! Il ne peut plus tenir debout !",
	//Name can't speak right now.
	L"%s ne peut pas parler pour le moment.",

	//22-24 plural versions 
	L"%d miliciens ont été promus vétérans.",
	L"%d miliciens ont été promus soldats.",
	L"%d soldats ont été promus vétérans.",

	//25
	L"Echanger",

	//26
	//Name has gone psycho -- when the game forces the player into burstmode (certain unstable characters)
	L"%s est devenu dingue !",

	//27-28
	//Messages why a player can't time compress.
	L"Nous vous déconseillons d'utiliser la Compression du temps ; vous avez des mercenaires dans le secteur %s.",
	L"Nous vous déconseillons d'utiliser la Compression du temps lorsque vos mercenaires se trouvent dans des mines infestées de créatures.",

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
	L"%s n'a plus de trousse à outils !",
	L"%s n'a aucune compétence en mécanique !",

	//44-45
	L"Temps de réparation",
	L"%s ne peut pas voir cette personne.",

	//46-48
	L"Le prolongateur de %s est tombé !",
	L"Pas plus de %d personnes pour former la milice sont autorisées dans ce secteur.",	
    L"Etes-vous sûr ?",

	//49-50
	L"Compression du temps",
	L"Le réservoir est plein.",

	//51-52 Fast help text in mapscreen.
	L"Compression du temps (|E|s|p|a|c|e)",
	L"Arrêt de la Compression du temps (|E|c|h|a|p)",

	//53-54 "Magic has unjammed the Glock 18" or "Magic has unjammed Raven's H&K G11" 
	L"%s a désenrayé le %s",
	L"%s a désenrayé le %s de %s",//inverted !!! magic has unjammed the g11 of raven

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

	L"%s: attachement de protection détruit !",
	L"%s tire %d fois de plus que prévu !",
	L"%s tire 1 fois de plus que prévu !",

	L"You need to close the item description box first!",	// TODO.Translate
};

STR16 gzCWStrings[] = 
{
	L"Faut-il appelez des renforts pour %s dans les secteurs adjacents ?",
};

// WANNE: Tooltips
STR16 gzTooltipStrings[] =
{
	// Debug info
	L"%s|Emplacement: %d\n",
	L"%s|Luminosité: %d / %d\n",
	L"%s|Distance de la |Cible: %d\n",
	L"%s|I|D: %d\n",
	L"%s|Ordres: %d\n",
	L"%s|Attitude: %d\n",
	L"%s|P|A |Actuel: %d\n",
	L"%s|Santé |Actuel: %d\n",
	// Full info
	L"%s|Casque: %s\n",
	L"%s|Veste: %s\n",
	L"%s|Pantalon: %s\n",
	// Limited, Basic
	L"|Armure: ",
	L"Casque ",
	L"Veste ",
	L"Pantalon",
	L"usé",
	L"pas d'armure",
	L"%s|L|V|N: %s\n",
	L"Pas de lunette de vision de nuit",
	L"%s|Masque |à |Gaz: %s\n",
	L"pas de masque à gaz",
	L"%s|Position |tête |1: %s\n",
	L"%s|Position |tête |2: %s\n",
	L"\n(dans le sac de transport) ",
	L"%s|Arme: %s ",
	L"pas d'arme",
	L"Pistolet",
	L"SMG",
	L"Fusil",
	L"MG",
	L"Fusil à pompe",
	L"Couteau",
	L"Armes lourdes",
	L"pas de casque",
	L"pas de veste",
	L"pas de pantalon",
	L"|Armure: %s\n", 
	 // Added - SANDRO
	L"%s|Compétence 1: %s\n",
	L"%s|Compétence 2: %s\n", 
	L"%s|Compétence 3: %s\n",
};

STR16 New113Message[] =
{
	L"La tempête débute .",
	L"Tempête finie.",
	L"Il commence à pleuvoir.",
	L"La pluie cesse.",
	L"Attention aux tireurs isolés...",
	L"Tir de suppression !",	
	L"RAF.",
	L"AUTO",
	L"LG",
	L"RAF. LG",
	L"LG AUTO",
	L"UB",			// TODO.Translate
	L"UBRST",
	L"UAUTO",
	L"Tireur embusqué !",
	L"Unable to split money due to having an item on your cursor.",
	L"Arrivée de nouvelles recrues est déroutée au secteur %s, car le point d'arrivée prévu %s est sous contrôle ennemi.",
	L"Article supprimé",
	L"A supprimé tous les articles de ce type",
	L"Article vendu",
	L"A vendu tous les articles de ce type",
	L"Vous devriez vérifier si votre accessoire de vision convient bien à ce type de lieu",
	// Real Time Mode messages
	L"Encore en combat",
	L"pas d'ennemie en vue",
	L"Mode silencieux en temps réel désactivé",
	L"Mode silencieux en temps réel activé",
	L"Ennemis repérés !", // this should be enough - SANDRO
	//////////////////////////////////////////////////////////////////////////////////////
	// These added by SANDRO
	L"%s a réussi son vol !",
	L"%s n'avait pas assez de points d'action pour voler tous les articles choisis.",
	L"Voulez-vous faire de la chirurgie sur %s avant de le bander ? (Vous pouvez lui guérir %i Santé.)",
	L"Voulez-vous faire de la chirurgie sur %s ? (Vous pouvez lui guérir %i Santé.)",
	L"Voulez-vous lui faire les premiers soins d'abord ? (%i patient(s))",
	L"Voulez-vous faire les premiers soins sur ce patient d'abord ?",
	L"Appliquez les premiers soins automatiquement avec chirurgie ou sans ?",
	L"La chirurgie sur %s est finie.",
	L"%s est touché au torse et perd un maximum de points de vie !",
	L"%s est touché au torse et perd %d points de vie !",
	L"%s a regagné 1 point sur les %s perdus",
	L"%s a regagné %d points sur les %s perdus",
	L"Vos compétences de reconnaissance vous ont empêchés d'être pris en embuscade par l'ennemi !",
	L"Grâce à vos compétences de reconnaissance vous avez réussi à éviter un groupe de félins !",
	L"%s est frappé à l'aine et tombe dans la douleur !",
	//////////////////////////////////////////////////////////////////////////////////////
	L"Attention: Cadavre ennemi trouvé !!!",
	L"%s [%d rnds]\n%s %1.1f %s",
	L"Insufficient AP Points! Cost %d, you have %d.",	// TODO.Translate
};

STR16 New113HAMMessage[] = 
{
	// 0 - 5
	L"%s sombre dans la peur !",
	L"%s est cloué au sol !",
	L"%s tire plus de fois que désiré !",
	L"Vous ne pouvez pas former de milice dans ce secteur.",
	L"La milice prend %s.",
	L"Vous ne pouvez pas former de milice alors que des ennemies sont présents !", 
	// 6 - 10
	L"Manque %s points en Commandement pour former la milice.",
	L"Pas plus de %d milices mobiles peuvent être entrainées dans ce secteur.",
	L"Aucune pièce dans %s ou autours pour des nouvelles milices mobiles !",
	L"Vous devez avoir %d villes de milice dans chaque secteur libérée de %s  pour pouvoir former une milice mobile.",
	L"Ne peut pas diriger une installation tant que les ennemies sont présents !",
	// 11 - 15
	L"Manque de %s en sagesse pour pourvoir installer des équipements destinés au personnel.",
	L"%s est déja entièrement pourvu en personnel.",
	L"Cela va coûter %d$ par heure pour construire cette infrastucture. Voulez-vous continuer ?",
	L"Vous n'avez pas assez d'argents pour payer toute les infrastructures aujourd'hui.%d $ ont déjà été payés, mais vous devez encore %d $. Les habitants ne sont pas patients...",
	L"Vous n'avez pas assez d'argents pour payer toute les infrastructures aujourd'hui. Vous devez %d$. Les habitants ne sont pas patients...",
	// 16 - 20
	L"Vous avez une dette échue de %d $ pour l'opération d'installation et aucun argent pour le régler !",
	L"Vous avez une dette échue de %d $ pour l'opération d'installation. Vous ne pouvez pas assigner ce merc au installation avant que vous n'ayez assez d'argent pour régler la dette entière.",
	L"Vous avez une dette échue de %d $ pour l'opération d'installation. Voulez-vous tout payer d'un coup ?",
	L"N/A est dans ce secteur",
	L"Dépenses quotidiennes",
	// 21 - 25
	L"Fonds insuffisants pour payer toute la milice enrôlée ! %d milices se sont dissolu et sont rentrés chez eux.",

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
	L"Nous vous informons que de par ses perfomances passées, Gaston voit ses honoraires augmentés. Personellement, je ne suis pas surpris. ± ± Speck T. Kline ± ",
	// Stogie: Text from Line 43 in Email.edt
	L"Soyez informé que, à paritr de maintenant, le honoraires de Stogie ont augmentés en accord avec ses compétences. ± ± Speck T. Kline ± ", 
	// Tex: Text from Line 45 in Email.edt
	L"Sachez que l'experience de Text lui autorisent une promotion. Son salaire a donc été ajusté pour refleter sa valeur équitablement. ± ± Speck T. Kline ± ",
	// Biggens: Text from Line 49 in Email.edt
	L"Prenez note. De par des performances accrues Biggens voit le prix de ses services augmentés. ± ± Speck T. Kline ± ",
};

// WANNE: This is email text (each 2 line), when we left a message on AIM and now the merc is back
STR16	New113AIMMercMailTexts[] =
{
	// Monk
	L"TR du serveur AIM: Message de Victor Kolesnikov",
	L"Salut. Ici Monk. Message reçu. Je suis disponible si vous voulez me voir. ± ± J’attends votre appel. ±",

	// Brain
	L"TR du serveur AIM: Message de Janno Allik",
	L"Je suis prêt à conciderer votre offre. Il y a un temps et un lieu pour tout. ± ± Janno Allik ±",

	// Scream
	L"TR du serveur AIM: Message de Lennart Vilde",
	L"Lennart Vilde est maintenant disponible! ±",

	// Henning
	L"TR du serveur AIM: Message de Henning von Branitz",
	L"J’ai reçu votre message, merci. Pour parler d’embauche, contactez-moi sur le site web de l’AIM. ± ± Henning von Branitz ±",

	// Luc
	L"TR du serveur AIM: Message de Luc Fabre",
	L"Message reçu, merci! Je suis heureux de considérer votre proposition. Vous savez où me trouver. ± ± Au plaisir de vous entendre.±",

	// Laura
	L"TR du serveur AIM: Message de Dr. Laura Colin",
	L"Salutations! Merci pour votre message, il semble intéressant. ± ± Visiter l’AIM à nouveau, je serais ravie d’en entendre plus. ± ± Cordialement! ± ± Dr. Laura Colin ±",

	// Grace
	L"TR du serveur AIM: Message de Graziella Girelli",
	L"Vous vouliez me contacter, mais vous n’avez pas réussi.± ± Une réunion de famille. Je suis sûr que vous comprenez? J’en ai maintenant assez de la famille et serait très heureuse si vous voulez me contacter de nouveau sur le site AIM. ± ± Ciao! ±",

	// Rudolf
	L"TR du serveur AIM: Message de Rudolf Steiger",
	L"Vous savez combien j’ai d’appel par jour ? Tous les branleurs pensent pouvoir m’appeler. ± ± Mais je suis de retour, si vous avez quelque chose d’intéressant pour moi. ±",

	// WANNE: Generic mail, for additional merc made by modders, index >= 178
	L"TR du serveur AIM: Message des disponibilités des mercs",
	L"J'ai reçu votre message. J'attends votre appel. ±",
};

// WANNE: These are the missing skills from the impass.edt file
// INFO: Do not replace the ± characters. They indicate the <B2> (-> Newline) from the edt files
STR16 MissingIMPSkillsDescriptions[] =
{
	// Sniper
	L"Tireur isolé : Des yeux de faucon, vous pouvez tirer les ailes d'une mouche à cent mètres! ± ",
	// Camouflage
	L"Camouflage : Sans compter que derrière vous même les buissons semblent synthétiques! ± ",	
	// SANDRO - new strings for new traits added
	// Ranger
	L"Soldat: Vous êtes celui du déserts du Texas, n'est-ce pas ! ± ",	
	// Gunslinger
	L"Bandit: Avec un pistolet ou deux, vous pouvez être aussi mortel que Billy the Kid ! ± ",
	// Squadleader
	L"Commandant: Naturel leader et commandant, vous êtes le gros calibre, sans blague ! ± ",
	// Technician
	L"Technicien: Fixer les objets, retirer les pièges, planter les bombes, c'est ça votre boulot ! ± ",
	// Doctor
	L"Docteur: Vous pouvez faire une intervention chirurgicale avec un couteau Suisse et un chewing gum et cela n'importe où ! ± ",
	// Athletics
	L"Athletic: Votre vitesse et votre vitalitée sont au top des possibilitées actuelles! ± ",
	// Bodybuilding
	L"Culturiste: Cette grande figure musclée qui ne peut pas être dominée est en faîte vous en réalité ! ± ",
	// Demolitions
	L"Démolition: Vous pouvez réduire à néant toute une ville rien qu'avec des produits ménagers ! ± ",
	// Scouting
	L"Reconnaissance: Rien n'échappe à votre vigilance ! ± ",
	// Covert ops
	L"Covert Operations: You make 007 look like an amateur! ± ",		// TODO.Translate
};

STR16 NewInvMessage[] = 
{
	L"Le sac à dos ne peut être ramassé pour le moment",
	L"Pas de place pour le sac à dos",
	L"Sac à dos non trouvé",
	L"La fermeture éclair fonctionne seulement en combat",
	L"Ne peut se déplacer si la fermeture éclair est ouverte",
	L"Êtes vous sûr de vouloir voulez vendre tous les articles du secteur ?",
	L"Êtes vous sûr de vouloir supprimer tous les articles du secteur ?",
	L"Ne peut pas escalader avec un sac à dos",
};

// WANNE - MP: Multiplayer messages
STR16 MPServerMessage[] =
{
	// 0
	L"Initialisation du serveur RakNet..",
	L"Le serveur a démarré, en attente de connection...",
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
	L"Utilisez la vue aérienne pour sélectionner la carte que vous voulez jouer. Si vous voulez changer de carte, vous devez le faire avant de cliquer sur 'Démarrer la partie'.",
};

STR16 MPClientMessage[] =
{
	// 0
	L"Initialisation du client RakNet...",		
	L"Connection à l'IP: %S ...",
	L"Réception des optiosn de jeu:",
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
	L"Vous êtes prêt. En attente des autre clients... Cliquez sur 'OK' si vous n'êtes plus prêt.",
	L"Laissez-nous, la bataille commence !",
	L"Un client doit poser sa candidature pour démarrer la partie.",
	// 15
	L"Le jeu ne peut démarrer. Aucun mercenaire n'a été engagé...",
	L"En attente de 'OK' de la part du serveur pour ouvrir le portable...",
	L"Interrompu",
	L"Fin de l'interromption",
	L"Coordonnées de réseau de souris :",
	// 20
	L"X: %d, Y: %d",
	L"Réseau numéro: %d",
	L"Le serveur figure seulement",
	L"Choissiez les étapes à ignorer:  ('1' - Activer portable/l'embauche)  ('2' - lancer/charger level)  ('3' - Unlock UI)  ('4' - placement de finition)",	
	L"Secteur=%s, Clients max.=%d, Mercs max.=%d, Game_Mode=%d, Same Merc=%d, Multiplicateur de Dégâts=%f, Timed Turns=%d, Secs/Tic=%d, Dis BobbyRay=%d, Dis Aim/Merc Equip=%d, Dis Morale=%d, Testing=%d",
	// 25
	L"",
	L"Nouvelle conncetion: Client #%d - '%S'.",
	L"Equipe: %d.",//not used any more
	L"'%s' (client %d - '%S') a été tué par '%s' (client %d - '%S')",
	L"Client kické #%d - '%S'",
	// 30
	L"Début de manche pour les numéros de clients: #1 - '%S' | #2 - '%S' | #3 - '%S' | #4 - '%S'",
	L"Début de manche pour le client #%d",
	L"Requête pour le realtime...",
	L"Commutation en mode realtime.",
	L"Erreur lors de la commutation.",
	// 35
	L"Dévérouiller le portable pour l'embauche ? (Tous les clients sont connectés?)",
	L"Le serveur a dévérouillé le portable pour l'embauche. Vous pouvez commencez a embauché !",
	L"Interruption.",
	L"Vous ne pouvez pas changer la dropzone si vous êtes seulement un client et pas le gérant du serveur.",
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
	L"Connection échouée, reconnection dans 5 s. Encore %i tentatives...",
	//50
	L"Connection échouée, abandon de l'opération...",
	L"Vous ne pouvez pas démarrer la partie tant qu'un autre joueur ne s'est pas connecté",
	L"%s : %s",
	L"Envoyer à tous",
	L"Alliés seulement",
	// 55
	L"Vous ne pouvez pas rejoindre cette partie car elle a déjà commencé.",
	L"%s (equipe): %s",
	L"#%i - '%s'",
	L"%S - 100/100",
	L"%S - %i/100",
	// 60
	L"Réception de tous les fichiers depuis le serveur.",
	L"'%S' a fini de télécharger depuis le serveur.",
	L"'%S' a commencé à télécharger depuis le serveur.",
	L"Vous ne pouvez pas démarrer le jeu tant que tout les joueurs n'ont pas finit de recevoir les fichiers",
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
	L"Le client n'as pas réussi à démarrer. Terminé.",
	L"Le client s'est déconnecté et s'est fermé.",
	L"Le client n'est pas démarré.",
	L"INFO: Si le jeu est bloqué (la barre de progression des adversaires ne se déplace pas), notifier le au serveur en appuyant sur ALT + E pour aller directement à votre tour de jeu !",
	// 80
	L"AI's turn - %d left",		// TODO.Translate
};

STR16 gszMPEdgesText[] =
{
	L"N",
	L"S",
	L"E",
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
	L"Mercenaire pris : ",
	L"Vous ne pouvez pas changer le bord de départ tant que le portable est ouvert.",
	L"Vous ne pouvez pas changer d'équipe tant que le portable est ouvert.",
	L"Mercenaire aléatoire : ",
	L"Y",
	L"Difficulté :",
	L"Version Serveur :",
};

STR16 gzMPSScreenText[] =
{
	L"Tableaux des scores",
	L"Continue",
	L"Annulé",
	L"Joueurs",
	L"Tués",
	L"Morts",
	L"Armée de la reine",
	L"Touchés",
	L"Ratés",
	L"Précision",
	L"Dégâts faits",
	L"Dégâts reçus",
	L"Attendez le serveur avant d'appuyer sur 'Continue'."
};

STR16 gzMPCScreenText[] =
{
	L"Annulé",
	L"Connection au serveur",
	L"Obtention des options du serveur",
	L"Téléchargement des fichiers modifiés",
	L"Appuyer sur 'ESC' pour annulé ou 'Y' pour parler",
	L"Appuyer sur 'ESC' pour annulé",
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
	L"'ENTRER' pour envoyer, 'ESC' pour annuler",
};

// Following strings added - SANDRO
STR16 pSkillTraitBeginIMPStrings[] =
{
	// For old traits
	L"A la page suivante, vous allez choisir vos traits de compétence selon votre spécialisation professionnel comme un mercenaire. Pas plus de deux traits différents ou un trait expert peuvent être choisis.",
	L"Vous pouvez aussi choisir seulement un ou même aucun trait, ce qui vous donnera un bonus à vos points d'attributs, une sorte de compensation. Notez que les compétences : mécanique, ambidextre et camouflage ne peuvent pas être prises aux niveaux experts.",
	// For new major/minor traits
	L"L'étape suivante est le choix de vos traits de compétences. A la première page vous pouvez choisir jusqu'à deux traits principaux, qui représentent surtout votre rôle dans une équipe. Tandis qu'à la deuxième page, c'est la liste de vos traits mineurs, qui représentent des exploits personnels.",
	L"Pas plus de trois choix au total sont possibles. Ce qui signifie que si vous ne choisissez aucun trait principal, vous pourrez alors choisir trois traits secondaires. Si vous choisissez deux traits principaux (ou un en expert), vous pourrez alors choisir qu'un seul trait secondaire...",
};

STR16 sgAttributeSelectionText[] = 
{
	L"Ajustez s'il vous plaît vos attributs physiques selon vos vraies capacités. Vous ne pouvez pas augmenter les scores au-dessus de",
	L"Examen des attributs et compétences de l'I.M.P.",
	L"Points bonus :",
	L"Départ au niveau",
	// New strings for new traits
	L"A la page suivante vous allez spécifier vos attributs physiques comme : la santé, la dextérité, l'agilité, la force et la sagesse. Les attributs ne peuvent pas aller plus bas que %d.",
	L"Le reste est appelé 'habilitées' et à la différence des attributs, ils peuvent être mis à zéro signifiant que vous serez un incapable dans cette habilitée !",
	L"Tous les scores sont mis à un minimum au début. Notez que certains attributs sont mis a des valeurs spécifiques correspondant aux traits de compétence que vous avez choisis. Vous ne pouvez pas mettre ces attributs plus bas.",
};

STR16 pCharacterTraitBeginIMPStrings[] =
{
	L"Analyse du cacractère I.M.P.",
	L"L'analyse de votre personnage est la prochaine étape. À la première page on vous montrera une liste d'attitudes à choisir. Nous imaginons bien que vous pourriez vous identifier à plusieurs d'entre elles, mais vous ne pourrez en choisir qu'une seule. Choisissez celle qui vous correspond le plus. ",
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
	L"Attitudes I.M.P.",
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
	L"Traits de caractère I.M.P.",
};

STR16 gzIMPColorChoosingText[] = 
{
	L"Teinte et musculature I.M.P.",
	L"Couleurs I.M.P.",
	L"Choisissez les couleurs respectives de votre peau, vos cheveux et vos vêtements. Ainsi que votre physionomie (traits physiques). ",
	L"Choisissez les couleurs respectives de votre peau, vos cheveux et vos vêtements.",
	L"Cocher içi pour utiliser une prise en main alternative du fusil.",
	L"\n(Attention: vous devez avoir une grande force pour utiliser cela.)",
};

STR16 sColorChoiceExplanationTexts[]=
{
	L"Cheveux",
	L"Teinte",
	L"T-shirt",
	L"Pantalon",
	L"Corps normal",
	L"Corps musclé",
};

STR16 gzIMPDisabilityTraitText[]=
{
	L"Pas d'handicap",
	L"Intolérance à la chaleur",
	L"Nerveux",
	L"Claustrophobe",
	L"Mauvais nageur",
	L"Peur des insectes",
	L"Distrait",
	L"Psychotique",
	L"Handicaps I.M.P.",
};

// HEADROCK HAM 3.6: Error strings for assigning a merc to a facility
STR16 gzFacilityErrorMessage[]=
{
	L"%s n'as pas assez de force pour accomplir cette tâche.",
	L"%s n'as pas assez de dextérité pour accomplir cette tâche.",
	L"%s n'est pas assez agile pour accomplir cette tâche.",
	L"%s n'est pas assez en bonne santé pour accomplir cette tâche.",
	L"%s n'as pas assez de sagesse pour accomplir cette tâche.",
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
	L"Loyauté insuffisante à %s. Les habitants refusent de vous permettre de faire de cette tâche.",
	L"Trop de gens sont déjà en train de travailler à %s.",
	L"Trop de gens sont déjà en train de faire cette tâche à %s.",
	// 16 - 20
	L"%s n'as pas trouvé d'objets à réparer.",
	L"%s a perdu %s alors qu'il travaillait dans le secteur %s !",
	L"%s a perdu %s alors qu'il travaillait sur %s à %s !",
	L"%s a été blessé alors qu'il travaillait dans le secteur %s et nécessite des soins médicaux immédiats !",
	L"%s a été blessé alors qu'il travaillait %s à %s et nécessite des soins médicaux immédiats !",
	// 21 - 25
	L"%s a été blessé alors qu'il travaillait dans le secteur %s. Il ne semble pas être en trop mauvais état.",
	L"%s a été blessé alors qu'il travaillait sur %s à %s. Il ne semble pas être en trop mauvais état.",
	L"Les résidents de %s semblent être excédés par la présence de %s.",
	L"Les résidents de %s semblent être excédés par le travail de %s sur %s.",
	L"Les actions de %s dans le secteur %s ont causé une perte de loyauté à travers la région !",
	// 26 - 30
	L"Les actions de %s sur %s à %s ont causé une perte de loyauté à travers la région !",
	L"%s est ivre.", // <--- This is a log message string.
	L"%s est devenu gravement malade dans le secteur %s, et commence à manque à son devoir.",
	L"%s est devenu gravement malade et ne peut continuer son travail sur %s à %s.",
	L"%s a été blessé dans le secteur %s.", // <--- This is a log message string.
	// 31 - 35
	L"%s a été sévèrement blessé dans le secteur %s.", //<--- This is a log message string.


};

STR16 gzFacilityRiskResultStrings[]=
{
	L"Force",
	L"Agilité",
	L"Dextérite",
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
	L"Equipe",
	L"Eat",// TODO.Translate
	L"Reste",
	L"Réparer les objets",
	L"Réparer %s", // Vehicle name inserted here
	L"Réparer le Robot",
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
	L"Elève Force",
	L"Elève Dextérité",
	L"Elève Agilité",
	L"Elève Santé",
	L"Elève Tir",
	// 21-25
	L"Elève Médecin",
	L"Elève Mécanique",
	L"Elève Commandement",
	L"Elève Explosif",
	L"Entraineur Force",
	// 26-30
	L"Entraineur Dextérité",
	L"Entraineur Agilité",
	L"Entraineur Santé",
	L"Entraineur Tir",
	L"Entraineur Médecin",
	// 30-35
	L"Entraineur Mécanique",
	L"Entraineur Commandement",
	L"Entraineur Explosif",
	L"Interrogate Prisoners",	// added by Flugente	TODO.Translate
};
STR16 Additional113Text[]=
{
	L"Jagged Alliance 2 v1.13 mode fenêtré exige une profondeur de couleur de 16 bit ou moins.",

	// WANNE: Savegame slots validation against INI file
	L"Internal error in reading %s slots from Savegame: Number of slots in Savegame (%d) differs from defined slots in ja2_options.ini settings (%d)",
	L"Mercenaire (MAX_NUMBER_PLAYER_MERCS) / Vehicle (MAX_NUMBER_PLAYER_VEHICLES)", 
	L"Ennemi (MAX_NUMBER_ENEMIES_IN_TACTICAL)", 
	L"Créature (MAX_NUMBER_CREATURES_IN_TACTICAL)", 
	L"Milice (MAX_NUMBER_MILITIA_IN_TACTICAL)", 
	L"Civil (MAX_NUMBER_CIVS_IN_TACTICAL)",
};

// SANDRO - Taunts (here for now, xml for future, I hope)
STR16 sEnemyTauntsFireGun[]=
{
	L"Suce moi ça !",
	L"Prends toi ça !",
	L"Viens par là !",
	L"T'es à moi !",
	L"Meurs !",
	L"T'as peur enfoiré ?",
	L"Ca va faire mal !",
	L"Viens la batard !",
	L"Allez viens ! J'ai pas toute la vie !",
	L"Vas voir ta mère !",
	L"Je vais t'envoyer sous terre dans peu de temps !",
	L"Retourne chez ta mère, looserr !",
	L"Hey, tu veux jouer ?!",
	L"T'aurais du rester chez toi, p'tit con !",
	L"Enculé !",
};

STR16 sEnemyTauntsFireLauncher[]=
{

	L"On va faire un barbecue ici.",
	L"Un cadeau d'la part du patron !",
	L"Bim Bam Boum !",
	L"Sourit, l'oiseau va sortir !",
};

STR16 sEnemyTauntsThrow[]=
{
	L"Attrape !",
	L"Et c'est partit !",
	L"Prend toi çà !",
	L"Et un pour toi",
	L"Mouhahaha.",
	L"Attrape ça sale porc!",
	L"Sa va faire mal !.",
};

STR16 sEnemyTauntsChargeKnife[]=
{
	L"J'vais découper ton crâne.",
	L"Viens voir papa.",
	L"Montre moi tes couilles !",
	L"Je vais te découper en pièces !",
	L"Hannibal Lecter, ppfft tu me connais pas !",
};

STR16 sEnemyTauntsRunAway[]=
{
	L"On est vraiment dans un grosse merde...",
	L"Ils disent de rejoindre l'armée. Mais pas pour cette merde !",
	L"J'en ai assez.",
	L"Oh my God.",
	L"On n'est pas assez payé pour ça !",
	L"C'est vraiment trop pour moi.",
	L"Je vais rammener quelques potes.",

};

STR16 sEnemyTauntsSeekNoise[]=
{
	L"T'as entendu !",
	L"Qui est là ?",
	L"Qu'est ce que s'est ?",
	L"Hey ! What the...",

};

STR16 sEnemyTauntsAlert[]=
{
	L"Ils sont là !",
	L"Yeah la partie peut commencer.",
	L"J'ai espéré que ceci n'arriverai jamais.",

};

STR16 sEnemyTauntsGotHit[]=
{
	L"Ouch !",
	L"Ugh !",
	L"Ca... ça fait mal !",
	L"You fuck !",
	L"Tu vas le regret.. uhh.. ter ça.",
	L"What the..!",
	L"Maintenant vous... m'avez énervé !",

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
	L"Général",
	L"Avancés",
};

STR16 gzItemDescTabButtonShortText[] =
{
	L"Desc",
	L"Gen",
	L"Ava",
};

STR16 gzItemDescGenHeaders[] =
{
	L"Primaire",
	L"Secondaire",
	L"Coût PA",
	L"Rafale / Auto",
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
	L"|P|a|g|e |d|e |d|e|s|c|r|i|p|t|i|o|n :\n \nMontre des informations textuelles de base sur cet objet.",
	L"|P|r|o|p|r|i|é|t|é|s |g|é|n|é|r|a|l|e|s :\n \nMontre des données spécifiques de cet objet.",
	L"|P|r|o|p|r|i|é|t|é|s |a|v|a|n|c|é|e|s :\n \nMontre des bonus donnés par cet objet.",
};

STR16 gzUDBHeaderTooltipText[]=
{
	L"|P|r|o|p|r|i|é|t|é|s |p|r|i|m|a|i|r|e|s :\n \nPropriétés et données liées à la classe de cet objet\n(Armes / Armures / etc).",
	L"|P|r|o|p|r|i|é|t|é|s |s|e|c|o|n|d|a|i|r|e|s :\n \nLes caractéristiques supplémentaires de cet objet,\net/ou capacités secondaires possibles.",
	L"|C|o|û|t |e|n |P|A :\n \nCoût en PA pour tirer ou manipuler cette arme.",
	L"|R|a|f|a|l|e |/ |A|u|t|o|m|a|t|i|q|u|e :\n \nDonnées liées au tir de cette arme en mode Rafale ou Auto.",
};

STR16 gzUDBGenIndexTooltipText[]=
{
	L"|P|r|o|p|r|i|é|t|é |i|c|ô|n|e\n \nSurvol avec la souris pour révéler le nom de la propriété.",
	L"|V|a|l|e|u|r |b|a|s|i|q|u|e\n \nValeurs de base données par cet objet, excluant\nles bonus ou pénalités lié aux accessoires ou munitions.",
	L"|B|o|n|u|s |d|e|s |a|c|c|e|s|s|o|i|r|e|s\n \nBonus ou pénalités donnée par les munitions ou accessoires.",
	L"|V|a|l|e|u|r |f|i|n|a|l|e\n \nValeur finale donné par cette objet, incluant les\nbonus/pénalités des accessoires et munitions.",
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
	L"|H|a|n|d|l|i|n|g |D|i|f|f|i|c|u|l|t|y",	// TODO.Translate
	L"|N|i|v|e|a|u |d|e |v|i|s|é|e",
	L"|A|g|g|r|a|n|d|i|s|s|e|m|e|n|t |d|e |l|a |l|u|n|e|t|t|e",
	L"|F|a|c|t|e|u|r |d|e |p|r|o|j|e|c|t|i|o|n",
	L"|C|o|u|v|e|r|t|u|r|e |d|u |f|l|a|s|h |d|u |c|a|n|o|n",
	L"|I|n|t|e|n|s|i|t|é",
	L"|F|i|a|b|i|l|i|t|é",
	L"|F|a|c|i|l|i|t|é |d|e |r|é|p|a|r|a|t|i|o|n",
	L"|P|o|r|t|é|e |m|i|n|i|m|u|m |p|o|u|r |b|o|n|u|s |d|e |v|i|s|é|e",
	L"|F|a|c|t|e|u|r |d|e |d|é|g|â|t|s",	
	L"|N|o|m|b|r|e |d|e |P|A| |p|o|u|r |m|e|t|t|r|e |e|n |j|o|u|e",
	L"|N|o|m|b|r|e |d|e |P|A| |p|o|u|r |t|i|r|e|r",
	L"|N|o|m|b|r|e |d|e |P|A| |t|i|r |e|n |R|a|f|a|l|e",
	L"|N|o|m|b|r|e |d|e |P|A| |t|i|r |e|n |A|u|t|o|m|a|t|i|q|u|e",
	L"|N|o|m|b|r|e |d|e |P|A| |p|o|u|r |r|e|c|h|a|r|g|e|r",
	L"|N|o|m|b|r|e |d|e |P|A| |p|o|u|r |r|e|c|h|a|r|g|e|r |m|a|n|u|e|l|l|e|m|e|n|t",
	L"",	// No longer used!
	L"|T|o|t|a|l |R|e|c|o|i|l",		// TODO.Translate
	L"|T|i|r |a|u|t|o|m|a|t|i|q|u|e |p|o|u|r |5 |P|A",
};

STR16 szUDBGenWeaponsStatsExplanationsTooltipText[]=
{
	L"\n \nDétermine si des balles tirées par cette arme dévieront\nloin de l'impact d'origine.\n \nEchelle: 0-100.\nValeur élevée recommandée.",
	L"\n \nDétermine la quantité moyenne de dégâts faits par\ndes balles tirées de cette arme, avant\n de tenir compte de l'armure et de la pénétration d'armure.\n \nValeur élevée recommandée.",
	L"\n \nDistance maximale (en cases) que vont parcourir les balles\ntirées par cette arme avant de redescendre vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nDetermines the difficulty of holding and firing\nthis gun.\n \nHigher Handling Difficulty results in lower\nchance-to-hit - when aimed and especially when\nunaimed.\n \nLower is better.",	// TODO.Translate
	L"\n \nCeci est le nombre de niveau de visée supplémentaire que\nvous pouvez ajouter en visant avec cette arme.\n \nRéduire le nombre de niveau de visée signifie que chaque\nniveau ajoute proportionnellement plus de précision au tir.\nPar conséquent, avoir peu de niveaux de visée vous\npermettera de garder une bonne précision avec une vitesse\nde mise en joue élevée !\n \nValeur faible recommandée.",
	L"\n \nUne valeur plus grande de *1.0, réduit proportionnellement\nles erreurs de visée à distance.\n \nN'oubliez pas qu'un trop gros zoom sur une cible\nproche vous pénalisera !\n \nLa valeur de 1.0 signifie qu'aucune lunette est installée.",
	L"\n \nRéduit proportionnellement les erreurs de visée à distance.\n \nCes effets ne sont valables qu'à une distance donnée,\net se dissipent ou disparaissent\nà une longue distance.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette propriété est en vigueur, l'arme\nne produit pas de flash lors du tir.\n \nLes ennemis ne seront plus en mesure de vous repérer\nà cause du flash du tir (mais ils\npourront toujours vous entendre !).",
	L"\n \nDistance (en cases) de l'intensité sonore que fait votre arme\nlorsque vous tirez avec.\n \nLes ennemis placés en deçà de cette distance entendront\nvotre tir.\n \nValeur faible recommandée.",
	L"\n \nDétermine la vitesse de déterioration de cett arme\nà l'usage.\n \nValeur élevée recommandée.",
	L"\n \nDétermine la difficulté de réparation de cette arme.\n \nValeur élevée recommandée.",
	L"\n \nPortée minimum où la lunette de visée fournit un bonus de visée.",
	L"\n \nFacteur de chance de toucher accordé par un laser.",	
	L"\n \nLe nombre de PA requis pour mettre en joue.\n \nQuand cette arme est prête, vous pouvez tirez plusieurs fois\nsans avoir de coût supplémentaire.\n \nAnnule automatiquement cette opération si vous faîtes des\nactions autre que pivoter ou tirer.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour effectuer\nune attaque simple avec cette arme.\n \nPour les fusils, c'est le coût pour un tir\nsimple sans niveau de visée.\n \nSi cet icone est grisé, les tirs simples\n ne sont pas possible.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour tirer une Rafale.\n \nle nombre de balles tirées pour chaque rafale est\ndéterminé par l'arme elle-même, et indiqué\npar le nombre de balles sur cette icone.\n \nSi cette icone est grisé, le mode rafale \nn'est pas possible avec cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour tirer en Automatique.\n \nSi vous voulez tirez plus de balles,\ncela coûtera plus de PA.\n \nSi cette icone est grisé, le mode Auto\nn'est pas possible avec cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour recharger cette arme.\n \nValeur faible recommandée.",
	L"\n \nLe nombre de PA requis pour recharger cette arme\nentre chaque tir.\n \nValeur faible recommandée.",
	L"",	// No longer used!
	L"\n \nThe total distance this weapon's muzzle will shift\nbetween each and every bullet in a burst or\nautofire volley, if no Counter Force is applied.\n \nLower is better.", // HEADROCK HAM 5: Altered to reflect unified number.	// TODO.Translate
	L"\n \nIndique le nombre de balles qui seront ajoutés\nau mode Auto tous les 5 PA que vous dépensez.\n \nValeur élevée recommandée.",
};

STR16 szUDBGenArmorStatsTooltipText[]=
{
	L"|V|a|l|e|u|r |d|e |p|r|o|t|e|c|t|i|o|n",
	L"|R|e|c|o|u|v|r|e|m|e|n|t",
	L"|T|a|u|x |d|e |d|é|g|r|a|d|a|t|i|o|n",
};

STR16 szUDBGenArmorStatsExplanationsTooltipText[]=
{
	L"\n \nCette propriété d'armure définit de combien elle\nabsorbe les dégâts de chaque attaque.\n \nN'oubliez pas que les attaques perforantes et\ndivers facteurs aléatoires peuvent altérés\nla réduction final des dégâts.\n \nValeur élevée recommandée.",
	L"\n \nDétermine le recouvrement de l'armure\nsur votre corps.\n \nSi le recouvrement est inférieur à 100%, les attaques\nont une certaine chance de passé à travers l'armure\nen causant de un maximum de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nIndique à quelle vitesse les conditions de l'armure\nvont chutés, et qui est proportionnel aux\ndégâts subis.\n \nValeur faible recommandée.",
};

STR16 szUDBGenAmmoStatsTooltipText[]=
{
	L"|T|a|u|x |d|e |p|é|n|é|t|r|a|t|i|o|n",
	L"|C|u|l|b|u|t|e |d|e |b|a|l|l|e",
	L"|P|r|e|-|I|m|p|a|c|t |E|x|p|l|o|s|i|o|n",
	L"|T|e|m|p|e|r|a|t|u|r|e |M|o|d|i|f|i|c|a|t|i|o|n",		// TODO.Translate
	L"|P|o|i|s|o|n |P|e|r|c|e|n|t|a|g|e",	// TODO.Translate
	L"|D|i|r|t |M|o|d|i|f|i|c|a|t|i|o|n",	// TODO.Translate
};

STR16 szUDBGenAmmoStatsExplanationsTooltipText[]=
{
	L"\n \nCeci est la capacité de la balle à pénétrer\nl'armure de la cible.\n \nAvec une valeur supérieure à 1.0, la balle réduiera fortement\nla valeur de protection de l'armure touchée.\n \nValeur élevée recommandée.",
	L"\n \nDétermine le potentiel de la balle à faire des dégâts\nsur le corps après avoir traversée l'armure.\n \nAvec une valeur supérieure à 1.0, la balle fera de lourds dégâts\naprès pénétration\nAvec une valeur inférieure à 1.0, la balle fera des dégâts moindre\naprès pénétration.\n \nValeur élevée recommandée.",
	L"\n \nMultiplicateur de potentiel de dégâts juste avant\nl'impact de la balle.\n \nAvec une valeur supérieure à 1.0, la balle fera de lourds dégâts\nUne valeur inférieure à 1.0 fera des dégâts moindre.\n \nValeur élevée recommandée.",
	L"\n \nAdditional heat generated by this ammunition.\n \nLower is better.",		// TODO.Translate
	L"\n \nDetermines what percentage of a\nbullet's damage will be poisonous.",	// TODO.Translate
	L"\n \nAdditional dirt generated by this ammunition.\n \nLower is better.",		// TODO.Translate
};

STR16 szUDBGenExplosiveStatsTooltipText[]=
{
	L"|D|é|g|â|t|s",
	L"|D|é|g|â|t|s |é|t|o|u|r|d|i|s|a|n|t",
	L"|E|x|p|l|o|d|e|s |o|n| |I|m|p|a|c|t",		// HEADROCK HAM 5	// TODO.Translate
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n",
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n |é|t|o|u|r|d|i|s|a|n|t|e",
	L"|R|a|y|o|n |d|'|e|x|p|l|o|s|i|o|n |s|o|n|o|r|e",
	L"|D|é|b|u|t |r|a|y|o|n |g|a|z |l|a|c|r|y|m|o|g|è|n|e",
	L"|D|é|b|u|t |r|a|y|o|n |g|a|z |m|o|u|t|a||r|d|e",
	L"|D|é|b|u|t |r|a|y|o|n |f|l|a|s|h |l|u|m|i|n|e|u|x",
	L"|D|é|b|u|t |r|a|y|o|n |f|u|m|é|e",
	L"|D|é|b|u|t |r|a|y|o|n |i|n|c|e|n|d|i|e",
	L"|F|i|n |r|a|y|o|n |g|a|z |l|a|c|r|y|m|o|g|è|n|e",
	L"|F|i|n |r|a|y|o|n |g|a|z |m|o|u|t|a||r|d|e",
	L"|F|i|n |r|a|y|o|n |f|l|a|s|h |l|u|m|i|n|e|u|x",
	L"|F|i|n |r|a|y|o|n |f|u|m|é|e",
	L"|F|i|n |r|a|y|o|n |i|n|c|e|n|d|i|e",
	L"|D|u|r|é|e |d|e |l|'|e|f|f|e|t",
	// HEADROCK HAM 5: Fragmentation		// TODO.Translate
	L"|N|u|m|b|e|r |o|f |F|r|a|g|m|e|n|t|s",
	L"|D|a|m|a|g|e |p|e|r |F|r|a|g|m|e|n|t",
	L"|F|r|a|g|m|e|n|t |R|a|n|g|e",
	// HEADROCK HAM 5: End Fragmentations
	L"|I|n|t|e|n|s|i|t|é |s|o|n|o|r|e",
	L"|V|o|l|a|t|i|l|i|t|é",
};

STR16 szUDBGenExplosiveStatsExplanationsTooltipText[]=
{
	L"\n \nLa quantité de dégâts causés par cet explosif.\n \nNotez que les explosifs de type 'explosion' livrent des dégâts\nseulement une fois (quand ils explosent), tandis que les\nexplosifs à effets prolongés livrent cette quantité de dégâts\nà chaque tour jusqu'à ce que l'effet se dissipe.\n \nValeur élevée recommandée.",
	L"\n \nLa quantité de dégâts non mortels (étourdissant) causés\npar cet explosif.\n \nNotez que les explosifs de type 'explosion' livrent des\ndégâts seulement une fois (quand ils explosent), tandis\nque les explosifs à effets prolongés livrent cette\nquantité de dégâts d'étourdissement à chaque tour jusqu'à\nce que l'effet se dissipe.\n \nValeur élevée recommandée.",
	L"\n \nThis explosive will not bounce around -\nit will explode as soon as it hits any obstacle.",	// HEADROCK HAM 5	// TODO.Translate
	L"\n \nRayon de l'explosion causé par cet objet.\n \nPlus les ennemis seront loin du centre de l'explosion\nmoins ils subiront de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nRayon de l'etourdissement causé par cet objet.\n \nPlus les ennemis seront loin du centre de l'explosion\nmoins ils subiront de dégâts.\n \nValeur élevée recommandée.",
	L"\n \nDistance du parcours du bruit causé par ce piège.\n Les ennemis placés en deçà de cette distance entendront\n votre piège et sonneront l'alerte.\n \nValeur faible recommandée.",
	L"\n \nRayon de départ libéré par la lacrymogène.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nA noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ libéré par le gaz moutarde.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nA noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ émis par le flash lumineux.\n \nLes cases autours du centre de l'effet deviendront très\nlumineuses, quand celles autours ne seront que\nlégèrement plus lumineuse que la normale.\n \nA noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nA noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ libéré par la fumée.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz. plus important,\nquiconque se trouvant à l'intérieur de la fumée aura des difficultés à se repérer,\net perdra aussi sa visibilité.\n \nA noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ causés par les flammes.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour.\n \nA noter également la fin du rayon et la durée de\nl'effet (afficher ci-dessous).\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par la lacrymogène avant\nqu'il ne se dissipe.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nA noter également le début du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par le gaz moutarde avant\nqu'il ne se dissipe.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz.\n \nA noter également le début du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin émis par le flash lumineux.\n \nLes cases autours du centre de l'effet deviendront très\nlumineuses, quand celles autours ne seront que\nlégèrement plus lumineuse que la normale.\n \nA noter également la fin du rayon et la durée de\nl'effet.\n \nA noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	L"\n \nRayon de fin libéré par la fumée.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour,\nà moins qu'ils portent un masque à gaz. plus important,\nquiconque se trouvant à l'intérieur de la fumée aura des difficultés à se repérer,\net perdra aussi sa visibilité.\n \nA noter également la fin du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nRayon de départ causés par les flammes.\n \nLes ennemis placés en deçà de cette distance subiront\ndes dégâts et des étourdissements à chaque tour.\n \nA noter également la fin du rayon et la durée de\nl'effet.\n \nValeur élevée recommandée.",
	L"\n \nDurée des effets de l'explosion.\n \nChaque tour, le rayon s'aggrandit d'un case dans\n toutes les directions, avant d'atteindre\nla vlauer de fin de rayon indiquée.\n \nQuand la durée a été atteinte, les effets se\ndissipent complètement.\n \nA noter aussi que contrairement aux autres explosifs qui\nont une durée d'effet, le flash lumineux faiblit\nau cours du temps, avant de disparaître.\n \nValeur élevée recommandée.",
	// HEADROCK HAM 5: Fragmentation		// TODO.Translate
	L"\n \nThis is the number of fragments that will\nbe ejected from the explosion.\n \nFragments are similar to bullets, and may hit\nanyone who's close enough to the explosion.\n \nHigher is better.",
	L"\n \nThe potential amount of damage caused by each\nfragment ejected from the explosion.\n \nHigher is better.",
	L"\n \nThis is the average range to which fragments\nfrom this explosion will fly.\n \nSome fragments may end up further away, or fail\nto cover the distance altogether.\n \nHigher is better.",
	// HEADROCK HAM 5: End Fragmentations
	L"\n \nDistance (en cases) en deçà de laquelle chaque\nsoldats et mercs peuvent entendre l'explosion.\n \nLes ennemis qui entendent cette explosion peuvent alerter de\nvotre présence.\n \nValeur faible recommandée.",
	L"\n \nCette valeur représente la chance (sur 100) que cette\nexplosif explose spontanément chaque fois qu'il est endommagé\n(Par exemple, quand il y a d'autres explosions à proximité).\n \nTransporter des explosifs hautement volatiles en combat\nest donc extrêmement risqué et devrait être évitée.\n \nEchelle: 0-100.\nValeur faible recommandée.",
};

STR16 szUDBGenSecondaryStatsTooltipText[]=
{
	L"|B|a|l|l|e|s |t|r|a|ç|a|n|t|e|s",
	L"|M|u|n|i|t|i|o|n|s |A|n|t|i|-|T|a|n|k",
	L"|I|g|n|o|r|e |l|'|a|r|m|u|r|e",
	L"|M|u|n|i|t|i|o|n|s |a|c|i|d|e|s",
	L"|M|u|n|i|t|i|o|n|s |c|a|s|s|a|n|t |s|e|r|r|u|r|e",
	L"|R|é|s|i|s|t|a|n|t |a|u |e|x|p|l|o|s|i|f|s",
	L"|W|a|t|e|r|p|r|o|o|f",
	L"|E|l|e|c|t|r|o|n|i|q|u|e",
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
	L"|T|r|o|u|s|s|e |à |o|u|t|i|l|s",
	L"|O|p|t|i|q|u|e|s |t|h|e|r|m|i|q|u|e|s",
	L"|D|i|s|p|o|s|i|t|i|f |à |r|a|y|o|n|s |X",
	L"|C|o|n|t|i|e|n|t |d|e |l|'|e|a|u |p|o|t|a|b|l|e",
	L"|C|o|n|t|i|e|n|t |d|e |l|'|a|l|c|o|o|l",
	L"|T|r|o|u|s|s|e |d|e |1|e|r |s|o|i|n|s",
	L"|T|r|o|u|s|s|e |d|e |s|o|i|n|s",
	L"|B|o|m|b|e |p|o|u|r |s|e|r|r|u|r|e |d|e |p|o|r|t|e",
	L"|D|r|i|n|k",// TODO.Translate
	L"|M|e|a|l",
	L"|A|m|m|o |B|e|l|t",
	L"|A|m|m|o |V|e|s|t",
	L"|D|e|f|u|s|a|l |K|i|t",	// TODO.Translate
	L"|C|o|v|e|r|t |I|t|e|m",	// TODO.Translate
};

STR16 szUDBGenSecondaryStatsExplanationsTooltipText[]=
{
	L"\n \nCes munitions, en mode Auto ou rafale, ont la propriété d'être\ndes des balles traçantes.\n \nLa lumière qu'apporte les balles traçantes lors d'une rafale\npermet d'avoir une meilleur précision et d'être ainsi plus\nmortel malgré le recul de l'arme.\n \nDe plus, ces balles créent un halo lumineux permettant de\nrévéler l'ennemi dans la nuit. Cependant, elles révèlent\naussi la position du tireur à l'ennemi !\n \nLes balles traçantes désactive automatiquement le\ncache-flamme installé sur l'arme utilisé.",
	L"\n \nCes munitions peuvent faire des dégâts aux tanks.\n \nLes munitions SANS cette propriété ne feront aucun dégâts quelque\nsoit le tanks.\n \nMême avec cette propriété, n'oubliez pas que la plupart des armes\nne feront que peu de dégâts, donc n'en abusez pas.",
	L"\n \nCes munitions ignorent complètement l'armure.\n \nQuand vous tirez sur un ennemi avec une armure, cela sera comme s'il\nn'en avait pas, permettant ainsi de faire un maximum de dégâts !",
	L"\n \nLorsque cette munition frappe une cible avec une armure,\ncette dernière se dégradera très rapidement.\n \nCeci peut potentiellement retirer l'armure de la cible !",
	L"\n \nCette munition est exceptionnelle pour casser les serrures.\n \nTirez directement sur la serrure de la porte ou du\ncontaineur pour faire de lourds dégâts sur le mécanisme.",
	L"\n \nCette armure est trois fois plus résistante contre les\nexplosifs que sa valeur indiquée.\n \nQuand une explosion heurte cette armure, la valeur de\nsa protection est considéré comme trois fois plus\nélevée que celle indiquée.",
	L"\n \nCet objet est imperméable à l'eau. Il ne recevra pas de\ndégâts causé par l'eau.\n \nLes objets SANS cette propriété vont progressivement se\ndéteriorer si la personne nage avec.",
	L"\n \nCet objet est de nature électronique, et contient des\ncircuits complexes.\n \nLes objets électroniques sont intrinsèquement plus\ndifficiles à réparer, d'autant plus si vous n'avez pas\nles compétences nécessaires.",
	L"\n \nLorsque cet objet est porté sur le visage, il le protègera\nde tous les gaz nocifs.\n \nNotez que certains gaz sont corrosifs, et pourrait bien\npénétrer à travers le masque...",
	L"\n \nCet objet requière des piles. Sans les piles vous ne pouvez pas\nactiver ces principales caractéristiques.\n \nPour utiliser un jeu de piles, attachez-les à cette objet\ncomme si vous m'étiez une lunette de visée à votre arme.",
	L"\n \nCet objet peut être utilisé pour crocheter des portes\nou des containeurs vérouillés.\n \nLe crochetage est silencieux, mais nécessite des\ncompétences en Mécanique. This item modifies\nthe lockpicking chance by ", //JMich_SkillsModifiers: needs to be followed by a number",		// TODO.Translate
	L"\n \nCet objet peut être utilisé pour couper les clôtures de fil.\n \nCela autorise le mercenaire a passé à travers des zones\nsécurisés, pour éventuellement surprendre l'ennemi !",
	L"\n \nCet objet peut être utilisé pour frapper des portes\nou des containeurs vérouillés.\n \nFrapper des serrures requière une grande Force,\ngénère beaucoup de bruits, et peut facilement\nvous épuisez. Cependant, c'est une bonne façon\nd'ouvrir une serrure sans avoir des talents en\nMécaniques ou des outils adéquates. This item improves \nyour chance by ", //JMich_SkillsModifiers: needs to be followed by a number		// TODO.Translate
	L"\n \nCet objet peut être utilisé pour détecter des objets métalliques\nenfouis sous terre.\n \nNaturellement, sa fonction première est de détecter les mines\nsans que vous ayez les compétences nécessaires pour les repérer\nà l'oeil nu.\n \nPeut-être trouverez-vous certains trésors cachés...",
	L"\n \nCet objet peut être utilisé pour faire exploser une bombe\nqui aura été amorçée par un détonateur.\n \nPlantez la bombe en 1er, puis utilisez votre Déclencheur\nà distance pour l'activer quand c'est le bon moment.",
	L"\n \nQuand il est attaché à un dispositif explosif et positionné\ndans le bon sens, ce détonateur peut être déclenché par un\ndétonateur (séparé) à distance.\n \nLes détonateurs à distance sont excellent pour faire des\npièges, car ils se déclenchent quand vous le souhaitez.\n \nDe plus, vous avez tout le temps pour déguerpir !",
	L"\n \nQuand il est attaché à un dispositif explosif et positionné\ndans le bon sens, ce détonateur va lancer un compte à\nrebours défini, et explosera quand le temps sera écoulé.\n \nCes détonateurs avec minuterie sont pas chers et facile à\ninstaller, mais vous aurez besoin de temps pour pouvoir\ndéguerpir de là !",
	L"\n \nCet objet contient de l'essence.\n \nIl pourrait arrivé à point nommé si vous aviez besoin\nde remplir un réservoir d'essence...",
	L"\n \nCet objet contient divers outils qui peuvent être utilisés\npour réparer d'autres objets.\n \nUne trousse à outil est toujours nécessaire lorsque vous\nêtes en mission de Réparation. This item modifies\nthe effectiveness of repair by ", //JMich_SkillsModifiers: need to be followed by a number		// TODO.Translate
	L"\n \nQuand équipé sur le visage, cet objet donne la capacité de\nrepérer les ennemis à travers les murs, grâce à leur\nsignature thermique.",
	L"\n \nCe merveilleux dispositif peut être utilisé pour repérer\nles ennemis en utilisant les rayons X.\n \nCela révelera tous les ennemis à une certaine distance\npour une courte période de temps.\n \nGardez cela loin de vos organes reproductifs !",
	L"\n \nCet objet contient de l'eau potable bien fraiche.\nA utillisé lorsque vous êtes assoiffé.",
	L"\n \nCet objet contient du digestif, gnôle, eau-de-vie, liqueur,\nqu'importe comment vous appelez cela.\n \nA utlisez avec modération. Boire ou conduire !\nPeut causer une cirrhose du foie.",
	L"\n \nIl s'agit d'un kit médical basic, contenant les ustensibles\nrequis pour faire une intervention médicale basic.\n \nIl peut être utilisé pour soigner un mercenaire blessé et\nempêcher le saignement.\n \nPour une guérison optimale, utlisez une véritable trousse\nde soins et/ou beaucoup de repos.",
	L"\n \nIl s'agit d'un kit médicale complet, qui peut être utilisé\npour une opération chirurgicale ou autre cas sérieux.\n \nUne trousse de soins est toujours nécessaire lorsque vous\nêtes en mission de Docteur.",
	L"\n \nCet objet peut être utilisé pour faire sauter les portes\nou containeurs vérouillés.\n \nDes compétences en explosion sont nécessaire pour éviter\nune explosion prématurée.\n \nExploser les serrures est relativement facile et rapide\nà faire. Cependant, c'est très bruyant et dangereux pour\nla plupart des mercernaires.",
	L"\n \nThis item will still your thirst\nif you drink it.",// TODO.Translate
	L"\n \nThis item will still your hunger\nif you eat it.",
	L"\n \nWith this ammo belt you can\nfeed someone else's MG.",
	L"\n \nYou can feed an MG with ammo\nbelts stored in this vest.",
	L"\n \nThis item improves your trap disarm chance by ",		// TODO.Translate
	L"\n \nThis item and everything attached/inside\nit is hidden from curious eyes.",	// TODO.Translate
};

STR16 szUDBAdvStatsTooltipText[]=
{
	L"|F|a|c|t|e|u|r |d|e |p|r|é|c|i|s|i|o|n",
	L"|F|l|a|t |S|n|a|p|s|h|o|t |M|o|d|i|f|i|e|r",
	L"|P|e|r|c|e|n|t |S|n|a|p|s|h|o|t |M|o|d|i|f|i|e|r",
	L"|F|l|a|t |A|i|m|i|n|g |M|o|d|i|f|i|e|r",
	L"|F|a|c|t|e|u|r |p|o|u|r|c|e|n|t|a|g|e |d|e |v|i|s|é|e",
	L"|F|a|c|t|e|u|r |n|i|v|e|a|u |d|e |v|i|s|é|e |a|u|t|o|r|i|s|é|e",
	L"|A|i|m|i|n|g |C|a|p |M|o|d|i|f|i|e|r",
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
	L"|F|a|c|t|e|u|r |d|e |c|o|n|t|r|e |f|o|r|c|e |m|a|x|i|m|u|m",
	L"|F|a|c|t|e|u|r |d|e |p|r|é|c|i|s|i|o|n |d|e |c|o|n|t|r|e |f|o|r|c|e",
	L"|F|a|c|t|e|u|r |d|e |f|r|é|q|u|e|n|c|e |d|e |c|o|n|t|r|e |f|o|r|c|e",
	L"|F|a|c|t|e|u|r |d|e |P|A |t|o|t|a|l",
	L"|F|a|c|t|e|u|r |d|e |P|A |m|i|s|e |e|n |j|o|u|e",
	L"|F||a|c|t|e|u|r |d|e |P|A |e|n |a|t|t|a|q|u|e |s|i|m|p|l|e",
	L"|F||a|c|t|e|u|r |d|e |P|A |e|n |R|a|f|a|l|e",
	L"|F||a|c|t|e|u|r |d|e |P|A |e|n |A|u|t|o",
	L"|F||a|c|t|e|u|r |d|e |P|A |p|o|u|r |r|e|c|h|a|r|g|e|r",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |m|u|n|i|t|i|o|n",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |R|a|f|a|l|e",
	L"|C|a|c|h|e|-|f|l|a|m|m|e",
	L"|F|a|c|t|e|u|r |i|n|t|e|n|s|i|t|é |s|o|n|o|r|e",
	L"|F|a|c|t|e|u|r |t|a|i|l|l|e |o|b|j|e|t",
	L"|F|a|c|t|e|u|r |d|e |f|i|a|b|i|l|i|t|é",
	L"|C|a|m|o|u|f|l|a|g|e |e|n |f|o|r|ê|t",
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
};

// Alternate tooltip text for weapon Advanced Stats. Just different wording, nothing spectacular.
STR16 szUDBAdvStatsExplanationsTooltipText[]=
{
	L"\n \nLorsque attaché à une arme de distance, cet objet modifie la\nvaleur de sa précision.\n \nLe gain en précision permet à l'arme de pouvoir toucher une\ncible à des distances plus élevées plus souvent\n \nEchelle : -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie la précision du tireur pour un tir avec\nune arme à distance de la valeur suivante.\n \nEchelle: -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie la précision du tireur pour un tir avec\nune arme à distance avec un pourcentage calculé à partir\nde sa précision initiale.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie le gain de précision pris à chaque\nniveau de visée, de la valeur suivante.\n \nEchelle : -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCet objet modifie le gain de précision pris à chaque\nniveau de visée, d'un pourcentage calculé à partir\nde sa précision initiale.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie le nombre de niveau de visée que cette arme\npeut avoir.\n \nRéduire le nombre de niveau de visée signifie que chaque\nniveau ajoute proportionnellement plus de précision au tir.\nPar conséquent, même les bas niveaux de visée vous\npermetterons de garder une bonne précision avec une vitesse\npour viser élevée !\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la précision maximale du tireur équipé d'une\narme à distance, avec un pourcentage basé sur sa précision\ninitiale.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme à distance, cet objet modifie sa\ndifficulté de manipulation.\n \nUne meilleure prise en main permet une meilleure précision\nde l'arme, avec ou sans niveaux de visée supplémentaires.\n \nNotez que c'est basé sur le facteur de prise en main des\narmes, qui est plus élevé pour les fusils et armes lourdes\nque les pistolets et armes légères.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la difficulté des tirs hors de la portée de l'arme.\n \nUne valeur élevée peut augmenter la portée maximale de l'arme de\nquelques cases.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la difficulté de touché une cible en mouvement\navec une arme à distance.\n \nUne valeur élevée peut vous aider à toucher une cible en\nmouvement, même à distance.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la puissance d'impact de votre arme\nde la valeur suivante.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la puissance d'impact de votre arme de mêlée\nde la valeur suivante.\n \nCeci s'applique uniquement aux armes de mêlée, tranchantes\nou contondantes.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie sa porté effective.\n \nLa portée maximale dicte essentiellement dans quelle mesure une balle\ntirée par l'arme peut voler avant de commencer à tomber\nbrusquement vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nfournit un aggrandissement supplémentaire, réussisant des coups\nplus facilement que la normale.\n \nNotez qu'un facteur d'aggrandissment trop élevée est préjudiciable\nquand la cible est plus PROCHE que la distance optimale.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nprojette un point sur la cible, rendant le tir plus facile.\n \nCette projection est seulement utile à une certaine distance\n, au delà elle diminue puis éventuellement disparaît.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\ndes modes Rafale ou tir Auto, cet objet modifie\nle recul horizontal de l'arme par le\npourcentage suivant.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\ndes modes Rafale ou tir Auto, cet objet modifie\nle recul vertical de l'arme par le\npourcentage suivant.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à faire\nface au recul durant une salve en mode Rafale ou Auto.\n \nUne valeur élevée permet d'aider le tireur a controler une arme\navec un fort recul, même s'il a peu de Force.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à compenser\nle recul durant une salve en mode Rafale ou Auto.\n \nUne valeur élevée permet de corriger le recul pour garder le canon\nsur la cible, même à longue distance,rendant ainsi la salve\nplus précise.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie la capacité du tireur à adapter\nà chaque fréquence l'effort de compensation du recul durant une\nsalve en mode Rafale ou Auto.\n \nUne fréquence élevée de compensation permet une salve très précise,\net ainsi permettre des tirs en Rafale et Auto à très longues portées.\n \nValeur élevée recommandée.",
	L"\n \nCet objet modifie directement le nombre de PA\nque le mercenaire a durant chaque début de tour.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le coût en PA pour mettre en joue.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme, cet objet\nmodifie le coût en PA pour faire une attaque simple.\n \nNotez que pour les armes ayant un mode Auto/Rafale, le\ncoût est directement influencé par ce facteur !\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\nun mode Rafale, cet objet modifie le coût en PA d'un tir en Rafale.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance ayant\nun mode Auto, cet objet modifie le coût en PA d'un tir en Auto.\n \nNotez que cela ne modifie pas le coût supplémentaire\npour ajouter des balles à la salve, mais seulement\nle coût initiale d'une salve.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le coût en PA pour recharger.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nchange la taille des munitions qui peuvent être chargé\ndans l'arme.\n \nCette arme peut maintenant accepter des tailles plus ou moins grandes de munitions\nayant un même calibre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie le nombre de balles tiré\npar cette arme en Rafale.\n \nSi cette arme n'a pas de mode Rafale, et que la\nvaleur est positive, alors cet objet donnera à l'arme la possibilité\nde tirer en mode Rafale.\n \nInversement, si il y a un mode Rafale\net une valeur négative, cela peut retirer le mode Rafale.\n \nValeur élevée généralement recommandée. Gardée bien à l'esprit\nque le mode Rafale est là pour conserver les munitions...",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nva cacher le flash du canon.\n \nCela permettera au tireur de ne pas sa faire repérer\net de rester à couvert si il l'est.\nChose important de nuit...",
	L"\n \nLorsque attaché à une arme, cet objet\nmodifie la distance à laquelle un tir sera entendu par les\nennemis et les mercenaires.\n \nSi ce facteur modifie l'intensité sonore de l'arme à 0\n, elle deviendra alors intédectable.\n \nValeur faible recommandée.",
	L"\n \nCet objet modifie la taille de n'importe quel objet\npouvant être attaché.\n \nLa taille est importante dans le Nouveau Systéme d'Inventaire,\noù les poches n'acceptent qu'une taille et des formes spécifiques.\n \nAugmenter la taille d'un objet peut le rendre trop gros pour des poches.\n \nInversement, réduire sa taille peut permettre de l'insérer dans plus de poches,\net les poches seront à même de contenir plus d'objets.\n \nValeur faible généralement recommandée.",
	L"\n \nLorsque attaché à une arme, cet objet modifie la valeur\nde fiabilité de l'arme.\n \nSi positive, l'état de l'arme se détériore moins\nrapidement si utilisé en combat. Mais hors combat elle\nse détériore plus rapidement.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone forestière du porteur.\n \nPour avoir un facteur de camouflage efficace en forêt\nvous devez être près d'arbres ou d'herbes hautes.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone urbaine du porteur.\n \nPour avoir un facteur de camouflage efficace en zone urbaine\nvous devez être près de batîments.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone désertique du porteur.\n \nPour avoir un facteur de camouflage efficace en zone désertique\nvous devez être près du sable ou d'une végétation désertique.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie le camouflage en zone enneigée du porteur.\n \nPour avoir un facteur de camouflage efficace en zone enneigée\nvous devez être près de cases enneigés.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie la discrétion du porteur en rendant le mercenaire\nplus difficile à entendre lorsqu'il se déplace en mode silencieux.\n \nNotez que cela ne change en rien la visibilité du mercenaire,\nmais seulement la quantité de sons émis lors d'un déplacement en silence.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet\nmodifie l'audition du porteur du pourcentage suivant.\n \nUne valeur positive rend possible l'écoute de sons\nprovenant de longues distances.\n \nInversement, une valeur négative détériore l'audition du porteur.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision dans toutes les conditions.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsqu'il y a peu de lumière ambiante.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque l'intensité de la lumière\nest normale ou forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque l'intensité de la lumière est très forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nla vision du porteur.\n \nModification de la vision lorsque vous êtes dans un sous-sol sombre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché ou porté à un autre objet, cet objet modifie\nle champs de vision du porteur.\n \nRéduisant le champs de vision de chaque coté.\n \nValeur faible recommandée.",
	L"\n \nHabilité du tireur à faire face au recul\nlors d'un tir en mode Rafale ou Auto.\n \nValeur élevée recommandée.",
	L"\n \nFréquence de recalcule du tireur pour ajuster la force\nqu'il doit mettre pour contrer le recul de l'arme, lors d'un tir\nen mode Rafale ou Auto.\n \nUne fréquence faible rend la salve plus précise en supposant que\nle tireur puisse surmonter le recul correctement.\n \nValeur faible recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie sa chance de toucher la cible (CTH).\n \nAugmenter son CTH permet de toucher plus souvent\nune cible,en supposant que le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nLorsque attaché à une arme de distance, cet objet\nmodifie ses bonus de visée.\n \nAugmenter les bonus de visées permet de toucher\nune cible à longue distance plus souvent, en supposant\nque le tireur a bien visé.\n \nValeur élevée recommandée.",
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
};

STR16 szUDBAdvStatsExplanationsTooltipTextForWeapons[]=
{
	L"\n \nLa précision de cette arme a été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAugmenter la précision permet de toucher une cible\nà longue distance plus souvent.\n \nEchelle: -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la précision du tireur\n,de n'importe quel mode de tir, de la valeur suivante.\n \nEchelle: -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la précision du tireur\n,de n'importe quel mode de tir, du pourcentage suivant.\nPourcentage basé sur la précision initiale du tireur.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la quantité de précision gagnée\nà chaque niveau de visée de la valeur suivante.\n \nEchelle: -100 to +100.\nValeur élevée recommandée.",
	L"\n \nCette arme modifie la quantité de précision gagnée\nà chaque niveau de visée du pourcentage suivant.\nPourcentage basé sur la précision initiale du tireur.\n \nValeur élevée recommandée.",
	L"\n \nLe nombre de niveaux de visée supplémentaires permis par\ncette arme a été modifié par une munition, un accessoire\n, ou bien intégré dans les attributs.\nSi le nombre de niveau de visée a baissé, c'est que l'arme est\nplus rapide à viser sans perdre en précision.\n \nInversement, si le nombre de visée a augmenté,l'arme sera\nplus lente à viser sans perdre en précision.\n \nValeur faible recommandée.",
	L"\n \nCette arme modifie la précision maximum du tireur\nbasé sur un pourcentage de la précision initiale maximale.\n \nValeur élevée recommandée.",
	L"\n \nLes accessoires ou les caractéristiques de l'arme modifient\nsa difficulté de prise en main.\n \nUne meilleure prise en main l'arme plus précise,\navec ou sans niveaux de visée supplémentaires.\n \nNotez que c'est basé sur le facteur de prise en main des armes\n,qui est plus élevé pour les fusils et armes lourdes que\nles pistolets et armes légères.\n \nValeur faible recommandée.",
	L"\n \nL'habilité de l'arme à compenser les tirs\nqui sont hors de portée est modifié par un\naccessoire ou les caractéristiques de l'arme.\n \nUne valeur élevée peut augmenter la portée maximale\nde l'arme de quelques cases.\n \nValeur élevée recommandée.",
	L"\n \nL'habilité de l'arme à toucher une cible en mouvement\nà distance a été modifié par un accessoire ou\nun attribut inhérent à l'arme.\n \nUne valeur élevée peut vous aider à toucher\nune cible en mouvement, même à distance.\n \nValeur élevée recommandée.",
	L"\n \nLa puissance d'impact de votre arme a été modifié\npar une munition, un accessoire ou attribut inhérent à l'arme.\n \nValeur élevée recommandée.",
	L"\n \nLa puissance d'impact de votre arme de mêlée a été modifié\npar une munition, un accessoire ou attribut inhérent à l'arme.\n \nCeci s'applique uniquement aux armes de mêlée, tranchantes\nou contondantes.\n \nValeur élevée recommandée.",
	L"\n \nLa portée maximum de votre arme a été augmenté ou diminué\ngrâce à une munition, un accessoire ou attribut inhérent à l'arme.\n \nLa portée maximale dicte essentiellement dans quelle mesure une balle\ntirée par l'arme peut voler avant de commencer à tomber\nbrusquement vers le sol.\n \nValeur élevée recommandée.",
	L"\n \nCette arme est équipée d'une visée optique,\nrendant les tirs à distance plus facile à réaliser.\n \nNotez qu'un facteur d'aggrandissment trop élevée est préjudiciable\nquand la cible est plus PROCHE que la distance optimale.\n \nValeur élevée recommandée.",
	L"\n \nCette arme est équipée d'un système de projection\n(tel qu'un laser), qui projette un point sur la\ncible, rendant le tir plus facile.\n \nCette projection est seulement utile à une certaine distance\n, au delà elle diminue puis éventuellement disparaît.\n \nValeur élevée recommandée.",
	L"\n \nLe recul horizontal de cette arme a été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAucun effet si l'arme ne possède pas de mode Auto et/ou Rafale.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nLe recul vertical de cette arme a été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAucun effet si l'arme ne possède pas de mode Auto et/ou Rafale.\n \nRéduire le recul permet de garder plus facilement le canon\npointé sur la cible pendant la salve.\n \nValeur faible recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à faire face\nau recul durant une salve en mode Auto ou Rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme.\n \nUne valeur élevée permet d'aider le tireur a controler une arme\navec un fort recul, même s'il a peu de Force.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à compenser\nle recul durant une salve en mode Auto ou Rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme.\n \nUne valeur élevée permet de corriger le recul pour garder le canon\nsur la cible, même à longue distance,\nrendant ainsi la salve plus précise.\n \nValeur élevée recommandée.",
	L"\n \nCette arme modifie la capacité du tireur à adapter à chaque\nà chaque fréquence l'effort de compensation du recul durant\nune salve en mode Auto ou Rafale,\ngrâce à une munition, un accessoire ou un attribut inhérent à l'arme..\n \nUne fréquence élevée de compensation permet une salve très précise,\net ainsi permettre des tirs en Rafale et Auto à très longues portées,\nen supposant que le tireur puisse couvrir le recul correctement.\n \nValeur élevée recommandée.",
	L"\n \nLorsque tenue en main, cette arme modifie la quantité de\nPA que le mercenaire a au début de chaque tour.\n \nValeur élevée recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour mettre en joue avec cette arme a été\nmodifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une attaque simple avec cette arme\na été modifié.\n \nNotez que les modes Auto et Rafale de l'arme,\nont leur coût directement influencé par ce facteur.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une Rafale avec cette arme\na été modifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour faire une salve en Auto avec cette arme\na été modifié.\n \nNotez que cela ne modifie pas le coût supplémentaire en PA\nlorsque vous ajoutez des balles à la salve, mais\nseulement son coût initial.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle nombre de PA pour recharger cette arme\na été modifié.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla taille des munitions qui peuvent être chargé sur cette arme\na été modifié.\n \nCette arme peut maintenant accepter des tailles plus ou moins grandes de munitions\nayant un même calibre.\n \nValeur élevée recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla quantité de balles tirée par cette arme en mode Rafale\na été modifié.\n \nSi cette arme n'a pas de mode Rafale, et que la\nvaleur est positive, alors cet objet donnera à l'arme la possibilité\nde tirer en mode Rafale.\n \nInversement, si il y a un mode Rafale\net une valeur négative, cela peut retirer le mode Rafale.\n \nValeur élevée généralement recommandée. Gardée bien à l'esprit\nque le mode Rafale est là pour conserver les munitions...",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\ncette arme ne produira pas de flash lors du tir.\n \nCela permettera au tireur de ne pas sa faire repérer\net de rester à couvert si il l'est.\nChose important de nuit...",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nle bruit généré par l'arme a été modifié. La distance\nà laquelle les ennemis et mercenaires peuvent entendre votre tir à changé.\n \nSi ce facteur modifie l'intensité sonore de l'arme à 0\n, elle deviendra alors intédectable.\n \nValeur faible recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla catégorie de la taille de cette arme a changé.\n \nLa taille est importante dans le Nouveau Systéme d'Inventaire,\noù les poches n'acceptent qu'une taille et des formes spécifiques.\n \nAugmenter la taille d'un objet peut le rendre trop gros pour des poches.\n \nInversement, réduire sa taille peut permettre de l'insérer dans plus de poches,\net les poches seront à même de contenir plus d'objets.\n \nValeur faible généralement recommandée.",
	L"\n \nGrâce à une munition, un accessoire ou un attribut inhérent à l'arme,\nla fiabilité de cette arme a changé.\n \nSi positive, l'état de l'arme se détériore\nmoins rapidement si utilisé en combat. Mais hors combat\nelle se détériore plus rapidement.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone forestière du porteur.\n \nPour avoir un facteur de camouflage efficace en forêt\nvous devez être près d'arbres ou d'herbes hautes.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone urbaine du porteur.\n \nPour avoir un facteur de camouflage efficace en zone urbaine\nvous devez être près de batîments.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone désertique du porteur.\n \nPour avoir un facteur de camouflage efficace en zone désertique\nvous devez être près du sable ou d'une végétation désertique.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nle camouflage en zone enneigée du porteur.\n \nPour avoir un facteur de camouflage efficace en zone enneigée\nvous devez être près de cases enneigés.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nla discrétion du porteur en rendant le mercenaire\nplus difficile à entendre lorsqu'il se déplace en mode silencieux.\n \nNotez que cela ne change en rien la visibilité du mercenaire,\nmais seulement la quantité de sons émis lors d'un déplacement en silence.\n \nValeur élevée recommandée.",
	L"\n \nQuand cette arme est tenue à la main, elle modifie\nl'audition du porteur du pourcentage suivant.\n \nUne valeur positive rend possible l'écoute de sons\nprovenant de longues distances.\n \nInversement, une valeur négative détériore l'audition du porteur.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision dans toutes les conditions.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision de nuit lorsqu'il y a peu de lumière ambiante.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision de jour lorsque l'intensité de la lumière\nest normale ou forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision lorsque l'intensité de la lumière est très forte.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie la vision du porteur du pourcentage suivant,\ngrâce à un accessoire ou un attribut inhérent à l'arme.\n \nModification de la vision lorsque vous êtes dans un sous-sol sombre.\n \nValeur élevée recommandée.",
	L"\n \nLorsque cette arme est prête à tirer,\nelle modifie le champs de vision du porteur.\n \nRéduisant le champs de vision de chaque coté.\n \nValeur faible recommandée.",
	L"\n \nHabilité du tireur à faire face au recul\nlors d'un tir en mode Rafale ou Auto.\n \nValeur élevée recommandée.",
	L"\n \nFréquence de recalcule du tireur pour ajuster la force\nqu'il doit mettre pour contrer le recul de l'arme, lors d'un tir\nen mode Rafale ou Auto.\n \nUne fréquence faible rend la salve plus précise en supposant que\nle tireur puisse surmonter le recul correctement.\n \nValeur faible recommandée.",
	L"\n \nLa chance de toucher la cible avec cette arme\na été modifié par une munition, un accessoire ou\nun attribut inhérent à l'arme.\n \nAugmenter la chance de toucher permet de toucher plus souvent\nune cible,en supposant que le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nLes bonus de visée de cette arme ont été modifié\npar une munition, un accessoire ou un attribut inhérent à l'arme.\n \nAugmenter les bonus de visées permet de toucher\nune cible à longue distance plus souvent, en supposant\nque le tireur a bien visé.\n \nValeur élevée recommandée.",
	L"\n \nA single shot causes this much heat.\nThe type of ammunition can affect this value.\n \nLower is better.",					// TODO.Translate
	L"\n \nEvery turn, the temperature is lowered\nby this amount.\nWeapon attachments can affect this.\n \nHigher is better.",
	L"\n \nIf a gun's temperature is above this,\nit will jam more frequently.",
	L"\n \nIf a gun's temperature is above this,\nit will be damaged more easily.",
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

STR16	gzNewLaptopMessages[]=
{
	L"Ask about our special offer!",
	L"Temporarily Unavailable",
	L"This special press preview of Jagged Alliance 2: Unfinished Business contains the only first 6 sector maps. The final version of the game will feature many more - please see the included readme file for details.",
};

STR16	zNewTacticalMessages[]=
{
	//L"Distance cible: %d tiles, Brightness: %d/%d",
	L"Attachez le transpondeur à votre PC portable.",
	L"Vous n'avez pas les moyens d'engager %s",
	L"Pour une durée limitée, les frais ci-dessus couvrent la mission entière, équipement ci-dessous compris.",
	L"Engagez %s et découvrez dès à présent notre prix \"tout compris\".  Aussi inclus dans cette incroyable offre, l'équipement personnel du mercenaire sans frais supplémentaires.",
	L"Frais",
	L"Il y a quelqu'un d'autre dans le secteur...",
	//L"Portée arme: %d tiles, de chances: %d pourcent",
	L"Afficher couverture",
	L"Ligne de vision",
	L"Les nouvelles recrues ne peuvent arriver ici.",
	L"Comme votre PC n'a pas de transpondeur, vous ne pouvez engager de nouvelles recrues. Revenez à une sauvegarde précédente et réessayez.",
	L"%s entend le son de métal broyé provenant d'en dessous du corps de Jerry.  On dirait que l'antenne de votre PC ne sers plus à rien.",  //the %s is the name of a merc.  @@@  Modified
	L"Apres avoir scanné la note laissée par le Deputy Commander Morris, %s sent une oppurtinité. La note contient les coordonnées pour le lancement de missiles sur Arulco. Elle contient aussi l'emplacement de l'usine d'où les missiles proviennent.",
	L"En examinant le panneau de contrôle, %s s'aperçoît que les chiffres peuvent être inversés pour que les missiles détruisent cette même usine.  %s a besoin de trouver un chemin pour s'enfuir.  L'ascenseur semble être la solution la plus rapide...",
	L"Ceci est un jeu IRON MAN et vous ne pouvez pas sauvegarder s'il ya des ennemis dans les parages.",	//	@@@  new text
	L"(ne peut sauvegarder en plein combat)", //@@@@ new text
	L"Le nom de la campagne actuelle est supérieur à 30 lettres.",							// @@@ new text
	L"La campagne actuelle est introuvable.",																	// @@@ new text
	L"Campagne: Par défaut ( %S )",																							// @@@ new text
	L"Campagne: %S",																													// @@@ new text
	L"Vous avez choisi la campagne %S. Cette campagne est un mod d'Unfinished Business. Etes-vous sûr de vouloir jouer la campagne %S ?",			// @@@ new text
	L"Pour pouvoir utiliser l'éditeur, veuillez choisir une autre campagne que celle par défaut.",		///@@new
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
	L"%s wears suspicious LBE gear!",
	L"%s possesses military hardware!",
	L"%s carries too many guns!",
	L"%s equipment is too good!",
	L"%s gun has too many attachments!",
	L"%s was seen performing suspicious activities!",
	L"%s does not look like a civilian!",
	L"%s bleeding was discovered!",
	L"%s is drunk and doesn't look like a soldier!",
	L"On closer inspection, %s's disguise does not hold!",
	L"%s isn't supposed to be here!",
	L"%s isn't supposed to be here at this time!",
	L"%s was seen near a fresh corpse!",
	L"%s equipment raises a few eyebrows!",
	L"%s is seen targeting a soldier!",
	L"%s has seen through %s disguise!",
	L"No clothes item found in Items.xml!",
	L"This does not work with the old trait system!",
	L"Not enough Aps!",
	L"Bad palette found!",
	L"You need the covert skill to do this!",
	L"No uniform found!",
	L"%s is now disguised as a civilian.",
	L"%s is now disguised as a soldier.",
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
	L"%d prisoners were interrogated.",
	L"%d prisoners paid ransom money.",
	L"%d prisoners revealed enemy positions.",
	L"%d prisoners joined our cause.",
	L"Prisoners start a massive riot in %s!",
};

#endif //FRENCH
