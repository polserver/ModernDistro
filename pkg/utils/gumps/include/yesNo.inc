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

const LOCALIZED := 1;
const NOT_LOCALIZED := 0;

/*
 * YesNo( mobile, prompt:="", yes_text:="Yes", no_text:="No", delay:=0, yn_localized := NOT_LOCALIZED )
 *
 * Purpose
 * Template to create a yes / no gump.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * prompt:	Primary text field, instructing the mobile on what the choices are about.
 * yes_text:	Text to be displayed next to the 'YES' button.
 * no_text:	The text to be displayed next to the 'NO' button.
 * delay:   Time until gump will automatically close on the client if no action is taken.
 *
 * Return value
 * Returns '0' if no is selected.
 * Returns '1' if yes is selected.
 *
 */
function YesNo( mobile, prompt:="", yes_text:="Yes", no_text:="No", delay:=0, yn_localized := NOT_LOCALIZED )

        var params := array{mobile, prompt, yes_text, no_text, delay, yn_localized};

        var script := Run_Script( ":gumps:scripts/yesNo/yesNoGump", params );
        if( script.errortext )
                SendSysMessage( mobile, "Error starting script <:gumps:yesNoGump> -->"+script.errortext );
                return 0;
        endif

	return script;
endfunction

