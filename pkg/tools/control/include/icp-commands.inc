/* $Id: icp-commands.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
function ICP_CommandsGump(pkg_name)
	var pkg_elem := DFFindElement(pkg_info, pkg_name);
	var cmd_list := pkg_elem.GetProp("TextCommands");
	if ( !cmd_list )
		cmd_list := array{};
	endif

	var gump := GFCreateGump();
	ICP_GumpHeader(gump, COMMANDS_BTN);

	var y_pos := 80;
	var counter := 0;
	var btn_num := 0;
	var page_num := 1;
	if ( cmd_list.Size() > 0 )
		foreach script in cmd_list
			btn_num := btn_num+1;
			counter := counter+1;
			y_pos := y_pos+20;
			ICP_AddCmdLine(gump, script, btn_num, y_pos, counter, page_num);
			SleepMS(2);
		endforeach
	else
		GFTextMid(gump, 0, 250, 500, 2100, "There are no text commands listed for this package.");
	endif

	return gump;
endfunction

function ICP_AddCmdLine(byref gump, script, btn_num, byref y_pos, byref counter, byref page_num)
	var temp := ICP_ParseScript(script);
//	cmd_level commented out to fix unused variable.
//	Left in place, in case needed at a later date.
//	var cmd_level := temp.cmdlevel;
	var path := temp.script_path;
	var cmd_name := ICP_CommandName(path);

	GFAddButton(gump, 25, y_pos+3, 2117, 2118, GF_CLOSE_BTN, btn_num);
	GFTextLine(gump, 45, y_pos, 2100, path+"  (."+cmd_name+")");

	//Check if we need to set it up to make a new page
	if ( counter >= 16 )
		//Down arrow
		GFAddButton(gump, 460, 420, 0xA58, 0xA59, GF_PAGE_BTN, page_num+1);
		page_num := page_num+1;
		GFPage(gump, page_num);
		//Up arrow
		GFAddButton(gump, 460, 105, 0xA5A, 0xA5B, GF_PAGE_BTN, page_num-1);

		y_pos := 80;
		counter := 0;
	endif
endfunction

function ICP_CommandName(path)
	if ( !path["/"] )
		while ( path[":"] )
			var slash_pos := Find(path, ":", 1);
			path := path[slash_pos+1, Len(path)];

			SleepMS(2);
		endwhile
	else
		while ( path["/"] )
			var slash_pos := Find(path, "/", 1);
			path := path[slash_pos+1, Len(path)];

			SleepMS(2);
		endwhile
	endif
	return path;
endfunction

function ICP_StartCommand(who, input, pkg_elem)
	var script_list := pkg_elem.GetProp("TextCommands");
	var script_line := script_list[input];
	var temp := ICP_ParseScript(script_line);

	if ( CInt(temp.cmdlevel) > who.cmdlevel )
		GumpPrompt(who, "Command level is not high enough to use command.");
		return 0;
	else
		var script := Start_Script(temp.script_path, who);
		if ( script.errortext )
			GumpPrompt(who, "Error starting script [P] "+script.errortext);
		else
			GumpPrompt(who, "Started [P] "+temp.script_path);
		endif

		return script;
	endif
endfunction