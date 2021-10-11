// //$Id: craftGumps.inc 1458 2009-03-10 04:55:43Z muaddib_pol $

include ":itemutils:itemdesc";

var craft_gumps := DFOpenDataFile(":crafting:CraftGumps", DF_CREATE);
var menu_config := ReadConfigFile( ":crafting:craftConfig" );
var arinfo   := array;
var arcnt    := 61500;
var craft_settings;
var atc := 1;
var PageNum := 1;
// Gump backgrounds - Used in various functions - easier to use globally.
var BKGND_SILVERTRIM := GFGetCfgConst("Defaults", "BackGround");

// This function is the main backbone for building the Crafting Menu.
function sendCraftGump(who, skill_name)

	var menu_file    := menu_config[skill_name].MenuFile;
	var skill_config := ReadConfigFile(menu_file);
	craft_skill      := menu_config[skill_name].Attribute;
	display_skill    := menu_config[skill_name].Name;
	// Craft Cprop is a dictionary storing the information for each crafting skill
	craft_settings := GetObjProperty(who, "Craft");
	if( TypeOfInt(craft_settings) != OT_DICTIONARY )
		craft_settings := cBaseCraftDict();
		SetObjProperty(who, "Craft", craft_settings);
	endif

	while( 1 )

		craftL := craft_settings[skill_name].last;
		rcolor := craft_settings[skill_name].retain;
		makermark := craft_settings[skill_name].mark;

		var craft_gump := Craft_IndexGump(who, skill_name);
		if( !craft_gump )
			return 0;
		endif
	        var res := GFSendGump(who, craft_gump);

		// Exit the menu, return 0 so crafting script knows it was cancled
		if(res[0] == 1)
			return 0;
		endif

		var the_objtype := res[0];

		// Handle setting up of Objtype if they pressed "Last Item". Returns with
		// an error if you haven't crafted anything with the skill yet.
		if( res[61450] )
			the_objtype := craftL;
			if( !the_objtype || (TypeOf(the_objtype) == "Struct") )
				SendSysMessage(who, "You haven't made anything yet.");
				return 0;
			endif
		endif

		// See if the Use Material Color button was pressed.
		if(res[61452])
			case(rcolor)
			  1: craft_settings[skill_name].retain := 0;
			     SetObjProperty(who, "Craft", craft_settings);
			  default: craft_settings[skill_name].retain := 1;
			     SetObjProperty(who, "Craft", craft_settings);
			endcase
			continue;
		endif

		// See if the Maker's Mark button was pressed.
		if(res[61453])
			case(makermark)
			  1: craft_settings[skill_name].mark := 0;
			     SetObjProperty(who, "Craft", craft_settings);
			  default: craft_settings[skill_name].mark := 1;
			     SetObjProperty(who, "Craft", craft_settings);
			endcase
			continue;
		endif

		// This decides if you pressed the Info button, if so, sends info gump
		if( (res[0] >= 61500) && (res[0] <= 62000) )
			the_objtype := arinfo[(res[0] - 61499)];
			var infores := GFCraftInfoPage(who, the_objtype, skill_config);
			case(infores)
			  0: continue;
			  default: the_objtype := infores;
			endcase
		endif

		// Sets the objtype in the Last Item cprop. Redundant to ensure it sets.
		// If a button return (the_objtype) is between 0xF000 and 0xFFFF it is NOT
		// a craftable objtype anyway.
		craft_settings[skill_name].last := the_objtype;
		SetObjProperty(who, "Craft", craft_settings);


		// If the second skill or main required to craft the item is higher than what the player has, skip
		// now as they cannot craft it yet.
		if( TypeOf(craft_config[Hex(the_objtype)].skillid) == "String" )
			if((AP_GetSkill(who, craft_config[Hex(the_objtype)].skillid) < craft_config[Hex(the_objtype)].skill2)
			    || (AP_GetSkill(who, craft_skill) < craft_config[Hex(the_objtype)].skill))
				SendSysMessage(who, "You are not skilled enough to make that item.");
				return 0;
			endif
		endif

		// Sets the amount you had entered into an array to return to crafting
		atc := CInt(GFExtractData(res, atc));
		if(!atc || CInt(atc) < 1)
			atc := 1;
		elseif(atc > CInt(menu_config["Setup"].MaxAmount))
			SendSysMessage(who, "You can only make between 1 and "+menu_config["Setup"].MaxAmount+" at a time.");
			return 0;
		endif
		craft_settings[skill_name].amount := atc;
		SetObjProperty(who, "Craft", craft_settings);

		// Build the return array and give it to crafting.
		var minfo := array;
		minfo.Append(the_objtype);
		minfo.Append(atc);
		minfo.Append((res[61451]));
		minfo.Append((res[61454]));
		return minfo;

	endwhile

