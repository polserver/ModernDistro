use uo;
use os;
use basic;
use math;
use util;

include "include/objtype";
include "include/utility";

var coinarray := {};
var createdamt := 0;
var incrementsOfOne := {"Zero",    "One",     "Two",       "Three",    "Four",
              			"Five",    "Six",     "Seven",     "Eight",    "Nine",
              			"Ten",     "Eleven",  "Twelve",    "Thirteen", "Fourteen",
              			"Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};

var incrementsOfTen := {"",      "",      "Twenty",  "Thirty", "Fourty",
              			"Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};

var millionName := "Million",
	thousandName := "Thousand",
	hundredName := "Hundred",
	andName := "and";

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

function ConvertCurrencyToText( value )

	var string_currency := "";
	if( IsNegative( value ) )
		return error{"errortext":="ConvertCurrencyToText function does not handle negative values."};
	endif

	if( value >= 1000000 )
		string_currency := string_currency+ParseTriplet( value / 1000000 )+" "+millionName;
		value := value % 1000000;
		if( value == 0 )
			return string_currency;
		endif
	endif


	if( value >= 1000 )
		if( Len( string_currency ) > 0 )
			string_currency := string_currency+", ";
		endif
		string_currency := string_currency+ParseTriplet( value / 1000 ) + " " + thousandName;
		value := value % 1000;
		if( value == 0 )
			return string_currency;
		endif
	endif

	if( Len( string_currency ) > 0 )
		string_currency := string_currency+", ";
	endif

	string_currency := string_currency+ParseTriplet( value );

	return string_currency;
endfunction

function ParseTriplet( i )

	var result := "";
	if( i >= 100 )
		result := result + incrementsOfOne[(i / 100)+1] + " " + hundredName;
		i := i % 100;
		if( i == 0 )
			return result;
		endif
	endif

	if( Len( result ) > 0 )
		result := result + " " + andName + " ";
	endif

	if( i >= 20 )
		result := result + incrementsOfTen[(i / 10)+1];
		i := i % 10;
		if( i == 0 )
			return result;
		endif
	endif

	if( Len( result ) > 0 )
		result := result + " ";
	endif

	result := result + incrementsOfOne[i+1];
	return result;
endfunction

function IsNegative( byref value )

	return ( value < 0 );
endfunction

// Takes an int and converts into human readable format
// 500234343 becomes 500.234.343 for example
// Limited to int32 numbers (so max 2.147.483.647)
function MakeNumberHumanReadable(number)

	var delimiter := ",";

	number := CStr(number);

	var is_negative := 0;
	if(number[1] == "-")
		is_negative := 1;
		var newnumber := "";
		var c := 2;
		for(c;c<=len(number);c:=c+1)
			newnumber := newnumber + number[c];
		endfor
		number := newnumber;
	endif

	var numlen := len(number);

	var res := numlen%3;

	var i, pre := "";
	for( i:=1; i<=res; i:=i+1 )
		pre := pre+CStr(number[i]);
	endfor

	var prelen := len(pre);

	if(numlen > 3 && prelen > 0)
		pre := pre+delimiter;
	endif

	var j,after := "";
	for( j:=prelen+1;j<=numlen;j:=j+1)
		after := after+CStr(number[j]);
	endfor

	var afterlen := len(after);

	var h, final := "";
	for(h:=1;h<=afterlen;h:=h+1)
		final := final+after[h];
		if( !( h%3 ) && h < afterlen)
			final := final+delimiter;
		endif
	endfor
	final := pre+final;

	if(is_negative)
		final := "-"+final;
	endif

	return CStr(final);

endfunction

