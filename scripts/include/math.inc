////////////////////////////////////////////////////////////
//
//
// math.inc
//
// Siggy's Improved Math Include, v2.1
// Revision Date:  01-Sep-2017
//
// Note: The contact information below is obsolete.
// It is preserved to give attribution to Sigismund,
// the original creator of this include file.
//
// Questions, comments, flames?   Send to:
// EMail: cjtdennis@comcast.net
// ICQ: 10520050
//
// New contact info:
// http://wwww.polserver.com
//
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
//
// History:
//
// v2.0:
//	- Added Min and Max functions.
//	- Added Bounded function.
//	- Added SplitLog function.
//
// v1.5:
//	- Factorial(x) now returns 1 if x==0.
//	- Log(x, y) now uses Log10 instead of successive
//	  approximation.
//
// v1.4:
//	- Fixed errors in Mean and StdDev.
//	- Minor code cleanup across include.
//
// v1.3:
//	- Removed items duplicated in math.em.
//
// 2.0:
// There were a couple of functions added that brought
// this file to revision 2.0 but were not documented.
//
// 2.1:
// Added Functions to return dice roll strings based on
// Nando's formulae in the POL Dice Generator.
// The new functions were written by ThisIsMe.
//
// 2.2
// Removed more functions duplicated in math.em.
// Removed needless functions e.g. IncRevision() which
// added 1 to the parameter passed to it
////////////////////////////////////////////////////////////

use math;


const CONST_E := 2.718282;
const CONST_PI := 3.141592;

// Used in FormulateDiceString() function
CONST SIDE_CNT    := 0;
CONST DICE_CNT    := 1;

// Used in the RoundToTens() function
const ROUND_NORMAL := 0;
const ROUND_DOWN := 1;
const ROUND_UP := 2;

///////////////////////////////////////////////////////////
//
// Log(b, x)
//
// Finds the logarithm to base <b> of <x>.
//
///////////////////////////////////////////////////////////
function Log(b, x)

	if (x<=0)
		return 0;
	endif
	return (Log10(x) / Log10(b));

endfunction

///////////////////////////////////////////////////////////
//
// SplitLog(x)
//
// Returns array of {characteristic, mantissa} for x.
// (If x==1.12345, the characteristic is "1", and the
// mantissa is (0.12345)
//
///////////////////////////////////////////////////////////
function SplitLog(x)

	return { (CInt(x)), (x - CInt(x)) };

endfunction

///////////////////////////////////////////////////////////
//
// Bounded(x, i, y, inclusive := 1)
//
// Checks to see if a number "i" falls between the two
// bounds "x" and "y".  If inclusive is set to nonzero,
// the function will return true if (i==x) || (i==y).
//
///////////////////////////////////////////////////////////
function Bounded(x, i, y, inclusive := 1)
	if (inclusive)
		if ( (i >= Min({x, y})) && (i <= Max({x, y})) )
			return 1;
		else
			return 0;
		endif
	else
		if ( (i > Min({x, y})) && (i < Max({x, y})) )
			return 1;
		else
			return 0;
		endif
	endif
endfunction

///////////////////////////////////////////////////////////
//
// Trunc(number, places)
//
// Truncates a dbl to a fixed amount of decimal places.
//
///////////////////////////////////////////////////////////
function Trunc(number, places)

	var base := Pow(10, places);

	number := CDbl(CInt(number * Pow(10, places)));
	number := number / base;

	return number;

endfunction

///////////////////////////////////////////////////////////
//
// Factorial(N);
//
// Returns the factorial of a number:
// Factorial(10) == 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1
//
///////////////////////////////////////////////////////////
function Factorial(N)

	N := CInt(N);
	var x := 1;

	while (N > 0)
		x := x * N;
		N := N - 1;
	endwhile
	return x;

endfunction

