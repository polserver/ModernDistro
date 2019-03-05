use uo;
use os;
use cfgfile;

function FoL_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":FountainOfLife:settings" );
	if( cfg.errortext )
		SysLog( "Error::FoL_GetSettingsCfgFile() - Unable to open [:FountainOfLife:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function FoL_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := FoL_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::FoL_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
