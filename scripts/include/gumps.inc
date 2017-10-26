/////////////////////////////////////////////////////////////////
//
// GUMPS.INC - v2.4
//
// Gump Functions - to make life easier! :) 
//	-Myr
// Tastes good with ketchup too.
//	-Austin
//
// Author: Myrathi <tjh@myrathi.co.uk>
//
//==Revision v2.4b
// Updated: 28-May-2002, Austin
//  - Fixed the scroll constant.
//  - Added an extra scroll constant.
//==Revision v2.4
// Updated: 26-Nov-2001, Austin
//  - Fixed character width constants.
//  - Added another gump background 'BKGND_DARK_STONE'
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
//  - Added a sleepms() in places to stop runaway scripts! Yay!
//  - Added GFExtractData(): extracts string from TextEntry key
//
//==Revision v1.2==
// Updated: 30-Jan-2000, Myrathi
//  - Added storage/retrieval functions to allow multiple gumps
//    in different functions in the same script! (headache)
//  - GFSendGumpV() displays a gump from a "stored gump" var. :)
//  - Added Shinigami's text-positioning functions...
//    ( XGFTextWidth/Left/Mid/Right() )
//
//==Revision v1.1==
// Updated: 27-Jan-2000, Myrathi
//  - Changed the GFref variable to a structure (nicer)
//  - GFInitGump now accepts optional 'x', 'y' origin params.
//  - GFSetRef now returns the old {x,y} origin
//  - GFSetRefRel added - new origin is *relative* to old one
//  - GFTextCrop added. Supports "croppedtext" control
//  - GF[Get|Set]NextIndex and GFIncIndex functions added
//  - GF[Get|Set]Data and GFGet/SetLayout added to stop mixups
//    between zero-based and one-based indexing. (Use 0-idxs!!)
//  - Appropriate functions now return a structure containing
//    indexes to the changes in the layout array as well as
//    data-index and/or keyid values (see headers or help file)
//  - Added constants for the index functions
//  - Added two internal helper-functions (XGFxxxxxxxx)
//
//==Revision v1.0==
// Updated: 25-Jan-2000, Myrathi
//  - GFInitGump now auto-inserts "nodispose".
//  - GFSetRadioGroup added for advanced radio-button support
//  - Minor bugs skwished. No other bugs found.
//  - Updated to full revision v1.0 :)
//
//==Revision v0.1a==
// Created: 24-Jan-2000, Myrathi
//  - Basic gump creation. A couple of minor bugs :/
//
// Notes:
//  + All functions take hex values and convert to integer
//    form, where necessary (gumps crash with hex values! Ugh!)
//
//  + "Page 0" is the main background! It shows at ALL times!
//    Anything you want on separate pages should be on page
//    1, 2, 3 and so on showing 0+1, 0+2, 0+3, 0+4, etc, etc.
//
// To do:
//  + Add the option of supplying the KeyIDs (is this wanted?)
//  + Update the const-list of commonly used ID's (GFX + colours)
//  + Update "GFTextMid()" to cater for mid-x coord only (no 'w')
//
// Bugs:
//  - None known... now :)
//
// Thanks:
//  + To Ignatius for his GUMPDOCS file which cleared up
//    a couple of issues I was unsure of. :)
//  + To Shinigami for "croppedtext" and his align-functions :]
//  + To Horus for shutting up and letting me work! :P"
//
/////////////////////////////////////////////////////////////////

//***** Copied from Hope WoD scripts for housing system

use uo;
use os;
use basic;

// Constants (for GF[Get|Set]NextIndex)
/////////////////////////////////////////////////////////////////
const GFIDX_TEXT	:= 0x01;	// Static Text (TextLine)
const GFIDX_TEXTENTRY	:= 0x02;	// return Text (TextEntry)
const GFIDX_BUTTON	:= 0x03;	// Action Buttons (ButtonID)
const GFIDX_CHECKBOX	:= 0x04;	// Check-boxes
const GFIDX_RADIO	:= 0x05;	// Radio Buttons
/////////////////////////////////////////////////////////////////

// Constants for index starting values (SHOULD be enough! Heh!)
/////////////////////////////////////////////////////////////////
const GFINIT_TEXT	:= 0x000;	// Static Text (GFdata[])
const GFINIT_TEXTENTRY	:= 0x201;	// return Text (GFdata[])
const GFINIT_BUTTON	:= 0x401;	// Action Buttons (KeyIDs)
const GFINIT_CHECKBOX	:= 0x601;	// Check-boxes (KeyIDs)
const GFINIT_RADIO	:= 0x801;	// Radio Buttons (KeyIDs)
/////////////////////////////////////////////////////////////////

