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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - New STOMP laptop strings
//these strings match up with the defines in IMP Skill trait.cpp
STR16 gzIMPSkillTraitsText[]=
{
	L"Crochetage",
	L"Corps à corps",
	L"Électronique",
	L"Opérations de nuit",
	L"Lancer",
	L"Instructeur",
	L"Armes lourdes",
	L"Armes automatiques",
	L"Discrétion",
	L"Ambidextre",
	L"Couteau",
	L"Tireur isolé",
	L"Camouflage",
	L"Arts martiaux",

	L"aucune",
	L"IMP : Spécialtés",
	L"(Expert)",

};

//added another set of skill texts for new major traits
STR16 gzIMPSkillTraitsTextNewMajor[]=
{
	L"Armes automatiques",
	L"Armes lourdes",
	L"Tireur d'élite",
	L"Éclaireur",
	L"Flingueur",
	L"Arts martiaux",
	L"Meneur",
	L"Technicien",
	L"Médecin",
	L"Déguisement",

	L"Personne",
	L"IMP : Traits pincipaux",
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
	L"Espion",
};

//added another set of skill texts for new minor traits
STR16 gzIMPSkillTraitsTextNewMinor[]=
{
	L"Ambidextre",
	L"Mêlée",
	L"Le lancer",
	L"Opérations de nuit",
	L"Discrétion",
	L"Athlétique",
	L"Culturiste",
	L"Sabotage",
	L"Instructeur",
	L"Reconnaissance",
	L"Opérateur radio",
	L"Survival",	//TODO.Translate

	L"Personne",
	L"IMP : Traits mineurs",
};

//these texts are for help popup windows, describing trait properties
STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[]=
{
	L"+%d%s de chance de toucher avec un fusil d'assaut\n",
	L"+%d%s de chance de toucher avec un pistolet mitrailleur\n",
	L"+%d%s de chance de toucher avec une mitrailleuse légère\n",
	L"-%d%s du nombre de PA nécessaire pour tirer avec une mitrailleuse légère\n",
	L"-%d%s du nombre de PA nécessaire pour préparer une arme automatique légère\n",
	L"La pénalité due au mode rafale/auto, est réduite de %d%s\n",
	L"Réduire la probabilité de tirer plus de balles que prévu en mode auto\n",

};
STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer une grenade\n",
	L"-%d%s du nombre de PA nécessaire pour tirer une roquette\n",
	L"+%d%s de chance de toucher avec une grenade\n",
	L"+%d%s de chance de toucher avec une roquette\n",
	L"-%d%s du nombre de PA nécessaire pour tirer au mortier\n",
	L"Réduit la pénalité de chance de toucher du mortier de %d%s\n",
	L"+%d%s dégâts fait à un char avec une arme lourde, grenades ou explosifs\n",
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
	L"-%d%s du nombre de PA nécessaire pour recharger avec un fusil à action manuelle\n",
	L"Ajoute un niveau de visée en plus pour chaque fusil\n",
	L"Ajoute %d niveau(x) de visée pour chaque fusil\n",

	L"Fait viser plus rapidement avec un : réduit d'un niveau de visée\n",
	L"Fait viser plus rapidement avec un fusil : réduit de %d niveau(x) de visée\n",

};
STR16 gzIMPMajorTraitsHelpTextsRanger[]=
{
	L"+%d%s de chance de toucher avec un fusil\n",
	L"+%d%s de chance de toucher avec un fusil à pompe\n",
	L"-%d%s du nombre de PA nécessaire pour recharger le fusil à pompe\n",
	L"-%d%s PA pour tirer avec les fusils à pompe\n",
	L"Ajoute un niveau de visée pour les fusils à pompe\n",
	L"Ajoute %d niveaux de visée pour les fusils à pompe\n",
	L"+%d%s effective range with Shotguns\n",  // TODO.Translate
	L"-%d%s APs to reload single Shotgun shells\n",  // TODO.Translate
	L"Adds %d more aim click for Rifles\n",  // TODO.Translate
	L"Adds %d more aim clicks for Rifles\n",  // TODO.Translate
};

