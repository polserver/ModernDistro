/*

	spawnSystem originally created by Austin for the 0.97 Distro
	
	Bugs fixed by Edwards
	
	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;
use polsys;

include ":datafile:datafile";

function SS_GetRealmDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Realms", DF_CREATE );
	return data_file;
endfunction

function SS_GetRealmDataElem( elem_name )

	var data_file := SS_GetRealmDataFile();
	var realm_elem := DFFindElement( data_file, elem_name, DF_NO_CREATE );

	return realm_elem;
endfunction

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

function SS_RemoveRealm( realm_name )

	var data_file := SS_GetRealmDataFile();
	return data_file.DeleteElement( Lower( realm_name ));
endfunction

function SS_GetRealmNames( index:=0 )

	var data_file := SS_GetRealmDataFile();
	var realm_names := DFGetElemNames( data_file );
	
	if( index )
		return realm_names[index];
	else
		return realm_names;
	endif
endfunction

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

function SS_GetRegionsInRealm( realm_name )

	var realm_elem := SS_GetRealmDataElem( realm_name );
	var region_list := realm_elem.GetProp( "Regions" );
	if( !region_list )
		region_list := dictionary;
	endif
	
	return region_list;
endfunction
