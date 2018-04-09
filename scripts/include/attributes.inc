/////////////////////////////////////////////////////
// 06/25/2017 Yukiko removed ATTRIBUTEID_ from the
// atribute constant declarations. I left SKILLID_
// and VITALID_ alone to avoid ambiguities, 
// especially with skills else there would have been
// naming conflicts. I removed attribute aliases as
// well such as evalint. I expanded the attribute
// constant names to their full name, example
// TESTEID is now TASTE_IDENTIFICATION.
/////////////////////////////////////////////////////

use vitals;
use attributes;
use uo;
use os;
use math;
use util;
use cfgfile;

include "include/client";
include "include/eventID";
include ":SkillLocks:common";

const CFGFILESPEC_SKILLSDEF        := ":attributes:skillsdef";
const CORE_SETTINGS_MIN_ATTRIBUTE  := 0;
const CORE_SETTINGS_MAX_ATTRIBUTE  := 60000;
const CORE_SETTINGS_MIN_VITAL      := 100;
const CORE_SETTINGS_MAX_VITAL      := 10000000;
const CORE_SETTINGS_MIN_REGEN_RATE := -30000;
const CORE_SETTINGS_MAX_REGEN_RATE := 30000;
const CPROP_NAME_PREFIX_REGEN_RATE := "NewRegenRateFor";
const DEFAULT_SETTING_SKILL_ALWAYS_GAIN_UNTIL  := 10.0;
const DEFAULT_SETTING_SKILL_CHECK_MAX_CHANCE  := 100;
const DEFAULT_SETTING_SKILL_CHECK_MIN_CHANCE  := 0;
const ALCHEMY          := "alchemy";
const ANATOMY          := "anatomy";
const ANIMALLORE       := "animallore";
const ARCHERY          := "archery";
const ARMSLORE         := "armslore";
const BEGGING          := "begging";
const BLACKSMITHY      := "blacksmithy";
const BOWCRAFT         := "bowcraft";
const CAMPING          := "camping";
const CARPENTRY        := "carpentry";
const CARTOGRAPHY      := "cartography";
const COOKING          := "cooking";
const DETECTING_HIDDEN  := "detectinghidden";
const DISCORDANCE          := "discordance";
const EVALUATING_INTELLIGENCE    	   := "evaluatingintelligence";
const FOCUS             := "focus";
const FENCING          := "fencing";
const FISHING          := "fishing";
const FORENSIC_EVALUATION        := "forensicevaluation";
const HEALING          := "healing";
const HERDING           := "herding";
const HIDING           := "hiding";
const IMBUING           := "imbuing";
const INSCRIPTION      := "inscription";
const ITEM_IDENTIFICATION           := "itemidentification";
const LOCKPICKING      := "lockpicking";
const LUMBERJACKING    := "lumberjacking";
const MACEFIGHTING     := "macefighting";
const MAGERY           := "magery";
const RESISTING_SPELLS             := "resistingspells";
const MEDITATION       := "meditation";
const MINING           := "mining";
const MUSICIANSHIP     := "musicianship";
const PARRYING           := "parry";
const PEACEMAKING      := "peacemaking";
const POISONING        := "poisoning";
const PROVOCATION      := "provocation";
const REMOVETRAP       := "removetrap";
const SNOOPING         := "snooping";
const SPIRITSPEAK      := "spiritspeak";
const STEALING         := "stealing";
const STEALTH          := "stealth";
const SWORDSMANSHIP    := "swordsmanship";
const TACTICS          := "tactics";
const TAILORING        := "tailoring";
const TAMING           := "animaltaming";
const TASTE_IDENTIFICATION          := "tasteidentification";
const TINKERING        := "tinkering";
const TRACKING         := "tracking";
const VETERINARY       := "veterinary";
const WRESTLING        := "wrestling";
const CHIVALRY          := "chivalry";
const NECROMANCY       := "necromancy";
const NINJITSU         := "ninjitsu";
const BUSHIDO          := "bushido";
const SPELLWEAVING     := "spellweaving";
const THROWING         := "throwing";
const MYSTICISM         := "mysticism";
const ATTRIBUTEID_DEXTERITY        := "dexterity";
const ATTRIBUTEID_INTELLIGENCE     := "intelligence";
const ATTRIBUTEID_STRENGTH         := "strength";
const VITALID_LIFE                 := "Life";
const HITS                         := VITALID_LIFE;
const VITALID_MANA                 := "Mana";
const VITALID_STAMINA              := "Stamina";

var skill_settings :=  AP_GetSettingsCfgElem("Skills");

Function DoRecalcVitals( mob )
  set_critical(1);
  if(mob.acctname)
    if(mob.ip)
      RecalcVitals(mob);
    endif
  else
    RecalcVitals(mob);
  endif
  set_critical(0);
endfunction

function ApplyManaDamage(who, dmg)
  return SetMana(who, GetMana(who)-dmg);
endfunction

function ApplyStaminaDamage(who, dmg)
  return SetStamina(who, GetStamina(who)-dmg);
endfunction

function AwardPoints(who, skillid, skillpoints)

    // If it ain't a mobile then don't bother
    if(!who.isA(POLCLASS_MOBILE))
		return 0;
    // If the skillid is an error then abort
    elseif(Cint(skillid)== error)
		return 0;
    endif
	
    // Make the skillid an integer
    skillid := Cint(skillid);
    // Reads the info from skillsdef.cfg to determine which stats will be affected by the skills being used.
    var elem := GetSkillConfigElem(skillid);
    if(!elem)
	return elem;
    endif
    // Get the string attribute name using the skill number
    var attributeid := GetAttributeIdBySkillId(skillid);
    if((attributeid != ATTRIBUTEID_STRENGTH) and (attributeid != ATTRIBUTEID_DEXTERITY) and (attributeid != ATTRIBUTEID_INTELLIGENCE))
		if(!who.npctemplate)
			var skillarray := GetObjProperty(who,"SkillArray");
			if(!skillarray)
			skillarray := {};
			foreach thing in GetAttributeIdsWithoutStatIds()
				if(GetAttributeBaseValue(who, thing) > 0)
				skillarray.append(GetSkillIdByAttributeId(thing));
				endif
			endforeach
			SetObjProperty(who,"SkillArray", skillarray);
			endif
			var skill := GetSkillIdByAttributeId(attributeid);
			var newarray := {};
			foreach thing in skillarray
			if((thing != skill) and (GetBaseSkill(who, thing) > 0))
				newarray.append(thing);
			endif
			endforeach
			newarray.append(skill);
			skillarray := newarray;
			SetObjProperty(who,"SkillArray", skillarray);
		endif
    endif
    AwardRawAttributePoints(who, attributeid, skillpoints, skillid);
    var adv := elem.StrAdv;
    if(adv)
	adv := SplitWords(adv);
	if(RandomInt(1000)< Cdbl(adv[1])*10)
	    AwardRawAttributePoints(who, ATTRIBUTEID_STRENGTH, RandomDiceRoll(adv[2]), skillid);
	endif
    endif
    adv := 0;
    adv := elem.DexAdv;
    if(adv)
	adv := SplitWords(adv);
	if(RandomInt(1000)< Cdbl(adv[1])*10)
	    AwardRawAttributePoints(who, ATTRIBUTEID_DEXTERITY, RandomDiceRoll(adv[2]), skillid);
	endif
    endif
    adv := 0;
    adv := elem.IntAdv;
    if(adv)
	adv := SplitWords(adv);
	if(RandomInt(1000)< Cdbl(adv[1])*10)
	    AwardRawAttributePoints(who, ATTRIBUTEID_INTELLIGENCE, RandomDiceRoll(adv[2]), skillid);
	endif
    endif
    return 1;
