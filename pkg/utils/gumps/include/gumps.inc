/*
 * Purpose
 * TODO
 *
 * Function List
 * GFAddButton(byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0)
 * GFAddImageTileButton(byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0, tilepic_id, hue:=0, tile_x, tile_y )
 * GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background, scrollbar, hue := 0, customstring:="")
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
 * GFSendGump(mobile, byref gump)
 * GFSetRadioGroup(byref gump, group_id)
 * GFTextCrop(byref gump, x, y, width, height, hue:=0, text:="")
 * GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text:="", txt_id:=0, lmt := 0)
 * GFTextLine(byref gump, x, y, hue:=0, text:="")
 * GFTilePic(byref gump, x, y, tile_id, hue := 0)
 * GFDrawWrappedText(byref gump, x, y, text_lines, txt_color)
 * GFPicTiled(byref gump, x, y, width, height, gump_id)
 * GFTooltip(byref gump, cliloc_id, customstring)
 * GFItemProperty ( byref gump, item_serial )
 * GFClosable(byref gump, bool)
 * GFAddAlphaRegion(byref gump, x, y, width, height)
 *
 * Private Function List
 * XGFError(text)
 * XGFAddToLayout(byref gump, string)
 *
 */
use uo;
use os;
use basic;

include ":gumps:autoClose";
include ":gumps:gumpPrompt";
include ":gumps:gumps_ex";
include ":gumps:htmlGump";
include ":gumps:playerSelectionGump";
include ":gumps:requestGump";
include ":gumps:selectionGump";
include ":gumps:textConsts";
include ":gumps:yesNo";

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
	case ( bool )
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
	case ( bool )
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
	case ( bool )
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
 * A return value of 0 means cancelled or the client force closed it
 * or a right click to close. For this reason you cannot have a button value of zero (0).
 */
