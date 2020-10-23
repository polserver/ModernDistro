use uo;
use util;
use math;
use basic;
use cfgfile;
use storage;

/**************************************************************************

	STANDARD POL INCLUDE FILE v0.4

	Last Update: 2001-03-02

	This file is a collection of the most used (and useful) functions
	available in the POL community. It's intended to be a replacement
	for those include files that everyone has but were never released
	to the public, providing a wide set of single functions.

	This file is supposed to be kept by the community but administrated
	by the one member. If you have a function to add to this file, or
	have a code	tweak or optimization, don't hesitate to send an email to:
	HellRaider <zion@uo.com.br>

***************************************************************************/

/*
	CreateStackArrayInContainer (container, objtype, amount, limit_per_stack := 60000)
		- Creates a dynamic number os stacks into the given container,
		based on the given amount, respecting a limit amount for each
		stack.
		Returns: array of ObjRefs to each created stack.
		Examples:
		CreateStackArrayInContainer(container, UOBJ_GOLDCOIN, 90000)
		- Creates 2 piles of gold into the container, the first
		with 60k coins, and the second with 30k coins.
		CreateStackArrayInContainer(container, UOBJ_GOLDCOIN, 90000, 10000)
		- Creates 9 piles of 10k gold into the container.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-19
*/
const STD_DAY_LENGTH := 7200;	// A whole UO day in seconds (7200 is equivalent to 2 hours)
const STD_GOLD := 0x0EED;		// ObjType for gold coins.

function CreateStackArrayInContainer (container, objtype, amount, limit_per_stack := 60000)
	var result array;
	for loops := 1 to (amount/limit_per_stack)
		result.append(CreateItemInContainer(container, objtype, limit_per_stack));
	endfor
	result.append(CreateItemInContainer(container, objtype, amount-(amount/limit_per_stack)*limit_per_stack));
	return result;
endfunction

/*
	GetFormattedTime()
		Returns: string containing the current in-game time in
		military/international format "HH:MM".
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetFormattedTime()
    var time := GetTimeOfDay();
    var raw_time := 24.0/(CDbl(STD_DAY_LENGTH)/time);
    var hours := CInt(raw_time);
    var minutes := CInt((raw_time-hours)*60);
    if (minutes < 10)
		minutes := "0"+minutes;
    endif
    if (hours < 10)
		hours := "0"+hours;
    endif
    return (hours+":"+minutes);
endfunction

/*
	GetGoldInContainer(container)
		- Loops through the given container looking for gold stacks.
		Returns: an array where the first element is the sum of gold
		within the container, and the following elements are ObjRefs
		to the gold piles found.
		Example: if used at a container with 2 piles of 10k gold, the
		function would return {20000, 1st_pile, 2nd_pile}.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-18
*/
function GetGoldInContainer (container)
	var result := {0};
	foreach item in EnumerateItemsInContainer(container)
		if (item.objtype == STD_GOLD)
			result.append(item);
			result[1] := result[1]+item.amount;
		endif
	endforeach
	return result;
endfunction

/*
	GetSpelledTime()
		Returns: string containing the spelled in-game time, just like
		in OSI Shards.
		Examples:
		"half past two in the morning."
		"eight o'clock at night."
		"midnight."
	HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetSpelledTime()
	var time := GetTimeOfDay();
    var raw_time := 24.0/(CDbl(STD_DAY_LENGTH)/time);
    var hours := CInt(raw_time);
    var minutes := CInt((raw_time-hours)*60);
	var ext;
	if (hours<11)
		ext := " in the morning.";
	elseif (hours>11 && hours<19)
		ext := " in the afternoon.";
	else
		ext := " at night.";
	endif
	if (hours>12)
		hours := hours-12;
	endif
	if (minutes<15)
		if (!hours || hours == 12)
			return SpellHour(hours);
		else
			return SpellHour(hours)+" o'clock"+ext;
		endif
	elseif (minutes > 14 && minutes < 30)
		if (!hours || hours == 12)
			return "quarter past "+SpellHour(hours);
		else
			return "quarter past "+SpellHour(hours)+ext;
		endif
	elseif (minutes > 29 && minutes < 45)
		if (!hours || hours == 12)
			return "half past "+SpellHour(hours);
		else
			return "half past "+SpellHour(hours)+ext;
		endif
	else
		if (!hours || hours == 12)
			return "quarter to "+SpellHour(hours+1);
		else
			return "quarter to "+SpellHour(hours+1)+ext;
		endif
	endif
endfunction

// HellRaider <zion@uo.com.br> 2000-12-13
function SpellHour (hours)
	case (hours)
		0:	return "midnight.";
		1:	return "one";
		2:	return "two";
		3:	return "three";
		4:	return "four";
		5:	return "five";
		6:	return "six";
		7:	return "seven";
		8:	return "eight";
		9:	return "nine";
		10:	return "ten";
		11:	return "eleven";
		12:	return "midday.";
	endcase
endfunction

/*
	GetStacksOfTypeInContainer (container, objtype)
		- Loops through the given container looking for item stacks from the
		given objtype.
		Returns: an array where the first element is the sum of items from the
		given objtype within the container, and the following elements are
		ObjRefs to the item stacks found.
		Example: if used at a container with 2 stacks of 50 cloth, the function
		would return {100, 1st_stack_of_cloth, 2nd_stack_of_cloth}.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-18
*/
function GetStacksOfTypeInContainer (container, objtype)
	var result := {0};
	foreach item in EnumerateItemsInContainer(container)
		if (item.objtype == objtype)
			result.append(item);
			result[1] := result[1]+item.amount;
		endif
	endforeach
	return result;
