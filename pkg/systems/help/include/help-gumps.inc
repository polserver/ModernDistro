/*
        Maintened by The POL Distro Team

        2022-01-12

        Last Modifications 2009-12-28
             * Changed text presentation for intro
             * Removed DrawWrappedText() and replaced it for GFDrawWrappedText() in gumps.inc
*/
use os;
use file;

include ":datafile:datafile";
include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":help:help";
include ":moongates:moongate";
include ":timeutils:time";
include "include/arrays";
include "include/shardInfo";

CONST CLOSE_BTN			:= 0xA00;
CONST MAIN_PAGE_BTN		:= 0xA01;
CONST STUCK_PAGE_BTN	        := 0xA02;
CONST STAFF_PAGE_BTN		:= 0xA03;
CONST SEND_PAGE_BTN		:= 0xA04;
CONST HELP_TOPICS_BTN		:= 0xA05;
CONST WORD_WRAP_BTN		:= 0xA06;

CONST UNCACHE_GUMPS_BTN		:= 0xA07;

CONST HELP_TOPIC_START		:= 0xA001;
CONST PAGE_TXT_START		:= 0xB001;
CONST PAGE_TXT_END		:= 0xB00D;

var g_datafile := DFOpenDataFile( ":jailKeeper:players" );

function BuildGumpTemplate()

	var gump := LoadHelpInfo( "Template", "Gump" );
	if( gump )
		return gump;
	endif

	gump := GFCreateGump();

        GFClosable( gump, 0 );
	GFResizePic( gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 600, 450 );
	GFResizePic( gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 570, 25 );
	GFTextMid( gump, 15, 20, 570, 1720, "Help Menu" );

	// Main stone area where content goes
	GFResizePic( gump, 205, 45, GFGetCfgConst( "Defaults", "ForeGround" ), 380, 325 );

	// Box with the GM in it
	GFResizePic( gump, 15, 45, GFGetCfgConst( "Defaults", "ForeGround"), 185, 325 );
	GFResizePic( gump, 20, 50, GFGetCfgConst( "BackGrounds", "GoldTrim"), 175, 315 );

	GFGumpPic( gump, 20, 125, 0xC, 1023 );
	GFGumpPic( gump, 20, 125, 0xC72B, 1170 );
	GFGumpPic( gump, 20, 125, 0xC5B9 );

	var help := "This menu will help guide you to the best place for answers about
		    "+SHARD_NAME+".";
	GFDrawWrappedText( gump, 40, 60, GFWordWrap( help, 175 ));

	// Bar along the bottom with URL and cancel button.
	GFResizePic( gump, 15, 375, GFGetCfgConst( "Defaults", "ForeGround" ), 570, 60 );

	var for_help := "If you cannot find the help you require on this list, please visit our
		        webpage at "+SHARD_WEBSITE+" for more informations.";

        //GFHTMLArea( gump, 70, 385, 570, 60, for_help );

	GFDrawWrappedText( gump, 70, 385, GFWordWrap( for_help, 570 ), 1720 );

	SaveHelpInfo( "Template", "Gump", gump );

	return gump;
endfunction

function BuildIntroGump()

	var gump := BuildGumpTemplate();
	var text:="", y_pos := 63;

	GFAddButton( gump, 220, y_pos, 4005, 4006, GF_CLOSE_BTN, HELP_TOPICS_BTN );
	GFTextLine( gump, 255, y_pos, 1720, "Help topics for "+SHARD_NAME+"." );

	text := "Select this option if you have a general gameplay question, need
                 help learning to use a skill, or if you would like to search the
                 UO Knowledge Base.";
	GFDrawWrappedText( gump, 220, ( y_pos+=20 ), GFWordWrap( text, 362 ));

	GFAddButton( gump, 220, y_pos+=3, 4005, 4006, GF_CLOSE_BTN, STUCK_PAGE_BTN );
	GFTextLine( gump, 255, y_pos, 1720, "My character is physically stuck." );

	text := "This choice only coverts cases where your characters is physically stuck
                 in a location they cannot move out of. This option will work once every 2 hours.";
	GFDrawWrappedText( gump, 220, ( y_pos+=20 ), GFWordWrap( text, 362 ));

	GFAddButton( gump, 220, y_pos+=3, 4005, 4006, GF_CLOSE_BTN, STAFF_PAGE_BTN );
	GFTextLine( gump, 255, y_pos, 1720, "Contact a staff member." );

	text := "A staff member is an experienced player on the shard who has volunteered time
		to help answer questions. Selecting this option will let you send a message to a
		Counselor or GameMaster.";
	GFDrawWrappedText( gump, 220, ( y_pos+=20 ), GFWordWrap( text, 362 ));

	GFAddButton( gump, 500, 340, 2119, 2120, GF_CLOSE_BTN, CLOSE_BTN );

	return gump;
