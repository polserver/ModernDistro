// //$Id: crafting.inc 1484 2009-03-12 21:27:20Z muaddib_pol $

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	name:		Main Crafting Backbone (include file)
//	version: 1.0b3
//	author:	MuadDib
//
//	Purpose: Functions for creating crafting menus, info menus, item creation, resource selection,
//					 skill gain, etc.
//
//	To-Do:	1.) Create some cusotmized crafting includes for inscription, etc that require it.
//			2.) Create textcmds for editing the Menu and item configs, to add new items ingame.
//			3.) Create MethodScript system for Toolwear!
//			4.) UPGRADE all buttons (except ones to make the item, which uses the button's objtype)
//				to use the 0xF000 - 0xFFFF range. That range is CORE RESERVED for objtypes, thus,
//				should NEVER contain custom items added by shard admins. Therefore anything in that
//				range should not be craftable and thus, not in these menus.
//			5.)	Add Recipe ability for items.
//			6.)	Add Required Object entry for Items
//				Item Level means that objtype is required to be near for this individual item.
//				This is mostly for Cooking only, but could be used in any other skill for other
//				skills for example, to ensure near a loom for Tailoring for certain items, etc.
//
////////////////////////////////////////////////////////////////////////////////////////////////////
use polsys;
use datafile;

include ":attributes:attributes";
include ":datafile:datafile";
// include ":keys:key";
include ":crafting:craftGumps";
include ":crafting:craftingSkill";
include ":gumps:gumps";
include ":gumps:gumps_ex";
include ":itemutils:itemdesc";

CONST CRAFT_CYCLE_ATTEMPT := 1;
CONST CRAFT_CYCLE_SUCCESS := 2;
CONST CRAFT_CYCLE_FAILED  := 3;

var resource_config := ReadConfigFile(":crafting:resourceList");

var craft_skill;
var display_skill;
var craftL		:= 0;
var rcolor		:= 1;
var makermark := 1;

// Function used to target material and check ability for crafting.
// Returns an array of ItemRef for the item and Objtype for the
// resourceList of the material, and the Points bonus the resource
// gives for using it. If chk is passed, it is used for the "cprop"
// to check the item for. This is used for example, for boards that
// was originally colored logs. The colored log objtype is stored
// in the cprop, and is used to get it's "ResourceList" reference.
function MaterialSelection(who, craft_skill, type)
SendSysMessage(who, ""+type);
  if(resource_config.errortext)
    SendSysMessage(who, resource_config.errortext, 1, 33);
    return 0;
  endif
	SendSysMessage(who, "Select the "+Lower(type)+" you would like to work with.");
	var material := Target(who);
	if( !material )
    SendSysMessage(who, "Cancelled.", 3, 33);
		return 0;
	elseif( !(material in EnumerateItemsInContainer(who.backpack)) )
		SendSysMessage(who, "That is not in your backpack.");
		return 0;
	elseif( !Accessible(who, material) || (!material.movable) )
		SendSysMessage(who, "You cannot use that.");
		return 0;
	endif

	var materialobjtype := 0;
	if(CInt(GetObjProperty(material, type)) > 0)
		materialobjtype := GetObjProperty(material, type);
	else
		materialobjtype := CStr ( Hex ( material.objtype ) );
	endif
  if( !canUseMaterial(who, materialobjtype, craft_skill) )
		SendSysMessage(who, "You are not skilled enough to make anything out of that.");
		return 0;
	endif
	//var rtype := Lower(resource_config[materialobjtype].Type);
	var rtype := Lower(resource_config[materialobjtype].type);

  var materialchk := 0;
	if( rtype == type )
		materialchk := 1;
	endif
	if( materialchk == 0 )
		SendSysMessage(who, "That's not the proper material.");
		SendSysMessage(who, CStr(Hex(materialobjtype)) + " " + rtype + " " + type, 1, 88);
		return 0;
	endif
	if( ( material.objtype >= 0xF95) && (material.objtype <= 0xF9C) )
		var totalcloth := material.amount * 50;
		var cloth;
		if( DestroyItem(material) )
			cloth := CreateItemInBackpack(who, 0x1767, totalcloth);
			if( !cloth )
				cloth := CreateItemAtLocation(who.x, who.y, who.z, 0x1767, totalcloth);
			endif
		endif
		material := cloth;
	endif

	var retarray := array;
	retarray.Append(material);
	retarray.Append(materialobjtype);
	retarray.Append(resource_config[materialobjtype].Ptmod);
	return retarray;

