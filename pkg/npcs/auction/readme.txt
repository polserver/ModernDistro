auction  version 1.0
by Zulu (zulu@zuluhotel.com)   November 12, 2000

==================================================

1. Installing the auction system
   create a directory \pol\pkg\opt\auction\
   copy the files to that directory

2. If you have an old version of auction
   copy fixauction.src to \pol\scripts\textcmd\admin\
   when your shard starts, run fixauction
   when the script is done, the fixauction scripts can be deleted

3. Copy auc.src and barter.src to \pol\scripts\textcmd\player\
   players type .auc to join, bid, view, etc
   players type .barter <message> to send messages

4. Copy barterban.src to \pol\scripts\textcmd\gm\
   to use type .barterban and click on player
   it will automatically turn on and off barterban for that player

5. Copy auctioneer.src to \pol\scripts\ai\

6. Copy the follow to \pol\config\npcdesc.cfg

NpcTemplate auctioneer
{
    Name        <random> the Auctioneer
    script      auctioneer

    ObjType     0x190
    Color       33784
    TrueColor   33784
    Gender      0
    STR         200
    INT         200
    DEX         200
    HITS        200
    MANA        200
    STAM        200
    Magery      90
    Inscription 50
    Wrestling 50
    Tactics 50
    Meditation 90
    Privs       invul
    Settings    invul
    guardignore 1
    alignment   good
    speech 1
    Wrestling 100
    AttackSkillId Wrestling
    AttackSpeed 80
    AttackDamage 5d100
    dstart 10
}

