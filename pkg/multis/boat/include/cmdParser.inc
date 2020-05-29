use os;

function ParseCommands( byref text, byref cmd_keys )

	text := Lower( text );
	var cmd_order := array{};

	var match := 0, pos := 0;
	foreach cmd in ( cmd_keys )
		pos := pos + 1;
		cmd := Lower( cmd );
		while( text[cmd] )
			text[cmd] := "[["+CStr( pos )+"]] ";
			match := 1;
			SleepMS(2);
		endwhile
		SleepMS(2);
	endforeach

	if( match == 0 )
		return cmd_order; // Empty array
	endif

	text := SplitWords( text );
	foreach cmd in ( text )
		if( cmd["[["] && cmd["]]"] )
			cmd["[["] := "";
			cmd["]]"] := "";
			cmd := cmd_keys[CInt( cmd )];
			if( cmd )
				cmd_order.Append( cmd );
			endif
		endif
		SleepMS(2);
	endforeach

	text := "";

	return cmd_order;
endfunction
