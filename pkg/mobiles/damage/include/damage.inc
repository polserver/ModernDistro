/* $Id: damage.inc 779 2006-08-30 23:39:16Z AustinHeilman $
 *
 * Purpose
 * Handles damages and resistances to damage types
 *
 */
// Version 2
// 20171-11-15  Yukiko changed resistances from using a CProp to use built-in members'
//
// To do: Add the ability to have custom resistances through the use of a CProp.
//

use uo;
use os;
use math;
use polsys;
use cfgfile;

include ":damage:settings";
include ":damage:report";
include "include/NPCUtil";
include ":attributes:attributes";
include ":magery:spells";

/*
 * http://uo.stratics.com/content/guides/resistance.shtml
 * Damage constants listed here to prevent typos in scripts.
 *
 */

// Read only members
enum DAMAGE_MEMBERS
  DMG_FORCED         := -1,                    // This isn't an official member. It's used for applying damage without taking resistances into account.
  DMG_COLD              := "Cold",
  DMG_ENERGY            := "Energy",
  DMG_FIRE              := "Fire",
  DMG_PHYSICAL          := "Physical",
  DMG_POISON            := "Poison"
endenum

/*
 * ApplyDamageEX(mobile, amount, type, source)
 *
 * Purpose: Does damage to mobile taking into account mobile's AR
 *
 * Parameters
 * mobile:
 * amount:
 * type:
 * source:
 *
 * Return value
 *
 */
function ApplyDamageEX(mobile, amount, type:=DMG_FORCED, source:=0)
	amount := CInt(amount);
	amount := CalcDamageAmount(mobile, amount, type);

	if ( amount == error )
		return 0;
	endif

	var settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.DisplayDamage )
		DisplayDamage(mobile, amount, source);
	endif

	if ( amount < 0 )
		HealDamage(mobile, Abs(amount));
	else
		if ( source )
			SetLastDamageInfo(mobile, amount, type, source);
			SetScriptController(source);
		endif
    // Added for necro wraith form
    if(source and GetObjProperty(source, "wraith"))
      DoManaSuck(mobile, source, amount);
    endif
    if(GetObjProperty(source , "cursed_weapon"))
      LifeDrain(source, amount, 50);
    endif
		ApplyDamage(mobile, amount);
	endif

	return amount;
endfunction

/*
 * ApplyRawDamageEX(mobile, amount, type, source)
 *
 * Purpose: Does damage to a mobile ignoring the mobile's AR
 *
 * Parameters
 * mobile:
 * amount:
 * type:
 * source:
 *
 * Return value
 *
 */
function ApplyRawDamageEX(mobile, amount, type:=DMG_FORCED, source:=0)
	amount := CInt(amount);
	amount := CalcDamageAmount(mobile, amount, type);

	if ( amount == error )
		return 0;
	endif

	var settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.DisplayDamage )
		DisplayDamage(mobile, amount, source);
	endif

	if ( amount < 0 )
		HealDamage(mobile, Abs(amount));
	else
		if ( source )
			SetLastDamageInfo(mobile, amount, type, source);
			SetScriptController(source);
		endif
  // Added for necro wraith form
  if(source and GetObjProperty(source, "wraith"))
    DoManaSuck(mobile, source, amount);
  endif
  if(GetObjProperty(source , "cursed_weapon"))
    LifeDrain(source, amount, 50);
  endif

		ApplyRawDamage(mobile, amount);
	endif

	return amount;
endfunction

/*
 * ApplySpellDamageEX(mobile, amount, type, source, circle)
 *
 * Purpose: Does spell damage to mobile taking into account mobile's AR
 *
 * Parameters
 * mobile: Victim
 * amount: Raw damage
 * type:   Fire, cold, energy, poison or physical
 * source: Source of the damage
 * circle: Circle or level of spell. Basic spells are 1 through 8.
 *
 * Return value: damage amount done
 *
 */
