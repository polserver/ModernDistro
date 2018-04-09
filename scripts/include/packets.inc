////////////////////////////////////////////////
//packets.inc
//  v1.1
//
//  Miscellaneous packet functions,
//    S1tH
//    Thanks to Maximilian Scherr for the following functions:
//      SetPlayerWarMode( who, value )
//      EnableChatButton( who )
//      SendIdleWarning( who )
//
//    sithilius@hotmail.com
//    38288137
////////////////////////////////////////////////

use uo;

////////////////////////////////////////////////
//Function Listing
////////////////////////////////////////////////
//SetPlayerWarMode( who, value )                   Toggle a players war mode.
//EnableChatButton( who )                          Enable the "chat" button in a player paperdoll. WARNING! See function definition.
//SendIdleWarning( who )                           Sends an idle warning telling the player they will be kicked in 1 minute if they dont do anything.
//UpdatePlayer( who, forwho, graphic )             Shows another player onscreen as having different body, until their next movement.
//PlayCharAnim( who, anim )                        Look at the definition of this one... more options you can pass (speed, forwards/backwards, etc).
//PlayCharAnimPrivate( who, anim, forwho )         See definition for more details and explanation.
//PlayCharAnimWithRepitition( ... )		   See definition for explanation.
//PlayCharAnimWithRepititionPrivate( ... )         See definition for explanation
//PlayMidiMusic( who, musicID )                    Plays some MIDI.
//SetWeatherForAll( type, num )                    Sets weather conditions for all online clients.
//SetWeatherForPlayer( who, type, num )            Sets a players weather conditions.
//DrawGamePlayer( who, body, colour )              Draws the clients character with whatever body graphic you want, without showing other clients.
//NormaliseGamePlayer( who )                       Return what the client sees of themselves to normal.
//SetOverallLightForAll( lightlevel )		   Sets the 'overall' lightlevel for all online characters.
//SetOverallLightForPlayer( who, lightlevel )      Sets the 'overall' lightlevel for a specific character.
//SetCursorHue( who, hue )                         Changes cursor colour between gold (trammel) and normal (felucca?).
//SetSeasonForAll( season )                        Sets the weather season for all online characters.
//SetSeasonForPlayer( who, season )                Sets the weather season for a specific character.
//SetQuestArrow( who, active, xLoc, yLoc )         Sets if the player has a 'quest arrow', and where its pointing to.
////////////////////////////////////////////////

Const CURSOR_NORM := 0; //Felucca / Old UO
Const CURSOR_GOLD := 1; //Trammel i think...

Const SEASON_SPRING := 0;     //Trammel.
Const SEASON_SUMMER := 1;     //Old Britannia
Const SEASON_FALL := 2;       //Fall
Const SEASON_WINTER := 3;     //Screwed attempt by OSI for snow. Snow has no edges and looks fucked.
Const SEASON_DESOLATION := 4; //Felucca - no leaves on trees, gravestones everywhere...

Const WEATHER_STARTS_TO_RAIN := 0;
Const WEATHER_FIERCE_STORM   := 1;
Const WEATHER_BEGINS_SNOW    := 2;
Const WEATHER_STORM_BREWING  := 3;
Const WEATHER_DISABLE        := 0xFF;

Const ANIM_DIR_FORWARDS  := 0; //The animation will run the frames in normal order.
Const ANIM_DIR_BACKWARDS := 1; //The animation will run the frames in reverse.
Const ANIM_DEF_SPEED     := 0; //0 = fastest, 30 = way too fucken slow, 70 = wtf are you doing?

//////////////////////////////////////////////////////////////////////////////
//Sets a characters war mode. Value:
//  1     War
//  0     Peace
//////////////////////////////////////////////////////////////////////////////
function SetPlayerWarMode( who, value )
  if( !who.acctname )
    if( who.IsA(POLCLASS_NPC) )
      return who.SetWarMode(value);
    endif
  else
    var packetString := "72" + fixPacketLength(value, 1);
    packetString := packetString + "003200";
    return SendPacket(who, packetString);
  endif
