/*
        Maintened by The POL Distro Team

        2022-01-12

        Last Modifications 2009-11-16
             * Added server time with page time so we staff arround the world know the delay...
*/
use uo;
use os;
use util;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":gumps:gumpPrompt";
include ":help:help";

CONST CLOSE_BTN		:= 0xA00;
CONST QUEUE_TYPES_BTN	:= 0xA01;
CONST PAGE_LIST_BTN	:= 0xA02;
CONST SHOW_PAGE_BTN	:= 0xA03;
CONST DELETE_PAGE_BTN	:= 0xA04;
CONST GOTO_BTN		:= 0xA06;
CONST SUMMON_BTN	:= 0xA07;
CONST EMAIL_BTN		:= 0xA0A;
CONST ACCTNOTE_BTN      := 0xA0B;
CONST INFO_BTN          := 0XA0C;

CONST SHOW_QUEUE_START	:= 0xA000;
CONST SHOW_PAGE_START	:= 0xB000;

function BuildGumpTemplate()

	var gump := LoadHelpInfo( "QueueTemplate", "Gump" );
	if( gump )
		return gump;
	endif

	gump := GFCreateGump();
	GFResizePic( gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 540, 450 );
	GFResizePic( gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 510, 25 );
	GFTextMid( gump, 15, 20, 570, 2100, "Help Queue" );

	// Left bar
	GFResizePic( gump, 15, 45, GFGetCfgConst( "Defaults", "ForeGround" ), 125, 325 );

	// Content Area
	GFResizePic( gump, 145, 45, GFGetCfgConst( "Defaults", "ForeGround" ), 380, 325 );

	// Bar along the bottom
	GFResizePic( gump, 15, 375, GFGetCfgConst( "Defaults", "ForeGround" ), 510, 60 );

	return gump;
endfunction

function ShowQueueTypes( cmdlevel )

	var gump := BuildGumpTemplate();
	var y_pos := 55;
	foreach level in ( GetQueueNamesAccess( cmdlevel ))
		GFAddButton( gump, 27, y_pos+3, 2117, 2118, GF_CLOSE_BTN, SHOW_QUEUE_START+_level_iter );
		GFTextLine( gump, 47, y_pos, 2100, level );
		y_pos += 20;
		SleepMS(2);
	endforeach

	return gump;
endfunction

function ShowPagesInQueue( queue_name, cmd_level )

	var gump := ShowQueueTypes( cmd_level );

	var data_file := GetHelpDataFile();
	var data_elem := DFFindElement( data_file, "Queue-"+queue_name, DF_NO_CREATE );

	var y_pos := 55;
	var prop_names := data_elem.PropNames();
	var counter := 0, num_pages := prop_names.Size();

	GFPage(gump, 1);
	foreach prop_name in ( prop_names )
		var page_info := data_elem.GetProp( prop_name );
		var mobile := SystemFindObjectBySerial( CInt( prop_name ));

		var status := "[online]";
		if( !mobile.connected )
			status := "[offline]";
		endif

		GFAddButton( gump, 160, y_pos+3, 2117, 2118, GF_CLOSE_BTN, SHOW_PAGE_START+_prop_name_iter );
		GFTextLine( gump, 180, y_pos, 2100, mobile.name+" "+status );
		var summary;
		if( Len( page_info.message ) > 38 )
			summary := page_info.message[1, 38]+"[...]";
		else
			summary := page_info.message;
		endif

		GFTextLine( gump, 180, y_pos+20, 2100, summary );

		counter += 1;
		y_pos += 45;

		if( counter >= 7 && _prop_name_iter < num_pages )
			GFAddButton( gump, 510, 345, 0xA92, 0xA93, GF_PAGE_BTN, gump.cur_page+1 );
			GFPage( gump, gump.cur_page+1);
			GFAddButton( gump, 510, 50, 0xA90, 0xA91, GF_PAGE_BTN, gump.cur_page-1 );
			counter := 0;
			y_pos := 50;
		endif

		SleepMS(2);
	endforeach

	return gump;
endfunction

function ShowPageInQueue( staff, queue_name, page_id, cmdlevel )

	var data_file := GetHelpDataFile(),
	    data_elem := DFFindElement( data_file, "Queue-"+queue_name, DF_NO_CREATE ),
            list_elem := DFGetElemProps( data_elem ),
	    elem_info := list_elem[page_id],
	    page_info := data_elem.GetProp( elem_info ),
	    mobile := SystemFindObjectBySerial( CInt( elem_info )),
	    gump := ShowQueueTypes( cmdlevel );

	var status := "[online]";
	if( !mobile.connected )
		status := "[offline]";
	endif

	GFTextLine( gump, 160, 55, 2100, "Page From: "+mobile.name+" "+status );

        var page_time := StrFormatTime( "%m-%d-%Y %I:%M:%S %p", CInt( page_info.time ));
        if( page_time )
	       GFTextLine( gump, 160, 75, 2100, "Page Time: "+page_time );
        endif

        var server_time := StrFormatTime( "%m-%d-%Y %I:%M:%S %p", CInt( POLCore().systime ));
        if( server_time )
	       GFTextLine( gump, 160, 95, 1720, "Server Time: "+server_time );
        endif

	GFResizePic( gump, 160, 120, GFGetCfgConst( "BackGrounds", "GoldTrim" ), 355, 230 );
	var y_pos := 125;
	foreach line in ( GFWordWrap( page_info.message, 370 ))
		GFTextLine( gump, 165, y_pos, 2100, line );
		y_pos += 20;
		SleepMS(2);
	endforeach

	var btn_type := GF_DUMMY_BTN;
	if( cmdlevel >= 3 )
		btn_type := GF_CLOSE_BTN;
	endif

	GFAddButton( gump, 30, 387, 2117, 2118, btn_type, GOTO_BTN );
	GFTextLine( gump, 50, 385, 2100, "Goto" );

        if( staff.cmdlevel >= 3 )
		GFAddButton( gump, 30, 409, 2117, 2118, btn_type, SUMMON_BTN );
		GFTextLine( gump, 50, 405, 2100, "Summon" );

		GFAddButton( gump, 160, 409, 2117, 2118, btn_type, INFO_BTN );
		GFTextLine( gump, 180, 405, 2100, "Info" );
        endif

	GFAddButton( gump, 160, 387, 2117, 2118, btn_type, ACCTNOTE_BTN );
	GFTextLine( gump, 180, 385, 2100, "Acct Info" );

	GFAddButton( gump, 290, 385, 2117, 2118, GF_CLOSE_BTN, EMAIL_BTN );
	GFTextLine( gump, 310, 382, 2100, "Send Message" );
	GFAddButton( gump, 290, 409, 2117, 2118, GF_CLOSE_BTN, DELETE_PAGE_BTN );
	GFTextLine( gump, 310, 405, 2100, "Delete Page" );

	return gump;
endfunction
