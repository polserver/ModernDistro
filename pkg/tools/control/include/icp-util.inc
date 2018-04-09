
/* 
 * 
 * 
 * 
 */
function ICP_ParseScript(script)
	script := SplitWords(script);
	var cmd_level := 0;
	var script_path := script[1];
	if ( script.Size() > 1 )
		cmd_level := CInt(script[1]);
		script_path := CStr(script[2]);
	endif
	
	var temp := struct;
	temp.+cmdlevel := cmd_level;
	temp.+script_path := script_path;
	return temp;
endfunction
