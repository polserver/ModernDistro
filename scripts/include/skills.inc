use attributes;

include "include/client";
include ":attributes:attributes";

Const MAX_SKILL_BONUS := 20;




///////////////////
//  Gets the name of the skill
///////////////////

function GetSkillName (skillid)
//	if (!skillid and skillid != 0)
//		return "None";
//	endif

	var skillmastercfg := ReadConfigFile(":attributes:skillmaster");
	var skillelem := FindConfigElem(skillmastercfg, skillid);
	if (skillelem.desc)
		return skillelem.desc;
	else
		return "None";
	endif
endfunction




///////////////////
//  Gets the character title for that skill
///////////////////

function GetSkillTitle (skillid)
	var skillmastercfg := ReadConfigFile(":attributes:skillmaster");
	var skillelem := FindConfigElem(skillmastercfg, skillid);
	if (skillelem.title)
		return skillelem.title;
	else
		return "";
	endif
endfunction




///////////////////
//  Creates an array with the character's specializations.  Returns 0 if they haven't set
//  their specializations
///////////////////

function LoadSpecializations (character)
	if (!GetObjProperty (character, "primary1"))
		if (GetObjProperty (character, "primary1") != 0)
			return array {};
		endif
	endif

	var myspecs := array {};
	myspecs[1] := GetObjProperty (character, "primary1");
	myspecs[2] := GetObjProperty (character, "primary2");
	myspecs[3] := GetObjProperty (character, "primary3");
	myspecs[4] := GetObjProperty (character, "secondary1");
	myspecs[5] := GetObjProperty (character, "secondary2");
	myspecs[6] := GetObjProperty (character, "secondary3");
	myspecs[7] := GetObjProperty (character, "tradeskill");
	myspecs[8] := GetObjProperty (character, "compskill");
  

	return myspecs;
endfunction




///////////////////
//  Figures out if we're already specialized in a skill.  If so, returns what position (1-7)
///////////////////

function CheckIfSpecialized (character, skillid)
	var myspecs := LoadSpecializations (character);
	for i := 1 to 8
		if (myspecs[i] == skillid)
			return i;
		endif
	endfor
	return 0;
endfunction


///////////////////
//  Determines if the given skill is a trade skill
///////////////////

function IsTradeSkill(skillid)
	var skillmastercfg := ReadConfigFile(":attributes:skillmaster");
	var skillelem := FindConfigElem(skillmastercfg, skillid);
	if (skillelem.tradeskill)
		return 1;
	else
		return 0;
	endif
endfunction





///////////////////
//  Launches the script associated with a given skill
///////////////////

function LaunchSkillScript (byref character, skillid)
	if (!skillid and skillid != 0)
		return;
	endif
	
	SetObjProperty (character, "lastskill", skillid);
	if (character.paralyzed)
		SendSysmessage (character, "You are paralyzed and cannot do that.");
		return;
	endif

	if (skillid != SKILLID_STEALTH and skillid != SKILLID_HIDING)
		character.hidden := 0;
	endif

	var skillmastercfg := ReadConfigFile (":attributes:skillmaster");
	var skillelem := FindConfigElem (skillmastercfg, skillid);
	if (skillelem.script)
		start_script (skillelem.script, character);
	else
		sendsysmessage (character, "Skill assigned to incorrect script!");
	endif
endfunction




///////////////////
//  Gets a nice looking number for skill display (25.6, for example, instead of 256)
///////////////////

function GetSkillValDisplay (byref character, skillid)
	if (!skillid and skillid != 0)
		return "";
	endif
	
	var attribute := GetAttributeIDBySkillID (skillid);
	if (!attribute)
		return "";
	endif

	var str := CSTR (GetAttributeBaseValue (character, attribute) + GetAttributeTemporaryMod (character, attribute));
	if (!str or !len (str))
		return "0.0";
	endif
	
	str [len(str), 1] := "."+ str [len(str), 1];
	if (CDBL (str) < 1)
		str := "0" + str;
	endif
	return padstring (str, 6);
endfunction





///////////////////
//  called when someone equips an item with a skill mod
///////////////////

