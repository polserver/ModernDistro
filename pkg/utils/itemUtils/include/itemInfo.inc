/* $Id: itemInfo.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
use uo;
use os;
use cfgfile;
use polsys;


/*
 * IU_GetTileInfo(graphic)
 *
 * Purpose
 * Retrieves information on a tile from ::tiles.cfg
 *
 * Parameters
 * graphic:	Graphic number of the tile to retrieve information for.
 *
 * Return value
 * Returns a config elem on success.
 * Returns an error on failure.
 *
 */
function IU_GetTileInfo(graphic)
	var config := ReadConfigFile("::tiles");
	return config[Hex(CInt(graphic))];
endfunction

function IsStackableItem( byref item )

	var item_info := IU_GetTileInfo( item.graphic );

	return CInt( item_info.Stackable );
endfunction

function CheckImpassable( byref objtype )

	var item_info := IU_GetTileInfo( objtype );

	return CInt( item_info.Blocking );
endfunction

function GetLayer( byref item )

	var item_info := IU_GetTileInfo( item.objtype );

	return CInt( item_info.Layer );
endfunction

function GetWeight( byref item )

	var item_info := IU_GetTileInfo( item.objtype );

	return GetConfigString( item_info, "Weight" );
endfunction

function CheckStairs( byref objtype )

	if( objtype in { 0x35D2, 0x35D3, 0x35D4, 0x35D5, 0x35D6, 0x3609, 0x360A, 0x360B, 0x360C, 0x360D } )
		return 1;
	endif

	var item_info := IU_GetTileInfo( objtype ),
		desc := GetConfigString( item_info, "Desc" );

	return ( desc["stairs"] || desc["stair"] || desc["steps"] || desc["ramp"] || desc["ladder"] );
endfunction

function GetGumpArt( byref objtype, byref gender:=0 )

	var item_info := IU_GetTileInfo( objtype ),
		anim_id := GetConfigInt( item_info, "AnimID" );

	if( anim_id.errortext )
		return error{ "errortext":=Hex( objtype )+" does not have an Animation ID and may not be equipable." };
	endif

	return ( ( gender ? 60000 : 50000 ) + anim_id );
endfunction
