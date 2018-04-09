// //$Id: toolWearMethod.inc 453 2006-06-26 19:05:31Z austinheilman $

include ":attributes:attributes";

exported function CheckToolWear (tool, who, craft_skill)

	var swings := GetObjProperty(tool,"swings");
	if( TypeOf(swings) != "Integer" )
		var mods := GetObjProperty(tool, "durability");
		var bonus := 0;
		case(mods)
			"durable":        bonus := 10;
			"substantial":    bonus := 20;
			"massive":        bonus := 30;
			"fortified":      bonus := 40;
			"indestructable": bonus := 50;
		endcase
		swings := Cint(50 * tool.quality) + bonus;
		SetObjProperty(tool,"swings", swings);
	endif
	if(swings <= 0)
		DestroyItem(tool);
		SendSysMessage(who,"Your tool broke.");
	else
		var skill := AP_GetSkill(who, craft_skill);
		if(skill > 80)
			skill := 80;
		elseif(skill < 20)
			skill := 20;
		endif
		if((RandomInt(100)+1) > skill)
			if(tool.IsA(POLCLASS_WEAPON))
				if(tool.hp > 1)
					SetObjProperty(tool,"swings", CInt(swings - 1));
					tool.hp := tool.hp - 1;
				elseif((tool.hp > tool.maxhp) or (tool.hp == 1))
					SendSysMessage(who, "Your tool broke.");
					DestroyItem(tool);
				endif
			else
				SetObjProperty(tool, "swings", CInt(swings - 1));
			endif
		endif
	endif

endfunction

exported function SetToolDurability(tool, who, craft_skill)

	var item_config   := ReadConfigFile(":*:itemdesc");
	var item_script  := item_config[tool.objtype].Script;

	if( TypeOf(item_script) != "Struct" )
		if(!tool.isA(POLCLASS_CONTAINER) && !tool.isA(POLCLASS_ARMOR))
			var myskill := AP_GetSkill(who, craft_skill);
			var swings := 0;
			var mods := GetObjProperty(tool, "durability");
			var bonus := 0;
			case(mods)
				"durable":        bonus := 10;
				"substantial":    bonus := 20;
				"massive":        bonus := 30;
				"fortified":      bonus := 40;
				"indestructable": bonus := 50;
				default: bonus := 0;
			endcase
			swings := Cint(50 + myskill) + bonus;
			SetObjProperty(tool, "swings", swings);
			if(!tool.quality or tool.quality == error)
				tool.quality := 1.0;
			endif
		endif
	endif

	return 1;

endfunction
