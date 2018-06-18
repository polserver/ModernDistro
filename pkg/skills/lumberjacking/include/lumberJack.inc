use uo;
use os;
use util;
use cfgfile;
use file;

include ":datafile:datafile";
include ":lumberjacking:settings";

/*
 * LJ_IsTree(graphic)
 *
 * Purpose
 * Determines if the graphic can be used for lumberjacking.
 * Uses settings.cfg elem TreeList
 *
 * Parameters
 * graphic:	Graphic to verify
 *
 * Return value
 * Returns 1 if the object can be used.
 *
 */
function LJ_IsTree(graphic)
	var cfg_elem := LJ_GetSettingsCfgElem("TreeList");
	graphic := CStr(graphic);

	return ( graphic in GetConfigStringArray(cfg_elem, "Graphic") );
endfunction

/*
 * LJ_IsFurniture(object)
 *
 * Purpose
 * Determines if the object is furniture, and can thus
 * either be destroyed, or returned to deed form.
 *
 * Parameters
 * object:	Object to verify
 *
 * Return value
 * Returns 1 if the object can be destroyed.
 *
 */
function LJ_IsFurniture(object)
	return ( CInt(GetObjProperty(object, "furniture")) );
endfunction

/*
 * LJ_InRect(x_pos, y_pos, rect_string)
 *
 * Purpose
 * Determines if a coordinate is within an area.
 *
 * Parameters
 * x_pos:	X position of the coordinate to verify.
 * y_pos:	Y position of the coordinate to verify.
 * rect_string:	Rect line from WoodRegions.cfg
 *
 * Return value
 * Returns 1 if the coordinates are in the rect.
 *
 */
function LJ_InRect(x_pos, y_pos, rect)
	var x1 := CInt(rect[1]);
	var y1 := CInt(rect[2]);
	var x2 := CInt(rect[3]);
	var y2 := CInt(rect[4]);

	return ( (x_pos >= x1 && x_pos <= x2) && (y_pos >= y1 && y_pos <= y2) );
endfunction



/*
 * LJ_GetDataFile()
 *
 * Purpose
 * Retrieves the lumberjacking datafile
 *
 * Parameters
 *
 * Return value
 * data file
 *
 */
function LJ_GetDataFile()
	var data_file := DFOpenDataFile(":lumberjacking:TreeSettings", DF_CREATE);

	return data_file;
endfunction

/*
 * LJ_GetDataElem()
 *
 * Purpose
 * Retrieves an elem from the lumberjacking data file.
 *
 * Parameters
 * elem_name:	Name of the elem to retrieve.
 *
 * Return value
 * Data elem
 *
 */
function LJ_GetDataElem(elem_name)
	var data_file := LJ_GetDataFile();
	var data_elem := DFFindElement(data_file, elem_name, DF_CREATE);

	return data_elem;
endfunction

/*
 * LJ_GetWoodType(realm, x_pos, y_pos)
 *
 * Purpose
 * Retrieves the type of wood set for an X and Y coordinate.
 *
 * Parameters
 * realm:	Name of the realm the coordinates are on.
 * x_pos:	X coordinate to use.
 * y_pos:	Y coordinate to use.
 *
 * Return value
 * returns an object type number
 *
 */
function LJ_GetWoodType(realm, x_pos, y_pos)
	var pos_string := CStr(realm+" "+x_pos+"x"+y_pos);

	var data_elem := LJ_GetDataElem(pos_string);

	var type := data_elem.GetProp("Type");
	if ( !type )
		type := LJ_PickWoodType(realm, x_pos, y_pos);
		LJ_SetWoodType(realm, x_pos, y_pos, type);
	endif

	return type;
endfunction

/*
 * LJ_SetWoodType(realm, x_pos, y_pos, wood_type)
 *
 * Purpose
 * Sets the wood type for an X Y coordinate on a realm.
 * 
 * Parameters
 * realm:	Name of the realm to set the wood type on.
 * x_pos:	X coordinate to set the wood type on.
 * y_pos:	Y coordinate to set the wood type on.
 * wood_type:	object NAME of the wood to set.
 *
 * Return value
 * Returns 1
 *
 */
