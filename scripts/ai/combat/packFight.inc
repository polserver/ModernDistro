

include ":attributes:attributes";
include ":damage:damage";

function Fight( opponent )
  if ((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
    SetWarMode(0);
    opponent := 0;
    return;
  endif
  if (opponent.multi.serial)
    if (me.multi.serial != opponent.multi.serial)
      flee(opponent);
      return;
    endif
  endif
  set_priority(50);
  SetOpponent( opponent );
  prepare_for_fight( opponent );
  TurnToward( opponent );
  var loops := 0;
  var ev;
  var waittime := 0;
  while ( (opponent) && not (opponent.dead || opponent.hidden || opponent.concealed) && (Distance(me,opponent) < 20) )
    if (!CloseDistance( opponent ) )
      loops := loops + 1;
      waittime := 0;
    else
      loops := 0;
      waittime := 1;
    endif
    in_combat_event_loop(opponent, loops);
    ev := wait_for_event( waittime );
    case (ev.type)
      SYSEVENT_DAMAGED:    foreach mobile in ListMobilesInLineOfSight(me, 10);
                             if(mobile.npctemplate == me.npctemplate)
                               var evnt := array;
                               evnt.+ type;
                               evnt.+ source;
                               evnt.source := ev.source;
                               evnt.type := EVID_ALL_ATTACK_CMD;
                               SendEvent(mobile, evnt);
                             endif
                           endforeach
                           opponent := ev.source;
                           SetOpponent( opponent );
      EVID_ALL_ATTACK_CMD: if((RandomInt(4) + 1) >= 3)
                             Fight(ev.source);
                           endif
      SYSEVENT_ENGAGED:    if (ev.source)
                             foreach mobile in ListMobilesInLineOfSight(me, 10);
                               if(mobile.npctemplate == me.npctemplate)
                                 var evnt := array;
                                 evnt.+ type;
                                 evnt.+ source;
                                 evnt.source := ev.source;
                                 evnt.type := EVID_ALL_ATTACK_CMD;
                                 SendEvent(mobile, evnt);
                               endif
                             endforeach
                             process_combat_event(opponent);
                             if (RandomInt(6)==1)
                               if ((!ev.source.npctemplate) || (ev.source.script == "tamed") || (ev.source.script == "employed"))
                                 opponent := ev.source;
                                 SetOpponent( opponent );
                                 TurnToward( opponent );
                               endif
                             endif
                           endif
      EVID_PEACEMADE:      SetWarMode( 0 );
                           SetOpponent( 0 );
                           sleep(1);
                           DisableCombatEvents();
                           EnableMainEvents();
                           return;
      EVID_HERDING:           Herd(ev);
    endcase
  endwhile
  post_combat();
endfunction

function EnableCombatEvents()
  EnableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE );
endfunction

function DisableCombatEvents()
  DisableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE );
endfunction

function flee(opponent)
  if ( (me.script == "immobile") || (me.script == "immobilespell") )
    sleep(2);
    return;
  endif
  var numflees := GetObjProperty(me,"#flees");
  if (numflees)
    if ( numflees > 10 )
      MoveObjectToLocation(me,5288,1176,0,"britannia_alt",MOVEOBJECT_FORCELOCATION);
      SetObjProperty(me,"guardkill",1);
      KillMobile(me);
    else
      numflees := numflees + 1;
    endif
  else
    numflees := 1;
  endif
  SetObjProperty(me,"#flees",numflees);
  var runs := 0;
  while ( (Distance(me, opponent) < 20) && (runs < 50) )
    RunAwayFrom(opponent);
    runs := runs +1;
    wait_for_event(0);
  endwhile
  RestartScript( me );
endfunction