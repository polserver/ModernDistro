use uo;
use os;
use cfgfile;

function OpenLandTilesCfgFile()

	var cfg_file := ReadConfigFile( "::landtiles" );
	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		return error{"errortext":="Could not find and open '::landtiles' -> "+cfg_file.errortext};
	endif

	return cfg_file;
endfunction

function FindLandTileInfo( byref tile_id, byref cfg_file := 0 )

	var cfg_elem := 0;

	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := OpenLandTilesCfgFile();
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