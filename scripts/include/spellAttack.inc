function send_attack(ttarget, source, spellid := 0)
  var ev := array;
  ev.+type;
  ev.+source;
  ev.type := 0x0002;
  ev.source := source;
  SendEvent(ttarget, ev);
  if (spellid)
    ev.+spell;
    ev.type := 0x0800;
    ev.spell := spellid;
    SendEvent(ttarget, ev);
  endif
endfunction