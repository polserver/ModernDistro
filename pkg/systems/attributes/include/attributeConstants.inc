use uo;

const ATTRIBUTEID_DEXTERITY        := "Dexterity";
const ATTRIBUTEID_INTELLIGENCE     := "Intelligence";
const ATTRIBUTEID_STRENGTH         := "Strength";
const VITALID_LIFE                 := "Life";
const HITS                         := VITALID_LIFE;
const VITALID_MANA                 := "Mana";
const VITALID_STAMINA              := "Stamina";



enum VITALS
	HITS			:= "Hits",
	MANA			:= "Mana",
	STAMINA			:= "Stamina",
	HUNGER			:= "Hunger"
endenum

enum STATS
	STRENGTH		:= "Strength",
	DEXTERITY		:= "Dexterity",
	INTELLIGENCE		:= "Intelligence"
endenum

enum SKILLS
	ALCHEMY			:= "Alchemy",
	ANATOMY			:= "Anatomy",
	ANIMALLORE		:= "AnimalLore",
	ITEM_IDENTIFICATION	:= "ItemIdentification",
	ARMSLORE		:= "ArmsLore",
	PARRYING			:= "Parry",
	BEGGING			:= "Begging",
	BLACKSMITHY		:= "Blacksmithy",
	BOWCRAFT	:= "BowcraftFletching",
	PEACEMAKING		:= "Peacemaking",
	CAMPING			:= "Camping",
	CARPENTRY		:= "Carpentry",
	CARTOGRAPHY		:= "Cartography",
	COOKING			:= "Cooking",
	DETECTING_HIDDEN	:= "DetectingHidden",
	DISCORDANCE		:= "Discordance",
	EVALUATING_INTELLIGENCE	:= "EvaluatingIntelligence",
	HEALING			:= "Healing",
	FISHING			:= "Fishing",
	FORENSIC_EVALUATION	:= "ForensicEvaluation",
	HERDING			:= "Herding",
	HIDING			:= "Hiding",
	PROVOCATION		:= "Provocation",
	INSCRIPTION		:= "Inscription",
	LOCKPICKING		:= "Lockpicking",
	MAGERY			:= "Magery",
	RESISTING_SPELLS	:= "ResistingSpells",
	TACTICS			:= "Tactics",
	SNOOPING		:= "Snooping",
	MUSICIANSHIP		:= "Musicianship",
	POISONING		:= "Poisoning",
	ARCHERY			:= "Archery",
	SPIRITSPEAK		:= "SpiritSpeak",
	STEALING		:= "Stealing",
	TAILORING		:= "Tailoring",
	TAMING		:= "AnimalTaming",
	TASTE_IDENTIFICATION	:= "TasteIdentification",
	TINKERING		:= "Tinkering",
	TRACKING		:= "Tracking",
	VETERINARY		:= "Veterinary",
	SWORDSMANSHIP		:= "Swordsmanship",
	MACEFIGHTING		:= "MaceFighting",
	FENCING			:= "Fencing",
	WRESTLING		:= "Wrestling",
	LUMBERJACKING		:= "Lumberjacking",
	MINING			:= "Mining",
	MEDITATION		:= "Meditation",
	STEALTH			:= "Stealth",
	REMOVETRAP		:= "RemoveTrap",
	NECROMANCY		:= "Necromancy",
	FOCUS		:= "BattleFocus",
	CHIVALRY		:= "Chivalry",
  BUSHIDO       := "Bushido",
  NINJITSU      := "Ninjitsu",
  SPELLWEAVING := "Spellweaving",
  MYSTICISM     := "Mysticism",
  IMBUING       := "Imbuing",
  THROWING      := "Throwing"

endenum

//const SKILLID__HIGHEST := 48;
var List_of_Skills := array{"Alchemy", "Anatomy", "AnimalLore", "ItemIdentification", "ArmsLore", "Parry", "Begging", "Blacksmithy", "BowcraftFletching", "Peacemaking",
                  "Camping", "Carpentry", "Cartography", "Cooking", "DetectingHidden", "Discordance", "EvaluatingIntelligence", "Healing", "Fishing",
                  "ForensicEvaluation", "Herding", "Hiding", "Provocation", "Inscription", "Lockpicking", "Magery", "ResistingSpells", "Tactics", "Snooping",
                  "Musicianship", "Poisoning", "Archery", "SpiritSpeak", "Stealing", "Tailoring", "AnimalTaming", "TasteIdentification", "Tinkering",
                  "Tracking", "Veterinary", "Swordsmanship", "MaceFighting", "Fencing", "Wrestling", "Lumberjacking", "Mining", "Meditation", "Stealth",
                  "RemoveTrap", "Necromancy", "BattleFocus", "Chivalry", "Bushido", "Ninjitsu", "Spellweaving", "Mysticism", "Imbuing", "Throwing"};
