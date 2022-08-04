use uo;
use os;
use cfgfile;

function OpenOffsetsCfgFile()

	var cfg_file := ReadConfigFile( ":itemutils:offset" );
	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		return error{"errortext":="Could not find and open ':itemutils:offset' -> "+cfg_file.errortext};
	endif

	return cfg_file;
endfunction

function FindTileOffsetInfo( byref tile_id, byref cfg_file := 0 )

	var cfg_elem := 0;
	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := OpenOffsetsCfgFile();
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	cfg_elem := FindConfigElem( cfg_file, tile_id );
	if( TypeOfInt( cfg_elem ) != OT_CONFIGELEMREF )
		return error{"errortext":="Could not find tile info on '"+Hex( tile_id )+"' -> "+cfg_elem.errortext};
	endif

	return cfg_elem;
endfunction

function ItemBounds( byref tile_id )

	var b := FindTileOffsetInfo( tile_id );
	if( b.errortext )
		return b.errortext;
	endif

	return Struct{ "Height":=b.yMax, "Width":=b.xMax, "X":=b.xMin, "Y":=b.yMin };
endfunction