endfunction

function AwardRawAttributePoints(who, attributeid, rawpoints, skillid)
  if(!who.isA(POLCLASS_MOBILE))
    var ret := error;
    ret.+errortext := "Invalid parameter type: " + who + " isn't a mobileref.";
    return ret;
  endif
  rawpoints := Cdbl(rawpoints);
  if(rawpoints == error)
    return rawpoints;
  elseif(rawpoints == 0.0)
    return 1;
  endif
  set_critical(1);
  var initial := GetAttributeBaseValue(who, attributeid);
  var current := initial;
  if(rawpoints > 0)
    var to_increase := GetNeededRawPointsToIncrease(current);
    while(rawpoints >= to_increase)
      rawpoints  := rawpoints - to_increase;
      current    := current + 1;
      to_increase := GetNeededRawPointsToIncrease(current);
    endwhile
    if(RandomInt(Cint(to_increase)) < rawpoints)
      current := current + 1;
    endif
  elseif(rawpoints < 0)
    var to_decrease  := GetNeededRawPointsToIncrease(current-1);
    while(rawpoints >= to_decrease && current > 0)
      rawpoints  := rawpoints - to_decrease;
      current    := current - 1;
      to_decrease  := GetNeededRawPointsToIncrease(current-1);
    endwhile
    if(RandomInt(Cint(to_decrease))< rawpoints)
      current := current - 1;
    endif
  endif
  if(current != initial)
    if(who.acctname)
      if(current > CORE_SETTINGS_MAX_ATTRIBUTE)
        current := CORE_SETTINGS_MAX_ATTRIBUTE;
      endif
      var pid := getprocess(GetObjProperty(who, "cap_pid"));
      var k := struct;
      if((attributeid == ATTRIBUTEID_STRENGTH) or (attributeid == ATTRIBUTEID_DEXTERITY) or (attributeid == ATTRIBUTEID_INTELLIGENCE))
        k.+skid   := attributeid;
        k.+skil   := skillid;
        k.+type   := EVID_STATCAP;
        k.+base   := current;
        pid.sendevent(k);
      else
        k.+skid   := attributeid;
        k.+type   := EVID_SKILLCAP;
        k.+base   := current;
        pid.sendevent(k);
      endif
    else
      SetAttributeBaseValue(who, attributeid, current);
    endif
    DoRecalcVitals(who);
  endif
  set_critical(0);
  return 1;
endfunction

function AwardRawSkillPoints(who, skillid, rawpoints)
  return AwardRawAttributePoints(who, GetAttributeIdBySkillId(skillid), rawpoints, skillid);
endfunction

function BaseToRawSkill(basevalue)
  if(basevalue <= 200)
    var rawvalue := basevalue * 20.48;
    if(rawvalue > Cint(rawvalue))
      return Cint(rawvalue + 1);
    else
      return Cint(rawvalue);
    endif
  else
    var scale  := basevalue / 100.0;
    var remain   := scale - Cint(scale);
    return Cint(Pow(2, Cint(scale))*(1024 *(1.0 + remain)+ 1));
  endif
endfunction

function GetAttributeIdBySkillId(skillid)
  case(skillid)
    SKILLID_ALCHEMY:              return ALCHEMY;
    SKILLID_ANATOMY:              return ANATOMY;
    SKILLID_ANIMALLORE:           return ANIMALLORE;
    SKILLID_ITEM_IDENTIFICATION:               return ITEM_IDENTIFICATION;
    SKILLID_ARMSLORE:             return ARMSLORE;
    SKILLID_PARRYING:                return PARRYING;
    SKILLID_BEGGING:              return BEGGING;
    SKILLID_BLACKSMITHY:          return BLACKSMITHY;
    SKILLID_BOWCRAFT:             return BOWCRAFT;
    SKILLID_PEACEMAKING:          return PEACEMAKING;
    SKILLID_CAMPING:              return CAMPING;
    SKILLID_CARPENTRY:            return CARPENTRY;
    SKILLID_CARTOGRAPHY:          return CARTOGRAPHY;
    SKILLID_COOKING:              return COOKING;
    SKILLID_DETECTING_HIDDEN:      return DETECTING_HIDDEN;
    SKILLID_DISCORDANCE:           return DISCORDANCE;
    SKILLID_EVALUATING_INTELLIGENCE:              return EVALUATING_INTELLIGENCE;
    SKILLID_HEALING:              return HEALING;
    SKILLID_FISHING:              return FISHING;
    SKILLID_FORENSIC_EVALUATION:            return FORENSIC_EVALUATION;
    SKILLID_HERDING:              return HERDING;
    SKILLID_HIDING:               return HIDING;
    SKILLID_PROVOCATION:          return PROVOCATION;
    SKILLID_INSCRIPTION:          return INSCRIPTION;
    SKILLID_LOCKPICKING:          return LOCKPICKING;
    SKILLID_MAGERY:               return MAGERY;
    SKILLID_RESISTING_SPELLS:      return RESISTING_SPELLS;
    SKILLID_TACTICS:              return TACTICS;
    SKILLID_SNOOPING:             return SNOOPING;
    SKILLID_MUSICIANSHIP:         return MUSICIANSHIP;
    SKILLID_POISONING:            return POISONING;
    SKILLID_ARCHERY:              return ARCHERY;
    SKILLID_SPIRITSPEAK:          return SPIRITSPEAK;
    SKILLID_STEALING:             return STEALING;
    SKILLID_TAILORING:            return TAILORING;
    SKILLID_TAMING:               return TAMING;
    SKILLID_TASTE_IDENTIFICATION:              return TASTE_IDENTIFICATION;
    SKILLID_TINKERING:            return TINKERING;
    SKILLID_TRACKING:             return TRACKING;
    SKILLID_VETERINARY:           return VETERINARY;
    SKILLID_SWORDSMANSHIP:        return SWORDSMANSHIP;
    SKILLID_MACEFIGHTING:         return MACEFIGHTING;
    SKILLID_FENCING:              return FENCING;
    SKILLID_WRESTLING:            return WRESTLING;
    SKILLID_LUMBERJACKING:        return LUMBERJACKING;
    SKILLID_MINING:               return MINING;
    SKILLID_MEDITATION:           return MEDITATION;
    SKILLID_STEALTH:              return STEALTH;
    SKILLID_REMOVETRAP:           return REMOVETRAP;
    SKILLID_NECROMANCY:           return NECROMANCY;
    SKILLID_FOCUS:                return FOCUS;
    SKILLID_CHIVALRY:              return CHIVALRY;
    SKILLID_BUSHIDO:               return BUSHIDO;
    SKILLID_NINJITSU:              return NINJITSU;
    SKILLID_SPELLWEAVING:          return SPELLWEAVING;
    SKILLID_MYSTICISM:             return MYSTICISM;
    SKILLID_IMBUING:               return IMBUING;
    SKILLID_THROWING:              return THROWING;
  endcase
  var ret        := error;
  ret.+errortext := "No equivalent attributeid found for skillid " + skillid;
  return ret;
endfunction

