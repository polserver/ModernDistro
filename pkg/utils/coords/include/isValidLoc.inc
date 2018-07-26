/* $Id: isValidLoc.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 */
use uo;
use polsys;

function isValidLoc(x, y, realm := "britannia" )
   if(realm == "britannia" || realm == "britannia_alt")

	  if((x >= 5112) && (x <= 5213))
	    if((y >= 1093) && (y <= 1213))
	      return 0;
	    endif
	  endif

	  if((x >= 5120) && (x <= 5374))
	    if((y >= 265) && (y <= 512))
	      return 0;
	    endif
	  endif

	  if((x >= 5890) && (x <= 6142))
	    if((y >= 514) && (y <= 1278))
	      return 0;
	    endif
	  endif

	  if((x >= 5376) && (x <= 5910))
	    if((y >= 1030) && (y <= 1276))
	      return 0;
	    endif
	  endif

	  if((x >= 5422) && (x <= 6142))
	    if((y >= 1536) && (y <= 1796))
	      return 0;
	    endif
	  endif

	  if((x >= 5632) && (x <= 6142))
	    if((y >= 1784) && (y <= 2303))
	      return 0;
	    endif
	  endif

	  if((x >= 5120) && (x <= 5680))
	    if((y >= 2048) && (y <= 2303))
	      return 0;
	    endif
	  endif

  else
  	var t_realm := Realms()[realm];

  	if( x >= t_realm.width || y >= t_realm.height )
  		return 0;
  	endif
  endif
  return 1;
endfunction