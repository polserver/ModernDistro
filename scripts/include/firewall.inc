use os;
use util;
use uo;

include "include/client";
include ":attributes:attributes";

const SOUND_EFFECT_FIREFIELD:= 0x1de;
const UOBJ_FIREFIELD_NS := 0x398c;
const UOBJ_FIREFIELD_EW := 0x3996;
const SOUND_EFFECT_POISON := 0x247;
const EFFECT_RED_SPARKLE := 0x374a;

function firefield( caster )
var thetarget;
var x;
var y;
var z;
var i;
var fires := { };
var duration;
var listeners := { };
var sufferers := { };
thetarget := TargetCoordinates(caster);
if ((!thetarget))
        SendSysMessage(caster, "Targetting Cancelled.");
endif
x := thetarget.x;
y := thetarget.y;
z := thetarget.z;
if (!CheckLosAt( caster, x, y, z ))
        SendSysMessage(caster, "You can't see that!");
        return;
endif
var dir := get_direction(caster, x, y, z);
case dir
	"north" : fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
	"south"  :fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
	"east"  : fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        "west"  : fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        default:  var firetype := RandomInt(2) + 1;
        	  case firetype
        	  	1: fires:= CreateNorthSouthField(caster, UOBJ_FIREFIELD_NS, x, y, z, 5);
        	  	2: fires:= CreateEastWestField(caster, UOBJ_FIREFIELD_EW, x, y, z, 5);
        	  endcase
endcase
Detach();
foreach fire in fires
        PlaySoundEffect(fire, SOUND_EFFECT_FIREFIELD);
endforeach
var counter := 0;
duration := CInt(GetAttribute( caster, MAGERY ) );
for (counter := 1; counter <= duration; counter := counter + 1)
foreach fire in fires
	if (fire)
        PlaySoundEffect(fire, SOUND_EFFECT_FIREFIELD);
    endif
endforeach
sleep(1);
endfor
foreach fire in fires
    DestroyItem( fire );
endforeach
endfunction