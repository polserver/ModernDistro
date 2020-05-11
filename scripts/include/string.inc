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

// Constants for the RemoveSpaces() function.
const CLR_LEADING_SPACES 	:= 0x1;
const CLR_TRAILING_SPACES 	:= 0x2;
const CLR_DOUBLE_SPACES		:= 0x4;
const CLR_ALL_SPACES		:= 0x8;

// Constants for the RemoveFromString() function.
CONST STR_DEL_ONCE		:= 0x1;
CONST STR_DEL_ALL		:= 0x2;
CONST STR_DEL_LEADING		:= 0x4;
CONST STR_DEL_TRAILING		:= 0x8;

// Function TruncateArticle(str)
// This function will remove an indefinite article ("a" or "an")
// from a string "str", returning the rest of the string.  If there
// is no article, it returns the original string "str".
function TruncateArticle(str)

	str := CStr(str);
	var i := len(str);

	var an_articlelist := { "an ", "An ", "AN ", "aN " };
	var a_articlelist := { "a ", "A " };

	if ( str[1,3] in an_articlelist )
		str := (str[4,i-3]);
	elseif ( str[1,2] in a_articlelist )
		str := (str[3,i-2]);
	endif

	return str;

endfunction


// function AddArticle(str)
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

function AddArticle(str)

	str := CStr(str);
	var vowellist := { "a", "A", "e", "E", "i", "I", "o", "O", "u", "U" };
	var i := str[1];

	if ( i in vowellist )
		str := "an " + str;
	else
		str := "a " + str;
	endif

	return str;

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
		sleepms(2);
	endfor

	return MultiArray;

endfunction


// function PadString(str, stringlength)
// This function will pad a string "str" with enough spaces to reach
// a length indicated by "stringlength".  If the string is already
// longer than stringlength, the function will return 0.

function PadString(str, stringlength)

	var padding := stringlength - len(str);
	if (padding < 0)
		return 0;
	endif

	var padstr := "                    ";     // 20 spaces

	while(padding > 20)
		str := str + padstr;
		padding := padding - 20;
	endwhile

	if (padding > 0)
		str := str + padstr[1, padding];
	endif

	return str;

endfunction



// function PadArray(arry, arraylength)
// This function will pad an array "arry" with enough 0s to reach length
// specified in "arraylength".  If the length of the array is already
// greater than arraylength, the function will return 0.

function PadArray(arry, arraylength)

	var padding := arraylength - len(arry);

	if (padding < 0)
		return 0;
	elseif (padding == 0)
		return arry;
	endif

	var q := len(arry);
	var i;

	for (i := 1; i <= padding; i := i + 1)
		q := q + 1;
		arry[q] := 0;
	endfor

	return arry;

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

// Makes the string lowercase, then capitalizes the first letter.
function UCFirst(text)
	text := CStr(text);
	if ( len(text) > 1 )
		text := lower(text);
		text[1] := upper(text[1]);
	endif
	return text;
endfunction

// Makes the first letter of the string lowercase.
function LCFirst(text)
	text := CStr(text);

	if ( Len(text) > 1 )
		text[1] := Lower(text[1]);
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

///////////////////////////////////////////////////////////
//
// RemoveFromString( text, remove, flags:=STR_DEL_ALL )
//
// Purpose:
// Removes the value passed in "remove" from a string passed
// in "text" based on flags passed.
// Flags are: STR_DEL_ONCE
//            STR_DEL_ALL
//            STR_DEL_LEADING
//            STR_DEL_TRAILING
//
// Returns the modified string.
//
///////////////////////////////////////////////////////////
function RemoveFromString( text, remove, flags:=STR_DEL_ALL )
	text := CStr(text);
	remove := CStr(remove);

	if ( flags == STR_DEL_ALL )
		// Remove all instances of 'remove'
		while ( text[remove] )
			text[remove] := "";
			SleepMs(2);
		endwhile
	elseif ( flags == STR_DEL_ONCE )
		// Only remove first instance of 'remove'
		// Really its just better to do text[replace] := "" in a script
		// but this is here since it is used for leading/trailing removes.
		text[remove] := "";
	else
		var remove_len := Len(remove);
		if ( flags & STR_DEL_LEADING )
			// Remove from the begining of 'text' only.
			while ( text[1, remove_len] == remove )
				text[1, remove_len] := "";
				SleepMs(2);
				if ( flags & STR_DEL_ONCE )
					// Remove only first instance of 'remove' from the begining.
					break;
				endif
			endwhile
		endif

		if ( flags & STR_DEL_TRAILING )
			// Remove from the end of 'text' only.
			var end_start := (Len(text) - remove_len)+1;
			while ( text[end_start, Len(text)] == remove )
				text[end_start, Len(text)] := "";
				SleepMs(2);
				if ( flags & STR_DEL_ONCE )
					// Remove only first instance of 'remove' from the ending.
					break;
				endif
				end_start := (Len(text) - remove_len)+1;
			endwhile
		endif
	endif

	return text;
