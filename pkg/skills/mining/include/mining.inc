/* $Id: mining.inc 564 2006-07-04 14:40:57Z austinheilman $
 *
 */

use uo;
use os;
use cfgfile;

include ":mining:settings";

/*
 * IsMinable(x, y, realm, objtype)
 *
 * Purpose
 * Determines is a coordinate set or object type is a minable tile.
 *
 * Parameters
 * x:		X coordinate to check for the landtile.
 * y:		Y coordinate to check for the landtile.
 * realm:	Realm to check the landtile on.
 * objtype:	Optional parameter. Static item selected with TargetCoordinates()
 *
 * Return Values
 * Returns 1 if the landtile or static object clicked are minable.
 * Returns 0 if not.
 *
 */
function IsMinable(x, y, realm, objtype:=0)
	var cfg_elem := MSP_GetSettingsCfgElem("MinableTiles");

	var land_scape :=  GetMapInfo(x, y, realm).landtile;
	if ( CStr(land_scape) in (GetConfigStringArray(cfg_elem, "Landscape")) )
		return 1;
	elseif ( objtype )
		if ( CStr(objtype) in (GetConfigStringArray(cfg_elem, "Static")) )
			return 1;
		endif
	endif

	return 0;
endfunction

/*
 * ListOreTypes()
 *
 * Purpose
 * Retrieves a list of ores that can be mined from settings.cfg
 *
 * Parameters
 *
 * Return Values
 * Returns an array of strings.
 *
 */
function ListOreTypes()
	var ore_types_elem := MSP_GetSettingsCfgElem("OreTypes");
	return GetConfigStringArray(ore_types_elem, "OreType");
endfunction

/*
 * ListOreVeinsNearMobile(mobile, range)
 *
 * Purpose
 * Finds ore veins that are near a mobile.
 *
 * Parameters
 * targ:	Struct (.x, .y, .z, .realm) to search around.
 * range:	Distance to search.
 *
 * Return Values
 * Returns an array of item references
 *
 */
function ListVeinsNearMobile(targ, range)
	var vein_list := array;
	foreach item in ( ListItemsNearLocation(targ.x, targ.y, targ.z, range, targ.realm) )
		if ( item.IsOreVein() )
			vein_list.Append(item);
		endif
		SleepMS(2);
	endforeach

	return vein_list;
endfunction