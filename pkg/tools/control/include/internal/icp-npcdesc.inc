function ICP_NPCDescGump( pkg_name, info_elem )

	var gump := GFCreateGump();

	ICP_GumpHeader( gump, NPCDESC_BTN );

	if( info_elem.GetProp( "Main" ).Main )
		pkg_name := "";
	endif

	var npc_cfg := ReadConfigFile( ":"+pkg_name+":npcdesc" );
	if( !npc_cfg )
		GFTextMid( gump, 0, 250, 500, 2100, "There are no NPCs that belong to this package." );
	else
		var elem_names := GetConfigStringKeys( npc_cfg );
		var y_pos := 100;
		var x_pos := 25;
		var x_count := 1;
		var y_count := 1;
		var page_num := 1;
		foreach key in ( elem_names )
			var cfg_elem := npc_cfg[key];
			//var graphic := cfg_elem.ObjType;
			var pic_color := CInt( cfg_elem.Color );
			if( pic_color )
				pic_color -= 1;
			endif
			var name_color := ICP_GetNPCNameColor( cfg_elem.alignment );

			GFTilePic( gump, x_pos, y_pos, 8440, pic_color );
			GFTextCrop( gump, x_pos, y_pos+15, 95, 25, name_color, key );
			GFAddButton( gump, x_pos+5, y_pos+35, 2117, 2118, GF_CLOSE_BTN, _key_iter );

			x_pos += 120;
			x_count += 1;
			if( x_count > 4 )
				x_count := 1;
				y_pos += 80;
				x_pos := 25;
				y_count += 1;
			endif

			if( y_count > 4 )
				y_pos := 100;
				x_pos := 25;
				x_count := 1;
				y_count := 1;
				GFAddButton( gump, 460, 420, 0xA58, 0xA59, GF_PAGE_BTN, page_num+1 );
				page_num += 1;
				GFPage(gump, page_num);
				GFAddButton( gump, 460, 105, 0xA5A, 0xA5B, GF_PAGE_BTN, page_num-1 );
			endif
			SleepMS(2);
		endforeach
	endif

	return gump;
endfunction

function ICP_CreateNPC( who, template, pkg_name )

	var npc_cfg := ReadConfigFile( ":"+pkg_name+":npcdesc" );
	var keys := GetConfigStringKeys( npc_cfg );
	template := keys[template];
	template := ":"+pkg_name+":"+template;

	SendSysMessage( who, "Target where to create the npc ["+template+"].", color := 88 );
	var targ := TargetCoordinates( who );
	if( !targ )
		SendSysMessage( who, "Cancelled." );
		return 0;
	endif

	var npc := CreateNPCFromTemplate( template, targ.x, targ.y, targ.z, 0, who.realm );
	if( !npc )
		SendSysMessage( who, "Error - Unable to create npc ["+template+"] ->"+npc.errortext );
	endif

	return 1;
endfunction

function ICP_GetNPCNameColor( reputation )

	var reputation_cfg := ReadConfigFile( "::repsys" );
	//We know the config is there - core won't start without it.
	var reputation_elem := reputation_cfg["NameColoring"];
	if( !reputation_elem )
		SysLog( "Unable to read in repsys.cfg elem [NameColoring] ->"+reputation_elem.errortext );
		SysLog( "You probably don't have an elem name set for it. Thats naughty!" );
		return 0;
	endif

	case( reputation )
		"good":		return ( GetConfigInt( reputation_elem, "innocent" ) -1 );
		"neutral":	return ( GetConfigInt( reputation_elem, "attackable" ) -1 );
		"evil":		return ( GetConfigInt( reputation_elem, "murderer" ) -1 );
		 // No alignment for an npc is considered neutral
		default: 	return ( GetConfigInt( reputation_elem, "attackable" ) -1 );
	endcase
endfunction
