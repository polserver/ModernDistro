
include ":customhousing:house";

//This was split off from the main house.inc file simply for neatness




///////////////////
//  determines if the given person if a friend of the house
///////////////////

function IsAFriend (sign, who)
	if (who.acctname == GetObjProperty (sign, "account"))
		return 1;
	endif

	var friends := GetObjProperty (sign, "housefriends");
	if (!friends)
		return 0;
	endif

	foreach key in (friends.keys())
		if (who.acctname == key)
			var tempvar := friends[key];
			if (tempvar[1] == "account" or tempvar[1] == "character")
				return 1;
			endif
		elseif (who.serial == key)
			var tempvar := friends[key];
			if (tempvar[1] == "account" or tempvar[1] == "character")
				return 1;
			endif
		endif
	endforeach 

	return 0;
endfunction




///////////////////
//  Some players aren't really friends, but are allowed to place merchants
///////////////////

function IsMerchantHouseFriend (sign, theacctname)
	if (theacctname == GetObjProperty (sign, "account"))
		return 1;
	endif

	var friends := GetObjProperty (sign, "housefriends");
	if (!friends)
		return 0;
	endif
	
	if (theacctname in (friends.keys()))
		return 1;
	endif

	return 0;
endfunction




///////////////////
//  This function really should be in sign.src, but it was messy, so I stuck it here
///////////////////

function AdjustHouseFriendsFromSign (character, sign, friendnum)
	if (friendnum > 8)
		SendSysMessage (character, "Error: invalid gump return/friend number!");
		return;
	endif

	var friends := GetObjProperty (sign, "housefriends");
	if (!friends)
		friends := dictionary;
	endif
	
	var friendkeys := friends.keys();

	//see if they wanted to remove a friend
	if (friendnum <= friendkeys.size())
		var chosenkey := friendkeys[friendnum];
		if (!chosenkey)
			SendSysmessage (character, "Error!  Can't find friend key!");
			return;
		endif
		
		var chosenfriend := friends[chosenkey];
		if (!chosenfriend)
			SendSysmessage (character, "Error!  Can't find friend from selected key!");
			return;
		endif

		SendSysMessage (character, "Are you sure you want to remove " + chosenfriend[2] + " as a house friend?");
		if (!YesNo (character, "Remove?"))
			SendSysMessage (character, "Canceled.");
			return;
		endif
		
		friends.erase (chosenkey);
		SetObjProperty (sign, "housefriends", friends);
		SendSysMessage (character, "Friend removed.");
		return;
	endif
	
	//Otherwise we're adding a new friend
	SendSysMessage(character, "Who do you want to add as a friend?");
	var friend := Target (character);
	if (!friend)
		SendSysMessage (character, "Canceled.");
		return;
	elseif (!friend.acctname)
		SendSysMessage (character, "Invalid target!  Only players can be house friends");
		return;
	endif
	
	if (friend.acctname in friendkeys)
		SendSysMessage (character, "That player's account is already a friend of the house.");
		return;
	elseif (friend.serial in friendkeys)
		SendSysMessage (character, "That player is already a friend of the house.  If you want to change their options, remove them first.");
		return;
	endif
	
	var friend_gump_layout := array {
		"page 0",
		"nodispose",
		"resizepic 50 50 2620 310 250",
		"text 70 59 40 0",
		"page 1",

		"button 70 95 2118 2117 1 1 1",
		"text 100 90 40 1",
		"button 70 115 2118 2117 1 1 2",
		"text 100 110 40 2",
		"button 70 135 2118 2117 1 1 3",
		"text 100 130 40 3"
	};
	
	var friend_gump_data := array {
	       "Select friend type:",         //0
	       "Single character friend",
	       "Set account as friend",
	       "Merchant-only rights"
	};
	
	var friend_gump_return := SendDialogGump (character, friend_gump_layout, friend_gump_data);
	if (!friend_gump_return or !friend_gump_return[0])
		SendSysMessage (character, "Canceled.");
		return;
	endif
	
	var newfriendentry := Array {};
	case (friend_gump_return[0])
		1:	newfriendentry := {"character", friend.name};
			friends.insert (friend.serial, newfriendentry);
			SetObjProperty (sign, "housefriends", friends);
			SendSysMessage (character, "New friend added!");
			return;
		2:	newfriendentry := {"account", friend.name};
			friends.insert (friend.acctname, newfriendentry);
			SetObjProperty (sign, "housefriends", friends);
			SendSysMessage (character, "New friend account added!");
			return;
		3:	newfriendentry := {"merchant", friend.name};
			friends.insert (friend.acctname, newfriendentry);
			SetObjProperty (sign, "housefriends", friends);
			SendSysMessage (character, "New friend account added!");
			return;
		default:
			SendSysmessage (character, "Error:  Invalid gump return");
			return;
	endcase
endfunction