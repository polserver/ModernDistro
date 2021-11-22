use uo;
use os;
use cfgfile;
use util;
use math;

include "include/client";
include "include/objtype";
include "include/spellAttack";

var caster;
var npccfg := ReadConfigFile(":*:npcdesc");
var weaponcfg := readconfigfile(":combat:itemdesc");
const UOBJ_NEGATIONFIELD := 0x2aa0a;


function maincast(parms, delivery, circle, noto);


	var cast_on;
	var spellscroll := 0;
	var scroll;
	var tgtopt;
	if (parms[1] == "#MOB")
		caster := parms[2];
		if (caster.squelched)
			return 0;
		endif
		if ( (GetObjProperty(caster, "Devotion") == "Fortune") and (circle > 5) )
			SendSYsMessage(caster, "Your faith prevents this.");
			return 0;
		endif
		if (mobspellcast(caster, circle, spellscroll) == 0)
			return 0;
		endif
		cast_on := parms[3];
	elseif (parms[1] == "#scroll")
		spellscroll := 1;
		caster := parms[2];
		if ( (GetObjProperty(caster, "Devotion") == "Fortune") and (circle > 5) )
			SendSYsMessage(caster, "Your faith prevents this.");
			return 0;
		endif
		if (caster.squelched)
			SendSysMessage(caster, "You are unable to speak!");
			return 0;
		endif
		scroll := parms[3];
		if ( !ReserveItem(scroll) )
			SendSysMessage(caster,"You have lost your concentration.");
			return 0;
		endif

		if ( !SubtractAmount(scroll, 1) )
			SendSysMessage(caster,"You have lost your concentration.");
			return 0;
		endif

		if ( spellcast(caster, circle, spellscroll) == 0)
			return 0;
		endif

		if (delivery == "notarget")
			cast_on := caster;
			if ( !can_cast(caster, circle) )
				cast_on := 0;
			endif
		elseif(delivery == "direct")
			if (noto == "helpful")
				cast_on := CanTargetSpell(caster, circle, TGTOPT_HELPFUL + TGTOPT_CHECK_LOS);
			elseif (noto == "neutral")
				cast_on := CanTargetSpell(caster, circle, TGTOPT_CHECK_LOS);
			else
				cast_on := CanTargetSpell(caster, circle, TGTOPT_HARMFUL + TGTOPT_CHECK_LOS);
			endif

			if (!cast_on)
				return 0;
			endif

		elseif(delivery == "indirect")
			cast_on := CanTargetArea(caster, circle);
			if (!cast_on)
				return 0;
			endif
		else
			return 0;
		endif
	else
		caster := parms;
		if ( (GetObjProperty(caster, "Devotion") == "Fortune") and (circle > 5) )
			SendSYsMessage(caster, "Your faith prevents this.");
			return 0;
		endif
		if (caster.squelched)
			SendSysMessage(caster, "You are unable to speak!");
			return 0;
		endif
		if( spellcast(caster, circle, spellscroll) == 0)
			return 0;
		endif
		if(delivery == "notarget")
			cast_on := caster;
		elseif(delivery == "direct")
			if(noto == "helpful")
				cast_on := CanTargetSpell(caster, circle, TGTOPT_HELPFUL + TGTOPT_CHECK_LOS);
			elseif(noto == "neutral")
				cast_on := CanTargetSpell(caster, circle, TGTOPT_CHECK_LOS);
			else
				cast_on := CanTargetSpell(caster, circle, TGTOPT_HARMFUL + TGTOPT_CHECK_LOS);
			endif

			if (!cast_on)
				return 0;
			endif

		elseif(delivery == "indirect")
			cast_on := CanTargetArea(caster, circle);

			if (!cast_on)
				return 0;
			endif
		else
			return 0;
		endif
	endif

	if (newusemana(caster, circle, cast_on, delivery) == 0)
		PlaySoundEffect(caster,0x005d);
		PlayObjectCenteredEffect(caster, 0x3735,0x0a, 0x1e);
		return 0;
	endif

	if (!can_cast(caster, circle) )
		return 0;
	endif

	var points := getpoints(circle, spellscroll);
	AwardRawAttributePoints( caster, MAGERY, points);
	var newparms := {};
	newparms[1] := caster;
	newparms[2] := cast_on;
	return newparms;