endfunction


function Craft_IndexGump(who, skill_name)

	var menu_file    := menu_config[skill_name].MenuFile;
	var skill_config := ReadConfigFile(menu_file);

	// Now let's get the amount, based on the menu file) from the Cprop.
	atc := craft_settings[skill_name].amount;
	// If it == nothing, or is an error, set it to 1. Error should never occur, but, just in case.
	if( !atc || TypeOf(atc) != "Integer" )
		atc := 1;
	endif
	// Done here, it resets the last item crafted when using the menu.
	rcolor := craft_settings[skill_name].retain;
	makermark := craft_settings[skill_name].mark;

	// Resets the array for objtypes within the menu. Used with
	// The info page and it's buttons.
	arinfo  := array;
	// Button counter for the Info buttons for items. This way, we can seperate craft now buttons
	// from the info buttons. Just remember to make sure any item's button # should NOT be within
	// the 61500 to 62000 range. If you have 50 items, button range will be 61500 to 61550. So please
	// take care with your items.
	arcnt   := 61500;

	var gump := GFCreateGump(50, 25);
	GFResizePic(gump, 2, 0, BKGND_SILVERTRIM, 522, 440);
	GFGumpPic(gump, 10, 10, 2624);
	GFGumpPic(gump, 10, 205, 2624);
	GFGumpPic(gump, 10, 225, 2624);
	GFGumpPic(gump, 250, 10, 2624);
	GFGumpPic(gump, 250, 205, 2624);
	GFGumpPic(gump, 250, 225, 2624);
	GFGumpPic(gump, 10, 33, 5055);
	GFGumpPic(gump, 90, 33, 5055);
	GFGumpPic(gump, 10, 43, 5061);
	GFGumpPic(gump, 90, 43, 5061);
	GFGumpPic(gump, 10, 285, 5055);
	GFGumpPic(gump, 90, 285, 5055);
	GFGumpPic(gump, 10, 330, 5055);
	GFGumpPic(gump, 90, 330, 5055);
	GFHTMLArea(gump, 10, 12, 510, 20, "<basefont color=#ffffff>"+CStr(GetConfigString(skill_config["Index"], "Title")));
	GFHTMLArea(gump, 50, 405, 150, 18, "<basefont color=#ffffff>Exit");
	GFAddButton(gump, 15, 402, 4017, 4019, GF_CLOSE_BTN, 1);

	if( menu_config["Setup"].MakeLast == 1 )
		GFHTMLArea(gump, 305, 405, 150, 18, "<basefont color=#ffffff>Make Last");
		GFAddButton(gump, 270, 402, 4005, 4007, GF_CLOSE_BTN, 61450);
	endif

	GFHTMLArea(gump, 410, 405, 75, 20, "<basefont color=#ffffff>Amount");
	GFGumpPic(gump, 455, 402, 2443);
	atc := GFTextEntry(gump, 465, 405, 100, 30, 1152, atc);

	GFHTMLArea(gump, 10, 302, 150, 20, "<basefont color=#ffffff><center>Notices</center>");
	GFHTMLArea(gump, 10, 37, 200, 20, "<basefont color=#ffffff><center>Categories</center>");
	GFHTMLArea(gump, 215, 37, 305, 20, "<basefont color=#ffffff><center>Selections</center>");

	if( menu_config[skill_name].MakersMark == 1 )
		if( makermark == 1 )
			GFHTMLArea(gump, 305, 345, 150, 18, "<basefont color=#ffffff>Mark Item");
		else
			GFHTMLArea(gump, 305, 345, 150, 18, "<basefont color=#ffffff>*Do Not Mark");
		endif
		GFAddButton(gump, 270, 342, 4005, 4007, GF_CLOSE_BTN, 61453);
	endif
	if( menu_config[skill_name].RetainColor == 1 )
		if( rcolor == 1 )
			GFHTMLArea(gump, 305, 365, 150, 18, "<basefont color=#ffffff>Use Material Color");
		else
			GFHTMLArea(gump, 305, 365, 150, 18, "<basefont color=#ffffff>*Do Not Color");
		endif
		GFAddButton(gump, 270, 362, 4005, 4007, GF_CLOSE_BTN, 61452);
	endif
	if( menu_config[skill_name].Repair == 1 )
		GFHTMLArea(gump, 305, 385, 150, 18, "<basefont color=#ffffff>Repair Item");
		GFAddButton(gump, 270, 382, 4005, 4007, GF_CLOSE_BTN, 61451);
	endif
	if( menu_config["Setup"].Resmelt == 1 )
		if( menu_config[skill_name].Smelt == 1 )
			GFHTMLArea(gump, 50, 385, 150, 18, "<basefont color=#ffffff>Resmelt Item");
			GFAddButton(gump, 15, 382, 4005, 4007, GF_CLOSE_BTN, 61454);
		endif
	endif


	// This foreach builds all the pages for Categories and Selections
	// IndexY = the Y position for lines in the Categories.
	var IndexY := 63;
	// Page Number counter to keep track of pages while building the gump
	PageNum := 1;
	foreach MenuName in GetConfigStringArray(skill_config["Index"], "Group")
		// Add the Category to page 0 for background
		GFPage(gump, "0");
		GFAddButton(gump, 15, (IndexY-3), 4005, 4007, GF_PAGE_BTN, PageNum);
		while (MenuName["_"])
			MenuName["_"] := " ";
			SleepMS(1);
		endwhile
		GFHTMLArea(gump, 50, IndexY, 150, 18, ("<basefont color=#ffffff>"+MenuName));
		// Now let's build the page for this Category
		GFPage(gump, PageNum);
		GFCraftNewPage(gump, skill_config, MenuName);
		// Cycle all the variables up accordingly
