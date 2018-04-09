include ":attributes:attributes";
include ":damage:damage";

function process_combat_event(opponent)
  spellattack(opponent);
  if((GetHp(me) * 100/GetMaxHp(me)) < flee_point)
	EraseObjProperty(me,"#flees");
	flee(opponent);
  endif
endfunction

function spellattack(opponent);
  if(RandomInt(3) == 1)
	return;
  endif
  if(CheckLineOfSight(me, opponent))
    PerformAction(me,0x0c);
    var dmg := ((RandomInt(9) + 1) + (GetHp(me) / 10));
    PlaySoundEffect(me, 0x16b);
    sleep(2);
    PlayMovingEffect(me, opponent, 0x36d4, 10, 1, 1);
    ApplyRawDamageEX(opponent, dmg, DMG_FIRE, 0);
  endif
endfunction

function in_combat_event_loop(opponent, loops)
  spellattack(opponent);
  if(loops > 50)
	flee(opponent);
	return;
  endif
endfunction

function prepare_for_fight(opponent)
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  sleep(1);
  look_around();
endfunction