function ApplySpellDamageEX(mobile, amount, type:=DMG_FORCED, source:=0, circle := 1)
	amount := CInt(amount);

	if( mobile.IsA(POLCLASS_NPC) )
	    amount := amount * 2;
	endif

	// Check to see if we are using magic resistance with damage spells.
	var settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.UseMRInAllSpells )
	    amount := Resisted(circle, source, mobile, amount);
	endif

	// Check to see if we will take standard resistances into account against spell damage.
	settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.UseDamageMembers )
      amount := CalcDamageAmount(mobile, amount, type);
	endif

	if ( amount == error )
	    return 0;
	endif

	settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.DisplayDamage )
	    DisplayDamage(mobile, amount, source);
	endif

	if ( amount < 0 )
	    HealDamage(mobile, Abs(amount));
	else
    if ( source )
      SetLastDamageInfo(mobile, amount, type, source);
      SetScriptController(source);
    endif

	    ApplyDamage(mobile, amount);
	endif
  // Added for necro wraith form
  if(source and GetObjProperty(source, "wraith"))
    DoManaSuck(mobile, source, amount);
  endif
  if(GetObjProperty(source, "cursed_weapon"))
    LifeDrain(source, amount, 50);
  endif

	return amount;
endfunction

/*
 * ApplyRawSpellDamageEX(mobile, amount, type, source, circle)
 *
 * Purpose: Does spell damage to a mobile ignoring the mobile's AR
 *
 * Parameters
 * mobile: Victim
 * amount: Raw damage
 * type:   Fire, cold, energy, poison or physical
 * source: Source of the damage
 * circle: Circle or level of spell. Basic spells are 1 through 8.
 *
 * Return value: damage amount done
 *
 */
function ApplyRawSpellDamageEX(mobile, amount, type:=DMG_FORCED, source:=0, circle := 1)
	amount := CInt(amount);
	var settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.UseMRInAllSpells )
		amount := Resisted(circle, source, mobile, amount);
	endif

	settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.UseDamageMembers )
    amount := CalcDamageAmount(mobile, amount, type);
	endif


	if ( amount == error )
		return 0;
	endif

	settings := DMG_GetSettingsCfgElem("Settings");
	if ( settings.DisplayDamage )
		DisplayDamage(mobile, amount, source);
	endif

	if ( amount < 0 )
		HealDamage(mobile, Abs(amount));
	else
		if ( source )
			SetLastDamageInfo(mobile, amount, type, source);
			SetScriptController(source);
		endif
		ApplyRawDamage(mobile, amount);
	endif
  // Added for necro wraith form
  if(source and GetObjProperty(source, "wraith"))
    DoManaSuck(mobile, source, amount);
  endif
  if(GetObjProperty(source , "cursed_weapon"))
    LifeDrain(source, amount, 50);
  endif

	return amount;
endfunction

/*
 * DisplayDamage(damaged, amount, source)
 *
 * Purpose
 * Displays the damage done above the damaged mobile.
 *
 * Parameters
 * damaged:	Mobile that was damaged.
 * amount:	Amount of damage that was done.
 * source:	Source of the damage.
 *
 * Return Value
 * Returns 1
 *
 */
function DisplayDamage(damaged, amount, source:=0)

	var dmg_packet := CreatePacket(0x0B, 0x07);

	if( amount <= 255 )
		dmg_packet.SetInt8(0x06, amount);
	else
		dmg_packet.SetInt16(0x05, amount);
	endif

	dmg_packet.SetInt32(0x01, damaged.serial);

	if ( source.acctname )
		dmg_packet.SendPacket(source);
	endif

	if( damaged.acctname )
		dmg_packet.SendPacket(damaged);
	endif

	return 1;
endfunction

/*
 * CalcDamageAmount(mobile, amount, type)
 *
 * Purpose
 * Calculates the amount of damage based on the damage type and resistance.
 * Resistance is in percent of damage to resist.
 *
 * Parameters
 * mobile:	Mobile that is being damaged and to check resistances on.
 * amount:	Amount of base damage to deal.
 * type:	Type of damage that is being dealt.
 *
 * Return value
 * Integer
 *
 */
