function Fight(opponent)
  if((opponent.cmdlevel > 0) || (opponent.serial == me.serial))
    SetWarMode(0);
    opponent := 0;
	return;
  endif
  set_priority(50);
  SetOpponent(opponent);
  prepare_for_fight(opponent);
  TurnToward(opponent);
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
	process_combat_event(opponent);
	ev := wait_for_event(waittime);
	case (ev.type)
      SYSEVENT_DAMAGED: if((Distance(me, ev.source) < Distance(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent))
					      opponent := ev.source;
			  		      SetOpponent(opponent);
					      TurnToward(opponent);
                        elseif(RandomInt(3)==1)
					      opponent := ev.source;
			  		      SetOpponent(opponent);
					      TurnToward(opponent);
			            endif
	  SYSEVENT_ENGAGED: if(ev.source)
			              if(RandomInt(6)==1)
					        opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
					        process_combat_event(opponent);
			              endif
			            endif
      EVID_PEACEMADE:   SetWarMode(0);
			            SetOpponent(0);
			            sleep(1);
			            DisableCombatEvents();
			            EnableMainEvents();
			            return;
      EVID_HERDING:     Herd(ev);
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

function flee(opponent)
  if((me.script == "immobile") || (me.script == "immobilespell"))
	sleep(2);
	return;
  endif
  var numflees := GetObjProperty(me,"#flees");
  if(numflees)
	if(numflees > 10)
      RestartScript(me);
	else
	  numflees := numflees + 1;
	endif
  else
	numflees := 1;
  endif
  SetObjProperty(me,"#flees", numflees);
  var runs := 0;
  while((Distance(me, opponent) < 5) && (runs < 50))
	WalkAwayFrom(opponent);
	runs := runs +1;
	wait_for_event(0);
  endwhile
  RestartScript(me);
endfunction