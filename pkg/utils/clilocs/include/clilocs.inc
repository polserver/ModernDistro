
/*===============================================================
* Current Version
* CLLILOCS.INC - v1.0
* 
*
* -- Revision v1.0 --
*  Yukiko
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/*
 * CL_GetClilocsCfgFile()
 *
 * Purpose
 * Reads in :staticHousing:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function CL_GetClilocsCfgFile()
	var cfg := ReadConfigFile(":clilocs:clilocs");

	if ( cfg.errortext )
		SysLog("Error::CL_GetClilocsCfgFile() - Unable to open [:clilocs:clilocs.cfg] ->"+cfg.errortext);
	endif

	return cfg;
endfunction

/*
 * CL_GetClilocsCfgElem(elem_number, cfg_file)
 *
 * Purpose
 * Retrieves an elem from the clilocs config file. 
 *
 * Parameters
 * elem_number:	A string matching the elem number to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by CL_GetClilocsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function CL_GetClilocsCfgElem(elem_name, byref cfg_file:=0)

  if ( !cfg_file )
		cfg_file := CL_GetClilocsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

  if ( elem.errortext )
		SysLog("Error::CL_GetClilocsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif
	return elem;
endfunction

////////////////////////////////////////////////////////////
//
// CL_GetCliloc(cliloc_number)
// Purpose: Retrieves the text from a cliloc when passed
// the cliloc number.
//
// Input: A cliloc numer converted to a string.
// Returns: The text associated with the cliloc number
// or "No_cliloc_found" if it couldn't find the cliloc.
//
////////////////////////////////////////////////////////////
function CL_GetCliloc(cliloc_number)

  var element := CL_GetClilocsCfgElem("clilocs");
  var cliloctext := GetConfigString(element, cliloc_number);
  if(cliloctext.errortext)
    cliloctext := "No_cliloc_found";
  endif
  if(SubStr(cliloctext, (Len(cliloctext) - 8), 9) == " Original")
  //	SendSysMessage(who, "Yes. Original is in there.");
    cliloctext := SubStrReplace(cliloctext, "", (Len(cliloctext) - 8), 0);
  endif

  return cliloctext;
endfunction
