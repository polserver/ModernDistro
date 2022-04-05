use uo;
use os;

//include ":areas:managment";
include ":attributes:attributes";
include ":magery:spells";

function runebookRecall( who, parms, book, charges, use_charge:=0 )

	var runebook_info := array{ book, parms, use_charge };
	if( use_charge )
		var script := Start_Script( ":magery:spellStarter", {who, 32, 1, 0, runebook_info, 1, 0, 0, 1} );
		if( script.errortext )
			SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
			return 0;
		endif
		charges -= 1;
		SetCharges( book, who, charges );
  else
		var script := Start_Script( ":magery:spellStarter", {who, 32, 0, 0, runebook_info, 0} );
		if( script.errortext )
			SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
			return 0;
		endif
	endif

	return 1;
endfunction

function runebookGate( who, parms, book )

	var runebook_info := array{ book, parms };
	// var script := Start_Script( ":magery:spellStarter", {who, 52, 1, runebook_info} );
	var script := Start_Script( ":magery:spellStarter", {who, 52, 0, 0, runebook_info} );
	if( script.errortext )
		SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
		return 0;
	endif

	return 1;
endfunction

function runebookSacredJourney( who, parms, book )
  // Just a quick fix to declog my unused variable warnings until this is actually finished.
  parms := parms;
  book := book;
  SendSysMessageCL ( who, 1153203, color := 0x31C ); // This function has not been implemented yet.
endfunction