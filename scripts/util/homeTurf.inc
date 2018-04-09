///////////////////////////////////////////////////////////
//
//   homeTurf
//
//   Author: rje
//
//   Date: 1/99
//
//   This small include file contains data and logic for
//   creating a bounding box for some mobile.
//
///////////////////////////////////////////////////////////
var hometurf := 0; // for ht_setTurf()

/////////////////////////////////////////////////////////
//
//  setHomeTurf()
//
//  This sets a boundingbox in which the creature 'stakes
//  out' as its own.
//
/////////////////////////////////////////////////////////
function ht_setHomeTurf()

   if ( GetObjProperty( me, "bounded" ) )

      var x1, x2, y1, y2;
      var mystring;
 
      x1 := me.x - 10;
      y1 := me.y - 10;
      x2 := x1 + (RandomInt(50) * 4) + 1;
      y2 := y1 + (RandomInt(50) * 4) + 1;
       
      mystring := CStr(x1) + " " + CStr(y1) + " " + CStr(x2) + " " + CStr(y2);
      hometurf := MakeBoundingBox( mystring );

   endif

endfunction