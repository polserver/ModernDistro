/*
*/
use uo;
use os;

include ":attributes:attributes";

function DetectHidden( mobile, hider )

        if( !hider.hidden )
                //Just make sure our hider is hidden before performing the check.
                return 1; //Already revealed.
        endif

	var mobile_skill := AP_GetSkill( mobile, DETECTING_HIDDEN ),
	    hider_skill  := AP_GetSkill( hider, HIDING );


        if( mobile.IsA( POLCLASS_NPC ))
                if( !mobile_skill )
                        return 0;
                endif

                mobile_skill *= 2;
        endif

	var chance := CInt( mobile_skill - hider_skill );
        if( chance < 0 )
               chance := 5;
        endif

	if( RandomInt( 100 ) > chance && !mobile.multi )
	       return 0;
	endif

        hider.hidden := 0;
        PrintTextAbovePrivate( hider, "You have been discovered!", hider );

	return 1;
endfunction
