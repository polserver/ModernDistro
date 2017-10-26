use uo;
use os;
use cfgfile;

include ":datafile:datafile";

/*
 * GetCmdHelpText(path)
 *
 * Purpose
 * Retrieves the help text for a text command.
 *
 * Parameters
 * path:	Path to the script. 
 *		If not set, will find the path of the script that calls the function.
 * 
 * Return value
 * Returns a string
 *
 */
function GetCmdHelpText(path:="")
	if ( !path )
		path := GetProcess(GetPid()).name;
	endif
	
	// Attempt to find the text from cache first
	var help_text := GetCmdHelpTextFromCache(path);
	if ( help_text )
		return help_text;
	endif
	
	// Get all config/cmdhelp.cfg files and search them.
	var cfg := ReadConfigFile(":*:config/cmdhelp");
	if ( !cfg )
		return cfg;
	endif
	var cfg_elem := cfg[path];
	if ( !cfg )
		return cfg_elem;
	endif
	
	// Get the "Text" lines from the elem.
	help_text := "";
	foreach entry in ( GetConfigStringArray(cfg_elem, "Text") )
		help_text := help_text + entry;
		SleepMS(2);
	endforeach
	// Cache the text for faster retrieval next time.
	SetCmdHelpTextCache(path, help_text);
	
	return help_text;
endfunction

/*
 * GetCmdHelpTextFromCache(path)
 *
 * Purpose
 * Retrieves the cached help text for a text command.
 *
 * Parameters
 * path:	Path to the script. 
 *		If not set, will find the path of the script that calls the function.
 * 
 * Return value
 * Returns a string
 *
 */
function GetCmdHelpTextFromCache(path:="")
	if ( !path )
		path := GetProcess(GetPid()).name;
	endif
	
	var data_file := DFOpenDataFile(":commands:helptext", DF_CREATE);
	var data_elem := DFFindElement(data_file, path, DF_NO_CREATE);
	return data_elem.GetProp("HelpText");
endfunction

/*
 * SetCmdHelpTextCache(path)
 *
 * Purpose
 * Saves the help text for a text command.
 *
 * Parameters
 * path:	Path to the script. 
 * text:	Text to cache.
 * 
 * Return value
 * 1 on success
 * error on failure
 *
 */
function SetCmdHelpTextCache(path, text)
	var data_file := DFOpenDataFile(":commands:helptext", DF_CREATE);
	var data_elem := DFFindElement(data_file, path, DF_NO_CREATE);
	
	return data_elem.SetProp("HelpText", text);
endfunction

/*
 * RemoveCmdHelpTextCache(path)
 *
 * Purpose
 * Removes the cached help text for a text command.
 *
 * Parameters
 * path:	Path to the script. 
 * 
 * Return value
 * Returns a string
 *
 */
function RemoveCmdHelpTextCache(path)
	var data_file := DFOpenDataFile(":commands:helptext", DF_CREATE);
	return data_file.DeleteElement(path);
endfunction
	