use uo;

function Multi_DestroyMulti( multi )

	return DestroyMulti( multi );
endfunction

function Multi_GetTarget( mobile, name, flags:=0, dist:=20 )

	var targ := TargetMultiPlacement( mobile, name, flags );
	if( !targ )
		SendSysMessage( mobile, "Cancelled."+targ.errortext );
		return 0;
	elseif( !CheckLosAt( mobile, targ.x, targ.y, targ.z ) && flags != CRMULTI_IGNORE_ALL )
		SendSysMessage( mobile, "You do not have line of sight to there." );
		return 0;
	elseif( CoordinateDistance( mobile.x, mobile.y, targ.x, targ.y ) > dist )
		SendSysMessage( mobile, "That is too far away." );
		return 0;
	endif

	return targ;
endfunction

function Multi_MakeMulti( mobile, name, targ, flags:=0 )

	var multi := CreateMultiAtLocation( targ.x, targ.y, targ.z, name, flags, targ.realm );
	if( multi.errortext )
		SendSysMessage( mobile, "Error creating multi "+name+" -->"+multi.errortext );
                return 0;
	endif

	return multi;
endfunction
