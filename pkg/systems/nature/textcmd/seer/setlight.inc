
function set_light(who, level, radius)

	var locals := ListMobilesNearLocation(who.x, who.y, who.z, radius, who.realm);
	
	foreach dude in locals
		if (dude.template)
			break;
		endif
		
		dude.setlightlevel(level, 3600);
	endforeach

endfunction
