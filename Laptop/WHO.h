#ifndef __WHO_H
#define __WHO_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// main page (explains what this is all about)
BOOLEAN EnterWHOMain( );
void ExitWHOMain( );
void HandleWHOMain( );
void RenderWHOMain( );

// on this site you can conclude a contract with the WHO to get strategic data on diseases
BOOLEAN EnterWHOContract( );
void ExitWHOContract( );
void HandleWHOContract( );
void RenderWHOContract( );

// this page contains helpful tips on diseases
BOOLEAN EnterWHOTips( );
void ExitWHOTips( );
void HandleWHOTips( );
void RenderWHOTips( );

#endif //__WHO_H
