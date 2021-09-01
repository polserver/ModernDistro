//include "include/wodinc";
include ":customhousing:housefriends";

Const HOUSE_LOCKDOWN_ITEM	:= 1;
Const HOUSE_RELEASE_ITEM		:= 2;
Const HOUSE_SECURE_CONTAINER	:= 3;
Const HOUSE_RELEASE_CONTAINER	:= 4;
Const HOUSE_RAISE_ITEM		:= 5;
Const HOUSE_LOWER_ITEM		:= 6;
Const HOUSE_DISPLAY_ITEM		:= 7;

Const HOUSE_VALUE_LIMIT		:= 6;

///////////////////
//  gets the housevalue of the given account
///////////////////

function GetAccountHouseValue (acctname)
	acctname := Lower (acctname);
	if (!acctname)
		return acctname.errortext := ("No accountname supplied");
	endif
	var acct := FindAccount (acctname);
	if (!acct)
		return acctname.errortext :=  ("Account " + acctname + " does not exist");
	endif

	var global_prop_name := "#housing_of_" + acctname;
	var existing_houses := GetGlobalProperty (global_prop_name);
	if (!existing_houses or !len (existing_houses))
		return 0;
	endif

	var hv_counter := 0;
	//make sure each sign exists and is still registered to the account
	foreach sign_serial in existing_houses
		var sign := SystemFindObjectBySerial (sign_serial);
		if (sign)
			var house_owner := Lower (GetObjProperty (sign, "account"));
			if (house_owner and house_owner == acctname)
				var sign_hv := GetHouseValueOfSign (sign);
				if (sign_hv)
					hv_counter := hv_counter + sign_hv;
				endif
			endif
		endif
	endforeach
	return hv_counter;
endfunction




///////////////////
//  adds the value of the house to the owner's housevalue
///////////////////

function RecordHouseSignOwner (sign)
	if (!sign)
		return sign.errortext := "Sign does not exist!";
	endif

	var owneraccount := Lower (GetObjProperty (sign, "account"));
	if (!owneraccount)
		var try_counter := 0;
		repeat
			sleep (1);
			try_counter := try_counter + 1;
			owneraccount := Lower (GetObjProperty (sign, "account"));
		until ((try_counter > 20) or owneraccount);
	endif

	set_critical (1);
	var global_prop_name := "";
	if (owneraccount)
		var acct := FindAccount (owneraccount);
		if (acct)
			global_prop_name := "#housing_of_" + owneraccount;
		else
			global_prop_name := "#housing_no_owner";
		endif
	else
		global_prop_name := "#housing_no_owner";
	endif
	
	var existing_houses := GetGlobalProperty (global_prop_name);
	if (!existing_houses)
		existing_houses := Array {};
	endif
	
	if (! (sign.serial in existing_houses))
		existing_houses.append (sign.serial);
		SetGlobalProperty (global_prop_name, existing_houses);
	endif

	set_critical (0);
	return 1;
endfunction




///////////////////
//  determines if adding the housevalue will put a character over their limit
///////////////////

function HouseValueUnderLimit (character, housevalue);
	var oldhousevalue := GetAccountHouseValue (character.acctname);
	var newhousevalue := oldhousevalue + housevalue;
	if (newhousevalue <= HOUSE_VALUE_LIMIT)
		return 1;
	else
		return 0;
	endif
endfunction




///////////////////
//  gets a default house price, based on its size
///////////////////

function GetDefaultHousePrice (housevalue)
	case (housevalue)
		2:
			return 90000;
		3:
			return 150000;
		4:
			return 400000;
		5:
			return 800000;
		default:
			return 40000;
	endcase
endfunction




///////////////////
//  determines the housevalue of a given deed
///////////////////

function GetHouseValueByDeed (deed)
	case (deed)
		0x16023: //tower
		0x16230: //customthreestoryhousedeed
			return 3;
		0x16024: //keep
		0x16025: //castle
			return 5;
		0x16026: //largepatiohousedeed
		0x16020: //largebrickhouse
		0x16021: //twostorywood
		0x16022: //twostorystone
		0x16229: //smalltowerdeed
		0x1622A: //sandstonepatiohousedeed
		0x1622B: //twostorylogcabindeed
		0x1622C: //twostoryvilladeed
		0x1622D: //largemarblepatiohousedeed
		0x1622E: //ranch house
		0x16231: //customtwostoryhousedeed
			return 2;
		default:
			return 1;
	endcase
