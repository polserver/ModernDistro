/* $Id: uo_extend.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 *
 */

use uo;
use os;
use util;
use polsys;

// For: EnumerateOnlineCharactersABC()
//
CONST SORT_ASCENDING  := 0x0; // Default
CONST SORT_DESCENDING := 0x1;

// For: GetIsAType()
CONST ISA_GET_MAIN	:= 0x0; // Default
CONST ISA_GET_ALL	:= 0x1;

/*
 * EnumerateOnlineCharactersABC(dir)
 *
 * Purpose
 * Builds a list of online characters in ABC order.
 *
 * Parameters
 * dir:	Determines if the list should be made ascending or descending.
 *
 * Return Value:
 * Returns an array of mobile references.
 *
 */
function EnumerateOnlineCharactersABC(dir:=SORT_ASCENDING)
	var dict := dictionary;
	var z_list := EnumerateOnlineCharacters();
	foreach player in z_list
		var key := Upper(player.name);
		if ( !dict.Exists(key) )
			dict[key] := array{};
		endif
		dict[key].Append(player);
		SleepMS(2);
	endforeach

	z_list := array{};
	foreach list in ( dict )
		foreach player in ( list )
			if ( dir == SORT_ASCENDING )
				z_list.Append(player);
			else
				z_list.Insert(1, player);
			endif
			SleepMS(2);
		endforeach
		SleepMS(2);
	endforeach

	return z_list;
endfunction

/*
 * EnumeratePhysicalItems(mobile)
 *
 * Purpose
 * Builds a list of equipped items on a mobile that are safe to access.
 *
 * Parameters
 * mobile:	Mobile to get the equipment on.
 *
 * Return Value:
 * Returns an array of item references.
 *
 */
function EnumeratePhysicalItems(mobile)
	var equip_list := array{};

	// Hair, beard, backpack, mount
	var skip_layers := array{11, 16, 21, 25};
	var i;

	for ( i:=0; i<=25; i:=i+1 )
		if ( i in skip_layers ) 
			continue;
		else
			var item := GetEquipmentByLayer(mobile, i);
			if ( item )
				equip_list.Append(item);
			endif
		endif
		SleepMS(2);
	endfor

	return equip_list;
endfunction

/*
 * GetIsAType(object, flags)
 *
 * Purpose
 * Retrieves the 'IsA' type for an object or all of the
 * classes it belongs to.
 *
 * Parameters:
 * object:	Anything belonging to the UObject class heirarchy.
 * flags:	Flags...
 *
 * Return value:
 * Returns an error on failure.
 * Returns an integer if ISA_GET_MAIN is passed for flags.
 * Returns an array of integers if ISA_GET_ALL is used.
 *
 */
function GetIsAType(object, flags:=ISA_GET_MAIN)
	var type_list := array;
	var i := 16; // Max number of IsA Id #s
	for ( i; i > 0; i := i - 1 )
		if ( !object.IsA(i) )
			continue;
		elseif ( flags & ISA_GET_ALL )
			type_list.Append(i);
		else
			return i;
		endif
		SleepMS(2);
	endfor

	if ( type_list.Size() > 0 )
		return type_list;
	else
		return error{"errortext":="Unable to determine IsA type of '"+TypeOf(object)+"'"};
	endif
endfunction

/*
 * GetIsATypeName(isa_id)
 *
 * Purpose
 * Retrieves the name for an IsA id number.
 *
 * Parameters:
 * isa_id:	Any number that matches an IsA number.
 *
 * Return value:
 * Returns an error on failure.
 * Returns a string on success.
 *
 */
function GetIsAName(isa_id)
	isa_id := CInt(isa_id);
	var type := "POLCLASS_";
	case ( isa_id )
		1: return type+"UOBJECT"; break;
		2: return type+"ITEM"; break;
		3: return type+"MOBILE"; break;
		4: return type+"NPC"; break;
		5: return type+"LOCKABLE"; break;
		6: return type+"CONTAINER"; break;
		7: return type+"CORPSE"; break;
		8: return type+"DOOR"; break;
		9: return type+"SPELLBOOK"; break;
		10: return type+"MAP"; break;
		11: return type+"MULTI"; break;
		12: return type+"BOAT"; break;
		13: return type+"HOUSE"; break;
		14: return type+"EQUIPMENT"; break;
		15: return type+"ARMOR"; break;
		16: return type+"WEAPON"; break;
		default: return error{"errortext":="'"+isa_id+"' is not a valid IsA number."};
	endcase
endfunction

/*
 * Just a development function to test lists..
 * Dont get many people online on my computer, but wanna make stuff work
 * on other shards!
 *
 */
function BuildFakePlayerList(amount:=0)
	var list := array{};
	var player := struct{"name", "serial", "x", "y", "z", "realm"};
	var realm_list := Realms();
	var realm_names := realm_list.Keys();
	
	for ( amount; amount>0; amount:=amount-1)
		player.name := "FakePlayer-"+amount;
		player.serial := amount;
		player.realm := realm_names[RandomInt(realm_names.Size())];
		player.x := RandomInt(realm_list[player.realm].width)+1;
		player.y := RandomInt(realm_list[player.realm].height)+1;
		player.z := CInt(GetMapInfo(player.x, player.y, player.realm).z);
		
		list.Append(player);
		SleepMS(2);
	endfor
	
	return list;
endfunction
	