use uo;
use os;

include ":gumps:gumps";
include ":gumps:gumpCaching";
include ":merchantnodes:nodeEvents";
include ":merchantnodes:settings";

enum MN_BUTTONS
	ACCEPT_BTN				:= 0xA,
	CLOSE_BTN				:= 0xB,
	ADD_MERCHANT_BTN		:= 0xC,
	TOGGLE_NODE_BTN			:= 0xD,
	RESTOCK_ALL_BTN			:= 0xE,
	RESPAWN_MERCHANTS_BTN	:= 0xF,
	UNCACHE_BTN				:= 0x10,
	NODE_SETTINGS_BTN		:= 0x11
endenum

function MN_BuildBaseGump()

	var gump := GFLoadGump( ":gumps:gumps", "MerchantNode", "MerchantNode" );
	if( gump.errortext )
		gump := GFCreateGump( 50, 50 );
		GFDisposable( gump, 0 );
		GFPage( gump, 0 );
		GFResizePic( gump, 0, 0, 9200, 705, 400 );
		GFPicTiled( gump, 10, 10, 685, 350, 3004 );
		GFHTMLArea( gump, 10, 10, 685, 20, "<basefont color=#8B4513><div align=center>Merchant Node Interface" );

		GFHTMLArea( gump, 20, 35, 80, 20, "<div align=center>NPCTemplate" );
		GFHTMLArea( gump, 150, 35, 200, 20, "<div align=center>Location" );
		GFHTMLArea( gump, 355, 35, 80, 20, "<div align=center>Enabled" );
		GFHTMLArea( gump, 440, 35, 80, 20, "<div align=center>Restock" );
		GFHTMLArea( gump, 525, 35, 80, 20, "<div align=center>Delete" );
		GFHTMLArea( gump, 610, 35, 80, 20, "<div align=center>Customize" );

		GFHTMLArea( gump, 539, 316, 150, 20, "Restock Merchants" );

		GFHTMLArea( gump, 539, 338, 150, 20, "Respawn Merchants" );

		GFAddButton( gump, 15, 363, 2122, 2123, GF_CLOSE_BTN, ACCEPT_BTN );
		GFAddButton( gump, 75, 363, 2119, 2120, GF_CLOSE_BTN, CLOSE_BTN );
		GFHTMLArea( gump, 170, 365, 100, 22, "<div align=left>Add Merchant" );

		GFAddButton( gump, 275, 363, 4011, 4013, GF_CLOSE_BTN, NODE_SETTINGS_BTN );
		GFHTMLArea( gump, 310, 365, 100, 22, "<div align=left>Node Settings" );

		GFPage( gump, 1 );
		GFStoreGump( ":gumps:gumps", "MerchantNode", "MerchantNode", gump );
	endif

	return gump;
endfunction

