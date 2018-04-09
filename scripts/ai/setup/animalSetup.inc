var me := Self();
var flee_point := 10;
drop_anchor();

  if (GetObjProperty(me, "frozen"))
        me.frozen := 1;
  endif

start_script("NPCKeeper", me);