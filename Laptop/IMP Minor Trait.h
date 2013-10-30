#ifndef __IMP_MINOR_TRAIT__H_
#define __IMP_MINOR_TRAIT__H_

#include "Types.h"

void EnterIMPMinorTrait( void );
void RenderIMPMinorTrait( void );
void ExitIMPMinorTrait( void );
void HandleIMPMinorTrait( void );

enum
{
	// minor traits
	IMP_SKILL_TRAITS_NEW_AMBIDEXTROUS,
	IMP_SKILL_TRAITS_NEW_MELEE,
	IMP_SKILL_TRAITS_NEW_THROWING,
	IMP_SKILL_TRAITS_NEW_NIGHT_OPS,
	IMP_SKILL_TRAITS_NEW_STEALTHY,
	IMP_SKILL_TRAITS_NEW_ATHLETICS,
	IMP_SKILL_TRAITS_NEW_BODYBUILDING,
	IMP_SKILL_TRAITS_NEW_DEMOLITIONS,
	IMP_SKILL_TRAITS_NEW_TEACHING,
	IMP_SKILL_TRAITS_NEW_SCOUTING,
	IMP_SKILL_TRAITS_NEW_RADIO_OPERATOR,		// added by Flugente
	//////
	IMP_SKILL_TRAITS_NEW_MINOR_NONE,
	IMP_SKILL_TRAITS_NEW_NUMBER_MINOR_SKILLS,
};

void AddSelectedMinorTraitsToSkillsList();

INT8 CountNumMinorTraitsSelected( BOOLEAN fIncludeNoneSkill );

UINT8 StrengthRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 DexterityRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 AgilityRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 HealthRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 WisdomRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 LeaderShipRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 MarksmanshipRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 MechanicalRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 MedicalRequiredAdjustmentForMinorTraits( INT32 iInitialValue );
UINT8 ExplosivesRequiredAdjustmentForMinorTraits( INT32 iInitialValue );

#endif
