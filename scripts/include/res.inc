use uo;

var layout := {
"page 0",
"nodispose",
"noclose",
"gumppic 300 230 2070",
"button 330 305 2073 2072 1 0 0",
"button 400 305 2076 2075 1 0 1",
"text 332 264 40 0"
};

var data := {
"Resurrect Now?"
};

function ResNow(who)
  var x := who.x;
  var y := who.y;
  var z := who.z;
  var ret := SendDialogGump(who,layout,data);
  if(!ret)
	return 0;
  endif
  if(MoveObjectToLocation(who, x, y, z, who.realm, MOVEOBJECT_FORCELOCATION))
    return ret[0];
  else
    return 0;
  endif
endfunction