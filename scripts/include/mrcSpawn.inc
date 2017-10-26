// Some comments added by Yukiko - denoted by capital 'Y'

use cfgfile;
include ":attributes:attributes";
include "include/utility";

var mrcspawn_cfg := ReadConfigFile("::mrcspawn");
var itemconfig := ReadConfigFile(":*:itemdesc");
var storage := find_or_create_storage("Merchant Storage");

function FindMerchantType(mrctype)
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

function create_node_merc(merchanttype, name)
	var inv_rs := find_or_create_item(storage, name + " RS");
	var inv_fs := find_or_create_item(storage, name + " FS");
	var inv_pb := find_or_create_item(storage, name + " PB");
	var inv_1c := find_or_create_item(storage, name + " 1C");
	SetObjProperty(inv_rs, "MerchantType", merchanttype);
	SetObjProperty(inv_rs, "MyGold", 5000);
	StockInventory(merchanttype, inv_rs);
	RestockInitialInventory(inv_rs, inv_fs);
endfunction

function destroy_node_merc(name)
	DestroyRootItemInStorageArea(storage, name + " RS");
	DestroyRootItemInStorageArea(storage, name + " FS");
	DestroyRootItemInStorageArea(storage, name + " PB");
	DestroyRootItemInStorageArea(storage, name + " 1C");
endfunction

function StockInventory(merchanttype, inventory)
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

function SpawnItem(inventory, itemstr)
	var split := SplitWords(itemstr);
	var objtype := split[1];
	var amt := Cint(split[2]);
	var buyprice	:= itemconfig[objtype].VendorBuysFor;
	var sellprice := itemconfig[objtype].VendorSellsFor;
	if(Cint(objtype))
		objtype := Cint(objtype);
	else
		objtype := Cint(GetObjtypeByName(objtype));
	endif
	if(!objtype)
		syslog("could not determine objtype for itemstr: " + itemstr);
		return;
	endif
	var res;
	if(amt > 0)
		var i;
		for( i := 1; i <= amt; i := i + 1 )
			res := CreateItemInInventory(inventory, objtype, 1);
			res.buyprice	:= buyprice;
			res.sellprice := sellprice;
		endfor
	endif
endfunction

function checkstock(rs, pak)
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
	var multiplier, amt, mlt, things;
	var newarray := {};
	foreach thing in EnumerateItemsInContainer(source)
		newarray.append(thing.objtype);
		amt := 0;
		multiplier := CInt(GetObjProperty(thing, "Multiplier"));
		var chk := CountExisting(destination, thing.objtype);
		things := chk[2];
		amt := chk[1];
		if(amt >= (thing.amount * multiplier))
			multiplier := multiplier - 1;
			if(multiplier < 1)
				multiplier := 1;
			endif
		elseif(amt == 0)
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
		if(amt != mlt)
			foreach widget in things
				DestroyItem(widget);
			endforeach
			if(amt > mlt)
				res := CreateItemInInventory(destination, thing.objtype, mlt);
				if(res)
					res.sellprice := sellprice;
					res.buyprice := buyprice;
				endif
			elseif(amt < mlt)
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
		if((!(thing.objtype in newarray)) and (!GetObjProperty(thing, "ClearRestock")))
			DestroyItem(thing);
		endif
	endforeach
endfunction

function RestockMyInventory(source, destination)
	var merchanttype := GetObjProperty(source, "MerchantType");
	var multiplier, amt, mlt, items, mygold;
	var newarray := {}; // Array of what's for sale
	// cycle through everything that's for sale
	foreach thing in EnumerateItemsInContainer(source)// this is the "ideal" stock.
		newarray.append(thing.objtype);
		amt := 0;
		mygold := GetObjProperty(source, "MyGold");
		multiplier := CInt(GetObjProperty(thing, "Multiplier")); // multiplier of increments.
		var chk := CountExisting(destination, thing.objtype); // to see what we got *really* for sale
		items := chk[2];// returns all of a particular item, stacked or unstacked. (array)
		amt := chk[1];
		
		// determine whether to increase or decrease stock.
		if(amt >= (thing.amount * multiplier))
			multiplier := multiplier - 1;
			if(multiplier < 1)
				multiplier := 1;
			endif
		elseif(amt == 0)
			multiplier := multiplier + 1;
			if(multiplier > 20)
				multiplier := 20;
			endif
		endif
		SetObjProperty(thing, "Multiplier", multiplier);
		
		mlt := Cint(thing.amount * multiplier); // mlt is the amount that should be in stock
		
		if(amt != mlt)
			// define characteristics of the items
			var buyprice := Cint(thing.buyprice);
			var sellprice := Cint(thing.sellprice);
			var res;

			// remove what remains
			foreach widget in items
				mygold := mygold + Cint(widget.amount * buyprice);
				DestroyItem(widget);
			endforeach
			var i;
			// make one of everything you need
			for(i:=1; i<= mlt; i := i+1)
				if((mygold > buyprice) or (merchanttype == "Architect"))
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
		if((!(thing.objtype in newarray)) and (!GetObjProperty(thing, "ClearRestock")))
			mygold := mygold + Cint(thing.amount * thing.buyprice);
			DestroyItem(thing);
		endif
	endforeach
	SetObjProperty(source, "MyGold", mygold);
endfunction

function CountExisting(container, objtype)
	var amt := 0;
	var things := {};
	foreach thing in EnumerateItemsInContainer(container)
		if((thing.objtype == objtype) and (Cint(GetObjProperty(thing, "ClearRestock")) == 0))
			things.append(thing);
			amt := amt + thing.amount;
		endif
	endforeach
	var ary := {};
	ary.append(amt);
	ary.append(things);
	return ary;
endfunction

function ModifyPCSellList(merchanttype, customer)
	var backpackitems := ListRootItemsInContainer(customer.backpack);
	var prodgroups := FindConfigElem(mrcspawn_cfg, merchanttype);
	var buys := GetConfigStringArray(prodgroups, "Buys");
	var normcost, normsell, percentage;
	
	//  If not buying lets call the whole thing off.
	if(buys.size() < 1)
		return 0;
	endif

	var iteminit := 0;
	var itemmark := -1;
	
	// set buy price for each item to be 0 to start. This means it won't be sellable.
        // Commented out by Yukiko - This section prevented us from setting custom buy prices for items
        
        
/*	foreach item in backpackitems
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