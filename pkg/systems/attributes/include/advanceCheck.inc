// $Id: advanceCheck.inc 1050 2007-04-23 22:45:05Z tekproxy $

/*===============================================================
* Current Version
* ADVANCECHECK.INC - v1.0
* Updated 9/28/2005 8:29PM
*
* -- Revision v1.0 --
* Austin:
*  Created include file
===============================================================*/

use uo;
use os;
use util;
use math;

include ":attributes:attributes_ex";
include ":skilllocks:common";

/*
 * CheckStatsForSkill(mobile, skill_name)
 *
 * Purpose
 * Performs a stat check on all stats that are listed within a skill's attributes.cfg elem.
 *
 * Parameters
 * mobile:	Mobile to do the stat checks on.
 * skill_name:	Skill to get the stat advance list from.
 *
 * Return value
 * Returns 1 on success, 0 on failure
 *
 */
function CheckStatsForSkill(mobile, skill_name)
	var settings := AP_GetSettingsCfgElem("Stats");

	var next_check := CInt(GetObjProperty(mobile, "#LastStatGain"))+GetConfigInt(settings, "GainWait");
	if ( next_check > ReadGameClock() )
		SkillDbgMsg(mobile, "Stat advance-wait timer has not yet expired.");
		return 0;
	endif

	foreach stat_entry in (AP_GetSkillStats(skill_name))
		var stat_name := _stat_entry_iter;
		var check_chance := stat_entry[1];
		var stat_mult := CDbl(RandomDiceRoll(stat_entry[2]));

		var roll := RandomInt(100)+1;

		if ( roll <= check_chance )
			SkillDbgMsg(mobile, "("+roll+" < "+check_chance+"%) Checking stat "+stat_name);
			CheckStatAdvancement(mobile, stat_name, stat_mult);
		else
			SkillDbgMsg(mobile, "("+roll+" > "+check_chance+"%) skipping stat check for "+stat_name);
		endif

		SleepMS(10);
	endforeach

	return 1;
endfunction

/*
 * CheckStatAdvancement(mobile, stat_name, stat_mult)
 *
 * Purpose
 * Does an advancement check on a stat.
 *
 * Parameters
 * mobile:	Mobile to do the stat advancement check on.
 * stat_name:	Name of the stat to do the advancement check on.
 * stat_mult:	Multiplier to effect the chance of a gain.
 *		(rolled dice from an attributes.cfg StatCheck entry)
 *
 * Return value
 * Returns 1 on success, 0 on failure
 *
 */
function CheckStatAdvancement(mobile, stat_name, stat_mult)
	if ( AP_CheckStatCap(mobile, stat_name) )
		SkillDbgMsg(mobile, "Stat cap for stat '"+stat_name+"' reached. No advance." );
		return 0;
	elseif ( AP_CheckStatCap(mobile) )
		SkillDbgMsg(mobile, "Total stat cap reached. No advance." );
		return 0;
	endif

	var settings := AP_GetSettingsCfgElem("Stats");
	var stat_cap := AP_GetStatCap(stat_name);
	var stat_value := AP_GetTrueStat(mobile, stat_name);
	var stat_left := stat_cap - stat_value; // Points until it is capped
	stat_mult *= CDbl(settings.GainSpeed);

	SkillDbgMsg(mobile, "Stat Cap: "+stat_cap);
	SkillDbgMsg(mobile, "Stat Current: "+stat_value);
	SkillDbgMsg(mobile, "Left to cap: "+stat_left+" ->"+stat_cap+"-"+stat_value);

	var chance := ((stat_left * stat_mult) / (stat_cap / 2));
	if ( chance < 1.0 )
		chance := 1.0;
	endif
	SkillDbgMsg(mobile, "Chance: "+chance);
	SkillDbgMsg(mobile, "---");
	var random := RandomFloat(100.0);
	SkillDbgMsg(mobile, "Check %age: "+random);

	if ( random < chance )
		var advance_factor := CDbl(settings.AdvanceFactor);
		var advance := CDbl(0.1 + (0.1 * CDbl(CInt(chance / advance_factor))));
		if ( advance > stat_left )
			advance := stat_left;
		elseif ( advance < 0.1 )
			advance := 0.1;
		endif

		var temp := stat_value + advance;
		while ( temp == AP_GetTrueStat(mobile, stat_name) )
			// Kludge for an old core precision bug.
			// Makes sure it always goes up atleast 0.1.
			temp += 0.1;
			SleepMS(10);
		endwhile

		SkillDbgMsg(mobile, "Passed. Advance "+stat_name+": "+CStr(advance));
		AP_SetTrueStat(mobile, stat_name, temp);
		SetObjProperty(mobile, "#LastStatGain", ReadGameClock());

		return 1;
	endif

	SkillDbgMsg(mobile, "Failed. No advance.");
	return 0;