endfunction


function can_cast(me, circle)

	if (GetObjProperty(me, "CastWithArmor"))
		return 1;
	endif

	var castermed := GetAttribute(me, "meditation") + 1;
	circle := circle - ( CInt(castermed / 24) );
	if (circle < 1)
		circle := 1;
	endif

	foreach item in ListEquippedItems(me)
		if ( (weaponcfg[item.objtype].blockcircle) && (weaponcfg[item.objtype].blockcircle <= circle) && !weaponcfg[item.objtype].errortext)
			if (!GetObjProperty(item, "IgnoreBlockcircle") )
				SendSysmessage(me, "Your equipment prevents you from casting this spell!");
				return 0;
			endif
		endif
	endforeach

	foreach negation in ListItemsNearLocationOfType(me.x, me.y, me.z, 30, UOBJ_NEGATIONFIELD, me.realm);
		if (Distance(me, negation) <= (Cint(GetObjProperty(negation, "Range") ) ) )
			PrintTextAbove(me, "Something intereferes with this spell.");
			PlaySoundEffect(me,0x005d);
			PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
			return 0;
		endif
	endforeach

	return 1;

endfunction


function CanTargetSpell(caster, circle, TargOpt := TGTOPT_CHECK_LOS)

	if ( !can_cast(caster, circle) )
		return 0;
	endif

	var cast_on := Target( caster, TargOpt );
	return cast_on;

endfunction


function CanTargetArea(caster, circle)

	if ( !can_cast(caster, circle) )
		return 0;
	endif
	var cast_loc := TargetCoordinates( caster );
	if (!cast_loc.x)
		return 0;
	endif
	if (!CheckLosAt( caster, cast_loc.x, cast_loc.y, cast_loc.z ))
		SendSysMessage( caster, "You cannot see that." );
		return 0;
	endif
	return cast_loc;

endfunction


function Reflected(cast_on)

	if (GetObjProperty(cast_on, "mr") ==  "1")
		sleepms(500);
		PlaySoundEffect(cast_on, SFX_SPELL_MAGIC_REFLECTION);
		PlayObjectCenteredEffect( cast_on, FX_CURSE_EFFECT, 10,10);
		SendSysMessage(cast_on, "You have reflected the spell!");
		EraseObjProperty(cast_on, "mr");
		return 1;
	else
		return 0;
	endif

endfunction


function Resisted(circle, caster, cast_on, amt)

	var sorcery_skill := Cint(AP_GetSkill(caster, MAGERY));
	if(cast_on.dead)
		return 0;
	endif

	if (SkillCheck( cast_on, RESISTING_SPELLS, (circle * 10), (circle * 30) ) > 0)
		SendSysMessage(cast_on, "You feel yourself resisting magical energy!");
		PlaySoundEffect(cast_on, SFX_SPELL_WEAKEN);
		PlayObjectCenteredEffect( cast_on, FX_CURSE_EFFECT, 5,5);
		return CInt(amt / 2);
	else
		return amt;
	endif

endfunction


function CalcSpellDamage(circle, caster, cast_on)
	var die_string := CStr(circle) + "d8";
	var dmg := RandomDiceRoll( die_string ) ;
	dmg := dmg + Cint( AP_GetSkill(caster, MAGERY)/10);
	if ( dmg > (circle * 8) )
		dmg := circle * 8;
	endif
	return dmg;

endfunction


function CanTargetThing(caster, circle, TargOpt := TGTOPT_CHECK_LOS)

	if (!can_cast(caster, circle))
		return 0;
	endif

	var cast_on := Target( caster, TargOpt );
	return cast_on;

endfunction


