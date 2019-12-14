use uo;
use os;
use file;

// ID used for the global prop which stores all control script PID's
const CS_GlobalID := "#ControlScripts";
// File used to store all logs for system.
const CS_LogFile := "::logs/ScriptController.log";

// event id's used by the controller.
const CS_EVENT_START := 0x01;
const CS_EVENT_KILL := 0x02;

/**
 * [startControlScript alternative to start_script() to be used for launching control scripts for packages]
 *
 * @param  {[script path]} scriptString [string to script being started e.g. :guildSystem:start.ecl]
 * @param  {object} params  [can be any object which is passed to the script as a parameter e.g. int, string, array, struct]
 *
 * @return {[int or errortext]}              [returns 1 on success or error text on failure.]
 */
function startControlScript( scriptString, params := 0 )
	var caller := GetProcess();
	var script := Start_Script( scriptString, params );
	if( !script.errortext )
		// check if the the script calling has a package name, if not its likely a system script being called.
		if( script.package.name )
			// if the script has a package name print the package name and PID to console.
			LogToFile("["+script.pid+"]Started Control: "+script.package.name);
		else
			SysLog("["+script.pid+"]Started Control for system script");
		endif
		//return 1 on success.
		return 1;
	else
		// log error to console and return errortext to calling script.
		SysLog("Script '"+scriptString+"' failed to start with error: "+script.errortext);
		return script.errortext;
	endif
endfunction


function CS_ErrorLog( errorMessage )
	LogToFile(CS_LogFile, "[ERROR]"+errorMessage, LOG_DATETIME );
endfunction