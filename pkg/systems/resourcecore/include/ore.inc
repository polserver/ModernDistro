use uo;
use os;
use cfgfile;
use datafile;

function RC_OpenOreDatafile()

	var datafile := OpenDataFile( ":resourcecore:ore" );
	if( !datafile )
		datafile := CreateDataFile( ":resourcecore:ore", DF_KEYTYPE_STRING );
		if( !datafile )
			return error{"errortext":="Could not open ':resourcecore:ore' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function RC_FindOreDataElem( byref elem_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := RC_OpenOreDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.FindElement( elem_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		df_elem := datafile.CreateElement( elem_name );
		if( df_elem.errortext )
			return error{"errortext":="Unable to create datafile element "+df_elem.errortext};
		endif
	endif

	return df_elem;
endfunction