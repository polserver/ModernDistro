use uo;
use os;
use math;
use cfgfile;
use datafile;

include ":resourcecore:settings";

/*
 *
 * RC_OpenWoodDatafile()
 *
 * Parameters:
 * none
 *
 * Purpose:
 * Opens the wood datafile
 *
 * Date: 2020-09-20	17:31:49
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * datafile_ref or error
 *
 */
 function RC_OpenWoodDatafile()

	var datafile := OpenDataFile( ":resourcecore:wood" );
	if( !datafile )
		datafile := CreateDataFile( ":resourcecore:wood", DF_KEYTYPE_STRING );
		if( !datafile )
			return error{"errortext":="Could not open ':resourcecore:wood' : "+datafile.errortext};
		endif
	endif

	return datafile;
endfunction

/*
 *
 * RC_FindWoodDataElem( byref elem_name, byref datafile := 0 )
 *
 * Parameters:
 * elem_name - string name of an element in datafile
 * datafile - datafile or 0, function will open the correct datafile if this is not present.
 *
 * Purpose:
 * Make finding a data elem easier. Will either find the data elem or create one.
 *
 * Date: 2020-09-20	17:31:49
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * data_elem_ref or error
 *
 */
function RC_FindWoodDataElem( byref elem_name, byref datafile := 0 )

	var df_elem := 0;
	if( TypeOfInt( datafile ) != OT_DATAFILEREF )
		datafile := RC_OpenWoodDatafile();
		if( datafile.errortext )
			return datafile.errortext;
		endif
	endif

	df_elem := datafile.FindElement( elem_name );
	if( TypeOfInt( df_elem ) != OT_DATAELEMREF )
		df_elem := datafile.CreateElement( elem_name );
		if( df_elem.errortext )
			return error{"errortext":="Unable to create datafile element "+df_elem.errortext};
		endif
	endif

	return df_elem;
endfunction

function RC_GetWoodProps( byref data_elem, byref prop_name )

	var df_prop := data_elem.GetProp( prop_name ),
		settings := 0;

	if( df_prop.errortext )
		settings := RC_GetSettingsCfgElem( "WoodSettings" );
	endif
       
        if( df_prop.errortext )
		settings := RC_GetSettingsCfgElem( "FruitSettings" );
		case( prop_name )
			"Harvest": return 0;
			"Quantity": return CInt( settings.MaxYield );
		endcase
	endif


	return df_prop;
endfunction

function RC_GetTreeCfgFile()

	var cfg := ReadConfigFile( ":resourcecore:trees" );
	if( cfg.errortext )
		return error{ "errortext":="Error::RC_GetTreeCfgFile() - Unable to open [:resourcecore:trees.cfg] ->"+cfg.errortext };
	endif

	return cfg;
endfunction

function RC_GetTreesCfgElem( byref elem_name, byref cfg_file:=0 )

	if( TypeOfInt( cfg_file ) != OT_CONFIGFILEREF )
		cfg_file := RC_GetTreeCfgFile();
		if( cfg_file.errortext )
			return cfg_file.errortext;
		endif
	endif

	var cfg_elem := FindConfigElem( cfg_file, elem_name );
	if( cfg_elem.errortext )
		return error{ "errortext":="Error::RC_GetTreesCfgElem() - Unable to find elem ["+elem_name+"] ->"+cfg_elem.errortext };
	endif

	return cfg_elem;
endfunction

function RC_IsTree( byref objtype )

	return ( objtype in GetConfigIntKeys( RC_GetTreeCfgFile() ) );
endfunction

// This was: ValidateTargetCoords
function RC_LumberjackValidateTarget( targ, who )

	if( targ.item.serial || targ.mobile.serial )
		return 1;
	endif

	if( !targ.objtype )
		return 1;
	endif

	foreach item in ListStaticsAtLocation( targ.x, targ.y, targ.z, LIST_IGNORE_Z, who.realm );
		if( item.objtype == targ.objtype )
			return 1;
		endif
	endforeach

	Broadcast( who.name+" has sent invalid objtype target data! Check their actions.", 0x3, 0x32, 4 );

	return 0;
endfunction

function RC_GetFruitProps( byref data_elem, byref prop_name )

	var df_prop := data_elem.GetProp( prop_name ),
		settings := 0;

	if( df_prop.errortext )
		settings := RC_GetSettingsCfgElem( "FruitSettings" );
		case( prop_name )
			"FruitHarvest": return 0;
			"FruitQuantity": return CInt( settings.MaxYield );
		endcase
	endif

	return df_prop;
endfunction

function RC_HarvestFruit( byref mobile, byref use_on )

	var data_elem := RC_FindWoodDataElem( Cstr( use_on.x+"_"+use_on.y+"_"+use_on.z+"_"+use_on.realm ), 0 ),
		last_harvest := RC_GetFruitProps( data_elem, "FruitHarvest" ),
		quantity :=  RC_GetFruitProps( data_elem, "FruitQuantity" ),
		settings := RC_GetSettingsCfgElem( "FruitSettings" ),
		cfg_file := RC_GetTreesCfgElem( use_on.objtype ),
		create_item := 0;

	quantity += Floor( Cint( ReadGameClock() - last_harvest ) / CInt( settings.SecondsPerRegrow ) );
	if( quantity > CInt( settings.MaxYield ) )
		quantity := CInt( settings.MaxYield );
	endif

	if( quantity < 1 )
		SendSysMessage( mobile, "There's not enough fruit here to harvest.", 0x3, 0x22 );
		return 0;
	endif

	if( CoordinateDistance( mobile.x, mobile.y, use_on.x, use_on.y ) > 2 )
		SendSysMessage( mobile, "You must be nearer the tree to harvest fruit!", 0x3, 0x22 );
		return 0;
	endif

	while( mobile.connected )
		PerformAction( mobile, 33 );
		PlaySoundEffectXYZ( use_on.x, use_on.y, use_on.z, 0x59, use_on.realm );

		SleepMS( 1800 );

		create_item := CreateItemInBackpack( mobile, cfg_file.FruitType, 1 );
		if( create_item.errortext )
			SendSysMessage( mobile, "You can't place any fruit into your backpack!", 0x3, 0x22 );
			return 0;
		endif

		SendSysMessage( mobile, "You place the fruit into your backpack.", 0x3, 0x42 );
		quantity -= 1;
		last_harvest := ReadGameClock();

		data_elem.SetProp( "FruitHarvest", last_harvest );
		data_elem.SetProp( "FruitQuantity", quantity );

		if( quantity < 1 )
			SendSysMessage( mobile, "There's not enough fruit here to harvest.", 0x3, 0x22 );
			return 0;
		elseif( CoordinateDistance( mobile.x, mobile.y, use_on.x, use_on.y ) > 2 )
			SendSysMessage( mobile, "You must be nearer the tree to harvest fruit!", 0x3, 0x22 );
			return 0;
		endif

		if( mobile.dead )
			return 0;
		endif
	endwhile

	return 1;
endfunction
