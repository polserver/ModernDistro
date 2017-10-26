/*

	spawnSystem originally created by Austin for the 0.97 Distro
	
	Bugs fixed by Edwards
	
	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":spawnsystem:groups";
include ":spawnsystem:realms";
include ":spawnsystem:regions";

function SS_MoveRegionToNewRealm( region_name, realm_name )

	region_name := Lower( region_name );
	realm_name := Lower( realm_name );
	
	var cur_realm := SS_GetRegionRealm( region_name );
	SS_RemoveRegionFromRealm( cur_realm, region_name );
	SS_AddRegionToRealm( realm_name, region_name );
	SS_SetRegionRealm( region_name, realm_name );
	
	return 1;
endfunction

function SS_FullRemoveRegion( region_name )

	region_name := Lower( region_name );
	
	var realm_name := SS_GetRegionRealm( region_name );
	SS_RemoveRegionFromRealm( realm_name, region_name );
	SS_RemoveRegion( region_name );
	
	return 1;
endfunction

function SS_RestartSpawner()

	var pid := CInt( GetGlobalProperty( "#SpawnSystemPid" ));
	var process := GetProcess( pid );

	process.Kill();

	Start_Script( ":spawnsystem:spawnSystem" );

	return 1;
endfunction
