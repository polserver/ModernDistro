//$Id: regen.inc 373 2006-06-17 18:27:33Z austinheilman $

/*===============================================================
* Current Version
* REGEN.INC - v1.0
* Updated 9/21/2005 8:14AM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;

/*
 * AP_SetVitalRegenRate(mobile, vital_name, number)
 *
 * Purpose
 * Sets the regeneration rate modifier for an attribute.
 * Base values are in settings.cfg (multiply by 100)
 *
 * Parameters
 * mobile:	Mobile reference to set the modifier on.
 * skill_name:	Name of the vital to set the regen modifier for.
 * number:	Modifier (+/-) points to set for the regen rate.
 *
 * Return value
 * Returns 1
 *
 */
function AP_SetVitalRegenRateMod( mobile, vital_name, number )

	vital_name := Lower( vital_name );
	number := CInt( number );

	var regen_prop := AP_LoadVitalRegenRateProp( mobile );

	if( number == 0 )
		// Remove keys with no modifier
		regen_prop.Erase( vital_name );
	else
		regen_prop[vital_name] := CInt( number );
	endif

	AP_SaveVitalRegenRateProp( mobile, regen_prop );
 
	return 1;
endfunction

/*
 * AP_GetVitalRegenRateMod(mobile, vital_name)
 *
 * Purpose
 * Gets the regeneration rate modifier for a vital.
 *
 * Parameters
 * mobile:	Mobile reference to get the regen information from.
 * vital_name:	Name of the vital to get the regeneration rate for.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_GetVitalRegenRateMod( mobile, vital_name )

	vital_name := Lower( vital_name );

	var regen_prop := AP_LoadVitalRegenRateProp( mobile );
	return CInt( regen_prop[vital_name] );
endfunction

/*
 * AP_GetVitalRegenRateMod(mobile, vital_name)
 *
 * Purpose
 * Gets the regeneration rate modifier for a vital.
 *
 * Parameters
 * mobile:	Mobile reference to get the regen information from.
 * vital_name:	Name of the vital to get the regeneration rate for.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_LoadVitalRegenRateProp( mobile )

	var regen_prop := GetObjProperty( mobile, "%RegenRates%" );
	if( Lower( TypeOf( regen_prop )) != "dictionary" )
		regen_prop := dictionary;
	endif

	return regen_prop;
endfunction

/*
 * AP_SaveVitalRegenRateProp(mobile, regen_prop)
 *
 * Purpose
 * Saves the dictionary of regeneration settings on a mobile.
 * Updates the regeneration rate on the mobile.
 *
 * Parameters
 * mobile:	Mobile reference to save the regen information on.
 * regen_prop:	Dictionary containing regeneration modifier values.
 *
 * Return value
 * Returns the result of saving the property.
 *
 */
function AP_SaveVitalRegenRateProp( mobile, byref regen_prop )

	if( Lower( TypeOf( regen_prop )) != "dictionary" )
		return error{"errortext":="Regen prop must be a dictionary."};
	endif

	var result := SetObjProperty( mobile, "%RegenRates%", regen_prop );
	if( result != error )
		RecalcVitals( mobile );
	endif

	return result;
endfunction
