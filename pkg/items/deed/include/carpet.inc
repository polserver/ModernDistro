function CreateCarpet(who, x1, y1, x2, y2, carpet_z, carpet_color, carpet_type, carpet_deed, sign, carpet_tiles)

	if(who.multi)
		var lockdowns := GetObjProperty(sign, "RemainingLockdowns");

		if(lockdowns > 0)
			SetObjProperty(sign, "RemainingLockdowns", lockdowns - 1);
		else
			SendSysMessage(who, "You do not have encough lockdowns for this to be placed.");
		return;
		endif
	endif
  var i;
  var j;

  var piece_center;
  var piece_left;
  var piece_right;
  var piece_top;
  var piece_bottom;
  var piece_NW;
  var piece_NE;
  var piece_SW;
  var piece_SE;

case (carpet_type)
//Blue Gold carpet
  1:
	piece_center := 0x0AD1;
	piece_SE := 0x0AD2;
	piece_NW := 0x0AD3;
	piece_SW := 0x0AD4;
	piece_NE := 0x0AD5;
	piece_left := 0x0AD6;
	piece_top := 0x0AD7;
	piece_right := 0x0AD8;
	piece_bottom := 0x0AD9;

//Blue Red Carpet
  2:
	piece_center := 0x0AFA;
	piece_SE := 0x0AC2;
	piece_NW := 0x0AC3;
	piece_SW := 0x0AC4;
	piece_NE := 0x0AC5;
	piece_left := 0x0AF6;
	piece_top := 0x0AF7;
	piece_right := 0x0AF8;
	piece_bottom := 0x0AF9;

//Blue Stars Carpet
  3:
	piece_center := 0x0AC1;
	piece_SE := 0x0AC2;
	piece_NW := 0x0AC3;
	piece_SW := 0x0AC4;
	piece_NE := 0x0AC5;
	piece_left := 0x0AF6;
	piece_top := 0x0AF7;
	piece_right := 0x0AF8;
	piece_bottom := 0x0AF9;

//Blue Carpet
  4:
	piece_center := 0x0ABE;
	piece_SE := 0x0AC2;
	piece_NW := 0x0AC3;
	piece_SW := 0x0AC4;
	piece_NE := 0x0AC5;
	piece_left := 0x0AF6;
	piece_top := 0x0AF7;
	piece_right := 0x0AF8;
	piece_bottom := 0x0AF9;
//Orangish Red Carpet
  5:
	piece_center := 0x0AEB;
	piece_SE := 0x0AE3;
	piece_NW := 0x0AE4;
	piece_SW := 0x0AE5;
	piece_NE := 0x0AE6;
	piece_left := 0x0AE7;
	piece_top := 0x0AE8;
	piece_right := 0x0AE9;
	piece_bottom := 0x0AEA;

//Golden Brown Carpet
  6:
	piece_center := 0x0ADA;
	piece_SE := 0x0ADB;
	piece_NW := 0x0ADC;
	piece_SW := 0x0ADD;
	piece_NE := 0x0ADE;
	piece_left := 0x0ADF;
	piece_top := 0x0AE0;
	piece_right := 0x0AE1;
	piece_bottom := 0x0AE2;

//Orangish Blue Carpet
  7:
    piece_center := 0x0AED;
	piece_SE := 0x0AEE;
	piece_NW := 0x0AEF;
	piece_SW := 0x0AF0;
	piece_NE := 0x0AF1;
	piece_left := 0x0AF2;
	piece_top := 0x0AF3;
	piece_right := 0x0AF4;
	piece_bottom := 0x0AF5;

//Red Gold Stars Carpet
  8:
    piece_center := 0x0AC6;
	piece_SE := 0x0AC9;
	piece_NW := 0x0ACA;
	piece_SW := 0x0ACB;
	piece_NE := 0x0ACC;
	piece_left := 0x0ACD;
	piece_top := 0x0ACE;
	piece_right := 0x0ACF;
	piece_bottom := 0x0AD0;

//Red Carpet
  9:
    piece_center := 0x0AC8;
	piece_SE := 0x0AC9;
	piece_NW := 0x0ACA;
	piece_SW := 0x0ACB;
	piece_NE := 0x0ACC;
	piece_left := 0x0ACD;
	piece_top := 0x0ACE;
	piece_right := 0x0ACF;
	piece_bottom := 0x0AD0;

