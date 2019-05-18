use uo;
use cfgfile;

var timecfg := ReadConfigFile( "::days" );

function GetTime_minus_newday()

	var gametime := ReadGameClock() - GetGlobalProperty("NewDay");
	if ( (gametime < 0) or (!gametime) )
		gametime := 0;
	endif
	return (gametime);

endfunction

function GetTme()
  var cfg := ReadConfigFile( "::days" );
  var elem := FindConfigElem( cfg, "WholeDay" );
  var length := GetConfigInt( elem, "Length" );
  var day_length := (60 * length);
  var x := ReadGameClock();
  var bigpart := x / day_length;
  var smallpart := bigpart * day_length;
  x := (x - smallpart);
  return ( x );
endfunction


function GetTransition()

    var elem := FindConfigElem( timecfg , "Transition" );
    var length := GetConfigInt( elem , "Length" );
    return length;

endfunction


function GetDayLength()

	var elm := FindConfigElem(timecfg, "WholeDay");
	var DayLength := GetConfigInt(elm, "Length");
	if (!DayLength)
		DayLength := 180;
	endif

	return DayLength;

endfunction


function GetTimeofDay()

	var DayLength := GetDayLength();
	var MinutesElapsed := CDbl( GetTime_minus_newday() / 60 );
	var mmm := (MinutesElapsed / DayLength) * (24.0 * 60.0);
	var hhh := CInt(mmm / 60);
	mmm := CInt(mmm - (hhh * 60) );

	if (mmm < 10)
		mmm := "0" + CStr(mmm);
	endif
	if (hhh < 10)
		hhh := "0" + CStr(hhh);
	endif

	return (CStr(hhh) + ":"	+ CStr(mmm) );

endfunction

///////////////////
//  Turns the number of seconds into a string describing it in time terms
///////////////////

function GetTimeDesc (seconds, showseconds := 1)
	seconds := CInt (seconds);
	var minutes := CInt (seconds/60);
	var hours := CInt (minutes/60);
	var days := CInt (hours/24);

	if (seconds < 60)
		if (showseconds)
			return (seconds + " seconds");
		else
			return "< 1 minute";
		endif
	endif

	hours := hours - days*24;
	minutes := minutes - (days*24 + hours)*60;

	var returnstring := "";
	if (days)
		if (days > 1)
			returnstring := returnstring + days + " days, ";
		else
			returnstring := returnstring + "1 day, ";
		endif
	endif

	if (hours or days)
		if (hours > 1)
			returnstring := returnstring + hours + " hours, ";
		else
			returnstring := returnstring + "1 hour, ";
		endif
	endif

	if (minutes or hours or days)
		if (minutes > 1)
			returnstring := returnstring + minutes + " minutes";
		else
			returnstring := returnstring +  "1 minute";
		endif
	endif

	if (showseconds)
		seconds := CInt (seconds%60);
		if (seconds > 1)
			returnstring := returnstring + ", " + seconds + " seconds, ";
		elseif (seconds == 1)
			returnstring := returnstring + ", " + "1 second, ";
		else
			returnstring := returnstring + ", " + "0 seconds, ";
		endif
	endif

	return returnstring;
endfunction


function GetHourOfDay()
  var time := GetTimeofDay();
  var colon := find(time, ":", 1);
  return CInt(time[1, colon]);
endfunction

function HourWithinRange(hr, hrstart, hrend)
  hr := CInt(hr) + 24;
  hrstart := CInt(hrstart) + 24;
  hrend := CInt(hrend) + 24;
  if (hrstart > hrend)
	if (hr >= hrstart || hr < hrend)
	  return 1;
	endif
  else
	if (hr >= hrstart && hr <= hrend)
	  return 1;
	endif
  endif
  return 0;
endfunction

function GetDaysSinceShardStart()

    var cfg := ReadConfigFile( "::days" );
    var elem := FindConfigElem( cfg , "WholeDay" );
    var length := GetConfigInt( elem , "Length" );
    var day_length := (60 * length);

    var x := polcore().systime - 1023840000;
    var bigpart := x / day_length;

    return ( bigpart );

