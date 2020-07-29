/*
 * $Id: timedScripts.inc 917 2006-12-17 17:40:16Z tekproxy $
 *
 */

use uo;
use cfgfile;
use os;

include ":timedScripts:report";

// Event flags
CONST EVENT_ADD_TIMER			:= 0x1;
CONST EVENT_LOWER_TIMER			:= 0x2;
CONST EVENT_PLAYER_DIED			:= 0x4;
CONST EVENT_NPC_DIED			:= 0x8;
CONST EVENT_LOGOFF			:= 0x16;

// Constants
CONST NOSTART_CONTROLLER		:= 0x0;
CONST START_CONTROLLER			:= 0x1;

/*
 * TS_GetCfgFile()
 *
 * Purpose
 * Retrieves timedScripts.cfg
 *
 * Parameters
 * None
 *
 * Return Value
 * Config file
 *
 */
function TS_GetCfgFile()
	var timed_cfg := ReadConfigFile(":timedScripts:timedScripts");

	if ( timed_cfg.errortext )
		TS_ReportText("Error::TS_GetSettingsCfgFile() - Unable to open [:timedScripts:timedScripts] ->"+timed_cfg.errortext, TS_REPORT_SYSLOG);
	endif

	return timed_cfg;
endfunction

/*
 * TS_GetCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file.
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by TS_GetCfgFile()
 *
 * Return value
 * A config file elem.
 *
 */
function TS_GetCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := TS_GetCfgFile();
	endif

	var new_elem := cfg_file[elem_name];

	if ( new_elem.errortext )
		TS_ReportText("Error::TS_GetCfgElem - Unable to find elem ["+elem_name+"] ->"+new_elem.errortext, TS_REPORT_SYSLOG);
	endif

	return new_elem;
endfunction

/*
 * TS_GetControllerProcess(mobile, start := NOSTART_CONTROLLER)
 *
 * Purpose
 * Retrieves a script reference to the timer controller for a mobile.
 *
 * Parameters
 * mobile:	Mobile to get the timer controll process for.
 * start:	Optional parameter. Default is NOSTART_CONTROLLER.
 *		NOSTART_CONTROLLER - Will only try to get a reference to the script.
 *		START_CONTROLLER - Will start the controller if it is not running.
 *
 * Return value
 * Returns a script reference.
 * Returns an error the controller is not running.
 *
 */
function TS_GetControllerProcess(mobile, start := NOSTART_CONTROLLER)
	var process := GetProcess(GetObjProperty(mobile, "#TimerControlPID"));
	if ( !process )
		if ( start )
			process := TS_StartController(mobile);
		endif
	endif

	return process;
endfunction

/*
 * TS_StartController(mobile)
 *
 * Purpose
 * Starts the timer controller for a mobile.
 *
 * Parameters
 * mobile:	Mobile to start the controller on.
 *
 * Return value
 * Returns a script reference.
 * Returns error on failure.
 *
 */
function TS_StartController(mobile)
	if ( TS_GetControllerProcess(mobile, NOSTART_CONTROLLER) )
		return error{"errortext":="Error::TS_StartController() - Controller is already running."};
	endif

	var script := Start_Script(":timedScripts:timerControl", mobile);
	if ( script.errortext )
		TS_ReportText("Error::TS_StartController() ->"+script.errortext, TS_REPORT_SYSLOG);
	endif

	return script;
endfunction

/*
 * TS_GetTimerList(mobile)
 *
 * Purpose
 * Retrieves the timer_list dictionary on the mobile.
 *
 * Parameters
 * mobile:	Mobile to retrieve the timer list from.
 *
 * Return value
 * Returns a dictionary
 *
 */
function TS_GetTimerList(mobile)
	var timer_list := GetObjProperty(mobile, "%TimerList%");
	if ( Lower(TypeOf(timer_list)) != "dictionary" )
		timer_list := dictionary;
	endif

	return timer_list;
endfunction

/*
 * TS_SaveTimerList(mobile, timer_list)
 *
 * Purpose
 * Saves the timer_list dictionary on the mobile.
 *
 * Parameters
 * mobile:	Mobile to save the list on.
 * timer_list:	Dictionary timer_list to save.
 *
 */
function TS_SaveTimerList(mobile, byref timer_list)
	return SetObjProperty(mobile, "%TimerList%", timer_list);
endfunction

/*
 * TS_GetTimerNames(mobile, type:="")
 *
 * Purpose
 * Retrieves a list of names of timed scripts running on a mobile.
 *
 * Parameters
 * mobile:	Mobile to get the list from.
 * type:	Optional parameter - Will only list timers of a certain type.
 *		If left unset, all will be retuned.
 *
 * Return value
 * Array of strings
 *
 */
