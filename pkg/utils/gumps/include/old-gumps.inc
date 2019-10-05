/////////////////////////////////////////////////////////////////
//
// GUMPS.INC - v2.6
//
// Gump Functions - to make life easier! :) 
//	-Myr
// Tastes good with ketchup too.
//	-Austin
//
// Author: Myrathi <tjh@myrathi.co.uk>
// Maintainer: Austin <Austin@tsse.net>
//
//==Revision v2.7
// Updated: 25-Nov-2003, Madman
// - Added some additional BKGND constants supplied by Jouni Airaksinen back in June
//
//==Revision v2.6
// Updated: 23-Apr-2003, HellRazor
//  - Added GFCheckerTrans function for gumps with transparent backgrounds (code by Kiwi)
//
//==Revision v2.5
// Updated: 06-Jan-2003, Austin
// - Added GFHTMLArea()
//
//==Revision v2.4b
// Updated: 28-May-2002, Austin
//  - Fixed the scroll constant.
//  - Added an extra scroll constant.
//
//==Revision v2.4
// Updated: 05-Dec-2001, Austin
// - Fixed character width constants.
//
//==Revision v2.3
// Updated: 26-Nov-2001, Austin
// - Fixed a lot of the {} syntax into array() and struct.
// - Added ability to arbitrarily assign return values for buttons
// - Fixed text area. Will set it to "" if nothing is passed 
//   for the default text.
// - 'local' updated to 'var'
//
//==Revision v2.2==
// Updated: 15-Aug-2001, Myrathi
//  - Fixed a bug in GFUser2() that would stop compilation with
//    the latest ECompile executables (092+)
//  - Ensured 093/094 compatibility.
//
//==Revision v2.1==
// Updated: 24-Apr-2000, Myrathi
//  - Added BKGND_???????? 'gumpid' constants for GFResizePic()
//  - Added some missing characters to Shini's align-functions
//
//==Revision v2.0==
// Updated: 19-Apr-2000, Myrathi
//  - Couple of minor bugfixes.
//  - Updated the HTML help files.
//  - Code more robust. Moved to full revision v2.0! :)
//
//==Revision v1.3==
// Updated: 04-Feb-2000, Myrathi
//	- Added a sleepms() in places to stop runaway scripts! Yay!
//	- Added GFExtractData(): extracts string from TextEntry key
//
//==Revision v1.2==
// Updated: 30-Jan-2000, Myrathi
//	- Added storage/retrieval functions to allow multiple gumps
//	  in different functions in the same script! (headache)
//	- GFSendGumpV() displays a gump from a "stored gump" var. :)
//	- Added Shinigami's text-positioning functions...
//	  ( XGFTextWidth/Left/Mid/Right() )
//
//==Revision v1.1==
// Updated: 27-Jan-2000, Myrathi
//	- Changed the GFref variable to a structure (nicer)
//	- GFInitGump now accepts optional 'x', 'y' origin params.
//	- GFSetRef now returns the old {x,y} origin
//	- GFSetRefRel added - new origin is *relative* to old one
//	- GFTextCrop added. Supports "croppedtext" control
//	- GF[Get|Set]NextIndex and GFIncIndex functions added
//	- GF[Get|Set]Data and GFGet/SetLayout added to stop mixups
//	  between zero-based and one-based indexing. (Use 0-idxs!!)
//	- Appropriate functions now return a structure containing
//	  indexes to the changes in the layout array as well as
//	  data-index and/or keyid values (see headers or help file)
//	- Added constants for the index functions
//	- Added two internal helper-functions (XGFxxxxxxxx)
//
//==Revision v1.0==
// Updated: 25-Jan-2000, Myrathi
//	- GFInitGump now auto-inserts "nodispose".
//	- GFSetRadioGroup added for advanced radio-button support
//  - Minor bugs skwished. No other bugs found.
//  - Updated to full revision v1.0 :)
//
//==Revision v0.1a==
// Created: 24-Jan-2000, Myrathi
//  - Basic gump creation. A couple of minor bugs :/
//
// Notes:
//	+ All functions take hex values and convert to integer
//	  form, where necessary (gumps crash with hex values! Ugh!)
//
//	+ "Page 0" is the main background! It shows at ALL times!
//	  Anything you want on separate pages should be on page
//	  1, 2, 3 and so on showing 0+1, 0+2, 0+3, 0+4, etc, etc.
//
// To do:
//	+ Add the option of supplying the KeyIDs (is this wanted?)
//	+ Update the const-list of commonly used ID's (GFX + colours)
//  + Update "GFTextMid()" to cater for mid-x coord only (no 'w')
//
// Bugs:
//	- None known... now :)
//
// Thanks:
//	+ To Ignatius for his GUMPDOCS file which cleared up
//	  a couple of issues I was unsure of. :)
//	+ To Shinigami for "croppedtext" and his align-functions :]
//	+ To Horus for shutting up and letting me work! :P"
//
/////////////////////////////////////////////////////////////////

use uo;
use os;
use basic;