STR16 gzIMPMajorTraitsHelpTextsGunslinger[]=
{
	L"-%d%s du nombre de PA nécessaire pour tirer avec un pistolet ou un revolver\n",
	L"+%d%s de la distance effective avec un pistolet ou un revolver pour viser\n",
	L"+%d%s de chance de toucher avec un pistolet ou un revolver\n",
	L"+%d%s de chance de toucher avec un pistolet automatique",
	L" (en tir manuel seulement)",
	L"+%d%s de bonus par niveau de visée avec un pistolet, pistolet automatique ou un revolver\n",
	L"-%d%s du nombre de PA nécessaire pour dégainer un pistolet, pistolet automatique ou un revolver\n",
	L"-%d%s du nombre de PA nécessaire pour recharger un pistolet, pistolet automatique ou un revolver\n",
	L"Ajoute un niveau de visée pour les pistolets, les pistolets automatiques ou les revolvers\n",
	L"Ajoute %d niveaux de visée pour les pistolets, les pistolets automatiques ou les revolvers\n",

};
STR16 gzIMPMajorTraitsHelpTextsMartialArts[]=
{
	L"-%d%s du nombre de PA nécessaire pour les attaques au corps à corps (mains nues ou avec un coup de poing américain)\n",
	L"+%d%s de chance de toucher avec les mains nues\n",
	L"+%d%s de chance de toucher avec un coup de poing américain\n",
	L"+%d%s de dégâts des attaques au corps à corps (mains nues ou avec un coup de poing américain)\n",
	L"+%d%s de dégâts sur le souffle des attaques au corps à corps (mains nues ou avec un coup de poing américain)\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et prend en peu de temps à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et prend un peu plus de temps à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et prend plus de temps à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et prend beaucoup plus de temps à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et prend énormément de temps à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et met des heures à récupérer\n",
	L"L'ennemi est abasourdi en raison de votre attaque au corps à corps et va sûrement ne pas se relever\n",
	L"Le coups de poing va faire +%d%s de dégâts en plus\n",
	L"Votre coup de pied spécial va faire +%d%s de dégâts en plus\n",
	L"+%d%s de chance d'esquiver une attaque au corps à corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps à corps à mains nues",
	L" ou avec le coup de poing américain",
	L" (+%d%s avec le coup de poing américain)",
	L"+%d%s de chance d'esquiver une attaque au corps à corps avec un coup de poing américain\n",
	L"+%d%s de chance d'esquiver une attaque de n'importe quelle arme de mêlée\n",
	L"-%d%s du nombre de PA nécessaire pour voler l'arme de son ennemi\n",
	L"-%d%s du nombre de PA nécessaire pour changer de posture (debout, accroupie, coucher), se retourner, monter/descendre du toit et sauter les obstacles\n",
	L"-%d%s du nombre de PA nécessaire pour changer de posture (debout, accroupie, coucher)\n",
	L"-%d%s du nombre de PA nécessaire pour se retourner\n",
	L"-%d%s du nombre de PA nécessaire pour monter/descendre du toit et sauter les obstacles\n",
	L"+%d%s de chance d'ouvrir la porte\n",
	L"Vous obtenez une animation spéciale pour votre combat au corps à corps\n",
	L"-%d%s de chance d'être interrompu quand vous bougez\n",

};
STR16 gzIMPMajorTraitsHelpTextsSquadleader[]=
{
	L"+%d%s de PA par tour pour les mercenaires aux alentours\n",
	L"+%d d'expérience effective pour les mercenaires aux alentours possédant un niveau plus bas que : %s\n",
	L"+%d d'expérience effective pour le calcul de l'aide apportée à un coéquipier avec un tir de couverture\n",
	L"+%d%s de tolérance aux tirs de couverture pour les mercenaires aux alentours et %s pour soi\n",
	L"+%d de gain de moral pour les mercenaires aux alentours\n",
	L"-%d de perte de moral pour les mercenaires aux alentours\n",
	L"Bonus valables pour %d cases aux alentours",
	L" (%d cases pour en encadrement élargi)",
	L"(Le maximum de bonus simultané pour un joueur est de %d)\n",
	L"+%d%s de résistance à la peur du %s\n",
	L"Inconvénient : %dx perte de moral à la mort du %s pour les autres mercenaires\n",
	L"+%d%s chance de déclencher des interruptions de groupe\n",

};
STR16 gzIMPMajorTraitsHelpTextsTechnician[]=
{
	L"+%d%s de vitesse de réparation\n",
	L"+%d%s pour le crochetage (serrure normale/électronique)\n",
	L"+%d%s pour désactiver un piège électronique\n",
	L"+%d%s pour attacher un objet spécial ou combiner différents objets\n",
	L"+%d%s pour débloquer une arme en plein combat\n",
	L"Réduit la pénalité pour réparer les objets électroniques de %d%s\n",
	L"Augmente la chance de repérer les mines et les pièges (+%d par niveau de détection)\n",
	L"+%d%s de chance de toucher avec le robot contrôlé par %s\n",
	L"%s vous accorde la capacité de réparer le robot\n",
	L"Pénalité réduite sur la vitesse à réparer le robot de %d%s\n",
	L"Able to restore item threshold to 100%% during repair\n",	// TODO.Translate
};
STR16 gzIMPMajorTraitsHelpTextsDoctor[]=
{
	L"A les talents pour faire une opération chirurgicale, en utilisant une trousse médicale, sur le soldat blessé\n",
	L"La chirurgie rend immédiatement %d%s de santé perdue.",
	L" (Cela consomme une grande partie de la trousse médicale.)",
	L"Peut guérir des stats perdues par les coups critiques par le",
	L" chirurgien ou",
	L" le docteur assigné.\n",
	L"+%d%s de l'efficacité du docteur assigné\n",
	L"+%d%s de vitesse de bandage\n",
	L"+%d%s de la vitesse de régénération naturelle de tous soldats présent dans le même secteur",
	L" (un maximum de %d bonus par secteur)",

};
STR16 gzIMPMajorTraitsHelpTextsCovertOps[]=		
{
	L"Peut se déguiser en civil ou en soldat pour se glisser derrière les lignes ennemies\n",
	L"Sera détecté(e) par des actions suspectes ou un paquetage douteux\nou d'être près d'un cadavre encore chaud\n",
	L"Sera automatiquement détecté(e) avec un\ndéguisement de soldat à %d cases d'un ennemi\n",
	L"Sera automatiquement détecté(e) avec un déguisement de soldat à %d cases d'un cadavre encore chaud\n",
	L"+%d%s de chance de toucher avec une arme d'espion\n",
	L"+%d%s de chance de tuer un ennemi instantanément avec une arme d'espion\n",
	L"Le nombre de PA pour se déguiser, est réduit de %d%s\n",
};

