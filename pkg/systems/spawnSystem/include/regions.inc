/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":datafile:datafile";

/*
 * SS_GetRegionDataFile()
 *
 * Purpose
 * Quickly pull up the datafile that stores region information.
 *
 * Parameters
 *
 * Return Value
 * Data file reference
 *
 */
function SS_GetRegionDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Regions", DF_CREATE );
	return data_file;
endfunction

/*
 * SS_GetRegionDataElem(elem_name)
 *
 * Purpose
 * Quickly retrieve a region elem for the spawner.
 *
 * Parameters
 * elem_name:	Name of the elem to retrieve
 *
 * Return Value
 * A datafile elem reference.
 *
 */
function SS_GetRegionDataElem( elem_name )

	var data_file := SS_GetRegionDataFile();
	var region_elem := DFFindElement( data_file, elem_name, DF_NO_CREATE );

	return region_elem;
endfunction

/*
 * SS_AddRegion(region_name)
 *
 * Purpose
 * Adds a region name to the spawner's region list
 *
 * Parameters
 * region_name:	A string containing the name of the new region.
 * realm_name:	Optional parameter to set the region's realm on creation.
 *
 * Return Value
 * Returns 1 on success.
 *
 */
function SS_AddRegion( region_name, realm_name:="" )

	var data_file := SS_GetRegionDataFile();

	region_name := Lower( region_name );
	realm_name := Lower( realm_name );

	if( data_file.FindElement( region_name ) != error )
		return error{"errortext":="Region already exists."};
	endif

	var result := data_file.CreateElement( region_name );
	if( realm_name && result )
		SS_SetRegionRealm( region_name, realm_name );
	endif

	return result;
endfunction

/*
 * SS_RemoveRegion(region_name)
 *
 * Purpose
 * Removes a region elem from the spawner's list
 *
 * Parameters
 * region_name:	A string containing the name of the region to remove.
 *
 * Return Value
 * Result of the elem deletion.
 *
 * Notes:
 *    See SS_FullRemoveRegion() in commontasks.inc
 *
 */
function SS_RemoveRegion( region_name )

	var data_file := SS_GetRegionDataFile();
	return data_file.DeleteElement( Lower( region_name ));
endfunction

/*
 * SS_GetRegionNames(region_name:="")
 *
 * Purpose
 * Retrieves a list of region names listed in the spawner.
 *
 * Parameters
 * realm_name:	Optional parameter to list regions that belong to a specific realm.
 *		- Note its better to get them from SS_GetRegionsInRealm(realm_name) in realms.inc
 * index:	0 - Default
 *		0 - Will return all names.
 *		# - Will return a specific name in the list.
 *
 * Return Value
 * An array of strings
 *
 */
function SS_GetRegionNames( realm_name:="", index:=0 )

	var data_file := SS_GetRegionDataFile();

	realm_name := Lower( realm_name );

	var region_names := array{};
	if( !realm_name )
		region_names := DFGetElemNames( data_file );
	else
		foreach region_name in ( DFGetElemNames( data_file ))
			if( Lower( SS_GetRegionRealm( region_name )) == Lower( realm_name ))
				region_names.Append( Lower( region_name ));
			endif
			SleepMS(10);
		endforeach
	endif

	if( index )
		return region_names[index];
	else
		return region_names;
	endif
endfunction

/*
 * SS_GetRegionRealm(region_name)
 *
 * Purpose
 * Get the name of the realm the region is set to.
 *
 * Parameters
 * region_name:	Name of the region to lookup.
 *
 * Return Value
 * A string containing the name of a realm
 *
 */