// Constants (for GF[Get|Set]NextIndex)
/////////////////////////////////////////////////////////////////
const GFIDX_TEXT        := 0x01;	// Static Text (TextLine)
const GFIDX_TEXTENTRY   := 0x02;	// Return Text (TextEntry)
const GFIDX_BUTTON      := 0x03;	// Action Buttons (ButtonID)
const GFIDX_CHECKBOX    := 0x04;	// Check-boxes
const GFIDX_RADIO       := 0x05;	// Radio Buttons
/////////////////////////////////////////////////////////////////

// Constants for index starting values (SHOULD be enough! Heh!)
/////////////////////////////////////////////////////////////////
const GFINIT_TEXT       := 0x000;	// Static Text (GFdata[])
const GFINIT_TEXTENTRY  := 0x201;	// Return Text (GFdata[])
const GFINIT_BUTTON     := 0x401;	// Action Buttons (KeyIDs)
const GFINIT_CHECKBOX   := 0x601;	// Check-boxes (KeyIDs)
const GFINIT_RADIO      := 0x801;	// Radio Buttons (KeyIDs)
/////////////////////////////////////////////////////////////////

// 'gumpid' Constants for the GFResizePic() function
/////////////////////////////////////////////////////////////////
const BKGND_SILVERTRIM          := 83;    // Fancy, dark-grey backing
const BKGND_SCROLL              := 2520;  // UO journal scroll
const BKGND_OLD_JOURNAL_SCROLL  := 5150;  // Old journal scroll
const BKGND_GOLDTRIM            := 2620;  // Gold-press, black backing
const BKGND_PAPER               := 3000;  // Very thin edging
const BKGND_PAPER_EX            := 3500;  //  \_with fancy inner border
const BKGND_BLACKSLAB           := 3600;  // Raised edge (blocky)
const BKGND_STONESLAB           := 5054;  // Plain, small corners
const BKGND_STONE               := 5100;  // Marble stone, rounded edge
const BKGND_DARK_STONE          := 5120;  // Same as previous, but much darker
/////////////////////////////////////////////////////////////////

// added by Jouni Airaksinen
//const BKGND_GRAY_SILVER                := 9100; // crashed client (?)
//const BKGND_GRAY_GOLD                  := 9150; // crashed client (?)
const BKGND_STONESLAB2                 := 9200; // transparent corners
const BKGND_SMOOTH_DARK_STONE          := 9250;
const BKGND_SMOOTH_DARK_STONE_BEIGE    := 9260;
const BKGND_BLACKHARD                  := 9270;
const BKGND_BEIGE_PAPER                := 9300;
const BKGND_WHITE_PAPER                := 9350;
const BKGND_BEIGE_SCROLL               := 9380;
const BKGND_WHITE_SCROLL               := 9390;
const BKGND_GRAY_GOLDROUNDED           := 9400;
const BKGND_GRAY_LIGHT_GOLDROUNDED     := 9450;
const BKGND_GRAY_PRESSED_GOLDROUNDED   := 9500;
const BKGND_GRAY_SILVERROUNDED         := 9550;
const BKGND_GRAY_LIGHT_SILVERROUNDED   := 9559;
const BKGND_GRAY_PRESSED_SILVERROUNDED := 9568;

//Standard text colors on TSSE
/////////////////////////////////////////////////////////////////
const StdColor := 1890;
const HeaderColor := 1890;
const SelectedColor := 1720;
/////////////////////////////////////////////////////////////////

// CONSTants for the GFAddButton() function
CONST GF_PAGE_BTN	:= 0x0;
CONST GF_CLOSE_BTN	:= 0x1;
CONST GF_DUMMY_BTN	:= 0x2;


// Function List
/////////////////////////////////////////////////////////////////
//
// GFSetRef( x := 0, y := 0 )
// GFSetRefRel( dx, dy );
// GFInitGump( sx := 0, sy := 0 )
// GFPage( page )
// GFNoClose()
// GFNoDispose()
// GFNoMove()
// GFUser1( line )
// GFUser2( l_line, d_pos, d_line )
// GFGumpPic( x, y, gumpid )
// GFResizePic( x, y, gumpid, w, h )
// GFHTMLArea(x, y, width, height, background:=1, scroll_bar:=1, text:="")
// GFTilePic( x, y, tileid )
// GFTextLine( x, y, colour, text );
// GFTextMid(x, y, w, colour, text );
// GFTextRight( rx, y, colour, text );
// GFTextCrop( x, y, w, h, colour, text );
// GFTextEntry( x, y, w, h, colour, text := "" )
// GFButtonPage( x, y, release, press, page )
// GFButtonID( x, y, release, press, close )
// GFCheckBox( x, y, off, on, state )
// GFRadioButton( x, y, off, on, state )
// GFSetRadioGroup( group )
//
// GFExtractData( keyid, keyarray )
//
// GFGetNextIndex( type )
// GFSetNextIndex( type, value )
// GFIncIndex( type )
//
// GFGetData( index )
// GFSetData( index, text )
// GFGetLayout( index )
// GFSetLayout( index, text )
//
// GFStoreGump()
// GFRestoreGump( gumpvar )
//
// GFSendGump( who )
// GFSendGumpV( who, gumpvar )
//
// GFCheckerTrans( x, y, w, h )
//
/////////////////////////////////////////////////////////////////

