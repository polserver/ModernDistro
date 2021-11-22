use uo;
use os;
use cfgfile;
use util;

include "include/fields";
include ":attributes:attributes";
include "include/client";
include "include/objtype";
include ":magery:spellAttack";

var caster;
var npccfg := ReadConfigFile("::npcdesc");
const EVID_DISCHARGE_DUP := 0x123457;

function maincast(parms, delivery, circle, noto, id);
  var cast_on;
  var spellscroll := 0;
  var scroll;
  var tgtopt := TGTOPT_CHECK_LOS;
  if(parms[1] == "#MOB")
    caster := parms[2];
    if(mobspellcast(caster, circle, spellscroll) == 0)
      return 0;
    endif
    cast_on := parms[3];
  elseif (parms[1] == "#scroll")
    spellscroll := 1;
    caster := parms[2];
    EraseObjProperty(caster, "IsMeditating");
    EraseObjProperty(caster, "HealTimer");
    if(spellcast(caster, circle, spellscroll, id) == 0)
      return 0;
    endif
    scroll := parms[3];
    if(!ReserveItem(scroll))
      SendSysMessage(caster,"You lost your concentration.", 3, 40);
      return 0;
    endif
    if(delivery == "notarget")
      cast_on := caster;
      if(!can_cast(caster))
        cast_on := 0;
      endif
    elseif(delivery == "direct")
      if(noto == "helpful")
        cast_on := CanTargetSpell(caster, circle, TGTOPT_HELPFUL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      elseif(noto == "neutral")
        cast_on := CanTargetSpell(caster, circle, TGTOPT_NEUTRAL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      else
        cast_on := CanTargetSpell(caster, circle, TGTOPT_HARMFUL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      endif
      if(!cast_on)
        return 0;
      endif
    elseif(delivery == "indirect")
      cast_on := CanTargetArea(caster, circle);
      if(!can_cast(caster))
        cast_on := 0;
      endif
      if (!cast_on)
        return 0;
      endif
    else
      return 0;
    endif
  else
    caster := parms;
    EraseObjProperty(caster, "IsMeditating");
    EraseObjProperty(caster, "HealTimer");
    if (spellcast(caster, circle, spellscroll, id) == 0)
      return 0;
    endif
/*    if (!ConsumeReagents(caster, id))
      SendSysMessage(caster, "You do not have the proper reagents.");
      return 0;
    endif
*/
    if(delivery == "notarget")
      cast_on := caster;
      if(!can_cast(caster))
        cast_on := 0;
      endif
    elseif(delivery == "direct")
      if(noto == "helpful")
        cast_on := CanTargetSpell(caster, circle, TGTOPT_HELPFUL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      elseif(noto == "neutral")
        cast_on := CanTargetSpell(caster, circle, TGTOPT_NEUTRAL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      else
        cast_on := CanTargetSpell(caster, circle, TGTOPT_HARMFUL | tgtopt);
        if(!can_cast(caster))
          cast_on := 0;
        endif
      endif
      if (!cast_on)
        return 0;
      endif
    elseif(delivery == "indirect")
      cast_on := CanTargetArea(caster, circle);
      if(!can_cast(caster))
        cast_on := 0;
      endif
      if (!cast_on)
        return 0;
      endif
    else
      return 0;
    endif
  endif
  // Removed to test new POL 96 magic system
/*  if(newusemana(caster, circle, cast_on, delivery) == 0)
    PlaySoundEffect(caster,0x005d);
    PlayObjectCenteredEffect(caster, 0x3735,0x0a, 0x1e);
    return 0;
  endif */
  if(spellscroll == 1)
    if(!SubtractAmount(scroll, 1))
      return 0;
    endif
  endif
  var points := getpoints(circle, spellscroll, SKILLID_MAGERY);
  AwardRawSkillPoints(caster, SKILLID_MAGERY, points);
  var newparms := {};
  newparms[1] := caster;
  newparms[2] := cast_on;
  return newparms;
endfunction

function can_cast(who)
  var hand1 := (GetEquipmentByLayer(who, 1));
  var hand2 := (GetEquipmentByLayer(who, 2));
  if(hand1)
    if(hand1.graphic != 0x0efa)
      SendSysMessage(who,"You can't cast spells with things in your hands.", 3, 40);
      return 0;
    endif
  endif
  if(hand2)
    if(hand2.graphic != 0x0efa)
      SendSysMessage(who,"You can't cast spells with things in your hands.", 3, 40);
      return 0;
    endif
  endif
  return 1;
endfunction

function CanTargetSpell(caster, circle, targopt := (TGTOPT_NEUTRAL | TGTOPT_CHECK_LOS) )
  circle := circle; // stops ecompile "not used" warning
  if(!can_cast(caster))
    return 0;
  endif
  var cast_on := Target(caster, targopt);
  if(!cast_on)
    return 0;
  endif
  if(cast_on.hidden)
    SendSysMessage(caster,"You can't see that.", 3, 40);
    return 0;
  endif
  if(Distance(caster, cast_on) >= 12)
    SendSysMessage(caster, "That is too far away.", 3, 40);
    return 0;
  endif
  return cast_on;
endfunction

function CanTargetArea(caster, circle)
  circle := circle; // stops ecompile "not used" warning
  if (!can_cast(caster))
    return 0;
  endif
  var cast_loc := TargetCoordinates(caster);
  if (!cast_loc.x)
    return 0;
  endif
  if (!CheckLosAt(caster, cast_loc.x, cast_loc.y, cast_loc.z))
    SendSysMessage(caster, "You can't see that.", 3, 40);
    return 0;
  endif
  if(CoordinateDistance(caster.x, caster.y, cast_loc.x, cast_loc.y) >= 12)
    SendSysMessage(caster, "That is too far away.", 3, 40);
    return 0;
  endif
  return cast_loc;
endfunction

function Reflected(cast_on)
  var mr := CInt(GetObjProperty(cast_on, "mr"));
  if(mr >= 1)
    PlaySoundEffect(cast_on, SFX_SPELL_MAGIC_REFLECTION);
    PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 10,10);
    SendSysMessage(cast_on, "Your magic reflect spell saved you!");
    if(mr == 1)
      EraseObjProperty(cast_on, "mr");
    elseif(mr == 2)
      var holder, pid;
      var eqp := GetObjProperty(cast_on, "EquipTimers");
      foreach thing in ListEquippedItems(cast_on)
        foreach entry in eqp
          if((thing.serial == entry[1]) and (entry[2] == "ref"))
            holder := GetObjProperty(thing, "pid");
            break;
          endif
        endforeach
      endforeach
      pid := getprocess(holder);
      if(pid)
        var k := struct;
        k.+type := EVID_DISCHARGE_DUP;
        pid.sendevent(k);
      else
        EraseObjProperty(cast_on, "mr");
      endif
    endif
    return 1;
  endif
  return 0;
endfunction

function Resisted(circle, caster, cast_on, amt)
	// Purpose: to check for resistance to spells cast upon a character
	// called by: most spells
	// returns: amount of damage not resisted?

        var magery := CInt(AP_GetSkill(caster, MAGERY));
        var evalint := CInt(AP_GetSkill(caster, EVALUATING_INTELLIGENCE));
        var resist := CInt(AP_GetSkill(cast_on, RESISTING_SPELLS));

        // chk1 relates to the resist ability.
        var chk1 := (resist / 5);
        // chk2 relates to the difficulty level
        var chk2 := resist - (((magery - 20) / 5) + (circle * 5));

        if(chk1 < 1)
                chk1 := 1;
        endif
        if(chk2 < 1)
                chk2 := 1;
        endif

        // What an odd construct.
        // If you're hit with an easy spell then you have a difficulty of your resist
        var diff := 0;
        if(chk1 > chk2)
                diff := chk1;
        else
                diff := chk2;
        endif

        var points := getresistpoints(circle, cast_on, SKILLID_RESISTING_SPELLS);
        if(cast_on.dead)
                return 0;
        endif
        if(SkillCheck(cast_on, RESISTING_SPELLS, diff, points) > 0)
                SendSysMessage(cast_on, "You feel yourself resisting magical energy!");
                PlaySoundEffect(cast_on, SFX_SPELL_WEAKEN);
                PlayObjectCenteredEffect(cast_on, FX_CURSE_EFFECT, 5,5);
                amt := (amt * 0.5);
                if(amt < 1)
                        amt := 1;
                endif
        endif
        var modamt := 1;
        if(resist > evalint)
                modamt := (1 + ((evalint - resist) / 200.0));
        elseif(evalint > resist)
                modamt := (1 + ((evalint - resist) / 500.0));
        endif
        amt := (amt * modamt);
        return CInt(amt);
endfunction


function CalcSpellDamage(circle, caster, cast_on)
  cast_on := cast_on;
  var die_string := circle+"d8";
  var dmg := RandomDiceRoll(die_string);
  dmg := dmg + CInt(AP_GetSkill(caster, MAGERY)/10);
  if(dmg > (circle * 8))
    dmg := circle * 8;
  endif
  return dmg;
endfunction

function CanTargetThing(caster, circle, targopt := TGTOPT_CHECK_LOS)
  if (!can_cast(caster))
    return 0;
  endif
  var cast_on := Target(caster, targopt);
  if(!cast_on)
    return;
  endif
  return cast_on;
endfunction

function spellcast(me, circle, scroll, id)
  var spellcfg := ReadConfigFile(":spells:spells");
  var val := SplitWords(spellcfg[id].val);
  var times := CInt(val[1]);
  var slp   := CInt(val[2]);
  var mhp := AP_GetVital(me, VITALID_LIFE);
  var umana, diff;
  var n := 1;
  if(scroll == 0)
    case (circle)
      1:   umana :=   4;     diff :=  10;
      2:   umana :=   6;     diff :=  20;
      3:   umana :=   9;     diff :=  30;
      4:   umana :=  11;     diff :=  50;
      5:   umana :=  14;     diff :=  60;
      6:   umana :=  20;     diff :=  70;
      7:   umana :=  40;     diff :=  85;
      8:   umana :=  50;     diff :=  95;
      9:   umana :=  75;     diff := 100;
     10:   umana := 100;     diff := 100;
    endcase
  elseif(scroll == 1)
    case (circle)
      1:   umana :=   4;     diff := 10;
      2:   umana :=   6;     diff := 10;
      3:   umana :=   9;     diff := 10;
      4:   umana :=  11;     diff := 20;
      5:   umana :=  14;     diff := 30;
      6:   umana :=  20;     diff := 50;
      7:   umana :=  40;     diff := 60;
      8:   umana :=  50;     diff := 70;
      9:   umana :=  75;     diff := 85;
     10:   umana := 100;     diff := 95;
    endcase
  else
    return 0;
  endif
  if((mhp > AP_GetVital(me, VITALID_LIFE)) && ((RandomInt(99)+1) > (AP_GetSkill(me, WRESTLING)/2)))
    SendSysMessage(me,"You lost your concentration.", 3, 40);
    return 0;
  endif
  if (umana > AP_GetVital(me, VITALID_MANA))
    me.frozen :=1;
    PerformAction(me,ANIM_CAST_DIR);
    sleepms(1200);
    me.frozen :=0;
    SendSysMessage(me,"You do not have enough mana to cast that.", 3, 40);
    PlaySoundEffect(me,0x005d);
    PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
    return 0;
  endif
  if(can_cast(me))
    me.frozen :=1;
    for (n:=1; n<=times; n:=n+1)
      if((mhp > AP_GetVital(me, VITALID_LIFE)) && ((RandomInt(99)+1) > (AP_GetSkill(me, WRESTLING)/2)))
        SendSysMessage(me,"You lost your concentration.", 3, 40);
        PlaySoundEffect(me,0x005d);
        PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
        me.frozen :=0;
        return 0;
      endif
      PerformAction(me,ANIM_CAST_DIR);
      sleepms(900);
    endfor
    if(slp)
      sleepms(slp * 100);
    endif
    me.frozen :=0;
    if (SkillCheck(me,MAGERY, diff, 0) > 0)
      if((mhp > AP_GetVital(me, VITALID_LIFE)) && ((RandomInt(99)+1) > (AP_GetSkill(me, WRESTLING)/2)))
        SendSysMessage(me,"You lost your concentration.", 3, 40);
        return 0;
      endif
      return 1;
    else
      SendSysMessage(me,"Your spell fizzles.", 3, 40);
      PlaySoundEffect(me,0x005d);
      PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
      return 0;
    endif
  endif
endfunction

function newusemana(who, circle, cast_on, delivery)
  if(delivery == "direct")
    if(!CheckLineOfSight(who, cast_on))
      SendSysMessage(who, "You can't see that.", 3, 40);
      return 0;
    endif
  elseif(delivery == "indirect")
    if(!CheckLosAt(who, cast_on.x, cast_on.y, cast_on.z))
      SendSysMessage(who, "You can't see that.", 3, 40);
      return 0;
    endif
  endif
  if(cDistance(who.x, who.y, cast_on.x, cast_on.y) > 12)
    var check := 0;
    foreach thing in EnumerateItemsInContainer(who.backpack)
      if(thing == cast_on)
        check := 1;
        break;
      endif
    endforeach
    if(!check)
      SendSysMessage(who,"The target is out of range.", 3, 40);
      return 0;
    endif
  endif
  var umana;
    case (circle)
    1: umana :=  4;
    2: umana :=  6;
    3: umana :=  9;
    4: umana := 11;
    5: umana := 14;
    6: umana := 20;
    7: umana := 40;
    8: umana := 50;
  endcase
  var mana := AP_GetVital(who, VITALID_MANA);
  if(mana >= umana)
    AP_SetVital(who, VITALID_MANA, Cint(mana - umana));
    return 1;
  else
    SendSysMessage(who,"You do not have enough mana to cast that.", 3, 40);
    return 0;
  endif
endfunction

function mobspellcast(me, circle, scroll)
  scroll := scroll; // stops ecompile "not used" warning
  var diff, points, umana;
  case(circle)
    1:   diff := 10;     points :=  20;    umana :=   4;
    2:   diff := 20;     points :=  40;    umana :=   6;
    3:   diff := 30;     points :=  80;    umana :=   9;
    4:   diff := 50;     points := 100;    umana :=  11;
    5:   diff := 60;     points := 130;    umana :=  14;
    6:   diff := 70;     points := 160;    umana :=  20;
    7:   diff := 85;     points := 190;    umana :=  40;
    8:   diff := 95;     points := 210;    umana :=  50;
  endcase
  if(umana > AP_GetVital(me, VITALID_MANA))
    return 0;
  endif
  if(can_cast(me))
    if(SkillCheck(me, MAGERY, (diff - 10), points) > 0)
      return 1;
    else
      return 0;
    endif
  endif
endfunction

function getpoints(c, scroll, skillid)
  var points;
  var diff;
  if(scroll == 1)
    case(c)
      1:   points :=  10; diff :=  0;
      2:   points :=  20; diff := 10;
      3:   points :=  20; diff := 10;
      4:   points :=  40; diff := 20;
      5:   points :=  60; diff := 30;
      6:   points := 100; diff := 50;
      7:   points := 120; diff := 60;
      8:   points := 140; diff := 70;
    endcase
  else
    case(c)
      1:   points :=  20; diff := 20;
      2:   points :=  40; diff := 30;
      3:   points :=  80; diff := 40;
      4:   points := 100; diff := 50;
      5:   points := 120; diff := 60;
      6:   points := 140; diff := 70;
      7:   points := 160; diff := 86;
      8:   points := 180; diff := 100;
    endcase
  endif
  var skill := AP_GetSkill(caster, AP_SkillIDToAttributeName(skillid));
  if(skill > (diff - 20))
    if((skill - diff) <= 5)
      points := points;
    elseif((skill - diff) <= 10)
      points := (points * 3) / 4;
    elseif((skill - diff) <= 15)
      points := points / 2;
    elseif((skill - diff) <= 15)
      points := points / 4;
    else
      points := 0;
    endif
  endif
  return points;
endfunction

function getresistpoints(c, victim, skillid)
  var points;
  var diff;
  case(c)
    1:   points :=  20; diff := 20;
    2:   points :=  40; diff := 30;
    3:   points :=  80; diff := 40;
    4:   points := 100; diff := 50;
    5:   points := 120; diff := 60;
    6:   points := 140; diff := 70;
    7:   points := 160; diff := 86;
    8:   points := 180; diff := 100;
  endcase
  var skill := AP_GetSkill(victim, AP_SkillIDToAttributeName(skillid));
  if(skill > (diff - 20))
    if((skill - diff) <= 5)
      points := points;
    elseif((skill - diff) <= 10)
      points := (points * 3) / 4;
    elseif((skill - diff) <= 15)
      points := points / 2;
    elseif((skill - diff) <= 15)
      points := points / 4;
    else
      points := 0;
    endif
  endif
  return points;
endfunction

function cDistance(x1, y1, x2, y2)
  var xd := x1 - x2;
  var yd := y1 - y2;
  if (xd < 0)
    xd := -xd;
  endif
  if (yd < 0)
    yd := -yd;
  endif
  if (xd > yd)
    return xd;
  else
    return yd;
  endif
endfunction

function usemana(who, circle, cast_on)
  if(cast_on.mobile)
    if(!CheckLineOfSight(who, cast_on))
      SendSysMessage(who, "You can't see that.", 3, 40);
      return 0;
    endif
  else
    if(!CheckLosAt(who, cast_on.x, cast_on.y, cast_on.z))
      SendSysMessage(who, "You can't see that.", 3, 40);
      return 0;
    endif
  endif
  if(cDistance(who.x, who.y, cast_on.x, cast_on.y) > 12)
    SendSysMessage(who,"The target is out of range.", 3, 40);
    return 0;
  endif
  var umana;
  case (circle)
    1: umana :=   4;
    2: umana :=   6;
    3: umana :=   9;
    4: umana :=  11;
    5: umana :=  14;
    6: umana :=  20;
    7: umana :=  40;
    8: umana :=  50;
    9: umana :=  75;
   10: umana := 100;
  endcase
  var mana := AP_GetVital(who, VITALID_MANA);
  if(mana >= umana)
    var ms := AP_SetVital(who, VITALID_MANA, Cint(mana - umana));
    return 1;
  else
    SendSysMessage(who,"You do not have enough mana to cast that.", 3, 40);
    return 0;
  endif
endfunction