#ifndef _XML_STRUCTUREDATA_HPP_
#define _XML_STRUCTUREDATA_HPP_

#include "XML_auto_parse.h"

#include <vfs/Tools/vfs_parser_tools.h>
#include <vfs/Tools/vfs_tools.h>

class CStructureDataStates
{
public:
	enum States
	{
		STATE_NONE,
		STATE_STRUCTURE_FILE,
		// ***********
		STATE_HEADER,
		STATE_HEADER_id,
		STATE_HEADER_num_structures,
		STATE_HEADER_num_images, 
		STATE_HEADER_struct_stored,
		STATE_HEADER_tile_locs_stores,
		STATE_HEADER_data_size,
		STATE_HEADER_flags,
		STATE_HEADER_flags_any,
		// ***********
		STATE_AUXIMAGE_DATA,
		STATE_AUX_image,
		STATE_AUX_image_tile_index,
		STATE_AUX_image_wall_orient,
		STATE_AUX_image_num_tiles,
		STATE_AUX_image_num_frames,
		STATE_AUX_image_cur_frame,
		STATE_AUX_image_FLAGS,
		STATE_AUX_image_FLAGS_any,
		// ***********
		STATE_TILE_OFFSETS,
		STATE_TOFF_offset,
		// ***********
		STATE_STRUCTURE_DATA,
		// --
		STATE_SD_structure,
		STATE_SD_structure_number,
		STATE_SD_structure_armour,
		STATE_SD_structure_hitpoits,
		STATE_SD_structure_density,
		STATE_SD_structure_wallrientation,
		STATE_SD_structure_destr_partner,
		STATE_SD_structure_desct_delta,
		STATE_SD_structure_ZoffsetX,
		STATE_SD_structure_ZoffsetY,
		STATE_SD_structure_flags,
		STATE_SD_structure_flags_any,
		STATE_SD_structure_numtiles,
		// --
		STATE_SD_TILES,
		STATE_SD_TILES_tile,
		STATE_SD_TILES_tile_OffsetX,
		STATE_SD_TILES_tile_OffsetY,
		STATE_SD_TILES_tile_OffsetBase,
		STATE_SD_TILES_tile_vehitLoc,
		STATE_SD_TILES_tile_flags,
		STATE_SD_TILES_tile_flags_any,
		STATE_SD_TILES_tile_profile,
		STATE_SD_TILES_tile_profile_any,
	};
};

