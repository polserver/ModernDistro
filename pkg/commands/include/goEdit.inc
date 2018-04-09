/* $Id: goEdit.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */


function Go_EditSendLocation(who, public:=1)
	var gump;
	if( public )
		gump := Go_SelectEditGump(1);
		num_keys := go_keys_base.Size();
	else
		gump := Go_SelectEditGump(0);
		num_keys := go_keys.Size();
	endif

	var input := GFSendGump(who, gump);
        input := input[0];

        if(input)
		var send_Location;
		if( public )
			send_Location := go_prop_base[Lower(go_keys_base[input])];
		else
			send_Location := go_prop[Lower(go_keys[input])];
		endif
                if ( send_Location )
			var edit_gump;
			if( public )
				edit_gump := Go_EditLocationGump(send_Location);
			else
				edit_gump := Go_EditLocationGump(send_Location, 0);
			endif
                        var edit_input := GFSendGump(who, edit_gump);

                        if ( edit_input[edit_update] )
                                edit_name := GFExtractData(edit_input, edit_name);
                                xg_pos := CInt(GFExtractData(edit_input, xg_pos));
                                yg_pos := CInt(GFExtractData(edit_input, yg_pos));
                                zg_pos := CInt(GFExtractData(edit_input, zg_pos));

                                if ( !edit_name )
                                        GumpPrompt(who, "Cancelled - No name entered");
                                        return 0;
                                elseif ( Lower(edit_name) != send_Location.Name )
					if( public )
						if ( go_prop_base.Exists( Lower(edit_name) ) )
							GumpPrompt(who, "Cancelled - location already exists with the name ["+edit_name+"]");
							return 0;
						endif
					else
						if ( go_prop.Exists( Lower(edit_name) ) )
							GumpPrompt(who, "Cancelled - location already exists with the name ["+edit_name+"]");
							return 0;
						endif
					endif
                                endif

                                var original_Realm := send_Location.Realm;
				if( public )
					go_prop_base.Erase(Lower(send_Location.Name));
					go_elem.SetProp("base_Distro", go_prop_base);
					Go_SetLocation("base_Distro", who, edit_name, original_Realm, xg_pos, yg_pos, zg_pos);
				else
					go_prop.Erase(Lower(send_Location.Name));
					go_elem.SetProp(who.acctname, go_prop);
					Go_SetLocation(who.acctname, who, edit_name, original_Realm, xg_pos, yg_pos, zg_pos);
				endif
                                SendSysMessage(who, "Updated ["+edit_name+"]");
                                Go_RefreshMyGumps(who);
                        elseif ( edit_input[edit_delete] )
                                SendSysMessage(who, "Deleted ["+send_Location.Name+"]");
				if( public )
					go_prop_base.Erase(Lower(send_Location.Name));
					go_elem.SetProp("base_Distro", go_prop_base);
				else
					go_prop.Erase(Lower(send_Location.Name));
					go_elem.SetProp(who.acctname, go_prop);
				endif
                                Go_RefreshMyGumps(who);
                        else
                                SendSysMessage(who, "Cancelled");
                        endif
                else
                        GumpPrompt(who, "Location ["+send_Location.Name+"] does not exist.");
                        return 0;
                endif
        else
                SendSysMessage(who, "Editing Cancelled");
                return 0;
        endif

        return 0;

endfunction


function Go_SelectEditGump(public)

	var gump := Go_GumpHeader(public);
        GFHTMLArea(gump,  25, 60, 150, 20, "<basefont color=#7FFFD4>Name");
        GFHTMLArea(gump, 175, 60, 150, 20, "<basefont color=#7FFFD4>Realm");
        GFHTMLArea(gump, 295, 60, 150, 20, "<basefont color=#7FFFD4>Edit");

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


function Go_EditLocationGump(send_Location, public:=1)

	var gump := Go_GumpHeader(public);

	edit_update := GFAddButton(gump, 25, 447, 4008, 4010, GF_CLOSE_BTN, UPDATE_BUTTON);
	GFHTMLArea(gump,  57, 450, 40, 20, "<basefont color=#7FFFD4>Update");
	edit_delete := GFAddButton(gump, 100, 447, 4005, 4007, GF_CLOSE_BTN, DELETE_BUTTON);
	GFHTMLArea(gump,  132, 450, 40, 20, "<basefont color=#7FFFD4>Delete");

	GFTextLine(gump, 20, 120, 2100, "Name");
	GFResizePic(gump, 120, 117, 2620, 180, 30);
	edit_name := GFTextEntry(gump, 130, 122, 160, 25, 2100, send_Location.Name);

	GFTextLine(gump, 20, 143, 2100, "X Coordinate");
	GFResizePic(gump, 120, 140, 2620, 100, 30);
	xg_pos := GFTextEntry(gump, 130, 145, 80, 25, 2100, send_Location.X);

	GFTextLine(gump, 20, 166, 2100, "Y Coordinate");
	GFResizePic(gump, 120, 163, 2620, 100, 30);
	yg_pos := GFTextEntry(gump, 130, 168, 80, 25, 2100, send_Location.Y);

	GFTextLine(gump, 20, 189, 2100, "Z Coordinate");
	GFResizePic(gump, 120, 186, 2620, 100, 30);
	zg_pos := GFTextEntry(gump, 130, 191, 80, 25, 2100, send_Location.Z);

	return gump;

endfunction