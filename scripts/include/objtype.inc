/////////////////////////////////////////////////////////////////////////////
//
// include/objtype.inc - Object Type definitions
//
// TODO: consider moving these into include/client.inc
//       or, moving CID_XX constants in here
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x0000 to 0x3FFF - "Standard" objects
//
/////////////////////////////////////////////////////////////////////////////

const UOBJ_STONEWALL_NS := 0x005f;
const UOBJ_STONEWALL_EW := 0x0060;
const UOBJ_GUARDS       := 0xa340;
const UOBJ_FISHSTEAK    := 0x097a;

const UOBJ_BOX          := 0x09aa;

const UOBJ_FISH_START   := 0x09cc;
const UOBJ_FISH_END     := 0x09cf;

const UOBJ_SFISH_START  := 0x0dd6;
const UOBJ_SFISH_END    := 0x0dd9;

const UOBJ_KINDLING     := 0x0de1;
const UOBJ_CAMPFIRE     := 0x0de3;
const UOBJ_EMBERS       := 0x0de9;
const UOBJ_ASHES        := 0x0dea;

const UOBJ_BANDAGE      := 0x0e21;
const UOBJ_BLANK_SCROLL := 0x0e34;

const UOBJ_BANKBOX      := 0x0e40;
const UOBJ_MAGICBAG     := 0x6698;
const UOBJ_GMPACK		:= 0x19669;


const UOBJ_SPELLBOOK        := 0x0efa;

const UOBJ_POISON_POTION    := 0x0f0a;
const UOBJ_EMPTY_BOTTLE     := 0x0f0e;


const UOBJ_SHOVEL           := 0x0f39;

const UOBJ_XBOW             := 0x0f4f;

const UOBJ_BLUE_MOONGATE    := 0x0f6c;




///////////////////
//  Reagents
///////////////////

const UOBJ_BLACKMOOR          := 0x0f79;          //Powdered
const UOBJ_BONE               := 0x0f7e;          //Powdered
const UOBJ_BRIMSTONE          := 0x0f7f;          //Powdered
const UOBJ_DAEMON_BONE        := 0x0f80;          //Powdered
const UOBJ_DEAD_WOOD          := 0x0f90;          //Powdered
const UOBJ_FERTILE_DIRT       := 0x0f81;          //Powdered
const UOBJ_OBSIDIAN		        := 0x0f89;          //Powdered
const UOBJ_PIG_IRON		        := 0x0f8a;          //Powdered
const UOBJ_PUMICE             := 0x0f8b;          //Powdered
const UOBJ_SERPENT_SCALE      := 0x0f8e;          //Powdered
const UOBJ_VOLCANIC_ASH       := 0x0f8f;          //Powdered

const UOBJ_BATWING		        := 0x0f78;          //Essence
const UOBJ_BLOODSPAWN         := 0x0f7c;          //Essence
const UOBJ_DRAGONS_BLOOD      := 0x0f82;          //Essence
const UOBJ_EXECUTIONERS_CAP   := 0x0f83;          //Essence
const UOBJ_EYE_OF_NEWT        := 0x0f87;          //Essence
const UOBJ_VIAL_OF_BLOOD      := 0x0f7d;          //Essence
const UOBJ_WYRMS_HEART        := 0x0f91;          //Essence

const UOBJ_BLACK_PEARL        := 0x0f7a;
const UOBJ_BLOOD_MOSS         := 0x0f7b;
const UOBJ_GARLIC             := 0x0f84;
const UOBJ_GINSENG            := 0x0f85;
const UOBJ_MANDRAKE_ROOT      := 0x0f86;
const UOBJ_NIGHTSHADE         := 0x0f88;
const UOBJ_SULFUROUS_ASH      := 0x0f8c; // CHECKME spelling
const UOBJ_SPIDER_SILK        := 0x0f8d;
const UOBJ_SULPHUROUS_ASH     := 0x0f8c;

const UOBJ_REAPER_HEARTWOOD   := 0x1706f;
const UOBJ_ENT_HEARTWOOD      := 0x1706e;
const UOBJ_ENCHANTED_BONE     := 0x1a153;

///////////
//  Gems //
///////////

