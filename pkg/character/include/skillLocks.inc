use uo;
use os;

include ":character:character";

/*
 * SL_GetLockState(player, skill_id)
 *
 * Purpose
 * Retrieves the skill lock state for a skill ID.
 *
 * Parameters
 * mobile:	Mobile to retrieve the settings for.
 * skill_id:	Skill ID to retrieve the flag for.
 *
 * Return Value
 * An integer
 *
 */
function SL_GetLockState( mobile, skill_id )

	var data_elem := CHARACTER_GetCharacterElem( mobile.serial ),
		skill_locks := Struct;

	if( data_elem.errortext )
		SendSysMessage( mobile, "Unable to access character's info -> "+data_elem.errortext );
		return 0;
	endif

	// +1 is added because skills start at 0 and arrays start at 1
	// Hex because cprop IDs need to be strings.
	skill_id := Hex( CInt( skill_id )+1 );

	skill_locks := data_elem.GetProp( "SkillLocks" );
	if( skill_locks.errortext )
		skill_locks := data_elem.SetProp( "SkillLocks", Struct );
		return Struct;
	endif

	return CInt( skill_locks[skill_id] );
endfunction

/*
 * SL_SaveLockState(player, skill_id, state)
 *
 * Purpose
 * Retrieves the skill lock state for a skill ID.
 *
 * Parameters
 * mobile:	Mobile to retrieve the settings for.
 * skill_id:	Skill ID to retrieve the flag for.
 * state:	State of the skill lock.
 *
 * Return Value
 * 1
 *
 */
function SL_SaveLockState(mobile, skill_id, state)
	var data_elem := CHARACTER_GetCharacterElem( mobile.serial ),
		data_prop := 0;

	if( data_elem.errortext )
		SendSysMessage( mobile, "Unable to access character's info -> "+data_elem.errortext );
		return 0;
	endif

	data_prop := data_elem.GetProp( "SkillLocks" );
	if( data_prop.errortext )
		data_prop := data_elem.SetProp( "SkillLocks", Struct );
		data_prop := data_elem.GetProp( "SkillLocks" );
	endif

	skill_id := Hex( CInt( skill_id )+1 );
	state := CInt( state );

	if( state == 0 )
		if( data_prop.Exists( skill_id ) )
			data_prop.Erase( skill_id );
			data_elem.SetProp( "SkillLocks", data_prop );
		endif
	else
		if( data_prop.Exists( skill_id ) )
			data_prop.Erase( skill_id );
		endif
		data_prop.Insert( skill_id, state );
		data_elem.SetProp( "SkillLocks", data_prop );
	endif

	return 1;
endfunction