///////////////////////////////////////////////////////////
//
// YRoot(x, y)
//
// Returns the <y> root of a number <x>:
//
///////////////////////////////////////////////////////////
function YRoot(x, y)

	var i := 0.000;
	var n := 0.000;
	var root := 2.000;
	var a := CInt(Sqr(x));

	if (a < 4)
		a := 4;
	endif

	var success := 0;
	while (!success)
		n := Pow(root, y);
		i := x - n;
		i := i / (a * root);
		i := Trunc(i, 3);
		root := root + i;
		if (i == 0)
			success := 1;
			if ( Pow(Root, y) > x )
				root := root - 0.001;
			endif
			if ( Pow((root + 0.001), y) == x)
				root := root + 0.001;
			endif
		endif
	endwhile

	return root;

endfunction

///////////////////////////////////////////////////////////
//
// Mean(ArrayX)
//
// Returns the arithmatic mean (average) of an array of numbers.
//
///////////////////////////////////////////////////////////
function Mean(ArrayX)

	var n, i:=0.000;

	foreach n in ArrayX
		i := i + n;
	endforeach

	return (i / Len(ArrayX));

endfunction

///////////////////////////////////////////////////////////
//
// StdDev(ArrayX)
//
// Returns the Standard Deviation of an array of numbers.
//
///////////////////////////////////////////////////////////
function StdDev(ArrayX)

	var m, n, i := 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);

	foreach m in ArrayX
		n := m - ArrayMean;
		n := Pow(n, 2);
		i := i + n;
	endforeach

	i := i / (ArrayLen - 1);
	n := Sqr (i);

	return n;

endfunction

///////////////////////////////////////////////////////////
//
// ZTest(ArrayX, mu, sigma)
//
// Returns the test statistic for a ZTest.   This is used to show
// the likelihood that a sample is drawn from the population.  The
// mean and standard deviation for the population must be known.
// The z-statistic is computed as follows:
//
//	z==(mean - mu) / (sigma / (Sqr(n))
//
// mean==the mean of the ArrayX data
// mu==the mean of the entire population
// sigma==the standard deviation of the population
// n==the number of samples in the data
//
///////////////////////////////////////////////////////////
function ZTest(ArrayX, mu, sigma)

	var i, n;
	var z:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);

	i := ArrayMean - mu;
	n := sigma / Sqr(ArrayLen);

	z := i/n;

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// TTest(ArrayX, mu)
//
// Returns the test statistic for a TTest.   This is used to show
// the likelihood that a sample is drawn from the population, when
// the mean and standard deviation for the entire population are
// unknown.  The t-statistic is computed as follows:
//
//	t==(mean - mu) / (s / (Sqr(n))
//
// mean==the mean of the ArrayX data
// mu==the estimated mean of the entire population
// s==the standard deviation of the samples
// n==the number of samples in the data
//
///////////////////////////////////////////////////////////
function TTest(ArrayX, mu)

	var i, n, s;
	var t:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);
	var s := StdDev(ArrayX);

	i := ArrayMean - mu;
	n := s / Sqr(ArrayLen);

	t := i/n;

	return t;

endfunction



/////////////////////////////////////////////////////////////////////
// BITWISE FUNCTIONS: These are very old, and not very efficient.  //
// Use the standard $$, ||, etc. for bitwise comparisons.          //
// Left intact to support any legacy code.                         //
/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//
// BitIn(ArrayX)
//
// Takes an array of 8 bits and converts them into an integer.
// This is used for the flag support.  IMPORTANT: bit positon is
// *NOT* the same thing as the array position!   The first bit in
// the flag is the 8th bit in the array, and vice versa.  This is
// because it processes the bit values in the following order:
// 128, 64, 32, 16, 8, 4, 2, 1.
//
///////////////////////////////////////////////////////////
function BitIn(ArrayX)

	var i := 8;
	var n;
	var loop := 1;
	var bitvalue := 0;

	While (i >= 0)

		n := 8 - i;
		if (ArrayX[i]==1)
			bitvalue := bitvalue + Pow(2, n);
		endif
		i := i - 1;

	endwhile

	return bitvalue;