const UOBJ_GEM_AMBER		        := 0x0f25;
const UOBJ_GEM_STAR_SAPPHIRE	        := 0x0f0f;
const UOBJ_GEM_EMERALD		        := 0x0f10;
const UOBJ_GEM_SAPPHIRE		        := 0x0f11;
const UOBJ_GEM_RUBY			:= 0x0f13;
const UOBJ_GEM_CITRINE		        := 0x0f15;
const UOBJ_GEM_AMETHYST		        := 0x0f16;
const UOBJ_GEM_TOURMALINE	        := 0x0f18;
const UOBJ_GEM_DIAMOND		        := 0x0f30;
const UOBJ_GEM_START                    := 0x0f0f;
const UOBJ_GEM_END                      := 0x0f30;


////////////////
// Food Items //
////////////////

const UOBJ_EGG				:= 0x09b5;
const UOBJ_RAW_RIB			:= 0x09f1;
const UOBJ_RAW_FISHSTEAK		:= 0x097a;
const UOBJ_RAW_BIRD			:= 0x09b9;
const UOBJ_RAW_HAM			:= 0x09c9;
const UOBJ_BACON_SLAB		        := 0x0976;
const UOBJ_RAW_SAUSAGE		        := 0x09c0;
const UOBJ_HONEY			:= 0x09ec;
const UOBJ_CARROT			:= 0x0c77;
const UOBJ_APPLE			:= 0x09d0;
const UOBJ_PEACH			:= 0x172c;
const UOBJ_LEMON			:= 0x1728;
const UOBJ_LIME			        := 0x172a;
const UOBJ_BANANA			:= 0x171f;
const UOBJ_PEAR			        := 0x172d;
const UOBJ_GRAPES			:= 0x0d1a;
const UOBJ_DATES			:= 0x1727;
const UOBJ_PUMPKIN			:= 0x0c6a;
const UOBJ_LETTUCE			:= 0x0c70;
const UOBJ_CABBAGE			:= 0x0c7c;
const UOBJ_ONION			:= 0x0c6d;
const UOBJ_TURNIP			:= 0x0d3a;
const UOBJ_COCONUT			:= 0x1726;
const UOBJ_RAW_CHICKEN_LEG	        := 0x1607;
const UOBJ_RAW_LEG_OF_LAMB	        := 0x1609;


//////////
// Keys //
//////////

const UOBJ_COPPER_KEY                   := 0x100e;
const UOBJ_GOLD_KEY                     := 0x100f;
const UOBJ_IRON_KEY                     := 0x1010;
const UOBJ_KEY_RING                     := 0x1011;
const UOBJ_MAGIC_KEY                    := 0x1012;
const UOBJ_RUSTY_IRON_KEY               := 0x1013;

const UOBJ_CLOCK                        := 0x104c;
const UOBJ_CLOCK_FRAME                  := 0x104e;
const UOBJ_CLOCK_PARTS                  := 0x104f;
const UOBJ_AXLE_AND_GEARS               := 0x1051;
const UOBJ_GEARS                        := 0x1053;
const UOBJ_HINGE                        := 0x1055;
const UOBJ_SEXTANT                      := 0x1057;
const UOBJ_SEXTANT_PARTS                := 0x1059;
const UOBJ_AXLE                         := 0x105b;
const UOBJ_SPRINGS                      := 0x105d;

const UOBJ_WAX                          := 0x1422;
const UOBJ_EMPTY_KETTLE                 := 0x09ed;
const UOBJ_KETTLE_OF_WAX                := 0x142a;
const UOBJ_DIPPING_STICK                := 0x1428;
const UOBJ_SKULL                        := 0x1ae2;



/////////////////////
// Tailoring items //
/////////////////////

const UOBJ_SEWING_KIT                   := 0x0f9d;
const UOBJ_BOLT_OF_CLOTH                := 0x0f95;
const UOBJ_CLOTH_BOLT                   := 0x0f95;
const UOBJ_CLOTH                        := 0x1765;
const UOBJ_WOOL                         := 0x0df8;
const UOBJ_HIDE                         := 0x1078;

const UOBJ_THREAD_START                 := 0x0fa0;
const UOBJ_THREAD_END                   := 0x0fa1;

const UOBJ_THREAD1			:= 0x0fa0;
const UOBJ_THREAD2			:= 0x0fa1;
const UOBJ_YARN			        := 0x0e1d;
const UOBJ_BOLT_START		        := 0x0f95;
const UOBJ_BOLT_END			:= 0x0f9c;
const UOBJ_CLOTH_START		        := 0x175d;
const UOBJ_CLOTH_END		        := 0x1768;
const UOBJ_COLORED_HIDE_START	        := 0x17050;
const UOBJ_COLORED_HIDE_END	        := 0x17054;
const UOBJ_TGOLD_INGOT                  := 0x1be9;
const UOBJ_LOOM_START                   := 0x105f;
const UOBJ_LOOM_END                     := 0x1066;


