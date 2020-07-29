use uo;

function TrainingSkill(character, skillid)
	var skill := GetObjProperty(character, "LastUsedSkill");
	if(!skill)
		skill := array {};
	endif

	skill[skillid+1] := ReadGameClock() + 259200;
	SetObjProperty(character, "LastUsedSkill", skill);
endfunction