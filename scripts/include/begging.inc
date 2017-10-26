
function begpurse(me)
  var amt := Cint(GetObjProperty(me, "begpurse"));
  if(!amt)
    amt := 50;
  endif
  var cnfg := ReadConfigFile("::npcdesc");
  var wealth := cnfg[me.npctemplate].wealth;
  var cap := 50;
  case(wealth)
    "upper":  cap := 150;
    "middle": cap := 100;
    "poor":   cap := 50;
    "broke":  cap := 0;
  endcase
  if(amt < cap)
    amt := amt + 1;
    SetObjProperty(me, "begpurse", amt);
  endif
endfunction