//////////////////////////
// Practice Dummy stuff //
//////////////////////////
const UOBJ_DUMMYSWING_NS                := 0x1071;
const UOBJ_DUMMYFREEZE_NS               := 0x1070;
const UOBJ_DUMMYSWING_EW                := 0x1075;
const UOBJ_DUMMYFREEZE_EW               := 0x1074;

const UOBJ_BLOOD                        := 0x122d;

const UOBJ_BOW                          := 0x13b2;
const UOBJ_HEAVY_XBOW                   := 0x13fd;
const UOBJ_ARROW			:= 0x0f3f;
const UOBJ_BOLT		        	:= 0x1bfb;
const UOBJ_FIRE_ARROW		        := 0x16051;


const UOBJ_MAP1                         := 0x14eb;
const UOBJ_MAP2                         := 0x14ec;
const UOBJ_ROLLED_MAP1                  := 0x14ed;
const UOBJ_ROLLED_MAP2                  := 0x14ee;

const UOBJ_HOUSE_DEED1                  := 0x14ef;
const UOBJ_SHIP_DEED1                   := 0x14f1;
const UOBJ_SHIP_DEED2                   := 0x14f2;

const UOBJ_CHEST                        := 0x1940;


const UOBJ_WOOD_YEW                     := 0x16351;
const UOBJ_WOOD_ASH                     := 0x16352;
const UOBJ_WOOD_CYPRESS                 := 0x16353;
const UOBJ_WOOD_OAK                     := 0x16354;
const UOBJ_WOOD_WORMWOOD                := 0x16355;

const UOBJ_FEATHER                      := 0x1bd1;
const UOBJ_SHAFTS                       := 0x1bd4;
const UOBJ_SHAFT	                := 0x1bd4;


////////////////////////////////////////////////
// Types of wood available from various trees //
////////////////////////////////////////////////

const UOBJ_LOG                         := 0x1bdd;
const UOBJ_YEW_LOG                      := 0x16351;
const UOBJ_ASH_LOG                      := 0x16352;
const UOBJ_CYPRESS_LOG                  := 0x16353;
const UOBJ_OAK_LOG                      := 0x16354;
const UOBJ_WALNUT_LOG                   := 0x16358;
const UOBJ_CEDAR_LOG                    := 0x16359;
const UOBJ_MAPLE_LOG                    := 0x16361;
const UOBJ_PINE_LOG                     := 0x16364;
const UOBJ_GOSSAMER_LOG                 := 0x16365;
const UOBJ_WILLOW_LOG                   := 0x16363;
const UOBJ_BANANA_LOG                   := 0x16362;
const UOBJ_SPIDER_LOG                   := 0x16366;
const UOBJ_JUNIPER_LOG                  := 0x16367;

// These logs are not naturally occurring logs.
const UOBJ_VENOMOUS_LOG                 := 0x1635B;
const UOBJ_WORMWOOD_LOG                 := 0x16355;

/////////////////////////////////////////////////////////////////////////
// Types of boards are available from various trees when logs are sawn //
/////////////////////////////////////////////////////////////////////////

const UOBJ_BOARD                          := 0x1bd7;
const UOBJ_YEW_BOARD                      := 0x16370;
const UOBJ_ASH_BOARD                      := 0x16371;
const UOBJ_CYPRESS_BOARD                  := 0x16372;
const UOBJ_OAK_BOARD                      := 0x16373;
const UOBJ_WALNUT_BOARd                   := 0x16377;
const UOBJ_CEDAR_BOARD                    := 0x16378;
const UOBJ_MAPLE_BOARD                    := 0x16380;
const UOBJ_PINE_BOARD                     := 0x16381;
const UOBJ_GOSSAMER_BOARD                 := 0x16382;
const UOBJ_WILLOW_BOARD                   := 0x16383;
const UOBJ_BANANA_BOARD                   := 0x16384;
const UOBJ_SPIDER_BOARD                   := 0x16385;
const UOBJ_JUNIPER_BOARD                  := 0x16386;

