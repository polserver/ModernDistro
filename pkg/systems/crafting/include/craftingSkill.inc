// //$Id: craftingSkill.inc 1487 2009-03-13 19:19:37Z muaddib_pol $

// This is a modified version of the ShilCheckSkill export. The purpose
// of this function is to mimic the OSI % chance system crafting uses.
function CraftCheckSkill(who, craft_skill, difficulty, points, report:=0)

	difficulty := CDbl(difficulty);
	points := CDbl(points);
	if(difficulty == error)
		return 0;
	elseif(difficulty == 0.0)
		return 100;
	endif
	var chance;
	var GainCap := 1200;
	// I'm Lazy. Change this later.
	var effective := CDbl(AP_GetSkill(who, craft_skill));
	if ( !effective || effective.errortext )
		effective := 1.0;
	endif
	var hunger  := CInt(GetObjProperty(who, "hunger"));
	if(!hunger || hunger.errortext)
		hunger := 2;
	endif
	case(hunger)
		0: effective := (effective - 10.1); points := (points * 0.95);
		1: effective := (effective + 20.1); points := (points * 1.05);
		5:
		6: effective := (effective - 15.1); points := (points * 0.95);
		7:
		8: effective := (effective - 20.1); points := (points * 0.97);
		9:
		10: effective := (effective - 40.1); points := (points * 0.95);
	endcase
	if(effective < 0)
		effective := 0.0;
	endif
	chance := (effective - difficulty) * 2.0 * 10.0;

	var GainCheck := chance;
	if(chance < 0)
		chance := 0.0;
	elseif(chance > 1000)
		chance := 1000.0;
	endif

	if(report)
		if ( CDbl(chance/10.0) < 1.0 && CDbl(chance/10.0) > 0.0 )
			return 1;
		else
			return CDbl(chance/10.0);
		endif
	endif

	playCraftWork(who, craft_skill, CRAFT_CYCLE_ATTEMPT);

	var temp_chance;
	temp_chance := (effective - difficulty) * 2.5 + 500;
	if(temp_chance < 0)
		temp_chance := 0;
	elseif(temp_chance > 1000)
		temp_chance := 1000;
	endif
	if(points != 0)
		points  := points * ((1000-temp_chance) / 1000);
	endif
	if(GainCheck >= GainCap)
		points := 0;
	endif
	if ( CDbl(AP_GetSkill(who, craft_skill)) < 35.0 )
		points := points * 2;
		chance := 100;
	endif

	if(RandomInt(1000) < chance)
		if(points > 0)
			CheckSkillAdvance(who, craft_skill, points, chance);
		endif
		return 1;
	else
		if(effective < 10)
			if(points > 0)
				chance := chance / 2;
				points := points / 2;
				CheckSkillAdvance(who, craft_skill, points, chance);
			endif
		endif
		return 0;
	endif

	return 1;

endfunction

// This is a modified version of the CraftCheckSkill. The purpose
// of this function is to mimic the OSI % chance system exceptional.
// This system is, .2% chance per .1 skill above the required skill (+30)
// to craft the item as exceptional. Aidskill has been added to help
// add a custom method for POL on this. Aidskills would be either
// arms lore (as in with blacksmithy), or otherwise it is the
// Secondary skill required to craft the item. See the config files
// In the crafting pkgs for their layouts and the defining of them.
function ExceptionalCraft(skill, reqd, aidskill:=0, report:=0);

	if(!aidskill or aidskill.errortext)
		aidskill := skill;
	endif
	skill := CDbl(skill);
	reqd := CDbl(reqd) + 30.0;
	var excmod := skill + (aidskill/3.3333333333333333333333333333333);
	var chance := (excmod - reqd) * 2.0 * 10.0;
	if(chance < 0)
		chance := 0;
	elseif(chance > 1000)
		chance := 1000;
	endif
	if(report)
		return CDbl(chance/10.0);
	endif
	if(RandomInt(1000) < chance)
		return 1;
	else
		return 0;
	endif

endfunction