// Internal Functions
/////////////////////////////////////////////////////////////////
//
// XGFReset()
// XGFRetVal( lytidx := -1, dtidx := -1, keyid := -1 )
//
// XGFTextWidth(gumptext)
// XGFTextLeft(gumptext, x, width)
// XGFTextMid(gumptext, x, width)
// XGFTextRight(gumptext, rightx)
//
/////////////////////////////////////////////////////////////////

// Global variables
/////////////////////////////////////////////////////////////////
var GFref := struct;		// X- and Y-offsets for gump operations
    GFref.+x := 0;
    GFref.+y := 0;

var GFlayout := array();// Main layout array for entire gump
var GFdata := array();	// Storage array for static-text and
			//  text-entry initialisation strings
			// Indexes/KeyIDs for...
var GFdtidx;		// ...text in GFdata (index)
var GFtxtid;		// ...TeXT-entry returns (keyid)
var GFbutid;		// ...'action' BUTtons (keyid)
var GFcbxid;		// ...CheckBoXes (keyid)
var GFradid;		// ...RADio buttons (keyid)
/////////////////////////////////////////////////////////////////

// Return-value structure layout
/////////////////////////////////////////////////////////////////
//  rvsl.lytidx;	// ...index into layout array
//  rvsl.dtidx;	// ...index into data array
//  rvsl.keyid;	// ...KeyID allocated / other data (eg. page#)
/////////////////////////////////////////////////////////////////

// Global Initialisation (Just in case! :-P )
/////////////////////////////////////////////////////////////////
//
XGFReset();

// Function Definitions
/////////////////////////////////////////////////////////////////

// Sets the top-left co-ordinate origin (absolute)
//
// Returns: A structure {x,y} containing the old origin
//
function GFSetRef( x := 0, y := 0 )
	var oldref := GFref;
	GFref.x := CInt(x);
	GFref.y := CInt(y);
	return oldref;
endfunction

// Sets the top-left co-ordinate origin (relative to current)
//
// Returns: A structure {x,y} containing the old origin
//
function GFSetRefRel( dx, dy )
	var oldref := GFref;
	GFref.x := GFref.x + CInt(dx);
	GFref.y := GFref.y + CInt(dy);
	return oldref;
endfunction

// Initialises this include file's global variables
//
function GFInitGump( sx := 0, sy := 0 )
	XGFReset();
	GFSetRef( sx, sy );
	GFPage( 0 );
	GFNoDispose();
endfunction

// Gump page #x - start of definition
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = Page number
//
function GFPage( page )
	page := CInt(page);
	GFlayout.append( "page " + CStr(page) );
	return XGFRetVal( GFlayout.size(), -1, page );
endfunction

// Won't let the gump be closed with a right-click
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFNoClose()
	GFlayout.append( "noclose" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Won't let the gump be closed by clicking ESCAPE
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFNoDispose()
	GFlayout.append( "nodispose" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Disallows dragging of the gump.
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFNoMove()
	GFlayout.append( "nomove" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Adds a generic line to the GFlayout array
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFUser1( l_line )
	GFlayout.append( l_line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Adds a generic line to the GFlayout array and
// a line to the GFdata array at position 'd_pos'
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed
//
function GFUser2( l_line, d_pos, d_line )
	GFlayout.append( l_line );
	GFdata[CInt(d_pos)] := d_line;
	return XGFRetVal( GFlayout.size(), d_pos );
endfunction

// Single "art gump"
// (Uses "Gump" art from InsideUO)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFGumpPic( x, y, gumpid )
	var line := "gumppic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(gumpid));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Resized "gump" made up of art from 'gumpid' to 'gumpid+8'
// (Uses "Gump" art from InsideUO)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFResizePic( x, y, gumpid, w, h )
	var line := "resizepic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(gumpid)) + " " + CStr(CInt(w)) + " " + CStr(CInt(h));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

//
// Sets up an area on the gump that can be used as an html gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function GFHTMLArea(x, y, width, height, background:=1, scroll_bar:=1, text:="")
	//x y w h l# bg sb
	//"htmlgump 52 79 276 348 1 0 1",
	var line := "htmlgump "+CStr(GFref.x+CInt(x))+" "+CStr(GFref.y+CInt(y));
	line := line+" "+CStr(CInt(width))+" "+CStr(CInt(height));
	line := line+" "+CStr(GFdtidx);
	line := line+" "+CStr(CInt(background))+" "+CStr(CInt(scroll_bar));
	GFlayout.append(line);
	
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Single "tile" graphic
// (Uses "Static Tiles" from InsideUO [under Artwork])
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFTilePic( x, y, tileid )
	var line := "tilepic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(tileid));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Display a line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function GFTextLine(x, y, colour, text)
	text := CStr(text);
	var line := "text " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(colour)) + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Display a centred line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function GFTextMid(x, y, w, colour, text )
	text := CStr(text);
	x := XGFTextMid( text, CInt(x), w );
	var line := "text " + CStr(GFref.x + x) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(colour)) + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Display a right-aligned line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function GFTextRight( rx, y, colour, text )
	text := CStr(text);
	rx := XGFTextRight( text, CInt(rx) );
	var line := "text " + CStr(GFref.x + rx) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(colour)) + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Display a line of text on the gump but restrict it to a bounding box
// (shows '...' if it exceeds the rightmost edge)
//
// Syntax : x   y   w   h  colour  dtidx
// Example: 125 300 150 50 32      0
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function GFTextCrop( x, y, w, h, colour, text )
	text := CStr(text);
	var line := "croppedtext " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(w)) + " " + CStr(CInt(h)) + " " + CStr(CInt(colour));
	line := line + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Display a text-entry control on the gump
