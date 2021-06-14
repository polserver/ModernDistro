use uo;
use os;
use cfgfile;

function SL_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":serverlist:settings");

	if ( cfg.errortext )
		SysLog("Error::SL_GetSettingsCfgFile() - Unable to open [:settings:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

function SL_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := SL_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::SL_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

function SL_CheckDebug()
	var settings_elem := SL_GetSettingsCfgElem("Settings");
	
	return GetConfigInt(settings_elem, "Debug");
endfunction