endfunction

function GetHour()

    var time_sec := GetTme();
    var length := (GetDayLength() * 60) * 1.0 ;

    var temp := length / time_sec;
    var raw_time := 24.0 / temp;

    var hours := cint(raw_time);

    return cint(hours);

endfunction


///////////////////
//  Figures out what the current year is
///////////////////

function GetYear ()
	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;
	var current_year_counter := 0;
	while (systime > 31536000)
		if (current_year_counter%4 == 0)
			systime := systime - 31622400;
		else
			systime := systime - 31536000;
		endif
		current_year_counter := current_year_counter + 1;
	endwhile;
	return (2000 + current_year_counter);
endfunction

///////////////////
//  Figures out what the current month is (by name)
///////////////////

function GetMonth ()
	var MonthDays := array { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	var MonthNames := array {
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;

	var current_year_counter := 0;
	var current_month_counter := 1;
	var done := 0;
	repeat
		var seconds_in_this_month := 0;
		if (current_month_counter == 2)
			if (current_year_counter%4 == 0)
				seconds_in_this_month := SECONDS_PER_DAY * 29;
			else
				seconds_in_this_month := SECONDS_PER_DAY * 28;
			endif
		else
			seconds_in_this_month := SECONDS_PER_DAY * MonthDays[current_month_counter];
		endif

		if (systime > seconds_in_this_month)
			systime := systime - seconds_in_this_month;
			current_month_counter := current_month_counter + 1;
			if (current_month_counter > 12)
				current_month_counter := 1;
				current_year_counter := current_year_counter + 1;
			endif
		else
			done := 1;
		endif
	until (done);
	return MonthNames [current_month_counter];
endfunction


///////////////////
//  Figures out what the current year is (real-world time)
///////////////////

function Systime_GetYear ()
	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;
	var current_year_counter := 0;
	var done := 0;
	repeat
		if (current_year_counter%4 == 0)
			systime := systime - 31622400;
		else
			systime := systime - 31536000;
		endif
		if (systime >= 0)
			current_year_counter := current_year_counter + 1;
		else
			done:= 1;
		endif
	until (done);
	return (2000 + current_year_counter);
endfunction





///////////////////
//  Figures out what the current month is (by name) (real-world time)
///////////////////

function Systime_GetMonth ()
	var MonthDays := array { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	var MonthNames := array {
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;
	//convert to GMT
//	systime := systime + 43200;
	//convert to days
	var days := CInt (systime / SECONDS_PER_DAY);

	var current_year_counter := 0;
	var current_month_counter := 1;
	var done := 0;
	repeat
		if (current_month_counter == 2)
			if (current_year_counter%4 == 0)
				days := days - 29;
			else
				days := days - 28;
			endif
		else
			days := days - MonthDays[current_month_counter];
		endif

		if (days > 0)
			current_month_counter := current_month_counter + 1;
			if (current_month_counter > 12)
				current_month_counter := 1;
				current_year_counter := current_year_counter + 1;
			endif
		else
			done := 1;
		endif
	until (done);
	return MonthNames [current_month_counter];
endfunction





///////////////////
//  Figures out what the current date is (real-world time)
///////////////////

function Systime_GetDate ()
	var MonthDays := array { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;
	//convert to GMT
//	systime := systime + 43200;
	//convert to days
	var days := CInt (systime / SECONDS_PER_DAY) + 1;

	var current_year_counter := 0;
	var current_month_counter := 1;
	var done := 0;
	repeat
		if (current_month_counter == 2)
			if (current_year_counter%4 == 0)
				if (days > 29)
					days := days - 29;
				else
					return days;
				endif
			else
				if (days > 28)
					days := days - 28;
				else
					return days;
				endif
			endif
		else
			if (days > 28)
				days := days - MonthDays[current_month_counter];
			else
				return days;
			endif
		endif

		if (days > 0)
			current_month_counter := current_month_counter + 1;
			if (current_month_counter > 12)
				current_month_counter := 1;
				current_year_counter := current_year_counter + 1;
			endif
		else
			done := 1;
		endif
	until (done);

	return days;
endfunction




///////////////////
//  Figures out what the current hour is, military time (real-world time)
///////////////////

function Systime_GetHour ()
	var systime := polcore().systime;
	//convert to GMT
//	systime := systime + 43200;
	systime := CInt (systime % SECONDS_PER_DAY);

	var hour := CInt (systime / 3600);
	return hour;
endfunction




///////////////////
//  Figures out what the current minute is (real-world time)
///////////////////

function Systime_GetMinute ()
	var systime := polcore().systime;
	systime := CInt (systime % 3600);

	var minute := CInt (systime / 60);
	return minute;
endfunction




///////////////////
//  This function returns a string in the format "month date, year hour:minutes"
///////////////////

function Systime_GetReadableTimeString ()
	var MonthDays := array { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	var MonthNames := array {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec"
	};

	//Seconds since 1/1/1970
	var systime := polcore().systime;
	//Lets skip ahead to y2k
	systime := systime - 946684800;
	//convert to GMT
//	systime := systime + 43200;
	//convert to days
	var days := CInt (systime / SECONDS_PER_DAY) + 1;

	var current_year_counter := 0;
	var current_month_counter := 1;
	var days_in_month, done := 0;
	repeat
		if (current_month_counter == 2)
			if (current_year_counter%4 == 0)
				days_in_month := 29;
			else
				days_in_month := 28;
			endif
		else
			days_in_month := MonthDays[current_month_counter];
		endif

		if (days_in_month <= days)
			days := days - days_in_month;
			current_month_counter := current_month_counter + 1;
			if (current_month_counter > 12)
				current_month_counter := 1;
				current_year_counter := current_year_counter + 1;
			endif
		else
			done := 1;
		endif
	until (done);

	if (days < 10)
		days := "0" + days;
	endif
	var timestring := MonthNames [current_month_counter] + " " + days + ", " + (2000 + current_year_counter);

	//now figure out hours and minutes
	systime := polcore().systime;
	//convert to GMT
//	systime := systime + 43200;
	systime := CInt (systime % SECONDS_PER_DAY);
	var hour := CInt (systime / 3600);

	systime := polcore().systime;
	systime := CInt (systime % 3600);
	var minute := CInt (systime / 60);

	if (hour == 0)
		hour := 24;
	elseif (hour < 10)
		hour := "0" + hour;
	endif
	if (minute < 10)
		minute := "0" + minute;
	endif

	timestring := timestring + " " + hour + ":" + minute + "GMT";
	return timestring;
endfunction




///////////////////
//  Gets the current year_month, used for recording playtime information
///////////////////

function GetCurrentPlaytimeMonth ()
	var year := Systime_GetYear ();
	var month := lower (Systime_GetMonth ());

	return (year + "_" + month + "_play_time");
endfunction




///////////////////
//  Gets the previous year_month, used for recording playtime information
///////////////////

function GetPreviousPlaytimeMonth (previous_month_counter := 1)
	var year := Systime_GetYear ();
	var month := lower (Systime_GetMonth ());

	repeat
		case (month)
			"december":
				month := "november";
			"november":
				month := "october";
			"october":
				month := "september";
			"september":
				month := "august";
			"august":
				month := "july";
			"july":
				month := "june";
			"june":
				month := "may";
			"may":
				month := "april";
			"april":
				month := "march";
			"march":
				month := "february";
			"february":
				month := "january";
			"january":
				month := "december";
				year := year - 1;
			default:
				Syslog ("Error figuring out month " + month);
				return "";
		endcase
		previous_month_counter := previous_month_counter - 1;
	until (!previous_month_counter);

	return (year + "_" + month + "_play_time");
endfunction

