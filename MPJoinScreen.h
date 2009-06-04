#ifndef _MP_JOIN_SCREEN_H_
#define _MP_JOIN_SCREEN_H_

#include "VFS/vfs.h"

#ifdef USE_VFS

class CPropertyContainer;

class CUniqueServerId
{
public:
	utf8string const& GetServerId(vfs::Path dir, CPropertyContainer* props = NULL);
private:
	utf8string _id;
};

extern CUniqueServerId s_ServerId;

#endif

UINT32	MPJoinScreenInit( void );
UINT32	MPJoinScreenHandle( void );
UINT32	MPJoinScreenShutdown( void );

void		SaveJoinSettings(bool ReSaving);



#endif
