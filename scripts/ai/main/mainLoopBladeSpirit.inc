function main_AI_loop()
  var ev;
  var wanders := 0;
  EnableMainEvents();
  look_around();
  while(1)
    ev := os::wait_for_event(1);
	if(wanders > 60)
	  wanders :=0;
	  ev := sleepmode();
	endif
	if(ev)
	  repeat
	    case(ev.type)
          SYSEVENT_ENTEREDAREA:	if(CheckLineOfSight( me, ev.source ))
									fight(ev.source);
								endif

          EVID_ENGAGED:
          EVID_DAMAGED:         Fight(ev.source);
	    endcase
      until(!(ev := os::wait_for_event(1)));
    endif
    look_around();
    wander();
	wanders := wanders +1;
  endwhile
endfunction

function look_around()
  var him, dista;
  var dis := 20;
  foreach critter in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    dista := Distance(me, critter);
    if(dista < dis)
      if((critter.npctemplate != "bladespirit") && (critter.npctemplate != "vortex"))
        him := critter;
        dis := dista;
      endif
    endif
  endforeach
  if(him)
    Fight(him);
  endif
endfunction

function EnableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + EVID_LEFTAREA + EVID_DISENGAGED + EVID_OPPONENT_MOVED);
  EnableEvents(EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(EVID_ENGAGED + EVID_DAMAGED + EVID_ENTEREDAREA);
endfunction