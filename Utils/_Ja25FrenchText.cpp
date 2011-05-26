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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SANDRO - New STOMP laptop strings
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
	L"(Expert)",

};

//added another set of skill texts for new major traits
STR16 gzIMPSkillTraitsTextNewMajor[]=
{
	L"Auto Weapons",
	L"Heavy Weapons",
	L"Marksman",
	L"Hunter",
	L"Gunslinger",
	L"Hand to Hand",
	L"Deputy",
	L"Technician",
	L"Paramedic",

	L"None",
	L"I.M.P. Major Traits",
	// second names
	L"Machinegunner",
	L"Bombardier",
	L"Sniper",
	L"Ranger",
	L"Gunfighter",
	L"Martial Arts",
	L"Squadleader",
	L"Engineer",
	L"Doctor",
};

//added another set of skill texts for new minor traits
STR16 gzIMPSkillTraitsTextNewMinor[]=
{
	L"Ambidextrous",
	L"Melee",
	L"Throwing",
	L"Night Ops",
	L"Stealthy",
	L"Athletics",
	L"Bodybuilding",
	L"Demolitions",
	L"Teaching",
	L"Scouting",

	L"None",
	L"I.M.P. Minor Traits",
};

//these texts are for help popup windows, describing trait properties
STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[]=
{
	L"+%d%s Chance to Hit with Assault Rifles\n",
	L"+%d%s Chance to Hit with SMGs\n",
	L"+%d%s Chance to Hit with LMGs\n",
	L"-%d%s APs needed to fire with LMGs\n",
	L"-%d%s APs needed to ready light machine guns\n",
	L"Auto fire/burst chance to hit penalty is reduced by %d%s\n",
	L"Reduced chance for shooting unwanted bullets on autofire\n",

};
STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[]=
{
	L"-%d%s APs needed to fire grenade launchers\n",
	L"-%d%s APs needed to fire rocket launchers\n",
	L"+%d%s chance to hit with grenade launchers\n",
	L"+%d%s chance to hit with rocket launchers\n",
	L"-%d%s APs needed to fire mortar\n",
	L"Reduce penalty for mortar CtH by %d%s\n",
	L"+%d%s damage to tanks with heavy weapons, grenades and explosives\n",
	L"+%d%s damage to other targets with heavy weapons\n",

};
STR16 gzIMPMajorTraitsHelpTextsSniper[]=
{
	L"+%d%s Chance to Hit with Rifles\n",
	L"+%d%s Chance to Hit with Sniper Rifles\n",
	L"-%d%s effective range to target with all weapons\n",
	L"+%d%s aiming bonus per aim click (except for handguns)\n",
	L"+%d%s damage on shot",
	L" plus",
	L" per every aim click",
	L" after first",
	L" after second",
	L" after third",
	L" after fourth",
	L" after fifth",
	L" after sixth",
	L" after seventh",
	L"-%d%s APs needed to chamber a round with bolt-action rifles \n",
	L"Adds one more aim click for rifle-type guns\n",
	L"Adds %d more aim clicks for rifle-type guns\n",

};
STR16 gzIMPMajorTraitsHelpTextsRanger[]=
{
	L"+%d%s Chance to Hit with Rifles\n",
	L"+%d%s Chance to Hit with Shotguns\n",
	L"-%d%s APs needed to pump Shotguns\n",
	L"+%d%s group travelling speed between sectors if traveling by foot\n",
	L"+%d%s group travelling speed between sectors if traveling in vehicle (except helicopter)\n",
	L"-%d%s less energy spent for travelling between sectors\n",
	L"-%d%s weather penalties\n",
	L"+%d%s camouflage effectiveness\n",
	L"-%d%s worn out speed of camouflage by water or time\n",

};
STR16 gzIMPMajorTraitsHelpTextsGunslinger[]=
{
	L"-%d%s APs needed to fire with pistols and revolvers\n",
	L"+%d%s effective range with pistols and revolvers\n",
	L"+%d%s chance to hit with pistols and revolvers\n",
	L"+%d%s chance to hit with machine pistols",
	L" (on single shots only)",
	L"+%d%s aiming bonus per click with pistols, machine pistols and revolvers\n",
	L"-%d%s APs needed to raise pistols and revolvers\n",
	L"-%d%s APs needed to reload pistols, machine pistols and revolvers\n",
	L"Adds %d more aim click for pistols, machine pistols and revolvers\n",
	L"Adds %d more aim clicks for pistols, machine pistols and revolvers\n",

};
STR16 gzIMPMajorTraitsHelpTextsMartialArts[]=
{
	L"-%d%s AP cost of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"+%d%s chance to hit with hand to hand attacks with bare hands\n",
	L"+%d%s chance to hit with hand to hand attacks with brass knuckles\n",
	L"+%d%s damage of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"+%d%s breath damage of hand to hand attacks(bare hands or with brass knuckles)\n",
	L"Enemy knocked out due to your HtH attacks takes slightly longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes much longer to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes very long to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes extremely long to recuperate\n",
	L"Enemy knocked out due to your HtH attacks takes long hours to recuperate\n",
	L"Enemy knocked out due to your HtH attacks probably never stand up\n",
	L"Focused (aimed) punch deals +%d%s more damage\n",
	L"Your special spinning kick deals +%d%s more damage\n",
	L"+%d%s change to dodge hand to hand attacks\n",
	L"+%d%s on top chance to dodge HtH attacks with bare hands",
	L" or brass knuckles",
	L" (+%d%s with brass knuckles)",
	L"+%d%s on top chance to dodge HtH attacks with brass knuckles\n",
	L"+%d%s chance to dodge attacks by any melee weapon\n",
	L"-%d%s APs needed to steal weapon from enemy hands\n",
	L"-%d%s APs needed to change state (stand, crouch, lie down), turn around, climb on/off roof and jump obstacles\n",
	L"-%d%s APs needed to change state (stand, crouch, lie down)\n",
	L"-%d%s APs needed to turn around\n",
	L"-%d%s APs needed to climb on/off roof and jump obstacles\n",
	L"+%d%s chance to kick doors\n",
	L"You gain special animations for hand to hand combat\n",

};
STR16 gzIMPMajorTraitsHelpTextsSquadleader[]=
{
	L"+%d%s APs per round of other mercs in vicinity\n",
	L"+%d effective exp level of other mercs in vicinity, which have lesser level than the %s\n",
	L"+%d effective exp level to count as a standby when counting friends' bonus for suppression\n",
	L"+%d%s total suppression tolerance of other mercs in vicinity and %s himself\n",
	L"+%d morale gain of other mercs in vicinity\n",
	L"-%d morale loss of other mercs in vicinity\n",
	L"The vicinity for bonuses is %d tiles",
	L" (%d tiles with extended ears)",
	L"(Max simultaneous bonuses for one soldier is %d)\n",
	L"+%d%s fear resistence of %s\n",
	L"Drawback: %dx morale loss for %s's death for all other mercs\n",

};
STR16 gzIMPMajorTraitsHelpTextsTechnician[]=
{
	L"+%d%s to repairing speed\n",
	L"+%d%s to lockpicking (normal/electronic locks)\n",
	L"+%d%s to disarming electronic traps\n",
	L"+%d%s to attaching special items and combining things\n",
	L"+%d%s to unjamming a gun in combat\n",
	L"Reduce penalty to repair electronic items by %d%s\n",
	L"Increased chance to detect traps and mines (+%d detect level)\n",
	L"+%d%s CtH of robot controlled by the %s\n",
	L"%s trait grants you the ability to repair the robot\n",
	L"Reduced penalty to repair speed of the robot by %d%s\n",
};
STR16 gzIMPMajorTraitsHelpTextsDoctor[]=
{
	L"Has ability to make surgical intervention by using medical bag on wounded soldier\n",
	L"Surgery instantly returns %d%s of lost health back.",
	L" (This drains the medical bag a lot.)",
	L"Can heal lost stats (from critical hits) by the",
	L" surgery or",
	L" doctor assignment.\n",
	L"+%d%s effectiveness on doctor-patient assignment\n",
	L"+%d%s bandaging speed\n",
	L"+%d%s natural regeneration speed of all soldiers in the same sector",
	L" (max %d these bonuses per sector)",

};
STR16 gzIMPMajorTraitsHelpTextsNone[]=
{
	L"No bonuses",
};

STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[]=
{
	L"Reduce penalty to shoot dual weapons by %d%s\n",
	L"+%d%s speed of reloading guns with magazines\n",
	L"+%d%s speed of reloading guns with loose rounds\n",
	L"-%d%s APs needed to pickup items\n",
	L"-%d%s APs needed to work backpack\n",
	L"-%d%s APs needed to handle doors\n",
	L"-%d%s APs needed to plant/remove bombs and mines\n",
	L"-%d%s APs needed to attach items\n",
};
STR16 gzIMPMinorTraitsHelpTextsMelee[]=
{
	L"-%d%s APs needed to attack by blades\n",
	L"+%d%s chance to hit with blades\n",
	L"+%d%s chance to hit with blunt melee weapons\n",
	L"+%d%s damage of blades\n",
	L"+%d%s damage of blunt melee weapons\n",
	L"Aimed attack by any melee weapon deals +%d%s damage\n",
	L"+%d%s chance to dodge attack by melee blades\n",
	L"+%d%s on top chance to dodge melee blades if having a blade in hands\n",
	L"+%d%s chance to dodge attack by blunt melee weapons\n",
	L"+%d%s on top chance to dodge blunt melee weapons if having a blade in hands\n",

};
STR16 gzIMPMinorTraitsHelpTextsThrowing[]=
{
	L"-%d%s basic APs needed to throw blades\n",
	L"+%d%s max range when throwing blades\n",
	L"+%d%s chance to hit when throwing blades\n",
	L"+%d%s chance to hit when throwing blades per aim click\n",
	L"+%d%s damage of throwing blades\n",
	L"+%d%s damage of throwing blades per aim click\n",
	L"+%d%s chance to inflict critical hit by throwing blade if not seen or heard\n",
	L"+%d critical hit by throwing blade multiplier\n",
	L"Adds %d more aim click for throwing blades\n",
	L"Adds %d more aim clicks for throwing blades\n",

};
STR16 gzIMPMinorTraitsHelpTextsNightOps[]=
{
	L"+%d to effective sight range in dark\n",
	L"+%d to general effective hearing range\n",
	L"+%d to effective hearing range in dark on top\n",
	L"+%d to interrupts modifier in dark\n",
	L"-%d need to sleep\n",

};
STR16 gzIMPMinorTraitsHelpTextsStealthy[]=
{
	L"-%d%s APs needed to move quietly\n",
	L"+%d%s chance to move quietly\n",
	L"+%d%s stealth (being 'invisible' if unnoticed)\n",
	L"Reduced cover penalty for movement by %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsAthletics[]=
{
	L"-%d%s APs needed for moving (running, walking, swatting, crawling, swimming, etc.)\n",
	L"-%d%s energy spent for movement, roof-climbing, obstacle-jumping, swimming, etc.\n",
};
STR16 gzIMPMinorTraitsHelpTextsBodybuilding[]=
{
	L"Has %d%s damage resistance\n",
	L"+%d%s effective strength for carrying weight capacity \n",
	L"Reduced energy lost when hit by HtH attack by %d%s\n",
	L"Increased damage needed to fall down if hit to legs by %d%s\n",

};
STR16 gzIMPMinorTraitsHelpTextsDemolitions[]=
{
	L"-%d%s APs needed to throw grenades\n",
	L"+%d%s max range when throwing grenades\n",
	L"+%d%s chance to hit when throwing grenades\n",
	L"+%d%s damage of set bombs and mines\n",
	L"+%d%s to attaching detonators check\n",
	L"+%d%s to planting/removing bombs check\n",
	L"Decreases chance enemy will detect your bombs and mines (+%d bomb level)\n",
	L"Increased chance shaped charge will open the doors (damage multiplied by %d)\n",

};
STR16 gzIMPMinorTraitsHelpTextsTeaching[]=
{
	L"+%d%s bonus to train militia\n",
	L"+%d%s bonus to effective leadership for determining militia training\n",
	L"+%d%s bonus to teaching other mercs\n",
	L"Skill value counts to be +%d higher for being able to teach this skill to other mercs\n",
	L"+%d%s bonus to train stats through self-practising assignment\n",

};
STR16 gzIMPMinorTraitsHelpTextsScouting[]=
{
	L"+%d to effective sight range with scopes on weapons\n",
	L"+%d to effective sight range with binoculars (and scopes separated from weapons)\n",
	L"-%d tunnel vision with binoculars (and scopes separated from weapons)\n",
	L"If in sector, adjacent sectors will show exact number of enemies\n",
	L"If in sector, adjacent sectors will show presence of enemies if any\n",
	L"Prevents the enemy to ambush your squad\n",
	L"Prevents the bloodcats to ambush your squad\n",

};
STR16 gzIMPMinorTraitsHelpTextsNone[]=
{
	L"No bonuses",
};

STR16 gzIMPOldSkillTraitsHelpTexts[]=
{
	L"+%d%s bonus to lockpicking\n",	// 0
	L"+%d%s hand to hand chance to hit\n",
	L"+%d%s hand to hand damage\n",
	L"+%d%s chance to dodge hand to hand attacks\n",
	L"Eliminates the penalty to repair and handle\nelectronic things (locks, traps, rem. detonators, robot, etc.)\n",
	L"+%d to effective sight range in dark\n",
	L"+%d to general effective hearing range\n",
	L"+%d to effective hearing range in dark on top\n",
	L"+%d to interrupts modifier in dark\n",
	L"-%d need to sleep\n",
	L"+%d%s max range when throwing anything\n",	// 10
	L"+%d%s chance to hit when throwing anything\n",
	L"+%d%s chance to instantly kill by throwing knife if not seen or heard\n",
	L"+%d%s bonus to train militia and instruct other mercs\n",
	L"+%d%s effective leadership for militia training calculations\n",
	L"+%d%s chance to hit with rocket/greande launchers and mortar\n",
	L"Auto fire/burst chance to hit penalty is divided by %d\n",
	L"Reduced chance for shooting unwanted bullets on autofire\n",
	L"+%d%s chance to move quietly\n",
	L"+%d%s stealth (being 'invisible' if unnoticed)\n",
	L"Eliminates the CtH penalty for second hand when firing two weapons at once\n",	// 20
	L"+%d%s chance to hit with melee blades\n",
	L"+%d%s chance to dodge attacks by melee blades if having blade in hands\n",
	L"+%d%s chance to dodge attacks by melee blades if having anything else in hands\n",
	L"+%d%s chance to dodge hand to hand attacks if having blade in hands\n",
	L"-%d%s effective range to target with all weapons\n",
	L"+%d%s aiming bonus per aim click\n",
	L"Provides permanent camouflage\n",
	L"+%d%s hand to hand chance to hit\n",
	L"+%d%s hand to hand damage\n",
	L"+%d%s chance to dodge hand to hand attacks if having empty hands\n",	// 30
	L"+%d%s chance to dodge hand to hand attacks if not having empty hands\n",
	L"+%d%s chance to dodge attacks by melee blades\n",
	L"Can perform spinning kick attack on weakened enemies to deal double damage\n",
	L"You gain special animations for hand to hand combat\n",
	L"No bonuses",
};

STR16 gzIMPNewCharacterTraitsHelpTexts[]=
{
	L"A: No advantage.\nD: No disadvantage.",
	L"A: Has better performance when a couple of mercs are nearby.\nD: Gains no morale when no other merc is nearby.",
	L"A: Has better performance when no other merc is nearby.\nD: Gains no morale when in a group.",
	L"A: His morale sinks a little slower and grows faster than normal.\nD: Has lesser chance to detect traps and mines.",
	L"A: Has bonus on training militia and is better at communication with people.\nD: Gains no morale for actions of other mercs.",
	L"A: Slightly faster learning when assigned on practicing or as a student.\nD: Has lesser suppression and fear resistance.",
	L"A: His energy goes down a bit slower except on assignments such as doctor, repairman, militia trainer or if learning certain skills.\nD: His wisdom, leadership, explosives, mechanical and medical skills improve slightly slower.",
	L"A: Has slightly better chance to hit on burst/autofire and inflicts slightly bigger damage in close combat\n    Gains a little more morale for killing.\nD: Has penalty for actions which need patience like repairing items, picking locks, removing traps, doctoring, training militia.",
	L"A: Has bonus for actions which need patience like repairing items, picking locks, removing traps, doctoring and training militia.\nD: His interrupt chance is slightly lowered.",
	L"A: Increased resistance to suppression and fear.\n    Morale loss for taking damage and companions deaths is lower for him.\nD: Can be hit easier and enemy penalty for moving target is lesser in his case.",
	L"A: He gains morale when on non-combat assignments (except training militia).\nD: Gains no morale for killing.",
	L"A: Has greater chance of inflicting stat loss, which may also inflict special painful wounds.\n   Gains bonus morale for inflicting stat loss.\nD: Has penalty for communication with people and his morale sinks faster if not fighting.",
	L"A: Has better performance when there are some mercs of opposite gender nearby.\nD: Morale of other mercs of the same gender grows slower if nearby.",

};

STR16 gzIMPDisabilitiesHelpTexts[]=
{
	L"No effects.",
	L"Has problems with breathing and reduced overall performance if in tropical or desert sectors.",
	L"Can suffer panic attack if left alone in certain situations.",
	L"His overall performance is reduced if underground.",
	L"If trying to swim he can easily drown.",
	L"A look at large insects can cause big problems\nand being in tropical sectors also reduce his performance a bit.",
	L"Sometimes forgets what orders he got and therefore loses some APs if in combat.",
	L"He can go psycho and shoot like mad once in a while\nand can lose morale if unable to do so with given weapon.",

};



STR16 gzIMPProfileCostText[]=
{
	L"The profile cost is %d$. Do you authorize the payment? ",
};

STR16 zGioNewTraitsImpossibleText[]=
{
	L"You cannot choose the New Trait System with PROFEX utility deactivated. Check your JA2_Options.ini for entry: READ_PROFILE_DATA_FROM_XML.",
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//@@@:  New string as of March 3, 2000.
STR16	gzIronManModeWarningText[]=
{
	L"Vous avez choisi le mode IRON MAN. La difficultée du jeu s'en trouveras considerablement augmentée du fait de l'imposssiblité de sauvegarder en territoire ennemi. Ce paramètre prendra effet tout au long de la partie.  Etes vous vraiment sûr de vouloir jouer en mode IRON MAN ?",
};

STR16 gzDisplayCoverText[]=
{
	L"Cover: %d/100 %s, Brightness: %d/100",
	L"Gun Range: %d/%d tiles, Chance to hit: %d/100",
	L"Gun Range: %d/%d tiles, Muzzle Stability: %d/100",
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
