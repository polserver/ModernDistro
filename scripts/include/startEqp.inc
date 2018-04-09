use cfgfile;
use os;
use util;
include ":attributes:attributes";
//
//  See starteqp.cfg for a description of what we expect in the config file.
//

var unique_items_created := {};
var starteqp_cfg;
;
var template, ammotype, ammoamount, theammo;
var magicspawnchance := 0;
var magicadjustment  := 0;
var making_starting_equipment := 0;

var thecritter;

function MakeLoot(corpse, lootgroup)
  var npcdesc := ReadConfigFile("::npcdesc");
  var elem := FindConfigElem(npcdesc, corpse.npctemplate);
  var mybow := corpse.npctemplate.missileweapon;
  var mprop, madj, has_backpack;
  if(elem)
    mprop := CInt(GetConfigInt(elem, "Magicitemchance"));
    madj  := CInt(GetConfigInt(elem, "MagicAdjustment"));
  else
    mprop := 1;
    if(corpse.objtype == 0x1A340)
      case(CInt(GetObjProperty(corpse,"level")))
        1: madj := 0;
        2: madj := 2;
        3: madj := 4;
        4: madj := 6;
        5: madj := 8;
      endcase
    else
      magicadjustment := 0;
    endif
  endif
  if(GetHp(corpse))
    has_backpack := GetEquipmentByLayer(corpse, 0x15);
    if (!has_backpack)
        var newbackpack := CreateItemAtLocation(corpse.x, corpse.y, corpse.z, 0xe75,1, corpse.realm);
        if (!EquipItem(corpse, newbackpack))
          return 0;
        endif
    endif

    foreach thing in ListEquippedItems(corpse)
      if(thing.objtype == 0xe75)
        corpse := thing;
        break;
      endif
    endforeach
  endif
  thecritter := corpse;
  if(mprop)
    magicspawnchance := 1;
  else
    if((corpse.objtype == 0x1A340) || (corpse.objtype == 0xe40))
      magicspawnchance := 1;
    else
      magicspawnchance := 0;
    endif
  endif
  magicadjustment  := madj;
  starteqp_cfg := ReadConfigFile("::nlootgroup");
  if(lootgroup)
    CreateFromGroup(corpse, lootgroup);
  else
    var lootgroups := GetConfigStringArray(elem, "lootgroup");
    foreach str in lootgroups
      CreateFromGroup(corpse, CInt(str));
    endforeach
  endif
  
    if (corpse.npctemplate.ammotype)
        ammotype := corpse.npctemplate.ammotype;
        if (ammotype)
                EquipFromTemplate(corpse, mybow);
                ammoamount := corpse.npctemplate.ammoamount;
            if (!ammoamount)
                ammoamount := 30;
            endif
            theammo := CreateItemInBackpack(corpse, ammotype, ammoamount);
        endif
    endif

endfunction

function CreateStartingEquipment(who, skillids)
  making_starting_equipment := 1;
  starteqp_cfg := ReadConfigFile("starteqp");
  foreach skillid in skillids
    var skillname := GetSkillName(CInt(skillid));
    CreateFromGroup(who.backpack, skillname);
  endforeach
  CreateFromGroup(who.backpack, "all");
endfunction