// These boards are not naturally occurring boards.
// That is to say they cannont be found on standard trees.
const UOBJ_WORMWOOD_BOARD                 := 0x16374;
const UOBJ_HEARTWOOD_BOARD                := 0x16375;
const UOBJ_BONEWOOD_BOARD                 := 0x16376;
const UOBJ_IRONWOOD_BOARD                 := 0x16379;
const UOBJ_VENOMOUS_BOARD                 := 0x1637A;
const UOBJ_SPIRITWOOD_BOARD               := 0x1637B;
const UOBJ_SILVERWOOD_BOARD               := 0x1637C;
const UOBJ_ROSEWOOD_BOARD                 := 0x1637D;
const UOBJ_REAPERWOOD_BOARD               := 0x1637E;
const UOBJ_ENTHEARTWOOD_BOARD             := 0x1637F;


//////////////////////////////////////////
//  Ore and ingotsand Blacksmithy stuff //
//////////////////////////////////////////

const UOBJ_FORGE_START  := 0x197a;  // see also UOBJ_SMALL_FORGE
const UOBJ_FORGE_END    := 0x19a9;
const UOBJ_SMALL_FORGE  := 0xfb1;  // see also UOBJ_FORGE_START..UOBJ_FORGE_END
const UOBJ_FORGE1       := 0x19a2;
const UOBJ_FORGE2       := 0x19a6;
const UOBJ_FORGE3       := 0x1982;
const UOBJ_FORGE4       := 0x1996;
const UOBJ_FORGE5       := 0x199a;
const UOBJ_FORGE6       := 0x198e;
const UOBJ_FORGE7       := 0x1992;
const UOBJ_FORGE8       := 0x1986;
const UOBJ_FORGE9       := 0x198a;
const UOBJ_ANVIL1       := 0x0faf;
const UOBJ_ANVIL2       := 0x0fb0;

const UOBJ_IRON_ORE             := 0x16300;
const UOBJ_DULLCOPPER_ORE       := 0x16301;
const UOBJ_SHADOW_ORE		:= 0x16302;
const UOBJ_COPPER_ORE		:= 0x16303;
const UOBJ_BRONZE_ORE           := 0x16304;
const UOBJ_KINDRED_ORE          := 0x16305;
const UOBJ_AGAPITE_ORE          := 0x16306;
const UOBJ_VERITE_ORE           := 0x16307;
const UOBJ_VALORITE_ORE         := 0x16308;

const UOBJ_IRON_INGOT           := 0x16309;
const UOBJ_DULLCOPPER_INGOT     := 0x1630a;
const UOBJ_SHADOW_INGOT		:= 0x1630b;
const UOBJ_COPPER_INGOT		:= 0x1630c;
const UOBJ_BRONZE_INGOT         := 0x1630d;
const UOBJ_KINDRED_INGOT        := 0x1630e;
const UOBJ_AGAPITE_INGOT        := 0x1630f;
const UOBJ_VERITE_INGOT         := 0x16310;
const UOBJ_VALORITE_INGOT       := 0x16311;

/////////////////////////
// musical instruments //
/////////////////////////

const UOBJ_TAMB         := 0x0e9d;
const UOBJ_TAMBORINE		        := 0x0e9d;
const UOBJ_TAMBORINE2		        := 0x0e9e;
const UOBJ_LUTE			        := 0x0eb3;
const UOBJ_LUTE2			:= 0x0eb4;
const UOBJ_DRUM			        := 0x0e9c;
const UOBJ_HARP			        := 0x0eb2;
const UOBJ_STANDING_HARP		:= 0x0eb1;


//////////////
// Footwear //
//////////////

const UOBJ_SANDALS			:= 0x170d;
const UOBJ_SHOES			:= 0x170f;
const UOBJ_BOOTS			:= 0x170b;
const UOBJ_THIGH_BOOTS		        := 0x1711;


///////////////////
// leather armor //
///////////////////

const UOBJ_LEATHER_GORGET	        := 0x13c7;
const UBOJ_LEATHER_SLEEVES	        := 0x13c5;
const UOBJ_LEATHER_SLEEVES2	        := 0x13cd;
const UOBJ_LEATHER_GLOVES	        := 0x13c6;
const UOBJ_LEATHER_CAP		        := 0x1db9;
const UOBJ_LEATHER_LEGGINGS	        := 0x13cb;
const UOBJ_LEATHER_TUNIC		:= 0x13cc;
const UOBJ_LEATHER_TUNIC2	        := 0x13d3;
const UOBJ_LEATHER_FEMALE	        := 0x1c06;
const UOBJ_LEATHER_SKIRT		:= 0x1c08;
const UOBJ_LEATHER_BUSTIER	        := 0x1c0a;
const UOBJ_LEATHER_SHORTS	        := 0x1c00;




