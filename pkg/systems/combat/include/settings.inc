/* 
 * $Id: settings.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 */

use uo;
use os;
use cfgfile;

/*
 * CS_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :combat:settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function CS_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":combat:settings");

	if ( cfg.errortext )
		SysLog("Error::CS_GetSettingsCfgFile() - Unable to open [:combat:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * CS_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by CS_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function CS_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := CS_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::CS_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction