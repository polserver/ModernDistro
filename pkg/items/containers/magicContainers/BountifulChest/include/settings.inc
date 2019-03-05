use uo;
use os;
use cfgfile;

function BC_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":BountifulChest:settings" );
	if( cfg.errortext )
		SysLog( "Error::BC_GetSettingsCfgFile() - Unable to open [:BountifulChest:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function BC_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := BC_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::BC_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
