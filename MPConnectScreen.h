#ifndef _MP_CONNECT_SCREEN_H_
#define _MP_CONNECT_SCREEN_H_


UINT32	MPConnectScreenInit( void );
UINT32	MPConnectScreenHandle( void );
UINT32	MPConnectScreenShutdown( void );

void	SetConnectScreenHeadingW( STR16 cmsg );
void	SetConnectScreenHeadingA( const char* cmsg );
void	SetConnectScreenSubMessageW( STR16 cmsg );
void	SetConnectScreenSubMessageA( const char* cmsg );

#endif