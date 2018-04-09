use cfgfile;
use uo;
use storage;
use vitals;

function HLEnumerateOnlineCharacters (ex_staff := 0, ex_concealed_staff := 0);

        var list := array;
        var processedList := array;
        var index, index2, ignore;
        list := EnumerateOnlineCharacters ();
        index2 := 1;
        
        for (index := 1; index <= list.size(); index := index + 1)
            ignore := 0;
            if (list[index].cmdlevel)
                if (ex_concealed_staff and list[index].concealed)
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

//returns all the 'top-level' items in container
function ListRootItemsInContainer(container)

	var ret := { };
        foreach item in EnumerateItemsInContainer(container)
		if ( item.container == container )
			ret.append(item);
		endif
	endforeach

	return ret;

endfunction

//returns all the 'top-level' items in container of objtype
function ListRootItemsInContainerOfObjtype(container, objtype)

	var ret := { };
        foreach item in EnumerateItemsInContainer(container)
		if ( (item.container == container) && (item.objtype == objtype) )
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

