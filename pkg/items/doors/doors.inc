use os;
use uo;
use util;
use cfgfile;

include "include/client";
include "include/objtype";
include "include/NPCBackpacks";

var doordesc := ReadConfigFile (":doors:itemdesc");

///////////////////
//  opens the given door, ignoring locked status
///////////////////

function OpenDoor (door)
	var xmod := CInt(doordesc[door.objtype].xmod);
	var ymod := CInt(doordesc[door.objtype].ymod);

	var doorx := door.x;
	var doory := door.y;
	if ( GetObjProperty (door, "x") )
		doorx := GetObjProperty (door, "x");
		doory := GetObjProperty (door, "y");
	elseif (door.graphic == door.objtype)
		SetObjProperty (door, "x", door.x);
		SetObjProperty (door, "y", door.y);
	endif

	var newx := doorx + xmod;
	var newy := doory + ymod;
	var newz := door.z;

	var opengraphic := GetObjProperty (door, "opengraphic");
	if (!opengraphic or opengraphic == "")
		opengraphic := door.graphic + 1;
	endif

	if (door.graphic == door.objtype)
		set_critical (1);
		door.movable := 1;
		door.graphic := opengraphic;
		MoveObjectToLocation (door, newx, newy, newz, door.realm, MOVEOBJECT_FORCELOCATION);    
		door.movable := 0;
		set_critical (0);
		PlayDoorOpenSound (door);
	endif
endfunction




///////////////////
//  closes the given door, ignoring locked status
///////////////////

function CloseDoor (door)
	if (door.graphic == door.objtype)
		return;
	endif

	var xmod := CINT (doordesc[door.objtype].xmod);
	var ymod := CINT (doordesc[door.objtype].ymod);

	var newx := door.x - xmod;
	var newy := door.y - ymod;
	if (GetObjProperty (door, "x"))
		newx := GetObjProperty (door, "x");
		newy := GetObjProperty (door, "y");
		EraseObjProperty (door, "x");
		EraseObjProperty (door, "y");
	endif

	var newz := door.z;
	if (door.graphic != door.objtype)
		set_critical (1);
		door.movable := 1;
		door.graphic := door.objtype;
		MoveObjectToLocation (door, newx, newy, newz, door.realm, MOVEOBJECT_FORCELOCATION);
		door.movable := 0;
		set_critical (0);
		PlayDoorCloseSound (door);
	endif
endfunction




///////////////////
// plays the sound effect for opening the door
///////////////////

function PlayDoorOpenSound (door)
	case (doordesc[door.objtype].doortype)
		"wood":
			PlaySoundEffect (door, SFX_OPEN_WOODEN_DOOR);
		"metal": 
			PlaySoundEffect (door, SFX_OPEN_METAL_DOOR);
		default:
			PlaySoundEffect (door, SFX_OPEN_METAL_DOOR);
	endcase
endfunction




///////////////////
// plays the sound effect for closing the door
///////////////////

function PlayDoorCloseSound (door)
	case (doordesc[door.objtype].doortype)
		"wood":
			PlaySoundEffect (door, SFX_CLOSE_WOODEN_DOOR);
		"metal": 
			PlaySoundEffect (door, SFX_CLOSE_METAL_DOOR);
		default:
			PlaySoundEffect (door, SFX_CLOSE_METAL_DOOR);
	endcase
endfunction




///////////////////
//  checks to see if the door is locked and, if it is, tries to find a key to open it in the
//  character's backpack
///////////////////

function IsLocked (character, door)
	if (!door.locked)
		return 0;
	endif

	var doorlockid := GetObjProperty (door, "lockid");
	if (!doorlockid)
		return 1;
	endif

	//look for keys
	foreach item in EnumerateItemsInContainer (character.backpack)
		case (item.objtype)
			UOBJ_COPPER_KEY:
			UOBJ_GOLD_KEY:
			UOBJ_IRON_KEY:
			UOBJ_MAGIC_KEY:
			UOBJ_RUSTY_IRON_KEY:
				var keylockid := GetObjProperty (item, "lockid");
				if (keylockid and keylockid == doorlockid)
					PrintTextAbovePrivate (door, "You quickly unlock, use and re-lock the door", character);
					return 0;
				endif
			UOBJ_KEY_RING:
				var insidering := FindMyPack (item.serial);
				foreach otherkey in EnumerateItemsInContainer (insidering)
					var keylockid := GetObjProperty (otherkey, "lockid");
					if (keylockid and keylockid == doorlockid)
						PrintTextAbovePrivate (door, "You quickly unlock, use and re-lock the door", character);
						return 0;
					endif
				endforeach
		endcase
	endforeach

	return 1;				

endfunction
