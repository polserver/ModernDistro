use uo;
use os;

// Container Constants...
Const CT_BONES		:= "Bones";
Const CT_LEATHER	:= "Leather";
Const CT_METAL		:= "Metal";
Const CT_STRAW		:= "Straw";
Const CT_WOOD		:= "Wood";

function AmountInContainer( container, properties )

	if( Lower( TypeOf( properties )) != "dictionary" )
		return error{"errortext":="Property list must be a dictionary."};
	endif

	var amount := 0;

	MAIN: foreach item in ( EnumerateItemsInContainer( container ))
		foreach property in ( properties.Keys() )
			if( item.Get_Member( property ) != properties[property] )
				continue MAIN;
			endif
			SleepMS(2);
		endforeach

		amount += item.amount;

		SleepMS(2);
	endforeach

	return amount;
endfunction

function FindMatchesInContainer( container, properties )

	if( Lower( TypeOf( properties )) != "dictionary" )
		return error{"errortext":="Property list must be a dictionary."};
	endif

	var item_list := array{};

	MAIN: foreach item in ( EnumerateItemsInContainer( container ))
		foreach property in ( properties.Keys() )
			if( item.Get_Member( property ) != properties[property] )
				continue MAIN;
			endif
			SleepMS(2);
		endforeach

		item_list.Append( item );

		SleepMS(2);
	endforeach

	return item_list;
endfunction

function CreateLockID()

	var lock_id := GetGlobalProperty( "NextLockID" );
	if( !lock_id || lock_id.errortext )
		lock_id := 1;
	endif

	SetGlobalProperty( "NextLockID", lock_id+1 );

	return lock_id;
endfunction
