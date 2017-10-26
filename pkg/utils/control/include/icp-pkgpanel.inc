/* $Id: icp-pkgpanel.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */
function ICP_PackagePanels(who, pkg_name)
	var info_elem := DFFindElement(pkg_info, pkg_name);
	if ( !info_elem )
		GumpPrompt(who, "Error: Package ["+pkg_name+"] [P] Not found!");
		return 0;
	endif

	var cur_menu := GENERAL_BTN;
	var input := GENERAL_BTN;
	var gump := 0;
	while ( 1 )
		if ( input )
			if ( input > START_ID )
				cur_menu := input;
				gump := ICP_GetPkgGump(who, pkg_name, cur_menu);
			else
				ICP_PkgPanelInput(who, pkg_name, cur_menu, info_elem, input);
			endif
		endif

		input := GFSendGump(who, gump);
		input := input[0];

		if ( !input )
			break;
		elseif ( input == BACK_BTN )
			break;
		elseif ( input == UNCACHE_BTN )
			input := cur_menu;
			pkg_gumps.DeleteElement(pkg_name);
			SendSysMessage(who, "Uncached ["+pkg_name+"].");
		endif
	endwhile

	return input;
endfunction

function ICP_PkgPanelInput(who, pkg_name, cur_menu, info_elem, input)
	// Used to fix unused var report in ecompile: pkg_name;
	pkg_name := pkg_name;
	if ( cur_menu == SCRIPTS_BTN )
		ICP_StartScript(who, input, info_elem);
		return 1;
	elseif ( cur_menu == COMMANDS_BTN )
		ICP_StartCommand(who, input, info_elem);
		return 1;
	elseif ( cur_menu == ITEMDESC_BTN )
		ICP_CreateItem(who, input);
		return 1;
	else
		//SendSysMessage(who, "Error - Unknown input ["+input+"]");
		return 0;
	endif
endfunction

function ICP_GetPkgGump(who, pkg_name, btn_number)
	// Used to fix unused var report in ecompile: who;
	who := who;
	var gump := 0;
	if ( btn_number == GENERAL_BTN )
		gump := ICP_RetrieveGump(pkg_name, "general");
		if ( !gump )
			//SendSysMessage(who, "Building package ["+pkg_name+"] general gump...");
			gump := ICP_GeneralGump(pkg_name);
			ICP_SaveGump(pkg_name, gump, "general");
		endif
	elseif ( btn_number == SCRIPTS_BTN )
		gump := ICP_RetrieveGump(pkg_name, "scripts");
		if ( !gump )
			//SendSysMessage(who, "Building package ["+pkg_name+"] scripts gump...");
			gump := ICP_ScriptsGump(pkg_name);
			ICP_SaveGump(pkg_name, gump, "scripts");
		endif
	elseif ( btn_number == COMMANDS_BTN )
		gump := ICP_RetrieveGump(pkg_name, "commands");
		if ( !gump )
			//SendSysMessage(who, "Building package ["+pkg_name+"] commands gump...");
			gump := ICP_CommandsGump(pkg_name);
			ICP_SaveGump(pkg_name, gump, "commands");
		endif
	elseif ( btn_number == ITEMDESC_BTN )
		gump := ICP_RetrieveGump(pkg_name, "itemdesc");
		if ( !gump )
			//SendSysMessage(who, "Building package ["+pkg_name+"] itemdesc gump...");
			gump := ICP_ItemDescGump(pkg_name);
			ICP_SaveGump(pkg_name, gump, "itemdesc");
		endif
	endif

	return gump;
endfunction