function CalcDamageAmount(mobile, amount, type)

	amount := CInt(amount);
  // If type == DMG_FORCED we don't take resistances into account.
	if ( type == DMG_FORCED )
		return amount;
	endif

	var resistance := CDbl(GetResistance(mobile, type));
// Broadcast("mobile = " + mobile.name);
// Broadcast("resistance is = " + CStr(resistance));
// Broadcast("damage before resist = " + CStr(amount));
	if ( resistance == error )
		// Do nothing, there is no resistance.
	else
		// Calculate percent that was resisted.
		// Examples:
		// Damage = 50
		// [ Resistance = 10 so Resisted = 5, Amount = 45 ]
		// [ Resistance = 100 so Resisted = 50, Amount = 0 ]
		// [ Resistance = -100 so Resisted = -50, Amount = -50 ]
		// [ Resistance = 47.3 so Resisted = 23.65, Amount = 26.35 ]
		var resistd := (resistance * 0.010) * CDbl(amount);
		amount := amount - resistd;
// Broadcast("damage after resist = " + CStr(amount));
	endif

	return CInt(amount);
endfunction

/*
 * SetLastDamageInfo(mobile, amount, type, source)
 *
 * Purpose
 *
 * Parameters
 * mobile:
 * amount:
 * type:
 * source:
 *
 * Return value
 *
 */
function SetLastDamageInfo(mobile, amount, type, source)
	var last_damage := struct;
	last_damage.+serial	:= source.serial;
	last_damage.+time	:= POLCore().systime;
	last_damage.+amount	:= amount;
	last_damage.+type	:= type;

	if ( source.acctname )
		last_damage.+acctname := source.acctname;
	endif

	if ( source.name )
		last_damage.+name := source.name;
	elseif ( source.IsA(POLCLASS_ITEM) )
		last_damage.+name := source.desc;
	endif

	SetObjProperty(mobile, "LastDamage", last_damage);

	return last_damage;
endfunction

/*
 * GetLastDamageInfo(mobile)
 *
 * Purpose
 *
 * Parameters
 * mobile:
 *
 * Return value
 *
 */
function GetLastDamageInfo(mobile)
	return GetObjProperty(mobile, "LastDamage");
endfunction

/*
 * GetResistanceNames()
 *
 * Purpose
 * Retrieves a list of resistance names from settings.cfg
 *
 * Parameters
 *
 * Return Value
 * Returns an array of strings
 *
 */
function GetResistanceNames()
	var cfg_elem := DMG_GetSettingsCfgElem("DamageMembers");

	return GetConfigStringArray(cfg_elem, "Type");
endfunction

/*
 * GetResistance(mobile, type)
 *
 * Purpose
 * Retrieves the resistances list for a mobile, taking into consideration
 * the magic resist skill.
 *
 *
 * Parameters
 * mobile:	Mobile to retrieve the resistances for.
 * type:	Optional argument.
 *		If 0 or blank, will return a dictionary of all resistances.
 *		If specified, will return a double for that value.
 *
 * Return value
 * dictionary or double
 *
 */
function GetResistance(mobile, type:=0)
    var amt := 0;
    // We use the damage type to check if the mobile has a
    // corresponding resistance and return the amount of resistance.
    case(type)
      DMG_COLD: amt := mobile.resist_cold;
      DMG_ENERGY: amt := mobile.resist_energy;
      DMG_FIRE: amt := mobile.resist_fire;
      DMG_PHYSICAL: amt := mobile.resist_physical;
      DMG_POISON: amt := mobile.resist_poison;
    endcase
		return amt;
endfunction

/* Not used.
 * GetNPCResistance(template, type)
 *
 * Purpose
 * Retrieves base resistances from an npc's npcdesc.cfg
 *
 * Parameters
 * template:	Can take in a template name or an npc reference.
 * type:	Find a specific resistance value. If 0, will retrieve them all.
 *
 * Return Value
 * Returns a double or a dictionary.
 *
 */
