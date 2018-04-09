/* $Id: facings.inc 554 2006-07-04 10:48:14Z austinheilman $
 *
 */
use uo;
use os;
use math;

const FACE_POS_BEHIND := 1;
const FACE_POS_INFRONT := 2;
 
 
/*
 * TurnObjectToward(object, x_pos, y_pos)
 *
 * Purpose
 * Sets an objects .facing member to face an x, y coordinate.
 *
 * Purpose
 * object:	Object to set the facing on.
 * x_pos:	X position to check.
 * y_pos:	Y position to check.
 *
 * Return value
 * Object's original facing
 *
 */
function TurnObjectToward(object, x_pos, y_pos)
	var orig := object.facing;

	object.facing := GetFacing(object.x, object.y, x_pos, y_pos);

	return orig;
endfunction


/*
 * IsFacing(mobile, x_pos, y_pos)
 *
 * Purpose
 * Determines if a mobile is facing a set of coordinates.
 *
 * Parameters
 * mobile:	Mobile to check the coordinates with.
 * x_pos:	X position to check.
 * y_pos:	Y position to check.
 *
 * Return value
 * Returns 1 if the mobile is facing the coordinates.
 * Returns 0 if not.
 *
 */
 function IsFacing(mobile, x, y)
	return ( mobile.facing == GetFacing(mobile.x, mobile.y, x, y) );
endfunction


/*
 * InPosition(mobile, x_pos, y_pos, flag)
 * Determines if a set of coordinates are behind a character.
 * Example ( @ - character * - valid )
 *         @
 *        ***
 *       *****
 *      *******
 *
 * Parameters
 * mobile:	Mobile to check the coordinates with.
 * x_pos:	X position to check.
 * y_pos:	Y position to check.
 *
 * Return value
 * Returns 1 if the coordinates are behind the mobile.
 * Returns 0 if not.
 *
 */
function InPosition(tgt_mob, locx, locy, flag)
	var frot := Abs(((tgt_mob.facing+4)%8)-GetFacing(tgt_mob.x,tgt_mob.y,locx,locy));
	if ( frot > 4 )
		frot := 8-frot;
	endif

	if ( flag == FACE_POS_BEHIND )
		return (frot < 2);
	elseif ( flag == FACE_POS_INFRONT )
		return (frot > 2);
	endif
endfunction

/*
 * GetPointsByfacing(x_pos, y_pos, facing, dist)
 *
 * Purpose
 * Starts at an X, Y position, then based off the facing and distance, returns coordinates on that line.
 *
 * Parameters
 * x_pos:	X coordinate to start on.
 * y_pos:	Y coordinate to start on.
 * facing:	Direction (0-7) to go on the line.
 * dist:	Distance to go along the line.
 *
 * Return Value
 * Returns an array of structs containing .x .y members for coordinates.
 *
 */
function GetPointsByFacing(x_pos, y_pos, facing, dist)
	var coord := struct{"x", "y"};
	var coord_list := array{};

	var xmod := 0, ymod := 0;
	case ( facing ) // Should find a more sleek mathish way to do this!
		0: xmod := 0;  ymod := -1; break;
		1: xmod := +1; ymod := -1; break;
		2: xmod := +1; ymod := 0;  break;
		3: xmod := +1; ymod := +1; break;
		4: xmod := 0;  ymod := +1; break;
		5: xmod := -1; ymod := +1; break;
		6: xmod := -1; ymod := 0;  break;
		7: xmod := -1; ymod := -1; break;
		default:
			return error{"errortext":="Invalid facing."};
			break;
	endcase

	var i;
	for( i:=1; i<=dist; i+=1 )
		coord.x := x_pos+(i*xmod);
		coord.y := y_pos+(i*ymod);
		coord_list.Append(coord);
		SleepMS(2);
	endfor

	return coord_list;
endfunction
