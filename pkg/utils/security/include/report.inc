/*
        Created by *Edwards

        2011-04-02

        A security in a serious project is a priority for the players.
        Thanks to the DistroTeam for the wonderfull idea.
*/
use uo;
use os;
use file;
use polsys;

include "include/string";

CONST REPORT_DISABLE	:= 0x0;
CONST REPORT_CONSOLE	:= 0x1;
CONST REPORT_SYSLOG	:= 0x2;
CONST REPORT_BROADCAST	:= 0x4;
CONST REPORT_ALL	:= 0x7;

function ReportText( report_text:="?", flags:=REPORT_DISABLE, text_color:=0 )

	if( !flags )
		return 0;
	elseif( flags & REPORT_CONSOLE )
		Print( ""+report_text );
	elseif( flags & REPORT_SYSLOG )
		var script_name := CStr( GetProcess( GetPid() ).name ),
                    pkg_name := GetPkgName( script_name );
                    
		LogToFile( "::log/"+pkg_name+".log", "["+script_name+"]: "+report_text, LOG_DATETIME );
	elseif( flags & REPORT_BROADCAST )
		Broadcast( ""+report_text, 3, text_color );
	endif
	
	return 1;
endfunction

function GetPkgName( script_name )

        var i;
        for( i:=1; i<=len(script_name); i+=1 )
                if( !Check_ValidChar( script_name[i] ))
                        script_name[i] := " ";
                endif

                SleepMS(5);
        endfor

        foreach poss_pkg in ( SplitWords( script_name ))
                if( GetPackageByName( poss_pkg ))
                        return poss_pkg;
                endif
                SleepMS(5);
        endforeach

        return 0;
endfunction
