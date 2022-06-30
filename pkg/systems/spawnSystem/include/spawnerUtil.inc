/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;
use util;

include ":datafile:datafile";
include ":spawnsystem:regions";
include "include/housing";

/*
 * SS_SelectCoordsInArea(x1, y1, x2, y2)
 *
 * Purpose
 * Takes in four numbers (two coordinates) and
 * picks a random coordinate within the area they create.
 *
 * Parameters
 * x1:	The top-left spot of the X axis.
 * y1:	The top-left spot of the Y axis.
 * x2:	The bottom-right spot of the X axis.
 * y2:	The bottom-right spot of the Y axis.
 *
 * Return value
 * A structure containing an X and Y property.
 *
 */
function SS_SelectCoordsInArea( x1, y1, x2, y2 )

	var coord := struct;
	coord.+x := RandomInt( x2-x1 ) + x1;
	coord.+y := RandomInt( y2-y1 ) + y1;

	return coord;
endfunction

function SS_GetSpawnCoords( coords, realm_name )

	var spawn_coords := SS_SelectCoordsInArea( coords.x1, coords.y1, coords.x2, coords.y2 );
	var z_pos := CInt( GetWorldHeight( spawn_coords.x, spawn_coords.y, realm_name ));

	spawn_coords.+z := z_pos;
	spawn_coords.+realm := realm_name;

	return spawn_coords;
endfunction

function SS_ItemValidLocation( spawn_coords )

	var list_near := ListItemsAtLocation( spawn_coords.x, spawn_coords.y, spawn_coords.z, spawn_coords.realm );
	if( list_near.Size() )
		return 0;
	endif

	var list_statics := ListMultisInBox( spawn_coords.x-4, spawn_coords.y-4, spawn_coords.z, spawn_coords.x+4, spawn_coords.y+4, spawn_coords.z, spawn_coords.realm );
	if( list_statics.Size() )
		return 0;
	endif

	if(!SS_IsInHouse( spawn_coords ))
		return 0;
	endif

	return 1;
endfunction

function SS_NPCValidLocation( spawn_coords )

	if( !SS_IsValidCreationTile( spawn_coords.x, spawn_coords.y, spawn_coords.z, spawn_coords.realm ))
		return 0;
	endif

	var list_statics := ListMultisInBox( spawn_coords.x-4, spawn_coords.y-4, spawn_coords.z, spawn_coords.x+4, spawn_coords.y+4, spawn_coords.z, spawn_coords.realm );
	if( list_statics.Size() )
		return 0;
	endif

	if(!SS_IsInHouse( spawn_coords ))
		return 0;
	endif

	return 1;
endfunction

/*
 * SS_IsValidCreationTile(x, y, z, realm)
 *
 * Determines if items and npcs can exist on an X Y Z tile.
 *
 * Parameters
 * x:		X coordinate
 * y:		Y coordinate
 * z:		Z coordinate
 * realm:	Realm name
 *
 * Return Value
 * Returns 1 if it is valid.
 * Returns error if it is not.
 *
 */
function SS_IsValidCreationTile( x, y, z, realm )

  var landtiles_cfg := ReadConfigFile( "::landtiles" );
	var map_info := GetMapInfo( x, y, realm );
	var tile_elem := landtiles_cfg[Hex( map_info.landtile )];

	if( !tile_elem.MoveLand )
		var tiles_cfg := ReadConfigFile( "::tiles" );
		foreach static in ( ListStaticsAtLocation( x, y, z, 0, realm ))
			if( static.z == z )
				if( tiles_cfg[Hex( static.objtype )].MoveLand )
					return 1;
				endif
			endif
			SleepMS(10);
		endforeach

		foreach item in ( ListItemsAtLocation( x, y, z, realm ))
			if( item.z == z )
				if( tiles_cfg[item.graphic].MoveLand )
					return 1;
				endif
			endif
			SleepMS(10);
		endforeach

		return 0;
	endif

	return 1;
endfunction

function SS_FindSpawnZone( creature )

	var areas := SS_GetRegionNames();

	var found := array{};
	foreach region_name in areas

		var coords := SS_GetRegionCoordinates( region_name );

		var MinX := coords.x1,
		MaxX := coords.x2,
		MinY := coords.y1,
		MaxY := coords.y2,
		Realm := SS_GetRegionRealm( region_name );

		if( SS_IsInBox( creature, MinX, MaxX, MinY, MaxY, Realm ))
			found.Append( region_name );
		endif

		SleepMS(5);
	endforeach

	return found;
endfunction

function SS_IsInBox( mobile, x1, x2, y1, y2, realm )

	if( mobile.realm == realm )
		if(( mobile.x > x1 && mobile.x < x2 ) && ( mobile.y > y1 && mobile.y < y2 ))
			return 1;
		endif
	endif

	return 0;
endfunction

function SS_IsInHouse( spawn_coords )

	// Are we trying to spawn in a static house?
	var hsign := GetHouseSign( spawn_coords );
	var IsLocInHouse := hsign.IsLocationInsideHouse( spawn_coords );;

	// Are we trying to spawn in a static house?
	// IsLocInHouse := FindHouseForLocation (spawn_coords.x, spawn_coords.y, spawn_coords.z, spawn_coords.realm);

	if(IsLocInHouse)
		return 0;
	endif

	// Are we trying to spawn in a multi?
	// var sh := GetStandingHeight(spawn_coords.x, spawn_coords.y, spawn_coords.z, spawn_coords.realm);
	// if ( sh.multi )
	//	return 0;
	// endif

  return 1;

endfunction