function spellcast( me, circle, scroll)

	var mhp := AP_GetVital(me, HITS);
	var val;
	var delay;
	var umana;
	var diff;
	var points;
	var n := 1;
	if(scroll == 0)
		case (circle)
			1:   val :=  1;     delay := 250;     umana :=   4;     diff :=  10;
			2:   val :=  1;     delay := 500;     umana :=   6;     diff :=  20;
			3:   val :=  2;     delay := 250;     umana :=   9;     diff :=  30;
			4:   val :=  2;     delay := 750;     umana :=  11;     diff :=  50;
			5:   val :=  3;     delay := 250;     umana :=  14;     diff :=  60;
			6:   val :=  3;     delay := 750;     umana :=  20;     diff :=  70;
			7:   val :=  5;     delay :=  10;     umana :=  40;     diff :=  85;
			8:   val :=  8;     delay :=  10;     umana :=  50;     diff :=  95;
			9:   val :=  9;     delay :=  10;     umana :=  75;     diff := 100;
			10:   val := 10;     delay :=  10;     umana := 100;     diff := 100;
		endcase
	elseif(scroll == 1)
		case (circle)
			1:   val :=  1;     delay := 250;     umana :=   4;     diff := 10;
			2:   val :=  1;     delay := 500;     umana :=   6;     diff := 10;
			3:   val :=  2;     delay := 250;     umana :=   9;     diff := 10;
			4:   val :=  2;     delay := 750;     umana :=  11;     diff := 20;
			5:   val :=  3;     delay := 250;     umana :=  14;     diff := 30;
			6:   val :=  3;     delay := 750;     umana :=  20;     diff := 50;
			7:   val :=  5;     delay :=  10;     umana :=  40;     diff := 60;
			8:   val :=  8;     delay :=  10;     umana :=  50;     diff := 70;
			9:   val :=  9;     delay :=  10;     umana :=  75;     diff := 85;
			10:   val := 10;     delay :=  10;     umana := 100;     diff := 95;
		endcase
	else
		return 0;
	endif

	if(mhp < AP_GetVital(me, HITS))
		sendsysmessage(me,"You have lost your concentration.");
		return 0;
	endif

	if (umana > AP_GetVital(me, MANA) )
		PerformAction(me,ANIM_CAST_DIR);
		sendsysmessage(me,"You do not have enough mana to cast this spell.");
		PlaySoundEffect(me,0x005d);
		PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
		return 0;
	endif

	if ( can_cast(me, circle) )
		if(mhp > AP_GetVital(me, HITS) )
			sendsysmessage(me,"You've lost your concentration.");
			return 0;
		endif
		me.frozen :=1;
		For (n:=1;n<=val;n:=n+1)
			if(mhp > AP_GETV(me, HITS))
				sendsysmessage(me,"You have lost your concentration.");
				PlaySoundEffect(me,0x005d);
				PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
				me.frozen :=0;
				return 0;
			endif
			PerformAction(me,ANIM_CAST_DIR);
			sleepms(1200);
		endfor
		sleepms(delay);
		me.frozen :=0;
		if ( SkillCheck( me,MAGERY, diff, 0 ) > 0)
			if(mhp > AP_GetVital(me, HITS))
				sendsysmessage(me,"You have lost your concentration.");
				return 0;
			endif
			return 1;
		else
			sendsysmessage(me,"The spell fails.");
			PlaySoundEffect(me,0x005d);
			PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
			return 0;
		endif
	endif

endfunction


function newusemana(who, circle, cast_on, delivery)

	if(delivery == "notarget")
		if(len(EnumerateItemsInContainer(who.backpack) >= 125))
			sendsysmessage(who,"You do not have enough room in your backpack.");
			return 0;
		endif
	  endif

	if(cdist(who.x , who.y, cast_on.x, cast_on.y) > 20)
		sendsysmessage(who,"The target is out of range.");
		return 0;
	endif

	var umana;
	case (circle)
		1:	umana :=  4;
		2:	umana :=  6;
		3:	umana :=  9;
		4:	umana := 11;
		5:	umana := 14;
		6:	umana := 20;
		7:	umana := 40;
		8:	umana := 50;
		9:	umana := 75;
		10:	umana := 100;
	endcase

	if(AP_GetVital(who, MANA) >= umana)
		AP_SetVital(who, MANA, (AP_GetVital(who, MANA) - umana));
		return 1;
	else
		sendsysmessage(who,"You do not have enough mana to cast this spell.");
		return 0;
	endif

endfunction


