/*

	spawnSystem originally created by Austin for the 0.97 Distro
	
	Bugs fixed by Edwards
	
	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":datafile:datafile";

function SS_GetRegionDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Regions", DF_CREATE );
	return data_file;
endfunction

function SS_GetRegionDataElem( elem_name )

	var data_file := SS_GetRegionDataFile();
	var region_elem := DFFindElement( data_file, elem_name, DF_NO_CREATE );

	return region_elem;
endfunction

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

function SS_RemoveRegion( region_name )

	var data_file := SS_GetRegionDataFile();
	return data_file.DeleteElement( Lower( region_name ));
endfunction

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

function SS_GetRegionRealm( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.GetProp( "Realm" );
endfunction

function SS_SetRegionRealm( region_name, realm_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.SetProp( "Realm", Lower( realm_name ));
endfunction

function SS_GetRegionCoordinates( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	return region_elem.GetProp( "Coordinates" );
endfunction

function SS_SetRegionCoordinates( region_name, x1, y1, x2, y2 )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	
	var coords := struct{"x1":=CInt( x1 ), "y1":=CInt( y1 ), "x2":=CInt( x2 ), "y2":=CInt( y2 )};
		
	return region_elem.SetProp( "Coordinates", coords );
endfunction

function SS_GetGroupsInRegion( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var group_list := region_elem.GetProp( "Groups" );
	if( !group_list )
		group_list := dictionary;
	endif
	
	return group_list;
endfunction

function SS_GetGroupInRegion( region_name, group_name )
 
 	var group_list := SS_GetGroupsInRegion( Lower( region_name ));
 	return group_list[Lower( group_name )];
endfunction

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

function SS_RemoveGroupFromRegion( region_name, group_name )

	region_name := Lower( region_name );
	group_name := Lower( group_name );
	
	var group_list := SS_GetGroupsInRegion( region_name );
	
	group_list.Erase( group_name );
	
	var region_elem := SS_GetRegionDataElem( region_name );
	region_elem.EraseProp( "Spawns-"+group_name ); //Clears the spawns list too
	return region_elem.SetProp( "Groups", group_list );
endfunction

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
	
	var region_elem := SS_GetRegionDataElem( region_name );
	region_elem.SetProp( "Spawns-"+group_name, spawn_list );
        
        return 1;
endfunction

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

function SS_GetRegionSpawnWait( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var wait := CInt( region_elem.GetProp( "SpawnWait" ));
	if( !wait )
		wait := 300;
	endif
 
	return wait;
endfunction

function SS_SetRegionSpawnWait( region_name, seconds )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	
	seconds := CInt( seconds );
	if( seconds < 1 )
		seconds := 1;
	endif
	
	region_elem.SetProp( "SpawnWait", seconds );
 
	return 1;
endfunction

function SS_GetRegionSpawnTime( region_name )

	var region_elem := SS_GetRegionDataElem( Lower( region_name ));
	var wait := CInt( region_elem.GetProp( "LastSpawn" ));
	if( wait < 1 )
		wait := 1;
	endif
 
	return wait;
endfunction

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