// 'gumpid' Constants for the GFResizePic() function
/////////////////////////////////////////////////////////////////
const BKGND_SILVERTRIM          := 83;    // Fancy, dark-grey backing
const BKGND_SCROLL              := 2520;  // UO journal scroll
const BKGND_OLD_JOURNAL_SCROLL  := 5150;  // Old journal scroll
const BKGND_GOLDTRIM            := 2620;  // Gold-press, black backing
const BKGND_PAPER               := 3000;  // Very thin edging
const BKGND_PAPER_EX            := 3500;  //  \-> with fancy inner border
const BKGND_BLACKSLAB           := 3600;  // Raised edge (blocky)
const BKGND_STONESLAB           := 5054;  // Plain, small corners
const BKGND_STONE               := 5100;  // Marble stone, rounded edge
const BKGND_DARK_STONE          := 5120;  // Same as previous, but much darker
/////////////////////////////////////////////////////////////////

// function List
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
// GFTilePic( x, y, tileid )
// GFTextLine( x, y, colour, text );
// GFTextMid(x, y, w, colour, text );
// GFTextRight( rx, y, colour, text );
// GFTextCrop( x, y, w, h, colour, text );
// GFTextEntry( x, y, w, h, colour, text := "" )
// GFButtonPage( x, y, release, press, page )
// GFButtonID( x, y, release, press, close, id := 0 )
// GFCheckBox( x, y, off, on, state, id := 0 )
// GFRadioButton( x, y, off, on, state, id := 0 )
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
			//	text-entry initialisation strings
			// Indexes/KeyIDs for...
var GFdtidx;		// ...text in GFdata (index)
var GFtxtid;		// ...TeXT-entry returns (keyid)
var GFbutid;		// ...'action' BUTtons (keyid)
var GFcbxid;		// ...CheckBoXes (keyid)
var GFradid;		// ...RADio buttons (keyid)
/////////////////////////////////////////////////////////////////

// return-value structure layout
/////////////////////////////////////////////////////////////////
//	rvsl.lytidx;	// ...index into layout array
//	rvsl.dtidx;	// ...index into data array
//	rvsl.keyid;	// ...KeyID allocated / other data (eg. page#)
/////////////////////////////////////////////////////////////////

// Global Initialisation (Just in case! :-P )
/////////////////////////////////////////////////////////////////
//
XGFReset();

// function Definitions
/////////////////////////////////////////////////////////////////

// Sets the top-left co-ordinate origin (absolute)
//
// returns: A structure {x,y} containing the old origin
//
function GFSetRef( x := 0, y := 0 )
	var oldref := GFref;
	GFref.x := CInt(x);
	GFref.y := CInt(y);
	return oldref;
endfunction

// Sets the top-left co-ordinate origin (relative to current)
//
// returns: A structure {x,y} containing the old origin
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= Page number
//
function GFPage( page )
	page := CInt(page);
	GFlayout.append( "page " + CStr(page) );
	return XGFRetVal( GFlayout.size(), -1, page );
endfunction

