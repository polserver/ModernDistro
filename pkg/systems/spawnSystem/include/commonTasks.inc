/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Official Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":spawnsystem:groups";
include ":spawnsystem:realms";
include ":spawnsystem:regions";

/*
 * SS_MoveRegionToNewRealm(region_name, realm_name)
 *
 * Purpose
 * Makes calls to regions.inc and realms.inc to move a spawn region from one realm to another.
 * No error checking is present!
 *
 * Parameters
 * region_name:	Name of the region to move
 * realm_name:	Name of the realm to move the region to
 *
 * Returns
 * 1 (No useful return value)
 *
 */
function SS_MoveRegionToNewRealm( region_name, realm_name )

	region_name := Lower( region_name );
	realm_name := Lower( realm_name );

	var cur_realm := SS_GetRegionRealm( region_name );
	SS_RemoveRegionFromRealm( cur_realm, region_name );
	SS_AddRegionToRealm( realm_name, region_name );
	SS_SetRegionRealm( region_name, realm_name );

	return 1;
endfunction

/*
 * SS_FullRemoveRegion(region_name)
 *
 * Purpose
 * Calls SS_RemoveRegionFromRealm() and SS_RemoveRegion() to cleanly remove a region.
 *
 * Parameters
 * region_name:	Name of the region to erase.
 *
 * Return Value
 * Returns 1
 *
 */
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