function MN_PopulateGump( byref gump, byref node )

	var status := node.GetStatus();

	GFAddButton( gump, 660, 15, 4026, 4028, GF_CLOSE_BTN, UNCACHE_BTN );
	GFHTMLArea( gump, 575, 15, 80, 22, "<div align=left>Uncache Gump" );
	if( status )
		GFAddButton( gump, 504, 313, 4005, 4007, GF_CLOSE_BTN, RESTOCK_ALL_BTN );
		GFAddButton( gump, 504, 335, 4005, 4007, GF_CLOSE_BTN, RESPAWN_MERCHANTS_BTN );
		GFAddButton( gump, 135, 363, 4008, 4010, GF_CLOSE_BTN, ADD_MERCHANT_BTN );
	else
		GFGumpPic( gump, 504, 313, 4005 );
		GFGumpPic( gump, 504, 335, 4005 );
		GFGumpPic( gump, 135, 363, 4008 );
	endif

	var y := 55,
		merchant_list := node.LoadMerchantInfo(),
		anchor := Array,
		enabled := 0,
		template := "",
		alive := 0;
	foreach key in ( merchant_list.Keys() )
		anchor := merchant_list[key].Anchor;
		enabled := merchant_list[key].Enabled;
		template := merchant_list[key].Template;

		if( SystemFindObjectBySerial( key ).IsA( POLCLASS_NPC ) )
			alive += 1;
		endif

		GFHTMLArea( gump, 20, y, 80, 20, "<div align=center>"+template );
		GFHTMLArea( gump, 150, y, 200, 20, "<div align=center>"+anchor.X+" "+anchor.Y+" "+anchor.Z+" "+anchor.Realm );

		if( status )
			if( enabled )
				GFCheckBox( gump, 385, y, 211, 210, 0, 5000+_key_iter );
			else
				GFCheckBox( gump, 385, y, 210, 211, 0, 5000+_key_iter );
			endif
		else
			GFGumpPic( gump, 385, y, 210 );
		endif

		if( status )
			GFCheckBox( gump, 470, y, 210, 211, 0, 6000+_key_iter ); // Restock
			GFAddButton( gump, 549, y-1, 4002, 4004, GF_CLOSE_BTN, 7000+_key_iter ); // Delete
			GFAddButton( gump, 630, y-1, 4011, 4013, GF_CLOSE_BTN, 8000+_key_iter ); // Customize
		else
			GFGumpPic( gump, 470, y, 210 );
			GFGumpPic( gump, 549, y-1, 4002 );
			GFGumpPic( gump, 630, y-1, 4011 );
		endif

		y+=25;
	endforeach

	if( alive == merchant_list.Size() )
		GFTextLine( gump, 15, 15, 0x65, "Current:  "+alive+"/"+merchant_list.Size() );
	elseif( !alive )
		GFTextLine( gump, 15, 15, 0x22, "Current:  "+alive+"/"+merchant_list.Size() );
	else
		GFTextLine( gump, 15, 15, 0x35, "Current:  "+alive+"/"+merchant_list.Size() );
	endif

	var	txt := "";
	status := status.errortext ? -1 : status;
	case( status )
		1:			txt := "<basefont color=#83E631><div align=right>Merchant Node Enabled";
					GFAddButton( gump, 15, 333, 4021, 4022, GF_CLOSE_BTN, TOGGLE_NODE_BTN ); // Toggle Enabled/Disabled button
					GFTextLine( gump, 50, 334, 0x3F, "Node Enabled" );

		0:  		txt := "<basefont color=#DC143C><div align=right>Merchant Node Disabled";
					GFAddButton( gump, 15, 333, 4020, 4022, GF_CLOSE_BTN, TOGGLE_NODE_BTN ); // Toggle Enabled/Disabled button
					GFTextLine( gump, 50, 334, 0x22, "Node Disabled" ); // Enabled/Disabled text

		default:	txt := "<basefont color=#83E631><div align=right>Merchant Node Enabled";
					GFAddButton( gump, 15, 333, 4021, 4022, GF_CLOSE_BTN, TOGGLE_NODE_BTN ); // Toggle Enabled/Disabled button
					GFTextLine( gump, 50, 334, 0x3F, "Node Enabled" );
					SetObjProperty( node, "Status", "on" );
	endcase

	GFHTMLArea( gump, 501, 363, 200, 22, txt, 0, 0 );
endfunction

function MN_CustomizeVendorGump( byref mobile, byref node, serial )

	var info := node.LoadMerchantInfo(),
		gump := GFCreateGump( 50, 50 );

	var props := info[serial].Info;


	GFDisposable( gump, 0 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9200, 350, 334 );
	GFPicTiled( gump, 10, 10, 330, 314, 3004 );

	GFHTMLArea(gump, 10, 10, 330, 20, "<div align=center>Static Merchant Properties");
	GFHTMLArea(gump, 20, 40, 310, 40, "Enter a name which this vendor will always spawn with, leave blank for random.");

	GFPicTiled( gump, 20, 85, 310, 24, 2624 );
	GFPicTiled( gump, 21, 86, 308, 22, 3004 );
	GFTextEntry( gump, 22, 87, 306, 20, 0, props.Name != "<Random>" ? props.Name : "", 33 );

	GFHTMLArea( gump, 20, 114, 310, 40, "Select a static Gender the merchant will always spawn as." );

	GFSetRadioGroup( gump, 1 );
	GFRadioButton( gump, 20, 159, 208, 209, props.Gender != -1 ? 0 : 1, 100 );
	GFHTMLArea( gump, 45, 159, 200, 20, "Randomize" );

	GFRadioButton( gump, 20, 184, 208, 209, props.Gender != 0 ? 0 : 1, 101 ); // Male
	GFAddHTMLLocalized( gump, 45, 184, 200, 20, 1015327 );

	GFRadioButton( gump, 20, 209, 208, 209, props.Gender != 1 ? 0 : 1, 102 ); // Female
	GFAddHTMLLocalized( gump, 45, 209, 200, 20, 1015328 );

// Page break
	GFPicTiled( gump, 15, 234, 320, 2, 2624 );

	GFCheckBox( gump, 20, 241, 210, 211, ( props.RandomizeSkinHue != 1 ? 0 : 1 ), 200 );
	GFHTMLArea( gump, 45, 241, 200, 20, "Randomize Skin Hue" );
	GFCheckBox( gump, 20, 266, 210, 211, ( props.RandomizeSpeechHue != 1 ? 0 : 1 ), 201 );
	GFHTMLArea( gump, 45, 266, 200, 20, "Randomize Speech Hue" );

	GFAddButton( gump, 135, 297, 4023, 4025, GF_CLOSE_BTN, ACCEPT_BTN );
	GFAddButton( gump, 170, 297, 4017, 4019, GF_CLOSE_BTN, CLOSE_BTN );

	var result := GFSendGump( mobile, gump );
	if( result[0] <= 1 || result[0] == CLOSE_BTN )
		return 0;
	endif

	var name := GFExtractData( result, 33 ),
		gender := -1,
		skin_hue := 0,
		speech_hue := 0;
	if( name.errortext )
		name:="<Random>";
	elseif( name == "" )
		name:="<Random>";
	endif

	foreach key in ( result.keys )
		if( key == 100 )
			gender := -1;
		elseif( key == 101 )
			gender := 0;
		elseif( key == 102 )
			gender := 1;
		endif

		if( key == 200 )
			skin_hue := 1;
		elseif( key == 201 )
			speech_hue := 1;
		endif
	endforeach

	Set_Critical( 1 );
	info[serial].Erase( "Info" );
	info[serial].Insert( "Info", Struct{ "Gender":=gender, "Name":=name, "RandomizeSkinHue":=skin_hue, "RandomizeSpeechHue":=speech_hue } );

	node.SaveMerchantInfo( info );
	Set_Critical( 0 );
	SleepMS( 50 );

	return 1;
