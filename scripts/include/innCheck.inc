use uo;
use os;
use cfgfile;
use util;

function Inncheck(character)

  var element, rect;
  var cfgfile := ReadConfigFile("::innlocation");
  var entries := GetConfigStringKeys(cfgfile);
  foreach listing in entries
    element :=  cfgfile[listing];
    rect := element.rect;
    rect := SplitWords(rect);
    if((character.x >= CInt(rect[1])) && (character.x <= CInt(rect[3])) && (character.y >= CInt(rect[2])) && (character.y <= CInt(rect[4])))
      return 1;
      break;
    endif
  endforeach
  return 0;

endfunction