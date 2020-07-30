
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
  while((opponent) && (!opponent.dead) && (!opponent.hidden) && (!opponent.concealed) && (Distance(me,opponent) < 3))
	if(!RunAwayFrom(opponent))
	  loops := loops + 1;
	  waittime := 0;
	else
	  loops := 0;
	  waittime := 1;
	endif
	ev := wait_for_event(waittime);
	process_combat_event(opponent);
	if(ev.type != SYSEVENT_SPEECH)
	  case (ev.type)
        SYSEVENT_DAMAGED: if((ev.source.npctemplate) and (CheckLineOfSight(me, ev.source)))
                            opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
                          elseif((Distance(me, ev.source) < Distance(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent))
					        opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
                          elseif(RandomInt(3)==1)
					        opponent := ev.source;
			  		        SetOpponent(opponent);
					        TurnToward(opponent);
			              endif
	    SYSEVENT_ENGAGED: if(ev.source)
			                if(RandomInt(6) == 1)
					          opponent := ev.source;
			  		          SetOpponent(opponent);
					          TurnToward(opponent);
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
	endif
  endwhile
  post_combat();
endfunction

function EnableCombatEvents()
  EnableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
  DisableEvents(SYSEVENT_SPEECH);
endfunction

function DisableCombatEvents()
  DisableEvents(SYSEVENT_ENGAGED + SYSEVENT_DAMAGED + EVID_PEACEMADE);
endfunction

function flee(opponent)
  if((me.script == "immobile") || (me.script == "immobilespell"))
	sleep(2);
	return;
  else
    var numflees := GetObjProperty(me,"#flees");
	if(numflees > 10)
      RestartScript(me);
	else
	  numflees := numflees + 1;
	endif
    SetObjProperty(me,"#flees", numflees);
    var runs := 0;
    var chk := 0;
    var ev;
    while((Distance(me, opponent) < 15) && (runs < 50) && (CheckLineOfSight(me, opponent)))
      ev := wait_for_event(1);
      chk := process_flee_event(opponent);
	  WalkAwayFrom(opponent);
	  runs := runs +1;
	  case (ev.type)
        SYSEVENT_DAMAGED: if(((ev.source.npctemplate) and (CheckLineOfSight(me, ev.source))) ||(Distance(me, ev.source) < Distance(me, opponent)) || (!CheckLineOfSight(me, opponent)) || (!opponent) || (RandomInt(3)==1))
					        opponent := ev.source;
			  		        SetOpponent(opponent);
			              endif
        EVID_PEACEMADE:   chk := 1;
        EVID_HERDING:     Herd(ev);
	  endcase
	  if(chk)
	    break;
	  endif
    endwhile
  endif
endfunction
