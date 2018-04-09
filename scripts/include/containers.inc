/* 
 * $Id: containers.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */

use uo;
use os;

/*
 * AmountInContainer(container, properties)
 *
 * Purpose
 * Finds the amount of items with a certain objtype and 
 * optional additional properties that are in a container.
 *
 * Parameters
 * container:	Container to search
 * properties:	Optional - dictionary ( dict[property] = value )
 *		Example: dict{"objtype"->0x1235}; // only objtype 0x1235
 *		Example: dict{"color"->0} // only count if uncolored
 *
 * Return value
 * Returns an integer
 *
 */
function AmountInContainer(container, properties)
	if ( Lower(TypeOf(properties)) != "dictionary" )
		return error{"errortext":="Property list must be a dictionary."};
	endif
	
	var amount := 0;
	
	MAIN: foreach item in ( EnumerateItemsInContainer(container) )
		foreach property in ( properties.Keys() )
			if ( item.Get_Member(property) != properties[property] )
				continue MAIN;
			endif
			SleepMS(2);
		endforeach
		
		amount := amount + item.amount;
		
		SleepMS(2);
	endforeach
	
	return amount;
endfunction

/*
 * FindMatchesInContainer(container, params)
 *
 * Purpose
 * Builds a list of items with properties matching 'params'
 *
 * Parameters
 * container:	Container to search
 * properties:	Optional - dictionary ( dict[property] = value )
 *		Examples: dict{"color"->0} // only count if uncolored
 *			  dict{"objtype"->0x123A, "color"->45}
 *			  dict{"container"->itemref}
 *
 * Return value
 * Returns an array of item refs.
 *
 */
function FindMatchesInContainer(container, properties)
	if ( Lower(TypeOf(properties)) != "dictionary" )
		return error{"errortext":="Property list must be a dictionary."};
	endif
	
	var item_list := array{};
	
	MAIN: foreach item in ( EnumerateItemsInContainer(container) )
		foreach property in ( properties.Keys() )
			if ( item.Get_Member(property) != properties[property] )
				continue MAIN;
			endif
			SleepMS(2);
		endforeach
		
		item_list.Append(item);
		
		SleepMS(2);
	endforeach
	
	return item_list;
endfunction

/*
 * ListRootItemsInContainer(container)
 *
 * Purpose
 * Returns an array of all of the top level items in a container.
 *
 * Parameters
 * container:	Container to search
 *
 * Return value
 * Returns an array of item refs.
 *
 */
function ListRootItemsInContainer(container)
	var item_list := array{};
	
        foreach item in ( EnumerateItemsInContainer(container) )
		if ( item.container == container )
			item_list.Append(item);
		endif
		SleepMS(2);
	endforeach

	return item_list;
endfunction

