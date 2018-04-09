
var me := Self();
var speech;
var ammotype;
var ammoamount;
var theammo;
var master;
var spells := {};

const SKILLID_HIDE := 21;

var cast_pct;
var num_casts;
var count_casts;
var saywords := 1;
var graphics;
var num_changes;
var will_cast;
var will_breathe;
var flee_point;

drop_anchor();

if( me.name["<random>"] )
  SetName( me, RandomName( me ) );
endif
var npccfg := ReadConfigFile("npcdesc");
var speechelem := npccfg[me.npctemplate];
speech := GetConfigString(speechelem, "speech");
spells := GetConfigStringArray( speechelem, "spell" );
cast_pct := speechelem.cast_pct;
num_casts  := speechelem.num_casts;
count_casts  := speechelem.count_casts;
var equipname := speechelem.equip;
saywords := speechelem.saywords;
if (!cast_pct)
  cast_pct := 10;
endif
flee_point := speechelem.flee_point;
if (!flee_point)
  flee_point := 10;
endif
if (equipname)
  EquipFromTemplate(me, equipname);
endif
ammotype := npccfg[me.npctemplate].ammotype;
var mybow := npccfg[me.npctemplate].missileweapon;
if (ammotype)
  if (me.backpack)
    DestroyItem(me.backpack);
  endif
  EquipFromTemplate(me, mybow);
  ammoamount := npccfg[me.npctemplate].ammoamount;
  theammo := CreateItemInBackpack(me, ammotype, ammoamount);
endif
if (npccfg[me.npctemplate].equip)
  EquipFromTemplate(me, npccfg[me.npctemplate].equip);
endif
if (lower(speechelem["alignment"])=="evil")
  foreach critter in ListMobilesInLineOfSight( me, 10)
    if (!critter.npctemplate || critter.script == "tamed")
      fight(critter);
    endif
  endforeach
endif
