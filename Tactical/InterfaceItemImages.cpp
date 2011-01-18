#include "InterfaceItemImages.h"

#include "DEBUG.H"
#include "vobject.h"
#include "utilities.h"

#include <vfs/Core/vfs.h>

extern void WriteMessageToFile( const STR16 pString );

/******************************************************************************/

bool					g_bUsePngItemImages = false;
// old item image handles
UINT32					guiGUNSM;
UINT32					guiP1ITEMS;
UINT32					guiP2ITEMS;
UINT32					guiP3ITEMS;
// new item image handles
MDItemVideoObjects		g_oGUNSM;
MDItemVideoObjects		g_oP1ITEMS;
MDItemVideoObjects		g_oP2ITEMS;
MDItemVideoObjects		g_oP3ITEMS;

/******************************************************************************/

MDItemVideoObjects::MDItemVideoObjects()
{}

UINT32 MDItemVideoObjects::getVObjectForItem(UINT32 key)
{
	std::map<UINT32,UINT32>::iterator it = m_mapVObjects.find(key);
	if(it != m_mapVObjects.end())
	{
		return it->second;
	}
	SGP_THROW(_BS(L"Item key not registered : ") << key << _BS::wget);	
}

void MDItemVideoObjects::registerItem(UINT32 key, vfs::Path const& sFileName)
{
	std::map<UINT32,UINT32>::iterator it = m_mapVObjects.find(key);
	if(it != m_mapVObjects.end())
	{
		SGP_THROW(_BS(L"Item image already registered : ") << key << _BS::wget);
	}
	// LOAD INTERFACE GUN PICTURES
	UINT32 uiVObject;
	VOBJECT_DESC VObjectDesc;
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP(const_cast<STR>(sFileName.to_string().c_str()), VObjectDesc.ImageFile);
	if(! AddVideoObject( &VObjectDesc, &uiVObject )) 
	{
		SGP_THROW(_BS(L"Could not add video object for file \"") << sFileName << _BS::wget);
	}
	m_mapVObjects.insert(std::make_pair(key,uiVObject));
}

bool MDItemVideoObjects::registerItemsFromFilePattern(vfs::Path const& sFilePattern)
{
	std::wstringstream wss;
	int item = 0;
	vfs::CVirtualFileSystem::Iterator it = getVFS()->begin(sFilePattern);
	if(it.end())
	{
		return false;
	}
	for(; !it.end(); it.next())
	{
		wss.str(it.value()->getName().c_wcs());
		if( !(wss >> item) )
		{
			std::wstring err = _BS(L"Could not extract item number from file \"") << wss.str() << L"\"" << _BS::wget;
			WriteMessageToFile( const_cast<STR16>(err.c_str()) );
			continue;
		}
		try
		{
			this->registerItem(item, it.value()->getPath());
		}
		catch(std::exception& ex)
		{
			SGP_RETHROW( _BS(L"Registering item from file \"") << wss.str() << L"\" failed" << _BS::wget, ex );
		}
	}
	return true;
}


void MDItemVideoObjects::unRegisterAllItems()
{
	std::map<UINT32,UINT32>::iterator it = m_mapVObjects.begin();
	for(; it != m_mapVObjects.end(); ++it)
	{
		// later
	}
}

/******************************************************************************/

bool RegisterItemImages()
{
	VOBJECT_DESC	VObjectDesc;

	//// LOAD INTERFACE GUN PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdguns.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiGUNSM ) )
			AssertMsg(0, "Missing INTERFACE\\mdguns.sti" );
	}
	else if(!g_oGUNSM.registerItemsFromFilePattern(L"INTERFACE/mdguns/*.png"))
	{
		return false;
	}

	// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp1items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP1ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdplitems.sti" );
	}
	else if(!g_oP1ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP1ITEMS/*.png"))
	{
		return false;
	}

	// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp2items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP2ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp2items.sti" );
	}
	else if(!g_oP2ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP2ITEMS/*.png"))
	{
		return false;
	}

	// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp3items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP3ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp3items.sti" );
	}
	else if(!g_oP3ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP3ITEMS/*.png"))
	{
		return false;
	}

	return true;
}