//		if( PageNum >= 15 )
    // For each page you wish to add to a category add 5 to the following check.
    // Note: I am not sure if there is a limit to the number of pages you may assign to a gump.
		if( PageNum >= 42 )
			SysLog("Crafting: Max Amount of menus exceeded! Reduce amount of Groups in "+menu_file);
			SendSysMessage(who, "Error! Max Amount of Menus Exceeded. Please report to shard staff.");
			return 0;
		endif
		IndexY := IndexY + 20;
		PageNum := PageNum+1;
	endforeach

	return gump;

endfunction

// Used to draw each additional page to the overall gump
function GFCraftNewPage(byref gump, skill_config, MenuName)

	var ItemX    := 255;
	var ButtonX  := 220;
	var ItemY    := 63;
	var ButtonX2 := 480;
	foreach item in GetConfigStringArray(skill_config[MenuName], "Item")
		var objtype   := GetObjTypeByName(item);
		if(TypeOf(objtype) == "Struct")
			continue;
		endif
		var itemdesc  := GetObjTypeDesc(objtype);

		if(TypeOf(itemdesc) == "Struct")
			continue;
		endif
		itemdesc := Lower(itemdesc);

		if(ItemY >= 243)
			ItemY := 63;
			GFAddButton(gump, 370, 260, 4005, 4007, GF_PAGE_BTN, (PageNum+1));
			GFHTMLArea(gump, 405, 263, 100, 18, "<basefont color=#ffffff>Next Page");
			PageNum := PageNum+1;
			GFPage(gump, PageNum);
			GFAddButton(gump, 220, 260, 4005, 4007, GF_PAGE_BTN, (PageNum-1));
			GFHTMLArea(gump, 255, 263, 100, 20, "<basefont color=#ffffff>Prevoius Page");
		endif
		GFHTMLArea(gump, ItemX, ItemY, 220, 18, ("<basefont color=#ffffff>"+itemdesc));
		GFAddButton(gump, ButtonX, (ItemY-3), 4005, 4007, GF_CLOSE_BTN, objtype);
		if( menu_config["Setup"].Info == 1 )
			GFAddButton(gump, ButtonX2, (ItemY-3), 4011, 4012, GF_CLOSE_BTN, arcnt);
			arinfo.Append(objtype);
			arcnt := arcnt + 1;
		endif
		ItemY := ItemY + 20;
		SleepMS(1);
	endforeach

	return 1;

