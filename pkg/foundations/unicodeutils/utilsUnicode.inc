/**
 * Functions to ease Unicode text handling.
 *
 * @author   Andromeda UO
 * @author   Jouni Airaksinen, mintus@andromeda-uo.net
 * @url      http://www.andromeda-uo.net/
 * @version  0.5 (%G%)
 */

use unicode;

/**
 * Concats Unicode and normal text.
 *  
 * @param  strs       String or Array of Strings and/or Unicode strings (arrays).
 * @return            Concation of the strings in Unicode.
 */
function UC(strs) 
  var result := array {};
  if (TypeOf(strs) != "Array")
    result := CAscZ(CStr(strs));
  else
    foreach str in (strs)
      if (TypeOf(str) != "Array")
        str := CAscZ(CStr(str));
      endif
      result := result + str;     // POL 2003-06-19 or later required for this operation
/*      foreach elem in (str)
        result.append(elem); 
      endforeach */
    endforeach
  endif
  return result;
endfunction 


//
// Wrappers for easier use, note the different parameter order
//

function BroadcastU(uc_text, color := _DEFAULT_UCCOLOR, font:=_DEFAULT_UCFONT, langcode := "ENG")
  return BroadcastUC(UC(uc_text), langcode, font, color);
endfunction

function PrintTextAboveU(above_object, uc_text, color := _DEFAULT_UCCOLOR, font := _DEFAULT_UCFONT, langcode := "ENG")
  return PrintTextAboveUC(above_object, UC(uc_text), langcode, font, color);
endfunction

function PrintTextAbovePrivateU(above_object, uc_text, character, color := _DEFAULT_UCCOLOR, font := _DEFAULT_UCFONT, langcode := "ENG")
  return PrintTextAbovePrivateUC(above_object, UC(uc_text), langcode, character, font, color);
endfunction

function SendSysMessageU(character, uc_text, color := _DEFAULT_UCCOLOR, font := _DEFAULT_UCFONT, langcode := "ENG")
  return SendSysMessageUC(character, UC(uc_text), langcode, font, color);
endfunction

function RequestInputU(character, item, uc_prompt, langcode := "ENG") // item is a placeholder, pass anything
  return RequestInputUC(character, item, UC(uc_prompt), langcode);
endfunction

// overriding wrappers (edit uo.em and remove/comment the function prototypes these override)

function Broadcast(uc_text, font := _DEFAULT_UCFONT, color := _DEFAULT_UCCOLOR, langcode := "ENG")
  return BroadcastUC(UC(uc_text), langcode, font, color);
endfunction

function PrintTextAbove(above_object, uc_text, font := _DEFAULT_UCFONT, color := _DEFAULT_UCCOLOR, langcode := "ENG")
  return PrintTextAboveUC(above_object, UC(uc_text), langcode, font, color);
endfunction

function PrintTextAbovePrivate(above_object, uc_text, character, font := _DEFAULT_UCFONT, color := _DEFAULT_UCCOLOR, langcode := "ENG")
  return PrintTextAbovePrivateUC(above_object, UC(uc_text), langcode, character, font, color);
endfunction

function SendSysMessage(character, uc_text, font := _DEFAULT_UCFONT, color := _DEFAULT_UCCOLOR, langcode := "ENG")
  return SendSysMessageUC(character, UC(uc_text), langcode, font, color);
endfunction

/*
function RequestInput(character, item, uc_prompt, langcode := "ENG") // item is a placeholder, pass anything
  return RequestInputUC(character, item, UC(uc_prompt), langcode);
endfunction
*/