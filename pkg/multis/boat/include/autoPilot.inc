use os;
use math;

// Bresenham's line drawing algorithm. Determines a line between two waypoints.
// Used here to get the next coordinate to move.

function BresenhamLine( x1, y1, x2, y2 )

	x1 := CInt( x1 ); y1 := CInt( y1 );
	x2 := CInt( x2 ); y2 := CInt( y2 );

	var dx := Abs( x2 - x1 )+0.5;
	var dy := Abs( y2 - y1 )+0.5;
	var vx := 0, vy := 0;

	if( x2 > x1 )
		vx := 1;
	elseif( x2 < x1 )
		vx := -1;
	endif

	if( y2 > y1 )
		vy := 1;
	elseif( y2 < y1 )
		vy := -1;
	endif

	var point := struct{"x":=0, "y":=0};

	if( x1 == x2 && y1 == y2 )
		point.x := x1;
		point.y := y1;
		return point;
	endif

	if( dx >= dy )
		dy := CDbl( dy ) / CDbl( dx );
		for loop := 0 to 1
			// 0 = current cordinate
			// 1 = next coordinate
			point.x := x1 + ( loop * vx );
			point.y := CDbl( loop * vy * dy );
			if( point.y - Floor( point.y ) >= 0.5 )
				point.y := Ceil( point.y );
			endif
			point.y := CInt( y1 + point.y );
		endfor
		return point;
	else // dx < dy
		dx := CDbl( dx ) / CDbl( dy );
		for loop := 0 to 1
			// 0 = current cordinate
			// 1 = next coordinate
			point.x := CDbl( loop * vx * dx );
			if( point.x - Floor( point.x ) >= 0.5 )
				point.x := Ceil( point.x );
			endif
			point.x := CInt( x1 + point.x );
			point.y := y1 + ( loop * vy );
		endfor

		return point;
	endif
endfunction

// convert facing 0-7 to turn code 0 - 3
function DetermineTurnCode( boat, desired )

	var current := CInt( boat.facing / 2 );
	desired := CInt( desired / 2 );

	if( desired < current )
		desired := desired + 4;
	endif

	return ( desired - current );
endfunction
