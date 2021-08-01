

include "include/client";
include ":damage:damage";
include ":attributes:attributes";

function main_AI_loop()
  var ev;
  var wanders := 60;
  var next_wander := 0;
  var next_regrow := ReadGameClock() + 1800;
  var nextbark := ReadGameClock() + 30;
  EnableMainEvents();
  while (1)
    if(ReadGameClock() > next_wander)
      wander();
      next_wander := ReadGameClock()+5;
      wanders := wanders +1;
      if(wanders > 20)
        wanders :=0;
        ev := sleepmode();
      endif
    else
      ev := os::wait_for_event(15);
    endif
    if(ev)
      case (ev.type)
        SYSEVENT_DOUBLECLICKED: if(((RandomInt(499) + 1) == 250) || (ev.source.cmdlevel >= 3))
                                  PlaySoundEffect(me, 0xd7);
                                  sleepms(10);
                                  PlaySoundEffect(me, SFX_SPELL_EXPLOSION);
                                  PlayStationaryEffect(me.x, me.y, me.z, FX_EXPLODE_3, 7, 0x10, 0, me.realm);
                                  sleepms(5);
                                  MoveObjectToLocation(me,0,0,0,"britannia_alt",MOVEOBJECT_FORCELOCATION);
                                  KillMobile(me);
                                endif
        SYSEVENT_ENGAGED:       RunLikeHell(ev.source);
        EVID_HERDING:           Herd(ev);
      endcase
    endif
    if(ReadGameClock() > next_regrow)
      next_regrow := ReadGameClock()+1800;
      if(me.graphic == 223)
        me.graphic := 207;
      endif
    endif
    if(ReadGameClock() > nextbark)
      nextbark := ReadGameClock() + ((RandomInt(30)+1) + 60);
      case (RandomInt(2))
        0: PlaySoundEffect(me,0xd7);
        1: PlaySoundEffect(me,0xd8);
      endcase
    endif
  endwhile
endfunction

function RunLikeHell(opponent)
  var ev;
  while((opponent) && not (opponent.dead || opponent.hidden || opponent.concealed) && (Distance(me,opponent) < 25))
        RunAwayFrom(opponent);
    repeat
        case (ev.type)
      SYSEVENT_DAMAGED: if(RandomInt(3) == 1)
                          Fight(ev.source);
                                  return;
                        endif
    endcase
    until (!(ev := os::wait_for_event(1)));
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_ENTEREDAREA + SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_DOUBLECLICKED + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED);
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