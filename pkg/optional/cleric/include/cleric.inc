use uo;
use cfgfile;
use os;

include ":cleric:faiths";
include ":attributes:attributes";

//Special Effects
const CAST_ANIM := 17;
const CHANT_SOUND := 587;

//Meditation skill
const CLERIC_SKILL := MEDITATION;

//Focci
const POWER_FOCUS := 0x2F014;
const SPIRIT_FOCUS := 0x2F015;
const ARCANE_FOCUS := 0x2F016;
const TIME_FOCUS := 0x2F017;
const EARTH_FOCUS := 0x2F018;
const LIFE_FOCUS := 0x2F019;

//Cleric tools
const CLERIC_BOOK := 0x2F903;
const HP_SYMBOL := 0x2F904;

//Glyphs
const GLYPH_OF_WARDING := 0x2F907;
const SYMBOL_OF_PAIN := 0x2F908;



function RiteInfo(elem)
	// Send a number: Return the rite.
	var config := ReadConfigFile(":cleric:rites");
	return config[elem];
endfunction

function SacramentInfo(elem)
	var config := ReadConfigFile(":cleric:sacraments");
	return config[elem];
endfunction

function DevotionType(what, rite)
	//Tricky function to use. (what, riteinfo(num))
	//Gets the devotion prop from what, and looks for it in the rite.
	if (!GetConfigString(rite, "Devotion") || GetObjProperty(what, "Devotion") == "Faithless")
		return 1;
	endif
	foreach devotion in GetConfigStringArray(rite, "Devotion")
		if (GetObjProperty(what, "Devotion") == devotion)
			return 1;
		endif
	endforeach
	return 0;
endfunction

function DevotionPiety(devotion, who:=0)
//Get the number of people online that are not GMs
//and the number in that cleric's faith to determine
//faith wide piety.
//Piety = Int(OnLine_InFaith characters / Total OnLine characters * 10)
//If Piety < 1 then Piety := 1
	var piety := 0;
	var online := 0;
	var infaith := 0;
// svar sects := getsects(devotion);
	foreach player in EnumerateOnlineCharacters()
		if (player.cmdlevel < 2)
			if (GetObjProperty(player, "Devotion") == devotion)
				infaith := infaith + 1;
			endif
			online := online+1;
		endif
	endforeach
	if (online == 0)
		online := online + 1;
	endif
	piety := CDbl(infaith)/CDbl(online);
	piety := piety*10;
	// The following line is for future activation of a CProp of BasePiety to be added to clerics
	piety := piety + GetObjProperty(who, "BasePiety");
	if (piety < 1)
		piety := 1;
	endif
	// Now let's round up piety
	if((piety - CInt(piety)) >= 0.5)
  		piety := (CInt(piety) + 1);
	else
  		piety := CInt(piety);
	endif
	// Call HPBoost to see if the caster is the HP (if the caster has the HP symbol equipped)
	piety := piety+hpboost(who);
	// Okay we now have the true piety so let's return with it
	return piety;
endfunction

function hpboost(who)
	// Looks for HP_SYMBOL equipped on caster
	var owner, devotion;
	var is_hp := IsHP(who);
	foreach item in ListEquippedItems(who)
		if (item.objtype == HP_SYMBOL)
			devotion := GetObjProperty(item, "Devotion");
			owner := GetObjProperty(item, "Owner");
			if (who.serial == owner && is_hp && GetObjProperty(who, "Devotion") == devotion)
				return 1;
			endif
		endif
	endforeach
	return 0;
endfunction

function IsHP(who)
	var devotion := GetObjProperty(who, "Devotion");
	var faith := Faiths_FindFaith( devotion );
	if(faith.getprop("HighPriest") == who.serial)
		return 1;
	endif
	return 0;
endfunction

