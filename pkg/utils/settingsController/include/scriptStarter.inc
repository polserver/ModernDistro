use uo;
use os;


function startControlScript( scriptString, params := 0 )
	var caller := GetProcess();
	var script := Start_Script( scriptString, params );
	if( !script.errortext )
		if( script.package.name )
			SysLog("["+script.pid+"]Started Control: "+script.package.name);
		else
			SysLog("["+script.pid+"]Started Control for system script");
		endif
	else
		SysLog("Script '"+scriptString+"' failed to start with error: "+script.errortext);
	endif
endfunction