endfunction

// Returns a struct of options set in the gump.
function MN_SettingsGump( byref mobile, byref node:=0, byref warnings:=0 )

	var orig_range := node.GetRange(),
		orig_respawn := node.GetRespawnTime(),
		orig_restock := node.GetRestockTime();
	if( !node.IsMerchantNode() )
		orig_range := CInt( MN_FindSettingsElem( "Settings" ).DefaultRange );
		orig_respawn := CDbl( MN_FindSettingsElem( "Settings" ).RespawnTime );
		orig_restock := CDbl( MN_FindSettingsElem( "Settings" ).RestockTime );
	endif

	var gump := GFCreateGump( 50, 50 );

	GFDisposable( gump, 0 );
	GFPage( gump, 0 );
	GFResizePic( gump, 0, 0, 9200, 300, 155 );
	GFPicTiled( gump, 10, 10, 280, 135, 3004 );

	GFHTMLArea( gump, 10, 10, 280, 20, "<basefont color=#8B4513><div align=center>Merchant Node Settings" );

	GFHTMLArea( gump, 15, 37, 180, 20, "Listen Range:" );
	GFPicTiled( gump, 205, 35, 80, 24, 2624 );
	GFPicTiled( gump, 206, 36, 78, 22, 3004 );
	GFTextEntry( gump, 207, 37, 76, 20, 0, orig_range, 101, 2 );
	if( warnings )
		GFTooltip( gump, 1042971, "<div align=left><BASEFONT color=#6699CC>If changed, will require shard restart to take effect." );
	endif

	GFHTMLArea( gump, 15, 62, 180, 20, "Respawn Time (in minutes):" );
	GFPicTiled( gump, 205, 60, 80, 24, 2624 );
	GFPicTiled( gump, 206, 61, 78, 22, 3004 );
	GFTextEntry( gump, 207, 62, 76, 20, 0, orig_respawn, 102, 4 );

	GFHTMLArea( gump, 15, 87, 180, 20, "Restock Time (in minutes):" );
	GFPicTiled( gump, 205, 85, 80, 24, 2624 );
	GFPicTiled( gump, 206, 86, 78, 22, 3004 );
	GFTextEntry( gump, 207, 87, 76, 20, 0, orig_restock, 103, 4 );

	GFAddButton( gump, 90, 119, 2122, 2123, GF_CLOSE_BTN, ACCEPT_BTN );
	GFAddButton( gump, 155, 119, 2119, 2120, GF_CLOSE_BTN, CLOSE_BTN );

	var result := GFSendGump( mobile, gump );
	if( result[0] == CLOSE_BTN || result[0] <= 1 )
		return 0;
	endif

	var range := VerifyIntegerValue( GFExtractData( result, 101 ) ),
		respawn := VerifyIntegerValue( GFExtractData( result, 102 ) ),
		restock := VerifyIntegerValue( GFExtractData( result, 103 ) );

	if( range.errortext )
		return error{ "errortext":="Range text entry may only contain integers." };
	elseif( respawn.errortext )
		return error{ "errortext":="Respawn text entry may only contain integers." };
	elseif( restock.errortext )
		return error{ "errortext":="Restock text entry may only contain integers." };
	endif

	var opts := Struct;
	if( CDbl( respawn ) != orig_respawn )
		opts.Insert( "RespawnTime", CDbl( respawn ) );
	endif

	if( CDbl( restock ) != orig_restock )
		opts.Insert( "RestockTime", CDbl( restock ) );
	endif

	if( range != orig_range )
		opts.Insert( "Range", range );
	endif

	return opts;
endfunction

// Simply verifies that the entry boxes only contain integers,
// if so, we return an integer of the text value else we return
// errortext...
function VerifyIntegerValue( byref text )

	var i := 1;
	for( i:=1; i<=Len( text ); i++ )
		if( !( text[i] in { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "." } ) )
			return error{ "errortext":="Value must contain only integers." };
		endif
	endfor

	return CInt( text );
endfunction
