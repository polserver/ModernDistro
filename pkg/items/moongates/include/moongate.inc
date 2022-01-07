/*
 * $Id$
 *
 */

use uo;
use os;
use polsys;
use cfgfile;

include ":datafile:datafile";

// constants for the moongate gump.
CONST MG_CANCEL_BTN	:= 0x0;
CONST MG_OKAY_BTN	:= 0x10;
CONST MG_LOCATION_START := 0xA00;


/*
 * MG_GetRealmDataFile()
 * 
 * Purpose
 * Quickly pull up the datafile.
 *
 * Parameters
 *
 * Return Value
 * Data file reference
 *
 */
function MG_GetDataFile()
	var data_file := DFOpenDataFile(":moongates:gateinfo", DF_CREATE);
	return data_file;
endfunction

/*
 * MG_GetRealmDataElem(elem_name)
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
function MG_GetDataElem(elem_name)
	var data_file := MG_GetDataFile();
	var realm_elem := DFFindElement(data_file, elem_name, DF_CREATE);

	return realm_elem;
endfunction

/*
 * MG_FindDestination(button_id)
 *
 * Purpose
 * Takes in a radiobox id value and finds the match for it.
 *
 * Parameters
 * button_id:	Radio box that was checked.
 *
 * Return Value
 * 
 * Returns a struct with .x, .y, .z, .realm
 *
 */
function MG_FindDestination(button_id)
	// Not too happy with how I designed this part. - Austin
	// Should probably save a list in a datafile and directly access it by the 'select' variable.
	
	var config := ReadConfigFile(":moongates:moongate");
	var btn_id := 1;
	foreach realm in ( Realms() )
		realm := realm; // To avoid a compiler warning.
		var cfg_elem := config[_realm_iter];
		foreach entry in ( GetConfigStringDictionary(cfg_elem, "Town") )
			if ( btn_id == button_id )
				var coords := SplitWords(entry);
				var location := struct;
				location.+x := CInt(coords[1]);
				location.+y := CInt(coords[2]);
				location.+z := CInt(coords[3]);
				location.+realm := CStr(_realm_iter);
				return location;
			endif
			btn_id := btn_id+1;
			SleepMS(2);
		endforeach
		SleepMS(2);
	endforeach
	
endfunction