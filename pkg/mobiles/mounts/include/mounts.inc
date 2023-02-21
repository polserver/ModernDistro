/*
        Maintened by the POL Distro Team

        2009-02-21
*/
use uo;
use os;
use cfgfile;

include "include/client";
include ":attributes:attributes";
include "include/facings";
include "include/eventID";
// include ":animaltaming:taming";
// include ":brainAI:npcCommands";
// include ":damage:damage_ex";

///////////////////////////////////////////////////////////
//
// MP_GetMountsCfgFile()
//
// Purpose: This is an internal function used by other
// 			functions within this file to retrieve
// 			the mounts.cfg file reference.
//
// Parameter: None.
//
// Returns: The mounts.cfg file reference or error
//			if the file cannot be found.
//
///////////////////////////////////////////////////////////
function MP_GetMountsCfgFile()

	var cfg := ReadConfigFile( ":mounts:mounts" );
	if( cfg.errortext )
		SysLog( "Error::MP_GetMountsCfgFile() - Unable to open [:mounts:mounts.cfg] ->"+cfg.errortext );
	endif

	return cfg;
endfunction

///////////////////////////////////////////////////////////
//
// P_GetMountsCfgElem( elem_name, byref cfg_file:=0 )
//
// Purpose: This function returns a reference to an
//			element within mounts.cfg.
//
//
// Parameters:	elem_name:: string name of the element
//				to retrieve.
//				cfg_file:: file reference to mounts.cfg.
//				The default is 0 because it will get the
//				file reference by calling the function
//				MP_GetMountsCfgFile().
//
// Returns:		An element reference or an error if
//				the element cannot be found in mounts.cfg.
//
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
//
// MP_GetMountPieceGraphic( npc_mobile )
//
// Purpose: This function returns the mount piece
// 			ObjType number when the ObjType	of the NPC
//			is passed to the function.
// Example: Pass it 0xCC and it will return 0x3EA2.
//
// Parameter:	npc_mobile:: mobile reference or
//				npc_mobile:: integer - mobile ObjType
//
// Returns: mount piece ObjType (graphic) or error if
// 			the NPC ObjType cannot be found in mounts.cfg
//
///////////////////////////////////////////////////////////
function MP_GetMountPieceGraphic( npc_mobile )
	var npc_mobile_graphic;
	if( npc_mobile.IsA( POLCLASS_NPC ))
		npc_mobile_graphic := Hex( npc_mobile.graphic );
	else
		npc_mobile_graphic := Hex( CInt( npc_mobile ));
	endif

	var elem := MP_GetMountsCfgElem( "MobileToMount" );
	var tile_graphic := GetConfigInt( elem, npc_mobile_graphic );

	if( tile_graphic )
		return tile_graphic;
	else
		return error{"errortext":="Error::MP_GetMountPieceGraphic() - No mount tile graphic matches npc_mobile graphic '"+npc_mobile_graphic+"'"};
	endif
endfunction

///////////////////////////////////////////////////////////
//
// MP_GetMobileGraphic( mount_piece )
//
// Purpose: This function returns the ObjType (graphic)
//			for the mount piece passed to it.
// Example:	Pass it 0x3EA2 and it will return 0xCC.
//
//
// Parameter:	mount_piece:: mount piece object reference
//				or an integer ObjType for a mount piece.
//
// Returns:	ObjType:: integer ObjType for an NPC
//			as defined in the npcdesc.cfg file or error
//			if the mount piece cannot be found
//			in mounts.cfg.
//
///////////////////////////////////////////////////////////
function MP_GetMobileGraphic( mount_piece )

	if( mount_piece.IsA( POLCLASS_ITEM ))
		mount_piece := mount_piece.graphic;
	else
		mount_piece := CInt( mount_piece );
	endif

	var elem := MP_GetMountsCfgElem( "MountToMobileGraphic" );

	var tile_graphic := GetConfigInt( elem, mount_piece );

	if( tile_graphic )
		return tile_graphic;
	else
		return error{"errortext":="Error::MP_GetMobileGraphic() - No mobile tile graphic matches tile graphic '"+mount_piece+"'"};
	endif
