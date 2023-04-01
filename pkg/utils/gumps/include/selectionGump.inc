/* $Id: selectiongump.inc 747 2006-08-25 04:02:34Z AustinHeilman $
 *
 */

/*===============================================================
* Current Version
* SELECTIONGUMP.INC - v1.0
* Updated 7/4/2006 1:10AM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;

include ":gumps:gumps";

CONST BTN_TYPE_RADIO	:= 0x1;	// Allows selecting one item in the list. "OKAY" must be clicked to return selections.
CONST BTN_TYPE_CHECKBOX	:= 0x2;	// Allows multiple selections in the list. "OKAY" must be clicked to return the selections.
CONST BTN_TYPE_NORMAL	:= 0x4;	// Allows selecting an item in the list. Gump returns choice when selection is made.
CONST BTN_NOTYPE		:= 0x8; // Displays a list of strings contained in options with no selection button.

CONST OKAY_BTN		:= 0xA00;

/*
 * SelectionGump( mobile, title, options, btn_type:=BTN_TYPE_NORMAL )
 *
 * Purpose
 * Creates gump with a list in a gump with optional ways to select an item or
 * multiple items from the list. See the comments for the button types above.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * title:   String that will be used for gump's title.
 * options:	Array containing strings to display next to the buttons.
 * btn_type:	Type of button to display next to the items in the list.
 *
 * Return Values
 * Returns a single value from the options array if RADIO or NORMAL are used.
 * Returns an array of option values if CHECKBOX is used.
 * Returns an error on failure.
 *
 */
function SelectionGump( mobile, title, options, btn_type:=BTN_TYPE_NORMAL )
	if( Lower( TypeOf( options )) != "array" )
		return error{"errortext":="Options list must be an array."};
	endif

	var s_gump := GFCreateGump();

	GFResizePic( s_gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 310, 480 );
	GFResizePic( s_gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 280, 450 );

	GFAddAlphaRegion( s_gump, 0, 0, 310, 480 );

	GFTextMid( s_gump, 15, 20, 280, 1720, title );

	if(btn_type != BTN_TYPE_NORMAL)
		GFAddButton( s_gump, 135, 440, 2128, 2129, GF_CLOSE_BTN, OKAY_BTN );
	endif

	GFPage( s_gump, 1 );
	var num_options := options.Size();
	var y_pos := 60;
	foreach entry in ( options )
		case( btn_type )
			BTN_TYPE_NORMAL:
				GFAddButton( s_gump, 30, y_pos+3, 2117, 2118, GF_CLOSE_BTN, _entry_iter );
				break;
			BTN_TYPE_CHECKBOX:
				GFCheckBox( s_gump, 30, y_pos, 210, 211, 0, _entry_iter );
				break;
			BTN_TYPE_RADIO:
				GFRadioButton( s_gump, 30, y_pos, 210, 211, 0, _entry_iter );
				break;
		endcase

		GFTextLine( s_gump, 60, y_pos, 2100, entry );
		y_pos := y_pos+20;

		if( _entry_iter % 19 == 0 && _entry_iter < num_options )
			GFAddButton( s_gump, 283, 405, 0xA92, 0xA93, GF_PAGE_BTN, s_gump.cur_page+1 );
			GFPage( s_gump, s_gump.cur_page+1 );
			GFAddButton( s_gump, 283, 45, 0xA90, 0xA91, GF_PAGE_BTN, s_gump.cur_page-1 );
			y_pos := 60;
		endif


		SleepMS(2);
	endforeach

	var input := GFSendGump( mobile, s_gump );
	if( !input[0] )
		return error{"errortext":="No option was selected."};
	endif
	var selections_made := 0;
	case( btn_type )
		BTN_TYPE_NORMAL:
			var val := input[0];
			return options[val];
		BTN_TYPE_CHECKBOX:
			var values := array;
			foreach key in ( input.keys )
				if( key >= 1 && key <= num_options )
					values.Append( options[key] );
					selections_made := 1;
				endif
				SleepMS(2);

			endforeach
			// The following returns an error if no checkboxes were checked and the Okay button is clicked.
			if(selections_made)
				return values;
			else
				return error{"errortext":="No option was selected."};
			endif
		BTN_TYPE_RADIO:
			foreach key in ( input.keys )
				if ( key >= 1 && key <= num_options )
					return options[key];
				endif
				SleepMS(2);
			endforeach
	endcase
