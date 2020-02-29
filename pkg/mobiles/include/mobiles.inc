
function GetRelativeFacingCoords(dir)
	// Return relative X coordinate of square in front of mobile based on Facing (dir)
	var x_coord := CInt(Sin(dir*0.78)*1.5);
	// Return relative Y coordinate of square in front of mobile based on Facing (dir)
	var y_coord := CInt(Cos(dir*0.78)*-1.5);
	return {x_coord, y_coord};
endfunction

// Alternate formulae for above.
// Need to test both for efficiency.
// Return relative X coordinate based on Facing (dir)
// If (facing & 3 == 0)
  // return 0;
// ElseIf (facing & 4 == 4)
  // return -1;
// Endif
// Return 1;

// Return relative Y coordinate based on Facing (dir)
// facing := (facing + 1) & 7;
// if (facing & 3 == 3)
  // return 0;
// ElseIf (facing & 4 == 4)
   // return 1;
// EndIf
// return -1;