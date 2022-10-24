include ":npcs:skillLists";

var cost;

function MerchantTrain(me, who, text)

	// A player's skill cannot be trained higher than max_skill_to_train
	var max_skill_to_train := 10;
	var max_train_cost := max_skill_to_train * 10;
	var actual_train_cost := 0;

	if(GetObjProperty(who, "#arena"))
		say("You cannot train in an arena");
		return;
	endif
	var totaltrain := 0;
	var words := SplitWords(text);
	var skill := words[3];
	if(words[4])
		skill := skill + " " + words[4];
	endif
	if(!skill)
		var trainable := "";
		var holder := "";
		foreach thing in getskilllist(me)
			if(holder == "")
				holder := thing;
			else
				holder := holder + ", " + thing;
			endif
			totaltrain := totaltrain + 1;
		endforeach
		trainable  := trainable + holder;
		if(totaltrain > 0)
			say("I can train thee in the following skills: ");
			say(trainable);
		else
			say("I cannot train thee.");
		endif
	else
		skill := lower(skill);
		foreach attrib_name in getskilllist(me)
			attrib_name := lower(attrib_name);
			if( attrib_name == skill )
				// All values are in "tenths" unless noted otherwise.
				// This simplifies things because the amount of gold to buy training is
				// the same as the training level in tenths.

				// max_skill_to_train is the maximum points a vendor can train a player, in tenths.
				var max_skill_to_train := 100;
				var player_skill_cap := who.skillcap * 10;
				// player_total_skill_pts is the total of all of the player's earned skill points.
				var player_total_skill_pts := AP_GetPlayerTotalSkillPoints( who );
				// available_point_pool is the unused points available for gains.
				var available_point_pool := player_skill_cap - player_total_skill_pts;
				if( player_skill_cap - player_total_skill_pts <= 0 )
					// No room for training any skill points.
					Say( "I could try to train you in " + attrib_name + ", but you would be wasting your coin." );
					return;
				endif
				// Here we check to see if the player can be trained the max_skill_to_train amount.
				var variance := available_point_pool - max_skill_to_train;
				if( variance < 0 )
					// Nope. Train to the variance level.
					max_skill_to_train := Abs( variance );
				endif
				PrintTextAbovePrivate( me, "I will train the in " + attrib_name + " for " + max_skill_to_train + ".", who );
				SetObjProperty(who, "TrainMaxCost", max_skill_to_train);
				SetObjProperty(who, "TrainSkill", attrib_name);
				return;
			endif
		endforeach
		PrintTextAbovePrivate(me, "I know not of that skill.", who);
	endif
endfunction

function TrainSkill(me, who, item)
	if(((Lower(item.name[1,4]) == "head") && (item.name.Length() < 6))  and ((me.npctemplate == "townguard") or (me.npctemplate == "orderguard") or (me.npctemplate == "chaosguard")))
		var parms := array;
		parms.append(me);
		parms.append(who);
		parms.append(item);
		MoveObjectToLocation(item, 5852, 1161, 0, "britannia_alt", MOVEOBJECT_FORCELOCATION);
		item.movable := 0;
		ReleaseItem(item);
		start_script(":bountyboard:collectBounty", parms);
		return;
	endif
	if((item.objtype == 0xeed) && (GetObjProperty(who, "Hire") == me.serial))
		if(GetObjProperty(who, "#arena"))
			say("Im not here for a job!  I'm here to fight!");
			return;
		else
			hireme(me, who, item);
			return;
		endif
	endif
	if(GetObjProperty(who, "#arena"))
		say("You cannot train in an arena");
		return;
	endif
	var gold_received := item.amount;
	if((item.objtype == 0xeed) && (item.amount != 0))
		var train_max_cost := GetObjProperty(who, "TrainMaxCost");
		var attribute_name := GetObjProperty(who, "TrainSkill");
		// If coins are dropped on a vendor and no train request was previously asked for.
		if(!train_max_cost || !attribute_name)
			say("I don't know what this is for, but thanks!");
			DestroyItem(item);
			return;
		endif
		// If the player gives too much gold.
		if(gold_received > train_max_cost)
			say( "You gave me too much gold. I returned the excess." );
			var excess_gold := gold_received - train_max_cost;
			CreateItemInContainer( who.backpack, 0xEED, excess_gold );
			gold_received := gold_received - excess_gold;
		endif
		SetAttributeBaseValue( who, attribute_name, gold_received );
		PrintTextAbovePrivate(me, "Let me show you how it's done.", who);
		DoRecalcVitals(who);
		DestroyItem(item);
		SendSysMessage(who, "Your skill increases.");
	else
		say("Bah, I have no use for this. Be off with ye.");
		MoveItemToContainer(item, who.backpack);
	endif
endfunction

function GoldForSkillGain(npc_skill, pc_skill)
	return Cint((npc_skill - pc_skill) * 10);
endfunction

function SkillGainForGold(pc_level, gold_received)
	return (gold_received / 10) + pc_level;
endfunction

function hireme(me, who, item)
	var amt := item.amount;
	if(amt < cost)
		say("I will not work for any less than " + cost + " gold coins per day.");
		MoveItemToContainer(who.backpack, item);
	else
		EraseObjProperty(who, "Hire");
		var days := amt / cost;
		say("I will work for you for " + days + " days.");
		DestroyItem(item);
		SetObjProperty(me, "master", who.serial);
		SetObjProperty(me,"script", me.script);
		days := days * 10800;
		SetObjProperty(me, "Timer", (ReadGameClock() + days));
		me.script := "employed";
		RestartScript(me);
		return;
	endif
endfunction

//////////////////////////////////////////////////////////////////////////////////////////
//
//	FindDiffOfSkillsCapAndSkillToTrain( who )
//
//	Purpose:	Determines the remainig, if any, skill points the player has
//				available before he reaches his total skill cap.
//
//	Parameter:	mobileref::who
//
//	Returns:	integer (in 'tenths')::the remaining skill points, or <= 0 if none
//				available.
//
//	Created:	2022-10-16 02:04:24 by Yukiko
//
//////////////////////////////////////////////////////////////////////////////////////////
function FindDiffOfSkillsCapAndSkillToTrain( who )
	var player_skill_cap := who.skillcap * 10;
	var player_total_skill_pts := AP_GetPlayerTotalSkillPoints( who );
	// We need to determine how many skill points the player can be trained.
	var difference := player_skill_cap - player_total_skill_pts;
	return difference;
endfunction