endfunction

///////////////////////////////////////////////////////////
//
// MP_GetMountTemplate( mount_piece )
//
// Purpose: Retrieves the NPCTemplate name from a
//			mount piece.
//
//
//
// Parameter:	mount_piece:: object reference to a
//				mountpiece or integer - graphic number
//				of a mountpiece.
//
// Returns:		string:: NPC template name or error.
//
///////////////////////////////////////////////////////////
function MP_GetMountTemplate( mount_piece )

	var mount_graphic, template;
	if( mount_piece.IsA( POLCLASS_ITEM ))
		template := GetObjProperty( mount_piece, "NPCTemplate" );
		if( template )
			return template;
		else
			mount_graphic := Hex( mount_piece.graphic );
		endif
	else
		mount_graphic := Hex( CInt( mount_piece ));
	endif

	if( template )
		return template;
	else
		return error{"errortext":="Error::MP_GetMountTemplate() - No NPC template match for mount piece graphic '"+mount_graphic+"'"};
	endif
endfunction

///////////////////////////////////////////////////////////
//
// MP_Mount( mobile, npc_mount )
//
// Purpose:	This function mounts a character on a
//			mountable NPC.
//
// Parameters:	mobile:: Object reference to a character.
//				npc_mount:: Object reference to a
//				mountable NPC.
//
// Returns:		Mount piece object reference or error.
//
///////////////////////////////////////////////////////////
function MP_Mount( mobile, npc_mount )

	// mobile.graphic := 400 + mobile.gender;
	var mount_piece := MP_GetMountPiece( mobile );
	if( mount_piece )
		return error{"errortext":="Mobile is already mounted."};
	endif

	var mount_graphic := CInt(MP_GetMountPieceGraphic( npc_mount ));
	if( !mount_graphic )
		return mount_graphic;
	endif
	mount_piece := CreateItemAtLocation( mobile.x, mobile.y, mobile.z, "MountPiece", 1 );
	mount_piece.graphic := mount_graphic;
	mount_piece.color := npc_mount.color;

	SetName( mount_piece, npc_mount.name );

	//Retrieve the mount...
	SetObjProperty( mount_piece, "serial", npc_mount.serial );
	SetObjProperty( mount_piece, "color", npc_mount.color );

	var result := EquipItem( mobile, mount_piece );
	if( !result )
		// Unable to equip the mount piece.
		DestroyItem( mount_piece );
		return result;
	else
		// Setting equipped mounts prevents un-equip exploits.
		mount_piece.movable := 0;
	endif

	mobile.facing := npc_mount.facing;

	MoveObjectToLocation( npc_mount, 5302, 1163, 0, "britannia", MOVEOBJECT_FORCELOCATION );
	SetObjProperty( npc_mount, "mounted", 1 );
	SetObjProperty( npc_mount, "MustSleep", mobile.serial );

	npc_mount.hidden := 1;
	npc_mount.frozen := 1;

	return mount_piece;
endfunction

///////////////////////////////////////////////////////////
//
// function MP_Unmount( mobile )
//
// Purpose: Dismounts a character, if mounted.
//
// Parameter: mobile:: Object reference to a mobile.
//
// Returns:	The mount NPC:: object reference
//
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
//
// MP_DeathUnmount( corpse )
//
// Purpose: Finds a mount piece in the corpse
//			and restores the mounted NPC.
//
// Parameter:	corpse:: corpse object reference
//
// Returns:		object reference to the restored NPC
//
///////////////////////////////////////////////////////////
function MP_DeathUnmount( corpse )

	var mobile;
	if( !corpse.IsA( POLCLASS_CORPSE ))
		return error{"errortext":="Object passed is not a corpse."};
	endif

	foreach item in ( EnumerateItemsInContainer( corpse ))
		if ( item.IsMount() )
			mobile := SystemFindObjectBySerial(GetObjProperty(corpse, "serial"), SYSFIND_SEARCH_OFFLINE_MOBILES);
			return MP_RestoreMountToAnimal( mobile, item );
			// return MP_RestoreMountToAnimal( corpse, item );
			break;
		endif
        SleepMS(5);
	endforeach

	return error{"errortext":="No mount pieces were found on the corpse."};
