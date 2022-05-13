
include ":damage:damage";

function HandleMods(who, item)
  set_critical(1);
  var itemdesc := ReadConfigFile("itemdesc");
  var penalty  := CInt(itemdesc[item.objtype].DexPenalty);
  var restrict := Cint(itemdesc[item.objtype].GenderSpecific);
  if((restrict == 1) and (who.graphic == 400))
    SendSysMessage(who, "Only females may wear that.");
    return 0;
  endif
  if(penalty)
    SetObjProperty(item, "#equipped", who.serial);
    AP_SetStatMod(who, DEXTERITY, AP_GetStatMod(who, DEXTERITY) - penalty);
  endif
  var type := GetObjProperty(item, "type");
  if(!type)
    return 1;
  endif
  var val := 0;
  if(!who.ip)
    return 1;
  endif
  if(type)
	  case(type)
		"accuracy":     val := accuracy(who,item);
		"cursed":       val := cursed(who,item);
		"sight":        val := sight(who,item);
		"reflect":      val := reflect(who,item);
		"protection":   val := protection(who,item);
		"wizzohat":     val := wizzohat(who,item);
		"strength":     val := strengthBoost(who,item);
		"invisibility": val := invisibility(who,item);
		default:        val := 1;
	  endcase
  endif
  // The following lines are commented out because the Core handles
  // AoS modifiers.
  // if(item.resist_physical)
    // ModifyResistance(who, RES_PHYSICAL_MOD, item.resist_physical);
  // endif
  // if(item.resist_fire)
    // ModifyResistance(who, RES_FIRE_MOD, item.resist_fire);
  // endif
  // if(item.resist_cold)
    // ModifyResistance(who, RES_COLD_MOD, item.resist_cold);
  // endif
  // if(item.resist_poison)
    // ModifyResistance(who, RES_POISON_MOD, item.resist_poison);
  // endif
  // if(item.resist_energy)
    // ModifyResistance(who, RES_ENERGY_MOD, item.resist_energy);
  // endif

  // if(item.damage_physical)
    // ModifyDamage(who, DMG_PHYSICAL_MOD, item.damage_physical);
    // SendSysMessage(who, CStr(item.damage_physical));
  // endif
  // if(item.damage_fire)
	// Print(item.damage_fire);
    // ModifyDamage(who, DMG_FIRE_MOD, item.damage_fire);
  // endif
  // if(item.damage_cold)
    // ModifyDamage(who, DMG_COLD_MOD, item.damage_cold);
  // endif
  // if(item.damage_poison)
    // ModifyDamage(who, DMG_POISON_MOD, item.damage_poison);
  // endif
  // if(item.damage_energy)
    // ModifyDamage(who, DMG_ENERGY_MOD, item.damage_energy);
  // endif

  return val;
  set_critical(0);
endfunction

function strengthBoost(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function wizzohat(who,item)
  AP_SetStatMod(who, INTELLIGENCE, AP_GetStatMod(who, INTELLIGENCE) + 10);
  AP_SetStatMod(who, STRENGTH, AP_GetStatMod(who, STRENGTH) - 5);
  AP_SetStatMod(who, DEXTERITY, AP_GetStatMod(who, DEXTERITY) - 5);
  SetObjProperty(item, "#equipped", who.serial);
  return 1;
endfunction

function accuracy(who,item)
  var amt := CInt(GetObjProperty(item,"gain"));
  if((!amt) || (amt == 0))
    amt := 5;
  endif
  var cfg := ReadConfigFile(":combat:itemdesc");
  var attrib := cfg[item.objtype].Attribute;
  case(attrib)
    "Wrestling": ModifyAttributeTemporaryMod(who, WRESTLING,     amt);
    "Fencing":   ModifyAttributeTemporaryMod(who, FENCING,       amt);
    "Archery":   ModifyAttributeTemporaryMod(who, ARCHERY,       amt);
    "Swords":    ModifyAttributeTemporaryMod(who, SWORDSMANSHIP, amt);
    "Mace":      ModifyAttributeTemporaryMod(who, MACEFIGHTING,  amt);
  endcase
  SetObjProperty(item, "#equipped", who.serial);
  return 1;
endfunction

function cursed(who,item)
  PlaySoundEffect(who, 0x246);
  if(!GetObjProperty(item,"named"))
    item.name := item.name + " [Cursed]";
    SetObjProperty(item,"named", 1);
  endif
  PrintTextAbovePrivate(who,"you have been cursed!", who);
  SetObjProperty(item, "#equipped", who.serial);
  return 1;
endfunction

function invisibility(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function sight(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function protection(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction

function reflect(who,item)
  var gain := GetObjProperty(item, "gain");
  if(!gain)
    gain := 5;
  endif
  SetObjProperty(item, "#equipped", who.serial);
  var parms := {who, item};
  start_script("chargeUpkeep", parms);
  return 1;
endfunction