function CreateFromGroup(who, group)

  if(making_starting_equipment != 1)
	starteqp_cfg := ReadConfigFile("::nlootgroup");
  endif
  var elem := starteqp_cfg[group];
  foreach str in GetConfigStringArray(elem, "Stack")
    CreateFromStackString(who, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Item")
    CreateFromItemString(who, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Random")
    CreateFromRandomString(who, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Group")
    CreateFromGroup(who, str);
  endforeach
  foreach str in GetConfigStringArray(elem, "Unique")
    CreateFromUniqueString(who, str);
  endforeach
endfunction

function GetSkillName(skillid)
  var cfg := ReadConfigFile(":attributes:uoskills");
  return cfg[skillid].attribute;
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  An 'Item' string can have one of the following forms:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Item [ObjTypeName]"                    "Item Mortar"
//  "Item [Count] [ObjTypeName]"            "Item 4 EmptyBottle"
//  "Item [ObjTypeName] [Colornum]"         "Item Robe 0xE9"
//  "Item [Count] [ObjTypeName] [Colornum]"  "Item 2 Robe 0xE9"
//  "item [ObjTypeName] [Colornum] [chancetospawn]  "item Longsword 0 100"
//  "Item [Count] [ObjTypeName] [Colornum] [chancetospawn]  "item 1 Longsword 0 100"
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromItemString(who, str)
  var params := SplitWords(str);
  var count, objname, color, chance;
  var couleurs := ReadConfigFile("::colors");
  count := RandomDiceRoll(CStr(params[1]));
  params[1] := params[2];
  params[2] := params[3];
  params[3] := params[4];
  if (!count)
    count := 1;
  endif
  objname := params[1];
  if(CInt(params[2]))
    color := CInt(params[2]);
  elseif (params[2])
    color := 0;
    var colorg := FindConfigElem(couleurs, params[2]);
    if(colorg)
      var str := GetConfigStringArray(colorg, "color");
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
  if (!color)
    color := 0;
  endif
  if(CInt(params[3]))
    chance := CInt(params[3]);
  else
    chance := 100;
  endif
  if(CInt(RandomInt(100)+1) <= CInt(chance))
    var i, canbemagic;
    for(i:=1; i<=CInt(count); i:=i+1)
      if (CInt(objname))
        objname := CInt(objname);
      else
        Lower(objname);
      endif
      var item := CreateItemInContainer(who, objname);
        if(!item)
          syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
        endif
      item.color := color;
      if(making_starting_equipment)
        item.newbie := 1;
        continue;
      endif
      if( (item.graphic in array( 0xdf2, 0xdf3, 0xdf4, 0xdf5, 0x13f9 )) && (magicspawnchance == 1))
        ProcessWand(item);
      else
        if(magicspawnchance == 1)
          canbemagic := MagicAllowedandType(objname);
          case(canbemagic)
            1: WeaponTable(item);
            2: ArmorTable(item);
            3: ClothTable(item);
          endcase
        endif
      endif
    endfor
  endif
endfunction

function ProcessWand(item)
  var amt;
  case(RandomInt(10) + 1)
    1: amt :=  15;   SetObjProperty(item, "SpellId",  1);   SetObjProperty(item, "damagemod", "of clumsiness");
    2: amt :=  15;   SetObjProperty(item, "SpellId",  3);   SetObjProperty(item, "damagemod", "of feeblemindedness");
    3: amt :=  15;   SetObjProperty(item, "SpellId",  4);   SetObjProperty(item, "damagemod", "of healing");
    4: amt :=  15;   SetObjProperty(item, "SpellId",  8);   SetObjProperty(item, "damagemod", "of weakness");
    5: amt :=  10;   SetObjProperty(item, "SpellId", 12);   SetObjProperty(item, "damagemod", "of harming");
    6: amt :=   5;   SetObjProperty(item, "SpellId", 18);   SetObjProperty(item, "damagemod", "of fireballs");
    7: amt :=   5;   SetObjProperty(item, "SpellId", 29);   SetObjProperty(item, "damagemod", "of greater healing");
    8: amt :=   3;   SetObjProperty(item, "SpellId", 30);   SetObjProperty(item, "damagemod", "of lightning");
    9: amt :=  10;   SetObjProperty(item, "SpellId", 31);   SetObjProperty(item, "damagemod", "of mana draining");
   10: amt := 100;   SetObjProperty(item, "SpellId", 65);   SetObjProperty(item, "damagemod", "of identification");
  endcase
  SetObjProperty(item, "charges", (amt + RandomInt(10)));
  item.usescript := ":wands:magicWands";
  var dsc;
  if(item.objtype == 0x13f9)
    dsc := "gnarled staff";
  else
    dsc := "wand";
  endif
  SetName(item, "a magic " + dsc);
endfunction

function ClothTable(item)
  var val := RandomInt(19) + 1;
  if(val >= 14)
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "sight");
    SetObjProperty(item, "damagemod", " of nightsight");
  elseif(val >= 11)
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "protection");
    SetObjProperty(item, "damagemod", " of protection");
  elseif(val <= 8)
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "strength");
    SetObjProperty(item, "damagemod", " of strength");
  elseif(val > 5)
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "invisibility");
    SetObjProperty(item, "damagemod", " of invisibility");
  elseif(val == 3)
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "blessing");
    SetObjProperty(item, "damagemod", " of blessing");
  else
    var charges := RandomDiceRoll("1d30") + 10;
    SetObjProperty(item, "charges", charges);
    SetObjProperty(item, "type", "reflect");
    SetObjProperty(item, "damagemod", " of magic reflection");
  endif
  AddName(item);
