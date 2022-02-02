use uo;
use os;
use file;
use cfgfile;
use polsys;

/*
 *
 * OpenDecorationCfgfile( file_name )
 *
 * Parameters:
 * file_name - string pkg formatted name of a decoration file.
 *
 * Purpose:
 * Return the config file of the string name given.
 *
 * Date: 2022-01-23	23:07:34
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * cfg_file ref
 *
 */
function OpenDecorationCfgfile( file_name )

	var cfg_file := ReadConfigFile( file_name );
	if( cfg_file.errortext )
		return error{ "errortext":="Unable to open '"+file_name+"' -> "+cfg_file.errortext };
	endif

	return cfg_file;
endfunction

/*
 *
 * ListDecorationCfgfileContents( cfg_file )
 *
 * Parameters:
 * cfg_file - config_file ref
 *
 * Purpose:
 * Returns all element keys found within the config file sent.
 *
 * Date: 2022-01-23	23:09:07
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array
 *
 */
function ListDecorationCfgfileContents( cfg_file )

	if( TypeOfInt( cfg_file ) == OT_STRING )
		cfg_file := OpenDecorationCfgfile( cfg_file );
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	return GetConfigIntKeys( cfg_file );
endfunction

/*
 *
 * ListDecorationConfigFiles( directory )
 *
 * Parameters:
 * directory - string pkg formatted directory
 *
 * Purpose:
 * Lists all config files within the directory given.
 *
 * Date: 2022-01-23	23:10:06
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Array of strings containing cfgfile names.
 *
 */
function ListDecorationConfigFiles( byref directory )

	var files := ListDirectory( directory, "cfg" );
	var temp := {};
	if( files.errortext )
		return error{"errortext":="Could not find files in '"+directory+"' -> "+files.errortext};
	endif

	// Need to strip off '.cfg' from each file name.
	foreach cfg_file in files
		temp.append(StrReplace(cfg_file, ".cfg", ""));
	endforeach
	return temp;

endfunction

/*
 *
 * CreateItemForDecoration( file_name, key, cfg_elem )
 *
 * Parameters:
 * file_name - string file name the item is being created from. (Used to store on item in a struct)
 * key - key of the item that needs to be created. (Used to store on item in a struct)
 * cfg_elem - cfg_elem_ref of item to create.
 *
 * Purpose:
 * To create an item from one of the decoration files.
 *
 * Date: 2022-01-23	23:11:28
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
function CreateItemForDecoration( file_name, key, cfg_elem )

	var desc := GetItemDescriptor( cfg_elem.Objtype ),
		item := 0;

	if( desc.errortext )
		item := CreateItemAtLocation( cfg_elem.X,  cfg_elem.Y, cfg_elem.Z, cfg_elem.Objtype, cfg_elem.Amount, cfg_elem.Realm );
		item.color := cfg_elem.Color;
		item.movable := 0;
		// Place the 'Static' CProp on the item. Now only the udestry commands can delete it.
		SetObjProperty( item, "Static", 0x1 );
		SetObjProperty( item, "DecorationInfo", Struct{ "FileName":=file_name, "Key":=key } );
	else
		desc.Color := cfg_elem.Color;
		desc.Movable := 0;
		// Place the 'Static' CProp on the item. Now only the udestry commands can delete it.
		desc.CProps.Insert( "Static", 0x1 );
		desc.CProps.Insert( "DecorationInfo", Struct{ "FileName":=file_name, "Key":=key } );
		item := CreateItemAtLocation( cfg_elem.X,  cfg_elem.Y, cfg_elem.Z, desc, cfg_elem.Amount, cfg_elem.Realm );
	endif

	if( cfg_elem.Name != "" )
		SetName( item, cfg_elem.Name );
	endif
endfunction

/*
 *
 * RemoveDecorationFromFacet( cfg_elem )
 *
 * Parameters:
 * file_name - string file name the item is being created from. (Used to store on item in a struct)
 * key - key of the item that needs to be created. (Used to store on item in a struct)
 * cfg_elem - cfg_elem_ref of item to create.
 *
 * Purpose:
 * Deletes a specific item at a specific location referenced in the cfg_elem.
 *
 * Date: 2022-02-01
 *
 * Author: Yukiko
 *
 * Return Values:
 * none
 *
 */
function RemoveDecorationFromFacet( cfg_elem )

	var	things := ListItemsAtLocation( cfg_elem.x, cfg_elem.y, cfg_elem.z, cfg_elem.realm );

	foreach thing in things
		if( thing.objtype == cfg_elem.objtype )
			DestroyItem( thing );
			break;
		endif
	endforeach

endfunction
