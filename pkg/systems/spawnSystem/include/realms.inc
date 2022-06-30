/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;
use polsys;

include ":datafile:datafile";

/*
 * SS_GetRealmDataFile()
 *
 * Purpose
 * Quickly pull up the datafile that stores realm information.
 *
 * Parameters
 *
 * Return Value
 * Data file reference
 *
 */
function SS_GetRealmDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Realms", DF_CREATE );
	return data_file;
endfunction

/*
 * SS_GetRealmDataElem(elem_name)
 *
 * Purpose
 * Quickly retrieve a realm elem for the spawner.
 *
 * Parameters
 * elem_name:	Name of the elem to retrieve
 *
 * Return Value
 * A datafile elem reference.
 *
 */
function SS_GetRealmDataElem( elem_name )

	var data_file := SS_GetRealmDataFile();
	var realm_elem := DFFindElement( data_file, elem_name, DF_NO_CREATE );

	return realm_elem;
endfunction

/*
 * SS_AddRealm(realm_name)
 *
 * Purpose
 * Adds a realm name to the spawner's realm list
 *
 * Parameters
 * realm_name:	A string containing the name of the new realm.
 *
 * Return Value
 * Returns 1 on success.
 *
 */
function SS_AddRealm( realm_name )

	var core_realms := Realms();
	realm_name := Lower( realm_name );

	if( !core_realms.Exists( Lower( realm_name )))
		return error{"errortext":="'"+realm_name+"' is not a valid realm name."};
	endif

	var data_file := SS_GetRealmDataFile();
	if( data_file.FindElement( realm_name ) != error )
		return error{"errortext":="Realm already exists."};
	endif

	return data_file.CreateElement( realm_name );
endfunction

/*
 * SS_RemoveRealm(realm_name)
 *
 * Purpose
 * Removes a realm elem from the spawner's list
 *
 * Parameters
 * realm_name:	A string containing the name of the realm to remove.
 *
 * Return Value
 * Result of the elem deletion.
 *
 */
function SS_RemoveRealm( realm_name )

	var data_file := SS_GetRealmDataFile();
	return data_file.DeleteElement( Lower( realm_name ));
endfunction

/*
 * SS_GetRealmNames(index)
 *
 * Purpose
 * Retrieves a list of realm names listed in the spawner.
 *
 * Parameters
 * index:	0 - Default
 *		0 - Will return all names.
 *		# - Will return a specific name in the list.
 *
 * Return Value
 * An array of strings
 *
 */
function SS_GetRealmNames( index:=0 )

	var data_file := SS_GetRealmDataFile();
	var realm_names := DFGetElemNames( data_file );

	if( index )
		return realm_names[index];
	else
		return realm_names;
	endif
endfunction

/*
 * SS_AddRegionToRealm(realm_name, region_name)
 *
 * Purpose
 * Adds a region name to the realm's elem.
 *
 * Parameters
 * realm_name:	The realm the region will be added to.
 * region_name:	The name of the region to add.
 *
 * Return Value
 * 1 on success
 *
 */
function SS_AddRegionToRealm( realm_name, region_name )

	var realm_elem := SS_GetRealmDataElem( realm_name );
	var region_list := SS_GetRegionsInRealm( realm_name );
	region_name := Lower( region_name );

	if( region_list.Exists( region_name ))
		return error{"errortext":="Region already exists in that realm."};
	else
		region_list.Insert( region_name, 1 );
		return realm_elem.SetProp( "Regions", region_list );
	endif
endfunction

/*
 * SS_RemoveRegionFromRealm(realm_name, region_name)
 *
 * Purpose
 * Removes a region name from the realm's elem.
 *
 * Parameters
 * realm_name:	The realm the region will be removed from.
 * region_name:	The name of the region to remove.
 *
 * Return Value
 * Returns the modified region list.
 *
 * Notes:
 *    See SS_FullRemoveRegion() in commontasks.inc
 *
 */
function SS_RemoveRegionFromRealm( realm_name, region_name )

	var realm_elem := SS_GetRealmDataElem( realm_name );
	var region_list := SS_GetRegionsInRealm( realm_name );
	region_name := Lower( region_name );

	if( !region_list.Exists( region_name ))
		return error{"errortext":="No region exists by that name."};
	else
		region_list.Erase( region_name );
		return realm_elem.SetProp( "Regions", region_list );
	endif
endfunction

/*
 * SS_GetRegionsInRealm(realm_name)
 *
 * Purpose
 * Lists the spawn regions that exist within a realm.
 *
 * Parameters
 * realm_name:	Name of the realm to retrieve the list from.
 *
 * Return Value
 * Returns an array of spawn region names in the realm.
 *
 */
function SS_GetRegionsInRealm( realm_name )

	var realm_elem := SS_GetRealmDataElem( realm_name );
	var region_list := realm_elem.GetProp( "Regions" );
	if( !region_list )
		region_list := dictionary;
	endif

	return region_list;
endfunction
