function main_AI_loop()
  var ev;
  var wanders := 60;
  var next_wander := ReadGameClock();
  EnableMainEvents();
  while(1)
    if(ReadGameClock() > next_wander)
      SpoutTheNews();
      wanders := wanders +1;
      next_wander := ReadGameClock()+60;
      if(wanders > 60)
        wanders :=0;
        ev := sleepmode();
      endif
    else
      ev := os::wait_for_event(60);
    endif
    if(ev)
      repeat
        case(ev.type)
          SYSEVENT_SPEECH:  if(!ev.source.isA(POLCLASS_NPC))
                              if(lower(ev.text) == "news")
                                wanders :=0;
                                next_wander := ReadGameClock()+60;
                                SpoutTheNews();
                              endif
                            endif
          SYSEVENT_ENGAGED:
          SYSEVENT_DAMAGED: Fight(ev.source);
                            ReturnHome();
        endcase
      until(!(ev := os::wait_for_event(60)));
    endif
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  DisableEvents(SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, HALT_THRESHOLD);
  EnableEvents(SYSEVENT_SPEECH, 3);
  DisableEvents(SYSEVENT_ITEM_GIVEN);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
  DisableEvents(SYSEVENT_ITEM_GIVEN);
endfunction

function SpoutTheNews()
  var news := GetGlobalProperty("news");
  if(!news)
    say("No news is good news.");
    return;
  endif
  var i;
  for(i := 1; i <= len(news); i := i + 1)
    say(news[i]);
    sleep(2);
  endfor
endfunction

function ReturnHome()
  var myhome := GetObjProperty(me,"myhome");
  MoveObjectToLocation(me,myhome[1],myhome[2],myhome[3], me.realm, MOVEOBJECT_FORCELOCATION);
endfunction