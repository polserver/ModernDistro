include ":npcs:garment";

var speech;
var ammotype;
var ammoamount;
var theammo;

var master;
var spells := {};
var me := Self();

var cast_pct;
var num_casts;
var count_casts;
var saywords := 1;
var summons := 3;

var graphics;
var num_changes;
var will_cast;
var will_breathe;
var flee_point;

	drop_anchor();

var npccfg := ReadConfigFile("npcdesc");
var speechelem := npccfg[me.npctemplate];
speech := GetConfigString(speechelem, "speech");
spells := GetConfigStringArray( speechelem, "spell" );
cast_pct := speechelem.cast_pct;
num_casts  := speechelem.num_casts;
count_casts  := speechelem.count_casts;
var equipname := speechelem.equip;
saywords := speechelem.saywords;
if(!cast_pct)
  cast_pct := 10;
endif
flee_point := speechelem.flee_point;
if(!flee_point)
  flee_point := 10;
endif
start_script("NPCKeeper", me);