use uo;
use os;
use cfgfile;

function RC_GetSettingsCfgFile()

	var cfg := ReadConfigFile( ":resourcecore:settings" );
	if( cfg.errortext )
		return error{ "errortext":="Error::RC_GetSettingsCfgFile() - Unable to open [:resourcecore:settings.cfg] ->"+cfg.errortext };
	endif

	return cfg;
endfunction

function RC_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := RC_GetSettingsCfgFile();
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	var elem := FindConfigElem( cfg_file, elem_name );
	if( elem.errortext )
		return error{ "errortext":="Error::RC_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext };
	endif

	return elem;
endfunction