endfunction

/*
	GetStorageArea (areaname)
		- Finds a storage area by name. If no matching storage area is found,
		this function creates a new storage area with the given name.
		Returns: reference to the storage area found or created.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetStorageArea (areaname)
	var area := FindStorageArea(areaname);
	if (!area)
		return CreateStorageArea(areaname);
	endif
	return area;
endfunction

/*
	GetStorageRoot (storage, rootname, objtype := 0x0E7C)
		- Finds a container with the given name in the storage area.
		If no matching root container is found, this function creates one
		using the given objtype (default is a metalbox, like a bank box).
		Returns: reference to the root container found or created.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetStorageRoot (storage, rootname, objtype := 0x0E7C)
	var item := FindRootItemInStorageArea(storage, rootname);
	if (!item)
		return CreateRootItemInStorageArea(storage, rootname, objtype);
    endif
	return item;
endfunction

/*
	GetStrAfter (needle, byref haystack)
		- This function finds the first occurrence of needle within haystack
		then returns the remaining characters in the string.
		Returns: all characters found after needle in haystack.
			GetStrAfter("this ", "this rocks") returns "rocks";
			GetStrAfter("sky ", "the sky is blue") returns "is blue";
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetStrAfter (needle, byref haystack)
	return haystack[Find(haystack, needle, 1)+len(needle), len(haystack)];
endfunction

/*
	GetStrBefore (needle, byref haystack)
		- This function finds the first occurrence of needle within haystack
		then returns the preceding characters in the string.
		Returns: all characters preceding needle in haystack.
			GetStrBefore(" ", "pol rocks") returns "pol";
			GetStrBefore(" is ", "the sky is blue") returns "the sky";
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function GetStrBefore (needle, byref haystack)
	return haystack[1, Find(haystack, needle, 1)];
endfunction

/*
	GetTimeOfDay()
		Returns: elapsed seconds in the current UO day.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
//function GetTimeOfDay()
//    var time := ReadGameClock();
//	return (time-((time/STD_DAY_LENGTH)*STD_DAY_LENGTH));
//endfunction

/*
	ListObjectsOfClassNearLocation(x, y, z, range, POLCLASS, flags := LISTEX_FLAG_NORMAL)
		- Returns an array of itemrefs of the specified POLCLASS;
		- flags can be a set of LISTEX_* constants, and are only used
		with POLCLASS_UOBJECT, POLCLASS_MOBILE and POLCLASS_NPC.
		Returns: array of ObjRefs found.
		Notes: POLCLASS_UOBJECT will return both, mobiles and items.
		This function is limited and it'd be a good idea to have core support for it.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function ListObjectsOfClassNearLocation (x, y, z, range, POLCLASS, flags := LISTEX_FLAG_NORMAL, objrealm := _DEFAULT_REALM)
	case (POLCLASS)
		POLCLASS_UOBJECT:
			return ListObjectsInBox(x-range, y+range, z-10, x+range, y-range, z+10, objrealm);
		POLCLASS_MOBILE: //I'll separate it from POLCLASS_NPC for performance purposes.
			return ListMobilesNearLocationEx(x, y, z, range, flags, objrealm);
		POLCLASS_NPC:
			var result array;
			foreach mobile in ListMobilesNearLocationEx(x, y, z, range, flags, objrealm)
				if (mobile.isa(POLCLASS_NPC)) result.append(mobile); endif
			endforeach
			return result;
		default:
			var result array;
			foreach item in ListItemsNearLocation(x, y, z, range, objrealm)
				if (item.isa(POLCLASS)) result.append(item); endif
			endforeach
			return result;
	endcase
endfunction

/*
	MoveItemToForcedLocation(item, x, y, z)
		- Moves item ignoring it's movable state.
		Returns: nonzero if successful, error otherwise.
		This function forces the move in every aspect: using MOVEITEM_FORCELOCATION
		and setting the item movable state to 1 if it's zero. The movable state is
		set back to it's original state.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function MoveItemToForcedLocation (item, x, y, z)
	if (!item.movable)
		item.movable := 1;
		var result := MoveObjectToLocation(item, x, y, z, item.realm, MOVEOBJECT_FORCELOCATION);
		item.movable := 0;
		return result;
	endif
	return MoveObjectToLocation(item, x, y, z, item.realm, MOVEOBJECT_FORCELOCATION);
endfunction

/*
	MoveItem(item, byref place, flags := MOVEOBJECT_NORMAL)
		- Moves item either to a container or to a place depending of
		the parameter passed. Item will be moved to a container if the
		parameter is a ContainerRef, or a location if it's an array.
		- flags can be a set of MOVEITEM_* constants.
		- If an array of 2 integers is passed as the 'place' paramemter
		this function finds an appropriated 3rd parameter to be the Z
		coordinate of the location; since this parameter is passed byref
		the original array will have the 3rd parameter set once the function
		returns.
		Possible usages:
		MoveItem(item, TargetCoordinates(player));
		MoveItem(item, {1234, 5678});
		MoveItem(item, {1234, 5678, 9});
		MoveItem(item, player.backpack);
		Returns: nonzero if successfull, error if the item couldn't be
		moved or if an invalid parameter was passed.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function MoveItem (item, byref place, flags := MOVEOBJECT_NORMAL)
	if (place.isa(POLCLASS_CONTAINER))
		return MoveItemToContainer(item, place);
	elseif (place == array)
		case (place.size())
			0:1:
				var result := error;
				result.+errortext := "Parameter place has less than 2 elements.";
				return result;
			2:
				place[3] := GetWorldHeight(place[1], place[2], item.realm);
				continue;
			default:
				return MoveObjectToLocation(item, place[1], place[2], place[3], flags);
		endcase
	endif
	var result := error;
	result.+errortext := "Invalid place: expected an ContainerRef or Array, got "+place+".";
	return result;
endfunction

/*
	RandomListInt (tableref)
		- Returns a random integer based on a special config file "table".
		The passed parameter must be a reference to a config file "table".
		The table is meant to be structured like this:

		List [table name]
		{
		Count 4
		Elem0		1	5
		Elem1		6	10
		Elem2		11	15
		Elem3		16
		}

		Count counts the number of "Elements" within the table. The elements
		are inserted starting by zero. Each element may hold either, a number
		or a range. In the previous example, the first 3 elements are ranges,
		and the fourth element is a unique number. This function will first
		choose a random element, and then check if it's a range or an unique
		number, if it's an unique number the function will return the number,
		otherwise the function will return a random number within the specified
		range. Note that number 16 has a 25% chance to be picked up, while
		each number (inclusive) from 1 to 15 has a 5% chance (75%/15).
		Possible usages:
		RandomListInt(ReadConfigFile("::colors")[ALL_COLORS]);
		RandomListInt(config[24]);
		Returns: random integer.
	Thanks to HellRaider <zion@uo.com.br> 2001-01-17
*/
function RandomListInt (tableref)
	var elem := SplitWords(GetConfigString(tableref, "Elem"+RandomInt(tableref.Count)));
	elem[1] := CInt(elem[1]);
	if (elem.Size() == 1)
		return elem[1];
	else
		return RandomInt(CInt(elem[2])-elem[1])+elem[1];
	endif
