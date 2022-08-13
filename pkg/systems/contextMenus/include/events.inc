use uo;
use os;

include "include/sysEvent";

/*
 *
 * ContextMenu_SendNPCEvent( mobile, npc, speech_event )
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
		Print( "ERROR::ContextMenu_SendNPCEvent -> "+event.errortext );
	endif
endfunction

function ContextMenu_SendMerchantNodeEvent( byref mobile, byref npc, byref str )

    var node_serial := GetObjProperty( npc, "MerchantNode" );
    if( node_serial.errortext )
        PrintTextAbove( npc, "Context Menus are  broken for me.", 0x3, 0x22 );
        return 0;
    endif

    var node := SystemFindObjectBySerial( node_serial );
    var process := GetProcess( GetObjProperty( node, "#Pid" ) );
    var event := process.SendEvent( Struct{ "source":=mobile, "npc":=npc, "text":=npc.name+" "+str, "type":=SYSEVENT_SPEECH } );
    if( event.errortext )
        Print( "ERROR::ContextMenu_SendMerchantNodeEvent -> "+event.errortext );
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

function ContextMenu_HandleJoinEvent( byref mobile, byref obj )

    if( obj.IsClassMaster() )
        ContextMenu_SendNPCEvent( mobile, obj, CM_JOIN );
    endif
endfunction

function ContextMenu_HandleResignEvent( byref mobile, byref obj )

    if( obj.IsClassMaster() )
        ContextMenu_SendNPCEvent( mobile, obj, CM_RESIGN );
    endif
endfunction