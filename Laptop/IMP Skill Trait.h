#ifndef __IMP_SKILL_TRAIT__H_
#define __IMP_SKILL_TRAIT__H_

#include "Types.h"

void EnterIMPSkillTrait( void );
void RenderIMPSkillTrait( void );
void ExitIMPSkillTrait( void );
void HandleIMPSkillTrait( void );

enum
{
	IMP_SKILL_TRAITS__LOCKPICK,
	IMP_SKILL_TRAITS__HAND_2_HAND,
	IMP_SKILL_TRAITS__ELECTRONICS,
	IMP_SKILL_TRAITS__NIGHT_OPS,
	IMP_SKILL_TRAITS__THROWING,
	IMP_SKILL_TRAITS__TEACHING,
	IMP_SKILL_TRAITS__HEAVY_WEAPONS,
	IMP_SKILL_TRAITS__AUTO_WEAPONS,
	IMP_SKILL_TRAITS__STEALTHY,
	IMP_SKILL_TRAITS__AMBIDEXTROUS,
	IMP_SKILL_TRAITS__KNIFING,
	IMP_SKILL_TRAITS__PROF_SNIPER,
	IMP_SKILL_TRAITS__CAMO,
	IMP_SKILL_TRAITS__MARTIAL_ARTS,
	IMP_SKILL_TRAITS__NONE,
	IMP_SKILL_TRAITS__NUMBER_SKILLS,
};

// other set for new traits - SANDRO
enum
{
	// major traits
	IMP_SKILL_TRAITS_NEW_AUTO_WEAPONS,
	IMP_SKILL_TRAITS_NEW_HEAVY_WEAPONS,
	IMP_SKILL_TRAITS_NEW_PROF_SNIPER,
	IMP_SKILL_TRAITS_NEW_RANGER,
	IMP_SKILL_TRAITS_NEW_GUNSLINGER,
	IMP_SKILL_TRAITS_NEW_MARTIAL_ARTS,
	IMP_SKILL_TRAITS_NEW_SQUADLEADER,
	IMP_SKILL_TRAITS_NEW_TECHNICIAN,
	IMP_SKILL_TRAITS_NEW_DOCTOR,
	IMP_SKILL_TRAITS_NEW_MAJOR_NONE,
	IMP_SKILL_TRAITS_NEW_NUMBER_MAJOR_SKILLS,
};

INT8	CountNumSkillTraitsSelected( BOOLEAN fIncludeNoneSkill );
INT32	DoesPlayerHaveExtraAttibutePointsToDistributeBasedOnSkillSelection();

void AddSelectedSkillsToSkillsList();

INT32 StrengthRequiredDueToMajorSkills( void );
INT32 AgilityRequiredDueToMajorSkills( void );
INT32 DexterityRequiredDueToMajorSkills( void );
INT32 HealthRequiredDueToMajorSkills( void );
INT32 LeadershipRequiredDueToMajorSkills( void );
INT32 WisdomRequiredDueToMajorSkills( void );
INT32 MarksmanshipRequiredDueToMajorSkills( void );
INT32 MechanicalRequiredDueToMajorSkills( void );
INT32 MedicalRequiredDueToMajorSkills( void );
INT32 ExplosivesRequiredDueToMajorSkills( void );

#endif
