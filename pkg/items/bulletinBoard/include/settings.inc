use uo;
use os;
use cfgfile;

function BB_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":bulletinboard:config/settings" );
	if( cfg.errortext )
		SysLog( "Error::BB_GetSettingsCfgFile() - Unable to open [:bulletinboard:config/settings] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function BB_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := BB_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];
	if( elem.errortext )
		SysLog( "Error::BB_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction