use os;

include ":chat:settings";

function CheckForValidChars(conference_name)
	// Be sure to check config if you use non-english keyboard layouts.
	var settings_elem := CH_GetSettingsCfgElem("Settings");
	var valid_chars := GetConfigString(settings_elem, "ValidConfChars");
	conference_name := lower(conference_name);
	
	for i := 1 to Len(conference_name)
		if ( Find(valid_chars, conference_name[i], 1) == 0 )
				Print("Found invaild char: "+conference_name[i]);
				return 0;
		endif
	endfor
	
	return 1;
endfunction

function CreateConference(conference_name, conference_pass:="", character:="")	
	// I don't know how OSI validates conference names, this is just off of the top of my head
	var settings_elem := CH_GetSettingsCfgElem("Settings");
	if ( len(conference_name) < GetConfigInt(settings_elem, "MinConfNameLength") ||
	     len(conference_name) > GetConfigInt(settings_elem, "MaxConfNameLength") || 
	     !CheckForValidChars(conference_name) )
		if ( character )
			SendChatClilocMessage(character, 0x0007); // invalid conf name
			Print("Invalid conf name.");
		else
			Print("CreateConference() error: Invalid conference name.");
		endif
		return 0;
	endif
	
	// Check if conference of this name already exists
	Print("Checking if conf already exists: "+CH_GetProp("Conferences", conference_name));
	if ( CH_GetProp("Conferences", conference_name) != error )
		if ( character )
			SendChatClilocMessage(character, 0x0008); // conf already exists with that name
		else
			Print("CreateConference() error: Conference of that name already exists.");
		endif
		return 0;
	endif
	
	var settings := dictionary{"pass" -> conference_pass};
	if ( CH_SetProp("Conferences", conference_name, settings) )
		// Send new conference to everyone ("" = everyone because of SendChatPacket())
		SendConferences("", conference_name, conference_pass);
		return 1;
	else
		Print("Error creating conference: "+conference_name);
		return 0;
	endif
endfunction

function SendConferences(targ:="", conference_name:="", conference_pass:="")
	var conferences := CH_GetDataElem("Conferences");
	var packet;
	
	if ( conference_name )
		if ( CH_CheckDebug() )
			Print("Sending conferences ("+conference_name+") to "+targ);
		endif

		SendChatPacket(CreateConferencePacket(conference_name, conference_pass), targ);
	else	
		foreach conference in (conferences.PropNames())
			conferences := CH_GetProp("Conferences", conference);
			conference_pass := conferences.pass;

			if ( CH_CheckDebug() )
				Print("Sending conferences ("+conferences.name+") to "+targ);
			endif

			// Send packet targ, if targ = "" it goes to everyone
			SendChatPacket(CreateConferencePacket(conference, conference_pass), targ);
		endforeach
	endif
	
endfunction

function CreateConferencePacket(conference_name, conference_pass)
	// Create conference packet and send it to everyone chatting
	var packet := CreatePacket(0xB2, 15 + (Len(conference_name) * 2));
	packet.SetInt16(1, 15 + (Len(conference_name) * 2)); // packet length
	packet.SetInt16(3, 0x03E8); // create conference subcmd
	packet.SetUnicodeString(9, CAscZ(conference_name), 0); // unicode conference name

	if ( conference_pass )
		packet.SetInt16(11 + (Len(conference_name) * 2), 0x31); // password required
	else
		packet.SetInt16(11 + (Len(conference_name) * 2), 0x30); // no password
	endif

	return packet;
endfunction

// Checks password, sends join packet to player and adds them to datafiles.
function AddPlayerToConference(character, conference_name, conference_pass:="")
	Print("Adding player to conference: "+character+" "+conference_name+" "+conference_pass);
	
	if ( conference_pass )
		var conference := CH_GetProp("Conferences", conference_name);
			
		if ( conference.pass != conference_pass )
			SendChatClilocMessage(character, 0x000E); // incorrect password
			return 0;
		endif
	endif
	
	var join_packet := CreatePacket(0xB2, 13 + (Len(conference_name) * 2));
	join_packet.SetInt16(1, 13 + (Len(conference_name) * 2)); // packet size
	join_packet.SetInt16(3, 0x03F1); // you have joined message
	join_packet.SetUnicodeString(9, CAscZ(conference_name), 0);
	join_packet.SendPacket(character);

	CH_AddPlayerToConference(character, conference_name);

	return 1;
endfunction

// Handles messagetypes 0x0001 - 0x0024 & 0x0028 - 0x002C
// Targ can be array of charrefs or charref because SendChatPacket handle's both
function SendChatClilocMessage(targ, type, text1:="", text2:="")
	var packet := CreatePacket(0xB2, MSGLEN_VARIABLE);
	packet.SetInt16(3, type); // message type
	packet.SetInt32(5, 0); // unknown
	
	if ( text1 )
		packet.SetUnicodeString(9, CAscZ(text1)); // text for %1
	endif
	
	if ( text2 )
		packet.SetInt16(packet.GetSize(), 0); // null
		packet.SetUnicodeString(packet.GetSize(), CAscZ(text2)); // text for %2
	endif
	
	packet.SetInt16(packet.GetSize(), 0); // null
	packet.SetInt16(1, packet.GetSize()); // packet length
	
	Print("Sending cliloc type:"+type+" is:"+packet);

	SendChatPacket(packet, targ);
endfunction

function DestroyConference(conference_name)
	/*
	MessageType 0x03E9 (Destroy conference): 
	BYTE[4] Unknown (00 00 00 00) 
	BYTE[?] Unicode Channel Name 
	BYTE[4] Unknown (00 00 00 00) 
	*/
	
	// kick players out
	// remove from datafiles
endfunction


// Handles 0x0025 (Message), 0x0026 (Emote), 0x0027 (OOC)
// from_type: 0x0030 = user, 0x0031 = moderator, 0x0032 = muted, 0x0034 = me, 0x0035 = system 
function SendServerMessage(targ, type, from, message)
	var packet := CreatePacket(0xB2, MSGLEN_VARIABLE);
	packet.SetInt16(3, type); // message type
	//packet.SetString(5, targ.uclang, 1); // language code is set via SendChatPacket()
	packet.SetInt16(8, 0); // null terminator

	// todo: Determine from type (user, mod, etc...)
	var from_type;
	if ( from == "" )
		from_type := 0x0035; // system
	else
		from_type := 0x0030; // user
	endif
	
	packet.SetInt16(10, from_type);
	packet.SetUnicodeString(12, CAscZ(from.name)); // from username
	packet.SetInt16(packet.GetSize(), 0); // null terminator
	packet.SetUnicodeString(packet.GetSize(), CAscZ(message)); // message
	packet.SetInt16(packet.GetSize(), 0); // null terminator

	SendChatPacket(packet, targ, "", 5, 5, "uclang");
	
	return 1;	
endfunction