endfunction


/*
 * SelectionGumpSmall( mobile, title, options, btn_type:=BTN_TYPE_NORMAL )
 *
 * Purpose
 * Creates gump with a list in a gump with optional ways to select an item or
 * multiple items from the list. See the comments for the button types above.
 *
 * Parameters
 * mobile:	Mobile to send the gump to.
 * title:   String that will be used for gump's title.
 * options:	Array containing strings to display next to the buttons.
 * btn_type:	Type of button to display next to the items in the list.
 *
 * Return Values
 * Returns a single value from the options array if RADIO or NORMAL are used.
 * Returns an array of option values if CHECKBOX is used.
 * Returns an error on failure.
 *
 */
function SelectionGumpSmall( mobile, title, options, btn_type:=BTN_TYPE_NORMAL )
	if( Lower( TypeOf( options )) != "array" )
		return error{"errortext":="Options list must be an array."};
	endif

	var s_gump := GFCreateGump();
	GFResizePic( s_gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 310, 240 );
	GFResizePic( s_gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 280, 210 );
	GFAddAlphaRegion( s_gump, 0, 0, 310, 240 );

	GFTextMid( s_gump, 15, 20, 280, 1720, title );

	if(btn_type != BTN_TYPE_NORMAL)
		GFAddButton( s_gump, 135, 200, 2128, 2129, GF_CLOSE_BTN, OKAY_BTN );
	endif

	GFPage( s_gump, 1 );
	var num_options := options.Size();
	var y_pos := 60;
	foreach entry in ( options )
		case( btn_type )
			BTN_TYPE_NORMAL:
				GFAddButton( s_gump, 30, y_pos+3, 2117, 2118, GF_CLOSE_BTN, _entry_iter );
				break;
			BTN_TYPE_CHECKBOX:
				GFCheckBox( s_gump, 30, y_pos, 210, 211, 0, _entry_iter );
				break;
			BTN_TYPE_RADIO:
				GFRadioButton( s_gump, 30, y_pos, 210, 211, 0, _entry_iter );
				break;
		endcase

		GFTextLine( s_gump, 60, y_pos, 2100, entry );
		y_pos := y_pos+20;

		if( _entry_iter % 7 == 0 && _entry_iter < num_options )
			GFAddButton( s_gump, 283, 200, 0xA92, 0xA93, GF_PAGE_BTN, s_gump.cur_page+1 );
			GFPage( s_gump, s_gump.cur_page+1 );
			GFAddButton( s_gump, 283, 45, 0xA90, 0xA91, GF_PAGE_BTN, s_gump.cur_page-1 );
			y_pos := 60;
		endif


		SleepMS(2);
	endforeach

	var input := GFSendGump( mobile, s_gump );
	if( !input[0] )
		return error{"errortext":="No option was selected."};
	endif
	var selections_made := 0;
	case( btn_type )
		BTN_TYPE_NORMAL:
			var val := input[0];
			return options[val];
		BTN_TYPE_CHECKBOX:
			var values := array;
			foreach key in ( input.keys )
				if( key >= 1 && key <= num_options )
					values.Append( options[key] );
					selections_made := 1;
				endif
				SleepMS(2);

			endforeach
			// The following returns an error if no checkboxes were checked and the Okay button is clicked.
			if(selections_made)
				return values;
			else
				return error{"errortext":="No option was selected."};
			endif
		BTN_TYPE_RADIO:
			foreach key in ( input.keys )
				if ( key >= 1 && key <= num_options )
					return options[key];
				endif
				SleepMS(2);
			endforeach
	endcase
endfunction


