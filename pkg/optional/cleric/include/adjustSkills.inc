function SetSkills(who)

	// Skill adjustments to be made when an individual becomes a cleric.
	if( ( CDbl( GetAttributeBaseValue( who, MEDITATION ) ) / 10.0 ) < 40.0 )
		SetAttributeBaseValue( who, MEDITATION, CInt( CDbl( 40 ) * 10.0 ) );
	endif

	if( ( CDbl( GetAttributeBaseValue( who, SPIRIT_SPEAK ) ) / 10.0 ) < 40.0 )
		SetAttributeBaseValue( who, SPIRIT_SPEAK, CInt( CDbl( 40 ) * 10.0 ) );
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
	var pre_cleric_skills := Struct{
		HIDING:=GetAttributeBaseValue( who, HIDING ),
		SWORDSMANSHIP:=GetAttributeBaseValue( who, SWORDSMANSHIP ),
		STEALTH:=GetAttributeBaseValue( who, STEALTH ),
		FENCING:=GetAttributeBaseValue( who, FENCING ),
		MAGERY:=GetAttributeBaseValue( who, MAGERY )
	};
	SetObjProperty(who, "PreClericSkills", pre_cleric_skills);

	// Drop the skills that a cleric 'should' not have.
	// These will be returned if the cleric leaves the faith.
	SetAttributeBaseValue( who, HIDING, 0 );
	AP_ModifySkillMod(who, HIDING, 0);
	SetAttributeCap(who, HIDING, 0);

	SetAttributeBaseValue( who, SWORDSMANSHIP, 0 );
	AP_ModifySkillMod(who, SWORDSMANSHIP, 0);
	SetAttributeCap(who, SWORDSMANSHIP, 0);


	SetAttributeBaseValue( who, STEALTH, 0 );
	AP_ModifySkillMod(who, STEALTH, 0);
	SetAttributeCap(who, STEALTH, 0);

	SetAttributeBaseValue( who, FENCING, 0 );
	AP_ModifySkillMod(who, FENCING, 0);
	SetAttributeCap(who, FENCING, 0);

	SetAttributeBaseValue( who, MAGERY, 0 );
	AP_ModifySkillMod(who, MAGERY, 0);
	SetAttributeCap(who, MAGERY, 0);
endfunction

function ResetSkills(who)

	// Penalize the excommunicated cleric.
	SetAttributeBaseValue( who, MEDITATION, 0 );
	SetAttributeBaseValue( who, SPIRIT_SPEAK, 0 );

	// Restore skill caps to the defaults
	SetAttributeCap(who, HIDING, GetAttributeDefaultCap( HIDING ));
	SetAttributeCap(who, SWORDSMANSHIP, GetAttributeDefaultCap( SWORDSMANSHIP ));
	SetAttributeCap(who, STEALTH, GetAttributeDefaultCap( STEALTH ));
	SetAttributeCap(who, FENCING, GetAttributeDefaultCap( FENCING ));
	SetAttributeCap(who, MAGERY, GetAttributeDefaultCap( MAGERY ));

	// Restore the pre-cleric skills that were dropped.
	var pre_cleric_skills := GetObjProperty(who, "PreClericSkills");
	foreach key in ( pre_cleric_skills.Keys() )
		SetAttributeBaseValue( who, key, pre_cleric_skills[key] );
	endforeach
endfunction
