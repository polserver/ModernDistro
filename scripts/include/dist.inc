/*
 *  dist( obj1, obj2 )
 *      - returns the largest of the x-distance and y-distance 
 *        between two objects.
 */

use uo;

function dist( obj1, obj2 )
    return Distance( obj1, obj2 );

            /* implements:
                while (obj1.container)
                    obj1 := obj1.container;
                endwhile
    
                while (obj2.container)
                    obj2 := obj2.container;
                endwhile

                var xd := obj1.x - obj2.x;
                var yd := obj1.y - obj2.y;
                if (xd < 0)
                    xd := -xd;
                endif
                if (yd < 0)
                    yd := -yd;
                endif
                if (xd > yd)
                    return xd;
                else
                    return yd;
                endif
            */
endfunction

function coordist( x1, y1, x2, y2 )

   var xd := x1 - x2;
   var yd := y1 - y2;
   if (xd < 0)
      xd := -xd;
   endif
   if (yd < 0)
      yd := -yd;
   endif
   if (xd > yd)
      return xd;
   else
     return yd;
   endif
endfunction
