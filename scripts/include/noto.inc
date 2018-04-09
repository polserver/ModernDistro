use uo;
use util;
use math;

include ":attributes:attributes";
include "include/virtue";

var nototitles :=
{
    {"Outcast",     "Wretched",     "Nefarious", "Dread",         "Dread"},
    {"Despicable",  "Dastardly",    "Wicked",    "Evil",          "Evil"},
    {"Scoundrel",   "Malicious",    "Vile",      "Villainous",    "Dark"},
    {"Unsavory",    "Dishonorable", "Ignoble",   "Sinister",      "Sinister"},
    {"Rude",        "Disreputable", "Notorious", "Infamous",      "Dishonored"},
    {"None",        "Notable",      "Prominent", "Reknowned",     ""},
    {"Fair",        "Upstanding",   "Reputable", "Distinguished", "Distinguished"},
    {"Kind",        "Respectable",  "Proper",    "Eminent",       "Eminent"},
    {"Good",        "Honorable",    "Admirable", "Noble",         "Noble"},
    {"Honest",      "Commendable",  "Famed",     "Illustrious",   "Illustrious"},
    {"Trustworthy", "Estimable",    "Great",     "Glorious",      "Glorious"}
};

function AdjustNoto(attacker, defender)
  if(attacker.isA(POLCLASS_NPC))
    return;
  endif
  var karma   := CInt(GetObjProperty(attacker, "Karma"));
  var fame    := CInt(GetObjProperty(attacker, "Fame"));
  var dkarma  := CInt(GetObjProperty(defender, "Karma"));
  var dfame   := CInt(GetObjProperty(defender, "Fame"));
  var dfchk, dkchk;
  if(defender.isA(POLCLASS_MOBILE))
    dkarma := dkarma / 100.0;
    dkchk := dkarma * 10;
  elseif(!GetStrength(defender))
    dkarma := dkarma * 10;
    dkchk := dkarma * 10;
    dkchk := 0 - dkchk;
  endif
  if(defender.isA(POLCLASS_MOBILE))
    dfame := dfame / 100.0;
    dfchk := dfame * 10;
  elseif(!GetStrength(defender))
    dfchk := dfame * 100;
    dfame := dfame * 10;
  endif
  var kmod      := GetKarmaLevel(karma) - 6;
  var fmod      := GetFameLevel(fame);
  var dkmod     := GetKarmaLevel(dkchk) - 6;
  var dfmod     := GetFameLevel(dfchk);
  var famegain  := dfame;
  var karmagain := (0 - dkarma);
  var chk := 0;
// The following conditional was added to handle shrine system.
// The lines commented out were written for a different karma/fame system and until I understand the system that we currently use
// they will be left commented out.
  if (dKarma > 15)
//    kmod := -1;
//    dKarma := Abs(Karma);
    AdjustVirtue(attacker, "Compassion", -1, 100);
  else
//    dKarma := Abs(vKarma);
    AdjustVirtue(attacker, "Valor", 1, 50);
    if (dKarma > 150)
        AdjustVirtue(attacker, "Honor", 1, 10);
    endif
  endif
