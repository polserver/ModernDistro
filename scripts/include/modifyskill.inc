///////////////////////////////////////////////////////////////////////////
// ModifySkill include - Alles zur Veraenderung der Skills
//
//   Author: Shinigami
//   eMail : Shinigami@gmx.net
//
// TODO:
// - Was passiert eigentlich, wenn'ich GetSkill, ...
//   mit einer Zahl aufrufe? bzw. mit einem falschen string?
// - auf lange Sicht die RawSkill-Funktionen ausmerzen, also schauen,
//   wieso dort grade das Raw-Teil benutzt wird. ebendeso prüfen,
//   inwieweit die ganzen GetSkill-Teile mit der einen Nachkommastelle
//   klarkommen...

///////////////////////////////////////////////////////////////////////////
// Benutzte CProps
//   CInt(Sk_*) - Raw Skills

///////////////////////////////////////////////////////////////////////////
// Skill-Manipulation
//     CheckSkillPergon          - Prueft und erhoeht Skill
//     MakeCheckSkillPergon      - Prueft und erhoeht Skill
//     CalcCheckSkillPergon      - Berechnet und realisiert den Skillanstieg
//     AdvanceStatsPergon        - Erhoeht die Stats nach einem erfolgreichen SkillCheck
//     GetSkillPergon            - Liefert den Skillwert                    / 0..9000
//     GetSkillFloatPergon       - Liefert den wahren Skillwert             / 0,0..9000,0
//     SetSkillPergon            - Setzt den Skillwert                      / 0,0..6000,0 (Player 0,0..209,0)
//     GetRawSkillPergon         - Liefert den Raw Skillwert                / 0..0x7fffffff
//     SetRawSkillPergon         - Setzt den Raw Skillwert                  / 0..0x7fffffff
//     AwardRawPointsPergon      - Erhoeht den Raw Skillwert                / -0x7fffffff..0x7fffffff
//     CheckSkillLimitAndCut     - Prueft das SkillLimit und schnippelt ggf. gemaess Stufe
//     RawSkillToBaseSkillPergon - Wandelt die Punkte in %'te um
//     BaseSkillToRawSkillPergon - Wandelt die %'te in Punkte um
//     ShowSkillAdvMsgPergon     - Skillanstiegsmeldung anzeigen
//     HasPlayerSkill            - Hat der Spieler diesen Skill?
//     AlterSkillModPergon       - Ändert den Skill-Modifikator
//     GetSkillAverage           - ermittelt Skill-Mittelwerte
//     GetSkillOverview          - Ueberblick ueber Skills von who besorgen

// Initialisierung
//     InitSkillsMore          - Initialisiert die zusaetzlichen Skills
//     CheckLimitConstants     - Prueft die Konstanten der Limits
//     CheckPlayerLimitsAndCut - Prueft das Limit aller Skills sowie Attribute und schnippelt ggf.

// Skill-Konfiguration
//     GetWholeSkillConfig   - Ermittelt die statischen Daten aller Skills
//     GetTitleIDSkill       - Liefert die passende TitelID (GlobalProp SKTITLES)
//     GetSkillDefaultPoints - Liefert die Standard-Punkte des Skills
//     CheckSkillNameProp    - Ueberprueft den SkillNamen in der Property
//     GetSkillName          - Liefert den Namen des Skills

///////////////////////////////////
// Umbauarbeiten Pol092 -> Pol094
///////////////////////////////////
//
// Faellt raus:
//#   CheckSkill_                   -> CheckSkillPergon [& Hook]
//#   GetSkill_                     -> GetSkillPergon
//#   GetRawSkill_                  -> GetRawSkillPergon
//#   SetRawSkill_                  -> SetRawSkillPergon
//#   AwardRawPoints_               -> AwardRawPointsPergon
//#   character._dexterity          -> GetDexPergon
//#   character._intelligence       -> GetIntPergon
//#   character._strength           -> GetStrPergon
//#   character._setdex(stat)       -> SetDexPergon
//#   character._setint(stat)       -> SetIntPergon
//#   character._setstr(stat)       -> SetStrPergon
//#   character._dexterity_mod      -> GetDexModPergon  / SetDexModPergon
//#   character._intelligence_mod   -> GetIntModPergon  / SetIntModPergon
//#   character._strength_mod       -> GetStrModPergon  / SetStrModPergon
//?   character._hp                 -> GetHPPergon      / SetHPPergon
//#   character._mana               -> GetManaPergon    / SetManaPergon
//#   character._stamina            -> GetStaminaPergon / SetStaminaPergon
//#   character._maxhp              -> GetMaxHPPergon
//#   character._maxmana            -> GetMaxManaPergon
//#   character._maxstamina         -> GetMaxStaminaPergon
//#   character._hp_regen_rate      -> GetHPRegenRatePergon
//#   character._mana_regen_rate    -> GetManaRegenRatePergon
//#   character._stamina_regen_rate -> GetStaminaRegenRatePergon
//#   GetStatus_
//#   GetStats_
//#   weapon.skillid                -> weapon.attribute
//#   ConsumeMana_                  -> ConsumeManaPergon
//
// GlobalProp's
//   "Sk_"+        -> entfallen
//
// CProp's
//   "SkRealSkill" -> entfaellt
//   "SkilliD"     -> aus der Zahl 'n String machen, sowohl in den Scripten, als auch
//                    in Pol/data/*.txt
//
// SkillKonstanten
//   Nach dem regulaeren Ausdruck "const*skill" suchen und fixen
//
// Config's
//   z.B. tinkering:tinker muss angepasst werden
//
// Auf sinnvolle Benutzung prüfen (ggf. durch Konstanten ersetzen)
//   "SkCh_           -> ggf. fixen
//   "SkOld_          -> entfernen
//   "Sk_             -> ggf. fixen
//   MAX_NORMAL_SKILL -> ForEach skillid in GetGlobalProperty(SKNAMENARRAY)
//   MAX_MORE_SKILL   -> ForEach skillid in GetGlobalProperty(SKNAMENARRAY)
//   MAX_SKILLS       -> ForEach skillid in GetGlobalProperty(SKNAMENARRAY)
//
// Nutzung von
//   RawSkillToBaseSkillPergon (gibt ja auch GetSkillFloatPergon)
//   BaseSkillToRawSkillPergon
//
// Fehler
//   UnloadConfigFile*itemdesc -> rausschmeissen ("Entladen verursacht Fehler!")
//
// Problem
//   ItemDesc.cfg - "StrRequired" -> "StrRequiredPlayer"

/////////////////
// Bibliotheken&Includes
/////////////////
use attributes;
use math;
use os;
use polsys;
use util;
use datafile;
include "include/client";
include "include/debug";
include "include/diebesliste";
include "include/gumputil";
include "include/itemnpc";
include "include/math";
include "include/msgs";
include "include/npc";
include "include/pergonutil";
include "include/place";
include "include/server";
include "include/string";
include "include/vitals";

