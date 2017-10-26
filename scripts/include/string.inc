///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// string.inc
//
// Siggy's String Function Include, v1.3
//
// History:
//
//  Version 1.4
//  - Yukiko
//    added SearchStr()
//	Version 1.3
//	- Austin
//		added SetString()
//		added UCFirst()
//		added RemoveSpaces()
//	Version 1.2 - 26-Apr-2000
//	- Myrathi Added SortMultiArrayByIndex
//	- Code tweaks by Myrathi
//
//	Version 1.1 - 12-Apr-2000
//	- Added ReturnIndex
//	- Code cleanup
//
//	Version 1.0 - 10-Apr-2000
//
//
// Questions, comments, flames?   Send to Sig at -
// EMail: prostheticdelirium@worldnet.att.net
// ICQ: 10520050
//
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
use os;
use basic;
use basicio;

const CONST_EMPTY := { };
const CONST_NULL := "";

const CLR_LEADING_SPACES 	:= 0x1;
const CLR_TRAILING_SPACES 	:= 0x2;
const CLR_DOUBLE_SPACES		:= 0x4;
const CLR_ALL_SPACES		:= 0x8;

// Function TruncateArticle(x)
// This function will remove an indefinite article ("a" or "an")
// from a string "x", returning the rest of the string.  If there
// is no article, it returns the original string "x".

function TruncateArticle(x)

	x := CStr(x);
	var i := len(x);

	var an_articlelist := { "an ", "An ", "AN ", "aN " };
	var a_articlelist := { "a ", "A " };

	if ( x[1,3] in an_articlelist )
		x := (x[4,i-3]);
	elseif ( x[1,2] in a_articlelist )
		x := (x[3,i-2]);
	endif

	return x;

endfunction


// function AddArticle(x)
// This function will prefix an indefinite article ("a" or "an") in
// front of a string.  If the string begins with a consonant, it
// uses "a", and if the string begins with a vowel it will use "an".
// Note, this will occasionally produce grammatically incorrect
// phrases involving strings beginning with "h" or glided "u" sounds,
// such as "a hour", "an unit", or "a eunuch".  For the curious, the
// "a" is used before consonants, including the "y" sound in some
// phoenemes of "u".  The "an" article is used before a vowel sound,
// or before an "h" in words where the accent falls on any syllable
// other than the first (like "historic").

function AddArticle(x)

	x := CStr(x);
	var vowellist := { "a", "A", "e", "E", "i", "I", "o", "O", "u", "U" };
	var i := x[1];

	if ( i in vowellist )
		x := "an " + x;
	else
		x := "a " + x;
	endif

	return x;

endfunction


// function CreateArrayIndex(MainArray)
// This function will sort an array, and create an index of the
// original positions for each element.  This index can then be used
// to sort other arrays in the same order as the first one.  For
// example, if you have the array:
//
//      var myarray := { 4, 5, 9, 0, 100, 22, 7 };
//
// the function will return the following:
//
//	result := CreateArrayIndex(myarray);
//	SortedArray := result[1];  // { 0, 4, 5, 7, 9, 22, 100 }
//	IndexArray := result[2];   // { 4, 1, 2, 7, 3, 6, 5}
//
// This function is useful for preserving information across multiple
// arrays when you need to sort one of them.

function CreateArrayIndex(MainArray)

	var IndexArray := CONST_EMPTY;
	var ArrayLen := Len(MainArray);
	var OldArray := MainArray;
	var NewArray := MainArray.sort;
	var i, q;

	for (q := 1; q <= ArrayLen; q := q + 1)
		for (i := 1; i <= ArrayLen; i := i + 1)
			if (OldArray[q] == MainArray[i])
				IndexArray[q] := i;
				i := ArrayLen + 1;
			endif
		endfor
	endfor

	return ( { NewArray, IndexArray } );

endfunction


// function SortArrayByIndex(OldArray, IndexArray)
// This function will sort "OldArray" by the order outlined in
// "IndexArray", where "IndexArray" is an array of sequential numbers
// equal to { 1, 2, 3, ... , len(OldArray) }.  If the IndexArray is a
// different size than OldArray, or if the sequence is broken, then
// the function will return 0.

function SortArrayByIndex(OldArray, IndexArray)

	var ArrayLen := Len(OldArray);

	if (ArrayLen != Len(IndexArray) )
		return 0;
	endif

	var NewArray := CONST_EMPTY;
	var i, q;

	for (i := 1; i <= ArrayLen; i := i + 1)
		q := IndexArray[i];
		if (q > ArrayLen)
			return 0;
		endif

		NewArray[i] := OldArray[q];

	endfor

	return NewArray;

endfunction


// function SortMultiArrayByIndex(MultiArray, SubIndex)
// (Function added 26-Apr-2000 by Myrathi)
// This function will sort an array of arrays (or structures) by
// the contents of the sub-index provided. So, if you have an
// array (zCoords) of [x,y,z] structures, calling the function
// with 'fn(zCoords,2)' will sort by the second sub-index... that
// is, the 'y' member. Returns '0' if the SubIndex is invalid
// (only checks against the first index)

function SortMultiArrayByIndex(MultiArray, SubIndex)

	var ArrayLen := Len(MultiArray);
	if ( ArrayLen < 2 )
		return MultiArray;
	endif

	if ( SubIndex > len(MultiArray[1]) )
		return 0;
	endif

	var i, k, f, s;
	for(i := 1; i < ArrayLen; i := i + 1)
		for( k := i+1; k <= ArrayLen; k := k + 1)
			f := MultiArray[i];
			s := MultiArray[k];
			if ( s[SubIndex] < f[SubIndex] )
				MultiArray[i] := s;
				MultiArray[k] := f;
			endif
		endfor
		//sleepms(2);
	endfor

	return MultiArray;

