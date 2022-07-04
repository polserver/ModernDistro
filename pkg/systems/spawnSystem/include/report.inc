/* $Id$
 *
 */

use uo;
use os;
use file;

CONST SPAWN_REPORT_DISABLE	:= 0x0;
CONST SPAWN_REPORT_CONSOLE	:= 0x1;
CONST SPAWN_REPORT_SYSLOG	:= 0x2;
CONST SPAWN_REPORT_BROADCAST	:= 0x4;
CONST SPAWN_REPORT_ALL		:= 0x7;

function SS_ReportText(report_text:="?", flags:=SPAWN_REPORT_DISABLE)
	if ( flags & SPAWN_REPORT_CONSOLE )
		Print(""+report_text);
	endif

	if ( flags & SPAWN_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		LogToFile("::log/spawnSystem.log", "["+script_name+"]: "+report_text, LOG_DATETIME);
	endif

	if ( flags & SPAWN_REPORT_BROADCAST )
		foreach mobile in EnumerateOnlineCharacters()
			if ( mobile.cmdlevel >= 2 )
				SendSysMessage(mobile, ""+report_text);
			endif
			SleepMS(5);
		endforeach
	endif

	return 1;
endfunction
