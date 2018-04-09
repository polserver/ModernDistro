/**
 * Contains some functions to perform date and time conversions.
 * TODO : Date formatting
 *
 * @author     Andromeda UO
 * @author     Jouni Airaksinen, mintus@andromeda-uo.net
 * @url        http://www.andromeda-uo.net/
 * @version    1.1 (%G%)
 * @copyright  (C) Copyright 2002-2003
 */
 
include "include/constsDate";
include "include/utilsStrings";


// polcore().systime is based to, January 1th 1970 (00:00 Greenwich Time ?)
const DateDelta := 719163; // days since 1/1/0001 till 1/1/1970

const SECONDS_IN_MINUTE := 60;
const HOURS_IN_DAY := 24;
const MINUTES_IN_HOUR := 60;

const SECONDS_IN_HOUR := SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
const SECONDS_IN_DAY := SECONDS_IN_HOUR * HOURS_IN_DAY;

const AVERAGE_MONTH_LENGTH := 30.4375; // 4 year period
const AVERAGE_YEAR_LENGTH := 365.25;   // 4 year period

// arrays //

// The MonthDays array can be used to quickly find the number of
//  days in a month:  MonthDays[IsLeapYear(Y) + 1][M]

var MonthDays := array { 
  array { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
  array { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 } };

// Lookup arrays for month and day names (short and long versions available).
// To localize these edit constsDate.inc.

var ShortMonthNames := array {
  SShortMonthNameJan,  SShortMonthNameFeb,  SShortMonthNameMar,  SShortMonthNameApr,
  SShortMonthNameMay,  SShortMonthNameJun,  SShortMonthNameJul,  SShortMonthNameAug,
  SShortMonthNameSep,  SShortMonthNameOct,  SShortMonthNameNov,  SShortMonthNameDec };
  
var LongMonthNames := array {
  SLongMonthNameJan,  SLongMonthNameFeb,  SLongMonthNameMar,  SLongMonthNameApr,
  SLongMonthNameMay,  SLongMonthNameJun,  SLongMonthNameJul,  SLongMonthNameAug,
  SLongMonthNameSep,  SLongMonthNameOct,  SLongMonthNameNov,  SLongMonthNameDec };

var ShortDayNames := array {
    SShortDayNameSun, SShortDayNameMon, SShortDayNameTue, SShortDayNameWed,
    SShortDayNameThu, SShortDayNameFri, SShortDayNameSat };

var LongDayNames := array {
    SLongDayNameSun, SLongDayNameMon, SLongDayNameTue, SLongDayNameWed,
    SLongDayNameThu, SLongDayNameFri, SLongDayNameSat };


// functions //

/**
 * Tells if given year is a leap year.
 *  
 * @param year            Year to check for leap year.
 * @return                Returns 1 if given year is a leap year, otherwise 0.
 */
function IsLeapYear(year)
  return ((year % 4) == 0) and (((year % 100) <> 0) or ((year % 400) == 0));
endfunction

/**
 * Returns number of days in given year.
 *  
 * @param year            Year from which the retreive the number of days.
 * @return                Returns number of days in the given year.
 */
function DaysInYear(year)
  return (365 + IsLeapYear(year));
endfunction

/**
 * Returns index to day of weeks array of the given time.
 * <p>
 * eg. LongDayNames[DayOfWeek(polcore().systime)]
 * would return the current day of week as a string.
 *  
 * @param time            Time from which to get the day of week index.
 * @return                Index to the array of day of week.
 */
function DayOfWeek(time)
  return ((CInt(time / SECONDS_IN_DAY) + DateDelta) % 7) + 1;
endfunction

/**
 * Returns current date part of the polcore().systime
 *  
 * @param time            Time from which to get the current date part.
 * @return                Returns current date part of the polcore().systime.
 */
function CDateStr(time)
  var Year, Month, Day;
  DecodeDate(time, Year, Month, Day);  
  return CStr(Year) + DateSeparator + CPadStr(Month, 2) + DateSeparator + CPadStr(Day, 2);
endfunction

/**
 * Returns current time part of the polcore().systime
 *  
 * @param time            Time from which to get the current time part.
 * @return                Returns current time part of the polcore().systime.
 */
function CTimeStr(time)
  var Hour, Minute, Second;
  DecodeTime(time, Hour, Minute, Second);
  return CPadStr(Hour, 2) + TimeSeparator + CPadStr(Minute, 2) + TimeSeparator + CPadStr(Second, 2);
endfunction

/**
 * Returns current time and date parts of the polcore().systime
 * <p>
 * Design notes: Does not use CTimeStr() or CDateStr() functions by design.
 *  
 * @param time            Time from which to get the current time and date parts.
 * @return                Returns current time and date parts of the polcore().systime.
 */
function CDateTimeStr(time)
  var Year, Month, Day;
  var Hour, Minute, Second;
  DecodeDate(time, Year, Month, Day);  
  DecodeTime(time, Hour, Minute, Second);
  return CStr(Year) + DateSeparator + CPadStr(Month, 2) + DateSeparator + CPadStr(Day, 2) + "  " + 
    CPadStr(Hour, 2) + TimeSeparator + CPadStr(Minute, 2) + TimeSeparator + CPadStr(Second, 2);
endfunction

/**
 * Converts time into POL log formatted date string.
 * <p>
 * With default constsDate.inc this returns same string as CDateStr() does.
 *  
 * @param time            Time from which to format the log date string.
 * @return                Returns the POL log formatted date string.
 */
function CLogTime(time)
  var Year, Month, Day;
  DecodeDate(time, Year, Month, Day);  
  return CStr(Year) + "-" + CPadStr(Month, 2) + "-" + CPadStr(Day, 2);
endfunction

/**
 * Converts Time (seconds) to counter string.
 * <p>
 * eg. with 100 seconds function would return string "1 mins, 40 secs".
 * <p>
 * TODO : Custom formatting string
 *  
 * @param time            Time from which to format the counter string. 
 * @param showdays        Set 0 to hide days in the string. Defaults to 1.
 * @param showhours       Set 0 to hide hours in the string. Defaults to 1.
 * @param showminutes     Set 0 to hide minutes in the string. Defaults to 1.
 * @param showseconds     Set 0 to hide seconds in the string. Defaults to 1.
 * @return                Returns formatted counter string.
 */

function CDateTimeCounterStr(time, showdays := 1, showhours := 1, showminutes := 1, showseconds := 1)
  var Days := CInt(time / SECONDS_IN_DAY);
  time := CInt(time % SECONDS_IN_DAY);
  var Hour := CInt(time / SECONDS_IN_HOUR);
  var Second := CInt(time % SECONDS_IN_HOUR);
  var Minute := CInt(Second / SECONDS_IN_MINUTE);
  Second := CInt(Second % SECONDS_IN_MINUTE);
  var R := "";
  if (showdays) 
    R := CStr(Days) + " days";
  endif
  if (showhours) 
    R := R + ", " + CStr(Hour) + " hours";
  endif
  if (showminutes) 
    R := R + ", " + CStr(Minute) + " mins";
  endif
  if (showseconds) 
    R := R + ", " + CStr(Second) + " secs";
  endif
  return R;
endfunction

/**
 * Converts Time's time part to military integer. Eg. 20 hours 54 minutes 
 * will be 2054. Does not pad with zeroes, use CPadStr() in utilStrings.inc 
 * to do that.
 *  
 * @param time            Time from which to format the military integer.
 * @return                Returns the military integer.
 */
function CMilitaryTime(time)
  var Hour, Minute, Second;
  DecodeTime(time, Hour, Minute, Second);
  return (Hour * 100) + Minute;
endfunction

/**
 * Does division and returns result and reminder.
 * Used internally. Should be moved to separate Maths.inc.
 *  
 * @param dividend        Value to divide.
 * @param divisor         Divisor.
 * @param result          Result of the division. Byref value.
 * @param remainder       Remainder of the division. Byref value.
 */
function DivMod(dividend, divisor, byref result, byref remainder)
  result := 0;
  remainder := 0;
  if (divisor)
    result := CInt(dividend / divisor);
    remainder := CInt(dividend % divisor);
  endif
endfunction

/**
 * Used internally to decode Time to Hours, Minutes and Seconds.
 *  
 * @param time            Time value to decode.
 * @param hour            Hour result of the decode. Byref value.
 * @param minute          Minute result of the decode. Byref value.
 * @param second          Second result of the decode. Byref value.
 */
function DecodeTime(time, byref hour, byref minute, byref second)
  time := CInt(time % SECONDS_IN_DAY); // ingore beyond one day
  hour := CInt(time / SECONDS_IN_HOUR);
  second := CInt(time % SECONDS_IN_HOUR);
  minute := CInt(second / SECONDS_IN_MINUTE);
  second := CInt(second % SECONDS_IN_MINUTE);  
endfunction

const D1 := 365;
const D4 := D1 * 4 + 1;
const D100 := D4 * 25 - 1;
const D400 := D100 * 4 + 1;

/**
 * Used internally to decode Time to Years, Months and Days.
 *  
 * @param time            Time value to decode.
 * @param year            Year result of the decode. Byref value.
 * @param month           Month result of the decode. Byref value.
 * @param day             Day result of the decode. Byref value.
 */
function DecodeDate(time, byref year, byref month, byref day)
//function DecodeDate(Time, byref Year, byref Month, byref Day, byref DOW, byref Reminder)
//  Reminder := CInt(Time % SECONDS_IN_DAY);
  
  var T := CInt(time / SECONDS_IN_DAY) + DateDelta;
  if (T <= 0)
    year := 0;
    month := 0;
    day := 0;
//    DOW := 0;
  else
//    DOW := T % 7;
    T := T - 1;
    // Calculate years. Meaning of D??? is to take care of leapyears.
    var Y := 1;
    while (T >= D400)
      T := T - D400;
      Y := Y + 400;
    endwhile
    var I, D;
    DivMod(T, D100, I, D);
    if (I == 4)
      I := I - 1;
      D := D + D100;
    endif    
    Y := Y + (I * 100);
    DivMod(D, D4, I, D);
    Y := Y + (I * 4);
    DivMod(D, D1, I, D);
    if (I == 4)
      I := I - 1;
      D := D + D1;
    endif
    Y := Y + I;    
    // Reminder has months and days.
    var IsLeap := IsLeapYear(Y) + 1;
    var M := 1;
    while (1)
      I := MonthDays[IsLeap][M];
      if (D < I) break; endif
      D := D - I;
      M := M + 1;
    endwhile
    year := Y;
    month := M;
    day := D + 1;
  endif
endfunction