//
// Syntax : x   y   w   h  colour  txtid  dtidx
// Example: 125 300 150 50 32      1050   0
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//	.keyid  = KeyID allocated to the text when the Gump returns
//	          (This method is DAFT! But hey, I didn't write it! :-P" )
//              (Returns eg:  "Key=2024, Value=2024: This is the text" )
//
function GFTextEntry( x, y, w, h, colour, text := "" )
	if ( text == error )
		text := "";
	else
		text := CStr(text);
	endif
	
	var line := "textentry " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(w)) + " " + CStr(CInt(h)) + " " + CStr(CInt(colour));
	line := line + " " + CStr(GFtxtid) + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	GFtxtid := GFtxtid + 1;		// Update KeyID (txtid)
	return XGFRetVal( GFlayout.size(), GFdtidx - 1, GFtxtid - 1 );
endfunction

// Standard "button" syntax:
// "button x y release press close page keyid"

// Display a button that changes to another page
//
// Syntax: x, y, release, press, 0, page
// (close=0 - don't want it closing when it's changing page, eh?)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = Page number that the button changes to
//
function GFButtonPage( x, y, release, press, page )
	// "button x y release press 0 page"
	var line := "button " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(release)) + " " + CStr(CInt(press));
	line := line + " 0 " + CStr(CInt(page));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size(), -1, page );
endfunction

// Display an "action button" (returns a key value and can close gump)
//
// Syntax: x, y, release, press, close, 0, keyid
// (page=0 - it returns a value instead of changing page)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = KeyID allocated to the button - returned from Gump if pressed
//	          (butid)
//
function GFButtonID( x, y, release, press, close, id := 0 )
	// "button x y release press close 0 key"
	var line := "button " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(release)) + " " + CStr(CInt(press)) + " ";
	if ( close == 0 )
		line := line + "0 0 ";
	else
		line := line + "1 0 ";
	endif
	id := CInt(id);		// Ensure it's an integer
	if ( id < 1 )		// Catch errors (0) and invalid (negative) values
		id := GFbutid;
		GFbutid := GFbutid + 1;		// Update KeyID (butid) (only if used)
	endif
	line := line + CStr(id);
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size(), -1, id );
endfunction

// Displays a check-box (off/on)
//
// Syntax: x, y, off, on, initial state (0/1), cbxid
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = KeyID allocated to the checkbox (cbxid)
//
function GFCheckBox( x, y, off, on, state, id := 0)
	var line := "checkbox " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(off)) + " " + CStr(CInt(on)) + " ";
	if ( state == 0 )
		line := line + "0 ";
	else
		line := line + "1 ";
	endif
	id := Cint(id);
	if (id < 1)
		id := GFcbxid;
		GFcbxid := GFcbxid + 1;	
	endif
	line := line + CStr(id);

	GFlayout.append( line );
	return XGFRetVal( GFlayout.size(), -1, id );
endfunction

// Displays a radio button (off/on)
//
// Syntax: x, y, off, on, initial state (0/1), radid
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = KeyID allocated to the radio-button (radid)
//
function GFRadioButton( x, y, off, on, state, id := 0)
	if (state == error)
		state := 0;
	endif
	var line := "radio " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(off)) + " " + CStr(CInt(on)) + " ";
	if ( state == 0 )
		line := line + "0 ";
	else
		line := line + "1 ";
	endif
	id := CInt(id);
	if (id < 1)
		id := GFradid;
		GFradid := GFradid + 1;
	endif
	line := line + CStr(id);
	
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size(), -1, id );
endfunction

// Groups radio button (I think)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = The group number (integer form)
//
function GFSetRadioGroup( group )
	group := CInt(group);
	GFlayout.append( "group " + CStr(group) );
	return XGFRetVal( GFlayout.size(), -1, group );
endfunction

// Extracts string from a TextEntry key
//
// Returns: The text string
//
function GFExtractData( keyid, keyarray )
	var keystr := CStr(keyid)+": ";
	var datstr := keyarray[CInt(keyid)];
	var retval := datstr[len(keystr)+1,len(datstr)];
	sleepms(2);
	return retval;
endfunction

// Returns: Value of the next index to be used
//
function GFGetNextIndex( type )
	sleepms(2);
	case ( type )
		GFIDX_TEXT:
			return GFdtidx;
		GFIDX_TEXTENTRY:
			return GFtxtid;
		GFIDX_BUTTON:
			return GFbutid;
		GFIDX_CHECKBOX:
			return GFcbxid;
		GFIDX_RADIO:
			return GFradid;
		default:
			print( "GFGetNextIndex(): Bad type" );
	endcase
	return;
endfunction

