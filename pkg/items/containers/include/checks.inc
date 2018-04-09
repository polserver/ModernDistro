use uo;
use os;

function CP_IsLocked( container )

	if( container.locked )
		return 1;
	elseif( CP_RootContainerLocked( container ))
		return 1;
	else
		return 0;
	endif
endfunction

function CP_RootContainerLocked( object )

	var temp := object;
	while( temp.container )
		if(( temp.container ).locked )
			return 1;
		else
			// Move onto checking the next container.
			temp := temp.container;
		endif
		SleepMS(2);
	endwhile

	return 0;
endfunction
