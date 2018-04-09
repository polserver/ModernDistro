//
// **** THIS IS THE OLD / ORIGINAL GUMPS.INC ****
// To use this in scripts use:
// include :gumps:includes/old/old-gumps.inc
//
// You should begin migrating to the new gumps include though!
//
/////////////////////////////////////////////////////////////////
//
// GUMPS.INC - v2.2
//
// Gump Functions - to make life easier! :) 
//	-Myr
// Tastes good with ketchup too.
//	-Austin
//
// Author: Myrathi <tjh@myrathi.co.uk>
//
//==Revision v2.6
// - Added GFHTMLArea()
//==Revision v2.5
// - Added word wrap function
//==Revision v2.4b
// Updated: 28-May-2002, Austin
//  - Fixed the scroll CONSTant.
//  - Added an extra scroll CONSTant.
//==Revision v2.4
// - Fixed character width CONSTants.
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
//  - Added BKGND_???????? 'gumpid' CONSTants for GFResizePic()
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
//	- Added a SleepMS() in places to stop runaway scripts! Yay!
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
//	- Added CONSTants for the index functions
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
//	+"Page 0" is the main background! It shows at ALL times!
//	  Anything you want on separate pages should be on page
//	  1, 2, 3 and so on showing 0+1, 0+2, 0+3, 0+4, etc, etc.
//
// To do:
//	+ Add the option of supplying the KeyIDs (is this wanted?)
//	+ Update the CONST-list of commonly used ID's (GFX + colours)
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

// CONSTants (for GF[Get|Set]NextIndex)
/////////////////////////////////////////////////////////////////
CONST GFIDX_TEXT		:= 0x01;	// Static Text (TextLine)
CONST GFIDX_TEXTENTRY		:= 0x02;	// Return Text (TextEntry)
CONST GFIDX_BUTTON		:= 0x03;	// Action Buttons (ButtonID)
CONST GFIDX_CHECKBOX		:= 0x04;	// Check-boxes
CONST GFIDX_RADIO		:= 0x05;	// Radio Buttons
/////////////////////////////////////////////////////////////////

// CONSTants for index starting values (SHOULD be enough! Heh!)
/////////////////////////////////////////////////////////////////
CONST GFINIT_TEXT		:= 0x000;	// Static Text (GFdata[])
CONST GFINIT_TEXTENTRY		:= 0x201;	// Return Text (GFdata[])
CONST GFINIT_BUTTON		:= 0x401;	// Action Buttons (KeyIDs)
CONST GFINIT_CHECKBOX		:= 0x601;	// Check-boxes (KeyIDs)
CONST GFINIT_RADIO		:= 0x801;	// Radio Buttons (KeyIDs)
/////////////////////////////////////////////////////////////////

// 'gumpid' CONSTants for the GFResizePic() function
/////////////////////////////////////////////////////////////////
CONST BKGND_SILVERTRIM		      := 83;    // Fancy, dark-grey backing
CONST BKGND_SCROLL		          := 2520;  // UO journal scroll
CONST BKGND_OLD_JOURNAL_SCROLL	:= 5150;  // Old journal scroll
CONST BKGND_GOLDTRIM		        := 2620;  // Gold-press, black backing
CONST BKGND_PAPER               := 3000;  // Very thin edging
CONST BKGND_PAPER_EX		        := 3500;  //  \_with fancy inner border
CONST BKGND_BLUESLAB		        := 3600;  // Raised edge (blocky)
CONST BKGND_SANDSTONE		        := 5054;  // Custom sandstone background
CONST BKGND_STONESLAB		        := 5064;  // Gray stone slab background
CONST BKGND_STONE		            := 5100;  // Marble stone, rounded edge
CONST BKGND_DARK_STONE		      := 5120;  // Same as previous, but much darker
/////////////////////////////////////////////////////////////////
CONST GF_MAIN_BKGND		:= BKGND_BLUESLAB;
CONST GF_MAIN_FRGND		:= BKGND_STONESLAB;
////////////////////////////////////////////////////////////////

CONST CHKBOX_GRAY_A		:= 5078;
CONST CHKBOX_GRAY_B		:= 5079;
CONST CHKBOX_SAND_A		:= 210;
CONST CHKBOX_SAND_B		:= 211;

//Standard text colors on TSSE
/////////////////////////////////////////////////////////////////
CONST GF_STDCOLOR		:= 1890;
CONST GF_HDRCOLOR		:= 1890;
CONST GF_SLCTDCOLOR		:= 1720;

/////////////////////////////////////////////////////////////////
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
function OldGFSetRef( x := 0, y := 0 )
	var oldref := GFref;
	GFref.x := CInt(x);
	GFref.y := CInt(y);
	return oldref;
endfunction

// Sets the top-left co-ordinate origin (relative to current)
//
// Returns: A structure {x,y} containing the old origin
//
function OldGFSetRefRel( dx, dy )
	var oldref := GFref;
	GFref.x := GFref.x + CInt(dx);
	GFref.y := GFref.y + CInt(dy);
	return oldref;
endfunction

// Initialises this include file's global variables
//
function OldGFInitGump( sx := 0, sy := 0 )
	XGFReset();
	OldGFSetRef( sx, sy );
	OldGFPage( 0 );
	OldGFNoDispose();
endfunction

// Gump page #x - start of definition
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = Page number
//
function OldGFPage( page )
	page := CInt(page);
	GFlayout.Append( "page "+CStr(page) );
	return XGFRetVal( GFlayout.Size(), -1, page );
endfunction

// Won't let the gump be closed with a right-click
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFNoClose()
	GFlayout.Append( "noclose" );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Won't let the gump be closed by clicking ESCAPE
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFNoDispose()
	GFlayout.Append( "nodispose" );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Disallows dragging of the gump.
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFNoMove()
	GFlayout.Append( "nomove" );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Adds a generic line to the GFlayout array
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFUser1( l_line )
	GFlayout.Append( l_line );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Adds a generic line to the GFlayout array and
// a line to the GFdata array at position 'd_pos'
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed
//
function OldGFUser2( l_line, d_pos, d_line )
	GFlayout.Append( l_line );
	GFdata[CInt(d_pos)] := d_line;
	return XGFRetVal( GFlayout.Size(), d_pos );
endfunction

// Single "art gump"
// (Uses "Gump" art from InsideUO)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFGumpPic( x, y, gumpid, hue:=0 )
	var line := "gumppic "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(gumpid));
	line := line+" hue="+CStr(CInt(hue));
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Resized "gump" made up of art from 'gumpid' to 'gumpid+8'
// (Uses "Gump" art from InsideUO)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFResizePic( x, y, gumpid, w, h )
	var line := "resizepic "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(gumpid))+" "+CStr(CInt(w))+" "+CStr(CInt(h));
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size() );
endfunction

//
// Sets up an area on the gump that can be used as an html gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function OldGFHTMLArea(x, y, width, height, background, scroll_bar, text:="")
	//x y w h l# bg sb
	//"htmlgump 52 79 276 348 1 0 1",
	var line := "htmlgump "+CStr(GFref.x+CInt(x))+" "+CStr(GFref.y+CInt(y));
	line := line+" "+CStr(CInt(width))+" "+CStr(CInt(height));
	line := line+" "+CStr(GFdtidx);
	line := line+" "+CStr(CInt(background))+" "+CStr(CInt(scroll_bar));
	GFlayout.Append(line);
	
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1 );
endfunction

// Single "tile" graphic
// (Uses "Static Tiles" from InsideUO [under Artwork])
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function OldGFTilePic( x, y, tileid )
	var line := "tilepic "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(tileid));
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size() );
endfunction

// Display a line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function OldGFTextLine(x, y, colour, text)
	text := CStr(text);
	var line := "text "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(colour))+" "+CStr(GFdtidx);
	GFlayout.Append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1 );
endfunction

// Display a centred line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function OldGFTextMid(x, y, w, colour, text )
	text := CStr(text);
	x := XGFTextMid( text, CInt(x), w );
	var line := "text "+CStr(GFref.x + x)+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(colour))+" "+CStr(GFdtidx);
	GFlayout.Append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1 );
endfunction

// Display a right-aligned line of text on the gump
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.dtidx  = Data-array index changed (0-based index)
//
function OldGFTextRight( rx, y, colour, text )
	text := CStr(text);
	rx := XGFTextRight( text, CInt(rx) );
	var line := "text "+CStr(GFref.x + rx)+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(colour))+" "+CStr(GFdtidx);
	GFlayout.Append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1 );
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
function OldGFTextCrop( x, y, w, h, colour, text )
	text := CStr(text);
	var line := "croppedtext "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(w))+" "+CStr(CInt(h))+" "+CStr(CInt(colour));
	line := line+" "+CStr(GFdtidx);
	GFlayout.Append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1 );
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
function OldGFTextEntry( x, y, w, h, colour, text := "" )
	if ( text == error )
		text := "";
	else
		text := CStr(text);
	endif
	
	var line := "textentry "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(w))+" "+CStr(CInt(h))+" "+CStr(CInt(colour));
	line := line+" "+CStr(GFtxtid)+" "+CStr(GFdtidx);
	GFlayout.Append( line );
	GFdtidx := GFdtidx + 1;		// Update index into GFdata[]
	GFdata[GFdtidx] := text;	// arrays start at #1... id#0 goes into [1]!!
	GFtxtid := GFtxtid + 1;		// Update KeyID (txtid)
	return XGFRetVal( GFlayout.Size(), GFdtidx - 1, GFtxtid - 1 );
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
function OldGFButtonPage( x, y, release, press, page )
	// "button x y release press 0 page"
	var line := "button "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(release))+" "+CStr(CInt(press));
	line := line+" 0 "+CStr(CInt(page));
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size(), -1, page );
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
function OldGFButtonID( x, y, release, press, close, id := 0 )
	// "button x y release press close 0 key"
	var line := "button "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(release))+" "+CStr(CInt(press))+" ";
	if ( close == 0 )
		line := line+"0 0 ";
	else
		line := line+"1 0 ";
	endif
	id := CInt(id);		// Ensure it's an integer
	if ( id < 1 )		// Catch errors (0) and invalid (negative) values
		id := GFbutid;
		GFbutid := GFbutid + 1;		// Update KeyID (butid) (only if used)
	endif
	line := line + CStr(id);
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size(), -1, id );
endfunction

// Displays a check-box (off/on)
//
// Syntax: x, y, off, on, initial state (0/1), cbxid
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = KeyID allocated to the checkbox (cbxid)
//
function OldGFCheckBox( x, y, off, on, state, id := 0)
	var line := "checkbox "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(off))+" "+CStr(CInt(on))+" ";
	if ( state == 0 )
		line := line+"0 ";
	else
		line := line+"1 ";
	endif
	id := Cint(id);
	if (id < 1)
		id := GFcbxid;
		GFcbxid := GFcbxid + 1;	
	endif
	line := line + CStr(id);

	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size(), -1, id );
endfunction

// Displays a radio button (off/on)
//
// Syntax: x, y, off, on, initial state (0/1), radid
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = KeyID allocated to the radio-button (radid)
//
function OldGFRadioButton( x, y, off, on, state, id := 0)
	if (state == error)
		state := 0;
	endif
	var line := "radio "+CStr(GFref.x + CInt(x))+" "+CStr(GFref.y + CInt(y));
	line := line+" "+CStr(CInt(off))+" "+CStr(CInt(on))+" ";
	if ( state == 0 )
		line := line+"0 ";
	else
		line := line+"1 ";
	endif
	id := CInt(id);
	if (id < 1)
		id := GFradid;
		GFradid := GFradid + 1;
	endif
	line := line + CStr(id);
	
	GFlayout.Append( line );
	return XGFRetVal( GFlayout.Size(), -1, id );
endfunction

// Groups radio button (I think)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//	.keyid  = The group number (integer form)
//
function OldGFSetRadioGroup( group )
	group := CInt(group);
	GFlayout.Append( "group "+CStr(group) );
	return XGFRetVal( GFlayout.Size(), -1, group );
endfunction

// Extracts string from a TextEntry key
//
// Returns: The text string
//
function OldGFExtractData( keyid, keyarray )
	var keystr := CStr(keyid)+": ";
	var datstr := keyarray[CInt(keyid)];
	var retval := datstr[Len(keystr)+1,Len(datstr)];
	SleepMS(2);
	return retval;
endfunction

// Returns: Value of the next index to be used
//
function OldGFGetNextIndex( type )
	SleepMS(2);
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
	return 0;
endfunction

// Sets the next index to be used
//
// Returns: The old index value
//
function OldGFSetNextIndex( type, value )
	var oldidx;
	value := CInt(value);
	SleepMS(2);
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
			return 0;
	endcase
	return oldidx;
