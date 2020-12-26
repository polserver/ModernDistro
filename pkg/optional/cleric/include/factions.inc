use uo;
use os;
use datafile;

include ":factions:factions_ex";

function FACTIONS_OpenDatafile()

	var datafile := OpenDataFile( ":factions:factions" );
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := CreateDataFile( ":factions:factions", DF_KEYTYPE_STRING );
		if( TypeOfInt( datafile ) != OT_DATAFILEREF )
			return error{"errortext":="Could not find or create ':factions:factions' -> "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function FACTIONS_FindFaction( byref faction_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := FACTIONS_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.FindElement( faction_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to find datafile elem '"+faction_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

function FACTIONS_AddFaction( byref faction_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := FACTIONS_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.CreateElement( faction_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		return error{"errortext":="Unable to create datafile elem '"+faction_name+"' -> "+df_elem.errortext};
	endif

	return df_elem;
endfunction

function FACTIONS_DeleteFaction( byref faction_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := FACTIONS_OpenDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.DeleteElement( faction_name );
	if( df_elem.errortext )
		return error{"errortext":="Unable to delete datafile elem '"+faction_name+"' -> "+df_elem.errortext};
	endif

	return 1;
endfunction
