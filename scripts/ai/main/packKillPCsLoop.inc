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
      if(RandomInt(3) == 3)
        if(RandomInt(1) == 1)
          PlaySoundEffect(me, idlesnd1);
        else
          PlaySoundEffect(me, idlesnd2);
        endif
      endif
    endif
    wanders := wanders +1;
    if(wanders >= 60)
      wanders :=0;
      ev := sleepmode();
    else
      ev := os::wait_for_event(5);
    endif
    if(ev)
      repeat
        case(ev.type)
          SYSEVENT_ENTEREDAREA: if((!(ev.source).npctemplate) ||(((ev.source).script == "employed") ||((ev.source).script == "tamed")))
                                  foreach mobile in ListMobilesInLineOfSight(me, 10);
                                    if(mobile.npctemplate == me.npctemplate)
                                      var evnt := array;
                                      evnt.+ type;
                                      evnt.+ source;
	                                  evnt.source := ev.source;
	                                  evnt.type := EVID_ALL_ATTACK_CMD;
                                      SendEvent(mobile, evnt);
                                    endif
                                  endforeach
                                endif
          EVID_ALL_ATTACK_CMD:  Fight(ev.source);
          SYSEVENT_ENGAGED:
          SYSEVENT_DAMAGED:     foreach mobile in ListMobilesInLineOfSight(me, 10);
                                  if(mobile.npctemplate == me.npctemplate)
                                    var evnt := array;
                                    evnt.+ type;
                                    evnt.+ source;
	                                evnt.source := ev.source;
	                                evnt.type := EVID_ALL_ATTACK_CMD;
                                    SendEvent(mobile, evnt);
                                  endif
                                endforeach
                                Fight(ev.source);
          EVID_HERDING:           Herd(ev);
        endcase
      until(!(ev := os::wait_for_event(2)));
    endif
   endwhile
endfunction

function look_around()
  foreach npc in ListMobilesInLineOfSight(me, HALT_THRESHOLD)
    if((!npc.npctemplate) ||(npc.script == "tamed") ||(npc.script == "employed"))
      foreach mobile in ListMobilesInLineOfSight(me, 10);
        if(mobile.npctemplate == me.npctemplate)
          var evnt := array;
          evnt.+ type;
          evnt.+ source;
	      evnt.source := npc;
	      evnt.type := EVID_ALL_ATTACK_CMD;
          SendEvent(mobile, evnt);
        endif
       endforeach
       Fight(npc);
       return;
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

function get_help(opponent)
    var inev;
    var ev array;
    ev.+type := EVID_ALL_ATTACK_CMD;
    ev.+source := opponent;
    foreach critter in ListMobilesNearLocation(me.x, me.y, me.z, 4, me.realm)
      if((critter.npctemplate) && (critter.script != "employed") && (critter.script != "tamed") && (critter.serial != me.serial))
        SendEvent(critter, ev);
      endif
      inev := wait_for_event(0);
      if(inev.type == EVID_ALL_ATTACK_CMD)
        return;
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