function GetNPCResistance(template, type:=0)
	var cprop_resistances;

	if ( template.IsA(POLCLASS_NPC) )
		cprop_resistances := GetObjProperty(template, "%Resistances%");
		template := template.npctemplate;
	endif

	var npc_elem := NPC_GetNPCConfig(template);
	if ( type )
		if ( cprop_resistances.Exists(type) )
			return cprop_resistances[type];
		else
			return GetConfigReal(npc_elem, type+"-Resistance");
		endif
	else
		var resistances := dictionary;
		foreach name in ( GetResistanceNames() )
			if ( cprop_resistances.Exists(name) )
				resistances[name] := cprop_resistances[name];
			else
				resistances[name] := GetConfigReal(npc_elem, name+"-Resistance");
			endif
			SleepMS(2);
		endforeach
		return resistances;
	endif
endfunction



/*
 * SetResistance(mobile, name, value)
 *
 * Purpose
 * Sets a resistance value on a mobile.
 *
 * Parameters
 * mobile:	Mobile to set the resistance value for.
 * name:	Name of the resistance type to set.
 * value:	Value for the resistance.
 *
 * Return value
 * Returns result of SetObjProperty()
 *
 */
function SetResistance(mobile, name, value)
  value := CInt(value);
  case(name)
    DMG_COLD: mobile.resist_cold_mod := value;
    DMG_ENERGY: mobile.resist_energy_mod := value;
    DMG_FIRE: mobile.resist_fire_mod := value;
    DMG_PHYSICAL: mobile.resist_physical_mod := value;
    DMG_POISON: mobile.resist_poison_mod := value;
  endcase
	return value;
endfunction

/*
 * ModifyResistance(mobile, name, value)
 * Purpose
 * Modifies a resistance value on a mobile.
 *
 * mobile:	Mobile to set the resistance value for.
 * name:	Name of the resistance type to change.
 * value:	Adjustment to the value.
 *
 * Return value
 * Returns result of SetObjProperty()
 *
 */
function ModifyResistance(mobile, name, value)
  value := CInt(value);
  case(name)
    DMG_PHYSICAL: mobile.resist_physical_mod := mobile.resist_physical_mod + value;
    DMG_FIRE: mobile.resist_fire_mod := mobile.resist_fire_mod + value;
    DMG_COLD: mobile.resist_cold_mod := mobile.resist_cold_mod + value;
    DMG_POISON: mobile.resist_poison_mod := mobile.resist_poison_mod + value;
    DMG_ENERGY: mobile.resist_energy_mod := mobile.resist_energy_mod + value;
  endcase
	return value;
endfunction


/*
 * SetDamage(mobile, name, value)
 *
 * Purpose
 * Sets a damage value on a mobile.
 *
 * Parameters
 * mobile:	Mobile to set the damage value for.
 * name:	Name of the damage type to set.
 * value:	Value for the damage.
 *
 * Return value
 * Returns none
 *
 */
function SetDamage(mobile, name, value)
  value := CInt(value);
  case(name)
    DMG_COLD: mobile.damage_cold_mod := value;
    DMG_ENERGY: mobile.damage_energy_mod := value;
    DMG_FIRE: mobile.damage_fire_mod := value;
    DMG_PHYSICAL: mobile.damage_physical_mod := value;
    DMG_POISON: mobile.damage_poison_mod := value;
  endcase
	return value;
endfunction

/*
 * ModifyDamage(mobile, name, value)
 * Purpose
 * Modifies a damage value on a mobile.
 *
 * mobile:	Mobile to set the damage value for.
 * name:	Name of the damage type to change.
 * value:	Adjustment to the value.
 *
 * Return value
 * Returns nothing
 *
 */
function ModifyDamage(mobile, name, value)
  value := CInt(value);
  case(name)
    DMG_COLD: mobile.damage_cold_mod := mobile.damage_cold_mod + value;
    DMG_ENERGY: mobile.damage_energy_mod := mobile.damage_energy_mod + value;
    DMG_FIRE: mobile.damage_fire_mod := mobile.damage_fire_mod + value;
    DMG_PHYSICAL: mobile.damage_physical_mod := mobile.damage_physical_mod + value;
    DMG_POISON: mobile.damage_poison_mod := mobile.damage_poison_mod + value;
  endcase
	return value;