endfunction

///////////////////////////////////////////////////////////
//
// BitOut(x)
//
// This function takes an unsigned, short integer (0-255) and returns
// an array containing the individual bit values.   IMPORTANT: the
// first element of the array is NOT the first bit, it is the 8th
// bit, and so on down.   See the desc for BitIn for the details.
//
///////////////////////////////////////////////////////////
function BitOut(x)

	x := Cint(x);
	var loop := 1;
	var bitvalue := 0;
	var ArrayX := {0,0,0,0,0,0,0,0};
	var n;

	while (loop <= 8)

		n := 8 - loop;
		bitvalue := Pow(2, n);
		if ( (x - bitvalue) >= 0)
			ArrayX[loop] := 1;
			x := x - bitvalue;
		endif
		loop := loop + 1;

	endwhile

	return ArrayX;

endfunction

///////////////////////////////////////////////////////////
//
// BitAnd(x, y)
// Performs a bitwise logical conjunction for two unsigned, short
// integers (0-255).  If both values for a position are set, then
// that position in the result is set.   If either are reset, then
// the position in the result is reset.
//
///////////////////////////////////////////////////////////
function BitAnd(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if ( (ArrayX[loop]==1) AND (ArrayY[loop]==1) )
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// BitOr(x, y)
//
// Performs a bitwise logical disjunction for two unsigned, short
// integers (0-255).  If either value (or both values) for a
// position are set, then that position in the result is set.   If
// both are reset, then the position in the result is reset.
//
///////////////////////////////////////////////////////////
function BitOr(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if ( (ArrayX[loop]==1) OR (ArrayY[loop]==1) )
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// BitXor(x, y)
//
// Performs a bitwise logical exclusion for two unsigned, short
// integers (0-255).  If one and ONLY one value for a position is
// set, then that position in the result is set.   If both are
// either set or reset, then the position in the result is reset.
//
///////////////////////////////////////////////////////////
function BitXor(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] <> 0)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// BitEqv(x, y)
//
// Performs a bitwise logical equivalence for two unsigned, short
// integers (0-255).  If one and ONLY one value for a position is
// set, then that position in the result is reset.   If both are
// either set or reset, then the position in the result is set.
//
///////////////////////////////////////////////////////////
function BitEqv(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop]==0)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// BitImp(x, y)
//
// Performs a bitwise logical implication for two unsigned, short
// integers (0-255).  If the a position in the first integer is set,
// and the corresponding position for the second integer is reset,
// then that position in the result is reset.  All other combinations
// will result in the value being set.
//
///////////////////////////////////////////////////////////
function BitImp(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] < 1)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// BitNot(x)
//
// Performs a logical negation on a short, unsigned integer (0-255).
// If a position in the integer is set, then that position in the
// result is reset.  Conversely, if a position in the integer is
// reset, then that position in the result is set.
//
///////////////////////////////////////////////////////////
function BitNot(x)

	var ArrayX := BitOut(x);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop]==0)
			ArrayX[loop] := 1;
			z := z + Pow(2, n);
		else
			ArrayX[loop] := 0;
		endif

		loop := loop + 1;

	endwhile

	return z;

endfunction

///////////////////////////////////////////////////////////
//
// FlagSet(flag, bit)
//
// This function will set the <bit> position in the <flag> register.
// Both <flag> and <bit> are integers.
//
///////////////////////////////////////////////////////////
function FlagSet(flag, bit)

	bit := 9 - bit;

	var FlagArray := BitOut(flag);
	FlagArray[bit] := 1;

	bit := BitIn(FlagArray);

	return bit;

endfunction

///////////////////////////////////////////////////////////
//
// FlagReset(flag, bit)
//
// This function will reset the <bit> position in the <flag> register.
// Both <flag> and <bit> are integers.
//
///////////////////////////////////////////////////////////
function FlagReset(flag, bit)

	bit := 9 - bit;

	var FlagArray := BitOut(flag);
	FlagArray[bit] := 0;

	bit := BitIn(FlagArray);

	return bit;