///////////////////
// studded armor //
///////////////////

const UOBJ_STUDDED_GORGET	        := 0x13d6;
const UOBJ_STUDDED_GLOVES	        := 0x13d5;
const UOBJ_STUDDED_SLEEVES	        := 0x13d4;
const UOBJ_STUDDED_SLEEVES2	        := 0x13dc;
const UOBJ_STUDDED_LEGGINGS	        := 0x13da;
const UOBJ_STUDDED_TUNIC		:= 0x13db;
const UOBJ_STUDDED_FEMALE	        := 0x1c02;
const UOBJ_STUDDED_BUSTIER	        := 0x1c0c;




////////////////////
// ringmail armor //
////////////////////

const UOBJ_RINGMAIL_TUNIC	        := 0x13ec;
const UOBJ_RINGMAIL_TUNIC2	        := 0x13ed;
const UOBJ_RINGMAIL_SLEEVES	        := 0x13ee;
const UOBJ_RINGMAIL_SLEEVES2	        := 0x13ef;
const UOBJ_RINGMAIL_LEGGINGS	        := 0x13f0;
const UOBJ_RINGMAIL_LEGGINGS2	        := 0x13f1;
const UOBJ_RINGMAIL_GLOVES	        := 0x13f2;
const UOBJ_RINGMAIL_GLOVES2	        := 0x13eb;



/////////////////////
// chainmail armor //
/////////////////////

const UOBJ_CHAINMAIL_COIF	        := 0x13bb;
const UOBJ_CHAINMAIL_COIF2	        := 0x13c0;
const UOBJ_CHAINMAIL_LEGGINGS	        := 0x13be;
const UOBJ_CHAINMAIL_LEGGINGS2          := 0x13c3;
const UOBJ_CHAINMAIL_TUNIC	        := 0x13bf;
const UOBJ_CHAINMAIL_TUNIC2	        := 0x13c4;
const UOBJ_CHAINMAIL_GLOVES	        := 0x17066;




/////////////////////
// Platemail armor //
/////////////////////

const UOBJ_PLATE_FEMALE		        := 0x1c04;
const UOBJ_PLATE_FEMALE2		:= 0x1c05;
const UOBJ_PLATE_BREASTPLATE	        := 0x1415;
const UOBJ_PLATE_BREASTPLATE2	        := 0x1416;
const UOBJ_PLATE_ARMS		        := 0x1410;
const UOBJ_PLATE_ARMS2		        := 0x1417;
const UOBJ_PLATE_LEGS		        := 0x1411;
const UOBJ_PLATE_LEGS2		        := 0x141a;
const UOBJ_PLATE_GLOVES		        := 0x1414;
const UOBJ_PLATE_GLOVES2		:= 0x1418;
const UOBJ_PLATE_GORGET		        := 0x1413;




////////////////
// Bone armor //
////////////////

const UOBJ_BONE_ARMS		        := 0x144e;
const UOBJ_BONE_TUNIC		        := 0x144f;
const UOBJ_BONE_GLOVES		        := 0x1450;
const UOBJ_BONE_HELM		        := 0x1451;
const UOBJ_BONE_LEGS		        := 0x1452;




///////////
// helms //
///////////

const UOBJ_PLATE_HELM		        := 0x1412;
const UOBJ_PLATE_HELM2		        := 0x1419;
const UOBJ_CLOSEHELM		        := 0x1408;
const UOBJ_CLOSEHELM2		        := 0x1409;
const UOBJ_HELMET			:= 0x140a;
const UOBJ_HELMET2			:= 0x140b;
const UOBJ_BASCINET			:= 0x140c;
const UOBJ_BASCINET2		        := 0x140d;
const UOBJ_NOSEHELM			:= 0x140e;
const UOBJ_NOSEHELM2		        := 0x140f;




/////////////
// shields //
/////////////

const UOBJ_BUCKLER			:= 0x1b73;
const UOBJ_BRONZESHIELD		        := 0x1b72;
const UOBJ_METALSHIELD		        := 0x1b7b;
const UOBJ_KITESHIELD		        := 0x1b74;
const UOBJ_KITESHIELD2		        := 0x1b75;
const UOBJ_HEATER			:= 0x1b76;
const UOBJ_HEATER2			:= 0x1b77;
const UOBJ_WOODEN_SHIELD		:= 0x1b7a;
const UOBJ_WOODEN_KITESHIELD	        := 0x1b78;
const UOBJ_WOODEN_KITESHIELD2	        := 0x1b79;
const UOBJ_CHAOS_SHIELD		        := 0x1bc3;
const UOBJ_ORDER_SHIELD		        := 0x1bc4;
const UOBJ_ORDER_SHIELD2		:= 0x1bc5;




