use npc;
use cfgfile;

var speech_cfg := ReadConfigFile("speechgroup");

function process_text(inspeech, mood, templatename)
  var retval;
  var elem;
  var tmpword;
  var words := SplitWords(inspeech);
  elem := speech_cfg[templatename];
  foreach word in words
    var wordgroup := GetConfigInt(elem, mood);
    if(!wordgroup)
      wordgroup := GetConfigInt(elem, "default");
    endif
    if(!wordgroup)
      wordgroup := templatename;
    endif
    elem := speech_cfg[wordgroup];
    tmpword := GetConfigString(elem,  word );
    if(tmpword)
      retval := tmpword;
    endif
  endforeach
  if(!retval)
    retval := GetConfigString(elem, "default" );
  endif
  if((!retval) || (retval == "5"))
    retval := "";
  endif
  return retval;
endfunction

function TakeItem(you,it)
  TurnToward(you);
  if(!GetObjProperty(me,"serial"))
	SetObjProperty(me, "serial", me.serial);
  endif
  var mypack := FindMyPack(me.serial);
  var whatdesc := "";
  if(!len(it.name))
    whatdesc := it.desc;
  else
	whatdesc := it.name;
  endif
  var keyitem := lower("keyitem" + whatdesc);
  var returnthing := lower("return" + whatdesc);
  var reply := GetObjProperty(me, keyitem);
  if(reply)
	TurnToward(you );
	say(reply);
	DestroyItem(it);
  else
    if(speech)
	  if(GetObjProperty(it,"slave"))
		TurnToward(you);
		GiveQuestieDirections(you, it);
	  else
		TurnToward(you);
		say("Eh? What's this?");
	  endif
	endif
	MoveItemToContainer(it,you.backpack);
	return;
  endif
  var gavething := 0;
  foreach item in EnumerateItemsInContainer(mypack)
	var returnit := GetObjProperty(item,"returnfor");
	if(returnit == returnthing)
	  gavething := 1;
	  EraseObjProperty(item,"returnfor");
	  MoveItemToContainer(item, you.backpack);
	endif
  endforeach
  if(!me.backpack)
	var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
	if(!EquipItem(me, newbackpack))
	  return;
	endif
  endif
  foreach item in EnumerateItemsInContainer(me.backpack)
	var returnit := GetObjProperty(item,"returnfor");
	if(returnit == returnthing)
	  gavething := 1;
	  EraseObjProperty(item,"returnfor");
	  MoveItemToContainer(item, you.backpack);
	endif
  endforeach
  if(!gavething)
	return;
  endif
  var newresponse := GetObjProperty( me, "done"+keyitem);
  if(newresponse)
	SetObjProperty(me, keyitem, newresponse);
	EraseObjProperty(me, "done" + keyitem);
  else
	EraseObjProperty(me,keyitem);
  endif
endfunction

function check_speech(text, byref speaker)
	if (!speech)
		return;
	endif
	if (BarkToKeyWord(text,speaker))
		return;
	endif
	if (DefaultHello(text,speaker))
		return;
	endif
	if ((GetObjProperty(me,"nofriends")) || (RandomInt(4)!=1) )
		return;
	endif
	if (speaker.cmdlevel)
		return;
	endif
	var myfriends := { };
	var i := 0;
      for( i := 1; i <= 3; i := i+1 )
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			break;
		elseif (myfriends[i] == speaker.name)
			TurnToward( speaker );
			GreetFriend(text,speaker.name,i);
			return;
		endif
	  endfor
var maxfriend := i - 1;
	if (i > 3)
		//too many friends, pick a slot at random
		i := RandomInt(3)+1;
		maxfriend := 3;
	endif
 var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty:=0;
	endif
	if ( RandomInt(20) <= (loyalty+10) )
		//don't ditch the old friend
		TurnToward( speaker );
		i := RandomInt(maxfriend)+1;
		ChatAboutFriends(text,speaker, i);
		return;
	endif
	var nexthello := GetObjProperty(me,"nexthello");
	if (!nexthello)
		nexthello := ReadGameClock();
	endif
	if ( nexthello <= ReadGameClock() )
		TurnToward( speaker );
		say("Say, " + speaker.name + ", You sure do look familiar...");
		SetObjProperty(me,"friend"+i,speaker.name);
		SetObjProperty(me,"loyalty"+i,0);
		SetObjProperty(me,"nexthello",ReadGameClock()+300);
	else
		TurnToward( speaker );
		i := RandomInt(maxfriend)+1;
		ChatAboutFriends(text,speaker, i);
	endif
