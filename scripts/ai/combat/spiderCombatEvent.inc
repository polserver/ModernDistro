include ":attributes:attributes";

function process_combat_event(opponent)
  send_web(opponent);
  if ((AP_GetVital(me, VITALID_LIFE) * 100/AP_GetVitalMaximumValue(me, VITALID_LIFE)) < flee_point)
	EraseObjProperty(me,"#flees");
	flee(opponent);
  endif
endfunction

function process_flee_event(unused opponent)
  return 0;
endfunction

function in_combat_event_loop(opponent, loops)
	send_web(opponent);

	if ( loops > 50 )
		flee(opponent);
		return;
	endif
endfunction

function post_combat()
	DisableCombatEvents();
	EnableMainEvents();

	SetWarMode( 0 );
	num_casts := 2;

	sleep(1);
	look_around();
endfunction

function prepare_for_fight(unused opponent)
	DisableMainEvents();
	EnableCombatEvents();
endfunction

function send_web(opponent)
	if (RandomInt(20) != 1)
		return;
	endif

	if (!CheckLineOfSight( me, opponent ))
		return;
	endif

	PrintTextAbove(me, "The spider spits a web!");

	//	-causes client crash-
	//
	//PlayMovingEffectXYZ( me.x, me.y, me.z,
	//                         opponent.x, opponent.y, opponent.z,
	//                         UOBJ_SPIDERWEB, 8, 0, 0, me.realm );
	//
	//	-causes client crash-

	sleepms(1250);
	CreateItemAtLocation(opponent.x, opponent.y, opponent.z, UOBJ_SPIDERWEB, 1, opponent.realm);
endfunction