// Sets the next index to be used
//
// Returns: The old index value
//
function GFSetNextIndex( type, value )
	var oldidx;
	value := CInt(value);
	sleepms(2);
	case ( type )
		GFIDX_TEXT:
			oldidx := GFdtidx;
			GFdtidx := value;
		GFIDX_TEXTENTRY:
			oldidx := GFtxtid;
			GFtxtid := value;
		GFIDX_BUTTON:
			oldidx := GFbutid;
			GFbutid := value;
		GFIDX_CHECKBOX:
			oldidx := GFcbxid;
			GFcbxid := value;
		GFIDX_RADIO:
			oldidx := GFradid;
			GFradid := value;
		default:
			print( "GFSetNextIndex(): Bad type" );
			return;
	endcase
	return oldidx;
endfunction

// Increments the current <type> index by one
//
// Returns: The new index value
//
function GFIncIndex( type )
	var oldidx := GFSetNextIndex( type, GFGetNextIndex( type ) + 1 );
	return (oldidx + 1); // Because it's just been incremented! :P"
endfunction

// Gets text from GFdata[]
//
// Returns: the text
//
function GFGetData( index )
	// Internally, the gump uses 0-based indexing...
	// Take that into account when altering the 1-based array
	return GFdata[index+1];
endfunction

// Set the text in GFdata[]
//
// **WARNING** NO ERROR CHECKING!
//
// Returns: Index changed
//
function GFSetData( index, text )
	// Internally, the gump uses 0-based indexing...
	// Take that into account when altering the 1-based array
	GFdata[index+1] := text;
	return index;
endfunction

// Get a text item from the layout array
//
// Returns: the text
//
function GFGetLayout( index )
	return GFlayout[index];
endfunction

// Sets a specified text item in the layout array
//
// **WARNING** NO ERROR CHECKING!
//
// Returns: Index changed
//
function GFSetLayout( index, text )
	GFlayout[index] := text;
	return index;
endfunction

// Stores a structure containing the gump and extra details
//
// Returns: A structure:
//	.layout =	A copy of GFlayout[]
//	.data   =	A copy of GFdata[]
//	.indexes
//		.dt	= Copy of GFdtidx
//		.txt	= Copy of GFtxtid
//		.but	= Copy of GFbutid
//		.cbx	= Copy of GFcbxid
//		.rad	= Copy of GFradid
//
function GFStoreGump()
	var idx := struct;
	idx.+dt := GFdtidx;
	idx.+txt := GFtxtid;
	idx.+but := GFbutid;
	idx.+cbx := GFcbxid;
	idx.+rad := GFradid;

	var ret := struct;
	ret.+layout := GFlayout;
	ret.+data := GFdata;
	ret.+indexes := idx;

	return ret;
endfunction

// Restores a gump layout from a structure saved by GFStoreGump()
//
// Returns:
//	0 = Failure
//	1 = Success
//
function GFRestoreGump( gumpvar )
	if ( !(gumpvar.layout) or !(gumpvar.data) or !(gumpvar.indexes) )
		print("GFRG(): missing one of .layout/.data/.indexes");
		return 0;
	endif
	var idx := gumpvar.indexes;
	if ( !(idx.dt) or !(idx.txt) or !(idx.but) or !(idx.cbx) or !(idx.rad) )
		print("GFRG(): missing index value");
		return 0;
	endif

	GFlayout := gumpvar.layout;
	GFdata := gumpvar.data;
	GFdtidx := idx.dt;
	GFtxtid := idx.txt;
	GFbutid := idx.but;
	GFcbxid := idx.cbx;
	GFradid := idx.rad;
	return 1;
endfunction

// Call this to display/use the defined gump
//
// Returns: an array of keys and values
//          (ie. what the SendDialogGump() returns)
//
// NOTE: This WILL crash the client if you've created duff arrays!!
//
function GFSendGump( who )
	return SendDialogGump( who, GFlayout, GFdata );
endfunction

// Call this to display a gump using "gumpvar"
//
// Returns: an array of keys and values
//		(ie. what the SendDialogGump() returns)
//
// NOTE: This WILL crash the client if you've corrupted the array!
//
function GFSendGumpV( who, gumpvar )
	if ( !(gumpvar.layout) or !(gumpvar.data) )
		return;
	endif
	return SendDialogGump( who, gumpvar.layout, gumpvar.data );
endfunction

/////////////////////////////////////////////////////////////////
//
// INTERNAL FUNCTIONS
//
/////////////////////////////////////////////////////////////////

// Called (internally) to reset all the variables.
//
function XGFReset( )
	GFlayout := array();
	GFdata := array();
	GFdtidx := GFINIT_TEXT;
	GFtxtid := GFINIT_TEXTENTRY;
	GFbutid := GFINIT_BUTTON;
	GFcbxid := GFINIT_CHECKBOX;
	GFradid := GFINIT_RADIO;
endfunction

