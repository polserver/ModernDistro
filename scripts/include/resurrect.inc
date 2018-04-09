function ResurrectWithPenalty(who)
 var lastresed := Cint(GetObjProperty(who, "#lastresed"));
    var deathcount := Cint(GetObjProperty(who, "#deathcount"));

    if(Cint(GetGlobalProperty("#IgRound")) && GetObjProperty(who, "IgTeam" ))
        SendSysMessage(who, "To resurrect you have to wait until the end of the round.",3,40);
        return 0;
    endif
    
    if(ReadGameClock() < lastresed && deathcount >= 5)
        SendSysMessage(who, "Your soul is too weak to be resurrected. You must wait " + (lastresed - ReadGameClock()) + " seconds.",3,40);
        return 0;
    else
        Resurrect(who);
        SetMaxVitals(who);
        RevokePrivilege(who, "freemove");
        if(deathcount >= 5)
          EraseObjProperty(who, "#deathcount");
        endif
    endif
    return 1;
endfunction

function DestroyDeathShroud(who)
  var D_Robe := GetEquipmentByLayer( who, 22 );
  If( D_Robe )
    DestroyItem( D_Robe );
  Endif
endfunction

function SetMaxVitals(who)
        who.setlightlevel( 0, 2400 );
        SendSysMessage(who, "Welcome to the world of the living!",3,63);
        EraseObjProperty(who, "#lastresed");
        DestroyDeathShroud(who);
endfunction
