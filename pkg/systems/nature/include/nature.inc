
const FELUCCA := 1;
const TRAMMEL := 2;
const DAY := 3;
const CURRENTLIGHT := 4;
const CURRENTWEATHER := 5;
const FELUCCANEXT := 6;
const TRAMMELNEXT := 7;
const WINDDIR := 8;
const WINDSTR := 9;

const YEAR := 1;
const MONTH := 2;

const NIGHTLIGHT := 27;

const DAWNTIME := 900;

const DEBUG := 0;

function SetWeatherOff()
  var everyone := EnumerateOnlineCharacters();
  foreach dude in everyone
    if (dude.x < 5120)
      MoveObjectToLocation(dude,dude.x,dude.y,dude.z, dude.realm,MOVEOBJECT_FORCELOCATION);
    endif
  endforeach
endfunction

//The following functions are created by
//Tharaxis.

//This weather include is a portion of my larger include called
//"The Lost Functions" which consist of many OSI commands that
//POL doesn't use, allowing a whole host of new features without
//requiring the explicit support of the POL core. And no, I'm not
//going to release it until the core gets further support for
//packet manipulation so I can add the final pieces such as
//working client-side messageboards, etc.
// - Tharaxis
//
//Please note, this include does NOT set region weather levels but
//instead those of either the world, or individual clients, a region
//specific version is in the works.

//The weather settings are there because POL disables them in the 2.0.x series
//and Ignition.
//
//Set the Weather style and intensity:
//
// weather_id:
//
//   0 - light rain ("It starts to rain")
//   1 - rain/thunder ("A fierce storm approaches")
//   2 - snow ("It begins to snow")
//   3 - brewing storm - not entirely sure the effect ("A storm is brewing")
//   255 - None (Turns sound effects off?)
//   254 - Unknown effect...
//
// weather intensity is set on a 0-70 scale, 70=torrential rain, 0=light drizzle.
//
// A client appears to be able to turn weather off by teleporting
// (something about the command the client sends causes weather to stop
// for that client when they teleport, so to turn off the weather before
// the default 6 min timeout just make them teleport.

function set_world_weather(weather_id, weather_intensity)
  var weatherid := CStr(weather_id);
  var weatherintensity := CStr(weather_intensity);
  var id_len := len(weatherid);
  if( id_len == 0 )
    weatherid := "00";
  elseif( id_len == 1)
   weatherid := "0" + weatherid;
  endif 
  var int_len := len(weatherintensity);
  if( int_len == 0 )
    weatherintensity := "00";
  elseif( int_len == 1)
    weatherintensity := "0" + weatherintensity;
  endif 
  var packet := "65"+weatherid+""+weatherintensity+"00";
  foreach client in EnumerateOnlineCharacters()
    if (client.x < 5120)
//    MoveObjectToLocation(client,client.x,client.y,client.z,client.realm,MOVEOBJECT_FORCELOCATION);
      SendPacket( client, packet );
    endif
  endforeach 
endfunction

function set_weather(for_who, weather_id, weather_intensity)
  var weatherid := CStr(weather_id);
  id_len := len(weatherid);
  if( id_len = 0 )
    weatherid := "00";
  elseif( id_len = 1)
    weatherid := "0" + weatherid;
  endif 
  var int_len := len(weatherintensity);
  if( int_len = 0 )
    weatherintensity := "00";
  elseif( int_len = 1)
    weatherintensity := "0" + weatherintensity;
  endif 
  var packet := "65"+weatherid+""+weatherintensity+"00";
  SendPacket( for_who, packet );
endfunction


// Back to original functions

function FindLight(moonphase)
  case(moonphase)
    0: return 0;
    1: return 1;
    2: return 2;
    3: return 3;
    4: return 4;
    5: return 3;
    6: return 2;
    7: return 1;
    default:
  endcase
endfunction
