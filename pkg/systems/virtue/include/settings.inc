//$Id: settings.inc 373 2006-06-17 18:27:33Z austinheilman $

use uo;
use os;
use basic;
use cfgfile;

/*
 * VS_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :virtue:config/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function VS_GetSettingsCfgFile()
	var cfg := ReadConfigFile(":virtue:settings");

	if ( cfg.errortext )
		SysLog("Error::VS_GetSettingsCfgFile() - Unable to open [:virtue:settings.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * VS_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by VS_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function VS_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := VS_GetSettingsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::VS_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

/*
 *
 * VS_Capitalize(word)
 *
 * Purpose
 * Capitalizes the first letter of the word passed to it.
 * Used mostly to capitalize the first letter of a shrine's
 * name when retrieving shrine messages.
 * Yes. I admit this function could be made unnecessary
 * if I did not insist on capitalizing the first letter of
 * a shrine's name in the settings.cfg file's shrine message
 * entries, eg. VirtueShrineMsg but hey, I refactored this
 * package from a different shard. So to quote Frank Sinatra,
 * "I did it my way". :)
 *
 * Parameter: a string - the word to capitalize.
 *
 * Returns: word capitalized or "-Error" if the passed string was empty.
 *
*/
function VS_Capitalize(word)

	if(Len(word < 1))
		SysLog("Error::VS_Capitalize() - Cannot capitalize an empty string.");
		return "-Error";
	endif
	word[1] := Upper(word[1]);
	return word;
endfunction

/*
 *
 * VS_GetSettingsCfgString(key, elem_name)
 *
 * Purpose
 * Retrieves a string from a cfg recotd.
 *
 * Parameters
 * key:			A string matching the record name where the string is found.
 *              eg. VirtueShrineMsg
 * elem_name:	The element name in the record where the string to be retrieved is.
 *              eg. Pos4
 *
 * Return value
 * The string associated with elem_name.
 *
 */
function VS_GetSettingsCfgString(key, elem_name)
	var cfg_file := VS_GetSettingsCfgFile();
	var record := FindConfigElem( cfg_file, key );
	var strng := GetConfigString( record, elem_name );

	if ( record.errortext )
		SysLog("Error::VS_GetSettingsCfgElem() - Unable to find elem ["+key+"] ->"+record.errortext);
	endif

	return strng;
endfunction
