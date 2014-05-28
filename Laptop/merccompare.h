#ifndef __MERCCOMPARE_H
#define __MERCCOMPARE_H

/**
* @file
* @author Flugente (bears-pit.com)
*/

// main page (explains what this is all about)
BOOLEAN EnterMercCompareMain( );
void ExitMercCompareMain( );
void HandleMercCompareMain( );
void RenderMercCompareMain( );

BOOLEAN InitMercCompareDefaults( );
void DisplayMercCompareDefaults( );
void RemoveMercCompareDefaults( );
void GetMercCompareText( UINT8 ubNumber, STR16 pString );

// displays a few quotes of customers
void GameInitMercCompareCustomers( );
BOOLEAN EnterMercCompareCustomers( );
void ExitMercCompareCustomers( );
void HandleMercCompareCustomers( );
void RenderMercCompareCustomers( );

// comparison of characters
void GameInitMercCompareAnalyze( );
BOOLEAN EnterMercCompareAnalyze( );
void ExitMercCompareAnalyze( );
void HandleMercCompareAnalyze( );
void RenderMercCompareAnalyze( );

BOOLEAN DisplayMercData( UINT8 usProfileA, UINT8 usProfileB );

// squad opinion overview
void GameInitMercCompareMatrix( );
BOOLEAN EnterMercCompareMatrix( );
void ExitMercCompareMatrix( );
void HandleMercCompareMatrix( );
void RenderMercCompareMatrix( );

#endif //__MERCCOMPARE_H