endfunction

/*
 * CheckSkillAdvance(mobile, skill_name, award_diff, pass_chance, pretend)
 *
 * Purpose
 * Does an advancement check on a skill and considers skill locks.
 *
 * Parameters
 * mobile:	Mobile to do the skill advancement check on.
 * skill_name:	Name of the skill to check.
 * award_diff:	Difficulty to use for advancement award.
 * pass_chance:	Chance of getting a gain check.
 * pretend:	Do not actually make any skill changes (for testing).
 *
 * Return value
 * Returns amount advanced on success, 0 on failure
 *
 */
function CheckSkillAdvance(mobile, skill_name, award_diff, pass_chance, pretend:=0)
	var skill_id := AP_AttributeNameToSkillId(skill_name);
	var skill_lock_state := SLPH_GetLockState(mobile, skill_id);

	if ( skill_lock_state != 0 )
		SkillDbgMsg(mobile, "Skill lock state is not set to raise. Aborting.");
		return 0;
	endif
	
	var settings := AP_GetSettingsCfgElem("Skills");

	if ( !pretend )
		var next_check := CInt(GetObjProperty(mobile, "#LastSkillGain"))+GetConfigInt(settings, "GainWait");
		if ( next_check > ReadGameClock() )
			SkillDbgMsg(mobile, "Skill advance-wait timer has not yet expired.");
			return 0;
		endif
	endif
	
	var skill_cap := AP_GetSkillCap(skill_name);
	var true_skill := AP_GetTrueSkill(mobile, skill_name);
	var limit_left := CDbl(skill_cap - true_skill);
	
	var adv_check := RandomFloat(100.0);

	var multiplier := CDbl(settings.GainSpeed);
	var advance_factor := CDbl(settings.AdvanceFactor);
	
	// Determine amount of skill to gain
	var advance := 0.1;
	if (award_diff > true_skill)
		// For every advance_factor points difference between award_diff and true skill
		// there is +0.1 skill gain.
		advance := CDbl(0.1 * (1.0 + CInt((award_diff - true_skill) / advance_factor)));
	endif

	if (advance > limit_left)
		advance := limit_left;
	elseif (advance < 0.1)
		advance := 0.1;
	endif

	// Check if we advance the skill
	if ( true_skill < CDbl(settings.FreeGainUntil) )
		SkillDbgMsg(mobile, "Skill is < "+CDbl(settings.FreeGainUntil)+". Allowing for advancement.");
		
		if ( (true_skill + advance > settings.FreeGainUntil) && !(adv_check <= pass_chance) )
			limit_left := settings.FreeGainUntil - true_skill;
			
			if ( limit_left < 0.1 )
				SkillDbgMsg(mobile, "No free points left and did not pass advance check.");
			endif
		endif
	elseif ( DifficultyTooEasy(true_skill, award_diff) )
		SkillDbgMsg(mobile, "No challenge. No advance. True skill ("+true_skill+") > "+award_diff);
		return 0;
	elseif ( adv_check <= pass_chance )
		SkillDbgMsg(mobile, "Passed advance check: +"+(pass_chance - adv_check)+"%");
	else
		SkillDbgMsg(mobile, "Did not pass skill advance check.");
		return 0;
	endif

	SkillDbgMsg(mobile, "Skill Cap: "+skill_cap+"%");
	SkillDbgMsg(mobile, "Left to cap: "+limit_left+" ->"+skill_cap+"-"+true_skill);
	SkillDbgMsg(mobile, "Multiplier: "+multiplier);

	var int_chance := 0;
	var int_bonus := CDbl(settings.IntBonus);
	if ( int_bonus )
		int_chance := AP_GetStat(mobile, "Intelligence") / int_bonus;
	endif
	
	var task_chance := (100.0 - award_diff);
	var task_factor := CDbl(settings.TaskChanceFactor);
	if ( advance_factor )
		task_chance /= task_factor;
	endif
	
	var chance := CDbl((int_chance + task_chance) + (Abs(int_chance + task_chance) * multiplier));

	SkillDbgMsg(mobile, "INT gives "+int_chance+"%");
	SkillDbgMsg(mobile, "Task gives "+task_chance+"%");
	SkillDbgMsg(mobile, "Multiplier on "+(int_chance+task_chance)+" gives "+chance);
	SkillDbgMsg(mobile, "---");

	var check_roll := RandomFloat(100.0);
	SkillDbgMsg(mobile, "Check %age: "+check_roll);

	if ( check_roll > chance )
		SkillDbgMsg(mobile, "Failed advancement check. No advance.");
		return 0;
	endif
	
	SkillDbgMsg(mobile, "Passed. Advance "+skill_name+": "+advance);

	var lower_skill, lower_skill_decrease, new_skill;

	// If skill + advance is greater than specific/general skill caps or if it's already at a cap and the skill is set to raise
	if ( true_skill + advance > AP_GetSkillCap(skill_name) || true_skill + advance > AP_GetSkillCap() ||
		   AP_CheckSkillCap(mobile, skill_name) || AP_CheckSkillCap(mobile) )

		// Skill cap reached and skill is set to raise, so try to find another skill to lower
		SkillDbgMsg(mobile, "Skill cap for attribute '"+skill_name+"' already reached.");

		// NOTE: It might be a good idea to try to randomly find a skill set to lower instead of searching
		//       sequentially, which will always lower the same skill.
		var stop := AP_ListAttributesByType("Skill").Size();
		var i;
		for ( i := 0; i <= stop; i += 1 )
			if ( SLPH_GetLockState(mobile, i) == 1 )
				lower_skill := AP_SkillIdToAttributeName(i);

				if ( AP_GetTrueSkill(mobile, lower_skill) > 0.0 )
					// On OSI skills lower relative to their difficulty--NOT at the same rate.
					// To emulate this it would be necessary to define the difficulty for each
					// skill. This is where you would do the calculations.
					// Remember, whatever you set lower_skill_decrease to will be the amount that
					// the skill can raise, so don't set this to 0.1 unless you want all skills to
					// raise at that speed when lowering another skill.

					// Set to -1 to ignore lower_skill_decrease and always decrease the skill
					// the same amount that the other skill is raising.
					lower_skill_decrease := -1;
					break;
				else
					lower_skill := "";
				endif
			endif
		endfor

		if ( lower_skill )
			SkillDbgMsg(mobile, "Skill cap reached, but lowering "+lower_skill+" to compensate.");
			
			var lower_skill_value := AP_GetTrueSkill(mobile, lower_skill);
	
			// If the skill to be lowered is too low to allow for advancement, reduce advancement
			if ( advance > lower_skill_value )
				advance := lower_skill_value;
			endif
	
			// If the amount lower_skill can drop is less than advancement, reduce advancement
			if ( lower_skill_decrease != -1 && lower_skill_decrease < advance )
				advance := lower_skill_decrease;
			elseif ( lower_skill_decrease == -1 )
				lower_skill_decrease := advance;			
			endif
			
			new_skill := lower_skill_value - advance;
			while ( new_skill == AP_GetTrueStat(mobile, lower_skill) )
				// Kludge for an old core precision bug.
				new_skill -= 0.1;
				SleepMS(10);
			endwhile
	
			AP_SetTrueSkill(mobile, lower_skill, new_skill);
		else
			SkillDbgMsg(mobile, "Total skill cap already reached.");
			return 0;
		endif
	endif

	new_skill := true_skill + advance;
	while ( new_skill == AP_GetTrueStat(mobile, skill_name) )
		// Kludge for an old core precision bug.
		new_skill += 0.1;
		SleepMS(10);
	endwhile

	if ( !pretend )
		SetObjProperty(mobile, "#LastSkillGain", ReadGameClock());
		AP_SetTrueSkill(mobile, skill_name, new_skill);
	endif

	return advance;
endfunction