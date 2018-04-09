/**
 * Functions to process strings in various ways.
 *
 * @author     Andromeda UO
 * @author     Jouni Airaksinen, mintus@andromeda-uo.net
 * @url        http://www.andromeda-uo.net/
 * @version    1.1 (%G%)
 * @copyright  (C) Copyright 2002-2003
 */

use uo;

// --------------------------------------------------------------------------
//
// Msg constants 
//


// --------------------------------------------------------------------------
//
// String functions
//

/**
 * This function inserts values from array into given format string. 
 * <p>
 * Example: var s := Format("Do you, %s, want to continue?", {who.name});
 *  
 * @param formatstr       String which can have multiple %s and they are 
 *                        replaced with the elements of strings in same order.
 * @param strings         Array of the values to be inserted.
 * @return                Resulting string value.
 */
function Format(formatstr, strings)
  var st := 1;
  foreach str in (strings)
    var p := find(formatstr, "%s", st);
    if (p)
      st := p + len(str);
      formatstr := CStr(formatstr[1 , p - 1]) + str + CStr(formatstr[p + 2, len(formatstr)]);
    else 
      return formatstr;
    endif
  endforeach
  return formatstr;
endfunction

/**
 * This function XOR encodes/decodes string with value given. If you XOR string
 * twice with same value you get the original string. Note, the string might 
 * result with unwanted control characters. You might want to use 
 * CAscZ(str) (and maybe Pack()) for it before storing.
 *  
 * @param str             String which will be XOR'ed
 * @param value           The 'seed' value to use.
 * @return                Resulting string value.
 */
function XorString(str, value)
  var str_asc := CAscZ(str);
  var I;
  for (I := 1; I < len(str_asc); I := I + 1)
    str_asc[I] := (str_asc[I] ^ value);
  endfor
  return CChrZ(str_asc);
endfunction


var NumbersArray := array { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };  // preinitialize the array
/**
 * removes numbers from a given string and returns the new string.
 *  
 * @param text            String from which all number characters
 *                        should be removed.
 * @return                Resulting string value.
 */ 
function RemoveNumbers(text)
  var I;
  var r := "";
  for (I := 1; I <= len(text); I := I + 1)
    if (!(text[I] in NumbersArray))
      r := r + text[I];
    endif
  endfor
  return r;
endfunction


/**
 * Parse delimited string to array of strings. This in a sense corresponds
 * the core SplitWords() function with the difference you can specify the
 * delimeter character (instead of hardcoded space).
 * Note that the function doesn't remove eg. trailing/leading spaces like
 * the SplitWords() does.
 *  
 * @param str             String which to parse into components.
 * @param delimiter       Character which is used to cut the string (defaults to " ").
 *                        Should be just one character.
 * @return                Resulting array of the components.
 */ 
function ParseDelimitedString(str, delimiter := " ")
/*  if (delimiter == " ")
    return SplitWords(str);
  endif*/
  var r := array {};
  var st := 1;
  var p := find(str, delimiter, st);
  while (p)
    r.append(str[st, p - 1]);
    st := p + 1;
    p := find(str, delimiter, st);
  endwhile
  r.append(str[st, len(str)]);
  return r;
endfunction

/**
 * Returns string of given character.
 * Does not do check validity of  char parameter. This for effiency.
 *  
 * @param length          Length of the resulting string.
 * @param char            Character which is used to create the string.
 * @return                Resulting string value.
 */ 
function StringOfChar(length, char)
  var R := char;
  if (length > 1)
    var I;
    for (I := 1; I < length; I + 1)
      R := R + char;
    endfor
  endif
  return R;
endfunction

/**
 * Pads string with given character (defaults to "0").
 *  
 * @param value           String to pad.
 * @param length          Length of the resulting string into which to pad.
 * @param padchar         Character which is used to padding. Defaults to "0".
 * @return                Resulting string value.
 */ 
function CPadStr(value, length, padchar := "0")
  value := CStr(value);
  if (len(value) < length)
    value := StringOfChar(length - len(value), padchar) + value;
  endif
  return value;
endfunction

/**
 * Trims leading commas and spaces and ending spaces from a string.
 * Useful eg. with title_prefix, title_suffix properties.
 *  
 * @param text            String "cleap up".
 * @return                Resulting string value.
 */ 
function CleanUp(text)
  if (text == error)
    return "";
  endif
  var I;
  var S := 1;
  // trim leading "," and " "
  for (I := 1; I <= len(text); I := I + 1) 
    if (!(text[I] in {",", " "}))
      break;
    endif
    S := S + 1;
  endfor
  text := text[S, len(text)];
  
  // trim ending " "
  var E := len(text);
  for (I := E; I > 0; I := I - 1)
    if (text[I] != " ")
      E := I;
      break;
    endif
  endfor
  return text[1, E];
endfunction

