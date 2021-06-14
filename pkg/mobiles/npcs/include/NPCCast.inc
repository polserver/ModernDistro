use os;
use npc;
use util;
use cfgfile;

include "include/client";

function castspell(caster, cast_on, spellname)
  var spellid := GetSpellID(spellname);
  if(!spellid)
    return;
  endif
  if(!CheckLineOfSight(caster, cast_on))
    return;
  endif
  var parms := array;
  parms[1] := caster;
  parms[2] := spellid;
  parms[3] := 1;
  parms[4] := cast_on;
  Detach();
  start_script(":magery:spellStarter", parms);
endfunction

function GetSpellID(spellname)
  var spellid := 0, circle;
  circle := 1;
  case(lower(spellname))
    "clumsy"        : spellid := 1;
    "feeblemind"    : spellid := 3;
    "heal"          : spellid := 4;
    "magicarrow"    : spellid := 5;
    "nightsight"    : spellid := 6;
    "reactivearmor" : spellid := 7;
    "weaken"        : spellid := 8;
    "agility"       : spellid := 9;
    "cunning"       : spellid := 10;
    "cure"          : spellid := 11;
    "harm"          : spellid := 12;
    "protection"    : spellid := 15;
    "strength"      : spellid := 16;
    "bless"         : spellid := 17;
    "fireball"      : spellid := 18;
    "poison"        : spellid := 20;
    "telekinesis"   : spellid := 21;
    "teleport"      : spellid := 22;
    "wallofstone"   : spellid := 24;
    "archcure"      : spellid := 25;
    "archprot"      : spellid := 26;
    "curse"         : spellid := 27;
    "firefield"     : spellid := 28;
    "greaterHeal"   : spellid := 29;
    "lightning"     : spellid := 30;
    "manadrain"     : spellid := 31;
    "bladespirit"   : spellid := 33;
    "dispelfield"   : spellid := 34;
    "reflect"       : spellid := 36;
    "mindblast"     : spellid := 37;
    "paralyze"      : spellid := 38;
    "poisonfield"   : spellid := 39;
    "summoncreature": spellid := 40;
    "dispel"        : spellid := 41;
    "energyBolt"    :  spellid := 42;
    "explosion"     : spellid := 43;
    "invisibility"  : spellid := 44;
    "masscurse"     : spellid := 46;
    "paralyzeField" : spellid := 47;
    "reveal"        : spellid := 48;
    "chainlightning": spellid := 49;
    "energyfield"   : spellid := 50;
    "flameStrike"   : spellid := 51;
    "manavamp"      : spellid := 53;
    "massdispel"    : spellid := 54;
    "meteorswarm"   : spellid := 55;
    "polymorph"     : spellid := 56;
    "earthquake"    : spellid := 57;
    "vortex"        : spellid := 58;
    "resurrect"     : spellid := 59;
    "summonair"     : spellid := 60;
    "summondaemon"   : spellid := 61;
    "summonearth"   : spellid := 62;
    "summonfire"    : spellid := 63;
    "summonwater"   : spellid := 64;
  endcase
  return spellid;
endfunction

function cast_offensive_spell(caster, cast_on)
  if((GetObjProperty(cast_on, "summoned")) && (RandomInt(3) >= 1))
    var nexttarget := SystemFindObjectBySerial(GetObjProperty(cast_on, "master"));
    castspell(caster, cast_on, "dispel");
    if(!cast_on)
      fight(nexttarget);
    endif
  else
    var rint := RandomInt(100);
    var thespell := 0;
    foreach thing in spells
      if(rint <= thing[2])
        thespell := thing[1];
        break;
      endif
    endforeach
    if(!thespell)
      thespell := spells[1][1];
    endif
    castspell(caster, cast_on, thespell);
  endif
endfunction