/* $Id: settings.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 * Current Version
 * SETTINGS.INC - v1.0
 * Updated 5/10/2006 8:29PM
 *
 * -- Revision v1.0 --
 * Austin:
 *  Created include file
 ===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * ACCT_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :accounts:config/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function ACCT_GetSettingsCfgFile()
	UnloadConfigFile(":accounts:config/settings");
	var cfg := ReadConfigFile(":accounts:config/settings");

	if ( cfg.errortext )
		SysLog("Error::ACCT_GetSettingsCfgFile() - Unable to open [:accounts:config/settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * ACCT_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by ACCT_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function ACCT_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := ACCT_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::ACCT_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

