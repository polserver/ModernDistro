use uo;
use os;

include ":gumps:gumps";
include ":gumps:gumpCaching";
include ":itemutils:itemdesc";
include ":itemutils:itemInfo";
include ":security:itemReport";

/*
 *
 * CreateGump( search_string, page, search_results, explicit_search )
 *
 * Parameters:
 * search_string - string to search item names for
 * page - page the player is on. (starts at 0)
 * search_results - Dictionary containing objtype as key and struct as key value.
 * explicit_search - Boolean, determines which error msg to display.
 *
 * Purpose:
 * Builds and returns a nice search gump for items defined in a shard's itemdesc file.
 *
 * Date: 2022-12-01 00:14:09
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * gump
 *
 * Notes:
 * Thanks to ServUO for the gump as it is, unabashedly a complete copy of
 * their [add gump.
 *
 */
function CreateGump( byref search_string, page, search_results, explicit_search )

	var datafile := GFOpenCacheDatafile( ":itemutils:gumps" );
	datafile.DeleteElement( "CreateGump" );
	var gump := GFLoadGump( datafile, "CreateGump", "BaseGump" );
	if( gump.errortext )
		gump := GFCreateGump( 50, 50 );
		GFDisposable( gump, 1 );

		GFPage( gump, 0 );
		GFResizePic( gump, 0, 0, 0x13BE, 420, 280 );
		GFPicTiled( gump, 10, 10, 400, 20, 0xA40 );
		GFAddAlphaRegion( gump, 10, 10, 400, 20 );
		GFPicTiled( gump, 41, 11, 184, 18, 0xBBC );
		GFPicTiled( gump, 42, 12, 182, 16, 0xA40 );
		GFAddAlphaRegion( gump, 42, 12, 182, 16 );

		GFAddButton( gump, 10, 9, 0xFAB, 0xFAD, GF_CLOSE_BTN, 2 );

		GFAddHTMLLocalized( gump, 230, 10, 100, 20, 3010005, 0, 0, 0x7FFF );

		GFPicTiled( gump, 10, 40, 400, 200, 0xA40 );
		GFAddAlphaRegion( gump, 10, 40, 400, 200 );
		GFStoreGump( datafile, "CreateGump", "BaseGump", gump );
	endif
	GFTextEntry( gump, 44, 10, 180, 20, 0x480, search_string, 3, 50 );

	var i:=1,
		index := ( 2+(page*10) );
	if( search_results.Size() > 0 )
		for( i := (page * 10); i < ((page + 1) * 10) && i < search_results.Size(); ++i )
			GFTextLine( gump, 44, 39+( ( i % 10 ) * 20 ), 0x480, search_results[search_results.Keys()[i+1]].Name );
			GFAddButton( gump, 10, 39+( ( i % 10 )*20 ), 0xFB7, 0xFB9, GF_CLOSE_BTN, 6+i );
		endfor
	else
		GFTextLine( gump, 15, 44, 0x480, explicit_search ? "Nothing matched your search terms." : "No results to display." );
	endif

	GFPicTiled( gump, 10, 250, 400, 20, 0xA40 );
	GFAddAlphaRegion( gump, 10, 250, 400, 20 );

	if( page > 0 )
		GFAddButton( gump, 10, 249, 0xFAE, 0xFB0, GF_CLOSE_BTN, 4 );
	else
		GFGumpPic( gump, 10, 249, 0xFAE );
	endif
	GFAddHTMLLocalized( gump, 44, 250, 170, 20, 1061028, 0, 0, page > 0 ? 0x7FFF : 0x5EF7 ); // Previous page

	if( ( ( page + 1 ) * 10 ) < search_results.Size() )
		GFAddButton( gump, 210, 249, 0xFA5, 0xFA7, GF_CLOSE_BTN, 5 );
	else
		GFGumpPic( gump, 210, 249, 0xFA5 );
	endif
	GFAddHTMLLocalized( gump, 244, 250, 170, 20, 1061027, 0, 0, ( ( page + 1 ) * 10 ) < search_results.Size() ? 0x7FFF : 0x5EF7 ); // Next page

	return gump;
endfunction

// Struct{ Objtype -> Struct{ "Desc":=item.desc, "Objtype":=item.objtype } }
/*
 *
 * BuildList( search_string, search_results )
 *
 * Parameters:
 * search_string - String which is used to find matching items from.
 * search_results - Empty Dictionary which if all goes will will contain
 *					objtype as key and struct as key value.
 *
 * Purpose:
 * Finds items that match the search string.
 *
 * Date: 2022-12-01 00:23:07
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
function BuildList( byref search_string, byref search_results )

	var itemdesc_cfg := GetItemDescCfgFile(),
		item_desc;
	foreach key in ( GetConfigIntKeys( itemdesc_cfg ) )
		item_desc := GetItemDescriptor( key );
		if( Find( Lower( item_desc.Name ), Lower( search_string ), 1 ) )
			search_results.Insert( key, GetItemInfo( key, item_desc ) );
		endif

		if( search_results.Size() >= 5000 )
			break;
		endif
		SleepMS( 1 );
	endforeach
endfunction

/*
 *
 * GetItemInfo( objtype, item_desc )
 *
 * Parameters:
 * objtype - integer
 * item_desc - Struct, either grabbed from GetItemDescriptor OR an empty struct.
 *
 * Purpose:
 * Fetches pertinent info from tiles.cfg and creates and returns a struct containing
 * Stackable, Name, Desc & Objtype
 *
 * Date: 2022-12-01 00:28:56
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Struct containing Stackable, Name, Desc & Objtype
 *
 */
