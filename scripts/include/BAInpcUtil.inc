/* $Id: npcUtil.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 */
/*===============================================================
* Current Version
* NPCUTIL.INC - v1.0
* Updated 8/31/2005 4:23AM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use polsys;
use cfgfile;

/*
 * NPC_ParseTemplateName(template_name)
 *
 * Purpose
 * Finds the appropriate npcdesc.cfg to load.
 * This enables npcs to be placed in packages and not just the ::npcdesc file.
 * ReadConfigFile(":*:npcdesc") does not work in this case because packaged npcs
 * have the package name at the begining of their .npctemplate member. Reading the
 * specific config file - helps to avoid possible elem name collisions.
 *
 * Parameters
 * template_name:	Name of the npc template to parse
 *
 * Return value
 * Returns struct .package .template
 *
 */
function NPC_ParseTemplateName(template_name)
	if ( template_name.IsA(POLCLASS_NPC) )
		template_name := template_name.npctemplate;
	endif

	//Find positions of ":" characters
	var colon_a := Find(template_name, ":", 1);
	var colon_b := Find(template_name, ":", 2);

	var parsed := struct{"package", "template"};

	if ( !colon_a || !colon_b )
		//Not a complete package name - use default npcdesc.cfg
		parsed.package := "";
		parsed.template := template_name;
	else
		parsed.package := template_name[colon_a+1, colon_b-2];
		parsed.template := template_name[colon_b+1, Len(template_name)];
	endif

	return parsed;
endfunction

/*
 * NPC_GetNPCConfig(template)
 *
 * Purpose
 * Retrieves a config elem for an npc template.
 *
 * Parameters
 * info:	Template info from NPC_ParseTemplateName()
 *
 * Return value
 * Config elem reference
 *
 */
function NPC_GetNPCConfig(template)
	if ( !template.package )
		template := NPC_ParseTemplateName(template);
	endif

	var npc_cfg := ReadConfigFile(":"+template.package+":npcdesc");
	if ( npc_cfg.errortext )
		return error{"errortext":="Error::NPC_GetNPCConfig() - Could not open :"+template.package+":npcdesc ->"+npc_cfg.errortext};
	endif

	var cfg_elem := npc_cfg[template.template];
	if ( cfg_elem.errortext )
		return error{"errortext":="Error::NPC_GetNPCConfig() - Could not find elem ["+template.template+"] ->"+cfg_elem.errortext};
	endif

	return cfg_elem;
endfunction

/*
 * NPC_GetAllTemplateNames()
 *
 * Purpose
 * Retrieves a list of all npc templates in the shard's npcdesc.cfg files
 *
 * Parameters
 *
 * Return value
 * Returns an array
 *
 */
function NPC_GetAllTemplateNames()
	var elem_names := array{};
	elem_names := elem_names + GetConfigStringKeys(ReadConfigFile("::npcdesc"));
	foreach package in (Packages())
		if ( package.npcdesc )
			var pkg_name := ":"+package.name+":";
			foreach template_name in (GetConfigStringKeys(ReadConfigFile(pkg_name+"npcdesc")))
				elem_names.Append(pkg_name+template_name);
				SleepMS(2);
			endforeach
		endif
		SleepMS(2);
	endforeach

	return elem_names;
endfunction
