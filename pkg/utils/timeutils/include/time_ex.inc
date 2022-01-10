/////////////////////////////////////////////////////////////////////
//
// This file will contain utilities related to time, date, and years.
//
// The functions in this file may be specialized, as in
// GetChAnimalYear(), and thus do not belong in the main time.inc
// file.
//
/////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//
// GetChAnimalYear( yyyy )
//
// Purpose: Returns the animal corresponding to the year
//			according to the Chinese calendar.
//
//
//
// Parameter: integer:: year
//
// Returns: string:: animal
//
///////////////////////////////////////////////////////////
function GetChAnimalYear( yyyy )

    return { "Monkey", "Rooster", "Dog", "Pig", "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse", "Sheep" }[(yyyy%12)+1];

endfunction

///////////////////////////////////////////////////////////
//
// IsLeapYear ( year )
//
// Purpose: Determine whether a year, using the Gregorian
//			calendar, is a leap year.
//
//
//
// Parameter: integer:: year
//
// Returns: integer:: 1 if leap year, 0 if not.
//
///////////////////////////////////////////////////////////
function IsLeapYear ( year )

    return year % 4 == 0 && ( year % 100 != 0 || year % 400 == 0 );

endfunction