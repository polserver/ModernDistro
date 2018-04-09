// $Id: dayNight.inc 630 2011-09-12 23:10:05Z AustinHeilman $
//
//

use uo;
use math;

include ":daynight:settings";
include ":datafile:datafile";
include ":timeutils:time";

enum DAYNIGHT_EVENTS
	SUNSETTING	:= 0x1,
	DARK		:= 0x2,
	SUNRISING	:= 0x4,
	DAY		:= 0x8
endenum


/*
 * DAYNIGHT_GetDataFile(mobile)
 *
 * Purpose
 *
 * Parameters
 *
 * Return Value
 *
 */
function DAYNIGHT_GetDataFile(realm:=_DEFAULT_REALM)
	var data_file := DFOpenDataFile(":daynight:settings", DF_CREATE);
	if ( data_file == error )
		return data_file;
	elseif ( realm )
		data_file := DFFindElement(data_file, CStr(realm), DF_CREATE);
	endif

	return data_file;
endfunction

function GetRLSecondsInFullDay()
	var settings_elem := DAYNIGHT_GetSettingsCfgElem("Settings");
	
	return settings_elem.SecondsInFullGameDay;
endfunction	

function GetRLMinutesInFullDay()
	var total_seconds := GetRLSecondsInFullDay();
	
	var minutes := CDbl(total_seconds / 60);
	
	return minutes;
endfunction

function GetRLHoursInFullDay()
	var total_seconds := GetRLSecondsInFullDay();
	
	var hours := Cdbl((total_seconds / 60) / 60);
	
	return hours;
endfunction

function GetGameTime()
	var day_length := GetRLSecondsInFullDay();
	
	var game_clock := ReadGameClock();
	
	// Finds where based on the game clock, the world's seconds are at.
	// ReadGameClock() is always cumulative for shard up-time.
	return CInt(game_clock % day_length);
endfunction

function GetGameHour()
	var time_sec := GetGameTime() * 24;
	var length := GetRLSecondsInFullDay();
	var hours := CDbl(time_sec) / CDbl(length);
	
	var rounded := CInt(FormatRealToString(hours, 0));
	if ( rounded > hours )
		if ( GetGameMinute() == 0)
			hours += 1;
		endif
	endif
		
	return CInt(hours);
endfunction

function GetSecondsInGameHour()
	var length := GetRLSecondsInFullDay();
	
	return length / 24;
endfunction

function GetGameMinute()
	var time_sec := GetGameTime();
	var length := (GetRLMinutesInFullDay() * 60) * 1.0 ;
	
	var temp := length / time_sec;
	var raw_time := 24.0 / temp;
	
	var hours := CInt(raw_time);
	var remainder := raw_time - hours;
	var minutes := CInt(remainder * 60);
  if ( minutes < 10 )
    minutes := CStr ( "0" + minutes );
  endif
	return minutes;
endfunction

