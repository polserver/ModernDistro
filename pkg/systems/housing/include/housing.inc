//	homeinfo[1] = Owner Character Serial
//	homeinfo[2] = NW X coord
//	homeinfo[3] = NW Y coord
//	homeinfo[4] = SE X coord
//	homeinfo[5] = SE Y coord
//	homeinfo[6] = NW Z coord
//	homeinfo[7] = SE Z coord
//	homeinfo[8] = num lockdowns
//	homeinfo[9]	= num secures


///////////////////
//  tries to find a house sign near the character that belongs to that character
///////////////////
function FindHouseSign (who)
	foreach item in ListObjectsInBox (who.x-100, who.y-100, -128, who.x+100, who.y+100, +127, who.realm)
		case (item.objtype)
			0x160ff:
				if (item.IsAffiliated( who ))
					if (IsInsideTheHouse (who, item) )
						return item;
					endif
				endif
		endcase
	endforeach
	return 0;
endfunction

///////////////////
//  tries to find a house sign that covers the given location
///////////////////

function FindHouseForLocation (x, y, z, map)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, map)
		case (item.objtype)
			0x160ff:
			0x0bd2:
			0x0bd0:
				if (IsLocationWithinHouseSign (item, x, y, z))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction

//////////////////////////////////////////////
// Checks to see if the character is in the house
// covered by the sign
/////////////////////////////////////////////

function IsInsideTheHouse(char, sgn)

  return IsLocationWithinHouseSign (sgn, char.x, char.y, char.z);
  
endfunction

///////////////////
//  determines if the given sign covers the given X-Y location
///////////////////

function IsLocationWithinHouseSign (sign, locx, locy, locz)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return 0;
	endif

	if ( (locx >= homeinfo[2] && locx <= homeinfo[4]) &&
	     (locy >= homeinfo[3] && locy <= homeinfo[5])  &&
       (locz >= homeinfo[6] && locz <= homeinfo[7]))
		return 1;
	else
		return 0;
	endif
endfunction

///////////////////
//  Determines the base (lowest) Z level of the house zone the player is currently in.
//  Mainly useful for deed items but who knows, might be useful in other areas.
///////////////////
function GetHouseZoneBaseZLevel(targ, sign)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return;
	endif

  var zlevel := 0;
  var chk := 0;
      if ( (targ.realm == sign.realm) && (targ.x >= homeinfo[1] && targ.x <= homeinfo[4]) && (targ.y >= homeinfo[2] && targ.y <= homeinfo[5]) &&
       (targ.z >= homeinfo[3] && targ.z <= homeinfo[6]) )
        zlevel := CInt(homeinfo[3]);GetStandingHeight( targ.x, targ.y, targ.z - 1, realm := _DEFAULT_REALM );
      endif
  return zlevel;
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

////////////////////////
// Determines if character is in the owner, co-owner of the house
///////////////////////
function IsInOC(who, sign)

		var inlist := 0;
		var infoarr := GetObjProperty( sign, "homeinfo" );
		if (infoarr)
			if ((who.x >= infoarr[2]) && (who.x <= infoarr[4]) && (who.y >= infoarr[3]) && (who.y <= infoarr[5]))
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

function SetLockdownInfo( sign, mobile, item, type := "lockeddown" )
    if(GetObjProperty(sign, "numlockdowns") > 0)
      if (item.movable == 1)
        item.movable := 0;
        SetObjProperty(item, "lockeddown", 1);
        SetObjProperty(sign, "numlockdowns", GetObjProperty(sign, "numlockdowns") - 1);
        PrintTextAbove(item, "Locked Down.");
      else
        SendSysMessage(mobile, "That is not yours to lock down!", 2, 33);
      endif
    else
      SendSysMessage(mobile, "The house has no more lockdowns remaining.", 2, 33);
    endif
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