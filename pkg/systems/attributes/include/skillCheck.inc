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

const ADV_SKILL		:= 0x1;
const ADV_STATS		:= 0x2;
const ADV_ALL		:= ADV_SKILL+ADV_STATS; // 3
const ADV_DISABLE	:= 0x4; // 4 - because the default is ADV_ALL.

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
 * difficulty:
 * award_diff:		Advanced as if the difficulty were this value - defaults to difficulty.
 * advance_flags:	Controls the turning on/off of stat and skill advancements for the check.
 *
 * Return value
 * Returns > 0 if the skill check was successful
 *
 */
function SkillCheck( mobile, attrib_name, difficulty, award_diff:="", advance_flags:=ADV_ALL )
  var skill_check_result := 0;
	if( mobile.dead )
		SendSysMessage( mobile, "You can't use skills while dead.", 3, 33 );
        print("SkillCheck failed because the player is dead.");
		return -100;
	elseif( GetObjProperty( mobile, "NoGains" ))
		SendSysMessage( mobile, "Your skills are disabled.", 3, 33 );
        print("SkillCheck failed because the player is set to 'NoGain'.");
		return -100;
	elseif( mobile.IsA( POLCLASS_NPC ))
		if( GetObjProperty( mobile, "CanTrain" ))
				advance_flags := ADV_STATS;
		else
				advance_flags := ADV_DISABLE;
		endif
	endif

	// Difficulty checks
	if( difficulty != -1 )
		difficulty := CDbl( difficulty );
	endif

	var effective_skill := AP_GetSkill( mobile, attrib_name ),
	pass_chance := CalcSuccessPercent( mobile, attrib_name, difficulty, effective_skill ),
	check_roll := RandomDiceRoll( "1d100+1" );

	// if( !mobile.IsA( POLCLASS_NPC ) && !mobile.cmdlevel )
	if( !mobile.IsA( POLCLASS_NPC ) )
		if( !mobile.cmdlevel || ( mobile.cmdlevel && GetObjProperty( mobile, "StaffScriptTester" ) ) )

			var hunger := CInt( AP_GetVital( mobile, HUNGER ));
			if( !hunger )
				// hunger := 1;
				hunger := 9;
			endif
			// This determines how much hunger will affect success, I think :P
			var hc_settings := AP_GetSettingsCfgElem( "Hunger" ),
			hunger_setting := CInt( hc_settings.MaxHungerAffect ),
			hunger_value := CInt( pass_chance * CDbl( hunger_setting ));
			// Print("Hunger Val:"+hunger_value);

			// pass_chance -= CInt( hunger_value - CDbl( hunger_value * CDbl( hunger / 10 )));
			pass_chance -= CInt( hunger_value + CDbl( hunger_value * CDbl( hunger / 10 )));
			// Print("Pass chance 2:"+pass_chance);
		endif
	endif

	// Ensure skill- and stat-gain if it's a -1 skillcheck
	// Do this AFTER the CalcSuccessPercent() call so '-1' isn't messed up

	if( difficulty == -1 )
		difficulty := effective_skill;
	endif

	if( award_diff == "" )
		award_diff := AP_GetSkillDefaultAward( attrib_name );
	endif

	if( advance_flags & ADV_SKILL )
		CheckSkillAdvance( mobile, attrib_name, difficulty, CInt( award_diff ));
	endif
	if( advance_flags & ADV_STATS )
		CheckStatsForSkill( mobile, attrib_name );
	endif

  skill_check_result := (( pass_chance - check_roll ) + 1 );

  // print("Check skill result:"+skill_check_result);
  if( skill_check_result < 0)
    skill_check_result := 0;
  endif

	return skill_check_result;
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
function CalcSuccessPercent( unused mobile, unused attrib_name, difficulty, effective_skill )
	var sc_settings := AP_GetSettingsCfgElem( "Skills" );
	var min_chance := CInt( sc_settings.MinChance );
	var max_chance := CInt( sc_settings.MaxChance );
	var percent := CInt( 50.0 + (( effective_skill - difficulty ) * 1.5 )); // For every 5 points difference there will be a +-10 for percent
	// Note: variable declaration was wrong so this may need some testing
	/*
		Previous version redeclared percent inside if so it was local only.

		var neg1chance := CInt( sc_settings.MaxNeg1Chance ), <<< that little coma instead of semicolon :-)
		percent := CInt( neg1chance * effective_skill / 150 );
		After change we now have new formula that may need checking for difficulty == -1
	*/
	if( difficulty == -1 )
		var neg1chance := CInt( sc_settings.MaxNeg1Chance );
		percent := CInt( neg1chance * effective_skill / 150 );
	endif
	if( percent < min_chance )
		percent := min_chance;
	elseif( percent > max_chance )
		percent := max_chance;
	endif
	return percent;
endfunction
/*
 * DifficultyTooEasy( effective_skill, difficulty )
 *
 * Purpose
 * Determines if the efective_skill versus the difficulty
 * is too easy to accomplish the task.
 *
 * Parameters
 * effective_skill - The effective skill of the character.
 * difficulty - The difficulty of the task.
 *
 * Return value
 * Returns 0 if not too easy. Returns 1 if it is too easy.
 *
 */
function DifficultyTooEasy( effective_skill, difficulty )
	var sc_settings := AP_GetSettingsCfgElem( "Skills" );
        var max_diff := difficulty + CDbl( sc_settings.TooEasy );
        if( max_diff > effective_skill )
                return 0;
        endif
	return 1;
endfunction
/*
 * DifficultyTooHard( effective_skill, difficulty )
 *
 * Purpose
 * Determines if the efective_skill versus the difficulty
 * is too hard to accomplish the task.
 *
 * Parameters
 * effective_skill - The effective skill of the character.
 * difficulty - The difficulty of the task.
 *
 * Return value
 * Returns 0 if not too hard. Returns 1 if it is too hard.
 *
 */
function DifficultyTooHard(effective_skill, difficulty)
	var settings := AP_GetSettingsCfgElem("Skills");
	var max_diff := difficulty + CDbl( sc_settings.TooHard );
	if( max_diff <= effective_skill )
		return 0;
	endif
	return 1;
endfunction