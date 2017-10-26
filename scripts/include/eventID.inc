//
// event IDs to be returned in eventid member of wait_for_event
// result.
//
// See documentation for details.
//
// TODO: move this to OS.EM, when .EM files can have constants.
//
// Author: ens
//
// Last modification: 01/22/1999
//


const EVID_SPEECH           := 0x0001;
const EVID_ENGAGED          := 0x0002;
const EVID_DISENGAGED       := 0x0004;
const EVID_NODE             := 0x0005;
const EVID_DAMAGED          := 0x0008;
const EVID_HERDING          := 0x1111;
const EVID_WAKEUP           := 0x1112;
const EVID_SKILLCAP         := 0x1113;
const EVID_STATCAP          := 0x1114;
const EVID_SPAWNREGION      := 0x1115;
const EVID_SR_DEATH         := 0x1116;
const EVID_SR_PENDING       := 0x1117;
const EVID_ENTEREDAREA      := 0x0010;
const EVID_STAFF_LOGIN      := 0x0011;
const EVID_PLAYER_LOGIN     := 0x0012;
const EVID_PLAYER_LOGOFF    := 0x0013;
const EVID_PLAYER_DELAY     := 0x0014;
const EVID_FIND_COUN        := 0x0015;
const EVID_FIND_GM          := 0x0016;
const EVID_GUILD_DISBAND    := 0x0017;
const EVID_GUILD_CREATED    := 0x0018;

const EVID_ALL_ATTACK_CMD := 0x12;
const EVID_ALL_FOLLOW_CMD := 0x11;

const EVID_LEFTAREA         := 0x0020;

const EVID_OPPONENT_MOVED   := 0x0040;

const EVID_HOSTILE_MOVED    := 0x0080; // not implemented

const EVID_MERCHANT_BOUGHT  := 0x0100;
const EVID_MERCHANT_SOLD    := 0x0200;

const EVID_ITEM_GIVEN       := 0x0400;
const EVID_DOUBLECLICKED    := 0x0800;

const EVID_RESERVED_1000    := 0x00001000;
const EVID_RESERVED_2000    := 0x00002000;
const EVID_RESERVED_4000    := 0x00004000;
const EVID_RESERVED_8000    := 0x00008000;

const EVID_SPAWNPOINT_CREATED	:= 1010;
const EVID_SPAWNPOINT_DESTROYED := 1020;
const EVID_SPAWNPOINT_RESET		:= 1040;
const EVID_SPAWNPOINT_CHANGED	:= 1080;
const EVID_SPAWN_DEATH			:= 1100;
const EVID_SPAWNREGION_DEATH	:= 3001;
const EVID_RESERVED_00010000    := 0x00010000;
const EVID_RESERVED_00020000    := 0x00020000;
const EVID_RESERVED_00040000    := 0x00040000;
const EVID_RESERVED_00080000    := 0x00080000;
const EVID_RESERVED_00100000    := 0x00100000;
const EVID_RESERVED_00200000    := 0x00200000;
const EVID_RESERVED_00400000    := 0x00400000;
const EVID_RESERVED_00800000    := 0x00800000;
const EVID_RESERVED_01000000    := 0x01000000;
const EVID_RESERVED_02000000    := 0x02000000;
const EVID_RESERVED_04000000    := 0x04000000;
const EVID_RESERVED_08000000    := 0x08000000;
const EVID_RESERVED_10000000    := 0x10000000;
const EVID_RESERVED_20000000    := 0x20000000;
const EVID_RESERVED_40000000    := 0x40000000;
const EVID_RESERVED_80000000    := 0x80000000;

/* FIXME: these need to be renumbered, one clashes with a reserved event */
const EVID_SPELL_ATTACK     := 0x0800;
const EVID_BUDDYATTACKED     := 0x1600;
const EVID_PEACEMADE         := 0x90000000;