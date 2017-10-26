/* $Id: settings.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 * Purpose
 * Provide various functions and constants for the shard's behaviour.
 *
 */
use cfgfile;
use os;

/*
 * QueryConfigString(what)
 *
 * Purpose
 * Returns a config string from the config file passed. If no
 * config file, or elem name, is passed, it uses the defaults.
 * shardconfig.cfg is the default config file, Main is the
 * default elem name.
 *
 * Parameters
 * what:	The name of the config elem to return
 * cfg_file:	Optional. Packaged name of the config file.
 * elem_name:	Optional. Elem name to look up in the config.
 *
 * Return value
 * The config file's value for the desired elem.
 *
 */
function QueryConfigString(what, cfg_file, elem_name)
	var ret_value := GetConfigString(FindConfigElem(ReadConfigFile(cfg_file), elem_name), what);
	if( ret_value.errortext )
		SysLog("Error::QueryConfigString() - Unable to open ["+cfg_file+"] ->"+ret_value.errortext);
	endif
	return ret_value;
endfunction


/*
 * QueryConfigInt(what)
 *
 * Returns a config integer from the config file passed. If no
 * config file, or elem name, is passed, it uses the defaults.
 * shardconfig.cfg is the default config file, Main is the
 * default elem name.
 *
 * Parameters
 * what:	The name of the config elem to return
 * cfg_file:	Optional. Packaged name of the config file.
 * elem_name:	Optional. Elem name to look up in the config.
 *
 * Return value
 * The config file's value for the desired elem.
 *
 */
function QueryConfigInt(what, cfg_file, elem_name)
	var ret_value := GetConfigInt(FindConfigElem(ReadConfigFile(cfg_file), elem_name), what);
	if( ret_value.errortext )
		SysLog("Error::QueryConfigInt() - Unable to open ["+cfg_file+"] ->"+ret_value.errortext);
	endif
	return ret_value;
endfunction