function EquipItemWithSkillMod (byref character, byref item)
	if (!character or !item)
		return;
	endif

	//DEBUG - temporary code for the conversion to POL 0.94	
	EraseObjProperty (character, "allskillmods");
	EraseObjProperty (character, "tempskillmods");

	var skillid := GetObjProperty (item, "skilladv");
	if (!skillid and skillid != 0)
		return;
	endif

	var skill_bonus := GetObjProperty (item, CStr("skill"+skillid));
	if (!skill_bonus)
		return;
	endif

	set_critical (1);
	var allskillmods := GetObjProperty (character, "#allskillmods");
	if (!allskillmods)
		allskillmods := {};
	endif

	var previous_skill_bonus := 0;	//skill bonus for items already equipped
	var maximum_skill_bonus := MAX_SKILL_BONUS;	//So we can have a single special item that takes us above
										//the normally allowed bonus
	foreach skillmod in allskillmods
		//we've already got the skill bonus for this item, so just return
		if (skillmod[1] == item.serial)
			set_critical (0);
			return;
		endif

		//if we already have an item equiped that gives us this skill...
		if (skillmod[2] == skillid)
			previous_skill_bonus := previous_skill_bonus + skillmod[3];
			if (skillmod[3] > maximum_skill_bonus)
				maximum_skill_bonus := skillmod[3];
			endif
		endif
	endforeach

	var new_skill_mod := array{};
	new_skill_mod [1] := item.serial;
	new_skill_mod [2] := skillid;
	new_skill_mod [3] := skill_bonus;

	allskillmods.append (new_skill_mod);
	SetObjProperty (character, "#allskillmods", allskillmods);
	set_critical (0);
	
	if (skill_bonus > maximum_skill_bonus)
		maximum_skill_bonus := skill_bonus;
	endif

	//Set the maximum amount of skill bonus that you can get
	if (previous_skill_bonus)
		if (previous_skill_bonus + skill_bonus > maximum_skill_bonus)
			skill_bonus := maximum_skill_bonus - previous_skill_bonus;
			if (skill_bonus <= 0)
				return;
			endif
		endif
	endif
	AddSkillBonus (character, skillid, skill_bonus);
endfunction




///////////////////
//  called when someone unequips an item with a skill mod
///////////////////

function UnequipItemWithSkillMod (byref character, byref item)
	if (!character or !item)
		return;
	endif

	var skillid := GetObjProperty (item, "skilladv");
	if (!skillid and skillid != 0)
		return;
	endif

	var skill_bonus := GetObjProperty (item, CStr("skill"+skillid));
	if (!skill_bonus)
		return;
	endif

	set_critical (1);
	var allskillmods := GetObjProperty (character, "#allskillmods");
	if (!allskillmods)
		//we don't have any skillmods.  Why are we trying to remove one?
		set_critical (0);
		return;
	endif

	//we need to make a new allskillmods list, without the one we're trying to remove, and
	//also figure out what other skill bonuses we have
	var new_allskillmods := array{};
	var other_skill_bonus := 0;
	var maximum_skill_bonus := MAX_SKILL_BONUS;	//So we can have a single special item that takes us above
										//the normally allowed bonus

	foreach skillmod in allskillmods
		if (skillmod [1] != item.serial)
			new_allskillmods.append (skillmod);
			if (skillmod[2] == skillid)
				other_skill_bonus := other_skill_bonus + skillmod[3];
			endif
		endif
		if (skillmod[3] > maximum_skill_bonus)
			maximum_skill_bonus := skillmod[3];
		endif
	endforeach

	//well, we went through them all and didn't remove anything, so I guess
	//we didn't really have that item equiped in the first place...
	if (len (allskillmods) == len (new_allskillmods))
		set_critical (0);
		return;
	endif

	if (len (new_allskillmods) )
		SetObjProperty (character, "#allskillmods", new_allskillmods);
	else
		EraseObjProperty (character, "#allskillmods");
	endif
	set_critical (0);
	
	if (other_skill_bonus + skill_bonus > maximum_skill_bonus)
		skill_bonus := maximum_skill_bonus - other_skill_bonus;
		if (skill_bonus <= 0)
			return;
		endif
	endif
	AddSkillBonus (character, skillid, 0-skill_bonus);
endfunction




///////////////////
// undoes all skill bonuses
///////////////////

function EraseAllSkillBonuses (byref character)
	set_critical (1);
	for skillid := 0 to 57
		var skillname := GetAttributeIDBySkillID (skillid);
		SetAttributeTemporaryMod (character, skillname, 0);
	endfor

	EraseObjProperty (character, "#allskillmods");
	EraseObjProperty (character, "#tempskillmods");
	set_critical (0);
endfunction




///////////////////
//  determines the character's skillcap for the given skill
///////////////////

function GetBaseSkillCap (byref me, skillid)
	if (!me.acctname)
		return 0;
	endif

	if (skillid == GetObjProperty (me, "primary1"))
		return 1000;
	elseif (skillid == GetObjProperty (me, "tradeskill"))
		return 1000;
	elseif (skillid == GetObjProperty (me, "primary2"))
		return 1000;
	elseif ( skillid == GetObjProperty (me, "primary3") )
		return 1000;
	elseif ( skillid == GetObjProperty (me, "compskill") )
		return 900;
	elseif ( skillid == GetObjProperty (me, "secondary1") )
		return 800;
	elseif ( skillid == GetObjProperty (me, "secondary2") )
		return 800;
	elseif ( skillid == GetObjProperty (me, "secondary3") )
		return 800;
	else
		return 600;
	endif
