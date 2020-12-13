use uo;
use os;
use cfgfile;
use datafile;

include ":resourcecore:mapTiles";
include ":resourcecore:settings";

function RC_OpenClayDatafile()

	var datafile := OpenDataFile( ":resourcecore:clay" );
	if( !datafile )
		datafile := CreateDataFile( ":resourcecore:clay", DF_KEYTYPE_STRING );
		if( !datafile )
			return error{"errortext":="Could not open ':resourcecore:clay' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function RC_FindClayDataElem( byref elem_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := RC_OpenClayDatafile();
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

function RC_GetClayProps( byref data_elem, byref prop_name )

	var df_prop := data_elem.GetProp( prop_name ),
		settings := 0;

	if( df_prop.errortext )
		settings := RC_GetSettingsCfgElem( "ClaySettings" );
		case( prop_name )
			"Harvest": return 0;
			"Quantity": return CInt( settings.MaxYield );
		endcase
	endif

	return df_prop;
endfunction
