use uo;
use datafile;

include ":character:character";

// forwhom is the player's profile.
// towhom is the player being sent the profile.
function RequestProfile( byref forwhom, byref towhom )

	var df_elem := 0,
		footer := Array,
		profile := Array;

	if( forwhom.IsA( POLCLASS_NPC ) )
		if( towhom.cmdlevel >= 3 )
			footer := CAscZ( "NPC Template "+forwhom.npctemplate );
			profile := CAscZ( "I do not have a profile yet." );
		endif
	else
		df_elem := CHARACTER_GetCharacterElem( forwhom.serial );
		if( df_elem.errortext )
			SendSysMessage( towhom, "Could not find the player's profile info: -> "+df_elem.errortext, 0x3, 0x22 );
			return 0;
		endif

		profile := df_elem.GetProp( "Profile" );
		if( profile.errortext )
			profile := "";
		endif
	endif

	if( towhom == forwhom )
		footer := CAscZ( "Enter your profile here. Any other player will be able to read this." );
	endif

	SendCharProfile( towhom, forwhom, forwhom.name, footer, profile );
endfunction

// forwhom is the player's profile.
// towhom is the player being sent the profile.
function UpdateProfile( byref forwhom, byref towhom, byref uctext )

	var df_elem := 0;

	if( forwhom != towhom )
		SendSysMessage( towhom, "You can only change your own profile.", 0x3, 0x32 );
		return 0;
	endif

	df_elem := CHARACTER_GetCharacterElem( forwhom.serial );
	if( df_elem.errortext )
		SendSysMessage( towhom, "Could not find the player's profile info: -> "+df_elem.errortext, 0x3, 0x22 );
		return 0;
	endif

	if( !uctext && !uctext.Size() )
		df_elem.SetProp( "Profile", "" );
		SendSysMessage( towhom, "Your profile has been deleted.", 0x3, 0x32 );
	else
		df_elem.SetProp( "Profile", uctext );
		SendSysMessage( towhom, "Your profile has been changed.", 0x3, 0x42 );
	endif
endfunction