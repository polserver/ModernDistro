use uo;
use os;
use file;

include ":security:reports_ex";

Const CM_REPORT_DISABLED	:= 0x0;
Const CM_REPORT_CONSOLE		:= 0x1;
Const CM_REPORT_SYSLOG		:= 0x2;
Const CM_REPORT_BROADCAST	:= 0x4;

function ContextMenu_ReportText( mobile, report_text:="?", flags:=CM_REPORT_CONSOLE )

	if( !flags )
		return 0;
	endif

	if( flags & CM_REPORT_CONSOLE )
		Print( report_text );
	endif

	if( flags & CM_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		VerifyDirectoryExists( "::log/contextMenus/" );
		LogToFile( "::log/contextMenus/contextLogFile.log", "Character: "+mobile.name+" ["+script_name+"]: "+report_text, LOG_DATETIME );
	endif

	if( flags & CM_REPORT_BROADCAST )
		Broadcast( report_text, 0x3, 0x32, 3 );
	endif

	return 1;
endfunction
