// Some comments added by Yukiko - denoted by capital 'Y'
use cfgfile;

include ":attributes:attributes";
include ":containers:storageAreas";
include ":itemutils:itemdesc";
include ":itemutils:itemInfo";
include "include/utility";

var mrcspawn_cfg := ReadConfigFile("::mrcspawn");
var itemconfig := ReadConfigFile(":*:itemdesc");
var storage := CP_GetStorageArea( CP_MERCHANT_AREA, CP_CREATE );

function FindMerchantType( mrctype )

	var npcdesccfg	:= ReadConfigFile(":*:npcdesc");
	var elem := FindConfigElem(npcdesccfg, Lower(mrctype));
	var retval := 0;
	if(!GetConfigStringArray(elem, "CProp"))
	    Print("Restock error " + GetConfigStringArray(elem, "CProp").errortext + "  reading " + mrctype);
	endif
	foreach thing in GetConfigStringArray(elem, "CProp")
		var split := SplitWords(thing);
		if(split[1] == "MerchantType")
			retval := split[2] - split[2][1];
			break;
		endif
	endforeach
	return retval;
endfunction

function Create_Node_Merc( merchanttype, name )

	var shop_sell := CP_GetStorageAreaContainer( name+" ShopSell", CP_MERCHANT_AREA, CP_CREATE );
	var shop_resale := CP_GetStorageAreaContainer( name+" ShopeResale", CP_MERCHANT_AREA, CP_CREATE );
	var shop_buy := CP_GetStorageAreaContainer( name+" ShopBuy", CP_MERCHANT_AREA, CP_CREATE );

	SetObjProperty( shop_resale, "MerchantType", merchanttype );
	SetObjProperty( shop_resale, "MyGold", 5000 );
	StockInventory( merchanttype, shop_sell );
	RestockInitialInventory( shop_sell, shop_resale );
endfunction

function Destroy_Node_Merc(name)

	CP_RemoveStorageAreaContainer( name+" ShopSell", CP_MERCHANT_AREA );
	CP_RemoveStorageAreaContainer( name+" ShopeResale", CP_MERCHANT_AREA );
	CP_RemoveStorageAreaContainer( name+" ShopBuy", CP_MERCHANT_AREA );
endfunction

function RestockInventory( merchanttype, name )

	var container := CP_GetStorageAreaContainer( name+" ShopSell", CP_MERCHANT_AREA, CP_CREATE );
	foreach item in ( EnumerateItemsInContainer( container, ENUMERATE_ROOT_ONLY ) )
		DestroyItem( item );
		SleepMS( 5 );
	endforeach

	var mtype := FindConfigElem( mrcspawn_cfg, merchanttype );
	if(mtype)
		foreach productgroup in GetConfigStringArray(mtype, "Sells");
			var pg_elem := FindConfigElem(mrcspawn_cfg, productgroup);
			foreach itemstr in GetConfigStringArray(pg_elem, "Item")
				SpawnItem( container, itemstr );
				sleepms(10);
			endforeach
		endforeach
	endif
endfunction

function StockInventory( merchanttype, inventory )

	var mtype := FindConfigElem(mrcspawn_cfg, merchanttype);
	if(mtype)
		foreach productgroup in GetConfigStringArray(mtype, "Sells");
			var pg_elem := FindConfigElem(mrcspawn_cfg, productgroup);
			foreach itemstr in GetConfigStringArray(pg_elem, "Item")
				SpawnItem(inventory, itemstr);
				sleepms(10);
			endforeach
		endforeach
	endif
endfunction

function SpawnItem(byref inventory, itemstr)

	var entry := SplitWords( itemstr );
	var objtype := entry[1];
	if( !CInt( objtype ) && !GetObjtypeByName( objtype ) )
		return 0;
	endif

	var count := CInt( entry[2] ) <= 0 ? 1 : CInt( entry[2] );
	CreateItemInInventory( inventory, objtype, count );
endfunction

function CheckStock(rs, pak)
// purpose: to remove items that the vendor does not carry, but have been sold to it.
// and replenish the purchasing gold supply for the vendor.
	var mygold := Cint(GetObjProperty(rs, "MyGold"));
	foreach thing in EnumerateItemsInContainer(pak)
		var crs := Cint(GetObjProperty(thing, "ClearRestock"));
		var cst;
		if(crs)
			if(ReadGameClock() > CInt(crs))
				cst := CInt(itemconfig[thing.objtype].VendorBuysFor) + 5;
				if(!cst)
					cst := 1;
				endif
				mygold := CInt(mygold + (cst * thing.amount));
				DestroyItem(thing);
			endif
		endif
	endforeach
	SetObjProperty(rs, "MyGold", mygold);
