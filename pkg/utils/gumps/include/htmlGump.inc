/*===============================================================
* Current Version
* HTMLGUMP.INC - v1.0
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
 * SendHTMLMessage(who, text)
 *
 * Purpose
 * Sends a HTML gump containing the text to the character.
 *
 * Parameters
 * who:     Reference to the character which should receive the gump
 * text:    The text to send, may contain HTML tags
 *
 * Return value
 * The gump's return value
 *
 */
function SendHTMLGump(mobile, title, text)
	var gump := GFCreateGump();
	GFResizePic(gump, 0, 0, GetCfgConst("Defaults", "BackGround"), 360, 480);

	GFResizePic(gump, 15, 12, GetCfgConst("Defaults", "ForeGround"), 330, 30);
	GFTextMid(gump, 15, 17, 330, 2100, title);

	GFResizePic(gump, 15, 45, GetCfgConst("Backgrounds", "GOLDTRIM"), 330, 390);
	GFHTMLArea(gump, 22, 53, 322, 375, text, 0, 1);
	
	GFResizePic(gump, 15, 438, GetCfgConst("Defaults", "ForeGround"), 330, 30);
	GFAddButton(gump, 150, 443, 2128, 2129, GF_CLOSE_BTN);
	
	return GFSendGump(mobile, gump);
endfunction
