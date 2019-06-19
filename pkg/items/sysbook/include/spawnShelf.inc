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
use file;

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

	var bookContents := GetRandomBookContents();

	// If the book is not set to spawn, then do not create it in the shelf.
	if(!bookContents.spawn)
		return 0;
	endif

	var book := CreateItemInContainer(shelf, 0xFF0, 1);
	case(RandomInt(4)+1)
		1: book.graphic := 0xFEF;
		2: book.graphic := 0xFF0;
		3: book.graphic := 0xFF1;
		4: book.graphic := 0xFF2;
	endcase
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

	var books_data_file := OpenDataFile("shard_library");

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
	book.spawn := elem.getprop("spawn");
	return book;

endfunction

