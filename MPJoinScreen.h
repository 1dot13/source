#ifndef _MP_JOIN_SCREEN_H_
#define _MP_JOIN_SCREEN_H_

#include <vfs/Core/vfs.h>

namespace vfs{
	class PropertyContainer;
}

class CUniqueServerId
{
public:
	vfs::String const& getServerId(vfs::Path dir, vfs::PropertyContainer* props = NULL);

	static void uniqueRandomString(vfs::String& str);
private:
	vfs::String _id;
};

extern CUniqueServerId s_ServerId;

UINT32	MPJoinScreenInit( void );
UINT32	MPJoinScreenHandle( void );
UINT32	MPJoinScreenShutdown( void );

void	SaveJoinSettings(bool ReSaving);

void	MpIniExists();



#endif
