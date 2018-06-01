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
 * MSP_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :mining:settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function MSP_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":mining:settings");

	if ( cfg.errortext )
		SysLog("Error::MSP_GetSettingsCfgFile() - Unable to open [:mining:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * MSP_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by MSP_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function MSP_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := MSP_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::MSP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