function LJ_SetWoodType(realm, x_pos, y_pos, wood_type)
	var pos_string := CStr(realm+" "+x_pos+"x"+y_pos);
	
	var data_elem := LJ_GetDataElem(pos_string);
	
	return data_elem.SetProp("Type", wood_type);
endfunction

/*
 * LJ_SetWoodAmount(realm, x_pos, y_pos, amount)
 *
 * Purpose
 * Sets the wood amount for an X Y coordinate on a realm.
 * 
 * Parameters
 * realm:	Name of the realm to set the wood amount on.
 * x_pos:	X coordinate to set the wood amount on.
 * y_pos:	Y coordinate to set the wood amount on.
 * amount:	Amount of wood to make available.
 *
 * Return value
 * Returns 1
 *
 */
function LJ_SetWoodAmount(realm, x_pos, y_pos, amount)
	var pos_string := CStr(realm+" "+x_pos+"x"+y_pos);
	
	var data_elem := LJ_GetDataElem(pos_string);
	
	data_elem.SetProp("Amount", CInt(amount));
endfunction

/*
 * LJ_GetWoodAmount(realm, x_pos, y_pos)
 *
 * Purpose
 * Retrieves the amount of wood set for an X and Y coordinate.
 *
 * Parameters
 * realm:	Name of the realm the coordinates are on.
 * x_pos:	X coordinate to use.
 * y_pos:	Y coordinate to use.
 *
 * Return value
 * returns a number
 *
 */
function LJ_GetWoodAmount(realm, x_pos, y_pos)
	var pos_string := CStr(realm+" "+x_pos+"x"+y_pos);
	
	var data_elem := LJ_GetDataElem(pos_string);
	
	var amount := data_elem.GetProp("Amount");
	if ( amount == error )
		var settings := LJ_GetSettingsCfgElem("Settings");
		amount := CInt(settings.MaxWood);
		LJ_SetWoodAmount(realm, x_pos, y_pos, amount);
	endif
	
	return CInt(amount);
endfunction

/*
 * LJ_PickWoodType(realm, x_pos, y_pos)
 *
 * Purpose
 * Selects a random wood type for a tree based on
 * the realm and region coordinates in woodRegions.cfg
 *
 * Parameters
 * realm:	Name of the realm the coordinates are on.
 * x_pos:	X coordinate to use.
 * y_pos:	Y coordinate to use.
 *
 * Return value
 * returns an object type number
 *
 */
function LJ_PickWoodType(realm, x_pos, y_pos)
	var wood_cfg := ReadConfigFile(":lumberjacking:wood-"+realm);
	if ( !wood_cfg )
		return GetObjTypeByName("Log");
	endif

	var wood_found := array{"Log"};
	foreach region_name in ( GetConfigStringKeys(wood_cfg) )
		var region_elem := wood_cfg[region_name];
		if ( !LJ_InRect(x_pos, y_pos, region_elem.Rect) )
			continue;
		endif

		var wood_types := GetConfigStringArray(region_elem, "WoodType");
		if ( wood_types.Size() >= 1 )
			wood_found := wood_found+wood_types;
		endif
		SleepMS(2);
	endforeach

	var selected_wood := wood_found[RandomInt(wood_found.Size())+1];
	selected_wood := GetObjTypeByName(selected_wood);
	if ( !selected_wood )
		return GetObjTypeByName("Log");
	endif
	
endfunction

