function main_AI_loop()
  var ev;
  var wanders := 60;
  var next_loot := ReadGameClock() + 20;
  var next_wander := 0;
  EnableMainEvents();
  while(1)
    if(ReadGameClock() > next_loot)
      grabloot();
      next_loot := ReadGameClock() + 10;
      ev := os::wait_for_event(3);
    else
      if(ReadGameClock() > next_wander)
        wander();
        wanders := wanders +1;
        next_wander := ReadGameClock()+4;
        if(wanders > 60)
          wanders :=0;
          ev := sleepmode();
        else
          ev := os::wait_for_event(3);
        endif
      else
        ev := os::wait_for_event(3);
      endif
    endif
    if(ev)
      repeat
        case(ev.type)
          SYSEVENT_SPEECH:     if(!ev.source.isA(POLCLASS_NPC))
                                 next_wander := ReadGameClock() + 30;
                                 check_speech(ev.text, ev.source);
                               endif
          SYSEVENT_DAMAGED:
          SYSEVENT_ENGAGED:    Run(ev.source);
          SYSEVENT_ITEM_GIVEN: next_wander := ReadGameClock()+60;
                               TakeItem(ev.source,ev.item);
          EVID_HERDING:        Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(1)));
    endif
 endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_SPEECH, 3);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, HALT_THRESHOLD);
  EnableEvents(SYSEVENT_ITEM_GIVEN);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
  DisableEvents(SYSEVENT_ITEM_GIVEN);
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