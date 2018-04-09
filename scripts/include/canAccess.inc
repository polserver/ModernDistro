
function can_access(who, item, flags := 0)
  var owner := Cint(GetObjProperty(item, "Owner"));
  if((owner) and (owner != who.serial))
    SendSysMessage(who, "You cannot use that. (1)");
    return 0;
  elseif(GetObjProperty(item, "Vendored"))
    SendSysMessage(who, "You cannot use that. (2)");
    return 0;
  elseif(!Accessible(who, item))
    SendSysMessage(who, "You cannot use that. (3)");
    return 0;
  elseif((!flags["distance"]) and ((Distance(who, item) > 2)))
    SendSysMessage(who, "That item is too far away.");
    return 0;
  elseif((!flags["unmovable"]) and (item.movable == 0))
    SendSysMessage(who, "Sorry but that is locked down.");
    return 0;
  elseif(!ReserveItem(item))
    SendSysMessage(who, "That item is already in use.");
    return 0;
  else
    return 1;
  endif
endfunction