endfunction

//////////////////////////////////////////////////////////////////////////////
//Allows the player to click the "chat" button on their paperdoll.
//WARNING! If the player clicks this, POL doesn't recognize it, and will
//complain about it (quite a bit) in the console. Could be useful if you
//have an aux package listening in to enable chat though.
//////////////////////////////////////////////////////////////////////////////
function EnableChatButton( who )
  return SendPacket(who, "B90001");
endfunction

//////////////////////////////////////////////////////////////////////////////
//Sends the client a little gump saying
//"You have been idle for too long. If you do not do anything
// within the next minute, you will be logged out."
//////////////////////////////////////////////////////////////////////////////
function SendIdleWarning( who )
  return SendPacket(who, "5307");
endfunction

//////////////////////////////////////////////////////////////////////////////
//Changes another characters graphic for a specific client
//  who        Character whos graphic to change
//  forwho     Character to change for
//  graphic    New graphic to show
//////////////////////////////////////////////////////////////////////////////
function UpdatePlayer( who, forwho, graphic )
    if( !graphic )
        graphic := who.graphic;
    endif
    
    var packetString := "77";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(graphic),2);
    packetString := packetString + fixPacketLength(hex(who.x),2);
    packetString := packetString + fixPacketLength(hex(who.y),2);
    packetString := packetString + fixPacketLength(hex(who.z),1);
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + fixPacketLength(hex(0),2); //Colour (doesnt work)
    packetString := packetString + fixPacketLength(hex(0),1); //Flag... dunno?
    packetString := packetString + fixPacketLength(hex(0),1); //Highlight colour (doesnt work)
    return SendPacket( forwho, packetString );
endfunction

//////////////////////////////////////////////////////////////////////////////
//Animates a character
//  who    Character to animate
//  anim   Animation ID (constant from client.inc)
//  speed  Frame delay (lesser = faster)
//  dir    Play frames backwards or forwards? (use constants at header!!!)
//////////////////////////////////////////////////////////////////////////////
function PlayCharAnim( who, anim, speed := ANIM_DEF_SPEED, dir := ANIM_DIR_FORWARDS )
    var packetString := "6E";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(anim),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + "0001"; //Repitition
    packetString := packetString + fixPacketLength(hex(dir),1);
    packetString := packetString + "00"; //Does this animation repeat?
    packetString := packetString + fixPacketLength(hex(speed),1);
    foreach chr in ListMobilesNearLocation( who.x, who.y, who.z, 16 );
        SendPacket( chr, packetString );
    endforeach
endfunction

//////////////////////////////////////////////////////////////////////////////
//Animates a character privately
//  who    Character to animate
//  anim   Animation ID (constant from client.inc)
//  forwho Character that sees the animation
//  speed  Frame delay (lesser = faster)
//  dir    Play frames backwards or forwards? (use constants at header!!!)
//////////////////////////////////////////////////////////////////////////////
function PlayCharAnimPrivate( who, anim, forwho, speed := ANIM_DEF_SPEED, dir := ANIM_DIR_FORWARDS )
    var packetString := "6E";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(anim),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + "0001"; //Repitition
    packetString := packetString + fixPacketLength(hex(dir),1);
    packetString := packetString + "00"; //Does this animation repeat?
    packetString := packetString + fixPacketLength(hex(speed),1);
    SendPacket( forwho, packetString );
endfunction

