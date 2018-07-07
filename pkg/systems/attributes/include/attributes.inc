//$Id: attributes.inc 1134 2008-01-16 13:42:02Z austinheilman $

/*===============================================================
* Current Version
* ATTRIBUTES.INC - v1.0
* Updated 9/24/2005 6:52PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;
use vitals;
use attributes;

include ":attributes:advanceCheck";
include ":attributes:attributeConstants";
include ":attributes:caps";
include ":attributes:regen";
include ":attributes:settings";
include ":attributes:skillCheck";
include ":attributes:skills";
include ":attributes:stats";
include ":attributes:vitals";
include ":attributes:titles";
include ":datafile:datafile";

/* AP_GetAttributesCfgFile()
 * 
 * Purpose
 * Loads attributes.cfg
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function AP_GetAttributesCfgFile()

	var cfg_file := ReadConfigFile( ":attributes:attributes" );
	if( cfg_file.errortext )
		SysLog( "Error::AP_GetAttributesCfgFile() - Unable to open attributes.cfg ->"+cfg_file.errortext );
	endif

	return cfg_file;
endfunction

/*
 * AP_GetAttributeCfgElem(attribute_name, cfg_file)
 *
 * Purpose
 * Retrieves a config elem for an attribute in attributes.cfg
 *
 * Parameters
 * attribute_name:	Name of the attribute to get the elem of.
 * cfg_file:		Optional parameter - reference to a config already read in.
 *
 * Return value
 * Returns a config file elem.
 *
 */
function AP_GetAttributeCfgElem( attribute_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := AP_GetAttributesCfgFile();
	endif

	var cfg_elem := cfg_file[attribute_name];
	if( cfg_elem.errortext )
		SysLog( "Error::AP_GetAttributeCfgElem() - unable to find cfg elem ["+attribute_name+"] ->"+cfg_elem.errortext );
	endif

	return cfg_elem;
endfunction

/* AP_GetAttributesDataFile()
 * 
 * Purpose
 * Loads the attributes datafile.
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function AP_GetAttributesDataFile()

	var data_file := DFOpenDataFile( ":attributes:attributes", DF_CREATE );
	if( data_file.errortext )
		SysLog( "Error::AP_GetAttributesDataFile() - Unable to open attributes.txt ->"+data_file.errortext );
	endif

	return data_file;
endfunction

/*
 * AP_GetAttributeDataElem(attribute_name, cfg_file)
 *
 * Purpose
 * Retrieves a datafile elem from the attributes package.
 *
 * Parameters
 * attribute_name:	Name of the elem to retrieve.
 *
 * Return value
 * Returns a datafile elem.
 *
 */
function AP_GetAttributeDataElem( elem_name )

	var data_file := AP_GetAttributesDataFile();
	var data_elem := DFFindElement( data_file, elem_name, DF_CREATE );

	if( data_elem.errortext )
		SysLog( "Error::AP_GetAttributeDataElem() - unable to find elem ["+elem_name+"] ->"+data_elem.errortext );
	endif

	return data_elem;
endfunction

/*
 * AP_ListAttributesByType(type)
 *
 * Purpose
 * Gets the name of all attributes of a certain type.
 *
 * Parameters
 * Type:	String containing the type of attributes to list.
 *
 * Return value
 * Returns an array
 *
 */
function AP_ListAttributesByType( type:="" )

	if( !type )
		var cfg_file := ReadConfigFile( ":attributes:attributes" );
		return GetConfigStringKeys( cfg_file );
	endif

	type := Lower( type );
	var data_elem := AP_GetAttributeDataElem( "Categories" );
	return data_elem.GetProp( type );
endfunction