endfunction

// Used for creating the Info Page gump for an item selected from the
// crafting gump. This gump will display the information about item such
// as skills required, their amt required, materials and their required
// amount per peice, success chances and exceptional chances based on
// skill the character has, show a picture of the item (Non-colored).
// Also says if the item can be exceptionally made and if can have the
// name of it's maker put on the item at GM skill. Can either return to
// the Crafting gump, or create the item immediately from this page.
function GFCraftInfoPage(who, objtype, skill_config)

//	objtype := Hex(objtype);
	var i_config := ReadConfigFile(":*:itemdesc");
	var thisitem := i_config[objtype];
	if(TypeOf(thisitem) != "ConfigElemRef")
		thisitem := i_config[("0x0"+(CStr(objtype) - "0x"))];
	endif
	var elem     := craft_config[objtype];
	var graphic  := CInt(thisitem.graphic);
	var itemdesc := GetObjTypeDesc(objtype);
	itemdesc := Lower(itemdesc);

	if(!graphic || TypeOf(graphic) == "Struct")
		graphic := CInt(objtype);
	endif

//	var tooltip := thisitem.tooltip;
	var tooltip := elem.tooltip;
	if(TypeOf(tooltip) != "String")
		tooltip := 0;
	endif

	// Set the variables for skill required levels
	var skillreq  := elem.skill;
	var diff      := elem.difficulty;
	var skillreqb := elem.skill2;
	if(!skillreqb || skillreqb.errortext)
		skillreqb  := 0;
	endif
	// Set up skill 2 if one required
	var aid_skill := elem.skillID;
	// Get their xxx.x skill amount
	var temp1 := CDbl(AP_GetSkill(who, craft_skill));
	// Get their xxx.x skill amount of the secondary skill entry
	var temp2 := 0;
	if( TypeOf(craft_config[objtype].skillID) == "String" )
		temp2 := CDbl(AP_GetSkill(who, craft_config[objtype].skillID));
	endif

	// If there was no secondary skill in the cfg entry, set it to main skill
	if(temp2 == error)
		temp2 := temp1;
	endif
	// Figure the sickening odd Success Chance percentages.
	var schance   := 0.0;
	if((TypeOf(aid_skill) == "Integer") && (skillreqb != 0))
		if(TypeOf(skillreqb) == "Double" or TypeOf(skillreqb) == "Integer")
			// Although CraftCheckSkill() would return 0%, let's not run all that code
			// since it is a given anyway.
			if(skillreqb > temp2)
				schance := "0%";
			else
				schance := CStr(CraftCheckSkill(who, craft_skill, diff, 0, 1))+"%";
			endif
		endif
	else
		if(skillreq > temp1)
			schance := "0%";
			SendSysMessage(who, "0? HOW!? Skill: " + skillreq + " : Cfg : "+temp1);
		else
			schance := CStr(CraftCheckSkill(who, craft_skill, diff, 0, 1))+"%";
		endif
	endif
	if(elem.placeholder == 1)
		schance := "100%";
	endif
	if(TypeOf(aid_skill) != "String")
		aid_skill := 0;
	endif

	// Get the material type required to craft the item.
	var matname   := Lower(elem.type);

	if( TypeOf(elem.mtype) == "String" )
		matname := elem.mtype;
	endif

	// Get the materials amount required to craft the item
	var material  := elem.material;

	if((elem.material2 > 0) && (elem.material2 != error))
		// Material 2 name
		matname := matname+"\n"+elem.material2obj;
		material := material+"\n"+elem.material2;
	endif

	// Below added for checking for parts. If parts are present, then create the part list
	// instead.
	var tmatname  := 0;
	var tmpmat    := "";
	if(TypeOf(elem.parts) == "String")
		tmpmat := SplitWords(elem.parts);
		matname  := "";
		material := "";
		if((Len(tmpmat)/2) < 5)
			for j := 1 to (Len(tmpmat)/2)
				tmatname := GetObjTypeDesc(CInt(tmpmat[j]));
				case(j)
					1:
					2:
					3: matname := matname + tmatname+"\n";
					4: matname := matname + tmatname;
					default: matname := matname + tmatname+"\n";
				endcase
				material := material + (tmpmat[j+(Len(tmpmat)/2)])+"\n";
			endfor
		else
			material := 0;
			for j := 1 to (Len(tmpmat)/2)
				tmatname := GetObjTypeDesc(CInt(tmpmat[j]));
				if(j == (Len(tmpmat)/2))
					matname := matname+"& "+(tmpmat[j+(Len(tmpmat)/2)])+" "+tmatname+" required";
				else
					matname := matname + (tmpmat[j+(Len(tmpmat)/2)])+" "+tmatname+", ";
				endif
			endfor
		endif
	endif
	// Exception % chance wording.
	var echance   := 0;
	if(elem.Exceptional == 1)
		echance := CStr(ExceptionalCraft(temp1, skillreq, temp2, 1))+"%";
	endif

	var gump := GFCreateGump(50, 25);
	GFResizePic(gump, 2, 0, BKGND_SILVERTRIM, 521, 420);
	GFGumpPic(gump, 10, 10, 2624);
	GFGumpPic(gump, 10, 205, 2624);
	GFGumpPic(gump, 250, 10, 2624);
	GFGumpPic(gump, 250, 205, 2624);
	GFGumpPic(gump, 10, 30, 5055);
	GFGumpPic(gump, 90, 30, 5055);
	GFResizePic(gump, 160, 120, 5054, 356, 13);
	GFGumpPic(gump, 10, 210, 5055);
	GFGumpPic(gump, 90, 210, 5055);
	GFGumpPic(gump, 10, 290, 5055);
	GFGumpPic(gump, 90, 290, 5055);
	GFGumpPic(gump, 10, 370, 5055);
	GFGumpPic(gump, 90, 370, 5055);
	GFHTMLArea(gump, 10, 27, 150, 18, "<basefont color=#ffffff>ITEM");
	GFHTMLArea(gump, 170, 117, 150, 18, "<basefont color=#ffffff>SKILLS");
	GFHTMLArea(gump, 10, 207, 150, 18, "<basefont color=#ffffff>MATERIALS");
	GFHTMLArea(gump, 10, 287, 150, 18, "<basefont color=#ffffff>OTHER");
	GFAddButton(gump, 15, 387, 4014, 4016, GF_CLOSE_BTN, 1);
	GFHTMLArea(gump, 50, 390, 150, 18, "<basefont color=#ffffff>BACK");
	GFAddButton(gump, 270, 387, 4005, 4007, GF_CLOSE_BTN, CInt(objtype));
	GFHTMLArea(gump, 305, 390, 150, 18, "<basefont color=#ffffff>MAKE NOW");
	GFTilePic(gump, 20, 50, CInt(graphic));
	GFHTMLArea(gump, 430, 132, 150, 18, ("<basefont color=#ffffff>"+skillreq));

	// Tooltip info from itemdesc.cfg appended in.
	if(tooltip != 0)
		GFHTMLArea(gump, 170, 300, 300, 40, ("<basefont color=#ffffff>"+tooltip));
	endif
	// Set up the text for if the item can retain the color of material
	if(craft_config[objtype].retain == 1)
		GFTextLine(gump, 500, 220, 1153, "*");
		GFTextLine(gump, 170, 335, 1153, "* The item retains the color of the material");
	endif
	if(craft_config[objtype].mark == 1)
		GFTextLine(gump, 170, 355, 1153, "This item may hold its maker's mark");
	endif
	// Material Required
	GFHTMLArea(gump, 170, 219, 300, 80, ("<basefont color=#ffffff>"+matname));
	if(material != 0)
		GFHTMLArea(gump, 430, 219, 20, 80, ("<basefont color=#ffffff>"+material));
	endif
	// Success Chance % text
	GFTextLine(gump, 170, 80, 1153, "Success Chance:");
	GFTextLine(gump, 430, 80, 1153, schance);
	// Excpetional Chance % text
	if(echance != 0)
		GFTextLine(gump, 170, 100, 1153, "Exceptional Chance:");
		GFTextLine(gump, 430, 100, 1153, echance);
	endif
	// Title of the menu
	GFHTMLArea(gump, 10, 12, 510, 20, ("<basefont color=#ffffff>"+CStr(GetConfigString(skill_config["Index"], "Title"))));
	// Item Name
	GFHTMLArea(gump, 330, 40, 150, 20, ("<basefont color=#ffffff>"+itemdesc));
	// Skill name
	GFHTMLArea(gump, 170, 132, 150, 20, ("<basefont color=#ffffff>"+display_skill+" required"));

	if((skillreqb > 0) && (skillreqb != error))
		// Skill 2 name
		GFHTMLArea(gump, 170, 152, 150, 20, ("<basefont color=#ffffff>"+Lower(aid_skill)+" required"));
		GFHTMLArea(gump, 430, 152, 150, 20, ("<basefont color=#ffffff>"+skillreqb));
	endif

	var ifres := GFSendGump(who, gump);

	if(ifres[0] == 1)
		return 0;
	else
		return ifres[0];
	endif

endfunction


function cBaseCraftDict()

	var baseDict  := dictionary;
	foreach skillentry in GetConfigStringArray(menu_config["Skills"], "Skill")
		var temp := dictionary;
		temp.Insert( "skill", menu_config[skillentry].Attribute );
		temp.Insert( "amount", CInt(menu_config["Setup"].DefaultAmount) );
		temp.Insert( "mark", CInt(menu_config[skillentry].MakersMark) );
		temp.Insert( "retain", CInt(menu_config[skillentry].RetainColor) );
		temp.Insert( "last", 0 );

		baseDict.Insert( menu_config[skillentry].Attribute, temp );
	endforeach

	return baseDict;

endfunction

function Craft_RetrieveGump(elem_name, prop_name)

        var elem := DFFindElement(craft_gumps, elem_name);
        return (elem.GetProp(prop_name));

endfunction

function Craft_SaveGump(elem_name, gump, prop_name)

        var elem := DFFindElement(craft_gumps, elem_name, DF_CREATE);
        return (elem.SetProp(prop_name, gump));

endfunction
