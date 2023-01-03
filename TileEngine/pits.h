#ifndef __PITS_H
#define __PITS_H

void Add3X3Pit( INT32 iMapIndex );
void Add5X5Pit( INT32 iMapIndex );
void Remove3X3Pit( INT32 iMapIndex );
void Remove5X5Pit( INT32 iMapIndex );

void SearchForOtherMembersWithinPitRadiusAndMakeThemFall( INT32 sGridNo, INT16 sRadius );

void AddAllPits();
void RemoveAllPits();

extern BOOLEAN gfShowPits;
extern BOOLEAN gfLoadPitsWithoutArming;

void HandleFallIntoPitFromAnimation( UINT8 ubID );


#endif