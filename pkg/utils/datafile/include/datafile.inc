/* $Id: datafile.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 * Purpose
 * TODO
 *
 * Function List *
 * DFFindElement(byref file_ref, elem_name, create)
 * DFGetElemNames(byref file_ref)
 * DFGetElemProps(elem_ref)
 * DFGetProp(byref elem_ref, prop_name, create, assign_val)
 * DFOpenDataFile(file_name, create, flags)
 * CfgToDF(cfg_name, df_name, propnames)
 * DFToCfg(df_name, cfg_name)
 *
 * Private Function List *
 * DF_OutPut(text)
 *
 * Global Variables *
 * Datafile.em CONSTants
 * CONST DF_KEYTYPE_STRING       := 0x00; // default
 * CONST DF_KEYTYPE_INTEGER      := 0x01;
 *
 */
use uo;
use os;
use file;
use basic;
use cfgfile;
use datafile;

// Determines if a file or elem will be created when a read attempt is made.
CONST DF_NO_CREATE	:= 0;
CONST DF_CREATE		:= 1;

// Set to 0 in your script to turn off SysLog() calls.
var DF_DEBUG_MODE	:= 1;


/*
 * DFOpenDataFile(filename, create, flags)
 *
 * Purpose
 * Creates a datafile handler.
 *
 * Parameters
 * filename:	Path and name of the data file. (Example ":pkg-name:file-name")
 * create:	Creation flags:
 *		DF_NO_CREATE	Will return an error if the datafile does not exist. *Default
 *		DF_CREATE	Will create the datafile if it does not exist.
 * flags:	Datafile.em flags:
 *		DF_KEYTYPE_STRING	Elem names will be handled as strings.
 *		DF_KEYTYPE_INTEGER	Elem names will be handled as integers.
 *
 * Return value
 * A datafile reference.
 *
 */
function DFOpenDataFile(file_name, create:=DF_NO_CREATE, flags:=DF_KEYTYPE_STRING)
	var data_file := OpenDataFile(file_name);
	if ( (!data_file) && (create) )
		DF_OutPut("Debug::DFOpenDataFile() - Creating data file: "+file_name);
		CreateDataFile(file_name, flags);
		data_file := OpenDataFile(file_name);
	endif

	if ( data_file )
		return data_file;
	elseif ( create )
		var errmsg := error{"errortext":="Error::DFOpenDataFile() - Could not open "+file_name+" : "+data_file.errortext};
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction

/*
 * DFPurgeFile(byref file_ref)
 *
 * Purpose
 * Erases all elems in a datafile. (WIPES IT CLEAN)
 *
 * Parameters
 * file_ref:	The datafile to wipe.
 *
 * Return value
 * Returns 1
 *
 */
function DFPurgeFile(byref file_ref)
	foreach elem_name in ( DFGetElemNames(file_ref) )
		file_ref.DeleteElement(elem_name);
		SleepMS(2);
	endforeach
	
	return 1;
endfunction

/*
 * DFGetElemNames(byref file_ref)
 *
 * Purpose
 * Retrieves a list of all elem names that are in a data file.
 *
 * Parameters
 * file_ref:	The datafile to retrieve the elem names in.
 *
 * Return value
 * An array of strings or integers.
 *
 */
function DFGetElemNames(byref file_ref)
	var elem_keys := file_ref.keys();
	if ( elem_keys == error )
		var errmsg := error{"errortext":="Error::DFGetElemNames() - Could not return elem keys :"+elem_keys.errortext};
		DF_OutPut(errmsg.errortext);
		return errmsg;
	else
		return elem_keys;
	endif
endfunction


/*
 * DFFindElement(byref file_ref, elem_name, create)
 *
 * Purpose
 * Creates a datafile elem handler.
 *
 * Parameters
 * file_ref:	Data file to retrieve/create the element in.
 * elem_name:	Name of the element to retrieve.
 * create:	Creation flags:
 *		DF_NO_CREATE	Will return an error if the data elem does not exist. *Default
 *		DF_CREATE	Will create the data elem if it does not exist.
 *
 * Return value
 * A data file element reference.
 *
 */
function DFFindElement(byref file_ref, elem_name, create:=DF_NO_CREATE)
	var temp := file_ref.FindElement(CStr(elem_name));
	if ( !temp && create )
		DF_OutPut("Debug::DFFindElement() - Creating elem: "+CStr(elem_name));
		file_ref.CreateElement(CStr(elem_name));
		temp := file_ref.FindElement(CStr(elem_name));
	endif

	if ( temp )
		return temp;
	elseif (create)
		var errmsg := error{"errortext":="Error::DFFindElement() - Could not open data elem ["+elem_name+"] - "+temp.errortext};
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction


/*
 * DFGetElemProps(byref elem_ref)
 *
 * Purpose
 * Retrieves a list of all prop names that are in a data elem.
 *
 * Parameters
 * elem_ref:	The data elem to retrieve the property names in.
 *
 * Return value
 * An array of strings.
 *
 */
function DFGetElemProps(elem_ref)
	var prop_names := elem_ref.PropNames();
	if ( prop_names == error )
		var errmsg := error{"errortext":="Error::DFGetElemProps() - Could not return prop names :"+prop_names.errortext};
		DF_OutPut(errmsg.errortext);
		return errmsg;
	else
		return prop_names;
	endif
endfunction


/*
 * DFGetProp(byref elem_ref, prop_name, create, assign_val)
 *
 * Purpose
 * Retrieves the information in a cprop, or assigns it a value if told to do-so.
 *
 * Parameters
 * elem_ref:	The data elem to retrieve the property from.
 * prop_name:	The name of the property to retrieve.
 * create:	Creation flags:
 *		DF_NO_CREATE	Will return an error if the property does not exist. *Default
 *		DF_CREATE	Will create the property if it does not exist and an assign value is set.
 * assign_val:	Value to set the property to if it is created.
 *
 * Return value
 * The value of the property.
 *
 */
function DFGetProp(byref elem_ref, prop_name, create:=DF_NO_CREATE, assign_val:=0)
	var temp := elem_ref.GetProp(prop_name);

	if ( (temp==error) && (create) )
		elem_ref.SetProp(prop_name, assign_val);
		temp := elem_ref.GetProp(prop_name);
	endif

	if ( temp || (create) && (!assign_val) )
		// Second case is for creating and returning
		// what ever you get. Used for cases where the
		// value is '0' but not an error.
		return temp;
	elseif ( create )
		var errmsg := error{"errortext":="Error::DFGetProp() - Could not read data file property "+prop_name+":"+temp.errortext};
		DF_OutPut(errmsg.errortext);
		return errmsg;
	endif
endfunction


/*
 * DF_OutPut(text)
 *
 * Purpose
 * Internal function that is used to control system logging.
 *
 * Parameters
 * text:	Text to log.
 *
 * Return value
 * Returns 1
 *
 */
function DF_OutPut(text)
	if ( DF_DEBUG_MODE )
		//SysLog(text);
		var script_name := GetProcess(GetPid()).name;
		LogToFile("::log/dataFile.log", "["+script_name+"]: "+text, LOG_DATETIME);
	endif
	
	return 1;
endfunction
