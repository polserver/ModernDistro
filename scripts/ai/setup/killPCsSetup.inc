var me := Self();

var npccfg := ReadConfigFile("npcdesc");
var speechelem := npccfg[me.npctemplate];
var flee_point := speechelem.flee_point;
if(!flee_point)
  flee_point := 10;
endif
start_script("NPCKeeper", me);