endfunction

function RestockInitialInventory(source, destination)
// When vendors start up, they need a slightly different script.
	var merchanttype := GetObjProperty(source, "MerchantType");
	merchanttype := merchanttype;
	var multiplier, amount_of_things, mlt, things;
	var newarray := Array;
	foreach thing in EnumerateItemsInContainer(source)
		newarray.append(thing.objtype);
		amount_of_things := 0;
		multiplier := CInt(GetObjProperty(thing, "Multiplier"));
		var check := CountExisting(destination, thing.objtype);
		things := check[2];
		amount_of_things := check[1];
		if(amount_of_things >= (thing.amount * multiplier))
			multiplier := multiplier - 1;
			if(multiplier < 1)
				multiplier := 1;
			endif
		elseif(amount_of_things == 0)
			multiplier := multiplier + 1;
			if(multiplier > 20)
				multiplier := 20;
			endif
		endif
		SetObjProperty(thing, "Multiplier", multiplier);
		mlt := Cint(thing.amount * multiplier);
		var buyprice := Cint(thing.buyprice);
		var sellprice := Cint(thing.sellprice);
		var res;
		if(amount_of_things != mlt)
			foreach widget in things
				DestroyItem(widget);
			endforeach
			if(amount_of_things > mlt)
				res := CreateItemInInventory(destination, thing.objtype, mlt);
				if(res)
					res.sellprice := sellprice;
					res.buyprice := buyprice;
				endif
			elseif(amount_of_things < mlt)
				var i;
				for(i:=1; i<= mlt; i := i+1)
					var res := CreateItemInInventory(destination, thing.objtype, 1);
					res.sellprice := sellprice;
					res.buyprice := buyprice;
					sleepms(1);
				endfor
			endif
		endif
		sleepms(1);
	endforeach
	foreach thing in EnumerateItemsInContainer(destination)
		if((!(thing.objtype in newarray)) && (!GetObjProperty(thing, "ClearRestock")))
			DestroyItem(thing);
		endif
	endforeach
endfunction

function RestockMyInventory(source, destination)
	var merchanttype := GetObjProperty(source, "MerchantType");
	var multiplier, amount_of_things, mlt, items, mygold;
	var newarray := {}; // Array of what's for sale
	// cycle through everything that's for sale
	foreach thing in EnumerateItemsInContainer(source)// this is the "ideal" stock.
		newarray.append(thing.objtype);
		amount_of_things := 0;
		mygold := GetObjProperty(source, "MyGold");
		multiplier := CInt(GetObjProperty(thing, "Multiplier")); // multiplier of increments.
		var check := CountExisting(destination, thing.objtype); // to see what we got *really* for sale
		items := check[2];// returns all of a particular item, stacked or unstacked. (array)
		amount_of_things := check[1];

		// determine whether to increase or decrease stock.
		if(amount_of_things >= (thing.amount * multiplier))
			multiplier := multiplier - 1;
			if(multiplier < 1)
				multiplier := 1;
			endif
		elseif(amount_of_things == 0)
			multiplier := multiplier + 1;
			if(multiplier > 20)
				multiplier := 20;
			endif
		endif
		SetObjProperty(thing, "Multiplier", multiplier);

		mlt := Cint(thing.amount * multiplier); // mlt is the amount that should be in stock

		if(amount_of_things != mlt)
			// define characteristics of the items
			var buyprice := Cint(thing.buyprice);
			var sellprice := Cint(thing.sellprice);
			// var res;

			// remove what remains
			foreach widget in items
				mygold := mygold + Cint(widget.amount * buyprice);
				DestroyItem(widget);
			endforeach
			var i;
			// make one of everything you need
			for(i:=1; i<= mlt; i := i+1)
				if((mygold > buyprice) || (merchanttype == "Architect"))
					var res := CreateItemInInventory(destination, thing.objtype, 1);
					res.sellprice := sellprice;
					res.buyprice := buyprice;
					if(merchanttype != "Architect")
						mygold := mygold - buyprice;
					endif
				sleepms(1);
				endif
			endfor
		endif
		SetObjProperty(source, "MyGold", mygold);
		sleepms(1);
	endforeach

	// ditch the leftovers
	foreach thing in EnumerateItemsInContainer(destination)
		if((!(thing.objtype in newarray)) && (!GetObjProperty(thing, "ClearRestock")))
			mygold := mygold + Cint(thing.amount * thing.buyprice);
			DestroyItem(thing);
		endif
	endforeach
	SetObjProperty(source, "MyGold", mygold);
