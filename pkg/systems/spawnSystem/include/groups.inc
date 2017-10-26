/*

	spawnSystem originally created by Austin for the 0.97 Distro
	
	Bugs fixed by Edwards
	
	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":datafile:datafile";

function SS_GetGroupDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Groups", DF_CREATE );
		
	return data_file;
endfunction

function SS_GetGroupDataElem( elem_name )

	var data_file := SS_GetGroupDataFile();
	var group_elem := DFFindElement( data_file, Lower( elem_name ), DF_NO_CREATE );
	
	return group_elem;
endfunction

function SS_AddGroup( group_name, engine_name:="" )

	group_name := Lower( group_name );
		
	var data_file := SS_GetGroupDataFile();
	
	if( data_file.FindElement( group_name ) != error )
		return error{"errortext":="Group already exists."};
	endif
	
	var result := data_file.CreateElement( group_name );
	
	if( engine_name && result )
		SS_SetGroupEngine( group_name, engine_name );
	endif
	
	return result;
endfunction

function SS_RemoveGroup( group_name )

	var data_file := SS_GetGroupDataFile();
	return data_file.DeleteElement( Lower( group_name ));
endfunction

function SS_GetGroupNames( index:=0 )

	var data_file := SS_GetGroupDataFile();
	var group_names := DFGetElemNames( data_file );
	
	if( index )
		return group_names[index];
	else
		return group_names;
	endif
endfunction

function SS_GetGroupEngine( group_name )

	var group_elem := SS_GetGroupDataElem( Lower( group_name ));
	return group_elem.GetProp( "Engine" );
endfunction

function SS_SetGroupEngine( group_name, engine_name )

	group_name := Lower( group_name );
	
	var group_elem := SS_GetGroupDataElem( group_name );
	
	return group_elem.SetProp( "Engine", engine_name );
endfunction

function SS_AddEntryToGroup( group_name, entry_name )

	group_name := Lower( group_name );
	entry_name := Lower( entry_name );
	
	var entry_list := SS_GetEntriesInGroup( group_name );
		
	if( entry_list.Exists( entry_name ))
		return 0;
	else
		entry_list.Insert( entry_name, 1 );
		var group_elem := SS_GetGroupDataElem( group_name );
		return group_elem.SetProp( "EntryList", entry_list );
	endif
endfunction

function SS_RemoveEntryFromGroup( group_name, entry_name )

	group_name := Lower( group_name );
	entry_name := Lower( entry_name );
	
	var entry_list := SS_GetEntriesInGroup( group_name );
	if( !entry_list.Exists( entry_name ))
		return 0;
	else
		entry_list.Erase( entry_name );
		var group_elem := SS_GetGroupDataElem( group_name );
		return group_elem.SetProp( "EntryList", entry_list );
	endif
endfunction

function SS_GetEntriesInGroup( group_name )

	group_name := Lower( group_name );
	
	var group_elem := SS_GetGroupDataElem( group_name );
	var group_list := group_elem.GetProp( "EntryList" );
	
	if( !group_list )
		group_list := dictionary;
	endif
	
	return group_list;
endfunction

function SS_GetEntryByNumber( group_name, number )

	group_name := Lower( group_name );
	number := CInt( number );
	
	var entries := SS_GetEntriesInGroup( group_name );
	entries := entries.Keys();
	var entry := entries[number];
	if( entry )
		return CStr( entry );
	else
		return "";
	endif
endfunction
