use uo;
use os;
use cfgfile;

function Gems_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":gems:settings" );
	if( cfg.errortext )
		SysLog( "Error::Gems_GetSettingsCfgFile() - Unable to open [:gems:settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function Gems_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := Gems_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if( elem.errortext )
		SysLog( "Error::Gems_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
