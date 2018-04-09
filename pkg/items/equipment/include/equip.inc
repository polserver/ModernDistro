/*
        Maintened by *Edwards

        2008-08-30

        Last Modification 2009-10-26
             * Cleaned codes for better performances

        Last Modification 2009-11-16
             * Cleaned codes for better performances

        Last Modification 2009-11-19
             * Added physical protection checks since it has been removed from combat process..
*/
use uo;
use os;
use math;

include ":attributes:attributes";
include ":equipment:equipment";
include ":equipment:canEquip";
include ":equipment:protections";
//include ":loot:identify";

function EquipEffect( mobile, item )

/*        var script := Start_Script( ":classes:showClasse", {mobile, 1} );
        if( script.errortext )
	       SysLog( script.errortext );
        endif
*/
	if( NotYours( mobile, item ))
		return 0;
/*	elseif( !ProhibitedByClasse( mobile, item ))
		SendSysMessage( mobile, "Your specialisation prevents you from equipping this." );
		return 0;
*/
	elseif( !CanEquipRitualEquipment( mobile, item ))
		return 0;
        elseif( GetObjProperty( item, "OfBlackrock" ))
		AP_SetVital( mobile, MANA, 0 );
	elseif( item.IsStaffRobe() )
                equip_StaffRobe( mobile, item );
	elseif( item.objtype == 0x8260 ) //Tazz Longbow
		GrantPrivilege( mobile, "firewhilemoving" );
		mobile.Enable( "firewhilemoving" );
        endif

        if( CInt( GetObjProperty( item, "Cursed" )) == 1 && !mobile.cmdlevel )
		ApplyCurse( mobile, item );
                SetObjProperty( item, "Cursed", 2 );
	endif

        ApplyEquipmentMods( mobile, item, EQUIP_BOTH_MODS );
        ApplyProtMod( mobile, item );
        ApplyArMod( mobile, item );

        return 1;
endfunction

function NotYours( mobile, item )

	var owner_serial := GetObjProperty( item, "owner" );
	if( !owner_serial )
                if( GetObjProperty( item, "IsDonatorItem" ))
                	SetObjProperty( item, "owner", mobile.serial );
                endif
                return 0;
	endif

        var owner := SystemFindObjectBySerial( owner_serial ),
            owner_name := CStr( owner.name );

	if( owner_serial != mobile.serial )
		SendSysMessage( mobile, "You can't equip that, it belongs to "+owner_name+"." );

                //Return the item to the owner
                var script := Start_Script( ":equipment:equip/itemReturner", item );
	        if( script.errortext )
		       SysLog( "Error: Unable to start <:security:equip/itemReturner> ->"+script.errortext );
	        endif

		return 1;
	endif

	return 0;
endfunction

function ApplyCurse( mobile, item )

	PlaySoundEffect( mobile, 0x214 );
	PlayObjectCenteredEffect( mobile, 0x374A, 7, 14 );
 
        NameByEnchant( item );
	SendSysMessage( mobile, "That item is cursed, and reveals itself to be "+item.desc );

        return 1;
endfunction

function ApplyProtMod( mobile, item )

	var possibles := GetProtections(),
            protections := GetObjProtections( mobile );

	foreach protection in possibles
		var level := CInt( GetObjProperty( item, protection ));
		if( level )
			foreach thing in ListEquippedItems( mobile )
				var temp_level := CInt( GetObjProperty( thing, protection ));
				if( temp_level && level < temp_level )
                                        level := temp_level;
				endif
                                SleepMS(5);
			endforeach

                        protections[protection] := CInt( level );
		endif
                SleepMS(5);
	endforeach

        SetMobileProtections( mobile, protections );

	return 1;
endfunction

function equip_StaffRobe( mobile, item )

        case( mobile.cmdlevel )
		0:      NotifyStaffOnline( mobile, "is trying to equip a staff robe. Item destroyed." );
                        DestroyItem( item );
			break;

		1:	item.color := 1170;
			break;

		2:	item.color := 1159;
			break;

		3:	item.color := 1172;
			break;

		4:	item.color := 1;
			break;

		5:	item.color := 1172;
			break;
        endcase

	ApplyCurse( mobile, item );
        SetObjProperty( item, "Cursed", 2 );
        SetObjProperty( item, "owner", mobile.serial );

        return 1;
endfunction

function ApplyArMod( mobile, item )
        
	var ar_mod := CInt( GetObjProperty( item, "ArBonus" ));
	if( ar_mod )
		mobile.ar_mod := mobile.ar_mod + ar_mod;
	endif

        return 1;
endfunction

function CanEquipRitualEquipment( mobile, item )

        if( item.IsRitualEquipment() )
                if( !GetObjProperty( item, "Consecrated" ))
                        SendSysMessage( mobile, "This item is not consecrated." );
                        return 0;
                endif
        endif

        return 1;
endfunction
