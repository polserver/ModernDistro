/*
 * $Id: poisonUtil.inc 625 2006-07-10 04:42:02Z austinheilman $
 *
 */

use uo;
use os;

include ":timedScripts:report";
include ":timedScripts:timedScripts";
include ":attributes:attributes";

/*
 * TS_CurePoison(mobile, cure_amount, cure_type:="", cure_level:=1)
 *
 * Purpose
 * Will take in a cure amount and try to cure / lower the duration
 * of poisons running on a mobile.
 *
 * Parameters
 * mobile:	Mobile to cure the poisons on.
 * cure_amount:	Number of duration-lowering points.
 * cure_type:	Cures poisons (timers) of that type. (Note: Most are "P" for poison)
 * level:	Level of the cure.
 *		Examples:
 *		A level 2 cure is twice as effective against a level 1 poison.
 *		It will be half as effective against a level 4 poison.
 *
 * Return value
 * Returns 1 if any curing took place
 * Returns 0 if no curing took place
 *
 */
function TS_CurePoison(mobile, cure_amount, cure_type:="", cure_level:=1)
	cure_amount := CInt(cure_amount);
	cure_level := CInt(cure_level);

	var poison_names := TS_GetTimerNames(mobile, cure_type);

	var num_cured := 0;
	//Run through all curable poisons, and keep curing until it runs out of curing power.
	foreach timer_name in ( poison_names )
		SendSysMessage(mobile, "Found poison "+timer_name);
		var elem_info := TS_GetCfgElem(timer_name);
		if ( elem_info.errortext )
			TS_ReportText("Error::TS_CurePoison() - Unable to find config elem ["+timer_name+"] - "+elem_info.errortext, TS_REPORT_SYSLOG);
			return 0;
		endif

		if ( elem_info.NoCure ) // Incurable
			continue; // Go to the next point in the loop.
		endif
		var timer_info := TS_GetTimerInfo(mobile, timer_name);
		var duration := TS_GetTimerDuration(mobile, timer_name);
		cure_amount := CInt(cure_amount / CInt(elem_info.Virulence));

		if ( elem_info.AllOrNothingCure )
			if ( (timer_info.level - 2) > cure_level )
				// If cure level is less than two levels lower than the poison
				// level, there is no chance to cure.
				continue;
			endif

			var chance := CInt(timer_info.level / (cure_level * 0.01)); // cure_level % of timer_level.
			var roll := RandomInt(100)+1;
			if ( roll <= chance )
				cure_amount -= duration;
				TS_LowerDuration(mobile, timer_name, -1);
				num_cured += 1;
			endif
		else
			var multiply := CInt(CDbl(cure_level) / CDbl(timer_info.level));
			if ( multiply <= 0 )
				multiply := 1;
			endif

			num_cured += 1;

			if ( (multiply * cure_amount) >= duration )
				// There is enough cure power to stop the poison.
				// Adjust remaining cure points and stop the poison.
				cure_amount := cure_amount - (duration / multiply);
				TS_LowerDuration(mobile, timer_name, -1);
			else
				// Out of cure power - cure with what is left of cure_amount.
				TS_LowerDuration(mobile, timer_name, cure_amount);
				break;
			endif
		endif
		SleepMS(2);
	endforeach

	return num_cured;
endfunction