function GetConcentratingClerics(who, range:=5, method := "piety", matchreligion := 0)
	// Finds all the clerics concentrating on the who cleric
	// in the defined range. Returns their combined piety.
	var piety;
	var clericlist := { };
	foreach person in ListMobilesInLineOfSight(who, range)
		if (!matchreligion || GetObjProperty(person, "Devotion") == GetObjProperty(who, "Devotion"))
			if(GetObjProperty(person, "#concentrating") == who.serial && person != who)
				clericlist.append(person);
				piety := piety + DevotionPiety(GetObjProperty(person, "Devotion"));
				SendSysMessage(who, person.name + " is concentrating on you.");
			endif
		endif
	endforeach

	if (method["piety"])
		return piety;
	else
		return clericlist;
	endif
endfunction

function ClericalTarget(byref who, byref npctarget, prompt := "Select a target", ops := TGTOPT_CHECK_LOS + TGTOPT_NEUTRAL)
	var x := who.x;
	var y := who.y;
	who.hidden := 0;
	if (npctarget)
		return npctarget;
	endif

	if (who.isA(POLCLASS_NPC))
		return;
	endif

	SendSysMessage(who, prompt);

	var targ := Target(who, ops);

	if (who.x == x && who.y == y)
		return targ;
	else
		SendSysMessage(who, "You break your concentration.");
		return 0;
	endif
endfunction

function ClericalTargetCoordinates(byref who, byref params := "", prompt := "Select a target")
	var x := who.x;
	var y := who.y;
	who.hidden := 0;
	if (params[3])
		return params[3];
	endif
	if (who.isA(POLCLASS_NPC))
		return;
	endif
	SendSysMessage(who, prompt);
	var targ := TargetCoordinates(who);

	if (who.x == x && who.y == y)
		return targ;
	else
		SendSysMessage(who, "You lose concentration");
		return 0;
	endif
endfunction

function ClericalGetFacing(who, targx, targy, targz)
	targz := targz;
	var ns_desc := "";
	var ew_desc := "";
	var thedesc;

	if (targy < who.y)
		ns_desc := "north";
	elseif  (targy > who.y)
		ns_desc := "south";
	endif

	if (targx < who.x)
		ew_desc := "west";
	elseif  (targx > who.x)
		ew_desc := "east";
	endif

	if (ns_desc == "" && ew_desc == "")
		thedesc := "nearby";
	else
		thedesc := ns_desc + ew_desc;
	endif
	return thedesc;
endfunction

function IsCleric(who)
	var faithsdb := Faiths_OpenDatafile();
	var valid_clerics := faithsdb.Keys();
	var elem;
	var faiths_list := {};
	foreach key in valid_clerics
		elem := faithsdb.FindElement(key);
		if(GetObjProperty(who, "Cleric") == elem.GetProp("Cleric"))
			return 1;
		endif
	endforeach
	return 0;
endfunction

function FocusType(word)
	// You can send this the focus name
	// it will return the objtype of it
	word := lower(word);

	if (word == "power focus")
		return POWER_FOCUS;
	elseif (word == "spirit focus")
		return SPIRIT_FOCUS;
	elseif (word == "arcane focus")
		return ARCANE_FOCUS;
	elseif (word == "time focus")
		return TIME_FOCUS;
	elseif (word == "earth focus")
		return EARTH_FOCUS;
	elseif (word == "life focus")
		return LIFE_FOCUS;
	endif
endfunction

function FocusName(number)
	// You can send this the focus objtype
	// it will return the name of it

	if (number == POWER_FOCUS)
		return "power focus";
	elseif (number == SPIRIT_FOCUS)
		return "spirit focus";
	elseif (number == ARCANE_FOCUS)
		return "arcane focus";
	elseif (number == TIME_FOCUS)
		return "time focus";
	elseif (number == EARTH_FOCUS)
		return "earth focus";
	elseif (number == LIFE_FOCUS)
		return "life focus";
	endif
endfunction

function CheckMana(who, amount)
	if (AP_GetVital(who, MANA) >= amount)
		AP_SetVital(who, MANA, (AP_GetVital(who, MANA) - amount));
		return 1;
	else
		return 0;
	endif
endfunction

function DistanceCheck(who, targ, range)
	if (who.isA(POLCLASS_NPC))
		return 1;
	elseif (Distance(who, targ) <= range)
		return 1;
	else
		return 0;
	endif
endfunction

function ClericReleaseScript(pid)
	var process := getprocess(pid);
	process.SendEvent("done");
endfunction
