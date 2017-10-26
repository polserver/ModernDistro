include ":attributes:attributes";
function process_combat_event(opponent)
  DoPoison(opponent);
  if((GetHp(me) * 100/GetMaxHp(me)) < flee_point)
	EraseObjProperty(me,"#flees");
	flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  return 0;
endfunction

function in_combat_event_loop(opponent, loops)
  DoPoison(opponent);
  if(loops > 50)
	flee(opponent);
	return;
  endif
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode(0);
  SetOpponent(0);
  sleep(1);
  look_around();
endfunction

function prepare_for_fight(opponent)
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function DoPoison(you)
  SetObjProperty(me, "#lastbreath", ReadGameClock() + 10);
  if((Distance(me,you) < 2) && (RandomInt(4)==1))
	var p_level := CInt(GetMaxHp(me)/20);
	var poison_level := CInt(GetObjProperty(you, "poison_level"));
    if(p_level < 1)
      p_level := 1;
    endif
	if(p_level < poison_level)
	  p_level := poison_level;
	else
	  SetObjProperty(you, "poison_level",CInt(p_level));
	endif
	Detach();
	set_critical(0);
    SetObjProperty(you, "LastHit", {me.name, me.serial, "poison spell"});
	var passparms := {you, me, "snake venom"};
    SetObjProperty(you, "PoisonStamp", ReadGameClock());
	start_script(":spells:poisonDamage", passparms);
  endif
endfunction