endfunction




///////////////////
//  figures out how much of a skill bonus a character has in a given skill
///////////////////

function DetermineSkillMod (byref character, skillid)
	set_critical (1);

	var allskillmods := GetObjProperty (character, "#allskillmods");
	if (!allskillmods)
		allskillmods := array;
	endif

	var first_skill_positive_bonus := 0;
	var first_skill_negative_bonus := 0;
	foreach skillmod in allskillmods
		if (skillmod[2] == skillid)
			first_skill_positive_bonus := first_skill_positive_bonus + skillmod[3];
		elseif (skillmod[2] - 100 == skillid)
			first_skill_negative_bonus := first_skill_negative_bonus + skillmod[3];
		endif
	endforeach

	var tempskillmods := GetObjProperty (character, "#tempskillmods");
	if (!tempskillmods)
		tempskillmods := array;
	endif

	var second_skill_positive_bonus := 0;
	var second_skill_negative_bonus := 0;
	foreach skillmod in tempskillmods
		if (skillmod [1] == skillid)
			second_skill_positive_bonus := second_skill_positive_bonus + skillmod[2];
		elseif (skillmod[1]-100 == skillid)
			second_skill_negative_bonus := second_skill_negative_bonus + skillmod[2];
		endif
	endforeach

	set_critical (0);
	var skill_bonus := (first_skill_positive_bonus - first_skill_negative_bonus
			+ second_skill_positive_bonus - second_skill_negative_bonus);
	return skill_bonus;
endfunction




///////////////////
//  for skill mods that last a specified amount of time
///////////////////

function DoSkillMod (byref character, skillid, amount, duration)
	set_critical(1);
	var tempskillmods := GetObjProperty (character, "#tempskillmods");
	if (!tempskillmods)
		tempskillmods := array;
	endif

	var newmod := array {};
	newmod[1] := skillid;
	newmod[2] := amount;
	newmod[3] := ReadGameClock()+duration;

	//if we already had a skillmod for this skill, the function returns 0, and we don't add any more skill
	var give_skill := 0;
	if (AddSkillModToPreviousMods (tempskillmods, newmod))
		give_skill := 1;
	endif

	SetObjProperty (character, "#tempskillmods", tempskillmods);
	if (give_skill)
		AddSkillBonus (character, skillid, amount);
	endif
	set_critical(0);

	if (!character.npctemplate)
		return;
	endif

	//if its an NPC, sleep, start an external script
	var parms := array {};
	parms[1] := character;
	parms[2] := skillid;
	parms[3] := amount;
	parms[4] := duration;
	parms[5] := "skillmod";
	start_script("::/misc/unmodnpc", parms);
endfunction

function RemoveNPCSkillMod (byref parms)
	var who := parms[1];
	var prop := parms[2];
	var amount := parms[3];
	var duration := parms[4];
  if (amount and prop)
  endif
	sleep (duration + 1);
	if (who)
		CheckSkillMods (who);
	endif
endfunction



///////////////////
//  adds it to the previous skill mods.  If there's already a mod for this skill, just extend the duration
///////////////////

function AddSkillModToPreviousMods (byref tempskillmods, byref newmod)
	foreach skillmod in tempskillmods
		if (skillmod [1] == newmod[1])
			//we already have a tempmod in this skill, so just extend the duration
			skillmod[3] := newmod[3];
			return 0;
		endif
	endforeach

	tempskillmods.append (newmod);
	return 1;
endfunction




///////////////////
//  checks to see if some of the timed skillmods have worn off, and removes them if they have
///////////////////

function CheckSkillMods (byref character)
	set_critical (1);
	var tempskillmods := GetObjProperty (character, "#tempskillmods");

	//if we don't have any, don't worry about it
	if (!tempskillmods)
		EraseObjProperty (character, "#tempskillmods");
		set_critical (0);
		return;
	endif

	var newmods := array;
	foreach skillmod in tempskillmods
		if (skillmod[3] < ReadGameClock())
			AddSkillBonus (character, skillmod[1], 0-skillmod[2]);
		else
			newmods.append (skillmod);
		endif
	endforeach

	SetObjProperty (character, "#tempskillmods", newmods);
	set_critical (0);

endfunction




///////////////////
//  Adds the specified amount of skill to a character.  To remove skill, pass a negative skill_bonus
///////////////////

function AddSkillBonus (character, skillid, skill_bonus)
	//done to be able to stack blesses and curses
	if (skillid >= 100)
		skillid := skillid - 100;
		skill_bonus := 0-skill_bonus;
	endif

	var skillname := GetAttributeIDBySkillID (skillid);

	var skill_mod := GetAttributeTemporaryMod (character, skillname) + skill_bonus*10;
	SetAttributeTemporaryMod (character, skillname, skill_mod);
endfunction


