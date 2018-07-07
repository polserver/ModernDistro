//$Id: attributes_ex.inc 569 2006-07-04 19:06:18Z austinheilman $

/*===============================================================
* Current Version
* ATTRIBUTES_EX.INC - v1.0
* Updated 9/20/2005 11:42AM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

include ":attributes:attributes";

/*
 * AP_SkillIdToAttributeName(skill_number)
 *
 * Purpose
 * Gets the name of the attribute that is associated with a skill number.
 * uoskills.cfg -> Attributes.cfg
 *
 * Parameters
 * skill_number:	Skill ID to convert
 *
 * Return value
 * Returns a string.
 *
 */
function AP_SkillIdToAttributeName( skill_number )

	var skills_cfg := ReadConfigFile( ":attributes:uoskills" );
	if( skills_cfg.errortext )
		SysLog( "Error::AP_IDToAttributeName() - Unable to open :attributes:uoskills.cfg ->"+skills_cfg.errortext );
	endif

	var cfg_elem := skills_cfg[skill_number];
	if( cfg_elem.errortext )
		SysLog( "Error::AP_IDToAttributeName() - unable to find cfg elem ["+skill_number+"] ->"+cfg_elem.errortext );
	endif

        return cfg_elem.Attribute;
endfunction

/*
 * AP_AttributeNameToSkillId(attribute_name)
 *
 * Purpose
 * Gets the skill number (ID) that an attribute name is associated with.
 * Attributes.cfg -> uoskills.cfg
 *
 * Parameters
 * attribute_name:	Attribute name to convert.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_AttributeNameToSkillId( attribute_name )

	var cfg_elem := AP_GetAttributeCfgElem( attribute_name );

	return cfg_elem.SkillId;
endfunction
