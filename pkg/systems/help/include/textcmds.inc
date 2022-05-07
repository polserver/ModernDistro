use uo;
use os;
use file;

include "include/arrays";
include ":gumps:gumps";

//////////////////////////////////////////////////////////////////////////////////////////
//
// SendCmdHelpWindow( byref mobile, byref file_loc )
//
// Purpose: To Display a command help file in a HTML gunp.
//			The file is a plain text file with a chf (Command Help File) extension.
//			The chf files are plain text files.
//
// Example usage from a script:
// In your script's includes block enter include ":help:textcmds";
//	program TextCommand( who, text )
//		if( (text == Lower( "help" )) || (text == Lower( "h" )) || (text == "?") )
//			SendCmdHelpWindow( who, ":commands:gm/textcommand.chf" );
//			return;
//		endif
//
// Parameter:	mobile::mobile ref
//				file_loc::string containing the filespec
//				Example: If your text command is named boats and is
//				in the boats package and your help file in
//				the same package is named boats.chf, the filespec
//				would be ":boats:boats.chf"
//
// Returns:		Nothing
//
//////////////////////////////////////////////////////////////////////////////////////////
function SendCmdHelpWindow( byref mobile, byref file_loc )

	var text_lines := ReadFile( file_loc ),
		text := "";
	if ( text_lines.errortext )
		text := "Could not access "+file_loc+"... ERROR -> "+text_lines.errortext;
		Print(text);
	endif

	text := ImplodeArray("\n", text_lines);
	if( !text )
		text := "There is no help info for this command defined.";
	endif

	var gump := GFCreateGump( 50, 50 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9250, 300, 350 );
	GFPicTiled( gump, 13, 13, 274, 324, 3004 );
	GFHTMLArea( gump, 18, 18, 264, 287, text, 1, 1 );
	GFAddButton( gump, 135, 310, 4023, 4025, GF_CLOSE_BTN, 0 );

	GFSendGump( mobile, gump );
endfunction
