/*

	spawnSystem originally created by Austin for the 0.97 Distro

	Bugs fixed by Edwards

	Adapted for the Basic Distro by Yukiko 10-Sep-2015


*/
use uo;
use os;
use cfgfile;

/*
 * SS_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in an engine's settings.cfg or if no engine name is passed, the main settings.cfg.
 *
 * Parameters
 * engine_name:	A string containing the name of an engine.
 *
 * Return value
 * A config file reference.
 *
 */
function SS_GetSettingsCfgFile( engine_name:="" )

	var cfg_path := ":spawnsystem:settings";
	if( engine_name )
		cfg_path := ":spawnsystem:engines/"+engine_name+"/settings";
	endif

	var cfg := ReadConfigFile( ":spawnsystem:settings" );
	if( cfg.errortext )
		SysLog( "Error::SS_GetSettingsCfgFile() - Unable to open "+cfg_path+".cfg ->"+cfg.errortext );
	endif

	return cfg;
endfunction

/*
 * SS_GetSettingsCfgElem(cfg_file, elem_name)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * cfg_file:	A config file reference.
 * elem_name:	A string matching the elem name to be retrieved.
 *
 * Return value
 * A config file elem reference.
 *
 */
function SS_GetSettingsCfgElem( cfg_file, elem_name )

	var cfg := SS_GetSettingsCfgFile();
	var elem := cfg[elem_name];
	if( elem.errortext )
		SysLog( "Error::SS_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] in "+cfg_file.name+" ->"+elem.errortext );
	endif

	return elem;
endfunction