// Called (internally) to setup the return structure
// of a function
//
//  rvsl.lytidx;	// ...index into layout array
//  rvsl.dtidx;	// ...index into data array
//  rvsl.keyid;	// ...KeyID allocated / other data (eg. page#)
//
function XGFRetVal( lytidx := -1, dtidx := -1, keyid := -1 )
	var rvsl := struct;
	if ( lytidx > -1 )
		rvsl.+lytidx := lytidx;
	endif
	if ( dtidx > -1 )
		rvsl.+dtidx := dtidx;
	endif
	if ( keyid > -1 )
		rvsl.+keyid := keyid;
	endif
	sleepms(2);	// Will hopefully stop runaway script problem :/

	return rvsl;
endfunction

/////////////////////////////////////////////////////////////////
//  Author:    Shinigami
//  eMail:     Shinigami@gmx.net
//  Modified:  Myrathi
/////////////////////////////////////////////////////////////////

// Constants
/////////////////////////////////////////////////////////////////
var aGFTlength := struct;
       aGFTlength.+charsf:=   // Space as Filler is not allowed
         "||||||||||||||"+ //  2
         "`',.;:::::::::"+ //  3
         "!!!!!!!!!!!!!!"+ //  4
         "il1[]]]]]]]]]\""+ //  5
         "bcejot”(){}==="+ //  6
         "adfpqrszI„~~~~"+ //  7
         "ghnuvy <>-----"+ //  8
         "xJ578$?+++++++"+ //  9
         "kCE2369^/*****"+ // 10
         "mwABDFOPŽ™40%%"+ // 11
         "GLNQSZ@#######"+ // 12
         "HRT&__________"+ // 13
         "KšUYYYYYYYYYYY"+ // 14
         "XXXXXXXXXXXXXX"+ // 15
         "VVVVVVVVVVVVVV"+ // 16
         "MMMMMMMMMMMMMM"+ // 18
         "WWWWWWWWWWWWWW"; // 21
       aGFTlength.+width:= array
       (
          2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
          12, 13, 14, 15, 16, 18, 21
       );
/////////////////////////////////////////////////////////////////

// Functions
/////////////////////////////////////////////////////////////////

// Returns the Width of GumpText
//
Function XGFTextWidth(gumptext)
  var points:=0;
  var j;

  For (j:=len(gumptext); j>0; j:=j-1)
    var i:=Find(aGFTlength.charsf, gumptext[j], 1)-1;
    If (i>=0)
      points:=points+aGFTlength.width[CInt(i/14)+1];
    Else
      print("XGFTextWidth: Unknown Character = '"+gumptext[j]+"'");
    EndIf
  EndFor

  Return (points);
EndFunction

// Returns the X-Value (Left-Alignment)
//
Function XGFTextLeft(gumptext, x, width)
  gumptest := gumptest;
  width := width;
  Return (x);
EndFunction

// Returns the X-Value (Mid-Alignment)
//
Function XGFTextMid(gumptext, x, width)
  Return (x+CInt((width-XGFTextWidth(gumptext))/2));
EndFunction

// Returns the X-Value (Right-Alignment)
//

Function XGFTextRight(gumptext, rightx)
  Return (rightx-XGFTextWidth(gumptext));
EndFunction

// Creates gump with tranparent background
//
function GFCheckerTrans( x, y, w, h )
	var line := "checkertrans" + " " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(w)) + " " + CStr(CInt(h));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

/*
 * GFCreateGump(x, y, width, height)
 *
 * Purpose
 * Creates the base gump.
 * Allows a gump to be handled more like an object.
 *
 * Parameters
 * x:	   The top-left spot of the X axis.
 * y:	   The top-left spot of the Y axis.
 * width:   The gump's total width.
 * height:  The gump's total height.
 *
 * Return value
 * The gump object.
 *
 */
function GFCreateGump(x:=0, y:=0, width:=0, height:=0)
	var gump := struct;

	gump.+base := struct;
	// X- and Y-offsets for gump operations
	gump.base.+x := CInt(x);
	gump.base.+y := CInt(y);
	// Background width and length for gump
	gump.base.+width := CInt(width);
	gump.base.+height := CInt(height);

	//Used for tracking the location of certain gump settings
	//in the layout array. This allows them to be enabled/disabled at will.
	gump.base.+MoveLoc := 0;	// Movable flag location
	gump.base.+CloseLoc := 0;	// NoClose flag location
	gump.base.+DisposeLoc := 0;	// NoDispose flag location

	gump.+layout	:= array;	// Main layout array for entire gump
	gump.+data	:= array;	// Storage array for static-text and
					// text-entry initialisation strings
					// Indexes/KeyIDs for...
	
	// Starting points for data values
	gump.+data_id	:= 1;
	
	// Current page the gump is writing to
	gump.+cur_page := 0;

	return(gump);
endfunction

/*
 * GFClosable(byref gump, bool)
 *
 * Purpose
 * Sets the gump closable 1/0.
 *
 * Parameters
 * gump:	Reference to the gump
 * bool:	0: the gump is not closable
 *		  1: the gump is closable
 *
 * Return value
 * Returns 1 on success, error on failure.
 *
 */
 function GFClosable(byref gump, bool)
	case(bool)
		0:
			if ( gump.base.CloseLoc )
				return XGFError("Gump already contains the 'NoClose' flag.");
			endif

			(gump.layout).Append("NoClose");
			gump.base.CloseLoc := (gump.layout).Size();
			break;
		1:
			if ( !gump.base.CloseLoc )
				return XGFError("Gump is already closable.");
			endif

			(gump.layout).Erase(gump.base.CloseLoc);
			gump.base.CloseLoc := 0;
			break;
		default:
			return XGFError("gumps::Closable: Invalid bool value '"+bool+"'");
	endcase
	return 1;
 endfunction

