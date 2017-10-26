function CheckToolWear (who, tool, skillname)
    // A bonus for those who save their starting tools.
    // *grins*
    if(tool.newbie)
	return;
    endif
  var swings := GetObjProperty(tool,"swings");
  if(!swings)
    var mods := GetObjProperty(tool, "durability");
    var bonus := 0;
    case(mods)
      "durable":        bonus := 10;
      "substantial":    bonus := 20;
      "massive":        bonus := 30;
      "fortified":      bonus := 40;
      "indestructable": bonus := 50;
    endcase
    swings := Cint(50 * tool.quality) + bonus;
    SetObjProperty(tool,"swings", swings);
  endif
  if(swings <= 0)
    DestroyItem(tool);
    SendSysMessage(who,"Your tool broke.", font := 2, color := 33);
  else
    var skill := AP_GetSkill(who, skillname);
    if(skill > 80)
      skill := 80;
    elseif(skill < 20)
      skill := 20;
    endif
    if((RandomInt(100)+1) > skill)
      if(tool.isa(POLCLASS_WEAPON))
        if(tool.hp > 1)
          SetObjProperty(tool,"swings", (swings - 1));
          tool.hp := tool.hp - 1;
        elseif((tool.hp > tool.maxhp) or (tool.hp == 1))
          SendSysMessage(who,"Your tool broke.", font := 2, color := 33);
          DestroyItem(tool);
        endif
      else
        SetObjProperty(tool,"swings", (swings - 1));
      endif	
    endif
  endif
endfunction  