endfunction

///////////////////////////////////////////////////////////
//
// MP_RestoreMountToAnimal( mobile, mount_piece )
//
// Purpose:	Removes the mount piece and restores the NPC.
//
// Parameter:	mobile:: mobile ref to the mounted mobile
//				mount_piece:: Mount piece that will be
//				used to create or restore the mount
//
// Returns:	object ref to the npc that was created
//			or restored.
//
///////////////////////////////////////////////////////////
function MP_RestoreMountToAnimal( mobile, mount_piece )

	var orig_serial := CInt( GetObjProperty( mount_piece, "serial" ));
	var animal := SystemFindObjectBySerial( orig_serial );
	if( animal.IsA( POLCLASS_NPC ))
		// Supports systems where the npc is moved to a storage area rather than killed.
		EraseObjProperty( animal, "MustSleep" );

		MoveObjectToLocation( animal, mobile.x, mobile.y, mobile.z, mobile.realm, MOVEOBJECT_FORCELOCATION );
		animal.hidden := 0;
		animal.frozen := 0;
		animal.facing := mobile.facing;
	else
		var template := MP_GetMountTemplate( mount_piece );
		if( !template )
			DestroyItem( mount_piece );
			return template;
		endif

		animal := CreateNPCFromTemplate( template, mobile.x, mobile.y, mobile.z );
		animal.facing := mobile.facing;
		animal.color := mount_piece.color;
		animal.name := mount_piece.desc;
		animal.hidden := 0;
		animal.frozen := 0;

		var mobile_graphic := MP_GetMobileGraphic( mount_piece );
		if( mobile_graphic )
			animal.graphic := mobile_graphic;
		endif
	endif

	SetObjProperty( animal, "happiness", 100 );
	SetObjProperty( animal, "master", mobile.serial );
	SetObjProperty( animal, "script", animal.script );
    animal.setmaster( mobile );
    animal.script := "tamed";
    RestartScript(animal);
	EraseObjProperty( animal, "mounted" );

	DestroyItem( mount_piece );

	if( mobile.dead )
		var last_damage := GetObjProperty( mobile, "LastDamage" );
		var last_attacker := last_damage.serial;
		var source := SystemFindObjectBySerial(CInt(last_attacker));
		// var opponent := source;
		TurnObjectToward(animal, source.x, source.y);
		SendEvent( animal, Struct{ "source":=source, "type":=EVID_ENGAGED} );
		// var last_damage := GetLastDamageInfo( mobile ),
			// last_attacker := last_damage[1];

		// var source := SystemFindObjectBySerial( last_attacker );
		// if( source )
			// AI_Attack( animal, source );
		// endif
	endif

	return animal;
endfunction

///////////////////////////////////////////////////////////
//
// MP_GetMountPiece( mobile )
//
// Purpose: Finds the mount piece equipped on the rider.
//
//
//
// Parameter:	mobile:: mobile ref to the rider
//
// Returns:		object reference to the mount piece or
//				error if no mount is piece equipped.
//
///////////////////////////////////////////////////////////
function MP_GetMountPiece( mobile )

	var mount_piece := GetEquipmentByLayer( mobile, LAYER_MOUNT );
	if( mount_piece.errortext )
		return error{"errortext":=mount_piece.errortext};
	endif

        return 1;
endfunction
