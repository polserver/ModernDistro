use uo;
use os;

include "include/client";
include ":attributes:attributes";

function summon_creature( caster, circle := 8, npctemplate, cast_loc, controldiff := 50, summontype := "undefined" )
	summontype := summontype; // fix ecompile warning
	var thecritter;
	var magery := GetAttribute(caster, MAGERY);
	var duration := 20 + (magery * 2);
	var parms := struct;

	if (SkillCheck(caster, MEDITATION, controldiff, (circle * 50) ) > 0)
		parms .+ script := "tamed";
		thecritter := CreateNpcFromTemplate(npctemplate, cast_loc.x, cast_loc.y,cast_loc.z, parms, caster.realm);

		SetObjProperty(thecritter,"master", caster.serial);
		SetObjProperty(thecritter, "summoned",1);
		thecritter.master := caster;
	else
		SendSysMessage(caster, "You fail to control the creature!", color := 33);
		thecritter := CreateNpcFromTemplate(npctemplate, cast_loc.x, cast_loc.y,cast_loc.z, parms, caster.realm);
		SetObjProperty(thecritter,"master", caster.serial);
		SetObjProperty(thecritter, "summoned", 1);
		thecritter.master := caster;
	endif

	if (circle > 7)
		thecritter.setMurderer(1);
	endif

	SetBaseAttributeBaseValue( thecritter, RESISTING_SPELLS,  magery*10  );
	PlaySoundEffect(caster, SFX_SPELL_SUMMON_ELEMENTAL);

	Detach();
	sleep(duration);
  if (thecritter)
    PlaySoundEffect( thecritter, SFX_SPELL_MANA_VAMPIRE );
    PlayStationaryEffect( thecritter.x, thecritter.y, thecritter.z, FX_SMOKE, 0xa, 0xa, 0, thecritter.realm );
    MoveObjectToLocation(thecritter, 6143, 0, 0, "britannia_alt", MOVEOBJECT_FORCELOCATION);
    SetScriptController(thecritter);
    ApplyDamage(thecritter, AP_GetVital(thecritter, VITALID_LIFE) + 100);
  endif
endfunction
