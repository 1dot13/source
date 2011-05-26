#include <export/jsd/structure.h>
#include <vfs/Core/vfs_file_raii.h>
#include <vfs/Tools/vfs_tools.h>

#include <Types.h>
#include <XMLWriter.h>
#include <sgp_auto_memory.h>
#include <Structure Internals.h>

bool ja2xp::ConvertStructure(vfs::tReadableFile* pStructureFile, vfs::tWritableFile* pOutputFile)
{
	vfs::tReadableFile *pFile = pStructureFile;
	if(!pFile)
	{
		return false;
	}
	vfs::COpenReadFile rfile(pFile);

	XMLWriter xmlw;
	xmlw.addAttributeToNextValue( "filename", vfs::String::as_utf8( rfile->getPath()() ) );
	xmlw.openNode("STRUCTURE_FILE");

	// Read Header
	STRUCTURE_FILE_HEADER Header;
	rfile->read((vfs::Byte*)&Header, sizeof( STRUCTURE_FILE_HEADER ));

	// Write Header data
	xmlw.openNode("HEADER");
	xmlw.addValue("szId", std::string(Header.szId,0,4));
		xmlw.addValue("usNumberOfStructures", Header.usNumberOfStructures);
		xmlw.addValue("usNumberOfImages", Header.usNumberOfImages);

		xmlw.addValue("usNumberOfStructuresStored", Header.usNumberOfStructuresStored);
		xmlw.addValue("usNumberOfImageTileLocsStored", Header.usNumberOfImageTileLocsStored);
		xmlw.addValue("usStructureDataSize", Header.usStructureDataSize);
		if(Header.fFlags > 0)
		{
			xmlw.addAttributeToNextValue("uint",(void*)((UINT32)Header.fFlags));
			xmlw.openNode("flags");
			xmlw.addFlag(Header.fFlags, STRUCTURE_FILE_CONTAINS_AUXIMAGEDATA,	"STRUCTURE_FILE_CONTAINS_AUXIMAGEDATA");
			xmlw.addFlag(Header.fFlags, STRUCTURE_FILE_CONTAINS_STRUCTUREDATA,	"STRUCTURE_FILE_CONTAINS_STRUCTUREDATA");
			xmlw.closeNode();
		}
		else
		{
			xmlw.addValue("flags");
		}
	xmlw.closeNode();

	if (Header.fFlags & STRUCTURE_FILE_CONTAINS_AUXIMAGEDATA)
	{
		UINT32 uiDataSize = sizeof( AuxObjectData ) * Header.usNumberOfImages;
		sgp::AutoArray<AuxObjectData> pAuxData(uiDataSize);
		VFS_THROW_IFF( pAuxData(), L"" );

		// Read AuxImage data
		VFS_THROW_IFF( uiDataSize == rfile->read( (vfs::Byte*)pAuxData(), uiDataSize ), L"" );

		// Write AuxImage data
		xmlw.addAttributeToNextValue("number_of", Header.usNumberOfImages);
		xmlw.openNode("AUXIMAGE_DATA");
		for(int i = 0; i < Header.usNumberOfImages; ++i)
		{
			xmlw.addAttributeToNextValue("index",i);
			xmlw.openNode("auximage");
				AuxObjectData& auximage = pAuxData[i];
				xmlw.addValue("tile_loc_index",		(int)auximage.usTileLocIndex);
				xmlw.addValue("wall_orientation",	(int)auximage.ubWallOrientation);
				xmlw.addValue("number_of_tiles",	(int)auximage.ubNumberOfTiles);
				xmlw.addValue("current_frame",		(int)auximage.ubCurrentFrame);
				xmlw.addValue("number_of_frames",	(int)auximage.ubNumberOfFrames);
				if(auximage.fFlags > 0)
				{
					xmlw.addAttributeToNextValue("uint", (void*)((UINT32)auximage.fFlags));
					xmlw.openNode("flags");
					xmlw.addFlag(auximage.fFlags, AUX_FULL_TILE,		"AUX_FULL_TILE");
					xmlw.addFlag(auximage.fFlags, AUX_ANIMATED_TILE,	"AUX_ANIMATED_TILE");
					xmlw.addFlag(auximage.fFlags, AUX_DYNAMIC_TILE,		"AUX_DYNAMIC_TILE");
					xmlw.addFlag(auximage.fFlags, AUX_INTERACTIVE_TILE,	"AUX_INTERACTIVE_TILE");
					xmlw.addFlag(auximage.fFlags, AUX_IGNORES_HEIGHT,	"AUX_IGNORES_HEIGHT");
					xmlw.addFlag(auximage.fFlags, AUX_USES_LAND_Z,		"AUX_USES_LAND_Z");
					xmlw.closeNode();
				}
				else
				{
					xmlw.addValue("flags");
				}
			xmlw.closeNode();
		}
		xmlw.closeNode(); // auximage_data

		xmlw.addAttributeToNextValue("number_of", Header.usNumberOfImageTileLocsStored);
		xmlw.openNode("TILE_OFFSETS");
		if (Header.usNumberOfImageTileLocsStored > 0)
		{
			uiDataSize = sizeof( RelTileLoc ) * Header.usNumberOfImageTileLocsStored;
			sgp::AutoArray<RelTileLoc> pTileLocData(Header.usNumberOfImageTileLocsStored);
			VFS_THROW_IFF( pTileLocData(), L"" );

			VFS_THROW_IFF( uiDataSize == rfile->read((vfs::Byte*)pTileLocData(), uiDataSize), L"" );

			for(int i = 0; i < Header.usNumberOfImageTileLocsStored; ++i)
			{
				RelTileLoc& off = pTileLocData[i];
				xmlw.addAttributeToNextValue("index",i);
				xmlw.addAttributeToNextValue("x",(int)off.bTileOffsetX);
				xmlw.addAttributeToNextValue("y",(int)off.bTileOffsetY);
				xmlw.addValue("offset");
			}
		}
		xmlw.closeNode();
	}
	else
	{
		xmlw.addAttributeToNextValue("number_of", 0);
		xmlw.addValue("AUXIMAGE_DATA");
		xmlw.addAttributeToNextValue("number_of", Header.usNumberOfImageTileLocsStored);
		xmlw.addValue("TILE_OFFSETS");
	}

	if (Header.fFlags & STRUCTURE_FILE_CONTAINS_STRUCTUREDATA)
	{
		UINT32 usNumberOfStructuresStored = Header.usNumberOfStructuresStored;
		UINT32 uiDataSize = Header.usStructureDataSize;
		// Determine the size of the data, from the header just read,
		// allocate enough memory and read it in
		sgp::AutoArray<UINT8> pubStructureData( uiDataSize );
		VFS_THROW_IFF(pubStructureData(), L"");

		VFS_THROW_IFF( uiDataSize == rfile->read((vfs::Byte*)pubStructureData(), uiDataSize), L"" );

		xmlw.addAttributeToNextValue("number_of", Header.usNumberOfStructures);
		if(Header.usNumberOfStructures != Header.usNumberOfStructuresStored)
		{
			xmlw.addAttributeToNextValue("stored", Header.usNumberOfStructuresStored);
		}
		xmlw.openNode("STRUCTURE_DATA");

		UINT8* p_current	= pubStructureData();
		UINT8* p_end		= pubStructureData() + uiDataSize;
		for(unsigned int i = 0; i < usNumberOfStructuresStored; ++i)
		{
			if (p_current + sizeof( DB_STRUCTURE ) > p_end)
			{	
				// gone past end of file block?!
				// freeing of memory will occur outside of the function
				break;
			}

			DB_STRUCTURE* structure = ((DB_STRUCTURE*)p_current);
			p_current += sizeof( DB_STRUCTURE );

			//xmlw.addAttributeToNextValue("index", i);
			xmlw.addAttributeToNextValue("index", (int)structure->usStructureNumber);			
			xmlw.openNode("STRUCTURE");

				//xmlw.addValue("usStructureNumber",	(int)structure->usStructureNumber);
				xmlw.addValue("ubArmour",			(int)structure->ubArmour);
				xmlw.addValue("ubHitPoints",		(int)structure->ubHitPoints);
				xmlw.addValue("ubDensity",			(int)structure->ubDensity);
				xmlw.addValue("ubWallOrientation",	(int)structure->ubWallOrientation);
				xmlw.addValue("bDestructionPartner",(int)structure->bDestructionPartner);
				xmlw.addValue("bPartnerDelta",		(int)structure->bPartnerDelta);
				xmlw.addValue("bZTileOffsetX",		(int)structure->bZTileOffsetX);
				xmlw.addValue("bZTileOffsetY",		(int)structure->bZTileOffsetY);
				if(structure->fFlags > 0)
				{
					xmlw.addAttributeToNextValue("uint",(void*)((UINT32)structure->fFlags));
					xmlw.openNode("flags");

					xmlw.addFlag(structure->fFlags, STRUCTURE_BASE_TILE,		"STRUCTURE_BASE_TILE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_OPEN,				"STRUCTURE_OPEN");
					xmlw.addFlag(structure->fFlags, STRUCTURE_OPENABLE,			"STRUCTURE_OPENABLE");
					// synonyms for STRUCTURE_OPENABLE
					xmlw.addFlag(structure->fFlags, STRUCTURE_CLOSEABLE,		"STRUCTURE_CLOSEABLE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_SEARCHABLE,		"STRUCTURE_SEARCHABLE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_HIDDEN,			"STRUCTURE_HIDDEN");

					xmlw.addFlag(structure->fFlags, STRUCTURE_MOBILE,			"STRUCTURE_MOBILE");
					// STRUCTURE_PASSABLE is set for each structure instance where
					// the tile flag TILE_PASSABLE is set
					xmlw.addFlag(structure->fFlags, STRUCTURE_PASSABLE,			"STRUCTURE_PASSABLE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_EXPLOSIVE,		"STRUCTURE_EXPLOSIVE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_TRANSPARENT,		"STRUCTURE_TRANSPARENT");

					xmlw.addFlag(structure->fFlags, STRUCTURE_GENERIC,			"STRUCTURE_GENERIC");
					xmlw.addFlag(structure->fFlags, STRUCTURE_TREE,				"STRUCTURE_TREE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_FENCE,			"STRUCTURE_FENCE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_WIREFENCE,		"STRUCTURE_WIREFENCE");

					xmlw.addFlag(structure->fFlags, STRUCTURE_HASITEMONTOP,		"STRUCTURE_HASITEMONTOP");
					xmlw.addFlag(structure->fFlags, STRUCTURE_SPECIAL,			"STRUCTURE_SPECIAL");
					xmlw.addFlag(structure->fFlags, STRUCTURE_LIGHTSOURCE,		"STRUCTURE_LIGHTSOURCE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_VEHICLE,			"STRUCTURE_VEHICLE");

					xmlw.addFlag(structure->fFlags, STRUCTURE_WALL,				"STRUCTURE_WALL");
					xmlw.addFlag(structure->fFlags, STRUCTURE_WALLNWINDOW,		"STRUCTURE_WALLNWINDOW");
					xmlw.addFlag(structure->fFlags, STRUCTURE_SLIDINGDOOR,		"STRUCTURE_SLIDINGDOOR");
					xmlw.addFlag(structure->fFlags, STRUCTURE_DOOR,				"STRUCTURE_DOOR");

					// a "multi" structure (as opposed to multitiled) is composed of multiple graphics & structures
					xmlw.addFlag(structure->fFlags, STRUCTURE_MULTI,			"STRUCTURE_MULTI");
					xmlw.addFlag(structure->fFlags, STRUCTURE_CAVEWALL,			"STRUCTURE_CAVEWALL");
					xmlw.addFlag(structure->fFlags, STRUCTURE_DDOOR_LEFT,		"STRUCTURE_DDOOR_LEFT");
					xmlw.addFlag(structure->fFlags, STRUCTURE_DDOOR_RIGHT,		"STRUCTURE_DDOOR_RIGHT");

					xmlw.addFlag(structure->fFlags, STRUCTURE_NORMAL_ROOF,		"STRUCTURE_NORMAL_ROOF");
					xmlw.addFlag(structure->fFlags, STRUCTURE_SLANTED_ROOF,		"STRUCTURE_SLANTED_ROOF");
					xmlw.addFlag(structure->fFlags, STRUCTURE_TALL_ROOF,		"STRUCTURE_TALL_ROOF");
					xmlw.addFlag(structure->fFlags, STRUCTURE_SWITCH,			"STRUCTURE_SWITCH");

					xmlw.addFlag(structure->fFlags, STRUCTURE_ON_LEFT_WALL,		"STRUCTURE_ON_LEFT_WALL");
					xmlw.addFlag(structure->fFlags, STRUCTURE_ON_RIGHT_WALL,	"STRUCTURE_ON_RIGHT_WALL");
					xmlw.addFlag(structure->fFlags, STRUCTURE_CORPSE,			"STRUCTURE_CORPSE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_PERSON,			"STRUCTURE_PERSON");

					// COMBINATION FLAGS
					xmlw.addFlag(structure->fFlags, STRUCTURE_ANYFENCE,			"STRUCTURE_ANYFENCE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_ANYDOOR,			"STRUCTURE_ANYDOOR");
					xmlw.addFlag(structure->fFlags, STRUCTURE_OBSTACLE,			"STRUCTURE_OBSTACLE");
					xmlw.addFlag(structure->fFlags, STRUCTURE_WALLSTUFF,		"STRUCTURE_WALLSTUFF");
					xmlw.addFlag(structure->fFlags, STRUCTURE_BLOCKSMOVES,		"STRUCTURE_BLOCKSMOVES");
					xmlw.addFlag(structure->fFlags, STRUCTURE_TYPE_DEFINED,		"STRUCTURE_TYPE_DEFINED");
					xmlw.addFlag(structure->fFlags, STRUCTURE_ROOF,				"STRUCTURE_ROOF");

					xmlw.closeNode();
				}
				else
				{
					xmlw.addValue("flags");
				}
				xmlw.addValue("ubNumberOfTiles",			(int)structure->ubNumberOfTiles);
				xmlw.addAttributeToNextValue("number_of",	(int)structure->ubNumberOfTiles);
				xmlw.openNode("TILES");
				for(int i = 0; i < structure->ubNumberOfTiles; ++i)
				{
					if (p_current + sizeof( DB_STRUCTURE_TILE ) > p_end)
					{
						// gone past end of file block?!
						// freeing of memory will occur outside of the function
						break;
					}
					DB_STRUCTURE_TILE* structTile = ((DB_STRUCTURE_TILE*)p_current);
					p_current += sizeof( DB_STRUCTURE_TILE );

					xmlw.addAttributeToNextValue("index", i);
					xmlw.openNode("STRUCTURE_TILE");
						xmlw.addValue("bXPosRelToBase",			(int)structTile->bXPosRelToBase);
						xmlw.addValue("bYPosRelToBase",			(int)structTile->bYPosRelToBase);
						xmlw.addValue("sPosRelToBase",			(int)structTile->sPosRelToBase);
						xmlw.addValue("ubVehicleHitLocation",	(int)structTile->ubVehicleHitLocation);
						if(structTile->fFlags > 0)
						{
							xmlw.addAttributeToNextValue("uint", (void*)((UINT32)structTile->fFlags));
							xmlw.openNode("flags");
							xmlw.addFlag(structTile->fFlags, TILE_ON_ROOF,	"TILE_ON_ROOF");
							xmlw.addFlag(structTile->fFlags, TILE_PASSABLE,	"TILE_PASSABLE");
							xmlw.closeNode();
						}
						else
						{
							xmlw.addValue("flags");
						}
						xmlw.openNode("profile");
						for(int y=PROFILE_Y_SIZE-1; y >= 0; y--)
						{
							std::stringstream ss;
							for(int x = 0; x < PROFILE_X_SIZE - 1; ++x)
							{
								ss << (int)structTile->Shape[y][x] << ",";
							}
							ss << (int)structTile->Shape[y][PROFILE_X_SIZE-1];
							xmlw.addValue("y"+vfs::toString<char>(y), ss.str());
						}
						xmlw.closeNode();
					xmlw.closeNode();
				}
				xmlw.closeNode();

			xmlw.closeNode();
		}
		xmlw.closeNode();
	}
	else
	{
		xmlw.addAttributeToNextValue("number_of", Header.usNumberOfStructuresStored);
		xmlw.addValue("STRUCTURE_DATA");
	}
	xmlw.closeNode(); // root
	xmlw.writeToFile(pOutputFile);

	return true;
}
