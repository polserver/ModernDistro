/* $Id: equipment.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 * Purpose
 * Allows modifiers to be easily set and removed.
 * Supports itemdesc.cfg modifiers and custom modifiers.
 *
 * Note:
 * AP_GetStatMod() and AP_GetSkillMod()
 * Will work on skills as well. They're more-so formalities.
 * Mostly we expect objects to edit stats so that is why that function name
 * was chosen to be used in this script.
 *
 */
use uo;
use cfgfile;

include ":attributes:attributes";

CONST	EQUIP_ITEMDESC_MODS	:= 0x1;
CONST	EQUIP_CUSTOM_MODS	:= 0x2;
CONST	EQUIP_BOTH_MODS		:= 0x3;
CONST	EQUIP_CSTM_OR_ITEMDESC	:= 0x4;


/* 
 * GetEquipmentModifiers(item, flags:=EQUIP_CSTM_OR_ITEMDESC)
 *
 * Purpose
 * Retrieves the attribute modifiers for an item.
 *
 * Parameters
 * item:	Item to retrieve modifier information for.
 * flags:	Where to retrieve the modifiers from.
 *		EQUIP_ITEMDESC_MODS	- Retrieve modifiers from the item's itemdesc.cfg entry.
 *		EQUIP_CUSTOM_MODS	- Retrieve custom modifiers set on the item.
 *		EQUIP_ALL_MODS		- Merge dictionaries and use them all.
 *		EQUIP_CUSTM_OR_ITEMDESC	- Uses custom modifiers. If none exist, uses itemdesc.cfg entries.
 *
 * Return value
 * Returns a dictionary.
 *
 */
function GetEquipmentModifiers(item, flags:=EQUIP_CSTM_OR_ITEMDESC)
	var cstm_mods;
	if ( flags & EQUIP_CUSTOM_MODS || flags & EQUIP_CSTM_OR_ITEMDESC )
		cstm_mods := GetObjProperty(item, "AttribMods");
		if ( Lower(TypeOf(cstm_mods)) != "dictionary" )
			cstm_mods := dictionary;
		endif
		
		if ( flags & EQUIP_CSTM_OR_ITEMDESC )
			// Custom mods found, so don't use itemdesc.cfg ones.
			return cstm_mods;
		endif
	endif
	
	var itemdesc_mods;
	if ( flags & EQUIP_ITEMDESC_MODS || flags & EQUIP_CSTM_OR_ITEMDESC )
		var item_cfg := ReadConfigFile(":*:itemdesc");
		item_cfg := item_cfg[item.objtype];
		itemdesc_mods := GetConfigStringDictionary(item_cfg, "AttribMod");
	endif
	
	if ( flags & EQUIP_BOTH_MODS )
		// Note: Custom mods override itemdesc.cfg ones here.
		foreach modifier in ( cstm_mods )
			modifier := modifier; // So the compiler does not issue a warning.
			itemdesc_mods.Insert(_modifier_iter, modifier);
			SleepMS(2);
		endforeach
	endif
	
	if ( itemdesc_mods.Size() > 0 )
		return itemdesc_mods;
	else
		return cstm_mods;
	endif
endfunction

	
/*
 * AddModifierToEquipment(item, attrib_name, modifier)
 *
 * Purpose
 * Adds a custom modifier to a piece of equipment.
 * Useful for making an item magic and as a result effects
 * attributes when equipped / unequipped.
 * 
 * Parameters
 * item:	Item to set the attribute modifier on.
 * attrib_name:	Name of the attribute to modify when equipped / un-equipped.
 * modifier:	Amount to raise or lower the attribute.
 * 
 * Return value
 * Returns 1 on success
 * Returns error on failure.
 */
function AddModifierToEquipment(item, attrib_name, modifier)
	if ( !item.IsA(POLCLASS_EQUIPMENT) )
		return error{"errortext":="That is not a member of the equipment class."};
	endif
	
	var cstm_mods := GetObjProperty(item, "AttribMods");
	if ( Lower(TypeOf(cstm_mods)) != "dictionary" )
		cstm_mods := dictionary;
	endif
	
	modifier := CDbl(modifier);
	if ( modifier == 0.0 )
		return error{"errortext":="No modifier value was set."};
	elseif ( !AP_GetAttributeCfgElem(attrib_name) )
		return error{"errortext":="That is not a valid attribute name."};
	endif
	
	cstm_mods[attrib_name] := modifier;
	
	return SetObjProperty(item, "AttribMods", cstm_mods);
