use uo;
use os;
use file;

CONST MS_REPORT_DISABLE	:= 0x0;
CONST MS_REPORT_CONSOLE	:= 0x1;
CONST MS_REPORT_SYSLOG		:= 0x2;
CONST MS_REPORT_BROADCAST	:= 0x4;
CONST MS_REPORT_ALL		:= 0x8;

function MS_ReportText(report_text:="?", flags:=MS_REPORT_CONSOLE)
	if ( !flags )
		return 0;
	endif
	
	if ( flags & MS_REPORT_CONSOLE )
		Print(""+report_text);
	endif
	
	if ( flags & MS_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		LogToFile("::log/magery.log", "["+script_name+"]: "+report_text, LOG_DATETIME);
	endif
	
	if ( flags & MS_REPORT_BROADCAST )
		Broadcast(""+report_text);
	endif
	
	return 1;
endfunction
