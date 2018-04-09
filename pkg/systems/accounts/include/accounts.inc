/* $Id: accounts.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 *
 */

use uo;
use os;

include ":accounts:settings";

// Max numbers of characters an account can have.
CONST ACCT_MAX_CHARACTERS	:= 6;

/*
 * CreateNewAccount(username, password, cmdlevel, expansion, email)
 *
 * Purpose
 * Used to create a new account and set appropriate settings on it.
 *
 * Parameters:
 * username:	Username of the account to make.
 * password:	Password to set on the account. Note it is case-sensitive!
 * cmdlevel:	Default command level for the account.
 * expansion:	UO Expansion the account should use. DEfault is 'T2A'
 * email:	Email address to set on the account.
 *
 * Return Value:
 * Returns error on failure.
 * Returns an account object on success.
 *
 */
function CreateNewAccount(username, password, cmdlevel:=0, expansion:="ML", email:="")
	var account := CreateAccount(username, password, 1);
	if ( account.errortext )
		return account;
	endif

	if ( cmdlevel )
		account.SetDefaultCmdLevel(CInt(cmdlevel));
	endif

	if ( expansion )
		account.Set_UO_Expansion(expansion);
	endif

	if ( email )
		account.SetProp("EMail", email);
	endif

	ACCT_SetLastLogin(account);

	return account;
endfunction

/*
 * ACCT_SetLastLogin(account, time)
 *
 * Purpose
 * Sets the time the account was last used.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 * time:	The last time the account was used. If 0 is passed, POLCore().systime is used.
 *
 * Return value
 * Returns 1 on success.
 * Returns error on failure.
 *
 */
function ACCT_SetLastLogin(account, time:=0)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	if ( !time )
		time := POLCore().systime;
	endif

	return account.SetProp("LastLogin", time);
endfunction

/*
 * ACCT_GetLastLogin(account)
 *
 * Purpose
 * Gets the time the account was last used.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 *
 * Return value
 * Returns an integer on success.
 * Returns error on failure.
 *
 */
function ACCT_GetLastLogin(account)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	return CInt(account.GetProp("LastLogin"));
endfunction

/*
 * ACCT_GetCharacters(account)
 *
 * Purpose
 * Gets a list of the characters in an account.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 *
 * Return value
 * Returns an array of mobile references on success.
 * Returns error on failure.
 *
 */
function ACCT_GetCharacters(account)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	var mobile_list := array;
	for i:=1 to ACCT_MAX_CHARACTERS
		var mobile := account.GetCharacter(i);
		if ( mobile )
			mobile_list.Append(mobile);
		endif
		SleepMS(2);
	endfor

	return mobile_list;
endfunction

/*
 * ACCT_GetNumCharacters(account)
 *
 * Purpose
 * Gets the number of characters on an account.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 *
 * Return value
 * Returns an integer on success.
 * Returns error on failure.
 *
 */
function ACCT_GetNumCharacters(account)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	var count := 0;
	for i:=1 to ACCT_MAX_CHARACTERS
		if ( account.GetCharacter(i) )
			count := count+1;
		endif
		SleepMS(2);
	endfor

	return count;
endfunction

/*
 * ACCT_DeleteAllCharacters(account)
 *
 * Purpose
 * Removes all characters from an account.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 *
 * Return value
 * Returns an integer on success.
 * Returns error on failure.
 *
 */
function ACCT_DeleteAllCharacters(account)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	var count := 0;
	for i:=1 to ACCT_MAX_CHARACTERS
		account.DeleteCharacter(i);
		SleepMS(2);
	endfor

	return count;
endfunction

/*
 * ACCT_DeleteAccount(account)
 *
 * Purpose
 * Runs acctDelete/acctDelete.ecl in all packages for any account cleanup.
 * Removes all characters from the account.
 * Removes the account.
 *
 * Parameters
 * account:	Can be an account reference or a string for an account name.
 *
 * Return value
 * Returns an integer on success.
 * Returns error on failure.
 *
 */
function ACCT_DeleteAccount(account)
	if ( TypeOf(account) != "AccountRef" )
		account := FindAccount(account);
		if ( account.errortext )
			return account;
		endif
	endif

	foreach package in ( POLCore().packages )
		Run_Script_To_Completion(":"+package+":acctDelete/acctDelete", account);
		SleepMS(2);
	endforeach

	ACCT_DeleteAllCharacters(account);

	return account.Delete();
endfunction


//////////////////////////////////////////////////////
// FindValidPlayer(player)
//
// Purpose: To validate that the player named actually
//          exists on an account.
//
// Input: A string containing the name of a possible player
//
// Returns: The player's serial if they exist.
//          or 0 if they do not belong to an account.
//
//////////////////////////////////////////////////////




function FindValidPlayer(player)
  var playerserial := 0;
	var acctlist := ListAccounts();
  var i := 1;
  
	// Stuff for verifying that the player exists
	foreach acctnam in acctlist
		var acctref := FindAccount(acctnam);
		for ( i := 1; i<=5; i:=i+1 )
			var playername := CStr(acctref.GetCharacter(i).name);
			if(!playername)
				continue;
			endif 
			if ( Lower(playername) == Lower(player) )
				playerserial := acctref.GetCharacter(i).serial;
				break;
			endif
		endfor
	endforeach

  return playerserial;
  
endfunction
