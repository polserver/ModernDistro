use uo;
use os;
use cfgfile;
use datafile;

function RC_OpenCropsDatafile()

	var datafile := OpenDataFile( ":resourcecore:crops" );
	if( !datafile )
		datafile := CreateDataFile( ":resourcecore:crops", DF_KEYTYPE_STRING );
		if( !datafile )
			return error{"errortext":="Could not open ':resourcecore:crops' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

function RC_FindCropsDataElem( byref elem_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := RC_OpenCropsDatafile();
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

function RC_GetCropStageGraphic( byref croptype, byref stage )

	var graphic := 0,
		list := Array;

	if( !stage )
		return 0x2233;
	endif

	case( croptype )
		"Cabbage":		list := { 0xC61, 0xC62, 0xC63 }; break;
		"Canteloupe":	list := { 0xC5E, 0xC5F, 0xC60 }; break;
		"Carrot":		list := { 0xC68, 0xC69, 0xC76 }; break;
		"Cotton":		list := { 0xC51, 0xC53, 0xC4F }; break;
		"Corn":			list := { 0xC7E, 0x3D18, 0xC7D }; break;
		"Flax":			list := { 0x1A99, 0x1A9A, 0x1A9B }; break;
		"Hay":			list := { 0x1A96, 0x1A95, 0x1A94, 0x1A92 }; break;
		"Honeydew":		list := { 0xC5E, 0xC5F, 0xC60 }; break;
		"Lettuce":		list := { 0xC61, 0xC62, 0xC63 }; break;
		"Onion":		list := { 0xC68, 0xC69, 0xC6F }; break;
		"Pumpkin":		list := { 0xC5E, 0xC5F, 0xC60 }; break;
		"Turnip":		list := { 0xC61, 0xC62, 0xC63 }; break;
		"Watermelon":	list := { 0xC5E, 0xC5F, 0xC60 }; break;
		"Wheat":		list := { 0xDAE, 0xC56, 0xC57, 0xC58}; break;
	endcase

	graphic := list[stage];
	if( graphic.errortext )
		graphic := list[1];
	endif

	return graphic;
endfunction

