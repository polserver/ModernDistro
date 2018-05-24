
/* 
 * Purpose
 * Takes in a string and breaks it into a command level and script path.
 * Example: 2 :pkg:path
 * 
 * Parameters
 * script:	The string that will be parsed.
 *
 * Return Value:
 * Returns a struct with .cmdlevel and .script_path
 * 
 */
function ICP_ParseScript(script)
	script := SplitWords(script);
	var cmd_level := 0;
	var script_path := script[1];
	if ( script.Size() > 1 )
		cmd_level := script[1];
		script_path := CStr(script[2]);
	endif
	
	var temp := struct;
	temp.+cmdlevel := cmd_level;
	temp.+script_path := script_path;
	return temp;
endfunction

/* 
 * Purpose
 * Removes .ecl from the end of a script name.
 * 
 * Parameters
 * script_name:	Name of the script to parse.
 *
 * Return Value
 * Returns a string.
 * 
 */
function ICP_GetScriptName(script_name)
	if ( script_name[".ecl"] )
		script_name := script_name[1, Len(script_name)-4];
	endif
	
	return script_name;
endfunction

/*
 * Purpose
 * Sets up the path to use a script struct with StartScript
 *
 * Parameters
 * script:	Script struct from ListTextCommands()
 *
 * Return Value
 * Returns a string
 *
 */
function ICP_FormatScriptPath(script, pkg_name)
	var name := "";
	if ( pkg_name == "" )
		// :: makes it want to do pol/scripts/...
		// So a "../" is thrown in to take it to the root.
		name := "::../"+script.dir+script.script;
	else
		name := ":"+pkg_name+":"+script.dir+script.script;
	endif
	
	return name;
endfunction

/*
 *
 */
function ICP_SaveUserSetting(who, setting, value)
	var data_file := DFOpenDataFile(":control:usrsettings", DF_CREATE);
	var data_elem := DFFindElement(data_file, Hex(who.serial), DF_CREATE);

	return data_elem.SetProp(setting, value);
endfunction

/*
 *
 */
function ICP_LoadUserSetting(who, setting)
	var data_file := DFOpenDataFile(":control:usrsettings", DF_CREATE);
	var data_elem := DFFindElement(data_file, Hex(who.serial));

	return data_elem.GetProp(setting);
endfunction