//Rugs
//Solid Grey Rug
  10:
	piece_center := 0x1098;
	piece_top := 0x1099;
	piece_right := 0x109A;
	piece_bottom := 0x109B;
	piece_left := 0x109C;
	piece_NW := 0x109D;
	piece_NE := 0x109E;
	piece_SE := 0x109F;
	piece_SW := 0x10A0;

//Solid Beige Rug
  11:
    piece_center := 0x0AA9;
	piece_top := 0x0AAA;
    piece_right := 0x0AAB;
    piece_bottom := 0x0AAC;
    piece_left := 0x0AAD;
	piece_NW := 0x0AAE;
    piece_NE := 0x0AAF;
    piece_SE := 0x0AB0;
	piece_SW := 0x0AB1;

//Solid Olive Green Rug
12:
    piece_center := 0x0AB3;
	piece_top := 0x0AB4;
	piece_right := 0x0AB5;
	piece_bottom := 0x0AB6;
	piece_left := 0x0AB7;
	piece_NW := 0x0AB8;
	piece_NE := 0x0AB9;
	piece_SE := 0x0ABA;
	piece_SW := 0x0ABB;
endcase

var piece;
var the_serials	:= {};

	for( i := x1+1; i <= x2-1; i := i + 1 )
		for( j := y1+1; j <= y2-1; j := j + 1 )
			piece := CreateItemAtLocation( i, j, carpet_z, piece_center, 1 );
			piece.color := carpet_color;
			piece.movable := 0;
			the_serials.Append ( piece.serial );
			SetObjProperty(piece, "deedserial", carpet_deed.serial);
			SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
			SetObjProperty(piece, "lockeddown", 1);
			SetObjProperty(piece, "lockdownsused", carpet_tiles);
		endfor
	endfor

	for( i := x1+1; i <= x2-1; i := i + 1 )
		piece := CreateItemAtLocation( i, y1, carpet_z, piece_top, 1 );
		piece.color := carpet_color;
		piece.movable := 0;
		the_serials.Append ( piece.serial );
		SetObjProperty(piece, "deedserial", carpet_deed.serial);
		SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
		SetObjProperty(piece, "lockeddown", 1);
		SetObjProperty(piece, "lockdownsused", carpet_tiles);
	endfor

	for( i := x1+1; i <= x2-1; i := i + 1 )
		piece := CreateItemAtLocation( i, y2, carpet_z, piece_bottom, 1 );
		piece.color := carpet_color;
		piece.movable := 0;
		the_serials.Append ( piece.serial );
		SetObjProperty(piece, "deedserial", carpet_deed.serial);
		SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
		SetObjProperty(piece, "lockeddown", 1);
		SetObjProperty(piece, "lockdownsused", carpet_tiles);
	endfor

	for( j := y1+1; j <= y2-1; j := j + 1 )
		piece := CreateItemAtLocation( x1, j, carpet_z, piece_left, 1 );
		piece.color := carpet_color;
		piece.movable := 0;
		the_serials.Append ( piece.serial );
		SetObjProperty(piece, "deedserial", carpet_deed.serial);
		SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
		SetObjProperty(piece, "lockeddown", 1);
		SetObjProperty(piece, "lockdownsused", carpet_tiles);
	endfor

	for( j := y1+1; j <= y2-1; j := j + 1 )
		piece := CreateItemAtLocation( x2, j, carpet_z, piece_right, 1 );
		piece.color := carpet_color;
		piece.movable := 0;
		the_serials.Append ( piece.serial );
		SetObjProperty(piece, "deedserial", carpet_deed.serial);
		SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
		SetObjProperty(piece, "lockeddown", 1);
		SetObjProperty(piece, "lockdownsused", carpet_tiles);
	endfor

	piece := CreateItemAtLocation( x1, y1, carpet_z, piece_NW, 1 );
	piece.color := carpet_color;
	piece.movable := 0;
	the_serials.Append ( piece.serial );
	SetObjProperty(piece, "deedserial", carpet_deed.serial);
	SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
	SetObjProperty(piece, "lockeddown", 1);
	SetObjProperty(piece, "lockdownsused", carpet_tiles);

	piece := CreateItemAtLocation( x1, y2, carpet_z, piece_SW, 1 );
	piece.color := carpet_color;
	piece.movable := 0;
	the_serials.Append ( piece.serial );
	SetObjProperty(piece, "deedserial", carpet_deed.serial);
	SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
	SetObjProperty(piece, "lockeddown", 1);
	SetObjProperty(piece, "lockdownsused", carpet_tiles);

	piece := CreateItemAtLocation( x2, y1, carpet_z, piece_NE, 1 );
	piece.color := carpet_color;
	piece.movable := 0;
	the_serials.Append ( piece.serial );
	SetObjProperty(piece, "deedserial", carpet_deed.serial);
	SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
	SetObjProperty(piece, "lockeddown", 1);
	SetObjProperty(piece, "lockdownsused", carpet_tiles);

	piece := CreateItemAtLocation( x2, y2, carpet_z, piece_SE, 1 );
	piece.color := carpet_color;
	piece.movable := 0;
	the_serials.Append ( piece.serial );
	SetObjProperty(piece, "deedserial", carpet_deed.serial);
	SetObjProperty(piece, "DeedObjType", carpet_deed.objtype);
	SetObjProperty(piece, "lockeddown", 1);
	SetObjProperty(piece, "lockdownsused", carpet_tiles);

	// Add multi part CProps.
	var carpet_pieces := {};
	for( i := x1+1; i <= x2-1; i := i + 1 )
		for( j := y1+1; j <= y2-1; j := j + 1 )
			carpet_pieces := ListItemsAtLocation( i, j, carpet_z, who.realm );
			foreach thing in carpet_pieces
				if(thing.IsCarpet())
					SetObjProperty(thing, "DeedOwnerSerial", who.serial);
					SetObjProperty(thing, "OtherItems", the_serials );
				endif
			endforeach
		endfor
	endfor
	for( i := x1+1; i <= x2-1; i := i + 1 )
		carpet_pieces := ListItemsAtLocation( i, y1, carpet_z, who.realm );
		foreach thing in carpet_pieces
			if(thing.IsCarpet())
				SetObjProperty(thing, "DeedOwnerSerial", who.serial);
				SetObjProperty(thing, "OtherItems", the_serials );
			endif
		endforeach
	endfor
	for( i := x1+1; i <= x2-1; i := i + 1 )
		carpet_pieces := ListItemsAtLocation( i, y2, carpet_z, who.realm );
		foreach thing in carpet_pieces
			if(thing.IsCarpet())
				SetObjProperty(thing, "DeedOwnerSerial", who.serial);
				SetObjProperty(thing, "OtherItems", the_serials );
			endif
		endforeach
	endfor
	for( j := y1+1; j <= y2-1; j := j + 1 )
		carpet_pieces := ListItemsAtLocation( x1, j, carpet_z, who.realm );
		foreach thing in carpet_pieces
			if(thing.IsCarpet())
				SetObjProperty(thing, "DeedOwnerSerial", who.serial);
				SetObjProperty(thing, "OtherItems", the_serials );
			endif
		endforeach
	endfor
	for( j := y1+1; j <= y2-1; j := j + 1 )
		carpet_pieces := ListItemsAtLocation( x2, j, carpet_z, who.realm );
		foreach thing in carpet_pieces
			if(thing.IsCarpet())
				SetObjProperty(thing, "DeedOwnerSerial", who.serial);
				SetObjProperty(thing, "OtherItems", the_serials );
			endif
		endforeach
	endfor

	carpet_pieces := ListItemsAtLocation( x1, y1, carpet_z, who.realm );
	foreach thing in carpet_pieces
		if(thing.IsCarpet())
			SetObjProperty(thing, "DeedOwnerSerial", who.serial);
			SetObjProperty(thing, "OtherItems", the_serials );
		endif
	endforeach
	carpet_pieces := ListItemsAtLocation( x1, y2, carpet_z, who.realm );
	foreach thing in carpet_pieces
		if(thing.IsCarpet())
			SetObjProperty(thing, "DeedOwnerSerial", who.serial);
			SetObjProperty(thing, "OtherItems", the_serials );
		endif
	endforeach
	carpet_pieces := ListItemsAtLocation( x2, y1, carpet_z, who.realm );
	foreach thing in carpet_pieces
		if(thing.IsCarpet())
			SetObjProperty(thing, "DeedOwnerSerial", who.serial);
			SetObjProperty(thing, "OtherItems", the_serials );
		endif
	endforeach
	carpet_pieces := ListItemsAtLocation( x2, y2, carpet_z, who.realm );
	foreach thing in carpet_pieces
		if(thing.IsCarpet())
			SetObjProperty(thing, "DeedOwnerSerial", who.serial);
			SetObjProperty(thing, "OtherItems", the_serials );
		endif
	endforeach

endfunction