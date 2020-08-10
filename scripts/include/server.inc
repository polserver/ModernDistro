////////////////////////////////////////////////////////////////////////////
// server.inc - Constants used on the server side
// Thanks to Turley for this include.
// Some references to packages may not be valid for the Distro.
// Some referenced CProps may not be valid for the Distro.
////////////////////////////////////////////////////////////////////////////

// Workarounds
// ignore constants for .setfacing (frozen or not)
Const FACING_NORMAL := 0x0;
Const FACING_FORCE := 0x1;
// workarounds end

// age limits of accounts / characters {{{
// Where is the limit to the ancient character / account?
Const ANCIENT_TIME := 3600 * 24 * 360; // (12 months)
// Where is the boundary between 'active' and 'inactive' character / account?
Const ACTIVE_TIME := 3600 * 24 * 60; // (2 months)
// How long does an account count as new?
Const ACCT_NEW_TIME := 3600 * 24 * 31; // 1 month
// When is a name allowed automatically?
Const AUTO_CHECK_TIME := 3600 * 24 * 14; // 14 days
//}}}
// Properties to Accounts {{{
// e-mail address for communication (string)
Const PLAYEREMAIL := "eMail";

// Charanlegung
// create next char with this cmdlevel
Const ACCT_NEWCHAR_CMDLVL := "CharKennCMDLevel";
// Chars in the account are blocked (old dock)
Const ACCT_NEWCHAR_BLOCK := "NoCharKenn";

// times (ReadGameClock ())
// when was Account last used?
Const ACCT_LAST_USE := "lastused";
// When was the account created?
Const ACCT_CREATED := "created";

// Counter for failed login attempts
Const LOG_LOGINFAIL := "FailedLogins";

// should tips be displayed?
Const ACCT_TIP_DISABLED := "TipNo";

// which tip was last displayed
Const ACCT_TIP_LAST := "TipLast";

// Web password (password for your web interface)
Const ACCT_WEBPASS := "WebPass";
//}}}

// Properties on players {{{
// which motd was last created for the player
Const PROP_MOTD := "MsgOfDayID";
// has the name been approved?
Const PROP_NAME_UNCHECKED := "name_unchecked";

// special rights (switchable via .iddqd) (Boolean)
Const I_AM_GOD := "IamGod";
// Show Defiled Stafflers in Online Lists? (Boolean)
Const PROP_HIDE_STAFF_ONLINE := "#Unconcealed";
// List of last visited positions
Const PROP_LOC_LIST := "goback";

// do not display weapon rule hints
Const PROP_NO_WEAPON_RULE_NOTICE := "Weapons RulesMute";

// experience points
Const EXPERIENCE := "Experience";
Const EXPERIENCE_RANK := "ExperienceRank";
Const EXPERIENCE_NO_DISPLAY := "NoExpRank";

// hide tooltip of the character (guild, title, cmdlevel) for others
Const HIDE_CHAR_TOOLTIP := "notooltip";

// Do not use any AoS tooltips in the sales / purchasing menu (Boolean)
Const NO_AOS_TOOLTIP := "No_AoS_Tooltip";

// Mayor will not highlight in .online (Boolean)
Const HIDE_BM_ONLINE := "Hide_BM_Online";

// Player is not a newbie, but is in the region (Boolean)
Const PLACE_INNEWBIEAREA := "InNewbieArea";
// player is on the Goetterinsel (Boolean)
Const PLACE_ATGODPALACE := "AtGodPalace";
// Player is in a fight (Array)
Const ARENABATTLE := "ArenaBattle";
// was the player criminal before the fight?
Const PROP_ARENACRIME := "#ArenaCrimeBefore";
// was donated to the gods (access Getterinsel)?
Const PROP_GODDONATION := "donation_ok";

// player is new on Pergon, on Jhe'lom (Boolean)
Const TYPNEWBIE := "jhelomnewbie";
// player may use the escape ferry
Const TYPFLIGHT := "permissive";
// Player is PK
Const TYPKILLER := "permared";

// player is a librarian (Boolean)
Const TYPIBILIBRARY := "Librarian";
// Player is a QuestCharacter (Boolean)
Const TYPQUESTCHAR := "QuestChar";
// Player is a treasurer / Kaemmerer etc. (Boolean)
Const TYPKASSENWART := "treasurer";

// Player does not want PvP
Const PROP_WANTS_PVP := "pvpwanted";
// Player prefers RP
Const PROP_WANTS_RP := "rpwanted";
// Character does not want to be attacked
Const PROP_NOATTACK := "Peacemaker";
// Characters want to use magic light
Const PROP_MAGILITE := "magilite";
// Character wants automatic warmode
Const PROP_AUTOWAR := "autowar";
// Player wants job in the online survey
Const PROP_SHOW_PROF := "showprof_online";