function GetItemInfo( byref objtype, item_desc )

// Meet the kludge...If an objtype is sent that is not defined in
// an itemdesc file, we need to put this kludge in...I think...
	if( TypeOfInt( item_desc ) != OT_STRUCT )
		item_desc := Struct;
	endif

	var item_info := 0;
	if( item_desc.Graphic )
		item_info := IU_GetTileInfo( item_desc.graphic );
	else
		item_info := IU_GetTileInfo( objtype );
		if( !item_desc.Size() )
			item_desc := Struct{ "Desc":=item_info.Desc, "Name":=item_info.Desc };
		endif
	endif

	return Struct{ "Desc":=item_desc.Desc, "Name":=item_desc.Name, "Objtype":=objtype, "Stackable":=CInt( item_info.Stackable ) };
endfunction

/*
 *
 * CreateItemStacksAtLocation( mobile, objtype, amount, x, y, z, realm )
 *
 * Parameters:
 * mobile - mobile_ref
 * objtype - objtype to create
 * amount - amount to create, can exceed 60000
 * x - integer, x coordinate
 * y - integer, y coordinate
 * z - integer, z coordinate
 * realm - string, realm name
 *
 * Purpose:
 * Creates stacks of a given item based on the objtype passed at a given location.
 *
 * Date: 2022-12-01 00:36:10
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array of Structs which contain object serial and object desc for logging purposes.
 *
 */
function CreateItemStacksAtLocation( byref mobile, byref objtype, byref amount, byref x, byref y, byref z, byref realm )

	var created := 0,
		objs := Array;
	if( amount > 60000 )
		while( amount > 60000 )
			created := CreateItemAtLocation( x, y, z, objtype, 60000, realm );
			if( created.errortext )
				SendSysMessage( mobile, "Could not create item at location.", 0x3, 0x32 );
				return objs;
			endif
			objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
			amount -= 60000;
		endwhile
	endif

	if( amount > 0 )
		created := CreateItemAtLocation( x, y, z, objtype, amount, realm );
		if( created.errortext )
			SendSysMessage( mobile, "Could not create item at location.", 0x3, 0x32 );
			return objs;
		endif
		objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
	endif

	return objs;
endfunction

/*
 *
 * CreateItemStacksInContainer( mobile, objtype, amount, container )
 *
 * Parameters:
 * mobile - mobile_ref
 * objtype - objtype to create
 * amount - amount to create, can exceed 60000
 * container - container_ref to create items in
 *
 * Purpose:
 * Create stacks of an item in a given container.
 *
 * Date: 2022-12-01 00:54:35
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array of Structs which contain object serial and object desc for logging purposes.
 *
 */
function CreateItemStacksInContainer( byref mobile, byref objtype, byref amount, byref container )

	var created := 0,
		objs := Array;
	if( amount > 60000 )
		while( amount > 60000 )
			created := CreateItemInContainer( container, objtype, 60000 );
			if( created.errortext )
				SendSysMessage( mobile, "Your inventory is full.", 0x3, 0x32 );
				return objs;
			endif
			objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
			amount -= 60000;
		endwhile
	endif

	if( amount > 0 )
		created := CreateItemInContainer( container, objtype, amount );
		if( created.errortext )
			SendSysMessage( mobile, "Your inventory is full.", 0x3, 0x32 );
			return objs;
		endif
		objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
	endif

	return objs;
endfunction

/*
 *
 * CreateItemUnstackableAtLocation( mobile, objtype, amount, x, y, z, realm )
 *
 * Parameters:
 * mobile - mobile_ref
 * objtype - objtype to create
 * amount - amount to create
 * x - integer, x coordinate
 * y - integer, y coordinate
 * z - integer, z coordinate
 * realm - string, realm name
 *
 * Purpose:
 * Creates unstackable items at a given location.
 *
 * Date: 2022-12-01 01:09:49
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array of Structs which contain object serial and object desc for logging purposes.
 *
 */
function CreateItemUnstackableAtLocation( byref mobile, byref objtype, byref amount, byref x, byref y, byref z, byref realm )

	var i := 1,
		created := 0,
		objs := Array;
	for( i:=1; i<=amount; i+=1 )
		created := CreateItemAtLocation( x, y, z, objtype, 1, realm );
		if( created.errortext )
			SendSysMessage( mobile, "Could not create item at location.", 0x3, 0x32 );
			return objs;
		endif
		objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
	endfor

	return objs;
endfunction

/*
 *
 * CreateItemUnstackableInContainer( mobile, objtype, amount, container )
 *
 * Parameters:
 * mobile - mobile_ref
 * objtype - objtype to create
 * amount - amount to create
 * container - container_ref to create items in
 *
 * Purpose:
 * Creates many unstackable items in a given container.
 *
 * Date: 2022-12-01 01:05:31
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array of Structs which contain object serial and object desc for logging purposes.
 *
 */
function CreateItemUnstackableInContainer( byref mobile, byref objtype, byref amount, byref container )

	var i := 1,
		created := 0,
		objs := Array;
	for( i:=1; i<=amount; i+=1 )
		created := CreateItemInContainer( container, objtype, 1 );
		if( created.errortext )
			SendSysMessage( mobile, "Your inventory is full.", 0x3, 0x32 );
			return objs;
		endif
		objs.Append( Struct{ "Serial":=created.serial, "Desc":=created.Desc } );
	endfor

	return objs;
endfunction
