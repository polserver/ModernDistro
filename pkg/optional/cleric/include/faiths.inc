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

function Faiths_FindFaith( byref Faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.FindElement( Faith_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to find datafile elem '"+Faith_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

function Faiths_AddFaith( byref Faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.CreateElement( Faith_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to create datafile elem '"+Faith_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

function Faiths_DeleteFaith( byref Faith_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := Faiths_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.DeleteElement( Faith_name );
	if( df_elem.errortext )
		return error{"errortext":="Unable to delete datafile elem '"+Faith_name+"' -> "+df_elem.errortext};
	endif

	return 1;
endfunction

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

