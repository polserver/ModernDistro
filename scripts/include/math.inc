////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

use math;


const CONST_E := 2.718282;
const CONST_PI := 3.141592;



function Log(b, x)

// Finds the logarithm to base <b> of <x>.

	if (x<=0)
		return 0;
	endif
	return (Log10(x) / Log10(b));

endfunction


function SplitLog(x)
// Returns array of {characteristic, mantissa} for x.
// (If x==1.12345, the characteristic is "1", and the 
// mantissa is (0.12345)

	return { (CInt(x)), (x - CInt(x)) };
endfunction


function Bounded(x, i, y, inclusive := 1)
// Checks to see if a number "i" falls between the two
// bounds "x" and "y".  If inclusive is set to nonzero,
// the function will return true if (i==x) || (i==y).
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


//function Min(byref x)
// returns the least value of array "x"
//	x.sort();
//	return x[1];
//endfunction


//function Max(byref x)
// returns the greatest value of array "x"
//	x.sort();
//	return x[(len(x))];
//endfunction


function Mod(Numerator, Denominator)

// Returns the modulus of the fraction:
//
//		 Numerator
//		-----------
//		Denominator


	return (Numerator - (CInt(Numerator/Denominator) * Denominator));
	
endfunction




function Remainder(numerator, denominator)

// Use this to get the remainder for non-integers.
// Returns the remainder of the fraction:
//
//		 Numerator
//		-----------
//		Denominator

	while (numerator >= denominator)
		numerator := numerator - denominator;
	endwhile

	return numerator;

endfunction


function Trunc(number, places)

// Truncates a dbl to a fixed amount of decimal places.

	var base := Pow(10, places);

	number := CDbl(CInt(number * Pow(10, places)));
	number := number / base;

	return number;

endfunction


function Fac(factorial);

// Returns the factorial of a number:
// Fac(10) == 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1

	factorial := CInt(factorial);
	var x := 1;

	while (factorial > 0)
		x := x * factorial;
		factorial := factorial - 1;
	endwhile
	return x;

endfunction


function Exp(base, power)

// Obsolete: math.em has Pow(b, p)

	return Pow(base, power);

endfunction


function YRoot(x, y)

// Returns the <y> root of a number <x>:
	
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


function Inc(a)
// Increments an integer by one.
	return (a + 1);

endfunction


function Dec(a)
// Decrements an integer by one.
	return a - 1;

endfunction


function Sqr(x)
// Obsolete: math.em has Sqrt(x)
	return Sqrt(x);

endfunction


function Mean(ArrayX)

// Returns the arithmatic mean (average) of an array of numbers.

	var n, i:=0.000;

	foreach n in ArrayX
		i := i + n;
	endforeach

	return (i / Len(ArrayX));

endfunction


function StdDev(ArrayX)
// Returns the Standard Deviation of an array of numbers.

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


function ZTest(ArrayX, mu, sigma)
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

	var i, n;
	var z:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);

	i := ArrayMean - mu;
	n := sigma / Sqr(ArrayLen);

	z := i/n;

	return := z;

endfunction


function TTest(ArrayX, mu)

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


	var i, n, s;
	var t:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);
	var s := StdDev(ArrayX);

	i := ArrayMean - mu;
	n := s / Sqr(ArrayLen);

	t := i/n;

	return := t;

endfunction



//
// BITWISE FUNCTIONS: These are very old, and not very efficient.
// Use the standard $$, ||, etc. for bitwise comparisons.
// Left intact to support any legacy code.


function BitIn(ArrayX)

// Takes an array of 8 bits and converts them into an integer.
// This is used for the flag support.  IMPORTANT: bit positon is
// *NOT* the same thing as the array position!   The first bit in 
// the flag is the 8th bit in the array, and vice versa.  This is
// because it processes the bit values in the following order:
// 128, 64, 32, 16, 8, 4, 2, 1.

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


function BitOut(x)

// This function takes an unsigned, short integer (0-255) and returns
// an array containing the individual bit values.   IMPORTANT: the 
// first element of the array is NOT the first bit, it is the 8th
// bit, and so on down.   See the desc for BitIn for the details.

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


function BitAnd(x, y)

// Performs a bitwise logical conjunction for two unsigned, short
// integers (0-255).  If both values for a position are set, then
// that position in the result is set.   If either are reset, then
// the position in the result is reset.

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


function BitOr(x, y)

// Performs a bitwise logical disjunction for two unsigned, short
// integers (0-255).  If either value (or both values) for a 
// position are set, then that position in the result is set.   If 
// both are reset, then the position in the result is reset.

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	locan n;

	while (loop <= 8)

		n := loop - 1;
		if ( (ArrayX[loop]==1) OR (ArrayY[loop]==1) )
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitXor(x, y)

// Performs a bitwise logical exclusion for two unsigned, short
// integers (0-255).  If one and ONLY one value for a position is
// set, then that position in the result is set.   If both are 
// either set or reset, then the position in the result is reset.

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


function BitEqv(x, y)

// Performs a bitwise logical equivalence for two unsigned, short
// integers (0-255).  If one and ONLY one value for a position is
// set, then that position in the result is reset.   If both are 
// either set or reset, then the position in the result is set.

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


function BitImp(x, y)

// Performs a bitwise logical implication for two unsigned, short
// integers (0-255).  If the a position in the first integer is set,
// and the corresponding position for the second integer is reset,
// then that position in the result is reset.  All other combinations
// will result in the value being set.

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


function BitNot(x)

// Performs a logical negation on a short, unsigned integer (0-255).
// If a position in the integer is set, then that position in the
// result is reset.  Conversely, if a position in the integer is 
// reset, then that position in the result is set.

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


function FlagSet(flag, bit);

// This function will set the <bit> position in the <flag> register.
// Both <flag> and <bit> are integers.

	bit := 9 - bit;
	
	var FlagArray := BitOut(flag);
	FlagArray[bit] := 1;

	bit := BitIn(FlagArray);

	return bit;

endfunction


function FlagReset(flag, bit);

// This function will reset the <bit> position in the <flag> register.
// Both <flag> and <bit> are integers.

	bit := 9 - bit;
	
	var FlagArray := BitOut(flag);
	FlagArray[bit] := 0;

	bit := BitIn(FlagArray);

	return bit;

endfunction


function FlagCheck(flag, bit);

// This function will check the flag in the <bit> position of the
// <flag> register.  It will return a 1 if the flag is set, and a 0
// if the flag is reset.  Both <flag> and <bit> are integers.

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

