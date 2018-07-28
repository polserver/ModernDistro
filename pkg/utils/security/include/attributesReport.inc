/*
        Maintained by The POL Distro Team

        2018-07-25

        A security in a serious project is a priority for the players.
        Thanks to the DistroTeam for the wonderfull idea.
*/
use uo;
use os;
use file;

CONST GAIN_REPORT_DISABLE	:= 0x0;
CONST GAIN_REPORT_CONSOLE	:= 0x1;
CONST GAIN_REPORT_SYSLOG	:= 0x2;
CONST GAIN_REPORT_BROADCAST	:= 0x4;
CONST GAIN_REPORT_ALL		:= 0x7;

function GAIN_ReportText( who, report_text:="?", flags:=GAIN_REPORT_DISABLE )

	if( !flags )
		return 0;
	endif

	if( flags & GAIN_REPORT_CONSOLE )
		Print( ""+report_text );
	endif

	if( flags & GAIN_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
                var acct := who.acctname;
                
                LogToFile( "::log/attributes/"+acct+".log", "Character: "+who.name+" ["+script_name+"]: "+report_text, LOG_DATETIME );
	endif

	if( flags & GAIN_REPORT_BROADCAST )
		Broadcast( ""+report_text );
	endif
	
	return 1;
endfunction