function GetAttributeIds()
  return { ALCHEMY,
     ANATOMY,
     ANIMALLORE,
     ARCHERY,
     ARMSLORE,
     BEGGING,
     BLACKSMITHY,
     BOWCRAFT,
     BUSHIDO,
     CAMPING,
     CARPENTRY,
     CARTOGRAPHY,
     CHIVALRY,
     COOKING,
     DETECTING_HIDDEN,
     ATTRIBUTEID_DEXTERITY,
     DISCORDANCE,
     EVALUATING_INTELLIGENCE,
     FENCING,
     FISHING,
     FOCUS,
     FORENSIC_EVALUATION,
     HEALING,
     HERDING,
     HIDING,
     IMBUING,
     INSCRIPTION,
     ATTRIBUTEID_INTELLIGENCE,
     ITEM_IDENTIFICATION,
     LOCKPICKING,
     LUMBERJACKING,
     MACEFIGHTING,
     MAGERY,
     RESISTING_SPELLS,
     MEDITATION,
     MINING,
     MUSICIANSHIP,
     MYSTICISM,
     NECROMANCY,
     NINJITSU,
     PARRYING,
     PEACEMAKING,
     POISONING,
     PROVOCATION,
     REMOVETRAP,
     SNOOPING,
     SPELLWEAVING,
     SPIRITSPEAK,
     STEALING,
     STEALTH,
     ATTRIBUTEID_STRENGTH,
     SWORDSMANSHIP,
     TACTICS,
     TAILORING,
     TAMING,
     TASTE_IDENTIFICATION,
     THROWING,
     TINKERING,
     TRACKING,
     VETERINARY,
     WRESTLING
};
endfunction

function AlterDexterityMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function AlterIntelligenceMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function AlterSkillTemporaryMod(who, skillid, delta)
  return AlterAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), Cint(delta*10));
endfunction

function AlterSkillTemporaryModBaseValue(who, skillid, delta_tenth)
  return AlterAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), delta_tenth);
endfunction

function AlterStrengthMod(who, delta)
  var ret := AlterAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, Cint(delta*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function GetBaseDexterity(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_DEXTERITY)/ 10);
endfunction

function GetBaseIntelligence(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE)/ 10);
endfunction

function GetBaseSkill(who, skillid)
  return Cint(GetBaseSkillBaseValue(who, skillid)/ 10);
endfunction

function GetBaseSkillBaseValue(who, skillid)
  return GetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetBaseStrength(who)
  return Cint(GetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH)/ 10);
endfunction

function GetDefaultPointsValue(skillid)
  var elem := GetSkillConfigElem(skillid);
  if(!elem)
    return 0;
  else
    return Cint(elem.DefaultPoints);
  endif
endfunction

function GetDexterity(who)
  return GetAttribute(who, ATTRIBUTEID_DEXTERITY);
endfunction

function GetDexterityMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY)/ 10);
endfunction

function GetEffectiveSkill(who, skillid)
  return GetAttribute(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetEffectiveSkillBaseValue(who, skillid)
  var attributeid  := GetAttributeIdBySkillId(skillid);
  var baseskill  := GetAttributeBaseValue(who, attributeid);
  var tempmod  := GetAttributeTemporaryMod(who, attributeid);
  var intinsicmod  := GetAttributeIntrinsicMod(who, attributeid);
  var effective  := Cint(baseskill + tempmod + intinsicmod);
  if(effective < 0)
    return 0;
  else
    return effective;
  endif
endfunction

function GetHp(who)
  return Cint(GetVital(who, VITALID_LIFE)/ 100);
endfunction

function GetIntelligence(who)
  return GetAttribute(who, ATTRIBUTEID_INTELLIGENCE);
endfunction

function GetIntelligenceMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE)/ 10);
endfunction

function GetMana(who)
  return(GetVital(who, "Mana")/ 100);
endfunction

function GetMaxHp(who)
  return Cint(GetVitalMaximumValue(who, VITALID_LIFE)/ 100);
endfunction

function GetMaxMana(who)
  return Cint(GetVitalMaximumValue(who, VITALID_MANA)/ 100);
endfunction

function GetMaxStamina(who)
  return Cint(GetVitalMaximumValue(who, VITALID_STAMINA)/ 100);
endfunction

function GetNeededRawPointsToIncrease(basevalue)
  if(basevalue < 200)
    return 20.48;
  elseif(basevalue >= 2800)
    return Cdbl(0x7fffffff);
  else
    return Cdbl(Pow(2.0, Cint(basevalue/100))* 10.24);
  endif
endfunction

function GetSkillConfigElem(skillid)
  var skdefcfg := ReadConfigFile(CFGFILESPEC_SKILLSDEF);
  if(!skdefcfg)
    return skdefcfg;
  else
    return FindConfigElem(skdefcfg, skillid);
  endif
endfunction

function GetAttributeIdsWithoutStatIds()
  return { ALCHEMY,
     ANATOMY,
     ANIMALLORE,
     ARCHERY,
     ARMSLORE,
     BEGGING,
     BLACKSMITHY,
     BOWCRAFT,
     CAMPING,
     CARPENTRY,
     CARTOGRAPHY,
     COOKING,
     DETECTING_HIDDEN,
     DISCORDANCE,
     EVALUATING_INTELLIGENCE,
     FENCING,
     FISHING,
     FORENSIC_EVALUATION,
     HEALING,
     HERDING,
     HIDING,
     INSCRIPTION,
     ITEM_IDENTIFICATION,
     LOCKPICKING,
     LUMBERJACKING,
     MACEFIGHTING,
     MAGERY,
     RESISTING_SPELLS,
     MEDITATION,
     MINING,
     MUSICIANSHIP,
     PARRYING,
     PEACEMAKING,
     POISONING,
     PROVOCATION,
     REMOVETRAP,
     SNOOPING,
     SPIRITSPEAK,
     STEALING,
     STEALTH,
     SWORDSMANSHIP,
     TACTICS,
     TAILORING,
     TAMING,
     TASTE_IDENTIFICATION,
     TINKERING,
     TRACKING,
     VETERINARY,
     WRESTLING,
      NECROMANCY,
      FOCUS,
      CHIVALRY,
      BUSHIDO,
      NINJITSU,
      SPELLWEAVING,
      MYSTICISM,
      IMBUING,
      THROWING
};
endfunction

function GetHpRegenRate(who)
  return GetVitalRegenRate(who, VITALID_LIFE);
endfunction

function GetManaRegenRate(who)
  return GetVitalRegenRate(who, VITALID_MANA);
endfunction

