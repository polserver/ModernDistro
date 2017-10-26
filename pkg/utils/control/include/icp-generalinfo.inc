function ICP_GeneralGump(pkg_name)
	var pkg_elem := DFFindElement(pkg_info, pkg_name);
	var main_info := pkg_elem.GetProp("Main");
		
	var gump := GFCreateGump();
	
	ICP_GumpHeader(gump, GENERAL_BTN);
	GFPage(gump, 0);
	GFTextLine(gump, 25, 100, 2100, "Name:");
	GFTextLine(gump, 130, 100, 2100, main_info.Name);
	GFTextLine(gump, 25, 120, 2100, "Core Name:");
	GFTextLine(gump, 130, 120, 2100, pkg_name);
	GFTextLine(gump, 25, 140, 2100, "Version:");
	GFTextLine(gump, 130, 140, 2100, main_info.Version);
	GFTextLine(gump, 25, 180, 2100, "Description:");
	GFPage(gump, 1);
	GFAddButton(gump, 130, 180, 2710, 2711, GF_PAGE_BTN, 2);
	
	GFTextLine(gump, 25, 230, 2100, "Creator:");
	GFTextLine(gump, 130, 230, 2100, main_info.Creator);
	GFTextLine(gump, 25, 250, 2100, "Email:");
	GFTextLine(gump, 130, 250, 2100, main_info.C_Email);

	GFTextLine(gump, 25, 290, 2100, "Maintainer:");
	GFTextLine(gump, 130, 290, 2100, main_info.Maintainer);
	GFTextLine(gump, 25, 310, 2100, "Email:");
	GFTextLine(gump, 130, 310, 2100, main_info.M_Email);
	
	GFPage(gump, 2);
	GFAddButton(gump, 130, 180, 2711, 2710, GF_PAGE_BTN, 1);
	GFResizePic(gump, 25, 205, GetCfgConst("BackGrounds", "GOLDTRIM"), 450, 220);
	var y_pos := 210;
	foreach line in (GFWordWrap(main_info.Description, 440))
		GFTextLine(gump, 30, y_pos, 2100, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	return gump;
endfunction