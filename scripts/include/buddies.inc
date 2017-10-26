

include ":attributes:attributes";
function ask_for_help(opponent)

var range := CInt(GetHp(me)/10);


var ev := array;
ev.+ type;
ev.+source;
ev.+buddy;


ev.type := EVID_BUDDYATTACKED;
ev.source := opponent;
ev.buddy := me;

foreach critter in ListMobilesNearLocation(me.x, me.y, me.z, range, me.realm)
        if (!critter.isA(POLCLASS_NPC))
                SendEvent(critter, ev);
        endif
endforeach

endfunction



              