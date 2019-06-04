/////////////////////////////////////////////////////////////////////
//
// sysBooks.inc
//
// Purpose
// contains functions used by the sysbook package.
//
//
// Creator: Yukiko
// Please note that I did not write some functions in this file.
// Some functions are taken from the Zulu Books package.
//
// Date 2019-05-28
//
/////////////////////////////////////////////////////////////////////

use datafile;
use file;

/////////////////////////////////////////////////////////////////////
//
// GetSysBookCount(shelf)
//
// Purpose
// This function counts the number of SysBooks in the bookshelf.
//
//
// Parameters
// shelf is an object reference to a bookshelf.
//
// Return value
// An integer count of SysBooks in the shelf.
//
/////////////////////////////////////////////////////////////////////
function GetSysBookCount(shelf)

	var count := 0;
	var contents := EnumerateItemsInContainer(shelf);

	foreach thing in contents
		if(GetObjProperty(thing, "SysBook"))
			count := count + 1;
		endif
	endforeach

	return count;

endfunction

/////////////////////////////////////////////////////////////////////
//
// RestockTheShelf(byref shelf, byref num_to_add)
//
// Purpose
// Restocks a bookshelf with SysBooks
//
//
// Parameters
// shelf - the shelf to restock.
// num_to_add - the number of books to add.
//
// Return value
// Nothing is returned.
//
/////////////////////////////////////////////////////////////////////
function RestockTheShelf(byref shelf, byref num_to_add)

	for i := 1 to num_to_add
		AddABook(shelf);
	endfor

endfunction

/////////////////////////////////////////////////////////////////////
//
// AddABook(bookcase)
//
// Purpose
// Adds one book to a bookshelf.
//
//
// Parameters
// shelf - The bookshelf in which to add the book.
//
// Return value
// No return value.
//
/////////////////////////////////////////////////////////////////////
function AddABook(byref shelf)
	var book := CreateItemInContainer(shelf, 0xFF0, 1);
	case(RandomInt(4)+1)
		1: book.graphic := 0xFEF;
		2: book.graphic := 0xFF0;
		3: book.graphic := 0xFF1;
		4: book.graphic := 0xFF2;
	endcase
	var bookContents := GetRandomBookContents();
	book.setprop("contents", bookContents.conents);
	book.setprop("title",    bookContents.title);
	book.setprop("author",   bookContents.author);
	book.name := bookContents.title;
	SetObjProperty(book, "NoWrite", 1);
	book.setprop("SysBook", 1);
endfunction

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetRandomBookContents()
//	Will return the contents of a randomly selected book from the datastore.
//
//	Returns: either a struct with the book content or 0 on error.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
function GetRandomBookContents()

	var books_data_file := OpenDataFile("staticbooks");

	if(!books_data_file)
		LogToFile("::log/sysbook.log", "[bookShelf/control.ecl]: Datastore file error: " + books_data_file.errortext, LOG_DATETIME);
		print("Sysbook error - check sysbook.log");
		return 0;
	endif
	var booklist_keys :=books_data_file.Keys();
	var bookid := booklist_keys.randomentry();
	var elem := books_data_file.FindElement(bookid);
	var book := struct{};

	book.author := elem.getprop("author");
	book.title := elem.getprop("title");
	book.conents := elem.getprop("contents");
	return book;

endfunction

///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	GetBookContents( bookid ) -
//	Will return the contents of the book at bookId in the datastore.
//
//	Returns: either a struct with the book content or 0 on error.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
function GetBookContents(bookid)

	var books_data_file := OpenDataFile("staticbooks");

	if(!books_data_file)
		LogToFile("::log/sysbook.log", "[bookShelf/control.ecl]: Datastore file error: " + books_data_file.errortext, LOG_DATETIME);
		print("Sysbook error - check sysbook.log");
		return 0;
	endif

	var elem := books_data_file.FindElement(bookid);
	var book := struct{};

	book.author := elem.getprop("author");
	book.title := elem.getprop("title");
	book.conents := elem.getprop("contents");
	return book;

endfunction

