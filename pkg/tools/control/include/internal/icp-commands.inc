function ICP_CommandsGump( pkg_name, pkg_elem )

	var cmd_list := pkg_elem.GetProp( "TextCommands" );
	if( !cmd_list )
		cmd_list := array{};
	endif

	if( pkg_elem.GetProp( "Main" ).Main )
		pkg_name := "";
	endif

	var gump := GFCreateGump();
 
	ICP_GumpHeader( gump, COMMANDS_BTN );

	var y_pos := 80;
	var counter := 0;
	var btn_num := 0;
	if( cmd_list.Size() > 0 )
		foreach level in ( cmd_list )
			foreach script in ( level )
				btn_num += 1;
				counter += 1;
				y_pos += 20;
				ICP_AddCmdLine( gump, script, btn_num, y_pos, counter );
				SleepMS(2);
			endforeach
			SleepMS(2);
		endforeach
	else
		GFTextMid( gump, 0, 250, 500, 2100, "There are no text commands listed for this package." );
	endif

	return gump;
endfunction

function ICP_AddCmdLine( byref gump, script, btn_num, byref y_pos, byref counter )

	var script_name := ICP_GetScriptName( script.script );
	var script_path := script.dir+script.script;
 
	GFAddButton( gump, 25, y_pos+3, 2117, 2118, GF_CLOSE_BTN, btn_num );
	GFTextLine( gump, 45, y_pos, 2100, script_path+"  (."+script_name+")" );

	//Check if we need to set it up to make a new page
	if( counter >= 16 )
		//Down arrow
		GFAddButton( gump, 460, 420, 0xA58, 0xA59, GF_PAGE_BTN, gump.cur_page+1 );
		GFPage( gump, gump.cur_page+1 );
		//Up arrow
		GFAddButton( gump, 460, 105, 0xA5A, 0xA5B, GF_PAGE_BTN, gump.cur_page-1 );

		y_pos := 80;
		counter := 0;
	endif

	return 1;
endfunction

function ICP_StartCommand( who, input, pkg_elem, pkg_name )

	var script_list := pkg_elem.GetProp( "TextCommands" );

	if( pkg_elem.GetProp( "Main" ).Main )
		pkg_name := "";
	endif

	var script;
	foreach level in ( script_list )
		var num_scripts := level.Size();
		if( input > num_scripts )
			input -= num_scripts;
			continue;
		else
			script := level[input];
			break;
		endif
                SleepMS(2);
	endforeach

	if( !script )
		GumpPrompt( who, "Command level is not high enough to use that command." );
		return 0;
	endif

	var path := ICP_FormatScriptPath( script, pkg_name );
	script := Start_Script( path, who );
	if( script.errortext )
		GumpPrompt( who, "Error starting "+path+" [P] "+script.errortext );
	else
		GumpPrompt( who, "Started [P] "+path );
	endif

	return script;
endfunction
