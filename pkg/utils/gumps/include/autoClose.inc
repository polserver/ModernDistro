/*
 * Purpose: Stores all the CloseGump custom function calls for easy sorting and finding.
 * TODO: Think of more possible conditions one would need or want to close a gump automatically.
 *
 * Function List
 *  GFCloseGump ( mobile, pid, ret_val:=0 )
 *  GFCloseGumpTimed ( mobile, pid, timer, ret_val:=0 )
 *  GFCloseGumpOnMovedCoordinateDistance ( mobile, pid, variance:=1, ret_val:=0 )
 *  GFCloseGumpOnMovedDistance ( mobile, pid, variance:=1, ret_val:=0 )
 */

/*
 * GFCloseGump(mobile, pid, ret_val:=0)
 *
 * Purpose
 * Instructs a client to close a gump.
 *
 * Parameters
 * mobile:		Player that has the gump open.
 * pid:		Pid of the script that has the open gump.
 * ret_val:	What return value the gump should send when closed.
 *
 * Return Value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function GFCloseGump( mobile, pid, ret_val:=0 )

	return CloseGump( mobile, pid, ret_val );
endfunction

/*
 * GFCloseGumpTimed(mobile, pid, timer, ret_val:=0)
 *
 * Purpose
 * Instructs a client to close a gump after duration is up.
 *
 * Parameters
 * mobile:		Player that has the gump open.
 * pid:		Pid of the script that has the open gump.
 * timer: Duration the gump should be left open for the client.
 * ret_val:	What return value the gump should send when closed.
 *
 * Return Value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function GFCloseGumpTimed( mobile, pid, timer, ret_val:=0 )
	return Start_Script( ":gumps:scripts/autoClose/autoClose", array{mobile, pid, timer, ret_val} );
endfunction

/*
 * GFCloseGumpOnMovedCoordinateDistance( mobile, pid, variance:=1, ret_val:=0 )
 *
 * Purpose
 * Instructs a client to close a gump once the character has moved squares greater than the variance parameter.
 *
 * Parameters
 * mobile:		Player that has the gump open.
 * pid:		Pid of the script that has the open gump.
 * variance: Amount of tiles the player can move before the gump is automatically closed.
 * ret_val:	What return value the gump should send when closed.
 *
 * Return Value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function GFCloseGumpOnMovedCoordinateDistance( mobile, pid, variance:=1, ret_val:=0 )
	return Start_Script( ":gumps:scripts/autoClose/autoCloseOnMovedCoordinateDistance", array{mobile, pid, variance, ret_val} );
endfunction

/*
 * GFCloseGumpOnMovedDistance ( mobile, object, pid, variance:=1, ret_val:=0 )
 *
 * Purpose
 * Instructs a client to close a gump after character moves squares equal to variance parameter away from the object.
 *
 * Parameters
 * mobile:		Player that has the gump open.
 * object:    item/NPC/Character reference
 * pid:		Pid of the script that has the open gump.
 * variance: Amount of tiles the player can move from the object before the gump is automatically closed.
 * ret_val:	What return value the gump should send when closed.
 *
 * Return Value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function GFCloseGumpOnMovedDistance( mobile, object, pid, variance:=1, ret_val:=0 )
	return Start_Script( ":gumps:scripts/autoClose/autoCloseOnMovedDistance", array{mobile, object, pid, dist_traveled, ret_val} );
endfunction

/*
 * GFCloseGumpOnLeaveArea ( mobile, x_1, y_1, x_2, y_2, pid, variance:=1, ret_val:=0 )
 *
 * Purpose
 * Instructs a client to close a gump if character leaves the area.
 *
 * Parameters
 * mobile:		Player that has the gump open.
 * x_1:       min X location of a given area ( upper left corner of the box is generally what you will get this parameter and the y_1 parameter from )
 * y_1:       min Y location of a given area ( upper left corner of the box is generally what you will get this parameter and the x_1 parameter from )
 * x_2:       max X location of a given area ( lower right corner of the box is generally what you will get this parameter and the y_2 parameter from )
 * y_2:       max Y location of a given area ( lower right corner of the box is generally what you will get this parameter and the x_2 parameter from )
 * pid:		Pid of the script that has the open gump.
 * ret_val:	What return value the gump should send when closed.
 *
 * Return Value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function GFCloseGumpOnLeaveArea( mobile, x_1, y_1, x_2, y_2, pid, variance:=1, ret_val:=0 )
	return Start_Script( ":gumps:scripts/autoClose/autoCloseOnLeaveArea", array{mobile, x_1, y_1, x_2, y_2, pid, ret_val} );
endfunction