/*
 * GetCfgConst(elem_name, prop_name)
 *
 * Purpose
 * Rather than have a lot of CONSTants at the top for
 * different gump images, they are placed in GumpInfo.cfg
 * and retrieved with this function. 
 * The idea is that it is easier to update GumpInfo.cfg than
 * to recompile all scripts that use these includes.
 *
 * Parameters
 * elem_name:	The elem of the config to look at.
 * prop_name:	The name of the CONSTant to retrieve.
 *
 * Return value
 * An integer for the gump graphical number
 *
 */
function GetCfgConst(elem_name, prop_name)
	var cfg := ReadConfigFile(":gumps:config/GumpInfo");
	if ( cfg.errortext )
		return XGFError("GetCfgConst():: Unable to open GumpInfo.cfg -> "+cfg.errortext);
	endif
	
	var elem := cfg[elem_name];
	if ( elem.errortext )
		return XGFError("GetCfgConst():: Unable to find GumpInfo.cfg["+elem_name+"] ->"+elem.errortext);
	endif
	
	var value := GetConfigInt(elem, prop_name);
	if ( !value )
		return XGFError("GetCfgConst():: Unable to find property ["+prop_name+"] in elem ["+elem_name+"]");
	endif
	
	return value;
endfunction

/*
 * GFResizePic2(byref gump, x, y, gump_id, width, height)
 *
 * Purpose
 * Adds a background to the gump.
 *
 * Parameters
 * gump:	Reference to the gump.
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * gump_id:	The decimal number representing the gump.
 * width:	The width (pixels) of the gump from position X.
 * height:	The height (pixels) of how far to scale the gump downwards from position Y.
 *
 * Return value
 * No return value.
 *
 */
function GFResizePic2(byref gump, x, y, gump_id, width, height)
	width := CInt(width);
	height := CInt(height);
	if ( !width )
		width := gump.base.width;
	endif
	if ( !height )
		height := gump.base.width;
	endif

	(gump.layout).Append("ResizePic "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(gump_id))+" "+CStr(width)+" "+CStr(height));
	return 1;
endfunction

/*
 * GFWordWrap(text, pixels)
 *
 * Purpose
 * Breaks a string up into an array of strings.
 * Each string will be less than or equal to 'pixels' in width.
 *
 * Parameters
 * text:	The text to break up.
 * pixels:	The maximum width a line of text can be.
 *
 * Return value
 * An array of strings
 *
 */
function GFWordWrap(text, pixels)
	var wrap_lines := array{};
	var word_list := SplitWords(text);
	//var num_words := word_list.Size();

	var space_size := GFTextWidth(" ");

	var cur_line := "";
	var cur_length := 0;
	var start_new := 0;

	foreach word in ( word_list )
		var word_width := GFTextWidth(word);

		if ( word == "[P]" )
			// [P] will force a line break
			word := "";
			word_width := 0;
			start_new := 1;
		elseif ( (word_width+cur_length) > pixels )
			start_new := 1;
			if ( cur_line[Len(cur_line)] == " " )
				cur_line[Len(cur_line)] := "";
			endif
		endif

		if ( start_new )
			start_new := 0;
			wrap_lines.Append(cur_line);
			cur_line := "";
			cur_length := 0;
		endif
		if ( word )
			cur_line += word+" ";
			cur_length += word_width+space_size;
		endif

		SleepMS(2);
	endforeach

	//Make sure nothing got left behind.
	if ( wrap_lines[wrap_lines.Size()] != cur_line )
		if ( cur_line[Len(cur_line)] == " " )
			cur_line[Len(cur_line)] := "";
		endif
		wrap_lines.Append(cur_line);
	endif

	return wrap_lines;
endfunction

/*
 * GFTextLine2(byref gump, x, y, hue, text)
 *
 * Purpose
 * Adds a text label to the gump.
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * hue:		The decimal number representing the Label's hue
 * text:	The string that is to be exposed.
 *
 * Return value
 * Returns where in the data array the text was placed.
 *
 */
function GFTextLine2(byref gump, x, y, hue:=0, text:="")
	gump.data.Append(""+CStr(text));
	var index := gump.data.Size()-1;
	(gump.layout).Append("Text "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(hue))+" "+CStr(index));

	return (index);
endfunction

/*
 * GFAddButton(byref gump, x, y, off_id, on_id, btn_type, btn_value)
 *
 * Purpose
 * Adds a button (page/reply) to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * off_id:	The release-id (graphic) of the button.
 * on_id:	The pressed-id (graphic) of the button.
 * btn_type	The type of button to use:
 *		GF_PAGE_BTN	Will move the gump to another page.
 *		GF_CLOSE_BTN	Will close the gump and return the value of the button and other data.
 *		GF_DUMMY_BTN	Seems to do nothing but change the graphic when you click.
 * btn_value	If the button type if set to a page, it will go to the assigned page number.
 *		Otherwise, if clicked, it will return the value. If set to 0, it will assign the
 *		next available usable number.
 *
 * Return value
 * Return value is the button value.
 *
 */
