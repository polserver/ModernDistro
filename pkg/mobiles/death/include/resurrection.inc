/* $Id: resurrection.inc 636 2006-08-01 09:37:49Z austinheilman $
 *
 */
use uo;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include "include/client";

function ResurrectionGump(mobile)
	var gump := GFCreateGump();

	GFResizePic(gump, 0, 0, GFGetCfgConst("Defaults", "BackGround"), 400, 175);
	GFResizePic(gump, 15, 15, GFGetCfgConst("Defaults", "ForeGround"), 370, 145);

	GFTilePic(gump, 0, 20, 2);
	GFTilePic(gump, 20, 20, 3);

	GFTilePic(gump, 335, 20, 4);
	GFTilePic(gump, 355, 20, 5);

	GFTextMid(gump, 15, 45, 370, 2100, "Resurrect Now?");

	GFAddButton(gump, 100, 100, 2076, 2075, GF_CLOSE_BTN, 2);
	GFAddButton(gump, 230, 100, 2073, 2072, GF_CLOSE_BTN, 1);

	var input := GFSendGump(mobile, gump);
	input := input[0];
	if ( !input[0] || input[0] == 1 )
		return 0;
	else
		return 1;
	endif
endfunction

function Resurrection(mobile)
	if ( !mobile.IsA(POLCLASS_MOBILE) )
		return error{"errortext":="object is not a mobile."};
	elseif ( !mobile.dead )
		return error{"errortext":="mobile is not dead."};
	endif

	var result := Resurrect(mobile);

	mobile.graphic := mobile.trueobjtype;
	PlaySoundEffect(mobile, SFX_RESURRECT);
	PlayObjectCenteredEffect(mobile, GFX_BLUE_SPARKLES, 10,10);

	return result;
endfunction

