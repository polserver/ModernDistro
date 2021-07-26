use os;
use uo;
use util;
use cfgfile;

include ":keys:key";

// New requried functions.....
// Checks to see if the item can be used...
function CanUseDoor( byref mobile, byref door )

	if( mobile.hidden )
		mobile.hidden := 0;
	endif

	if( mobile.dead )
		return 0;
	elseif( Distance( mobile, door ) > 2 )
		return 0;
	elseif( door.container )
		SendSysMessage( mobile, "You cannot use that while it is in a container.", 0x3, 0x22 );
		return 0;
	elseif( GetObjProperty( door, "WoodType" ) || GetObjProperty( door, "IngotType" ) )
		if( !GetObjProperty( door, "lockeddown" ) )
			SendSysMessage( mobile, "You have to lock it down before you can use it!", 0x3, 0x22 );
			return 0;
		endif
	elseif( GetObjProperty( door, "leverlinks" ) && !GetObjProperty( door, "ignorejammed" ) ) // Activeted from lever, no manuel intervention is possibly
		if( !door.IsOpen() )
			PrintTextAbovePrivate( door, "It appears to be jammed...", mobile, 0x3, 0x32 );
		endif
		return 0;
	endif

	return 1;
endfunction

///////////////////
//  checks to see if the door is locked and, if it is, tries to find a key to open it in the
//  character's backpack
///////////////////
function DoorIsLocked( byref mobile, byref door )

	if( !door.locked )
		// Nothing to worry about.
		return 0;
	elseif( KP_HasKeyForLock( mobile, door ) )
		PrintTextAbovePrivate( door, "You quickly unlock, open, and relock the door.", mobile, 0x3, 0x42 );
		return 0;
	elseif( door.locked && mobile.cmdlevel >= 3 )
		//Potential abuse of this from staff.
		SendSysMessage( mobile, "That is locked, but you open it with your godly powers.", 0x3, 0x42 );
		SetObjProperty( door, "#LastStaffUsed", array{mobile.serial, ReadGameClock()} );
		return 0;
	endif

	PrintTextAbovePrivate( door, "That is locked.", mobile, 0x3, 0x22 );

	return 1;
endfunction

function IsOccupied( item )

	var mod := item.Offset(),
		x := item.x,
		y := item.y;

	if( mod.XMod > 0 || mod.XMod < 0 )
		x := item.x-CInt(mod.XMod);
	endif

	if( mod.YMod > 0 || mod.YMod < 0 )
		y := item.y-CInt(mod.YMod);
	endif

	return ListMobilesNearLocationEX( x, y, item.z, 0, LISTEX_FLAG_HIDDEN+LISTEX_FLAG_NORMAL, item.realm );
endfunction
