use basic;
use storage;

function find_or_create_storage( areaname )
  var area;
  area := FindStorageArea( areaname );
  if(!area)
	area := CreateStorageArea( areaname );
    if(area)
    endif
  endif
  return area;
endfunction

function find_or_create_item(storage, name, objtype := 0x1966a)
	var item_found := FindRootItemInStorageArea(storage, name);
	if(item_found)
		return item_found;
	endif

	item_found := CreateRootItemInStorageArea( storage, name, objtype);
	return item_found;
endfunction

function IsLocationAccessible(character, x, y, z)
  var xd, yd, zd;
  xd := character.x - x;
  if(xd < -1 || xd > 1)
    return 0;
  endif
  yd := character.y - y;
  if(yd < -1 || yd > 1)
    return 0;
  endif
  zd := character.z - z;
  if(zd < -10 || zd > 10)
    return 0;
  endif
  return CheckLosAt( character, x, y, z );
endfunction

///////////////////
// Sends messages to all online GMs. Useful for important errors
///////////////////

function SendGMErrorMessage(text, syslogit := 0, mincmdlevel := 1)
    if ( syslogit )
        syslog(text);
    endif

    foreach player in EnumerateOnlineCharacters()
        if ( player.cmdlevel )
            if ( player.cmdlevel >= mincmdlevel )
                SendSysMessage(player, "Staff system message: " + text, color := 220);
            endif
        endif
    endforeach
endfunction

//////////////////////////////////////////////////////////////////////////////////////
//
//  Determines if the object is within a rectangle
//  Note: The scope of this function is to determine whether
//  an object lies within a two dimwnsional area.
//  There is no 'z' check performed.
//
//  Parameters:
//  Name                  Type
//  object                UO_OBJECT reference
//  boundary              Array containing the {nwx, nwy, sex, sey}
//                        The function assumes that the 'boundary' parameter
//                        might contain numeric values as strings in its elements.
//
/////////////////////////////////////////////////////////////////////////////////////////

function IsInsideRectangle (object, boundary)

    if ((object.x >= CInt(boundary[1]) && object.x <= CInt(boundary[3])) && (object.y >= CInt(boundary[2]) && object.y <= CInt(boundary[4])))
        return 1;
    else
        return 0;
    endif
endfunction

//////////////////////////////////////////////////////////////////////////////////////////////
//  Determines if the object is within a rectangle at a specified Z coordinate
//
//  Note: The scope of this function is to determine whether an object
//  lies within a two dimwnsional area at a specified Z coordinate.
//
//  Parameters:
//  Name                  Type
//  object                UO_OBJECT reference
//  boundary              Array containing the {nwx, nwy, sex, sey}
//  ZCoord                The desired Z level
//
//                        The function assumes that the 'boundary' and ZCoord parameters
//                        might contain numeric values as strings in its elements.
//
//
//  Returns               0 if 'object' isn't within the rectangle at the ZCoord
//                        1 if 'object' is in the rectangle AND at the desired ZCoord
//////////////////////////////////////////////////////////////////////////////////////////////

function IsInsideRectAtZLevel (object, boundary, ZCoord)

    SendSysMessage(object, ZCoord + "     " +  CStr(object.z));
    if ( (object.x >= CInt(boundary[1]) && object.x <= CInt(boundary[3])) &&
         (object.y >= CInt(boundary[2]) && object.y <= CInt(boundary[4])) && object.z == CInt(ZCoord) )
        return 1;
    else
        return 0;
    endif
endfunction

function HLEnumerateOnlineCharacters (ex_staff := 0, ex_concealed_staff := 0)

        var list := array;
        var processedList := array;
        var index, index2, ignore;
        list := EnumerateOnlineCharacters ();
        index2 := 1;

        for (index := 1; index <= list.size(); index := index + 1)
            ignore := 0;
            if (list[index].cmdlevel)
                if (ex_concealed_staff && list[index].concealed)
                    ignore := 1;
                endif
                if (ex_staff)
                    ignore := 1;
                endif
            endif
            if (GetObjProperty (list[index], "HideDev"))
                ignore := 1;
            endif
            if (!ignore)
                processedList[index2] := list[index];
                index2 := index2 + 1;
            endif
        endfor
        return processedList;
endfunction



//returns all the items in container of objtype
function EnumerateItemsInContainerOfObjtype(container, objtype)

	var ret := { };
        foreach item in EnumerateItemsInContainer(container)
		if ( item.objtype == objtype )
			ret.append(item);
		endif
	endforeach

	return ret;

endfunction

//returns all the 'top-level' items in container of objtype
function ListRootItemsInContainerOfObjtype(container, objtype)

	var ret := { };
        foreach item in EnumerateItemsInContainer(container, ENUMERATE_ROOT_ONLY)
		if (item.objtype == objtype)
			ret.append(item);
		endif
	endforeach

	return ret;

endfunction

function InInventory(inventory, it)

	foreach item in EnumerateItemsInContainer(inventory)
		if (item == it)
			return 1;
		endif
	endforeach

	return 0;

endfunction

function IsContainer( it )

	if (it.objtype == 0x7057)
		return 0;//trashcans don't count as containers
	endif

	var waslocked := it.locked;
	it.locked := 1;
	if (it.locked)
		it.locked := waslocked;
		return 1;
	else
		return 0;
	endif

endfunction

function IsDungeon( x, y )

	if ( (x >= 5120) && (x <= 6143) )
		if ( (y >= 0) && (y <= 2303) )
			return 1;
		endif
	endif

	return 0;

endfunction

function NoTeleportZone( x, y )

	if ( (x >= 5120) && (x <= 6143) )
		if ( (y >= 2080) && (y <= 2303) )
			return 1;
		endif
	endif

	return 0;

endfunction

//	GetStacksOfTypeInContainer (container, objtype)
//	Loops through the given container looking for item stacks from the given objtype.
//	Returns: an array where the first element is the sum of items from the
//		 given objtype within the container, and the following elements are
//	ObjRefs to the item stacks found.
//	Example: if used at a container with 2 stacks of 50 cloth, the function
//		 would return {100, 1st_stack_of_cloth, 2nd_stack_of_cloth}.
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

//	SubtractFromStackArray (byref arrayofobjrefs, amount)
//	Loops through the array of objects until the given amount of items is subtracted, or the last item is destroyed. IE if 2 piles
//	of 10k gold are passed as the first argument, and 15000 is passed as the second argument, the first pile will be destroyed and the
//	second pile will have 5000 gold subtracted from it.
//	In case the array contains non-POL-items objects, those objects are ignored.
//	Returns: amount of items successfully subtracted.

function SubtractFromStackArray (byref arrayofobjrefs, amount)
	var needed := amount;
	foreach item in arrayofobjrefs
		if (!item.IsA(POLCLASS_ITEM)) continue; endif
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

