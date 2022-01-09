/*
        Created by *Edwards

        2008-10-11
*/
use uo;
use os;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":datafile:datafile";
include ":moongates:moongate";

var g_datafile := DFOpenDataFile( ":jailKeeper:players", DF_CREATE );

function Goto_Cell( mobile )

       var data_elem := DFFindElement( g_datafile, mobile.serial );
       var cell := data_elem.GetProp( "JailCell" );
       if( !cell )
               cell := 1;
       endif
       
       case( cell )
              1:
                        MoveObjectToLocation( mobile, 5275, 1164, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              2:
                        MoveObjectToLocation( mobile, 5286, 1164, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              3:
                        MoveObjectToLocation( mobile, 5295, 1164, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              4:
                        MoveObjectToLocation( mobile, 5306, 1164, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              5:
                        MoveObjectToLocation( mobile, 5275, 1174, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              6:
                        MoveObjectToLocation( mobile, 5286, 1174, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              7:
                        MoveObjectToLocation( mobile, 5295, 1174, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              8:
                        MoveObjectToLocation( mobile, 5306, 1174, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              9:
                        MoveObjectToLocation( mobile, 5281, 1185, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
              10:
                        MoveObjectToLocation( mobile, 5302, 1185, 0, "britannia", MOVEOBJECT_FORCELOCATION );
                        break;
       endcase
       
       SetObjProperty( mobile, "NoGains", 1 );
       
       return 1;
endfunction

function BuildGumpForGate( byref gump )

        var config := ReadConfigFile(":moongates:moongate");
        if( config.errortext )
                SysLog( "Error reading <:moongates:moongate> -->"+config.errortext );
                return gump;
        endif

        GFClosable( gump, 0 );

	GFResizePic( gump, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 340, 330 );
	GFResizePic( gump, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 310, 300 );

	GFTextMid( gump, 20, 30, 310, 2100, "Select a Destination" );
        GFAddButton( gump, 35, 250, 2128, 2129, GF_CLOSE_BTN, 0x10 );

	var y_pos := 60;
	var page := 2;
	foreach realm in ( Realms() )
		realm := realm; // To avoid compiler warnings.
		var cfg_elem := config[_realm_iter];
		GFAddButton( gump, 30, y_pos+3, 2117, 2118, GF_PAGE_BTN, page );
		GFTextLine( gump, 50, y_pos, 2100, cfg_elem.Faction );
		y_pos += 20;
		page += 2;
	endforeach

	page := 2;
	var btn_id := 1;
	foreach realm in ( Realms() )
		realm := realm; // To avoid compiler warnings.
		y_pos := 60;
		var cfg_elem := config[_realm_iter];
		GFPage( gump, page );

		foreach entry in ( GetConfigStringDictionary(cfg_elem, "Town") )
			entry := entry; // To avoid compiler warnings.
			GFRadioButton( gump, 150, y_pos, 210, 211, 0, btn_id+0xA00 );
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