////////////////////////
// Konstanten (Skills)
////////////////////////
Const SKILLID_ALCHEMIE            := "Alchemy";                // 00 SkCh_alchemy
Const SKILLID_ANATOMIE            := "Anatomy";                // 01 SkCh_anatomy
Const SKILLID_TIERKUNDE           := "AnimalLore";             // 02 SkCh_animallore
Const SKILLID_GEGENSTAENDE_IDENT  := "ItemID";                 // 03 SkCh_itemid
Const SKILLID_WAFFENKUNDE         := "ArmsLore";               // 04 SkCh_armslore
Const SKILLID_SCHILDKAMPF         := "Parry";                  // 05 SkCh_parry
Const SKILLID_AXTKAMPF            := "Begging";                // 06 SkCh_begging
Const SKILLID_ERZVERARBEITUNG     := "Blacksmithy";            //*07 SkCh_blacksmithy
Const SKILLID_INFANTERIEWAFFEN    := "Bowcraft";               // 08 SkCh_bowcraft
Const SKILLID_FRIEDENSTIFTEN      := "PeaceMaking";            // 09 SkCh_peacemaking
Const SKILLID_WILDNISKUNDE        := "Camping";                // 10 SkCh_camping
Const SKILLID_HOLZVERARBEITUNG    := "Carpentry";              //*11 SkCh_carpentry
Const SKILLID_KARTOGRAPHIE        := "Cartography";            // 12 SkCh_cartography
Const SKILLID_KOCHEN_BRATEN       := "Cooking";                // 13 SkCh_cooking
Const SKILLID_VERSTECKTES_FINDEN  := "DetectingHidden";        // 14 SkCh_detectinghidden
Const SKILLID_ABRICHTEN           := "Enticement";             // 15 SkCh_enticement
Const SKILLID_SPRUCHZAUBEREI      := "EvaluatingIntelligence"; // 16 SkCh_evaluatingintelligence
Const SKILLID_SEGNEN              := "Healing";                // 17 SkCh_healing
Const SKILLID_ANGELN              := "Fishing";                // 18 SkCh_fishing
Const SKILLID_OBDUKTION           := "ForensicEvaluation";     // 19 SkCh_forensicevaluation
Const SKILLID_TIERZUCHT           := "Herding";                // 20 SkCh_herding
Const SKILLID_VERSTECKEN          := "Hiding";                 // 21 SkCh_hiding
Const SKILLID_PROVOZIEREN         := "Provocation";            // 22 SkCh_provocation
Const SKILLID_MAGISCHEGEGENST     := "Inscription";            // 23 SkCh_inscription
Const SKILLID_SCHLOESSER_KNACK    := "LockPicking";            // 24 SkCh_lockpicking
Const SKILLID_MAGIE               := "Magery";                 // 25 SkCh_magery
Const SKILLID_MAGIERESISTENZ      := "MagicResistance";        // 26 SkCh_magicresistance
Const SKILLID_TAKTIK              := "Tactics";                // 27 SkCh_tactics
Const SKILLID_SCHNUEFFELN         := "Snooping";               // 28 SkCh_snooping
Const SKILLID_MUSIZIEREN          := "Musicianship";           // 29 SkCh_musicianship
Const SKILLID_VERGIFTEN           := "Poisoning";              // 30 SkCh_poisoning
Const SKILLID_BOGENSCHIESSEN      := "Archery";                // 31 SkCh_archery
Const SKILLID_AUSWEICHEN          := "Ausweichen";             // 32 SkCh_ausweichen
Const SKILLID_STEHLEN             := "Stealing";               // 33 SkCh_stealing
Const SKILLID_SCHNEIDERN          := "Tailoring";              //*34 SkCh_tailoring
Const SKILLID_ZAEHMEN             := "AnimalTaming";           // 35 SkCh_animaltaming
Const SKILLID_VORSCHMECKEN        := "TasteIdentification";    // 36 SkCh_tasteidentification
Const SKILLID_BASTELN             := "Tinkering";              //+37 SkCh_tinkering
Const SKILLID_SPUREN_LESEN        := "Tracking";               // 38 SkCh_tracking
Const SKILLID_TIERE_VERARZTEN     := "Veterinary";             // 39 SkCh_veterinary
Const SKILLID_SCHWERTKAMPF        := "SwordsmanShip";          // 40 SkCh_swordsmanship
Const SKILLID_HIEBWAFFEN          := "MaceFighting";           // 41 SkCh_macefighting
Const SKILLID_FECHTKUNST          := "Fencing";                // 42 SkCh_fencing
Const SKILLID_FAUSTKAMPF          := "Wrestling";              // 43 SkCh_wrestling
Const SKILLID_HOLZFAELLEN         := "Lumberjacking";          // 44 SkCh_lumberjacking
Const SKILLID_ERZABBAU            := "Mining";                 //*45 SkCh_mining
Const SKILLID_REGENERATION        := "Meditation";             // 46 SkCh_meditation
Const SKILLID_SCHLEICHEN          := "Stealth";                // 47 SkCh_stealth
Const SKILLID_FALLEN_ENTSCH       := "RemoveTrap";             // 48 SkCh_removetrap
Const SKILLID_WAFFENSCHMIEDEN     := "WaffenSchmied";          //*49 SkCh_waffenschmied
Const SKILLID_WERKZEUGSCHMIEDEN   := "WerkzeugSchmied";        //*50 SkCh_werkzeugschmied
Const SKILLID_KUNST_FEINSCHMIEDEN := "FeinSchmied";            //*51 SkCh_feinschmied
Const SKILLID_GLASVERARBEITUNG    := "GlasVerarb";             //*52 SkCh_glasverarb
Const SKILLID_ARTEFAKTE_AKTIV     := "Zauberrollen";           // 53 SkCh_zauberrollen
Const SKILLID_REZEPTE_SCHREIB     := "Rezepte";                // 54 SkCh_rezepte
Const SKILLID_VERFLUCHTEGEGENST   := "Fluchgegenst";           // 55 SkCh_fluchgegenst
Const SKILLID_GESEGNETEGEGENST    := "Heilmittel";             // 56 SkCh_heilmittel
Const SKILLID_STUDIEREN           := "Studieren";              // 57 SkCh_studieren
Const SKILLID_FLUCHEN             := "Verfluchen";             // 58 SkCh_verfluchen
Const SKILLID_VERZAUBERN          := "Verzaubern";             // 59 SkCh_verzaubern
Const SKILLID_ACKERBAU            := "Ackerbau";               // 60 SkCh_ackerbau
Const SKILLID_FOERSTEREI          := "Foersterei";             // 61 SkCh_foersterei
Const SKILLID_RUTEN_BENUTZEN      := "Ruten";                  // 62 SkCh_ruten
Const SKILLID_WEBEN               := "Weben";                  // 63 SkCh_weben
Const SKILLID_BACKEN              := "Backen";                 // 64 SkCh_backen
Const SKILLID_BRAUEN_KELTERN      := "Brauen";                 // 65 SkCh_brauen
Const SKILLID_MAHLEN              := "Mahlen";                 // 66 SkCh_mahlen
Const SKILLID_FALLEN_LEGEN        := "Fallenstellen";          // 67 SkCh_fallenstellen
Const SKILLID_GERBEN_FAERBEN      := "Gerben";                 // 68 SkCh_gerben
Const SKILLID_HAEUTEN             := "Haeuten";                // 69 SkCh_haeuten
Const SKILLID_LEDEREI_SCHUSTERN   := "Lederei";                //*70 SkCh_lederei
Const SKILLID_SCHLACHTEN          := "Schlachten";             // 71 SkCh_schlachten
Const SKILLID_SCHIFFBAU           := "Schiffbau";              // 72 SkCh_schiffbau

//////////////////////////////////////
// Konstanten (Klassen & Skilltypen)
//////////////////////////////////////
Const KLASSE_HANDWERKER := "Handwerker";
Const KLASSE_KRIEGER    := "Krieger";
Const KLASSE_MAGIER     := "Magier";
Const KLASSE_UNRELEVANT := "Unrelevant";

Const SKILLTYP_BERUF   := "Beruf";
Const SKILLTYP_HAUPT   := "Haupt";
Const SKILLTYP_NEBEN   := "Neben";
Const SKILLTYP_WEITERE := "Weitere";

Const SKILLTYPKENNUNG      := "SkCh_";
Const SKILLWERTKENNUNG     := "Sk_";

Const LAST_SKILL       := "#LastUsedSkill";

Const KLASSE_BERUF_NIX := "None";

Const SKILLLIMITRAW_BERUF      := 8388608;
Const SKILLLIMITRAW_HAUPT      := 262144;
Const SKILLLIMITRAW_NEWBIE_MAX := 262144;
Const SKILLLIMITRAW_NEBEN      := 65536;
Const SKILLLIMITRAW_WEITERE    := 16384;
Const SKILLLIMIT_BERUF         := 130;
Const SKILLLIMIT_HAUPT         := 80;
Const SKILLLIMIT_NEWBIE_MAX    := 80;
Const SKILLLIMIT_NEBEN         := 60;
Const SKILLLIMIT_WEITERE       := 40;

// Berufs- und Hauptskills werden mit je 10% initialisiert
Const LP_BASE_PER_BH_SKILL     := 10;
// von restlichen Lernpunkten kann man pro Skill nochmal bis zu 10% trainieren
Const LP_MAX_PER_SKILL         := 10;
// Limits fuer die Skills
Const LP_LIMIT_BERUFHAUPT      := 40;
Const LP_LIMIT_NEBEN           := 80;
Const LP_LIMIT_WEITERE         := 40;

///////////////////////////////
// Konstanten (ObjProperties)
///////////////////////////////

Const SKKLASSE := "SkKlasse";       // Klasse (siehe KLASSE_* Konstanten)                        / String
Const SKBERUF  := "SkBeruf";        // Beruf  (siehe CharSkills.cfg)                             / String

Const SKERFAHRUNG := "SkErfahrung";    // bisher ungenutzt                                          / Integer
Const SKLERN      := "SkLern";         // bisher ungenutzt                                          / Integer


///////////////
// Konstanten
///////////////

// Pol hat eine Obergrenze bzgl. der Skills.cfg
Const MAX_SKILLS := 72;
// Anzahl benutzter Skills auf Pergon
Const MAX_NORMAL_SKILL := 48;
Const MAX_MORE_SKILL   := MAX_SKILLS+1;

// CheckSkillPergon liefert nur 0 und 1, Wuerfelwert oder die genauen Rawpoints
Const RAWPOINTS      := 2;
Const QUALIFIZIERT   := 1;
Const UNQUALIFIZIERT := 0;

//Soll CheckSkillPergon auch stat points geben
Const ADVANCESTATS     := 1;
Const DONTADVANCESTATS := 0;

//Soll CheckSkillPergon den Skill erhöhen
Const GIVESKILL     := 1;
Const DONTGIVESKILL := 0;

// Ab wieviel neuen RawPunkten wird ein Anstieg angezeigt?
Const SKILLADV_STUFE := 500;

// Default-Divisor = 8 / 50% Punkte bei 50% Chance
Const WAFFENSKILL_PUNKTE := 30; // 30
Const TAKTIK_PUNKTE      := 10; // 15
Const SCHILDKAMPF_PUNKTE := 20; // 20

///////////////////////////////////////////////////////////////////////////
// Konstanten (IGNORE-* fuer Skill-/Berufsachen, sowie Globale Props)
// (siehe z.B. .setp, .erasep, .setglobal, .eraseglobal, .info, .editcprops)
///////////////////////////////////////////////////////////////////////////
Const IGNORE_PROPS :=
  " "+SKKLASSE+" "+SKBERUF+" "+SKERFAHRUNG+" "+SKLERN+" "+
  ATTRIBUTEWERTKENNUNG+ATTRIBUTEID_DEXTERITY+" "+
  ATTRIBUTEWERTKENNUNG+ATTRIBUTEID_INTELLIGENCE+" "+
  ATTRIBUTEWERTKENNUNG+ATTRIBUTEID_STRENGTH+" "+
  "SkRealSkill SkLastGained SkLastGainedMsg profile ct "+
  // Diebesliste macht ab und an Gump zu gross
  // SELF_SUPPORT* enthalten persoenliche Daten
  THEFT_LIST_CPROP+" "; //+SELF_SUPPORT1+" "+SELF_SUPPORT2+" ";
Const IGNORE_PROPS_MODIFIY :=
    IGNORE_PROPS+LOG_LASTHIT+" "+LOG_LASTDAMAGE+" CreatedBy ";
Const IGNORE_TYPES :=
    " "+SKILLTYP_BERUF+" "+SKILLTYP_HAUPT+" "+SKILLTYP_NEBEN+" "+
    SKILLTYP_WEITERE+" ";
Const IGNORE_GLOBAL_PROPS := " "+BAUSERVER+" ";


////////////////////////////
//                        //
//   Skill-Manipulation   //
//                        //
////////////////////////////

////////////////////////////////////////////////
// CheckSkillPergon - Prueft und erhoeht Skill
////////////////////////////////////////////////

Function CheckSkillPergon(mobile, skillid, difficulty, points, qualifiziert := UNQUALIFIZIERT, give_stats := ADVANCESTATS, give_skill := GIVESKILL)
  If (DEBUG_CHECKSKILLMESSAGE)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      syslog("CheckSkillPergon("+skillid+")");
    EndIf
  EndIf

  return (MakeCheckSkillPergon(mobile, skillid, difficulty, points, qualifiziert, give_stats, give_skill));
