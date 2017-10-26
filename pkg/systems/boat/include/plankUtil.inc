include "include/client";
include "include/objtype";
include "include/NPCBackpacks";




///////////////////
//  Determines if the plank is extened
///////////////////

function IsExtended( plank )
	return plank.graphic in {GID_PLANK_EXTENDED_FACING_EAST, 
						GID_PLANK_EXTENDED_FACING_WEST,
						GID_PLANK_EXTENDED_FACING_NORTH,
						GID_PLANK_EXTENDED_FACING_SOUTH };
endfunction                           




///////////////////
//  Determines if there's any mobiles standing on the extended plank
///////////////////

function IsPlankOccupied (plank)
	return ListMobilesNearLocation (plank.x, plank.y, plank.z, 0, plank.realm).size();
endfunction




///////////////////
//  Extend the plank
///////////////////

function Extend (plank)
	if (!IsExtended (plank))
		TogglePlank (plank);
	endif
endfunction




///////////////////
//  Retract the plank
///////////////////

function Retract (plank)
	if (IsExtended (plank))
		TogglePlank (plank);
	endif
endfunction




///////////////////
//  Toggles the plank from open to closed
///////////////////

function TogglePlank (plank)
	var partnertype := PlankPartner (plank.graphic);
	if (partnertype)
		plank.graphic := partnertype;
	endif
endfunction




///////////////////
//  Figures out the matching open/closed art for each plank facing
///////////////////

function PlankPartner (planktype)
	case (planktype)
		GID_PLANK_EXTENDED_FACING_EAST:  return GID_PLANK_RETRACTED_FACING_EAST;
		GID_PLANK_RETRACTED_FACING_EAST: return GID_PLANK_EXTENDED_FACING_EAST;
		GID_PLANK_EXTENDED_FACING_WEST:  return GID_PLANK_RETRACTED_FACING_WEST;
		GID_PLANK_RETRACTED_FACING_WEST: return GID_PLANK_EXTENDED_FACING_WEST;

		GID_PLANK_EXTENDED_FACING_NORTH:  return GID_PLANK_RETRACTED_FACING_NORTH;
		GID_PLANK_RETRACTED_FACING_NORTH: return GID_PLANK_EXTENDED_FACING_NORTH;
		GID_PLANK_EXTENDED_FACING_SOUTH:  return GID_PLANK_RETRACTED_FACING_SOUTH;
		GID_PLANK_RETRACTED_FACING_SOUTH: return GID_PLANK_EXTENDED_FACING_SOUTH;
            
		default:
			syslog ("Unknown plank type " + Hex (planktype));
			return error;
    endcase
endfunction



///////////////////
//  Tries to find the key that'll open the plank
///////////////////

function FindKey (me, plank)
	var doorlockid := GetObjProperty (plank, "lockid");
	var keylockid;

	foreach item in EnumerateItemsInContainer (me.backpack)
		case (item.objtype)
			UOBJ_COPPER_KEY:
			UOBJ_GOLD_KEY:
			UOBJ_IRON_KEY:
			UOBJ_MAGIC_KEY:
			UOBJ_RUSTY_IRON_KEY:
				keylockid := GetObjProperty (item, "lockid");
				if (keylockid and keylockid == doorlockid)
					return item;
				endif
			UOBJ_KEY_RING:
				foreach key in EnumerateItemsInContainer (FindMyPack (item.serial))
					keylockid := GetObjProperty (key, "lockid");
					if (keylockid and keylockid == doorlockid)
						return key;
					endif
				endforeach
		endcase
	endforeach
					
	return 0;
endfunction

