/*
        Created by *Edwards
        
        2009-10-26
*/
use uo;
use vitals;
use util;

function GetProtections()

         var protections := array{   "AirProtection",
			             "EarthProtection",
		                     "FireProtection",
			             "WaterProtection",
                                     "HolyProtection",
			             "NecroProtection",
                                     "PoisonProtection",
                                     "PermPoisonProtection",
                                     "MagicProtection",
                                     "PermMagicProtection",
                                     "PhysicalProtection",
                                     "SpellReflection",
                                     "PermSpellReflection",
                                     "FreeAction",
                                     "HealingIncrease",
                                     "AcidProtection" };
                                                  
         return protections;
endfunction

function GetObjProtections( mobile )

         var protections := GetObjProperty( mobile, "Protections" );
         if( !protections )
                  protections := struct;
         endif
        
         return protections;
endfunction

function GetMobileProtection( mobile, protection )

         var protections := GetObjProtections( mobile );

         return CInt( protections[protection] );
endfunction

function SetMobileProtections( mobile, protections )

         return SetObjProperty( mobile, "Protections", protections );
endfunction

function SetMobileProtection( mobile, protection, level )

         var protections := GetObjProtections( mobile );
         
         protections[protection] := CInt( level );

         return SetMobileProtections( mobile, protections );
endfunction

function PROT_GetElementalDamage( targ, amount, type )
        
	//var result := PROT_GetElementalProt( targ, type );
        var result := PROT_GetProt( targ, type );
	if( result <= 0 )
                return amount;
        elseif( result > 6 )
		HealDamage( targ, CInt( amount * ( result - 4 ) * 0.20 ));
                return 0;
        endif
        
        var dag_mod := CDbl( 0.13 );
        if( targ.IsA( POLCLASS_NPC ))
                dag_mod := CDbl( 0.5 );
        endif
        
	amount -= CDbl( amount * result * dag_mod );

        return CInt( amount );
endfunction

function PROT_GetElementalProt( mobile, type )

	var complementary,
	    opposed;

	case( type )
		DMG_AIR:	complementary 	:= DMG_FIRE;
			        opposed		:= DMG_WATER;
			        break;

		DMG_EARTH:	complementary 	:= DMG_WATER;
			        opposed		:= DMG_AIR;
			        break;

		DMG_FIRE:	complementary 	:= DMG_AIR;
			        opposed		:= DMG_EARTH;
			        break;

		DMG_WATER:	complementary 	:= DMG_EARTH;
			        opposed		:= DMG_FIRE;
			        break;

                DMG_HOLY:	complementary 	:= DMG_EARTH;
			        opposed		:= DMG_NECRO;
			        break;

                DMG_NECRO:	complementary 	:= DMG_FIRE;
			        opposed		:= DMG_HOLY;
			        break;
	endcase

	var prot_type := PROT_GetProt( mobile, type ),
	    prot_complementary := PROT_GetProt( mobile, complementary ),
	    prot_opposed := PROT_GetProt( mobile, opposed );

	if( !prot_opposed )
		prot_opposed := 6 - prot_opposed;
	endif

	var result := CInt( prot_type + ( prot_complementary / 2 ) - ( prot_opposed / 2 ));

	return result;
endfunction

function PROT_GetProt( mobile, type )

	var protection := "";
 
	case( type )
		DMG_AIR:	protection := "AirProtection";
			        break;

		DMG_EARTH:	protection := "EarthProtection";
			        break;

		DMG_FIRE:	protection := "FireProtection";
			        break;

		DMG_WATER:	protection := "WaterProtection";
			        break;

                DMG_HOLY:	protection := "HolyProtection";
			        break;

		DMG_NECRO:	protection := "NecroProtection";
			        break;
	endcase
 
        var level := CInt( GetMobileProtection( mobile, protection ));
	if( !level )
		level := 0;
	endif

	return level;
endfunction
