/* $Id: config.inc 896 2005-11-04 07:03:59Z muaddiblsd $
 *
 * Purpose
 * Provide various functions and constants for the shard's behaviour.
 *
 */
use cfgfile;
use os;

include ":logger:exception";

CONST SHARD_CFG     := ":config:config/shardconfig";
CONST DEFAULT_KEY   := "Main";

enum CFG_ENTRIES
    SHARD_NAME      := "ShardName",
    SHARD_TIMEZONE  := "TimeZone",

    LINGER_STAFF    := "StaffLingerTime",
    LINGER_PLAYER   := "PlayerLingerTime",

    REGEN_HITS      := "HitsRegenRate",
    REGEN_MANA      := "ManaRegenRate",
    REGEN_STAM      := "StamRegenRate",

    SAVE_TIME       := "SaveTime",
    SAVE_ANNOUNCE   := "SaveAnnounce"
endenum


/*
 * QueryConfigString(what)
 *
 * Purpose
 * Returns a config string from the config file passed. If no
 * config file, or elem name, is passed, it uses the defaults.
 * shardconfig.cfg is the default config file, Main is the
 * default elem name.
 *
 * Parameters
 * what:	The name of the config elem to return
 * cfg_file:	Optional. Packaged name of the config file.
 * elem_name:	Optional. Elem name to look up in the config.
 *
 * Return value
 * The config file's value for the desired elem.
 *
 */
function QueryConfigString(what, cfg_file:=SHARD_CFG, elem_name:=DEFAULT_KEY)
	var ret_value := GetConfigString(FindConfigElem(ReadConfigFile(cfg_file), elem_name), what);
	if( ret_value.errortext )
		SysLog("Error::QueryConfigString() - Unable to open ["+cfg_file+"] ->"+ret_value.errortext);
	endif
	return ret_value;
endfunction


/*
 * QueryConfigInt(what)
 *
 * Returns a config integer from the config file passed. If no
 * config file, or elem name, is passed, it uses the defaults.
 * shardconfig.cfg is the default config file, Main is the
 * default elem name.
 *
 * Parameters
 * what:	The name of the config elem to return
 * cfg_file:	Optional. Packaged name of the config file.
 * elem_name:	Optional. Elem name to look up in the config.
 *
 * Return value
 * The config file's value for the desired elem.
 *
 */
function QueryConfigInt(what, cfg_file:=SHARD_CFG, elem_name:=DEFAULT_KEY)
	var ret_value := GetConfigInt(FindConfigElem(ReadConfigFile(cfg_file), elem_name), what);
	if( ret_value.errortext )
		SysLog("Error::QueryConfigInt() - Unable to open ["+cfg_file+"] ->"+ret_value.errortext);
	endif
	return ret_value;
endfunction


/*
 * GetShardName()
 *
 * Purpose
 * Returns the shard's name as specified in the config file.
 *
 * Parameters
 * No parameters.
 *
 * Return value
 * The shard's name.
 *
 */
function GetShardName()
    return(QueryConfigString(SHARD_NAME));
endfunction


/*
 * GetStaffLingerTime()
 *
 * Purpose
 * Returns the linger time for staff in seconds.
 *
 * Parameters
 * No parameters.
 *
 * Return value
 * The staff linger time in seconds.
 *
 */
function GetStaffLingerTime()
    return(QueryConfigInt(LINGER_STAFF));
endfunction


/*
 * GetPlayerLingerTime()
 *
 * Purpose
 * Returns the linger time for players in seconds.
 *
 * Parameters
 * No parameters.
 *
 * Return value
 * The player linger time in seconds.
 *
 */
function GetPlayerLingerTime()
    return(QueryConfigInt(LINGER_PLAYER));
endfunction


/*
 * GetTimeZone()
 *
 * Purpose
 * Returns the timezone offset from the GMT time.
 *
 * Parameters
 * No parameters.
 *
 * Return value
 * The offset from GMT
 *
 */
function GetTimeZone()
    return(QueryConfigInt(SHARD_TIMEZONE));
endfunction


/*
 * GetDefaultRegenRate(stat)
 *
 * Purpose
 * Returns the default regen rate of a stat
 *
 * Parameters
 * stat:    The stat to query
 *
 * Return value
 * The default stat regen rate
 *
 */
function GetDefaultRegenRate(stat)
    if(stat != REGEN_HITS && stat != REGEN_MANA && stat != REGEN_STAM)
        return(Exception("config::GetDefaultRegenRate: Invalid stat '" + CStr(stat) + "'."));
    endif
    return(QueryConfigInt(stat));
endfunction


/*
 * GetSaveTime()
 *
 * Purpose
 * Returns the time between saves in minutes
 *
 * Parameters
 * No parameters
 *
 * Return value
 * The time between saves in minutes
 *
 */
function GetSaveTime()
    return(QueryConfigInt(SAVE_TIME));
endfunction


/*
 * GetSaveAnnounceTime()
 *
 * Purpose
 * Returns the time of the save announcement before a save in minutes
 *
 * Parameters
 * No parameters
 *
 * Return value
 * The offset of the save time and announce time
 *
 */
function GetSaveAnnounceTime()
    return(QueryConfigInt(SAVE_ANNOUNCE));
endfunction