endfunction

///////////////////////////////////////////////////////////
//
// FlagCheck(flag, bit)
//
// This function will check the flag in the <bit> position of the
// <flag> register.  It will return a 1 if the flag is set, and a 0
// if the flag is reset.  Both <flag> and <bit> are integers.
//
///////////////////////////////////////////////////////////
function FlagCheck(flag, bit)

	var FlagArray := BitOut(flag);
	bit := 9 - bit;

	if (FlagArray[bit] == 1)
		return 1;
	else
		return 0;
	endif

endfunction

/*
 * equal_probability_dice ( minimum, maximum )
 *
 * Purpose
 * To convert two numbers into an equally probable dice roll.
 *
 * Parameters
 * minimum : lowest value of the two numbers passed to function.
 * maximum : highest value of the two numbers passed to function.
 *
 * Thanks to Nando for the formulae for these dice roll functions
 *
 * Return value:
 * string dice roll representation of the minimum and maximum numbers passed.
 *
 */
function equal_probability_dice ( minimum, maximum )
	var val := "";
	minimum := CInt(minimum);
	maximum := CInt(maximum);

	if ( minimum < 1 || maximum < 1 )
		return 0;
	endif

	if ( minimum > maximum )
		var temp := minimum;
		minimum := maximum;
		maximum := temp;
	endif

	var x := 1;
	var z := minimum - 1;
	var y := ( maximum - minimum + 1 );

	if ( z > 0 )
		val := CStr ( x+"d"+y+"+"+z );
	else
		val := CStr ( x+"d"+y );
	endif

	return val;
endfunction

/*
 * normal_distribution_dice ( minimum, maximum )
 *
 * Purpose
 * To convert two numbers into a more evenly distributed dice roll.
 *
 * Parameters
 * minimum : lowest value of the two numbers passed to function.
 * maximum : highest value of the two numbers passed to function.
 *
 * Return value:
 * string dice roll representation of the minimum and maximum numbers passed.
 *
 * Thanks to Nando for the formulae for these dice roll functions
 *
 */
function normal_distribution_dice ( minimum, maximum )
	var val := "";
	minimum := CInt(minimum);
	maximum := CInt(maximum);

	if ( minimum < 1 || maximum < 1 )
		return 0;
	endif

	if ( minimum > maximum )
		var temp := minimum;
		minimum := maximum;
		maximum := temp;
	endif

	var x := 0;
	var y := 0;
	var z := 0;

	for ( z; z < minimum; z += 1 )
		x := minimum - z;
		if ( ( maximum - z ) % x == 0 )
			y := ( maximum - z ) / x;
			break;
		endif
	endfor

	if ( z > 0 )
		val := CStr ( x+"d"+y+"+"+z );
	else
		val := CStr ( x+"d"+y );
	endif

	return val;
endfunction


/*
 *
 * FormulateDiceString( mod, amount, setting )
 *
 * Parameters:
 * mod - Depending on the 'setting' parameter option, determines either the sides of dice to use or the number of dice to use.
 * amount - The total value the dice string should equal on a max roll value.
 * setting - Determines if the function should decide whether to find the max dice cnt or dice sides for the dice string.
 *
 * Purpose:
 * Formulates a dice string based on a max value and either a total number of sides or a number of dice.
 *
 * Date: 2021-11-23	00:59:43
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * string/error
 *
 */
function FormulateDiceString( byref mod, byref amount, byref setting := SIDE_CNT )

	if( mod > amount )
		return error{ "errortext":="'mod' cannot exceed amount." };
	endif

	var remainder := amount%mod,
		val := CInt( amount / mod );
	if( setting == SIDE_CNT ) // Original functionality...
		if( !remainder )
			return CStr( val+"d"+mod );
		else
			return CStr( val+"d"+mod+"+"+remainder );
		endif
	elseif( setting == DICE_CNT )
		if( !remainder )
			return CStr( mod+"d"+val );
		else
			return CStr( mod+"d"+val+"+"+remainder );
		endif
	endif

	return error{ "errortext":="Invalid 'setting' parameter." };
