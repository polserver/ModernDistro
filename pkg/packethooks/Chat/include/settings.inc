use uo;
use os;
use cfgfile;

function CH_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":chat:settings");

	if ( cfg.errortext )
		SysLog("Error::CH_GetSettingsCfgFile() - Unable to open [:settings:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

function CH_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := CH_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::CH_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

function CH_CheckDebug()
	var settings_elem := CH_GetSettingsCfgElem("Settings");
	
	return GetConfigInt(settings_elem, "Debug");
endfunction