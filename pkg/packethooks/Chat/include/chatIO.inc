use datafile;

include ":datafile:datafile";

function CH_GetDataFile()
	return DFOpenDataFile(":chat:settings", DF_CREATE);
endfunction

function CH_GetDataElem(elem_name)
	var data_file := CH_GetDataFile();
	return DFFindElement(data_file, elem_name, DF_CREATE);
endfunction

function CH_GetProp(elem_name, prop_name:="")
	var settings_elem := CH_GetDataElem(elem_name);

	if ( prop_name )
		return settings_elem.GetProp(prop_name);
	else
		var props := array{};
		foreach prop in (settings_elem.PropNames())
			props.append(settings_elem.GetProp(prop));
		endforeach
		return props;
	endif
endfunction

function CH_SetProp(elem_name, prop_name, value)
	var settings_elem := CH_GetDataElem(elem_name);
	return settings_elem.SetProp(prop_name, value);
endfunction

function CH_ModifyProp(elem_name, prop_name, value)
	var settings_elem := CH_GetDataElem(elem_name);
	var current_value := CH_GetProp(elem_name, prop_name);
	
	if ( current_value == error )
			current_value := "";
	endif
	
	return settings_elem.SetProp(prop_name, current_value+value);
endfunction

function CH_EreaseProp(elem_name, prop_name)
	var settings_elem := CH_GetDataElem(elem_name);
	return settings_elem.EreaseProp(prop_name);
endfunction

function CH_AddPlayerToChat(serial)
	var online := CH_ListPlayers();
	
	if ( Lower(TypeOf(online)) != "array" )
		online := array{};
	endif

	if ( !(serial in online) )
		online.Append(serial);
	endif
	
	CH_SetProp("Settings", "Online", online);
endfunction

function CH_RemovePlayerFromChat(serial)
	var online := CH_ListPlayers();
	
	if ( Lower(TypeOf(online)) != "array" )
		online := array{};
	endif

	for i := 1 to (online.Size())
		if ( online[i] == serial )
			online.Erase(i);
		endif
	endfor
	
	CH_SetProp("Settings", "Online", online);
endfunction

function CH_ListPlayers()
	var online := CH_GetProp("Settings", "Online");
	return online;
endfunction
		
function CH_AddPlayerToConference(character, conference_name)
	var conference := CH_GetProp("Conferences", conference_name);
	(conference.joined).Append(character.serial);
	
	SetObjProperty(character, "ChatConference", conference_name);
	
	return CH_SetProp("Conferences", conference_name, conference);
endfunction

// Todo: I think players can only be in one conf, so just use the prop
function CH_RemovePlayerFromConference(character, conference_name)
	var conference := CH_GetProp("Conferences", conference_name);
	for i := 1 to ((conference.joined).Size())
		if ( (conference.joined)[i] == character.serial )
			(conference.joined).Erase(i);
		endif
	endfor
	
	EraseObjProperty(character, "ChatConference");
	
	return CH_SetProp("Conferences", conference_name, conference);
endfunction

function CH_GetPlayerConference(character)
	return GetObjProperty(character, "ChatConference");
endfunction

// CH_AddPlayerToIgnoreList
// CH_RemovePlayerFromIgnoreList