endfunction

function GreetFriend(text, friend, friendnum)
	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	var nexthello := GetObjProperty(me,"nexthello");
	if (!loyalty)
		loyalty := 0;
	endif
	if (!nexthello)
		nexthello := ReadGameClock();
	endif
	if (loyalty >= 3)
		friend := "My "+ GetFriendTitle(loyalty) + friend;
	endif
	case (RandomInt(40))
	0:	say("Oh, " + friend + ", how's life?");
	1:	say("Surely, " + friend + ", I can be of great assistance.");
	2:	say("Glad you're back again, " + friend + ".");
	3:	say(friend + ", tell me about your latest adventures.");
	4:	say("Oh, " + friend + ", you're getting to be a real regular here, eh?");
	5:	say("Well, " + friend + ", I'm glad you came back.");
	6:	say("Always good to see you, " + friend);
	7:	say(friend + ", what I can do to make your travels easier?");
	8:	say("Nice to see you again, " + friend );
	9:	say("Good to see you, " + friend);
	10:	say( friend + ", I'm glad you came to see me.");
	11:	say( friend + ", it's always good to see you!");
	12:	say( friend + ", I hope you are well.");
	13:	say( "May life treat you right, " + friend);
	14:	say("Say, " + friend + " nice weather lately, isn't it?" );
	15:	say( friend + ", sometimes I wonder about you.");
	16:	say(friend + ", it's always a pleasure to serve you");
	17:	say(friend + ", you are always welcome.");
	18:	say("Well aren't you something, " + friend + "!");
	19:	say(friend + ", your presence brightens my day");
	20:	say(friend + " it's far too long between visits!");
	21:	say("So, " + friend + " care to stay and chat a while?" );
	22:	say(friend);
   default:	say(process_text(text, "default",speech));
	endcase
	if ( RandomInt(3) == 1 )
		loyalty := loyalty + 1;
		if (loyalty > 9)
			loyalty := 9;
		endif
	endif
	if ( nexthello <= ReadGameClock() )
		SetObjProperty(me,"nexthello",ReadGameClock()+300);
		SetObjProperty(me,"loyalty"+friendnum,loyalty);
	endif
endfunction

function ChatAboutFriends(text, byref speaker, friendnum)
	speaker := speaker; // stops "var not used" ecompile warning
	var loyalty := GetObjProperty(me,"loyalty"+friendnum);
	if (!loyalty)
		loyalty := 0;
	endif
	var friend := GetObjProperty(me,"friend"+friendnum);
	if (!friend )
		say(process_text(text, "default",speech));
		return;
	endif
	if (loyalty >= 3)
		friend := "My "+ GetFriendTitle(loyalty) + friend;
	endif
	case (RandomInt(50))
	1:	say(friend + " comes in here a lot.");
	2:	say(friend + " is quite a tradesperson.");
	3:	say("I don't see enough of " + friend + ".");
	4:	say(friend + " has been about town for a while now.");
	5:	say("Have you met " + friend + "?");
	6:	say("I've been swapping tales with " + friend + ".");
	7:	say(friend + " seems to be doing well.");
	8:	say("I'm hoping to see " + friend + " soon.");
	9:	say("You should get together with " + friend + " and me at the pub sometime.");
	10:	say("You remind me of " + friend);
	11:	say(friend + " is one of my best customers.");
	12:	say("Do you know " + friend + "?  Nice person!");
	13:	say("Did you hear " + friend + "'s death cry?  What a tragedy!");
	14:	say(friend + "'s achievements are becoming legendary.");
	15:	say("I'm proud to have a friend like " + friend );
	16:	say(friend + "'s antics are scripts for the bards.");
	17:	say(friend + " sure is nice.");
	18:	say("I believe " + friend + " may be stepping out with my neighbor.");
	19:	say("You should look " + friend + " up.");
	default:
		say(process_text(text, "default",speech));
	endcase
endfunction