function GetSkillIdByAttributeId(attributeid)
  case(Lower(attributeid))
    ALCHEMY:                    return SKILLID_ALCHEMY;
    ANATOMY:                    return SKILLID_ANATOMY;
    ANIMALLORE:                 return SKILLID_ANIMALLORE;
    ITEM_IDENTIFICATION:        return SKILLID_ITEM_IDENTIFICATION;
    ARMSLORE:                   return SKILLID_ARMSLORE;
    PARRYING:                   return SKILLID_PARRYING;
    BEGGING:                    return SKILLID_BEGGING;
    BLACKSMITHY:                return SKILLID_BLACKSMITHY;
    BOWCRAFT:                   return SKILLID_BOWCRAFT;
    PEACEMAKING:                return SKILLID_PEACEMAKING;
    CAMPING:                    return SKILLID_CAMPING;
    CARPENTRY:                  return SKILLID_CARPENTRY;
    CARTOGRAPHY:                return SKILLID_CARTOGRAPHY;
    COOKING:                    return SKILLID_COOKING;
    DETECTING_HIDDEN:           return SKILLID_DETECTING_HIDDEN;
    DISCORDANCE:                return SKILLID_DISCORDANCE;
    EVALUATING_INTELLIGENCE:    return SKILLID_EVALUATING_INTELLIGENCE;
    HEALING:                    return SKILLID_HEALING;
    FISHING:                    return SKILLID_FISHING;
    FORENSIC_EVALUATION:        return SKILLID_FORENSIC_EVALUATION;
    HERDING:                    return SKILLID_HERDING;
    HIDING:                     return SKILLID_HIDING;
    PROVOCATION:                return SKILLID_PROVOCATION;
    INSCRIPTION:                return SKILLID_INSCRIPTION;
    LOCKPICKING:                return SKILLID_LOCKPICKING;
    MAGERY:                     return SKILLID_MAGERY;
    RESISTING_SPELLS:           return SKILLID_RESISTING_SPELLS;
    TACTICS:                    return SKILLID_TACTICS;
    SNOOPING:                   return SKILLID_SNOOPING;
    MUSICIANSHIP:               return SKILLID_MUSICIANSHIP;
    POISONING:                  return SKILLID_POISONING;
    ARCHERY:                    return SKILLID_ARCHERY;
    SPIRITSPEAK:                return SKILLID_SPIRITSPEAK;
    STEALING:                   return SKILLID_STEALING;
    TAILORING:                  return SKILLID_TAILORING;
    TAMING:                     return SKILLID_TAMING;
    TASTE_IDENTIFICATION:       return SKILLID_TASTE_IDENTIFICATION;
    TINKERING:                  return SKILLID_TINKERING;
    TRACKING:                   return SKILLID_TRACKING;
    VETERINARY:                 return SKILLID_VETERINARY;
    SWORDSMANSHIP:              return SKILLID_SWORDSMANSHIP;
    MACEFIGHTING:               return SKILLID_MACEFIGHTING;
    FENCING:                    return SKILLID_FENCING;
    WRESTLING:                  return SKILLID_WRESTLING;
    LUMBERJACKING:              return SKILLID_LUMBERJACKING;
    MINING:                     return SKILLID_MINING;
    MEDITATION:                 return SKILLID_MEDITATION;
    STEALTH:                    return SKILLID_STEALTH;
    REMOVETRAP:                 return SKILLID_REMOVETRAP;
    NECROMANCY:                  return SKILLID_NECROMANCY;
    FOCUS:                       return SKILLID_FOCUS;
    CHIVALRY:                    return SKILLID_CHIVALRY;
    BUSHIDO:                     return SKILLID_BUSHIDO;
    NINJITSU:                   return SKILLID_NINJITSU;
    SPELLWEAVING:               return SKILLID_SPELLWEAVING;
    MYSTICISM:                  return SKILLID_MYSTICISM;
    IMBUING:                    return SKILLID_IMBUING;
    THROWING:                   return SKILLID_THROWING;
  endcase
  var ret        := error;
  ret.+errortext := "No equivalent skillid found for attributeid " + attributeid;
  return ret;
endfunction

function GetSkillIds()
  return { SKILLID_ALCHEMY,
     SKILLID_ANATOMY,
     SKILLID_ANIMALLORE,
     SKILLID_ITEM_IDENTIFICATION,
     SKILLID_ARMSLORE,
     SKILLID_PARRYING,
     SKILLID_BEGGING,
     SKILLID_BLACKSMITHY,
     SKILLID_BOWCRAFT,
     SKILLID_PEACEMAKING,
     SKILLID_CAMPING,
     SKILLID_CARPENTRY,
     SKILLID_CARTOGRAPHY,
     SKILLID_COOKING,
     SKILLID_DETECTING_HIDDEN,
     SKILLID_DISCORDANCE,
     SKILLID_EVALUATING_INTELLIGENCE,
     SKILLID_HEALING,
     SKILLID_FISHING,
     SKILLID_FORENSIC_EVALUATION,
     SKILLID_HERDING,
     SKILLID_HIDING,
     SKILLID_PROVOCATION,
     SKILLID_INSCRIPTION,
     SKILLID_LOCKPICKING,
     SKILLID_MAGERY,
     SKILLID_RESISTING_SPELLS,
     SKILLID_TACTICS,
     SKILLID_SNOOPING,
     SKILLID_MUSICIANSHIP,
     SKILLID_POISONING,
     SKILLID_ARCHERY,
     SKILLID_SPIRITSPEAK,
     SKILLID_STEALING,
     SKILLID_TAILORING,
     SKILLID_TAMING,
     SKILLID_TASTE_IDENTIFICATION,
     SKILLID_TINKERING,
     SKILLID_TRACKING,
     SKILLID_VETERINARY,
     SKILLID_SWORDSMANSHIP,
     SKILLID_MACEFIGHTING,
     SKILLID_FENCING,
     SKILLID_WRESTLING,
     SKILLID_LUMBERJACKING,
     SKILLID_MINING,
     SKILLID_MEDITATION,
     SKILLID_STEALTH,
     SKILLID_REMOVETRAP,
    SKILLID_NECROMANCY,
    SKILLID_FOCUS,
    SKILLID_CHIVALRY,
    SKILLID_BUSHIDO,
    SKILLID_NINJITSU,
    SKILLID_SPELLWEAVING,
    SKILLID_MYSTICISM,
    SKILLID_IMBUING,
    SKILLID_THROWING
  };
endfunction

function GetSkillTemporaryMod(who, skillid)
  return Cint(GetSkillTemporaryModBaseValue(who, skillid)/ 10);
endfunction

function GetSkillTemporaryModBaseValue(who, skillid)
  return GetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetStamina(who)
  return Cint(GetVital(who, VITALID_STAMINA)/ 100);
endfunction

function GetSkillIntrinsicMod(who, skillid)
  return Cint(GetSkillIntrinsicModBaseValue(who, skillid)/ 10);
endfunction

function GetSkillIntrinsicModBaseValue(who, skillid)
  return GetAttributeIntrinsicMod(who, GetAttributeIdBySkillId(skillid));
endfunction

function GetStrength(who)
  return GetAttribute(who, ATTRIBUTEID_STRENGTH);
endfunction

function GetStrengthMod(who)
  return Cint(GetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH)/ 10);
endfunction

function GetTrueBaseSkill(who, skillid)
  return Cdbl(GetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid))/ 10.0);
endfunction

function GetTrueEffectiveSkill(who, skillid)
  var id   := GetAttributeIdBySkillId(skillid);
  var base := GetAttributeBaseValue(who, id);
  var mod1 := GetAttributeTemporaryMod(who, id);
  var mod2 := GetAttributeIntrinsicMod(who, id);
  var true := Cdbl((base + mod1 + mod2)/ 10.0);
  if(true < 0.0)
    return 0.0;
  else
    return true;
  endif
endfunction

function GetTrueSkillTemporaryMod(who, skillid)
  return Cdbl(GetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid))/ 10.0);
endfunction

function GetVitalIds()
  return { VITALID_LIFE, VITALID_MANA, VITALID_STAMINA };
endfunction

function ModifyAttributeTemporaryMod(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ Cint(moda*10)));
endfunction

function ModifyAttributeTemporaryModBaseValue(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ moda));
endfunction

function ModifyDexterityMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_DEXTERITY, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function ModifyIntelligenceMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_INTELLIGENCE, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function ModifySkillTemporaryMod(who, skillid, moda)
  return ModifyAttributeTemporaryModBaseValue(who, GetAttributeIdBySkillId(skillid), Cint(moda*10));
endfunction

function ModifySkillTemporaryModBaseValue(who, skillid, moda)
  return ModifyAttributeTemporaryModBaseValue(who, GetAttributeIdBySkillId(skillid), moda);
endfunction

function ModifyStrengthMod(who, moda)
  if(ModifyAttributeTemporaryModBaseValue(who, ATTRIBUTEID_STRENGTH, Cint(moda*10)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function RawToBaseSkill(rawvalue)
  var scale := Cint(Log10(rawvalue)/Log10(2));
  if(scale <= 11)
    return Cint(rawvalue / 20.48);
  else
    return Cint((rawvalue / Pow(2,scale)+ scale - 11)* 100);
  endif
endfunction

function SetBaseDexterity(who, dexterity)
  dexterity := Cint(dexterity * 10);
  if(dexterity < 10)
    dexterity := 10;
  elseif(dexterity > CORE_SETTINGS_MAX_ATTRIBUTE)
    dexterity := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  var ret := SetAttributeBaseValue(who, ATTRIBUTEID_DEXTERITY, dexterity);
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetBaseDexterityBaseValue(who, dexterity)
  if(dexterity < 0)
    dexterity := 0;
  endif
  if(SetAttributeBaseValue(who, ATTRIBUTEID_DEXTERITY, Cint(dexterity)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetBaseIntelligence(who, intelligence)
  intelligence := Cint(intelligence * 10);
  if(intelligence < 10)
    intelligence := 10;
  elseif(intelligence > CORE_SETTINGS_MAX_ATTRIBUTE)
    intelligence := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  var ret := SetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE, intelligence);
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetBaseIntelligenceBaseValue(who, intelligence)
  if(intelligence < 0)
    intelligence := 0;
  endif
  if(SetAttributeBaseValue(who, ATTRIBUTEID_INTELLIGENCE, Cint(intelligence)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

//  These need to be converted to use attribute ids soon
function SetBaseSkill(who, skillid, skill)
  return SetBaseSkillBaseValue(who, skillid, Cint(skill*10));
endfunction

function SetBaseSkillBaseValue(who, skillid, skill_tenth)
  if(skill_tenth < CORE_SETTINGS_MIN_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MIN_ATTRIBUTE;
  elseif(skill_tenth > CORE_SETTINGS_MAX_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  return SetAttributeBaseValue(who, GetAttributeIdBySkillId(skillid), Cint(skill_tenth));
endfunction

// The following function will replace SetBaseSkill
function SetBaseAttribute(who, skillid, skill)
  return SetBaseAttributeBaseValue(who, skillid, Cint(skill*10));
endfunction

function SetBaseAttributeBaseValue(who, attribid, skill)
  var skill_tenth := Cint(skill*10);
  if(skill_tenth < CORE_SETTINGS_MIN_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MIN_ATTRIBUTE;
  elseif(skill_tenth > CORE_SETTINGS_MAX_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  return SetAttributeBaseValue(who, attribid, Cint(skill_tenth));
endfunction

function SetBaseStrength(who, strngth)
  strngth := Cint(strngth * 10);
  if(strngth < 10)
    strngth := 10;
  elseif(strngth > CORE_SETTINGS_MAX_ATTRIBUTE)
    strngth := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  var ret := SetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH, strngth);
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetBaseStrengthBaseValue(who, strngth)
  if(strngth < 0)
    strngth := 0;
  endif
  if(SetAttributeBaseValue(who, ATTRIBUTEID_STRENGTH, Cint(strngth)))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetDexterityMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetDexterityModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_DEXTERITY, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetHp(who, hp)
  return SetVital(who, VITALID_LIFE, Cint(hp*100));
endfunction

function SetIntelligenceMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetIntelligenceModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_INTELLIGENCE, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function SetMana(who, amount)
  SetVital(who, "Mana", cint(amount*100.0));
endfunction

function SetSkillTemporaryMod(who, skillid, mod_amt)
  return SetAttributeTemporaryMod(who, skillid, Cint(mod_amt*10));
endfunction

function SetSkillTemporaryModBaseValue(who, skillid, mod_tenth)
  return SetAttributeTemporaryMod(who, GetAttributeIdBySkillId(skillid), mod_tenth);
endfunction

function SetStamina(who, stamina)
  return SetVital(who, VITALID_STAMINA, Cint(stamina*100));
endfunction

function SetStrengthMod(who, mod_amt)
  var ret := SetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, Cint(mod_amt*10));
  if(ret)
    DoRecalcVitals(who);
  endif
  return ret;
endfunction

function SetStrengthModBaseValue(who, moda)
  if(SetAttributeTemporaryMod(who, ATTRIBUTEID_STRENGTH, moda))
    DoRecalcVitals(who);
    return 1;
  else
    return 0;
  endif
endfunction

function GetStaminaRegenRate(who)
  return GetVitalRegenRate(who, VITALID_STAMINA);
endfunction

function SetHpRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_LIFE, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetManaRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_MANA, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetStaminaRegenRate(who, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+VITALID_STAMINA, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

function SetVitalRegenRate(who, vitalid, hundreth_per_minute)
  if(hundreth_per_minute < CORE_SETTINGS_MIN_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MIN_REGEN_RATE;
  elseif(hundreth_per_minute > CORE_SETTINGS_MAX_REGEN_RATE)
    hundreth_per_minute := CORE_SETTINGS_MAX_REGEN_RATE;
  endif
  SetObjProperty(who, CPROP_NAME_PREFIX_REGEN_RATE+vitalid, hundreth_per_minute);
  return DoRecalcVitals(who);
endfunction

// Added from POL 097 attributes_ex.inc

/*
 * AP_SkillIdToAttributeName(skill_number)
 *
 * Purpose
 * Gets the name of the attribute that is associated with a skill number.
 * uoskills.cfg -> Attributes.cfg
 *
 * Parameters
 * skill_number:	Skill ID to convert
 *
 * Return value
 * Returns a string.
 *
 */
function AP_SkillIdToAttributeName(skill_number)
	var skills_cfg := ReadConfigFile(":attributes:uoskills");
	if ( skills_cfg.errortext )
		SysLog("Error::AP_IDToAttributeName() - Unable to open :attributes:uoskills.cfg ->"+skills_cfg.errortext);
	endif

	var cfg_elem := skills_cfg[skill_number];
	if ( cfg_elem.errortext )
		SysLog("Error::AP_IDToAttributeName() - unable to find cfg elem ["+skill_number+"] ->"+cfg_elem.errortext);
	endif

	return cfg_elem.Attribute;
endfunction

/*
 * AP_AttributeNameToSkillId(attribute_name)
 *
 * Purpose
 * Gets the skill number (ID) that an attribute name is associated with.
 * Attributes.cfg -> uoskills.cfg
 *
 * Parameters
 * attribute_name:	Attribute name to convert.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_AttributeNameToSkillId(attribute_name)
	var cfg_elem := AP_GetAttributeCfgElem(attribute_name);

	return cfg_elem.SkillId;
endfunction

// The following was added from POL 097 attributes.inc

/* AP_GetAttributesCfgFile()
 * 
 * Purpose
 * Loads attributes.cfg
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function AP_GetAttributesCfgFile()
	var cfg_file := ReadConfigFile(":attributes:attributes");
	if ( cfg_file.errortext )
		SysLog("Error::AP_GetAttributesCfgFile() - Unable to open attributes.cfg ->"+cfg_file.errortext);
	endif

	return cfg_file;
endfunction

/*
 * AP_GetAttributeCfgElem(attribute_name, cfg_file)
 *
 * Purpose
 * Retrieves a config elem for an attribute in attributes.cfg
 *
 * Parameters
 * attribute_name:	Name of the attribute to get the elem of.
 * cfg_file:		Optional parameter - reference to a config already read in.
 *
 * Return value
 * Returns a config file elem.
 *
 */
function AP_GetAttributeCfgElem(attribute_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := AP_GetAttributesCfgFile();
	endif
	
	var cfg_elem := cfg_file[attribute_name];
	if ( cfg_elem.errortext )
		SysLog("Error::AP_GetAttributeCfgElem() - unable to find cfg elem ["+attribute_name+"] ->"+cfg_elem.errortext);
	endif
	
	return cfg_elem;
endfunction

/* AP_GetAttributesCfgFile()
 * 
 * Purpose
 * Loads the attributes datafile.
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function AP_GetAttributesDataFile()
	var data_file := DFOpenDataFile(":attributes:attributes", DF_CREATE);
	if ( data_file.errortext )
		SysLog("Error::AP_GetAttributesDataFile() - Unable to open attributes.txt ->"+data_file.errortext);
	endif

	return data_file;
endfunction

/*
 * AP_GetAttributeDataElem(attribute_name, cfg_file)
 *
 * Purpose
 * Retrieves a datafile elem from the attributes package.
 *
 * Parameters
 * attribute_name:	Name of the elem to retrieve.
 *
 * Return value
 * Returns a datafile elem.
 *
 */
function AP_GetAttributeDataElem(elem_name)
	var data_file := AP_GetAttributesDataFile();
	var data_elem := DFFindElement(data_file, elem_name, DF_CREATE);
	
	if ( data_elem.errortext )
		SysLog("Error::AP_GetAttributeDataElem() - unable to find elem ["+elem_name+"] ->"+data_elem.errortext);
	endif
	
	return data_elem;
endfunction

/*
 * AP_ListAttributesByType(type)
 *
 * Purpose
 * Gets the name of all attributes of a certain type.
 *
 * Parameters
 * Type:	String containing the type of attributes to list.
 *
 * Return value
 * Returns an array
 *
 */
function AP_ListAttributesByType(type:="")
	if ( !type )
		var cfg_file := ReadConfigFile(":attributes:attributes");
		return GetConfigStringKeys(cfg_file);
	endif
	
	type := Lower(type);
	var data_elem := AP_GetAttributeDataElem("Categories");
	return data_elem.GetProp(type);
endfunction

function SkillCheck(who, attribid, difficulty, points)

    var always_gain_until;
    var min_chance;
    var max_chance, skillid;
    // POL used to use skill numbers instead of attribute names to identify skills.
    // Until we convert this to using attribute strings we need to convert the passed attribid to a skill number
    skillid := GetSkillIdByAttributeId(attribid);
	
    
    // Get the "always gain until" constant and multiply it by 10.
    always_gain_until := Cint(Cdbl(skill_settings.FreeGainUntil) * 10);// Usually 10
    min_chance        := Cint(Cint(skill_settings.MinChance) * 10);// Usually 0
    max_chance        := Cint(Cint(skill_settings.MaxChance) * 10);// Ususlly 100
	
    
    // Convert difficulty to a real.
    difficulty := CDbl(difficulty);
	
    // If difficulty does not convert to a real properly then return with a "fail"
    if(difficulty == error)
		return 0;
    // If difficulty is zero then we don't even test and return with success but no skill gain
    elseif(difficulty == 0.0)
		return 1;
    else
		difficulty := difficulty * 10.0;
    endif
	
    var chance;
	
    // Retrieve the base skill value. Same as GetAttributeBaseValue
    var baseskill  := GetBaseSkillBaseValue(who, skillid);
	
    // Retrieve the temporary mod for the passed skill. Same as GetAttributeTemporaryMod
    var tempmod  := GetSkillTemporaryModBaseValue(who, skillid);
	
    // Retrieve the intrinsic mod for the passed skill. Same as GetAttributeIntrinsicMod
    var intrisicmod  := GetSkillIntrinsicModBaseValue(who, skillid);
	
    // The effective skill is the sum of base and all mods
    var effective  := baseskill + tempmod + intrisicmod;
	
    // Here we take hunger into account when determining success
    var hunger  := CInt(GetObjProperty(who, "hunger"));
    case(hunger)
	0: effective := (effective - 50); points := (points * 0.95);
	1: effective := (effective + 50); points := (points * 1.05);
	5:
	6: effective := (effective - 10); points := (points * 0.95);
	7:
	8: effective := (effective - 30); points := (points * 0.97);
	9:
	10: effective := (effective - 50); points := (points * 0.95);
    endcase
    // Pretty obvious
    if(effective < 0)
	effective := 0;
    endif
    if(difficulty < 0)
		chance := effective + ((difficulty+10) * 15);
	else
		chance := (effective - difficulty) * 2.5 + 500;
		var temp_chance;
		if(effective != baseskill)
			temp_chance := (baseskill - difficulty) * 2.5 + 500;
		else
		  temp_chance := chance;
		endif
		if(temp_chance < min_chance)
			temp_chance := min_chance;
		elseif(temp_chance > max_chance)
			temp_chance := max_chance;
		endif
		if(points != 0)
			points  := points * ((1000-temp_chance) / 1000);
		endif
    endif
    // Keep chance in the range of min_chance and max_chance
    if(chance < min_chance)
	chance := min_chance;
    elseif(chance > max_chance)
	chance := max_chance;
    endif
    // If the dice roll falls under chance then award points
    if(RandomInt(1000) < chance)
//    SendSysMessage (who, "effective := " + effective, color := 88);
//    SendSysMessage (who, "difficulty := " + difficulty, color := 88);
//    SendSysMessage (who, "points := " + points, color := 88);
//    if(1)
	// The following is supposed to give us a bell curve of no gains if ABS(diff-skill) > 10
/*	if(effective > (difficulty + 100))
	    points := 0;
	elseif(effective < (difficulty + 100))
	    points := 0;
	endif
*/

	if(SLPH_GetLockState(who, skillid) == 2)
	    SendSysMessage(who, "You have the skill " + attribid + " locked.", 2, 56);
	    SendSysMessage(who, "You will not gain points in that skill.", 2, 56);
	    points := 0;
	endif

	// Hopefully this will prevent skills from being higher than individual skill caps.
	if(baseskill >= CInt(Cdbl(AP_GetSkillCap(attribid)) * 10))
	    SetBaseSkillBaseValue(who, skillid, CInt(Cdbl(AP_GetSkillCap(attribid)) * 10));
	    points := 0;
	endif

	if(points > 0)
	    // We want it to be harder to gain as the person gets closer to GM skill.
	    if(effective > 98)
			points := CInt(points * 5);
	    elseif(effective > 95)
			points := CInt(points * 10);
	    elseif(effective > 90)
			points := CInt(points * 20);
	    elseif(effective > 85)
			points := CInt(points * 30);
	    elseif(effective > 75)
			points := CInt(points * 40);
	    elseif(effective > 50)
			points := CInt(points * 50);
		else
			points := CInt(points * 60);
	    endif
	    if(!points)
			points := 1;
	    endif
		
	    AwardPoints(who, skillid, points);
	endif
	return 1;
    // Otherwise check baseskill against always_gain_until (agu) and if baseskill is less than agu then award points
    else
	if(SLPH_GetLockState(who, skillid) == 2)
	    SendSysMessage(who, "You have the skill " + attribid + " locked.", 2, 56);
	    SendSysMessage(who, "You will not gain points in that skill.", 2, 56);
	    points := 0;
	endif
	if(baseskill < always_gain_until)
	    if(points > 0)
		AwardPoints(who, skillid, points);
	    endif
	endif
	return 0;
    endif
endfunction

////////////////////////////////////////////////////////////////////
// The following are functions added from the POL 0.97 Distro.    //
// Yes, for now these are being added to "kluge" in things from   //
// the Distro that I want but eventually I hope to implement the  //
// full set of these functions so I can use new scripts as they   //
// are written.                                                   //
////////////////////////////////////////////////////////////////////

// Skills

/*
 * AP_GetSkill(mobile, skill_name)
 *
 * Purpose
 * Retrieves the base value of the skill plus its mod value.
 *
 * Parameters
 * mobile:	Mobile reference to retrieve the points from.
 * skill_name:	Name of the skill to retrieve the points for.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_GetSkill(who, skill_name)
	return AP_GetTrueSkill(who, skill_name)+AP_GetSkillMod(who, skill_name);
endfunction

/*
 * AP_GetTrueSkill(mobile, skill_name)
 *
 * Purpose
 * Retrieves the base value of the skill and turns it into a usable number.
 *
 * Parameters
 * mobile:	Mobile reference to retrieve the points from.
 * skill_name:	Name of the skill to retrieve the points for.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_GetTrueSkill(who, skill_name)
	return CDbl(GetAttributeBaseValue(who, skill_name)) / 10.0;
endfunction

/*
 * AP_GetSkillMod(mobile, skill_name)
 *
 * Purpose
 * Retrieves the base value of the skill mod and turns it into a usable number.
 *
 * Parameters
 * mobile:	Mobile reference to retrieve the points from.
 * skill_name:	Name of the skill to retrieve the modifier points for.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_GetSkillMod(who, skill_name)
	return CDbl(GetAttributeTemporaryMod(who, skill_name)) / 10.0;
endfunction


// Stats

/*
 * AP_GetStat(mobile, stat_name)
 *
 * Purpose
 * Retrieves the base value of the stat plus its mod value.
 *
 * Parameters
 * mobile:		Mobile reference to retrieve the points from.
 * stat_name:	Name of the stat to retrieve the points for.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_GetStat(who, stat_name)
	return AP_GetTrueStat(who, stat_name)+AP_GetStatMod(who, stat_name);
endfunction

/*
 * AP_GetTrueStat(mobile, stat_name)
 *
 * Purpose
 * Retrieves the base value of the stat and turns it into a usable number.
 *
 * Parameters
 * mobile:		Mobile reference to retrieve the points from.
 * stat_name:	Name of the stat to retrieve the points for.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_GetTrueStat(who, stat_name)
	return CDbl(GetAttributeBaseValue(who, stat_name)) / 10.0;
endfunction

/*
 * AP_GetStatMod(mobile, stat_name)
 *
 * Purpose
 * Retrieves the base value of the stat mod and turns it into a usable number.
 *
 * Parameters
 * mobile:		Mobile reference to retrieve the points from.
 * stat_name:	Name of the stat to retrieve the modifier points for.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_GetStatMod(who, stat_name)
	return CDbl(GetAttributeTemporaryMod(who, stat_name)) / 10.0;
endfunction

/*
 * AP_SetTrueStat(mobile, stat_name, points)
 *
 * Purpose
 * Sets the value of the stat.
 * Example: AP_SetTrueStat(mobile, "strength", 44.5)
 *
 * Parameters
 * mobile:		Mobile reference to set the points on.
 * stat_name:	Name of the stat to set the points on.
 * points:		Number of points to set the stat at.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_SetTrueStat(mobile, stat_name, points)
	SetAttributeBaseValue(mobile, stat_name, CInt(CDbl(points) * 10.0));
	RecalcVitals(mobile);
endfunction

/*
 * AP_SetStatMod(mobile, stat_name, points)
 *
 * Purpose
 * Sets the modifier value for the stat.
 * Example: 	AP_SetStatMod(mobile, "strength", -15.3)
 *
 * Parameters
 * mobile:		Mobile reference to set the modifier points on.
 * stat_name:	Name of the stat to set the modifier points on.
 * points:		Number of points to set the modifier stat at.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_SetStatMod(mobile, stat_name, points)
	SetAttributeTemporaryMod(mobile, stat_name, CInt(CDbl(points) * 10.0));
	RecalcVitals(mobile);
endfunction

/*
 * AP_ModifyStatMod(mobile, stat_name, points)
 *
 * Purpose
 * Modifies the modifier value for the stat.
 * Example: 	AP_SetStatMod(mobile, "strength", -15.3)
 *
 * Parameters
 * mobile:	Mobile reference to adjust the modifier points on.
 * stat_name:	Name of the stat to adjust the modifier points on.
 * points:	Number of points to adjust the modifier by.
 *
 * Return value
 * Returns the previous value
 *
 */
function AP_ModifyStatMod(mobile, stat_name, points)
	var orig_mod := AP_GetStatMod(mobile, stat_name);
	var new_mod := CDbl(points)+CDbl(orig_mod);

	AP_SetStatMod(mobile, stat_name, new_mod);

	return orig_mod;
endfunction


// Vitals

//$Id: vitals.inc 373 2006-06-17 18:27:33Z austinheilman $

/*
 * AP_GetStatCap(stat_name)
 *
 * Purpose
 * Gets the number a stat caps at.
 *
 * Parameters
 * stat_name: Name of the stat to check
 *
 * Return value
 * Returns a number or error
 *
 */
function AP_GetStatCap(skill_name)
	var cfg_elem := AP_GetSettingsCfgElem("Stats");

	// Check if there is a cap for that particular skill.
	var cap_val := GetConfigReal(cfg_elem, skill_name);

	return cap_val;
endfunction

/*
 * AP_CheckStatCap(mobile, stat_name)
 *
 * Purpose
 * Checks if a stat is greater than or equal to its maximum value in settings.cfg
 * If no stat_name is passed, it will check the totals.
 *
 * Parameters
 * mobile:		Mobile to check the stats on
 * stat_name:	Optional parameter - name of the stat to check.
 *
 * Return value
 * Returns 1 if the stat is >= to the cap
 *
 */
function AP_CheckStatCap(mobile, stat_name:="")
	var cfg_elem := AP_GetSettingsCfgElem("Stats");

	if ( stat_name )
		var cap := AP_GetStatCap(stat_name);
		return ( AP_GetTrueStat(mobile, stat_name) >= cap );
	else
		var total := 0.0;
		foreach attribute_name in ( AP_ListAttributesByType("Stat") )
			total += AP_GetTrueStat(mobile, attribute_name);
			SleepMS(2);
		endforeach

		return ( total >= GetConfigReal(cfg_elem, "TotalCap") );
	endif
endfunction

/*===============================================================
* Current Version
* VITALS.INC - v1.0
* Updated 9/24/2005 6:52PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use cfgfile;

/* AP_GetVitalsCfgFile()
 * 
 * Purpose
 * Loads vitals.cfg
 *
 * Parameters
 *
 * Return value
 * Config file
 *
 */
function AP_GetVitalsCfgFile()
	var cfg_file := ReadConfigFile(":attributes:vitals");
	if ( cfg_file.errortext )
		SysLog("Error::AP_GetVitalsCfgFile() - Unable to open vitals.cfg ->"+cfg_file.errortext);
	endif

	return cfg_file;
endfunction

/*
 * AP_GetVitalCfgElem(vital_name, cfg_file)
 *
 * Purpose
 * Retrieves a config elem for an vital in vitals.cfg
 *
 * Parameters
 * vital_name:	Name of the vital to get the elem of.
 * cfg_file:	Optional parameter - reference to a config already read in.
 *
 * Return value
 * Returns a config file elem.
 *
 */
function AP_GetVitalCfgElem(vital_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := AP_GetVitalsCfgFile();
	endif
	
	var cfg_elem := cfg_file[vital_name];
	if ( cfg_elem.errortext )
		SysLog("Error::AP_GetVitalsCfgElem() - unable to find cfg elem ["+vital_name+"] ->"+cfg_elem.errortext);
	endif
	
	return cfg_elem;
endfunction

/*
 * AP_GetVitalNames()
 *
 * Purpose
 * Retrieves a list of all vitals in vitals.cfg
 *
 * Parameters
 *
 * Return value
 * Returns an array of strings
 *
 */
function AP_GetVitalNames()
	return GetConfigStringKeys(AP_GetVitalsCfgFile());
endfunction

/*
 * AP_GetVital(mobile, vital_name)
 *
 * Purpose
 * Retrieves the number of vital points a mobile has.
 *
 * Parameters
 * mobile:		Mobile reference to retrieve the points from.
 * vital_name:	Name of the vital to retrieve the points for.
 *
 * Return value
 * Returns a double.
 *
 */
function AP_GetVital(mobile, vital_name)
	return CDbl(GetVital(mobile, vital_name)) / 100.0;
endfunction

/*
 * AP_GetVitalMaximumValue(mobile, vital_name)
 *
 * Purpose
 * Retrieves the maximum number of vital points a mobile can have.
 *
 * Parameters
 * mobile:		Mobile reference to check.
 * vital_name:	Name of the vital to retrieve the max. points for.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_GetVitalMaximumValue(mobile, vital_name)
	return CDbl(GetVitalMaximumValue(mobile, vital_name)) / 100.0;
endfunction

/*
 * AP_GetVitalRegenRate(mobile, vital_name)
 *
 * Purpose
 * Retrieves the (base + mod) regen rate of a vital on a mobile.
 *
 * Parameters
 * mobile:	Mobile reference to check.
 * vital_name:	Name of the vital to retrieve the regen rate of.
 *
 * Return value
 * Returns an integer.
 *
 */
function AP_GetVitalRegenRate(mobile, vital_name)
	return GetVitalRegenRate(mobile, vital_name);
endfunction

/*
 * AP_SetVital(mobile, vital_name, points)
 *
 * Purpose
 * Sets vital points on a mobile.
 *
 * Parameters
 * mobile:	Mobile reference to set the vital on.
 * vital_name:	Name of the vital to set.
 * points:	Number of points to set the vital to.
 *
 * Return value
 * Returns result of the SetVital() core function.
 *
 */
function AP_SetVital(mobile, vital_name, amount)
	return SetVital(mobile, vital_name, CInt(CDbl(amount)*100.0));
endfunction

/*
 * AP_ConsumeVital(mobile, vital_name, points)
 *
 * Purpose
 * Attempts to remove points from a vital.
 *
 * Parameters
 * mobile:	Mobile reference to consume the vital on.
 * vital_name:	Name of the vital to consume.
 * points:	Number of points to attempt to consume.
 *
 * Return value
 * Returns 1 on success
 *
 */
function AP_ConsumeVital(mobile, vital_name, points)
	points := CDbl(points);
	var cur_amount := AP_GetVital(mobile, vital_name);
	
	if ( points > cur_amount )
		return error{"errortext":="Insufficient points - unable to consume."};
	else
		cur_amount := cur_amount-points;
		AP_SetVital(mobile, vital_name, cur_amount);
		return 1;
	endif
endfunction

/*
 * AP_RefreshVitals(mobile)
 *
 * Purpose
 * Sets all vitals on a mobile to their maximum values.
 *
 * Parameters
 * mobile:	Mobile to refresh.
 *
 * Return value
 * Returns 1
 *
 */
function AP_RefreshVitals(mobile)
	var vital_cfg := ReadConfigFile(":attributes:vitals");
	foreach elem_name in GetConfigStringKeys(vital_cfg)
		AP_SetVital(mobile, elem_name, AP_GetVitalMaximumValue(mobile, elem_name));
		SleepMS(2);
	endforeach

	return 1;
endfunction

/*
 * AP_GetSettingsCfgFile(engine_name)
 *
 * Purpose
 * Reads in :attributes:configs/settings.cfg
 *
 * Parameters
 *
 * Return value
 * A config file reference.
 *
 */
function AP_GetSettingsCfgFile()
	var settings_cfg := ReadConfigFile(":attributes:settings");

	if ( settings_cfg.errortext )
		SysLog("Error::AP_GetSettingsCfgFile() - Unable to open [:attributes:settings.cfg] ->"+settings_cfg.errortext);
	endif

	return settings_cfg;
endfunction

/*
 * AP_GetSettingsCfgElem(elem_name, cfg_file)
 *
 * Purpose
 * Retrieves an elem from a config file. 
 *
 * Parameters
 * elem_name:	A string matching the elem name to be retrieved.
 * cfg_file:	Optional parameter - reference to a config already read in by AP_GetSettingsCfgFile()
 *
 * Return value
 * A config file elem reference.
 *
 */
function AP_GetSettingsCfgElem(elem_name, byref cfg_file:=0)
	if ( !cfg_file )
		cfg_file := AP_GetSettingsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];

	if ( elem.errortext )
		SysLog("Error::AP_GetSettingsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext);
	endif

	return elem;
endfunction

/*
 * AP_GetSkillCap(skill_name)
 *
 * Purpose
 * Gets the number a skill caps at.
 *
 * Parameters
 * skill_name: Name of the skill to check
 *
 * Return value
 * Returns a number or error
 *
 */
function AP_GetSkillCap(skill_name:="")
	var cfg_elem := AP_GetSettingsCfgElem("Skills");
	var cap_val;
	
	if ( skill_name )
		// Check if there is a cap for that particular skill.
		cap_val := GetConfigReal(cfg_elem, skill_name);
	
		if ( !cap_val )
			// No skill specific cap so get the general one.
			cap_val := GetConfigReal(cfg_elem, "SkillCap");
		endif
	else
		cap_val := GetConfigReal(cfg_elem, "SkillCap");
	endif
	
	return cap_val;
endfunction

function AP_SetTrueSkill( mobile, skill_name, points )

  var result;
	if( points >= 0 )
		result := SetAttributeBaseValue( mobile, skill_name, CInt( CDbl( points ) * 10.0 + 0.1 ));
	else
		result := SetAttributeBaseValue( mobile, skill_name, CInt( CDbl( points ) * 10.0 - 0.1 ));
	endif
 
	RecalcVitals( mobile );
	return result;
endfunction

