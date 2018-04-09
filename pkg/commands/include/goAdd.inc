/* $Id: goAdd.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */


function Go_AddNewLocation(who, public:=1)

	SendSysMessage(who, "Click where you want the new Go Location to go to.");
	var targ := TargetCoordinates(who);

	if (!targ)
		SendSysMessage(who, "Cancelled");
		return 0;
	endif

	var name := RequestInput(who, who.backpack, "What do you want to call this go location?");

	if ( !name )
		SendSysMessage(who, "Cancelled");
		return 0;
	endif

	if( public )
		go_prop_base := go_elem.GetProp("base_Distro");
		if ( go_prop_base.Exists(Lower(name)) )
			GumpPrompt(who, "You already have a public go location named ["+name+"]");
			return 0;
		endif
		Go_SetLocation("base_Distro", who, name, targ.realm, targ.x, targ.y, targ.z);
		GumpPrompt(who, "New public go location added: ["+name+"]");
	else
		go_prop := go_elem.GetProp(who.acctname);
		if ( go_prop.Exists(Lower(name)) )
			GumpPrompt(who, "You already have a private go location named ["+name+"]");
			return 0;
		endif
		Go_SetLocation(who.acctname, who, name, targ.realm, targ.x, targ.y, targ.z);
		GumpPrompt(who, "New private go location added: ["+name+"]");
	endif

	return 0;

endfunction

function Go_CopyLocation(who, public:=1)

	var gump;
	if( public )
		gump := Go_RetrieveGump("CopyIndex", "base_Distro");
		num_keys := go_keys_base.Size();
	else
		gump := Go_RetrieveGump("CopyIndex", who.acctname);
		num_keys := go_keys.Size();
	endif

	if  ( !gump )
		if( public )
			gump := Go_LocationCopyGump(1);
			Go_SaveGump("CopyIndex", gump, "base_Distro");
		else
			gump := Go_LocationCopyGump(0);
			Go_SaveGump("CopyIndex", gump, who.acctname);
		endif
	endif

	var input := GFSendGump(who, gump);
	input := input[0];

	if( input )
		var send_Location := 0;
		if( public )
			send_Location := go_prop_base[Lower(go_keys_base[input])];
		else
			send_Location := go_prop[Lower(go_keys[input])];
		endif
		if ( send_Location )
			if( public )
				go_prop_base := go_elem.GetProp("base_Distro");
				if ( go_prop.Exists(Lower(send_Location.Name)) )
					GumpPrompt(who, "You already have a private go location named ["+send_Location.Name+"]");
					return 0;
				endif
				Go_SetLocation(who.acctname, who, send_Location.Name, send_Location.Realm, send_Location.X, send_Location.Y, send_Location.Z);
				GumpPrompt(who, "Copied ["+send_Location.Name+"] to your private menu.");
			else
				go_prop := go_elem.GetProp(who.acctname);
				if ( go_prop_base.Exists(Lower(send_Location.Name)) )
					GumpPrompt(who, "You already have a public go location named ["+send_Location.Name+"]");
					return 0;
				endif
				Go_SetLocation("base_Distro", who, send_Location.Name, send_Location.Realm, send_Location.X, send_Location.Y, send_Location.Z);
				GumpPrompt(who, "Copied ["+send_Location.Name+"] to the public menu.");
			endif
			return 0;
		else
			GumpPrompt(who, "Location ["+send_Location.Name+"] does not exist.");
			Go_RefreshMyGumps(who);
			return 0;
		endif
	else
		SendSysMessage(who, "Copying Cancelled");
		Go_RefreshMyGumps(who);
		return 0;
	endif

	return 0;

endfunction

function Go_SetLocation(gump_Type, who, name, realm, x, y, z)

	if( !realm )
		realm := who.realm;
	endif

	z := GetMapInfo(x, y, realm).z;

	var temp := struct;
	temp.+Name := Lower(name);
	temp.+Realm := Lower(realm);
	temp.+X := x;
	temp.+Y := y;
	temp.+Z := z;
	case(gump_Type)
		"base_Distro": go_prop_base[Lower(name)] := temp;
			       go_elem.SetProp(gump_Type, go_prop_base);
		default:       go_prop[Lower(name)] := temp;
                               go_elem.SetProp(gump_Type, go_prop);
	endcase
	Go_RefreshMyGumps(who);

	return 0;

endfunction

function Go_LocationCopyGump(public:=1)

	var gump := Go_GumpHeader(public);
	GFHTMLArea(gump,  25, 60, 150, 20, "<basefont color=#7FFFD4>Name");
	GFHTMLArea(gump, 175, 60, 150, 20, "<basefont color=#7FFFD4>Realm");
	GFHTMLArea(gump, 295, 60, 150, 20, "<basefont color=#7FFFD4>Copy");

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