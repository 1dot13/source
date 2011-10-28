// WANNE: Yes, this should be disabled, otherwise we get weird behavior when running the game with a VS 2005 build!
//#pragma setlocale("FRENCH")

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - New STOMP laptop strings
//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"Crochetage",
	L"Corps-à-corps",
	L"Electronique",
	L"Operations de nuit",
	L"Lancer",
	L"Enseigner",
	L"Armes lourdes",
	L"Armes automatiques",
	L"Discrétion",
	L"Ambidextre",
	L"Couteau",
	L"Tireur isolé",
	L"Camouflage",
	L"Arts martiaux",

	L"aucune",
	L"Spécialtés I.M.P.",
	L"(Expert)",

};

//added another set of skill texts for new major traits
STR16 gzIMPSkillTraitsTextNewMajor[]=
{
	L"Armes automatiques",
	L"Armes lourdes",
	L"Tireur d'élite",
	L"Scout",
	L"Bandit",
	L"Corps-à-corps",
	L"Manager",
	L"Technicien",
	L"Médecin",

	L"Personne",
	L"I.M.P. Traits pincipaux",
	// second names
	L"Mitrailleur",
	L"Bombardier",
	L"Sniper",
	L"Chasseur",
	L"Combattant",
	L"Arts martiaux",
	L"Commandant",
	L"Ingénieur",
	L"Chirurgien",
};

//added another set of skill texts for new minor traits
STR16 gzIMPSkillTraitsTextNewMinor[]=
{
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

	L"Personne",
	L"I.M.P. Traits mineurs",
};