STR16 gzIMPMajorTraitsHelpTextsRadioOperator[]= 
{
	L"Peut utiliser du matériel de communication\n", 
	L"Peut demander des tirs d'artillerie alliés des secteurs voisins.\n",
	L"Via l'assignation 'Balayer les fréquences', les patrouilles ennemies seront localisées.\n",
	L"Les communications peuvent être brouillées à l'échelle d'un secteur.\n",
	L"Si les communications sont brouillées, l'opérateur peut rechercher la source du brouillage.\n",
	L"Peut appeler la milice des secteurs voisins en renfort.\n",
};

STR16 gzIMPMajorTraitsHelpTextsNone[]=
{
	L"Pas de bonus",
};

STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[]=
{
	L"Réduit la pénalité de tirer avec deux armes de %d%s\n",
	L"+%d%s de vitesse de rechargement d'une arme avec un chargeur\n",
	L"+%d%s de vitesse de rechargement d'une arme avec des cartouches\n",
	L"-%d%s du nombre de PA nécessaire pour prendre un objet\n",
	L"-%d%s du nombre de PA nécessaire pour utiliser le sac à dos\n",
	L"-%d%s du nombre de PA nécessaire pour ouvrir une porte\n",
	L"-%d%s du nombre de PA nécessaire pour poser/retirer un explosif ou une mine\n",
	L"-%d%s du nombre de PA nécessaire pour attacher des objets entre eux\n",
};
STR16 gzIMPMinorTraitsHelpTextsMelee[]=
{
	L"-%d%s du nombre de PA nécessaire pour attaquer avec une arme blanche\n",
	L"+%d%s de chance de toucher avec une arme blanche\n",
	L"+%d%s de chance de toucher avec une arme de mêlée\n",
	L"+%d%s de dégâts par une arme blanche\n",
	L"+%d%s de dégâts par une arme de mêlée\n",
	L"L'attaque visée par n'importe quelle arme de mêlée cause +%d%s de dégâts\n",
	L"+%d%s de chance d'esquiver des attaques avec une arme blanche\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme blanche, si vous avez une arme blanche à la main\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée, si vous avez une arme de mêlée à la main\n",

};
STR16 gzIMPMinorTraitsHelpTextsThrowing[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer un couteau\n",
	L"+%d%s de distance maximum quand vous lancez un couteau\n",
	L"+%d%s de chance de toucher votre cible en lançant un couteau\n",
	L"+%d%s de chance de toucher votre cible par niveau de visée en lançant un couteau\n",
	L"+%d%s de dégâts avec votre lancer de couteau\n",
	L"+%d%s de dégâts avec votre lancer de couteau par niveau de visée\n",
	L"+%d%s de chance d'infliger des dégâts critiques en lançant un couteau, si vous n'êtes pas vue ou entendu\n",
	L"+%d de dégâts critiques, si vous lancez plusieurs couteaux\n",
	L"Ajoute %d niveau(x) de visée pour lancer un couteau\n",
	L"Ajoute %d niveau(x) de visée pour lancer un couteau\n",

};
STR16 gzIMPMinorTraitsHelpTextsNightOps[]=
{
	L"Vue effective augmentée de +%d pendant la nuit\n",
	L"Audition augmentée de +%d\n",
	L"Audition augmentée de +%d lorsque vous êtes en hauteur\n",
	L"+%d de chance d'interrompre une action d'un ennemi pendant la nuit\n",
	L"Le besoin de dormir est réduit de -%d\n",

};
STR16 gzIMPMinorTraitsHelpTextsStealthy[]=
{
	L"-%d%s du nombre de PA nécessaire pour bouger silencieusement\n",
	L"+%d%s de chance de bouger silencieusement\n",
	L"+%d%s en discrétion (étant invisible, si inaperçu)\n",
	L"Réduit la pénalité due au déplacement discrétion de %d%s\n",
	L"-%d%s de chances d'être interrompu\n",

};
STR16 gzIMPMinorTraitsHelpTextsAthletics[]=
{
	L"-%d%s du nombre de PA nécessaire pour bouger (courir, marcher, s'accroupir, ramper, nager, etc.)\n",
	L"-%d%s d'énergie dépensée pour les mouvements, monter sur les toits, sauter les obstacles, etc.\n",
};
STR16 gzIMPMinorTraitsHelpTextsBodybuilding[]=
{
	L"A %d%s de résistance aux dégâts\n",
	L"+%d%s de force effective pour porter de lourdes charges\n",
	L"Réduit la perte d'énergie lorsque vous êtes touché au corps à corps de %d%s\n",
	L"Augmente de %d%s les dégâts nécessaires pour tomber à terre quand vous êtes touché aux jambes\n",

};
STR16 gzIMPMinorTraitsHelpTextsDemolitions[]=
{
	L"-%d%s du nombre de PA nécessaire pour lancer une grenade\n",
	L"+%d%s de la portée maximale d'une grenade\n",
	L"+%d%s de chance de toucher votre cible avec une grenade\n",
	L"+%d%s de dégâts causés par un explosif ou une mine\n",
	L"+%d%s pour contrôler un détonateur\n",
	L"+%d%s pour placer/retirer un contrôleur d'explosif\n",
	L"Diminue la chance que l'ennemi puisse détecter vos explosifs/mines (+%d par niveau d'explosif)\n",
	L"Augmente la chance de former une plus grosse détonation quand l'ennemi ouvrira une porte piégée (dégâts multipliés par %d)\n",

};
STR16 gzIMPMinorTraitsHelpTextsTeaching[]=
{
	L"+%d%s de bonus pour entraîner la milice\n",
	L"+%d%s de bonus en commandement pour entraîner la milice\n",
	L"+%d%s de bonus pour entraîner d'autres mercenaires\n",
	L"Il faut que votre compétence soit supérieure à +%d pour être capable d'enseigner cette habilité à un autre mercenaire\n",
	L"+%d%s de bonus lorsque vous vous entraînez tout seul\n",

};
STR16 gzIMPMinorTraitsHelpTextsScouting[]=
{
	L"+%d%% de distance effective avec une lunette de visée sur votre arme\n",
	L"+%d%% de distance effective avec vos jumelles (et la lunette de visée séparée de votre arme)\n",
	L"-%d%% de vues étroites avec vos jumelles (et la lunette de visée séparée de votre arme)\n",
	L"Si vous êtes dans le secteur, vous saurez le nombre exact d'ennemis présents dans les secteurs alentours\n",
	L"Si vous êtes dans le secteur, vous saurez la présence au non d'ennemis dans les secteurs alentours\n",
	L"Empêche l'ennemi de prendre en embuscade votre escouade\n",
	L"Empêche les chats sauvages de prendre en embuscade votre escouade\n",
};
STR16 gzIMPMinorTraitsHelpTextsSnitch[]=
{
	L"Vous informera à l'occasion sur les opinions de ses coéquipiers.\n",
	L"Dénoncer la mauvaise conduite de ses équipiers (drogue, alcool et chapardage).\n",
	L"Peut faire de la propagande dans les villes.\n",
	L"Peut recueillir les rumeurs dans les villes.\n",
	L"Peut infiltrer les prisons.\n",
	L"Augmente votre réputation de %d tous les jours si le moral est bon.\n",
	L"+ %d de portée auditive\n",
};