endfunction



/*
 * GetDamageMembers(mobile)
 *
 * Purpose
 * Retrieves the Damage member value from the mobile, if an NPC, from the mobile.weapon if not
 * If no damage member values are found, returns all damage as physical
 *
 * mobile: mobile refrence
 *
 * Return value:
 * Dictionary with the type of damage and porcentage, eg.: "DAMAGE_FIRE -> 0.5 DAMAGE_COLD -> 0.5"
*/

function GetDamageMembers(mobile)
	var damage_members := struct;

    damage_members.+"damage_cold" := mobile.npctemplate.damage_cold;
    damage_members.+"damage_energy" := mobile.npctemplate.damage_energy;
    damage_members.+"damage_fire" := mobile.npctemplate.damage_fire;
    damage_members.+"damage_physical" := mobile.npctemplate.damage_physical;
    damage_members.+"damage_poison" := mobile.npctemplate.damage_poison;

	if ( !damage_members )
		damage_members := struct;
		damage_members.+"DAMAGE_PHYSICAL" := 1.0;
	endif

	return damage_members;
endfunction


/*
 * ResistancePercentToMult(percent)
 *
 * Purpose
 * Turns a percentage into a resistance multiplier.
 *
 * Parameters
 * percent
 *
 * Return value
 * Double resistance multiplier
 *
 */
function ResistancePercentToMult(percent)
    if ( percent > 0 )
        //print("% to mult: "+percent+" is: "+(CDbl(100 - percent) * 0.010));
        return CDbl(100 - percent) * 0.010;
    else
        //print("% to mult: "+percent+" is: "+(CDbl(100 + percent) * 0.010));
        return CDbl(100 + percent) * 0.010;
    endif
endfunction

/*
 * ResistanceMultToPercent(percent)
 *
 * Purpose
 * Turns a resistance multiplier into a percentage.
 *
 * Parameters
 * mult
 *
 * Return value
 * Int percentage
 *
 */
function ResistanceMultToPercent(mult)
    if ( mult > 0 )
        //print("mult to %: "+mult+" is: "+CInt(100 - (mult * 100)));
        return CInt(100 - (mult * 100));
    else
        //print("mult to %: "+mult+" is: "+CInt(100 + (mult * 100)));
        return CInt(100 + (mult * 100));
    endif
endfunction

function DoManaSuck(defender, attacker, amount)

  // Added for necro wraith form
  // Get Spirit Speak skill level from the attacker and divide it by 5.
  // Then convert to percent (div by 100) and determine the mana amount to suck from defender.
    var sspeak := GetAttribute(attacker, SPIRITSPEAK);
    sspeak := CDbl(sspeak);
    sspeak := sspeak/5;
    if(sspeak < 8)
      sspeak := 8;
    endif
    sspeak := sspeak / 100;
    // The amount of mana to suck from the defender.
    var manasuck := amount * sspeak;
    var defmana := GetVital(defender, "Mana")/ 100;
    // If the defender has mana and it is more mana than we want to suck then take the amount.
    // else take what he's got left if any.
    if(defmana)
      if(manasuck <= defmana)
        ConsumeVital(defender, "Mana", CInt(manasuck * 100));
      else
        ConsumeVital(defender, "Mana", defmana);
      endif
    endif
endfunction

///////////////////////////////////////////////////
//
//
// LifeDrain(source, damage, percent)
//
// Purpose: To use a percentage of the damage done
// to apply healing to the attacker.
//
// Parameters:
// source - the attacker
// damage - the damage amount
// percent - the percent expressed as an integer eg. 25 = 25%
//
// Result:
// Heals the attacker up to max HP.
//
// Return:
// Amount healed.
//
///////////////////////////////////////////////////////

function LifeDrain(source, damage, percent)

    percent := CDbl(percent);
    percent := percent / 100;
    var amt := CInt(CDbl(damage) * percent);
    HealDamage(source, amt);
    SendSysMessage(source, amt + " life force from your opponent was applied to heal you.", 2, 66);

    return amt;

endfunction