endfunction

function BuildTextEntryGump( title, byref cur_txt )

	var gump := LoadHelpInfo( "TextEntry", "Gump" );
	if( !gump )
		gump := BuildGumpTemplate();

		GFResizePic( gump, 220, 75, GFGetCfgConst( "BackGrounds", "GoldTrim" ), 348, 265 );

		GFAddButton( gump, 220, 338, 4014, 4015, GF_CLOSE_BTN, MAIN_PAGE_BTN );
		GFTextLine( gump, 255, 340, 2100, "Back" );

		GFAddButton( gump, 325, 340, 2117, 2118, GF_CLOSE_BTN, WORD_WRAP_BTN );
		GFTextLine( gump, 345, 340, 2100, "Word Wrap" );

		GFAddButton( gump, 540, 338, 4029, 4030, GF_CLOSE_BTN, SEND_PAGE_BTN );
		GFTextLine( gump, 460, 340, 2100, "Send Page" );

		SaveHelpInfo( "TextEntry", "Gump", gump );
	endif

	// This part below can't be cached.
	GFTextMid( gump, 210, 55, 380, 2100, title );

	var i;
	var y_pos := 80;
	for( i:=PAGE_TXT_START; i<=PAGE_TXT_END; i+=1 )
		var cur_line := cur_txt[i-( PAGE_TXT_START-1 )];
		if( !cur_line )
			cur_line := "";
		endif

		GFTextEntry( gump, 225, y_pos, 330, 20, 2100, cur_line, i );

		y_pos += 20;
		SleepMS(2);
	endfor

	return gump;
endfunction

function BuildHelpTopicsIndexGump()

	var gump := BuildGumpTemplate();

 	GFAddButton( gump, 220, 338, 4014, 4015, GF_CLOSE_BTN, MAIN_PAGE_BTN );
	GFTextLine( gump, 255, 340, 2100, "Back" );

	GFPage( gump, 1 );
	var y_pos := 60;
	var help_cfg := ReadConfigFile( ":help:helptopics" );
	var elem_names := GetConfigStringKeys( help_cfg );
	SaveHelpInfo( "TopicNames", "List", elem_names );

	foreach elem_name in ( elem_names )
		GFAddButton( gump, 220, ( y_pos+3 ), 2117, 2118, GF_CLOSE_BTN, HELP_TOPIC_START+_elem_name_iter );
		GFTextLine( gump, 240, y_pos, 2100, elem_name );
		y_pos += 20;

		if( _elem_name_iter % 13 == 0 )
			GFAddButton( gump, 555, 338, 0xA58, 0xA59, GF_PAGE_BTN, gump.cur_page+1 );
			GFPage( gump, gump.cur_page+1 );
			GFAddButton( gump, 555, 56, 0xA5A, 0xA5B, GF_PAGE_BTN, gump.cur_page-1 );
			y_pos := 60;
		endif

		SleepMS(2);
	endforeach

	return gump;
endfunction

function BuildHelpTopicGump( topic_name )

	var gump := BuildGumpTemplate();

  	GFAddButton( gump, 220, 338, 4014, 4015, GF_CLOSE_BTN, HELP_TOPICS_BTN );
	GFTextLine( gump, 255, 340, 2100, "Back" );

	GFPage( gump, 1 );
	var help_file := GetHelpFilePath( topic_name );

	var text := ImplodeArray( "", ReadFile( help_file ));

	GFHTMLArea( gump, 220, 75, 348, 265, text, 1, 1 );

	return gump;
endfunction

