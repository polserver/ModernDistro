/*
 * $Id: food.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 */

use os;
use util;
use cfgfile;

/*
 * RandomFoodObjType(category:="All")
 *
 * Purpose
 * Selects an objtype from food.cfg
 *
 * Parameters
 * category:	Elem to use in food.cfg
 *
 * Return value
 * Returns a number.
 *
 */
function RandomFoodObjType(category:=0)
	var cfg_file := ReadConfigFile(":food_drink:food");
	if ( cfg_file.errortext )
		return error{"errortext":="Unable to read food.cfg ->"+cfg_file.errortext};
	endif

	var food_list := array{};
	if ( category )
		var cfg_elem := cfg_file[category];
		food_list := GetConfigStringArray(cfg_elem, "Food");
	else
		foreach elem_name in ( GetConfigStringKeys(cfg_file) )
			var cfg_elem := cfg_file[elem_name];
			var temp_list := GetConfigStringArray(cfg_elem, "Food");
			if ( temp_list.Size() > 0 )
				food_list := food_list + temp_list;
			endif
			SleepMS(2);
		endforeach
	endif

	if ( food_list.Size() > 0 )
		return CInt(food_list.randomentry());
	else
		return error{"errortext":="No food items found in food.cfg"};
	endif
endfunction