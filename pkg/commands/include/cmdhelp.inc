use uo;
use os;
use file;

include ":gumps:gumps";

function GetCmdHelp( byref mobile, byref help_file )

	var pid := GetObjProperty( mobile, "#HelpPid" );
	if( GetProcess( pid ) )
		CloseGump( mobile, pid, 0 );
	endif

	var script := Start_Script( ":commands:scripts/cmdhelpPrompt", { mobile, help_file } );
	if( script.errortext )
		SendSysMessage( mobile, "Unable to start ':commands:scripts/cmdhelpPrompt' -> "+script.errortext );
		return 0;
	endif
	SetObjProperty( mobile, "#HelpPid", script.pid );

	return 1;
endfunction

function SendCmdHelp( byref mobile, byref file_loc )

	var lines := ReadFile( file_loc );
	if( lines.errortext )
		SendSysMessage( mobile, "Could not access "+file_loc+"... ERROR -> "+lines.errortext, 0x3, 0x22 );
		return 0;
	endif

	var text := "";
	foreach line in ( lines )
		text += line+"<BR>";
	endforeach

	var gump := GFCreateGump( 50, 50 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9250, 300, 350 );
	GFPicTiled( gump, 13, 13, 274, 324, 3004 );
	GFHTMLArea( gump, 18, 18, 264, 287, text, 1, 1 );
	GFAddButton( gump, 135, 310, 4023, 4025, GF_CLOSE_BTN, 0 );

	GFSendGump( mobile, gump );
endfunction
