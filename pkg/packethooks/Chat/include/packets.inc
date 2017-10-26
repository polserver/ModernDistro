include ":chat:chatIO";
include ":chat:settings";

function SendChatPacket(byref packet, targ:="", except:=array{}, modify_offset:=0, modify_type:=0, modify_with:="");
	// Targs can be nothing, a charref, array of character serials or a conference name
	var targs;
	if ( !targ ) // everyone
		targs := CH_GetProp("Settings", "Online");
	elseif ( Lower(TypeOf(targ)) == "charref" ) // character refrence
		targs := array{ targ.serial };
	elseif ( Lower(TypeOf(targ)) == "array" ) // array of serials
		targs := targ;
	elseif ( Lower(TypeOf(targ)) == "string" ) // conference
		targs := (CH_GetProp("Conferences", targ)).joined;
	endif
	
	var character;
	var modify_val;
	foreach serial in targs
		if ( serial in except )
			continue;
		endif
		
		character := SystemFindObjectBySerial(serial);
		Print("character:"+character);
		
		if ( character )
			if ( modify_offset && modify_with )
				case ( modify_with )
					"uclang":
						modify_val := character.uc_lang;
						break;
					default:
						modify_val := "";
						break;
				endcase
				
				case ( modify_type )
					0: packet.SetInt8(modify_offset, modify_val);
					1: packet.SetInt16(modify_offset, modify_val);
					2: packet.SetInt32(modify_offset, modify_val);
					3: packet.SetInt16Flipped(modify_offset, modify_val);
					4: packet.SetInt32Flipped(modify_offset, modify_val);
					5: packet.SetString(modify_offset, modify_val);
					6: packet.SetUnicodeString(modify_offset, modify_val);
					7: packet.SetUnicodeStringFlipped(modify_offset, modify_val);
				endcase
			endif
					
			if ( CH_CheckDebug() )
				Print("SendPacketToTarget(): "+character.name+" packet: "+packet);
			endif
			
			packet.SendPacket(character);
		endif
	endforeach
	
	return targs.Size();
endfunction