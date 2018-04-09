var me := Self();
var equipt := GetObjProperty(me, "Equipt");

drop_anchor();
var npccfg := ReadConfigFile("npcdesc");
var speechelem := npccfg[me.npctemplate];
var speech := GetConfigString(speechelem, "speech");
var saywords := speechelem.saywords;
var flee_point := speechelem.flee_point;
if(!flee_point)
  flee_point := 10;
endif
start_script("NPCKeeper", me);
if(!me.backpack)
  var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1, me.realm);
  EquipItem(me, newbackpack);

  var myhome := { };
  myhome[1] := me.x;
  myhome[2] := me.y;
  myhome[3] := me.z;
  SetObjProperty(me,"myhome",myhome);
endif