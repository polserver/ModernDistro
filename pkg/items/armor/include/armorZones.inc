/* $Id: armorZones.inc 1051 2007-04-25 05:17:47Z tekproxy $
 *
 */

use uo;
use os;
use util;
use cfgfile;


/*
 * CS_GetRandomArmorZone()
 *
 * Purpose
 * Reads in pol/config/armrzone.cfg and based on the 'chance' settings,
 * selects a zone that was hit.
 *
 * Parameters
 * None
 *
 * Return Value
 * String
 *
 */
function CS_GetRandomArmorZone()
	// This function assumes that the 'Chance' totals in
	// armrzone.cfg all add up to 100. You will most likely
	// get the error return value if they do not.

	var cfg_file := ReadConfigFile("::armrzone");

	//last_num stores the max roll for that zone
	var last_num := 0;
	var max_num := 0;

	//Roll determines the layer we hit.
	//In the future, we may want to have anatomy affect this?
	var roll := RandomInt(100)+1;
	foreach zone_name in ( GetConfigStringKeys(cfg_file) )
		var cfg_elem := cfg_file[zone_name];
		last_num := max_num;
		max_num := last_num + CInt(cfg_elem.Chance);
		
		if ( roll > last_num && roll <= max_num  )
			return cfg_elem.Name;
		endif
		SleepMS(2);
	endforeach
	
	return error{"errortext":="Could not select a hit zone!"};
endfunction

/* CS_GetLayersInArmorZone(zone_name)
 *
 * Purpose
 * Retrieves the layer numbers an armor zone.
 *
 * Parameters
 * zone_name:	Name of the armor zone.
 *
 * Return value
 * Array
 *
 */
function CS_GetLayersInArmorZone(zone_name)
	var cfg_file := ReadConfigFile("::armrzone");
	var cfg_elem := cfg_file[zone_name];
	
	return GetConfigStringArray(cfg_elem, "Layer");
endfunction
	
/*
 * CS_GetEquipmentInArmorZone(mobile, zone_name)
 *
 * Purpose
 * Returns a list of all equipment that a mobile is wearing
 * that covers an armor zone.
 *
 * An armor piece can cover layers other than its .layer property
 * which is why this function is here. A chainmail tunic for example,
 * often covers arms and body.
 *
 * Parameters
 * mobile:	Mobile to evaluate the equipment on.
 * zone_name:	Armor zone to find equipment for.
 *
 * Return Value
 * String
 *
 */
function CS_GetEquipmentInArmorZone(mobile, zone_name)
	var itemdesc_cfg := ReadConfigFile(":*:itemdesc");
	var equip_list := array{};
	
	zone_name := Lower(zone_name);
	
	foreach item in ( ListEquippedItems(mobile) )
		var item_info := itemdesc_cfg[item.objtype];
		
		foreach zone in ( GetConfigStringArray(item_info, "Coverage") )
			if ( Lower(zone) == zone_name )
				equip_list.Append(item);
				break;
			endif
			SleepMS(2);
		endforeach
		
		SleepMS(2);
	endforeach
	
	return equip_list;
endfunction
