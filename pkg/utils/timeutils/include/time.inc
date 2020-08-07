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
 * Purpose: Convert the month number to the month's abbreviated name.
 *
 *
 * Parameters
 * Integer::month:	Ordinal number of the month.
 *
 * Return value
 * A string containing the abbreviated name of the month
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
 * GetMonthFullName(month)
 *
 * Purpose: Convert the month number to the month's name.
 *
 *
 * Parameters
 * Integer::month:	Ordinal number of the month.
 *
 * Return value
 * A string containing the full name of the month
 *
 */
function GetMonthFullName(month)
	var mon_nam := {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

	if( month > 12 || month <= 0 )
		return error{"errortext":="Month '"+CStr(month)+"' out of range."};
	endif
	return(mon_nam[month]);
endfunction


/*
 * GetMonthOrdinal(month)
 *
 * Purpose: Convert the month's name or abbreviation to
 *			the ordinal number of the month.
 *
 *
 * Parameters
 * String::month:	Month full name or standard abbreviation.
 *
 * Return value
 * An integer representing the ordinanal number of the month.
 *
 */
function GetMonthOrdinal(month)

	month := Lower(month);

	case(month)
		"jan":
		"january":		return "01";
		"feb":
		"february":		return "02";
		"mar":
		"march":		return "03";
		"apr":
		"april":		return "04";
		"may":			return "05";
		"jun":
		"june":			return "06";
		"jul":
		"july":			return "07";
		"aug":
		"august":		return "08";
		"sep":
		"september":	return "09";
		"oct":
		"october":		return "10";
		"nov":
		"november":		return "11";
		"dec":
		"december":		return "12";
		default:		return error{"errortext":="Invalid month name for 'month' parameter."};
	endcase
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
 * GetMonthDifferential( byref current_month, byref previous_month )
 *
 * Purpose
 * Returns the difference between month_1 and month_2
 * The intended use of this function is to determine
 * the number of months that have elapsed between
 * current_month and previous_month.
 *
 * Parameters
 * current_month:	Integer representing the first month.
 * previous_month:	Integer representing the second month.
 *
 * Return value
 * Integer indicating the the ordinal difference between
 * the two months.
 *
 */
function GetMonthDifferential( byref current_month, byref previous_month )

    if( current_month <= 0 && current_month >= 13 )
        return error{"errortext":="Invalid month numbers for 'month_1' parameter."};
    elseif( previous_month <= 0 && previous_month >= 13 )
        return error{"errortext":="Invalid month numbers for 'month_2' parameter."};
    endif

    return Abs( ( 12 + current_month - previous_month ) % 12 );
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
 * FormatTime(time_struct, format)
 *
 * Purpose
 * Returns a formatted time string.
 *
 * Parameters
 * time_struct:	The time struct to use as base
 * format:  The format string. The following tags will be expanded:
 *		  <YY>: Year, <MON>: Month name, <DAY>: Day number, <HH>: Hour, <MM>: Minute, <SS>: Second
 *
 * Return value
 * The expanded format string
 *
 */
function FormatTime(byref time_struct, format)
	format["<YY>"] := CStr(time_struct.year);
	format["<MON>"] := GetMonthName(time_struct.month);
	format["<DAY>"] := CStr(time_struct.day);

	if(time_struct.hour > 9)
		format["<HH>"] := CStr(time_struct.hour);
	else
		format["<HH>"] := "0"+CStr(time_struct.hour);
	endif

	if(time_struct.minute > 9)
		format["<MM>"] := CStr(time_struct.minute);
	else
		format["<MM>"] := "0"+CStr(time_struct.minute);
	endif

	if(time_struct.second > 9)
		format["<SS>"] := CStr(time_struct.second);
	else
		format["<SS>"] := "0"+CStr(time_struct.second);
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
