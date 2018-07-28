/*
        Maintained by The POL Distro Team

        2018-07-25

*/
use uo;
use os;
use file;

CONST CMD_REPORT_DISABLE	:= 0x0;
CONST CMD_REPORT_CONSOLE	:= 0x1;
CONST CMD_REPORT_SYSLOG		:= 0x2;
CONST CMD_REPORT_BROADCAST	:= 0x4;
CONST CMD_REPORT_ALL		:= 0x7;

function CMD_ReportText( who, report_text:="?", flags:=CMD_REPORT_DISABLE )

	if( !flags )
		return 0;
	endif
	
	if( flags & CMD_REPORT_CONSOLE )
		Print( ""+report_text );
	endif
	
	if( flags & CMD_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
                var acct := who.acctname;
                if( who.cmdlevel >= 1 )
		           LogToFile( "::log/commands/staff/"+acct+".log", "Character: "+who.name+" ["+script_name+"]: "+report_text, LOG_DATETIME );
                else
                           LogToFile( "::log/commands/"+acct+".log", "Character: "+who.name+" ["+script_name+"]: "+report_text, LOG_DATETIME );
                endif
	endif
	
	if( flags & CMD_REPORT_BROADCAST )
		Broadcast( ""+report_text );
	endif
	
	return 1;
endfunction
