function ICP_ScriptsGump( pkg_name )

	var pkg_elem := DFFindElement( pkg_info, pkg_name );
	var script_list := pkg_elem.GetProp( "Scripts" );
	if( !script_list )
		script_list := array{};
	endif

	var gump := GFCreateGump();
	ICP_GumpHeader( gump, SCRIPTS_BTN );

	var y_pos := 80;
	var counter := 0;
	var btn_num := 0;
	var page_num := 1;
	if( script_list.Size() > 0 )
		foreach script in ( script_list )
			btn_num += 1;
			counter += 1;
			y_pos += 20;
			ICP_AddScriptLine( gump, script, btn_num, y_pos, counter, page_num );
			SleepMS(2);
		endforeach
	else
		GFTextMid( gump, 0, 250, 500, 2100, "There are no scripts listed for this package." );
	endif

	return gump;
endfunction

function ICP_AddScriptLine( byref gump, script, btn_num, byref y_pos, byref counter, byref page_num )

	var temp := ICP_ParseScript( script );
	var path := temp.script_path;

	GFTextLine( gump, 45, y_pos, 2100, path );
	GFAddButton( gump, 25, y_pos+3, 2117, 2118, GF_CLOSE_BTN, btn_num );

	if( counter >= 16 )
		//Down arrow
		GFAddButton( gump, 460, 420, 0xA58, 0xA59, GF_PAGE_BTN, page_num+1 );
		page_num := page_num+1;
		GFPage( gump, page_num );
		//Up arrow
		GFAddButton( gump, 460, 105, 0xA5A, 0xA5B, GF_PAGE_BTN, page_num-1 );

		y_pos := 80;
		counter := 0;
	endif
endfunction

function ICP_StartScript( who, input, pkg_elem )

	var script_list := pkg_elem.GetProp( "Scripts" );
	var script_line := script_list[input];
	var temp := ICP_ParseScript( script_line );

	if( CInt( temp.cmdlevel ) > who.cmdlevel )
		GumpPrompt( who, "Command level is not high enough to start script." );
		return 0;
	else
		var script := Start_Script( temp.script_path, who );
		if( script.errortext )
			GumpPrompt( who, "Error starting script [P] "+script.errortext );
		else
			GumpPrompt( who, "Started [P] "+temp.script_path );
		endif

		return script;
	endif
endfunction
