use basic;
use math;

function ToSextant(x, y)
  var latitude_direction, longitude_direction;
  var latitude_degrees, longitude_degrees;
  var latitude_minutes, longitude_minutes;
  latitude_degrees := (y-1624)*0.087890625;
  longitude_degrees := (x-1323)*0.0703125;
  if(latitude_degrees < 0)
	latitude_direction := "N";
  else
	latitude_direction := "S";
  endif
  if(longitude_degrees < 0)
	longitude_direction := "W";
  else
	longitude_direction := "E";
  endif
  latitude_degrees := Abs(latitude_degrees);
  longitude_degrees := Abs(longitude_degrees);
  latitude_minutes := CInt((CInt((latitude_degrees-Floor(latitude_degrees))*100)*60)/100);
  longitude_minutes := CInt((CInt((longitude_degrees-Floor(longitude_degrees))*100)*60)/100);
  return(Floor(latitude_degrees) + "o " + latitude_minutes + "'" + latitude_direction + ", " + Floor(longitude_degrees) + "o " + longitude_minutes + "'" + longitude_direction);
endfunction