function ICP_IndexGump()
	var gump := GFCreateGump();
	GFResizePic(gump, 0, 0, BKGND_SILVERTRIM, 500, 480);
	GFResizePic(gump, 15, 15, BKGND_DARK_STONE, 470, 35);
	GFTextMid(gump, 15, 20, 470, 1153, "Package List");
	
	GFResizePic(gump, 15, 55, BKGND_DARK_STONE, 470, 410);
	GFPage(gump, 1);
	
	var y_pos := 65;
	var page_num := 1;
	var counter := 1;
	
	var pkg_elems := pkg_info.keys();
	pkg_elems.Sort();
	foreach pkg_name in pkg_elems
		var pkg_elem := DFFindElement(pkg_info, pkg_name);
		var main_info := pkg_elem.GetProp("Main");
		
		GFTextLine(gump, 65, y_pos, 2100, main_info.Name);
		if ( main_info.NoSupport )
			GFTextRight(gump, 455, y_pos, 32, "[Not ICP compliant]");
		else
			GFTextRight(gump, 395, y_pos, 65, "[OK]");
		endif
		GFAddButton(gump, 25, y_pos, 4005, 4006, GF_CLOSE_BTN, _pkg_name_iter);
		
		if ( counter >= 16 )
			y_pos := 65;
			counter := 1;
			
			GFAddButton(gump, 460, 435, 2648, 2649, GF_PAGE_BTN, page_num+1);
			page_num := page_num+1;
			GFPage(gump, page_num);
			GFAddButton(gump, 460, 70, 2650, 2651, GF_PAGE_BTN, page_num-1);
		else
			y_pos := y_pos+24;
			counter := counter+1;
		endif
		
		SleepMS(2);
	endforeach
	
	return gump;
endfunction

function ICP_PackageIndex(who)
	var gump := ICP_RetrieveGump("Index");
	if  ( !gump )
		SendSysMessage(who, "Building index gump...");
		gump := ICP_IndexGump();
		ICP_SaveGump("Index", gump);
	endif

	var input := GFSendGump(who, gump);
	input := input[0];
	if ( !input )
		SendSysMessage(who, "Cancelled");
		return 0;
	else
		var elems := (pkg_info.keys());
		return elems[input];
	endif
endfunction