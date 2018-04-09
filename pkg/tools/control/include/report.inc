use uo;
use os;
use file;

CONST ICP_REPORT_DISABLE	:= 0x0;
CONST ICP_REPORT_CONSOLE	:= 0x1;
CONST ICP_REPORT_SYSLOG		:= 0x2;
CONST ICP_REPORT_BROADCAST	:= 0x4;
CONST ICP_REPORT_ALL		:= 0x7;

function ICP_ReportText(report_text:="?", flags:=ICP_REPORT_DISABLE)
	if ( !flags )
		return 0;
	endif
	
	if ( flags & ICP_REPORT_CONSOLE )
		Print(""+report_text);
	endif
	
	if ( flags & ICP_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		LogToFile("::log/ICP.log", "["+script_name+"]: "+report_text, LOG_DATETIME);
	endif
	
	if ( flags & ICP_REPORT_BROADCAST )
		Broadcast(""+report_text);
	endif
	
	return 1;
endfunction
