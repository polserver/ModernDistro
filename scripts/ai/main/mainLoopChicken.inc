const GFX_UNSHORN_SHEEP := 0xcf;
const GFX_SHORN_SHEEP := 0xdf;

function main_AI_loop()
  var ev;
  var wanders := 60;
  var nextlay := CInt(ReadGameClock()+ 3600);
  var nextbark := ReadGameClock()+ 30;
  var next_wander := 0;
  EnableMainEvents();
  while(1)
    if(ReadGameClock()> next_wander)
      wander();
      next_wander := ReadGameClock() + 5;
      wanders := wanders + 1;
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
          SYSEVENT_DAMAGED: RunLikeHell(ev.source);
          SYSEVENT_ENGAGED: RunLikeHell(ev.source);
          EVID_HERDING:     Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(5)));
    endif
    if(ReadGameClock()> nextlay)
      nextlay := ReadGameClock()+ 2600 + CInt(3800 * RandomInt(2));
      CreateItemAtLocation(me.x, me.y, me.z, "egg", 1, me.realm);
    endif
    if(ReadGameClock()> nextbark)
      nextbark := ReadGameClock() + ((RandomInt(5) + 1)* 60);
      PlaySoundEffect(me,0x70);
    endif
  endwhile
endfunction

function RunLikeHell(opponent)
  var ev;
  while((opponent)&& not (opponent.dead || opponent.hidden || opponent.concealed)&& (Distance(me,opponent)< 25))
    RunAwayFrom(opponent);
    repeat
    until(!(ev := os::wait_for_event(1)));
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED + SYSEVENT_ENTEREDAREA);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, 8);
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