endfunction




///////////////////
//  determines the housevalue of the given sign, whether its one of a static house or deedplaced
///////////////////

function GetHouseValueOfSign (sign)
	//static houses:
	if (sign.objtype == 0x17060)
		if (GetObjProperty (sign, "housevalue"))
			return CINT (GetObjProperty (sign, "housevalue"));
		endif
		return 1;
	endif

	if (GetObjProperty (sign, "housevalue"))
		return CINT (GetObjProperty (sign, "housevalue"));
	endif
	//deed-placed houses:
	var house := SystemFindObjectBySerial (GetObjProperty (sign, "house_serial"));
	var deed := GetDeedType (house);
	return GetHouseValueByDeed (deed);
endfunction




///////////////////
//  determines the deed objtype that corresponds to a given house type
///////////////////

function GetDeedType (house)
	var deed := 0;
	case (house.objtype)
		69732:	deed := 0x16019;
		69734:	deed := 0x1601A;
		69738:	deed := 0x1601B;
		69740:	deed := 0x1601C;
		69742:	deed := 0x1601D;
		69748:	deed := 0x16020;
		69750:	deed := 0x16021;
		69752:	deed := 0x16022;
		69754:	deed := 0x16023;
		69756:	deed := 0x16024;
		69758:	deed := 0x16025;
		69772:	deed := 0x16026;
		69792:	deed := 0x16227;
		69794:	deed := 0x16228;
		69784:	deed := 0x16229;
		69788:	deed := 0x1622A;
		69786:	deed := 0x1622B;
		69790:	deed := 0x1622C;
		69782:	deed := 0x1622D;
		69773:	deed := 0x1622E;
		69736:	deed := 0x1622F;
	endcase
	if (house.objtype >= 0x113ec and house.objtype <= 0x1140a)
		deed := 0x16231;
	elseif (house.objtype >= 0x11410 and house.objtype <= 0x11416)
		deed := 0x16231;
	elseif (house.objtype >= 0x1141c and house.objtype <= 0x11422)
		deed := 0x16231;
	elseif (house.objtype >= 0x11428 and house.objtype <= 0x1142e)
		deed := 0x16231;
	elseif (house.objtype >= 0x11435 and house.objtype <= 0x1143a)
		deed := 0x16231;
	elseif (house.objtype >= 0x1140b and house.objtype <= 0x1147b)
		deed := 0x16230;
	endif
	return deed;

endfunction




///////////////////
//  Determines the maximum number of lockdowns permitted in a house
///////////////////

function GetMaxLockdowns (sign)
	var hvalue := GetHouseValueOfSign (sign);
	return (hvalue * 750);
endfunction




///////////////////
//  returns the number of items locked down in the house
///////////////////