/////////////////////////////////////////////////////////////////////////////////////////
//Repitition is totally wacked mega weird shit...
//It doesnt repeat the same animation that number of times but rather cycles through that number of following animations,
//then at the end i think it plays the original in reverse... why? NFI... test coz im not sure...
//Anyway its handy if you pass "reptimes" as "1", because it will play an animation, then the same in reverse.
// eg. A person falling over, then flying back up again :D
/////////////////////////////////////////////////////////////////////////////////////////
function PlayCharAnimWithRepitition( who, anim, reptimes, speed := ANIM_DEF_SPEED, dir := ANIM_DIR_FORWARDS )
    var packetString := "6E";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(anim),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + fixPacketLength(hex(reptimes),2);
    packetString := packetString + fixPacketLength(hex(dir),1);
    packetString := packetString + "01"; //Does this animation repeat?
    packetString := packetString + fixPacketLength(hex(speed),1);
    foreach chr in ListMobilesNearLocation( who.x, who.y, who.z, 16 );
        SendPacket( chr, packetString );
    endforeach
endfunction

/////////////////////////////////////////////////////////////////////////////////////////
//Above, but only seen by 'forwho'
/////////////////////////////////////////////////////////////////////////////////////////
function PlayCharAnimWithRepititionPrivate( who, anim, reptimes, forwho, speed := ANIM_DEF_SPEED, dir := ANIM_DIR_FORWARDS )
    var packetString := "6E";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(anim),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + fixPacketLength(hex(reptimes),2);
    packetString := packetString + fixPacketLength(hex(dir),1);
    packetString := packetString + "01"; //Does this animation repeat?
    packetString := packetString + fixPacketLength(hex(speed),1);
    SendPacket( forwho, packetString );
endfunction

//////////////////////////////////////////////////
//Plays MIDI music (untested)
//  who      Person who hears the music
//  musicID  MIDI to play, same as "playmusic.inc"
//////////////////////////////////////////////////
function PlayMidiMusic( who, musicID )
    var packetString := "6D" + fixPacketLength(hex(musicID),2);
    return SendPacket(who,packetString);
endfunction

///////////////////////////////////////////////////////
//Sets weather for all online clients:
//  type   Weather type. Constants at header.
//  num    Number of weather effects (dots?) onscreen. Max is 70.
//  temp   Temperature. Currently has no effect...
///////////////////////////////////////////////////////
function SetWeatherForAll( type, num, temp := 0 )
    if( num > 70 )
        num := 70;
    endif
    
    var packetString := "65";
    packetString := packetString + fixPacketLength(hex(type),1);
    packetString := packetString + fixPacketLength(hex(num),1);
    packetString := packetString + fixPacketLength(hex(temp),1);
    
    foreach chr in EnumerateOnlineCharacters()
        SendPacket( chr, packetString );
    endforeach
endfunction

///////////////////////////////////////////////////////
//Sets weather for a specific client:
//  type   Weather type. Constants at header.
//  num    Number of weather effects (dots?) onscreen. Max is 70.
//  temp   Temperature. Currently has no effect...
///////////////////////////////////////////////////////
function SetWeatherForPlayer( who, type, num, temp := 0 )
    if( num > 70 )
        num := 70;
    endif
    
    var packetString := "65";
    packetString := packetString + fixPacketLength(hex(type),1);
    packetString := packetString + fixPacketLength(hex(num),1);
    packetString := packetString + fixPacketLength(hex(temp),1);
    return SendPacket(who,packetString);
endfunction

////////////////////////////////////////////////////
//Changes how the clients character is drawn, like
//them polymorphing but no-one else sees it :)
//  who     Client whos character will change
//  body    Body graphic to change to
//  colour  Colour for the clients body
////////////////////////////////////////////////////
function DrawGamePlayer( who, body, colour )
    var packetString := "20";
    packetString := packetString + fixPacketLength(hex(who.serial),4);
    packetString := packetString + fixPacketLength(hex(body),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(colour),2);
    packetString := packetString + "00";
    packetString := packetString + fixPacketLength(hex(who.x),2);
    packetString := packetString + fixPacketLength(hex(who.y),2);
    packetString := packetString + "0000";
    packetString := packetString + fixPacketLength(hex(who.facing),1);
    packetString := packetString + fixPacketLength(hex(who.z),1);
    return SendPacket( who, packetString );
endfunction

///////////////////////////////////////////////////////
//Tells a client what their character really looks like
//  who    Person to send to
///////////////////////////////////////////////////////
function NormaliseGamePlayer( who )
    return DrawGamePlayer( who, who.graphic, who.color );
endfunction

///////////////////////////////////////////////////////
//Sets the 'overall' (world) light for everyone online
//  light    Light level. 0 = daylight, 30 = black.
///////////////////////////////////////////////////////
function SetOverallLightForAll( light )
    var packetString := "4F" + fixPacketLength(hex(light),1);
    foreach chr in EnumerateOnlineCharacters()
        SendPacket(chr,packetString);
    endforeach
endfunction

///////////////////////////////////////////////////////////
//Sets the 'overall' (world) light for a specific character
//  who     Person whos light to change.
//  light   Light level. 0 = daylight, 30 = black.
///////////////////////////////////////////////////////////
function SetOverallLightForPlayer( who, light )
    var packetString := "4F" + fixPacketLength(hex(light),1);
    return SendPacket(who,packetString);
endfunction

///////////////////////////////////////////////////////////
//Change a clients cursor colour, I think this is 2.0.0+
//  who      Clients cursor to change.
//  hue      Hue type, see constants at header.
///////////////////////////////////////////////////////////
function SetCursorHue( who, hue )
    var packetString := "BF";
    packetString := packetString + fixPacketLength(6,2);
    packetString := packetString + fixPacketLength("8",2);
    packetString := packetString + fixPacketLength(hue,1);
    return SendPacket(who,packetString);
endfunction

///////////////////////////////////////////////////////////
//Changes season for all online characters...
//  season    Season type. Constants at header.
///////////////////////////////////////////////////////////
function SetSeasonForAll( season )
    var packetString := "BC" + fixPacketLength(season,1) + "01";
    foreach chr in EnumerateOnlineCharacters()
        SendPacket(chr,packetString);
    endforeach
endfunction

///////////////////////////////////////////////////////////
//Changes season for a specific character
//  who      Character to change for
//  season   Season type. Constants at header.
///////////////////////////////////////////////////////////
function SetSeasonForPlayer( who, season )
    var packetString := "BC" + fixPacketLength(season,1) + "01";
    SendPacket(who,packetString);
    return;
endfunction

///////////////////////////////////////////////////////////
//Quest arrow pointing to a location. Must be disabled too!
//  who       Person who sees the quest arrow
//  active    Arrow Status. 0 = off, non-zero = on.
//  xLoc      X Location to point to.
//  yLoc      Y Location to point to.
///////////////////////////////////////////////////////////
function SetQuestArrow( who, active, xLoc, yLoc )
    var packetString := "BA" + fixPacketLength(active,1);
    packetString := packetString + fixPacketLength(hex(xLoc),2);
    packetString := packetString + fixPacketLength(hex(yLoc),2);
    print( packetString );
    return SendPacket(who,packetString);
endfunction

///////////////////////////////////////////////////////////////
//Pads a string with a "0" prefix, until it is of proper length
//  packetString   value to pad.
//  byteLength     number of bytes. Byte counted as two chars.
///////////////////////////////////////////////////////////////
function fixPacketLength( packetString, byteLength )
    if( !packetString || packetString == error )
        packetString := 0;
    endif
    packetString := cStr(packetString);
    
    if( packetString[1,2] == "0x" )
        packetString := cStr(packetString[3,len(packetString)]);
    endif
    
    if( len(packetString) > byteLength * 2 )
//		var extraBytes := (byteLength*2) - len(packetString);
		var extraBytes := len(packetString) - (byteLength*2) + 1;
		return cStr(packetString[extraBytes,len(packetString)]);
    endif
    
    while( len(packetString) < byteLength * 2 )
        packetString := "0" + packetString;
    endwhile
    
    return packetString;
endfunction


