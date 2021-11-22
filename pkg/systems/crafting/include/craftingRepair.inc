// //$Id: craftingRepair.inc 1471 2009-03-11 23:51:53Z muaddib_pol $

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	name:		repair.inc
//	version: 2.0
//	author:	MuadDib
//
//	Purpose: Contains all the central Repair functions involving repairing items, using repair
//					 deeds, etc.
//
//	Changes: Sept 14, 2005 - Updated repair_item() for swing counter (new crafting ready)
//		- Added CreateRepairDeed() Function with support for the following skills:
//		Smithy, Bowcraft, Carpenter, Tailor (new crafting ready)
//		- Moved repair_this() function to the include (new crafting ready)
//		- Moved s_weapon_armor() to the include (new crafting ready)
//		- Moved resmelt() to include (New crafting ready)
//
////////////////////////////////////////////////////////////////////////////////////////////////////


function repair_item(character, what, craft_skill )
	var maxhp;
	maxhp := what.maxhp;
	maxhp := CInt(maxhp * what.quality);
	var thehp := what.hp;
	var damaged := maxhp - thehp;
	var hisskill := AP_GetSkill(character, craft_skill);
	var damageamount := 0.0;
	damageamount := (125 -	hisskill)/3000.0;
	if(what.hp == what.maxhp)
		SendSysMessage(character, "That item is not damaged.");
		return 0;
	elseif((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
		SendSysMessage(character, "The brittle material breaks when you handle it");
		DestroyItem(what);
		return 0;
	elseif(what.quality <= damageamount)
		SendSysMessage(character, "The brittle material breaks when you handle it");
		DestroyItem(what);
		return 0;
	endif
	what.quality := what.quality - damageamount;
	if((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
		SendSysMessage(character, "The brittle material breaks when you handle it");
		DestroyItem(what);
		return 0;
	elseif (SkillCheck( character, craft_skill, CInt(damaged / 3), 0 ) > 0)
		var repaired_amount := CInt( ((randomint(hisskill) + 1))/2);
		if (damaged == 1)
			repaired_amount := 1;
		endif
		if (thehp + repaired_amount > what.maxhp)
			what.hp := what.maxhp;
			EraseObjProperty(what, "swings");
			SendSysMessage(character, "You	repair the item completely");
		else
			what.hp := thehp + repaired_amount;
			SendSysMessage(character, "You successfully repair some of the damage");
		endif
		if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
			SendSysMessage(character, "The brittle material breaks when you handle it");
			DestroyItem(what);
			return 0;
		endif
	else
		var damage_amount := randomint(10);
		if (thehp <= damage_amount)
			DestroyItem(what);
			SendSysMessage(character, "You fail and destroy the item");
		endif
		thehp := thehp - damage_amount;
		what.hp := thehp;
		if(randomint(2) == 1)
			what.quality := what.quality - (damaged/100);
		endif
		if(thehp <= 0)
			DestroyItem(what);
			SendSysMessage(character, "You fail and destroy the item");
		else
			SendSysMessage(character, "You fail and damage the item");
		endif
		if ((what.hp <= 0) || (what.quality <= 0.1) || (!what.quality))
			SendSysMessage(character, "The brittle material breaks when you handle it");
			DestroyItem(what);
			return 0;
		endif
	endif
endfunction

function CreateRepairDeed(who, scroll, craft_skill)

	var type, title, color;
	case( craft_skill )
		 BLACKSMITHY: 			type := "Blacksmith"; color := CInt(2110);
		 BOWCRAFT_FLETCHING: 	type := "Bowyer"; color := CInt(2116);
		 CARPENTRY: 			type := "Carpenter"; color := CInt(2113);
		 TAILORING: 			type := "Tailor"; color := CInt(2128);
	endcase
	var ramt := AP_GetSkill(who, craft_skill);
	if(ramt < 50)
		SendSysMessage(who, "You must be at least apprentice level to create a repair service contract.");
		return 0;
	endif
	if(ramt < 60 )
		title := "A repair service contract from an Apprentice "+type;
	elseif(ramt < 70 )
		title := "A repair service contract from a Journeyman "+type;
	elseif(ramt < 80 )
		title := "A repair service contract from an Expert "+type;
	elseif(ramt < 90 )
		title := "A repair service contract from an Adept "+type;
	elseif(ramt < 100 )
		title := "A repair service contract from a Master "+type;
	elseif(ramt < 110 )
		title := "A repair service contract from a Grandmaster "+type;
	elseif(ramt < 120 )
		title := "A repair service contract from an Elder "+type;
	elseif(ramt < 130 )
		title := "A repair service contract from a Legendary "+type;
	endif
	type := Lower(type);
	var ret := GetItemDescriptor(0x2EE09);
	ret.Color := color;
	ret.CProps.Insert("type", type);
	ret.CProps.Insert("skill", craft_skill);
	ret.CProps.Insert("level", ramt);
	var theitem := CreateItemInBackpack(who, ret, 1);
	if(theitem)
		SubtractAmount(scroll, 1);
	endif
	SetName(theitem, title);
	return 0;

endfunction

function repair_this(who, craft_skill := BLACKSMITHY)
	SendSysMessage(who, "Select item to repair.");
	var use_on := Target(who);
	if ( !use_on || use_on.errortext )
		return 0;
	endif

	if(use_on.movable == 0)
		SendSysMessage(who, "You cannot repair that.");
		return 0;
	elseif((!Accessible(who, use_on)) or (distance(who, use_on) > 2))
		SendSysMessage(who, "You cannot reach that.");
		return 0;
	endif
	if(CInt(use_on.objtype) == CInt(0xe34))
		case( craft_skill )
			 BLACKSMITHY:		CreateRepairDeed(who, use_on, craft_skill); break;
			 BOWCRAFT_FLETCHING:	CreateRepairDeed(who, use_on, craft_skill); break;
			 CARPENTRY:		CreateRepairDeed(who, use_on, craft_skill); break;
			 TAILORING:		CreateRepairDeed(who, use_on, craft_skill); break;
		endcase
		return 0;
	endif
	var forge := 0;
	case( CInt(craft_skill) )
		 BLACKSMITHY: forge := 1;
	endcase
	if ( !forge )
		if ( is_weapon_armor(use_on.objtype) )
			repair_item( who, use_on, craft_skill);
		else
			SendSysMessage(who, "You cannot repair that.");
		endif
	else
		if ( is_weapon_armor(use_on.objtype) )
			var near_items := ListItemsNearLocation(who.x, who.y, who.z,3);
			foreach item in near_items
				if ( (item.objtype == 0xFAF) || (item.objtype == 0xFB0) )
					repair_item( who, use_on, craft_skill);
					return 0;
				endif
			endforeach
			SendSysMessage( who, "You must be near an anvil to repair items!" );
			return 0;
		else
			SendSysMessage(who, "You cannot repair that");
		endif
	endif
	return 0;
endfunction

function is_weapon_armor(what)
	var combatcfg := ReadConfigFile(":*:itemdesc");
	var elem := FindConfigElem(combatcfg, what);
	var repairable := GetConfigInt(elem, "repairable");
	if ( repairable == 1 )
		return 1;
	else
		return 0;
	endif
endfunction

function resmelt(who, the_attribute)

	var near_items := ListItemsNearLocation(who.x, who.y, who.z, 3);
	var forge_near := 0;
	foreach forge in near_items
		if ( (forge.objtype == 0xfb1) || ((forge.objtype >= 0x197a) && (forge.objtype <= 0x19a9)) )
			forge_near := 1;
			break;
		endif
	endforeach
	if ( forge_near == 0 )
		SendSysMessage(who, "You must be near a forge.");
		return 0;
	endif

	SendSysMessage(who, "Select the item you wish to melt.");
	var use_on := Target(who, TGTOPT_CHECK_LOS);
	if ( !use_on || use_on.errortext )
		return 0;
	endif
	if ( use_on.container.serial != who.backpack.serial )
		SendSysMessage(who, "That is not in your backpack.");
		return 0;
	endif
	if ( !Accessible(who, use_on) || !ReserveItem( use_on ) )
		SendSysMessage(who, "You cannot use that.");
		return 0;
	endif
	if ( use_on.movable == 0 )
		SendSysMessage(who, "You cannot use that while it is locked down.");
		return 0;
	endif
	if ( use_on in ListEquippedItems(who) )
		SendSysMessage(who, "You cannot use that.");
		return 0;
	endif

	var cfgarray := GetConfigStringKeys(resource_config);
	var ingot := -1;
	if(use_on.isA(POLCLASS_CONTAINER))
		var contitems := EnumerateItemsInContainer(use_on);
		var skill, amt;
		var loop_counter := 3;
		foreach thing in contitems
			loop_counter := loop_counter + 1;
			if( loop_counter == 4 )
				SendSysMessage(who, "Smelting items in container......");
				loop_counter := 0;
			endif
			if(craft_config[thing.objtype])
				skill := craft_config[thing.objtype].skill;
				amt := CInt(craft_config[thing.objtype].material);
				if((!skill) || (!amt))
					return 0;
				endif
				skill := CInt(skill/2);
				if(skill < 5)
					skill := 5;
				endif
				amt := CInt((amt * AP_GetSkill(who, the_attribute) * 75 ) / 10000);
				if(amt < 1)
					amt := 1;
				endif
				ingot := -1;
				var mattype := CInt(GetObjProperty(use_on, "material"));
				if(mattype > 0)
					if(mattype != error)
						ingot := mattype;
					endif
				endif
				if(ingot == -1)
					foreach entry in cfgarray
						if(Lower(resource_config[entry].Type) == "metal")
							if(CInt(resource_config[entry].Color) == CInt(thing.color))
								ingot := entry;
							endif
						endif
					endforeach
				endif
				if(ingot == -1)
					SendSysMessage(who, "You have no idea how to smelt this strange ore!");
					return 0;
				endif
				PlaySoundEffect(who, 0x2c);
				Sleep(1);
				PlaySoundEffect(who, 0x22);
				if(DestroyItem(thing))
					if(SkillCheck( who, the_attribute, skill, 10 ) > 0)
						CreateItemInContainer(who.backpack, ingot, amt);
					else
						SendSysMessage(who, "You destroy the item.");
					endif
				endif
			endif
			SleepMS(500);
		endforeach
	else
		if(!craft_config[use_on.objtype])
			return 0;
		endif
		ingot := -1;
		var skill := craft_config[use_on.objtype].skill;
		var amt := CInt(craft_config[use_on.objtype].material);
		if((!skill) || (!amt))
			return 0;
		endif
		skill := CInt(skill/2);
		if(skill < 5)
			skill := 5;
		endif
		amt := CInt((amt * AP_GetSkill(who, the_attribute) * 75 ) / 10000);
		if(amt < 1)
			amt := 1;
		endif
		var mattype := CInt(GetObjProperty(use_on, "material"));
		if(mattype > 0)
			if(mattype != error)
				ingot := mattype;
			endif
		endif
		if(ingot == -1)
			foreach entry in cfgarray
				if(Lower(resource_config[entry].Type) == "metal")
					if(CInt(resource_config[entry].Color) == CInt(use_on.color))
						ingot := entry;
					endif
				endif
			endforeach
		endif
		if(ingot == -1)
			SendSysMessage(who, "You have no idea how to smelt this strange ore!");
			return 0;
		endif
		PlaySoundEffect(who, 0x2c);
		Sleep(1);
		if( DestroyItem(use_on) )
			PlaySoundEffect(who, 0x22);
			if(SkillCheck( who, the_attribute, skill, 10 ) > 0)
				CreateItemInContainer(who.backpack, ingot, amt);
			else
				SendSysMessage(who, "You destroy the item.");
			endif
		else
			SendSysMessage(who, "Couldn't destroy "+use_on.name);
		endif
	endif

endfunction