endfunction

// Used to create the actual item, handle it's coloring and marking of
// exceptional naming of the item itself. Returns objref if success, 0 if fail.
// All asked for items in the function line are able to be sent no matter the skill
// the script is for with crafting. Use_onobjtype is there only for when you are
// working with let's say boards for example, where they will have a cprop contianing
// the objtype of the original logs they was made from. Else you simply pass
// use_on.objtype for that. This will keep the customizability open for adding in
// special resources for crafting here also.
function CreateCraftItem(who, craft_skill, use_on, use_onobjtype, objtype, amount, itemname, reqd)

	playCraftWork(who, craft_skill, CRAFT_CYCLE_SUCCESS);
	var theitem := CreateItemInBackpack(who, objtype, 1);
	var elem := FindConfigElem(resource_config, use_onobjtype);
	if( theitem )
		var nme := (resource_config[use_onobjtype].Name)+" "+itemname;
		if( Find(nme, "plain ", 1) )
			nme[Find(nme, "plain ", 1),6] := "";
		endif
		if( (rcolor == 1) && (craft_config[objtype].retain == 1) )
			if( resource_config[use_onobjtype].Color )
				theitem.color := resource_config[use_onobjtype].Color;
			endif
		endif
		SetObjProperty(theitem, "material", use_onobjtype);
		if( !SubtractAmount(use_on, amount) )
			SendSysMessage(who, "You don't have enough materials in your backpack!");
			DestroyItem(theitem);
			return -1;
		endif
		var myskill := CDbl(AP_GetSkill(who, craft_skill));
		var aidskill := CDbl(AP_GetSkill(who, craft_config[objtype].skillID));
		// This snags the Qualities of the resource to use on item creation.
		// Format in the cfg for Quality is <dmg mod> <ar mod> <hp mod>.
		// 0 for a mod means there isn't one. Space delimited in the cfg,
		// this allows us to simply use SplitWords on it to make the array
		// of each of the settings.
		var temp := GetConfigString(elem, "Quality");
		temp := SplitWords(temp);
		var power	:= GetConfigString(elem, "Power");
		if( (TypeOf(power) != "String") )
			power := 0;
		endif
		var wpower := GetConfigString(elem, "WPower");
		if( (TypeOf(wpower) != "String") )
			wpower := 0;
		endif
		if( (craft_config[objtype].Exceptional == 1) && (ExceptionalCraft(myskill, reqd, aidskill)) )
			CheckSkillAdvance(who, craft_skill, 1, 50);
			SetObjProperty(theitem, "Exceptional", 1);
			if( TypeOf(craft_config[objtype].SkillID) == "String" )
				CheckSkillAdvance(who, craft_config[objtype].SkillID, 1, 50);
			endif
			if((craft_config[objtype].Mark == 1))
				if((makermark == 1) && (myskill >= 100))
					nme := "an exceptional "+nme+" ["+
					menu_config[craft_skill].ExceptionalName+" "+who.name+"]";
				else
					nme := "an exceptional "+nme;
				endif
			else
				nme := "an exceptional "+nme;
			endif
			if(theitem.isA(POLCLASS_WEAPON))
				theitem.dmg_mod := CInt(temp[1])+(RandomInt(4));
			elseif(theitem.isA(POLCLASS_ARMOR))
				theitem.ar_mod := CInt(temp[2])+(RandomInt(4));
			endif
			theitem.maxhp_mod := CInt(temp[3])+(RandomInt(11));
			theitem.hp := theitem.maxhp;
			SendSysMessage(who, "You create an exceptional quality item.");
		else
			var rint := randomint(AP_GetSkill(who, craft_skill));
			if(reqd >= 23)
				reqd := reqd - 20;
			else
				reqd := 3;
			endif
			if(rint < reqd)
				SendSysMessage(who, "You were barely able to make this item. It's quality is below average.");
				if(theitem.isA(POLCLASS_WEAPON))
					theitem.dmg_mod := CInt(temp[1])-(RandomInt(2)+1);
				elseif(theitem.isA(POLCLASS_ARMOR))
					theitem.ar_mod := CInt(temp[2])-(RandomInt(2)+1);
				endif
				theitem.maxhp_mod := CInt(temp[3])-(RandomInt(10)+1);
				theitem.hp := theitem.maxhp;
			else
				SendSysMessage(who, "You create the item and put it in your backpack.");
				if(theitem.isA(POLCLASS_WEAPON))
					theitem.dmg_mod := CInt(temp[1]);
				elseif(theitem.isA(POLCLASS_ARMOR))
					theitem.ar_mod := CInt(temp[2]);
				endif
				theitem.maxhp_mod := CInt(temp[3]);
				theitem.hp := theitem.maxhp;
			endif
			var word := "";
			if((nme[1] == "a") || (nme[1] == "e") || (nme[1] == "i") || (nme[1] == "o") || (nme[1] == "u"))
				word := "an ";
			else
				word := "a ";
			endif
			nme := ""+word + nme;
			nme := nme - "plain ";
		endif
		SetName(theitem, nme);
		if( theitem.isA(POLCLASS_CONTAINER) )
			SetObjProperty(theitem, "trappable", 1);
		elseif( theitem.isA(POLCLASS_ITEM) )
			SetToolDurability(who, theitem, craft_skill);
		endif
		if( (power) && (theitem.IsA(POLCLASS_ARMOR)) )
			if ( TypeOf(power) == "String" || TypeOf(power) == "Integer" )
				SetObjProperty(theitem, "power", power);
		//		theitem.equipscript := "equiparmor";
		//		theitem.unequipscript := "unequiparmor";\
			endif
		elseif( (wpower == "SILVER") && theitem.isA(POLCLASS_WEAPON) )
			SetObjProperty(theitem, "SilverHit", 1);
		elseif( wpower && theitem.isA(POLCLASS_WEAPON) )
			if ( TypeOf(wpower) == "String" || TypeOf(wpower) == "Integer" )
				SetObjProperty(theitem, "wpower", wpower);
		//		theitem.equipscript := "equipweapon";
		//		theitem.unequipscript := "unequipweapon";
			endif
		endif
	else
		SendSysMessage(who, "You don't have enough room in your backpack!");
		return 0;
	endif

	return theitem;