STR16 gzIMPMajorTraitsHelpTextsSurvival[] =	// TODO.Translate
{
	L"+%d%s de vitesse de déplacement du groupe entre les secteurs, s'ils sont à pied\n",
	L"+%d%s de vitesse de déplacement du groupe entre les secteurs, s'ils sont dans un véhicule (sauf l'hélicoptère)\n",
	L"-%d%s d'énergies nécessaire pour traverser les secteurs\n",
	L"-%d%s de pénalités du temps\n",
	//L"+%d%s d'efficacité du camouflage\n",
	L"-%d%s de l'usure du camouflage due au temps ou à l'eau\n",
	L"Can spot tracks up to %d tiles away\n",	// TODO.Translate

	L"%s%d%% disease resistance\n",
	L"%s%d%% food consumption\n",
	L"%s%d%% water consumption\n",
	L"+%d%% snake evasion\n",	// TODO.Translate
};

STR16 gzIMPMinorTraitsHelpTextsNone[]=
{
	L"Pas de bonus",
};

STR16 gzIMPOldSkillTraitsHelpTexts[]=
{
	L"+%d%s de bonus de crochetage\n",	// 0
	L"+%d%s de chance de toucher au corps à corps\n",
	L"+%d%s de dégâts au corps à corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps à corps\n",
	L"Élimine la pénalité due à la réparation et à la manipulation\nd'objets électriques (serrures, pièges, détonateurs, etc.)\n",
	L"+%d de vision effective pendant la nuit\n",
	L"+%d d'audition effective pendant la nuit\n",
	L"+%d d'audition effective pendant la nuit sur un toit\n",
	L"+%d de chance d'interrompre un ennemi pendant la nuit\n",
	L"Le besoin de dormir est réduit de -%d\n",
	L"+%d%s de distance maximale lors d'un lancer quelconque\n",	// 10
	L"+%d%s de chance de toucher lors d'un lancer quelconque\n",
	L"+%d%s de chance de tuer instantanément en lançant un couteau, si vous n'êtes pas vu ou entendu\n",
	L"+%d%s de bonus pour entraîner la milice et enseigner les autres mercenaires\n",
	L"+%d%s en commandement lors de l'entraînement de la milice mobile\n",
	L"+%d%s de chance de toucher votre cible avec une roquette/grenade ou un mortier\n",
	L"La pénalité due au tir en mode automatique est réduite de %d\n",
	L"Réduit la probabilité d'un tir en mode automatique non voulu\n",
	L"+%d%s de chance de bouger silencieusement\n",
	L"+%d%s en ruse (étant invisible, si inaperçu)\n",
	L"Élimine la pénalité lorsque vous tirez avec une arme dans chaque main à la suite\n",	// 20
	L"+%d%s de chance de toucher avec une arme de mêlée\n",
	L"+%d%s de chance d'esquiver une attaque de mêlée, si vous avez une arme blanche dans la main\n",
	L"+%d%s de chance d'esquiver une attaque de mêlée, si vous avez quelque chose d'autre dans la main\n",
	L"+%d%s de chance d'esquiver une attaque au corps à corps, si vous avez une arme blanche dans la main\n",
	L"-%d%s de distance effective nécessaire pour viser votre cible avec n'importe quelle arme\n",
	L"+%d%s de bonus par niveau de visée\n",
	L"Fournit un camouflage permanent\n",
	L"+%d%s de chance de toucher au corps à corps\n",
	L"+%d%s de dégâts au corps à corps\n",
	L"+%d%s de chance d'esquiver une attaque au corps à corps, si vous avez les mains vides\n",	// 30
	L"+%d%s de chance d'esquiver une attaque au corps à corps, si vous n'avez pas les mains vides\n",
	L"+%d%s de chance d'esquiver une attaque avec une arme de mêlée\n",
	L"Peut faire un coup de pied à des ennemis affaiblis qui fera le double de dégâts\n",
	L"Vous obtenez une animation spéciale pour votre combat au corps à corps\n",
	L"Pas de bonus",
};

