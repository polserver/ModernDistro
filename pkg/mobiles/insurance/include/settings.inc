use uo;
use os;
use cfgfile;

function InsP_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":insurance:settings" );
	if( cfg.errortext )
		SysLog( "Error::Insurance - InsP_GetSettingsCfgFile() - Unable to open [:insurance:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function InsP_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := InsP_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::InsP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
