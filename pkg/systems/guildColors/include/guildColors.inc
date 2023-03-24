use uo;
use os;
use datafile;

include ":guildcolors:settings";

function GC_GetGuildColorDatafile()

	var datafile := OpenDataFile( ":guildColors:colorEntries" );
	if( !datafile )
		CreateDataFile( ":guildColors:colorEntries", DF_KEYTYPE_INTEGER );
		datafile := OpenDataFile( ":guildColors:colorEntries" );
	endif

	if( datafile )
		return datafile;
	else
		return error{"errortext":="Could not open ':guildColors:colorEntries' : "+datafile.errortext};
	endif
endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	GC_RetrieveAvailableGuildColors()
//
//	Purpose:	Returns an array of integers of guild colors that are not in use
//				by any guild, and thus are available for purchase.
//
//	Parameter:	None
//
//	Returns:	array::of integers
//
//	Created:	2023-03-07 03:26:14 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function GC_RetrieveAvailableGuildColors()

	var color_df := GC_GetGuildColorDatafile(),
		keys := color_df.Keys(),
		active_list := Array,
		df_elem := 0,
		df_prop := 0,
		i := 0;

	for( i:=1; i<=keys.Size(); i+=1 )
		df_elem := color_df.FindElement( keys[i] );

		df_prop := df_elem.GetProp( "Purchased" );
		if( !df_prop || df_prop.errortext )
			active_list.Append( keys[i] );
		endif
	endfor

	return active_list;
endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	GC_GuildColorsInUse()
//
//	Purpose:	Returns an array of integers of guild colors that are in use
//				by guilds, and thus are not available for purchase.
//
//	Parameter:	None
//
//	Returns:	array::of integers
//
//	Created:	2023-03-07 03:23:03 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function GC_GuildColorsInUse()

	var color_df := GC_GetGuildColorDatafile(),
		keys := color_df.Keys(),
		inactive_list := Array,
		i := 0;

	for( i:=1; i<=keys.Size(); i+=1 )
		if( color_df.FindElement( keys[i] ).GetProp( "Purchased" ) )
			inactive_list.Append( keys[i] );
		endif
	endfor

	return inactive_list;
endfunction

// Buyer id can be a string OR more than likely an integer.
// if string the buyer is a faction
// if integer, the buyer is a guild and the integer is the guild_id
function GC_PurchaseGuildColor( byref mobile, byref color, byref buyer_id )

	var datafile := GC_GetGuildColorDatafile(),
		df_elem := 0,
		df_prop := 0,
		settings_cfg := 0;

	if( datafile.errortext )
		return datafile.errortext;
	endif

	df_elem := datafile.FindElement( color );
	if( df_elem.errortext )
		return error{ "errortext":="Error finding color from 'GC_PurchaseGuildColor' in datafile -> "+df_elem.errortext };
	endif

	df_prop := df_elem.GetProp( "Purchased" );
	if( df_prop > 0 )
		return error{ "errortext":="This color is no longer available as it has been purchased previously." };
	endif

	if( !mobile.SpendGold( CInt( GC_GetSettingsCfgElem( "Prices" ).Cost ) ) )
		return error{ "errortext":="You do not have enough gold." };
	endif

	df_elem.SetProp( "Purchased", ReadGameClock() );
	df_elem.SetProp( "PurchasedBy", buyer_id );

	return 1;
endfunction

function MakeColorAvailableForPurchase( byref color )

	var datafile := GC_GetGuildColorDatafile(),
		df_elem := 0,
		df_prop := 0,
		settings_cfg := 0;

	if( datafile.errortext )
		return datafile.errortext;
	endif

	df_elem := datafile.FindElement( color );
	if( df_elem.errortext )
		return error{ "errortext":="Error finding color from 'GC_PurchaseGuildColor' in datafile -> "+df_elem.errortext };
	endif

	// df_elem.SetProp( "Purchased", ReadGameClock() );
	// df_elem.SetProp( "PurchasedBy", buyer_id );
	df_elem.eraseprop( "Purchased" );
	df_elem.eraseprop( "PurchasedBy" );

endfunction