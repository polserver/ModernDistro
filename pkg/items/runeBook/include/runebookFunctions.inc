function CreateRunebookGump( who, book )

	var rune_list := GetRuneList( book );
	if( TypeOf( rune_list ) != "Array" )
		rune_list := array{};
		SetObjProperty( book, "%RuneList%", rune_list );
	endif

	var charge_prop := GetObjProperty( book, "%Charges%" );
	var charges := charge_prop["charges"];
	if( !charges )
		charges := 0;
	endif

	var max_charges := charge_prop["max_charges"];
	if( !max_charges )
		max_charges := 5;
	endif

	var book_gump := GFCreateGump(150, 200);

	GFGumpPic( book_gump, 100, 10, 2200);
  var g, h, xOffset;
  for ( g:=0; g<2; g+=1 )
    xOffset := 125 + ( g * 165 );
    GFGumpPic ( book_gump, xOffset, 50, 57 );
    xOffset += 20;
    for ( h:=0; h<6; h+=1 )
      GFGumpPic ( book_gump, xOffset, 50, 58 );
      xOffset += 15;
    endfor
    GFGumpPic ( book_gump, xOffset - 5, 50, 59 );
  endfor

	GFAddButton( book_gump, 130, 187, 2225, 2225, GF_PAGE_BTN, 2 );
	GFAddButton( book_gump, 165, 187, 2226, 2226, GF_PAGE_BTN, 3 );
	GFAddButton( book_gump, 200, 187, 2227, 2227, GF_PAGE_BTN, 4 );
	GFAddButton( book_gump, 235, 187, 2228, 2228, GF_PAGE_BTN, 5 );
	GFAddButton( book_gump, 300, 187, 2229, 2229, GF_PAGE_BTN, 6 );
	GFAddButton( book_gump, 335, 187, 2230, 2230, GF_PAGE_BTN, 7 );
	GFAddButton( book_gump, 370, 187, 2231, 2231, GF_PAGE_BTN, 8 );
	GFAddButton( book_gump, 405, 187, 2232, 2232, GF_PAGE_BTN, 9 );

  GFAddHTMLLocalized ( book_gump, 140, 40, 80, 18, 1011296, 0, 0); // Charges:
  GFHTMLArea( book_gump, 220, 40, 30, 18, charges );
  
  GFAddHTMLLocalized ( book_gump, 300, 40, 100, 18, 1011297, 0, 0); // Max Charges:
  GFHTMLArea( book_gump, 400, 40, 30, 18, max_charges );
	
	

	GFPage( book_gump, 1 );

	GFAddButton( book_gump, 125, 15, 2472, 2473, GF_CLOSE_BTN, 50 ); 
  GFAddHTMLLocalized ( book_gump, 158, 22, 100, 18, 1011299, 0, 0); // Rename book
	GFAddButton( book_gump, 393, 14, 2206, 2206, GF_PAGE_BTN, 2 );

	var pOX := 145;
	var pOY := 60;
	var charge_button    := 101;
	var recall_button    := 201;
	var gate_button      := 301;
	var sacred_button    := 401;
  var drop_rune_button := 501;
	var set_default_rune := 601;
  
	var rune_pos         := 1;
        var rune_use         := 101;

	for i := 2 to 9
		GFPage( book_gump, i );
		GFAddButton( book_gump, 125, 14, 2205, 2205, GF_PAGE_BTN, ( i-1 ));
		if( i != 9 )
			GFAddButton( book_gump, 393, 14, 2206, 2206, GF_PAGE_BTN, ( i+1 ));
		endif

		var mapcolor := 0;
    var rune_entry := rune_list[rune_pos];
		if( TypeOf( rune_entry ) == "Struct" )
			if( rune_entry.name )
				// Adds color to rune name in book based on facet location
            if ( rune_entry.realm == "britannia_alt" )
                mapcolor := 10;
            elseif ( rune_entry.realm == "britannia" )
                mapcolor := 81;
            elseif ( rune_entry.realm == "ilshenar" )
                mapcolor := 1280;
            elseif ( rune_entry.realm == "malas" )
                mapcolor := 1102;
            elseif ( rune_entry.realm == "tokuno" )
                mapcolor := 1154;
            elseif ( rune_entry.realm == "termur" )
                mapcolor := 1645;		
            else
                mapcolor := 0;
            endif

				GFPage( book_gump, 1 );
				GFTextCrop( book_gump, pOX,  pOY, 115, 17, mapcolor, rune_entry.name );
        GFAddButton( book_gump, pOX-15, pOY+5,  2103, 2104, GF_CLOSE_BTN, rune_use );
				GFPage( book_gump, i );

				GFAddButton( book_gump, 130, 65, 2103, 2104, GF_CLOSE_BTN, charge_button );
				GFTextCrop( book_gump, 145, 60, 115, 17, mapcolor, rune_entry.name );
				GFTextLine( book_gump, 135, 80, 0, ConvertLatLong( rune_entry.x, rune_entry.y, 1 ));
				GFTextLine( book_gump, 135, 95, 0, ConvertLatLong( rune_entry.x, rune_entry.y, 2 ));

				GFAddButton( book_gump, 135, 115, 2437, 2438, GF_CLOSE_BTN, drop_rune_button );
        GFAddHTMLLocalized ( book_gump, 150, 115, 100, 18, 1011298, 0, 0); // Drop Rune
				
        GFAddButton( book_gump, 160, 20, 2361, 2361, GF_CLOSE_BTN, set_default_rune );
        GFAddHTMLLocalized ( book_gump, 175, 15, 100, 18, 1011300, 0, 0); // Set default
        
				GFAddButton ( book_gump, 135, 140, 2103, 2104, GF_CLOSE_BTN, recall_button );
        GFAddHTMLLocalized ( book_gump, 150, 136, 110, 20, 1062722, 0, 0); // Recall
        
				GFAddButton ( book_gump, 135, 158, 2103, 2104, GF_CLOSE_BTN, gate_button );
        GFAddHTMLLocalized ( book_gump, 150, 154, 110, 20, 1062723, 0, 0); // Gate Travel
        
        GFAddButton ( book_gump, 135, 176, 2103, 2104, GF_CLOSE_BTN, sacred_button );
        GFAddHTMLLocalized ( book_gump, 150, 172, 110, 20, 1062724, 0, 0); // Sacred Journey

				charge_button += 1;
				drop_rune_button += 1;
				set_default_rune += 1;
				recall_button += 1;
				gate_button += 1;
        sacred_button += 1;
        rune_use += 1;
			endif
		endif

    rune_pos += 1;
		pOY += 15;

		rune_entry := rune_list[rune_pos];
		if( TypeOf( rune_entry ) == "Struct" )
			if( rune_entry.name )
				// Adds color to rune name in book based on facet location
            if ( rune_entry.realm == "britannia_alt" )
                mapcolor := 10;
                SendSysMessage(who, ""+rune_entry.realm);
            elseif ( rune_entry.realm == "britannia" )
                mapcolor := 81;
            elseif ( rune_entry.realm == "ilshenar" )
                mapcolor := 1280;
            elseif ( rune_entry.realm == "malas" )
                mapcolor := 1102;
            elseif ( rune_entry.realm == "tokuno" )
                mapcolor := 1154;
            elseif ( rune_entry.realm == "termur" )
                mapcolor := 1645;		
            else
                mapcolor := 0;
            endif

				GFPage( book_gump, 1);
				GFTextCrop( book_gump, pOX, pOY, 115, 17, mapcolor, rune_entry.name );
        GFAddButton( book_gump, pOX-15, pOY+5, 2103, 2104, GF_CLOSE_BTN, rune_use );
				GFPage( book_gump, i);

				GFAddButton( book_gump, 290, 65, 2103, 2104, GF_CLOSE_BTN, charge_button );
				GFTextCrop( book_gump, 305, 60, 115, 17, mapcolor, rune_entry.name );
				GFTextLine( book_gump, 295, 80, 0, ConvertLatLong( rune_entry.x, rune_entry.y, 1 ));
				GFTextLine( book_gump, 295, 95, 0, ConvertLatLong( rune_entry.x, rune_entry.y, 2 ));

				GFAddButton( book_gump, 295, 115, 2437, 2438, GF_CLOSE_BTN, drop_rune_button );
        GFAddHTMLLocalized ( book_gump, 310, 115, 100, 18, 1011298, 0, 0); // Drop Rune
        
				GFAddButton( book_gump, 300, 20, 2361, 2361, GF_CLOSE_BTN, set_default_rune );
				GFAddHTMLLocalized ( book_gump, 315, 15, 100, 18, 1011300, 0, 0); // Set default
        
				GFAddButton ( book_gump, 295, 140, 2103, 2104, GF_CLOSE_BTN, recall_button );
        GFAddHTMLLocalized ( book_gump, 310, 136, 110, 20, 1062722, 0, 0); // Recall
        
				GFAddButton ( book_gump, 295, 158, 2103, 2104, GF_CLOSE_BTN, gate_button );
        GFAddHTMLLocalized ( book_gump, 310, 154, 110, 20, 1062723, 0, 0); // Gate Travel
        
        GFAddButton ( book_gump, 295, 176, 2103, 2104, GF_CLOSE_BTN, sacred_button );
        GFAddHTMLLocalized ( book_gump, 310, 172, 110, 20, 1062724, 0, 0); // Sacred Journey
				
        charge_button += 1;
				drop_rune_button += 1;
				set_default_rune += 1;
				recall_button += 1;
				gate_button += 1;
        sacred_button += 1;
        rune_use += 1;
			endif
		endif

    rune_pos += 1;
		pOY += 15;
		if( pOY > 175 )
			pOY := 60;
			pOX := 305;
		endif

		SleepMS(2);
	endfor

	return book_gump;
