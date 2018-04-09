// Script for use under POL v0.95
// Copyright (C) 2003 Jacobo Lata Fernández
// choofi@infoactiva.net
//
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
// Originally created for the Gallaecia Shard - http://gallaecia.infoactiva.net
//
//
// /scripts/include/follow.src
//
// Function to follow a player/npc


use uo;
use os;

function FollowPlayer( attacker , victim , interval := 1 )
	if( GetObjProperty( attacker , "Following" ) )
		SendSysMessage( attacker , "You can't begin to follow a mobile if you're following another one" );
		return 0;
	endif
	SetObjProperty( attacker , "Following" , 1 );
	if( !attacker.concealed )
		attacker.concealed := attacker.cmdlevel;
	endif
	var warmode := attacker.warmode;
	while( 1 )
		if( attacker.warmode != warmode )
			break;
		endif
		MoveObjectToLocation( attacker , victim.x , victim.y , victim.z , victim.realm, MOVEOBJECT_FORCELOCATION );
		sleep( interval );
	endwhile
	EraseObjProperty( attacker , "Following" );
	return 1;
endfunction
