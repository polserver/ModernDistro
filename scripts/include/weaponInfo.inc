/*
 * $Id: weaponInfo.inc 744 2006-08-25 03:47:07Z AustinHeilman $
 *
 */

use cfgfile;

/*
 * CS_GetWeaponInfo(weapon, prop_name)
 *
 * Purpose
 * Retrieves a config property for a weapon.
 *
 * Parameters
 * weapon:	Weapon that is being looked up.
 * prop_name:	Itemdesc.cfg property to retrieve.
 *
 * Return value
 * Returns a string
 *
 */
function CS_GetWeaponInfo(weapon, prop_name)
	var config := ReadConfigFile(":*:itemdesc");
	config := Config[weapon.objtype];
	
	return GetConfigString(config, prop_name);
endfunction

/*
 * CS_GetAverageDamage(weapon)
 *
 * Purpose
 * Takes in the weapon's damage dice and damage modifier and
 * calculates its average damage.
 *
 * Parameters
 * weapon:	Weapon object to get the average damage for.
 *
 * Return value
 * Integer
 *
 */
function CS_GetAverageDamage(weapon)
	var dmg := CS_GetWeaponInfo(weapon, "Damage");
	
	dmg["d"] := " ";
	dmg["-"] := " ";
	dmg["+"] := " ";
	dmg := SplitWords(dmg);
	var X := CInt(dmg[1]);
	var Y := CInt(dmg[2]);
	var Z := CInt(dmg[3]);
	Z := Z + CInt((weapon).dmg_mod);
	// (x*((1+y)/2))+z average formula
	var average := (X*((1+Y)/2))+Z;
	
	return CInt(average);
endfunction