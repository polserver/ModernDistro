include ":attributes:attributes";
function process_combat_event(opponent)
  do_damage(opponent);
  if (RandomInt(10) == 1)
    if ( (!CInt(GetObjProperty(me, "guardkill"))) && (!CInt(GetObjProperty(me,"killme"))) )
      var it := CreateNpcFromTemplate(me.npctemplate, me.x, me.y -1, me.z, 0, realm := me.realm);
      if (!it.errortext)
        PrintTextAbove(me, "The slime splits when struck!");
        SetObjProperty(it,"killme",1);
      endif
    endif
  endif
  if ((AP_GetVital(me, VITALID_LIFE) * 100/AP_GetVitalMaximumValue(me, VITALID_LIFE)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(unused opponent)
  return 0;
endfunction

function in_combat_event_loop(opponent, loops)
  do_damage(opponent);
  if ( loops > 50 )
    flee(opponent);
    return;
  endif
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  sleep(1);
  look_around();
endfunction

function prepare_for_fight(unused opponent)
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function do_damage(evsource)
  if ( (Distance(me, evsource) < 3 ) && (RandomInt(40) == 1) )
    PrintTextAbovePrivate(me, "The slime spits acid at you!", evsource);
    if (!HasShield(evsource))
      var dmg := RandomInt(5) + 1;
      var things := ListEquippedItems( evsource );
      var thing := things[ CInt(RandomInt(len(things))+1) ];
      if ( CanEat(evsource, thing) )
        PrintTextAbovePrivate(evsource, "The corrosive liquid damages "+ thing.desc, evsource);
        if (dmg > thing.hp)
          dmg := thing.hp;
        endif
        thing.hp := thing.hp - dmg;
        if ( (thing.hp <= 0) || (thing.hp > thing.maxhp) )
          PrintTextAbovePrivate(evsource, thing.desc + " is destroyed!", evsource);
          DestroyItem(thing);
        endif
      endif
    endif
  endif
endfunction

function CanEat(unused character, item)
  if ( ( item.objtype >= 0x2030 ) && (item.objtype <= 0x2060) )
    return 0;
  endif
  if ( (item.objtype == 0x0e75) || (item.objtype == 0x1F021 ) )
    return 0;
  endif
  if (!item.maxhp)
    return 0;
  endif
  return 1;
endfunction

function HasShield(you)
  var shield := GetEquipmentByLayer( you, LAYER_HAND2 );
  if ( (shield.objtype == 0x1bc3) || (shield.objtype == 0x1bc4) )
    PrintTextAbovePrivate(you,"Your shield blocks the acid!",you);
    return 1;
  endif
  return 0;
endfunction