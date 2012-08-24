#ifndef _JA25ENGLISHTEXT__H_
#define _JA25ENGLISHTEXT__H_

extern	STR16 gzIMPSkillTraitsText[];

////////////////////////////////////////////////////////
// added by SANDRO
extern	STR16 gzIMPSkillTraitsTextNewMajor[];
extern	STR16 gzIMPSkillTraitsTextNewMinor[];

extern  STR16 gzIMPMajorTraitsHelpTextsAutoWeapons[];
extern  STR16 gzIMPMajorTraitsHelpTextsHeavyWeapons[];
extern  STR16 gzIMPMajorTraitsHelpTextsSniper[];
extern  STR16 gzIMPMajorTraitsHelpTextsRanger[];
extern  STR16 gzIMPMajorTraitsHelpTextsGunslinger[];
extern  STR16 gzIMPMajorTraitsHelpTextsMartialArts[];
extern  STR16 gzIMPMajorTraitsHelpTextsSquadleader[];
extern  STR16 gzIMPMajorTraitsHelpTextsTechnician[];
extern  STR16 gzIMPMajorTraitsHelpTextsDoctor[];
extern  STR16 gzIMPMajorTraitsHelpTextsCovertOps[];		// added by Flugente
extern  STR16 gzIMPMajorTraitsHelpTextsNone[];

extern	STR16 gzIMPMinorTraitsHelpTextsAmbidextrous[];
extern	STR16 gzIMPMinorTraitsHelpTextsMelee[];
extern	STR16 gzIMPMinorTraitsHelpTextsThrowing[];
extern	STR16 gzIMPMinorTraitsHelpTextsStealthy[];
extern	STR16 gzIMPMinorTraitsHelpTextsNightOps[];
extern	STR16 gzIMPMinorTraitsHelpTextsAthletics[];
extern	STR16 gzIMPMinorTraitsHelpTextsBodybuilding[];
extern	STR16 gzIMPMinorTraitsHelpTextsDemolitions[];
extern	STR16 gzIMPMinorTraitsHelpTextsTeaching[];
extern	STR16 gzIMPMinorTraitsHelpTextsScouting[];
extern	STR16 gzIMPMinorTraitsHelpTextsNone[];

extern STR16 gzIMPOldSkillTraitsHelpTexts[];

extern STR16 gzIMPNewCharacterTraitsHelpTexts[];

extern STR16 gzIMPDisabilitiesHelpTexts[];

extern STR16 gzIMPProfileCostText[];

extern STR16 zGioNewTraitsImpossibleText[];
///////////////////////////////////////////////////////

enum
{
	IMM__IRON_MAN_MODE_WARNING_TEXT,
};
extern	STR16	gzIronManModeWarningText[];

// display cover message (for tactical usually, seperated)
// display cover terrain type info (used in cover information)
enum
{
	DC_MSG__COVER_INFORMATION,
	DC_MSG__GUN_RANGE_INFORMATION,
	DC_MSG__NCTH_GUN_RANGE_INFORMATION,
	DC_MSG__COVER_DRAW_OFF,
	DC_MSG__COVER_DRAW_MERC_VIEW,
	DC_MSG__COVER_DRAW_ENEMY_VIEW,
	DC_TTI__WOOD,
	DC_TTI__URBAN,
	DC_TTI__DESERT,
	DC_TTI__SNOW,
	DC_TTI__WOOD_AND_DESERT,
	DC_TTI__UNKNOWN
};
extern STR16 gzDisplayCoverText[];

#endif


