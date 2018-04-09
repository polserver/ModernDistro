use uo;
use os;
use polsys;
use unicode;

include ":partySystem:config";
include ":partySystem:packetInfo";

CONST OFFSET_PARTY_SOURCE_PLAYERID := 6;
CONST OFFSET_PARTY_MESSAGE := 10;

CONST OFFSET_PARTY_REMOVE_NEWSIZE := 6;
CONST OFFSET_PARTY_REMOVE_PLAYERID := 7;
CONST OFFSET_PARTY_REMOVE_PLAYERIDS := 11;

// OSI has built-in messages for parties, and if you'd like to use those don't change this
// But if you want to have some special party messages, you'll probably want to turn this off
CONST USE_PACKETED_MESSAGES := 1;

function SendPartyMessage(targ, source, message, self_send:=0)
	if ( USE_PACKETED_MESSAGES )
		// Make sure message is unicode
		if ( Lower(TypeOf(message)) != "array" )
			message := CAscZ(message);
		endif
		
		// Create party message packet
		var packet := CreatePacket(MSGTYPE_PARTY, MSGLEN_VARIABLE);
		packet.SetInt16(OFFSET_PARTY_SUBCMD, SUBCMD_PARTY); // Set subcmd to Party
		packet.SetInt32(OFFSET_PARTY_SOURCE_PLAYERID, source.serial); // Set source serial
		packet.SetUnicodeString(OFFSET_PARTY_MESSAGE, message, 1); // Set unicode message
		packet.SetInt16(OFFSET_PARTY_MSGLEN, packet.GetLength()); // Set packet length
		
		// Send packet to party members if targ is an array, else send to targ
		if ( Lower(TypeOf(targ)) == "array" )
			packet.SetInt8(OFFSET_PARTY_SUBSUBCMD, 4); // Set subsubcmd to party message
			
			var member;

			foreach member_id in (targ)
				if ( self_send || source.serial != member_id )
					member := SystemFindObjectBySerial(member_id);
					if ( member )
						if ( DEBUG )
							Print("Sending party message to: "+member.name+" packet: "+packet);
						endif
						packet.SendPacket(member);
					endif
				endif

				SleepMS(2);
			endforeach
		else
			packet.SetInt8(OFFSET_PARTY_SUBSUBCMD, 3); // Set subsubcmd to private
			packet.SendPacket(targ);
		endif
	else
		// Send packet to party members if targ is an array, else send to targ
		if ( Lower(TypeOf(targ)) == "array" )
			var member;

			foreach member_id in (targ)
				if ( self_send || source.serial != member_id )
					member := SystemFindObjectBySerial(member_id);
					if ( member )
						SendSysMessageUC(member, CAscZ("["+source.name+"]: ")+message, member.uclang, 3, 368);
					endif
				endif

				SleepMS(2);
			endforeach
		else
			SendSysMessageUC(targ, CAscZ("["+source.name+"]: ")+message, targ.uclang, 3, 368);
		endif
	endif
	
	return 1;
endfunction

function SendRemovePacket(targ, kicked_id, party:=array{})
	if ( !targ.connected )
		return 0;
	endif

	// Build the remove packet
	var packet := CreatePacket(MSGTYPE_PARTY, MSGLEN_VARIABLE);
	packet.SetInt16(OFFSET_PARTY_SUBCMD, SUBCMD_PARTY); // Set subcmd to Party
	packet.SetInt8(OFFSET_PARTY_SUBSUBCMD, 2); // Set subsubcmd to Remove
	packet.SetInt8(OFFSET_PARTY_REMOVE_NEWSIZE, party.Size()); // Set new party size
	packet.SetInt32(OFFSET_PARTY_REMOVE_PLAYERID, kicked_id); // Set player's serial

	for i := 1 to (party.Size())
		if ( party[i] != kicked_id )
			packet.SetInt32(OFFSET_PARTY_REMOVE_PLAYERIDS + ((i - 1) * 4), party[i]);
		endif
		SleepMS(2);
	endfor

	packet.Set16(OFFSET_PARTY_MSGLEN, packet.GetSize()); // Set packet length
	
	if ( DEBUG )
		Print("Sending remove packet to: "+targ.name+ " ID: "+kicked_id+" - "+packet);
	endif
	
	packet.SendPacket(targ);
endfunction