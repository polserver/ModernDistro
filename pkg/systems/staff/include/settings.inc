use uo;
use os;
use cfgfile;

function STAFF_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":staff:settings" );

	if( cfg.errortext )
		SysLog( "Error::STAFF_GetSettingsCfgFile() - Unable to open [:automacrocheck:settings.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function STAFF_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := STAFF_GetSettingsCfgFile();
	endif

	var elem := FindConfigElem( cfg_file, elem_name );
	if( elem.errortext )
		SysLog( "Error::STAFF_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction
