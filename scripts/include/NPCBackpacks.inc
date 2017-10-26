use storage;

include "include/objtype";

function OpenTamedStorageAreas()
  var bank := FindStorageArea("Tamed Storage");
  if(!bank)
    bank := CreateStorageArea("Tamed Storage");
  endif
  return bank;
endfunction

function FindMyPack(myserial)
  var mybank := OpenTamedStorageAreas();
  var bank_obj_name := "Bankbox  " + Hex(myserial);
  var bankbox := FindRootItemInStorageArea(mybank, bank_obj_name);
  if(!bankbox)
    bankbox := CreateRootItemInStorageArea(mybank, bank_obj_name, UOBJ_BANKBOX);
  endif
  return bankbox;
endfunction

function FindExistingPack(myserial)
  var mybank := OpenTamedStorageAreas();
  var bank_obj_name := "Bankbox  " + Hex(myserial);
  var bankbox := FindRootItemInStorageArea( mybank, bank_obj_name );
  if(!bankbox)
    return 0;
  endif
  return bankbox;
endfunction

function MoveBackpackToCorpse(container)
  var myserial := GetObjProperty(container, "serial");
  if(!myserial)
	return;
  endif
  var mypack := FindExistingPack(myserial);
  if(!mypack)
    return 0;
  endif
  if(GetObjProperty(container, "summoned"))
    foreach thingie in EnumerateItemsInContainer(mypack)
	  if(Distance(thingie,container) != 0)
		MoveObjectToLocation(thingie, container.x, container.y, container.z, 0);
	  endif
    endforeach
    return 0;
  else
    foreach thingie in EnumerateItemsInContainer(mypack)
	  if(Distance(thingie,container) != 0)
		MoveItemToContainer(thingie, container);
	  endif
    endforeach
    return 1;
  endif
endfunction