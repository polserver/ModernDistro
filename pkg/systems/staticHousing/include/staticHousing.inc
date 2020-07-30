//
// helper functions for static housing
//
//
// A lot of these are good candidates to be moved to the sign method script
//
use cfgfile;


///////////////////
// Tries to find a house sign that covers the given location
// Note: Makes NO check to determine ownership, co-ownership or fiend status
// of the house in question.
// Returns objref to the sign.
///////////////////
function FindStaticHouseForLocation (x, y, z, map)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, map)
		case (item.objtype)
			0x20BD1:
			0x20BD2:
			0x20BCF:
			0x20BD0:
			0x260FA:
				if (IsLocationWithinStaticHouseSign (item, x, y, z))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction

/////////////////////////////////////////////////////////////////////////////
// footage { nwx, nwy, lowerz, sex, sey, upperz }
///////////////////
///////////////////////////////////////////////////////////////////
// IsInside
// Tests to see if the item is inside and if not alerts 'owner'
//
///////////////////////////////////////////////////////////////////

function IsItemInsideHouse(byref owner, sign, item)
	sign := FindStaticHouseForLocation(item.x, item.y, item.z, item.realm);
	footage := GetObjProperty (sign, "footage");
	var chk := 0;
	foreach thing in footage
		if ( (item.realm == sign.realm) &&
			(item.x >= thing[1] && item.x <= thing[4]) &&
			(item.y >= thing[2] && item.y <= thing[5]) &&
			(item.z >= thing[3] && item.z <= thing[6]) )
			chk := 1;
			break;
		endif
		sleepms(5);
		if(chk)
			break;
		endif
		sleepms(5);
	endforeach
	if(chk == 0)
		SendSysMessage(owner, "That is not inside your home.", 0x3, 0x21);
	endif
	return chk;
endfunction