STR16 gzIMPNewCharacterTraitsHelpTexts[]=
{
	L"+ : Pas d'avantages.\n- : Pas de désavantages.",
	L"+ : A de meilleures performances lorsque deux ou trois mercenaires sont proches.\n- : Ne gagne aucun moral quand aucun mercenaire est proche de lui.",
	L"+ : A de meilleures performances quand il est tout seul.\n- : Ne gagne aucun moral quand il est en groupe.",
	L"+ : Son moral diminue plus doucement et remonte plus rapidement que la normale.\n- : A moins de chance de détecter les mines et les pièges.",
	L"+ : Obtiens un bonus lorsqu'il entraîne la milice et à une meilleure communication.\n- : Ne gagne aucun moral pour les actions des autres mercenaires.",
	L"+ : Apprend plus rapidement en étant le professeur ou l'élève.\n- : A moins de résistance à la peur lors d'un tir de couverture.",
	L"+ : Son énergie descend un peu plus lentement sauf lorsqu'il est docteur, qu'il répare, qu'il entraîne ou qu'il apprend.\n- : Ses compétences en sagesse, commandement, explosifs, mécanique et médecine s'améliorent légèrement plus lentement.",
	L"+ : A un peu plus de chance de toucher lors d'un tir automatique et inflige plus de dégâts au corps à corps.\n    Obtiens un peu plus de morale lors d'un décès.\n- : A une pénalité lorsqu'il faut de la patience comme réparer des objets, déverrouiller une serrure, enlever des pièges, entraîner la milice.",
	L"+ : A un bonus lorsqu'il faut de la patience comme réparer des objets, déverrouiller une serrure, enlever des pièges, entraîner la milice.\n- : Sa chance d'interrompre une action ennemie est légèrement diminuée.",
	L"+ : Augmente la résistance à la peur lors d'un tir de couverture.\n    La perte de moral due aux dégâts reçus et à la mort d'un mercenaire est moindre.\n- : Vous êtes plus facilement vulnérable et l'ennemi a sa pénalité due à votre mouvement, réduite.",
	L"+ : Gagne du moral lorsque vous faites une mission qui n'est pas liée au combat (excepté l'entraînement de milice).\n- : Pas de gains lorsque vous tuez quelqu'un.",
	L"+ : A plus de chance d'infliger des pertes de stats sur l'ennemi, qui peut aussi infliger de lourds dégâts.\n    Gagne du moral lorsque vous infligez des pertes de stats sur l'ennemi.\n- : A une pénalité pour la communication et son moral baisse plus rapidement lorsqu'il ne combat pas.",
	L"+ : A de meilleures performances lorsqu'un certain type d'ennemi est opposé à lui.\n- : Les mercenaires qui possèdent le même type que l'ennemi gagne moins de moral.",

};