////////////////////
// wooden weapons //
////////////////////

const UOBJ_CLUB			        := 0x13b3;
const UOBJ_SHEEPHERDS_CROOK	        := 0x0e81;
const UOBJ_BLACK_STAFF		        := 0x0df0;
const UOBJ_GNARLED_STAFF	        := 0x13f9;
const UOBJ_QUARTER_STAFF	        := 0x0e8a;


const UOBJ_COPPER_COIN		:=0x0eea;
const UOBJ_SILVER_COIN		:=0x0ef0;
const UOBJ_GOLD_COIN		:=0x0eed;
const UOBJ_CLAY			:= 0xee19;
const UOBJ_SAND			:= 0xee18;
const UOBJ_GLASS		:= 0xee20;
const UOBJ_PORCELAIN		:= 0xee21;
const UOBJ_SNOW                 := 0x5001;


const UOBJ_CORPSE       := 0x2006;

const UOBJ_ARM          := 0x1cec;
const UOBJ_LEG          := 0x1ce2;
const UOBJ_TRUNK        := 0x1cee;
const UOBJ_HEART        := 0x1cef;
const UOBJ_HEAD         := 0x1dae;

////////////////
// Containers //
////////////////

const UOBJ_BACKPACK			:= 0x0e73;
const UOBJ_BAG				:= 0x0e76;
const UOBJ_POUCH			:= 0x0e79;
const UOBJ_BASKET			:= 0x0990;
const UOBJ_PICNIC_BASKET		:= 0x0e7a;
const UOBJ_WOODEN_BOX_1		        := 0x0e7d;
const UOBJ_WOODEN_BOX_2		        := 0x09aa;
const UOBJ_TRASHBARREL		        := 0x17057;

const UOBJ_TINKER_TOOLS := 0x1ebc;



/////////////////////////
// More things defined //
/////////////////////////

const UOBJ_STARTHAIR		:= 0x203b;
const UOBJ_ENDHAIR			:= 0x204d;


const UOBJ_RUNE                 := 0x1f14;
const UOBJ_RECALL_RUNE          := 0x1f14;
const UOBJ_RUNEBOOK             := 0x22C5;

const UOBJ_PARAFIELD_NS := 0x3967;
const UOBJ_PARAFIELD_EW := 0x3979;
const UOBJ_POISONFIELD_NS   := 0x3914;
const UOBJ_POISONFIELD_EW   := 0x3920;
const UOBJ_EFIELD_NS        := 0x3947;
const UOBJ_EFIELD_EW        := 0x3956;

const GID_PLANK_EXTENDED_FACING_WEST   := 0x3ed5;
const GID_PLANK_RETRACTED_FACING_WEST  := 0x3eb1;
const GID_PLANK_EXTENDED_FACING_EAST   := 0x3ed4;
const GID_PLANK_RETRACTED_FACING_EAST  := 0x3eb2;
const GID_PLANK_EXTENDED_FACING_SOUTH  := 0x3e84;
const GID_PLANK_RETRACTED_FACING_SOUTH := 0x3e85;
const GID_PLANK_EXTENDED_FACING_NORTH  := 0x3e89;
const GID_PLANK_RETRACTED_FACING_NORTH := 0x3e8a;

// ick! these are fine for humans, but not true for items..
// these are more of a CID.
const UOBJ_HUMAN_MALE   := 0x190;
const UOBJ_HUMAN_FEMALE := 0x191;

/////////////////////////////////////////////////////////////////////////////
//
// 0x4000 to 0x4FFF - Multis
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 0x5000 to 0xEFFF - To Be Determined
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
// 0xF000 to 0xFFFF - Reserved by system
//
/////////////////////////////////////////////////////////////////////////////

	//
	// Ship Components
	//
const EXTOBJ_TILLERMAN       := 0x1F010;
const EXTOBJ_PORT_PLANK      := 0x1F011;
const EXTOBJ_STARBOARD_PLANK := 0x1F012;
const EXTOBJ_HOLD            := 0x1F013;
