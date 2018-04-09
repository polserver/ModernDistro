/////////////////////////////////////////////////////
// customspells.inc -- Simplified gate and recall code
//                     for use with runebook.src
//
// Author(s):   Racalac
// Revisions:   1.0     11/13/99: Gate, Recall
//                                Mana, Reagent consumption
//                                Normal Spell Restrictions
// Author:      Yukiko
//              2.0     ??/??/06: Added realm support for POL 0.96  
//
// Notes: (rev1.0) - None
//
// Dependencies: (rev1.0) - client.inc
//                          objtype.inc
//                          spellrestrictions.inc
//
/////////////////////////////////////////////////////

use uo;
use os;
use cfgfile;
use vitals;
use attributes;

const RECALL_DELAY_MS	:= 2500;
const GATE_DELAY_MS	:= 4000;


function CustomRecall( caster, x, y, z, torealm, from_charge := 0 )

	var shp	:= caster.hp;
	var sx	:= caster.x;
	var sy	:= caster.y;
	var sz	:= caster.z;
	var smana	:= caster.mana;
//	var circle	:= 4;
	var spelldelay := RECALL_DELAY_MS;

	caster.hidden := 0;
	PerformAction( caster , 0x0010 );
        SpeakPowerWords( caster , 32 );
	
	if( !CanUseBook( caster ) )
		return 0;
	endif

	while( spelldelay >= 4500 )
		Sleepms( 4500 );
		PerformAction(caster, 0x0010);
		spelldelay := spelldelay - 4500;
		if( caster.hp < shp )
			SendSysmessage( caster , "You were interrupted during the casting", color := 33 );
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
			return 0;
		endif

		if( caster.x != sx or caster.y != sy or caster.z != sz )
			SendSysmessage( caster , "You've lost your concentration!", color := 33 );
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
			return 0;
		endif
	endwhile

	Sleepms( spelldelay );

	if( caster.hp < shp )
		SendSysmessage( caster , "You were interrupted during the casting", color := 33 );
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
		return 0;
	endif

	if( caster.x != sx or caster.y != sy or caster.z != sz )
		SendSysmessage( caster , "You've lost your concentration!", color := 33 );
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
		return 0;
	endif

	if( !from_charge )
        	if(!ConsumeReagents( caster, 32 ))
		   	PrintTextAbovePrivate(caster,"*Insufficient reagents*", caster, color := 33);
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
           		return 0;
        	endif
	
        	if(!ConsumeMana( caster , 32 ))
		   	PrintTextAbovePrivate(caster,"*Insufficient mana*", caster, color := 33);
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
           		return 0;
        	endif
	
	        if(!SkillCheck( caster, "magery" , GetSpellDifficulty(32), 50 ) )
        		caster.mana := smana;
			PrintTextAbovePrivate(caster,"*The spell fizzles*", caster, color := 33);
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
           		return 0;
        	endif
        endif
	
	if( caster.multi and GetObjProperty( caster.multi , "ownerserial" ) != caster.serial )
		var co_owners := {};
		var friends := {};
		var i;
		for( i := 1 ; i <= 5 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "Co-owner" + i ) )
				co_owners.append( GetObjProperty( caster.multi , "Co-owner" + i ));
			endif
		endfor
		
		for( i := 1 ; i <= 20 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "friend" + i ) )
				friends.append( GetObjProperty( caster.multi , "friend" + i ));
			endif
		endfor
			
		if( !( caster.serial in co_owners ) and !( caster.serial in friends ) )
			SendSysMessage( caster , "You can't recall away from there!", color := 33 );
			return 0;
		endif
	endif

	var tox := x;
	var toy := y;
	var toz := z;
	var map := caster.realm;
	if(caster.cmdlevel)
	    map := torealm;
	endif
	    

	PlaySoundEffect( caster, 0x0210 );
	sleepms(250);
	MoveObjectToLocation( caster, tox, toy, toz, caster.realm );
	PlaySoundEffect( caster, 0x01fd );
	if( caster.multi and GetObjProperty( caster.multi , "ownerserial" ) != caster.serial )
		var co_owners := {};
		var friends := {};
		var i;
		for( i := 1 ; i <= 5 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "Co-owner" + i ) )
				co_owners.append( GetObjProperty( caster.multi , "Co-owner" + i ));
			endif
		endfor
		
		for( i := 1 ; i <= 20 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "friend" + i ) )
				friends.append( GetObjProperty( caster.multi , "friend" + i ));
			endif
		endfor
			
		if( !( caster.serial in co_owners ) and !( caster.serial in friends ) )
			SendSysMessage( caster , "You can't recall there!", color := 33 );
			MoveObjectToLocation( caster , sx , sy , sz, map, MOVEOBJECT_FORCELOCATION );
			return 0;
		endif
	endif

        return 1;
        
endfunction


function CustomGate( caster, x, y, z, torealm )

	var shp	:= caster.hp;
	var sx	:= caster.x;
	var sy	:= caster.y;
	var sz	:= caster.z;
	var smana	:= caster.mana;
//	var circle	:= 7;
	var spelldelay := GATE_DELAY_MS;

	caster.hidden := 0;
	PerformAction( caster , 0x0010 );
        SpeakPowerWords( caster , 52 );

	if( !CanUseBook( caster ) )
		return 0;
	endif
	
	while( spelldelay >= 4500 )
		Sleepms( 4500 );
		PerformAction(caster, 0x0010);
		spelldelay := spelldelay - 4500;
		if( caster.hp < shp )
			SendSysmessage( caster , "You were interrupted during the casting" );
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
			return 0;
		endif

		if( caster.x != sx or caster.y != sy or caster.z != sz )
			SendSysmessage( caster , "You've lost your concentration!" );
			PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
			PlaySoundEffect(caster, 0x05d);
			return 0;
		endif
	endwhile

	Sleepms( spelldelay );

	if( caster.hp < shp )
		SendSysmessage( caster , "You were interrupted during the casting" );
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
		return 0;
	endif

	if( caster.x != sx or caster.y != sy or caster.z != sz )
		SendSysmessage( caster , "You've lost your concentration!" );
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
		return 0;
	endif
	
        if(!ConsumeReagents( caster, 52 ))
	   	PrintTextAbovePrivate(caster,"*No reagents!*", caster);
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
        	return 0;
        endif

        if(!ConsumeMana( caster , 52 ))
	   	PrintTextAbovePrivate(caster,"*No mana!*", caster);
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
        	return 0;
        endif

        if(!SkillCheck( caster, "magery", GetSpellDifficulty(52), 50 ) )
	       	caster.mana := smana;
		PlayObjectCenteredEffect(caster, 0x3735, 7, 7 );
		PlaySoundEffect(caster, 0x05d);
		SendSysmessage(caster, "The spell fizzles");
        	return 0;
        endif

	if( caster.multi and GetObjProperty( caster.multi , "ownerserial" ) != caster.serial )
		var co_owners := {};
		var friends := {};
		var i;
		for( i := 1 ; i <= 5 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "Co-owner" + i ) )
				co_owners.append( GetObjProperty( caster.multi , "Co-owner" + i ));
			endif
		endfor
		
		for( i := 1 ; i <= 20 ; i := i + 1 )
			if( GetObjProperty( caster.multi , "friend" + i ) )
				friends.append( GetObjProperty( caster.multi , "friend" + i ));
			endif
		endfor
			
		if( !( caster.serial in co_owners ) and !( caster.serial in friends ) )
			SendSysMessage( caster , "You can't gate from there!" );
			return 0;
		endif
	endif
	
	var tox := x;
	var toy := y;
	var toz := z;
	var map := caster.realm;
	if(caster.cmdlevel)
	    map := torealm;
	endif

	set_critical( 1 );
	var pregate := CreateItemAtLocation( caster.x, caster.y, caster.z, 0x1af3, 1, caster.realm );
	pregate.facing := 1;
	PlaySoundEffect( caster, 0x020f );
	sleepms(1500);

	var gate1 := CreateItemAtLocation( caster.x, caster.y, caster.z, 0x0f6c, 1, caster.realm );
	gate1.facing := 1;
	destroyitem(pregate);

	var gate2 := CreateItemAtLocation( tox, toy, toz, 0x0f6c, 1, map );
	gate2.facing := 2;
	if( gate2.multi )
		DestroyItem(gate2);
		SendSysMessage( caster , "You can't gate there.", color := 33 );
	endif
	
	if (!gate1 or !gate2)
		DestroyItem(gate1);
		DestroyItem(gate2);
		return 0;
	endif

	SetObjProperty( gate1, "GateDestX", gate2.x );
	SetObjProperty( gate1, "GateDestY", gate2.y );
	SetObjProperty( gate1, "GateDestZ", gate2.z );
	SetObjProperty( gate1, "DestRealm", gate2.realm );
	SetObjProperty( gate2, "GateDestX", gate1.x );
	SetObjProperty( gate2, "GateDestY", gate1.y );
	SetObjProperty( gate2, "GateDestZ", gate1.z );
	SetObjProperty( gate2, "DestRealm", gate1.realm );

	Detach();

	set_critical( 0 );

	PlaySoundEffect( caster, 0x020f );

	Sleep(30);
    
	set_critical(1);
	DestroyItem( gate1 );
	DestroyItem( gate2 );
	set_critical(0);

        return 1;
        
endfunction


function CanUseBook( me )

	var hand1 := GetEquipmentByLayer( me , 0x01 );
	var hand2 := GetEquipmentByLayer( me , 0x02 );

	if( hand1 or hand2 )
		var weapcfg := ReadConfigFile( ":*:itemdesc" );
		
		if( hand1 and weapcfg[hand1.objtype].BlocksCastingIfInHand != 0 )
			SendSysmessage( me , "Your hands must be free to use that.", color := 33 );
			PlayObjectCenteredEffect( me , 0x3735 , 7 , 7 );
			PlaySoundEffect( me , 0x05d );
			return 0;
		endif
		
		if( hand2 and weapcfg[hand2.objtype].BlocksCastingIfInHand != 0)
			SendSysmessage( me , "Your hand must be free to use that.", color := 33 );
			PlayObjectCenteredEffect( me , 0x3735 , 7 , 7 );
			PlaySoundEffect( me , 0x05d );
			return 0;
		endif
	endif
	
	return 1;
		
endfunction
