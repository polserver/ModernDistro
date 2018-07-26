/* $Id: findCity.inc 373 2006-06-17 18:27:33Z austinheilman $
 *
 */
/*
 * ToDo:	Needs upgraded for realm support.
 *		Needs upgraded to use config files for city coords.
 */
use os;
use uo;

function FindCity(me)
	if(me.x>=1244 && me.y>=1530 && me.x<=1704 && me.y<=1758)
		return "Britain";
	elseif(me.x>=4379 && me.y>=1045 && me.x<=4487 && me.y<=1178)
		return "Moonglow";
	elseif(me.x>=5137 && me.y>=3948 && me.x<=5196 && me.y<=4082)
		return "Delucia";
	elseif(me.x>=5196 && me.y>=3923 && me.x<=5322 && me.y<=4085)
		return "Delucia";
	elseif(me.x>=5638 && me.y>=3083 && me.x<=5799 && me.y<=3328)
		return "Papua";
	elseif(me.x>=5799 && me.y>=3121 && me.x<=5822 && me.y<=3327)
		return "Papua";
	elseif(me.x>=1294 && me.y>=3664 && me.x<=1506 && me.y<=3886)
		return "Jhelom";
	elseif(me.x>=494 && me.y>=926 && me.x<=600 && me.y<=1034)
		return "Yew";
	elseif(me.x>=590 && me.y>=796 && me.x<=670 && me.y<=886)
		return "Empath Abbey";
	elseif(me.x>=2394 && me.y>=370 && me.x<=2624 && me.y<=632)
		return "Minoc";
	elseif(me.x>=1794 && me.y>=2638 && me.x<=2136 && me.y<=2902)
		return "Trinsic";
	elseif(me.x>=538 && me.y>=2106 && me.x<=686 && me.y<=2274)
		return "Skara Brae";
	elseif(me.x>=3646 && me.y>=2040 && me.x<=3814 && me.y<=2280)
		return "Magincia";
	elseif(me.x>=3588 && me.y>=2446 && me.x<=3762 && me.y<=2690)
		return "Occlo";
	elseif(me.x>=2612 && me.y>=2082 && me.x<=2772 && me.y<=2256)
		return "Buccaneers Den";
	elseif(me.x>=3496 && me.y>=1062 && me.x<=3808 && me.y<=1400)
		return "Nujelm";
	elseif(me.x>=2794 && me.y>=634 && me.x<=3026 && me.y<=1000)
		return "Vesper";
	elseif(me.x>=2206 && me.y>=1116 && me.x<=2290 && me.y<=1236)
		return "Cove";
	elseif(me.x>=5120 && me.y>=0 && me.x<=5374 && me.y<=254)
		return "Wind";
	elseif(me.x>=1382 && me.y>=3955 && me.x<=1528 && me.y<=4044)
		return "Jhelom";
	endif
	return "no city";
endfunction