endfunction

/*
	Season (player, season_id)
	- Use this function to set one's client-side season.
	- Possible values for seasons:
		SEASON_SUMMER	: Looks like "old Britannia".
		SEASON_FALL		: What it suggests.
		SEASON_WINTER	: Ugly like a dog sucking mango.
		SEASON_FELUCCA	: Chaotic look!
		SEASON_SPRING	: Trammel, home of the care bears.
	- Note that this function doesn't check the validity of the passed
	season id, just because I don't want to sacrifice performance,
	please use the constant values above to avoid problems.
	Thanks to HellRaider <zion@uo.com.br> 2001-03-02
*/
const SEASON_SUMMER		:= 1;
const SEASON_FALL		:= 2;
const SEASON_WINTER		:= 3;
const SEASON_FELUCCA	:= 4;
const SEASON_SPRING		:= 5;
function Season (player, season_id)
	// Doesn't support cursor customization, yet.
	SendPacket(player, "BC0"+season_id+"01");
endfunction


/*
	SubtractFromStackArray (byref arrayofobjrefs, amount)
		- Loops through the array of objects until the given amount of
		items is subtracted, or the last item is destroyed. IE if 2 piles
		of 10k gold are passed as the first argument, and 15000 is passed
		as the second argument, the first pile will be destroyed and the
		second pile will have 5000 gold subtracted from it.
		- In case the array contains non-POL-items objects, those objects
		are ignored.
		Returns: amount of items successfully subtracted.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-18
*/
function SubtractFromStackArray (byref arrayofobjrefs, amount)
	var needed := amount;
	foreach item in arrayofobjrefs
		if (!item.isa(POLCLASS_ITEM)) continue; endif
		if (item.amount<needed)
			needed := needed-item.amount;
			SubtractAmount(item, item.amount);
		else
			SubtractAmount(item, needed);
			return amount;
		endif
	endforeach
	return (amount-needed);