function ChopFurniture(who, furniture)
	if ( !ReserveItem(furniture) )
		SendSysMessage(who, "That is already in use.");
		return 0;
	endif

	var sign;
	if ( GetObjProperty(furniture, "lockeddown") || GetObjProperty(furniture, "secure") )
		SendSysMessage(who, "You cannot destroy secure or locked down items.");
		return 0;
	endif
	if ( furniture.objtype == 0x7007 )
		var houseserial := GetObjProperty(furniture, "houseserial");
		sign := SystemFindObjectBySerial(houseserial);
		if ( GetObjProperty(sign, "barrelserial") == furniture.serial )
			EraseObjProperty(sign, "barrelserial");
			DestroyItem(furniture);
			return 1;
		endif
	endif

	var owner_serial := GetObjProperty(furniture , "DeedOwnerSerial");
	if( !owner_serial )
		return 0;
	endif
	if ( (who.serial != owner_serial) )
		if ( !who.multi || (GetObjProperty(who.multi, "ownerserial") != who.serial) )
			SendSysMessage(who, "You can only re-deed this decoration if you are the house owner or originally placed the decoration.");
			return 0;
		endif
	endif

	// FIXME: Need to upgrade this section for 097 Trap system!!!!!!
/*
	if(furniture.usescript == ":tinkering:tinkerTraps")
		var traptype := GetObjProperty(furniture,"trap_type");
		var trapstrength := GetObjProperty(furniture,"trap_strength");
		var trapperserial := CInt(GetObjProperty(furniture, "trapper"));
		var trapper := SystemFindObjectBySerial(trapperserial, SYSFIND_SEARCH_OFFLINE_MOBILES);
		if(trapper)
			SetObjProperty(who, "LastHit", {trapper.name, trapper.serial, "trapped chest" });
			SetScriptController(trapper);
		endif
		case traptype
			"1" :   PlaySoundEffect(who, SFX_224);
				var dmg := (RandomInt(20) + CInt(trapstrength));
				SendSysMessage(who, "You set off a needle trap!");
				ApplyDamage(who, dmg);
				furniture.usescript := "";
				EraseObjProperty( furniture, "trap_type" );
				EraseObjProperty( furniture, "trap_strength" );

			"2" :   start_script(":traps:poisonTrapTriggered", {who, furniture, "You set off a poison trap!", trapstrength});

			"3" :   PlaySoundEffect(who, SFX_208);
				SendSysMessage(who, "You set off an explosion trap!");
				PlayObjectCenteredEffect( who, FX_EXPLODE_3, 10,10);
				var dmg := (RandomInt(20) + CInt(trapstrength));
				ApplyRawDamage(who, dmg);
				furniture.usescript := "";
				EraseObjProperty( furniture, "trap_type" );
				EraseObjProperty( furniture, "trap_strength" );
		endcase
	endif
*/

	var the_serials  := GetObjProperty( furniture , "ItemsCreatedSerials" );
	var deed_objtype := GetObjProperty( furniture , "FromDeed" );
	if( !the_serials or !deed_objtype )
		return 0;
	endif
	
	var created := {};
	foreach serial in the_serials
		// We search both flags, in case a key exists for the furniture,
		// and needs to be found and destroyed!
		var part := SystemFindObjectBySerial( serial, SYSFIND_SEARCH_OFFLINE_MOBILES );
		if( part )
			created.append( part );
		endif
	endforeach
	if( !len(created) )
		return 0;
	endif
	
	var item_name, the_color;
	foreach item in created
		if( item.objtype != 0x100E )
			if( !item_name || item_name.errortext )
				item_name := GetObjProperty( item , "BaseName" );
			endif
			if( !the_color || the_color.errortext )
				the_color := GetObjProperty( item , "DeedColor" );
			endif
		endif
		if ( item.isa(POLCLASS_CONTAINER) )
			foreach thing in EnumerateItemsInContainer(item, ENUMERATE_ROOT_ONLY)
				if ( item.container )
					MoveItemToContainer(thing, item.container);
				else
					MoveObjectToLocation(thing, item.x, item.y, item.z, item.realm, MOVEOBJECT_FORCELOCATION);
				endif
			endforeach
		endif
		DestroyItem( item );
	endforeach

	var the_deed := CreateItemInBackpack( who , deed_objtype );
	if( the_color && !the_color.errortext )
		the_deed.color := the_color;
	endif
	if( item_name && !item_name.errortext )
		SetName( the_deed , item_name );
	endif
	
	PlaySoundEffect(who, 0x13a);
	return 1;
endfunction
