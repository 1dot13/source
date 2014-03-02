#include "BuildDefines.h"

#ifdef JA2EDITOR
#ifndef __EDITOR_UNDO_H
#define __EDITOR_UNDO_H

BOOLEAN AddToUndoList( INT32 iMapIndex );
void AddLightToUndoList( INT32 iMapIndex, INT32 iLightRadius, UINT8 ubLightID );

BOOLEAN RemoveAllFromUndoList();
BOOLEAN ExecuteUndoList();

void EnableUndo();
void DisableUndo();

void DetermineUndoState();

// Undo command flags
#define MAX_UNDO_COMMAND_LENGTH		10

//dnl ch86 220214
BOOLEAN CheckMapIndexInUndoList(INT32 iMapIndex);
void UpdateItemPoolInUndoList(INT32 iMapIndex, ITEM_POOL *pItemPoolOld, ITEM_POOL *pItemPoolNew);
void UpdateItemPoolMoveInUndoList(INT32 iMapIndex, INT32 iNewMapIndex);
void AddItemPoolGraphic(ITEM_POOL *pItemPoolHead);
void UndoItemPoolGraphicInUndoList(void);
void MergeItemPoolInUndoList(INT32 iMapIndex, ITEM_POOL *pItemPool);

#endif
#endif
