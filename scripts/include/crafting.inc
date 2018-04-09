
include "include/noto";
include ":gumps:requestGump";
include "util/bank"; // Used in FindCraftingBag function
include "include/itemdesc";

var itemdesc_cfg := ReadConfigFile (":*:itemdesc");


///////////////////
//  Tries to find the bag that crafted items will go into, if not the character's main backpack.
//  Set with .setcraftbag
///////////////////

function FindCraftingBag (who)
	//Make absolutely certain that they have a backpack
	if (!who.backpack)
		var newbackpack := CreateItemAtLocation (5288, 1176, 0, UOBJ_BACKPACK, 1, who.realm);
		if (!EquipItem (who, newbackpack))
			DestroyItem (newbackpack);
		endif
	endif

	var craftingbagserial := GetObjProperty (who, "craftingbagserial");
	if (!craftingbagserial)
		return who.backpack;
	endif
	
	foreach item in EnumerateItemsInContainer (who.backpack)
		if (item.serial == craftingbagserial)
			if (item.isa (POLCLASS_CONTAINER))
				return item;
                        elseif (item.objtype == 0x6699 and who.serial == CInt(GetObjProperty (item, "Owner")))
                            return FindBankBox (who, "magicbag_");
			endif
		endif
	endforeach

	//Still can't find it
	EraseObjProperty (who, "craftingbagserial");
	return who.backpack;
endfunction


function get_item_name(item_number, plural := 0)
	var elem;
        var item_name := "";
	item_number := CInt(item_number);
	elem:=FindConfigElem(itemdesc_cfg,item_number);
        if(elem)
            item_name:=GetConfigString(elem,"Desc");
        endif
	if(!item_name)
            item_name:=GetConfigString(elem,"Name");
	endif
	
	if ( !item_name["%"] ) // Plain desc line - easy enough to deal with.
		if ( plural == 0 )
			return item_name;
		elseif ( item_name[Len(item_name)] in array{"s"} )
			return item_name;
		elseif ( item_name[Len(item_name)] in array{"h","j","x","z"} )
			return item_name+"es";
		else
			return item_name+"s";
		endif
	endif
	
	
	/* This section will deal with:
	 	Bread Stick%s
		Bread Loa%ves/f%
		Lump%s% of Coal
		Loa%ves/f% of Bread
	*/
	var first := Find(item_name, "%", 1);
	var slash := Find(item_name, "/", first);
	var second := Find(item_name, "%", first + 1);
	
	if ( slash && !second )
		// Force-append a % if a '/' exists!
		item_name := item_name+"%";
		second := Len(item_name);
	elseif ( !slash )
		slash := second;
	endif

	// wwwww%xx[/yy]%[zzzzz]
	// 12345678     9 01234
	// 12345678 901 2 34567

	var s_plur := item_name[first+1, slash-(first+1)];
	var s_sing := "";
	if ( slash != first )
		s_sing := item_name[slash+1, second-(slash+1)];
	endif

	var s_first := item_name[1, first-1];
	var s_middle := s_plur;
	var s_last := "";
	if ( Len(item_name) > second )
		s_last := item_name[second+1, Len(item_name)-second];
	endif

	if ( plural == 0 )
		s_middle := s_sing;
	endif
	
	if ( !s_first )
		s_first := "";
	endif
	if ( !s_middle )
		s_middle := "";
	endif
	if ( !s_last )
		s_last := "";
	endif
	
	return s_first + s_middle + s_last;

//        return item_name;
endfunction
