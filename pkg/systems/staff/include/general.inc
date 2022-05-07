use uo;
use os;
use math;

function FormatByteAmount( total_bytes )

	if( total_bytes > 1000000000 )
		return CStr( CDbl( total_bytes/1073741824 )+" GB" );
	elseif( total_bytes > 1000000 )
		return CStr( CDbl( total_bytes/1048576 )+" MB" );
	elseif( total_bytes > 1000 )
		return CStr( CDbl( total_bytes / 1024 )+" KB" );
	endif

	return CStr( total_bytes+" Bytes" );
endfunction

function FormatSeconds( total_seconds )

	var time_weeks		:= Floor( total_seconds / 604800 ),
		time_days		:= Floor( total_seconds / 86400 ),
		time_hours		:= Floor( ( total_seconds % 86400 ) / 3600 ),
		time_minutes	:= Floor( ( total_seconds % 3600 ) / 60 ),
		time_seconds	:= Floor( ( total_seconds % 60 ) );

	if( time_weeks < 10 )
		time_weeks := "0"+time_weeks;
	endif

	if( time_days < 10 )
		time_days := "0"+time_days;
	endif

	if( time_hours < 10 )
		time_hours := "0"+time_hours;
	endif

	if( time_minutes < 10 )
		time_minutes := "0"+time_minutes;
	endif

	if( time_seconds < 10 ) then
		time_seconds := "0"+time_seconds;
	endif

	return time_weeks+":"+time_days+":"+time_hours+":"+time_minutes+":"+time_seconds;
endfunction

function FetchAccounts()

	var acct:=0,
		accounts:=ListAccounts(),
		i:=0,
		active:=0,
		banned:=0,
		jailed:=0;

	for( i:=1; i<=accounts.Size(); i+=1 )
		acct:=FindAccount( accounts[i] );
		if( acct.banned )
			banned+=1;
		elseif( !acct.enabled )
			jailed+=1;
		else
			active+=1;
		endif
	endfor

	return Struct{ "Active":=active, "Banned":=banned, "Jailed":=jailed };
endfunction
