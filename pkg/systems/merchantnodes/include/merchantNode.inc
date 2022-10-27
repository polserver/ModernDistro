use uo;
use os;

include "include/eventID";
include "include/mrcSpawn";
include "include/NPCUtil";
include "include/sysEvent";
include ":body:bodyHues";
include ":containers:storageAreas";
include ":npcs:randName";
include ":security:merchantNodeReport";

// ev=Struct{ ev.info, ev.serial, ev.nosave }
function MN_CreateNPC( byref ev, byref node )

	var list := node.LoadMerchantInfo(),
		props := MerchantSetup( node, ev.info );

// Setup merchant storage, banker types do not get storage areas...
	if( !( ev.info.Template in { "banker", "bucsbanker" } ) )
		Create_Node_Merc( ev.info.Template, ev.info.Template+" "+ev.info.Anchor.X+" "+ev.info.Anchor.Y+" "+ev.info.Anchor.Z );
	endif

	var npc := CreateNpcFromTemplate( ev.info.Template, ev.info.Anchor.X, ev.info.Anchor.Y, ev.info.Anchor.Z, props, ev.info.Anchor.Realm );
	if( !npc.IsA( POLCLASS_NPC ) )
		MerchantNode_ReportText( node.serial+" -> Unable to create npc at "+ev.info.Anchor.X+" "+ev.info.Anchor.Y+" "+ev.info.Anchor.Z+" "+ev.info.Anchor.Realm+": "+npc.errortext, MN_REPORT_CONSOLE+MN_REPORT_SYSLOG );
		return 0;
	endif

	if( !ev.nosave )
		if( ev.serial != 0 && list.Exists( ev.serial ) )
			list.Erase( ev.serial );
		endif
		list.Insert( npc.serial, ev.info );

		node.SaveMerchantInfo( list );
	endif

	return npc.serial;
endfunction

function MerchantSetup( byref node, byref npc_info )

	var template_elem := NPC_GetNPCConfig( npc_info.Template ),
		anchor := npc_info.Anchor,
		info := npc_info.Info;

// Gender, Name, Color done...
	var color := info.RandomizeSkinHue != 0 ? RandomHumanSkinHue() : template_elem.Color,
		gender := ( info.Gender == -1 || template_elem.Gender != 0 ) ? RandomInt( 2 ) : template_elem.Gender != 1 ? info.Gender : template_elem.Gender,
		name := info.Name == "<Random>" ? "<Random>" : info.Name,
		title := PreserveTitle( template_elem.Name );

// We should check gargoyle and elf names here too but I shan't worry about that now...
	if( name == "<Random>" )
		if( node.realm == "tokuno" )
			name := ( ( gender != 1 ) ? FetchRandomName( "TokunoFemale" ) : FetchRandomName( "TokunoMale" ) );
		else
			name := ( ( gender != 1 ) ? FetchRandomName( "HumanFemale" ) : FetchRandomName( "HumanMale" ) );
		endif
	endif

	var props := Struct{
						"Name" := name+title,
						"Color" := color,
						"TrueColor" := color,
						"Gender" := gender,
						"Graphic" := ( ( gender == 0 ) ? 0x190 : 0x191 ),
						"Facing" := RandomInt( 8 ),
						"Speech_Font" := 0x3,
						"Speech_Color" := info.RandomizeSpeechHue != 0 ? RandomIntMinMax( 2, 1000 ) : 0x0,
						"CProps" := Dictionary {
							"Anchor" -> { anchor.X, anchor.Y, anchor.Z, anchor.Realm },
							"MerchantNode" -> node.serial,
							"MerchantInfo" -> npc_info
							}
						};

	return props;
endfunction

function MN_DeleteNPC( byref ev, byref node )

	var info := node.LoadMerchantInfo(),
		template := "",
		anchor := Struct,
		npc := 0,
		pos := ev.position;

	npc := SystemFindObjectBySerial( ev.serial );
	if( npc.IsA( POLCLASS_NPC ) )
		template := info[ev.serial].Template;
		anchor := info[ev.serial].Anchor;
		Destroy_Node_Merc( template+" "+anchor.X+" "+anchor.Y+" "+anchor.Z );
		MoveObjectToLocation( npc, 6066, 1, 0, _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION );
		SetObjProperty( npc, "#Disabled", 0x1 );
		npc.Kill();
	endif
	info.Erase( ev.serial );
	node.SaveMerchantInfo( info );

	return 1;
endfunction

function MN_RespawnNPC( byref ev, byref death_list, byref node )

	death_list.Insert( ev.serial, Struct{ "Info":=ev.info, "Time":=ReadGameClock()+CInt( CDbl( node.GetRespawnTime()*60.0 ) ) } );

	node.SaveDeathList( death_list );

	return 1;
endfunction

