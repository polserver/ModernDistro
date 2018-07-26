/*
 * $Id*
 *
 */

use uo;
use os;
use math;

/*
 * GetCoordsInSquare(mid_x, mid_y, radius)
 *
 * Purpose
 * Retrieves the coordinates in the area of a square.
 *
 * Parameters
 * mid_x:	X coordinate of the middle of the square.
 * mid_y:	Y coordinate of the middle of the square.
 * radius:	Size of the square
 *
 * Return value
 * Array of structs (.x, .y)
 *
 */
function GetCoordsInSquare(mid_x, mid_y, radius)
	var x_row := CInt(mid_x)-CInt(radius);
	var y_col := CInt(mid_y)-CInt(radius);
	var stop_x := CInt(mid_x)+CInt(radius);
	var stop_y := CInt(mid_y)+CInt(radius);

	var point_list := array{};
	var coord := struct{"x", "y"};

	for ( x_row:=(mid_x-radius); x_row<=stop_x; x_row+=1 )
		for ( y_col:=(mid_y-radius); y_col<=stop_y; y_col+=1 )
			coord.x := x_row;
			coord.y := y_col;
			point_list.Append(coord);
			SleepMS(2);
		endfor
		SleepMS(2);
	endfor

	return point_list;
endfunction

/*
 * GetCoordsInRect(x1, y1, x2, y2)
 *
 * Purpose
 * Retrieves the coordinates in the area of a rectangle.
 *
 * Parameters
 * x1:	X coordinate
 * y1:	Y coordinate
 * x2:	X coordinate
 * y2:	Y coordinate
 *
 * Return value
 * Array of structs (.x, .y)
 *
 */
function GetCoordsInRect(x1, y1, x2, y2)
	var point_list := array{};
	var coord := struct{"x", "y"};

	x1 := CInt(x1);
	y1 := CInt(y1);
	x2 := CInt(x2);
	y2 := CInt(y2);

	if ( x1 > x2 )
		x1 *= -1;
		x2 *= -1;
	endif
	if ( y1 > y2 )
		y1 *= -1;
		y2 *= -1;
	endif

	var sx, sy;
	for ( sx:=x1; sx<=x2; sx+=1 )
		for ( sy:=y1; sy<=y2; sy+=1 )
			coord.x := Abs(sx);
			coord.y := ABs(sy);
			point_list.Append(coord);
			SleepMS(2);
		endfor
		SleepMS(2);
	endfor

	return point_list;
endfunction

/*
 * GetCoordsInCircle(mid_x, mid_y, radius)
 *
 * Purpose
 * Retrieves the coordinates in the area of a circle.
 *
 * Parameters
 * mid_x:	X coordinate of the middle of the square.
 * mid_y:	Y coordinate of the middle of the square.
 * radius:	Size of the circle
 *
 * Return value
 * Array of structs (.x, .y)
 *
 */
function GetCoordsInCircle(mid_x, mid_y, radius)
	mid_x := CInt(mid_x);
	mid_y := CInt(mid_y);
	radius := CInt(radius);
	return error{"errortext":="Hasn't been coded yet!"};
endfunction

/*
 * GetCoordsInPolygon(coord_list)
 *
 * Purpose
 * Takes in an array of structs containing .x, .y coordinates.
 * Each point is a segment that builds the shape.
 * Finds the coordinates inside of that shape.
 *
 *
 * Parameters
 * coord_list:	Array of structs. Each struct contains an x and y member.
 *
 * Return value
 * Returns an array of structs containing a .x and .y member.
 *
 */
function GetCoordsInPolygon(coord_list)
	var num_coords := coord_list.Size();
	if ( num_coords < 3 )
		return error{"errortext":="Needs atleast three coordinates to form a polygon."};
	endif

	//var start_coord := coord_list[1];
	//var end_coord := coord_list[num_coords];

	return error{"errortext":="Hasn't been coded yet!"};
endfunction