EndFunction

////////////////////////////////////////////////////
// MakeCheckSkillPergon - Prueft und erhoeht Skill
////////////////////////////////////////////////////

Function MakeCheckSkillPergon(mobile, skillid, difficulty, points, qualifiziert := UNQUALIFIZIERT, give_stats := ADVANCESTATS, give_skill := GIVESKILL)
  // ----------------------------------------
  // Experimentelles Logging für Umstellung zu Double-Berechnung
  // SendSysMessagePergon(mobile, "Test1: "+skillid+": "+points+"/"+difficulty);
  // If ((points <= 4) && !mobile.isa(POLCLASS_NPC))
  //   LogToFile("::log/experiment.log", "Kritische Berechnung: MakeCheckSkillPergon: Skill/Points: "+skillid+"/"+points);
  // EndIf
  // ----------------------------------------
  var ergpoints_org := 0.0;
  var ergpoints_new := 0.0;
  var erg_org := MakeCheckSkillPergon_Org(mobile, skillid, difficulty, points, ergpoints_org, qualifiziert, give_stats, give_skill);
  // setzt die Rawpoints nicht um, berechnet nur
  // Advance Stats auskommentiertt
  MakeCheckSkillPergon_New(mobile, skillid, difficulty, points, ergpoints_new, qualifiziert, give_stats);
  If (abs(ergpoints_org-ergpoints_new) > 1.0)
    LogToFile("::log/experiment.log", "Kritische Berechnung: MakeCheckSkillPergon: Skill/Points/RawpointsOrg/RawpointsNew: "+skillid+"/"+points+"/"+ergpoints_org+"/"+ergpoints_new);
  EndIf
  //----------- Zahlen für Auswertung sichern ----------------
  If (!mobile.isa(POLCLASS_NPC))
    var raw_dict := GetGlobalProperty("RawTestSammler");
    If (!(raw_dict))
      raw_dict := Dictionary;
    EndIf
    If (raw_dict.exists(skillid))
      raw_dict[skillid].org += ergpoints_org;
      raw_dict[skillid].new += ergpoints_new;
      raw_dict[skillid].newround += RoundRand(ergpoints_new);
    Else
      var raw_struct := Struct;
      raw_struct.+org := ergpoints_org;
      raw_struct.+new := ergpoints_new;
      raw_struct.+newround := RoundRand(ergpoints_new);
      raw_dict.insert(skillid, raw_struct);
    EndIf
    SetGlobalProperty("RawTestSammler", raw_dict);
  EndIf
  //----------------------------------------------------------
  
  If (qualifiziert == RAWPOINTS)
    return {erg_org, ergpoints_org};
  EndIf
  
  return erg_org;
EndFunction

