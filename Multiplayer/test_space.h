#pragma once

#define ENABLE_COLLISION (is_server && pBullet->pFirer->ubID<120) || (!is_server && is_client && pBullet->pFirer->ubID<20) || (!is_server && !is_client) 

void send_bullet(  BULLET * pBullet, UINT16 usHandItem);

INT8 FireBullet( SOLDIERTYPE * pFirer, BULLET * pBullet, BOOLEAN fFake );

#define NOCDCHECK

extern bool ovh_advance;
extern bool ovh_ready;
void request_ovh(UINT8 ubID);

