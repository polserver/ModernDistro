/////////////////////////////////////////
//
//   spawn.inc
//
//   Author: rje
//
//   Date: 12/31/98
//
//   Method and data for spawning.
//
/////////////////////////////////////////
var sp_newlyspawneddude   := 0;
var sp_template           := GetObjProperty( me, "template" );

///////////////////////////////////////////////////
//
//  spawn()
//
//  If we're happy enough, we'll spawn off a new
//  critter.  If we succeed, 1 is returned, other-
//  wise 0 is returned (so you can check to see if
//  anything happened).
//
///////////////////////////////////////////////////
function sp_spawn( fertility )

   if (fertility < 0) // let's not get carried away, ok?
      fertility := 0;
   endif

   if ( sp_template && RandomInt( fertility * 10 + 1000 ) == 0)
      sp_newlyspawneddude := CreateNpcFromTemplate( sp_template, me.x, me.y, me.z, 0, me.realm );
//      SetObjProperty( newDude, "spawnedBy", me.serial );
//      print( "I don't know nothin' 'bout birthin' no baby " + template + "s!" );
      return 1;
   else
      return 0;
   endif

endfunction