function TS_GetTimerNames(mobile, type:="")
	type := Lower(type);
	var timer_list := TS_GetTimerList(mobile);
	var name_list := array{};

	foreach timer_name in ( timer_list.Keys() )
		if ( type )
			var elem_info := TS_GetCfgElem(timer_name);
			if ( elem_info.errortext )
				TS_ReportText("Error::TS_GetTimerNames() - Unable to find config elem ["+timer_name+"] - "+elem_info.errortext, TS_REPORT_SYSLOG);
				return 0;
			elseif ( Lower(elem_info.Type) == type )
				name_list.Append(timer_name);
			endif
		else
			name_list.Append(timer_name);
		endif
		SleepMS(2);
	endforeach

	return name_list;
endfunction

/*
 * TS_GetTimerInfo(mobile, timer_name)
 *
 * Purpose
 * Retrieves information on a *running* timer on a mobile.
 *
 * Parameters
 * mobile:	Mobile to find the timer on.
 * timer_name:	Name of the timer (elem name from timedScripts.cfg) to find.
 *
 * Return value
 * Returns a struct or error
 *
 */
function TS_GetTimerInfo(mobile, timer_name)
	var timer_list := TS_GetTimerList(mobile);
	if ( timer_list.Exists(timer_name) )
		return timer_list[timer_name];
	else
		return error{"errortext":="Error::TS_GetTimerInfo() - No timer exists by that name."};
	endif
endfunction

/*
 * TS_GetTimerDuration(mobile, timer_name)
 *
 * Purpose
 * Returns the number of seconds a timer has left to run.
 *
 * Parameters
 * mobile:	Mobile to find the timer on.
 * timer_name:	Name of the timer (elem name from timedScripts.cfg) to find.
 *
 * Return value
 * Returns an integer on success
 * Returns an error on failure
 *
 */
function TS_GetTimerDuration(mobile, timer_name)
	var timer := TS_GetTimerInfo(mobile, timer_name);
	if ( !timer )
		return timer; // Returns the error
	endif

	return (timer.endtime - ReadGameClock());
endfunction

/*
 * TS_StartTimer(mobile, timer_name, duration, level := 0, attacker := 0)
 *
 * Purpose
 * Starts a timed script on a mobile. If it is already running, it adds to the time it runs.
 *
 * Parameters
 * mobile:	Mobile to start the timed script on.
 * timer_name:	Name of the timer (elem name from timedScripts.cfg) to start.
 * duration:	Number of seconds to add to the timer's run time.
 * attacker:	Optional - Sets an attacker.
 * level:	Optional - Sets the level of the timer (for poisons) or
 *			level is also the amount to boost a skill or stat for timed scripts that require it.
 *
 * Return value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function TS_StartTimer(mobile, timer_name, duration, level := 0, attacker := 0)
	duration := CInt(duration);
	if ( duration < 1 )
		return error{"errortext":="Error::TS_StartTimer() - Duration must be greater than 0."};
	endif
	var event := struct;
	event.+type := EVENT_ADD_TIMER;
	event.+name := CStr(timer_name);
	event.+amount := duration;
	event.+level := CInt(level);
	event.+attacker := attacker;

	var process := TS_GetControllerProcess(mobile, START_CONTROLLER);
	return process.SendEvent(event);
endfunction

/*
 * TS_LowerDuration(mobile, timer_name, amount)
 *
 * Purpose
 * Lowers the amount of time a timer will run.
 * If remaining time is <= 0, it will turn the timer off.
 * If -1 is passed for amount, it will turn the timer off.
 *
 * Parameters
 * mobile:	Mobile to lower the timed script on.
 * timer_name:	Name of the timer (elem name from timedScripts.cfg).
 * amount:	Number of seconds to lower the run time of the timer.
 *		-1 will stop it completely.
 *
 * Return value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function TS_LowerDuration(mobile, timer_name, amount)
	var event := struct;
	event.+type := EVENT_LOWER_TIMER;
	event.+name := CStr(timer_name);
	event.+amount := CInt(amount);

	var process := TS_GetControllerProcess(mobile);
	return process.SendEvent(event);
endfunction

/*
 * TS_LogOff(mobile)
 *
 * Purpose
 * Informs the timer controller that a player logged out.
 *
 * Parameters
 * mobile:	Mobile that logged off
 *
 * Return value
 * Returns 1 on success
 * Returns 0 on failure
 *
 */
function TS_LogOff(mobile)
	if ( mobile.IsA(POLCLASS_NPC) )
		return error{"errortext":="NPCs can't log off."};
	endif

	var event := struct;
	event.+type := EVENT_LOGOFF;

	var process := TS_GetControllerProcess(mobile);
	return process.SendEvent(event);
endfunction

/*
 * TS_Death(mobile)
 *
 * Purpose
 * Informs the timer controller that a moble died.
 *
 * Parameters
 * mobile:	Mobile that died.
 *
 * Return value
 * Returns 1 on success
 * Returns 0 on failure
 *
 */
function TS_Death(mobile)
	var event := struct;
	if ( mobile.IsA(POLCLASS_MOBILE) )
		event.+type := EVENT_PLAYER_DIED;
	elseif ( mobile.IsA(POLCLASS_CORPSE) )
		event.+type := EVENT_NPC_DIED;
	endif

	var process := TS_GetControllerProcess(mobile);
	return process.SendEvent(event);
endfunction