include ":attributes:attributes_ex";

function CheckToolWear (who, tool, attributeID)
	var swings := GetObjProperty(tool,"swings");
	if ( !swings )
		var mods := GetObjProperty(tool, "durability");
		var bonus := 0;
		case( mods )
			"durable":		bonus := 10; break;
			"substantial":		bonus := 20; break;
			"massive":		bonus := 30; break;
			"fortified":		bonus := 40; break;
			"indestructable": 	bonus := 50; break;
		endcase
		swings := Cint(50 * tool.quality) + bonus;
		SetObjProperty(tool,"swings", swings);
	endif
	if ( swings <= 0 )
		DestroyItem(tool);
		SendSysMessage(who,"Your tool broke.");
	else
		var skill := GetAttribute(who, attributeID);
		if ( skill > 80 )
			skill := 80;
		elseif ( skill < 20 )
			skill := 20;
		endif
		if ( (RandomInt(100)+1) > skill )
			if ( tool.isa(POLCLASS_WEAPON))
				if ( tool.hp > 1 )
					SetObjProperty(tool,"swings", (swings - 1));
					tool.hp := tool.hp - 1;
				elseif ( (tool.hp > tool.maxhp) or (tool.hp == 1) )
					SendSysMessage(who,"Your tool broke.");
					DestroyItem(tool);
				endif
			else
				SetObjProperty(tool,"swings", (swings - 1));
			endif	 
		endif
	endif
endfunction 

function SetToolDurability(who, tool, skillid)

	var toolfile := ReadConfigFile(":*:itemdesc");
	var toolscript := toolfile[tool.objtype].Script;

	if ( !toolscript.errortext )
		if ( !tool.isA(POLCLASS_CONTAINER) && !tool.isA(POLCLASS_ARMOR) )
			var myskill := GetEffectiveSkill(who, skillid);
			var swings := 0;
			var mods := GetObjProperty(tool, "durability");
			var bonus := 0;
			case( mods )
				"durable":		bonus := 10; break;
				"substantial":		bonus := 20; break;
				"massive":		bonus := 30; break;
				"fortified":		bonus := 40; break;
				"indestructable": 	bonus := 50; break;
				default: bonus := 0;
			endcase
			swings := Cint(50 + myskill) + bonus;
			SetObjProperty(tool, "swings", swings);
			if ( !tool.quality or tool.quality == error )
				tool.quality := 1.0;
			endif
		endif
	endif

endfunction 
