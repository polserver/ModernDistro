/*===============================================================
* Current Version
* YESNO.INC - v1.0
* Updated 9/14/2005 9:23PM
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
 * YesNo(mobile, prompt, yes_text, no_text)
 *
 * Purpose
 * Template to create a yes / no gump.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * prompt:	Primary text field, instructing the mobile on what the choices are about.
 * yes_text:	Text to be displayed next to the 'YES' button.
 * no_text:	The text to be displayed next to the 'NO' button.
 *
 * Return value
 * Returns '0' if no is selected.
 * Returns '1' if yes is selected.
 *
 */
function YesNo(mobile, prompt:="", yes_text:="Yes", no_text:="No")
	var yn_gump := GFCreateGump(140, 100);
	GFClosable(yn_gump, 0);
	GFResizePic(yn_gump, 0, 0, GetCfgConst("Defaults", "BackGround"), 400, 205);
	GFResizePic(yn_gump, 15, 15, GetCfgConst("Defaults", "ForeGround"), 370, 175);
	
	var y_pos := 20;
	foreach line in (GFWordWrap(prompt, 360))
		GFTextLine(yn_gump, 20, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	y_pos := 90;
	GFAddButton(yn_gump, 20, (y_pos+3), 2117, 2118, GF_CLOSE_BTN, 100);
	foreach line in (GFWordWrap(yes_text, 355));
		GFTextLine(yn_gump, 40, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
		
	y_pos := 140;
	GFAddButton(yn_gump, 20, (y_pos+3), 2117, 2118, GF_CLOSE_BTN);
	foreach line in (GFWordWrap(no_text, 355))
		GFTextLine(yn_gump, 40, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	var input := GFSendGump(mobile, yn_gump);
	input := input[0];
	if ( input == 100 )
		return 1;
	else
		return 0;
	endif	
endfunction


/* 
 * YesNo3(mobile, prompt, yes_text, no_text)
 *
 * Purpose
 * Template to create a yes / no gump. Bigger than the one above.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * prompt:	Primary text field, instructing the mobile on what the choices are about.
 * yes_text:	Text to be displayed next to the 'YES' button.
 * no_text:	The text to be displayed next to the 'NO' button.
 *
 * Return value
 * Returns '0' if no is selected.
 * Returns '1' if yes is selected.
 *
 */
function YesNo3(mobile, prompt:="", yes_text:="Yes", no_text:="No")
	var yn_gump := GFCreateGump(140, 100);
	GFClosable(yn_gump, 0);
	GFResizePic(yn_gump, 0, 0, GetCfgConst("Defaults", "BackGround"), 400, 405);
	GFResizePic(yn_gump, 15, 15, GetCfgConst("Defaults", "ForeGround"), 370, 375);
	
	var y_pos := 20;
	foreach line in (GFWordWrap(prompt, 360))
		GFTextLine(yn_gump, 20, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	y_pos := 300;
	GFAddButton(yn_gump, 20, (y_pos+3), 2117, 2118, GF_CLOSE_BTN, 100);
	foreach line in (GFWordWrap(yes_text, 355));
		GFTextLine(yn_gump, 40, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
		
	y_pos := 350;
	GFAddButton(yn_gump, 20, (y_pos+3), 2117, 2118, GF_CLOSE_BTN);
	foreach line in (GFWordWrap(no_text, 355))
		GFTextLine(yn_gump, 40, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	var input := GFSendGump(mobile, yn_gump);
	input := input[0];
	if ( input == 100 )
		return 1;
	else
		return 0;
	endif	
endfunction
