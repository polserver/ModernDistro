include ":attributes:attributes";
include ":timedScripts:timedScripts";

function process_combat_event(opponent)
  spellattack(opponent);
  if ( AP_GetVital(me, VITALID_LIFE) < (AP_GetStat(me, STRENGTH)/2))
    CastSpell(me,me,"greaterHeal");
    if (TS_GetTimerNames(me, "P").size() > 0)
      CastSpell(me, me, "cure");
    endif
  endif
  if ((AP_GetVital(me, VITALID_LIFE) * 100/AP_GetVitalMaximumValue(me, VITALID_LIFE)) < flee_point)
    EraseObjProperty(me,"#flees");
    flee(opponent);
  endif
endfunction

function spellattack(opponent)
  if (!GetObjProperty(me,"#lastbreath"))
    SetObjProperty(me,"#lastbreath",ReadGameClock() + 5);
  endif
  if (GetObjProperty(me,"#lastbreath") > ReadGameClock() )
    return;
  endif
  if (RandomInt(2)==1)
    sleep(1);
    cast_offensive_spell(me,opponent);
  endif
  SetObjProperty(me,"#lastbreath",ReadGameClock() + 5);
  foreach myfriend in ListMobilesNearLocation(me.x, me.y, me.z, HALT_THRESHOLD, me.realm)
    if (myfriend.npctemplate && myfriend.script != "tamed" )
      healerstuff(myfriend);
    endif
  endforeach
endfunction

function in_combat_event_loop(opponent, loops)
  spellattack(opponent);
  if ( loops > 50 )
    flee(opponent);
    return;
  endif
endfunction

function prepare_for_fight(unused opponent)
  if (GetObjProperty(me, "mr") != "1")
    CastSpell(me,me,"reflect");
  endif
  DisableMainEvents();
  EnableCombatEvents();
endfunction

function post_combat()
  DisableCombatEvents();
  EnableMainEvents();
  SetWarMode( 0 );
  SetOpponent( 0 );
  num_casts := 2;
  sleep(1);
  look_around();
endfunction

function healerstuff(mobile)
  if (!mobile.npctemplate || mobile.npctemplate["guard"] || mobile.script == "tamed")
    return;
  endif
    if( TS_GetTimerNames(me, "P").size() > 0 && CheckLineOfSight(me,mobile) )
    TurnToward(mobile);
    CastSpell(me,mobile,"cure");
  elseif( (AP_GetVital(mobile, VITALID_LIFE) < (AP_GetVitalMaximumValue(mobile, VITALID_LIFE)/2)) && (CheckLineOfSight(me,mobile))  )
    TurnToward(mobile);
    CastSpell(me,mobile,"greaterHeal");
  endif
endfunction

function process_flee_event(opponent)
  opponent := opponent; // stops ecompile complaining that it's not used
  return 0;
endfunction