endfunction


// function PadString(x, stringlength)
// This function will pad a string "x" with enough spaces to reach
// a length indicated by "stringlength".  If the string is already
// longer than stringlength, the function will return 0.

function PadString(x, stringlength)

	var padding := stringlength - len(x);
	if (padding < 0)
		return 0;
	endif

	var padstr := "                    ";     // 20 spaces

	while(padding > 20)
		x := x + padstr;
		padding := padding - 20;
	endwhile

	if (padding > 0)
		x := x + padstr[1, padding];
	endif

	return x;

endfunction



// function PadArray(x, arraylength)
// This function will pad an array "x" with enough 0s to reach length
// specified in "arraylength".  If the length of the array is already
// greater than arraylength, the function will return 0.

function PadArray(x, arraylength)

	var padding := arraylength - len(x);

	if (padding < 0)
		return 0;
	elseif (padding == 0)
		return x;
	endif

	var q := len(x);
	var i;

	for (i := 1; i <= padding; i := i + 1)
		q := q + 1;
		x[q] := 0;
	endfor

	return x;

endfunction


// function ReturnIndex(myarray, elementvalue)
// This function will return the index (location within an
// array) of an element.  If the element is not found, then
// the function will return 0.

function ReturnIndex(myarray, elementvalue)

	var i;
	for (i := 1; i <= len(myarray); i := i + 1)
		if (myarray[i] == elementvalue)
			return i;
		endif
	endfor

	return 0;

endfunction

// Fix for some string bugs. Pass the string and
// the value to set it to. If no value is passed,
// it sets value to "". This prevents it from being
// locked in instances such as the struct and dict bug where
// once they get <uninitialized object> they can no longer be
// written to.
//
// *Shouldnt be necessary anymore with pol 095.
//
function SetString(byref the_string, value:=0)
	if ( !value )
		the_string := "";
	else
		the_string := CStr(value);
	endif
endfunction

// Makes the string lowercase, then capitalizes the first letter.
function UCFirst(text)
	text := CStr(text);
	if ( len(text) > 1 )
		text := lower(text);
		text[1] := upper(text[1]);
	endif
	return text;
endfunction

// Removes spaces from a string based on the flags passed.
function RemoveSpaces(text, flags:=CLR_LEADING_SPACES)
	var pre := flags & CLR_LEADING_SPACES;
	var post := flags & CLR_TRAILING_SPACES;
	var dbl := flags & CLR_DOUBLE_SPACES;
	var all := flags & CLR_ALL_SPACES;
	
	if ( all )
		while ( text[" "] )
			text[" "] := "";
			sleepms(2);
		endwhile
	else
		if ( pre )
			while ( text[1] == " " )
				text[1] := "";
				sleepms(2);
			endwhile
		endif
		if ( post )
			while ( text[len(text)] == " " )
				text[len(text)] := "";
				sleepms(2);
			endwhile
		endif
	
		if ( dbl )
			while ( text["  "] )
				text["  "] := " ";
				sleepms(2);
			endwhile
		endif
	endif
	return text;
endfunction

function ArrayAppend(x, y)

	var xlen := len(x);
	var i := 0;
	for (i := 1; i <= len(y); i := i + 1)
		xlen := xlen + 1;
		x[xlen] := y[i];
	endfor

	return x;

endfunction


////////////////////////////////////////////////////
//
//  Function SearchStr
//
//  Parameters:
//    strng - string to search
//    substring - information to search for
//
//  Return value:
//    position of <substring> within <strng>
//          or
//    0 - If not found
//
///////////////////////////////////////////////////////

function SearchStr(strng, substring)
var sublen := Len(substring);
var fragment;
	if(Len(strng) >= sublen)
		for i := 1 to Len(strng)
//		for i := 1 to Len(strng)-Len(substring)
			fragment := SubStr(strng, i, sublen);
			if(fragment == substring)
				return i;
			endif
		endfor
	endif
	return 0;
endfunction


//////////////////////////////////////////////////////////////////////
//
//  Purpose:  To return a string representation
//            of an integer formatted with commas
//            at three digit intervals.
//
//  AddCommas(number)
//
//  Input:    An integer in the range 0 to 2147483647 decimal
//            or 0x0 to 0x7FFFFFFF hex.
//
//  Returns   A string representaton of a decimal integer with
//            commas inserted appropriately. Example: 2,147,483,647
//            or -1 if the integer exceeds 2147483647 decimal.
//
//////////////////////////////////////////////////////////////////////
function AddCommas(number)

  var strng := CStr(number);
  var newstring := "";
  var tempstring := "";
  var modstrlen := len(strng) %3;
  var setsof3digits := 1;
  if(len(strng) < 4)
    return strng;
  endif
  if(modstrlen)
    newstring := strng[1, modstrlen] + ",";
//  else
//    modstrlen := modstrlen + 2;
  endif
  setsof3digits := CInt((Len(strng) - modstrlen) / 3);
  tempstring := strng[modstrlen + 1, Len(strng)];
  for i := 1 to setsof3digits
    
    if(Len(tempstring) > 3)
      newstring := newstring + tempstring[1,3] + ",";
      tempstring := tempstring[4, Len(tempstring)];
    else
      newstring := newstring + tempstring[1,3];
    endif

  endfor
  
  
  return newstring;
endfunction