/////////////////////////////////////////////////////////////////
  if(karmagain >= 0)
    chk := 1;
  endif
  if(Abs(kmod) > Abs(dkmod))
    karmagain := 0;
  elseif(Abs(kmod) == Abs(dkmod))
    karmagain := Abs(karmagain) / 4;
    if(chk == 0)
      karmagain := (0 - karmagain);
    endif
  else
    karmagain := Abs(karmagain) / 2;
    var holder := 0;
    case(dkmod - kmod)
      1: holder := 0.15;
      2: holder := 0.30;
      3: holder := 0.45;
      4: holder := 0.60;
      5: holder := 0.75;
      6: holder := 0.90;
    endcase
    karmagain := karmagain + CInt(karmagain * holder);
    if(chk == 0)
      karmagain := (0 - karmagain);
    endif
  endif
  if(fmod == dfmod)
    famegain := famegain / 4;
  elseif(fmod > dfmod)
    famegain := 0;
  else
    famegain := famegain / 2;
  endif
  if(defender.isA(POLCLASS_MOBILE))
    famegain := famegain / 100;
    karmagain := karmagain / 100;
  endif
  karma := karma + karmagain;
  fame := fame + famegain;
  var online := 0;
  if(!attacker.isA(POLCLASS_NPC))
    foreach char in EnumerateOnlineCharacters()
      if(char == attacker)
        online := 1;
        break;
      endif
    endforeach
    if(online == 1)
      SetObjProperty(attacker, "Karma", karma);
      SetObjProperty(attacker, "Fame", fame);
      SendGainMessage(attacker, "Fame", famegain);
      SendGainMessage(attacker, "Karma", karmagain);
    else
      SetObjProperty(attacker, "ModKarma", kmod);
      SetObjProperty(attacker, "ModFame", famegain);
    endif
  else
    SetObjProperty(attacker, "Karma", karma);
    SetObjProperty(attacker, "Fame", fame);
  endif
  if(attacker.isA(POLCLASS_MOBILE))
    SetNotoTitle(attacker, karma, fame);
  endif
  if(defender.isA(POLCLASS_MOBILE))
    SetNotoTitle(defender, kmod, fmod);
  endif
endfunction

function GetFameLevel(fame)
  var fmod := 0;
  if((fame <= 1249) && (fame >= 0))
    fmod := 1;
  elseif((fame <= 2499) && (fame >= 1250))
    fmod := 2;
  elseif((fame <= 4999) && (fame >= 2500))
    fmod := 3;
  elseif((fame <= 9999) && (fame >= 5000))
    fmod := 4;
  elseif(fame >= 10000)
    fmod := 5;
  endif
  return fmod;
endfunction

function GetKarmaLevel(karma)
  var kmod := 0;
  if(karma <= -10000)
    kmod := 1;
  elseif((karma <= -5000) && (karma >= -9999))
    kmod := 2;
  elseif((karma <= -2500) && (karma >= -4999))
    kmod := 3;
  elseif((karma <= -1250) && (karma >= -2499))
    kmod := 4;
  elseif((karma <=  -625) && (karma >= -1249))
    kmod := 5;
  elseif((karma <=   624) && (karma >=  -624))
    kmod := 6;
  elseif((karma <=  1249) && (karma >=   625))
    kmod := 7;
  elseif((karma <=  2499) && (karma >=  1250))
    kmod := 8;
  elseif((karma <=  4999) && (karma >=  2500))
    kmod := 9;
  elseif((karma <=  9999) && (karma >=  5000))
    kmod := 10;
  elseif(karma >= 10000)
    kmod := 11;
  endif
  return kmod;
endfunction

function SetNotoTitle(who, karma := 0, fame := 0)
  var kmod := GetKarmaLevel(karma);
  var fmod := GetFameLevel(fame);
  var newtitle := nototitles[ (kmod) ];
  newtitle := "The " + CStr(newtitle[fmod]) + " ";
  if(newtitle["None"])
    newtitle := "";
  endif
  if(fmod == 5)
    if(who.gender == 1)
      newtitle := newtitle + "Lady ";
    else
      newtitle := newtitle + "Lord ";
    endif
  endif
  if(newtitle != who.title_prefix)
    who.title_prefix := newtitle;
    SendSysMessage(who, "you are now known as " + newtitle + who.name);
  endif
endfunction

function SendGainMessage(who, type, amount)
  var msgtext := "You have ";
  if(amount < 0)
    msgtext := msgtext + "lost ";
    amount := Abs(amount);
  else
    msgtext := msgtext + "gained ";
  endif
  if(amount > 150)
    msgtext := msgtext + "a great amount of ";
  elseif(amount > 125)
    msgtext := msgtext + "alot of ";
  elseif(amount > 75)
    msgtext := msgtext + "a good amount of ";
  elseif(amount > 30)
    msgtext := msgtext + "some ";
  elseif(amount > 0)
    msgtext := msgtext + "a little ";
  else
    return 0;
  endif
  msgtext := msgtext + type + ".";
  SendSysMessage(who, msgtext);
endfunction