endfunction

///////////////////////////////////////////////////////////
//
// AppendArray(arry1, arry2)
//
// Purpose:
// Append Arry2 to the end of Arry1.
//
// Returns Arry1 with Array2 appended.
//
///////////////////////////////////////////////////////////
function AppendArray(arry1, arry2)

	var xlen := len(arry1);
	var i := 0;
	for (i := 1; i <= len(arry2); i := i + 1)
		xlen := xlen + 1;
		arry1[xlen] := arry2[i];
	endfor

	return arry1;

endfunction


function Check_ValidChar(text, allow_space:=0)

         //Make sure you check a text without spaces or it will fail.
         var valid_char := array{ "a", "b", "c", "d", "e", "f", "g",
                                  "h", "i", "j", "k", "l", "m", "n",
                                  "o", "p", "q", "r", "s", "t", "u",
                                  "v", "w", "x", "y", "z", "A", "B",
                                  "C", "D", "E", "F", "G", "H", "I",
                                  "P", "O", "N", "M", "L", "K", "J",
                                  "Q", "R", "S", "T", "U", "V", "W",
                                  "X", "Y", "Z", "1", "2", "3", "4",
                                  "5", "6", "7", "8", "9", "0" };

         var len_text := Len( text );

         var i;
         for( i:=1; i<=len_text; i+=1 )
                  SleepMS(5);
                  var letter := CStr( text[i] );
                  if( letter in valid_char )
                           continue;
                  elseif( allow_space && ( text[i] == " " || CAsc( text[i] ) == 0 ))
                           continue;
                  endif

                  return 0;
         endfor

         return 1;
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
//            commas inserted appropriately. Example: "2,147,483,647"
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

///////////////////////////////////////////
// SortArrayABC - Sorted as array alphabetically
// arr := array(); dir := 1 ascending, 0 := descending,
// subindex := Which Subarray/substructing element to sort by
///////////////////////////////////////////
Function SortArrayABC(arr, dir := 1, subindex := 0)
  Var dict := dictionary, key;

  // Build Dict: Sortierkey-> {Numbers in Array}
  ForEach element in arr
    If (!subindex)
      key := element;
    Else
      key := element[subindex];
    EndIf
    If (!dict.exists(key))
      dict[key] := array{};
    EndIf
    dict[key].append(_element_iter);
    SleepMS(2);
  EndForEach

  // Keys Sorting accordingly
  var newarray := array{};
  var sorted := dict.keys();
  sorted.sort();
  If (dir == 0)
    sorted.reverse();
  EndIf

  ForEach key in (sorted)
    ForEach element in (dict[key])
      newarray.append(arr[element]);
      SleepMS(2);
    EndForEach
  EndForEach

  return newarray;
EndFunction

//////////////////////////////////////////////////////
//
// StripLeadingZero(str)
// Purpose:
// Strips leading zeros from a string except for "0x".
// This is needed because POL assumes a leading zero
// on a string representation of an integer is octal
// notation. If you send such a string to the CInt()
// function, you will get back an integer but it
// will be the base 10 conversion of an octal value.
//
// Parameters:
// str - The string from which to strip leading zeros.
//
// Returns:
// A string representation of a hexadecimal integer
// if the first 2 characters were "0x" or a string
// representation of a decimal number with leading
// zeros, if any, removed or an error if the string
// contains any characters that are not valid integer
// digits.
//
// Note: Validation is performed for valid hexadecimal
// and decimal string representations of integers.
//
//////////////////////////////////////////////////////
function ValidateStringInteger(str)

	var hex_numbers := {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
						"A", "a", "B", "b", "C", "c", "D", "d", "E", "e", "F", "f"};

	var dec_numbers := {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
	if(str[1,2] == "0x")
		for x := 1 to str.length()
			if(str[x,x] in hex_numbers)
				return str;
			else
				return error {"errortext" := "Not a valid string representation of an integer."};
			endif
		endfor
		return str;
	else
		str := Trim(str, TRIM_LEFT, "0");
		for x := 1 to str.length()
			if(str[x,x] in dec_numbers)
				// str := Trim(str, TRIM_LEFT, "0");
				return str;
			else
				return error {"errortext" := "Not a valid string representation of an integer."};
			endif
		endfor
	endif

endfunction

