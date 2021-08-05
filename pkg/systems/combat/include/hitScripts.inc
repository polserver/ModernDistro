/* $Id$
 *
 */

use uo;
use os;

/* GetWeaponHitScripts(object)
 *
 * Retrieves an array of all hit scripts that the object has when it hits.
 *
 * Parameters
 * object: 	Object to get the weapon hit scripts of.
 * 		Will read from a cprop array called "HitScripts"
 *		If the object is an NPC it will also get any WeaponHitScript lines from its config elem.
 *
 * Return Value:
 * Returns an array.
 *
 */
function GetWeaponHitScripts(object)
	var hit_scripts := GetObjProperty(object, "HitScripts");
	if ( TypeOf(hit_scripts) != "Array" )
		hit_scripts := {};
	endif

	if ( object.IsA(POLCLASS_NPC) )
		var cfg_elem := NPC_GetNPCConfig(object);
		var temp := GetConfigStringArray(cfg_elem, "WeaponHitScript");
		if ( temp.Size() > 0 )
			hit_scripts += temp;
		endif
	endif

	return hit_scripts;
endfunction

/* GetArmorHitScripts(object)
 *
 * Retrieves an array of all hit scripts that the object has when it hits.
 *
 * Parameters
 * mobile:	Mobile that is being hit. (Any scripts on the mobile its self)
 * armor: 	Armor to get the armor hit scripts of.
 * 		Will read from a cprop array called "ArmorHitScripts"
 *		If the object is an NPC it will also get any ArmorHitScript lines from its config elem.
 *
 * Return Value:
 * Returns an array.
 *
 */
function GetArmorHitScripts(mobile, armor)
	var hit_scripts := array;

	if ( mobile.IsA(POLCLASS_MOBILE) )
		// Get Cprops from the mobile its self.
		var temp := GetObjProperty(armor, "ArmorHitScripts");
		if ( TypeOf(temp) == "Array" )
			hit_scripts += temp;
		endif
	endif

	if ( mobile.IsA(POLCLASS_NPC) )
		// Get npc config elem armor hit scripts
		var cfg_elem := NPC_GetNPCConfig(mobile);
		var temp := GetConfigStringArray(cfg_elem, "ArmorHitScript");
		if ( TypeOf(temp) == "Array" )
			hit_scripts += temp;
		endif
	endif

	if ( armor.IsA(POLCLASS_ARMOR) )
		var temp := GetObjProperty(armor, "ArmorHitScripts");
		if ( TypeOf(temp) == "Array" )
			hit_scripts += temp;
		endif
	endif

	return hit_scripts;
endfunction

// AdjustDamageFactor(byref weapon, byref defender, byref attacker)
// Scales the damage done based on the weapon type and the target qualities.
function AdjustDamageFactor(weapon, defender, attacker)

	var cfile := ReadConfigFile("::npcdesc");
	var ttmp := FindConfigElem(cfile, defender.npctemplate);
	var immune := GetConfigStringArray(ttmp, "Immune");
	var resists := GetConfigStringArray(ttmp, "Resists");
	var vulnerable := GetConfigStringArray(ttmp, "Vulnerable");
	cfile := ReadConfigFile(":combat:itemdesc");
	ttmp := FindConfigElem(cfile, weapon.objtype);
	var DamageClass := GetConfigString(ttmp, "DamageClass");
	cfile := { };
	ttmp := { };

	var factor := 1;
	var silver := 1;

	if (DamageClass in vulnerable)
		factor := 1.5;
	elseif (DamageClass in resists)
		factor := 0.5;
	elseif (DamageClass in immune)
		factor := 0;
	endif