function SS_GetRegionRealm( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.GetProp( "Realm" );
endfunction

/*
 * SS_SetRegionRealm(region_name, realm_name)
 *
 * Purpose
 * Sets the realm the region belongs to.
 *
 * Parameters
 * region_name:	The name of the region to set.
 * realm_name:	The name of the realm the region belongs to.
 *
 * Return Value
 * 1 on success
 *
 */
function SS_SetRegionRealm( region_name, realm_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.SetProp( "Realm", Lower( realm_name ));
endfunction

/*
 * SS_GetRegionCoordinates(region_name)
 *
 * Purpose
 * Retrieves the coordinates for the region
 *
 * Parameters
 * region_name:	Name of the region to retrieve the coordinates of.
 *
 * Return Value
 * Returns a structure containing .x1, .y1, .x2, .y2
 *
 */
function SS_GetRegionCoordinates( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.GetProp( "Coordinates" );
endfunction

/*
 * SS_SetRegionCoordinates(region_name, x1, y1, x2, y2)
 *
 * Purpose
 * Sets the coordinates for a region
 *
 * Parameters
 * region_name:	Name of the region to set the coordinates on.
 * x1:		Top left X coordinate
 * y1:		Top left Y coordinate
 * x2:		Bottom right X coordinate
 * y2:		Bottom right Y coordinate
 *
 * Return Value
 * 1 on success
 *
 */
function SS_SetRegionCoordinates( region_name, x1, y1, x2, y2 )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));

	var coords := struct{"x1":=CInt( x1 ), "y1":=CInt( y1 ), "x2":=CInt( x2 ), "y2":=CInt( y2 )};

	return region_elem.SetProp( "Coordinates", coords );
endfunction

/*
 * SS_GetGroupsInRegion(region_name)
 *
 * Purpose
 * Retrieves the groups and their settings in a spawn region
 *
 * Parameters
 * region_name:	Name of the region to retrieve the groups from.
 *
 * Return Value
 * Dictionary
 * -keys are the group name, with a struct inside for settings.
 *
 */
function SS_GetGroupsInRegion( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var group_list := region_elem.GetProp( "Groups" );
	if( !group_list )
		group_list := dictionary;
	endif

	return group_list;
endfunction

/*
 * SS_GetGroupInRegion(region_name, group_name)
 * Retrieves a specific spawn group and its settings from a region
 *
 * Parameters
 * region_name:	Name of the region the spawn group is in.
 * group_name:	Name of the region to retrieve.
 *
 * Return Value
 * Struct
 *
 */
function SS_GetGroupInRegion( region_name, group_name )

 	var group_list := SS_GetGroupsInRegion( Lower( region_name ));
 	return group_list[Lower( group_name )];
endfunction

/*
 * SS_AddGroupToRegion(region_name, group_name, max_amt, spawn_wait, enabled)
 *
 * Purpose
 * Adds a spawn group to a spawn region
 *
 * Parameters
 * region_name:	Name of the region to add the group to.
 * group_name:	Name of the group to add to the region.
 * max_amt:	Max amount of objects (mobiles/items) the group should make.
 * enabled:	0 | 1
 *
 * Return Value
 * 1 on success
 *
 * Notes:
 * 	This function can also be used to update a spawn group's settings in a region.
 *
 */
function SS_AddGroupToRegion( region_name, group_name, max_amt, enabled )

	var settings := struct;
	settings.+max_amt := max_amt;
	settings.+enabled := enabled;

	region_name := Lower( region_name );
	group_name := Lower( group_name );

	var group_list := SS_GetGroupsInRegion( region_name );
	group_list.Insert( Lower( group_name ), settings );

	var region_elem := SS_GetRegionDataElem( region_name );
	return region_elem.SetProp( "Groups", group_list );
endfunction

/*
 * SS_RemoveGroupFromRegion(region_name, group_name)
 *
 * Purpose
 * Removes a spawn group from a spawn region
 *
 * Parameters
 * region_name:	Name of the region to remove from the group.
 * group_name:	Name of the group to remove from the region.
 *
 * Return Value
 * 1 on success
 *
 */
function SS_RemoveGroupFromRegion( region_name, group_name )

	region_name := Lower( region_name );
	group_name := Lower( group_name );

	var group_list := SS_GetGroupsInRegion( region_name );

	group_list.Erase( group_name );

	var region_elem := SS_GetRegionDataElem( region_name );
	region_elem.EraseProp( "Spawns-"+group_name ); //Clears the spawns list too
	return region_elem.SetProp( "Groups", group_list );
endfunction

/*
 * SS_AddSpawnToRegion(region_name, group_name, serial)
 *
 * Purpose
 * Used to track what has been spawned in a region's group list.
 *
 * Parameters
 * region_name:	Name of the region the object was created in.
 * group_name:	Name of the group the object is listed in.
 * object:	A reference to the object that was created.
 *
 * Return Value
 * 1 on success
 *
 */
function SS_AddSpawnToRegion( region_name, group_name, object )

	region_name := Lower( region_name );
	group_name := Lower( group_name );

	var spawn_list := SS_GetSpawnsInRegionGroup( region_name, group_name );

	var orig_pos := struct{"x":=object.x, "y":=object.y};

	var spawn_info := struct;
	spawn_info.+time := POLCore().systime;
	spawn_info.+orig_coords := orig_pos;

	spawn_list[object.serial] := spawn_info;

	SetObjProperty( object, "RegionSpawn", region_name );
	SetObjProperty( object, "SpawnGroup", group_name );

	var region_elem := SS_GetRegionDataElem( region_name );
	region_elem.SetProp( "Spawns-"+group_name, spawn_list );

        return 1;
endfunction

/*
 * SS_RemoveSpawnFromRegion(region_name, group_name, serial)
 *
 * Purpose
 * Used to track what has been spawned in a region's group list.
 *
 * Parameters
 * region_name:	Name of the region the object was created in.
 * group_name:	Name of the group the object is listed in.
 * serial:	Serial of the object that is being removed.
 *
 * Return Value
 * 1 on success
 *
 */
function SS_RemoveSpawnFromRegion( region_name, group_name, serial )

	region_name := Lower( region_name );
	group_name := Lower( group_name );

	var spawn_list := SS_GetSpawnsInRegionGroup( region_name, group_name );

	if( spawn_list.Exists( serial ))
		spawn_list.Erase( serial );
		var region_elem := SS_GetRegionDataElem( region_name );
		region_elem.SetProp( "Spawns-"+group_name, spawn_list );
		return 1;
	else
		return 0;
	endif
endfunction

/*
 * SS_GetSpawnsInRegionGroup(region_name, group_name)
 *
 * Purpose
 * Returns a list of objects that have been spawned in a region's group.
 *
 * Parameters
 * region_name:	Name of the region the group is in.
 * group_name:	Name of the group to get the spawn list of.
 *
 * Return Value
 * Dictionary
 *
 */
function SS_GetSpawnsInRegionGroup( region_name, group_name )

	region_name := Lower( region_name );
	group_name := Lower( group_name );

	var region_elem := SS_GetRegionDataElem( region_name );

	var spawns := region_elem.GetProp( "Spawns-"+group_name );
	if( !spawns )
		spawns := dictionary;
	endif

	return spawns;
endfunction

/*
 * SS_GetRegionSpawnWait(region_name)
 *
 * Purpose
 * Retrieves the number of seconds a region waits between spawn runs.
 *
 * Parameters
 * region_name:	Name of the region to get the spawn wait for.
 *
 * Return Value
 * Integer (seconds)
 *
 */
function SS_GetRegionSpawnWait( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var wait := CInt( region_elem.GetProp( "SpawnWait" ));
	if( !wait )
		wait := 300;
	endif

	return wait;
endfunction

/*
 * SS_SetRegionSpawnWait(region_name, seconds)
 *
 * Purpose
 * Sets the number of seconds a region waits between spawn runs.
 *
 * Parameters
 * region_name:	Name of the region to get the spawn wait for.
 * seconds:	Number of seconds between spawns cycles.
 *
 * Return Value
 * 1
 *
 */
function SS_SetRegionSpawnWait( region_name, seconds )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));

	seconds := CInt( seconds );
	if( seconds < 1 )
		seconds := 1;
	endif

	region_elem.SetProp( "SpawnWait", seconds );

	return 1;
endfunction

/*
 * SS_GetRegionSpawnTime(region_name)
 *
 * Purpose
 * Retrieves the last time the region was spawned.
 *
 * Parameters
 * region_name:	Name of the region to get the time from.
 *
 * Return Value
 * Integer (seconds)
 *
 */
function SS_GetRegionSpawnTime( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var wait := CInt( region_elem.GetProp( "LastSpawn" ));
	if( wait < 1 )
		wait := 1;
	endif

	return wait;
endfunction

/*
 * SS_SetRegionSpawnTime(region_name, seconds)
 *
 * Purpose
 * Sets the last time the region was spawned.
 *
 * Parameters
 * region_name:	Name of the region to set the spawn time for.
 * time:	Time the region was spawned. If 0, uses POLCore().systime.
 *
 * Return Value
 * 1
 *
 */
function SS_SetRegionSpawnTime( region_name, time:=0 )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));

	time := CInt( time );
	if( !time )
		time := POLCore().systime;
	endif

	region_elem.SetProp( "LastSpawn", time );

	return 1;
endfunction

function SS_SetRegionDifficulty( region_name, difficulty )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));

	difficulty := CInt( difficulty );
	if( difficulty )
		region_elem.SetProp( "Difficulty", difficulty );
	endif

	return 1;
endfunction

function SS_GetRegionDifficulty( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));

	return CInt( region_elem.GetProp( "Difficulty" ));
endfunction