endfunction

function ArmorTable(item)
  var newarray := {1, 2};
  var count := RandomInt(99) + 1;
  var counter;
  if(count >= 40)
    counter := 1;
  else
    counter := 2;
  endif
  count := 0;
  var num;
  while(count <= counter)
    num := RandomInt(99) + 1;
    if(num >=  50)
      num := 1;
    else
      num := 2;
    endif
    case(num)
      1: ApplyHPMod(item);
      2: ApplyARMod(item);
    endcase
    newarray.erase(num);
    count := count + 1;
  endwhile
  if((RandomInt(10)+1) <= 1)
    ExtraArmorTable(item);
  endif
  AddName(item);
endfunction

function ExtraArmorTable(item)
  var val := RandomInt(4) + 1;
  case(val)
    1: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "protection");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and protection";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of protection";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    2: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "strength");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and strength";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of strength";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    3:  var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "blessing");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and blessing";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of blessing";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
    4: var charges := RandomDiceRoll("1d30") + 10;
       SetObjProperty(item, "charges", charges);
       SetObjProperty(item, "type", "reflect");
       var dmgmod := GetObjProperty(item, "damagemod");
       if(dmgmod)
         dmgmod := dmgmod + " and magic reflection";
         SetObjProperty(item, "damagemod", dmgmod);
       else
         dmgmod := " of magic reflection";
         SetObjProperty(item, "damagemod", dmgmod);
       endif
  endcase
endfunction

