use uo;
use os;
use cfgfile;

function CP_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":containers:settings" );
	if( cfg.errortext )
		SysLog( "Error::CP_GetSettingsCfgFile() - Unable to open [:containers:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function CP_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := CP_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::CP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
