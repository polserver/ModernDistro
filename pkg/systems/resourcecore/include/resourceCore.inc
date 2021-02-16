use uo;
use os;

function RC_CheckToolDurability( byref mobile, byref tool )

	if( tool.hp > 0 )
		if( tool.hp >= 1 )
			tool.hp -= 1;
		endif
	elseif( tool.maxhp > 0 )
		tool.maxhp_mod -= 1;
		SendSysMessage( mobile, "Your tool has been damaged.", 0x3, 0x32 );
	endif

	return ( tool.maxhp >= 1 );
endfunction

function RC_StartCharacterTracker( byref mobile, byref x, byref y, byref realm, byref PID )

	var parameters := Struct{
								"Mobile":=mobile,
								"X":=x,
								"Y":=y,
								"Realm":=realm,
								"PID":=PID
							};
	return Start_Script( ":resourcecore:scripts/trackerCharacter", parameters );
endfunction