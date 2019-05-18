include ":attributes:attributes";


//
// CheckToolWear (who, tool, attributeID)
//
// Purpose
// Checks to see if a tool has broken.
//
//
// Parameters
// who: The character using the tool.
// tool: The tool being used.
// attributeID: The attribute name that applies to the tool
// being used, eg. a saw uses the attribute carpentry.
//
//
// Return value
// Returns a 0 if the tool "broke" or a 1 if the tool did not break.
//
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
		return 0;
	else
		var skill := AP_GetSkill(who, attributeID);
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
					return 0;
				endif
			else
				SetObjProperty(tool,"swings", (swings - 1));
				return 1;
			endif
		endif
	endif
endfunction

//
// SetToolDurability(who, tool, skillid)
//
// Purpose
// What does it do?
//
//
// Parameters
// List of parameters and their type to be passed to the function
// and if necessary a small description of each parameter
//
//
//
//
// Return value
// Return value and its type with a small description if necessary.
// If the function returns error values list them as well
//
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
