//$Id: settings.inc 375 2006-06-17 19:26:32Z austinheilman $

/*===============================================================
* Current Version
* SETTINGS.INC - v1.0
* Updated 5/12/2006 2:29PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * TIME_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :worldsaver:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function TIME_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":timeutils:config/settings");

	if ( cfg.errortext )
		SysLog("Error::TIME_GetSettingsCfgFile() - Unable to open [:timeutils:config/settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * TIME_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by TIME_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function TIME_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := TIME_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::TIME_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction
