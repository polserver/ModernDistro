use uo;
use os;
use util;
use cfgfile;

include ":attributes:attributes";
include ":magery:settings";
include ":magery:spells_ex";
include ":magery:report";
include "include/client";
include "include/sounds";
include "include/statMod";
include "include/spellAttack";


const EVID_DISCHARGE := 0x123457;

var MS_npccfg := ReadConfigFile("::npcdesc");

/*
 * MS_GetSpellsCfgFile()
 *
 * Purpose
 * Gets the spells.cfg file from the magery package.
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function MS_GetSpellsCfgFile()
	var cfg := ReadConfigFile(":magery:spells");

	if ( cfg.errortext )
		MS_ReportText("Error::MS_GetSpellsCfgFile() - Unable to open [:magery:spells.cfg] ->"+cfg.errortext, MS_REPORT_SYSLOG);
	endif

	return cfg;
endfunction

/*
 * MS_GetSpellsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by MS_GetSpellsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function MS_GetSpellsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := MS_GetSpellsCfgFile();
	endif

	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		MS_ReportText("Error::MS_GetSpellsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext, MS_REPORT_SYSLOG);
	endif

	return elem;
endfunction

/*
 * MS_SpellDebug(mobile, text)
 *
 * Purpose
 * Sends debug information to mobiles with a command level.
 *
 * Parameters
 * mobile:		Mobile that will see the debug message.
 * text:	Text to show.
 *
 * Return value
 * Returns 1 if the text was sent.
 * Returns 0 if the text was not sent.
 */
function MS_SpellDebug(mobile, text)
	if ( mobile.cmdlevel )
		SendSysMessage(mobile, ""+text);
		return 1;
	endif
	return 0;
endfunction

/*
 * MS_Target(mobile, targ, prompt, flags)
 *
 * Purpose
 * Used to handle targetting of other objects in spell scripts.
 *
 * Parameters
 * mobile:		Mobile that is doing the targetting
 * targ:	Pre-selected target (for NPCs)
 * prompt:	Message the user receives when targeting.
 * flags:	uo.em targeting flags.
 *
 * Return value
 * Returns an object reference to what was targeted.
 *
 */
function MS_Target(mobile, targ:=0, prompt := "Select a target.", flags:=TGTOPT_CHECK_LOS+TGTOPT_NEUTRAL, colour := 88)
	mobile.hidden := 0;

	if ( targ.IsA(POLCLASS_ITEM) )
		SendSysMessage(mobile, "It's an object!", color := 88);
		return targ;
	elseif ( mobile.npctemplate )
		return 0;
	endif

	var orig_x := mobile.x;
	var orig_y := mobile.y;
	var orig_realm := mobile.realm;
	var settings_elem := MS_GetSettingsCfgElem("Settings");
	var allow_precast := CInt(settings_elem.AllowPrecasting);

	SendSysMessage(mobile, prompt, color := colour);
	
	targ := Target(mobile, flags);
	if ( Distance(mobile, targ) > CInt(settings_elem.MaxRange) )
		SendSysMessage(mobile, "That is too far away.", color := 33);
		return 0;
	endif
	
	if ( !allow_precast )
		if ( mobile.x != orig_x || mobile.y != orig_y || mobile.realm != orig_realm )
			SendSysMessage(mobile, "Your concentration is disturbed, thus ruining thy spell.", color := 33);
			PlaySoundEffect(mobile, SFX_SPELL_FAIL);
			return 0;
		endif
	endif

	return targ;

endfunction

/*
 * MS_TargetCoordinates(mobile, targ, prompt)
 *
 * Purpose
 * Used to handle targetting of other objects in spell scripts.
 *
 * Parameters
 * mobile:		Mobile that is doing the targetting
 * targ:	Pre-selected target (for NPCs)
 * prompt:	Message the user receives when targeting.
 * 
 * Return value
 * Returns an object reference to what was targeted.
 *
 */
