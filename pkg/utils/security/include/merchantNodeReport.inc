use uo;
use os;
use file;

include ":security:reports_ex";

enum MN_ReportFlags
	MN_REPORT_DISABLED		:= 0x0,
	MN_REPORT_CONSOLE		:= 0x1,
	MN_REPORT_SYSLOG		:= 0x2,
	MN_REPORT_BROADCAST		:= 0x4
endenum

/*
 *
 * MerchantNode_ReportText( report_text, flags )
 *
 * Parameters:
 * report_text - String containing text to report, typically some error based info.
 * flags - Can contain any of the four flags found in the MN_ReportFlags.
 *
 * Purpose:
 * Simplifies how error reporting is done and where an error is sent.
 *
 * Date: 2022-10-16 12:54:18
 * Shard Name: Modern Distro
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * N/A
 *
 */
function MerchantNode_ReportText( report_text:="?", flags:=MN_REPORT_CONSOLE )

	if( !flags )
		return 0;
	endif

	if( flags & MN_REPORT_CONSOLE )
		Print( report_text );
	endif

	if( flags & MN_REPORT_SYSLOG )
		var script_name := GetProcess(GetPid()).name;
		VerifyDirectoryExists( "::log/merchantNode/" );
		LogToFile( "::log/merchantNode/merchantNodeLogFile.log", "["+script_name+"]: "+report_text, LOG_DATETIME );
	endif

	if( flags & MN_REPORT_BROADCAST )
		Broadcast( report_text, 0x3, 0x32, 3 );
	endif

	return 1;
endfunction
