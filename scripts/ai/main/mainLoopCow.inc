function main_AI_loop()
  var ev;
  var wanders := 0;
  EnableMainEvents();
  EnableEvents(SYSEVENT_DOUBLECLICKED);
  while(1)
    wander();
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
          SYSEVENT_DOUBLECLICKED: print("tipped");
          if(((RandomInt(19) + 1) == 10) || (ev.source.cmdlevel >= 3))
                                    PlaySoundEffect(me, 0x79);
                                    PerformAction(me, CInt(0x8));
                                  endif
          SYSEVENT_DAMAGED:       Fight(ev.source);
          SYSEVENT_ENGAGED:       Fight(ev.source);
          EVID_HERDING:           Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(5)));
    endif
   endwhile
endfunction

function RunLikeHell(opponent)
  var ev;
  while((opponent)&& not(opponent.dead || opponent.hidden || opponent.concealed)&&(Distance(me,opponent)< 25))
    WalkAwayFrom(opponent);
    repeat
      case(ev.type)
        SYSEVENT_DAMAGED:  Fight(ev.source);  return;
      endcase
    until(!(ev := os::wait_for_event(1)));
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_ENTEREDAREA + SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
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