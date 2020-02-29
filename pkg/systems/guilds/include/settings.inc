/*
        Maintened by The POL Distro Team

*/
use uo;
use os;
use cfgfile;

function Guild_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":guilds:settings" );
	if( cfg.errortext )
		SysLog( "Error::Guild_GetSettingsCfgFile() - Unable to open [:guilds:settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function Guild_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := Guild_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];
	if( elem.errortext )
		SysLog( "Error::Guild_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
