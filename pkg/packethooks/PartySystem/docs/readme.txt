[ - What is this? -]
A package to handle party system packets. It can handle all client-based party commands and status window updates (mana, stamina). However, extended functionality such as karma/fame sharing, party corpse looting criminality, faction stuff and area effect spells not damaging party members is not easily packaged so it's up to you if you even want your shard to have this functionality and to code it.


[ - Changes - ]
1.3:
  Fixed careless bugs I missed after moving around a bunch in an effort to optimize.
  Fixed issues with parties >2 people. I was all alone while testing this and didn't want to run 3 or 4 clients.
  Note: I know of a few people (including Repsak) who use this with 096 and it works OK, so I'm changing core required to 096.
        Let me know if you have issues.
  Also note: I make mention of this in the source as well. No packet guide even documents the invite packet that I use so no guide
             has any mention of a decline invite, which I think is what I need. If someone invites you and it times out, and you type
             /accept, the client still thinks it's being invited but the script makes an additional check. If I knew a way to timeout
             the invite or decline the invite I wouldn't need to check anymore as it would be handled by the client.
  Thanks to Repsak for testing, reporting and suggesting fixes for this version.
        
1.2:
  UO.com says to use "/t#" for private messaging which is handled client-side so "/#" support was removed (stratics lied to me)
  Fixed a bug with private messages to someone in party position 10
  Changed spacing and variable names to make it more like the distro

1.1:
  Added support for /# private messanging
    # is position in the party of the player you want to message, ex: "/1 hello" will send "hello" to the party leader
  Added support for the /add client command
  Removed SendStatus hook, it is not really necessary, requires the attributes package and conflicts with another package in the 096 distro
  Fixed a recursion bug (the one Pierce mentions)
  Fixed lots of little small bugs
  Removed the dependency on the new attributes package
  Party messages now use specific party packets (instead of SendSysMessageUC())
  Tried to improve some of the comments...
  Removed my buggy and incomplete onCorpseRemove, since the 096 distro will handle this somewhere else

1.0:
  Crappy initial release :)
  

[ - Installation - ]
Copy the files over to your packages directory and compile.

On OSI, parties share fame and karma, can loot mobs together, may be able to loot each other's corpses, cannot compose of rival factions, and area effect spells do not damage other party members. I'll add this functionality if Austin says it's OK, but it would make the distro more dependant on the party package and would make it more specific to OSI. You can read more about it here:
http://guide.uo.com/combat_2.html

If you have corpse looting criminality checks then you can add code to check if the owner of the corpse has PARTY_LOOT_PROP set to true and the person looting is in the party. For refrence, on OSI, you can loot a corpse if it is:
 - your own corpse
 - your agressor's corpse (someone who attacks you and you kill them)
 - a criminal/murderer's
 - fellow/rival guild member

By default this package uses the party message packets (subsub commands 3 and 4) to send messages to other party members. If they have PartyMessageColor=0 in uo.cfg then they will not be able to read the messages. You can fix this by telling all of your players to set it to PartyMessageColor=368 or you can open config.inc in the include directory and change USE_PACKETED_MESSAGES to 0. This will substitute the packets for SendSysMessageUC(<message>, 3, 368).


[ - Considerations - ]
Thanks to Max Sherr for his initial help getting this moving, to Kinetix who posted some code on Folko's old forums, to Aeros, grak, Pierce and Repsak who posted on the forums when they found something wrong and a massive thanks to all of the POL devs, especially Austin for answering all of my questions.


[ - TODO - ]
Get a packet log of someone playing on OSI using the party system to double check everything.


[ - Note - ]
While coding this I came accross two undocumented subsub commands for the party system: 5 and 7. From what I can tell, 5 works exactly like 4, which is send party message. Subsub command 7 is an "invite" command, that I might not be using correctly. If anyone could tell me the precise messages OSI sends and when it sends them for the party system, or perhaps a UOLOG (my wildest dream) of a party session, I would be very greatful and could make this package more OSI-like. The below information is my best guess, but it may be wrong.

Command 0xBF, subcommand 6:
	Subsubcommand 5: Tell full party a message? (Variable # of bytes) 
	· BYTE[4] id (of source) 
	· BYTE[n][2] Null terminated Unicode message. 
	Note: Server Message
	
	Subsubcommand 7: Send party invitation (4 bytes)
	· BYTE[4] id (of source) 
	Note: Server Message


[ - Contact - ]
Please send me any comments, questions or bugs.
E-mail: tekproxy@gmail.com
AIM: tekproxy