function PlayObjectCenteredEffectHuefx( center, effect, speed, 
loop := 0, color := 0, translucent := 0 )
var packetString := "C0";
packetString := packetString + "03"; 
packetString := packetString + fixPacketLength(hex
(center.serial),4);
packetString := packetString + fixPacketLength(hex(0),4);
packetString := packetString + fixPacketLength(hex(effect),2);
packetString := packetString + fixPacketLength(hex(0),10);
packetString := packetString + fixPacketLength(hex
(speed),1); //speed
packetString := packetString + fixPacketLength(hex
(loop),1); //loop
packetString := packetString + "000000000000" ;
packetString := packetString + fixPacketLength(hex
(color),2); //color
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(translucent),1); //tran
foreach chr in ListMobilesNearLocation( center.x, center.y, 
center.z, 16 );
SendPacket( chr, packetString );
endforeach
endfunction

function PlayMovingEffectHuefx( source, dest, effect, speed, loop := 
0, explode := 0, color := 0, translucent := 0)
var packetString := "C0";
packetString := packetString + "00"; 
packetString := packetString + fixPacketLength(hex
(source.serial),4);
packetString := packetString + fixPacketLength(hex
(dest.serial),4);
packetString := packetString + fixPacketLength(hex(effect),2);
packetString := packetString + fixPacketLength(hex(0),10);
packetString := packetString + fixPacketLength(hex
(speed),1); //speed
packetString := packetString + fixPacketLength(hex
(loop),1); //loop
packetString := packetString + "000000" ;
packetString := packetString + fixPacketLength(hex
(explode),1); //explode
packetString := packetString + "0000";
packetString := packetString + fixPacketLength(hex
(color),2); //color
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(translucent),1); //tran
foreach chr in ListMobilesNearLocation( source.x, source.y, 
source.z, 16 );
SendPacket( chr, packetString );
endforeach
endfunction

function PlayMovingEffectXYZHuefx(source, sourcex, sourcey, sourcez, 
destx, desty, destz, effect, speed, loop := 0, explode := 0, color := 
0, translucent := 0)
var packetString := "C0";
packetString := packetString + "00"; 
packetString := packetString + fixPacketLength(hex(0),8);
packetString := packetString + fixPacketLength(hex(effect),2);
packetString := packetString + fixPacketLength(hex(sourcex),2);
packetString := packetString + fixPacketLength(hex(sourcey),2);
packetString := packetString + fixPacketLength(hex(sourcez),1);
packetString := packetString + fixPacketLength(hex(destx),2);
packetString := packetString + fixPacketLength(hex(desty),2);
packetString := packetString + fixPacketLength(hex(destz),1);
packetString := packetString + fixPacketLength(hex
(speed),1); //speed
packetString := packetString + fixPacketLength(hex
(loop),1); //loop
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(explode),1); //explode
packetString := packetString + "0000";
packetString := packetString + fixPacketLength(hex
(color),2); //color
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(translucent),1); //tran
foreach chr in ListMobilesNearLocation( source.x, source.y, 
source.z, 16 );
SendPacket( chr, packetString );
endforeach
endfunction

function PlayStationaryEffectHuefx( x, y, z, effect, speed, loop := 
0, explode := 0, color := 0,translucent := 0)
var packetString := "C0";
packetString := packetString + "02"; 
packetString := packetString + fixPacketLength(hex(0),8);
packetString := packetString + fixPacketLength(hex(effect),2);
packetString := packetString + fixPacketLength(hex(x),2);
packetString := packetString + fixPacketLength(hex(y),2);
packetString := packetString + fixPacketLength(hex(z),1);
packetString := packetString + fixPacketLength(hex(0),5);
packetString := packetString + fixPacketLength(hex
(speed),1); //speed
packetString := packetString + fixPacketLength(hex
(loop),1); //loop
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(explode),1); //explode
packetString := packetString + "0000";
packetString := packetString + fixPacketLength(hex
(color),2); //color
packetString := packetString + "000000";
packetString := packetString + fixPacketLength(hex
(translucent),1); //tran
foreach chr in ListMobilesNearLocation( x, y, z, 16 );
SendPacket( chr, packetString );
endforeach
endfunction
