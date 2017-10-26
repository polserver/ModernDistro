//randname.inc
//
//assigns a random name from the list according to obtype and gender

use uo;
use os;
use cfgfile;
use util;
use basic;

include "include/randNameUtil";

function RandomName(character)
	var newname := " ";
	case( character.graphic )
	    OBJ_BIRD:		newname := AssignName( character, BIRD_IDX );
	    OBJ_DEMON:		newname := AssignName( character, DEMON_IDX );
	    OBJ_DEMON_SWORD:	newname := AssignName( character, DEMON_IDX );
	    OBJ_DRAGON_RED:	newname := AssignName( character, DRAGON_IDX );
	    OBJ_DRAGON_GREY: 	newname := AssignName( character, DRAGON_IDX );
	    OBJ_MALE: 		newname := AssignName( character, MALE_IDX );
	    OBJ_FEMALE:		newname := AssignName( character, FEMALE_IDX );
	    OBJ_LIZARD:		newname := AssignName( character, LIZARD_IDX );
	    OBJ_LIZARD_MACE:	newname := AssignName( character, LIZARD_IDX );
	    OBJ_LIZARD_SPEAR:	newname := AssignName( character, LIZARD_IDX );
	    OBJ_ORC:		newname := AssignName( character, ORC_IDX );
	    OBJ_ORC_CLUB:	newname := AssignName( character, ORC_IDX );
	    OBJ_ORC_CAPT:	newname := AssignName( character, ORC_IDX );
	    OBJ_OGRE:		newname := AssignName( character, OGRE_IDX );
	    OBJ_RATMAN:		newname := AssignName( character, RATMAN_IDX );
	    OBJ_RATMAN_SWORD:	newname := AssignName( character, RATMAN_IDX );
	    OBJ_RATMAN_AXE:	newname := AssignName( character, RATMAN_IDX );
	    default: 		exit;
	endcase
	return(newname);
endfunction

function AssignName( character, index )
    var cfg := ReadConfigFile( "names" );
    var elem := FindConfigElem( cfg, index );
    var number := GetConfigInt( elem, "Count" );
    var dice_str := "1d" + CStr(number);
    var choice := RandomDiceRoll( dice_str );
    var entry := "Name" + choice;
    var str := GetConfigString( elem, entry );
    var title := PreserveTitle( character );
    str := str + title;
    return( str );
endfunction

function PreserveTitle( character )
  var myname := character.name;
  var start := find( myname, ">", 1 );
  if( !start )
    return;
  endif
  var title := myname[ start+1, (len( myname ) - 8) ];
  if( !title )
    return("");
  endif
  return( title );
endfunction



