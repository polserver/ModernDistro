use uo;
use os;
use cliloc;

include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":gumps:gumpCaching";
include ":virtue:noto";

const LONG_COUNT_DECAY_TIME := 144000; // 40 hours
const SHORT_COUNT_DECAY_TIME := 28800; // 8 hours

function SendReportGump( mobile )

	var gump := GFLoadGump( ":gumps:gumps", "ReportMurder", "ReportMurder" );
	if( gump.errortext )
		gump := GFCreateGump( 150, 100 );

		GFClosable( gump, 0 );

		// Load Main Background
		GFResizePic( gump, 0, 0, GFGetCfgConst( "Defaults", "BackGround" ), 350, 425 );

		GFResizePic( gump, 15, 15, GFGetCfgConst( "Defaults", "ForeGround" ), 320, 25 );
		GFHTMLArea( gump, 15, 20, 320, 32, "<div align=center>Report Murder", 0, 0 );

		// BackgroundAreas
		GFResizePic( gump, 15, 45, GFGetCfgConst( "Defaults", "ForeGround" ), 320, 370 );
		GFHTMLArea( gump, 20, 50, 310, 60, "<div align=center><basefont color=#A52A2A>Now is the time for retribution!</basefont>\n<basefont color=#0E0E10>Check zero or more players to report for your murder.</basefont>", 1, 0 );

		// Okay Button
		GFAddButton( gump, 150, 382, 2128, 2129, GF_CLOSE_BTN, 0xA00 );
		GFStoreGump( ":gumps:gumps", "ReportMurder", "ReportMurder", gump );
	endif

	var reportables := mobile.reportables,
		num_report := reportables.Size();

	GFPage( gump,  1 );

	var y_pos := 140;
	foreach entry in ( reportables )

		if( entry.killer.cmdlevel > mobile.cmdlevel )
			continue;
		endif

		if( !entry.killer.cmdlevel )
			GFCheckBox( gump, 45, y_pos, 210, 211, 1, _entry_iter );
			GFHTMLArea( gump, 70, y_pos+2, 200, 22, entry.killer.name, 0, 0 );
		endif

		y_pos += 20;
		if( _entry_iter % 12 == 0 && _entry_iter < num_report )
			GFAddButton( gump, 30, 348, 0xA92, 0xA93, GF_PAGE_BTN, gump.cur_page+1 );
			GFPage( gump,  gump.cur_page+1 );
			GFAddButton( gump, 30, 95, 0xA90, 0xA91, GF_PAGE_BTN, gump.cur_page-1 );
			y_pos := 140;
		endif

		SleepMS( 2 );
	endforeach

	// And now we need to know the answer...
	var input := GFSendGump( mobile, gump );

	var holder := Array;
	foreach key in ( input.keys )
		if( key >= 1 && key <= num_report )
			var value := reportables[key];
			mobile.RemoveReportable( value.serial, value.gameclock );
			holder.Append( value );

			var source := SystemFindObjectBySerial( value.serial );
			SendSysMessageCL( source, 1049067, Array, 0x3, 0x32 ); // You have been reported for a murder!
			AdjustNoto( source, mobile );
			AdjustMurderCounts( source );
		endif
		SleepMS( 2 );
	endforeach

	if( holder.Size() )
		Start_Script( ":bountyboard:bounty", { mobile, holder } );
	endif

	foreach reportable in reportables
		mobile.RemoveReportable( reportable.serial, reportable.gameclock );
	endforeach

	return 1;
endfunction

function AdjustMurderCounts( source )

	var onlinetime := GetObjProperty( source, "onlinetimer" );
	var longcount := GetObjProperty( source, "longmurders" );
	if( !longcount )
		longcount := 0;
	endif

	SetObjProperty( source, "longmurders", longcount+1 );
	if( longcount>4 )
		source.SetMurderer( 1 );
		SendSysMessageCL( source, 502134, Array, 0x3, 0x22 ); // You are now known as a murderer!
	endif

	SetObjProperty( source, "decaylongcountat", onlinetime+LONG_COUNT_DECAY_TIME );

	var shortcount := GetObjProperty( source, "shortmurders" );
	if( !shortcount )
		shortcount := 0;
	endif
	SetObjProperty( source, "shortmurders", shortcount+1 );
	SetObjProperty( source, "decayshortcountat", onlinetime+SHORT_COUNT_DECAY_TIME );

	return 1;
endfunction
