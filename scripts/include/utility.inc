use basic;

include "include/myUtil";

function find_or_create_storage( areaname )
  var area;
  area := FindStorageArea( areaname );
  if(!area)
	area := CreateStorageArea( areaname );
    if(area)
    endif
  endif
  return area;
endfunction

function find_or_create_item(storage, name, objtype := 0x1966a)
  var item := FindRootItemInStorageArea(storage, name);
  if(item)
	return item;
  endif
  item := CreateRootItemInStorageArea( storage, name, objtype);
  return item;
endfunction

function IsLocationAccessible(character, x, y, z)
  var xd, yd, zd;
  xd := character.x - x;
  if(xd < -1 || xd > 1)
    return 0;
  endif
  yd := character.y - y;
  if(yd < -1 || yd > 1)
    return 0;
  endif
  zd := character.z - z;
  if(zd < -10 || zd > 10)
    return 0;
  endif
  return CheckLosAt( character, x, y, z );
endfunction

//function min(x, y)
//  if(x<= y)
//    return x;
//  else
//    return y;
//  endif
//endfunction

/////////////////// 
// Sends messages to all online GMs. Useful for important errors 
/////////////////// 

function SendGMErrorMessage(text, syslogit := 0, mincmdlevel := 1) 
    if ( syslogit ) 
        syslog(text); 
    endif 

    foreach player in EnumerateOnlineCharacters() 
        if ( player.cmdlevel ) 
            if ( player.cmdlevel >= mincmdlevel ) 
                SendSysMessage(player, "Staff system message: " + text, color := 220); 
            endif 
        endif 
    endforeach 
endfunction 

//////////////////////////////////////////////////////////////////////////////////////
//
//  Determines if the object is within a rectangle
//  Note: The scope of this function is to determine whether
//  an object lies within a two dimwnsional area.
//  There is no 'z' check performed.
//
//  Parameters:
//  Name                  Type
//  object                UO_OBJECT reference                  
//  boundary              Array containing the {nwx, nwy, sex, sey}
//                        The function assumes that the 'boundary' parameter
//                        might contain numeric values as strings in its elements.
//
/////////////////////////////////////////////////////////////////////////////////////////

function IsInsideRectangle (object, boundary)

    if ((object.x >= CInt(boundary[1]) && object.x <= CInt(boundary[3])) && (object.y >= CInt(boundary[2]) && object.y <= CInt(boundary[4])))
        return 1;
    else
        return 0;
    endif
endfunction

//////////////////////////////////////////////////////////////////////////////////////////////
//  Determines if the object is within a rectangle at a specified Z coordinate
//
//  Note: The scope of this function is to determine whether an object 
//  lies within a two dimwnsional area at a specified Z coordinate.
//
//  Parameters:
//  Name                  Type
//  object                UO_OBJECT reference                  
//  boundary              Array containing the {nwx, nwy, sex, sey}
//  ZCoord                The desired Z level
//
//                        The function assumes that the 'boundary' and ZCoord parameters
//                        might contain numeric values as strings in its elements.
//
//
//  Returns               0 if 'object' isn't within the rectangle at the ZCoord
//                        1 if 'object' is in the rectangle AND at the desired ZCoord
//////////////////////////////////////////////////////////////////////////////////////////////

function IsInsideRectAtZLevel (object, boundary, ZCoord)

    SendSysMessage(object, ZCoord + "     " +  CStr(object.z));
    if ( (object.x >= CInt(boundary[1]) && object.x <= CInt(boundary[3])) &&
         (object.y >= CInt(boundary[2]) && object.y <= CInt(boundary[4])) && object.z == CInt(ZCoord) )
        return 1;
    else
        return 0;
    endif
endfunction
