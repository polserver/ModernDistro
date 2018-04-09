/* $Id: time.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 * Purpose
 * Provide various functions and CONSTants for time formats.
 * 
 * Created by Folko
 *
 */
use uo;

include ":timeutils:settings";

enum TIME_UNITS
	// Base unit: Millisecond
	MIL_SECOND	:= 1000,
	MIL_MINUTE	:= MIL_SECOND * 60,
	MIL_HOUR	:= MIL_MINUTE * 60,
	MIL_DAY		:= MIL_HOUR * 24,
	MIL_WEEK	:= MIL_DAY * 7,
	MIL_MONTH	:= MIL_DAY * 30,
	MIL_YEAR	:= MIL_MONTH * 12,

	// Base unit: Second
	SEC_MINUTE  	:= 60,
	SEC_HOUR	:= SEC_MINUTE * 60,
	SEC_DAY	 	:= SEC_HOUR * 24,
	SEC_WEEK	:= SEC_DAY * 7,
	SEC_MONTH	:= SEC_DAY * 30,
	SEC_YEAR	:= SEC_DAY * 365,

	// SleepMs value to prevent RunAway
	NO_RUNAWAY  := 10
endenum

enum TIME_HELPERS
	SECONDS_IN_MINUTE	:= 60,		// (60)
	SECONDS_IN_HOUR		:= 3600,	// (SECONDS_IN_MINUTE * 60)
	SECONDS_IN_DAY		:= 86400,	// (SECONDS_IN_HOUR * 24)
	SECONDS_IN_WEEK		:= 604800,	// (SECONDS_IN_DAY * 7)
	SECONDS_IN_YEAR		:= 31536000,	// (SECONDS_IN_DAY * 365)
	MINUTES_IN_HOUR		:= 60,		// (60)
	MINUTES_IN_DAY		:= 1440,	// (MINUTES_IN_HOUR * 24)
	MINUTES_IN_WEEK		:= 10080,	// (MINUTES_IN_DAY * 7 )
	HOURS_IN_DAY		:= 24,		// (24)
	HOURS_IN_WEEK		:= 168		// (HOURS_IN_DAY * 7)
endenum

/*
 * GetMonthName(month)
 *
 * Purpose
 * Returns the name of the month
 *
 * Parameters
 * month:   The month to get the name of
 *
 * Return value
 * A string containing the name of the month
 *
 */
