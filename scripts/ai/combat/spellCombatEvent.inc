include ":attributes:attributes";
include ":poisonwatcher:poisons";

function process_combat_event(opponent)
  if((GetHp(me) < (GetMaxHP(me) / 2)) && (RandomInt(8) >= 5))
    CastSpell(me, me, "greaterHeal");
  elseif((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 5))
    CastSpell(me, me, "cure");
  else
    spellattack(opponent);
  endif
  if(((GetHp(me) * 100) / GetMaxHp(me)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  if((GetHp(me) < (GetMaxHP(me) / 2)) && (RandomInt(8) >= 3))
    CastSpell(me, me, "greaterHeal");
    return 0;
  elseif((ListPoisonsByType(me, "defaultPoison").size() > 0) && (RandomInt(8) >= 3))
    CastSpell(me, me, "cure");
    return 0;
  else
    spellattack(opponent);
  endif
  if(((GetHp(me) * 100) / GetMaxHp(me)) >= flee_point)
    return 1;
  endif
endfunction

function spellattack(opponent)
  if(LAST_BREATH > ReadGameClock())
    return;
  elseif((CheckLineOfSight(me, opponent)) and (dist(me, opponent) <= 12))
    cast_offensive_spell(me, opponent);
    LAST_BREATH := Cint(ReadGameClock() + (RandomInt(4) + 4));
  endif
endfunction

function prepare_for_fight(opponent)
  opponent := opponent; // stops "var not used" ecompile warning
  if((!CInt(GetObjProperty(me, "mr"))) and (RandomInt(3) == 2))
    CastSpell(me, me, "reflect");
  endif
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  if((GetHp(me) < (GetMaxHP(me) / 2)) && (RandomInt(4) >= 2))
    CastSpell(me, me, "greaterHeal");
  endif
  if(ListPoisonsByType(me, "defaultPoison").size() > 0)
    CastSpell(me, me, "cure");
  endif
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode(0);
  SetOpponent(0);
  sleep(1);
  look_around();
endfunction