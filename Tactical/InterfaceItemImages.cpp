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

//MM: New item tileslots start here
UINT32					guiP4ITEMS;
UINT32					guiP5ITEMS;
UINT32					guiP6ITEMS;
UINT32					guiP7ITEMS;
UINT32					guiP8ITEMS;
UINT32					guiP9ITEMS;

// new item image handles
MDItemVideoObjects		g_oGUNSM;
MDItemVideoObjects		g_oP1ITEMS;
MDItemVideoObjects		g_oP2ITEMS;
MDItemVideoObjects		g_oP3ITEMS;

//MM: New item tileslots start here
MDItemVideoObjects		g_oP4ITEMS;
MDItemVideoObjects		g_oP5ITEMS;
MDItemVideoObjects		g_oP6ITEMS;
MDItemVideoObjects		g_oP7ITEMS;
MDItemVideoObjects		g_oP8ITEMS;
MDItemVideoObjects		g_oP9ITEMS;

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

	//MM: New item tileslots start here
	// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp4items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP4ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp4items.sti" );
	}
	else if(!g_oP4ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP4ITEMS/*.png"))
	{
		return false;
	}

		// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp5items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP5ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp5items.sti" );
	}
	else if(!g_oP5ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP5ITEMS/*.png"))
	{
		return false;
	}

		// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp6items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP6ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp6items.sti" );
	}
	else if(!g_oP6ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP6ITEMS/*.png"))
	{
		return false;
	}

		// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp7items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP7ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp7items.sti" );
	}
	else if(!g_oP7ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP7ITEMS/*.png"))
	{
		return false;
	}

		// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp8items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP8ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp8items.sti" );
	}
	else if(!g_oP8ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP8ITEMS/*.png"))
	{
		return false;
	}

		// LOAD INTERFACE ITEM PICTURES
	if(!g_bUsePngItemImages)
	{
		VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
		FilenameForBPP("INTERFACE\\mdp9items.sti", VObjectDesc.ImageFile);
		if( !AddVideoObject( &VObjectDesc, &guiP9ITEMS ) )
			AssertMsg(0, "Missing INTERFACE\\mdp9items.sti" );
	}
	else if(!g_oP9ITEMS.registerItemsFromFilePattern(L"INTERFACE/MDP9ITEMS/*.png"))
	{
		return false;
	}

	return true;
}

