/*
        Maintained by The POL Distro Team
        
        2018-07-25
*/
use uo;

include "include/string";
include ":armor:armorZones";

function DamageItem( mobile, weapon_ref:=0, force_dmg:=0 )

         if( mobile.IsA( POLCLASS_NPC ))
                  //Do not dmg NPC equipped
                  return 1;
         endif

         var the_item;
         if( weapon_ref )
                  the_item := weapon_ref;
         else
                  var equipment := CS_GetEquipmentInArmorZone( mobile, CS_GetRandomArmorZone() );
                  the_item := equipment.RandomEntry();
         endif

         if( !the_item.hp )
                return 0;
         elseif( the_item.objtype == 0xF020 ) //Intrinsic weapon ( wrestling )
                return 0;
         elseif( GetObjProperty( the_item, "IsGMItem" ))
                return 0;
         elseif( the_item.hp <= 1 && GetObjProperty( the_item, "NoBreak" ))
                return 1;
         endif

         var temp := RandomInt( 100 );
         if( the_item.quality )
                temp *= the_item.quality;
         endif

         if( CInt( temp ) == 77 || force_dmg )
                var item_name := TruncateArticle( the_item.desc );
                if( the_item.hp > 1 )
			 the_item.hp := the_item.hp - 1;
		         SendSysMessage( mobile, "Your "+item_name+" takes some damage." );
                else
			 SendSysMessage( mobile, "Your "+item_name+" has been destroyed." );
                         DestroyItem( the_item );
		endif
	 endif

         return 1;
endfunction