STR16 gzIMPDisabilitiesHelpTexts[]=
{
	L"Pas d'effets.",
	L"A des problèmes de souffle/respiration et ses performances globales\nsont diminuées lorsqu'il est dans des zones tropicales ou désertiques.",
	L"Peut souffrir de panique, s'il est laissé seul dans certaines situations.",
	L"Ses performances globales sont réduites, s'il se trouve dans un sous-sol.",
	L"Peut facilement se noyer, s'il essaye de nager.",
	L"La vue de gros insectes, peut lui poser de gros problèmes\net être dans une zone tropicale lui réduit aussi\nlégèrement ses performances globales.",
	L"Peut parfois perdre les ordres donnés et ainsi perdre des PA lors d'un combat.",
	L"Il peut devenir psychopathe et tirer comme un fou de temps en temps\net peut perdre du moral, s'il n'est pas capable d'utiliser son arme.",
	L"Audition considérablement réduite.",
	L"Distance de vision réduit.",
	L"Drastically increased bleeding.",	// TODO.Translate
};



STR16 gzIMPProfileCostText[]=
{
	L"Ce profil coûte %d$. Voulez-vous autoriser ce paiement ? ",
};

STR16 zGioNewTraitsImpossibleText[]=
{
	L"Vous ne pouvez pas choisir le nouveau système de compétences avec l'outil PROFEX désactivé. Regardez votre fichier JA2_Options.ini avec l'entrée : READ_PROFILE_DATA_FROM_XML.",
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Vous avez choisi le mode IRON MAN. La difficulté du jeu s'en trouvera considérablement augmentée du fait de l'impossibilité de sauvegarder en territoire ennemi. Ce paramètre prendra effet tout au long de la partie. Êtes-vous vraiment sûr de vouloir jouer en mode IRON MAN ?",
	L"You have chosen SOFT IRON MAN mode. This setting makes the game slightly more challenging as you will not be able to save your game during turn-based combat. This setting will affect the entire course of the game.  Are you sure want to play in SOFT IRON MAN mode?",// TODO.Translate
	L"You have chosen EXTREME IRON MAN mode. This setting makes the game way more challenging as you will be able to save your progress only once per day - at %02d:00. This setting will affect the entire course of the game.  Do you seriously want to play in EXTREME IRON MAN mode?",// TODO.Translate
};

STR16 gzDisplayCoverText[]=
{
	L"Contraste : %d/100 %s, Luminosité : %d/100",
	L"Distance de tir : %d/%d cases, chance de toucher : %d/100",
	L"Distance de tir : %d/%d cases, stabilité du canon : %d/100",
	L"Désactivation de la couverture de l'affichage",
	L"Afficher le champ de vision du mercenaire sélectionné.",
	L"Afficher les zones de danger du(es) mercenaire(s)",
	L"Bois", // wanted to use jungle , but wood is shorter in german too (dschungel vs wald)
	L"Urbain",
	L"Désert",
	L"Neige",
	L"Bois et désert",
	L"Bois et urbain",
	L"Bois et neige",
	L"Désert et urbain",
	L"Désert et neige",
	L"Urbain et neige",
	L"-", // yes empty for now				// TODO.Translate
	L"Cover: %d/100, Brightness: %d/100",
	L"Footstep volume",
	L"Stealth difficulty",
	L"Trap level",
};


#endif
