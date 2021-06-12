use uo;
use os;

include "include/client";
include ":attributes:attributes";

///////////////////////////////////////////////////////////
//
// s_summon_creature( caster, npctemplate, cast_loc, qty := 1, circle := 8 )
//
// Purpose: Summon one or more NPC(s) of a specified NPC template
//			to the on or near the location passed in cast_loc.
//
//
//
// Parameters:	caster ::mobile ref of the character summoning the npc(s).
//				npctemplate ::string containing the NPC template name.
//				cast_loc ::struct containing the location to create
//				the NPC(s).
//				qty ::integer specifies the number of NPCs to create.
//				circle ::integer spcifying the circle level of the
//				spell used to summon the NPC(s).
//				Note: circle is not currently used by this function.
//
// Returns:
//
///////////////////////////////////////////////////////////
function s_summon_creature( caster, npc_template, cast_loc, qty := 1, circle := 8 )

	circle := circle;
	var thecreature;
	var thecritters := { };
	var npcdesc_cfg := ReadConfigFile(":*:npcdesc");
	var npcs := GetConfigStringKeys( npcdesc_cfg );
	if(!npc_template in npcs)
		Print(":sorcery:summon.inc could not find NPC template " + npc_template + ".");
		return 0;
	endif
	var magery_skill := AP_GetSkill(caster,MAGERY);
	var duration := 20 + (magery_skill * 2);
	var xrand_variance := 0;
	var yrand_variance := 0;
	var params := struct;
	var dict_params := dictionary{"master" -> caster.serial, "summoned" -> 1, "NeverRelease" -> 1};
	params.+ "script" := "::tamed";
	params.+ "CProps" := dict_params;

	for i := 1 to qty
		repeat
			// The following 2 lines produce an offset so that
			// multiple NPCs are not created in the same square.
			xrand_variance := RandomIntMinMax(-4,4);
			yrand_variance := RandomIntMinMax(-4,4);
			thecreature := CreateNpcFromTemplate(npc_template, xrand_variance + cast_loc.x, yrand_variance + cast_loc.y,cast_loc.z, params, cast_loc.realm);
			Sleepms(100);
		until(thecreature);
		AP_SetTrueSkill(thecreature, RESISTING_SPELLS, magery_skill*10);
		thecreature.master := caster.serial;
		thecritters.append(thecreature);
	endfor


	PlaySoundEffect(caster, SFX_SPELL_SUMMON_ELEMENTAL);

	Detach();
	sleep(duration);

	for i := 1 to qty
		PlaySoundEffect( thecritters[i], SFX_SPELL_MANA_VAMPIRE );
		PlayStationaryEffect( thecritters[i].x, thecritters[i].y, thecritters[i].z, FX_SMOKE, 0xa, 0xa );
		MoveObjectToLocation(thecritters[i], 0, 0, 0, "britannia_alt", MOVEOBJECT_FORCELOCATION);
		thecritters[i].kill();
	endfor
	return 1;

endfunction