function GetMonthName(month)
	var mon_nam := {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if( month > 12 || month <= 0 )
		return error{"errortext":="Month '"+CStr(month)+"' out of range."};
	endif
	return(mon_nam[month]);
endfunction


/*
 * GetMonthLength(month, year)
 *
 * Purpose
 * Returns the length of the month in days
 *
 * Parameters
 * month:   The month to get the length of
 * year:	The year, needed for February
 *
 * Return value
 * The length of the month, in days
 *
 */
function GetMonthLength(month, year := 0)
	var mon_lens := {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if( month > 12 || month <= 0 )
		return error{"errortext":="Month '"+CStr(month)+"' out of range."};
	endif
	if ( year && IsLeap(year) )
		mon_lens[2] := 29;
	endif
	return(mon_lens[month]);
endfunction


/*
 * IsLeap(year)
 *
 * Purpose
 * Returns whether a year is a leap year or not.
 *
 * Parameters
 * year:	The year to examine
 *
 * Return value
 * 0:	   The year is not a leap year
 * 1:	   The year is a leap year
 *
 */
function IsLeap(year)
	return(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
endfunction


/*
 * GetTime()
 *
 * Purpose
 * Returns current UNIX time stamp.
 *
 * Parameters
 * None
 *
 * Return value
 * The current time in UNIX format
 *
 */
function GetTime()
	return(POLCore().systime);
endfunction


/*
 * GetTimeStruct(time)
 *
 * Purpose
 * Returns current time as struct.
 *
 * Parameters
 * time:	The UNIX timestamp to use as base for the resulting struct
 *
 * Return value
 * The time as a struct
 *
 */
function GetTimeStruct(time)
	var ts, ts_off;
	var monlen;
	var timewarp := 0;

	time := time + CInt(TIME_GetSettingsCfgElem("Settings").TimeZone) * SECONDS_IN_HOUR;

	// UNIX time starts at 01/01/1970 00:00:00
	ts := struct;
	ts.+stamp	:= time;
	ts.+year	:= 1970;
	ts.+month	:= 1;
	ts.+day		:= 1;
	ts.+hour	:= 0;
	ts.+minute	:= 0;
	ts.+second	:= 0;

	// ts_off is the offset from 1/1/1970 to the last call; this avoids using like 35 iterations everytime this is called
	if(!ts_off)
		ts_off := struct;
		ts_off.+year	:= 1970;
		ts_off.+month   := 1;
		ts_off.+sum	 := 0;
	elseif(ts_off.sum < time) // Avoid "time warps"
		ts.year := ts_off.year;
		ts.month := ts_off.month;
		time := time - ts_off.sum;
	else
		timewarp := 1;
	endif

	while(time >= SECONDS_IN_YEAR)
		time := time - SECONDS_IN_YEAR;
		ts_off.sum := ts_off.sum + SECONDS_IN_YEAR;
		if(IsLeap(ts.year))
			time := time - SECONDS_IN_DAY;
			ts_off.sum := ts_off.sum + SECONDS_IN_DAY;
		endif
		ts.year := ts.year + 1;
	endwhile

	monlen := GetMonthLength(ts.month, ts.year) * SECONDS_IN_DAY;
	while(time >= monlen)
		time := time - monlen;
		ts_off.sum := ts_off.sum + monlen;
		ts.month := ts.month + 1;
		monlen := GetMonthLength(ts.month, ts.year) * SECONDS_IN_DAY;
	endwhile

	ts.day := time / SECONDS_IN_DAY + 1;
	time := time % SECONDS_IN_DAY;

	ts.hour := time / SECONDS_IN_HOUR;
	time := time % SECONDS_IN_HOUR;

	ts.minute := time / SECONDS_IN_MINUTE;
	time := time % SECONDS_IN_MINUTE;

	ts.second := time;

	return(ts);
endfunction


/*
 * FormatTime(ts, format)
 *
 * Purpose
 * Returns a formatted time string.
 *
 * Parameters
 * time:	The time struct to use as base
 * format:  The format string. The following tags will be expanded:
 *		  <YY>: Year, <MON>: Month name, <DAY>: Day number, <HH>: Hour, <MM>: Minute, <SS>: Second
 *
 * Return value
 * The expanded format string
 *
 */
function FormatTime(byref ts, format)
	format["<YY>"] := CStr(ts.year);
	format["<MON>"] := GetMonthName(ts.month);
	format["<DAY>"] := CStr(ts.day);
	
	if(ts.hour > 9)
		format["<HH>"] := CStr(ts.hour);
	else
		format["<HH>"] := "0"+CStr(ts.hour);
	endif
	
	if(ts.minute > 9)
		format["<MM>"] := CStr(ts.minute);
	else
		format["<MM>"] := "0"+CStr(ts.minute);
	endif
	
	if(ts.second > 9)
		format["<SS>"] := CStr(ts.second);
	else
		format["<SS>"] := "0"+CStr(ts.second);
	endif
	
	return format;
endfunction


/*
 * AddTimeStamp(what)
 *
 * Purpose
 * Adds a timestamp to a string.
 *
 * Parameters
 * what:	The string to be timestamped
 *
 * Return value
 * The string with an added timestamp at the beginning
 *
 */
function AddTimeStamp(what)
	var ts := GetTimeStruct(GetTime());

	return(FormatTime(ts, "[<MON> <DAY> <YY> <HH>:<MM>:<SS>]: ") + what);
endfunction
