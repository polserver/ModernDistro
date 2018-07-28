/*
        Maintained by The POL Distro Team

        2018-07-25

*/
use uo;
use cfgfile;

include ":attributes:attributes";
include ":weapons:weaponInfo";

function TP_CheckRange( mobile, weapon, targ )

	var wpn_range := CInt( CS_GetWeaponInfo( weapon, "MaxRange" ));
	if( !wpn_range )
		wpn_range := 1;
	endif

	if( Distance( mobile, targ ) > wpn_range )
		SendSysMessage( mobile, "You are too far away." );
		return 0;
	endif

	return wpn_range;
endfunction

function TP_CanAdvance( mobile, attrib_name )

	if( CDbl( AP_GetTrueSkill( mobile, attrib_name )) > CDbl( 75 ))
		return ADV_DISABLE;
	else
		return ADV_SKILL;
	endif
endfunction