endfunction

///////////////////////////////////////
//
// function round( num )
//
// Parameter: 'num' is a real number
//
// Returns:  The number sent either
// rounded up or down
//
//////////////////////////////////////

function Round ( num )
 var checker := CDbl ( num - Floor ( num ) );
 if ( checker < 0.5 )
  return Floor ( num );
 else
  return Ceil ( num );
 endif
endfunction

/*
 * IsBitOn ( value, shift_by )
 *
 * Purpose:
 * Returns a 1 if the binary digit at the shift_by location is set, e.g. equal to 1.
 * Otherwise it returns a 0.
 *
 * value:		The number on which to perform the test.
 * shift_by:	The bit position to test.
 */
function IsBitOn ( value, shift_by )
	return ( 1 & ( value >> shift_by ) );
endfunction

///////////////////////////////////////////////////////////////////////////////////////////////
//
// function RoundToTens( n, behaviour := NORMAL )
// Purpose: Round the passed number to an even multiple of 10.
// Examples:	RoundToTen( 145 ) will return 140.
//						RoundToTen( 146 ) will return 150.
// Parameters:	n - Is converted to an integer.
//						behaviour - constant as defined below.
//						ROUND_NORMAL will round up or down using normal rounding
//						rules, ie. if n is in the range of 140 - 145 it will return140.
//						if n is in the range of 146 to 150 it will return 150.
//						ROUND_DOWN will always round down to the nearest tens
//						place, ie. if n is between 140 - 149 it will return 140.
//						ROUND_UP will always round up to the nearest tens
//						place, ie. if n is between 140 - 149 it will return 150.
//
///////////////////////////////////////////////////////////////////////////////////////////////

function RoundToTens( n, behaviour := ROUND_NORMAL )
	n := CInt(n);

    // Smaller multiple
    var a := (n / 10) * 10;

    // Larger multiple
    var b := a + 10;

    // Return to even tens place
	if(behaviour  == ROUND_NORMAL)
		if(n-a > b -n)
			return b;
		else
			return a;
		endif
	elseif(behaviour == ROUND_DOWN)
			return a;
	elseif(behaviour == ROUND_UP)
			return b;
	endif

endfunction

/*
 *
 * FractionToDouble( fraction )
 *
 * Parameters:
 * fraction - String fraction value.
 *
 * Purpose:
 * Converts a string fraction to a double.
 *
 * Date: 2021-06-19	23:46:54
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * double
 *
 */
function FractionToDouble( fraction )

    var parts := SplitWords( fraction, "/" ),
        whole := 0,
        numerator := parts[1],
        denominator := CDbl( parts[2] );

    if( Len( numerator ) == 3 )
        parts := SplitWords( numerator, " " );
        whole := CDbl( parts[1] );
        numerator := CDbl( parts[2] );
    else
        numerator := CDbl( numerator );
    endif

    return CDbl( whole + ( numerator / denominator ) );
endfunction

///////////////////////////////////////////////////////////
//
// FunctionName: IsPrime( byref n )
//
// Purpose: Determines if a number sent to the function
//			is a prime number.
//
// Parameter: n:: integer
//
// Returns: 1 if n is a prime, 0 if not.
//
///////////////////////////////////////////////////////////
function IsPrime( byref n )

	if(TypeOf(n) != "Integer")
		return error{"errortext" := "The value passed was not an integer."};
	endif
	if( n == 2 )
        return 1;
    endif

    if( n < 2 || ( n % 2 == 0 ) )
        return 0;
    endif

    var i := 3;
    for( i:=3; i*i<=n; i+=2 )
        if( n % i == 0 )
            return 0;
        endif
    endfor

    return 1;
endfunction

