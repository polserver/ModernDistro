use uo;
use os;
use math;
use cfgfile;
use datafile;

include ":resourcecore:mapTiles";
include ":resourcecore:settings";

function RC_OpenDirtDatafile()

	var datafile := OpenDataFile( ":resourcecore:dirt" );
	if( !datafile )
		datafile := CreateDataFile( ":resourcecore:dirt", DF_KEYTYPE_STRING );
		if( !datafile )
			return error{"errortext":="Could not open ':resourcecore:dirt' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function RC_FindDirtDataElem( byref elem_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := RC_OpenDirtDatafile();
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

function RC_GetDirtProps( byref data_elem, byref prop_name )

	var df_prop := data_elem.GetProp( prop_name ),
		settings := 0;

	if( df_prop.errortext )
		settings := RC_GetSettingsCfgElem( "DirtSettings" );
		case( prop_name )
			"Harvest": return 0;
			"Quantity": return CInt( settings.MaxYield );
		endcase
	endif

	return df_prop;
endfunction

function RC_HarvestDirt( byref mobile, byref use_on, byref amount )

	var data_elem := RC_FindDirtDataElem( Cstr( use_on.x+"_"+use_on.y+"_"+use_on.z+"_"+use_on.realm ), 0 ),
		last_harvest := RC_GetDirtProps( data_elem, "Harvest" ),
		quantity :=  RC_GetDirtProps( data_elem, "Quantity" ),
		settings := RC_GetSettingsCfgElem( "DirtSettings" );

	quantity += Floor( Cint( ReadGameClock() - last_harvest ) / CInt( settings.SecondsPerRegrow ) );
	if( quantity > CInt( settings.MaxYield ) )
		quantity := CInt( settings.MaxYield );
	endif

	if( quantity < 1 )
		SendSysMessage( mobile, "There's not enough dirt here to harvest.", 0x3, 0x22 );
		return 0;
	elseif( CoordinateDistance( mobile.x, mobile.y, use_on.x, use_on.y ) > 2 )
		SendSysMessage( mobile, "You must be nearer to harvest any dirt!", 0x3, 0x22 );
		return 0;
	endif

	PlaySoundEffectXYZ( use_on.x, use_on.y, use_on.z, 0x58, use_on.realm );
	if( quantity < amount )
		SendSysMessage( mobile, "You manage to harvest "+quantity+" units of dirt.", 0x3, 0x32 );
		amount := quantity;
	endif

	quantity -= amount;
	if( quantity < 0 )
		quantity := 0;
	endif
	last_harvest := ReadGameClock();

	data_elem.SetProp( "Harvest", last_harvest );
	data_elem.SetProp( "Quantity", quantity );

	return amount;
endfunction
