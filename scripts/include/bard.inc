use uo;

include "include/client";
include ":attributes:attributes";
include "include/string";

function play(character, theskill, item, attrib)
  var success, fail;
  case(item.graphic)
    0x0eb1:  success := 0x44;  fail := 0x45;
    0x0eb2:  success := 0x46;  fail := 0x47;
    0x0eb3:  success := 0x4d;  fail := 0x4e;
    0x0eb4:  success := 0x4d;  fail := 0x4e;
    0x0e9c:  success := 0x39;  fail := 0x3a;
    0x0e9d:  success := 0x53;  fail := 0x54;
    0x0e9e:  success := 0x53;  fail := 0x54;
  endcase
  var thepoints := GetAttribute(character, MUSICIANSHIP);
  if(SkillCheck(character, MUSICIANSHIP, theskill, thepoints))
    if(SkillCheck(character, attrib, theskill, thepoints * 2))
      PlaySoundEffect( character, success);
      return 1;
    else
      PlaySoundEffect( character, fail);
      return 0;
    endif
  else
    PlaySoundEffect( character, fail);
    return 0;
  endif
endfunction

function findinstrument(character)
  var instruments := {0x0eb1, 0x0eb2, 0x0eb3, 0x0eb4, 0x0e9c, 0x0e9d, 0x0e9e};
  foreach item in EnumerateItemsInContainer(character.backpack)
    if(item.graphic in instruments)
      return item;
    endif
  endforeach
endfunction

function can_sing( me, circle)

	var cfg1 := readconfigfile(":combat:itemdesc");
//	var item;

	foreach item in ListEquippedItems(me)
		if ((cfg1[item.objtype].blockcircle) && (cfg1[item.objtype].blockcircle <= circle)) 
                	SendSysMessage(me, "Your equipment prevents you from playing the song!", color := 33);
                	return 0;
		endif
	endforeach

	return 1;

endfunction

function PlayNoise(me, instrument, difficulty)

	var cfg := ReadConfigFile (":musicianship:itemdesc");
	var elem := FindConfigElem (cfg, instrument.objtype);

	if (!difficulty)
		MakeSound (me, elem.succeed);
		return 1;
	elseif (  SkillCheck(me, instrument, MUSICIANSHIP, difficulty) )
		MakeSound (me, elem.succeed);
		return 1;
	else
		MakeSound (me, elem.fail);
		return 0;
	endif

endfunction




///////////////////
//  does the sound effect for the musical instrument, dependant on if people want to hear it or not
///////////////////

function MakeSound (character, soundeffect)
	foreach mobile in ListMobilesNearLocationEx (character.x, character.y, character.z, 15, 
			LISTEX_FLAG_NORMAL+LISTEX_FLAG_HIDDEN+LISTEX_FLAG_GHOST, character.realm)
		if (mobile.acct)
			var acct := mobile.acct;
			var sound_options := acct.GetProp ("sound_options");
			if (!sound_options or !sound_options[1])
				PlaySoundEffectPrivate (character, soundeffect, mobile);
			endif
		endif
	endforeach
endfunction

/////////////////
///  From this point down was added for songbooks
/////////////////

function PracticeSongInfo(songnumber)
	var config := ReadConfigFile("practicesongs");
	return config[songnumber];
endfunction

function SongInfo(songnumber)
	var config := ReadConfigFile("bardsongs");
	return config[songnumber];
endfunction

function BardReleaseScript(pid)
	var process := GetProcess(pid);
	process.SendEvent("done");
endfunction

function GetHarmonizedBards(who, range:=5)
	var numbards := 0;
	foreach person in ListMobilesInLineOfSight(who, range)
		if ( GetObjProperty(person, "#harmonizing") == who.serial )
			numbards := numbards + 1;
		endif
	endforeach
	
	return numbards;
endfunction

function BardTarget(byref who, byref npctarget, prompt := "Select a target", ops := TGTOPT_CHECK_LOS + TGTOPT_NEUTRAL)
	var x := who.x;
	var y := who.y;
	who.hidden := 0;
	if (npctarget)
		return npctarget;
	endif
	
	if (who.npctemplate)
		return;
	endif
	
	SendSysMessage(who, prompt);
	
	var targ := target(who, ops);
	
	if ( ( who.x == x ) && ( who.y == y ) )
		return targ;
	else
		SendSysMessage(who, "You stop playing your song.");
		return 0;
	endif
endfunction

function BardTargetCoordinates(byref who, byref params := "", prompt := "Select a target")
	var x := who.x;
	var y := who.y;
	
	who.hidden := 0;
	if (params[3])
		return params[3];
	elseif ( who.npctemplate )
		return;
	endif
	
	SendSysMessage(who, prompt);
	var targ := TargetCoordinates(who);
	
	if ( ( who.x == x ) && ( who.y == y ) )
		return targ;
	else
		SendSysMessage(who, "You stop playing your song.");
		return 0;
	endif
endfunction

