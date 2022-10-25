use uo;
use util;

include "include/objtype";
include ":containers:storageAreas";
include ":currency:currency";
include ":insurance:settings";
include ":itemUtils:itemdesc";
include ":itemutils:offsets";
include ":gumps:gumps";
include ":gumps:gumpCaching";

enum BTN_LIST
	CANCEL_BTN		:= 1,
	AUTO_REINSURE	:= 2,
	CONF_BTN		:= 3,
	PREV_PAGE		:= 4,
	NEXT_PAGE		:= 5
endenum


//////////////////////////////////////////////////////////////////////////////////////////
//
//	InsureItem( player, item )
//
//	Purpose:	Applies insurance to a single item.
//				Tries to deduct the cost from the player's bank.
//				If the player has the gold, the item is insured.
//				If not, the player is notified and the item is not insured.
//				Used by the Toggle Insurance context menu.
//
//	Parameters:	player::charref, itemref
//
//	 Returns: 1 on success, 0 on failure
//
//	Created: 2022-07-29 18:20:46 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function InsureItem( player, item )

	var cost := CalculateInsCost( item );

	// If the item can be insured
	if( PayForInsurance( player, item, cost ) )
		item.insured := 1;
		return 1;
	endif
	return 0;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	UninsureItem( player, item )
//
//	This function attempts to refund the insurance cost to the player's bank.
//	Used by the Toggle Insurance context menu.
//
//	Parameters: who::characterref, item::item ref
//
//	Returns:	1 on success, 0 on failure
//
//	Created: 2022-07-28 12:02:12 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function UninsureItem( byref player, item )

	var bankbox := CP_GetStorageContainerForMobile( player, CP_BANK_AREA, CP_CREATE );
	var refund := CalculateInsCost( item );
	if( !CreateItemInContainer( bankbox, 0xEED, refund ) )
		SendSysMessage( player, "I could not refund the insurance cost for " + item.name + ".", color := 33 );
		SendSysMessage( player, "Your bankbox may be full.", color := 33 );
		return 0;
	endif
	item.insured := 0;
	return 1;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	CalculateInsCost( item )
//
//	Purpose:	Calculates the amount to charge the player for insuring an item.
//				Note: Specialized armor sets cost 600gp per piece to insure. We do not
//				have those specialized sets of armor. So, I did not provide tests for them.
//				See https://www.uoguide.com/Item_Insurance for more information.
//
//	Parameter: item::itemreference
//
//	Returns: integer > 0
//
//	Created: 2022-07-28 12:08:00 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function CalculateInsCost( item )

	var cost := 0;
	// Get the percent modifier to determine the insurance cost for an item.
	var elem := InsP_GetSettingsCfgElem( "Settings" );
	var percent_mod := CDbl( elem.InsuranceRate );

	// The insurance cost is based on an item's sell price.
	// If the item does not have a sell price then we randomly create one.
	var sell_price := CInt( item.sellprice );
	if( !sell_price )
		item.sellprice := RandomIntMinMax( 9, 101 );
		sell_price := item.sellprice;
	endif
	// Accrding to uoguide.com the insurance cost is supposed to be a percentage
	// depending on "their worth to NPC vendors." However, the percent amount is
	// not given. So, I chose the method below for determining cost.
	// Insurance cost will be between 50% to 100% of the vendor sell price.
	cost := CInt( CDbl( sell_price ) * percent_mod );
	// Add a mod to price based on quality
	cost := CInt( CDbl( cost ) * item.quality );
	// Other possible price mods can be added below.
	return cost;

endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	PayForInsurance( who, item, cost )
//
//	This function attempts to extract the price for insurance from the player's
//	bank box.
//
//	Parameters: who::characterref, cost::integer
//
//	Returns: 1 upon success, 0 upon failure
//
//	Created: 2022-07-28 11:55:17 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function PayForInsurance( byref player, byref item, cost )

	var bankbox := CP_GetStorageContainerForMobile( player, CP_BANK_AREA, CP_CREATE );
	if(!SpendCoin(player, cost, bankbox, UOBJ_GOLD_COIN))
		// SendSysMessageCL( player, 1061079 ); // You lack the funds to purchase the insurance
		SendSysMessage(player, "You don't have enough in the bank to insure " + item.name +".", 1, 33);
		return 0;
	endif
	item.insured := 1;
	return 1;
endfunction

function IsInsurable( item )

	var excluded_layers := {11, 16, 21, 25, 26, 27, 28, 29, 30, 31};
	if( !(item.stackable) && !(item.cursed) && !(item.newbie) && !(item.IsA( POLCLASS_CONTAINER )) || item.IsQuiver() )
		if( !(item.layer in excluded_layers) )
			return 1;
		endif
	else
		return 0;
	endif

endfunction

function ItemInsuranceMenuGump( mobile, items )

	if( GetProcess( GetObjProperty( mobile, "#ItemInsuranceGump" ) ) )
		return 0;
	endif
	SetObjProperty( mobile, "#ItemInsuranceGump", GetPID() );

	var insure := Array,
		page := 0,
		i := 1;
	for( i:=1; i<=items.Size(); i++ )
		insure.Append( items[i].insured );
	endfor

// I think we start the while loop here...
	while( mobile.connected )
		var gump := GFLoadGump( ":gumps:gumps", "ItemInsuranceGumps", "ItemInsuranceMenu" ),
			reinsure := GetObjProperty( mobile, "AutoRenewInsurance" );
		if( gump.errortext )
			gump := GFCreateGump( 25, 50 );
			GFResizePic( gump, 0, 0, 9250, 526, 511 );
			GFPicTiled( gump, 13, 13, 500, 30, 3004 );
			GFPicTiled( gump, 13, 53, 500, 355, 3004 );
			GFPicTiled( gump, 13, 418, 500, 80, 3004 );

			GFAddHTMLLocalized( gump, 13, 18, 150, 20, 1114121, 0, 0, 0 ); // <CENTER>ITEM INSURANCE MENU</CENTER>
			GFAddHTMLLocalized( gump, 408, 18, 105, 20, 1114122, 0, 0, 0 ); // AUTO REINSURE
			GFAddHTMLLocalized( gump, 58, 67, 70, 20, 1062214, 0, 0, 0 ); // Item
			GFAddHTMLLocalized( gump, 263, 67, 70, 20, 1061038, 0, 0, 0 ); // Cost
			GFAddHTMLLocalized( gump, 413, 67, 70, 20, 1114311, 0, 0, 0 ); // Insured

			GFAddButton( gump, 18, 473, 4017, 4019, GF_CLOSE_BTN, CANCEL_BTN );
			GFAddHTMLLocalized( gump, 53, 475, 80, 20, 1011012, 0, 0, 0 ); // CANCEL

			GFAddButton( gump, 400, 473, 4005, 4007, GF_CLOSE_BTN, CONF_BTN );
			GFAddHTMLLocalized( gump, 435, 475, 50, 20, 1006044, 0, 0, 0 ); // OK

			GFAddHTMLLocalized( gump, 18, 420, 180, 20, 1114310, 0, 0, 0 ); // GOLD AVAILABLE:
			GFAddHTMLLocalized( gump, 18, 435, 300, 20, 1114123, 0, 0, 0 ); // TOTAL COST OF INSURANCE:

			GFStoreGump( ":gumps:gumps", "ItemInsuranceGumps", "ItemInsuranceMenu", gump );
		endif

		if( reinsure )
			GFAddButton( gump, 373, 13, 9723, 9724, GF_CLOSE_BTN, AUTO_REINSURE );
		else
			GFAddButton( gump, 373, 13, 9720, 9722, GF_CLOSE_BTN, AUTO_REINSURE );
		endif

		var balance := CalculateBankGold( mobile ),
			cost := 0;
		for( i:=1; i<=items.Size(); i++ )
			if( insure[i] )
				cost += CalculateInsCost( items[i] );
			endif
		endfor

