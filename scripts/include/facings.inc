/* $Id: facings.inc 554 2006-07-04 10:48:14Z austinheilman $
 *
 */
use uo;
use os;
use math;

const FACE_POS_BEHIND := 1;
const FACE_POS_INFRONT := 2;


/*
	+----------------------------> x
	|	NW		  N			NE
	|		+---+---+---+
	|		| 7 | 0 | 1 |
	|		+---+---+---+
	|	W	| 6 | C | 2 |	E
	|		+---+---+---+
	|		| 5 | 4 | 3 |
	|		+---+---+---+
	|	SW		  S			SE
	y
*/

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
 * IsRelativeFacing( mobile, x, y, vision_dist )
 * Determines if a location (x and y location) is relatively speaking
 * in front of the mobile.
 *
 * Notes:
 * 0 is directly in front of the mobile.
 *
 * 1 equates to what would essentially be a 90
 * degree view in front of the mobile
 *
 * 2 equates to what would essentially be a 180
 * degree view in front of the character.
 *
 * vision_dist = 0 ->*
 *
 *					 *
 * vision_dist = 1 ->*
 *					 *
 *
 *					**
 * vision_dist = 2 ->*
 *					**
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
function IsRelativeFacing( mobile, x, y, vision_dist:=2 )

	var obj_facing := GetFacing( mobile.x, mobile.y, x, y );
	var d := (mobile.facing-obj_facing)%8;

	if( d>3 )
		d := d - 8;
	elseif( d < -3 )
		d := d + 8;
 	endif

	return ( Abs( d ) <= vision_dist );
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
function GetPointsByFacing( x_pos, y_pos, facing, dist )

	var xmod := 0, ymod := 0;
	case( facing )
		0: xmod := 0;  ymod := -dist; break;
		1: xmod := +dist; ymod := -dist; break;
		2: xmod := +dist; ymod := 0;  break;
		3: xmod := +dist; ymod := +dist; break;
		4: xmod := 0;  ymod := +dist; break;
		5: xmod := -dist; ymod := +dist; break;
		6: xmod := -dist; ymod := 0;  break;
		7: xmod := -dist; ymod := -dist; break;
		default:
			return error{"errortext":="Invalid facing."};
			break;
	endcase

	return GetCoordsInLine( x_pos, y_pos, x_pos+xmod, y_pos+ymod );
endfunction

/*
 *
 * ConvertDirectionToLocX( facing )
 *
 * Parameters:
 * facing - facing value between 0-7
 *
 * Purpose:
 * Returns the x modifier for the given facing.
 *
 * Return Values:
 * int between -1 and 1
 *
 * Contributor:
 * Turley
 *
 */
function ConvertDirectionToLocX( byref facing )

	return CInt( Sin( facing * 0.78 ) * 1.5 );
endfunction

/*
 *
 * ConvertDirectionToLocY( facing )
 *
 * Parameters:
 * facing - facing value between 0-7
 *
 * Purpose:
 * Returns the y modifier for the given facing.
 *
 * Return Values:
 * int between -1 and 1
 *
 * Contributor:
 * Turley
 *
 */
function ConvertDirectionToLocY( byref facing )

	return CInt( Cos( facing * 0.78 ) * -1.5 );
endfunction

