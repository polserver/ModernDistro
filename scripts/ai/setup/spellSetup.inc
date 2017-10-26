var speech;
var master;
var spells := array;
var me := Self();

var cast_pct;
var num_casts;
var count_casts;
var saywords := 1;
var summons := 3;

var will_cast;
var flee_point;

var pncfg := ReadConfigFile("npcdesc");
var speechelem := pncfg[me.npctemplate];
speech := GetConfigString(speechelem, "speech");
cast_pct := speechelem.cast_pct;
num_casts  := speechelem.num_casts;
count_casts  := speechelem.count_casts;
saywords := speechelem.saywords;
if(!cast_pct)
  cast_pct := 10;
endif
flee_point := speechelem.flee_point;
if(!flee_point)
  flee_point := 10;
endif
SortSpellList();
start_script("NPCKeeper", me);

function SortSpellList();
  var spell_list := GetConfigStringArray(speechelem, "spell");
  var ct := 0;
  foreach thing in spell_list
    var split := SplitWords(thing);
    var spell_name := split[1];
    var spell_freq := Cint(split[2]);
    if(!spell_freq)
      spell_freq := Cint((100 - ct) / len(spells));
    endif
    ct := ct + spell_freq;
    var holder := array;
    holder.append(spell_name);
    holder.append(ct);
    spells.append(holder);
  endforeach
//  print("spells: " + spells);
endfunction