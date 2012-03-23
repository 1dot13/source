#ifndef _ITERFACEITEMIMAGES_H_
#define _ITERFACEITEMIMAGES_H_

#include "Types.h"
#include <vfs/Core/vfs_types.h>
#include <vfs/Core/vfs_path.h>
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

//MM: New item tileslots start here
extern UINT32				guiP4ITEMS;
extern UINT32				guiP5ITEMS;
extern UINT32				guiP6ITEMS;
extern UINT32				guiP7ITEMS;
extern UINT32				guiP8ITEMS;
extern UINT32				guiP9ITEMS;

// new item image handles
extern MDItemVideoObjects	g_oGUNSM;
extern MDItemVideoObjects	g_oP1ITEMS;
extern MDItemVideoObjects	g_oP2ITEMS;
extern MDItemVideoObjects	g_oP3ITEMS;

//MM: New item tileslots start here
extern MDItemVideoObjects	g_oP4ITEMS;
extern MDItemVideoObjects	g_oP5ITEMS;
extern MDItemVideoObjects	g_oP6ITEMS;
extern MDItemVideoObjects	g_oP7ITEMS;
extern MDItemVideoObjects	g_oP8ITEMS;
extern MDItemVideoObjects	g_oP9ITEMS;

bool RegisterItemImages();

#endif // _ITERFACEITEMIMAGES_H_