//these texts are for help popup windows, describing trait properties
STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[]=
{
	L"+%d%s de chance de toucher avec une arme d'assaut\n",
	L"+%d%s de chance de toucher avec un SMGs\n",
	L"+%d%s de chance de toucher avec un LMGs\n",
	L"-%d%s du nombre de PA nécessaire pour tirer avec un LMGs\n",
	L"-%d%s du nombre de PA nécessaire pour préparer une arme automatique légère\n",
	L"La pénalité dû au mode Rafale/Auto est réduit de %d%s\n",
	L"Réduire la propabilité de tirer plus de balles que prévu en mode Auto\n",

};
STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer une grenade\n",
	L"-%d%s du nombre de PA nécessaire pour tirer une roquette\n",
	L"+%d%s de chance de toucher avec une grenade\n",
	L"+%d%s de chance de toucher avec une roquette\n",
	L"-%d%s du nombre de PA nécessaire pour tirer au mortier\n",
	L"Réduit la pénalité de chance de toucher du mortier de %d%s\n",
	L"+%d%s dégâts fait à un tank avec une arme lourde, grenades ou explosifs\n",
	L"+%d%s dégâts fait sur les autres cibles avec une arme lourde\n",

};
STR16 gzIMPMajorTraitsHelpTextsSniper[]=
{
	L"+%d%s de chance de toucher avec un fusil\n",
	L"+%d%s de chance de toucher avec une arme de précision\n",
	L"-%d%s de la distance effective de toutes les armes pour viser\n",
	L"+%d%s de bonus par niveau de visée (excepté pour les pistolets)\n",
	L"+%d%s de dégâts sur tir",
	L" plus",
	L" pour chaque niveau",
	L" après le premier",
	L" après le deuxième",
	L" après le troisième",
	L" après le quatrième",
	L" après le cinquième",
	L" après le sixième",
	L" après le septième",
	L"-%d%s du nombre de PA nécessaire pour recharger avec le fusil de précision\n",
	L"Ajoute un niveau de visée en plus pour chaque fusil de précision\n",
	L"Ajoute %d niveaux de visée pour chaque fusil de précision\n",

};
STR16 gzIMPMajorTraitsHelpTextsRanger[]=
{
	L"+%d%s de chance de toucher avec un fusil\n",
	L"+%d%s de chance de toucher avec un fusil à pompe\n",
	L"-%d%s du nombre de PA nécessaire pour recharger le fusil à pompe\n",
	L"+%d%s de vitesse de déplacement du groupe entre les secteurs si ils sont à pieds\n",
	L"+%d%s de vitesse de déplacement du groupe entre les secteurs si ils sont dans un véhicule (excepté l'hélicopter)\n",
	L"-%d%s d'énergies nécessaire pour traverser les secteurs\n",
	L"-%d%s de pénalités du temps\n",
	L"+%d%s d'efficacité du camouflage\n",
	L"-%d%s de l'usure du camouflage dû au temps ou à l'eau\n",

};
STR16 gzIMPMajorTraitsHelpTextsGunslinger[]=
{
	L"-%d%s du nombre de PA nécessaire pour tirer avec un pistolet ou un revovler\n",
	L"+%d%s de la distance effective avec un pistolet ou un revolver pour viser\n",
	L"+%d%s de chance de toucher avec un pistolet ou un revolver\n",
	L"+%d%s de chance de toucher avec un pistolet automatique",
	L" (en tir manuel seulement)",
	L"+%d%s de bonus par niveau de visée avec un pistolet, pistolet automatique ou un revolver\n",
	L"-%d%s du nombre de PA nécessaire pour augmenter un pistolet, pistolet automatique ou un revolver\n",
	L"-%d%s du nombre de PA nécessaire pour recharger un pistolet, pistolet automatique ou un revolver\n",
	L"Ajoute un niveau de visée en plus pour les pistolets, les pistolets automatique ou les revolvers\n",
	L"Ajoute %d niveaux de visée pour chaque pistolet, pistolet automatique ou un revolver\n",

};
STR16 gzIMPMajorTraitsHelpTextsMartialArts[]=
{
	L"-%d%s du nombre de PA nécessaire pour les attaques au corps-à-corps (mains nues ou avec un coup de poing américain)\n",
	L"+%d%s de chance de toucher avec les mains nues\n",
	L"+%d%s de chance de toucher avec un coup de poing américain\n",
	L"+%d%s de dégâts des attaques au corps-à-corps (mains nues ou avec un coup de poing américain)\n",
	L"+%d%s de dégâts sur le souffle des attaques au corps-à-corps (mains nues ou avec un coup de poing américain)\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et prend en peu de temps à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et prend un peu plus de temps à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et prend plus de temps à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et prend beaucoup plus de temps à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et prend énormément de temps à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et met des heures à récupérer\n",
	L"L'ennemie est abasourdi en raison de votre attaque au corps-à-corps et vas surement ne pas se relever\n",
	L"Le coups de poing va faire +%d%s de dégâts en plus\n",
	L"Votre coups de pieds spécial va faire +%d%s de dégâts en plus\n",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps à main nues",
	L" ou avec le coup de poing américain",
	L" (+%d%s avec le coup de poing américain)",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps avec un coup de poing américain\n",
	L"+%d%s de chance d'esquiver une attaque de n'importe quelle arme de mêlée\n",
	L"-%d%s du nombre de PA nécessaire pour voler l'arme de son ennemie\n",
	L"-%d%s du nombre de PA nécessaire pour changer de posture (debout, accroupie, coucher), se retourner, monter/descendre du toit et sauter les obstacles\n",
	L"-%d%s du nombre de PA nécessaire pour changer de posture (debout, accroupie, coucher)\n",
	L"-%d%s du nombre de PA nécessaire pour se retourner\n",
	L"-%d%s du nombre de PA nécessaire pour monter/descendre du toit et sauter les obstacles\n",
	L"+%d%s de chance d'ouvrir la porte\n",
	L"Vous obtenez une animation spéciale pour votre combat au corps-à-corps\n",

};
STR16 gzIMPMajorTraitsHelpTextsSquadleader[]=
{
	L"+%d%s de PA par tours pour les mercenaires aux alentours\n",
	L"+%d d'expérience effective pour les mercenaires aux alentours possèdant un level plus bas que %s\n",
	L"+%d effective exp level to count as a standby when counting friends' bonus for suppression\n",
	L"+%d%s de tolérance aux tirs de suppression pour les mercenaires aux alentours et %s pour soi\n",
	L"+%d de gain de moral pour les mercenaires aux alentours\n",
	L"-%d de perte de moral pour les mercenaires aux alentours\n",
	L"Bonus valables pour %d cases aux alentours",
	L" (%d cases pour en encadrement élargis)",
	L"(Le maximum de bonus simultanés pour un joueur est de %d)\n",
	L"+%d%s de crainte de résistance à %s\n",
	L"Inconvénient: %dx perte de moral à la mort de %s pour les autres mercenaires\n",

};
STR16 gzIMPMajorTraitsHelpTextsTechnician[]=
{
	L"+%d%s de vitesse de réparation\n",
	L"+%d%s pour le crochetage (serrures normales/électroniques)\n",
	L"+%d%s pour désarmer un piège électronique\n",
	L"+%d%s pour attacher un objet spécial ou combiner différents objets\n",
	L"+%d%s pour débloquer une arme en plein combat\n",
	L"Réduit la pénalité pour réparer les objets électroniques de %d%s\n",
	L"Augmente la chance de repèrer les mines et les pièges (+%d par level de détection)\n",
	L"+%d%s de chance de toucher avec le robot controlé par %s\n",
	L"%s vous accorde la capacité de réparer le robot\n",
	L"Pénalité réduite pour réparer la vitesse du robot de %d%s\n",
};
STR16 gzIMPMajorTraitsHelpTextsDoctor[]=
{
	L"A les talents pour faire une opération chirurchical, en utilisant un sac médical, sur le soldat blessé\n",
	L"La chirurgie rend immédiatement %d%s de santé perdue.",
	L" (Cela consomme une grande partie du sac médical.)",
	L"Peut guérir des stats perdues par les coups critiques par le",
	L" chirurgien ou",
	L" le docteur assigné.\n",
	L"+%d%s de l'efficacité du docteur assigné\n",
	L"+%d%s de vitesse de bandage\n",
	L"+%d%s de la vitesse de régénération naturel de tous soldats présent dans le même secteur",
	L" (un maximum de %d bonus par secteur)",

};
STR16 gzIMPMajorTraitsHelpTextsNone[]=
{
	L"Pas de bonus",
};

STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[]=
{
	L"Réduit la pénalité de tirer avec 2 armes de %d%s\n",
	L"+%d%s de vitesse de rechargement d'une arme avec un chargeur\n",
	L"+%d%s de vitesse de rechargement d'une arme avec des cartouches\n",
	L"-%d%s du nombre de PA nécessaire pour prendre un objet\n",
	L"-%d%s du nombre de PA nécessaire pour utiliser le backpack\n",
	L"-%d%s du nombre de PA nécessaire pour ouvrir une porte\n",
	L"-%d%s du nombre de PA nécessaire pour poser/retirer une bombe ou une mine\n",
	L"-%d%s du nombre de PA nécessaire pour attacher des objets entre eux\n",
};
STR16 gzIMPMinorTraitsHelpTextsMelee[]=
{
	L"-%d%s du nombre de PA nécessaire pour attaquer avec un couteau\n",
	L"+%d%s de chance de toucher avec un couteau\n",
	L"+%d%s de chance de toucher avec une arme de mêlée\n",
	L"+%d%s de dégâts par un couteau\n",
	L"+%d%s de dégâts par une arme de mêlée\n",
	L"L'attaque visée par n'importe quelle arme de mêlée cause +%d%s de dégâts\n",
	L"+%d%s de chance d'esquiver des attaques avec un couteau\n",
	L"+%d%s de chance d'esquiver une attaque avec un couteau si vous avez un couteau à la main\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée si vous avez une arme de mêlée à la main\n",

};
STR16 gzIMPMinorTraitsHelpTextsThrowing[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer un couteau\n",
	L"+%d%s de distance maximum quand vous lancez un couteau\n",
	L"+%d%s de chance de toucher votre cible en lancant un couteau\n",
	L"+%d%s de chance de toucher votre cible par niveau de visée en lancant un couteau\n",
	L"+%d%s de dégâts avec votre lancer de couteau\n",
	L"+%d%s de dégâts avec votre lancer de couteau par niveau de visée\n",
	L"+%d%s de chance d'infliger des dégâts critiques en lançant un couteau si vous n'êtes pas vue ou entendu\n",
	L"+%d de dégâts critiques si vous lancez plusieurs couteaux\n",
	L"Ajoute %d niveaux de visée pour lancer un couteau\n",
	L"Ajoute %d niveaux de visée pour lancer un couteau\n",

};
STR16 gzIMPMinorTraitsHelpTextsNightOps[]=
{
	L"Vue effective augmentée de +%d dans la nuit\n",
	L"Audition augmentée de +%d\n",
	L"Audition augmentée de +%d lorsque vous êtes en hauteur\n",
	L"+%d de chance d'interrompre une action d'une ennemi dans la nuit\n",
	L"Le besoin de dormir est réduit de -%d\n",

};
STR16 gzIMPMinorTraitsHelpTextsStealthy[]=
{
	L"-%d%s du nombre de PA nécessaire pour bouger silencieusement\n",
	L"+%d%s de chance de bouger silencieusement\n",
	L"+%d%s en discrétion (étant 'invisible si inaperçu)\n",
	L"Réduit la pénalité dû au déplacement silencieux de %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsAthletics[]=
{
	L"-%d%s du nombre de PA nécessaire pour bouger (courir, marcher, tapper, ramper, nager, etc)\n",
	L"-%d%s d'énergie dépensée pour les mouvements, monter sur les toits, sauter les obstacles, etc.\n",
};
STR16 gzIMPMinorTraitsHelpTextsBodybuilding[]=
{
	L"A %d%s de résistance aux dégâts\n",
	L"+%d%s de force effective pour porter de lourdes charges\n",
	L"Réduit la perte d'énergie lorsque vous êtes touché au corps-à-corps de %d%s\n",
	L"Augmente les dégâts nécessaire pour que vous tombiez à terre, si vous êtes touché aux jambes, de %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsDemolitions[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer une grenade\n",
	L"+%d%s de la portée maximale d'une grenade\n",
	L"+%d%s de chance de toucher votre cible avec une grenade\n",
	L"+%d%s de dégâts causés par une bombe ou une mine\n",
	L"+%d%s pour contrôler un détonateur\n",
	L"+%d%s pour placer/retirer un contrôleur de bombe\n",
	L"Diminue la chance que l'ennemie puisse détecter vos bombes/mines (+%d par level de bombe)\n",
	L"Augmente la chance de former une plus grosse détonation quand l'ennemie ouvrira une porte piégée (dégâts multipliés par %d)\n",

};
STR16 gzIMPMinorTraitsHelpTextsTeaching[]=
{
	L"+%d%s de bonus pour entraîner la milice\n",
	L"+%d%s de bonus en Commandement pour entraîner la milice\n",
	L"+%d%s de bonus pour entraîner d'autres mercenaires\n",
	L"Il faut que votre compétence soit supérieure à +%d pour être capable d'enseigner cette habilité à un autre mercenaire\n",
	L"+%d%s de bonus lorsque vous vous entraînez tout seul\n",

};
STR16 gzIMPMinorTraitsHelpTextsScouting[]=
{
	L"+%d de distance effective avec une lunette de visée sur votre arme\n",
	L"+%d de distance effective avec vos jumelles (et la lunette de visée séparée de votre arme)\n",
	L"-%d de vues étroites avec vos jumelles (et la lunette de visée séparée de votre arme)\n",
	L"Si vous êtes dans le secteur, vous saurez le nombre exact d'ennemies présent dans les secteurs aux alentours\n",
	L"Si vous êtes dans le secteur,vous saurez la présence au non d'ennemies dans les secteurs alentours\n",
	L"Empêche l'ennemie de prendre en embuscade votre équipe\n",
	L"Empêche les félins (les gros chats) de prendre en embuscade votre équipe\n",

};
STR16 gzIMPMinorTraitsHelpTextsNone[]=
{
	L"Pas de bonus",
};

STR16 gzIMPOldSkillTraitsHelpTexts[]=
{
	L"+%d%s de bonus de crochetage\n",	// 0
	L"+%d%s de chance de toucher au corps-à-corps\n",
	L"+%d%s de dégâts au corps-à-corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps\n",
	L"Elimine la pénalité dû à la réparation et à la manipulation\nd'objets électriques (serrures, pièges, détonateurs, etc)\n",
	L"+%d de vision effective dans la nuit\n",
	L"+%d d'audition effective dans la nuit\n",
	L"+%d d'audition effective dnas la nuit sur un toit\n",
	L"+%d de chance d'interrompre un ennemie dans la nuit\n",
	L"Le besoin de dormir est réduit de -%d\n",
	L"+%d%s de distance maximale lors d'un lancer quelconque\n",	// 10
	L"+%d%s de chance de toucher lors d'un lancer quelconque\n",
	L"+%d%s de chance de tuer instantanément en lançant un couteau si vous n'êtes pas vu ou entendu\n",
	L"+%d%s de bonus pour entraîner la milice et enseigner aux autres mercenaires\n",
	L"+%d%s en Commandement lors de l'entraînement de la milice mobile\n",
	L"+%d%s de chance de toucher votre cible avec une roquette/grenade ou un mortier\n",
	L"La pénalité dû au tir en mode automatique est réduite de %d\n",
	L"Réduit la probabilité d'un tir en mode automatique non voulu\n",
	L"+%d%s de chance de bouger silencieusement\n",
	L"+%d%s en ruse (étant 'invisible si inaperçu)\n",
	L"Elimine la pénalité lorsque vous tirez avec une arme dans chaque main à la suite\n",	// 20
	L"+%d%s de chance de toucher avec une arme de mêlée\n",
	L"+%d%s de chance d'esquiver une attaque de mêlée si vous avez un couteau dans la main\n",
	L"+%d%s de chance d'esquiver une attaque de mêlée si vous avez quelque chose d'autre dans la main\n",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps si vous avez un couteau dans la main\n",
	L"-%d%s de distance effective nécessaire pour viser votre cible avec n'importe quelle arme\n",
	L"+%d%s de bonus par niveau de visée\n",
	L"Fournit un camouflage permanent\n",
	L"+%d%s de chance de toucher au corps-à-corps\n",
	L"+%d%s de dégâts au corps-à-corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps si vous avez les mains vides\n",	// 30
	L"+%d%s de chance d'esquiver une attaque au corps-à-corps si vous n'avez pas les mains vides\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée\n",
	L"Peut faire un coup de pied à des ennemies affaiblis qui fera le double de dégâts\n",
	L"Vous obtenez une animation spéciale pour votre combat au corps-à-corps\n",
	L"Pas de bonus",
};

STR16 gzIMPNewCharacterTraitsHelpTexts[]=
{
	L"+ : Pas d'avantages.\n- : Pas de désaventages.",
	L"+ : A de meilleures performances lorsque deux ou trois mercenaires sont proches.\n- : Ne gagne aucun moral quand aucun mercenaire est proche de lui.",
	L"+ : A de meilleures performances quand il est tout seul.\n- : Ne gagne aucun moral quand il est en groupe.",
	L"+ : Son moral diminue plus doucement et remonte plus rapidemment que la normale.\n- : A moins de chance de détecter les mines et les pièges.",
	L"+ : Obtiens un bonus lorsqu'il entraîne la milice et à une meilleur communication.\n- : Ne gagne aucun moral pour les actions des autres mercenaires.",
	L"+ : Apprend plus rapidemment en étant le professeur ou l'élève.\n- : A moins de résistance à la peur et la folie.",
	L"+ : Son énergie descend un peu plus lentement sauf lorsqu'il est docteur, qu'il répare, qu'il entraîne ou qu'il apprend.\n- : Ses compétences en sagesse, commandement, explosifs, mécanique et médecine s'améliorent légèrement plus lentement.",
	L"+ : A un peu plus de chance de toucher lors d'un tir automatique et inflige plus de dégâts au corps-à-corps.\n	Obtiens un peu plus de morale lors d'un kill.\n- : A une pénalité lorsqu'il faut de la patience comme réparer des objets, dévérouiller une serrure, enlever des pièges, entraîner la milice.",
	L"+ : A un bonus lorsqu'il faut de la patience comme réparer des objets, dévérouiller une serrure, enlever des pièges, entraîner la milice.\n- : Sa chance d'interrompre une action ennemie est légèrement diminué.",
	L"+ : Augmente la résistance à la folie et la peur.\n    La perte de moral dû aux dégâts reçus et à la mort d'un mercenaire est moindre.\n- : Vous êtes plus facilement touchable et l'ennemie a sa pénalité dû à votre mouvement réduite.",
	L"+ : Gagne du moral lorsque vous faîtes une mission qui n'est pas lié au combat (excepté l'entraînement de milice).\n- : Pas de gains lorsque vous tuez quelqu'un.",
	L"+ : A plus de chance d'infliger des pertes de stats sur l'ennemie, qui peut aussi infliger de lourds dégâts.\n	Gagne du moral lorsque vous infligez des pertes de stats sur l'ennemie.\n- : A une pénalité pour la communication et son moral baisse plus rapidement lorsqu'il ne combat pas.",
	L"+ : A de meilleures performances lorsqu'un certains types d'ennemies est opposé à lui.\n- : Les mercenaires qui possèdent le même type que l'ennemie gagne moins de moral.",

};

STR16 gzIMPDisabilitiesHelpTexts[]=
{
	L"Pas d'effets.",
	L"A des problèmes de souffle/respiration et ses performances globales\nsont diminuées lorsqu'il est dans des zones tropiques ou désertiques.",
	L"Peut souffrir de panique s'il est laissé seul dans certaines situations.",
	L"Ses performances globales sont réduites s'il se trouve dans un sous-sol.",
	L"Peut facilement se noyer s'il essaye de nager.",
	L"La vue de gros insectes peut lui poser de gros problèmes\net être dans une zone tropicale lui réduit aussi\nlégèrement ses performances globales.",
	L"Peut parfois perdre les ordres donnés et ainsi perdre des PA lors d'un combat.",
	L"Il peut devenir psychopathe et tirer comme un fou de temps en temps\net peut perdre du moral s'il n'est pas capable d'utiliser son arme.",

};



STR16 gzIMPProfileCostText[]=
{
	L"Ce profil coûte %d$. Voulez-vous autoriser ce paiement ? ",
};

STR16 zGioNewTraitsImpossibleText[]=
{
	L"Vous ne pouvez pas choisir le Nouveau Système de Trait avec l'outil PROFEX désactivé. Regardez votre fichier JA2_Options.ini avec l'entrée: READ_PROFILE_DATA_FROM_XML.",
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Vous avez choisi le mode IRON MAN. La difficultée du jeu s'en trouveras considerablement augmentée du fait de l'imposssiblité de sauvegarder en territoire ennemi. Ce paramètre prendra effet tout au long de la partie.  Etes-vous vraiment sûr de vouloir jouer en mode IRON MAN ?",
};

STR16 gzDisplayCoverText[]=
{
	L"Contraste: %d/100 %s, Luminosité: %d/100",
	L"Distance de tir: %d/%d cases, Chance de toucher: %d/100",
	L"Distance de tir: %d/%d cases, Stabilité du canon: %d/100",
	L"Désactivation de la couverture de l'affichage",
	L"Afficher la vue qu'on les ennemies",
	L"Afficher les zones de danger des mercenaires",
	L"Bois", // wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Urbain",
	L"Désert",
	L"Neige", // NOT USED!!!
	L"Bois et désert",
	L"" // yes empty for now
};


#endif