function MS_TargetCoordinates(mobile, targ:=0, prompt := "Select a target.", colour := 88)
	mobile.hidden := 0;

	if ( targ )
		return targ;
	elseif ( mobile.npctemplate )
		return 0;
	endif

	var orig_x := mobile.x;
	var orig_y := mobile.y;
	var orig_realm := mobile.realm;
	var settings_elem := MS_GetSettingsCfgElem("Settings");
	var allow_precast := CInt(settings_elem.AllowPrecasting);
	
	SendSysMessage(mobile, prompt, color := colour);
	targ := TargetCoordinates(mobile);

	if ( !targ )
		SendSysMessage(mobile, "Cancelled.", color := 33);
		return 0;
	elseif ( CoordinateDistance(mobile.x, mobile.y, targ.x, targ.y) > settings_elem.MaxRange )
		SendSysMessage(mobile, "That is too far away.", color := 33);
		return 0;
	endif
	
	if ( !allow_precast )
		if ( mobile.x != orig_x || mobile.y != orig_y || mobile.realm != orig_realm )
			SendSysMessage(mobile, "Your concentration is disturbed, thus ruining thy spell.", color := 33);
			PlaySoundEffect(mobile, SFX_SPELL_FAIL);
			return 0;
		endif
	endif
	
	return targ;
endfunction

/*
 * MS_AdjustSpellPoints(mage, targ, points)
 *
 * Purpose
 * Adjusts the effectiveness of a spell based on the target's magic resistance.
 *
 * Parameters
 * caster:	mobile is casting the spells.
 * targ:	mobile the spell is effecting.
 * points:	Strength of the spell.
 *
 * Return value
 * Integer - adjusted point
 *
 * Notes:
 * Magic resistance does not work like this since AoS. This function remains
 * just in case someone should want to use it.
 *
 */
function MS_AdjustSpellPoints(mage, targ, points)
	// TODO: fix this function -> http://guide.uo.com/skill_26.html
	
	var mage_skill := AP_GetSkill(mage, MAGERY);
	var defense := AP_GetSkill(targ, RESISTING_SPELLS);

	// Failed MR check results in only half your resistance being used!
	if ( SkillCheck(targ, RESISTING_SPELLS, -1) < 0 )
		defense := CInt(defense / 2);
	endif

	// Old formulas
	// CInt(points * (mage_skill - defense) / 100);
	// CInt(points * ((mage_skill * 1.3) - defense) / ( mage_skill * 1.3 ))
	var effect_level := CInt(points * (mage_skill-defense) / mage_skill );
	return effect_level;
endfunction

/*
 * MS_ReleaseScript(pid)
 *
 * Purpose
 * Informs the spell starter that the caster can cast another spell.
 * This allows the spell script to continue running without blocking future castings.
 *
 * Parameters
 * pid:	PId to the spell starter that started the spell.
 *
 * Return value
 * Returns 1
 *
 */
function ReleaseScript(pid)
	var process := GetProcess(pid);
	process.SendEvent("done");

	return 1;
endfunction

/*
 * MS_GetSpellDamage(mobile, spell_id)
 *
 * Purpose
 *   Calculates spell damage using base damage defined in spells.cfg and
 *   formula provided by stratics.
 *   Eval. Int. Bonus: ((EvalInt * 3) / 100) + 1
 *   Intelligence Bonus: (INT / 10) (Drop decimals from result)
 *   GM Inscription Bonus: 10% Damage Bonus
 *   Spell Damage Increase: Int. Bonus + Inscr. Bonus + Spell Damage Increase Items
 *   Total: round((Base Damage * Eval. Int. Bonus) * ((Spell Damage Increase / 100) + 1))
 *
 * Parameters
 *   mobile:		mobile refrence of caster
 *   targ:			target of spell
 *   min_damage:	minimum base damage
 *   max_damage:	maximum base damage
 *
 * Returns
 *   total damage
 *
 */
function MS_GetSpellDamage(mobile, targ, base_damage)
	var eval_int_bonus := CInt((AP_GetSkill(mobile, EVALUATING_INTELLIGENCE) * 3) / 100) + 1;
	var intel_bonus := CInt(AP_GetStat(mobile, "Intelligence") / 10);

	// TODO: Get SDI items percentage
	var sdi_bonus := 0;
	
	// SDI bonus capped at 15% for PvP
	if ( mobile.IsA(POLCLASS_MOBILE) && !mobile.IsA(POLCLASS_NPC) && targ.IsA(POLCLASS_MOBILE) && !targ.IsA(POLCLASS_NPC) )
		if ( sdi_bonus > 15 )
			sdi_bonus := 15;
		endif
	endif
	
	var damage_increase := intel_bonus + sdi_bonus;

	if ( AP_GetSkill(mobile, INSCRIPTION) >= 100 )
		damage_increase := damage_increase + 10;
	endif
        
        if (mobile.IsA(POLCLASS_NPC))
            base_damage := base_damage * 2;
        endif
	
	var total_damage := CInt(base_damage * eval_int_bonus * ((CDbl(damage_increase) / 100) + 1));
	return CDbl(total_damage);