endfunction

function SetBookName( book, who, auto:=0 )

	var charge_prop := GetObjProperty( book, "%Charges%" );
	var charges := charge_prop["charges"];
	if( !charges )
		charges := 0;
	endif

	var name := GetObjProperty( book, "Name" );
	if( !name )
		name := "a runebook";
	endif

	if( !auto )
    SendSysMessageCL ( who, 502414, color := 66 ); // Please enter a title for the runebook:
		name := RequestInput ( who, book, "" );
		if( !name )
			name := "a runebook";
      SendSysMessageCL( who, 1042021, color := 0x21 ); //Cancelled
		endif
	endif

	SetObjProperty( book, "Name", CStr( name ));
	SetName( book, name+" [charges: "+charges+"]" );

	return 1;
endfunction

function SetCharges( book, who, charges )

	var charge_prop := GetObjProperty( book, "%Charges%" );
	charge_prop["charges"] := CInt( charges );
	SetObjProperty( book, "%Charges%", charge_prop );
	SetBookName( book, who, 1 );

	return 1;
endfunction

function GetRuneList( book )

	var rune_list := GetObjProperty( book, "%RuneList%" );
	if( TypeOf( rune_list ) != "Array" )
		rune_list := array{};
		SetObjProperty( book, "%RuneList%", rune_list );
	endif

	return rune_list;
