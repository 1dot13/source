#ifndef _ITERFACEITEMIMAGES_H_
#define _ITERFACEITEMIMAGES_H_

#include "Types.h"
#include "VFS/vfs_types.h"
#include "VFS/vfs_path.h"
#include <map>

class MDItemVideoObjects
{
public:
	MDItemVideoObjects();

	UINT32	getVObjectForItem(UINT32 key);
	void	registerItem(UINT32 key, vfs::Path const& sFileName);
	bool	registerItemsFromFilePattern(vfs::Path const& sFilePattern);
	void	unRegisterAllItems();
private:
	std::map<UINT32,UINT32> m_mapVObjects;
};


extern bool					g_bUsePngItemImages;
// old item image handles
extern UINT32				guiGUNSM;
extern UINT32				guiP1ITEMS;
extern UINT32				guiP2ITEMS;
extern UINT32				guiP3ITEMS;
// new item image handles
extern MDItemVideoObjects	g_oGUNSM;
extern MDItemVideoObjects	g_oP1ITEMS;
extern MDItemVideoObjects	g_oP2ITEMS;
extern MDItemVideoObjects	g_oP3ITEMS;

bool RegisterItemImages();

#endif // _ITERFACEITEMIMAGES_H_
