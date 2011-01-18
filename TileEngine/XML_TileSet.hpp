#ifndef _XML_TILESET_HPP_
#define _XML_TILESET_HPP_

#include "XML_auto_parse.h"
#include "WorldDat.h"
#include <vfs/Tools/vfs_tools.h>

class CTilesetStates
{
public:
	enum States
	{
		STATE_NONE,
		STATE_JA2SET,
		STATE_tilesets,
		STATE_ts_tileset,
		STATE_ts_tileset_name,
		STATE_ts_tileset_ambientid,
		STATE_ts_tileset_files,
		STATE_ts_tileset_files_file,
	};
};

PARSER_CLASS_MACRO(CTilesetReader, CTilesetStates)
{
	typedef CTilesetReader _myt;
public:
	/**
	 *  NOOP
	 */
	PURE_TRANSITION(NOOP, _myt);


	/**
	 *	Tilesets
	 */
	DEFINE_TRANSITION_WDATA(TilesetsAttrib, _myt, false, struct TilesetInfo{int numTilesets; int numFiles;});
	TRANSITION_ENTER{
		long T_num, F_num;
		if(atts.getLong("numTilesets", T_num)) T_DATA.numTilesets = (int)T_num;
		else SGP_THROW(L"Attribute 'numTileset' not found");

		if(atts.getLong("numFiles",F_num)) T_DATA.numFiles = (int)F_num ;
		else SGP_THROW(L"Attribute 'numFiles' not found");
	}
	FINISH_TRANSITION;

	/**
	 *	Tileset
	 */
	TILESET* _current_tileset;
	DEFINE_TRANSITION(Tileset, _myt, false);
	TRANSITION_ENTER{
		long lindex;
		if(atts.getLong("index", lindex)){
			SGP_THROW_IFFALSE(lindex < MAX_TILESETS, _BS(L"tileset index is too large : ") << lindex << _BS::wget );
			P_DATA->_current_tileset = &(P_DATA->_tilesets[ lindex ]);
		}
		else SGP_THROW(L"Attribute 'index' not found");
	}
	TRANSITION_LEAVE{
		P_DATA->_current_tileset = NULL;
	}
	FINISH_TRANSITION;

	/**
	 *  Tileset Name
	 */
	DEFINE_TRANSITION(TilesetName, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->_current_tileset != NULL, L"No tileset selected" );
		std::string tmpString = vfs::trimString(char_data, 0, char_data.length());
		wcsncpy( P_DATA->_current_tileset->zName, vfs::String( tmpString ).c_str(), 32 );
	}
	FINISH_TRANSITION;

	/**
	 *  Tileset ID
	 */
	DEFINE_TRANSITION(TilesetAmbientID, _myt, true);
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->_current_tileset != NULL, L"No tileset selected" );
		int tmp;
		SGP_THROW_IFFALSE( vfs::convertTo<int>(char_data, tmp),
			_BS(L"Could not convert to int : ") << char_data << _BS::wget );
		P_DATA->_current_tileset->ubAmbientID = tmp;
			
	}
	FINISH_TRANSITION;

	/**
	 *  Tileset File
	 */
	DEFINE_TRANSITION_WDATA(TilesetFile, _myt, true, struct Index{int index;});
	TRANSITION_ENTER{
		long lindex;
		if(atts.getLong("index", lindex)){
			SGP_THROW_IFFALSE(lindex < NUMBEROFTILETYPES,
				_BS(L"tileset file index is too large : ") << lindex << _BS::wget );
			T_DATA.index = lindex;
		}
		else SGP_THROW(L"Attribute 'index' not found");
	}
	TRANSITION_LEAVE{
		SGP_THROW_IFFALSE( P_DATA->_current_tileset != NULL, L"No tileset selected" );
		std::string tmpString = vfs::trimString(char_data, 0, char_data.length());
		strncpy( P_DATA->_current_tileset->TileSurfaceFilenames[T_DATA.index], tmpString.c_str(), 32);
	}
	FINISH_TRANSITION;


	TILESET* _tilesets;
	CTilesetReader(TILESET tilesets[ MAX_TILESETS ]) : _tilesets(tilesets), _current_tileset(NULL)
	{
		TR_PASS  ( STATE_NONE,				"JA2SET",		STATE_JA2SET);
		TR_HANDLE( STATE_JA2SET,			"tilesets",		STATE_tilesets,					TilesetsAttrib );
		TR_HANDLE( STATE_tilesets,			"Tileset",		STATE_ts_tileset,				Tileset );
		TR_HANDLE( STATE_ts_tileset,		"Name",			STATE_ts_tileset_name,			TilesetName );
		TR_HANDLE( STATE_ts_tileset,		"AmbientID",	STATE_ts_tileset_ambientid,		TilesetAmbientID );
		TR_PASS  ( STATE_ts_tileset,		"Files",		STATE_ts_tileset_files );
		TR_HANDLE( STATE_ts_tileset_files,	"file",			STATE_ts_tileset_files_file,	TilesetFile );
	}
};

#endif // _XML_TILESET_HPP_
