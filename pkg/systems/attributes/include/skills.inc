//$Id: skills.inc 723 2006-08-22 02:11:00Z tekproxy $

/*===============================================================
* Current Version
* SKILLS.INC - v1.0
* Updated 9/24/2005 6:52PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;

include "include/eventID";

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
function AP_GetSkill( who, skill_name )

	return AP_GetTrueSkill( who, skill_name )+AP_GetSkillMod( who, skill_name );
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
function AP_GetTrueSkill( who, skill_name )

	return CDbl( GetAttributeBaseValue( who, skill_name )) / 10.0;
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
function AP_GetSkillMod( who, skill_name )

	return CDbl( GetAttributeTemporaryMod( who, skill_name )) / 10.0;
endfunction

/*
 * AP_SetTrueSkill(mobile, skill_name, points)
 *
 * Purpose
 * Sets the value of the skill.
 * Example: AP_SetTrueSkill(mobile, "strength", 44.5)
 *
 * Parameters
 * mobile:	Mobile reference to set the points on.
 * skill_name:	Name of the skill to set the points on.
 * points:	Number of points to set the skill at.
 *
 * Return value
 * The result of SetAttributeBaseValue()
 *
 */
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

/*
 * AP_SetSkillMod(mobile, skill_name, points)
 *
 * Purpose
 * Sets the modifier value for the skill.
 * Example: AP_SetSkillMod(mobile, "strength", -15.3)
 *
 * Parameters
 * mobile:	Mobile reference to set the modifier points on.
 * skill_name:	Name of the skill to set the modifier points on.
 * points:	Number of points to set the modifier skill at.
 *
 * Return value
 * The result of SetAttributeBaseValue()
 *
 */
function AP_SetSkillMod( mobile, skill_name, points )

	var result := SetAttributeTemporaryMod( mobile, skill_name, CInt( CDbl( points ) * 10 ));
	RecalcVitals( mobile );
	return result;
endfunction

/*
 * AP_ModifySkillMod(mobile, skill_name, points)
 *
 * Purpose
 * Modifies the modifier value for the skill.
 * Example: 	AP_SetStatMod(mobile, "alchemy", -15.3)
 *
 * Parameters
 * mobile:	Mobile reference to adjust the modifier points on.
 * stat_name:	Name of the skill to adjust the modifier points on.
 * points:	Number of points to adjust the modifier by.
 *
 * Return value
 * Returns the previous value
 *
 */
function AP_ModifySkillMod( mobile, stat_name, points )

	var orig_mod := AP_GetStatMod( mobile, stat_name );
	var new_mod := CDbl( points ) + CDbl( orig_mod );

	AP_SetSkillMod( mobile, stat_name, new_mod );

	return orig_mod;
endfunction

/*
 * AP_GetSkillScript(attribute_name)
 *
 * Purpose
 * Gets the script that a skill uses.
 *
 * Parameters
 * attribute_name:	Name of the attribute to get the script for.
 *
 * Return value
 * Returns a string.
 *
 */
function AP_GetSkillScript( attribute_name )

	var cfg := AP_GetAttributeCfgElem( attribute_name );
	return cfg.Script;
endfunction

/*
 * AP_GetSkillStats(attribute_name)
 *
 * Purpose
 * Retrieves a dictionary from the attribute's config elem
 * Key is the stat name, inside is an array containing the settings for it.
 * dict["StatName"] => array{gain chance, award points dice}
 *
 * Parameters
 * attribute_name:	Name of the skill to retrieve stat settings for.
 *
 * Return value
 * Returns a dictionary.
 *
 */
function AP_GetSkillStats( attribute_name )

	var cfg_elem := AP_GetAttributeCfgElem( attribute_name );
	var stat_entries := GetConfigStringDictionary( cfg_elem, "StatCheck" );
	foreach textline in ( stat_entries )
		textline := SplitWords( textline );
		SleepMS(2);
	endforeach

	return stat_entries;
endfunction

function AP_GetSkillDefaultAward( skill_name )

	var elem_info := AP_GetAttributeCfgElem( skill_name );
	if( !elem_info )
		return elem_info;
	endif

	if( elem_info.Title )
		return CInt( elem_info.AwardDefault / 5 );
	else
		return error{"errortext":="Attribute does not have a 'AwardDefault' entry."};
	endif
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
function AP_GetSkillCapByName(skill_name:="")
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



//////////////////////////////////////////////////////
//
//  These are wrapper functions to suport function
// calls for scripts written prior to Austin's
// attributes package for Distro 0.98.
// Hopefully this will provide a transition phase
// and eventually lead to those scripts being
// rewritten to take advantage of direct calls
// to the proper functions.
//
//////////////////////////////////////////////////////
const CORE_SETTINGS_MIN_ATTRIBUTE  := 0;
const CORE_SETTINGS_MAX_ATTRIBUTE  := 60000;
const CORE_SETTINGS_MIN_VITAL      := 100;
const CORE_SETTINGS_MAX_VITAL      := 10000000;




/////////////////////////////////////////////////////////
//
// Function returns an array of the skill numbers
// from uoskills.cfg
//
/////////////////////////////////////////////////////////

function GetSkillIDs()

var uoskillsfile := ReadConfigFile(":attributes:uoskills");
return GetConfigIntKeys(uoskillsfile);

endfunction

function GetEffectiveSkill(who, skillid)
  return AP_GetSkill(who, AP_SkillIdToAttributeName(skillid));
endfunction

function GetBaseSkill(who, skillid)
  return Cint(AP_GetTrueSkill(who, AP_SkillIdToAttributeName(skillid)));
endfunction

function GetBaseSkillBaseValue(who, skillid)
  return AP_GetTrueSkill(who, AP_SkillIdToAttributeName(skillid) * 10);
endfunction

function GetTrueEffectiveSkill(who, skillid)
  var id   := AP_SkillIdToAttributeName(skillid);
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

function SetBaseSkill(who, skillid, skill)
  return SetBaseSkillBaseValue(who, skillid, Cint(skill*10));
endfunction

function SetBaseSkillBaseValue(who, skillid, skill_tenth)
  if(skill_tenth < CORE_SETTINGS_MIN_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MIN_ATTRIBUTE;
  elseif(skill_tenth > CORE_SETTINGS_MAX_ATTRIBUTE)
    skill_tenth := CORE_SETTINGS_MAX_ATTRIBUTE;
  endif
  return SetAttributeBaseValue(who, AP_SkillIdToAttributeName(skillid), Cint(skill_tenth));
endfunction

function ModifyAttributeTemporaryMod(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ Cint(moda*10)));
endfunction

function ModifyAttributeTemporaryModBaseValue(who, attributeid, moda)
  return SetAttributeTemporaryMod(who, attributeid, Cint(GetAttributeTemporaryMod(who, attributeid)+ moda));
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
    var attributeid := AP_SkillIDToAttributeName(skillid);
    if((attributeid != ATTRIBUTEID_STRENGTH) and (attributeid != ATTRIBUTEID_DEXTERITY) and (attributeid != ATTRIBUTEID_INTELLIGENCE))
		if(!who.npctemplate)
			var skillarray := GetObjProperty(who,"SkillArray");
			if(!skillarray)
			skillarray := {};
			foreach thing in List_of_Skills
				if(GetAttributeBaseValue(who, thing) > 0)
				skillarray.append(AP_AttributeNameToSkillId(thing));
				endif
			endforeach
			SetObjProperty(who,"SkillArray", skillarray);
			endif
			var skill := AP_AttributeNameToSkillId(attributeid);
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
  return AwardRawAttributePoints(who, AP_SkillIdToAttributeName(skillid), rawpoints, skillid);
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


/*
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
    var attributeid := AP_SkillIDToAttributeName(skillid);
    if((attributeid != ATTRIBUTEID_STRENGTH) and (attributeid != ATTRIBUTEID_DEXTERITY) and (attributeid != ATTRIBUTEID_INTELLIGENCE))
      if(!who.npctemplate)
        var skillarray := GetObjProperty(who,"SkillArray");
        if(!skillarray)
          skillarray := {};
          foreach thing in List_of_Skills
            if(GetAttributeBaseValue(who, thing) > 0)
              skillarray.append(AP_AttributeNameToSkillId(thing));
            endif
          endforeach
          SetObjProperty(who,"SkillArray", skillarray);
        endif
  //			var skill := AP_SkillIdToAttributeName(attributeid);
        var skill := AP_AttributeNameToSkillId(attributeid);
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

function GetNeededRawPointsToIncrease(basevalue)
  if(basevalue < 200)
    return 20.48;
  elseif(basevalue >= 2800)
    return Cdbl(0x7fffffff);
  else
    return Cdbl(Pow(2.0, Cint(basevalue/100))* 10.24);
  endif
endfunction

function AwardRawSkillPoints(who, skillid, rawpoints)
  return AwardRawAttributePoints(who, AP_AttributeNameToSkillId(skillid), rawpoints, skillid);
endfunction
*/
function GetSkillConfigElem(skillid)
//  var skdefcfg := ReadConfigFile(":attributes:uoskills");
  var skdefcfg := ReadConfigFile(":attributes:skills");
  if(!skdefcfg)
    return skdefcfg;
  else
    return FindConfigElem(skdefcfg, skillid);
  endif
endfunction