function IsInGroup(bard, mobile, chklos := 0)
	if (bard == mobile)
		return 1;
	endif
	
	if ( (chklos) && ! (CheckLineOfSight(bard, mobile)) )
		return 0;
	elseif ( !( mobile.serial in GetObjProperty(bard, "BardGroup") ) )
		return 0;
	else
		return 1;
	endif
endfunction

function CheckStam(who, amount)
	if ( GetStamina(who) >= amount )
		SetStamina(who, GetStamina(who) - amount);
		return 1;
	else
		return 0;
	endif
endfunction


function BardResisted(caster, cast_on, amt, circle := 0)
	// Purpose: to check for resistance to spells cast upon a character
	// called by: most spells
	// returns: amount of damage not resisted?

        var musicianship := CInt(GetAttribute(caster, MUSICIANSHIP));
        var evalint := CInt(GetAttribute(caster, EVALUATING_INTELLIGENCE));
        var resist := CInt(GetAttribute(cast_on, RESISTING_SPELLS));
        
        // chk1 relates to the resist ability.
        var chk1 := (resist / 5);
        // chk2 relates to the difficulty level
        var chk2 := resist - (((musicianship - 20) / 5) + (circle * 5));
        
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
        
        var points := getbardresistpoints(circle, cast_on, RESISTING_SPELLS);
        if(cast_on.dead)
                return 0;
        endif
        if(SkillCheck(cast_on, RESISTING_SPELLS, diff, points))
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

function getbardresistpoints(c, victim, attribid)
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
  var skill := GetAttribute(victim, attribid);
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

// function GetAudience(who, range, flags)
// This function returns a list of valid "listeners" to the song,
// based on restrictions set forth in an array "flags".  "Flags"
// must have either "ALL", "HOSTILES", or "NEUTRALS" as one of 
// the elements, and may not have more than one.  In addition,
// the following elements will restrict certain types of mobiles:
//
//	"LOS":		only affects mobiles in LOS
//	"PEACE":	excludes mobiles in warmode
//	"WAR":		excludes mobiles not in warmode
//	"HUMAN":	only affects humans
//	"RACE":		reads a list of affected mobiles.
//	"SELF":		add the bard to the list.
//
// NOTE: If "RACE" is chosen, then the LAST ELEMENT in flags MUST
// be an ARRAY containing the list of graphics that are to be
// included in the audience.  

function GetAudience(who, range, flags)

	var audience := {};
	var templist := {};
	var tempflag := 0;
	var x;

	if ("ALL" in flags)
		if ("LOS" in flags)
			templist := ListMobilesInLineOfSight(who, range);
			templist[(len(templist) + 1)] := who;
		else
			templist := ListMobilesNearLocation(who.x, who.y, who.z, range);
		endif
		audience := ArrayAppend(audience, templist);
	elseif ("HOSTILES" in flags)
		if ("LOS" in flags)
			tempflag := 1;
		endif
		templist := ListHostiles(who, range, tempflag);
		audience := ArrayAppend(audience, templist);
	elseif ("NEUTRALS" in flags)
		if ("LOS" in flags)
			templist := ListMobilesInLineOfSight(who.x, range);
			templist[(len(templist) + 1)] := who;
		else
			templist := ListMobilesNearLocation(who.x, who.y, who.z, range);
		endif

		var hostilelist := ListHostiles(who, range);
		for (x := 1; x <= len(templist); x := x + 1)
			if (templist[x] in hostilelist)
				templist.Erase(x);
			endif
		endfor

		audience := ArrayAppend(audience, templist);
		tempflag := "";
	else
		return 0;
	endif

	if ("PEACE" in flags)
		for (x := 1; x <= len(audience); x := x + 1)
			tempflag := audience[x];
			if (tempflag.warmode == 1)
				audience.Erase(x);
			endif
		endfor
		tempflag := "";
	endif

	if ("WAR" in flags)
		for (x := 1; x <= len(audience); x := x + 1)
			tempflag := audience[x];
			if (tempflag.warmode == 0)
				audience.Erase(x);
			endif
		endfor
		tempflag := "";
	endif

	if ("HUMAN" in flags)
		for (x := 1; x <= len(audience); x := x + 1)
			tempflag := audience[x];
			if ( (tempflag.graphic != 0x190) and (tempflag.graphic != 0x191) )
				audience.Erase(x);
			endif
		endfor
		tempflag := "";
	endif

	if ("RACE" in flags)
		x := len(flags);
		var fxtype := flags[x];
		for (x := 1; x <= len(audience); x := x + 1)
			tempflag := audience[x];
			if ( tempflag.graphic in fxtype)
				x := x;
			else
				audience.Erase(x);
			endif
		endfor
	endif

	return audience;

endfunction

// function PlayMidi(who, listeners, song)
//
// This function will play midi #<song> to all players listed in an 
// array <listeners>.

function PlayMidi(listeners, song)

	var packet_id;

	if (!listeners)
		return 0;
	endif

	foreach character in listeners
		if (character.acct)
			packet_id := "6D00" + song;
			SendPacket (character, packet_id);
		endif
	endforeach

	return 1;

endfunction

