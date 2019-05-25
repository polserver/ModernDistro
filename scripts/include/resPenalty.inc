use uo;
include ":attributes:attributes";

var maximumloss := 0.20; //20% loss for 20 shortterm murders or more

function ResPenalties(who)
  MurdererStatLoss(who);
endfunction

function MurdererStatLoss(who)
  var shortc := GetObjProperty(who,"shortmurders");
  if(!shortc)
	return;
  endif
  if(shortc > 4)
	var multiplier := CInt(shortc)/100.0;
	if(multiplier > maximumloss)
	  multiplier := maximumloss;
	endif
	multiplier := 1.0 - multiplier;
	var str := AP_GetStat(who, STRENGTH);
	var dex := AP_GetStat(who, DEXTERITY);
	var inte := AP_GetStat(who, INTELLIGENCE);
	str := CInt(str * multiplier);
	dex := CInt(dex * multiplier);
	inte := CInt(inte * multiplier);
	AP_SetTrueStat(who, STRENGTH, str);
	AP_SetTrueStat(who, DEXTERITY, dex);
	AP_SetTrueStat(who, INTELLIGENCE, inte);
	AP_SetVital(who, VITALID_LIFE, who.maxhp);
	AP_SetVital(who, VITALID_STAMINA, who.maxstamina);
	AP_SetVital(who, VITALID_MANA, who.maxmana);
    var skillarray := GetObjProperty(who, "SkillArray");
    var newval, final;
    foreach entry in skillarray
      if((GetTrueEffectiveSkill(who, entry)  ) > 0.0)
        newval:= (GetTrueEffectiveSkill(who, entry)  );
        final:=(newval * multiplier);
        SetBaseSkillBaseValue(who, entry, CInt(final * 10));
      endif
    endforeach
	SendSysMessage(who,"Thy body pays the price for your sins.");
  endif
endfunction