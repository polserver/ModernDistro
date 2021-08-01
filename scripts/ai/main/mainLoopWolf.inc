function main_AI_loop()
  var ev;
  var wanders := 60;
  var hatelist := "animal cat barker tamed chicken sheep bird";
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
          SYSEVENT_ENTEREDAREA:
          SYSEVENT_LEFTAREA:     if(( me.npctemplate == "dog") &&(!ev.source.npctemplate))
                                   case(RandomInt(2))
                                     0: PlaySoundEffect(me,0x86);
                                     1: PlaySoundEffect(me,0x87);
                                   endcase
                                 elseif((ev.source.script) &&( hatelist[ev.source.script]))
                                   get_help( ev.source);
                                   Fight( ev.source);
                                 elseif( ev.source.npctemplate == me.npctemplate)
                                   WalkToward(ev.source);
                                   sleepms(500);
                                   WalkToward(ev.source);
                                 endif
          SYSEVENT_DAMAGED:
          SYSEVENT_ENGAGED:      if(ev.source)
                                   get_help( ev.source);
                                   Fight( ev.source);
                                 endif
          EVID_ALL_ATTACK_CMD:   Fight( ev.source);
          EVID_HERDING:          Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(1)));
    endif
  endwhile
endfunction

function look_around()
  var hatelist := "animal cat barker tamed chicken sheep bird";
  foreach critter in ListMobilesInLineOfSight( me, HALT_THRESHOLD)
    if((critter.script) &&( hatelist[critter.script]))
      Fight( critter);
      return;
    endif
  endforeach
endfunction

function EnableMainEvents()
  EnableEvents(EVID_HERDING);
  DisableEvents( SYSEVENT_SPEECH + SYSEVENT_LEFTAREA + SYSEVENT_DISENGAGED + SYSEVENT_OPPONENT_MOVED);
  EnableEvents( SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA + SYSEVENT_LEFTAREA, HALT_THRESHOLD);
endfunction

function DisableMainEvents()
  DisableEvents( SYSEVENT_SPEECH + SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + SYSEVENT_ENTEREDAREA + SYSEVENT_LEFTAREA);
endfunction

function get_help( opponent)
  var ev := struct;
  ev.+type := EVID_ALL_ATTACK_CMD;
  ev.+source := opponent;
  foreach critter in ListMobilesNearLocation( me.x, me.y, me.z, HALT_THRESHOLD, me.realm)
    if((critter.npctemplate == me.npctemplate) &&(critter.script != "tamed") &&(!critter.warmode))
      SendEvent(critter, ev);
    endif
  endforeach
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