// Bank balance...
		GFHTMLArea( gump, 228, 420, 150, 22, balance, 0, 0 );

// Total cost to insure all items...
		GFHTMLArea( gump, 228, 435, 150, 22, cost, 0, 0 );

		if( cost )
			GFAddHTMLLocalized( gump, 18, 450, 300, 20, 1114125, 0, 0, 0 ); // NUMBER OF DEATHS PAYABLE:
			GFHTMLArea( gump, 228, 450, 150, 22, CInt( balance / cost ), 0, 0 );
		endif

		var b:=0,
			y:=85;
		for( i:=page*4; i<(page+1)*4 && i<items.Size(); ++i )
			b := ItemBounds( items[i+1].graphic );
			GFAddImageTileButton( gump, 40, y, 2328, 2328, GF_DUMMY_BTN, 0, items[i+1].graphic, items[i+1].color, 40 - b.Width / 2 - b.X, 30 - b.Height / 2 - b.Y );
			GFItemProperty( gump, items[i+1].serial );

			if( insure[i+1] )
				GFAddButton( gump, 413, y, 9723, 9724, GF_CLOSE_BTN, 100+i );
				GFTextLine( gump, 263, y, 0x481, CalculateInsCost( items[i+1] ) );
			else
				GFAddButton( gump, 413, y, 9720, 9722, GF_CLOSE_BTN, 100+i );
				GFTextLine( gump, 263, y, 0x66C, CalculateInsCost( items[i+1] ) );
			endif

			y += 75;
		endfor

		if( page >= 1 )
			GFAddButton( gump, 18, 380, 4014, 4016, GF_CLOSE_BTN, PREV_PAGE );
			GFAddHTMLLocalized( gump, 53, 380, 70, 20, 1044044, 0, 0, 0 ); // PREV PAGE
		endif

		if( ( page + 1 ) * 4 < items.Size() )
			GFAddButton( gump, 400, 380, 4005, 4007, GF_CLOSE_BTN, NEXT_PAGE );
			GFAddHTMLLocalized( gump, 435, 380, 70, 20, 1044045, 0, 0, 0 ); // NEXT PAGE
		endif

		var result := GFSendGump( mobile, gump );
		if( !result[0] || result[0] == CANCEL_BTN )
			EraseObjProperty( mobile, "#ItemInsuranceGump" );
			return 0;
		endif

		case( result[0] )
			AUTO_REINSURE:	Set_Critical( 1 );
							reinsure := ( reinsure ? 0 : 1 );
							SetObjProperty( mobile, "AutoRenewInsurance", reinsure );
							Set_Critical( 0 );


							var items := EnumerateItemsInContainer( mobile.backpack )+ListEquippedItems( mobile );
							foreach item in ( items )
								if( !reinsure )
									EraseObjProperty( item, "Reinsure" );
								else
									if( item.insured )
										SetObjProperty( item, "Reinsure", 0x1 );
									endif
								endif
							endforeach

			CONF_BTN:	// we will pass items & insure arrays to function, within
						// this function we will run through all items in the items
						// array, if the position of the insure array is set to 1 but
						// the item is NOT insured already, we will charge the player
						// for that item's insurance cost.
						if( ItemInsuranceMenuConfirmGump( mobile ) )
							ToggleItemInsurance( mobile, items, insure );
						endif
						EraseObjProperty( mobile, "#ItemInsuranceGump" );

						return 1;

			PREV_PAGE:	if( page>=1 )
							page-=1;
						endif

			NEXT_PAGE:	if( ( page + 1 ) * 4 < items.Size() )
							page+=1;
						endif

			default:
						var indx := result[0]-99;
						if( indx >= 1 && indx <= items.Size() )
							insure[indx] := !insure[indx];
						endif
		endcase
	endwhile
