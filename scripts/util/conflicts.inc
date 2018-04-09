////////////////////////////////////////////////////////////
//
//  conflicts.inc
//
//  Author: rje
//
//  Date: 12/31/98
//
//  This module includes functions for targetting and
//  untargetting another creature, and general functions
//  for handling conflicts and eating!
//
////////////////////////////////////////////////////////////
use os;
use cfgfile;
include ":attributes:attributes";

var badguy         := 0;
var hunger         := 0;

var conf           := ReadConfigFile( "corpses" );
const UOBJECT_CORPSE  := 0x2006;  // for the carnivore in all of us
const UOBJECT_BLOOD   := 0x122d;
const UOBJECT_MEAT    := 0x09f1;
const UOBJECT_HUMAN   := 0x190;

const IN_RANGE        := 5;

//////////////////////////////////////////////////////////////////////////////
//
//  co_human()
//
//  Returns 1 if the object passed in has a human objid.
//
//////////////////////////////////////////////////////////////////////////////
function co_human(theobj)

   if ( theobj.objtype == UOBJECT_HUMAN
     || theobj.objtype == UOBJECT_HUMAN+1 )
      return 1;
   else
      return 0;
   endif

endfunction

/////////////////////////////////////////////////////
//
//  Target dude for termination.
//
/////////////////////////////////////////////////////
function co_target( dude )

   var msg := GetObjProperty( me, "targetText" );

   if (msg)
      say( msg );
   endif

   SetObjProperty( me, "BadGuy", dude );
   SetOpponent( dude );
   SetWarMode(1);
   badguy := dude;
   return dude;

endfunction

///////////////////////////////////////////////////
//
//  co_untarget
//
//  Undo the target lock.
//
///////////////////////////////////////////////////
function co_untarget()

   SetObjProperty( me, "BadGuy", "" );
   SetWarMode( 0 );
   set_critical( 0 );
   badguy := 0;

endfunction



////////////////////////////////////////////////////////////////
//
//  eatTheDead()
//
//  Search for a corpse in the immediate vicinity, and eat it.
//  Returns the amount of food derived from eating a corpse,
//  or 0 if no food was gotten.
//
//  hunger is updated accordingly.
//
////////////////////////////////////////////////////////////////
function co_eatTheDead()

   hunger := hunger + 1;

   foreach body in ListItemsNearLocation( me.x, me.y, me.z, IN_RANGE, me.realm )

      if ( ( body.objtype == UOBJECT_CORPSE )
       && ( GetObjProperty( body, "cut" ) != "1") )

         while ( Distance( me, body ) >= 1 )
            WalkToward( body );
         endwhile

         var c_type     := body.corpseType;
         var elem       := FindConfigElem(conf, c_type);
         var item1      := GetConfigInt( elem, "item1"   );
         var amount1    := GetConfigInt( elem, "amount1" );
         var item2      := GetConfigInt( elem, "item2"   );
         var amount2    := GetConfigInt( elem, "amount2" );

         SetObjProperty( body, "cut","1" );

           // item decay handled by core, v069+
           // local parms array;  // used to also have a local var for "theblood"
           // parms[1] := body;
           // parms[2] := CreateItemAtLocation( body.x, body.y, body.z, UOBJECT_BLOOD, 1, me. realm);// theblood
           // start_script("decay",parms);

         if ( ( item1 == UOBJECT_MEAT ) ||  ( item2 == UOBJECT_MEAT ) )
            say( "* munch munch *" );
            hunger := hunger - ( amount1 * 500 );
            return amount1 * 500;
         endif
      endif

   endforeach
   return 0;

endfunction

////////////////////////////////////////////////////////
//
//  co_fight()
//
//  A basic fighting routine plagiarized from Dan's
//  code.
//
//  * if the opponent is dead/gone/far away, return
//    to normal.
//
//  * if we're badly wounded, run!
//
//  * otherwise, close distance.
//
////////////////////////////////////////////////////////
function co_fight()
 
   var d  := Distance( me, badguy );

   if ((badguy.dead) || (!badguy)  || (d > 10) )
      co_untarget();
      return;
   elseif( GetHp(me) <= CInt( GetMaxHp(me) / 5 ) && d < 16)
      RunAwayFrom( badguy );
      TurnToward( badguy );
   elseif (d > 2)       
      RunToward(   badguy );
   elseif (d > 1)
      WalkToward(  badguy );
   endif       

endfunction