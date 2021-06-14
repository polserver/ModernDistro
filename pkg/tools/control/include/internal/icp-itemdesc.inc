include ":itemutils:itemdesc";

function ICP_ItemDescGump( pkg_name, info_elem )

	var gump := GFCreateGump();

	ICP_GumpHeader( gump, ITEMDESC_BTN );

	if( info_elem.GetProp( "Main" ).Main )
		pkg_name := "";
	endif

	var item_cfg := ReadConfigFile( ":"+pkg_name+":itemdesc" );
	if( !item_cfg )
		GFTextMid( gump, 0, 250, 500, 2100, "There are no items that belong to this package." );
	else
		var elem_names := GetConfigStringKeys( item_cfg );
		var y_pos := 100;
		var x_pos := 25;
		var x_count := 1;
		var y_count := 1;
		var page_num := 1;
		foreach key in ( elem_names )
			var cfg_elem := item_cfg[key];
			var desc := GetObjTypeDesc( CInt( key ));
			if( !desc )
				desc := cfg_elem.name;
			endif
			var graphic := cfg_elem.Graphic;
			if( !graphic )
				graphic := key;
			endif
			var color := CInt(cfg_elem.Color);
			if( color )
				color := color-1;
			endif

			GFTilePic( gump, x_pos, y_pos, graphic, color );
			GFTextCrop( gump, x_pos, y_pos+15, 95, 25, 2100, desc );
			GFAddButton( gump, x_pos+5, y_pos+35, 2117, 2118, GF_CLOSE_BTN, CInt( key ));

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

	UnloadConfigFile( ":"+pkg_name+":itemdesc" );

	return gump;
endfunction

function ICP_CreateItem( who, object )

	var question := "Where do you want to place the item you're about to create?";
	var yes := "My backpack.";
	var no := "At my feet.";

	var create_method := YesNo( who, question, yes, no );
	var item := 0;

	if( create_method )
		item := CreateItemInBackPack( who, object, 1 );
	else
		item := CreateItemAtLocation( who.x, who.y, who.z, object, 1, who.realm );
	endif

	if( !item )
		SendSysMessage( who, "Error - Unable to create ["+object+"] ->"+item.errortext );
	endif
endfunction
