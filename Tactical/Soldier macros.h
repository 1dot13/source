#ifndef SOLDIER_MACROS_H
#define SOLDIER_MACROS_H

// MACROS FOR EASIER SOLDIER CONTROL
#include "teamturns.h"
#include "soldier profile.h"
#include "assignments.h"
#include "Animation Data.h"

// MACROS
#define RPC_RECRUITED( p )	( ( p->ubProfile == NO_PROFILE ) ? FALSE : ( gMercProfiles[ p->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ) )

#define AM_AN_EPC( p )	( ( p->ubProfile == NO_PROFILE ) ? FALSE : ( gMercProfiles[ p->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE ) )

// rftr - this is for madlab's robot, since it has a profile
#define AM_A_ROBOT( p )	( ( p->ubProfile == NO_PROFILE ) ? FALSE : ( gMercProfiles[ p->ubProfile ].ubBodyType == ROBOTNOWEAPON ) )


#define OK_ENEMY_MERC( p ) ( !p->aiData.bNeutral && (p->bSide != gbPlayerNum ) && p->stats.bLife >= OKLIFE && (p->bTeam < 5 ))

// Checks if our guy can be controllable .... checks bInSector, team, on duty, etc...

// Checks if our guy is controllable but doesn't care about current assignment
#define OK_CONTROL_MERC( p ) ( p->stats.bLife >= OKLIFE && p->bActive && p->bInSector && p->bTeam == gbPlayerNum && !(p->usSkillCooldown[SOLDIER_COOLDOWN_CRYO]) )

#define OK_CONTROLLABLE_MERC( p ) ( OK_CONTROL_MERC(p) && ( p->bAssignment < ON_DUTY || p->bAssignment == VEHICLE )	)

// Checks if our guy can be controllable .... checks bInSector, team, on duty, etc...
#define OK_INSECTOR_MERC( p ) ( p->stats.bLife >= OKLIFE && p->bActive && p->bInSector && p->bTeam == gbPlayerNum && p->bAssignment < ON_DUTY )	

// Checkf if our guy can be selected and is not in a position where our team has an interupt and he does not have one...
#define OK_INTERRUPT_MERC( p ) ( ( INTERRUPT_QUEUED != 0 ) ? ( ( p->aiData.bMoved ) ? FALSE : TRUE ) : TRUE )

#define CREATURE_OR_BLOODCAT( p ) ( (p->flags.uiStatusFlags & SOLDIER_MONSTER) || p->ubBodyType == BLOODCAT )

#define COMBAT_JEEP( p ) ( p->ubBodyType == COMBAT_JEEP )
#define TANK( p ) (p->ubBodyType == TANK_NE || p->ubBodyType == TANK_NW )
#define ENEMYROBOT( p ) (p->ubBodyType == ROBOTNOWEAPON && p->bTeam == ENEMY_TEAM)
#define ARMED_VEHICLE( p )	( TANK( p ) || COMBAT_JEEP(p) )
#define BOXER( p ) ( p->flags.uiStatusFlags & SOLDIER_BOXER )
#define ISVIP( p ) ( p->usSoldierFlagMask & SOLDIER_VIP )

//#define OK_ENTERABLE_VEHICLE( p )	( ( p->flags.uiStatusFlags & SOLDIER_VEHICLE ) && !TANK( p ) && p->stats.bLife >= OKLIFE	)
#define OK_ENTERABLE_VEHICLE( p )	( ( p->flags.uiStatusFlags & SOLDIER_VEHICLE ) && (!ARMED_VEHICLE( p ) || !(p->flags.uiStatusFlags & SOLDIER_ENEMY) ) && p->stats.bLife >= OKLIFE	)

#endif