function BuildHelpStuckCommand( mobile )

        var gump := GFCreateGump();

        BuildGumpForGate( gump );

        var input := GFSendGump( mobile, gump );
        if( !input[0x10] )
		SendSysMessage( mobile, "Cancelled." );
	        return 0;
        endif

        var data_elem := DFFindElement( g_datafile, mobile.serial );
        var release_time := data_elem.GetProp( "JailReleaseTime" );
        if( release_time )
                if( POLCore().systime < release_time )
                        SendSysMessage( mobile, "Think about a better solution ( if any ). For now you stay jailed." );
                        return 0;
                endif
        endif

        SendSysMessage( mobile, "You will be teleported within the next 5 minutes." );
        mobile.frozen := 1;

        var time := 0;
        while( time < 300 )

                if( !mobile.dead && mobile.multi )
                        SendSysMessage( mobile, "You cannot use this option while standing in a house or a boat alive." );
                        mobile.frozen := 0;
                        return 0;
                endif

                var hostiles := ListHostiles( mobile, 20 );
                if( hostiles.Size() >= 1 )
                        SendSysMessage( mobile, "You cannot use this option while you are being attacked." );
                        mobile.frozen := 0;
                        return 0;
                endif

                var near := ListMobilesNearLocation( mobile.x, mobile.y, mobile.z, 14, mobile.realm );
                foreach player in near
                        if( player.cmdlevel > 0 )
                                SendSysMessage( mobile, "You cannot use this option while a staff member is surrounding." );
                                mobile.frozen := 0;
                                return 0;
                        endif
                        SleepMS(5);
                endforeach

                time += 1;
                Sleep(1);
        endwhile

        mobile.frozen := 0;

	var select := 0;
	foreach key in ( input.Keys )
		if( key >= 0xA00 )
			select := key-0xA00;
			break;
		endif
		SleepMS(2);
	endforeach

	var dest := MG_FindDestination( select );

	var result := MoveObjectToLocation( mobile, dest.x, dest.y, dest.z, dest.realm, MOVEOBJECT_NORMAL );
	if( result.errortext )
		SendSysMessage( mobile, "Cancelled." );
	        return 0;
	endif

 	SetObjProperty( mobile, "#TeleportedHelp", ReadGameClock() + ( SEC_MINUTE * 120 ));

        return 1;
endfunction

function BuildGumpForGate( byref gump )

        var config := ReadConfigFile( ":moongates:moongate" );
        if( config.errortext )
                SysLog( "Error reading <:moongates:moongate> -->"+config.errortext );
                return gump;
        endif

	GFResizePic( gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 340, 330 );
	GFResizePic( gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 310, 300 );

	GFTextMid( gump, 20, 30, 310, 2100, "Select a Destination" );
	GFAddButton( gump, 35, 250, 2128, 2129, GF_CLOSE_BTN, 0x10 );
	GFAddButton( gump, 35, 280, 2119, 2120, GF_CLOSE_BTN, 0x0 );

	var y_pos := 60;
	var page := 2;
	foreach realm in ( Realms() )
		realm := realm; // To avoid compiler warnings.
		var cfg_elem := config[_realm_iter];
                if( cfg_elem.errortext )
                        //No files for realm.
                	continue;
                endif

		GFAddButton( gump, 30, y_pos+3, 2117, 2118, GF_PAGE_BTN, page );
		GFTextLine( gump, 50, y_pos, 1720, cfg_elem.Faction );
		y_pos += 20;
		page += 2;
	endforeach

	page := 2;
	var btn_id := 1;
	foreach realm in ( Realms() )
		realm := realm; // To avoid compiler warnings.
		y_pos := 60;

		var cfg_elem := config[_realm_iter];
                if( cfg_elem.errortext )
                        //No files for realm.
                	continue;
                endif

		GFPage( gump, page );

		foreach entry in ( GetConfigStringDictionary( cfg_elem, "Town" ))
			entry := entry; // To avoid compiler warnings.

			GFRadioButton( gump, 150, y_pos, 210, 211, 0, btn_id+MG_LOCATION_START );
			GFTextLine( gump, 173, y_pos, 2100, _entry_iter );

			y_pos += 20;
			btn_id += 1;
			SleepMS(2);
		endforeach

		SleepMS(2);
		page += 2;
	endforeach

        return gump;
endfunction
