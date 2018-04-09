use uo;
use os;
use file;

CONST ACCT_REPORT_DISABLE	:= 0x0;
CONST ACCT_REPORT_CONSOLE	:= 0x1;
CONST ACCT_REPORT_SYSLOG	:= 0x2;
CONST ACCT_REPORT_BROADCAST	:= 0x4;
CONST ACCT_REPORT_ALL		:= 0x7;

function ACCT_ReportText(report_text:="?", flags:=ACCT_REPORT_DISABLE)
	if ( !flags )
		return 0;
	endif
	
	if ( flags & ACCT_REPORT_CONSOLE )
		Print(""+report_text);
	endif
	
	if ( flags & ACCT_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		LogToFile("::log/accounts.log", "["+script_name+"]: "+report_text, LOG_DATETIME);
	endif
	
	if ( flags & ACCT_REPORT_BROADCAST )
		Broadcast(""+report_text);
	endif
	
	return 1;
endfunction