endfunction

function GetRuneDetails( book, rune_pos )

	var rune_list := GetRuneList( book );
	if( TypeOf( rune_list ) != "Array" )
		rune_list := array{};
		SetObjProperty( book, "%RuneList%", rune_list );
	endif

	return rune_list[rune_pos];
endfunction

function ConvertLatLong( x, y, pos )

	var workit := ToSextant( x, y );
	var j, newtemp := "";
	for( j:=1; j<=Len( workit ); j:=j+1 )
		if( workit[j] == " " )
			workit[j] := "_";
		elseif( workit[j] == "," )
			workit[j] := " ";
			workit[j+1] := "";
		endif
                SleepMS(5);
	endfor

	newtemp := SplitWords( workit );
	if( pos == 1 )
		newtemp := newtemp[1];
		for( j:=1; j<=Len( newtemp ); j:=j+1 )
			if( newtemp[j] == "_" )
				newtemp[j] := " ";
			endif
                        SleepMS(5);
		endfor
		return newtemp;
	else
		newtemp := newtemp[2];
		for( j:=1; j<=Len( newtemp ); j:=j+1 )
			if( newtemp[j] == "_" )
				newtemp[j] := " ";
			endif
                        SleepMS(5);
		endfor
		return newtemp;
	endif
endfunction

function DeleteRune( book, rune_pos )

	var rune_list := GetRuneList( book );
	if( TypeOf( rune_list ) != "Array" )
		rune_list := array{};
		SetObjProperty( book, "%RuneList%", rune_list );
	endif

	if( rune_list[rune_pos] )
		rune_list.Erase( rune_pos );
		SetObjProperty( book, "%RuneList%", rune_list );
	endif
	return 1;
endfunction

function SetDefault( book, who, rune_pos )

	var rune_info := GetRuneDetails( book, rune_pos );
	SetObjProperty( book, "Destination", rune_info );
	SendSysMessage( who, "Default rune set to: "+rune_info.name );

	return 1;
endfunction

function ToSextant( x, y )

	var latitude_direction, longitude_direction;
	var latitude_degrees, longitude_degrees;
	var latitude_minutes, longitude_minutes;

	latitude_degrees := ( y-1624 )*0.087890625;
	longitude_degrees := ( x-1323 )*0.0703125;

	if( latitude_degrees < 0 )
		latitude_direction := "N";
	else
		latitude_direction := "S";
	endif

	if( longitude_degrees < 0 )
		longitude_direction := "W";
	else
		longitude_direction := "E";
	endif

	latitude_degrees := Abs( latitude_degrees );
	longitude_degrees := Abs( longitude_degrees );
	latitude_minutes := CInt(( CInt(( latitude_degrees-Floor( latitude_degrees )) * 100 ) * 60 ) / 100 );
	longitude_minutes := CInt(( CInt(( longitude_degrees-Floor( longitude_degrees )) * 100 ) * 60 ) / 100 );

	return( Floor( latitude_degrees )+"° "+latitude_minutes+"'"+latitude_direction+", "+Floor( longitude_degrees )+"° "+longitude_minutes+"'"+longitude_direction );
endfunction
