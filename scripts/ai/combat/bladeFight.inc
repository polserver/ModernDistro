function Fight(opponent)
  if((opponent.cmdlevel > 0) || (opponent == me))
    SetWarMode(0);
    opponent := 0;
	return;
  endif
  set_priority(50);
  SetOpponent(opponent);
  prepare_for_fight(opponent);
  var loops := 0;
  var ev;
  var waittime := 0;
  while((opponent) && not (opponent.dead || opponent.hidden || opponent.concealed) && (Distance(me,opponent) < 20) )
	if(!CloseDistance(opponent))
	  loops := loops + 1;
	  waittime := 0;
	else
	  loops := 0;
	  waittime := 1;
	endif
	in_combat_event_loop(opponent, loops);
	ev := wait_for_event(waittime);
	case (ev.type)
      SYSEVENT_DAMAGED:
	  SYSEVENT_ENGAGED:   if(ev.source)
			            if(RandomInt(6)==1)
				          if((!ev.source.npctemplate) || (ev.source.script == "tamed") || (ev.source.script == "employed"))
					        opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
				          endif
			            endif
			          endif
      EVID_PEACEMADE: SetWarMode(0);
			          SetOpponent(0);
			          sleep(1);
			          DisableCombatEvents();
			          EnableMainEvents();
			          return;
	endcase
  endwhile
  post_combat();
endfunction

function EnableCombatEvents()
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
endfunction

function DisableCombatEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
endfunction