endfunction

/*
 * MS_MobileCheck(mobile, targ)
 *
 * Purpose
 *   Checks if the target is a mobile and provides a central
 *   location for failure messages.
 *
 * Returns
 *   1 on success
 *
*/
function MS_MobileCheck(mobile, targ)
	if ( !targ.IsA(POLCLASS_MOBILE) )
		SendSysMessage(mobile, "This spell won't work on that!");
		return 0;
	endif
	
	return 1;
endfunction

/*
 * MS_GetPoisonLevel(mobile, targ)
 *
 * Purpose
 *   Calculates poison level based on uo.stratics:
 *  ((Magery + Poisoning) / 2):
 *  <65.1 = level 1
 *  65.1 - 85 = level 2
 *  85.1 - 99.9 = level 3
 *  100+ = level 4
 *
 * Parameters
 *   mobile:		mobile refrence of caster
 *   targ:			target refrence
 *
 * Returns
 *   int
 *
*/
function MS_GetPoisonLevel(mobile, targ)
	var poison_level := CDbl(AP_GetSkill(mobile, MAGERY) + AP_GetSkill(mobile, POISONING)) / 2;

	if ( poison_level < 65.1 )
		poison_level := 1;
	elseif ( poison_level >= 65.1 && poison_level <= 85 )
		poison_level := 2;
	elseif ( poison_level > 85 && poison_level <= 99.9 )
		poison_level := 3;
	elseif ( poison_level >= 100 )
		poison_level := 4;
	endif
	
	return poison_level;
endfunction


function MS_PlaySpellSFX(spellid, targ, success:=1)
	var cfg_elem := MS_GetSpellsCfgElem(spellid);
	var sfx := CInt(GetConfigInt(cfg_elem, "sFX"));
	
	Print("Playing SFX:"+sfx+" for spellid:"+spellid);
	PlaySoundEffect(targ, sfx);

	return 1;	
endfunction


function MS_PlaySpellGFX(spellid, targ, caster, speed:=7, loop:=14)
	var cfg_elem := MS_GetSpellsCfgElem(spellid);
	var gfx := CInt(GetConfigInt(cfg_elem, "GFX"));
	
	Print("Playing GFX:"+gfx+" for spellid:"+spellid+" of gfx type:"+CInt(cfg_elem.GFXType));
	case ( CInt(cfg_elem.GFXType) )
		0: return 1;
		1: PlayObjectCenteredEffect(targ, gfx, speed, loop);
		2: PlayMovingEffect(caster, targ, gfx, speed, loop, targ.realm);
		3: PlayStationaryEffect( targ.x, targ.y, targ.z, gfx, speed, loop, 0, targ.realm );
	endcase
	
	return 1;
endfunction


function MS_GetVaildTargets(targ, area_size:=1, check_los:=1, exclude:="", includes:="", include_self:=0, mself:="")
	var targs := array{};
        check_los := check_los;
        include_self := include_self;
        mself := mself;
	
	if ( !targ.realm )
		targs := ListMobilesNearLocation(targ.x, targ.y, targ.z, area_size);
	else
		targs := ListMobilesNearLocation(targ.x, targ.y, targ.z, area_size, targ.realm);
	endif
	
	// Remove targs with serial in exclude if exclude exists or not in includes if includes exists
	for i := 1 to (targs.Size())
		if ( (exclude != "" && (targs[i].serial in exclude || targs[i].serial == exclude )) || 
                (includes != "" && (!(targs[i].serial in includes) || targs[i].serial != includes )))
			targs.Erase(i);
		endif
	endfor
	
	return targs;
endfunction



// Old Resisted function from spellRestrictictions.inc
// This may become unnecessary if I figure out the new resist system

