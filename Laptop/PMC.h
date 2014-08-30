#ifndef __PMC_H
#define __PMC_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// main page (explains what this is all about)
BOOLEAN EnterPMCMain( );
void ExitPMCMain( );
void HandlePMCMain( );
void RenderPMCMain( );

// on this site you can conclude a contract with the WHO to get strategic data on diseases
BOOLEAN EnterPMCContract( );
void ExitPMCContract( );
void HandlePMCContract( );
void RenderPMCContract( );

// in case we hire miltia from the PMC, we have to know how many, where and when they should arrive
typedef struct
{
	UINT8	usId;				// number of this hiring event
	UINT8	usSectorToArrive;
	UINT8	usRegulars;
	UINT8	usVeterans;

	UINT32	usTimeToArrive;
} PMCReinforcesHireEvent;

extern std::vector<PMCReinforcesHireEvent> gPMCHiringEvents;

// global data on the PMC
typedef struct
{
	UINT32	usTotalMoneyEarned;

	UINT16	usRegularsHired;
	UINT16	usVeteransHired;

	UINT8	usRegularsAvailable;
	UINT8	usVeteransAvailable;
	UINT8	usUnused[2];
} PMCGlobalData;

extern PMCGlobalData gPMCData;

void GameInitPMC();
void HourlyUpdatePMC();
void HandlePMCArrival( UINT8 usId );

BOOLEAN SavePMC( HWFILE hwFile );
BOOLEAN LoadPMC( HWFILE hwFile );

#endif //__PMC_H
