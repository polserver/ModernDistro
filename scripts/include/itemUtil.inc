/*
        Created by *Edwards
        
        2009-07-29
        
        Last Modifications 2009-11-05
             * Added Accessible()
             
        Last Modifications 2010-03-09
             * Added EmptyHands_Check() but not activated
        
        To use:
       	if( !useItem_check( mobile, item, flags ))
          return 0;
        endif
*/
use uo;
use cfgfile;

const DEFAULT_ITEM     := 0x1;
const ITEM_INCONTAINER := 0x2;
const ITEM_EQUIPPED    := 0x4;
const ITEM_WILD        := 0x8;

function useItem_check( mobile, item, option := DEFAULT_ITEM, temp_dist := 0 )

	if( !ReserveItem( item ))
    SendSysMessage( mobile, "You cannot use that item." );
		return 0;
	elseif( !Accessible( mobile, item ))
    SendSysMessage( mobile, "You do not have access to that item." );
		return 0;
  endif

  if( option & ITEM_WILD )
    if( !item.movable )
      SendSysMessage( mobile, "That item is not movable." );
      return 0;
    elseif( !CheckLosAt( mobile, item.x, item.y, item.z ))
      SendSysMessage( mobile, "You cannot see that item." );
      return 0;
    elseif( temp_dist )
      if( !CoordinateDistance( mobile.x, mobile.y, item.x, item.y ) > temp_dist )
        SendSysMessage( mobile, "You are too far from the item." );
        return 0;
      endif
    endif
  elseif( option & ITEM_INCONTAINER )
    if( !item.movable )
      SendSysMessage( mobile, "That item is not movable." );
      return 0;
    elseif( !( item in EnumerateItemsInContainer( mobile.backpack )))
      SendSysMessage( mobile, "That item is not in your backpack." );
      return 0;
    endif
  elseif( option & ITEM_EQUIPPED )
    if( item in ListEquippedItems( mobile ))
      return 1;
    else
      SendSysMessage( mobile, "You must equip "+item.desc+" before using it." );
      return 0;
    endif
  endif

    if(( item.IsA( POLCLASS_CONTAINER ) || item.IsA( POLCLASS_DOOR )) && item.locked )
      SendSysMessage( mobile, "That item is locked!" );
      return 0;
    endif

  return 1;
endfunction

function EmptyHands_Check( mobile )

        //Below could be simplified but it saves a lot of process
        //since all items are using this function.
        
 	var equipped_left := GetEquipmentByLayer( mobile, 0x02 );
	if( equipped_left )
                var equipped_right := GetEquipmentByLayer( mobile, 0x01 );
                if( equipped_right )
			return 0;
                endif
                
		var cfg := ReadConfigFile( ":*:itemdesc" ),
		    two_handed := Cint( cfg[equipped_left.objtype].TwoHanded );
      
                if( two_handed )
			return 0;
                endif
	endif
 
        return 1;
endfunction
