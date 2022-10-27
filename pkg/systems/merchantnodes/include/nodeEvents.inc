use uo;
use os;

// ToDo: Document exactly what needs to be sent with each event type here...
enum MN_EVENTS
	DELETE_EVENT		:= 0x2,
	RESPAWN_ALL_EVENT	:= 0x4,
	RESPAWN_EVENT		:= 0x8,
	RESTOCK_ALL_EVENT	:= 0x10,
	RESTOCK_EVENT		:= 0x20,
	SLEEP_EVENT			:= 0x40,
	SPAWN_EVENT			:= 0x80,
	UPDATE_EVENT		:= 0x100
endenum

// Kind of silly but these are here for the two scripts started,
// makes the second parameter passed in the array more clear as
// to what is going on...
enum SCRIPT_OPTS
	FALSE				:= 0x0,
	TRUE				:= 0x1
endenum