use uo;
use os;

//include ":areas:managment";
include ":attributes:attributes";
include ":magery:spells";

function runebookRecall( who, parms, book, charges, use_charge:=0 )

	var runebook_info := array{ book, parms, use_charge };
	if( use_charge )
		var script := Start_Script( ":magery:spellStarter", {who, 32, 1, runebook_info, 0, 1, 0, 0, 1} );
		if( script.errortext )
			SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
			return 0;
		endif
		charges -= 1;
		book.SetCharges( who, charges );
  else
		var script := Start_Script( ":magery:spellStarter", {who, 32, 0, runebook_info} );
		if( script.errortext )
			SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
			return 0;
		endif
	endif

	return 1;
endfunction

function runebookGate( who, parms, book )

	var runebook_info := array{ book, parms };
	var script := Start_Script( ":magery:spellStarter", {who, 52, 1, runebook_info} );
	if( script.errortext )
		SendSysMessage( who, "Error - Could not start spell starter ->"+script.errortext );
		return 0;
	endif

	return 1;
endfunction
