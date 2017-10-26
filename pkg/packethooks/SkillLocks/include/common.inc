/*
 * $Id: common.inc 544 2006-07-04 03:13:44Z austinheilman $
 *
 */

include ":datafile:datafile";

/*
 * SLPH_GetDataFile()
 * 
 * Purpose
 * Quickly pull up the skill lock state datafile.
 *
 * Parameters
 *
 * Return Value
 * Data file reference
 *
 */
function SLPH_GetDataFile()
	var data_file := DFOpenDataFile(":skilllocks:settings", DF_CREATE);
	return data_file;
endfunction

/*
 * SLPH_GetDataElem(elem_name)
 * 
 * Purpose
 * Quickly retrieve a data elem.
 *
 * Parameters
 * elem_name:	Name of the elem to retrieve
 *
 * Return Value
 * A datafile elem reference.
 *
 */
function SLPH_GetDataElem(elem_name)
	var data_file := SLPH_GetDataFile();
	var realm_elem := DFFindElement(data_file, elem_name, DF_CREATE);
	return realm_elem;
endfunction

/*
 * SLPH_GetLockState(player, skill_id)
 * 
 * Purpose
 * Retrieves the skill lock state for a skill ID.
 *
 * Parameters
 * mobile:	Mobile to retrieve the settings for.
 * skill_id:	Skill ID to retrieve the flag for.
 *
 * Return Value
 * An integer
 *
 */
function SLPH_GetLockState(mobile, skill_id)
	var data_elem := SLPH_GetDataElem(Hex(mobile.serial));
	
	// +1 is added because skills start at 0 and arrays start at 1
	// Hex because cprop IDs need to be strings.
	skill_id := Hex(CInt(skill_id)+1);
	return CInt(data_elem.GetProp(skill_id));
endfunction

/*
 * SLPH_SaveLockState(player, skill_id, state)
 * 
 * Purpose
 * Retrieves the skill lock state for a skill ID.
 *
 * Parameters
 * mobile:	Mobile to retrieve the settings for.
 * skill_id:	Skill ID to retrieve the flag for.
 * state:	State of the skill lock.
 *
 * Return Value
 * 1
 *
 */
function SLPH_SaveLockState(mobile, skill_id, state)
	var data_elem := SLPH_GetDataElem(Hex(mobile.serial));
	
	skill_id := Hex(CInt(skill_id)+1);
	state := CInt(state);
	
	if ( state == 0 )
		data_elem.EraseProp(skill_id);
	else
		data_elem.SetProp(skill_id, state);
	endif
	
	return 1;
endfunction