//	if (attacker.cmdlevel > 0)
//		PrintTextAbove(defender, "DC == " + CStr(DamageClass) );
//		PrintTextAbove(defender, "IL == " + CStr(immune) );
//		PrintTextAbove(defender, "RL == " + CStr(resists) );
//		PrintTextAbove(defender, "VL == " + CStr(vulnerable) );
//	endif

	if ( GetObjProperty(weapon, "Silver") != error)
		if ("silver" in vulnerable)
			silver := 1;
			factor := factor * 1.5;
		elseif ("silver" in resists)
			silver := 1;
			factor := factor * 0.5;
		elseif ("silver" in immune)
			factor := 0;
		endif
	endif

	if ( (GetObjProperty(weapon, "Magic") != error) or (GetObjProperty(weapon, "magic") != error) )
		if ("magic" in vulnerable)
			factor := factor * 1.5;
		elseif ("magic" in resists)
			factor := factor * 0.5;
		elseif ( ("magic" in immune) and (!silver) )
			factor := 0;
		endif
	else
		if ("nonmagic" in vulnerable)
			factor := factor * 1.5;
		elseif ("nonmagic" in resists)
			factor := factor * 0.5;
		elseif ( ("nonmagic" in immune) and (!silver) )
			factor := 0;
		endif
	endif

	if (factor == 0)
		SendSysMessage(attacker, "Your weapon does not seem to be effective...");
		return 0;
	endif

	return factor;

endfunction

// DealDamage(defender, attacker, rawdamage)
// Modifies damage for anatomy, then applies it to the target.
function DealDamage(defender, attacker, rawdamage)

	rawdamage := rawdamage + ( CInt(GetAttribute(attacker, "anatomy") ) / 9);
        ApplyRawDamageEX(defender, rawdamage, DMG_PHYSICAL, attacker);

endfunction

// function CheckTempProtections(attacker, defender, rawdamage)
//
// Check for specific, temporary protections.
function CheckTempProtections(attacker, defender, rawdamage)

	var ProtCheck;
	var chkfile := ReadConfigFile("::npcdesc");
	var atmp := FindConfigElem(chkfile, attacker.npctemplate);
	var elemental := CStr(GetConfigString(atmp, "elemental") );

// Protection from Undead?
	ProtCheck := CInt(GetObjProperty(defender, "ProtUndead") );
	if ( (ProtCheck != error) and (elemental == "undead") )
		if (ProtCheck >= ReadGameClock() )
			PlaySoundEffect(defender, 0x01f9);
			PlayObjectCenteredEffect(defender, 0x376a, 7, 0x10);
			PrintTextAbove(defender, "The undead's attack has no effect!");
			rawdamage := 0;
		else
			EraseObjProperty(defender, "ProtUndead");
		endif
	endif
	if (rawdamage == 0)
		return 0;
	endif

// Protection from daemons?
	ProtCheck := CInt(GetObjProperty(defender, "Protdaemon") );
	if ( (ProtCheck != error) and (elemental == "daemonic") )
		if (ProtCheck >= ReadGameClock() )
			PlaySoundEffect(defender, 0x01f9);
			PlayObjectCenteredEffect(defender, 0x376a, 7, 0x10);
			PrintTextAbove(defender, "The fiend's attack has no effect!");
			rawdamage := 0;
		else
			EraseObjProperty(defender, "Protdaemon");
		endif
	endif
	if (rawdamage == 0)
		return 0;
	endif

// Protection from Evil?
	ProtCheck := CInt(GetObjProperty(defender, "ProtEvil") );
	if ( (ProtCheck != error) and (attacker.murderer) )
		if (ProtCheck >= ReadGameClock() )
			PlaySoundEffect(defender, 0x01f9);
			PlayObjectCenteredEffect(defender, 0x376a, 7, 0x10);
			SendSysMessage(defender, CStr(attacker.name) + " attacks, but at a lessened effect!");
			rawdamage := CInt(rawdamage / 2);
		else
			EraseObjProperty(defender, "ProtEvil");
		endif
	endif
	if (rawdamage == 0)
		return 0;
	endif

	return rawdamage;

endfunction

