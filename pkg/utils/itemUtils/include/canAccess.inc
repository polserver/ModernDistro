use cliloc;

///////////////////////////////////////////////////////////
//
// Function: can_access(who, item, flags := "")
//
// Purpose: Determines if a player can use an item.
//
// Parameters:	mobileref:: who
//				itemref::   item
//				string::    flags
// The 'flags' string can contain the following values:
// "distance" or "unmovable", or both words separated
// by a <space>.
// The logic is somewhat weird. If "unmovable" is not
// in flags can_access will check to see if the item
// is movable. If not, the function returns 0.
// The same is true if flags does not contain "distance"
// and the distance between the mobile and the item
// is greater than two squares a 0 is returned.
//
// Return values are: 1 if the player can use the item
// and 0 if not.
///////////////////////////////////////////////////////////
function can_access(who, item, flags := "")
	// The following conditional checks for 'ownership'
	// This is not yet implemented but I left it here for future expansion.
	var owner := Cint(GetObjProperty(item, "Owner"));
	if((owner) && (owner != who.serial))
		SendSysMessageCL(who, 1152632, color := 33); // That is not yours!
		return 0;
	elseif(GetObjProperty(item, "Vendored"))
		SendSysMessageCL(who, 500294, color := 33); // You cannot use that.
		return 0;
	elseif(!Accessible(who, item))
		SendSysMessageCL(who, 500294, color := 33); // You cannot use that.
		return 0;
	elseif((!flags["distance"]) && ((Distance(who, item) > 2)))
		SendSysMessageCL(who, 501975, color := 33); // That is too far away.
		return 0;
	elseif((!flags["unmovable"]) && (item.movable == 0))
		SendSysMessageCL(who, 1010482, color := 33); // You can't use this if it is locked down.
		return 0;
	elseif(!ReserveItem(item))
		SendSysMessageCL(who, 1062343, color := 33); // That item is currently in use.
		return 0;
	else
		return 1;
	endif
endfunction

