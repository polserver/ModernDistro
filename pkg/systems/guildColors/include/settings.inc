use uo;
use os;
use cfgfile;

function GC_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":guildcolors:config/settings" );
	if( cfg.errortext )
		SysLog( "Error::GC_GetSettingsCfgFile() - Unable to open [:guildcolors:config/settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function GC_GetSettingsCfgElem( byref elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := GC_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];
	if( elem.errortext )
		SysLog( "Error::GC_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
