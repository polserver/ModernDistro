use uo;
use os;

include ":gumps:gumps";
include ":gumps:gumps_ex";

function EngravingGump( byref mobile, byref title )

	var gump := GFCreateGump( 50, 50 );

	GFResizePic( gump, 0, 0, 0xA28, 400, 300 );

	GFPage( gump, 0 );

	GFHTMLArea( gump, 0, 20, 400, 32, title, 0, 0 ); // Title Text
	GFHTMLArea( gump, 25, 45, 350, 145, "Please enter the text to add to the selected object.\n\nLeave the text area blank to remove the text from the object without using up the tool.", 1, 1 ); // Title Text

	GFAddButton( gump, 125, 250, 0x81A, 0x81B, GF_CLOSE_BTN, 33 );
	GFAddButton( gump, 220, 250, 0x819, 0x818, GF_CLOSE_BTN, 42 );

	GFPicTiled( gump, 25, 195, 350, 40, 0xDB0 );
	GFPicTiled( gump, 26, 195, 350, 2, 0x23C5 );
	GFPicTiled( gump, 25, 195, 2, 40, 0x23C3 );
	GFPicTiled( gump, 25, 235, 350, 2, 0x23C5 );
	GFPicTiled( gump, 375, 195, 2, 42, 0x23C3 );

	GFTextEntry( gump, 28, 196, 343, 37, 0x4FF, "", 15, 78 );

	var result := GFSendGump( mobile, gump ),
		text_entry := "";
	if( result[0] == 33 )
		text_entry := GFExtractData( result, 15 );
		if( text_entry.errortext || !text_entry )
			return "!";
		endif

		if( !VerifyStringName( text_entry ) )
			return 0;
		endif

		return CleanUpText( text_entry );
	endif

	return 0;
endfunction

function VerifyStringName( txt )

	var valid_chars := { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
						 "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
						 "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
						 "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
						 "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", " ", ",", ".",
						 "-", "_"
					   };
	var i := 0;
	for( i:=1; i<=Len( txt ); i:=i+1 )
		if( !( txt[i] in valid_chars ) )
			return 0;
		endif
	endfor

	txt := SplitWords( txt );
	if( !txt.Size() )
		return 0;
	endif

	return 1;
endfunction

function CleanUpText( byref txt )

	txt := SplitWords( txt );

	var i := 0,
		cleaned := "";

	for( i:=1; i<=txt.Size(); i:=i+1 )
		if( i<txt.Size() )
			cleaned := cleaned+txt[i]+" ";
		else
			cleaned := cleaned+txt[i];
		endif
	endfor

	return cleaned;
endfunction
