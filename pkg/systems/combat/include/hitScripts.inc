/* $Id$
 *
 */

use uo;
use os;

//include ":brainai:npcUtil";

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
		hit_scripts := array;
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

// This is not called from within any script as of yet. - Yukiko
// AdjustNoto(attacker, defender)
// Applies Karma and Fame to the attacker and defender, only if the
// defender is killed as a result of the attacker.  Yes, NPCs can
// have their Karma and Fame adjusted as a result of their actions.
// Note: "evil" characters NEVER gain Karma for killing other evil
// characters.
/*
function AdjustNoto(attacker, defender)

    if !(defender.dead)
        return 0;
    endif

    var karma := CInt(GetObjProperty(attacker, "Karma") );
    var fame := CInt(GetObjProperty(attacker, "Fame") );
    var vkarma := CInt(GetObjProperty(defender, "Karma") );
    var vfame := CInt(GetObjProperty(defender, "Fame") );
    var kmod := 1;

    fame := fame + CInt( (vfame / 10) / ( CInt(Log10(fame)) + 1 ) );
    if !(defender.npctemplate)
        vfame := CInt(vfame * 0.9);
    endif

    if ( (karma < -15) && (vkarma < -15) )
        return;
    endif

    if (vkarma > 15)
        kmod := -1;
        karma := Abs(karma);
    else
        vkarma := Abs(vkarma);
    endif

    kmod := kmod * CInt( (vkarma / 10) / ( CInt(Log10(karma)) + 1 ) );
    karma := karma + kmod;

endfunction
*/
