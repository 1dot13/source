#ifndef __BRIEFINGROOM_H
#define __BRIEFINGROOM_H

#include "Cursors.h"
#include "soldier profile type.h"

void GameInitBriefingRoom();
BOOLEAN EnterBriefingRoom();
void ExitBriefingRoom();
void HandleBriefingRoom();
void RenderBriefingRoom();

BOOLEAN RemoveBriefingRoomDefaults();
BOOLEAN InitBriefingRoomDefaults();
BOOLEAN DrawBriefingRoomDefaults();

BOOLEAN DisplayBriefingRoomSlogan();
BOOLEAN DisplayBriefingRoomCopyright();

#endif