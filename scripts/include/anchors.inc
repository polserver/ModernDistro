function drop_anchor()
  var cfg := ReadConfigFile("::npcdesc");
  var dstart := cfg[me.npctemplate].dstart;
  var psub := cfg[me.npctemplate].psub;
  if(!psub)
    psub := 20;
  endif
  if(dstart)
    SetAnchor(me.x, me.y, dstart, psub);
  endif
  var parms := { me.x, me.y, me.z };
  SetObjProperty(me, "Anchor", parms);
endfunction
