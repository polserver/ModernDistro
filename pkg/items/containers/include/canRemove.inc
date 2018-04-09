use uo;

// include ":security:itemReport";

function Anti_Fast_Loot( mobile )

        if( GetObjProperty( mobile, "#Looting" ) > ReadGameClock() && !GetObjProperty( mobile, "#FastLooting" ))
                return 0;
        else
                SetObjProperty( mobile, "#Looting", ReadGameClock() + 1 );
                return 1;
        endif
endfunction

function AddLooter( mobile, corpse, item )

 	var serial	 := mobile.serial,
	    time	 := ReadGameClock(),
	    itemdesc     := item.desc,
            itemserial   := item.serial,
            corpseserial := corpse.serial;

        var last_loot := { serial, time, itemdesc, itemserial, corpseserial };
	SetObjProperty( corpse, "LastLoot", last_loot );
 
//        ITEM_ReportText( mobile, last_loot, ITEM_REPORT_SYSLOG );

	return last_loot;
endfunction
