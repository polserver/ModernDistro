use storage;

include "include/client";
include "include/objtype";

function AllocLockId()
  var lockid := GetGlobalProperty( "nextlockid" );
  if (!lockid)
    lockid := 1;
  endif
  SetGlobalProperty( "nextlockid", lockid+1 );
  return lockid;
endfunction

function FindKey(who, what)
  var lockid := GetObjProperty( what, "lockid" );
  if(!lockid)
    return 1;
  endif
  var keytypes := { UOBJ_COPPER_KEY, UOBJ_GOLD_KEY, UOBJ_IRON_KEY, UOBJ_MAGIC_KEY, UOBJ_RUSTY_IRON_KEY };
  foreach item in EnumerateItemsInContainer( who.backpack )
    if(item.graphic in keytypes)
      if(GetObjProperty( item, "lockid" ) == lockid)
        return item;
      endif
    endif
  endforeach
  return 0;
endfunction