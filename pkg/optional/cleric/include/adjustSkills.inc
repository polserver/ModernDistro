function SetSkills(who)

	// Skill adjustments to be made when an individual becomes a cleric.
	if (GetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(MEDITATION)) < 40)
		SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(MEDITATION), 40);
	endif
	if (GetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(SPIRIT_SPEAK)) < 40)
		SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(SPIRIT_SPEAK), 40);
	endif
	// This is as close to a class system as I am willing to get.
	// The following skill limitations are based on priest restrictions
	// from the view that priests should not use weapons that are intended
	// draw blood (eg. blades), magic, or deceptive practices, eg. hiding.
	// It could be argued that depending on the faith's alignment certain
	// restricted skills might be allowed, ie. evil religions might allow
	// hiding and stealth.
	// Given the fact that priests have rites, which are for all intents and
	// purposes spell casting, you should think carefully before giving
	// magery to priests.
	//
	// Preserve the pre-cleric skills.
	var pre_cleric_skills := struct{"hiding", "swordsmanship", "stealth", "fencing", "magery"};
	pre_cleric_skills.hiding := GetAttributeBaseValue( who, HIDING );
	pre_cleric_skills.swordsmanship := GetAttributeBaseValue( who, SWORDSMANSHIP );
	pre_cleric_skills.stealth := GetAttributeBaseValue( who, STEALTH );
	pre_cleric_skills.fencing := GetAttributeBaseValue( who, FENCING );
	pre_cleric_skills.magery := GetAttributeBaseValue( who, MAGERY );
	SetObjProperty(who, "PreClericSkills", pre_cleric_skills);
	// Drop the skills that a cleric 'should' not have.
	// These will be returned if the cleric leaves the faith.
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(HIDING), 0);
	AP_ModifySkillMod(who, HIDING, 0);
	SetAttributeCap(who, HIDING, 0);
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(SWORDSMANSHIP), 0);
	AP_ModifySkillMod(who, SWORDSMANSHIP, 0);
	SetAttributeCap(who, SWORDSMANSHIP, 0);
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(FENCING), 0);
	AP_ModifySkillMod(who, STEALTH, 0);
	SetAttributeCap(who, STEALTH, 0);
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(STEALTH), 0);
	AP_ModifySkillMod(who, FENCING, 0);
	SetAttributeCap(who, FENCING, 0);
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(MAGERY), 0);
	AP_ModifySkillMod(who, MAGERY, 0);
	SetAttributeCap(who, MAGERY, 0);

endfunction

function ResetSkills(who)

	// Penalize the excommunicated cleric.
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(MEDITATION), 0);
	SetBaseSkillBaseValue(who, AP_AttributeNameToSkillId(SPIRIT_SPEAK), 0);
	// Restore skill caps to the defaults
	SetAttributeCap(who, HIDING, GetAttributeDefaultCap( HIDING ));
	SetAttributeCap(who, SWORDSMANSHIP, GetAttributeDefaultCap( SWORDSMANSHIP ));
	SetAttributeCap(who, STEALTH, GetAttributeDefaultCap( STEALTH ));
	SetAttributeCap(who, FENCING, GetAttributeDefaultCap( FENCING ));
	SetAttributeCap(who, MAGERY, GetAttributeDefaultCap( MAGERY ));
	// Restore the pre-cleric skills that were dropped.
	var pre_cleric_skills := GetObjProperty(who, "PreClericSkills");
	SetAttributeBaseValue( who, HIDING, pre_cleric_skills.hiding );
	SetAttributeBaseValue( who, SWORDSMANSHIP, pre_cleric_skills.swordsmanship );
	SetAttributeBaseValue( who, STEALTH, pre_cleric_skills.stealth );
	SetAttributeBaseValue( who, FENCING, pre_cleric_skills.fencing );
	SetAttributeBaseValue( who, MAGERY, pre_cleric_skills.magery );

endfunction
