// HitScript Include

use uo;
use util;
use math;
use cfgfile;
use vitals;

include ":damage:damage";

// DealDamage(defender, attacker, rawdamage)
// Modifies damage for anatomy, then applies it to the target.

function DealDamage(defender, attacker, rawdamage)

	rawdamage := rawdamage + ( CInt(GetAttribute(attacker, "anatomy") ) / 9);
  ApplyRawDamageEX(defender, rawdamage, DMG_PHYSICAL, attacker);

endfunction


// AdjustNoto(attacker, defender)
// Applies Karma and Fame to the attacker and defender, only if the
// defender is killed as a result of the attacker.  Yes, NPCs can
// have their Karma and Fame adjusted as a result of their actions.
// Note: "evil" characters NEVER gain Karma for killing other evil
// characters.

function AdjustNoto(attacker, defender)

	if (!defender.dead)
		return 0;
	endif

	var Karma := CInt(GetObjProperty(attacker, "Karma") );
	var Fame := CInt(GetObjProperty(attacker, "Fame") );
	var vKarma := CInt(GetObjProperty(defender, "Karma") );
	var vFame := CInt(GetObjProperty(defender, "Fame") );
	var KMod := 1;

	Fame := Fame + CInt( (vFame / 10) / ( CInt(Log10(Fame)) + 1 ) );
	if (!defender.npctemplate)
		vFame := CInt(vFame * 0.9);
	endif

	if ( (Karma < -15) and (vKarma < -15) )
		return;
	endif

	if (vKarma > 15)
		KMod := -1;
		Karma := Abs(Karma);
	else
		vKarma := Abs(vKarma);
	endif

	KMod := KMod * CInt( (vKarma / 10) / ( CInt(Log10(Karma)) + 1 ) );
	Karma := Karma + KMod;

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

// Protection from demons?
	ProtCheck := CInt(GetObjProperty(defender, "Protdemon") );
	if ( (ProtCheck != error) and (elemental == "demonic") )
		if (ProtCheck >= ReadGameClock() )
			PlaySoundEffect(defender, 0x01f9);
			PlayObjectCenteredEffect(defender, 0x376a, 7, 0x10);
			PrintTextAbove(defender, "The fiend's attack has no effect!");
			rawdamage := 0;
		else
			EraseObjProperty(defender, "Protdemon");
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