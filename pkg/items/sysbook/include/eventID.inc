///////////////////////////////////////////
//
//	Event ID's used by the sysbook system
//
//////////////////////////////////////////

const EVID_SYSBOOK_BOOK_REMOVED	:=	0x2220;	//Triggered when a book is removed from the bookshelf.
const	EVID_SYSBOOK_START_SHELF	:=	0x2221;	//Triggered by a script or command to start the book shelf spawning again. (currently not written)
const	EVID_SYSBOOK_STOP_SHELF		:=	0x2222;	//Triggered by a script or command to stop the book shelf spawning and remove all books. (currently not written)
const	EVID_SYSBOOK_PAUSE_SHELF	:=	0x2223;	//Triggered by a script or command to pause the book shelf spawning but leave exisiting books there. (currently not written)