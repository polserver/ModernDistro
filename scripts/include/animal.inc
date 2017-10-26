include ":attributes:attributes";
function Expand_Stats(me)
  if(GetObjProperty(me, "#Expanded"))
    return;
  endif
  var npcdescfile:=ReadConfigFile("::npcdesc");
  if (npcdescfile)
    var template:=npcdescfile[me.npctemplate];
    var value:=CInt(template.str);
    if (value)
      if (value<=210)
        SetBaseStrength(me,value);
        SetStrengthMod(me,0);
      else
        SetBaseStrength(me,210);
        SetStrengthMod(me,value-210);
      endif
    endif
    value:=CInt(template.dex);
    if(value)
      if(value<=210)
        SetBaseDexterity(me,value);
        SetDexterityMod(me,0);
      else
        SetBaseDexterity(me,210);
        SetDexterityMod(me,value-210);
      endif
    endif
    value:=GetConfigInt(FindConfigElem(npcdescfile, me.npctemplate), "int");
    if(value)
      if(value<=210)
        SetBaseIntelligence(me,value);
        SetIntelligenceMod(me,0);
      else
        SetBaseIntelligence(me,210);
        SetIntelligenceMod(me,value-210);
      endif
    endif
    value:=CInt(template.hits);
    if (value)
      SetHp(me,value);
    endif
    value:=CInt(GetMana(template));
    if (value)
      SetMana(me,value);
    endif
    value:=CInt(template.stam);
    if (value)
      SetStamina(me,value);
    endif
    SetObjProperty(me, "#Expanded", 1);
  else
    syslog("Setup: Can't open 'npcdesc.cfg'!");
  endif
endfunction