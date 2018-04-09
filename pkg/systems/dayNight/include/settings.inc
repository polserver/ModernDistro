//$Id: settings.inc 697 2011-09-27 20:58:25Z AustinHeilman $

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
 * DAYNIGHT_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :attributes:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function DAYNIGHT_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":daynight:settings");

	if ( cfg.errortext )
		SysLog("Error::DAYNIGHT_GetSettingsCfgFile() - Unable to open [:attributes:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * DAYNIGHT_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by DAYNIGHT_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function DAYNIGHT_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := DAYNIGHT_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::DAYNIGHT_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

