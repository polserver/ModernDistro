include ":attributes:attributes";
include ":damage:damage";

function process_combat_event(opponent)
  if(Distance(me,opponent) < 2)
    var dmg := RandomInt(CInt(AP_GetStat(opponent, INTELLIGENCE) / 10));
    if(dmg > 12)
      dmg := 12;
    elseif(dmg < 2)
      dmg := 2;
    endif
        ApplyRawDamageEX(opponent, dmg, DMG_ENERGY, 0);
  endif
endfunction

function prepare_for_fight(unused opponent)
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