function Resisted(circle, caster, cast_on, amt)
	// Purpose: to check for resistance to spells cast upon a character
	// called by: most spells
	// returns: amount of damage not resisted?

        var magery := CInt(AP_GetSkill(caster, MAGERY));
        var evalint := CInt(AP_GetSkill(caster, EVALUATING_INTELLIGENCE));
        var resist := CInt(AP_GetSkill(cast_on, RESISTING_SPELLS));
        
        // chk1 relates to the resist ability.
        var chk1 := (resist / 5);
        // chk2 relates to the difficulty level
        var chk2 := resist - (((magery - 20) / 5) + (circle * 5));
        
        if(chk1 < 1)
                chk1 := 1;
        endif
        if(chk2 < 1)
                chk2 := 1;
        endif
        
        // What an odd construct.  
        // If you're hit with an easy spell then you have a difficulty of your resist
        var diff := 0;
        if(chk1 > chk2)
                diff := chk1;
        else
                diff := chk2;
        endif
        
        var points := getresistpoints(circle, cast_on, SKILLID_RESISTING_SPELLS);
        if(cast_on.dead)
                return 0;
        endif
        if(SkillCheck(cast_on, RESISTING_SPELLS, diff, points))
                SendSysMessage(cast_on, "You feel yourself resisting magical energy!");
                PlaySoundEffect(cast_on, SFX_SPELL_WEAKEN);
                PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 5,5);
                amt := (amt * 0.5);
                if(amt < 1)
                        amt := 1;
                endif
        endif
        var modamt := 1;
        if(resist > evalint)
                modamt := (1 + ((evalint - resist) / 200.0));
        elseif(evalint > resist)
                modamt := (1 + ((evalint - resist) / 500.0));
        endif
        amt := (amt * modamt);
        return CInt(amt);
endfunction

// Old magic reflect function from spellRestrictions.inc

function Reflected(cast_on)
  var mr := CInt(GetObjProperty(cast_on, "mr"));
  if(mr >= 1)
    PlaySoundEffect(cast_on, SFX_SPELL_MAGIC_REFLECTION);
    PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 10,10);
    SendSysMessage(cast_on, "Your magic reflect spell saved you!");
    if(mr == 1)
      EraseObjProperty(cast_on, "mr");
    elseif(mr == 2)
      var holder, pid;
      var eqp := GetObjProperty(cast_on, "EquipTimers");
      foreach thing in ListEquippedItems(cast_on)
        foreach entry in eqp
          if((thing.serial == entry[1]) and (entry[2] == "ref"))
            holder := GetObjProperty(thing, "pid");
            break;
          endif
        endforeach
      endforeach
      pid := getprocess(holder);
      if(pid)
        var k := struct;
        k.+type := EVID_DISCHARGE;
        pid.sendevent(k);
      else
        EraseObjProperty(cast_on, "mr");
      endif
    endif
    return 1;
  endif
  return 0;
endfunction

// Old function added for above old functions

function getresistpoints(c, victim, skillid)
  var points;
  var diff;
  case(c)
    1:   points :=  20; diff := 20;
    2:   points :=  40; diff := 30;
    3:   points :=  80; diff := 40;
    4:   points := 100; diff := 50;
    5:   points := 120; diff := 60;
    6:   points := 140; diff := 70;
    7:   points := 160; diff := 86;
    8:   points := 180; diff := 100;
  endcase
  var skill := AP_GetSkill(victim, AP_SkillIDToAttributeName(skillid));
  if(skill > (diff - 20))
    if((skill - diff) <= 5)
      points := points;
    elseif((skill - diff) <= 10)
      points := (points * 3) / 4;
    elseif((skill - diff) <= 15)
      points := points / 2;
    elseif((skill - diff) <= 15)
      points := points / 4;
    else
      points := 0;
    endif
  endif
  return points;
endfunction

function CalcSpellDamage(circle, caster, cast_on)
  cast_on := cast_on;
  var die_string := circle+"d8";
  var dmg := RandomDiceRoll(die_string);
  dmg := dmg + CInt(AP_GetSkill(caster, MAGERY)/10);
  if(dmg > (circle * 8))
    dmg := circle * 8;
  endif
  return dmg;
endfunction