endfunction

function ToggleItemInsurance( byref mobile, items, insure:=0 )

// We need to fetch the player's bank box...
	var i:=1;

// This is to create an array filled with 1's. This is
// mainly done to ensure that items passed in the ondeath
// auto reinsure stuff to correctly reinsure and charge the
// player for the insurance cost.
	if( TypeOfInt( insure ) != OT_ARRAY )
		insure := Array;
		for( i:=1; i<=items.Size(); i++ )
			if( GetObjProperty( items[i], "Reinsure" ) )
				insure.Append( 1 );
			endif
		endfor
	endif

	for( i:=1; i<=items.Size(); i++ )
		if( items[i].insured != insure[i] )
			if( insure[i] )
				var cost := CalculateInsCost( items[i] );
				if( ConsumeGoldFromBank( mobile, cost ) )
					SendSysMessageCL( mobile, 1060398, "\t"+cost, 0x3, 0x42 ); // ~1_AMOUNT~ gold has been withdrawn from your bank box.
				else
					SendSysMessageCL( mobile, 1061079, Array, 0x3, 0x22 );
					return 0;
				endif
			endif
			items[i].insured := insure[i];
			if( insure[i] && GetObjProperty( mobile, "AutoRenewInsurance" ) )
				SetObjProperty( items[i], "Reinsure", 0x1 );
			endif
		endif
	endfor
endfunction

function ItemInsuranceMenuConfirmGump( byref mobile )

	var gump := GFLoadGump( ":gumps:gumps", "ItemInsuranceGumps", "ConfirmInsurance" );
	if( gump.errortext )
		gump := GFCreateGump( 250, 200 );
		GFResizePic( gump, 0, 0, 9250, 254, 152 );
		GFPicTiled( gump, 13, 13, 228, 126, 3004 );
		GFAddHTMLLocalized( gump, 18, 18, 218, 80, 1114300, 1, 0, 0 ); // Do you wish to insure all newly selected items?

		GFAddButton( gump, 18, 116, 4017, 4019, GF_CLOSE_BTN, 33 );
		GFAddHTMLLocalized( gump, 53, 118, 80, 20, 1060051, 0, 0, 0 ); // CANCEL

		GFAddButton( gump, 126, 116, 4005, 4007, GF_CLOSE_BTN, 42 );
		GFAddHTMLLocalized( gump, 161, 118, 80, 20, 1073996, 0, 0, 0 ); // ACCEPT
		GFStoreGump( ":gumps:gumps", "ItemInsuranceGumps", "ConfirmInsurance", gump );
	endif

	var result := GFSendGump( mobile, gump );
	if( !result[0] || result[0] == 33 )
		SendSysMessageCL( mobile, 1042021, Array, 0x3, 0x32 ); // Cancelled
		return 0;
	endif

	return 1;
endfunction

function CalculateBankGold( byref mobile )

	var bankbox := CP_GetStorageContainerForMobile( mobile, CP_BANK_AREA, CP_CREATE );
	if( !bankbox )
		SendSysMessage( mobile, "You don't have a bank box.", 0x3, 0x22 );
		return error{ "errortext":="Could not find the player's bank box." };
	endif

	var gold := 0;
	foreach item in ( FindSubstance( bankbox, 0xEED, 1, 0, FINDSUBSTANCE_FIND_ALL ) )
		gold+=item.amount;
	endforeach

	return gold;
endfunction

function ConsumeGoldFromBank( byref mobile, amt )

	var bankbox := CP_GetStorageContainerForMobile( mobile, CP_BANK_AREA, CP_CREATE );
	if( !bankbox )
		SendSysMessage( mobile, "You don't have a bank box.", 0x3, 0x22 );
		return 0;
	endif

	return ConsumeSubstance( bankbox, 0xEED, CInt( amt ) );
endfunction

