include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":datafile:datafile";

// Button IDs
CONST COLLAPSE_BTN	:= 0xA00;
CONST EXPAND_BTN	:= 0xA01;
CONST CMDLIST_BTN	:= 0xA02;
CONST REFRESH_BTN	:= 0xA03;
CONST CMDLVLS_BTN	:= 0xA04;
CONST SETTINGS_BTN	:= 0xA05;
CONST APPLY_BTN		:= 0xA06;
// Check boxes
CONST TRANSPARENT_CHK	:= 0xA07;
CONST MOVABLE_CHK	:= 0xA08;
CONST LOGIN_CHK		:= 0xA09;
// Button ID ranges
CONST PKG_START		:= 0xB00;
CONST LVL_START		:= 0xC00;
CONST CMD_START		:= 0xD00;

// 'Page' flipping buttons
CONST PKG_PAGE_START	:= 0xA000;
CONST SCRIPT_PAGE_START	:= 0xB000;

CONST TRANSPARENT	 := 1;
CONST CLOSE_BTN		:= 0x01A;

function ICP_ExpandedMenuBar( transparency:=0 )

	var gump := GFCreateGump();
 
        GFClosable( gump, 0 );
	GFResizePic( gump, 0, 0, GFGetCfgConst( "BackGrounds", "STONESLAB" ), 640, 30 );
		
	GFAddButton( gump, 35, 4, 0x98D, 0x98D, GF_CLOSE_BTN, CMDLIST_BTN );
	GFAddButton( gump, 150, 4, 0x98D, 0x98D, GF_CLOSE_BTN, CMDLVLS_BTN );
	GFAddButton( gump, 500, 4, 0x98D, 0x98D, GF_CLOSE_BTN, SETTINGS_BTN );
	
	if( transparency )
		GFAddAlphaRegion( gump, 0, 0, 640, 30 );
	endif
	
	GFAddButton( gump, 5, 5, 0x15A4, 0x15A4, GF_CLOSE_BTN, COLLAPSE_BTN );
	
	GFTextLine( gump, 57, 5, 2100, "Commands" );
	GFTextLine( gump, 182, 5, 2100, "Levels" );
	GFTextLine( gump, 528, 5, 2100, "Settings" );
	
	return gump;
endfunction
