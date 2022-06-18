include ":attributes:attributes";
include ":timedScripts:poisonUtil";
include ":damage:damage";

function process_combat_event(opponent)
  if((AP_GetVital(me, VITALID_LIFE) < (AP_GetVitalMaximumValue(me, VITALID_LIFE) / 2)) && (RandomInt(8) >= 5))
    CastSpell(me, me, "greaterHeal");
  elseif((TS_GetTimerNames(me, "P").size() > 0) && (RandomInt(8) >= 5))
    CastSpell(me, me, "cure");
  else
    spellattack(opponent);
  endif
  if(((AP_GetVital(me, VITALID_LIFE) * 100) / AP_GetVitalMaximumValue(me, VITALID_LIFE)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function process_flee_event(opponent)
  if((AP_GetVital(me, VITALID_LIFE) < (AP_GetVitalMaximumValue(me, VITALID_LIFE) / 2)) && (RandomInt(8) >= 3))
    CastSpell(me, me, "greaterHeal");
    return 0;
  elseif((TS_GetTimerNames(me, "P").size() > 0) && (RandomInt(8) >= 3))
    CastSpell(me, me, "cure");
    return 0;
  else
    spellattack(opponent);
    return 0;
  endif
  if(((AP_GetVital(me, VITALID_LIFE) * 100) / AP_GetVitalMaximumValue(me, VITALID_LIFE)) >= flee_point)
    return 1;
  endif
endfunction

function spellattack(opponent)
  if(LAST_BREATH > ReadGameClock())
    return;
  endif
  if((CheckLineOfSight(me, opponent)) and (Distance(me, opponent) <= 15))
    if(RandomInt(4) == 1)
      PerformAction(me,0x0c);
      PlaySoundEffect(me, 0x16b);
      sleep(2);
      var base := Cint(((FLAME_STRENGTH * AP_GetVital(me, VITALID_LIFE)) / AP_GetVitalMaximumValue(me, VITALID_LIFE)) / 2);
      var dmg :=  RandomInt(base) + base;
      PlayMovingEffect(me, opponent, 0x36d4, 10, 1, 1);
      ApplyRawSpellDamageEX(opponent, dmg, DMG_FIRE, me, 7, "Flame Strike");
    else
	  cast_offensive_spell(me,opponent);
    endif
    LAST_BREATH := Cint(ReadGameClock() + (RandomInt(4) + 4));
  endif
endfunction

function in_combat_event_loop(opponent, loops)
  spellattack(opponent);
  if ( loops > 50 )
	flee(opponent);
	return;
  endif
endfunction

function prepare_for_fight(unused opponent)
  if (!CInt(GetObjProperty(me, "mr")))
    CastSpell(me,me,"reflect");
  endif
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  if ( AP_GetVital(me, VITALID_LIFE) < (AP_GetStat(me, STRENGTH)-10))
	  CastSpell(me,me,"greaterHeal");
  endif
  if (TS_GetTimerNames(me, "P").size() > 0)
    CastSpell(me, me, "cure");
    return 0;
  endif
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  summons := 1;
  sleep(1);
  look_around();
endfunction