function GetLockdowns (sign)
	var itemlist := dictionary;
	foreach item in ListObjectsInBox (sign.x-32, sign.y-32, -128, sign.x+32, sign.y+32, +127, sign.realm)
		if (!item.movable)
			if ( GetObjProperty( item, "lockeddown" ) == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
			if ( GetObjProperty( item, "gmlockeddown" ) == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
		endif
	endforeach

	Setobjproperty (sign, "lockdowns", itemlist.size() );
	return itemlist.size();
endfunction




///////////////////
//  determines the maximum allowed number of secures for the house
///////////////////

function GetMaxSecures( sign )
	var hvalue := GetHouseValueOfSign (sign);
	return (hvalue * 10);
endfunction
  

///////////////////
//  counts the number of secured items in the house
///////////////////

function GetSecures (sign)
	var itemlist := dictionary;
	foreach item in ListObjectsInBox (sign.x-32, sign.y-32, -128, sign.x+32, sign.y+32, +127, sign.realm)
		if (!item.movable)
			if ( GetObjProperty (item, "secured") == sign.serial )
				if (!itemlist.exists(item) )
					itemlist[item] := item;
				endif
			endif
		endif
	endforeach

	Setobjproperty (sign, "secures", itemlist.size() );
	return itemlist.size();
endfunction




///////////////////
//  refreshes the house
///////////////////

function RefreshHouse (sign)
	var lastvisit := GetObjProperty (sign, "lastvisit");
	if (!lastvisit)
		SetObjProperty (sign, "lastvisit", ReadGameClock() );
	elseif ( lastvisit < ReadGameClock() )
		SetObjProperty (sign, "lastvisit", ReadGameClock() );
	endif
endfunction




///////////////////
//  refreshes the house for a longer period of time
///////////////////

function LongTermRefresh (sign, owner)
	var onemonth := 2592000 + ReadGameClock();
	SetObjProperty (sign, "lastvisit" , onemonth);
	SendSysMessage (owner, "Your house has been refreshed for one month.");
endfunction




///////////////////
//  releases all locked down and secured items in the house.  Used when the house is destroyed
///////////////////

function ReleaseAll (owner, sign)
	var homeinfo := GetObjProperty (sign, "homeinfo");
//	foreach item in ListObjectsInBox (sign.x-32, sign.y-32, -128, sign.x+32, sign.y+32, +127, sign.realm)
	foreach item in ListObjectsInBox (homeinfo[2], homeinfo[3], -128, homeinfo[4], homeinfo[5], +127, sign.realm)
		if (!item.movable and item.isa (POLCLASS_ITEM))
//			if ( (item.x >= homeinfo[2] and item.x <= homeinfo[4]) and
//					(item.y >= homeinfo[3] and item.y <= homeinfo[5]) )
			if (!IsGMLockedDownItem (owner, item))
				var parms := array{};
				parms[1] := sign;
				parms[2] := owner;
				parms[3] := item;
				parms[4] := 0;
				if (GetObjProperty (item, "displaycontainer") == sign.serial)
					parms[4] := HOUSE_DISPLAY_ITEM;
					run_script_to_completion (":housing:lockunlock", parms);
				elseif (GetObjProperty (item, "lockeddown") == sign.serial)
					parms[4] := HOUSE_LOCKDOWN_ITEM;
					run_script_to_completion (":housing:lockunlock", parms);
				elseif (GetObjProperty (item, "secured") == sign.serial)
					parms[4] := HOUSE_SECURE_CONTAINER;
					run_script_to_completion (":housing:lockunlock", parms);
				endif
			endif
		endif
	endforeach

	EraseObjProperty (sign, "lockdowns");
	EraseObjProperty (sign, "secures" );
endfunction




///////////////////
//  releases a locked down or secured item
///////////////////

function ReleaseLockedDownItem (owner, item)
	if (GetObjProperty (item, "olditem"))
		if (!ReserveItem (item))
			return;
		endif
		var oldtype := GetObjProperty (item, "olditem");
		var olditem := CreateItemAtLocation (item.x, item.y, item.z, oldtype, 1, item.realm);
		olditem.name := item.name;
		olditem.color := item.color;
		olditem.graphic := item.graphic;
		olditem.movable := 1;
		if (getobjproperty (item, "lockid"))
			setobjproperty(olditem, "lockid", getobjproperty (item, "lockid") );
		endif
		PrinttextAbovePrivate (olditem, "Released!", owner);
		DestroyItem (item);
	else
		item.movable := 1;
		PrinttextAbovePrivate (item, "Released!", owner );
		ReleaseItem (item);
		if (item.decayat)
			item.decayat := ReadGameClock() + 600;
		endif
	endif
endfunction




///////////////////
//  allows the owner to pick a new owner for the house
///////////////////

function ChangeOwner (who, sign)
	SendSysMessage (who, "Transfer ownership to whom?");
	var newowner := Target (who);
	if (!newowner or !newowner.acctname)
		SendSysMessage (who, "Canceled.");
		return;
	endif

	if (!HouseValueUnderLimit (newowner, GetHouseValueOfSign (sign)))
		SendSysMessage (who, "That person owns too many houses!");
		return;
	endif

	EraseObjProperty (sign, "housefriends");
	SetObjProperty (sign, "account", newowner.acctname);

	if (GetObjProperty (sign, "homeinfo"))
		var homeinfo := GetObjProperty (sign, "homeinfo");
		homeinfo[1] := newowner.serial;
		SetObjProperty (sign, "homeinfo", homeinfo);
	else
		var house := SystemFindObjectBySerial (GetObjProperty (sign, "house_serial"));
		SetObjProperty (house, "ownerserial", newowner.serial);
	endif

	SetObjProperty (sign, "lastownername", newowner.name);

	SendSysMessage (who, "You are no longer the owner of this house!");
	SendSysMessage (newowner, "You are now the owner of this house!");
	SendSysMessage (newowner, "You should change the locks soon.");
endfunction




///////////////////
//  Finds any GM lockeddown items in the house and refunds 1/2 the purchase price to the houseowner
///////////////////

function IsGMLockedDownItem (owner, item)
	if (!GetObjProperty (item, "gmlockeddown"))
		return 0;
	endif
	
	var price := GetObjProperty (item, "price");
	if (!price)
		DestroyItem (item);
		return 1;
	endif
	
	price := CINT (price/2);

	CreateItemInContainer (owner.backpack, UOBJ_GOLD_COIN, price);
	SendSysMessage (owner, "You recieve " + price + " gold back for " + item.desc);
	DestroyItem (item);
	return 1;
endfunction




///////////////////
//  handles speech events that the sign hears
///////////////////

function handle_speech (event, sign)
	RefreshHouse (sign);

	var text := lower( event.text );
	if (text["i wish to lock this down"] or text["i wish to release this"])
		set_critical (1);
		LockdownItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to secure this"] or text["i wish to unsecure this"])
		set_critical (1);
		SecureItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to display this"] or text["i wish to remove this from display"])
		set_critical (1);
		DisplayContainer (sign, event.source);
		set_critical (0);
	elseif (text["i wish to raise this"])
		set_critical (1);
		RaiseItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to lower this"])
		set_critical (1);
		LowerItem (sign, event.source);
		set_critical (0);
	elseif (text["i wish to refresh this house"])
		LongTermRefresh (sign, event.source);
	endif

endfunction




///////////////////
//  allows the character to lock down items
///////////////////

function LockdownItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_LOCKDOWN_ITEM;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
// allows the player to secure containers
///////////////////

function SecureItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_SECURE_CONTAINER;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
//  allows the house owner to raise the z position of items in the house
///////////////////

function RaiseItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_RAISE_ITEM;
	start_script (":housing:lockunlock", parms);
endfunction




///////////////////
//  allows the house owner to lower the z position of items in the house
///////////////////

function LowerItem (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_LOWER_ITEM;
	start_script (":housing:lockunlock", parms);

endfunction




///////////////////
//  Puts a container on display
///////////////////

function DisplayContainer (sign, owner)
	var parms := array{};
	parms[1] := sign;
	parms[2] := owner;
	parms[3] := 0;
	parms[4] := HOUSE_DISPLAY_ITEM;
	start_script (":housing:lockunlock", parms);

endfunction



function IsInside (owner, sign, item)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if ( (item.realm == sign.realm) and
	     (item.x >= homeinfo[2] and item.x <= homeinfo[4]) and
	     (item.y >= homeinfo[3] and item.y <= homeinfo[5]) )
		return 1;
	else
		SendSysmessage (owner, "That is not inside your house." );
		return 0;
	endif
endfunction




///////////////////
//  tries to find a house sign near the character that belongs to that character
///////////////////

function FindHouseSign (character)
	foreach item in ListObjectsInBox (character.x-32, character.y-32, -128, character.x+32, character.y+32, +127, character.realm)
		case (item.objtype)
			0x17060:
			0x0bd2:
			0x0bd0:
				if (IsAFriend (item, character) or character.cmdlevel)
					if (IsInsideTheHouse (character, item) )
						return item;
					endif
				endif
		endcase
	endforeach
	return 0;
endfunction




///////////////////
//  determines if the character is within the area the house covers
///////////////////

function IsInsideTheHouse (character, sign)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return;
	endif

	if ( (character.realm == sign.realm) and
		 (character.x >= homeinfo[2] and character.x <= homeinfo[4]) and
	     (character.y >= homeinfo[3] and character.y <= homeinfo[5]) )
		return 1;
	else
		return 0;
	endif
endfunction




///////////////////
//  tries to find a house sign that covers the given location
///////////////////

function FindHouseSignAtLocation (x, y, realm)
	foreach item in ListObjectsInBox (x-32, y-32, -128, x+32, y+32, +127, realm)
		case (item.objtype)
			0x17060:
			0x0bd2:
			0x0bd0:
				if (IsLocationWithinHouseSign (item, x, y, realm))
					return item;
				endif
		endcase
	endforeach
	return 0;
endfunction




///////////////////
//  determines if the given sign covers the given X-Y location
///////////////////

function IsLocationWithinHouseSign (sign, x, y, rlm)
	var homeinfo := GetObjProperty (sign, "homeinfo");
	if (!homeinfo)
		return 0;
	endif

	if ( (x >= homeinfo[2] and x <= homeinfo[4]) and
	     (y >= homeinfo[3] and y <= homeinfo[5]) and
	     sign.realm == rlm )
		return 1;
	else
		return 0;
	endif
endfunction
