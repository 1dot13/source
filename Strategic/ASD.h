#ifndef __ASD_H
#define __ASD_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

#include "Types.h"

/** Flugente: Arulco special division decision code
 *
 *	As I do not want to alter the current strategic AI (fragile and obscure as it is), 
 *	I am leaving the control over a number of new toys to a second 'AI' - the 'Arulco Special Division'.
 *	This should control if, how and when new features are used by the AI.
 *	
 *	It has its own 'budget', which allows it to purchase new actions. This serves to limit the amount of the AIs actions.
 *	It also gives the player a way to compete with the AI - lower the AI budget, and the AI can do less harm.
 *	For example, part of the AI budget might come from mine income, which the player can take away.
 *	
 *	If possible, the AI should have to buy and maintain its assets. 
 *	This way the player could harm the AI by specifically targetting those assets if he chooses so.
 *	Ideally this gives the player another immersive way to do sth. in the game.
 *	For example, enemy helicopters have to be bought, fuelled and repaired.
 *	In order to stop enemy heli raids, the player could 
 *	  - shoot them down
 *	  - steal enemy fuel deliveries
 *	  - sabotage enemy helis in their airfield
 *	
 *	There is a cyclic update of this AI. Each time the AI decides whether it wants (and can afford) new assets.
 *	These assets then have to be ordered and can only be used once arrived.
 *	The AI also decides how to use these assets.
 */

void InitASD( );
BOOLEAN SaveASDData( HWFILE hwFile );
BOOLEAN LoadASDData( HWFILE hwFile );

// when called, evaluate the current situation, and perform actions to harm the player if necessary
void UpdateASD( );

// decide whether to purchase something
void ASDDecideOnPurchases();

// decide in what to do with helicopters
void ASDDecideHeliOperations();

// various flags for the AI to remember
#define	ASDFACT_HELI_UNLOCKED				0x00000001			// AI is allowed to purchase helicopters
#define	ASDFACT_JEEP_UNLOCKED				0x00000002			// AI is allowed to purchase jeeps
#define	ASDFACT_TANK_UNLOCKED				0x00000004			// AI is allowed to purchase tanks

void SetASDFlag( UINT32 aFlag );

UINT32 ASDResourceDeliveryTime( UINT8 aType );
UINT32 ASDResourceCostMoney( UINT8 aType );

// add resources to the AIs resource pool
void AddStrategicAIResources( UINT8 aType, INT32 aAmount );

// when we receive an ordered shipment, reduce our backlog
void ASDReduceOrderedStrategicResources( UINT8 aType, INT32 aAmount );

// enemy helis
class ENEMY_HELI
{
public:
	// determine best heli flight path (minimising contact to hostile SAM sectors) and return number of possible SAM contacts
	UINT8 SetHeliFlightPath( UINT8 aDest );

	void Destroy( );

public:
	UINT32	flagmask;			// flags: destroyed, refueling...
	UINT8	sector_current;		// we are here
	UINT8	sector_destination;	// we want to fly here
	UINT8	sector_waypoint;	// a sector we travel to between our starting point and our destiantion. Choosing this gives us a way to plot a path minimising SAM-exposure
	UINT8	sector_home;		// this is where our base is
	UINT8	troopcount;			// number of troops we carry
	UINT8	hp;
	UINT8	fuel;
};

extern std::vector<ENEMY_HELI>	gEnemyHeliVector;

void EnemyHeliInit( );

void AddNewHeli( );

// all enemy helis carrying troops in this sector drop these into combat, and get the order to head home
void EnemyHeliTroopDrop( UINT8 aSector );

std::set<UINT8> GetEnemyHeliSectors( BOOLEAN afKnownToPlayer );

void UpdateEnemyHeliRepair( INT16 id );
void UpdateEnemyHeliRefuel( INT16 id );
void UpdateEnemyHeli( INT16 id );
void EnemyHeliSAMCheck( INT16 id );
void EnemyHeliMANPADSCheck( INT16 id );
void EnemyHeliCheckPlayerKnowledge( INT16 id );

void RepairSamSite( UINT8 aSector );

BOOLEAN IsSectorAirSpacePlayerControlled( UINT8 aSector );

UINT8 NumPlayerAirSpaceOnHeliPath( UINT8 aStart, UINT8 aEnd );

// get the next sector in an enemy heli flight path.
UINT8 GetNextEnemyHeliSector( UINT8 aStart, UINT8 aDest );

void HandleEnemyHelicopterOnGroundGraphic( );

void UpdateAndDamageEnemyHeliIfFound( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ, INT32 sGridNo, UINT8 ubDamage, BOOLEAN aDestroyed = FALSE );


// enemy tanks (how and when they are used)
UINT32 ASDResourceCostFuel( UINT8 aType );

// if ASD is used, any tanks the queen uses in mobile attacks come from its pool, and we have to account for that
BOOLEAN ASDSoldierUpgradeToTank( );
BOOLEAN ASDSoldierUpgradeToJeep( );

#endif //__ASD_H
