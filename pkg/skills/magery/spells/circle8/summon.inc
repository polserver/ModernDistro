use uo;
use os;

include "include/client";
include ":attributes:attributes";

function summon_creature( caster, circle := 8, npctemplate, cast_loc, controldiff := 50, summontype := "undefined" )
    var thecreature;
    var thecritter;
    var magery := GetAttribute(caster, MAGERY);
    var duration := 20 + (magery * 2);
    var attributelist := { "None" };
    var parms := {}, totem, totemtype;
  
	SendSysMessage(caster, "Select a Totem to summon from, press <ESC> to summon unbound.", color := 88);
	totem := Target(caster);
	totemtype := GetObjProperty(totem, "Totem");
	if (!totemtype)
		SendSysMessage(caster, "Summoning an unbound spirit...", color := 88);
	elseif (totemtype != summontype)
		SendSysMessage(caster, "Your totem has no effect for this spell.", color := 33);
		SendSysMessage(caster, "Summoning an unbound spirit...", color := 88);
		controldiff := controldiff + 10;
	else
		attributelist := GetObjProperty(totem, "Attributes");
		if (!attributelist)
			attributelist := { "None" };
		endif
		npctemplate := GetObjProperty(totem, "SummonName");
	endif
	if (npctemplate=="nightmare")
		duration := CInt(duration * 0.2);
		controldiff := controldiff + 50;
	endif

	if ("Mastery" in attributelist);
		controldiff := controldiff - 20;
	endif

	if (SkillCheck(caster, MEDITATION, controldiff, (circle * 50) ) )
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

	SetBaseSkillBaseValue( thecritter, SKILLID_RESISTING_SPELLS,  magery*10  );
	PlaySoundEffect(caster, SFX_SPELL_SUMMON_ELEMENTAL);

	if ("Longevity" in attributelist)
		duration := CInt(duration * 1.5);
	endif

	if ("Intelligence" in attributelist)
		SetIntelligenceMod(thecritter, CInt(AP_GetSkill(caster, MEDITATION) / 3));
		SetMana(thecritter, thecritter.intelligence);
	endif

	if ("Speed" in attributelist)
		SetDexterityMod(thecritter, CInt(AP_GetSkill(caster, MEDITATION) / 3));
		SetStamina(thecritter, thecritter.dexterity);
	endif

	if ("Strength" in attributelist)
		SetStrengthMod(thecritter, CInt(AP_GetSkill(caster, MEDITATION) / 3));
		SetHp(thecritter, thecritter.strength);
	endif
	Detach();
	sleep(duration);
  if (thecritter)
    PlaySoundEffect( thecritter, SFX_SPELL_MANA_VAMPIRE );
    PlayStationaryEffect( thecritter.x, thecritter.y, thecritter.z, FX_SMOKE, 0xa, 0xa, 0, thecritter.realm );
    MoveObjectToLocation(thecritter, 6143, 0, 0, "britannia_alt", MOVEOBJECT_FORCELOCATION);
    SetScriptController(thecritter);
    ApplyDamage(thecritter, GetHp(thecritter) + 100);
  endif
endfunction