endfunction

// This function is used to create items that are marked "assemble" in the
// crafting configs. Reads a prop from the cfg entry for a list of parts and
// their amounts then checks the player's backpack for the items and works it.
function CraftAssembly(who, craft_skill, tool, objtype, amt)

	var skill := craft_config[objtype].skill;
	var theitem;
	var mainSkill := CInt(AP_GetSkill(who, craft_skill));
	var modpts := 0;
	if( mainSkill >= skill )
		modpts := (mainSkill - (skill - 10));
	endif
	var diff := Cint(craft_config[objtype].difficulty);
	var pts	:= Cint(craft_config[objtype].points);
	pts := pts + modpts;
	var counter := 0;
	var chk := 0;

	// Get a list of the parts from the cfg entry. Format from config is listed in cfg
	// header. Objtypes first, then amount for each objtype, in the same respective order.
	// To match them together, we divid the length of the array by 2 then use that.
	// Example : mod := Len(array)/2;
	// Example2: objtype := array[x]; amt := array[x+mod];
	var parts_list := SplitWords(craft_config[objtype].parts);
	if( TypeOf(parts_list) != "Array" || !parts_list )
		SendSysMessage(who, "This item does not require parts to be made! Contact a GM.");
		return 0;
	endif
	var temp_list := dictionary;
	for i := 1 to (parts_list.Size()/2)
		temp_list[CInt(parts_list[i])] := CInt(parts_list[i+(parts_list.Size()/2)]);
	endfor
	parts_list := temp_list;

	var material_list := dictionary;
	var x := who.x;
	var y := who.y;

	while( counter < amt )
		if((who.x != x) || (who.y != y))
			break;
		elseif(chk == 1)
			break;
		endif

		chk := 0;

		foreach thing in ( EnumerateItemsInContainer(who.backpack) )
			// If thing.objtype is the same as the objtype in the part list array position....
			var thing_objtype := CInt(thing.objtype);
			if( thing_objtype in parts_list.Keys() )
				if( CInt(material_list[thing_objtype]) < CInt(parts_list[thing_objtype]) )
					material_list[thing_objtype] := CInt(material_list[thing_objtype]) + thing.amount;
				endif
			endif
		endforeach

		foreach part in ( parts_list.Keys() )
			if( material_list[part] < parts_list[part] )
				SendSysMessage(who, "You do not have the required materials to make that.");
				return 0;
			endif
		endforeach

		Sleep(1);

		if( CraftCheckSkill(who, craft_skill, diff, pts) )
			playCraftWork(who, craft_skill, CRAFT_CYCLE_SUCCESS);
			Set_Critical(1);
			foreach material_objtype in ( parts_list.Keys() )
				var amount := parts_list[material_objtype];
				if ( amount > 0 )
					if( !ConsumeSubstance(who.backpack, material_objtype, amount) )
						SendSysMessage(who, "Error - Unable to consume ["+amount+"] of material ["+material_objtype+"]");
						return 0;
					endif
				endif
			endforeach
			Set_Critical(0);
			theitem := CreateItemInBackpack(who, objtype, 1);
			if( !theitem )
				SendSysMessage(who, "You don't have enough room in your backpack!");
				return 0;
			endif
			CheckToolWear (who, tool, craft_skill);
			theitem.movable := 1;
			SendSysMessage(who, "You create the item and put it in your backpack.");
			if( theitem.isA(POLCLASS_CONTAINER) )
				SetObjProperty(theitem, "trappable", 1);
			endif
		else
			playCraftWork(who, craft_skill, CRAFT_CYCLE_FAILED);
			Set_Critical(1);
			foreach material_objtype in ( parts_list.Keys() )
				var amount := parts_list[material_objtype];
				if ( amount > 0 )
					if( !ConsumeSubstance(who.backpack, material_objtype, amount) )
						SendSysMessage(who, "Error - Unable to consume ["+amount+"] of material ["+material_objtype+"]");
						return 0;
					endif
				endif
			endforeach
			Set_Critical(0);
			SendSysMessage(who, "You failed to create the item, and some of your materials are lost.");
		endif
		counter := counter + 1;
	endwhile

	return 1;

