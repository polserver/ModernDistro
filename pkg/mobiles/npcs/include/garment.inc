use cfgfile;
use os;
use util;

var couleurs;
var vetements;
var templt;

function Habille(who, categ)
  var group;
  couleurs := ReadConfigFile("::colors");
  vetements := ReadConfigFile("::cloths");
  templt := ReadConfigFile("::equip2");
  foreach equipped_item in ListEquippedItems(who)
    DestroyItem(equipped_item);
  endforeach
  if(couleurs && vetements && templt)
    group := FindConfigElem(templt, categ);
    if(group)
      var lastc := 0;
      foreach str in GetConfigStringArray(group, "Equip")
            lastc := EquipItemString(who, str, lastc);
      endforeach
      foreach str in GetConfigStringArray(group, "Armor")
        lastc := EquipItemString(who, str, lastc );
      endforeach
      foreach str in GetConfigStringArray(group, "Weapon")
        lastc := EquipItemString(who, str, lastc);
      endforeach
    endif
  endif
endfunction

function Clothe(who, categ)
  Habille(who, categ);
endfunction

function EquipItemString(who, str, lastcolor)
  var params := SplitWords(str);
  var objname;
  var color;
  var chance;
  var lastc := 0;
  objname := params[1];
  if(CInt(objname))
    objname := CInt(objname);
  endif
  if(CInt(params[2]))
    color := CInt(params[2]);
  else
    color := 0;
    if(params[2] == "lastcolor" || params[2] == "lastcolour")
      color := lastcolor;
    else
      var colorg := FindConfigElem(couleurs, params[2]);
      if(colorg)
        var str := GetConfigStringArray(colorg, "Color");
        var ar := {};
        var rndint := RandomInt(len(str)) + 1;
        ar := SplitWords(str[rndint]);
        if(len(ar) > 1)
          if(CInt(ar[2]) > 0)
            color := RandomInt(CInt(ar[2]) - CInt(ar[1]))+1;
          else
            color := CInt(str[RandomInt(len(str))+1]);
          endif
        else
          color := CInt(str[RandomInt(len(str))+1]);
        endif
        if(color == 1)
          color := 0;
        endif
      endif
    endif
  endif
  lastc := color;
  if(CInt(params[3]))
    chance := CInt( params[3] );
  else
    chance := 100;
  endif
  if(CInt(RandomInt(99)+1) <= CInt(chance))
    var item := CreateItemAtLocation(5837, 1001, 0, objname, 1, _DEFAULT_REALM);
    if(!item)
      var itemg := FindConfigElem(vetements, objname);
      if(itemg)
        var str := GetConfigStringArray(itemg, "Item");
        var newarray := {};
        var i := 1;
        var sex := "";
        foreach str3 in str
          var str2 := SplitWords(str3);
          sex := CStr(str2[2]);
          if(sex != error)
            if(sex == "m" || sex == "f")
              if(sex == "m" && who.graphic == 400)
                newarray[i] := str2[1];
                i := i + 1;
              elseif(sex == "f" && who.graphic == 401)
                newarray[i] := str2[1];
                i := i + 1;
              endif
            else
              newarray[i] := str2[1];
              i := i + 1;
            endif
          else
            newarray[i] := str2[1];
            i := i + 1;
          endif
        endforeach
        objname := CInt(newarray[RandomInt(len(newarray))+1]);
        item := CreateItemAtLocation(5837, 1001, 0, objname, 1, _DEFAULT_REALM);
      endif
    endif
    item.color := color;
    if(CInt(params[4]) == 1)
      MoveItemToContainer(item, who.backpack);
    else
      if(!EquipItem(who, item))
        MoveItemToContainer(item, who.backpack);
      endif
    endif
  endif
  return lastc;
endfunction
