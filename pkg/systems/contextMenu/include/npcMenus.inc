use uo;
use npc;

include "include/sysEvent";

include ":contextMenu:menuCreator";
include ":contextMenu:clilocs";

function CMP_NPC_GenerateMenu( mobile, object )
	var menu := struct{ "menuOptions":=array , "callbacks" := array };
	// If the NPC is a tame npc and the caller is their master.
	if( object.master == mobile )
		addMenuOption( menu,  CMP_NPC_CMD_GUARD, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" guard me"});
		addMenuOption( menu,  CMP_NPC_CMD_FOLLOW, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" follow me"});
		addMenuOption( menu,  CMP_NPC_CMD_DROP, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" drop"});
		addMenuOption( menu,  CMP_NPC_ADD_FRIEND, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" add friend"});
		addMenuOption( menu,  CMP_NPC_CMD_KILL, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" kill"});
		addMenuOption( menu,  CMP_NPC_CMD_STOP, callback :=  struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" stop"});
		addMenuOption( menu,  CMP_NPC_CMD_TRANSFER, callback := struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" transfer"});
		addMenuOption( menu,  CMP_NPC_CMD_STAY, callback :=   struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" stay"});
		addMenuOption( menu,  CMP_NPC_CMD_RELEASE, callback :=   struct{"type":= "speech","npc" := object, "mobile":= mobile, "text":= object.name+" release"});
	endif

	return menu;

endfunction

function SendSpeechToNPC( mobile, npc, text )


	var ev := struct{
	"type" := SYSEVENT_SPEECH,
	"source" := mobile,
	"text" := text
	};
	SendEvent(npc, ev);

endfunction