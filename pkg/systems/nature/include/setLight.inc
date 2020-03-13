
function set_light(who, level, radius)

	var locals := ListMobilesNearLocation(who.x, who.y, who.z, radius, who.realm);

	foreach mobile in locals
		if (mobile.template)
			break;
		endif

		mobile.setlightlevel(level, 3600);
	endforeach

endfunction
