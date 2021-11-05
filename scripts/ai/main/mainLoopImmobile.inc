function main_AI_loop()
  var next_wander := ReadGameClock() + 10;
  var ev;
  var wanders := 120;
  EnableMainEvents();
  while(1)
    if(ReadGameClock() > next_wander)
      wanders := wanders +1;
      next_wander := ReadGameClock()+120;
      if(wanders > 60)
        wanders :=0;
        ev := sleepmode();
      endif
    else
      ev := os::wait_for_event(120);
    endif
    repeat
      if(ev)
        case(ev.type)
          SYSEVENT_ENTEREDAREA: if((!(ev.source).npctemplate) ||(((ev.source).script == "employed") ||((ev.source).script == "tamed")))
									if(CheckLineOfSight( me, ev.source ))
										fight(ev.source);
									endif
								endif
          SYSEVENT_ENGAGED:
          SYSEVENT_DAMAGED:     Fight(ev.source);
        endcase
      endif
    until(!(ev := os::wait_for_event(120)));
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction