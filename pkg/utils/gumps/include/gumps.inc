/* $Id: gumps.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 * Purpose
 * TODO
 *
 * Function List
 * GFAddButton(byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0)
 * GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background, scrollbar, hue := 0)
 * GFCheckBox(byref gump, x, y, unc_id, che_id, status:=0, btn_value:=0)
 * GFCreateGump(x:=0, y:=0, width:=0, height:=0)
 * GFDisposable(byref gump, bool)
 * GFExtractData(input, data_id)
 * GFGumpPic(byref gump, x, y, gump_id, hue := 0)
 * GFHTMLArea(byref gump, x, y, width, height, text, background:=0, scrollbar:=0)
 * GFMovable(byref gump, bool)
 * GFPage(byref gump, page_num)
 * GFRadioButton(byref gump, x, y, unp_id, pres_id, status:=0, btn_value:=0)
 * GFResizePic(byref gump, x, y, gump_id, width:=0, height:=0)
 * GFSendGump(who, byref gump)
 * GFSetRadioGroup(byref gump, group_id)
 * GFTextCrop(byref gump, x, y, width, height, hue:=0, text:="")
 * GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text:="")
 * GFTextLine(byref gump, x, y, hue:=0, text:="")
 * GFTilePic(byref gump, x, y, tile_id, hue := 0)
 * GFTooltip(byref gump, cliloc_id)
 *
 *
 *
 * Private Function List
 * XGFError(text)
 * XGFAddToLayout(byref gump, string)
 *
 */
use uo;
use os;
use basic;

// CONSTants for the GFAddButton() function
CONST GF_PAGE_BTN	:= 0x0;
CONST GF_CLOSE_BTN	:= 0x1;
CONST GF_DUMMY_BTN	:= 0x2;


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
 * GFDisposable(byref gump, bool)
 *
 * Purpose
 * Sets the gump disposable 1/0.
 *
 * Parameters
 * gump:	Reference to the gump
 * bool:	0: the gump is not disposable
 *		1: the gump is disposable
 *
 * Notes:
 * You will almost always want to call this function every time
 * you make a gump and set it NoDispose which is GFDisposable(gump, 0)
 *
 * Return value
 * Returns 1 on success, error on failure.
 *
 */
function GFDisposable(byref gump, bool)
	case(bool)
		0:
			if ( gump.base.DisposeLoc )
				return XGFError("Gump already contains the 'NoDispose' flag.");
			endif

			(gump.layout).Append("NoDispose");
			gump.base.DisposeLoc := (gump.layout).Size();
			break;
		1:
			if ( !gump.base.CloseLoc )
				return XGFError("Gump is already disposable.");
			endif

			(gump.layout).Erase(gump.base.DisposeLoc);
			gump.base.DisposeLoc := 0;
			break;
		default:
			return XGFError("gumps::Disposable: Invalid bool value '"+bool+"'");
	endcase
	return 1;
 endfunction


/*
 * GFMovable(byref gump, bool)
 *
 * Purpose
 * Sets the gump movable 1/0.
 *
 * Parameters
 * gump:	Reference to the gump
 * bool:	0: the gump is not movable
 *		1: the gump is movable
 *
 * Return value
 * Returns 1 on success, error on failure.
 *
 */
function GFMovable(byref gump, bool)
		case(bool)
		0:
			if ( gump.base.MoveLoc )
				return XGFError("Gump already contains the 'NoMove' flag.");
			endif

			(gump.layout).Append("NoMove");
			gump.base.MoveLoc := (gump.layout).Size();
			break;
		1:
			if ( !gump.base.MoveLoc )
				return XGFError("Gump is already movable.");
			endif

			(gump.layout).Erase(gump.base.MoveLoc);
			gump.base.MoveLoc := 0;
			break;
		default:
			return XGFError("gumps::Movable: Invalid bool value '"+bool+"'");
	endcase
	return 1;
 endfunction


/*
 * GFPage(byref gump, page_num)
 *
 * Purpose
 * Adds a page to the gump.
 *
 * Parameters
 * gump:	Reference to the gump
 * page_num:	The integer representing the page's number
 *		  (0 is the background, always visible)
 *
 * Return value
 * No return value.
 *
 */
function GFPage(byref gump, page_num)
	gump.cur_page := CInt(page_num);
	
	(gump.layout).Append("Page "+CStr(gump.cur_page));
	
	return 1;
endfunction


/*
 * GFResizePic(byref gump, x, y, gump_id, width, height)
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
function GFResizePic(byref gump, x, y, gump_id, width, height)
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
 * GFAddAlphaRegion(byref gump, x, y, width, height)
 *
 * Purpose
 * Adds an alpha (transparent) region to the gump.
 *
 * Parameters
 * gump:	Reference to the gump.
 * x:	   The top-left spot of the X axis.
 * y:	   The top-left spot of the Y axis.
 * width:   The region's total width.
 * height:  The region's total height.
 *
 * Return value
 * No return value.
 *
 */
 function GFAddAlphaRegion(byref gump, x, y, width, height)
	(gump.layout).Append("CheckerTrans "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(width))+" "+CStr(CInt(height)));
	return 1;
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
 * GFCheckBox(byref gump, x, y, unc_id, che_id, status, btn_value)
 *
 * Purpose
 * Adds a check-box to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * unc_id:	The unchecked-id of the check-box.
 * che_id:	The checked-id of the check-box.
 * status:	The radio's initial state (pressed/unpressed).
 * btn_value:	The value the check box will return if checked.
 *
 * ----------------------
 * Notes: Check boxes can be used in a script through two ways:
 * var input := GFSendGump(gump);
 *
 * //Using input.keys
 * foreach key in (input.keys)
 * 	//Do stuff here
 * endforeach
 *
 * //Using arbitrary values
 * input[btn_value];
 * ----------------------
 *
 * Return value
 * Return value is the check box btn_value.
 *
 */
function GFCheckBox(byref gump, x, y, unc_id, che_id, status:=0, btn_value:=0)

	btn_value := CInt(btn_value);
	if ( btn_value < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id := gump.data_id+1;
		btn_value := gump.data_id;
	endif

	(gump.layout).Append("CheckBox "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(unc_id))+" "+CStr(CInt(che_id))+" "
				+ CStr(CInt(status))+" "+CStr(btn_value));

	return (btn_value);
endfunction


/*
 * GFSetRadioGroup(byref gump, group_id)
 *
 * Purpose
 * Adds a group-radio to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * group_id:	The number representing the group (0-9)
 *
 * Notes:
 * Only seems to work on page 1.
 *
 * Return value
 * No return value.
 *
 */
function GFSetRadioGroup(byref gump, group_id)
	(gump.layout).Append("Group "+CStr(CInt(group_id)));
	return 1;
endfunction


/*
 * GFHTMLArea(byref gump, x, y, width, height, text, background, scrollbar)
 *
 * Purpose
 * adds an HTML-element to the gump
 *
 * Parameters
 * gump:	Reference to the gump.
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * width:	The HTML's total width.
 * height:	The HTML's total height.
 * text:	The string that is to be exposed.
 * background:	The HTML-element's background.
 * scrollbar:	boolean value:
 *		0 The HTML-element does not have a scrollbar
 *		1 The HTML-element has a scrollbar
 *
 * Return value
 * Returns where in the data array the text was placed.
 *
 */
function GFHTMLArea(byref gump, x, y, width, height, text, background:=0, scrollbar:=0)
	gump.data.Append(""+CStr(text));
	var index := gump.data.Size()-1;

	(gump.layout).Append("HTMLGump "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(index)+" "
				+ CStr(CInt(background))+" "+CStr(CInt(scrollbar)));
	return (index);
endfunction


/*
 * GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background, scrollbar, hue)
 *
 * Purpose
 * adds an HTML-element to the gump
 *
 * Parameters
 * gump:	Reference to the gump.
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * width:	The HTML's total width.
 * height:	The HTML's total height.
 * cliloc:	The cliloc-id that will call the appointed text.
 * background:	The HTML-element's background.
 * scrollbar:	boolean value,
 *		0 The HTML-element does not have a scrollbar
 *		1 The HTML-element has a scrollbar
 *
 * Return value
 * No return value.
 *
 */
function GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background:=0, scrollbar:=0, hue:=0)
	if(hue == 0)
		(gump.layout).Append("XMFHTMLGump "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
					+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(cliloc)+" "
					+ CStr(CInt(background))+" "+CStr(CInt(scrollbar)));
	else
		(gump.layout).Append("XMFHTMLGumpColor "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
					+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(CInt(cliloc))+" "
					+ CStr(CInt(background))+" "+CStr(CInt(scrollbar))+" "+CStr(CInt(hue)));
	endif
	return 1;
endfunction


/*
 * GFGumpPic(byref gump, x, y, gump_id, hue)
 *
 * Purpose
 * Adds a gump-image to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * gump_id:	The gump-id (graphic #).
 * hue:		The decimal number representing the gump-id's hue.
 *
 * Notes:
 * The hue parameter is only supported by 3.5x or newer clients.
 *
 * Return value
 * No return value.
 *
 */
function GFGumpPic(byref gump, x, y, gump_id, hue := 0)
	var text := "GumpPic "+CStr(CInt(x))+" "+CStr(CInt(y))+" "+CStr(CInt(gump_id));

	hue := CInt(hue);
	if ( hue )
		text := text + " hue="+CStr(hue);
	endif
	(gump.layout).Append(text);
	return 1;
endfunction


/*
 * GFTilePic(byref gump, x, y, tile_id, hue)
 *
 * Purpose
 * Adds a gump-image to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * tile_id:	The artwork tile number.
 * hue:		The decimal number representing the item-id's hue (optional)
 *
 * Notes:
 * The hue parameter is only supported in 3.5x clients or newer.
 *
 * Return value
 * No return value.
 *
 */
function GFTilePic(byref gump, x, y, tile_id, hue := 0)
	var text := "TilePic "+CStr(CInt(x))+" "+CStr(CInt(y))+" "+CStr(CInt(tile_id));

	hue := CInt(hue);
	if ( hue )
		text := text + " hue="+CStr(hue);
	endif

	(gump.layout).Append(text);
	return 1;
endfunction


/*
 * GFTextLine(byref gump, x, y, hue, text)
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
function GFTextLine(byref gump, x, y, hue:=0, text:="")
	gump.data.Append(""+CStr(text));
	var index := gump.data.Size()-1;
	(gump.layout).Append("Text "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(hue))+" "+CStr(index));

	return (index);
endfunction


/*
 * GFTextCrop(byref gump, x, y, width, height, hue, text)
 *
 * Purpose
 * Adds a text cropped-label to the gump.
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * width:	The cropped-label's total width.
 * height:	The cropped-label's total height.
 * hue:		The decimal number representing the Label's hue
 * text:	The string that is to be exposed.
 *
 * Return value
 * Returns where in the data array the text was placed.
 *
 */
function GFTextCrop(byref gump, x, y, width, height, hue:=0, text:="")
	gump.data.Append(text);
	gump.data.Append(""+CStr(text));
	var index := gump.data.Size()-1;

	(gump.layout).Append("CroppedText "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(CInt(hue))+" "+CStr(index));

	return (index);
endfunction


/*
 * GFRadioButton(byref gump, x, y, unp_id, pres_id, status, btn_value)
 *
 * Purpose
 * Adds a radio to the gump
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * unp_id:	The unpressed-id of the radio.
 * pres_id:	The pressed-id of the radio.
 * status:	The radio's initial state (pressed/unpressed)
 * btn_value:	The value the radio button will return if checked.
 * ----------------------
 * Notes:
 * Radio buttons can be retrieved in a script through two ways:
 * var input := GFSendGump(gump);
 *
 * //Using input.keys
 * foreach key in (input.keys)
 * 	//Do stuff here
 * endforeach
 *
 * //Using arbitrary values
 * input[btn_value];
 * ----------------------
 *
 * Return value
 * Return value is the value of the radio button.
 *
 */
function GFRadioButton(byref gump, x, y, unp_id, pres_id, status:=0, btn_value:=0)
	btn_value := CInt(btn_value);
	if ( btn_value < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id := gump.data_id+1;
		btn_value := gump.data_id;
	endif

	(gump.layout).Append("Radio "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(unp_id))+" "+CStr(CInt(pres_id))+" "
				+ CStr(CInt(status))+" "+CStr(btn_value));
	return (btn_value);
endfunction


/*
 * GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text)
 *
 * Purpose
 * Adds a text-entry to the gump.
 *
 * Parameters
 * gump:	Reference to the gump
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * width:	Width of the text area in pixels from point X.
 * height:	Height of the text area in pixels from point Y.
 * hue:		The decimal number representing the text's hue
 * ini_text:	The initial string that is to be exposed.
 * txt_id:	The location of the text entry's input data.
 *
 * Notes:
 * Use GFExtractData() in gumps_ex.inc to easily remove the leading "#: "
 *
 * Return value
 * Text ID number where the text entry is set.
 *
 */
function GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text:="", txt_id:=0)

	gump.data.Append(""+CStr(ini_text));
	var index := gump.data.Size()-1;

	txt_id := CInt(txt_id);
	if ( txt_id < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id := gump.data_id+1;
		txt_id := gump.data_id;
	endif

	(gump.layout).Append("TextEntry "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(width))+" "+CStr(CInt(height))+" "
				+ CStr(CInt(txt_color))+" "+CStr(txt_id)+" "+CStr(index));

	return (txt_id);
endfunction


/*
 * GFSendGump(who, gump)
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
function GFSendGump(who, byref gump)
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



// Old gump stuff put in for compatibility sake.

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
const BKGND_SCROLL              := 2520;  // UO journal scroll
const BKGND_OLD_JOURNAL_SCROLL  := 5150;  // Old journal scroll
const BKGND_GOLDTRIM            := 2620;  // Gold-press, black backing
const BKGND_PAPER               := 3000;  // Very thin edging
const BKGND_PAPER_EX            := 3500;  //  \_with fancy inner border
const BKGND_BLACKSLAB           := 3600;  // Raised edge (blocky)
const BKGND_STONESLAB           := 5054;  // Plain, small corners
const BKGND_STONE               := 5100;  // Marble stone, rounded edge
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

// Initialises this include file's global variables
//
function GFInitGump( sx := 0, sy := 0 )
	XGFReset();
	GFSetRef( sx, sy );
	GFPageO( 0 );
	GFNoDispose();
endfunction

function GFPageO( page )
	page := CInt(page);
	GFlayout.append( "page " + CStr(page) );
	return XGFRetVal( GFlayout.size(), -1, page );
endfunction

// Resized "gump" made up of art from 'gumpid' to 'gumpid+8'
// (Uses "Gump" art from InsideUO)
//
// Returns: A structure...
//	.lytidx = Layout-array index changed
//
function GFResizePicO( x, y, gumpid, w, h )
	var line := "resizepic " + CStr(GFref.x + CInt(x)) + " " + CStr(GFref.y + CInt(y));
	line := line + " " + CStr(CInt(gumpid)) + " " + CStr(CInt(w)) + " " + CStr(CInt(h));
	GFlayout.append( line );
	return XGFRetVal( GFlayout.size() );
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

//////////////////////////////////
// End of old-gump stuff I think
//////////////////////////////////

// Call this to display/use the defined gump
//
// Returns: an array of keys and values
//          (ie. what the SendDialogGump() returns)
//
// NOTE: This WILL crash the client if you've created duff arrays!!
//
function GFSendGumpO( who )
	return SendDialogGump( who, GFlayout, GFdata );
endfunction

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

/*
 * GFTooltip(byref gump, cliloc_id)
 *
 * Purpose
 * Adds a tooltip to the previous layoutarray entry
 *
 * Parameters
 * gump:	Reference to the gump
 * cliloc_id:		The decimal number reference to the cliloc entry which will be used as the tooltip.
 *
 *
 * Return value
 * No return value.
 *
 */
function GFTooltip(byref gump, cliloc_id, customstring := "")
 var text := "Tooltip " + CInt(cliloc_id) + "@" + customstring + "@" + "'";
 (gump.layout).Append(text);

 return 1;
endfunction

/*
 * GFPicTiled(byref gump, x, y, width, height, gump_id)
 *
 * Purpose
 * Adds a pic tiled gump to the gump.
 *
 * Parameters
 * gump: Reference to the gump.
 * x:  The top-left spot of the X axis.
 * y:  The top-left spot of the Y axis.
 * width: The width (pixels) of the gump from position X.
 * height: The height (pixels) of how far to scale the gump downwards from position Y.
 * gump_id: The decimal number representing the gump.
 *
 * Return value
 * No return value.
 *
 */
function GFPicTiled(byref gump, x, y, width, height, gump_id)
 width := CInt(width);
 height := CInt(height);
 if ( !width )
  width := gump.base.width;
 endif
 if ( !height )
  height := gump.base.width;
 endif
  
 (gump.layout).Append("gumppictiled "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
    + CStr(width)+" "+CStr(height) +" "+ CStr(CInt(gump_id)));
 return 1;
endfunction