endfunction

/*
	ToSextant(x, y)
		Returns: a formated string containing the sextant coordinates
		for the location; ie, 29o 52'N, 138o 57'W
	Note: For some (rare) coords this function returns the minutes with 1 tile off OSI's.
	Each cartesian coord still have an unique sextant coord though (shouldn't be a problem).
	OSI uses a special rounding function for it.
	Thanks to HellRaider <zion@uo.com.br> 2000-12-13
*/
function ToSextant(x, y)
	var latitude_direction, longitude_direction;
	var latitude_degrees, longitude_degrees;
	var latitude_minutes, longitude_minutes;

	latitude_degrees := (y-1624)*0.087890625;
	longitude_degrees := (x-1323)*0.0703125;

	if (latitude_degrees < 0)
		latitude_direction := "N";
	else
		latitude_direction := "S";
	endif

	if (longitude_degrees < 0)
		longitude_direction := "W";
	else
		longitude_direction := "E";
	endif

	latitude_degrees := Abs(latitude_degrees);
	longitude_degrees := Abs(longitude_degrees);

	//The next lines get the first 2 digits from the decimal part of the real variables
	//and multiply by 60, returning an integer.
	latitude_minutes := ((((latitude_degrees-Floor(latitude_degrees))*100)*60)/100);
	longitude_minutes := ((((longitude_degrees-Floor(longitude_degrees))*100)*60)/100);

	return(	Floor(latitude_degrees)+"o "+latitude_minutes+"'"+latitude_direction+", "+
			Floor(longitude_degrees)+"o "+longitude_minutes+"'"+longitude_direction);
endfunction

/***************************************************************************

	HISTORY OF CHANGES

	Version 0.1
		- Added the followind functions:
			GetFormattedTime();
			GetSpelledTime();
			GetStorageArea (byref areaname);
			GetStorageRoot (storage, byref rootname, objtype := 0x0E7C);
			GetStrAfter (byref needle, byref haystack);
			GetStrBefore (byref needle, byref haystack);
			GetTimeOfDay();
			ListObjectsOfClassNearLocation(x, y, z, range, POLCLASS, flags := LISTEX_FLAG_NORMAL);
			MoveItemToForcedLocation(item, x, y, z);
			MoveItem(item, byref place, flags := MOVEOBJECT_NORMAL);
			ToSextant(x, y);

	Version 0.2
		- Added function GetGoldInContainer(container).

	Version 0.3
		- Added History of Changes.
		- Added function CreateStackArrayInContainer (container, objtype, amount, limit_per_stack := 60000).
		- Added function GetStacksOfTypeInContainer (container, objtype).
		- Added function SubtractFromStackArray (byref arrayofobjrefs, amount).
		- Fixed a bug in GetGoldInContainer(container) that would generate a compilation error.
		- Fixed a bug in MoveItem (item, byref place, flags := MOVEOBJECT_NORMAL).
		- Small tweak on the design of GetGoldInContainer(container). Since POL arrays don't support
		elements at index zero, the function was changed so the first element (index one) is the sum
		of the amount of gold found within the container, and the following elements are objrefs to
		the piles of gold.

	Version 0.4
		- Added RandomListInt(tableref).
		- Added Season(player, season_id).

***************************************************************************/
