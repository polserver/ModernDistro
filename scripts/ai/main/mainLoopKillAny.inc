function main_AI_loop()
  var ev;
  var wanders := 60;
  var next_loot := ReadGameClock() + 20;
  EnableMainEvents();
  look_around();
  while(1)
    if(ReadGameClock() > next_loot)
      grabloot();
      next_loot := ReadGameClock() + 5;
    else
      wander();
    endif
    wanders := wanders +1;
    if(wanders > 60)
      wanders :=0;
      ev := sleepmode();
    else
      ev := os::wait_for_event(5);
    endif
    if(ev)
      repeat
        case(ev.type)
          SYSEVENT_ENTEREDAREA:	if(CheckLineOfSight( me, ev.source ))
									fight(ev.source);
								endif

          SYSEVENT_ENGAGED:
          SYSEVENT_DAMAGED:     Fight(ev.source);
          EVID_HERDING:         Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(1)));
    endif
   endwhile
endfunction

function look_around()
  foreach critter in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    Fight(critter);
  endforeach
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction

function Herd(ev)
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