function WeaponTable(item)
  var newarray := array(1, 2, 3);
  var count := RandomInt(100) + 1;
  var counter;
  if(count >= 60)
    counter := 1;
  elseif(count >= 40)
    counter := 2;
  elseif(count >= 10)
    counter := 3;
  else
    counter := 4;
  endif
  if(counter == 1)
    DestroyItem(item);
    return;
  endif
  count := 0;
  var num, val;
  while(count <= counter)
    num := RandomInt(len(newarray)) + 1;
    val := newarray[num];
    case(val)
      1: ApplyAccyMod(item);
      2: ApplyHPMod(item);
      3: ApplyDmgMod(item);
    endcase
    newarray.erase(num);
    count := count + 1;
  endwhile
  AddName(item);
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Random' string follows the following form:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Random [Count] [GroupName]"            "Random 4 Reagents"
//  "Random [Count] [GroupName] [chancetospawn]"   "Random 4 Reagents 50"
//
//  'Count' entries from 'GroupName' are chosen randomly.  The same entry
//  will not be chosen twice.
//
/////////////////////////////////////////////////////////////////////////////
function CreateFromRandomString(who, desc)

  var params := SplitWords(desc);
  var amt := Cint(params[1]);
  var count := 1;
  if(amt != 1)
    count := RandomDiceRoll(Cstr(amt + 1));
  endif
  var group := params[2];
  var chance := params[4];
  var elem := FindConfigElem(starteqp_cfg, group);
  if(elem == error)
    syslog("Unable to find group " + group);
    return;
  endif
  var items := GetConfigStringArray(elem, "Item");
  if(items == error)
    syslog("Group " + group + " does not contain items!");
    return;
  endif
  if(!chance)
    chance := 100;
  endif
  var rand := RandomInt(99)+1;
  if(CInt(rand) <= CInt(chance))
    if(count > len(items))
      count := len(items);
    endif
    while(count)
      var n := RandomInt(len(items)) + 1;
      if(items[n])
        CreateFromItemString(who, items[n]);
        if(making_starting_equipment)
          items[n] := "";
        endif
        count := count - 1;
      endif
    endwhile
  endif
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Unique' string has the following form:
//
//  FORM                                    EXAMPLE
//  --------------------------------------------------------------------
//  "Unique [ObjTypeName]"                  "Unique Spellbook"
//
//  Only one of a unique item will be created as starting equipment.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromUniqueString(who, str)
  var params := SplitWords(str);
  var objtype;
  if (CInt(params[1]))
    objtype := params[1];
  else
    objtype := GetObjtypeByName(params[1]);
  endif
  if(!objtype)
    syslog("Don't know what a " + str + " is");
    return;
  endif
  if(objtype in unique_items_created)
    return;
  endif
  unique_items_created[len(unique_items_created) + 1] := objtype;
  var newstr := "1 " + str;
  CreateFromItemString(who, newstr);
endfunction

/////////////////////////////////////////////////////////////////////////////
//
//  A 'Stack' string is just like "Item" but stacks the item instead of making
//  count number of individual items.
//
/////////////////////////////////////////////////////////////////////////////

function CreateFromStackString(who, str)
  var params := SplitWords(str);
  var count;
  var objname;
  var color;
  var chance;
  count := RandomDiceRoll(CStr(params[1]));
  params[1] := params[2];
  params[2] := params[3];
  params[3] := params[4];
  if (!count)
    count := 1;
  endif
  objname := params[1];
  objname := Lower(objname);
  if(CInt(params[2]))
    color := CInt(params[2]);
  else
    color := 0;
  endif
  if (CInt(params[3]))
    chance := CInt(params[3]);
  else
    chance := 100;
  endif
  var rand := RandomInt(100)+1;
  if(CInt(rand) <= CInt(chance))
    if (CInt(objname))
      objname := CInt(objname);
    else
      Lower(objname);
    endif
    var item := CreateItemInContainer(who, objname, count);
    if(!item)
      syslog("Unable to create " + objname + " on " + who.name + ". Error= " + item.errortext);
    endif
    item.color := color;
    if(making_starting_equipment)
      item.newbie := 1;
    endif
  endif
endfunction

////
//Checks to see if that item is allowed to be magic and returns the type
////
function MagicAllowedandType(objname)
  var magicweapcfg := ReadConfigFile(":combat:magicweaps");
  var magicarmorcfg := ReadConfigFile(":combat:magicarmor");
  var magicclothcfg := ReadConfigFile(":combat:magiccloth");
  var elem1 := FindConfigElem(magicweapcfg, "1");
  var data1 := GetConfigInt(elem1, objname);
  if(data1)
    return 1;
  endif
  var elem2 := FindConfigElem(magicarmorcfg, "1");
  var data2 := GetConfigInt(elem2, objname);
  if(data2)
    return 2;
  endif
  var elem3 := FindConfigElem(magicclothcfg, "1");
  var data3 := GetConfigInt(elem3, objname);
  if(data3)
    return 3;
  endif
  return 0;
endfunction