/*
* SelectionGumpEx( mobile, title, options, btn_type:=BTN_TYPE_NORMAL, transparent := 0, tooltips :={} )
*
* Purpose: Displays a vertical rectangle that lists items that can be selected.
* The button type (btn_type) determines whether multiple items or only one item
* can be selected.
*
* Parameters:	mobile		-	reference to a player.
*				title		-	The title displayed at the top of the gump.
*				options		-	An array containing the list of choices to be displayed in the gump.
*				btn_type	-	The type of button to be displayed next to each choice.
*								BTN_TYPE_NORMAL - Single selection and returns immediately without
*								the need to click OKAY.
*								BTN_TYPE_CHECKBOX - Multiple selections can be made. Requires the user
*								to click OKAY.
*								BTN_TYPE_RADIO - Single selection but requires the user to click OKAY.
*				transparent	-	If set to 1 makes the gump transparent. This parameter is optional and
*								defaults to no transparency but if you pass a tooltips parameter you must
*								specify this parameter.
*				tooltips	-	Array of tooltips for each item in the options array. These can be
*								custom string tooltips or integer cliloc numbers. This parameter is
*								optional and defaults to an empty array (no tooltips) if no tooltip
*								array is passed to the function.
*
* Return Values: The user's selection which will be a single string if BTN_TYPE_NORMAL or BTN_TYPE_RADIO
*				 buttons are used. An array of strings selected if BTN_TYPE_CHECKBOX is used.
*				 Error if no selection is made and the gump is closed by right-clicking it.
*/
function SelectionGumpEx( mobile, title, options, btn_type:=BTN_TYPE_NORMAL, transparent := 0, tooltips :={} )

	if( Lower( TypeOf( options )) != "array" )
		return error{"errortext":="Options list must be an array."};
	endif

	var s_gump := GFCreateGump();

	GFResizePic( s_gump, 0, 0, 9250, 306, 476 );
	GFPicTiled( s_gump, 13, 13, 280, 450, 3004 );

	if(transparent)
		GFAddAlphaRegion( s_gump, 0, 0, 310, 480 );
	endif

	GFHTMLArea( s_gump, 15,  25, 280, 26, title, 1, 0 );

	if(btn_type != BTN_TYPE_NORMAL)
		GFAddButton( s_gump, 135, 440, 2128, 2129, GF_CLOSE_BTN, OKAY_BTN );
	endif

	GFPage( s_gump, 1 );
	var num_options := options.Size();
	var y_pos := 60;
	foreach entry in ( options )
		case( btn_type )
			BTN_TYPE_NORMAL:
				GFAddButton( s_gump, 30, y_pos+3, 9792, 9793, GF_CLOSE_BTN, _entry_iter );
				break;
			BTN_TYPE_CHECKBOX:
				GFCheckBox( s_gump, 30, y_pos, 9792, 9793, 0, _entry_iter );
				break;
			BTN_TYPE_RADIO:
				GFRadioButton( s_gump, 30, y_pos, 9792, 9793, 0, _entry_iter );
				break;
		endcase

		GFHTMLArea( s_gump, 60, y_pos+3, 210, 26, entry, 1, 0 );
		if(TypeOf(tooltips[_entry_iter]) == "String")
			GFTooltip(s_gump, 1042971, tooltips[_entry_iter]);
		elseif(TypeOf(tooltips[_entry_iter]) == "Integer")
			GFTooltip(s_gump, tooltips[_entry_iter]);
		endif
		y_pos := y_pos+31;

		if( _entry_iter % 12 == 0 && _entry_iter < num_options )
			GFAddButton( s_gump, 283, 412, 0xA92, 0xA93, GF_PAGE_BTN, s_gump.cur_page+1 );
			GFPage( s_gump, s_gump.cur_page+1 );
			GFAddButton( s_gump, 283, 64, 0xA90, 0xA91, GF_PAGE_BTN, s_gump.cur_page-1 );
			y_pos := 60;
		endif


		SleepMS(2);
	endforeach

	var input := GFSendGump( mobile, s_gump );
	if( !input[0] )
		return error{"errortext":="No option was selected."};
	endif
	var selections_made := 0;
	case( btn_type )
		BTN_TYPE_NORMAL:
			var val := input[0];
			return options[val];
		BTN_TYPE_CHECKBOX:
			var values := array;
			foreach key in ( input.keys )
				if( key >= 1 && key <= num_options )
					values.Append( options[key] );
					selections_made := 1;
				endif
				SleepMS(2);
			endforeach
			// The following returns an error if no checkboxes were checked and the Okay button is clicked.
			if(selections_made)
				return values;
			else
				return error{"errortext":="No option was selected."};
			endif
		BTN_TYPE_RADIO:
			foreach key in ( input.keys )
				if ( key >= 1 && key <= num_options )
					return options[key];
				endif
				SleepMS(2);
			endforeach
	endcase
endfunction

