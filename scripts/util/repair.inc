include ":attributes:attributes";

function repair_item(who, what, attribid)
  var maxhp;
  maxhp := what.maxhp;
  maxhp := CInt(maxhp * what.quality);
  var thehp := what.hp;
  var damaged := maxhp - thehp;
  var hisskill := GetAttribute(who, attribid);
  var damageamount := 0.0;
  damageamount := (125 -  hisskill)/3000.0;
  
  if(what.hp == what.maxhp)
    SendSysMessage(who, "That item is not damaged.");
    return 0; 
  endif

    for i := 1 to 3
	    PerformAction(who, ANIM_ATTACK_1HAND_DOWN);
	    PlaySoundEffect(who, SFX_ANVIL);
	    sleepms(1500);
	    PerformAction(who, ANIM_ATTACK_1HAND_DOWN);
	    PlaySoundEffect(who, SFX_ANVIL);
	    sleepms(1500);
	    PerformAction(who, ANIM_ATTACK_1HAND_DOWN);
	    PlaySoundEffect(who, SFX_ANVIL);
	    sleepms(1500);
    endfor

    if((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
    SendSysMessage(who, "The item was so badly damaged it broke.");
    DestroyItem(what);
    return 0;
  endif
  if(what.quality <= damageamount)
    SendSysMessage(who, "The item falls to pieces as you work it.");
    DestroyItem(what);
    return 0;
  endif
  what.quality := what.quality - damageamount;
  if((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
    SendSysMessage(who, "The item is so fragile it disintegrates during repair.");
    DestroyItem(what);
    return 0;
  endif
  if (SkillCheck(who, attribid, CInt(damaged / 3), 0))
    var repaired_amount := cint( ((randomint(hisskill) + 1))/2);
    if (damaged == 1)
      repaired_amount := 1;
    endif
    if (thehp + repaired_amount > what.maxhp)
      what.hp := what.maxhp;
      SendSysMessage(who, "You repair the item completely");
      return 1;
    else
      what.hp := thehp + repaired_amount;
      SendSysMessage(who, "You successfully repair some of the damage");
      return 1;
    endif
    if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
      SendSysMessage(who, "The brittle material breaks when you handle it");
      DestroyItem(what);
      Print("Destroyed item when " + who.name + " repaired it");
      return 0;
    endif     
  else
    var damage_amount := RandomInt(10);
    if (thehp <= damage_amount)
      DestroyItem(what);
      SendSysMessage(who, "You fail and destroy the item");
    endif
    thehp := thehp - damage_amount;
    what.hp := thehp;
    if(RandomInt(2) == 1)
      what.quality := what.quality - (damaged/100);
    endif
    if(thehp <= 0)
      DestroyItem(what);
      SendSysMessage(who, "You fail and destroy the item");
    else
      SendSysMessage(who, "You fail and damage the item");
    endif
    if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
      SendSysMessage(who, "The brittle material breaks when you handle it");
      DestroyItem(what);
      return 0;
    endif     
  endif
  return 0;
endfunction
