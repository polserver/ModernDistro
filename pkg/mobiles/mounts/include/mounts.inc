/*
        Maintened by *Edwards
        
        2009-02-21
*/
use uo;
use os;
use cfgfile;

include "include/client";
include ":attributes:attributes";
include ":animaltaming:taming";
include ":brainAI:npcCommands";
include ":damage:damage_ex";

function MP_GetMountsCfgFile()

	var cfg := ReadConfigFile( ":mounts:mounts" );
	if( cfg.errortext )
		SysLog( "Error::MP_GetMountsCfgFile() - Unable to open [:mounts:mounts.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

function MP_GetMountsCfgElem( elem_name, byref cfg_file:=0 )

	if( !cfg_file )
		cfg_file := MP_GetMountsCfgFile();
	endif
	
	var elem := cfg_file[elem_name];
	if( elem.errortext )
		SysLog( "Error::MP_GetMountsCfgElem() - Unable to find elem ["+elem_name+"] ->"+elem.errortext );
	endif

	return elem;
endfunction

function MP_GetMountGraphic( mobile )

	var mobile_graphic;
	if( mobile.IsA( POLCLASS_NPC ))
		mobile_graphic := Hex( mobile.graphic );
	else
		mobile_graphic := Hex( CInt( mobile ));
	endif
	
	var elem := MP_GetMountsCfgElem( "MobileToMount" );
	var tile_graphic := GetConfigInt( elem, mobile_graphic );
	
	if( tile_graphic )
		return tile_graphic;
	else
		return error{"errortext":="Error::MP_GetMountGraphic() - No mount tile graphic matches mobile graphic '"+mobile_graphic+"'"};
	endif
endfunction

function MP_GetMobileGraphic( mount )

	if( mount.IsA( POLCLASS_ITEM ))
		mount := mount.graphic;
	else
		mount := CInt( mount );
	endif
	
	var elem := MP_GetMountsCfgElem( "MountToMobileGraphic" );
	var tile_graphic := GetConfigInt( elem, mount );
	
	if( tile_graphic )
		return tile_graphic;
	else
		return error{"errortext":="Error::MP_GetMobileGraphic() - No mobile tile graphic matches tile graphic '"+mount+"'"};
	endif
endfunction

function MP_GetMountTemplate( mount )

	var mount_graphic, template;
	if( mount.IsA( POLCLASS_ITEM ))
		template := GetObjProperty( mount, "NPCTemplate" );
		if( template )
			return template;
		else
			mount_graphic := Hex( mount.graphic );
		endif
	else
		mount_graphic := Hex( CInt( mount ));
	endif
	
	if( template )
		return template;
	else
		return error{"errortext":="Error::MP_GetMountTemplate() - No NPC template match for mount piece graphic '"+mount_graphic+"'"};
	endif
endfunction

function MP_Mount( mobile, npc )

	mobile.graphic := 400 + mobile.gender;

	var mount_piece := MP_GetMount( mobile );
	if( mount_piece )
		return error{"errortext":="Mobile is already mounted."};
	endif
        
	var mount_graphic := MP_GetMountGraphic( npc );
	if( !mount_graphic )
		return mount_graphic;
	endif
	
	mount_piece := CreateItemAtLocation( mobile.x, mobile.y, mobile.z, "MountPiece", 1 );
	mount_piece.graphic := mount_graphic;
        mount_piece.color := npc.color;
        
	SetName( mount_piece, npc.name );
 
        //Retrieve the mount...
        SetObjProperty( mount_piece, "serial", npc.serial );
        SetObjProperty( mount_piece, "color", npc.color );
	
	var result := EquipItem( mobile, mount_piece );
	if( !result )
		// Unable to equip the mount piece.
		DestroyItem( mount_piece );
		return result;
	else
		// Setting equipped mounts prevents un-equip exploits.
		mount_piece.movable := 0;
	endif
 
        mobile.facing := npc.facing;
        
        MoveObjectToLocation( npc, 5140, 1773, 0, "britannia", MOVEOBJECT_FORCELOCATION );
        SetObjProperty( npc, "mounted", 1 );
        SetObjProperty( npc, "MustSleep", mobile.serial );
        
        npc.hidden := 1;
        npc.frozen := 1;
	
	return mount_piece;
endfunction

function MP_Unmount( mobile )

	var mount_piece := GetEquipmentByLayer( mobile, LAYER_MOUNT );
	if( !mount_piece )
		return error{"errortext":="Mobile does not have a mount item."};
	elseif( Lower( mount_piece.desc ) == "mount piece" )
		// Mount piece exists, but contains no npc information.
		DestroyItem( mount_piece );
		return 0;
	endif
 
	return MP_RestoreMountToAnimal( mobile, mount_piece );
endfunction

function MP_DeathUnmount( corpse )

	if( !corpse.IsA( POLCLASS_CORPSE ))
		return error{"errortext":="Object passed is not a corpse."};
	endif
	
	foreach item in ( EnumerateItemsInContainer( corpse ))
		if ( item.IsMount() )
			return MP_RestoreMountToAnimal( corpse, item );
			break;
		endif
                SleepMS(5);
	endforeach
	
	return error{"errortext":="No mount pieces were found on the corpse."};
endfunction

function MP_RestoreMountToAnimal( mobile, mount )

	var orig_serial := CInt( GetObjProperty( mount, "serial" ));
	var animal := SystemFindObjectBySerial( orig_serial );
	if( animal.IsA( POLCLASS_NPC ))
		// Supports systems where the npc is moved to a storage area rather than killed.
		animal.hidden := 0;
                animal.frozen := 0;
                animal.facing := mobile.facing;

                EraseObjProperty( animal, "MustSleep" );
                
		MoveObjectToLocation( animal, mobile.x, mobile.y, mobile.z, mobile.realm, MOVEOBJECT_FORCELOCATION );
	else
		var template := MP_GetMountTemplate( mount );
		if( !template )
			DestroyItem( mount );
			return template;
		endif
		
		animal := CreateNPCFromTemplate( template, mobile.x, mobile.y, mobile.z );
		animal.facing := mobile.facing;
		animal.color := mount.color;
		animal.name := mount.desc;
  
		var mobile_graphic := MP_GetMobileGraphic( mount );
		if( mobile_graphic )
			animal.graphic := mobile_graphic;
		endif
	endif

	tameCreature( animal, mobile );
        EraseObjProperty( animal, "mounted" );
 
	DestroyItem( mount );
 
        if( mobile.dead )
                var last_damage := GetLastDamageInfo( mobile ),
                    last_attacker := last_damage[1];
                    
                var source := SystemFindObjectBySerial( last_attacker );
                if( source )
                        AI_Attack( animal, source );
                endif
        endif
	
	return animal;
endfunction

function MP_GetMount( mobile )

	var mount_piece := GetEquipmentByLayer( mobile, LAYER_MOUNT );
	if( mount_piece.errortext )
		return error{"errortext":=mount_piece.errortext};
	endif
 
        return 1;
endfunction
