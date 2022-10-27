use uo;
use os;
use cliloc;

include "include/eventID";
include "include/sysEvent";
include ":insurance:insurance";
include ":itemutils:itemUtil";
include ":security:contextMenuReport";

/*
 *
 * ContextMenu_SendNPCEvent( mobile, npc, str )
 *
 * Parameters:
 * mobile - player ref accessing context menu
 * npc - npc to send event to
 * str - string to send to within the event struct.
 *
 * Purpose:
 * Sends an event to the npc ai script.
 *
 * Date: 2022-07-02 18:09:41
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * none
 *
 */
function ContextMenu_SendNPCEvent( byref mobile, byref npc, byref str )

	var event := SendEvent( npc, Struct{ "source":=mobile, "text":=npc.name+" "+str, "type":=EVID_SPEECH } );
	if( event.errortext )
        ContextMenu_ReportText( mobile, "ContextMenu_SendNPCEvent -> "+event.errortext, CM_REPORT_CONSOLE+CM_REPORT_SYSLOG );
	endif
endfunction

/*
 *
 * ContextMenu_SendMerchantNodeEvent( mobile, npc, str )
 *
 * Parameters:
 * mobile - player ref accessing context menu
 * npc - npc to send event to
 * str - string to send to within the event struct.
 *
 * Purpose:
 * Sends an event to the merchant node, this is only used for sending events to
 * merchant/banker npcs.
 *
 * Date: 2022-10-23 02:54:07
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * N/A
 *
 */
function ContextMenu_SendMerchantNodeEvent( byref mobile, byref npc, byref str )

    var node_serial := GetObjProperty( npc, "MerchantNode" );
    if( node_serial.errortext )
        PrintTextAbove( npc, "Context Menus are broken for me.", 0x3, 0x22 );
        return 0;
    endif

    var node := SystemFindObjectBySerial( node_serial );
    var event := node.SendMerchantNodeEvent( Struct{ "source":=mobile, "npc":=npc, "text":=npc.name+" "+str, "type":=SYSEVENT_SPEECH } );
    if( event.errortext )
        ContextMenu_ReportText( mobile, "ContextMenu_SendMerchantNodeEvent -> "+event.errortext, CM_REPORT_CONSOLE+CM_REPORT_SYSLOG );
    endif
endfunction

/*
 *
 * ContextMenu_OpenPack( byref mobile, byref obj )
 *
 * Parameters:
 * mobile - player
 * obj - mobile_ref of the owner of the backpack being accessed.
 * Parameter_2_Name
 *
 * Purpose:
 * Opens backpack
 *
 * Date: 2022-07-02 18:37:40
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Return_Value_Type - Description_of_return_value
 *
 */
function ContextMenu_OpenPack( byref mobile, byref obj )

	if( mobile == obj )
		UseItem( mobile.backpack, mobile );
	elseif( mobile != obj )
		if( obj.master == mobile )
			SendOpenSpecialContainer( mobile, obj.backpack );
		elseif( mobile.cmdlevel > obj.cmdlevel )
			SendOpenSpecialContainer( mobile, obj.backpack );
		endif
	endif

	return 1;
endfunction

/*
 *
 * ContextMenu_RotateObject( mobile, obj )
 *
 * Parameters:
 * Parameter_1_Name
 * Parameter_2_Name
 *
 * Purpose:
 * Purpose_Description
 *
 * Date: 2022-07-09 07:58:03
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Return_Value_Type - Description_of_return_value
 *
 */
function ContextMenu_RotateObject( byref mobile, byref obj )

    if( obj.IsA( POLCLASS_MOBILE ) )
        obj.facing := ( ( obj.facing + 1 ) % 8 );
    else
        Start_Script( ":itemutils:rotate/rotate", { mobile, obj } );
    endif
endfunction

function ContextMenu_InsureItem( byref mobile )

    var insured_items :={};
    var bank_balance := CalculateBankGold( mobile );
    var insured_subtotal := 0;
    var ins_item_cost := 0;
    while( 1 )
        SendSysMessageCL(mobile, 1060868, color := 88 ); // Target the item you wish to toggle insurance status on <ESC> to cancel
        var ins_item := Target( mobile );
        if( !ins_item )
            if( insured_items.size() > 0 )
                foreach thing in insured_items
                    if( GetObjProperty( thing, "ToggleInsOn" ) )
                        PayForInsurance( mobile, thing, CalculateInsCost( thing ) );
                        EraseObjProperty( thing, "ToggleInsOn" );
                        IncRevision( thing );
                    endif
                endforeach
            endif
            SendSysMessageCL( mobile, 1042115, color := 66 ); // All done.
            return 1;
        endif
        ins_item_cost := CalculateInsCost( ins_item );
        if( !((ins_item.container == mobile.backpack) || (ins_item in ListEquippedItems( mobile ))) )
            SendSysMessageCL( mobile, 1060871, font := 2, color := 33 ); // You can only insure items that you have equipped or that are in your backpack
            continue;
        endif
        if( (ins_item.stackable) || (ins_item.cursed) || (ins_item.IsA( POLCLASS_CONTAINER )) && !ins_item.IsQuiver() )
            SendSysMessageCL( mobile, 1060869, color := 33 ); // You cannot insure that
            continue;
        elseif( ins_item.newbie )
            SendSysMessageCL( mobile, 1060870, color := 56 ); // That item is blessed and does not need to be insured
            continue;
        endif
        if ( !(GetObjProperty( ins_item, "ToggleInsOn" )) && !(ins_item.insured) )
            insured_subtotal += ins_item_cost;
            if( !(insured_subtotal > bank_balance) )
                insured_items.append( ins_item );
                SetObjProperty( ins_item, "ToggleInsOn", 1 );
                SendSysMessageCL(mobile, 1060873, color := 66 ); // You have insured the item
            else
                SendSysMessage(mobile, "You don't have enough gold remaining in your bank to insure " + ins_item.name +".", 1, 33);
                insured_subtotal -= ins_item_cost;
                continue;
            endif
        else
            var to_be_removed := ins_item in insured_items;
            if( to_be_removed )
                insured_items.erase( to_be_removed );
            endif
            EraseObjProperty( ins_item, "ToggleInsOn" );
            insured_subtotal -= ins_item_cost;
            ins_item.insured := 0;
            IncRevision( ins_item );
            SendSysMessageCL( mobile, 1060874, color := 40 ); // You cancel the insurance on the item
        endif
    endwhile

endfunction

function ContextMenu_InsuranceMenu( byref mobile )

    if( !Run_Script( ":insurance:commands/player/insurancemenu", {mobile} ) )
        Print("Could not run script");
    endif

endfunction
