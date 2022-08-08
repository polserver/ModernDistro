use uo;
use os;
use datafile;

/*
 *
 * GFOpenCacheDatafile( path:=":gumps:gumps" )
 *
 * Parameters:
 * path - String name to a datafile.
 *
 * Purpose:
 * Retrieves a datafile object.
 *
 * Date: 2022-07-31 23:40:18
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * datafile_obj or error
 *
 */
function GFOpenCacheDatafile( path:=":gumps:gumps" )

	var datafile := OpenDataFile( path );
	if( datafile.errortext )
		datafile := CreateDataFile( path, DF_KEYTYPE_STRING );
		if( datafile.errortext )
			return error{ "errortext":="Could not find or create datafile. -> "+datafile.errortext };
		endif
	endif

	return datafile;
endfunction

/*
 *
 * GFFindCacheElem( elem_name, datafile )
 *
 * Parameters:
 * elem_name - string name to search for in gump datafile.
 * datafile - datafile_ref or string name to datafile path, default 0.
 *
 * Purpose:
 * Fetches a datafile elem from a given datafile.
 *
 * Date: 2022-07-31 23:36:01
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * df_elem_ref or error
 *
 */
function GFFindCacheElem( byref elem_name, byref datafile:=0 )

	if( TypeOfInt( datafile ) == OT_STRING )
		datafile := GFOpenCacheDatafile( datafile );
		if( datafile.errortext )
			return datafile.errortext;
		endif
	elseif( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := GFOpenCacheDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	var df_elem := datafile.FindElement( elem_name );
	if( df_elem.errortext )
		df_elem := datafile.CreateElement( elem_name );
		if( df_elem.errortext )
			return error{ "errortext":="Could not find or create. -> "+df_elem.errortext };
		endif
	endif

	return df_elem;
endfunction

/*
 *
 * GFStoreGump( datafile, elem_name, gump_name, data )
 *
 * Parameters:
 * datafile - datafile_ref or string name to datafile path, default 0.
 * elem_name - string name to search for in gump datafile.
 * gump_name - Name given to a gump which will be stored as a property of the retrieved datafile element.
 * data - Struct containing gump data.
 *
 * Purpose:
 * Stores a gump struct for later retrieval.
 *
 * Date: 2022-07-31 23:52:39
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Struct or error.
 *
 */
function GFStoreGump( datafile, byref elem_name, byref gump_name, byref data )

	var df_elem := GFFindCacheElem( elem_name, datafile );
	if( df_elem.errortext )
		return df_elem.errortext;
	endif

	return df_elem.SetProp( gump_name, data );
endfunction

/*
 *
 * GFLoadGump( datafile, elem_name, gump_name )
 *
 * Parameters:
 * datafile - datafile_ref or string name to datafile path, default 0.
 * elem_name - string name to search for in gump datafile.
 * gump_name - Name given to gump at time of storing.
 *
 * Purpose:
 * Returns a gump struct previously stored for the purpose of quick loading.
 *
 * Date: 2022-07-31 23:46:51
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Struct or error
 *
 */
function GFLoadGump( datafile, byref elem_name, byref gump_name )

	var df_elem := GFFindCacheElem( elem_name, datafile );
	if( df_elem.errortext )
		return df_elem.errortext;
	endif

	return df_elem.GetProp( gump_name );
endfunction

/*
 *
 * GFEraseGump( datafile, elem_name, gump_name )
 *
 * Parameters:
 * datafile - datafile_ref or string name to datafile path, default 0.
 * elem_name - string name to search for in gump datafile.
 * gump_name - Name given to gump at time of storing.
 *
 * Purpose:
 * Allows for erasing a previously stored individual gump.
 *
 * Date: 2022-07-31 23:55:49
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * 1 or error
 *
 */
function GFEraseGump( datafile, byref elem_name, byref gump_name )

	var df_elem := GFFindCacheElem( elem_name, datafile );
	if( df_elem.errortext )
		return df_elem.errortext;
	endif

	return df_elem.EraseProp( gump_name );
endfunction

/*
 *
 * GFListCachedGumps( datafile )
 *
 * Parameters:
 * datafile - datafile_ref or string name to datafile path, default ':gumps:gumps'.
 *
 * Purpose:
 * Returns a dictionary containing datafile elem names as
 * keys and all propnames for that given datafile elem stored
 * as an array. Useful for a clear cached gumps type function.
 *
 * Date: 2022-07-31 23:59:10
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Dictionary
 *
 */
function GFListCachedGumps( datafile:=":gumps:gumps" )

	var datafile := GFOpenCacheDatafile( datafile ),
		info := Dictionary;
	foreach key in ( datafile.Keys() )
		info.Insert( key, datafile.FindElement( key ).PropNames() );
	endforeach

	return info;
endfunction
