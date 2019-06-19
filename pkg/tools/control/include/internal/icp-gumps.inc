var BKGND_SILVERTRIM := GFGetCfgConst( "Defaults", "BackGround" );
var BKGND_DARK_STONE := GFGetCfgConst( "Defaults", "ForeGround" );

function ICP_RetrieveGump( elem_name, prop_name:="main" )

	var elem := DFFindElement( pkg_gumps, elem_name );
	return( elem.GetProp( prop_name ));
endfunction

function ICP_SaveGump( elem_name, gump, prop_name:="main" )

	var elem := DFFindElement( pkg_gumps, elem_name, DF_CREATE );
	return( elem.SetProp( prop_name, gump ));
endfunction

function ICP_GumpHeader( byref gump, cur_menu )

	GFResizePic( gump, 0, 0, BKGND_SILVERTRIM, 500, 480 );
	GFResizePic( gump, 15, 15, BKGND_DARK_STONE, 470, 75 );
	GFResizePic( gump, 15, 95, BKGND_DARK_STONE, 470, 368 );
	
	GFTextLine( gump, 30, 20, 2100, "General" );
	if( cur_menu == GENERAL_BTN )
		GFGumpPic( gump, 41, 43, 2154 );
	else
		GFAddButton( gump, 41, 43, 2151, 2154, GF_CLOSE_BTN, GENERAL_BTN );
	endif

	GFTextLine( gump, 105, 20, 2100, "Scripts" );
	if( cur_menu == SCRIPTS_BTN )
		GFGumpPic( gump, 117, 43, 2154 );
	else
		GFAddButton( gump, 117, 43, 2151, 2154, GF_CLOSE_BTN, SCRIPTS_BTN );
	endif
	
	GFTextLine( gump, 190, 20, 2100, "Commands" );
	if( cur_menu == COMMANDS_BTN )
		GFGumpPic( gump, 208, 43, 2154 );
	else
		GFAddButton( gump, 208, 43, 2151, 2154, GF_CLOSE_BTN, COMMANDS_BTN );
	endif
	
	GFTextLine( gump, 290, 20, 2100, "Itemdesc.cfg" );
	if( cur_menu == ITEMDESC_BTN )
		GFGumpPic( gump, 315, 43, 2154 );
	else
		GFAddButton( gump, 315, 43, 2151, 2154, GF_CLOSE_BTN, ITEMDESC_BTN );
	endif
	
	GFTextLine( gump, 390, 20, 2100, "NPCdesc.cfg" );
	if( cur_menu == NPCDESC_BTN )
		GFGumpPic( gump, 415, 43, 2154 );
	else
		GFAddButton( gump, 415, 43, 2151, 2154, GF_CLOSE_BTN, NPCDESC_BTN );
	endif
	
	//Bottom
	GFAddButton( gump, 25, 435, 4014, 4015, GF_CLOSE_BTN, BACK_BTN );
	GFTextLine( gump, 60, 437, 2100, "Back to package list" );
	
	GFAddButton( gump, 330, 435, 4002, 4003, GF_CLOSE_BTN, UNCACHE_BTN );
	GFTextLine( gump, 365, 437, 2100, "Uncache gumps" );
	
	GFPage( gump, 1 );
endfunction
