#ifndef __MILITIA_WEBSITE_H
#define __MILITIA_WEBSITE_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// main page (explains what this is all about)
BOOLEAN EnterMilitiaWebsiteMain( );
void ExitMilitiaWebsiteMain( );
void HandleMilitiaWebsiteMain( );
void RenderMilitiaWebsiteMain( );

STR16 OperationText( UINT32 aNum );

extern std::vector<UINT32> gIndividualMilitiaFilteredIdsVector;

#endif //__MILITIA_WEBSITE_H
