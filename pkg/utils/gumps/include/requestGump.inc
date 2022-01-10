/*===============================================================
* Current Version
* REQUESTGUMP.INC - v1.0
* Updated 9/14/2005 9:24PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;

include ":gumps:gumps";

CONST CANCEL_BTN_OFF	:= 0x0;
CONST CANCEL_BTN_ON	:= 0x1;

/*
 * RequestGump(mobile, question, option := "", pre_val := "", cancel_btn:=CANCEL_BTN_OFF, txt_lmt := 0)
 *
 * Purpose
 * Template to create a single field, single line, input gump.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * question:	Primary text field, instructing the mobile on what to enter.
 * option:	Optional text placed directly above the text entry area.
 * pre_val:	The text to display in the text entry field.
 * cancel_btn:	0 or CANCEL_BTN_OFF - No cancel button will be shown
 * 				1 or CANCEL_BTN_ON - A cancel button will be shown.
 *
 * Return value
 * Returns 'error' if the cancel button is clicked.
 * Returns 0 or the text that was entered.
 *
 */
function RequestGump(mobile, question, option := "", pre_val := "", cancel_btn:=CANCEL_BTN_OFF, txt_lmt := 0)

	if ( !pre_val || pre_val == error )
		pre_val := "";
	endif

	var r_gump := GFCreateGump(140, 100);
	GFClosable(r_gump, 0);

	GFResizePic(r_gump, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 400, 205);
	GFResizePic(r_gump, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 370, 175);

        GFAddAlphaRegion( r_gump, 0, 0, 400, 205 );

	var y_pos := 20;
	foreach line in (GFWordWrap(question, 370))
		GFTextLine(r_gump, 20, y_pos, 1153, line);
		y_pos += 20;
		SleepMS(2);
	endforeach

	y_pos := 75;
	foreach line in (GFWordWrap(option, 370))
		GFTextLine(r_gump, 20, y_pos, 1153, line);
		y_pos += 20;
		SleepMS(2);
	endforeach

	GFResizePic(r_gump, 20, 100, GFGetCfgConst("BackGrounds", "GOLDTRIM"), 360, 30);
	var r_txt;
	if ( !txt_lmt )
		r_txt := GFTextEntry(r_gump, 30, 105, 335, 25, 1153, pre_val);
	else
		r_txt := GFTextEntry(r_gump, 30, 105, 335, 25, 1153, pre_val, 0, txt_lmt);
  endif

	if ( cancel_btn )
		GFAddButton(r_gump, 90, 160, 2128, 2129, GF_CLOSE_BTN, 100);
		GFAddButton(r_gump, 240, 160, 2119, 2120, GF_CLOSE_BTN, 200);
	else
		GFAddButton(r_gump, 180, 160, 2128, 2129, GF_CLOSE_BTN, 100);
	endif

	var input := GFSendGump(mobile, r_gump);
	var text_area := GFExtractData(input, r_txt);

	if ( input[200] )
		return error;
	elseif ( !text_area || text_area == error )
		return 0;
	else
		return text_area;
	endif
endfunction

/*
 * RequestGumpEx (mobile, question, option := "", pre_val := "", cancel_btn:=CANCEL_BTN_OFF)
 *
 * Purpose
 * Template to create a single field, 3 line, input gump.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * question:	Primary text field, instructing the mobile on what to enter.
 * option:	Optional text placed directly above the text entry area.
 * pre_val:	The text to display in the text entry field.
 * cancel_btn:	0 - No cancel button will be shown
 * 		1 - A cancel button will be shown.
 *
 * Return value
 * Returns 'error' if the cancel button is clicked.
 * Returns 0 or the text that was entered.
 *
 */
function RequestGumpEx (mobile, question, option := "", pre_val := "", cancel_btn:=CANCEL_BTN_OFF)

	if ( !pre_val || pre_val == error )
		pre_val := "";
	endif

	var r_gump := GFCreateGump(140, 100);
	GFClosable(r_gump, 0);
	GFResizePic(r_gump, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 400, 205);
	GFResizePic(r_gump, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 370, 175);

	var y_pos := 20;
	foreach line in (GFWordWrap(question, 370))
		GFTextLine(r_gump, 20, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach

	y_pos := 75;
	foreach line in (GFWordWrap(option, 370))
		GFTextLine(r_gump, 20, y_pos, 1153, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach

	GFResizePic(r_gump, 20, 100, GFGetCfgConst("BackGrounds", "GOLDTRIM"), 360, 65);
	var r_txt := GFTextEntry(r_gump, 30, 105, 335, 50, 1153, pre_val);

	if ( cancel_btn )
		GFAddButton(r_gump, 90, 160, 2128, 2129, GF_CLOSE_BTN, 100);
		GFAddButton(r_gump, 240, 160, 2119, 2120, GF_CLOSE_BTN, 200);
	else
		GFAddButton(r_gump, 180, 160, 2128, 2129, GF_CLOSE_BTN, 100);
	endif

	var input := GFSendGump(mobile, r_gump);
	var text_area := GFExtractData(input, r_txt);

	if ( input[200] )
		return error;
	elseif ( !text_area || text_area == error )
		return 0;
	else
		return text_area;
	endif
endfunction