endfunction


function CreateCraftedMap(who, craft_skill, use_on, objtype, amount, itemname, reqd)

	playCraftWork(who, craft_skill, CRAFT_CYCLE_SUCCESS);
	var theitem := CreateItemInBackpack(who, objtype, 1);
	if( theitem )
		if( !SubtractAmount(use_on, amount) )
			SendSysMessage(who, "You don't have enough materials in your backpack!");
			DestroyItem(theitem);
			return -1;
		endif
		var rint := randomint(AP_GetSkill(who, craft_skill));
		if( reqd >= 23 )
			reqd := reqd - 20;
		else
			reqd := 3;
		endif
		if( rint < reqd )
			SendSysMessage(who, "You were barely able to make this item.	Its quality is below average.");
		else
			SendSysMessage(who, "You create the item and put it in your backpack.");
		endif
		var word := "";
		if((itemname[1] == "a") || (itemname[1] == "e") || (itemname[1] == "i") || (itemname[1] == "o") || (itemname[1] == "u"))
			word := "an ";
		else
			word := "a ";
		endif
		itemname := ""+word + itemname;
		itemname := itemname - "plain ";
		SetName(theitem, itemname);
	else
		SendSysMessage(who, "You don't have enough room in your backpack!");
		return 0;
	endif

	return theitem;

