use uo;
use os;
include ":attributes:attributes";


function UseMyMana(who, amount)

	if (AP_GetVital(who, MANA) < amount)
		AP_SetVital(who, MANA, 0);
		return 0;
	else
		AP_SetVital(who, MANA, AP_GetVital(who, MANA) - amount);
		return 1;
	endif

endfunction


function UseMyRegs(who, regs)

	var reg;
	var mypack := GetEquipmentByLayer(who, 0x15);
	var success := 1;

	foreach reg in regs
		if (!ConsumeSubstance(mypack, reg, 1) )
			success := 0;
		endif
	endforeach

	return success;

endfunction

function get_direction (character, x, y, z)
   var ns_desc := "";
   var ew_desc := "";
   var thedesc;
   z := z;

   if (y < character.y)
        ns_desc := "north";
   elseif  (y > character.y)
        ns_desc := "south";
   endif

   if (x < character.x)
        ew_desc := "west";
   elseif  (x > character.x)
        ew_desc := "east";
   endif

   if (ns_desc == "" and ew_desc == "")
   thedesc := "nearby";
   else
   thedesc := ns_desc + ew_desc ;
   endif
   // print ("the desc is " + thedesc);
   return thedesc;

endfunction

Function CreateNorthSouthField(caster, fieldtype,x,y,z, lngth := 5)
//needs to return an array of field elements
var counter := 0;
var i;
var fires := { };
// var tester := CreateItemAtLocation(x,y,z, 0xeed, 1, caster.realm);

    for ( i := x - 2; i <= x - 2 + lngth; i := i + 1)

        counter := counter  + 1;
        fires[counter] := CreateItemAtLocation( i,y,z,fieldtype, 1, caster.realm );
        SetObjProperty(fires[counter], "creater", caster.serial);
        SetObjProperty(fires[counter] , "f","1");

        if (!CheckLosAt( caster, fires[counter].x, fires[counter].y, fires[counter].z ) )
            DestroyItem(fires[counter]);
        endif

    endfor
//    destroyitem(tester);
    return fires;
endfunction

Function CreateEastWestField(caster, fieldtype,x,y,z, length := 5)
//needs to return an array of field elements

    var counter := 0;
    var i;
    var fires := { };
    var tester := CreateItemAtLocation(x,y,z, 0xeed, 1, caster.realm);
    for ( i := y - 2; i <= y - 2 + length; i := i + 1)

        counter := counter  + 1;
        fires[counter] := CreateItemAtLocation( x,i,z,fieldtype, 1, caster.realm );
        SetObjProperty(fires[counter] , "f","1");
        setobjproperty(fires[counter], "creater", caster.serial);

        if (!CheckLineOfSight(tester, fires[counter]) )
            DestroyItem(fires[counter]);
        endif

    endfor
    destroyitem(tester);
    return fires;
endfunction
