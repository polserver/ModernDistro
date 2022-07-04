use uo;
use os;

// Flags used by the ContextMenu_Disabled function
Const CM_RANGE_CHECK	:= 1;
Const CM_LOS_CHECK		:= 2;
Const CM_DEAD_CHECK		:= 4;

/*
 *
 * ContextMenu_ValidateMenu( mobile )
 *
 * Parameters:
 * mobile
 *
 * Purpose:
 * Addresses an issue whereby should a context menu be closed without
 * selecting a menu option (right click menu or clicking anywhere
 * outside the context menu), the next time a context menu is accessed
 * on any object, the menu appears but the selection is not returned as
 * the first iteration of the script is still active on the player
 * awaiting a response from the player from the menu. The client does
 * not return that the menu has been closed if no option is selected.
 *
 * Date: 2022-07-02 17:50:59
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * none
 *
 */
function ContextMenu_ValidateMenu( byref mobile )

	var process := GetProcess( GetObjProperty( mobile, "#ValidateContextMenu" ) );
	if( TypeOfInt( process ) == OT_SCRIPTEXREF )
		process.Kill();
	endif
	SetObjProperty( mobile, "#ValidateContextMenu", GetPid() );

	return 1;
endfunction

/*
 *
 * ContextMenu_Disabled( mobile, obj, flags, range )
 *
 * Parameters:
 * mobile - player ref
 * obj - obj ref the context menu belongs to.
 * flags - See constants at top of file.
 * range - range check between mobile and obj
 *
 * Purpose:
 * Determines whether on option should be disabled or not based
 * on Distance between object and mobile or Line of Sight between
 * object and mobile.
 *
 * Date: 2022-07-02 17:56:51
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * boolean
 *
 */
function ContextMenu_Disabled( byref mobile, byref obj, byref flags, byref range:=5 )

	if( ( 1 & ( flags >> 0 ) ) )
		if( Distance( mobile, obj ) > range )
			return 1;
		endif
	endif

	if( ( 1 & ( flags >> 1 ) ) )
		if( !CheckLineOfSight( mobile, obj ) )
			return 1;
		endif
	endif

	if( ( 1 & ( flags >> 2 ) ) )
		if( mobile.dead )
			return 1;
		endif
	endif

	return 0;
endfunction
