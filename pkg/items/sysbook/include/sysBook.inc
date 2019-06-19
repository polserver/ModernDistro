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
use math;

/////////////////////////////////////////////////////////////////////
//
// RetrieveOrInitShardLibrary()
//
// Purpose
// Tries to open the shard_library data file. If that is not
// successful, it assumes it does not exist and so it tries to
// create the shard_library data file. If no success it returns
// an error.
//
// Parameters
// None
//
// Return value
// Object reference to the shard_library data file or error.
//
/////////////////////////////////////////////////////////////////////
function RetrieveOrInitShardLibrary()
	var shard_library := OpenDataFile("shard_library");
	if(shard_library.errortext)
		Print(shard_library.errortext);
		shard_library := CreateDataFile("shard_library", DF_KEYTYPE_INTEGER);
	endif
	return shard_library;
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

	var books_data_file := OpenDataFile("shard_library");

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

function BookExists(df, bookid)
  return(df.FindElement(bookid) != error);
endfunction

function CreateBook(df, bookid, title, author, contents, spawn := 0)
  var elem := df.CreateElement(bookid);
  elem.SetProp("author", author);
  elem.SetProp("title", title);
  elem.SetProp("contents", contents);
  elem.SetProp("spawn", spawn);
endfunction

/////////////////////////////////////////////////////////////////////
//
// InitializeBookGump( byref mobile, byref title_list )
//
// Purpose:
// Displays a gump built from an array of strings, book titles.
// The titles are displayed in a vertical list with two checkboxes
// on the left. One for inclusion of the book in the Shard Library
// and one to designate whether the selected book will spawn in the
// book shelves set-up for book spawns.
//
// Parameters:
// mobile - a mobile reference.
// title_list - an array of book titles.
//
// Returns:
// An array of struncts. Each strunct has the following members:
// Spawn -	An integer set to 1 if the Spawn checkbox is checked or
// 			0 if the checkbox is not checked.
// Title -	A string containing the book title.
//
// Author:
// ThisIsMe
//
/////////////////////////////////////////////////////////////////////
function InitializeBookGump( byref mobile, byref title_list, gump_caption := "" )
	var gump := BuildBaseGump(gump_caption),
		selected_book := 0, // Stores key value of checked books...
		book_struct := Struct,
		book_array := Array,
		books_selected := 0;

	BuildBookList( gump, title_list );

	var result := GFSendGump( mobile, gump );
	if( 5000 in result.keys )
		foreach key in ( result.keys )
			if( key >= 1 && key <= 1000 )
				selected_book := key;
				book_struct.+Title := title_list[key];
				books_selected := 1;
				if( !( CInt( selected_book+1000 ) in result.keys ) )
					book_struct.+Spawn := 0x0;
				else
					book_struct.+Spawn := 0x1;
				endif
				book_array.Append( book_struct );
				book_struct := Struct; // Reset struct, else you will have a right mess of things...
			endif
		endforeach
	endif
	if(!books_selected)
		return error{"errortext":="No option was selected."};
	endif
	return book_array;
endfunction

function BuildBaseGump(header)

	var gump := GFCreateGump( 50,50 );

	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9250, 486, 445 );
	GFPicTiled( gump, 13, 13, 460, 419, 3004 );
	GFHTMLArea( gump, 15, 15, 456, 40, "<center>" + header + "</center>", 0, 0 );
	GFPicTiled( gump, 15, 38, 456, 27, 2624 );
	GFPicTiled( gump, 16, 39, 70, 25, 3004 );
	GFHTMLArea( gump, 18, 42, 70, 20, "SELECT", 0, 0 );
	GFPicTiled( gump, 87, 39, 70, 25, 3004 );
	GFHTMLArea( gump, 90, 42, 70, 20, "SPAWN", 0, 0 );
	GFPicTiled( gump, 158, 39, 312, 25, 3004 );
	GFHTMLArea( gump, 161, 42, 309, 36, "<center>TITLE</center>", 0, 0 );
	GFPicTiled( gump, 15, 405, 456, 1, 2624 );
	GFAddButton( gump, 441, 408, 4023, 4025, GF_CLOSE_BTN, 5000 );

	return gump;
endfunction

function BuildBookList( byref gump, byref title_list )
	var i := 1,
		y := 67,
		max_pages := CInt( title_list.Size() / 10 );

	if( Abs( max_pages - ( max_pages * 10 ) ) > 0 )
		max_pages := max_pages + 1;
	endif

	GFPage( gump, 1 );
	for( i:=1; i<=title_list.Size(); i:=i+1 )
		GFCheckBox( gump, 16, y, 9792, 9793, 0, 0+i );
		GFCheckBox( gump, 88, y, 9792, 9793, 1, 1000+i );
		GFHTMLArea( gump, 158, y+3, 309, 26, title_list[i], 1, 0 );

		y := y+31;
		if( ( ( i % 10 == 0 ) && ( i <= title_list.Size() ) ) || i == title_list.Size() )
			GFHTMLArea( gump, 36, 384, 309, 36, "Page "+gump.cur_page+" of "+max_pages, 0, 0 );
			if( i != title_list.Size() )
				GFAddButton( gump, 380, 381, 4005, 4007, GF_PAGE_BTN, gump.cur_page+1 );
				GFAddHTMLLocalized( gump, 412, 384, 60, 20, 1011066, 0, 0, 0 ); // Next Page
			endif
			GFPage( gump, gump.cur_page+1 );
			GFAddButton( gump, 268, 381, 4014, 4016, GF_PAGE_BTN, gump.cur_page-1 );
			GFAddHTMLLocalized( gump, 300, 384, 140, 20, 1011067, 0, 0, 0 ); // Previos Page
			y := 67;
		endif
	endfor

endfunction
