/*===============================================================
* Current Version
* GUMPPROMPT.INC - v1.0
* Updated 9/14/2005 9:24PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;

include ":gumps:gumps";
include ":gumps:gumps_ex";

/*
 * GumpPrompt(mobile, text, movable:=0, hue:=1153)
 *
 * Purpose
 * Template to create a simple prompt window with a message and an "OKAY" button.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * text:	Text to display on the prompt gump.
 * movable:	0 - Dialog will not be movable
 *		1 - Dialog will be movable.
 * hue:     colour of the text.
 *
 * Return value
 * Returns 1 when the okay button has been clicked.
 *
 */
function GumpPrompt(mobile, text, movable:=0, hue:=1153)
	var gump_prompt := GFCreateGump( 180, 100 );
	if( !movable )
		GFClosable( gump_prompt, 0 );
	endif
	GFResizePic( gump_prompt, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 300, 220 );
	GFResizePic( gump_prompt, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 270, 190 );

	GFHTMLArea( gump_prompt, 20, 20, 260, 154, text, 1, 1 );
	// var y_pos := 25;
	// foreach line in ( GFWordWrap( text, 260 ) )
	// 	GFTextLine( gump_prompt, 20, y_pos, hue, line );
	// 	y_pos := y_pos+20;
	// 	SleepMS( 2 );
	// endforeach

	GFAddButton( gump_prompt, 120, 179, 2128, 2129, GF_CLOSE_BTN, 1 );

	var prompt_input := GFSendGump( mobile, gump_prompt );

	return prompt_input[0];
endfunction

/*
 * GumpPromptEx(mobile, text, movable:=0, hue:=1153)
 *
 * Purpose
 * Template to create a simple prompt window with a message and an "OKAY" button.
 * Same as GumpPrompt but with a different, darker, border and background.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * text:	Text to display on the prompt gump.
 * movable:	0 - Dialog will not be movable
 *		1 - Dialog will be movable.
 * hue:     colour of the text.
 *
 * Return value
 * Returns 1 when the okay button has been clicked.
 *
 */
function GumpPromptEx(mobile, text, movable:=0, hue:=1153)
	var gump_prompt := GFCreateGump(180, 100);
	if ( !movable )
		GFClosable(gump_prompt, 0);
	endif
	GFResizePic(gump_prompt, 0, 0, GFGetCfgConst("Defaults", "BackGround2"), 300, 160);
	GFResizePic(gump_prompt, 15, 15, GFGetCfgConst("Defaults", "ForeGround2"), 270, 130);

	var y_pos := 25;
	foreach line in (GFWordWrap(text, 260))
		GFTextLine(gump_prompt, 40, y_pos, hue, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach

	GFAddButton(gump_prompt, 120, 120, 2128, 2129, GF_CLOSE_BTN, 1);

	var prompt_input := GFSendGump(mobile, gump_prompt);
	return prompt_input[0];
endfunction
