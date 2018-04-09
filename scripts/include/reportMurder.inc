use uo;
use os;

include "include/bounty";
include ":datafile:datafile";
include "include/gumps";

const LONG_COUNT_DECAY_TIME := 144000; //40 hours
const SHORT_COUNT_DECAY_TIME := 28800; //8 hours


var reportables;
var reportdata := array;

function SendReportGump(who)
  GFInitGump();
  GFPage(0);
  GFNoDispose();
  GFNoClose();
  GFNoMove();
  GFResizePic(0, 0, 2600, 480, 480);
  GFButtonID(380, 345, 249, 248, 1, 99);
  GFTextLine(50, 20, 0, "Now is the time for retribution!");
  GFTextLine(50, 50, 0, "Check zero or more players to report for your murder");
  GFTextLine(50, 70, 0, "and press OKAY");
  reportables := who.reportables;
  FillInArrays();
  SetObjProperty(who, "ReportMenu", reportdata);
  var ret := GFSendGump(who);
  if(ret == 0)
    return;
  endif
  EraseObjProperty(who, "ReportMenu");
  EraseObjProperty(who, "LoginReport");
  var holder := array;
  foreach id in (ret.keys)
    if(id in reportdata)
	  var mob := SystemFindObjectBySerial(id, SYSFIND_SEARCH_OFFLINE_MOBILES);
	  if(!mob.ip)
	    SetObjProperty(mob.killer, "LoginNotify", who.serial);
	  else
	    AdjustNoto(mob, who);
	  endif
	  holder.append(id);
	  AdjustMurderCounts(mob.killer);
	endif
  endforeach
  foreach reportable in(who.reportables)
	who.removeReportable(reportable.serial, reportable.gameclock);
  endforeach
  var parms := array;
  parms.append(who);
  parms.append(holder);
  start_script(":bountyboard:bounty", parms);
endfunction

function FillInArrays()
  var prev_y := 80;
  var buttonret := 10;
  GFPage(1);
  foreach reportable in reportables
    if(!reportable.killer.cmdlevel)
	  if(buttonret == 21)
	    prev_y := 80;
	  endif
	  reportdata.append(reportable.killer.serial);
	  GFCheckBox(40, prev_y+20, 210, 211, 0, reportable.killer.serial);
	  GFTextLine(60, prev_y+20, 0, reportable.killer.name);
	  prev_y := prev_y+20;
	  buttonret := buttonret+1;
    endif
  endforeach
endfunction

function AdjustMurderCounts(killer)
  var onlinetime := GetObjProperty(killer,"onlinetimer");
  var longcount := GetObjProperty(killer,"longmurders");
  if(!longcount)
	longcount := 0;
  endif
  longcount := longcount + 1;
  SetObjProperty(killer,"longmurders",longcount);
  if(longcount>4)
	killer.setMurderer(1);
	SendSysMessage(killer,"You are now known as a murderer!");
  endif
  SetObjProperty(killer,"decaylongcountat",onlinetime+LONG_COUNT_DECAY_TIME);
  var shortcount := GetObjProperty(killer,"shortmurders");
  if(!shortcount)
	shortcount := 0;
  endif
  shortcount := shortcount + 1;
  SetObjProperty(killer,"shortmurders",shortcount);
  SetObjProperty(killer,"decayshortcountat",onlinetime+SHORT_COUNT_DECAY_TIME);
endfunction
