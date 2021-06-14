use uo;
use os;
use datafile;

// include ":factions:factions_ex";

function Faiths_OpenDatafile()

	var datafile := OpenDataFile( ":cleric:faiths" );
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := CreateDataFile( ":cleric:faiths", DF_KEYTYPE_STRING );
		if( TypeOfInt( datafile ) != OT_DATAFILEREF )
			return error{"errortext":="Could not find or create ':cleric:faiths' -> "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

///////////////////////////////////////////////////////////
//
// Faiths_FindFaith( byref faith_name, byref datafile := 0 )
//
// Purpose: Attempts to find a faith data element in
// 			the faiths database.
//
//
// Parameters: faith_name - The faith to find.
//			   datafile - No need to pass this parameter.
//
// Returns: A datafile element or error.
//
///////////////////////////////////////////////////////////
function Faiths_FindFaith( byref faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return error{"errortext" := datafile.errortext};
		endif
	endif

	df_elem := datafile.FindElement( faith_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to find datafile elem '"+faith_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

///////////////////////////////////////////////////////////
//
// Faiths_AddFaith( byref Faith_name, byref datafile := 0 )
//
// Purpose: Adds a faith to the faiths database.
//
// Parameters: faith_name - new faith to be added.
//			   datafile - No need to pass this parameter.
//
// Returns: A datafile element if successful or error.
//
///////////////////////////////////////////////////////////
function Faiths_AddFaith( byref faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.CreateElement( faith_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to create datafile elem '"+faith_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

///////////////////////////////////////////////////////////
//
// Faiths_DeleteFaith( byref Faith_name, byref datafile := 0 )
//
// Purpose: Deletes a faith from the faiths datafile.
//
//
//
// Parameter: faith_name - the faith to remove.
//			  datafile - No need to pass this.
//
// Returns: 1 if successful, error if unsuccessful.
//
///////////////////////////////////////////////////////////
function Faiths_DeleteFaith( byref Faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return error{"errortext" := datafile.errortext};
		endif
	endif
	var faith_list := Faiths_ListFaiths();
	var is_valid_faith := faith in faith_list;
	if(!is_valid_faith)
		return error{"errortext":="Invalid faith --> " + Faith_name + "."};
	endif

	df_elem := datafile.DeleteElement( Faith_name );
	if( df_elem.errortext )
		return error{"errortext":="Unable to delete datafile elem '"+Faith_name+"' -> "+df_elem.errortext};
	endif

	return 1;
endfunction

/////////////////////////////////////////////////////////////////////
// Faiths_ListFaiths( byref datafile := 0 )
//
// Purpose: Get a list of all faiths on the shard.
// 			This function removes non-faith entries, eg. Foci
//			and UntroddenPaths from the returned list.
//
// Parameters: None.
//
// Returns: An array containing the faiths.
//
/////////////////////////////////////////////////////////////////////
function Faiths_ListFaiths( byref datafile := 0 )

	var faith_list := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	faith_list := datafile.keys();
	foreach faith in faith_list
		case(faith)
			"Foci": faith_list.erase(_faith_iter);
			"UntroddenPaths": faith_list.erase(_faith_iter);
		endcase
	endforeach
	return faith_list;

endfunction

