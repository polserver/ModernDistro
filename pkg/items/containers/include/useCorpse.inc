use uo;
use os;

//include ":areas:managment";		// Pkg required by Destiny's Reach scripts. ?Might implement later.

function OpenContainer( mobile, corpse )

	var owner := corpse.GetOwner();
	if( !owner )
		// No owner. Could be deleted or an NPC corpse.
	        return SendViewContainer( mobile, corpse );
	elseif( owner == mobile )
		// Owner is looting his/her own stuff.
	        return SendViewContainer( mobile, corpse );
	elseif( IsGuildMember( mobile, owner ))
	        return SendViewContainer( mobile, corpse );
// The following required by Destiny's Reach scripts.
// I might implement the 'areas' pkg later.
/*        elseif( A_IsIn( corpse, AREAS_NO_LOOTING ))
                if( owner.criminal || owner.murderer )
                        if( A_IsIn( corpse, AREAS_NO_DAMAGE ))
		                return 0;
                        endif
	                return SendViewContainer( mobile, corpse );
                endif
		return 0; */
        endif

	return SendViewContainer( mobile, corpse );
endfunction

function IsGuildMember( mobile, owner )

	// This should probably exist in a guilds include file.
	if( owner.Guildid == mobile.Guildid )
		// Both are members of the same guild.
		return 1;
	endif

	return 0;
endfunction
