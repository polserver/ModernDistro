use uo;
use os;

include "include/objtype";
include "include/utility";

var coinarray := {};
var createdamt := 0;

function CountCoins( byref container, coin_type )

	var amt := 0;
	foreach item in ( FindSubstance( container, coin_type, 1, 0, FINDSUBSTANCE_FIND_ALL ) )
		amt+=item.amount;
	endforeach

	return amt;
endfunction

function CreateCoins( container, coin_type, amt )

	if( !amt )
		return -1;
	endif

	var coin_array := Array,
		fail := 0,
		item := 0;
	if( amt > 60000 )
		while( amt > 60000 )
			item := CreateItemInContainer( container, coin_type, 60000 );
			if( item )
				amt -= 60000;
				coin_array.Append( item );
			else
				fail := 1;
				break;
			endif
		endwhile
	endif

	if( amt > 0 && !fail )
		item := CreateItemInContainer( container, coin_type, amt );
		if( item )
			coin_array.Append( item );
		else
			fail := 1;
		endif
	endif

	if( fail )
		foreach coin_stack in ( coin_array )
			DestroyItem( coin_stack );
		endforeach
	endif

// Some reverse logic, if fail does not equal 1, everything went well
// and will return 1, if however, it does equal 1, then we will return
// 0.
	return ( fail != 1 );
endfunction

function SpendCoin( who, amt, cont, cointype )

	who := who;

	var coins := CountCoins( cont, cointype );
	if( coins < amt )
		return 0;
	endif

	return ConsumeSubstance( cont, cointype, CInt( amt ) );
endfunction
