/* $Id: traps.inc 1105 2007-12-20 19:44:24Z tekproxy $
 *
 */

use uo;
use cfgfile;
use util;
use os;

/*
 * GetTrapInfo(trap_name)
 *
 * Purpose
 * Retrieves the config elem for a trap from traps.cfg
 *
 * Parameters
 * trap_name:	Config elem name of the trap.
 *
 * Return value
 * Returns a config elem on success.
 * Returns an error on failure.
 *
 */
function GetTrapInfo(trap_name)
	var trap_cfg := ReadConfigFile(":traps:traps");
	if ( trap_cfg.errortext )
		var temp := error{"errortxt":="GetTrapInfo()::Could not open :traps:traps.cfg ->"+trap_cfg.errortext};
		SysLog(temp.errortext);
		return temp;
	endif

	var trap_elem := trap_cfg[trap_name];
	if ( trap_elem.errortext )
		var temp := error{"errortext":="GetTrapInfo()::Could not find elem ["+trap_name+"] -> "+trap_elem.errortext};
		SysLog(temp.errortext);
		return temp;
	endif

	return trap_elem;
endfunction

/*
 * AddTrap(object, trap_name, strength, count := 0)
 *
 * Purpose
 * Adds a trap to an object (its use-script must use this include file)
 *
 * Parameters
 * object:	Object to add the trap to.
 * trap_name:	Name of the trap to add.
 * strength:	How powerful the trap will be.
 * count:	How many times the trap can be triggered when the object is used.
 *		See traps.cfg for more information on 'count'
 *
 * Return value
 * Returns 1 on success.
 * Returns an error on failure.
 *
 */
function AddTrap(object, trap_name, strength, count:=0)
	var trap_list := GetObjProperty(object, "TrapList");
	var tempstrength;
	if ( Lower(TypeOf(trap_list)) != "array" )
		trap_list := array{};
	endif

	var trap_elem := GetTrapInfo(trap_name);
	if ( trap_elem.errortext )
		return trap_elem; // Contains an error
	endif

	if ( !count )
		count := trap_elem.Count;
		if ( !count )
			count := -1;
		elseif ( find(count, "d", 1) )
			count := RandomDiceRoll(trap_elem.count);
		else
			count := CInt(trap_elem.Count);
		endif
	endif

	// Check for random die string passed for trap strength.
	if( TypeOf( strength == "String" ) )
		tempstrength := RandomDiceRoll( strength );
		if( tempstrength.errortext )
			tempstrength := RandomDiceRoll( "4d5" );
		endif
		strength := tempstrength;
	endif

	var trap := struct;
	trap.+Name := trap_name;
	trap.+Strength := strength;
	trap.+Count := count;

	trap_list.Append(trap);

	SetObjProperty(object, "TrapList", trap_list);

	return 1;
endfunction

/*
 * RemoveTrap(object, index)
 *
 * Purpose
 * Removes a trap from the object's trap list.
 *
 * Parameters
 * object:	Object to remove the trap from.
 * index:	Index in the list's array to remove the trap from.
 *
 * Return value
 * Returns 1 on success.
 * Returns an error on failure.
 *
 */
function RemoveTrap(object,  index:="all")
	var trap_list := GetObjProperty(object, "TrapList");
	if ( trap_list == error )
		return error{"errortext":="Object has no traps."};
	endif

	trap_list.Erase(index);

	if ( trap_list.Size() > 0 )
		SetObjProperty(object, "TrapList", trap_list);
	else
		EraseObjProperty(object, "TrapList");

		// Remove "[trapped]" from name
		if ( GetObjProperty(object, "TrapsRevealed") )
			var tempStr := SplitWords(object.name, "[trapped]");
			var newName := "";

			foreach part in tempStr
				newName += part;
			endforeach

			object.name := newName;
			EraseObjProperty(object, "TrapsRevealed");
		endif
	endif

	return 1;
endfunction

/*
 * RunTraps(object, mobile)
 *
 * Purpose
 * Runs all traps that are set on the object.
 *
 * Parameters
 * object:	Object to run the traps on.
 * mobile:	Who triggered the traps.
 *
 * Return value
 * Returns 1 on completion.
 *
 */
function RunTraps(object, mobile)
	var trap_list := GetObjProperty(object, "TrapList");
	if ( trap_list == error )
		return 0;
	endif

	foreach trap_info in ( trap_list )
		var cfg_info := GetTrapInfo(trap_info.name);

		var result := Start_Script(cfg_info.Script, {mobile, object, trap_info, _trap_info_iter});
		if ( result.errortext )
			PrintTextAbove(object, result.errortext);
			continue;
		endif

		if ( trap_info.Count > 0 ) // -1 = permanent trap
			trap_info.Count := trap_info.Count - 1;

			if ( trap_info.count == 0 )
				RemoveTrap(object, _trap_info_iter);
			endif
		endif
		SleepMS(2);
	endforeach

	return 1;
endfunction

/*
 * GetTrapList(object)
 *
 * Purpose
 * Retrieves the trap list from an object.
 *
 * Parameters
 * object:	Object to get trap list from
 *
 * Return value
 * Returns trap list or empty array if none found
 *
 */
function GetTrapList(object)
	var trap_list := GetObjProperty(object, "TrapList");

	if ( Lower(TypeOf(trap_list)) != "array" )
		trap_list := array{};
	endif

	return trap_list;
endfunction

/*
 * RevealTrap(object)
 *
 * Purpose
 * Adds "[trapped]" to object name and sets cprop on object to keep track of it being revealed.
 *
 * Parameters
 * object:	Object to reveal traps on (if any)
 *
 * Return value
 * int number of traps revealed
 *
 */
function RevealTraps(object)
	if ( !GetObjProperty(object, "TrapsRevealed") )
		var trapCount := Len(GetTrapList(object));

		if ( trapCount )
			SetObjProperty(object, "TrapsRevealed", 1);
			SetName(object, object.name + " [trapped]");
			return trapCount;
		endif
	endif

	return 0;
endfunction

/*
 * IsTrapRevealed(object)
 *
 * Purpose
 * Provides a layer of abstraction for checking if a trap has been revealed.
 *
 * Parameters
 * object:	Object ref
 *
 * Return value
 * 1 - has been revealed
 * 0 - has NOT been revealed
 *
 */
function IsTrapRevealed(object)
	return GetObjProperty(object, "TrapsRevealed");
endfunction