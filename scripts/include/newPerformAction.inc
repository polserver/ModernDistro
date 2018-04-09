use polsys;

function NewPerformAction(who, anim, action := 0, subaction := 0)
	var outpkt := CreatePacket(0xE2, 10);
  outpkt.SetInt32(1, who.serial);
	outpkt.SetInt16(5, anim);
	outpkt.SetInt16(7, action);
	outpkt.SetInt8(9, subaction);
	outpkt.SendAreaPacket(who.x, who.y, 18, who.realm);
//  sleepms (1500);
  return 0;
endfunction 

const NPA_ANIM_ATTACK := 0x0;
const NPA_ANIM_PARRY := 0x1;
const NPA_ANIM_BLOCK := 0x2;
const NPA_ANIM_DIE := 0x3;
const NPA_ANIM_HIT := 0x4;
const NPA_ANIM_FIDGET := 0x5;
const NPA_ANIM_EAT := 0x6;
const NPA_ANIM_EMOTE := 0x7;
const NPA_ANIM_ALERT := 0x8;
const NPA_ANIM_TAKEOFF := 0x9;
const NPA_ANIM_LAND := 0xa;
const NPA_ANIM_SPELL := 0xb;
const NPA_ANIM_STARTCOMBAT := 0xc;
const NPA_ANIM_ENDCOMBAT := 0xd;
const NPA_ANIM_PILLAGE := 0xe;
const NPA_ANIM_SPAWN := 0xf;

const NPA_ATTACK_PUNCH := 0x0;
const NPA_ATTACK_BOW := 0x1;
const NPA_ATTACK_CROSSBOW := 0x2;
const NPA_ATTACK_1HMACE := 0x3;
const NPA_ATTACK_1HSWORD := 0x4;
const NPA_ATTACK_1HFENCING := 0x5;
const NPA_ATTACK_2HMACE := 0x6;
const NPA_ATTACK_2HSWORD := 0x7;
const NPA_ATTACK_2HFENCING := 0x8;

const NPA_ACT_DIE := 0x0;
const NPA_DIE_FORWARD := 0x0;
const NPA_DIE_BACKWARD := 0x1;

const NPA_ACT_FIDGET := 0x0;
const NPA_SUB_FIDGET_1 := 0x0;
const NPA_SUB_FIDGET_2 := 0x1;

const NPA_EMOTE_BOW := 0x0;
const NPA_EMOTE_SALUTE := 0x1;

const NPA_SPELL_AREASPELL := 0x2;
const NPA_SPELL_DIRSPELL:= 0x3;
const NPA_AREASPELL_SUB := 0x0;
const NPA_DIRSPELL_SUB := 0x1;

function PerformAction2(who, oldaction, framecount:=5, repeatcount:=1, backward:=ACTION_DIR_FORWARD, repeatflag:=ACTION_NOREPEAT, delay:=1)
  var anim :=0, action := 0, subaction := 0;
  case (oldaction)
    0x00:// walk
    0x01:// walk with staff
    0x02:// run
    0x03:// run with staff
    0x04:// idle
      sleep (2);
      return;
    0x05:// look around
      anim := NPA_ANIM_FIDGET;
      action := NPA_ACT_FIDGET;
      subaction := NPA_SUB_FIDGET_1;
    0x06:// stomp feet
      anim := NPA_ANIM_FIDGET;
      action := NPA_ACT_FIDGET;
      subaction := NPA_SUB_FIDGET_2;
    0x07:// combat idle 1h
    0x08:// combat idle 2h
      sleep (2);
      return;
		ANIM_ATTACK_1HAND_WIDE:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_1HSWORD;
		ANIM_ATTACK_1HAND_JAB:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_1HFENCING;
      anim := 1;
		ANIM_ATTACK_1HAND_DOWN:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_1HMACE;
      subaction := 1;
		ANIM_ATTACK_2HAND_DOWN:
      action := NPA_ATTACK_2HMACE;
      anim := NPA_ANIM_ATTACK;
		ANIM_ATTACK_2HAND_WIDE:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_2HSWORD;
		ANIM_ATTACK_2HAND_JAB:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_2HFENCING;
      subaction := 1;
    ANIM_WALK_WARMODE:
      sleep (2);
      return;
    ANIM_CAST_DIR:
      anim := NPA_ANIM_SPELL;
      action := NPA_SPELL_DIRSPELL;
      subaction := NPA_DIRSPELL_SUB;
    ANIM_CAST_AREA:
      anim := NPA_ANIM_SPELL;
      action := NPA_SPELL_AREASPELL;
      subaction := NPA_AREASPELL_SUB;
		ANIM_ATTACK_BOW:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_BOW;
		ANIM_ATTACK_XBOW:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_CROSSBOW;
    ANIM_HIT:
      anim := NPA_ANIM_HIT;
    ANIM_DIE_BACK:
      anim := NPA_ANIM_DIE;
      if (who.graphic == 666 or who.graphic == 667)
        action := NPA_DIE_FORWARD;
      else
        action := NPA_DIE_BACKWARD;
      endif
    ANIM_DIE_FOR:
      anim := NPA_ANIM_DIE;
      if (who.graphic == 666 or who.graphic == 667)
        action := NPA_DIE_BACKWARD;
      else
        action := NPA_DIE_FORWARD;
      endif
    0x17:// horse walk
    0x18:// horse run
    0x19:// horse idle
      sleep (2);
      return;
		0x1A:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_2HSWORD;
    0x1B:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_BOW;
    0x1C:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_CROSSBOW;
    0x1D:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_PUNCH;
    0x1E:
      anim := NPA_ANIM_PARRY;
    0x1F:
      anim := NPA_ANIM_ATTACK;
      action := NPA_ATTACK_PUNCH;
    ANIM_BOW:
      anim := NPA_ANIM_EMOTE;
      action := NPA_EMOTE_BOW;
    ANIM_SALUTE:
      anim := NPA_ANIM_EMOTE;
      action := NPA_EMOTE_SALUTE;
    ANIM_EAT:
      anim := NPA_ANIM_EAT;
  endcase
  NewPerformAction(who, anim, action, subaction);
  sleep(2);
  return 0;
endfunction

/*

Spell
2, 0 Area
3, 1 Directional


*/