/*
        Maintained by The POL Distro Team

        2018-07-25

*/
use uo;
use os;
use file;

CONST DMG_REPORT_DISABLE	:= 0x0;
CONST DMG_REPORT_CONSOLE	:= 0x1;
CONST DMG_REPORT_SYSLOG		:= 0x2;
CONST DMG_REPORT_BROADCAST	:= 0x4;
CONST DMG_REPORT_ALL		:= 0x7;

function DMG_ReportText( who, report_text:="?", flags:=DMG_REPORT_DISABLE )

	if( !flags )
		return 0;
	endif

	if( flags & DMG_REPORT_CONSOLE )
		Print( ""+report_text );
	endif

	if( flags & DMG_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
                var acct := who.acctname;

                LogToFile("::log/damage/"+acct+".log", "Character: "+who.name+" ["+script_name+"]: "+report_text, LOG_DATETIME );
	endif

	if( flags & DMG_REPORT_BROADCAST )
		Broadcast( ""+report_text );
	endif

	return 1;
endfunction
