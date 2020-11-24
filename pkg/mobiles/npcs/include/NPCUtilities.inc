


use uo;
use cfgfile;

var npc_cfg := ReadConfigFile(":*:npcdesc");

///////////////////////////////////////////////////////////
//
// IsUndead( mobile )
//
// Purpose: Determines if the passed NPC is an undead.
//
//
//
// Parameter: mobile::MobileRef
//
// Returns: 1 or 0::integer
//
///////////////////////////////////////////////////////////
function IsUndead(mobile)

	var template := mobile.npctemplate;
	if(!template)
		return 0;
	endif
	// Strip off any package info.
	template := StripPkg(template);
	var elem := FindConfigElem(npc_cfg, template);
	var elemental := GetConfigString(elem, "elemental");
	if(Lower(elemental) == "undead")
		return 1;
	else
		return 0;
	endif

endfunction

///////////////////////////////////////////////////////////
//
// GetAlignment( mobile )
//
// Purpose: Retrieves the alignment of an NPC and
//			converts it to a string representation.
//
//
//
// Parameter: mobile::MobileRef
//
// Returns: Either "neutral", "evil" or "good"::string
//
///////////////////////////////////////////////////////////

function GetAlignment(mobile)

	var align := mobile.alignment;
	if(!mobile.npctemplate)
		return "not NPC";
	endif

	case(align)
		0: return "neutral";
		1: return "evil";
		2: return "good";
	endcase

endfunction


///////////////////////////////////////////////////////////
//
// StripPkg( template )
//
// Purpose: Strip the package name from an NPC template
// 			if present. This is an internal
//
//
// Parameter: template::string
//
// Returns:	clean template::string
//
///////////////////////////////////////////////////////////
function StripPkg(template)

	// Strip off any package info.
	var i, j;
	i := template.Find(":", 1);
	if(i)
		j := template.find(":", 2);
		template := template[j + 1, Len(template)];
	endif
	return template;

endfunction