function MN_RestockMerchant( byref ev, byref node )

	var npc := SystemFindObjectBySerial( ev.serial );
	if( !npc.IsA( POLCLASS_NPC ) )
		return 0;
	endif

	var info := node.LoadMerchantInfo(),
		anchor := info[ev.serial].Anchor,
		template := info[ev.serial].Template;

	var shop_sell := CP_GetStorageAreaContainer( template+" "+anchor.X+" "+anchor.Y+" "+anchor.Z+" ShopSell", CP_MERCHANT_AREA, CP_NOCREATE );
	var shop_resale := CP_GetStorageAreaContainer( template+" "+anchor.X+" "+anchor.Y+" "+anchor.Z+" ShopeResale", CP_MERCHANT_AREA, CP_NOCREATE );
	if( !shop_sell || !shop_resale )
		return 0;
	endif
	RestockMyInventory( shop_resale, shop_sell );

	return 1;
endfunction

function MN_SleepNode( byref node )

	// ValidateSpawns( node );
	var status := node.GetStatus();
	if( !status )
		DespawnNode( node );
	else
		SpawnNode( node );
	endif
	// ValidateSpawns( node );
	SetName( node, "a merchant node "+( status ? "[enabled]" : "[disabled]" ) );
	node.color := status ? 0 : 53;

	return 1;
endfunction

function MN_ProcessSpeech( byref event, byref node )

	var mobile := event.source,
		npc := event.npc,
		text := Lower( event.text );

// Just do some error checking here, we check to ensure the mobile is a player
// and that text contains proper keywords...
	var banker_speech := MN_MatchSpeech( text, GetConfigStringArray( MN_FindSettingsElem( "BankSpeech" ), "Text" ) ),
		merchant_speech := MN_MatchSpeech( text, GetConfigStringArray( MN_FindSettingsElem( "MerchantSpeech" ), "Text" ) );
	if( mobile.IsA( POLCLASS_NPC ) )
		return 0;
	elseif( !banker_speech && !merchant_speech )
		return 0;
	endif

	if( !npc.IsA( POLCLASS_NPC ) )
		FindCorrectMerchant( mobile, node, npc, text, banker_speech );
		if( !npc )
			return 0;
		endif
	endif

// Should we verify speech by npc type: bank needs banker, buy needs vendor, etc...?

	if( Distance( mobile, npc ) <= 4 )
		SendEvent( npc, Struct{ "type":=EVID_NODE, "source":=mobile, "text":=text } );
	endif
endfunction

function MN_MatchSpeech( byref text, byref list )

	var i := 1;
	for( i:=1; i<=list.Size(); i++ )
		if( text[list[i]] )
			return 1;
		endif
	endfor

	return 0;
endfunction

/*
 *
 * FindCorrectMerchant( mobile, node, npc, text )
 *
 * Parameters:
 * mobile - mobile player_ref
 * node - node item_ref
 * npc - variable to set to npc mobile_ref to.
 * text - string
 *
 * Purpose:
 * Finds the nearest merchant to the player and if the player uses the merchant's name
 * will use that merchant instead.
 *
 * Date: 2022-10-04 03:44:05
 * Shard Name: Pangaea
 *
 * Author: Oden (ThisIsMe)
 * Email: DavidJSGardner@Gmail.com
 * ICQ: 625-658-845
 * Skype: DavidJSGardner
 * Discord: ThisIsMe#7491
 *
 * Return Values:
 * N/A
 *
 */
function FindCorrectMerchant( byref mobile, byref node, byref npc, text, type )

	var range := 4,
		my_range := 0,
		vendor := 0,
		info := node.LoadMerchantInfo(),
		name := "";

	foreach serial in ( info.Keys() )
		vendor := SystemFindObjectBySerial( serial );
		if( CheckLineOfSight( mobile, vendor ) )
			name := SplitWords( vendor.name );
			if( text[Lower( name[1] )] )
				npc := vendor;
			endif
		endif
	endforeach

	if( !npc.IsA( POLCLASS_NPC ) )
		foreach serial in ( info.Keys() )
			if( CheckLineOfSight( mobile, serial ) )
				vendor := SystemFindObjectBySerial( serial );
				my_range := Distance( mobile, vendor );
				if( my_range < range )
					if( type && ( npc.npctemplate in { "banker", "bucsbanker" } ) )
						npc := vendor;
						range := my_range;
					else
						npc := vendor;
						range := my_range;
					endif
				endif
			endif
		endforeach
	endif
endfunction

function DespawnNode( byref node )

	var info := node.LoadMerchantInfo(),
		npc := 0;
	foreach serial in ( info.Keys() )
		info[serial].Enabled := 0;
		npc := SystemFindObjectBySerial( serial );
		MoveObjectToLocation( npc, 6066, 1, 0, _DEFAULT_REALM, MOVEOBJECT_FORCELOCATION );
		SetObjProperty( npc, "#Disabled", 0x1 );
		npc.Kill();
		SleepMS( 2 );
	endforeach
	node.SaveMerchantInfo( info );

	return 1;
endfunction

function SpawnNode( byref node )

	var info := node.LoadMerchantInfo(),
		new_serial := 0,
		new_info := Dictionary,
		i := 0;

	foreach serial in ( info.Keys() )
		info[serial].Enabled := 1;
		new_serial := MN_CreateNPC( Struct{ "info":=info[serial], "nosave":=1, "serial":=0 }, node );
		new_info.Insert( new_serial, info[serial] );
	endforeach
	node.SaveMerchantInfo( new_info );

	return 1;
endfunction