// sound effects on .msg, .letterbox and .page
Const PROP_NOTIFY_MESSAGE := "notify_msg";
Const PROP_NOTIFY_LETTER := "notify_letter";
Const PROP_NOTIFY_PAGE := "notify_page";

// hunger status
Const PROP_HUNGER := "hungrylevel";

// Who was last on the victim? (Array)
Const LOG_LASTHIT := "LastHit";
// Who last hurt the victim? (Array)
Const LOG_LASTDAMAGE := "LastDamage";
// Who did how much damage? (Dictionary)
Const LOG_DAMAGER := "#TotalDamager";
// death count
Const LOG_DEATHCOUNT := "DeathCounter";
Const LOG_DEATHCOUNTM := "DeathCounterMonth";

// penalty system
// jail time to be served / pillory / hell time
Const PROP_PUNISH_JAILTIME := "jailtime";
// Type of punishment (jail, quarry, pillory, death)
Const PROP_PUNISH_TYPE := "PunishType";
// Position before denouncing / quarry for dismissal
Const PROP_PUNISH_OLDPOS := "PunishPositionBefore";
// Number of stones to be knocked in the quarry
Const PROP_PUNISH_STONES := "PunishStones";

// time until skill use is possible again (Prop> ReadGameClock ())
Const PROP_NEXTSKILLUSE := "#UsesSkill";

// Followmode active
Const PROP_FOLLOWMODE := "#follows";
// .controlnpc
Const PROP_CONTROLNPC := "remotecontrol";
// uses magic compass?
Const PROP_COMPASS := "#magiccompass";
//}}}

// Properties on NPCs {{{
// it was summoned
// (values ??see pkg / magic / newmagic / spells / magicpergon.inc)
Const PROP_SUMMONED := "summoned";

// Animal remains forever loyal and can not be transferred
// from value 99 it does not starve anymore (scripts / include / animal.inc)
// FIXME: several levels, further bonuses
Const PROP_COMPANION := "companion";
//}}}

// Properties on players and items {{{
// Data warehouse for secondcharge support
Const SELF_SUPPORT1 := "SupportCheck1";
Const SELF_SUPPORT2 := "SupportCheck2";
//}}}

// Properties on Items {{{
// Cprop struct with material / manufacturer for tooltip
Const TOOLINFO := "ToolInfo";

// which message entry belongs to the motd item?
Const PROP_MOTD_ID := "MsgID";

// evidence
// Who Poisoned It (Name / Serial)
Const EV_POISONER := "p";

// who sold it (name / serial)
Const EV_VENDOR := "v";

// Who cut it (name / serial)
Const EV_CHOPPER := "c";
//}}}

// global property constants {{{
// Pergon or BauServer?
Const Pergon := "Pergon";
Const BAUSERVER := "BauServer";
// display name (eg for web interface)
Const GPROP_SERVERNAME := "ServerName";

// display name (eg for web interface)
Const GPROP_DEBUG_ACCOUNT := "DebugAccount";

// deactivate Skillcheck for ferry / escape gate?
Const PROP_DISABLE_SKILLCHECK := "newbiegatenoskillcheck";

// active shutdown?
Const PROP_SHUTDOWN_RUNNING := "#ShutDown";

// weapons rules active (value 111)
Const PROP_WEAPON_RULES_ACTIVE := "Weapon Rules Enabled";

// Check if people are fighting passively (for IsFighting)
Const PROP_DEFENSE_IS_FIGHT := "SelfDefenseIsFight";

// list of pages
Const GM_PAGE_LIST := "gmpages";
// Flag if Pages are currently being edited
Const GM_PAGE_ACTIVITY := "#gmpages";

// Account and Char Management
// Count the server starts
// (nice for statistics, but also as a slow logical clock)
Const SERVER_STARTUPS := "StartUps";
// Chars / accounts for statistics already used on the day
Const STAT_DAILY_ACCS := "DailyAccs";
Const STAT_DAILY_CHARS := "DailyChars";
// List of new accounts for account / name team
// struct {accounts: array of account names, lastupdate: int}
// Const NEW_ACCOUNTS := "#NewAccounts";

// activate unlimited character creation (for test server)
Const CREATE_ALLOW_FREE := "CharCreateFree";
// allows you to create two characters yourself (new concept)
Const CREATE_ALLOW_SELF := "CharCreateSelf";
//}}}