endfunction

// Used to check the skill of the character vs the
// the skill of the resource_config (resources config) to
// see if they can work with the material.
function canUseMaterial(who, use_on, craft_skill)

	if( AP_GetSkill(who, craft_skill) >= resource_config[use_on].Skill )
		return 1;
	else
		return 0;
	endif

endfunction

// Used to check if near the required objects to craft the item.
// Can be used for anvils, forges, flour mills, or whatever.
// Returns:
//			-1 = Not required for this craftable.
//			 1 = Success. Near one of the object types!
//			 0 = Fail. Not near any of the required objtypes.
function isNearRequiredObject(who, objtype)

	var ObjectList := SplitWords(craft_config[objtype].craftnear);
	if( TypeOf(ObjectList) != "Array" || !ObjectList || ObjectList.errortext )
		return -1; // -1 = not required.
	endif
	foreach item in ( ListItemsNearLocation(who.x, who.y, who.z, 3) )
		if( (Hex(item.objtype) in ObjectList) )
			return 1;
			break;
		endif
	endforeach
	return 0;
endfunction

// Used to send message about not being near a custom required object.
function RequiredObjectMissing(who, objtype)

	var ObjectList := SplitWords(craft_config[objtype].craftnear);
	if( TypeOf(ObjectList) != "Array" || !ObjectList || ObjectList.errortext )
		return -1; // -1 = not required. Should never happen unless you misuse this function.
	endif
	var ObjectName := GetObjTypeDesc(CInt(ObjectList[1]));
	SendSysMessage(who, "You must be near a "+ObjectName+" in order to create this.");
	return 1;
endfunction

// This function is used for those who have items from objtype 0 to 16383 the same
// as the items in the tiledata. It will convert the objtype to the correct Cliloc
// ID for it's name. Can be used in menu scripts such as crafting, etc. Just remember
// to pass the objtype as CInt(objtype) so it converts the hex to Integer.
// NOTE: Clilocs was implemented in uh, client 3.x+ I believe.
// 1020000 - First tiledata name entry
// 1036383 - Last tiledata name entry
// **** OLD AND OBSOLETE. NO LONGER USING CLILOCS IN THIS CRAFTING. LEFT
// **** IN FOR REFERENCE AND KNOWLEDGE
function ClilocObjtype(objtype)
	return -1;

	if( (objtype >= 0) && (objtype <= 16383) && objtype != error )
		return CInt(objtype)+1020000;
	else
		return -1;
	endif

endfunction


// This function is used to display a craft skill's action,
// sound, the loops to do it in, and the delay between loops.
// All delays are in seconds.
function playCraftWork(who, craft_skill, sound_type)

	var craftSound;
	case (sound_type)
		1: craftSound := menu_config[craft_skill].CraftSound1;
		2: craftSound := menu_config[craft_skill].CraftSound2;
		3: craftSound := menu_config[craft_skill].CraftSound3;
		default: craftSound := menu_config[craft_skill].CraftSound1;
	endcase

	var loops	:= menu_config[craft_skill].CraftLoops;
	var delay	:= menu_config[craft_skill].CraftDelay;
	var craftAction := menu_config[craft_skill].CraftAnim;

	if( loops )
		for i := 1 to loops
			Sleep(delay);
			if( craftAction )
				PerformAction( who, craftAction );
			endif
			if( craftSound )
				PlaySoundEffect(who, craftSound);
			endif
		endfor
	else
		Sleep(delay);
		if( craftAction )
			PerformAction( who, craftAction );
		endif
		if( craftSound )
			PlaySoundEffect(who, craftSound);
		endif
		Sleep(delay);
	endif

	return 0;
endfunction