endfunction


/*
 * RemoveModifierFromEquipment(item, attrib_name)
 *
 * Purpose
 * Removes a custom modifier from a piece of equipment.
 * Useful for removing effects that it places on
 * attributes when equipped / unequipped.
 * 
 * Parameters
 * item:	Item to remove the attribute modifier from.
 * attrib_name:	Name of the attribute modifier to remove.
 * 
 * Return value
 * Returns 1 on success
 * Returns error on failure.
 */
function RemoveModifierFromEquipment(item, attrib_name)
	if ( !item.IsA(POLCLASS_EQUIPMENT) )
		return error{"errortext":="That is not a member of the equipment class."};
	endif
	
	var cstm_mods := GetObjProperty(item, "AttribMods");
	cstm_mods.Erase(attrib_name);
	
	if ( cstm_mods > 0 )
		return SetObjProperty(item, "AttribMods", cstm_mods);
	else
		// No point in keeping an empty dictionary cprop.
		return EraseObjProperty(item, "AttribMods");
	endif
endfunction


/* 
 * ApplyEquipmentMods(mobile, item)
 *
 * Purpose
 * Applies skill and stat mods onto a mobile equipping something.
 *
 * Parameters
 * mobile:	Mobile equipping the item.
 * item:	Item being equipped.
 * flags:	Where to retrieve the modifiers from.
 *		EQUIP_ITEMDESC_MODS	- Retrieve modifiers from the item's itemdesc.cfg entry.
 *		EQUIP_CUSTOM_MODS	- Retrieve custom modifiers set on the item.
 *		EQUIP_ALL_MODS		- Merge dictionaries and use them all.
 *		EQUIP_CUSTM_OR_ITEMDESC	- Uses custom modifiers. If none exist, uses itemdesc.cfg entries.
 *
 * Return value
 * Returns 1
 *
 */
function ApplyEquipmentMods(mobile, item, flags := EQUIP_CSTM_OR_ITEMDESC)
	var attrib_mods := GetEquipmentModifiers(item, flags);
			
	foreach modifier in ( attrib_mods )
		modifier := modifier; // Stops compiler warnings.
		var attrib_name := _modifier_iter;
		var amount := CDbl(modifier);
		
		var cur_val := AP_GetStatMod(mobile, attrib_name);
		var new_val := cur_val + amount;
		
		//SendSysMessage(mobile, "Attrib["+attrib+"] Amount["+new_val+"]");
		AP_SetStatMod(mobile, attrib_name, new_val);
		
		SleepMS(2);
	endforeach
	
	return 1;
endfunction


/* 
 * RemoveEquipmentMods(mobile, item)
 *
 * Purpose
 * Removes skill and stat mods onto a mobile equipping something.
 *
 * Parameters
 * mobile:	Mobile un-equipping the item.
 * item:	Item being un-equipped.
 * flags:	Where to retrieve the modifiers from.
 *		EQUIP_ITEMDESC_MODS	- Retrieve modifiers from the item's itemdesc.cfg entry.
 *		EQUIP_CUSTOM_MODS	- Retrieve custom modifiers set on the item.
 *		EQUIP_ALL_MODS		- Merge dictionaries and use them all.
 *		EQUIP_CUSTM_OR_ITEMDESC	- Uses custom modifiers. If none exist, uses itemdesc.cfg entries.
 *
 * Return value
 * Returns 1
 *
 */
function RemoveEquipmentMods(mobile, item, flags:=EQUIP_CSTM_OR_ITEMDESC)
	var attrib_mods := GetEquipmentModifiers(item, flags);
	
	foreach modifier in ( attrib_mods )
		modifier := modifier; // Stops compiler warnings.
		var attrib_name := _modifier_iter;
		var amount := CDbl(modifier);
		
		var cur_val := AP_GetStatMod(mobile, attrib_name);
		var new_val := cur_val - amount;
		
		//SendSysMessage(mobile, "Attrib["+attrib+"] Amount["+amount+"]");
		AP_SetStatMod(mobile, attrib_name, new_val);
		
		SleepMS(2);
	endforeach
	
	return 1;
endfunction
