include ":attributes:attributes";
function process_combat_event(opponent)
  if ((GetHp(me) * 100/GetMaxHp(me)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  opponent := opponent;
  return 0;
endfunction

function in_combat_event_loop(opponent, loops)
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

function prepare_for_fight(opponent)
  opponent := opponent;
  DisableMainEvents();
  EnableCombatEvents();
endfunction