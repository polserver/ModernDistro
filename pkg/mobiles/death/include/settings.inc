/* $Id: settings.inc 556 2006-07-04 11:33:26Z austinheilman $
 *
 */

/*===============================================================
* Current Version
* SETTINGS.INC - v1.0
* Updated 7/4/2006 4:03AM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;

/*
 * DP_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :death:settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function DP_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":death:settings");

	if ( cfg.errortext )
		SysLog("Error::DP_GetSettingsCfgFile() - Unable to open [:death:settings.cfg] ->"+cfg.errortext);
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
 * cfg_file:	Optional parameter - reference to a config already read in by DP_GetSettingsCfgFile()
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
