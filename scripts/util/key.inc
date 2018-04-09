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
  foreach item in EnumerateItemsInContainer( who.backpack )
    if(item.IsKey())
      if(item.KeyMatchesLock(lockid))
        return item;
      endif
    endif
  endforeach
  return 0;
endfunction