endfunction

function CountExisting(container, objtype)
	var amount_of_things := 0;
	var things := {};
	foreach thing in EnumerateItemsInContainer(container)
		if((thing.objtype == objtype) && (Cint(GetObjProperty(thing, "ClearRestock")) == 0))
			things.append(thing);
			amount_of_things := amount_of_things + thing.amount;
		endif
	endforeach
	var ary := {};
	ary.append(amount_of_things);
	ary.append(things);
	return ary;
endfunction

function ModifyPCSellList(merchanttype, customer)
	var backpackitems := EnumerateItemsInContainer(customer.backpack, ENUMERATE_ROOT_ONLY);
	var prodgroups := FindConfigElem(mrcspawn_cfg, merchanttype);
	var buys := GetConfigStringArray(prodgroups, "Buys");
	var normcost, normsell, percentage;

	//  If not buying lets call the whole thing off.
	if(buys.size() < 1)
		return 0;
	endif

	var itemmark := -1;

	// set buy price for each item to be 0 to start. This means it won't be sellable.
        // Commented out by Yukiko - This section prevented us from setting custom buy prices for items
/*
	var iteminit := 0;

	foreach item in backpackitems
		item.buyprice := iteminit;
	endforeach
*/
	var buynobuyable := { };
	var objtype;
	var counter := 0;

	// Populate the array of items the vendor will buy.
	foreach prodgroup in buys
		foreach itemstr in GetConfigStringArray(FindConfigElem(mrcspawn_cfg, prodgroup), "Item")
			itemstr := SplitWords(itemstr);
			objtype := itemstr[1];
			if(CInt(objtype))
				objtype := CInt(objtype);
			else
				objtype := GetObjtypeByName(objtype);
			endif
			buynobuyable.append(objtype);
		endforeach
	endforeach

	// compare all the items in the top level of the backpack to things the vendor can buy and set the price.
	if(buynobuyable.size()) // Y - If the vendor has anything in her list of buyables
            foreach item in backpackitems
                    if((item.objtype in buynobuyable) && (!item.newbie))
                            counter := counter + 1;
                            // Y - Only equipment has quality so only equipment gets any added value for it.
                            if(!item.quality)
                                // Y - If this is not an equippable item it has no member named 'quality'
                                // Y - You cannot set a value to a non-existent member.
                                item.quality := 1;
                            endif
                            normcost := itemconfig[item.objtype].VendorBuysFor;
                            normsell := itemconfig[item.objtype].VendorSellsFor;
                            // If the item.sell price equals the default price entered in itemdesc
                            // then perform standard pricing mods for quality and durability.
                            // Otherwise the price has already undergone mods elsewhere and doesn't
                            // require the standard default mods.
                            if (normcost == item.buyprice)
                                normcost := normcost * item.quality;
                                normsell := normsell * item.quality;
                                percentage := ((item.hp * 10) / item.maxhp);
                                if(percentage)
                                    normcost := CInt((normcost * percentage) / 10);
                                    normsell := CInt((normsell * percentage) / 10);
                                    item.buyprice := normcost;
                                    item.sellprice := normsell;
                                else
                                    // Set item price to default itemdesc value
                                    item.buyprice := itemmark;
                                endif
                            endif
                            // Here is where we can add price mods for vendor friendlies, city reputation or merchant guilds
                            // or mods based on criminal status or whatever we want.
                    endif
            endforeach
	endif
	return counter;
endfunction

/*
function Merchants(merchantgroup)
	var result := array;
	var mrc := array;
	mrc.+name;
	mrc.+type;
	var group_elem := FindConfigElem(mrcspawn_cfg, merchantgroup);
	var count := GetConfigInt(group_elem, "MerchantCount" );
	var i;
	for( i := 1; i <= count; i := i + 1 )
		mrc.name := GetConfigString(group_elem, "Merchant" + i);
		mrc.type := GetConfigString(group_elem, "MerchantType"+i);
		result[i] := mrc;
	endfor
	return result;
endfunction
*/