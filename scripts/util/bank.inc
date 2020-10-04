//
// bank.inc: global bankbox functions
//
// public functions:
//      FindBankBox( character)      returns World Bank bankbox for a character
//      WipeBankBox( character )     does what you might think.
//      OpenWorldBank()              returns handle to World Bank.
//

use os;
use storage;

include ":containers:storageAreas";
include "include/objtype";
include "include/utility";

function FindBankBox( character,  bank_obj_name_prefix := "Bankbox of " )
	bank_obj_name_prefix := bank_obj_name_prefix; // Avoid compiler warnings

	var bank_region := CP_DEFAULT_AREA;
	var bankbox		:= CP_GetStorageContainerForMobile( character, bank_region, CP_CREATE );

  // var worldbank := OpenWorldBank();
  // var bank_obj_name := bank_obj_name_prefix + character.serial;
  // var bankbox := FindRootItemInStorageArea( worldbank, bank_obj_name );
  // var bankgump := UOBJ_BANKBOX;
//	case(bank_obj_name_prefix)
//		"Bankbox of "	: bankgump := UOBJ_BANKBOX;
//		"magicbag_"		: bankgump:= UOBJ_MAGICBAG;
//		"gm_backpack_"	: bankgump := UOBJ_GMPACK;
//	endcase*/
	// if(bank_obj_name_prefix == "Bankbox of ")
		// bankgump := UOBJ_BANKBOX;
	// endif
	// if(bank_obj_name_prefix == "magicbag_")
		// bankgump := UOBJ_MAGICBAG;
	// endif
	// if(bank_obj_name_prefix == "gm_backpack_")
		// bankgump := UOBJ_GMPACK;
	// endif
  // if (!bankbox)
    // bankbox := CreateRootItemInStorageArea( worldbank, bank_obj_name, bankgump );
  // endif
  if(!bankbox)
    syslog( "Unable to find or create bankbox for " + character.serial );
  endif
  return bankbox;
endfunction

function WipeBankBox( character, bank_obj_name_prefix := "Bankbox of " )
  var worldbank := OpenWorldBank();
  var bank_obj_name := bank_obj_name_prefix + character.serial;
  return DestroyRootItemInStorageArea( worldbank, bank_obj_name );
endfunction

function OpenWorldBank()
  var wbank := FindStorageArea( "World Bank" );
  if (!wbank)
    wbank := CreateStorageArea( "World Bank" );
  endif
  if (!wbank)
    syslog( "Unable to open or create world bank!" );
  endif
  return wbank;
endfunction

function SpendCoin(who, quantity, cont, cointype)
    var hisGold := GetStacksOfTypeInContainer(cont, cointype);
    who := who;
    if(hisGold[1] < quantity)
	return 0;
    endif
    SubtractFromStackArray (hisGold, quantity);
    return 1;
endfunction

