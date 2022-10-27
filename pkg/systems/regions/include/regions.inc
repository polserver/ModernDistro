use uo;
use os;
use cfgfile;

CONST RP_NAME		:= "Name";
CONST RP_TYPE		:= "RegionType";
CONST RP_NO_CAST	:= "NoRune";
CONST RP_NO_LIGHT	:= "NoLight";
CONST RP_NO_HOUSING	:= "NoHousing";
CONST RP_GO_LOC		:= "GoLoc";

CONST RT_CITY		:= "TownRegion";
CONST RT_DUNGEON	:= "DungeonRegion";
CONST RT_JAIL		:= "Jail";
CONST RT_MOONGATE	:= "Moongate";
CONST RT_VSHRINE	:= "VirtueShrine";

function REGIONS_OpenRegionsCfgFile()

	var cfg_file := ReadCOnfigFile( "::../regions/regions" );
	if( cfg_file.errortext )
		return error{"errortext":="REGIONS_OpenRegionsCfgFile -> Could not find '::../regions/regions' -> "+cfg_file.errortext};
	endif

	return cfg_file;
endfunction

function REGIONS_FindElem( byref region_name, byref cfg_file := 0 )

	var cfg_elem := 0;
	if( !cfg_file )
		cfg_file := REGIONS_OpenRegionsCfgFile();
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	cfg_elem := FindConfigElem( cfg_file, region_name );
	if( cfg_elem.errortext )
		return error{"errortext":="REGIONS_FindElem -> Could not find '"+region_name+"' -> "+cfg_elem.errortext};
	endif

	return cfg_elem;
endfunction

function REGIONS_GetProp( byref object, byref prop_name )

	var cfg_elem := REGIONS_FindElem( GetRegionName( object ) ),
		cfg_prop := 0;

	if( cfg_elem.errortext )
		return cfg_elem.errortext;
	endif

	cfg_prop := GetConfigString( cfg_elem, prop_name );
	if( cfg_prop.errortext )
		cfg_prop := error{"errortext":="Could not find region prop '"+prop_name+"' -> "+cfg_prop.errortext};
	endif

	if( CInt( cfg_prop ) )
		return CInt( cfg_prop );
	endif

	return cfg_prop;
endfunction

function REGIONS_GetPropAtLocation( byref x, byref y, byref realm, byref prop_name )

	var cfg_elem := REGIONS_FindElem( GetRegionNameAtLocation( x, y, realm ) ),
		cfg_prop := 0;

	if( cfg_elem.errortext )
		return cfg_elem.errortext;
	endif

	cfg_prop := GetConfigString( cfg_elem, prop_name );
	if( cfg_prop.errortext )
		cfg_prop := error{"errortext":="Could not find region prop '"+prop_name+"' -> "+cfg_prop.errortext};
	endif

	if( CInt( cfg_prop ) )
		return CInt( cfg_prop );
	endif

	return cfg_prop;
endfunction