function GFAddButton(byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0)
	var line := "Button "+CStr(x)+" "+CStr(y)+" "+CStr(off_id)+" "+CStr(on_id);

	btn_value := CInt(btn_value);
	if ( btn_value < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id := gump.data_id+1;
		btn_value := gump.data_id;
	endif

	if ( btn_type == GF_CLOSE_BTN )
		line := line+" 1 0 "+CStr(btn_value);
	elseif ( btn_type == GF_DUMMY_BTN )
		line := line+" 0 0 "+CStr(btn_value);
	else // Assume GF_PAGE_BTN
		line := line+" 0 "+CStr(btn_value);
	endif

	(gump.layout).Append(line);

	return (btn_value);
endfunction

/*
 * GFSendGump2(who, gump)
 *
 * Purpose
 * Sends the gump to the character specified.
 *
 * Parameters
 * who:		Reference to the character which should receive the gump.
 * gump:	The gump itself, consisting of it's layout and its data.
 *
 * Return value
 * The SendDialogGump().
 *
 */
function GFSendGump2(who, byref gump)
	var ret_val := SendDialogGump(who, gump.layout, gump.data, gump.base.x, gump.base.y);
	if ( ret_val.errortext )
		//Sometimes comes up when the gump has too much information.
		SendSysMessage(who, "Error - Unable to display gump.");
		SendSysMessage(who, "Error text is ->"+ret_val.errortext);
	endif
	
	return ret_val;
endfunction

/*
 * XGFErrorSetup(who, gump)
 *
 * Purpose
 * Internal function for assembling an error and making a system log.
 *
 * Parameters
 * text:	The errortext of the error.
 *
 * Return value
 * error with a .errortext member.
 *
 */
function XGFError(text)
	var temp := error{"errortext":=text};
	
	SysLog("Gump Error:: "+text);

	return temp;
endfunction

/*
 * GFGetTextWidth(char)
 *
 * Purpose
 * To figure how the width in pixels of text on a gump.
 *
 * Parameters
 * gump_text:	The text to get the pixel width of.
 *
 * NOTE: This needs to be expanded to support fonts - and possibly be config driven?
 *
 * Return value
 * The width, in pixels, of the text's width on a gump.
 *
 */
function GFTextWidth(gump_text)
	var gft_length := struct;
	gft_length.+charsf :=
	"||||||||||||||"+	//  2
	"`',.;:::::::::"+	//  3
	"!!!!!!!!!!!!!!"+	//  4
	"il1[]]]]]]]]]\""+	//  5
	"bcejot”(){}==="+	//  6
	"adfpqrszI„~~~~"+	//  7
	"ghnuvy <>-----"+	//  8
	"xJ578$?++++++ "+	//  9
	"kCE2369^/*****"+	// 10
	"mwABDFOPŽ™40%%"+	// 11
	"GLNQSZ@#######"+	// 12
	"HRT&__________"+	// 13
	"KšUYYYYYYYYYYY"+	// 14
	"XXXXXXXXXXXXXX"+	// 15
	"VVVVVVVVVVVVVV"+	// 16
	"MMMMMMMMMMMMMM"+	// 18
	"WWWWWWWWWWWWWW";	// 21
	gft_length.+width := array
	{
		2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 18, 21
	};

	var points:=0;
	var j := Len(gump_text);

	for ( j; j>0; j-=1 )
		var i := Find(gft_length.charsf, gump_text[j], 1) - 1;
		if ( i>=0 )
			points += gft_length.width[CInt(i/14)+1];
		else
			print("XGFTextWidth: Unknown Character = '"+gump_text[j]+"'");
		endif
	endfor

	return (points);
endfunction



/* 
 * GumpPrompt(mobile, text, movable:=0, hue:=1153)
 *
 * Purpose
 * Template to create a simple prompt window.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * text:	Text to display on the prompt gump.
 * movable:	0 - Dialog will not be movable
 *		1 - Dialog will be movable.
 * hue:     colour of the text.
 *
 * Return value
 * Returns 1 when the okay button has been clicked.
 *
 */
function GumpPrompt(mobile, text, movable:=0, hue:=1153)
	var gump_prompt := GFCreateGump(180, 100);
	if ( !movable )
		GFClosable(gump_prompt, 0);
	endif
	GFResizePic2(gump_prompt, 0, 0, GetCfgConst("Defaults", "BackGround"), 300, 160);
	GFResizePic2(gump_prompt, 15, 15, GetCfgConst("Defaults", "ForeGround"), 270, 130);
		
	var y_pos := 25;
	foreach line in (GFWordWrap(text, 260))
		GFTextLine2(gump_prompt, 20, y_pos, hue, line);
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach
	
	GFAddButton(gump_prompt, 120, 120, 2128, 2129, GF_CLOSE_BTN, 1);
	
	var prompt_input := GFSendGump2(mobile, gump_prompt);
	return prompt_input[0];
endfunction