function mobspellcast( me, circle, scroll)
  var mhp := AP_GetVital(me, HITS);
  var val;
  var delay;
  var umana;
  var diff;
  var points;
  var n := 1;
  case (circle)
    1:   val := 1;     delay := 250;     umana := 4;      diff := 10;     points :=  50;
    2:   val := 1;     delay := 500;     umana := 6;      diff := 20;     points := 100;
    3:   val := 2;     delay := 250;     umana := 9;      diff := 30;     points := 150;
    4:   val := 2;     delay := 750;     umana := 11;     diff := 50;     points := 175;
    5:   val := 3;     delay := 250;     umana := 14;     diff := 60;     points := 200;
    6:   val := 3;     delay := 750;     umana := 20;     diff := 70;     points := 225;
    7:   val := 5;     delay :=  10;     umana := 40;     diff := 85;     points := 250;
    8:   val := 8;     delay :=  10;     umana := 50;     diff := 95;     points := 275;
    9:   val :=  9;     delay :=  10;     umana :=  75;     diff := 85;	points := 300;
   10:   val := 10;     delay :=  10;     umana := 100;     diff := 95;	points := 350;

  endcase
  if (umana > (AP_GetVital(me, MANA)))
	PerformAction(me,ANIM_CAST_DIR);
    sleepms(1200);
    PlaySoundEffect(me,0x005d);
    PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
    return 0;
  endif
  if (can_cast(me, circle))
    if(mhp < AP_GetVital(me, HITS))
      return 0;
    endif
	For (n:=1;n<=val;n:=n+1)
      if(mhp < AP_GetVital(me, HITS))
        PlaySoundEffect(me,0x005d);
        PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
        return 0;
      endif
      sleepms(1200);
    endfor
    sleepms(delay);
    if (SkillCheck( me,MAGERY, diff, points ) > 0)
      if(mhp < AP_GetVital(me, HITS))
        return 0;
      endif
      return 1;
    else
      PlaySoundEffect(me,0x005d);
      PlayObjectCenteredEffect(me, 0x3735,0x0a, 0x1e);
      return 0;
    endif
  endif
endfunction


function getpoints(c, scroll)
  var points;
  var diff;
  if(scroll == 1)
    case(c)
      1:   points :=  50; diff := 10;
      2:   points :=  50; diff := 10;
      3:   points :=  50; diff := 10;
      4:   points := 100; diff := 20;
      5:   points := 150; diff := 30;
      6:   points := 175; diff := 50;
      7:   points := 200; diff := 60;
      8:   points := 225; diff := 70;
	9:	points := 250; diff := 85;
	10:	points := 300; diff := 95;
    endcase
  else
    case(c)
      1:   points :=  50; diff := 10;
      2:   points := 100; diff := 20;
      3:   points := 150; diff := 30;
      4:   points := 175; diff := 50;
      5:   points := 200; diff := 60;
      6:   points := 225; diff := 70;
      7:   points := 250; diff := 85;
      8:   points := 275; diff := 95;
      9:	points := 300; diff := 95;
     10:	points := 350; diff := 95;

    endcase
  endif
  var skill := AP_GetSkill(caster, MAGERY);
  if(skill > diff)
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


function cdist( x1 , y1 , x2 , y2 )
  var xd := Abs(x1 - x2);
  var yd := Abs(y1 - y2);
  if (xd > yd)
    return xd;
  else
    return yd;
  endif
endfunction


function usemana(who, circle, cast_on)
  if(cast_on.mobile)
    if(!CheckLineOfSight( who, cast_on))
      SendSysMessage( who, "You can't see that." );
	  return 0;
    endif
  else
    if(!CheckLosAt( who, cast_on.x, cast_on.y, cast_on.z ))
      SendSysMessage( who, "You can't see that." );
	  return 0;
    endif
  endif
  if(cdist(who.x , who.y, cast_on.x, cast_on.y) > 15)
    sendsysmessage(who,"The target is out of range.");
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
  if(AP_GetVital(who, MANA) >= umana)
    AP_SetVital(who, (AP_GetVital(who, MANA) - umana));
    return 1;
  else
    sendsysmessage(me,"You do not have enough mana to cast this spell.");
    return 0;
  endif
endfunction