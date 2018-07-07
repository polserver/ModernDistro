//$Id: vitals.inc 1064 2007-08-01 16:17:29Z tekproxy $

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
use vitals;
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

	var cfg_file := ReadConfigFile( ":attributes:vitals" );
	if( cfg_file.errortext )
		SysLog( "Error::AP_GetVitalsCfgFile() - Unable to open vitals.cfg ->"+cfg_file.errortext );
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
function AP_GetVitalCfgElem( vital_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := AP_GetVitalsCfgFile();
	endif

	var cfg_elem := cfg_file[vital_name];
	if( cfg_elem.errortext )
		SysLog( "Error::AP_GetVitalsCfgElem() - unable to find cfg elem ["+vital_name+"] ->"+cfg_elem.errortext );
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

	return GetConfigStringKeys( AP_GetVitalsCfgFile() );
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
function AP_GetVital( mobile, vital_name )

	return CDbl( GetVital( mobile, vital_name )) / 100.0;
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
function AP_GetVitalMaximumValue( mobile, vital_name )

	return CDbl( GetVitalMaximumValue( mobile, vital_name )) / 100.0;
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
function AP_GetVitalRegenRate( mobile, vital_name )

	return GetVitalRegenRate( mobile, vital_name );
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
function AP_SetVital( mobile, vital_name, amount )

	return SetVital( mobile, vital_name, CInt( CDbl( amount ) * 100.0 ));
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
function AP_ConsumeVital( mobile, vital_name, points )

	points := CDbl( points );
	var cur_amount := AP_GetVital( mobile, vital_name );

	if( points > cur_amount )
		return error{ "errortext":="Insufficient points - unable to consume." };
	else
		cur_amount := cur_amount - points;
		AP_SetVital( mobile, vital_name, cur_amount );
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
function AP_RefreshVitals( mobile )

	var vital_cfg := ReadConfigFile( ":attributes:vitals" );
	foreach elem_name in GetConfigStringKeys( vital_cfg )
		AP_SetVital( mobile, elem_name, AP_GetVitalMaximumValue( mobile, elem_name ));
		SleepMS(2);
	endforeach

	return 1;
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

const CORE_SETTINGS_MIN_REGEN_RATE := -30000;
const CORE_SETTINGS_MAX_REGEN_RATE := 30000;
const CPROP_NAME_PREFIX_REGEN_RATE := "NewRegenRateFor";



function GetHP(who)
  return AP_GetVital(who, VITALID_LIFE);
endfunction

function GetStamina(who)
  return AP_GetVital(who, VITALID_STAMINA);
endfunction

function GetMana(who)
  return AP_GetVital(who, VITALID_MANA);
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


function SetHP(who, stamina)
  return AP_SetVital(who, VITALID_LIFE, stamina);
endfunction

function SetStamina(who, stamina)
  return AP_SetVital(who, VITALID_STAMINA, stamina);
endfunction

function SetMana(who, stamina)
  return AP_SetVital(who, VITALID_MANA, stamina);
endfunction

function ApplyManaDamage(who, dmg)
  return SetMana(who, GetMana(who)-dmg);
endfunction

function ApplyStaminaDamage(who, dmg)
  return SetStamina(who, GetStamina(who)-dmg);
endfunction



//function GetStaminaRegenRate(who)
//  return GetVitalRegenRate(who, VITALID_STAMINA);
//endfunction

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

