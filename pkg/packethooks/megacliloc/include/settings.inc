use uo;
use os;
use cfgfile;

function MCliloc_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":megacliloc:settings" );
	if( cfg.errortext )
		SysLog( "Error::megacliloc - MCliloc_GetSettingsCfgFile() - Unable to open [:megacliloc:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function MCliloc_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := MCliloc_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::MCliloc_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
