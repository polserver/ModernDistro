use uo;
use os;
use util;


function AdjustVirtue(who, virtue, amt, prob)


	if ( (!who.acctname) or (RandomInt(100) > prob) )
		return 0;
	endif

	virtue := CStr("Virtue" + virtue );
	var virtuescore := CInt( GetObjProperty(who, virtue) );
	if (!virtuescore)
		virtuescore := 0;
	endif

	virtuescore := virtuescore + amt;
	if (virtuescore > 250)
		virtuescore := 250;
	elseif (virtuescore < -250)
		virtuescore := -250;
	endif

	SetObjProperty(who, virtue, virtuescore );
	return 1;

endfunction