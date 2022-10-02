use uo;
use os;
use cfgfile;
use util;
use basic;

include ":npcs:randNameUtil";

function OpenNamesCfgFile()

	var cfg_file := ReadConfigFile( "::names" );
	if( cfg_file.errortext )
		return error{ "errortext":="Unable to find names.cfg -> "+cfg_file.errortext };
	endif

	return cfg_file;
endfunction

function FindNamesElem( byref elem_name, byref cfg_file:=0 )

	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := OpenNamesCfgFile();
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	var cfg_elem := FindConfigElem( cfg_file, elem_name );
	if( cfg_elem.errortext )
		return error{ "errortext":="Unable to find '"+elem_name+"' in names.cfg -> "+cfg_elem.errortext };
	endif

	return cfg_elem;
endfunction

function FetchRandomName( byref elem_name )

	var cfg_elem := FindNamesElem( elem_name );
	if( cfg_elem.errortext )
		return 0;
	endif

	var names := GetConfigStringArray( cfg_elem, "Name" );
	if( names.errortext )
		return 0;
	endif

	return names.RandomEntry();
endfunction

function RandomName( character )

	var newname := " ";
	case( character.graphic )
		OBJ_BIRD:			newname := AssignName( character, "Bird" );

		OBJ_DAEMON:
		OBJ_DAEMON_SWORD:
							newname := AssignName( character, "Demonic" );

		OBJ_DRAGON_RED:
		OBJ_DRAGON_GREY:
							newname := AssignName( character, "Draconic" );

		OBJ_MALE: 			newname := AssignName( character, "HumanMale" );
		OBJ_FEMALE:			newname := AssignName( character, "HumanFemale" );

		OBJ_LIZARD:
		OBJ_LIZARD_MACE:
		OBJ_LIZARD_SPEAR:
							newname := AssignName( character, "Lizardman" );

		OBJ_ORC:
		OBJ_ORC_CLUB:
		OBJ_ORC_CAPT:
							newname := AssignName( character, "Orc" );

		OBJ_OGRE:			newname := AssignName( character, "Ogre" );

		OBJ_RATMAN:
		OBJ_RATMAN_SWORD:
		OBJ_RATMAN_AXE:
							newname := AssignName( character, "Ratman" );

		default: 		exit;
	endcase

	return newname;
endfunction

function AssignName( character, elem_name )

	return FetchRandomName( elem_name )+PreserveTitle( character );
endfunction

// Allows for passing a string or mobile_ref
function PreserveTitle( npc )

	var myname := "";
	if( TypeOfInt( npc ) == OT_STRING )
		myname := npc;
	else
		myname := npc.name;
	endif

	var start := Find( myname, ">", 1 );
	if( !start )
		return;
	endif

	var title := myname[ start+1, ( Len( myname ) - 8 ) ];
	if( !title )
		return "";
	endif

	return title;
endfunction
