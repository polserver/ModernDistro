use uo;
use os;
use cfgfile;

function MN_GetSettingsCfgFile()

	var cfg_file := ReadConfigFile( ":merchantnodes:settings" );
	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		return error{ "errortext":="MN_GetSettingsCfgFile -> "+cfg_file.errortext };
	endif

	return cfg_file;
endfunction

function MN_FindSettingsElem( byref elem_name, byref cfg_file:=0 )

	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := MN_GetSettingsCfgFile();
		if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
			return error{ "errortext":="MN_FindSettingsElem -> MN_GetSettingsCfgFile -> "+cfg_file.errortext };
		endif
	endif

	var cfg_elem := FindConfigElem( cfg_file, elem_name );
	if( TypeOfInt( cfg_elem ) != OT_CONFIGELEMREF )
		return error{ "errortext":="MN_FindSettingsElem -> "+cfg_elem.errortext };
	endif

	return cfg_elem;
endfunction
