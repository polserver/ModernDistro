include "include/skillLists";

var cost;

function MerchantTrain(me, who, text)

  // If player's skill exceeds this number, the NPC won't train it
  var maxskilltotrain := 10;

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
      say("I can train thee in the following skills:");
      say(trainable);
    else
      say("I can not train thee.");
    endif
  else
    skill := lower(skill);
    foreach thing in getskilllist(me)
      if(lower(thing) == skill)
        var holder := FindSkillId(thing);

// Since so many npcdesc.cfg entries have NPC's with 100 skill, set it lower
// for the purpose of training.

        var npclevel := AP_GetSkill(me, AP_SkillIDToAttributeName(holder));

        if(npclevel == 0)
          npclevel := RandomInt(12) + 22;
          SetBaseSkill(me, holder, npclevel);
          var npclevel := AP_GetSkill(me, AP_SkillIDToAttributeName(holder));
          npclevel := npclevel;

        endif
        if(npclevel > maxskilltotrain)
          npclevel := maxskilltotrain;
        endif
        var pclevel := GetBaseSkill(who, holder);
        if((pclevel >= npclevel) || (pclevel > maxskilltotrain))
          PrintTextAbovePrivate(me, "I cannot teach you anymore.", who);
          return;
        else
          var trainmaxcost := GoldForSkillGain(npclevel, pclevel);
          Print("trainmaxcost: " + trainmaxcost);
          PrintTextAbovePrivate(me, "I will train thee " + thing + " for " + trainmaxcost + " gold.", who);
          SetObjProperty(who, "TrainMaxCost", trainmaxcost);
          SetObjProperty(who, "TrainSkillID", holder);
          return;
        endif
      endif
    endforeach
    PrintTextAbovePrivate(me, "I know not of that skill.", who);
  endif
endfunction

function TrainSkill(me, who, item)
  if((item.objtype == 0x1da0) and ((me.npctemplate == "townguard") or (me.npctemplate == "orderguard") or (me.npctemplate == "chaosguard")))
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
  var variance := 0;
  var npc_level := 0;
  var pc_level := 0;
  var goldreceived := 0;
  var new_level := 0;
  var currentnpccost := 0;
  if((item.objtype == 0xeed) && (item.amount != 0))
    var skillid := GetObjProperty(who, "TrainSkillID");
    skillid := skillid;
    var trainmaxcost := GetObjProperty(who, "TrainMaxCost");
    var attributeid := AP_SkillIdToAttributeName(GetObjProperty(who, "TrainSkillID"));
    if((!trainmaxcost || !attributeid) && attributeid != 0)
      say("I don't know what this is for, but thanks!");
      DestroyItem(item);
      return;
    endif
    npc_level    := GetAttribute(me, attributeid);
    pc_level     := GetBaseSkill(who, attributeid);
    pc_level     := GetAttributeBaseValue(who, attributeid) / 10;
    goldreceived := item.amount;
    variance     := npc_level - pc_level;
    currentnpccost := GoldForSkillGain(npc_level, pc_level);
    if(pc_level < npc_level)
      if(currentnpccost != trainmaxcost)
        if(currentnpccost > trainmaxcost)
          npc_level := SkillGainForGold(pc_level, trainmaxcost);
          variance  := npc_level - pc_level;
        else
          if(goldreceived > currentnpccost)
            say("I can not train thee to that level.");
            MoveItemToContainer(who.backpack, item);
            return;
          else
            trainmaxcost := currentnpccost;
          endif
        endif
      endif
      if(trainmaxcost <= goldreceived)
        new_level := (npc_level * 10);
        EraseObjProperty( who, "TrainMaxCost" );
        EraseObjProperty( who, "TrainSkillID" );
      else
        new_level := ((pc_level + CInt((CDbl(variance) * (CDbl(goldreceived) / CDbl(trainmaxcost))))) * 10);
        SetObjProperty( who, "TrainMaxCost", trainmaxcost-goldreceived );
      endif

      PrintTextAbovePrivate(me, "Let me show you how it's done.", who);
      var pid := getprocess(GetObjProperty(who, "cap_pid"));
      var k := struct;
      k.+skid   := attributeid;
      k.+type   := EVID_SKILLCAP;
      k.+base   := new_level;
      pid.sendevent(k);
      DoRecalcVitals(who);
      DestroyItem(item);
      SendSysMessage(who, "Your skill increases.");
    else
      say("I'm not as good as thee.  Please find another to train thee.");
      MoveItemToContainer(who.backpack, item);
    endif
  else
    say("Bah, I have no use for this. Be off with ye.");
    MoveItemToContainer(item, who.backpack);
  endif
endfunction

function GoldForSkillGain(npc_skill, pc_skill)
  return Cint((npc_skill - pc_skill) * 10);
endfunction

function SkillGainForGold(pc_level, goldreceived)
  return (goldreceived / 10) + pc_level;
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