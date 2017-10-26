/* $Id: goGumps.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
// Gump backgrounds - Used in various functions - easier to use globally.
var BKGND_SILVERTRIM := GetCfgConst("Defaults", "BackGround");
var BKGND_DARK_STONE := GetCfgConst("Defaults", "ForeGround");


function Go_GumpHeader(public:=1)

	var headerGump := GFCreateGump();
	GFResizePic(headerGump, 0, 0, BKGND_SILVERTRIM, 400, 490);
	GFResizePic(headerGump, 15, 15, BKGND_DARK_STONE, 370, 35);
	if( public )
		GFHTMLArea(headerGump,  15, 20, 370, 20, "<basefont color=#7FFFD4><center>Go To Locations (Public)</center>");
	else
		GFHTMLArea(headerGump,  15, 20, 370, 20, "<basefont color=#7FFFD4><center>Go To Locations (Private)</center>");
	endif
	GFResizePic(headerGump, 15, 55, BKGND_DARK_STONE, 370, 380);
	GFResizePic(headerGump, 15, 440, BKGND_DARK_STONE, 370, 35);

	return headerGump;

endfunction


function Go_LocationIndex(who, gump_Type:="base_Distro")

	var gump := Go_RetrieveGump("LocationIndex", gump_Type);
	if(gump_Type == "base_Distro")
		num_keys := go_keys_base.Size();
	else
		num_keys := go_keys.Size();
	endif
	if  ( !gump )
		if(gump_Type == "base_Distro")
			gump := Go_LocationIndexGump(1);
		else
			gump := Go_LocationIndexGump(0);
		endif
		Go_SaveGump("LocationIndex", gump, gump_Type);
	endif

	var input := GFSendGump(who, gump);
	input := input[0];
        
	case(input)
		0:
			SendSysMessage(who, "Closed");
			return 0;
		ADD_NEW_PUBLIC:
			Go_AddNewLocation(who, 1);
			return 0;
		ADD_NEW_PRIVATE:
			Go_AddNewLocation(who, 0);
			return 0;
		COPY_PUBLIC_ITEM:
			Go_CopyLocation(who, 1);
			return 0;
		COPY_PRIVATE_ITEM:
			Go_CopyLocation(who, 0);
			return 0;
		EDIT_PUBLIC_ITEM:
			print("Edit Public"); Go_EditSendLocation(who, 1);
			return 0;
		EDIT_PRIVATE_ITEM:
			Go_EditSendLocation(who, 0);
			return 0;
		SHOW_PUBLIC_INDEX:
			SetObjProperty(who, "#Go_Gump", 1);
			Go_LocationIndex(who);
			return 0;
		SHOW_PRIVATE_INDEX: SetObjProperty(who, "#Go_Gump", 2);
			Go_LocationIndex(who, who.acctname);
			return 0;
		default:
			break;
	endcase

	if( input )
		var send_Location;
		var moved := who;
		if ( input <= num_keys )
			case(gump_Type)
				"base_Distro": send_Location := go_prop_base[Lower(go_keys_base[input])]; break;
				default:       send_Location := go_prop[Lower(go_keys[input])]; break;
			endcase
			SendSysMessage(who, "Who do you want to send to "+send_Location.Name);
			moved := target(who);
			if ( !moved )
				return 0;
			endif
		else
			input := input - num_keys;
			case( gump_Type )
				"base_Distro": send_Location  := go_prop_base[Lower(go_keys_base[input])]; break;
				default:       send_Location := go_prop[Lower(go_keys[input])]; break;
			endcase
		endif

		if ( send_Location )
			SendSysMessage(who, "Going to ["+send_Location.Name+"]");
			var result := MoveObjectToLocation(moved, CInt(send_location.X), CInt(send_location.Y), CInt(send_location.Z), send_location.Realm, MOVEOBJECT_FORCELOCATION);

			if ( result.errortext )
				GumpPrompt(who, "Error moving: X["+send_Location.X+"] Y["+send_Location.Y+"] Z["+send_Location.Z+"]");
			endif
		else
			GumpPrompt(who, "Location ["+send_Location.Name+"] does not exist.");
		endif

	endif
        return 0;

endfunction

function Go_LocationIndexGump(public:=1)

	var gump := Go_GumpHeader(public);
	GFHTMLArea(gump,  25, 60, 150, 20, "<basefont color=#7FFFD4>Name");
	GFHTMLArea(gump, 175, 60, 150, 20, "<basefont color=#7FFFD4>Realm");
	GFHTMLArea(gump, 295, 60, 150, 20, "<basefont color=#7FFFD4>Send");
	GFHTMLArea(gump, 345, 60, 150, 20, "<basefont color=#7FFFD4>Goto");
	if( public )
		GFAddButton(gump, 25, 447, 4008, 4010, GF_CLOSE_BTN, ADD_NEW_PUBLIC);
		GFHTMLArea(gump,  57, 450, 30, 20, "<basefont color=#7FFFD4>Add");
		GFAddButton(gump, 100, 447, 4005, 4007, GF_CLOSE_BTN, COPY_PUBLIC_ITEM);
		GFHTMLArea(gump,  132, 450, 30, 20, "<basefont color=#7FFFD4>Copy");
		GFAddButton(gump, 175, 447, 4008, 4010, GF_CLOSE_BTN, EDIT_PUBLIC_ITEM);
		GFHTMLArea(gump,  207, 450, 30, 20, "<basefont color=#7FFFD4>Edit");
		GFAddButton(gump, 250, 447, 4008, 4010, GF_CLOSE_BTN, SHOW_PRIVATE_INDEX);
		GFHTMLArea(gump,  282, 450, 100, 20, "<basefont color=#7FFFD4>Private Menu");
	else
		GFAddButton(gump, 25, 447, 4008, 4010, GF_CLOSE_BTN, ADD_NEW_PRIVATE);
		GFHTMLArea(gump,  57, 450, 30, 20, "<basefont color=#7FFFD4>Add");
		GFAddButton(gump, 100, 447, 4005, 4007, GF_CLOSE_BTN, COPY_PRIVATE_ITEM);
		GFHTMLArea(gump,  132, 450, 30, 20, "<basefont color=#7FFFD4>Copy");
		GFAddButton(gump, 175, 447, 4008, 4010, GF_CLOSE_BTN, EDIT_PRIVATE_ITEM);
		GFHTMLArea(gump,  207, 450, 30, 20, "<basefont color=#7FFFD4>Edit");
		GFAddButton(gump, 250, 447, 4008, 4010, GF_CLOSE_BTN, SHOW_PUBLIC_INDEX);
		GFHTMLArea(gump,  282, 450, 100, 20, "<basefont color=#7FFFD4>Public Menu");
	endif

	GFPage(gump, 1);

	var y_loc := 85;
	var page_num := 1;
	var counter := 1;
	var line_num := 0;

	if( public )
		go_keys_base.Sort();
		foreach place in ( go_keys_base )
			GFTextLine(gump,  25, y_loc, 2100, go_prop_base[place].Name);
			GFTextLine(gump, 175, y_loc, 2100, go_prop_base[place].Realm);
			GFAddButton(gump, 300, y_loc+3, 2117, 2118, GF_CLOSE_BTN, counter);
			GFAddButton(gump, 350, y_loc+3, 2117, 2118, GF_CLOSE_BTN, (num_keys+counter));

			counter := counter + 1;
			line_num := line_num+1;
			y_loc := y_loc + 20;
			if ( line_num >= 17 && line_num < num_keys )
				GFAddButton(gump, 370, 405, 2706, 2707, GF_PAGE_BTN, page_num+1);
				page_num := page_num + 1;
				GFPage(gump, page_num);
				GFAddButton(gump, 370,  85, 2704, 2705, GF_PAGE_BTN, page_num-1);
				y_loc := 85;
				line_num := 0;
			endif
			SleepMS(2);
		endforeach
	else
		go_keys.Sort();
		foreach place in ( go_keys )
			GFTextLine(gump,  25, y_loc, 2100, go_prop[place].Name);
			GFTextLine(gump, 175, y_loc, 2100, go_prop[place].Realm);
			GFAddButton(gump, 300, y_loc+3, 2117, 2118, GF_CLOSE_BTN, counter);
			GFAddButton(gump, 350, y_loc+3, 2117, 2118, GF_CLOSE_BTN, (num_keys+counter));

			counter := counter + 1;
			line_num := line_num+1;
			y_loc := y_loc + 20;
			if ( line_num >= 17 && line_num < num_keys )
				GFAddButton(gump, 370, 405, 2706, 2707, GF_PAGE_BTN, page_num+1);
				page_num := page_num + 1;
				GFPage(gump, page_num);
				GFAddButton(gump, 370,  85, 2704, 2705, GF_PAGE_BTN, page_num-1);
				y_loc := 85;
				line_num := 0;
			endif
			SleepMS(2);
		endforeach
	endif

	return gump;


endfunction

function Go_RetrieveGump(elem_name, prop_name:="base_Distro")

	var elem := DFFindElement(go_data_gumps, elem_name);
	return (elem.GetProp(prop_name));

endfunction

function Go_SaveGump(elem_name, gump, prop_name:="base_Distro")

	var elem := DFFindElement(go_data_gumps, elem_name, DF_CREATE);
	return (elem.SetProp(prop_name, gump));

endfunction

function Go_RefreshGump(elem_name, prop_name:="base_Distro")

	var elem := DFFindElement(go_data_gumps, elem_name);
	return (elem.EraseProp(prop_name));

endfunction