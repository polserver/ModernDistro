var lastface;
var nexttry;

function CloseDistance(opponent)
  var dst := 1;
  var d := Distance(me, opponent);
  if (d <= dst)
    return 1;
  elseif((d >= 25) or (opponent.concealed) or (opponent.hidden))
    return 2;
  else
    sleepms(3);
    RunToward( opponent );
    sleepms(3);
    RunToward( opponent );
    sleepms(3);
    RunToward( opponent );
    Opendoors(me);
    if(!(CheckLineOfSight(me, opponent)) and (nexttry < ReadGameClock()))
      nexttry :=  ReadGameClock() + 5;
      if(NavigateTo(opponent))
        return 2;
      endif
    endif
  endif
endfunction

function Opendoors(me)
  foreach door in ListItemsNearLocation(me.x, me.y, me.z, 1, me.realm)
    if((door.isa(POLCLASS_DOOR)) and (!door.locked))
      if(!GetObjProperty(door, "WhenOpened" ))
        start_script(":doors:openDoor", door);
      endif
      EraseObjProperty(door, "#inuse");
    endif
  endforeach
endfunction

function NavigateTo(destination);
  var dX := 0;
  var dY := 0;
  var maxdist := 30;
  for X:=1 to 24
    sleepms(1);
    if (CheckLOSat(me, me.x + X - 12, me.y, me.z) and CheckLOSat(destination, me.x + X - 12, me.y, me.z))
      if(CoordinateDistance(destination.x , destination.y , me.x+X-12 , me.y) < maxdist)
        dX := me.x+X-12;
        dY := me.y;
        maxdist := CoordinateDistance(destination.x , destination.y , me.x+X-12 , me.y );
      endif
    else
      for Y:=1 to 24
        if(CheckLOSat(me, me.x + X - 12, me.y + Y - 12, me.z) and CheckLOSat(destination, me.x+X-12, me.y+Y-12, me.z))
          if(CoordinateDistance(destination.x , destination.y , me.x+X-12 , me.y+Y-12 ) < maxdist)
            dX := me.x+X-12;
            dY := me.y+Y-12;
            maxdist := CoordinateDistance(destination.x , destination.y , me.x + X - 12 , me.y);
          endif
        endif
      endfor
    endif
  endfor
  if(dX)
    return CloseDistancetoLocation(dX, dY);
  endif
  return 1;
endfunction

function CloseDistancetoLocation(x, y)
  var chk := 0;
  repeat
    RunTowardLocation(x, y);
    RunTowardLocation(x, y);
    RunTowardLocation(x, y);
    var myface := me.facing;
    if(ABS(lastface - myface) == 4)
      lastface := myface;
      chk := chk + 1;
    endif
    lastface :=myface;
  until((CoordinateDistance( me.x , me.y , x , y ) <= 0) or (chk > 1));
  if(chk > 1)
    return 1;
  endif
  return 0;
endfunction

function NavigateAway(targ)
  var myface;
  var runaway := 0;
  repeat
    myface := me.facing;
    WalkToward(targ);
    WalkToward(targ);
    WalkToward(targ);
    if(ABS(me.facing - myface) == 4)
      runaway := runaway + 1;
    endif
  until ((Distance(me, targ) <= 1) or (runaway > 2) or (Distance(me, targ) > 20));
endfunction