Function MakeCheckSkillPergon_New(mobile, skillid, difficulty, points, byref ergpoints, qualifiziert := UNQUALIFIZIERT, give_stats := ADVANCESTATS)
  //alle relevanten Werte in Dbl führen
  points := CDbl(points);
  difficulty := CDbl(difficulty);

  var newpoints := points;
  var skillname:=GetSkillName(skillid);
  var result := 0.0;

  If (DEBUG_CHECKSKILL)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (skillname.errortext)
        syslog("FEHLER: MakeCheckSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
        return(0);
      EndIf
    EndIf

   // If (mobile.isa(POLCLASS_NPC))
   //   If (points)
   //     syslog("WARNUNG: MakeCheckSkillPergon darf von NPCs nicht mit Punkten aufgerufen werden!");
   //   EndIf
   // EndIf
  EndIf

  If (
    (
      GetObjProperty(mobile, SKILLTYPKENNUNG+skillname) And
      (!GetObjProperty(mobile, PROP_PUNISH_JAILTIME))
    ) Or
    (mobile.cmdlevel>=CMDLEVEL_SEER)
  )
    // zuletzt geprueften Skill sichern fuer checkgain etc.
    var last := struct;
    last.+skill := skillid;
    last.+time  := PolCore().systime;
    mobile.setprop(LAST_SKILL, last);

    If (skillname<>SKILLID_BASTELN) // Normaler Skill?
      var divisor := 3.0; // := 4.0;

      Case (skillid)
        SKILLID_HOLZVERARBEITUNG: // 2005/04/05 Shinigami: von 2 auf 1 gesetzt
        SKILLID_ANGELN:           // 2005/04/11 Shinigami: von 2 auf 1 gesetzt
        SKILLID_ABRICHTEN:
        SKILLID_SCHNUEFFELN:
        SKILLID_ZAEHMEN:
        SKILLID_MAGIE:
        SKILLID_FLUCHEN:
        SKILLID_ARTEFAKTE_AKTIV:
        SKILLID_SEGNEN:            divisor := 1.0;

        SKILLID_VERSTECKEN:          // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_SCHLEICHEN:          // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_ERZVERARBEITUNG:     // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_WAFFENSCHMIEDEN:     // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_WERKZEUGSCHMIEDEN:   // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_KUNST_FEINSCHMIEDEN: // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_GLASVERARBEITUNG:    // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_GERBEN_FAERBEN:      // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_LEDEREI_SCHUSTERN:   // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_ALCHEMIE:
        SKILLID_KOCHEN_BRATEN:
        SKILLID_STEHLEN:           divisor := 2.0;

        SKILLID_BOGENSCHIESSEN:    divisor := 4.0;

        SKILLID_SCHILDKAMPF:
        SKILLID_AXTKAMPF:
        SKILLID_INFANTERIEWAFFEN:
        SKILLID_TAKTIK:
       // SKILLID_BOGENSCHIESSEN:
        SKILLID_SCHWERTKAMPF:
        SKILLID_HIEBWAFFEN:
        SKILLID_FECHTKUNST:
        SKILLID_FAUSTKAMPF:        divisor := 8.0;

        SKILLID_SPRUCHZAUBEREI:    divisor := 9.0;
      EndCase

      newpoints := points/divisor;
      If ((!newpoints) And (points))
        newpoints := 1.0;
      EndIf

      //If ((skillid == SKILLID_SCHILDKAMPF) || (skillid == SKILLID_TAKTIK))
      //  SendSysMessagePergon(mobile, "skill = "+ skillid +" newpoints = " + newpoints);
      //EndIf
    Else // Nein, Basteln...
      If (DEBUG_CHECKSKILL)
        If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
          If (points)
            syslog("WARNUNG: Skill Basteln wurde in MakeCheckSkillPergon mit Punkten ("+points+") aufgerufen!");
          EndIf
        EndIf
      EndIf

      newpoints := 0.0; // Basteln steigt nicht...
    EndIf
    var skillinfo:=GetSkillInfo(skillname);
    If (skillinfo.unhides)
      If (mobile.hidden) // Um unnoetige Events zu sparen...
        mobile.hidden := 0;
      EndIf
    EndIf

    result := CalcCheckSkillPergon_New(mobile, skillname, difficulty, newpoints, ergpoints);

    If (result)
      //Erhöhung der Statswerte zum erhöhten Skill
      var IsKorr := 1.0;
      var KorrFaktor := 1.0;
      Case (skillname)
        SKILLID_MAGIE:
        SKILLID_FLUCHEN:
        SKILLID_SEGNEN: KorrFaktor := CalculateMagicKorrFaktor(mobile, difficulty);
        SKILLID_SPRUCHZAUBEREI: KorrFaktor := CalculateMagicKorrFaktor(mobile, difficulty, 3.0/2.0);

        SKILLID_BOGENSCHIESSEN:
        SKILLID_AXTKAMPF:
        SKILLID_INFANTERIEWAFFEN:
        SKILLID_SCHWERTKAMPF:
        SKILLID_HIEBWAFFEN:
        SKILLID_FECHTKUNST: KorrFaktor := CalculateCombatKorrFaktor(mobile, WAFFENSKILL_PUNKTE, points);
        SKILLID_TAKTIK: KorrFaktor := CalculateCombatKorrFaktor(mobile, TAKTIK_PUNKTE, points);
        SKILLID_ANATOMIE: KorrFaktor := CalculateCombatKorrFaktor(mobile, GetSkillDefaultPoints(SKILLID_ANATOMIE), points, 2.0);
        Default: IsKorr := 0.0;
      EndCase

      If (DEBUG_CHECKSKILL&&IsKorr)
        If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
          SendSysMessagePergon(mobile, "KorrFaktor fuer Stats: "+KorrFaktor, "", _DEFAULT_TEXT_FONT, 38);
        EndIf
      EndIf
      ///!!! wieder reaktivieren wenns startet AdvanceStatsPergon(mobile, skillid, KorrFaktor); // Stats ggf. erhoehen
    EndIf

  Else // NPCs, nicht aktivierte Player-Skills oder Knastologe
    var skillinfo:=GetSkillInfo(skillname);
    If (skillinfo.unhides)
      If (mobile.hidden) // Um unnoetige Events zu sparen...
        mobile.hidden := 0;
      EndIf
    EndIf

    result := CalcCheckSkillPergon_New(mobile, skillid, difficulty, 0.0, ergpoints);
  EndIf

  If (DEBUG_CHECKSKILL)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (DEBUG_RESULTS)
        syslog("MakeCheckSkillPergon("+mobile.name+", "+skillname+", "+difficulty+", "+points+"["+newpoints+"]) = "+result);
      EndIf
    EndIf
  EndIf

  If (qualifiziert == UNQUALIFIZIERT) // Wert unqualifiziert zurueckgeben?
    If (result>0)
      result := 1.0;
    EndIf
  EndIf

  return (result);
  give_stats:=give_stats; // compiler warnung unterdrücke
EndFunction

Function MakeCheckSkillPergon_Org(mobile, skillid, difficulty, points, byref ergpoints, qualifiziert := UNQUALIFIZIERT, give_stats := ADVANCESTATS, give_skill := GIVESKILL)
  var newpoints := points;
  var skillname:=GetSkillName(skillid);
  var result := 0;

 // If (skillid=SKILLID_WAFFENSCHMIEDEN) // Debugging
 //   var oldcritical := Set_Critical(1);
 //
 //   var maxpoints := GetGlobalProperty("CheckSkillMaxPoints");
 //   If (!maxpoints)
 //     maxpoints := 0;
 //   EndIf
 //
 //   If (maxpoints<points)
 //     maxpoints := points;
 //   EndIf
 //
 //   SetGlobalProperty("CheckSkillMaxPoints", maxpoints);
 //
 //   Set_Critical(oldcritical);
 //
 //   syslog("WaffenPunkte '"+mobile.name+"' "+difficulty+" "+points+" "+maxpoints);
 // EndIf

  If (DEBUG_CHECKSKILL)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (skillname.errortext)
        syslog("FEHLER: MakeCheckSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
        return (0);
      EndIf
    EndIf

   // If (mobile.isa(POLCLASS_NPC))
   //   If (points)
   //     syslog("WARNUNG: MakeCheckSkillPergon darf von NPCs nicht mit Punkten aufgerufen werden!");
   //   EndIf
   // EndIf
  EndIf

  If (
    (
      GetObjProperty(mobile, SKILLTYPKENNUNG+skillname) And
      (!GetObjProperty(mobile, PROP_PUNISH_JAILTIME))
    ) Or
    (mobile.cmdlevel>=CMDLEVEL_SEER)
  )
    If (skillname<>SKILLID_BASTELN) // Normaler Skill?
      var divisor := 3; // := 4;

      Case (skillname)
        SKILLID_HOLZVERARBEITUNG:              // 2005/04/05 Shinigami: von 2 auf 1 gesetzt
        SKILLID_ANGELN:                        // 2005/04/11 Shinigami: von 2 auf 1 gesetzt
        SKILLID_ABRICHTEN:
        SKILLID_SCHNUEFFELN:
        SKILLID_ZAEHMEN:
        SKILLID_MAGIE:
        SKILLID_FLUCHEN:
        SKILLID_ARTEFAKTE_AKTIV:
        SKILLID_SEGNEN:            
        SKILLID_MUSIZIEREN:         divisor := 1;

        SKILLID_VERSTECKEN:                    // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_SCHLEICHEN:                    // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_ERZVERARBEITUNG:               // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_WAFFENSCHMIEDEN:               // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_WERKZEUGSCHMIEDEN:             // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_KUNST_FEINSCHMIEDEN:           // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_GLASVERARBEITUNG:              // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_GERBEN_FAERBEN:                // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_LEDEREI_SCHUSTERN:             // 2005/04/05 Shinigami: von 3 auf 2 gesetzt
        SKILLID_ALCHEMIE:
        SKILLID_KOCHEN_BRATEN:
        SKILLID_STEHLEN:           divisor := 2;

        SKILLID_BOGENSCHIESSEN:    divisor := 4;

        SKILLID_SCHILDKAMPF:
        SKILLID_AXTKAMPF:
        SKILLID_INFANTERIEWAFFEN:
        SKILLID_TAKTIK:
       // SKILLID_BOGENSCHIESSEN:
        SKILLID_SCHWERTKAMPF:
        SKILLID_HIEBWAFFEN:
        SKILLID_FECHTKUNST:
        SKILLID_FAUSTKAMPF:        divisor := 8;

        SKILLID_SPRUCHZAUBEREI:    divisor := 9;
      EndCase

      newpoints := CInt(points/divisor);
      If ((!newpoints) And (points))
        newpoints := 1;
      EndIf
      //If ((skillid == SKILLID_SCHILDKAMPF) || (skillid == SKILLID_TAKTIK))
      //  SendSysMessagePergon(mobile, "skill = "+ skillid +" newpoints = " + newpoints);
      //EndIf
    Else // Nein, Basteln...
      If (DEBUG_CHECKSKILL)
        If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
          If (points)
            syslog("WARNUNG: Skill Basteln wurde in MakeCheckSkillPergon mit Punkten ("+points+") aufgerufen!");
          EndIf
        EndIf
      EndIf

      newpoints := 0; // Basteln steigt nicht...
    EndIf

    var skillinfo:=GetSkillInfo(skillname);
    If (skillinfo.unhides)
      If (mobile.hidden) // Um unnoetige Events zu sparen...
        mobile.hidden := 0;
      EndIf
    EndIf

    result := CalcCheckSkillPergon_Org(mobile, skillname, difficulty, newpoints, ergpoints, give_skill);
    If (result)
      var IsKorr := 1;
      var KorrFaktor := 1.0;
      Case (skillname)
        SKILLID_MAGIE:
        SKILLID_FLUCHEN:
        SKILLID_SEGNEN: KorrFaktor := CalculateMagicKorrFaktor(mobile, difficulty);
        SKILLID_SPRUCHZAUBEREI: KorrFaktor := CalculateMagicKorrFaktor(mobile, difficulty, 3.0/2.0);

        SKILLID_BOGENSCHIESSEN:
        SKILLID_AXTKAMPF:
        SKILLID_INFANTERIEWAFFEN:
        SKILLID_SCHWERTKAMPF:
        SKILLID_HIEBWAFFEN:
        SKILLID_FECHTKUNST: KorrFaktor := CalculateCombatKorrFaktor(mobile, WAFFENSKILL_PUNKTE, points);
        SKILLID_TAKTIK: KorrFaktor := CalculateCombatKorrFaktor(mobile, TAKTIK_PUNKTE, points);
        SKILLID_ANATOMIE: KorrFaktor := CalculateCombatKorrFaktor(mobile, GetSkillDefaultPoints(SKILLID_ANATOMIE), points, 2.0);
        Default: IsKorr := 0;
      EndCase

      If (DEBUG_CHECKSKILL&&IsKorr)
        If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
          SendSysMessagePergon(mobile, "KorrFaktor fuer Stats: "+KorrFaktor, "", _DEFAULT_TEXT_FONT, 38);
        EndIf
      EndIf
      If (give_stats)
        AdvanceStatsPergon(mobile, skillname, KorrFaktor); // Stats ggf. erhoehen
      EndIf
    EndIf

  Else // NPCs, nicht aktivierte Player-Skills oder Knastologe
    var skillinfo:=GetSkillInfo(skillname);
    If (skillinfo.unhides)
      If (mobile.hidden) // Um unnoetige Events zu sparen...
        mobile.hidden := 0;
      EndIf
    EndIf

    result := CalcCheckSkillPergon_Org(mobile, skillname, difficulty, 0, ergpoints, give_skill);
  EndIf

  If (DEBUG_CHECKSKILL)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (DEBUG_RESULTS)
        syslog("MakeCheckSkillPergon("+mobile.name+", "+skillname+", "+difficulty+", "+points+"["+newpoints+"]) = "+result);
      EndIf
    EndIf
  EndIf

  If (qualifiziert == UNQUALIFIZIERT) // Wert unqualifiziert zurueckgeben?
    If (result)
      result := 1;
    EndIf
  EndIf

  return (result);
EndFunction

/////////////////////////////////////////////////////////////////////
// CalcCheckSkillPergon - Berechnet und realisiert den Skillanstieg
/////////////////////////////////////////////////////////////////////

Function CalcCheckSkillPergon_New(mobile, skillid, difficulty, points, byref ergpoints)
  //alle relevanten Werte in Dbl führen
  var skillvalue := CDbl(GetSkillPergon(mobile, skillid));
  points := CDbl(points);
  difficulty := CDbl(difficulty);
  ergpoints := 0.0;

  var chance := CDbl(0);;
  var addpoints := CDbl(0);


  If (difficulty>=0)
    chance := Min(1.0, 0.025*(skillvalue-difficulty)+0.5);
    addpoints := (1.0-chance)*points;
  Else
    chance := Min(1.0, 0.01*skillvalue);
    addpoints := points;
  EndIf

  // Debug
  If (addpoints<0)
    syslog("Warnung!!! SkillDecrease - CalcCheckSkillPergon("+mobile.name+", "+skillid+", "+difficulty+", "+points+") -> "+skillvalue+" / "+chance+" / "+addpoints);
  EndIf

  If (chance>0)
    var randvalue := RandomInt(1000);

    If (skillvalue <= 10.0)
      If (addpoints)
        ergpoints := addpoints;
        AwardRawPointsPergon_New(mobile, skillid, addpoints);

        If (DEBUG_CHECKSKILL)
          If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
            SendSysMessagePergon(mobile, skillid+" = "+GetSkillPergon(mobile, skillid)+"/"+
              GetRawSkillPergon(mobile, skillid)+"/"+addpoints, "", _DEFAULT_TEXT_FONT, 36);
          EndIf
        EndIf
      EndIf

      If (CInt(chance*1000)>=randvalue)
        return (randvalue+1);
      Else
        return (0);
      EndIf
    Else
      ergpoints := addpoints;
      //SendSysMessagePergon(mobile, "Test3: "+skillid+": "+ergpoints);
      If (CInt(chance*1000)>=randvalue)
        If (addpoints)
          AwardRawPointsPergon_New(mobile, skillid, addpoints);

          If (DEBUG_CHECKSKILL)
            If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
              SendSysMessagePergon(mobile, skillid+" = "+GetSkillPergon(mobile, skillid)+"/"+
                GetRawSkillPergon(mobile, skillid)+"/"+addpoints, "", _DEFAULT_TEXT_FONT, 36);
            EndIf
          EndIf
        EndIf

        return (randvalue+1);
      Else
        return (0);
      EndIf
    EndIf
  Else
    return (0);
  EndIf
EndFunction

Function CalcCheckSkillPergon_Org(mobile, skillid, difficulty, points, byref ergpoints, give_skill := GIVESKILL)
  var skillvalue := GetSkillPergon(mobile, skillid);
  var chance;
  var addpoints;
  ergpoints := 0.0;

  If (difficulty>=0)
    chance := Min(1, 0.025*(skillvalue-difficulty)+0.5);
    addpoints := CInt((1-chance)*points);
  Else
    chance := Min(1, 0.01*skillvalue);
    addpoints := CInt(points);
  EndIf

  // Debug
  If (addpoints<0)
    syslog("Warnung!!! SkillDecrease - CalcCheckSkillPergon("+mobile.name+", "+skillid+", "+difficulty+", "+points+") -> "+skillvalue+" / "+chance+" / "+addpoints);
  EndIf

  If (chance>0)
    var randvalue := RandomInt(1000);

    If (skillvalue<=10)
      If (addpoints)
        ergpoints := addpoints;
        If (give_skill)
          AwardRawPointsPergon(mobile, skillid, addpoints);
        EndIf

        If (DEBUG_CHECKSKILL)
          If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
            SendSysMessagePergon(mobile, skillid+" = "+GetSkillPergon(mobile, skillid)+"/"+
              GetRawSkillPergon(mobile, skillid)+"/"+addpoints, "", _DEFAULT_TEXT_FONT, 36);
          EndIf
        EndIf
      EndIf

      If (CInt(chance*1000)>=randvalue)
        return (randvalue+1);
      Else
        return (0);
      EndIf
    Else
      ergpoints := addpoints;
      If (CInt(chance*1000)>=randvalue)
        If (addpoints)
          If (give_skill)
            AwardRawPointsPergon(mobile, skillid, addpoints);
          EndIf

          If (DEBUG_CHECKSKILL)
            If ((mobile.cmdlevel >= CMDLEVEL_SEER) && DEBUG_CHECKSKILLMESSAGE)
              SendSysMessagePergon(mobile, skillid+" = "+GetSkillPergon(mobile, skillid)+"/"+
                GetRawSkillPergon(mobile, skillid)+"/"+addpoints, "", _DEFAULT_TEXT_FONT, 36);
            EndIf
          EndIf
        EndIf

        return (randvalue+1);
      Else
        return (0);
      EndIf
    EndIf
  Else
    return (0);
  EndIf
EndFunction

///////////////////////////////////////////
// GetSkillPergon - Liefert den Skillwert
///////////////////////////////////////////

Function GetSkillPergon(mobile, skillid) // result = 0..9000
  var skillname:=GetSkillName(skillid);
  If (DEBUG_SKILLS)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (skillname.errortext)
        syslog("FEHLER: GetSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
        return(0);
      Else
        If (DEBUG_RESULTS)
          syslog("GetSkillPergon("+mobile.name+", "+skillname+") = "+GetAttribute(mobile, skillname));
        EndIf
      EndIf
    EndIf
  EndIf

  return (GetAttribute(mobile, skillname));
EndFunction

///////////////////////////////////////////////////////
// GetSkillFloatPergon - Liefert den wahren Skillwert
///////////////////////////////////////////////////////

Function GetSkillFloatPergon(mobile, skillid) // result = 0,0..9000,0
  var skillname:=GetSkillName(skillid);
  If (DEBUG_SKILLS)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (skillname.errortext)
        syslog("FEHLER: GetSkillFloatPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
        return(0);
      Else
        If (DEBUG_RESULTS)
           syslog("GetSkillFloatPergon("+mobile.name+", "+skillname+") = "+
              CDbl(GetAttributeBaseValue(mobile, skillname)+GetAttributeTemporaryMod(mobile, skillname))/10);
        EndIf
      EndIf
    EndIf
  EndIf

  return (CDbl(GetAttributeBaseValue(mobile, skillname)+GetAttributeTemporaryMod(mobile, skillname))/10);
EndFunction

/////////////////////////////////////////
// SetSkillPergon - Setzt den Skillwert
/////////////////////////////////////////

Function SetSkillPergon(mobile, skillid, value) // value = 0,0..6000,0 (Player 0,0..209,0)
  var skillname:=GetSkillName(skillid);

  If (DEBUG_SKILLS)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (skillname.errortext)
        syslog("FEHLER: SetSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
        return(0);
      EndIf
    EndIf
  EndIf
  
  If (skillname<>SKILLID_BASTELN) // Normaler Skill?
    If (!mobile.isa(POLCLASS_NPC)) // Player
      value:=Max(0,Min(209,value));

      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skills gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+skillname))
          SKILLTYP_BERUF:   value:=Min(SKILLLIMIT_BERUF, value);
          SKILLTYP_HAUPT:   value:=Min(SKILLLIMIT_HAUPT, value);
          SKILLTYP_NEBEN:   value:=Min(SKILLLIMIT_NEBEN, value);
          SKILLTYP_WEITERE: value:=Min(SKILLLIMIT_WEITERE, value);

          default:          value := 0;
        EndCase
      EndIf

      SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, BaseSkillToRawSkillPergon(CInt(value*10)));
    Else // NPC
      value:=Max(0,Min(6000, value));
    EndIf
  Else // Nein, Basteln...
    If (!mobile.isa(POLCLASS_NPC)) // Player
      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skill gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+SKILLID_BASTELN))
          SKILLTYP_BERUF:   value := SKILLLIMIT_BERUF;
          SKILLTYP_HAUPT:   value := SKILLLIMIT_HAUPT;
          SKILLTYP_NEBEN:   value := SKILLLIMIT_NEBEN;
          SKILLTYP_WEITERE: value := SKILLLIMIT_WEITERE;

          default:          value := 0;
        EndCase
      EndIf
    Else
      value := 0; // NPC braucht kein Basteln...
    EndIf
  EndIf

  If (DEBUG_SKILLS)
    If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
      If (DEBUG_RESULTS)
        syslog("SetSkillPergon("+mobile.name+", "+skillname+", "+value+")");
      EndIf
    EndIf
  EndIf

  var ret := SetAttributeBaseValue(mobile, skillname, CInt(value*10));
  return(ret);
