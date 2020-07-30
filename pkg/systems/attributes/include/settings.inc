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
 * AP_GetSettingsCfgFile(engine_name)
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
function AP_GetSettingsCfgFile()

	var att_cfg := ReadConfigFile( ":attributes:settings" );
	if( att_cfg.errortext )
		SysLog( "Error::AP_GetSettingsCfgFile() - Unable to open [:attributes:settings.cfg] ->" + att_cfg.errortext );
	endif

	return att_cfg;
endfunction

/*
 * AP_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by AP_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function AP_GetSettingsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := AP_GetSettingsCfgFile();
	endif

	var cfg_elem := cfg_file[elem_name];
	if( cfg_elem.errortext )
		SysLog( "Error::AP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->" + cfg_elem.errortext );
	endif

	return cfg_elem;
endfunction
