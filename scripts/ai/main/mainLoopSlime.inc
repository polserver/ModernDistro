function main_AI_loop(next_wander := 0)
  var ev;
  var wanders := 60;
  EnableMainEvents();
  while(1)
    ev := os::wait_for_event(5);
    repeat
      case(ev.type)
        SYSEVENT_ENGAGED:
        SYSEVENT_ENTEREDAREA: if((!(ev.source).npctemplate) ||(((ev.source).script == "employed") ||((ev.source).script == "tamed")))
									if(CheckLineOfSight( me, ev.source ))
										RunAwayFrom( ev.source);
									endif
//                               RunAwayFrom( ev.source);
                              endif
        SYSEVENT_DAMAGED:     if(ev.source)
                                Fight( ev.source);
                              endif
      endcase
    until(!(ev := os::wait_for_event(1)));
    if(ReadGameClock() > next_wander)
      wander_around();
      next_wander := ReadGameClock()+1;
      wanders := wanders +1;
      if(wanders > 60)
        wanders :=0;
        ev := sleepmode();
      endif
    endif
 endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents( SYSEVENT_DAMAGED);
  EnableEvents( SYSEVENT_ENTEREDAREA + SYSEVENT_ENGAGED, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents( SYSEVENT_ENGAGED);
  DisableEvents( SYSEVENT_DAMAGED);
  DisableEvents( SYSEVENT_ENTEREDAREA);
  cleareventque();
endfunction