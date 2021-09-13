//$Id: settings.inc 373 2006-06-17 18:27:33Z austinheilman $

/*===============================================================
* Current Version
* SETTINGS.INC - v1.0
* Updated 9/27/2005 2:54PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * DP_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :staticHousing:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function DP_GetSettingsCfgFile()
	UnloadConfigFile(":doors:settings");
	var cfg := ReadConfigFile(":doors:settings");

	if ( cfg.errortext )
		SysLog("Error::DP_GetSettingsCfgFile() - Unable to open [:doors:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * DP_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by HE_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function DP_GetSettingsCfgElem(elem_name, byref cfg_file:=0)

	if ( !cfg_file )
		cfg_file := DP_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::DP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction
