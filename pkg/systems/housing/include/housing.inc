///////////////////
//  tries to find a house sign that covers the given location
///////////////////

function FindHouseForLocation (x, y, map)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, map)
		case (item.objtype)
			0x160ff:
			0x0bd2:
			0x0bd0:
				if (IsLocationWithinHouseSign (item, x, y))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction




///////////////////
//  determines if the given sign covers the given X-Y location
///////////////////

function IsLocationWithinHouseSign (sign, x, y)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return 0;
	endif

	if ( (x >= homeinfo[2] and x <= homeinfo[4]) and
	     (y >= homeinfo[3] and y <= homeinfo[5]) )
		return 1;
	else
		return 0;
	endif
endfunction

////////////////////////
// Determines if character is in the owner, co-owner or friends lists of the house
///////////////////////
function IsInOCF(who, sign)

		var inlist := 0;
		var infoarr := GetObjProperty( sign, "homeinfo" );
		if (infoarr)
			if ((who.x >= infoarr[2]) && (who.x <= infoarr[4]) && (who.y >= infoarr[3]) && (who.y <= infoarr[5]))
				foreach player in (GetObjProperty(sign,"friends") )
					if (who.serial == player)
						inlist := 1;
					endif
				endforeach
				foreach player in (GetObjProperty(sign,"coowners") )
					if (who.serial == player)
						inlist := 1;
					endif
				endforeach
				if (who.acctname == (GetObjProperty(sign, "owneracct") ) )
					inlist := 1;
				endif
			endif
		endif
		return inlist;
endfunction

///////////////////////////////////////////
// CanBuildHere(who)
// Determines if this is a location where
// new housing can be constructed.
///////////////////////////////////////////

function CanBuildHere(who)

  var element, rect;
  var cfgfile := ReadConfigFile("::buildlocs");
  var entries := GetConfigStringKeys(cfgfile);
  foreach listing in entries
    element :=  cfgfile[listing];
    rect := element.rect;
    rect := SplitWords(rect);
    if((who.x >= CInt(rect[1])) && (who.x <= CInt(rect[3])) && (who.y >= CInt(rect[2])) && (who.y <= CInt(rect[4])))
      return 1;
      break;
    endif
  endforeach
  return 0;

endfunction 