use uo;
use os;

include ":contextmenus:constants";
include ":security:contextMenuReport";

// Flags used by the ContextMenu_Disabled function
Const CM_RANGE_CHECK	:= 1;
Const CM_LOS_CHECK		:= 2;
Const CM_DEAD_CHECK		:= 4;

/*
 *
 * ContextMenu_AddEntry( list, number, color:=0xFFFF, opts:=Struct )
 *
 * Parameters:
 * list - Array to append number and options to
 * number - enum entry or cliloc to append to list
 * color - Color of text in context menu
 * opts - Struct containing the following entries:
 *        "Flags" = this can contain any combination of the following CM_RANGE_CHECK CM_LOS_CHECK CM_DEAD_CHECK added together.
 *        "Range" = if CM_RANGE_CHECK flag is set, this should be in here, else it will default to 5.
 *        "Object" = object_ref for which the context menu is being accessed from, needed for CM_RANGE_CHECK and CM_LOS_CHECK
 *        "Mobile" = mobile_ref accessing context menu, this MUST be in the struct
 *
 * Purpose:
 * Appends a context menu entry to the provided list, this also does all the checks needed
 * if the proper entries are deined in the 'opts' struct.
 *
 * Date: 2022-09-27 18:27:49
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * N/A
 *
 */
function ContextMenu_AddEntry( byref list, number, byref color:=0xFFFF, byref opts:=Struct )

// Enum support, for older context entries...
    if( number <= 0x7FFF )
        number += 3000000;
    endif

    if( !opts.Keys().Size() )
        list.Append( Struct{ "cliloc":=number, "color":=color } );
    else
        if( !opts.Mobile.IsA( POLCLASS_MOBILE ) || !opts.Exists( "Mobile" ) )
            ContextMenu_ReportText( opts.Mobile, "ContextMenu_AddEntry -> opts Struct must contain player mobile reference with 'Mobile' as the key and does not.", CM_REPORT_CONSOLE+CM_REPORT_SYSLOG );
            return 0;
        endif

        if( opts.Flags & CM_RANGE_CHECK || opts.Flags & CM_RANGE_CHECK )
            if( !opts.Object.IsA( POLCLASS_UOBJECT ) || !opts.Exists( "Object" ) )
                ContextMenu_ReportText( opts.Mobile, "ContextMenu_AddEntry -> opts Struct must contain an UObject reference with 'Object' when checking Range or Line of Sight and does not.", CM_REPORT_CONSOLE+CM_REPORT_SYSLOG );
                return 0;
            endif
        endif

        if( opts.Flags & CM_RANGE_CHECK )
            if( opts.Range <= 0 || !opts.Exists( "Range" ) )
                opts.Insert( "Flags", 5 );
            endif
        endif

        list.Append( Struct{ "cliloc":=number, "color":=color, "disabled":=ContextMenu_Disabled( opts ) } );
    endif
endfunction

/*
 *
 * ContextMenu_Disabled( opts )
 *
 * Parameters:
 * opts - Struct containing the following entries:
 *        "Flags" this can contain any combination of the following CM_RANGE_CHECK CM_LOS_CHECK CM_DEAD_CHECK added together.
 *        "Range" if CM_RANGE_CHECK flag is set, this should be in here, else it will default to 5.
 *        "Object" object_ref for which the context menu is being accessed from, needed for CM_RANGE_CHECK and CM_LOS_CHECK
 *        "Mobile" mobile_ref accessing context menu, this MUST be in the struct
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
function ContextMenu_Disabled( byref opts )

    var mobile := opts.Mobile,
        flags := opts.Flags,
        range,
        obj;

    if( opts.Exists( "Range" ) )
        range := opts.Range;
    endif

    if( opts.Exists( "Object" ) )
        obj := opts.Object;
    endif

    if( flags & CM_RANGE_CHECK )
		if( Distance( mobile, obj ) > range )
			return 1;
		endif
	endif

	if( flags & CM_LOS_CHECK )
		if( !CheckLineOfSight( mobile, obj ) )
			return 1;
		endif
	endif

	if( flags & CM_DEAD_CHECK )
		if( mobile.dead )
			return 1;
		endif
	endif

	return 0;
endfunction
