/*
        2009-10-20

        Author: Austin

        Purpose
        Assists in the use of storage areas.

        CP_GetStorageContainerForMobile(container_id, storage_area_name:=0)
        CP_GetStorageAreaContainer(mobile, storage_area_name:=0, create:=CP_NOCREATE)
        CP_CreateStorageAreaContainer(container_id, storage_area_name:=0)
        CP_RegisterStorageArea(storage_area_name)
        CP_RemoveStorageAreaContainer(container_id, storage_area_name)
        CP_StorageDebugMsg(text)
*/
use uo;
use os;
use file;
use polsys;
use storage;

// * Global Variables *
CONST CP_NOCREATE	:= 0x0;
CONST CP_CREATE		:= 0x1;

CONST CP_DEFAULT_AREA	:= "World Bank";
CONST CP_DEFAULT_PREFIX	:= "[Root Container] ";
//CONST CP_DEFAULT_PREFIX	:= "Bankbox of ";

/*
 * Set the 'box_type' variable to the object name of the container type
 * you want to be used for bank box.
 * Example: "pouch" "metalchest" "barrel"
 *
 * This is ONLY used when a new root container is created. It can be changed on
 * the fly inside of a script for when you want to use specific container
 * types for special storage areas.
 *
 * Ideally, you should make an object type with the name 'BankBox' to better
 * control the number of items that can be put in the storage container.
 */
var container_type	:= "BankBox";

// Toggles the bank related debug messages on and off.
var STORAGE_DEBUG	:= 1;

/*
 * CP_GetMobileContainerID(mobile)
 *
 * Purpose
 * Sets up the container_id string for a mobile's root container.
 *
 * Parameters
 * mobile:	Mobile whos box will be retrieved.
 *
 * Return value
 * String
 *
 */
function CP_GetMobileContainerID( mobile, prefix := "Bankbox of " )

	if( mobile.IsA( POLCLASS_CORPSE ))
		return prefix + Hex( mobile.ownerserial );
	else
		return prefix + Hex(mobile.serial);
	endif
endfunction

/*
 * GetStorageContainerForMobile(mobile, storage_area_name, create)
 *
 * Purpose
 *
 * Parameters
 * mobile:		Owner of the container that will be retrieved (uses .serial of it)
 * storage_area_name:	Name of the storage area to retrieve the container from.
 * create:		Controls if the container will be created if it does not already exist.
 *
 * Return Value
 * A container object
 *
 */
function CP_GetStorageContainerForMobile( mobile, storage_area_name:=0, create:=CP_NOCREATE )

	if(( !storage_area_name ) || ( storage_area_name == error ))
		storage_area_name := CP_DEFAULT_AREA;
	endif

	var container_id := CP_GetMobileContainerID( mobile );
	var root_container := CP_GetStorageAreaContainer( container_id, storage_area_name, create );

	return root_container;
endfunction

/*
 * CP_GetStorageAreaContainer(container_id, storage_area_name, create)
 *
 * Purpose
 * Retrieves a root container that matches the id sent.
 *
 * Parameters
 * container_id:	Name of the root container to retrieve.
 * storage_area_name:	Name of the storage area to retrieve the container from.
 * create:		Controls if the container will be created if it does not already exist.
 *
 * Return value
 * A container object
 *
 */
function CP_GetStorageAreaContainer( container_id, storage_area_name:=0, create:=CP_NOCREATE )

	if(( !storage_area_name ) || ( storage_area_name == error ))
		storage_area_name := CP_DEFAULT_AREA;
	endif

	var container_name := CP_DEFAULT_PREFIX + container_id;
//	var container_name := container_id;

	var storage_area := FindStorageArea( storage_area_name );
	if( !storage_area )
		if( !create )
			return storage_area; // Will be an error
		else
			return CP_CreateStorageAreaContainer( container_id, storage_area_name );
		endif
	endif

	var root_container := FindRootItemInStorageArea( storage_area, container_name );
	if( !root_container && create )
		return CP_CreateStorageAreaContainer( container_id, storage_area_name );
	else
		return root_container;
	endif
endfunction

/*
 * CP_CreateStorageAreaContainer(container_id, storage_area_name)
 *
 * Purpose
 * Creates a root container in a storage area.
 *
 * Parameters
 * container_id:	Name of the container to create.
 * storage_area_name:	Name of the storage area to create the container in.
 *
 * Return value
 * A container object
 *
 */
function CP_CreateStorageAreaContainer( container_id, storage_area_name:=0 )

	if(( !storage_area_name ) || ( storage_area_name == error ))
		storage_area_name := CP_DEFAULT_AREA;
	endif

	var storage_area := FindStorageArea( storage_area_name );
	if( !storage_area )
		// The storage area does not exist - so create it.
		storage_area := CreateStorageArea( storage_area_name );
	endif

	var container_name := CP_DEFAULT_PREFIX + container_id;

	var root_container := CreateRootItemInStorageArea( storage_area, container_name, container_type );
	if( root_container.errortext )
		CP_StorageDebugMsg( "Error::CP_CreateStorageAreaContainer() - "+root_container.errortext );
	endif

	return root_container;
endfunction

/*
 * CP_RemoveStorageAreaContainer(container_id, storage_area_name)
 *
 * Purpose
 * Removes a root container from a storage area.
 * Also deletes all of its contents.
 *
 * Parameters
 * container_id:	Name of the container to remove.
 * storage_area_name:	Name of the storage area to remove the container from.
 *
 * Return value
 * Returns the result of the deletion.
 *
 */
function CP_RemoveStorageAreaContainer( container_id, storage_area_name:=0 )

	if(( !storage_area_name ) || ( storage_area_name == error ))
		storage_area_name := CP_DEFAULT_AREA;
	endif

	var storage_area := FindStorageArea( storage_area_name );
	var container_name := CP_DEFAULT_PREFIX + container_id;

	return DestroyRootItemInStorageArea( storage_area, container_name );
endfunction

/*
 * CP_GetStorageAreaNames()
 *
 * Purpose
 * Returns a list of storage area names
 *
 * Parameters
 *
 * Return value
 * Array
 *
 */
function CP_GetStorageAreaNames()

	var name_list := array;
	foreach storage_area in ( StorageAreas() )
		// Used to fix unused var report by ecompile: storage_area;
		storage_area := storage_area;
		name_list.Append( _storage_area_iter );
		SleepMS(2);
	endforeach

	return name_list;
endfunction

/*
 * CP_GetRootContainersInStorageArea(storage_area_name)
 *
 * Purpose
 *
 * Parameters
 *
 * Return value
 * Array
 *
 */
function CP_GetRootContainersInStorageArea( storage_area_name )

	var storage := StorageAreas();
	var root_list := array;
	foreach container in ( storage[storage_area_name] )
		root_list.Append( container );
		SleepMS(2);
	endforeach

	return root_list;
endfunction

/*
 * CP_StorageDebugMsg(text)
 *
 * Purpose
 *
 * Parameters
 *
 * Return value
 *
 */
function CP_StorageDebugMsg( report_text )

	if( STORAGE_DEBUG )
		var script_name := GetProcess( GetPid() ).name;
		LogToFile( "::log/storage.log", "["+script_name+"]: "+report_text, LOG_DATETIME );
	endif

	return 1;
endfunction

