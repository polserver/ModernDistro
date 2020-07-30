use uo;

include ":itemutils:itemdesc";
include ":npcs:NPCBackpacks";
include ":containers:storageAreas";

function KP_ToggleLock( object, mobile:=0 )

	if( !mobile )
		if( object.locked )
			PrintTextAbove( object, "*unlocked*" );
			object.locked := 0;
		else
			PrintTextAbove( object, "*locked*" );
			object.locked := 1;
		endif
	else
		if( object.locked )
			PrintTextAbovePrivate( object, "*unlocked*", mobile );
			object.locked := 0;
		else
			PrintTextAbovePrivate( object, "*locked*", mobile );
			object.locked := 1;
		endif
	endif

	return 1;
endfunction

function KP_IsLockable( object )

	if( object.IsA( POLCLASS_ITEM ))
		object := object.objtype;
	endif

	return GetItemDescInfo( object ).Lockable;
endfunction

function KP_HasKeyForLock( container, lock_id )
	var held_item := 0;

	if( container.IsA( POLCLASS_MOBILE ) )
		held_item := container.GetGottenItem();
		container := container.backpack;
	endif

	if( lock_id.IsA( POLCLASS_ITEM ) )
		lock_id := GetObjProperty( lock_id, "LockID" );
	endif

	var inventory := EnumerateItemsInContainer( container );
	if( held_item.IsKey() )
		inventory.Append( held_item );
	elseif( held_item.IsKeyring() )
		inventory := inventory+EnumerateItemsInContainer( held_item );
	endif

	foreach item in ( inventory )
		SleepMS( 5 );
		if( item.IsKey() )
			if( item.KeyMatchesLock( lock_id ) )
				return 1;
			endif
		endif
	endforeach

	return 0;
endfunction

function AllocateLockID()

	var lock_id := GetGlobalProperty( "NextLockID" );
	if( !lock_id )
		lock_id := 1;
	endif

	SetGlobalProperty( "NextLockID", lock_id + 1 );

	return lock_id;
endfunction

/*
function Create_ChestKey( chest )

	var the_key := CreateItemInContainer( chest, 0x100E, 1 );
	if( the_key.errortext )
		PrintTextAbove( the_key, "Error creating key -->"+the_key.errortext );
		return 0;
	endif

	var ided := GetObjProperty( chest, "IDed" );
	if( ided )
		SetObjProperty( the_key, "IDed", ided );
	endif

	var lock_id := CreateLockID();

	SetObjProperty( the_key, "LockID", lock_id );
	SetObjProperty( chest, "LockID", lock_id );

	return 1;
endfunction
*/
function Create_ChestKey( chest )

	var the_key := CreateItemInContainer( chest, 0x100E, 1 );
	if( the_key.errortext )
		PrintTextAbove( the_key, "Error creating key -->"+the_key.errortext );
		return 0;
	endif

        var ided := GetObjProperty( chest, "IDed" );
        if( ided )
                SetObjProperty( the_key, "IDed", ided );
        endif

	var lock_id := CreateLockID();

	SetObjProperty( the_key,  "LockID", lock_id );
	SetObjProperty( chest, "LockID", lock_id );

	return 1;
endfunction

/*
 *
 * FixOldKeyRings( keyring )
 *
 * Parameters:
 * keyring - keyring obj ref
 *
 * Purpose:
 * Removes the storage areas for keyrings, moving all keys within a storage area
 * to the keyring container itself.
 *
 * Date: 2019-08-22	05:49:18
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Boolean - 1 on success, 0 on failure
 *
 */
function DuplicateKeyRing( byref keyring )
	var failed := 0,
		LockID := 0,
		key_desc := 0,
		key := 0,
		on_ring := FindMyPack( keyring.serial );

	if( !on_ring )
		return 0;
	endif

	foreach otherkey in EnumerateItemsInContainer( on_ring )
		Sleepms( 5 );
		LockID := GetObjProperty( otherkey, "LockID" );
		if( LockID )
			key_desc := GetItemDescriptor( otherkey.objtype );
			key_desc.CProps.Insert( "LockID", LockID );
			key := CreateItemInContainer( keyring, key_desc, 1 );
			if( key )
				SetName( key, otherkey.desc );
				DestroyItem( otherkey );
			else
				failed := 1;
			endif
		endif
	endforeach

	if( !failed )
		CP_RemoveStorageAreaContainer(keyring.serial);
		return 1;
	endif

	return 0;
endfunction
