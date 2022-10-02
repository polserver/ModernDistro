use uo;

include ":attributes:attributes";

var maximumloss := 0.20; //20% loss for 20 shortterm murders or more

function ResPenalties(who)
	MurdererStatLoss(who);
endfunction

function MurdererStatLoss( who )

	var shortc := GetObjProperty( who, "shortmurders" );
	if( !shortc || shortc <= 4 )
		return 0;
	endif

	var multiplier := CDbl( CDbl( shortc ) / 100.0 );
	if( multiplier > maximumloss )
		multiplier := maximumloss;
	endif
	multiplier := 1.0 - multiplier;

	var amt := 0;
	foreach attribute in ( AP_ListAttributesByType() )
		amt := GetAttributeBaseValue( who, attribute );
		if( amt > 0 )
			SetAttributeBaseValue( who, attribute, CInt( amt*multiplier ) );
		endif
		SleepMS( 5 );
	endforeach
	AP_SetVital( who, HITS, AP_GetVitalMaximumValue( who, HITS ) );
	AP_SetVital( who, STAMINA, AP_GetVitalMaximumValue( who, STAMINA ) );
	AP_SetVital( who, MANA, AP_GetVitalMaximumValue( who, MANA ) );

	SendSysMessage( who, "Thy body pays the price for your sins.", 0x3, 0x22 );

	return 1;
endfunction