// command level {{{
// Changes to CmdLevel involve (at least): cmds.cfg,
// setcmdlevel.src, onlinesearch.inc
Const CMDLEVEL_PLAYER := 0;
Const CMDLEVEL_COUNSELOR := 1;
Const CMDLEVEL_QUESTCHAR := 2;
Const CMDLEVEL_SEER := 3;
Const CMDLEVEL_GM := 4;
Const CMDLEVEL_HIGHGM := 5;
Const CMDLEVEL_GL := 6;
Const CMDLEVEL_SCRIPTER := 7;
Const CMDLEVEL_DEVELOPER := 8;
//}}}

// NPC alignment {{{
Const ALIGNMENT_GOOD := 2;
Const ALIGNMENT_NEUTRAL := 0;
Const ALIGNMENT_EVIL := 1;
//}}}

// Housing {{{
// common constants for static and multihouses
Const PROP_HS_C_ACCOUNT := "owneracct";
Const PROP_HS_C_OWNER := "ownerserial";
Const PROP_HS_C_OWNERNAME := "lastownername";
Const PROP_HS_C_FRIENDS := "friendlist";
// moored
Const PROP_HS_C_FIXED := "lockeddown";
Const PROP_HS_C_FIX_NUM := "RemainingLockdowns";
// SC = secure container
Const PROP_HS_C_SC_NUM := "RemainingSecures";
Const PROP_HS_C_SC_BOX := "boxserial";
Const PROP_HS_C_SC_HOUSE := "houseserial";
// Props for rental ad
Const PROP_HS_C_RENT_TOPAY := "rent_topay_count";
Const PROP_HS_C_RENT_TOOLTIP := "rent_tooltip";

// Props for static houses
Const PROP_HS_S_CTRL_PID := "#PID";
Const PROP_HS_S_DIMENSIONS := "houseinfo";

// Props for multihouses
// reference to house to multicomponent
Const PROP_HS_M_HOUSE := "house_serial";

// Globalprop, if set, container inventory will be run
Const GPROP_HS_C_CONTINV := "container_inv";
//}}}

// music {{{
Const PROP_MUSIC_BARDING := "#barding";
Const PROP_MUSIC_DANCE := "DanceToBard";
Const PROP_MUSIC_LISTEN := "#listentobarding";
Const PROP_MUSIC_STEALTH := "#listenstealth";
Const PROP_MUSIC_SCR_SONGNAME := "musicname";
Const PROP_MUSIC_SCR_NOTES := "noteslist";
Const PROP_MUSIC_SCR_IS_COPY := "copied";
//}}}

Const REALM_DEATH := "deaths_domain";

Const GLOBAL_POL_CFG_STRUCT := "#PolCfgStruct";
Const GLOBAL_SSOPT_CFG_STRUCT := "#ServSpecOptCfgStruct";

Function ReadPolCFG ()
  Var lines := ReadFile ("pol.cfg");
  if(lines.errortext)
	Broadcast("Server.inc --> Unable to read pol.txt. Error is " + lines.errortext, 1, 33);
	return;
  endif
  Var pol := struct;
  ParseCfg (lines, pol);
  SetGlobalProperty (GLOBAL_POL_CFG_STRUCT, pol);
  return pol;
EndFunction

Function ReadSSOptCFG ()
  Var lines := ReadFile ("config/servspecopt.cfg");
  if(lines.errortext)
	Broadcast("Server.inc --> Unable to read pol.txt. Error is " + lines.errortext, 1, 33);
	return;
  endif
  Var ssopt := struct;
  ParseCfg (lines, ssopt);
  SetGlobalProperty (GLOBAL_SSOPT_CFG_STRUCT, ssopt);
EndFunction

Function ParseCfg (lines, byref cfgstruct)
  Foreach line in lines
    line := Trim (line);
    If (line [1] in {"#", "/"})
      continue;
    EndIf
    var pos := Find (line, "=", 1);
    If (! Pos)
      pos := Find (line, cchr (32), 1); // blank
      If (! Pos)
      pos := Find (line, cchr (9), 1); //tab
      If (! Pos)
          continue;
        EndIf
      EndIf
    EndIf
    var value := Trim (line [pos + 1, len (line) -pos]);
    var tvalue := Lower (CStr (value)); // Numeric check
    If ((tvalue == Lower (CStr (CInt (tvalue)))) || (tvalue == Lower (CStr (Hex (CInt (tvalue))))))
      value := CInt (value);
    EndIf
    cfgstruct.insert (Trim (line [1, pos-1]), value);
  EndForEach
EndFunction
