function main_AI_loop(next_wander := 0)
  var ev;
  var looter := GetObjProperty(me, "looter");
  var wanders := 60;
  look_around();
  EnableMainEvents();
  while(1)
    ev := os::wait_for_event(5);
    repeat
      case(ev.type)
        SYSEVENT_ENGAGED:
        SYSEVENT_DAMAGED:     if(ev.source)
                                Fight(ev.source);
                                endif
        SYSEVENT_ENTEREDAREA:	if((!critter.npctemplate) ||(critter.script == "tamed") || (critter.script == "employed"))
									if(CheckLineOfSight( me, ev.source ))
										fight(ev.source);
									endif
								endif
        EVID_HERDING:         Herd(ev);
      endcase
    until(!(ev := os::wait_for_event(5)));
    if(ReadGameClock() > next_wander)
      if(!me.hidden)
        wander();
      endif
      if(looter)
        grabloot();
      endif
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
  foreach critter in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    if((!critter.npctemplate) ||(critter.script == "tamed") || (critter.script == "employed"))
      Fight(critter);
      return;
    endif
  endforeach
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  EnableEvents(SYSEVENT_ENTEREDAREA + SYSEVENT_DAMAGED + SYSEVENT_ENGAGED, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_ENGAGED);
  DisableEvents(SYSEVENT_DAMAGED);
  DisableEvents(SYSEVENT_ENTEREDAREA);
  cleareventque();
endfunction

function EVALUATING_INTELLIGENCE(ev)
  var holder := ev.data;
  var lx := holder[1];
  var ly := holder[2];
  var loops := 0;
  var opponent;
  var waittime := 0;
  while(1)
	if(!CloseIn(me, lx, ly))
	  loops := loops + 1;
	  waittime := 0;
	else
	  loops := 0;
	  waittime := 1;
	endif
	if((loops >= 30) or (CoordinateDistance(me.x, me.y, lx, ly) <= 1))
	  break;
	endif
	ev := wait_for_event(waittime);
	case (ev.type)
      SYSEVENT_DAMAGED: opponent := ev.source;
			  		    SetOpponent(opponent);
					    TurnToward(opponent);
	  SYSEVENT_ENGAGED: opponent := ev.source;
			  		    SetOpponent(opponent);
					    TurnToward(opponent);
	endcase
  endwhile
  Return;
endfunction

function CloseIn(me, lx, ly)
  case (CoordinateDistance(me.x, me.y, lx, ly))
    0:       return 1;
    default: WalkTowardLocation(lx, ly);
             return 0;
  endcase
endfunction