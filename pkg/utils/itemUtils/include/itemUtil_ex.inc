use uo;
use os;
use polsys;

include "include/uo_extend";

// This is used to take the descriptor string name and returns the
// member string name...
var desc_props := Struct{
							"AR" := "ar",
							"Attribute" := "attribute",
							"Color" := "color",
							"Cursed" := "cursed",
							"Editable" := "editable",
							"Facing" := "facing",
							"Graphic" := "graphic",
							"Insured" := "insured",
							"Invisible" := "invisible",
							"Lockable" := "lockable",
							"Movable" := "movable",
							"Newbie" := "newbie",
							"NoDrop" := "no_drop",
							"NoDropException" := "no_drop_exception",
							"Quality" := "quality",
							"SaveOnExit" := "saveonexit",
							"Script" := "usescript",
							"VendorBuysFor" := "buyprice",
							"VendorSellsFor" := "sellprice"
						};

// This defines the obj class members which we may
// want to backup, these are only backed up IF the
// item's prop is either greater than 0 or is not
// an empty string.
var members := Dictionary{
							POLCLASS_ITEM -> { "defence_increase_cap_mod", "defence_increase_mod", "faster_casting_mod", "lower_mana_cost_mod", "lower_reagent_cost_mod", "spell_damage_increase_mod", "swing_speed_increase_mod", "luck_mod", "hit_chance_mod", "faster_cast_recovery_mod", "damage_cold_mod", "damage_energy_mod", "damage_fire_mod", "damage_physical_mod", "damage_poison_mod", "hp", "maxhp_mod", "resist_cold_cap_mod", "resist_cold_mod", "resist_energy_cap_mod", "resist_energy_mod", "resist_fire_cap_mod", "resist_fire_mod", "resist_physical_cap_mod", "resist_physical_mod", "resist_poison_cap_mod", "resist_poison_mod" },
							POLCLASS_LOCKABLE -> { "locked" },
							POLCLASS_CONTAINER -> { "max_items_mod", "max_slots_mod", "max_weight_mod", "no_drop_exception" },
							POLCLASS_MAP -> { "facetid", "gumpheight", "gumpwidth", "xeast", "xwest", "ynorth", "ysouth" },
							POLCLASS_ARMOR -> { "ar_mod" },
							POLCLASS_WEAPON -> { "dmg_mod", "speed_mod" }
						};


/*
 *
 * StoreItemsInfo( items, delete_item )
 *
 * Parameters:
 * items - Array of item obj refs
 * delete_item - if 1, destroys each item after it is stored.
 *
 * Purpose:
 * Stores all given items in a dictionary where objtype is the key
 * and the value is an array of structs where each struct defines
 * an individual item.
 *
 * Date: 2023-02-23 02:42:11
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
function StoreItemsInColdStorage( byref items, byref delete_item:=1 )

	var storage := Dictionary;
	foreach item in ( items )
		if( item.IsContainer() || item.IsQuiver() )
			continue;
		endif

		if( storage.Exists( item.objtype ) )
			storage[item.objtype].Append( ListItemProps( item ) );
		else
			storage.Insert( item.objtype, { ListItemProps( item ) } );
		endif

		if( delete_item )
			DestroyItem( item );
		endif
	endforeach

	return storage;
endfunction

/*
 *
 * ListItemProps( item )
 *
 * Parameters:
 * item - item to store in a struct.
 *
 * Purpose:
 * Purpose_Description
 *
 * Date: 2023-02-21 21:07:23
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * Struct
 *
 * Note:
 * This was a bit of a nightmare to write, it took many
 * iterations in my head and would have been so much easier
 * if there was a core function to return ALL member properties
 * an item could have based on their class.
 *
 */
function ListItemProps( byref item )

	var props := Struct{ "Amount":=item.amount, "CProps":=Dictionary, "Mods":=Dictionary },
		item_desc := GetItemDescriptor( item.objtype );

// Store itemdescriptor based stuff...
	foreach prop in ( item_desc.Keys() )
		if( !( prop in desc_props.Keys() ) )
			continue;
		elseif( item.Get_Member( desc_props[prop] ).errortext )
			continue;
		endif

		if( item.Get_Member( desc_props[prop] ) != item_desc[prop] )
			props.Insert( prop, item.Get_Member( desc_props[prop] ) );
		endif
	endforeach

// Fix for item name issues...
	if( item.name != item_desc["Desc"] )
		props.Insert( "Desc", item.name );
	endif

// Store CProps of Item...
	foreach name in ( GetObjPropertyNames( item ) )
		props.CProps.Insert( name, GetObjProperty( item, name ) );
	endforeach

// Store obj class specific props...
	var types := GetIsAType( item, ISA_GET_ALL ),
		i:=1;
	for( i:=1; i<types.Size(); i++ )
		if( types[i] in members.Keys() )
			foreach member in ( members[types[i]] )
				if( TypeOfInt( item.Get_Member( member ) ) == OT_LONG )
					if( item.Get_Member( member ) > 0 )
						props["Mods"].Insert( member, item.Get_Member( member ) );
					endif
				elseif( TypeOfInt( item.Get_Member( member ) ) == OT_DOUBLE )
					if( item.Get_Member( member ) > 0.0 )
						props["Mods"].Insert( member, item.Get_Member( member ) );
					endif
				elseif( TypeOfInt( item.Get_Member( member ) ) == OT_STRING )
					if( item.Get_Member( member ) != "" )
						props["Mods"].Insert( member, item.Get_Member( member ) );
					endif
				endif
			endforeach
		endif
	endfor

// Manage obj class specific methods...
	if( item.IsA( POLCLASS_SPELLBOOK ) )
		if( item.Spells().Size() )
			props.Insert( "Spells", item.Spells() );
		endif
	elseif( item.IsA( POLCLASS_MAP ) )
		if( item.GetPins().Size() )
			props.Insert( "Pins", item.GetPins() );
		endif
	endif

	return props;
endfunction

function RestoreItemsFromColdStorageInContainer( byref item_list, byref container )

	var desc := 0;
	foreach objtype in ( item_list.Keys() )
		foreach object in ( item_list[objtype] )

			desc := GetItemDescriptor( objtype );
			foreach key in ( object.Keys() )
				if( key in desc.Keys() )
					desc.Insert( key, object[key] );
				endif
			endforeach

			var item := CreateItemInContainer( container, desc, object.Amount );
			if( !item )
				return error{ "errortext":="Could not create item -> "+item.errortext };
			endif

			foreach mod in ( object.Mods.Keys() )
				item.Set_Member( mod, object.Mods[mod] );
			endforeach


			if( item.IsA( POLCLASS_SPELLBOOK ) )
				foreach id in ( object.Spells )
					item.AddSpell( id );
				endforeach
			elseif( item.IsA( POLCLASS_MAP ) )
				foreach pin in ( object.Pins )
					item.AppendPin( pin.x, pin.y );
				endforeach
			endif
		endforeach
	endforeach
endfunction

function RestoreItemsFromColdStorageAtLocation( byref item_list, byref location )

	if( TypeOfInt( location ) != OT_STRUCT )
		return error{ "errortext":="'location' parameter must be a struct containing X, Y, Z and Realm keys." };
	endif

	var desc := 0,
		new_list := Array;
	foreach objtype in ( item_list.Keys() )
		foreach object in ( item_list[objtype] )

			desc := GetItemDescriptor( objtype );
			foreach key in ( object.Keys() )
				if( key in desc.Keys() )
					desc.Insert( key, object[key] );
				endif
			endforeach
			desc.SaveOnExit := 0;
			desc.Movable := 1;

			var item := CreateItemAtLocation( location.X, location.Y, location.Z, desc, object.Amount, location.Realm );
			if( !item )
				return error{ "errortext":="Could not create item -> "+item.errortext };
			endif

			foreach mod in ( object.Mods.Keys() )
				item.Set_Member( mod, object.Mods[mod] );
			endforeach


			if( item.IsA( POLCLASS_SPELLBOOK ) )
				foreach id in ( object.Spells )
					item.AddSpell( id );
				endforeach
			elseif( item.IsA( POLCLASS_MAP ) )
				foreach pin in ( object.Pins )
					item.AppendPin( pin.x, pin.y );
				endforeach
			endif

			new_list.Append( item );
		endforeach
	endforeach

	return new_list;
endfunction
