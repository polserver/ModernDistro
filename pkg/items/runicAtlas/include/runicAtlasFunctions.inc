use cliloc;

function SetBookName( book, who, auto:=0 )
	if ( book.movable == 0 )
		SendSysMessageCL ( who, 502413, color := 33 ); // That cannot be done while the book is locked down.
		return 0;
	endif
	
	var charge_prop := GetObjProperty ( book, "%Charges%" );
	var charges := charge_prop["charges"];
	if( !charges )
		charges := 0;
	endif

	var getname := GetObjProperty ( book, "Name" );
	if( !getname )
		getname := "a runic atlas";
	endif

	if( !auto )
    SendSysMessageCL ( who, 502414, color := 66 ); // Please enter a title for the runebook:
		getname := RequestInput ( who, book, "" );
		if( !getname )
			getname := "a runic atlas";
		endif
	endif

	SetObjProperty( book, "Name", CStr( getname ));
	SetName( book, getname+" [charges: "+charges+"]" );
  SendSysMessageCL ( who, 1041531, color := 66 ); // You have changed the title of the rune book.
	return 1;
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