PARSER_CLASS_MACRO(CStructureDataReader, CStructureDataStates)
{
	typedef CStructureDataReader _myt;

	struct StructureData
	{
		DB_STRUCTURE structure;
		std::vector<DB_STRUCTURE_TILE> tiles;
	};
	std::vector<StructureData> _structures;
public:
	PURE_TRANSITION(NOOP, _myt);

	/**********************************************************************/
	/*                               HEADER                               */
	/**********************************************************************/

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header,_myt,false);
	TRANSITION_ENTER{
		memset(&(P_DATA->oHeader), 0, sizeof(STRUCTURE_FILE_HEADER));
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header_NumStructures,_myt,true);
	TRANSITION_LEAVE{
		P_DATA->oHeader.usNumberOfStructures = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header_NumImages,_myt,true);
	TRANSITION_LEAVE{
		P_DATA->oHeader.usNumberOfImages = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header_StructStored,_myt,true);
	TRANSITION_LEAVE{
		P_DATA->oHeader.usNumberOfStructuresStored = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header_TileLocsStored,_myt,true);
	TRANSITION_LEAVE{
		P_DATA->oHeader.usNumberOfImageTileLocsStored = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Header_DataSize,_myt,true);
	TRANSITION_LEAVE{
		P_DATA->oHeader.usStructureDataSize = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	DEFINE_TRANSITION(TR_Header_Flags,_myt,true);
	TRANSITION_LEAVE{
		if(std::string(tag_name) == "STRUCTURE_FILE_CONTAINS_STRUCTUREDATA")	P_DATA->oHeader.fFlags |= STRUCTURE_FILE_CONTAINS_STRUCTUREDATA;
		if(std::string(tag_name) == "STRUCTURE_FILE_CONTAINS_AUXIMAGEDATA")		P_DATA->oHeader.fFlags |= STRUCTURE_FILE_CONTAINS_AUXIMAGEDATA;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	/*                          AUXIMAGE_DATA                             */
	/**********************************************************************/
	int count_aux_tiles;
	DEFINE_TRANSITION(TR_AUXIMAGE_DATA, _myt, false);
	TRANSITION_ENTER{
		long num = 0;
		SGP_THROW_IFFALSE(atts.getLong("number_of",num), L"no attribute 'number_of'");
		if(num > 0)
		{
			SGP_THROW_IFFALSE(num == P_DATA->oHeader.usNumberOfImages , L"inconsistent data");
			P_DATA->pStructureFileRef->usNumberOfStructures = (UINT16)num;
			int size = sizeof(AuxObjectData) * num;
			P_DATA->pStructureFileRef->pAuxData = (AuxObjectData*)MemAlloc(size);
			memset(P_DATA->pStructureFileRef->pAuxData, 0, size);
			P_DATA->count_aux_tiles = 0;
		}
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	AuxObjectData* current_aux;
	DEFINE_TRANSITION(TR_AUX_image, _myt, false);
	TRANSITION_ENTER{
		long index = -1;
		SGP_THROW_IFFALSE(atts.getLong("index", index), L"no attribute 'index'");
		SGP_THROW_IFFALSE(index >= 0 && index < P_DATA->oHeader.usNumberOfImages, L"index out of range");
		P_DATA->current_aux = &P_DATA->pStructureFileRef->pAuxData[index];
	}
	TRANSITION_LEAVE{
		P_DATA->current_aux = NULL;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_tile_index, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		P_DATA->current_aux->usTileLocIndex = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_wall_orient, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		P_DATA->current_aux->ubWallOrientation = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_num_tiles, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		int num_tiles = atoi(char_data.c_str());
		P_DATA->current_aux->ubNumberOfTiles = num_tiles;
		P_DATA->count_aux_tiles += num_tiles;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_num_frames, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		P_DATA->current_aux->ubNumberOfFrames = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_cur_frame, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		P_DATA->current_aux->ubCurrentFrame = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_image_FLAGS, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_aux, L"aux is null");
		if(std::string(tag_name) == "AUX_FULL_TILE")		P_DATA->current_aux->fFlags |= AUX_FULL_TILE;
		if(std::string(tag_name) == "AUX_ANIMATED_TILE")	P_DATA->current_aux->fFlags |= AUX_ANIMATED_TILE;
		if(std::string(tag_name) == "AUX_DYNAMIC_TILE")		P_DATA->current_aux->fFlags |= AUX_DYNAMIC_TILE;
		if(std::string(tag_name) == "AUX_INTERACTIVE_TILE")	P_DATA->current_aux->fFlags |= AUX_INTERACTIVE_TILE;
		if(std::string(tag_name) == "AUX_IGNORES_HEIGHT")	P_DATA->current_aux->fFlags |= AUX_IGNORES_HEIGHT;
		if(std::string(tag_name) == "AUX_USES_LAND_Z")		P_DATA->current_aux->fFlags |= AUX_USES_LAND_Z;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	/*                           AUXTILE_DATA                             */
	/**********************************************************************/
	DEFINE_TRANSITION(TR_AUX_TILES, _myt, false);
	TRANSITION_ENTER{
		long num = 0;
		SGP_THROW_IFFALSE(atts.getLong("number_of",num), L"no attribute 'number_of'");
		if(num > 0)
		{
			SGP_THROW_IFFALSE(num == P_DATA->count_aux_tiles && num == P_DATA->oHeader.usNumberOfImageTileLocsStored,
				L"counted number and given number of tiles aux images are not equal");

			int size = num * sizeof(RelTileLoc);
			P_DATA->pStructureFileRef->pTileLocData = (RelTileLoc*)MemAlloc(size);
			memset(P_DATA->pStructureFileRef->pTileLocData, 0, size);
		}
	}
	FINISH_TRANSITION;

	DEFINE_TRANSITION(TR_AUX_Tile_offset, _myt, false);
	TRANSITION_ENTER{
		long index = 0, x = 0 , y = 0;
		SGP_THROW_IFFALSE(atts.getLong("index", index), L"nu attribute 'index'");
		SGP_THROW_IFFALSE(index >= 0 && index < P_DATA->oHeader.usNumberOfImageTileLocsStored, L"index out of range");
		SGP_THROW_IFFALSE(atts.getLong("x", x), L"no attribute 'x'");
		SGP_THROW_IFFALSE(atts.getLong("y", y), L"no attribute 'y'");
		P_DATA->pStructureFileRef->pTileLocData[index].bTileOffsetX = (INT8)x;
		P_DATA->pStructureFileRef->pTileLocData[index].bTileOffsetY = (INT8)y;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	/*                          STRUCTURE_DATA                            */
	/**********************************************************************/
	StructureData* current_structure;
	int structure_index, structure_count;
	DEFINE_TRANSITION(TR_Structure_data,_myt, false);
	TRANSITION_ENTER{
		long num = 0, stored = 0;
		SGP_THROW_IFFALSE(atts.getLong("number_of",num), L"structure data : no attribute 'number_of'");
		if(!atts.getLong("stored",stored))
		{
			stored = num;
		}
		if(num > 0)
		{
			// header consistency check (to be removed later ?)
			SGP_THROW_IFFALSE(num == P_DATA->oHeader.usNumberOfStructures, L"inconsistent data : number of structures");
			SGP_THROW_IFFALSE(stored == P_DATA->oHeader.usNumberOfStructuresStored, L"inconsistent data : number of stored structures");
			P_DATA->_structures.resize(stored);
			for(int i=0; i < stored; ++i)
			{
				memset(&P_DATA->_structures[i].structure, 0, sizeof(DB_STRUCTURE));
			}
			// save data
			P_DATA->pStructureFileRef->usNumberOfStructures = (UINT16)num;
			P_DATA->pStructureFileRef->usNumberOfStructuresStored = (UINT16)stored;
		}
		P_DATA->structure_count = 0;
		P_DATA->structure_index = -1;
	}
	TRANSITION_LEAVE{
		// copy gathered data into game object
		if(!P_DATA->_structures.empty())
		{
			SGP_THROW_IFFALSE(P_DATA->pStructureFileRef->usNumberOfStructuresStored == P_DATA->structure_count, L"wrong number of structures");

			int size = 0;
			for(::size_t i = 0; i < P_DATA->_structures.size(); ++i)
			{
				size += sizeof(DB_STRUCTURE);
				for(::size_t j = 0; j < P_DATA->_structures[i].tiles.size(); ++j)
				{
					size += sizeof(DB_STRUCTURE_TILE);
				}
			}

			SGP_THROW_IFFALSE(size == P_DATA->oHeader.usStructureDataSize,
				_BS(L"data size is supposed to be '") << P_DATA->oHeader.usStructureDataSize << L"' but is '" << size << L"'" << _BS::wget);

			P_DATA->structure_data_size = size;

			P_DATA->pStructureFileRef->pubStructureData = (UINT8*)MemAlloc(size);
			UINT8 *tmp = P_DATA->pStructureFileRef->pubStructureData;
			for(::size_t i = 0; i < P_DATA->_structures.size(); ++i)
			{
				memcpy(tmp, &P_DATA->_structures[i].structure, sizeof(DB_STRUCTURE));
				tmp += sizeof(DB_STRUCTURE);
				for(::size_t j = 0; j < P_DATA->_structures[i].tiles.size(); ++j)
				{
					memcpy(tmp, &P_DATA->_structures[i].tiles[j], sizeof(DB_STRUCTURE_TILE));
					tmp += sizeof(DB_STRUCTURE_TILE);
				}
			}
		}
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure, _myt, false);
	TRANSITION_ENTER{
		long index;
		SGP_THROW_IFFALSE( atts.getLong("index", index), L"no attribute 'index'" );
		SGP_THROW_IFFALSE( (index >= 0) && (index < P_DATA->pStructureFileRef->usNumberOfStructures), L"structure index out of range");
		SGP_THROW_IFFALSE( (P_DATA->structure_count >= 0) && (P_DATA->structure_count < P_DATA->pStructureFileRef->usNumberOfStructuresStored), L"structure count larger than announced");
		P_DATA->current_structure = &P_DATA->_structures[P_DATA->structure_count];
		P_DATA->current_structure->structure.usStructureNumber = (UINT16)index;
		P_DATA->structure_index = index;
	}
	TRANSITION_LEAVE{
		// finished this structure, increase index for next run
		P_DATA->structure_index = -1;
		P_DATA->structure_count += 1;
		P_DATA->current_structure = NULL;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	//DEFINE_TRANSITION(TR_Structure_number, _myt, true);
	//TRANSITION_LEAVE{
	//	SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
	//	P_DATA->current_structure->structure.usStructureNumber = atoi(char_data.c_str());
	//	SGP_THROW_IFFALSE( P_DATA->current_structure->structure.usStructureNumber == P_DATA->structure_index, L"inconsistent data");
	//}
	//FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_armour, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.ubArmour = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_hitpoints, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.ubHitPoints = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_density, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.ubDensity = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_wall_orient, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.ubWallOrientation = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;
	
	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_destr_partner, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.bDestructionPartner = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_partner_delta, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.bPartnerDelta = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_offX, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.bZTileOffsetX = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_offY, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		P_DATA->current_structure->structure.bZTileOffsetY = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_num_tiles, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		int num_tiles = atoi(char_data.c_str());
		SGP_THROW_IFFALSE(num_tiles >= 0, L"invalid number");
		P_DATA->current_structure->structure.ubNumberOfTiles = num_tiles;
		P_DATA->current_structure->tiles.resize(num_tiles);
		for(int i = 0 ;i < num_tiles; ++i)
		{
			memset(&P_DATA->current_structure->tiles[i], 0, sizeof(DB_STRUCTURE_TILE));
		}
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_Structure_FLAGS, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->current_structure, L"null structure" );
		if(std::string(tag_name) == "STRUCTURE_BASE_TILE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_BASE_TILE;
		if(std::string(tag_name) == "STRUCTURE_OPEN")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_OPEN;
		if(std::string(tag_name) == "STRUCTURE_OPENABLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_OPENABLE;
		if(std::string(tag_name) == "STRUCTURE_CLOSEABLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_CLOSEABLE;
		if(std::string(tag_name) == "STRUCTURE_SEARCHABLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_SEARCHABLE;
		if(std::string(tag_name) == "STRUCTURE_HIDDEN")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_HIDDEN;
		if(std::string(tag_name) == "STRUCTURE_MOBILE")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_MOBILE;
		if(std::string(tag_name) == "STRUCTURE_PASSABLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_PASSABLE;
		if(std::string(tag_name) == "STRUCTURE_EXPLOSIVE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_EXPLOSIVE;
		if(std::string(tag_name) == "STRUCTURE_TRANSPARENT")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_TRANSPARENT;
		if(std::string(tag_name) == "STRUCTURE_GENERIC")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_GENERIC;
		if(std::string(tag_name) == "STRUCTURE_TREE")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_TREE;
		if(std::string(tag_name) == "STRUCTURE_FENCE")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_FENCE;
		if(std::string(tag_name) == "STRUCTURE_WIREFENCE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_WIREFENCE;
		if(std::string(tag_name) == "STRUCTURE_HASITEMONTOP")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_HASITEMONTOP;
		if(std::string(tag_name) == "STRUCTURE_SPECIAL")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_SPECIAL;
		if(std::string(tag_name) == "STRUCTURE_LIGHTSOURCE")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_LIGHTSOURCE;
		if(std::string(tag_name) == "STRUCTURE_VEHICLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_VEHICLE;
		if(std::string(tag_name) == "STRUCTURE_WALL")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_WALL;
		if(std::string(tag_name) == "STRUCTURE_WALLNWINDOW")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_WALLNWINDOW;
		if(std::string(tag_name) == "STRUCTURE_SLIDINGDOOR")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_SLIDINGDOOR;
		if(std::string(tag_name) == "STRUCTURE_DOOR")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_DOOR;
		if(std::string(tag_name) == "STRUCTURE_MULTI")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_MULTI;
		if(std::string(tag_name) == "STRUCTURE_CAVEWALL")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_CAVEWALL;
		if(std::string(tag_name) == "STRUCTURE_DDOOR_LEFT")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_DDOOR_LEFT;
		if(std::string(tag_name) == "STRUCTURE_DDOOR_RIGHT")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_DDOOR_RIGHT;
		if(std::string(tag_name) == "STRUCTURE_NORMAL_ROOF")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_NORMAL_ROOF;
		if(std::string(tag_name) == "STRUCTURE_SLANTED_ROOF")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_SLANTED_ROOF;
		if(std::string(tag_name) == "STRUCTURE_TALL_ROOF")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_TALL_ROOF;
		if(std::string(tag_name) == "STRUCTURE_SWITCH")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_SWITCH;
		if(std::string(tag_name) == "STRUCTURE_ON_LEFT_WALL")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_ON_LEFT_WALL;
		if(std::string(tag_name) == "STRUCTURE_ON_RIGHT_WALL")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_ON_RIGHT_WALL;
		if(std::string(tag_name) == "STRUCTURE_CORPSE")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_CORPSE;
		if(std::string(tag_name) == "STRUCTURE_PERSON")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_PERSON;
		if(std::string(tag_name) == "STRUCTURE_ANYFENCE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_ANYFENCE;
		if(std::string(tag_name) == "STRUCTURE_ANYDOOR")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_ANYDOOR;
		if(std::string(tag_name) == "STRUCTURE_OBSTACLE")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_OBSTACLE;
		if(std::string(tag_name) == "STRUCTURE_WALLSTUFF")		P_DATA->current_structure->structure.fFlags |= STRUCTURE_WALLSTUFF;
		if(std::string(tag_name) == "STRUCTURE_BLOCKSMOVES")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_BLOCKSMOVES;
		if(std::string(tag_name) == "STRUCTURE_TYPE_DEFINED")	P_DATA->current_structure->structure.fFlags |= STRUCTURE_TYPE_DEFINED;
		if(std::string(tag_name) == "STRUCTURE_ROOF")			P_DATA->current_structure->structure.fFlags |= STRUCTURE_ROOF;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	/*                          STRUCTURE_TILES                           */
	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_TILES, _myt, false);
	TRANSITION_ENTER{
		long num = 0;
		if(atts.getLong("number_of",num))
		{
			SGP_THROW_IFFALSE(num == P_DATA->current_structure->structure.ubNumberOfTiles, L"inconsistent data");
		}
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DB_STRUCTURE_TILE* current_str_tile;
	DEFINE_TRANSITION(TR_SD_Tile, _myt, false);
	TRANSITION_ENTER{
		long index = 0;
		SGP_THROW_IFFALSE( atts.getLong("index",index), L"no attribute 'index'" );
		SGP_THROW_IFFALSE( index >= 0 && index < (long)P_DATA->current_structure->tiles.size(), L"structure index out of range");
		P_DATA->current_str_tile = &P_DATA->current_structure->tiles[index];
	}
	TRANSITION_LEAVE{
		P_DATA->current_str_tile = NULL;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_offX, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		P_DATA->current_str_tile->bXPosRelToBase = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_offY, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		P_DATA->current_str_tile->bYPosRelToBase = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_offBase, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		P_DATA->current_str_tile->sPosRelToBase = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_VehHitLoc, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		P_DATA->current_str_tile->ubVehicleHitLocation = atoi(char_data.c_str());
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_FLAGS, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		if(std::string(tag_name) == "TILE_ON_ROOF")		P_DATA->current_str_tile->fFlags |= TILE_ON_ROOF;
		if(std::string(tag_name) == "TILE_PASSABLE")	P_DATA->current_str_tile->fFlags |= TILE_PASSABLE;
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	DEFINE_TRANSITION(TR_SD_Tile_profile, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE(P_DATA->current_str_tile, L"null structure tile");
		int y = -1;
		if     (std::string(tag_name) == "y0") y = 0;
		else if(std::string(tag_name) == "y1") y = 1;
		else if(std::string(tag_name) == "y2") y = 2;
		else if(std::string(tag_name) == "y3") y = 3;
		else if(std::string(tag_name) == "y4") y = 4;

		if(y != -1)
		{
			int x = 0;
			vfs::String token;
			vfs::CTokenizer tok(char_data);
			while(tok.next(token, L','))
			{
				if(x >= 0 && x < 5)
				{
					int z;
					SGP_THROW_IFFALSE( vfs::convertTo<int>(token,z), L"" );
					if(z >= 0 && z < 16)
					{
						P_DATA->current_str_tile->Shape[y][x] = z;
					}
				}
				x++;
			}			
		}
	}
	FINISH_TRANSITION;

	/**********************************************************************/
	UINT32 structure_data_size;

	STRUCTURE_FILE_REF*		pStructureFileRef;
	STRUCTURE_FILE_HEADER	oHeader;
	CStructureDataReader(STRUCTURE_FILE_REF* sfr) 
		: pStructureFileRef(sfr), current_structure(NULL), current_str_tile(NULL), current_aux(NULL)
	{
		SGP_THROW_IFFALSE(sfr, L"");
		
		TR_PASS  (STATE_NONE,				"STRUCTURE_FILE",					STATE_STRUCTURE_FILE);
		// HEADER
		TR_HANDLE( STATE_STRUCTURE_FILE,	"HEADER",							STATE_HEADER,					TR_Header );
		TR_HANDLE( STATE_HEADER,			"usNumberOfStructures",				STATE_HEADER_num_structures,	TR_Header_NumStructures );
		TR_HANDLE( STATE_HEADER,			"usNumberOfImages",					STATE_HEADER_num_images,		TR_Header_NumImages );
		TR_HANDLE( STATE_HEADER,			"usNumberOfStructuresStored",		STATE_HEADER_struct_stored,		TR_Header_StructStored );
		TR_HANDLE( STATE_HEADER,			"usNumberOfImageTileLocsStored",	STATE_HEADER_tile_locs_stores,	TR_Header_TileLocsStored );
		TR_HANDLE( STATE_HEADER,			"usStructureDataSize",				STATE_HEADER_data_size,			TR_Header_DataSize );
		TR_PASS  ( STATE_HEADER,			"flags",							STATE_HEADER_flags );
		TR_HANDLE( STATE_HEADER_flags,		"",									STATE_HEADER_flags_any,			TR_Header_Flags );
		// AUX IMAGE
		TR_HANDLE( STATE_STRUCTURE_FILE,	"AUXIMAGE_DATA",		STATE_AUXIMAGE_DATA,				TR_AUXIMAGE_DATA);
		TR_HANDLE( STATE_AUXIMAGE_DATA,		"auximage",				STATE_AUX_image,					TR_AUX_image);
		TR_HANDLE( STATE_AUX_image,			"tile_loc_index",		STATE_AUX_image_tile_index,			TR_AUX_image_tile_index);
		TR_HANDLE( STATE_AUX_image,			"wall_orientation",		STATE_AUX_image_wall_orient,		TR_AUX_image_wall_orient);
		TR_HANDLE( STATE_AUX_image,			"number_of_tiles",		STATE_AUX_image_num_tiles,			TR_AUX_image_num_tiles);
		TR_HANDLE( STATE_AUX_image,			"number_of_frames",		STATE_AUX_image_num_frames,			TR_AUX_image_num_frames);
		TR_HANDLE( STATE_AUX_image,			"current_frame",		STATE_AUX_image_cur_frame,			TR_AUX_image_cur_frame);
		TR_PASS  ( STATE_AUX_image,			"flags",				STATE_AUX_image_FLAGS);
		TR_HANDLE( STATE_AUX_image_FLAGS,	"",						STATE_AUX_image_FLAGS_any,			TR_AUX_image_FLAGS);
		// AUX TILES
		TR_HANDLE( STATE_STRUCTURE_FILE,	"TILE_OFFSETS",			STATE_TILE_OFFSETS,					TR_AUX_TILES);
		TR_HANDLE( STATE_TILE_OFFSETS,		"offset",				STATE_TOFF_offset,					TR_AUX_Tile_offset);
		// STRUCTURE
		TR_HANDLE( STATE_STRUCTURE_FILE,	"STRUCTURE_DATA",		STATE_STRUCTURE_DATA,				TR_Structure_data );
		TR_HANDLE( STATE_STRUCTURE_DATA,	"STRUCTURE",			STATE_SD_structure,					TR_Structure );
//		TR_HANDLE( STATE_SD_structure,		"usStructureNumber",	STATE_SD_structure_number,			TR_Structure_number );
		TR_HANDLE( STATE_SD_structure,		"ubArmour",				STATE_SD_structure_armour,			TR_Structure_armour );
		TR_HANDLE( STATE_SD_structure,		"ubHitPoints",			STATE_SD_structure_hitpoits,		TR_Structure_hitpoints );
		TR_HANDLE( STATE_SD_structure,		"ubDensity",			STATE_SD_structure_density,			TR_Structure_density );
		TR_HANDLE( STATE_SD_structure,		"ubWallOrientation",	STATE_SD_structure_wallrientation,	TR_Structure_wall_orient );
		TR_HANDLE( STATE_SD_structure,		"bDestructionPartner",	STATE_SD_structure_destr_partner,	TR_Structure_destr_partner );
		TR_HANDLE( STATE_SD_structure,		"bPartnerDelta",		STATE_SD_structure_desct_delta,		TR_Structure_partner_delta );
		TR_HANDLE( STATE_SD_structure,		"bZTileOffsetX",		STATE_SD_structure_ZoffsetX,		TR_Structure_offX );
		TR_HANDLE( STATE_SD_structure,		"bZTileOffsetY",		STATE_SD_structure_ZoffsetY,		TR_Structure_offY );
		TR_PASS  ( STATE_SD_structure,		"flags",				STATE_SD_structure_flags );
		TR_HANDLE( STATE_SD_structure_flags,		"",				STATE_SD_structure_flags_any,		TR_Structure_FLAGS );
		TR_HANDLE( STATE_SD_structure,		"ubNumberOfTiles",		STATE_SD_structure_numtiles,		TR_Structure_num_tiles );
		// STRUCTURE TILES
		TR_HANDLE( STATE_SD_structure,		"TILES",				STATE_SD_TILES,						TR_SD_TILES);
		TR_HANDLE( STATE_SD_TILES,			"STRUCTURE_TILE",		STATE_SD_TILES_tile,				TR_SD_Tile);
		TR_HANDLE( STATE_SD_TILES_tile,		"bXPosRelToBase",		STATE_SD_TILES_tile_OffsetX,		TR_SD_Tile_offX);
		TR_HANDLE( STATE_SD_TILES_tile,		"bYPosRelToBase",		STATE_SD_TILES_tile_OffsetY,		TR_SD_Tile_offY);
		TR_HANDLE( STATE_SD_TILES_tile,		"sPosRelToBase",		STATE_SD_TILES_tile_OffsetBase,		TR_SD_Tile_offBase);
		TR_HANDLE( STATE_SD_TILES_tile,		"ubVehicleHitLocation",	STATE_SD_TILES_tile_vehitLoc,		TR_SD_Tile_VehHitLoc);
		TR_PASS  ( STATE_SD_TILES_tile,		"flags",				STATE_SD_TILES_tile_flags );
		TR_HANDLE( STATE_SD_TILES_tile_flags,		"",				STATE_SD_TILES_tile_flags_any,		TR_SD_Tile_FLAGS );
		TR_PASS  ( STATE_SD_TILES_tile,		"profile",				STATE_SD_TILES_tile_profile );
		TR_HANDLE( STATE_SD_TILES_tile_profile,		"",				STATE_SD_TILES_tile_profile_any,	TR_SD_Tile_profile );
	}
};

#endif // _XML_STRUCTUREDATA_HPP_
