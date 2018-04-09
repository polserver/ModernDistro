// //$Id: toolWear.inc 1472 2009-03-12 00:46:41Z muaddib_pol $

function CheckToolWear (who, tool, craft_skill)
	var swings := GetObjProperty(tool,"swings");
	if(!swings)
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
					SetObjProperty(tool,"swings", (swings - 1));
					tool.hp := tool.hp - 1;
				elseif((tool.hp > tool.maxhp) or (tool.hp == 1))
					DestroyItem(tool);
				endif
			else
				SetObjProperty(tool,"swings", (swings - 1));
			endif
		endif
	endif
endfunction

function SetToolDurability(who, tool, craft_skill)

	var toolfile := ReadConfigFile(":*:itemdesc");
	var toolscript := toolfile[tool.objtype].Script;

	if ( TypeOf(toolscript) == "String" )
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

endfunction
