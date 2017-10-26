function main_AI_loop(next_wander := 0)
  var ev;
  var wanders := 60;
  /*var next_loot := ReadGameClock() + 20;*/  // variable unused in this AI type
  EnableMainEvents();
  while(1)
    if(ReadGameClock() > next_wander)
      wander();
      next_wander := ReadGameClock()+3;
      wanders := wanders +1;
      if(wanders > 60)
        wanders :=0;
        ev := sleepmode();
      endif
    else
      ev := os::wait_for_event(5);
    endif
    if(ev)
      repeat
        case(ev.type)
          SYSEVENT_SPEECH:     if(!ev.source.isA(POLCLASS_NPC))
                                 wanders :=0;
                                 next_wander := ReadGameClock()+30;
                                 check_speech(ev.text, ev.source);
                               endif
          SYSEVENT_ENGAGED:
          SYSEVENT_DAMAGED:    Fight(ev.source);
          SYSEVENT_ITEM_GIVEN: next_wander := ReadGameClock()+60;
                               TakeItem(ev.source,ev.item);
        endcase
      until(!(ev := os::wait_for_event(5)));
    endif
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_SPEECH, 3);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, HALT_THRESHOLD);
  EnableEvents(SYSEVENT_ITEM_GIVEN);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
  DisableEvents(SYSEVENT_ITEM_GIVEN);
endfunction