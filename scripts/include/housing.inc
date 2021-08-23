
var footage, sign;

Const HOUSE_LOCKDOWN_ITEM		:= 1;
Const HOUSE_RELEASE_ITEM		:= 2;
Const HOUSE_SECURE_CONTAINER	:= 3;
Const HOUSE_RELEASE_CONTAINER	:= 4;
Const HOUSE_RAISE_ITEM			:= 5;
Const HOUSE_LOWER_ITEM			:= 6;
Const HOUSE_DISPLAY_ITEM		:= 7;

function GetHouseSign( who )

	foreach sign in ListObjectsInBox (who.x-32, who.y-32, -128, who.x+32, who.y+32, +127, who.realm)
		if(sign.objtype in {0xBD2, 0xBD0, 0x20BD1, 0x20BD2, 0x20BCF, 0x20BD0, 0x260FA})
			if(GetObjProperty(sign, "footage"))
				if(sign.IsObjectInsideHouse(who))
					return sign;
				endif
			endif
		endif
	endforeach
	return 0;
endfunction

///////////////////
// Tries to find a house sign that covers the given location
// Note: Makes NO check to determine ownership, co-ownership or fiend status
// of the house in question.
// Returns objref to the sign.
///////////////////
function FindHouseForLocation (x, y, z, map)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, map)
		case (item.objtype)
			0xBD0:
			0xBD2:
			0x20BD1:
			0x20BD2:
			0x20BCF:
			0x20BD0:
			0x260FA:
				if (item.IsObjectInsideHouse(struct{"x" := x, "y" := y, "z" := z, "realm" := item.realm}))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction

/////////////////////////////////////////////////////////////////////////////////////////
// checks whether or not the supplied "who" is on any of the house lists as specified.  //
// usage: statuscheck(who, 1) <- will tell you if "who" is a co-owner.                 //
/////////////////////////////////////////////////////////////////////////////////////////
function StatusCheck(byref who, type)

	var nfo := {};
	case(type)
		1: nfo := GetObjProperty(sign, "coowners");
		2: nfo := sign.GetFriendList();
		3: nfo := GetObjProperty(sign, "banlist");
	endcase
	if(who.serial in nfo)
		return 1;
	else
		return 0;
	endif
endfunction

///////////////////
//  releases a locked down or secured item
//  Required for the decorator command.
///////////////////
function ReleaseLockedDownItem(owner, item)
	var oldtype := GetObjProperty (item, "olditem");
	if (oldtype)
		if (!ReserveItem (item))
			return;
		endif
		var olditem := CreateItemAtLocation (item.x, item.y, item.z, oldtype, 1, item.realm);
		olditem.name := item.name;
		olditem.color := item.color;
		olditem.graphic := item.graphic;
		olditem.movable := 1;

		if (GetObjProperty(item, "LockID"))
			SetObjProperty(olditem, "LockID", GetObjProperty (item, "LockID") );
		endif

		PrinttextAbovePrivate (olditem, "(no longer locked down)", owner, 0x3, 0x21);
		DestroyItem (item);
	else
		item.movable := 1;
		PrinttextAbovePrivate (item, "(no longer locked down)", owner, 0x3, 0x21);
		ReleaseItem (item);

		if (item.decayat)
			item.decayat := ReadGameClock() + 600;
		endif
	endif
endfunction

///////////////////////////////////////////////////////
//
// Determines house value based on settings.cfg
//
///////////////////////////////////////////////////////
function DetermineHouseValue(price)
	var divisor := 1;
	var housevalue := 1;                                      // Defaults to 1. Makes it easy if HouseValuesDeterminedBy == "HousesOwned"
	price := CInt(price);
	if(settings.HouseValuesDeterminedBy == "HousesOwned")
		return housevalue;
	elseif(settings.HouseValuesDeterminedBy == "Price")
		divisor := Cint(settings.HouseValueDivisor);
		housevalue := CInt(price / divisor);
		if(housevalue < 1)
			housevalue := 1;
		endif
		return CInt(housevalue);
	endif
endfunction

// This will eventually return the HousePricePerSquare value in regions.cfg but for now it returns settings.DefaultPricePerSquare.
function GetCityPrice(house)

	var town := WhatCityAmIIn(house);
	var squareprice := CInt(GetHousePricePerSquare(town));
	if(!squareprice)
		squareprice := CInt(settings.DefaultPricePerSquare);
	endif
	return 50;

endfunction

function GetHousePricePerSquare(town)
	UnloadConfigFile("regions");
	var  reg_cfg := ReadConfigFile("::regions/regions");               // Reads regions.cfg

	var regions := {};                                    // Array of city names
	//var boundary := "";                                   // String containing the 4 coords defining a resgion.
	//var boundaries := {};                                 // Array with the boundary coords as each element.
	//var yes := 0;
	//var city := "";
	//var isCity := "";
	var region;
	var hpricepersq := 0;

	regions := GetConfigStringKeys( reg_cfg );          // Creates an array of city names

		foreach place in regions
			region := FindConfigElem( reg_cfg, place );
			if(Lower(place) == Lower(town))
				hpricepersq := region[place]."HousePricePerSquare";

				if(!hpricepersq)
					hpricepersq := CInt(settings.DefaultPricePerSquare);
				endif
				break;
			endif
		endforeach
		return CInt(hpricepersq);
endfunction

function GetMarketPrice()

// This will eventually return the current market value of a house based on availability but for now it returns CInt(settings.DefaultPricePerSquare).
//var town := WhatCityAmIIn(home);

	var sqprice := CInt(settings.DefaultPricePerSquare);
return sqprice;

endfunction

function GetHouseItems(footage, map)
	var itemlist := {};
	foreach thing in footage
		var holder := ListObjectsInBox(thing[1], thing[2], thing[3], thing[4], thing[5],thing[6], map);
		foreach obj in holder
			if(!obj.isa(POLCLASS_MOBILE))
				itemlist.append(obj);
			endif
		endforeach
	endforeach
  return itemlist;
endfunction

function GetHouseComponents()
	var components := GetObjProperty(sign, "components");
	if(components)
		return components;
	else
		return 0;
	endif
endfunction

function IsInHouse(trg)
	var hsign := GetHouseSign(trg);
	if(trg in GetHouseMobiles(hsign))
		return 1;
	else
		return 0;
	endif
endfunction

function GetHouseMobiles(hsign)
	var itemlist := {};
	// var hsign := GetHouseSign(mobile);
	// if(!hsign)
		// return itemlist;
	// endif
	footage := GetObjProperty(hsign, "footage");
	foreach thing in footage
		var holder := ListObjectsInBox(thing[1], thing[2], thing[3], thing[4], thing[5],thing[6]);
		foreach obj in holder
			if(obj.isa(POLCLASS_MOBILE))
				itemlist.append(obj);
			endif
		endforeach
	endforeach
	return itemlist;
endfunction

///////////////////
//  Determines the base (lowest) Z level of the house zone the player is currently in.
//  Mainly useful for deed items but who knows, might be useful in other areas.
///////////////////
function GetHouseZoneBaseZLevel(targ, sign)
	// var homeinfo := GetObjProperty (sign, "homeinfo");
	var fotage := GetObjProperty (sign, "footage");
	if (!fotage)
		return;
	endif

	var zlevel := 0;
  	foreach thing in fotage
		if ( (targ.realm == sign.realm) && (targ.x >= thing[1] && targ.x <= thing[4]) && (targ.y >= thing[2] && targ.y <= thing[5]) &&
			(targ.z >= thing[3] && targ.z <= thing[6]) )
			//zlevel := CInt(thing[3]);/
			zlevel := GetStandingHeight( targ.x, targ.y, targ.z, targ.realm ).z;
		endif
		return zlevel;
	endforeach

endfunction

// Fixed for multis.
function SetLockdownInfo( sign, mobile, item, type := "lockeddown" )
	type := type;
	var home := SystemFindObjectBySerial(GetObjProperty(sign, "signserial"));
    if(GetObjProperty(home, "RemainingLockdowns") > 0)
      if (item.movable == 1)
        item.movable := 0;
        SetObjProperty(item, "lockeddown", 1);
        SetObjProperty(home, "RemainingLockdowns", GetObjProperty(home, "RemainingLockdowns") - 1);
        PrintTextAbove(item, "Locked Down.");
      else
        SendSysMessage(mobile, "That is not yours to lock down!", 2, 33);
      endif
    else
      SendSysMessage(mobile, "The house has no more lockdowns remaining.", 2, 33);
    endif
endfunction

