/*
        Maintained by The POL Distro Team

        2018-07-25

*/
use uo;
use os;
use math;

include ":equipment:equipment";
include ":equipment:protections";
include ":security:itemReport";

function UnEquipEffect( mobile, item )


        RemoveEquipmentMods( mobile, item, EQUIP_BOTH_MODS );
        RemoveArMod( mobile, item );
        RemoveProtMod( mobile, item );

 	if( GetObjProperty( item, "OfBlackrock" ))
		AP_SetVital( mobile, MANA, 0 );
	endif

        if( item.IsStaffRobe() )
                unEquip_StaffRobe( item );
	elseif( item.objtype == 0x8260 ) //Tazz Longbow
		mobile.Disable( "firewhilemoving" );
		RevokePrivilege( mobile, "firewhilemoving" );
	endif

	var desc := item.desc;
	if( !desc )
		desc := item.name;
	endif

	EraseObjProperty( item, "EquippedOn" );
        ITEM_ReportText( mobile, "Unequip Desc: "+desc+" Serial: "+item.serial+" X,T,Z,Realm: "+mobile.x+","+mobile.y+","+mobile.z+","+mobile.realm, ITEM_REPORT_SYSLOG );

        return 1;
endfunction

function RemoveProtMod( mobile, item )

	var possibles := GetProtections(),
            protections := GetObjProtections( mobile );

	foreach protection in possibles
		var level := CInt( GetObjProperty( item, protection ));
		if( level )
                        level := 0;
			foreach thing in ListEquippedItems( mobile )
				var temp_level := CInt( GetObjProperty( thing, protection ));
				if( item != thing )
                                        if( temp_level && level < temp_level )
                                        	level := temp_level;
					endif
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

function RemoveArMod( mobile, item )

	var ar_mod := CInt( GetObjProperty( item, "ArBonus" ));
	if( ar_mod )
		mobile.ar_mod := mobile.ar_mod - ar_mod;
	endif

        return 1;
endfunction

function unEquip_StaffRobe( item )

        item.color := 0;

        return 1;
endfunction