function GetFriendTitle(loyalty)
  case (loyalty)
	3:
	4: return "Friend, ";
	5: return "Good Friend, ";
	6: return "Great Friend, ";
	7: return "Old Friend, ";
	8: return "Dear Friend, ";
	9: return "Dearest Friend, ";
	default: return "";
  endcase
endfunction

function YellToFriend(friend)
	if (friend.isA(POLCLASS_NPC))
		return;
	endif
	var myfriends := { };
	var i := 0;
	var afriend := 0;
        for( i := 1; i <= 5; i := i + 1 )
		myfriends[i] := GetObjProperty(me,"friend"+i);
		if (!myfriends[i])
			break;
		elseif (myfriends[i] == friend.name)
			afriend := 1;
			break;
		endif
	endfor
	if (!afriend)
		return;
	endif
	TurnToward(friend);
	var loyalty := GetObjProperty(me,"loyalty"+i);
	if (!loyalty)
		loyalty := 0;
	endif
	var friendname := friend.name + "!";
	if (loyalty >= 3)
		friendname := "My "+ GetFriendTitle(loyalty) + friendname;
	endif
	TurnToward( friend );
	say(friendname);
endfunction

function BarkToKeyWord(text, byref speaker)
	var words := SplitWords(text);
	var tmpword;
	var theword;
	var resp := 0;
	foreach word in words
		tmpword := GetObjProperty( me, "keyword"+ lower(word));
		if (tmpword)
			TurnToward( speaker );
			say(tmpword);
			theword := lower(word);
		  	resp := 1;
			break;
		endif
	endforeach
	if (!resp)
		return 0;
	endif
	if (!GetObjProperty(me,"serial"))
		SetObjProperty(me, "serial", me.serial);
	endif
	var mypack := FindMyPack(me.serial);
	var returnthing := "return" + theword;
	var doneword := "done" + theword;
	var keyword := "keyword" + theword;
	var gavething := 0;
	foreach item in EnumerateItemsInContainer(mypack)
		var returnit := GetObjProperty(item,"returnfor");
		if (returnit == returnthing)
			gavething := 1;
			EraseObjProperty(item,"returnfor");
			MoveItemToContainer(item, speaker.backpack);
		endif
	endforeach
	if (!me.backpack)
		var newbackpack := CreateItemAtLocation(me.x, me.y, me.z, 0xe75,1);
		if (!EquipItem(me, newbackpack))
			return;
		endif
	endif
	foreach item in EnumerateItemsInContainer(me.backpack)
		var returnit := GetObjProperty(item,"returnfor");
		if (returnit == returnthing)
			gavething := 1;
			EraseObjProperty(item,"returnfor");
			MoveItemToContainer(item, speaker.backpack);
		endif
	endforeach
	if (!gavething)
		return 1;
	endif
	var newresponse := GetObjProperty( me, doneword);
	if (newresponse)
		SetObjProperty( me, keyword, newresponse);
		EraseObjProperty( me, doneword);
	else
		EraseObjProperty( me, keyword);
	endif
	return 1;
endfunction

function DefaultHello(text, byref speaker)
	text := text; // stops "var not used" ecompile warning
	if ( GetObjProperty(me,"#lastgreet") )
		if ( ReadGameClock() > GetObjProperty(me,"#lastgreet") )
			BarkToKeyWord("hi",speaker);
			SetObjProperty(me,"#lastgreet",ReadGameClock()+30);
			return 1;
		else
			return 0;
		endif
	else
		BarkToKeyWord("hi",speaker);
		SetObjProperty(me,"#lastgreet",ReadGameClock()+30);
		return 1;
	endif
endfunction

function GiveQuestieDirections(you, it)
	var questie := SystemFindObjectBySerial(GetObjProperty(it,"slave"));
	var ns_desc := "";
	var ew_desc := "";
	var dir;
	if (questie.y < you.y)
        	ns_desc := "north";
	elseif  (questie.y > you.y)
		ns_desc := "south";
	endif
	if (questie.x < you.x)
		ew_desc := "west";
	elseif  (questie.x > you.x)
		ew_desc := "east";
	endif
	if (ns_desc == "" && ew_desc == "")
		dir := "nearby";
	else
		dir := ns_desc + ew_desc;
	endif
	say("Looking for " + questie.name + "?  I think they are " + dir );
endfunction