function GFAddButton(byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0)
	var line := "Button "+CStr(x)+" "+CStr(y)+" "+CStr(off_id)+" "+CStr(on_id);

	btn_value := CInt(btn_value);
	if ( btn_value < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id += 1;
		btn_value := gump.data_id;
	endif

	if ( btn_type == GF_CLOSE_BTN )
		line += " 1 0 "+CStr(btn_value);
	elseif ( btn_type == GF_DUMMY_BTN )
		line += " 0 0 "+CStr(btn_value);
	else // Assume GF_PAGE_BTN
		line += " 0 "+CStr(btn_value);
	endif

	(gump.layout).Append(line);

	return (btn_value);
endfunction

/*
 * GFAddImageTileButton ( byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0, tilepic_id, hue:=0, width, height )
 *
 * Purpose
 * Adds a button(page/reply) in the form of an art tile to the gump
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
 * tilepic_id The graphic number of the item you wish to portray as an item.
 * hue        Well fairly self explanatory I think but changes the color of the gump object.
 * width      The width active area of the button
 * height     The height active area of the button
 *
 * Return value
 * Return value is the button value.
 *
 */
function GFAddImageTileButton ( byref gump, x, y, off_id, on_id, btn_type:=GF_PAGE_BTN, btn_value:=0, tilepic_id, hue:=0, width_x, height_y )
	var line := "ButtonTileArt "+CStr(x)+" "+CStr(y)+" "+CStr(off_id)+" "+CStr(on_id);

	btn_value := CInt(btn_value);
	if ( btn_value < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id += 1;
		btn_value := gump.data_id;
	endif

	if ( btn_type == GF_CLOSE_BTN )
		line += " 1 0 "+CStr(btn_value);
	elseif ( btn_type == GF_DUMMY_BTN )
		line += " 0 0 "+CStr(btn_value);
	else // Assume GF_PAGE_BTN
		line += " 0 "+CStr(btn_value);
	endif

  line += " "+CStr(tilepic_id)+" "+CStr(hue)+" "+CStr(width_x)+" "+CStr(height_y);

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
		gump.data_id += 1;
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
 * GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background, scrollbar, hue, customstring)
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
 * hue: The Decimal Color Value of the cliloc'd text displayed
 * customstring: For clilocs that accept custom arguments.
 *
 * Return value
 * No return value.
 *
 */
function GFAddHTMLLocalized(byref gump, x, y, width, height, cliloc, background:=0, scrollbar:=0, hue:=0, customstring:="")
	if(hue == 0 && customstring == "")
		(gump.layout).Append("XMFHTMLGump "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
					+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(cliloc)+" "
          + " "+ CStr(CInt(background))+" "+ CStr(CInt(scrollbar)));
	elseif(hue > 0 && SizeOf(customstring) == 0)
		(gump.layout).Append("XMFHTMLGumpColor "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
					+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(CInt(cliloc))+" "
					+ " "+ CStr(CInt(background))+" "+ CStr(CInt(scrollbar))+" "+CStr(CInt(hue)));
  else
		(gump.layout).Append("XmfHtmlTok "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
					+ CStr(CInt(width))+" "+CStr(CInt(height))+" "+CStr(CInt(background))+" "
          + CStr(CInt(scrollbar))+" "+CStr(CInt(hue))+" "+CStr(CInt(cliloc))+" "
					+ "@"+CStr(customstring)+"@");
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
		text += " hue="+CStr(hue);
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
	var text := CStr(CInt(x))+" "+CStr(CInt(y))+" "+CStr(CInt(tile_id));

	hue := CInt(hue);
	if ( hue )
		text := "TilePicHue "+text+" "+CStr(hue);
	else
		text := "TilePic "+text;
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
		gump.data_id += 1;
		btn_value := gump.data_id;
	endif

	(gump.layout).Append("Radio "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
				+ CStr(CInt(unp_id))+" "+CStr(CInt(pres_id))+" "
				+ CStr(CInt(status))+" "+CStr(btn_value));
	return (btn_value);
endfunction

/*
 * GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text, txt_id:=0, lmt := 0)
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
 * lmt: The total length of text allowed to be entered into the text entry element.
 *      ( 0 is unlimited. )
 *
 * Notes:
 * Use GFExtractData() in gumps_ex.inc to easily remove the leading "#: "
 *
 * Return value
 * Text ID number where the text entry is set.
 *
 */
function GFTextEntry(byref gump, x, y, width, height, txt_color, ini_text:="", txt_id:=0, lmt := 0)

	gump.data.Append(""+CStr(ini_text));
	var index := gump.data.Size()-1;

	txt_id := CInt(txt_id);
	if ( txt_id < 1 ) // Catch errors (0) and invalid (negative) values
		gump.data_id += 1;
		txt_id := gump.data_id;
	endif

  if ( !lmt )
    (gump.layout).Append("TextEntry "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
          + CStr(CInt(width))+" "+CStr(CInt(height))+" "
          + CStr(CInt(txt_color))+" "+CStr(txt_id)+" "+CStr(index));
  else
    (gump.layout).Append("TextEntryLimited "+CStr(CInt(x))+" "+CStr(CInt(y))+" "
          + CStr(CInt(width))+" "+CStr(CInt(height))+" "
          + CStr(CInt(txt_color))+" "+CStr(txt_id)+" "+CStr(index)
          + " "+CStr(CInt(lmt)));
  endif
	return (txt_id);
endfunction

/*
 * GFSendGump(mobile, gump)
 *
 * Purpose
 * Sends the gump to the character specified.
 *
 * Parameters
 * mobile:		Reference to the character which should receive the gump.
 * gump:	The gump itself, consisting of it's layout and its data.
 *
 * Return value
 * The SendDialogGump().
 *
 */
function GFSendGump(mobile, byref gump)
	var ret_val := SendDialogGump(mobile, gump.layout, gump.data, gump.base.x, gump.base.y);
	if ( ret_val.errortext )
		//Sometimes comes up when the gump has too much information.
		SendSysMessage(mobile, "Error - Unable to display gump.");
		SendSysMessage(mobile, "Error text is ->"+ret_val.errortext);
	endif

	return ret_val;
endfunction


function GFDrawWrappedText( byref gump, byref x, byref y, byref text_lines, txt_color := 2100 )
	foreach line in ( text_lines )
		GFTextLine( gump, x, y, txt_color, line );
		y += 20;
		SleepMS(2);
	endforeach

	return 1;
endfunction

/*
 * XGFError(text, err_log)
 *
 * Purpose
 * Internal function for assembling an error and making a system log.
 *
 * Parameters
 * text:	The errortext of the error.
 * err_log:		0 - Will only return the error.
 *		1 - will make a system log.
 *
 * Return value
 * error with an .errortext member.
 *
 */
function XGFError(text, err_log:=0)
	if ( err_log )
		SysLog("Gump Error:: "+text);
	endif

	return error{"errortext":=text};
endfunction

/*
 * GFPicTiled(byref gump, x, y, width, height, gump_id)
 *
 * Purpose
 * Adds a pic tiled gump to the gump.
 *
 * Parameters
 * gump:	Reference to the gump.
 * x:		The top-left spot of the X axis.
 * y:		The top-left spot of the Y axis.
 * width:	The width (pixels) of the gump from position X.
 * height:	The height (pixels) of how far to scale the gump downwards from position Y.
 * gump_id:	The number representing the gump.
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

/*
 * GFTooltip(byref gump, cliloc_id, customstring)
 *
 * Purpose
 * Adds a tooltip to the previous layoutarray entry
 *
 * Parameters
 * gump: Reference to the gump
 * cliloc_id:  The decimal number reference to the cliloc entry which will be used as the tooltip.
 * customstring: Used for clilocs that accept custom arguments.
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
 * GFItemProperty ( byref gump, item_serial )
 *
 * Purpose
 * Adds a tooltip to the previous layoutarray entry which directly corresponds to the given item's serial tooltip properties.
 *
 * Parameters
 * gump: Reference to the gump
 * item_serial: item's serial id.
 *
 *
 * Return value
 * No return value.
 *
 */
 function GFItemProperty ( byref gump, item_serial )
 var text := "itemproperty " + CStr ( CInt ( item_serial ) );
 (gump.layout).Append(text);

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
 * elem_name:   The elem of the config to look at.
 * prop_name:   The name of the CONSTant to retrieve.
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