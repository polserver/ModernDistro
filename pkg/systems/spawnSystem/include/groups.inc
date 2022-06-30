/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;

include ":datafile:datafile";

/*
 * SS_GetGroupDataFile()
 *
 * Purpose
 * Quickly pull up the datafile that stores group information.
 *
 * Parameters
 *
 * Return Value
 * Data file reference
 *
 */
function SS_GetGroupDataFile()

	var data_file := DFOpenDataFile( ":spawnsystem:Groups", DF_CREATE );

	return data_file;
endfunction

/*
 * SS_GetGroupDataElem(elem_name)
 *
 * Purpose
 * Quickly retrieve a group elem for the spawner.
 *
 * Parameters
 * elem_name:	Name of the elem to retrieve
 *
 * Return Value
 * A datafile elem reference.
 *
 */
function SS_GetGroupDataElem( elem_name )

	var data_file := SS_GetGroupDataFile();
	var group_elem := DFFindElement( data_file, Lower( elem_name ), DF_NO_CREATE );

	return group_elem;
endfunction

/*
 * SS_AddGroup(group_name, engine_type)
 *
 * Purpose
 * Created a new group elem in the data file.
 *
 * Parameters
 * group_name:	String containing the name of the group.
 * engine_type:	Optional parameter to set the engine the group uses.
 *
 * Return Value
 * Returns the result of the elem creation
 *
 */
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

/*
 * SS_RemoveGroup(group_name)
 *
 * Purpose
 * Removes a group elem from the groups list
 *
 * Parameters
 * group_name:	A string containing the name of the group to remove.
 *
 * Return Value
 * Result of the elem deletion.
 *
 */
function SS_RemoveGroup( group_name )

	var data_file := SS_GetGroupDataFile();
	return data_file.DeleteElement( Lower( group_name ));
endfunction

/*
 * SS_GetGroupNames(index)
 *
 * Purpose
 * Retrieves a list of group names listed in the spawner.
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
function SS_GetGroupNames( index:=0 )

	var data_file := SS_GetGroupDataFile();
	var group_names := DFGetElemNames( data_file );

	if( index )
		return group_names[index];
	else
		return group_names;
	endif
endfunction

/*
 * SS_GetGroupEngine(group_name)
 *
 * Purpose
 * Retrieves the engine that the group will use.
 *
 * Parameters
 * group_name:	Name of the group to get the engine type from.
 *
 * Return Value
 * String
 *
 */
function SS_GetGroupEngine( group_name )

	var group_elem := SS_GetGroupDataElem( Lower( group_name ));
	return group_elem.GetProp( "Engine" );
endfunction

/*
 * SS_SetGroupEngine(group_name)
 *
 * Purpose
 * Sets the engine that the group will use.
 *
 * Parameters
 * group_name:	Name of the group to set the engine type for.
 * engine_name:	Name of the engine the group will use.
 *
 * Return Value
 * 1 on succcess
 *
 */
function SS_SetGroupEngine( group_name, engine_name )

	group_name := Lower( group_name );

	var group_elem := SS_GetGroupDataElem( group_name );

	return group_elem.SetProp( "Engine", engine_name );
endfunction

/*
 * SS_AddEntryToGroup(group_name, entry_name)
 *
 * Purpose
 * Adds an entry name to the groups's elem.
 *
 * Parameters
 * group_name:	The group the entry will be added to.
 * entry_name:	The name of the entry to add.
 *
 * Return Value
 * 1 on success
 *
 */
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

/*
 * SS_RemoveEntryFromGroup(group_name, entry_name)
 *
 * Purpose
 * Removes an entry name from the groups's elem.
 *
 * Parameters
 * group_name:	The group the entry will be removed from.
 * entry_name:	The name of the entry to remove.
 *
 * Return Value
 * 1 on success
 *
 */
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

/*
 * SS_GetEntriesInGroup(group_name)
 *
 * Purpose
 * Retrieves the entries in a spawn group
 *
 * Parameters
 * group_name:	Name of the group to retrieve the entries from.
 *
 * Return Value
 * Dictionary
 * Keys are the entries - nothing else is used.
 *
 */
function SS_GetEntriesInGroup( group_name )

	group_name := Lower( group_name );

	var group_elem := SS_GetGroupDataElem( group_name );
	var group_list := group_elem.GetProp( "EntryList" );

	if( !group_list )
		group_list := dictionary;
	endif

	return group_list;
endfunction

/*
 * SS_GetEntryByNumber(group_name, number)
 *
 * Purpose
 * Retrieves an entry by its position number.
 *
 * Parameters
 * group_name:	Name of the group to retrieve an entry from.
 *
 * Return Value
 * Returns a string on success.
 * Returns an empty string if nothing is set.
 *
 */
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
