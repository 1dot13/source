#ifndef __UI_CURSORS_H
#define __UI_CURSORS_H


#define REFINE_PUNCH_1				0
#define REFINE_PUNCH_2				(gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedPunches*2 : 6)

#define REFINE_KNIFE_1				0
#define REFINE_KNIFE_2				(gGameExternalOptions.fEnhancedCloseCombatSystem ? gSkillTraitValues.ubModifierForAPsAddedOnAimedBladedAttackes*2 : 6)


UINT8	GetProperItemCursor( UINT8 ubSoldierID, UINT16 ubItemIndex, INT32 usMapPos, BOOLEAN fActivated );
void DetermineCursorBodyLocation( UINT8 ubSoldierID, BOOLEAN fDisplay, BOOLEAN fRecalc );

void HandleLeftClickCursor( SOLDIERTYPE *pSoldier );
void HandleRightClickAdjustCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos );
void HandleWheelAdjustCursor( SOLDIERTYPE *pSoldier, INT32 usMapPos, INT32 sDelta, INT16 brstmode );
void HandleWheelAdjustCursorWOAB( SOLDIERTYPE *pSoldier, INT32 sMapPos, INT32 sDelta );

UINT8 GetActionModeCursor( SOLDIERTYPE *pSoldier );

extern BOOLEAN gfCannotGetThrough;

void HandleUICursorRTFeedback( SOLDIERTYPE *pSoldier );
void HandleEndConfirmCursor( SOLDIERTYPE *pSoldier );

BOOLEAN GetMouseRecalcAndShowAPFlags( UINT32 *puiCursorFlags, BOOLEAN *pfShowAPs );

// HEADROCK HAM B2.7: This function calculates the nearest value (display purposes only) 
// based on how trained the shooter is.
UINT32 ChanceToHitApproximation( SOLDIERTYPE * pSoldier, UINT32 uiChance );

#endif