EndFunction

//////////////////////////////////////////////////
// GetRawSkillPergon - Liefert den Raw Skillwert
//////////////////////////////////////////////////

Function GetRawSkillPergon(mobile, skillid) // result = 0..0x7fffffff
  var skillname:=GetSkillName(skillid);

  If (!mobile.isa(POLCLASS_NPC)) // Player
    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (skillname.errortext)
          syslog("FEHLER: GetRawSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
          return(0);
        EndIf
      EndIf
    EndIf

    var rawvalue := GetObjProperty(mobile, SKILLWERTKENNUNG+skillname);
    If (!rawvalue)
      var value := Min(2090, GetAttributeBaseValue(mobile, skillname));

      rawvalue := BaseSkillToRawSkillPergon(value);
      SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, rawvalue);
    EndIf

    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (DEBUG_RESULTS)
          syslog("GetRawSkillPergon("+mobile.name+", "+skillname+") = "+rawvalue);
        EndIf
      EndIf
    EndIf

    return (rawvalue);
  Else // NPC
    If (DEBUG_SKILLS)
      If (((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC) And DEBUG_NPCWARNUNG)
        syslog("WARNUNG: NPCs duerfen GetRawSkillPergon nicht benutzen!");
      EndIf
    EndIf

    return (0);
  EndIf
EndFunction

////////////////////////////////////////////////
// SetRawSkillPergon - Setzt den Raw Skillwert
////////////////////////////////////////////////

Function SetRawSkillPergon(mobile, skillid, rawvalue) // rawvalue = 0..0x7fffffff
  var skillname:=GetSkillName(skillid);

  If (!mobile.isa(POLCLASS_NPC)) // Player
    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (skillname.errortext)
          syslog("FEHLER: SetRawSkillPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
          return(0);
        EndIf
      EndIf
    EndIf

    If (skillname<>SKILLID_BASTELN) // Normaler Skill?
      rawvalue := Max(0,Min(SKILLLIMITRAW_BERUF, CInt(rawvalue)));

      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skills gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+skillname))
          SKILLTYP_BERUF:   rawvalue:=Min(SKILLLIMITRAW_BERUF,rawvalue);
          SKILLTYP_HAUPT:   rawvalue:=Min(SKILLLIMITRAW_HAUPT,rawvalue);
          SKILLTYP_NEBEN:   rawvalue:=Min(SKILLLIMITRAW_NEBEN,rawvalue);
          SKILLTYP_WEITERE: rawvalue:=Min(SKILLLIMITRAW_WEITERE,rawvalue);

          default:          rawvalue := 0;
        EndCase
      EndIf
    Else // Nein, Basteln...
      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skill gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+SKILLID_BASTELN))
          SKILLTYP_BERUF:   rawvalue := SKILLLIMITRAW_BERUF;
          SKILLTYP_HAUPT:   rawvalue := SKILLLIMITRAW_HAUPT;
          SKILLTYP_NEBEN:   rawvalue := SKILLLIMITRAW_NEBEN;
          SKILLTYP_WEITERE: rawvalue := SKILLLIMITRAW_WEITERE;

          default:          rawvalue := 0;
        EndCase
      EndIf
    EndIf

    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (DEBUG_RESULTS)
          syslog("SetRawSkillPergon("+mobile.name+", "+skillname+", "+rawvalue+")");
        EndIf
      EndIf
    EndIf

    SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, rawvalue);
    return (SetAttributeBaseValue(mobile, skillname, CInt(RawSkillToBaseSkillPergon(rawvalue))));
  Else // NPC
    If (DEBUG_SKILLS)
      If (((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC) And DEBUG_NPCWARNUNG)
        syslog("WARNUNG: NPCs duerfen SetRawSkillPergon nicht benutzen!");
      EndIf
    EndIf

    return (0);
  EndIf
EndFunction

/////////////////////////////////////////////////////
// AwardRawPointsPergon - Erhoeht den Raw Skillwert
/////////////////////////////////////////////////////
Function AwardRawPointsPergon_New(mobile, skillid, rawpoints) // rawpoints = -0x7fffffff..0x7fffffff
  mobile := mobile;
  skillid := skillid;
  rawpoints := rawpoints;
  //SendSysMessagePergon(mobile, "Award New: "+ CDbl(rawpoints));
  //rawpoints := RoundRand(rawpoints);
  //return AwardRawPointsPergon(mobile, skillid, rawpoints);
EndFunction

Function AwardRawPointsPergon(mobile, skillid, rawpoints) // rawpoints = -0x7fffffff..0x7fffffff
  var skillname:=GetSkillName(skillid);
  //SendSysMessagePergon(mobile, "Award Org: "+rawpoints);
  If (!mobile.isa(POLCLASS_NPC)) // Player
    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (skillname.errortext)
          syslog("FEHLER: AwardRawPointsPergon wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
          return(0);
        EndIf
      EndIf
    EndIf

    If (PlaceNoSkill(mobile)) // Knastologe bekommt keine Punkte
      rawpoints := 0;
    EndIf

    var rawvalue;
    var res;
    If (skillname<>SKILLID_BASTELN) // Normaler Skill?
      var old_crit := Is_Critical();
      Set_Critical(1);
      var oldrawvalue := GetObjProperty(mobile, SKILLWERTKENNUNG+skillname);
      If (!oldrawvalue)
        var value := Min(2090, GetAttributeBaseValue(mobile, skillname));

        oldrawvalue := BaseSkillToRawSkillPergon(value);
      EndIf

      If (GetObjProperty(mobile, TYPNEWBIE)) // Newbies koennen ihre Skills nicht austrainieren
        If (oldrawvalue>SKILLLIMITRAW_NEWBIE_MAX)
          rawpoints := 0;
        EndIf
      EndIf

      // Debug
      If (CInt(rawpoints)<0)
        syslog("Warnung!!! SkillDecrease - ("+mobile.name+", "+skillname+", "+rawpoints+")");
      EndIf

      rawvalue := Max(0,Min(SKILLLIMITRAW_BERUF, oldrawvalue+CInt(rawpoints)));

      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skills gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+skillname))
          SKILLTYP_BERUF:   rawvalue:=Min(SKILLLIMITRAW_BERUF, rawvalue);
          SKILLTYP_HAUPT:   rawvalue:=Min(SKILLLIMITRAW_HAUPT, rawvalue);
          SKILLTYP_NEBEN:   rawvalue:=Min(SKILLLIMITRAW_NEBEN, rawvalue);
          SKILLTYP_WEITERE: rawvalue:=Min(SKILLLIMITRAW_WEITERE, rawvalue);

          default:          rawvalue := 0;
        EndCase
      EndIf
      SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, rawvalue);
      res:=SetAttributeBaseValue(mobile, skillname, CInt(RawSkillToBaseSkillPergon(rawvalue)));
      Set_Critical(old_crit);

      // Skillanstiegsmeldung anzeigen
      If (CInt(oldrawvalue/SKILLADV_STUFE)<CInt(rawvalue/SKILLADV_STUFE))
        ShowSkillAdvMsgPergon(mobile, skillname);
      EndIf
    Else // Nein, Basteln...
      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skill gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+SKILLID_BASTELN))
          SKILLTYP_BERUF:   rawvalue := SKILLLIMITRAW_BERUF;
          SKILLTYP_HAUPT:   rawvalue := SKILLLIMITRAW_HAUPT;
          SKILLTYP_NEBEN:   rawvalue := SKILLLIMITRAW_NEBEN;
          SKILLTYP_WEITERE: rawvalue := SKILLLIMITRAW_WEITERE;

          default:          rawvalue := 0;
        EndCase
      EndIf
      var old_crit := Is_Critical();
      Set_Critical(1);
      SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, rawvalue);
      res:=SetAttributeBaseValue(mobile, skillname, CInt(RawSkillToBaseSkillPergon(rawvalue)));
      Set_Critical(old_crit);
    EndIf

    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (DEBUG_RESULTS)
          syslog("AwardRawPointsPergon("+mobile.name+", "+skillname+", "+rawvalue+")");
        EndIf
      EndIf
    EndIf

    Return (res);
  Else // NPC
    If (DEBUG_SKILLS)
      If (((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC) And DEBUG_NPCWARNUNG)
        syslog("WARNUNG: NPCs duerfen AwardRawPointsPergon nicht benutzen!");
      EndIf
    EndIf

    return (0);
  EndIf
EndFunction

////////////////////////////////////////////////////////////////////////////////////
// CheckSkillLimitAndCut - Prueft das SkillLimit und schnippelt ggf. gemaess Stufe
////////////////////////////////////////////////////////////////////////////////////

Function CheckSkillLimitAndCut(mobile, skillid)
  var skillname:=GetSkillName(skillid);

  If (!mobile.isa(POLCLASS_NPC)) // Player
    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (skillname.errortext)
          syslog("FEHLER: CheckSkillLimitAndCut wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
          return(0);
        EndIf
      EndIf
    EndIf

    var rawvalue;
    If (skillname<>SKILLID_BASTELN) // Normaler Skill?
      rawvalue := GetObjProperty(mobile, SKILLWERTKENNUNG+skillname);
      If (!rawvalue)
        var value := Min(2090,GetAttributeBaseValue(mobile, skillname));

        rawvalue := BaseSkillToRawSkillPergon(value);
      EndIf

      If (rawvalue<BaseSkillToRawSkillPergon(10)) // Der mag keine Skills unter 1%
        rawvalue := BaseSkillToRawSkillPergon(RandomInt(40)+11); // Zufaellig von 1 bis 5% setzen
      EndIf

      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skills gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+skillname))
          SKILLTYP_BERUF:   rawvalue:=Min(SKILLLIMITRAW_BERUF, rawvalue);
          SKILLTYP_HAUPT:   rawvalue:=Min(SKILLLIMITRAW_HAUPT, rawvalue);
          SKILLTYP_NEBEN:   rawvalue:=Min(SKILLLIMITRAW_NEBEN, rawvalue);
          SKILLTYP_WEITERE: rawvalue:=Min(SKILLLIMITRAW_WEITERE, rawvalue);

          default:          rawvalue := 0;
        EndCase
      EndIf
    Else // Nein, Basteln...
      If (mobile.cmdlevel < CMDLEVEL_SEER) // Skill gemaess Stufe abschnippeln
        Case (GetObjProperty(mobile, SKILLTYPKENNUNG+skillname))
          SKILLTYP_BERUF:   rawvalue := SKILLLIMITRAW_BERUF;
          SKILLTYP_HAUPT:   rawvalue := SKILLLIMITRAW_HAUPT;
          SKILLTYP_NEBEN:   rawvalue := SKILLLIMITRAW_NEBEN;
          SKILLTYP_WEITERE: rawvalue := SKILLLIMITRAW_WEITERE;

          default:          rawvalue := 0;
        EndCase
      EndIf
    EndIf

    If (DEBUG_SKILLS)
      If ((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC)
        If (DEBUG_RESULTS)
          syslog("CheckSkillLimitAndCut("+mobile.name+", "+skillname+")");
        EndIf
      EndIf
    EndIf

    SetObjProperty(mobile, SKILLWERTKENNUNG+skillname, rawvalue);
    return (SetAttributeBaseValue(mobile, skillname, CInt(RawSkillToBaseSkillPergon(rawvalue))));
  Else // NPC
    If (DEBUG_SKILLS)
      If (((!mobile.isa(POLCLASS_NPC)) Or DEBUG_NPC) And DEBUG_NPCWARNUNG)
        syslog("WARNUNG: NPCs duerfen CheckSkillLimitAndCut nicht benutzen!");
      EndIf
    EndIf

    return (0);
  EndIf
EndFunction

//////////////////////////////////////////////////////////////
// RawSkillToBaseSkillPergon - Wandelt die Punkte in %'te um
//////////////////////////////////////////////////////////////

Function RawSkillToBaseSkillPergon(rawvalue)
  return (RawSkillToBaseSkill(rawvalue));
EndFunction

//////////////////////////////////////////////////////////////
// BaseSkillToRawSkillPergon - Wandelt die %'te in Punkte um
//////////////////////////////////////////////////////////////

Function BaseSkillToRawSkillPergon(basevalue)
  return (BaseSkillToRawSkill(basevalue));
EndFunction

//////////////////////////////////////////////////////////
// ShowSkillAdvMsgPergon - Skillanstiegsmeldung anzeigen
//////////////////////////////////////////////////////////

Function ShowSkillAdvMsgPergon(mobile, skillid)
  var skillinfo:=GetSkillInfo(skillid);
  SendSysMessagePergon(mobile,
    "Euer Skill in "+skillinfo.TrainName+" hat sich verbessert.",
    "Your "+skillinfo.TrainName+" skill improved.",
    _DEFAULT_TEXT_FONT, FONTCOLOR_BLUE
  );
EndFunction

///////////////////////////////////////////////////
// HasPlayerSkill - Hat der Spieler diesen Skill?
///////////////////////////////////////////////////

Function HasPlayerSkill(who, skillid)
  return (GetObjProperty(who, SKILLTYPKENNUNG+skillid));
EndFunction

////////////////////////////////////////////////////
// AlterSkillModPergon - Ändert einen Skill-Modifikator
////////////////////////////////////////////////////
Function AlterSkillModPergon(mobile, skill, value) // value = -3000,0..3000,0
  return (AlterAttributModPergon(mobile, skill, value));
EndFunction

// ermittelt Skill-Mittelwert ueber die Skilltypen im wanted-Array
Function GetSkillAverage(who, wanted := "foo") // {{{
    // kaputter Hack, weil Compiler keine Array-Konstanten mag
    If (TypeOfInt(wanted) != OT_ARRAY)
        wanted := array{SKILLTYP_BERUF};
    EndIf

    var count := 0;
    var sum   := 0;
    // alle Skills durchgehen
    For idx := 0 to MAX_SKILLS
        SleepMS(2);
        // Typ ermitteln
        var skillname := GetSkillName(idx);
        var has := HasPlayerSkill(who, skillname);
        If (has and (has in wanted))
            // Typ passt, mitzaehlen und Skillwert aufsummieren
            sum     += GetSkillPergon(who, skillname);
            count   += 1;
        EndIf
    EndFor

    // Mittelwert der Skills bilden
    return sum/count;
EndFunction // }}}

// Ueberblick ueber die Skills von who besorgen, Formate:
// dictionary{ skill_id := struct{}
Const SKILL_OVER_BY_ID   := 0; // leichter Zugriff per skill_id
// dictionary{ type := array{struct{}}
Const SKILL_OVER_BY_TYPE := 1; // leichter Zugriff ueber Skilltyp
// mit jeweils struct{name, trainname, type, skill_id}
Function GetSkillOverview(who, format := SKILL_OVER_BY_ID) // {{{
    var skills := dictionary{};
    // alle Skills durchgehen
    For skill_id := 0 to MAX_SKILLS
        SleepMS(2);
        // Daten zum Skill ermitteln
        var info := GetSkillInfo(skill_id);
        var data := struct;
        data.name      := GetSkillName(skill_id);
        data.type      := HasPlayerSkill(who, data.name);
        If (!data.type)
            // den Skill hat er nicht
            continue;
        EndIf
        data.value     := GetSkillPergon(who, skill_id);
        data.trainname := info.trainname;
        data.skill_id  := skill_id;

        // entsprechend Format gruppieren
        If (format == SKILL_OVER_BY_ID)
            skills.insert(skill_id, data);
        Else
            If (!skills[data.type])
                // neues Array anlegen
                skills.insert(data.type, array{data});
            Else
                // Array erweitern
                skills[data.type].append(data);
            EndIf
        EndIf
    EndFor

    // Mittelwert der Skills bilden
    return skills;
EndFunction // }}}

/////////////////////
// Initialisierung //
/////////////////////

////////////////////////////////////////////////////////////
// InitSkillsMore - Initialisiert die zusaetzlichen Skills
////////////////////////////////////////////////////////////
Function InitSkillsMore(mobile := 0)
  If (!mobile)
    var attributesfile := ReadConfigFile(":attribute:attributes");
    Var attributedf:=OpenDataFile("::attributes");
    If (!attributedf)
      attributedf:=CreateDataFile("::attributes",DF_KEYTYPE_STRING);
    EndIf
    ForEach elem_name in ( attributedf.keys() )
      attributedf.DeleteElement(elem_name);
      SleepMS(2);
    EndForEach
    Var skillsinfo:=dictionary;
    Var sortedarray:={};
    If (attributesfile)
      For skill:=0 To MAX_SKILLS
        var name:=GetAttributeName(CStr(skill));
        skillsinfo.insert(attributesfile[name].TrainName,name);
      EndFor
      Foreach value in (skillsinfo)
        sortedarray.append(value);
      EndForEach
    EndIf
    var sortedelement:=attributedf.CreateElement("__Sorted__");
    sortedelement.setprop("attributes",sortedarray);
    For skill:=0 To MAX_SKILLS
      var name:=GetAttributeName(CStr(skill));
      var attrelement:=attributedf.CreateElement(name);
      var infostruct:=struct;
      ForEach key in (ListConfigElemProps(attributesfile[name]))
        var data:=GetConfigString( attributesfile[name],key);
        key:=Lower(key);
        If (key=="advdex")
          Var advstring := SplitWords(data);
          infostruct.+AdvDex:={CInt(CDbl(advstring[1])*10), CStr(advstring[2])};
        ElseIf (key=="advint")
          Var advstring := SplitWords(data);
          infostruct.+AdvInt:={CInt(CDbl(advstring[1])*10), CStr(advstring[2])};
        ElseIf (key=="advstr")
          Var advstring := SplitWords(data);
          infostruct.+AdvStr:={CInt(CDbl(advstring[1])*10), CStr(advstring[2])};
        ElseIf (key=="delay")
          infostruct.+Delay:=CInt(data);
        ElseIf (key=="default_points")
          infostruct.+default_points:=CInt(data);
        ElseIf (key=="unhides")
          infostruct.+UnHides:=CInt(data);
        ElseIf (key=="id")
          infostruct.+ID:=CInt(data);
        Else
          infostruct.insert(key,data);
        EndIf
      EndForEach
      If (!(infostruct.?Delay))
        infostruct.+Delay:=10;
      EndIf
      If (!(infostruct.?default_points))
        infostruct.+default_points:=100;
      EndIf
      If (!(infostruct.?UnHides))
        infostruct.+UnHides:=1;
      EndIf
      attrelement.setprop("Info",infostruct);
    EndFor

    // Title-Laengen ermitteln (siehe GetTitleIDSkill)

    var titlesdef := {
    // {         "(ungelernt)",    0}, // TitleID =  1 -> Skill <   27.5
    // {    "(fast Anfaenger)",    0}, // TitleID =  2 -> Skill <   30
    // {"(geradeso Anfaenger)",    0}, // TitleID =  3 -> Skill <   32.5
    // {         "(Anfaenger)",    0}, // TitleID =  4 -> Skill <   35
    // {   "(guter Anfaenger)",    0}, // TitleID =  5 -> Skill <   37.5
    // {    "(fast Schueler)",     0}, // TitleID =  6 -> Skill <   40
    // {"(geradeso Schueler)",     0}, // TitleID =  7 -> Skill <   42.5
    // {         "(Schueler)",     0}, // TitleID =  8 -> Skill <   45
    // {   "(guter Schueler)",     0}, // TitleID =  9 -> Skill <   47.5
    // {    "(fast Lehrling)",     0}, // TitleID = 10 -> Skill <   50
    // {"(geradeso Lehrling)",     0}, // TitleID = 11 -> Skill <   52.5
    // {         "(Lehrling)",     0}, // TitleID = 12 -> Skill <   55
    // {   "(guter Lehrling)",     0}, // TitleID = 13 -> Skill <   57.5
    // {    "(fast Geselle)",      0}, // TitleID = 14 -> Skill <   60
    // {"(geradeso Geselle)",      0}, // TitleID = 15 -> Skill <   62.5
    // {         "(Geselle)",      0}, // TitleID = 16 -> Skill <   65
    // {   "(guter Geselle)",      0}, // TitleID = 17 -> Skill <   67.5
    // {    "(fast Meister)",      0}, // TitleID = 18 -> Skill <   70
    // {"(geradeso Meister)",      0}, // TitleID = 19 -> Skill <   72.5
    // {         "(Meister)",      0}, // TitleID = 20 -> Skill <   75
    // {   "(guter Meister)",      0}, // TitleID = 21 -> Skill <   77.5
    // {    "(fast Werkmeister)",  0}, // TitleID = 22 -> Skill <   80
    // {"(geradeso Werkmeister)",  0}, // TitleID = 23 -> Skill <   82.5
    // {         "(Werkmeister)",  0}, // TitleID = 24 -> Skill <   85
    // {   "(guter Werkmeister)",  0}, // TitleID = 25 -> Skill <   87.5
    // {    "(fast Lehrmeister)",  0}, // TitleID = 26 -> Skill <   90
    // {"(geradeso Lehrmeister)",  0}, // TitleID = 27 -> Skill <   92.5
    // {         "(Lehrmeister)",  0}, // TitleID = 28 -> Skill <   95
    // {   "(guter Lehrmeister)",  0}, // TitleID = 29 -> Skill <   97.5
    // {    "(fast Grossmeister)", 0}, // TitleID = 30 -> Skill <  100
    // {         "(Grossmeister)", 0}, // TitleID = 31 -> Skill <  115
    // {         "(Genie)",        0}, // TitleID = 32 -> Skill <  125
    // {         "(Legende)",      0}  // TitleID = 33 -> Skill >= 125

    {         "(ungeuebt)",          // TitleID =  1 -> Skill <   27.5
              "(fast Laie)",         // TitleID =  2 -> Skill <   30
              "(geradeso Laie)",     // TitleID =  3 -> Skill <   32.5
              "(Laie)",              // TitleID =  4 -> Skill <   35
              "(guter Laie)",        // TitleID =  5 -> Skill <   37.5
              "(fast Novize)",       // TitleID =  6 -> Skill <   40
              "(geradeso Novize)",   // TitleID =  7 -> Skill <   42.5
              "(Novize)",            // TitleID =  8 -> Skill <   45
              "(guter Novize)",      // TitleID =  9 -> Skill <   47.5
              "(fast Eleve)",        // TitleID = 10 -> Skill <   50
              "(geradeso Eleve)",    // TitleID = 11 -> Skill <   52.5
              "(Eleve)",             // TitleID = 12 -> Skill <   55
              "(guter Eleve)",       // TitleID = 13 -> Skill <   57.5
              "(fast Prinzipal)",    // TitleID = 14 -> Skill <   60
              "(geradeso Prinzipal)",// TitleID = 15 -> Skill <   62.5
              "(Prinzipal)",         // TitleID = 16 -> Skill <   65
              "(guter Prinzipal)",   // TitleID = 17 -> Skill <   67.5
              "(fast Experte)",      // TitleID = 18 -> Skill <   70
              "(geradeso Experte)",  // TitleID = 19 -> Skill <   72.5
              "(Experte)",           // TitleID = 20 -> Skill <   75
              "(guter Experte)",     // TitleID = 21 -> Skill <   77.5
              "(fast Koryphaee)",    // TitleID = 22 -> Skill <   80
              "(geradeso Koryphaee)",// TitleID = 23 -> Skill <   82.5
              "(Koryphaee)",         // TitleID = 24 -> Skill <   85
              "(gute Koryphaee)",    // TitleID = 25 -> Skill <   87.5
              "(fast Adept)",        // TitleID = 26 -> Skill <   90
              "(geradeso Adept)",    // TitleID = 27 -> Skill <   92.5
              "(Adept)",             // TitleID = 28 -> Skill <   95
              "(guter Adept)",       // TitleID = 29 -> Skill <   97.5
              "(fast Grossmeister)", // TitleID = 30 -> Skill <  100
              "(Grossmeister)",      // TitleID = 31 -> Skill <  115
              "(Genie)",             // TitleID = 32 -> Skill <  125
              "(Legende)"},          // TitleID = 33 -> Skill >= 125
              {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    var index;
    For (index := 1; index<=titlesdef[1].size(); index += 1)
      // titlesdef[2][index] := CStr(GumpTextRight(titlesdef[1][index], 202, 137));
      titlesdef[2][index] := "202";
    EndFor
    var titleselement:=attributedf.CreateElement("__Titles__");
    titleselement.setprop("titles",titlesdef);
    CheckLimitConstants(); // Prueft die Konstanten der Limits
    CheckBerufeConfig();   // Prueft das ConfigFile des Berufesystems
    syslog("Die Skills wurden initialisiert ...");
  Else
    // Initial Attribut-RawWerte setzen
    SetDexPergon(mobile, GetDexPergon(mobile)-GetDexModPergon(mobile));
    SetIntPergon(mobile, GetIntPergon(mobile)-GetIntModPergon(mobile));
    SetStrPergon(mobile, GetStrPergon(mobile)-GetStrModPergon(mobile));
  EndIf
EndFunction

///////////////////////////////////////////////////////////
// CheckLimitConstants - Prueft die Konstanten der Limits
///////////////////////////////////////////////////////////
Function CheckLimitConstants()
  If (BaseSkillToRawSkillPergon(SKILLLIMIT_BERUF*10)<>SKILLLIMITRAW_BERUF)
    syslog("FEHLER: SKILLLIMITRAW_BERUF ["+SKILLLIMITRAW_BERUF+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(SKILLLIMIT_BERUF*10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(SKILLLIMIT_HAUPT*10)<>SKILLLIMITRAW_HAUPT)
    syslog("FEHLER: SKILLLIMITRAW_HAUPT ["+SKILLLIMITRAW_HAUPT+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(SKILLLIMIT_HAUPT*10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(SKILLLIMIT_NEWBIE_MAX*10)<>SKILLLIMITRAW_NEWBIE_MAX)
    syslog("FEHLER: SKILLLIMITRAW_NEWBIE_MAX ["+SKILLLIMITRAW_NEWBIE_MAX+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(SKILLLIMIT_NEWBIE_MAX*10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(SKILLLIMIT_NEBEN*10)<>SKILLLIMITRAW_NEBEN)
    syslog("FEHLER: SKILLLIMITRAW_NEBEN ["+SKILLLIMITRAW_NEBEN+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(SKILLLIMIT_NEBEN*10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(SKILLLIMIT_WEITERE*10)<>SKILLLIMITRAW_WEITERE)
    syslog("FEHLER: SKILLLIMITRAW_WEITERE ["+SKILLLIMITRAW_WEITERE+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(SKILLLIMIT_WEITERE*10)+") !");
  EndIf
  If (CInt(RawSkillToBaseSkillPergon(ATTRIBUTELIMITGMRAW_MAX)/10)<>ATTRIBUTELIMITGM_MAX)
    syslog("FEHLER: ATTRIBUTELIMITGM_MAX ["+ATTRIBUTELIMITGM_MAX+"] weicht vom korrekten Wert ab ("+CInt(RawSkillToBaseSkillPergon(ATTRIBUTELIMITGMRAW_MAX)/10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(ATTRIBUTELIMIT_MAX*10)<>ATTRIBUTELIMITRAW_MAX)
    syslog("FEHLER: ATTRIBUTELIMITRAW_MAX ["+ATTRIBUTELIMITRAW_MAX+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(ATTRIBUTELIMIT_MAX*10)+") !");
  EndIf
  If (BaseSkillToRawSkillPergon(ATTRIBUTELIMIT_MIN*10)<>ATTRIBUTELIMITRAW_MIN)
    syslog("FEHLER: ATTRIBUTELIMITRAW_MIN ["+ATTRIBUTELIMITRAW_MIN+"] weicht vom korrekten Wert ab ("+BaseSkillToRawSkillPergon(ATTRIBUTELIMIT_MIN*10)+") !");
  EndIf
EndFunction

////////////////////////////////////////////////////////////////////////////////////////////////
// CheckPlayerLimitsAndCut - Prueft das Limit aller Skills sowie Attribute und schnippelt ggf.
////////////////////////////////////////////////////////////////////////////////////////////////
Function CheckPlayerLimitsAndCut(mobile)
  If (mobile.cmdlevel<CMDLEVEL_SEER)
    For skillid:=0 To MAX_SKILLS
      CheckSkillLimitAndCut(mobile, skillid);
      SleepMS(1);
    EndFor

    CheckAttributLimitsAndCut(mobile);
  EndIf
EndFunction


/////////////////////////////
//                         //
//   Skill-Konfiguration   //
//                         //
/////////////////////////////

/////////////////////////////////////////////////////////////////////////
// GetTitleIDSkill - Liefert die passende TitelID
/////////////////////////////////////////////////////////////////////////
Function GetTitleIDSkill(who, skillid)
  var level := GetSkillFloatPergon(who, skillid);
  If (level<27.5)
    return (1);
  ElseIf (level<100)
    return (CInt((level-27.5)/2.5)+2);
  ElseIf (level<115)
    return (31);
  ElseIf (level<125)
    return (32);
  Else
    return (33);
  EndIf
EndFunction

///////////////////////////////////////////////////////////////////
// GetSkillDefaultPoints - Liefert die Standard-Punkte des Skills
///////////////////////////////////////////////////////////////////
Function GetSkillDefaultPoints(skillid)
  var skillinfo:=GetSkillInfo(skillid);
  If (DEBUG_SKILLS)
    If (!skillinfo)
      syslog("FEHLER: GetSkillDefaultPoints wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
      return(0);
    EndIf
  EndIf
  return (skillinfo.default_points);
EndFunction

////////////////////////////////////////////////////////////////////
// CheckSkillNameProp - Ueberprueft den SkillNamen in der Property
////////////////////////////////////////////////////////////////////
Function CheckSkillNameProp(object, propname)
  var skillid := GetObjProperty(object, propname);

  If (!IsSkillNameDefinied(skillid))
    syslog("FEHLER: CheckSkillNameProp wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
    return(0);
  EndIf
  If (IsNumeric(skillid))
    SetObjProperty(object, propname, GetSkillName(skillid));
    If (DEBUG_SKILLS)
      syslog("WARNUNG: CheckSkillNameProp hat SkillID ("+skillid+") umgewandelt ("+Hex(object.serial)+")");
    EndIf
  EndIf
  return (GetSkillName(skillid));
EndFunction

////////////////////////////////////////////////
// GetSkillName - Liefert den Namen des Skills
////////////////////////////////////////////////

Function GetSkillName(skillid)
  skillid:=GetAttributeName(CStr(skillid));
  If (skillid.errortext)
    syslog("FEHLER: GetSkillName wurde mit falscher SkillID ("+skillid+") aufgerufen !!!");
    return(0);
  EndIf
  return (skillid);
EndFunction

////////////////////////////////////////////////
// IsSkillNameDefinied - Gibts den Skills
////////////////////////////////////////////////

Function IsSkillNameDefinied(name)
  If (!GetAttributeName(CStr(name)))
    Return(0);
  EndIf
  Return(1);
EndFunction

////////////////////////////////////////////////
// GetSkillInfo - Liefert die kompletten Skillinfos
////////////////////////////////////////////////

Function GetSkillInfo(id)
  var attributesfile := OpenDataFile("::attributes");
  var name:=GetAttributeName(CStr(id));
  var info;
  If (name)
    var ele:=attributesfile.FindElement(name);
    info:=ele.getprop("Info");
  Else
    Return(0);
  EndIf
  Return(info);
EndFunction


////////////////////////////////////////////////
// GetSkillsSorted - Liefert nach trainname sortiert
////////////////////////////////////////////////

Function GetSkillsSorted()
  Var attributedf:=OpenDataFile("::attributes");
  If (attributedf)
    var ele:=attributedf.FindElement("__Sorted__");
    If (ele)
      return ele.getprop("attributes");
    EndIf
  EndIf
  Return(0);
EndFunction

////////////////////////////////////////////////
// GetTitles - Liefert alle Titel
////////////////////////////////////////////////

Function GetTitles()
  Var attributedf:=OpenDataFile("::attributes");
  If (attributedf)
    var ele:=attributedf.FindElement("__Titles__");
    If (ele)
      return ele.getprop("titles");
    EndIf
  EndIf
  Return(0);
EndFunction


////////////////////////////////////////////////
// CalculateCombatKorrFaktor - Liefert den Korrekturfaktor für Statanstiege, abhängig Waffenspeed
////////////////////////////////////////////////

Function CalculateCombatKorrFaktor(byref mobile, points_org, points_mod, modifier := 1.0)
  //Anhand der Originalpunkte wird die Waffengeschwindigkeit zurückgerechnet
  var Faktor := 1.0;
  var quelle := GetProcess(GetPid()).name;
  If ((quelle == "pkg/attribute/syshook.ecl")||(quelle == "pkg/std/combat/mainhitscript.ecl"))
    Faktor := points_mod*modifier/points_org;
    If ((Faktor < 0)||(Faktor > 5.0))
      syslog("FEHLER: CalculateCombatKorrFaktor Statfaktorberechnung bringt ungueltiges Ergebnis."
            +"Faktor="+Faktor+" ("+points_mod+"*"+modifier+"/"+points_org+")"
            +" - "+Hex(mobile.serial));
      Faktor := 1.0;
    EndIf
  EndIf
  return Faktor;
EndFunction

////////////////////////////////////////////////
// CalculateMagicKorrFaktor - Liefert den Korrekturfaktor für Statanstiege, abhängig Spruchschwierigkeit
////////////////////////////////////////////////

Function CalculateMagicKorrFaktor(byref mobile, spruchdiffi, modifier := 1.0)
  var diffiSpruch := spruchdiffi*modifier; //Spruchschwierigkeit zurückrechnen
  var Faktor := Pow((diffiSpruch-20)/85.0,4)*2.0468+0.23;
  If ((Faktor < 0.2)||(Faktor > 4.5))
    syslog("FEHLER: CalculateMagicKorrFaktor Statfaktorberechnung bringt ungueltiges Ergebnis. diffiSpruch="
      +diffiSpruch+" Faktor="+Faktor+" - "+Hex(mobile.serial));
    Faktor := 0.2;
  EndIf
  return Faktor;
EndFunction

// Delay (Number of seconds skills cannot be used after this one)
//   - Set a "Delay" property in skills.cfg.  Other skill use will
//     be disabled Until 'n' seconds after the initial skill was begun.
//     Note, we may want to change this timer to start after the skill
//     is complete, or after the skill script calls Detach().
//   - Only Hiding has a skill delay at this time.
//   - No attempt is made to disable skill use in item-use scripts.
//     (So, you could hide, then turn around and use a hammer etc)

// vim: sw=2 sts=2