endfunction

// Increments the current <type> index by one
//
// Returns: The new index value
//
function OldGFIncIndex( type )
	var oldidx := OldGFSetNextIndex( type, OldGFGetNextIndex( type ) + 1 );
	return (oldidx + 1); // Because it's just been incremented! :P"
endfunction

// Gets text from GFdata[]
//
// Returns: the text
//
function OldGFGetData( index )
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
function OldGFSetData( index, text )
	// Internally, the gump uses 0-based indexing...
	// Take that into account when altering the 1-based array
	GFdata[index+1] := text;
	return index;
endfunction

// Get a text item from the layout array
//
// Returns: the text
//
function OldGFGetLayout( index )
	return GFlayout[index];
endfunction

// Sets a specified text item in the layout array
//
// **WARNING** NO ERROR CHECKING!
//
// Returns: Index changed
//
function OldGFSetLayout( index, text )
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
function OldGFStoreGump()
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
function OldGFRestoreGump( gumpvar )
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
function OldGFSendGump( who )
	return SendDialogGump( who, GFlayout, GFdata );
endfunction

// Call this to display a gump using "gumpvar"
//
// Returns: an array of keys and values
//		(ie. what the SendDialogGump() returns)
//
// NOTE: This WILL crash the client if you've corrupted the array!
//
function OldGFSendGumpV( who, gumpvar )
	if ( !(gumpvar.layout) or !(gumpvar.data) )
		return 0;
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
	SleepMS(2);	// Will hopefully stop runaway script problem :/

	return rvsl;
endfunction

/////////////////////////////////////////////////////////////////
//  Author:    Shinigami
//  eMail:     Shinigami@gmx.net
//  Modified:  Myrathi
/////////////////////////////////////////////////////////////////

// CONSTants
/////////////////////////////////////////////////////////////////
var aGFTlength := struct;
       aGFTlength.+charsf:=   // Space as Filler is not allowed
         "||||||||||||||"+//  2
         "`',.;:::::::::"+//  3
         "!!!!!!!!!!!!!!"+//  4
         "il1[]]]]]]]]]\""+//  5
         "bcejotî(){}==="+//  6
         "adfpqrszIÑ~~~~"+//  7
         "ghnuvyÅ <>----"+//  8
         "xJ578$?+++++++"+//  9
         "kCE2369^/**** "+// 10
         "mwABDFOPéô40%%"+// 11
         "GLNQSZ@#######"+// 12
         "HRT&__________"+// 13
         "KöUYYYYYYYYYYY"+// 14
         "XXXXXXXXXXXXXX"+// 15
         "VVVVVVVVVVVVVV"+// 16
         "MMMMMMMMMMMMMM"+// 18
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

  For (j:=Len(gumptext); j>0; j:=j-1)
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

//
// Send it a string of text and the size in pixels.
// It will return an array. Each array elem is a wrapped line.
//

function WordWrap(text, max_letters := 50)
	CONST WSS := 10; //White Space Size " "
	
	var sentences := {};
	var cur_line := "";
	var line_length := 0;
	var word_list := SplitWords(text);
	
	foreach word in word_list
		if ( Len(word) > max_letters )
			word := "";
		endif
		
		while(cur_line["  "]) // Remove double spaces
			cur_line["  "] := " ";
			SleepMS(2);
		endwhile
		
		while (cur_line[1] == " ") // Remove leading spaces
			cur_line[1] := "";
			SleepMS(2);
		endwhile
		//[P] is a line break
		if (word["[P]"] || (line_length + XGFTextWidth(word)) >= max_letters)
			word["[P]"] := "";
			sentences.Append(cur_line);
			line_length := 0;
			cur_line := "";
		endif
		
		cur_line := cur_line+" "+word;
		line_length := line_length + (XGFTextWidth(word) + WSS);
		SleepMS(2);
	endforeach
	
	if ( sentences[Len(sentences)] != cur_line )
		while(cur_line["  "]) // Remove double spaces
			cur_line["  "] := " ";
			SleepMS(2);
		endwhile
				
		while (cur_line[1] == " ") // Remove leading spaces
			cur_line[1] := "";
			SleepMS(2);
		endwhile
			
		sentences.Append(cur_line);
	endif
	
	return sentences;
endfunction

