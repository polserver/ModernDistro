
function main_AI_loop()
  var ev := os::wait_for_event(0);
  var wanders := 60;
  var next_loot := ReadGameClock() + 20;
  EnableMainEvents();
  var rint := 0;
  while(1)
    look_around();
    wander();
    if(ReadGameClock() > next_loot)
      grabloot();
      next_loot := ReadGameClock() + 10;
    endif
    rint := RandomInt(4) + 1;
    if(rint == 4)
      PlaySoundEffect(me, idlesnd1);
    elseif(rint == 3)
      PlaySoundEffect(me, idlesnd2);
    endif
    wanders := wanders +1;
    if(wanders >= 60)
      wanders := 0;
      ev := sleepmode();
    else
      ev := os::wait_for_event(5);
    endif
    if(ev)
      case(ev.type)
        SYSEVENT_ENTEREDAREA:   if((!ev.source.npctemplate) or (ev.source.script == "employed") || (ev.source.script == "tamed"))
									if(CheckLineOfSight( me, ev.source ))
										Fight(ev.source);
									endif
                                endif
        SYSEVENT_ENGAGED:       Fight(ev.source);
        EVID_ALL_ATTACK_CMD:    Fight(ev.source);
        SYSEVENT_DAMAGED:       Fight(ev.source);
        EVID_HERDING:           Herd(ev);
      endcase
    endif
  endwhile
endfunction

function look_around()
  foreach npc in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    if((!npc.npctemplate) || (npc.script == "employed") || (npc.script == "tamed"))
      Fight(npc);
    endif
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