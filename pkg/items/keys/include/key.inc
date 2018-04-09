/*
        Maintened by *Edwards
        
        2009-10-18
*/
use uo;

include ":itemutils:itemdesc";

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

	if( container.IsA( POLCLASS_MOBILE ))
		container := container.backpack;
	endif
 
	if( lock_id.IsA( POLCLASS_ITEM ))
		lock_id := GetObjProperty( lock_id, "LockId" );
	endif
	
	foreach item in ( EnumerateItemsInContainer( container ))
		if( item.IsKey() )
			if( item.KeyMatchesLock( lock_id ))
				return 1;
			endif
		endif
		SleepMS(2);
	endforeach
	
	return 0;
endfunction

function AllocLockId()

	var lock_id := GetGlobalProperty( "NextLockID" );
	if( !lock_id )
		lock_id := 1;
	endif
 
	SetGlobalProperty( "NextLockID", lock_id + 1 );

	return lock_id;
endfunction

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

	SetObjProperty( the_key,  "LockId", lock_id );
	SetObjProperty( chest, "LockId", lock_id );
                
	return 1;
endfunction
