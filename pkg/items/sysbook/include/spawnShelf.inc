/////////////////////////////////////////////////////////////////////
//
// spawnShelf.inc
//
// Purpose
// Contains functions specific to the book spawning system for the
// sysbook package.
//
//
// Creator: Yukiko
//
// Date 2019-05-28
//
/////////////////////////////////////////////////////////////////////
use datafile;

/////////////////////////////////////////////////////////////////////
//
// RetrieveOrInitBookshelfDB()
//
// Purpose
// Retrieves the object reference for the spawnedbookshelves
// data file if it exists. If not, creates the file.
//
// Parameters
// None
//
// Return value
// Object reference to the spawnedbookshelves data file.
//
/////////////////////////////////////////////////////////////////////
function RetrieveOrInitBookshelfDB()
	var shelfdata := OpenDataFile(":sysbook:spawnedbookshelves");
	if(shelfdata.errortext)
		shelfdata := CreateDataFile(":sysbook:spawnedbookshelves", DF_KEYTYPE_INTEGER);
	endif
	return shelfdata;
endfunction
