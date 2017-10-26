// $Id: skillCheck.inc 1050 2007-04-23 22:45:05Z tekproxy $

/*===============================================================
* Current Version
* SKILLCHECK.INC - v1.0
* Updated 9/24/2005 6:52PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use util;
use cfgfile;

CONST ADV_SKILL		:= 0x1;
CONST ADV_STATS		:= 0x2;
CONST ADV_ALL		:= ADV_SKILL+ADV_STATS; // 3
CONST ADV_DISABLE	:= 0x4; // 4 - because the default is ADV_ALL.

var g_skill_debug	:= 0;

/*
 * SkillCheck(mobile, attrib_name, difficulty, award_diff, advance_flags)
 *
 * Purpose
 * Performs a skill check on an attribute and performs skill and stat advancements.
 * <= 0 means the skill check failed > 0 means it was successful.
 *
 * Parameters
 * mobile:		Mobile reference to do the skill check for.
 * attribute_name:	Name of the attribute to perform the check on.
 * award_diff:		Advanced as if the difficulty were this value - defaults to difficulty.
 * advance_flags:	Controls the turning on/off of stat and skill advancements for the check.
 *
 * Return value
 * Returns > 0 if the skill check was successful
 *
 */
function SkillCheck(mobile, attrib_name, difficulty, award_diff:="", advance_flags:=ADV_ALL)
	if ( mobile.dead )
		SendSysMessage(mobile, "You can't use skills while dead.");
		return -100;
	elseif ( GetObjProperty(mobile, "DisableSkills") )
		SendSysMessage(mobile, "Your skills are disabled.");
		return -100;
	elseif ( GetObjProperty(mobile, "NoGains") )
		advance_flags := ADV_DISABLE;
	endif
	
	// Global skill-debug ( for SkillDbgMsg() )
	g_skill_debug := (GetObjProperty(mobile, "SkillDebug") != error);
	
	var effective_skill := AP_GetTrueSkill(mobile, attrib_name);
	
	// Difficulty checks
	if ( difficulty == -1 )
		// -1 = Check skill against its self.
		SkillDbgMsg(mobile, "Skill based check. Difficulty = -1");
  elseif( difficulty == -3 )
		SkillDbgMsg(mobile, "Skill based check. Difficulty = -3");
	else
		difficulty := CDbl(difficulty);
	endif
	
	var pass_chance := CalcSuccessPercent(mobile, attrib_name, difficulty, effective_skill);
	var check_roll := RandomInt(100)+1;
	var pass_success := (pass_chance - check_roll)+1;
	// Ensure skill- and stat-gain if it's a -1 skillcheck
	// Do this AFTER the CalcSuccessPercent() call so '-1' isn't messed up
	if ( difficulty == -1 )
		difficulty := effective_skill;
	endif
	if ( award_diff == "" )
		award_diff := difficulty;
	endif
	
	if ( award_diff == -3 )
		award_diff := CInt(AP_GetAttributeCfgElem(attrib_name).AwardDefault);
    //SendSysMessage(mobile, "Award default for " + attrib_name + " is " + CStr(award_diff), 2, 88);
	endif
	
	SkillDbgMsg(mobile, "--");
	SkillDbgMsg(mobile, "Difficulty: "+difficulty+"% (Award: "+award_diff+")");
	SkillDbgMsg(mobile, "Effective Skill: "+effective_skill+"%");
	SkillDbgMsg(mobile, "Success Chance: "+pass_chance+"%");
	SkillDbgMsg(mobile, "Check Roll: "+check_roll);
	SkillDbgMsg(mobile, "Success: "+pass_success);
	SkillDbgMsg(mobile, "---");
	
	if ( !(advance_flags & ADV_SKILL) )
		SkillDbgMsg(mobile, "Skill advance disabled.");
	else
		CheckSkillAdvance(mobile, attrib_name, award_diff, pass_chance);
	endif
		
	if ( !(advance_flags & ADV_STATS) )
		SkillDbgMsg(mobile, "Stat advancement disabled.");
	else
		CheckStatsForSkill(mobile, attrib_name);
	endif
	
	return pass_success;
endfunction

/*
 * CalcSuccessPercent(mobile, attrib_name, difficulty, effective_skill)
 *
 * Purpose
 * Retrieves the percentage chance of success based on mobile's skill and the difficulty.
 *
 * Parameters
 *
 * Return value
 * Returns 1
 *
 */
function CalcSuccessPercent(mobile, attrib_name, difficulty, effective_skill)
	var settings := AP_GetSettingsCfgElem("Skills");
	
	var percent := 0;
	if ( difficulty == -1 )
		var max_skill := AP_GetSkillCap(attrib_name);
		var neg1chance := CDbl(settings.MaxNeg1Chance);
		
		percent := CInt(neg1chance * effective_skill / max_skill);
	elseif ( DifficultyTooHard(effective_skill, difficulty) )
		SkillDbgMsg(mobile, "Too hard.");
		percent := 0;
	elseif ( DifficultyTooEasy(effective_skill, difficulty) )
		SkillDbgMsg(mobile, "Too easy.");
		percent := 100;
	else
		// For every 5 points difference there will be a +-10 for percent
		percent := CInt(50.0 + ((effective_skill - difficulty) * 2.5));
	endif
	
	if ( percent < settings.MinChance )
		percent := CInt(settings.MinChance);
	elseif ( percent > settings.MaxChance )
		percent := CInt(settings.MaxChance);
	endif
	
	return percent;
endfunction

function DifficultyTooEasy(effective_skill, difficulty)
	var settings := AP_GetSettingsCfgElem("Skills");
	return ( (effective_skill + CDbl(settings.TooEasy)) > difficulty );
endfunction

function DifficultyTooHard(effective_skill, difficulty)
	var settings := AP_GetSettingsCfgElem("Skills");
	return ( (effective_skill + CDbl(settings.TooHard)) < difficulty );
endfunction

/*
 * SkillDbgMsg(mobile, message)
 *
 * Purpose
 * Checks to see if it should send a skill debug message to a mobile.
 *
 * Parameters
 * mobile:	Mobile to send the messages to.
 * message:	Debug message.
 *
 * Return value
 * Returns 1
 *
 */
function SkillDbgMsg(mobile, message)
	if ( g_skill_debug )
		SendSysMessage(mobile, "Skill Debug: "+message);
	endif
	
	return 1;
endfunction