////
//Picks and assigns a damagemod and a suffix for the item
////
function ApplyDmgMod(item)
  var numb := ((RandomInt(100)+1) - magicadjustment);
  if(numb < 1)
    numb := 1;
  endif
  if(numb >= 50)
    item.dmg_mod := 1;
    SetObjProperty(item, "damagemod", "of ruin");
  elseif(numb >= 25)
    item.dmg_mod := 3;
    SetObjProperty(item, "damagemod", "of might");
  elseif(numb >= 15)
    item.dmg_mod := 5;
    SetObjProperty(item, "damagemod", "of force");
  elseif(numb >= 5)
    item.dmg_mod := 7;
    SetObjProperty(item, "damagemod", "of power");
  elseif(numb >= 1)
    item.dmg_mod := 9;
    SetObjProperty(item, "damagemod", "of vanquishing");
  endif
  SilverMod(item);
endfunction

function ApplyAccyMod(item)
  var numb := ((RandomInt(100)+1) - magicadjustment);
  if(numb < 1)
    numb := 1;
  endif
  var modifier := 0;
  if(numb >= 50)
    modifier := 5;
    SetObjProperty(item, "accuracy", "accurate");
  elseif(numb >= 25)
    modifier := 10;
    SetObjProperty(item, "accuracy", "surpassingly accurate");
  elseif(numb >= 15)
    modifier := 15;
    SetObjProperty(item, "accuracy", "eminently accurate");
  elseif(numb >= 5)
    modifier := 20;
    SetObjProperty(item, "accuracy", "exceedingly accurate");
  elseif(numb >= 1)
    modifier := 25;
    SetObjProperty(item, "accuracy", "supremely accurate");
  endif
  SetObjProperty(item, "type", "accuracy");
  SetObjProperty(item, "gain", modifier);
endfunction

function ApplyHPMod(item)
  var numb := ((RandomInt(100)+1) - magicadjustment);
  if(numb < 1)
    numb := 1;
  endif
  if(numb >= 50)
    item.maxhp_mod := item.maxhp_mod + 05;
    SetObjProperty(item, "durability", "durable");
  elseif(numb >= 25)
    item.maxhp_mod := item.maxhp_mod + 10;
    SetObjProperty(item, "durability", "substantial");
  elseif(numb >= 15)
    item.maxhp_mod := item.maxhp_mod + 15;
    SetObjProperty(item, "durability", "massive");
  elseif(numb >= 5)
    item.maxhp_mod := item.maxhp_mod + 20;
    SetObjProperty(item, "durability", "fortified");
  elseif(numb >= 1)
    item.maxhp_mod := item.maxhp_mod + 25;
    SetObjProperty(item, "durability", "indestructable");
  endif
  item.hp := item.maxhp;
endfunction

function ApplyARMod(item)
  var numb := ((RandomInt(100)+1) - magicadjustment);
  if(numb < 1)
    numb := 1;
  endif
  if(numb >= 50)
    item.ar_mod := 5;
    SetObjProperty(item, "damagemod", "of defense");
  elseif(numb >= 25)
    item.ar_mod := 10;
    SetObjProperty(item, "damagemod", "of guarding");
  elseif(numb >= 15)
    item.ar_mod := 15;
    SetObjProperty(item, "damagemod", "of hardening");
  elseif(numb >= 5)
    item.ar_mod := 20;
    SetObjProperty(item, "damagemod", "of fortification");
  elseif(numb >= 1)
    item.ar_mod := 25;
    SetObjProperty(item, "damagemod", "of invulnerability");
  endif
endfunction

function SilverMod(item)
  var numb := (RandomInt(10) + 1);
  if(numb == 1)
    var hits := GetObjProperty(item, "OnHit");
    if(!hits)
      hits := {};
    endif
    hits.append("silverHit");
    SetObjProperty(item, "OnHit", hits);
    SetObjProperty(item, "silvermod", 1);
  endif
endfunction

function AddName(item)
  var currdesc := item.desc;
  if(currdesc[1] == "a")
    var space := find(item.desc, " ", 1);
    var newname := currdesc[space+1,len(item.desc)];
    SetName(item,"a magic " + newname);
  else
    SetName(item, "magic " + currdesc);
  endif
endfunction


