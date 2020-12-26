/*
Recursive equipment layer finder thingy
by Austin Heilman aheilman@home.com
written on 5-12-01
*/

use cfgfile;
use uo;
use util;

function destroyequip(mobile)
	//First, get a random item and make sure there is something there!
	if (isnude(mobile))
		return 0; // So if nothing returned, the character is naked.
	endif

	var cfg := ReadConfigFile(":cleric:equiplayers");
	var slot1;

	do
		slot1 := RandomDiceRoll("1d24");
	dowhile(!cfg[slot1] || !GetEquipmentByLayer(mobile, slot1));

	//Now use recursion to find the item covering it.
	//Return the item reference to the script that wants it.
	return GetEquipmentByLayer(mobile, findtopitem(mobile, slot1));
endfunction

function findtopitem(mobile, slot)
	var config := ReadConfigFile(":cleric:equiplayers");
	foreach protector in GetConfigStringArray(config[slot], "coveredby");
		if (GetEquipmentByLayer(mobile, CInt(protector)))
			return findtopitem(mobile, CInt(protector));
		endif
	endforeach
	return slot;
endfunction

function isnude(mobile)
	var config := ReadConfigFile(":cleric:equiplayers");
	var i;
	for (i:=1;i<=25;i:=i+1)
		if (config[i] && GetEquipmentByLayer(mobile, i))
			return 0;
		endif
	endfor
	return 1;
endfunction