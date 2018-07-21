var potion_config := ReadConfigFile(":alchemy:alchemy");

function rename_keg(keg, potion)
	var itemdesc := ReadConfigFile(":alchemy:itemdesc");
	var obj := FindConfigElem(itemdesc, potion);
	var amt  := CInt(GetObjProperty(keg, "PotionAmount"));
	if(amt > 0)
		var str := "a keg of " + obj.desc + "s. " + GetFullness(amt);
		SetName(keg, str);
	else
		var str := "specially lined keg for holding potions. " + GetFullness(amt);
		SetName(keg, str);
	endif
	return 0;
endfunction


function GetFullness(amt)
	var str;
	if (amt == 0)
		str := "The keg is empty.";
	elseif ((amt >= 1) && (amt <= 4))
		str := "The keg is nearly empty.";
	elseif ((amt >= 5) && (amt <= 19))
		str := "The keg is not very full.";
	elseif ((amt >= 20) && (amt <= 29))
		str := "The keg is about one quarter full.";
	elseif ((amt >= 30) && (amt <= 39))
		str := "The keg is about one third full.";
	elseif ((amt >= 40) && (amt <= 46))
		str := "The keg is almost half full.";
	elseif ((amt >= 47) && (amt <= 53))
		str := "The keg is approximately half full.";
	elseif ((amt >= 54) && (amt <= 69))
		str := "The keg is more than half full.";
	elseif ((amt >= 70) && (amt <= 79))
		str := "The keg is about three quarters full.";
	elseif ((amt >= 80) && (amt <= 95))
		str := "The keg is very full.";
	elseif ((amt >= 96) && (amt <= 99))
		str := "The liquid is almost to the top of the keg.";
	else
		str := "The keg is completely full.";
	endif

	return str;
endfunction

// This was changed to check the alchemy.cfg file, so that
// any new potion added would automatically be able to be put
// into a keg. YAY for drag-n-drop style addons!
function is_potion(objtype)
	// Initially see if it's in the alchemy.cfg file.
	var this_potion := potion_config[objtype];
	if( TypeOf(this_potion) != "ConfigElemRef" )
		this_potion := potion_config[("0x0"+(CStr(objtype) - "0x"))];
	endif

	// Now that we have checked 0x and 0x0 format, let's
	// verify and return the result.
	if( TypeOf(this_potion) != "ConfigElemRef" )
		return 0;
	else
		return 1;
	endif

	return 0;
endfunction