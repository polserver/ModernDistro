/////////////////////////////////
// Version 1.3
//
// DataFile.inc
//    Austin Heilman
//    Austin@tsse.net
//
// Created: 9/18/2001 8:21PM
// Updated: 6/7/2002 11:03AM
//
// =Revision 1.3
//        -- Added a global variable called 'DF_DEBUG_MODE'
//           Setting it to 0 in your script will make the functions
//           no longer SysLog() information to the console. By default
//           it is enabled.
//        -- Fixed a bug in DFSetProp() when I improved the .errortext
//           return stuff.
//
// =Revision 1.2
//	Austin
//        -- Added a flags option for DFOpenDataFile()
//        -- Added DFGetElemNames()
//        -- Changed the error stuff, to return .errortext
//
/////////////////////////////////
//
// Notes:
//    - Datafiles are package specific.
//      If you want to get a datafile from another package
//      you pretty much need to write a script that returns the
//      information and is run with a run_script_to_complection
//      *or* use start_script, pass the pid of the script that is
//      doing the starting and wait for the started script to send 
//      an event back with the needed information. The first method 
//      is easier, but also remember that it runs critical. The second 
//      is softer, but not as pretty.
//
/////////////////////////////////

use datafile;
use os;

// **************************************************************************
// * DFOPenDataFile(filename, create, flags);
// * DFGetElemNames(file_ref);
// * DFFindElement(file_ref, elem_name, create);
// * DFGetProp(elem_ref, prop_name, create, assignval);
// **************************************************************************

const DF_CREATE		:= 1;
const DF_NO_CREATE	:= 0;
// Datafile.em constants
//const DF_KEYTYPE_STRING       := 0x00; // default
//const DF_KEYTYPE_INTEGER      := 0x01;

// Set to 0 in your script to turn off SysLog() calls.
var DF_DEBUG_MODE	:= 1;

function DFOpenDataFile(filename, create:=DF_NO_CREATE, flags:=DF_KEYTYPE_STRING)
	var temp := OpenDataFile(filename);
	if ( (!temp) && (create) )
		DF_OutPut("Creating data file: " + filename);
		CreateDataFile(filename, flags);
		temp := OpenDataFile(filename);
	endif
	
	if ( temp )
		return temp;
	elseif ( create ) 
		var errmsg := error;
		errmsg.+errortext := "Could not open " + filename + ":" + temp.errortext;
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction

function DFGetElemNames(byref file_ref)
	var elem_keys := file_ref.keys();
	if ( elem_keys == error )
		var errmsg := error;
		errmsg.+errortext := "Could not return elem keys :"+elem_keys.errortext;
		DF_OutPut(errmsg.errortext);
		return errmsg;
	else
		return elem_keys;
	endif
endfunction

function DFFindElement(byref file_ref, elem_name, create:=DF_NO_CREATE)
	var temp := file_ref.FindElement( cstr(elem_name) );
	if (!temp && create)
		DF_OutPut("Creating elem: " +  cstr(elem_name) );
		file_ref.CreateElement( cstr(elem_name) );
		temp := file_ref.FindElement( cstr(elem_name) );
	endif
	
	if ( temp )
		return temp;
	elseif (create)
		var errmsg := error;
		errmsg.+errortext := "Could not open data elem " + elem_name + ":" + temp.errortext;
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction

function DFGetElemProps(elem_ref)
	var prop_names := elem_ref.keys();
	if ( prop_names == error )
		var errmsg := error;
		errmsg.+errortext := "Could not return prop names :"+prop_names.errortext;
		DF_OutPut(errmsg.errortext);
		return errmsg;
	else
		return prop_names;
	endif
endfunction

function DFGetProp(byref elem_ref, prop_name, create:=DF_NO_CREATE, assignval:=0)
	var temp := elem_ref.GetProp(prop_name);

	if ( (!temp) && (create) )
		elem_ref.SetProp(prop_name, assignval);
		temp := elem_ref.GetProp(prop_name);
	endif
	
	if ( temp || (create) && (!assignval) )
		// Second case is for creating and returning 
		// what ever you get. Used for cases where the
		// value is '0' but not an error.
		return temp;
	elseif (create)
		var errmsg := error;
		errmsg.+errortext := "Could not read data file property " + prop_name + ":" + temp.errortext;
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction

function DF_OutPut(text)
	if ( DF_DEBUG_MODE )
		SysLog(text);
	endif
endfunction
