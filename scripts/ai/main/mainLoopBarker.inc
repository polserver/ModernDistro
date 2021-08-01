function main_AI_loop()
  var ev;
  var wanders := 60;
  var mynoise := bark();
  EnableMainEvents();
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
          SYSEVENT_ENTEREDAREA: if(!ev.source.isA(POLCLASS_NPC))
                                  PlaySoundEffect(me, mynoise);
                                endif
          SYSEVENT_DAMAGED:     Fight(ev.source);
          SYSEVENT_ENGAGED:     RunLikeHell(ev.source);
          EVID_HERDING:           Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(2)));
    endif
  endwhile
endfunction

function RunLikeHell(opponent)
  var ev;
  while((opponent)&& not(opponent.dead || opponent.hidden || opponent.concealed)&&(Distance(me,opponent)< 25))
    WalkAwayFrom(opponent);
    repeat
      case(ev.type)
        SYSEVENT_DAMAGED: if(RandomInt(3)==1)
                            Fight(ev.source);
                            return;
                          endif
      endcase
    until(!(ev := os::wait_for_event(1)));
  endwhile
endfunction

function look_around()
  return;
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents(SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA);
endfunction

function bark()
  var noise := 0;
  case(me.graphic)
    0x06: case(RandomInt(3))   //bird
            0: noise := 0x1b;
            1: noise := 0x18;
            2: noise := 0x277;
          endcase
    0xc9: noise := 0x6b;       //cat
    0xe7:
    0xe9:
    0xe8:
    0xd8: noise := 0x7a;       //cow
    0xd9: case(RandomInt(2))   //dog
            0: noise := 0x86;
            1: noise := 0x87;
          endcase
    0xd1: case(RandomInt(2))   //goat
            0: noise := 0x9a;
            1: noise := 0x9b;
          endcase
    0xcb: noise := 0xc6;       //pig
    0xcc:
    0xc8:
    0xe2:
    0xe4: case(RandomInt(4))   //horses
            0: noise := 0xa9;
            1: noise := 0xaa;
            2: noise := 0xab;
            3: noise := 0xac;
          endcase
    0x51: case(RandomInt(4))   //toad
            0: noise := 0x267;
            1: noise := 0x268;
            2: noise := 0x26a;
            3: noise := 0x26d;
          endcase
    0xd3: case(RandomInt(5))   //bears
            0: noise := 0xa4;
            1: noise := 0xa5;
            2: noise := 0xa6;
            3: noise := 0xa7;
            4: noise := 0xa8;
          endcase
    0x1d: case(RandomInt(3))   //gorilla
            0: noise := 0x9f;
            1: noise := 0xa0;
            2: noise := 0xa2;
          endcase
  endcase
  return noise;
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