// Won't let the gump be closed with a right-click
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFNoClose()
	GFlayout.append( "noclose" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Won't let the gump be closed by clicking ESCAPE
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFNoDispose()
	GFlayout.append( "nodispose" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Disallows dragging of the gump.
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFNoMove()
	GFlayout.append( "nomove" );
	return XGFRetVal( GFlayout.size() );
endfunction

// Adds a generic line to the GFlayout array
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFUser1( l_line )
	GFlayout.append( l_line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Adds a generic line to the GFlayout array and
// a line to the GFdata array at position 'd_pos'
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.dtidx	= Data-array index changed
//
function GFUser2( l_line, d_pos, d_line )
	GFlayout.append( l_line );
	GFdata[CInt(d_pos)] := d_line;
	return XGFRetVal( GFlayout.size(), d_pos );
endfunction

// Single "art gump"
// (Uses "Gump" art from InsideUO)
//
// returns: A structure...
//	.lytidx == Layout-array index changed
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFResizePic( x, y, gumpid, w, h )
	var line := "resizepic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(gumpid)) + " " + CStr(CInt(w)) + " " + CStr(CInt(h));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Single "tile" graphic
// (Uses "Static Tiles" from InsideUO [under Artwork])
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//
function GFTilePic( x, y, tileid )
	var line := "tilepic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(tileid));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
endfunction

// Display a line of text on the gump
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.dtidx	= Data-array index changed (0-based index)
//
function GFTextLine( x, y, colour, text );
	var line := "text " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(colour)) + " " + CStr(GFdtidx);
	GFlayout.append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.size(), GFdtidx - 1 );
endfunction

// Display a centred line of text on the gump
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.dtidx	= Data-array index changed (0-based index)
//
function GFTextMid(x, y, w, colour, text )
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.dtidx	= Data-array index changed (0-based index)
//
function GFTextRight( rx, y, colour, text )
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
// Syntax :  x     y     w    h  colour dtidx
// Example: 125   300   150   50   32   0
//
// returns: A structure...
//  .lytidx == Layout-array index changed
//  .dtidx  == Data-array index changed (0-based index)
//
function GFTextCrop( x, y, w, h, colour, text )
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
// Syntax : x	 y  w   h  colour  txtid  dtidx
// Example: 125 300 150 50   32    1050    0
//
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.dtidx	= Data-array index changed (0-based index)
//	.keyid	= KeyID allocated to the text when the Gump returns
//	  (This method is DAFT! But hey, I didn't write it! :-P" )
//	  (returns eg:	"Key=2024, Value=2024: This is the text" )
//
function GFTextEntry( x, y, w, h, colour, text := "" )
	if (text == error)
		text := "";
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= Page number that the button changes to
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= KeyID allocated to the button - returned from Gump if pressed
//	 (butid)
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= KeyID allocated to the checkbox (cbxid)
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= KeyID allocated to the radio-button (radid)
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
// returns: A structure...
//	.lytidx == Layout-array index changed
//	.keyid	= The group number (integer form)
//
function GFSetRadioGroup( group )
	group := CInt(group);
	GFlayout.append( "group " + CStr(group) );
	return XGFRetVal( GFlayout.size(), -1, group );
endfunction

// Extracts string from a TextEntry key
//
// returns: The text string
//
function GFExtractData( keyid, keyarray )
	var keystr := CStr(keyid)+": ";
	var datstr := keyarray[CInt(keyid)];
	var retval := datstr[len(keystr)+1,len(datstr)];
	sleepms(2);
	return retval;
endfunction

// returns: Value of the next index to be used
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
// returns: The old index value
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
// returns: The new index value
//
function GFIncIndex( type )
	var oldidx := GFSetNextIndex( type, GFGetNextIndex( type ) + 1 );
	return (oldidx + 1); // Because it's just been incremented! :P"
endfunction

// Gets text from GFdata[]
//
// returns: the text
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
// returns: Index changed
//
function GFSetData( index, text )
	// Internally, the gump uses 0-based indexing...
	// Take that into account when altering the 1-based array
	GFdata[index+1] := text;
	return index;
endfunction

// Get a text item from the layout array
//
// returns: the text
//
function GFGetLayout( index )
	return GFlayout[index];
endfunction

// Sets a specified text item in the layout array
//
// **WARNING** NO ERROR CHECKING!
//
// returns: Index changed
//
function GFSetLayout( index, text )
	GFlayout[index] := text;
	return index;
endfunction

// Stores a structure containing the gump and extra details
//
// returns: A structure:
//	.layout == A copy of GFlayout[]
//	.data	= A copy of GFdata[]
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
// returns:
//	0 == Failure
//	1 == Success
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
// returns: an array of keys and values
// (ie. what the SendDialogGump() returns)
//
// NOTE: This WILL crash the client if you've created duff arrays!!
//
function GFSendGump( who )
	return SendDialogGump( who, GFlayout, GFdata );
endfunction

// Call this to display a gump using "gumpvar"
//
// returns: an array of keys and values
// (ie. what the SendDialogGump() returns)
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
//	rvsl.lytidx;	// ...index into layout array
//	rvsl.dtidx;	// ...index into data array
//	rvsl.keyid;	// ...KeyID allocated / other data (eg. page#)
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
//	Author:		Shinigami
//	eMail:		 Shinigami@gmx.net
//	Modified:	Myrathi
/////////////////////////////////////////////////////////////////

// Constants
/////////////////////////////////////////////////////////////////
var aGFTlength := struct;
aGFTlength.+charsf:=  // Space as Filler is not allowed
"||||||||||||||"+ //  2
"`',.;:::::::::"+ //  3
"!!!!!!!!!!!!!!"+ //  4
"il1[]]]]]]]]]\""+ // 5
"bcejotî(){}==="+ //  6
"adfpqrszIÑ~~~~"+ //  7
"ghnuvyÅ <>----"+ //  8
"xJ578$?+++++++"+ //  9
"kCE2369^/*****"+ // 10
"mwABDFOPéô40%%"+ // 11
"GLNQSZ@#######"+ // 12
"HRT&__________"+ // 13
"KöUYYYYYYYYYYY"+ // 14
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

// returns the Width of GumpText
//
function XGFTextWidth(gumptext)
	var points:=0;
	var j;

	for ( j:=len(gumptext); j>0; j:=j-1 )
		var i:=Find(aGFTlength.charsf, gumptext[j], 1)-1;
		if ( i>=0 )
			points:=points+aGFTlength.width[CInt(i/14)+1];
		else
			print("XGFTextWidth: Unknown Character == '"+gumptext[j]+"'");
		endif
	endfor

	return (points);
endfunction

// returns the X-Value (Left-Alignment)
//
function XGFTextLeft(gumptext, x, width)
	return (x);
endfunction

// returns the X-Value (Mid-Alignment)
//
function XGFTextMid(gumptext, x, width)
	return (x+CInt((width-XGFTextWidth(gumptext))/2));
endfunction

// returns the X-Value (Right-Alignment)
//
function XGFTextRight(gumptext, rightx)
	return (rightx-XGFTextWidth(gumptext));
endfunction


use os;
use uo;




///////////////////
//  Sends the given string to the character as an HTML gump.  The gump is scrollable
// and supports some basic HTML commands such as <b>...</b> and <i>...</i>
///////////////////

function SendStringAsHtmlWindow (who, text, title := "")
	var data := array {
		"page 0",    
		"nodispose", 
		"resizepic 40 40 5054 500 400",
		"tilepic 56 52 5357",
		"tilepic 65 49 4032",
		"text 109 50 62 0",
		"htmlgump 52 79 476 348 1 1 1"
	};
	var lines := array {
		title,
		text
	};
  
	return SendDialogGump (who, data, lines);
endfunction


///////////////////
//  similar to above, but the gump closes after a set time (in seconds)
///////////////////

function TimedYesNo (player, prompt, time)
	var layout := array {
		"page 0",
		"gumppic 0 0 2070",
		"button 30 75 2130 2129 1 0 1",
		"button 90 75 2119 2120 1 0 0",
		"text 40 30 1355 0"
	};

	var data := array {
		prompt
	};
	
	var parms := Array {
		player,
		GetPid(),
		ReadGameClock() + time
	};
	start_script (":modifiers:popup_gump_timer", parms);

	var res := SendDialogGump (player, layout, data);

	if (res)
		return 1;
	else
		return 0;
	endif
endfunction




///////////////////
//  This function displays possible options with a button beside each one.
//  returns the index of the button pressed
///////////////////

function DisplayMenuSelectionGump (character, title, selections)
	var selections_count := selections.size();
	if (!selections_count)
		return 0;
	elseif (selections_count == 1)
		return 1;
	endif
	
	var selection_columns := 1;
	if (selections_count > 45)
		SendSysMessage (character, "Error:  Too many options to display.");
		return 0;
	elseif (selections_count > 20)
		selection_columns := 3;
	elseif (selections_count > 10)
		selection_columns := 2;
	endif

	var selection_rows := CINT (selections_count/selection_columns) + 1;
	var menu_height := (selection_rows * 25) + 120;
	var menu_width := ((selection_columns-1) * 200) + 300;

	var MenuSelection_gump_layout := array {
		"page 0",
		"nodispose",
		"resizepic 50 50 2620 " + menu_width + " " + menu_height,
		"text 70 59 40 0",
		"page 1"
	};
	
	var MenuSelection_gump_data := array {
		title
	};

	var current_y := 100;
	var current_x := 80;
	for i := 1 to selections_count
		MenuSelection_gump_layout.append ("button " + current_x + " " + (current_y+5) + " 2118 2117 1 1 " + i);
		MenuSelection_gump_layout.append ("text " + (current_x+20) + " " + current_y + " 40 " + i);
		MenuSelection_gump_data.append (selections[i]);
		current_y := current_y + 25;
		if (i == selection_rows)
			current_y := 100;
			current_x := 280;
		elseif (i == (selection_rows*2))
			current_y := 100;
			current_x := 480;
		endif
	endfor

	var gump_return := SendDialogGump (character, MenuSelection_gump_layout, MenuSelection_gump_data);
	if (!gump_return or !gump_return[0])
		return 0;
	endif
	return gump_return[0];
endfunction





