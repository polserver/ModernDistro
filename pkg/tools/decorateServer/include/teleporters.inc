use uo;
use os;
use cfgfile;


function CreateTeleporters(facet)

	var checker, teleporter, tele_elem;
	var LogFile := "::/log/DecorateFacets";

    var fromx;
    var fromy;
    var fromz;
    var fromrealm;
    var tox;
    var toy;
    var toz;
    var torealm;
    var telekeys := {};
    var cfg := ReadConfigFile(":decorateServer:teleporters");
    if(cfg.errortext)
      LogToFile(LogFile, "Cannot find teleporters.cfg", 1);
      return 0;
    endif
  facet := Lower(facet);

    telekeys := GetConfigIntKeys( cfg );
    foreach key in telekeys
      tele_elem := FindConfigElem( cfg, key );
      fromx := GetConfigInt( tele_elem, "fromx" );
      fromy := GetConfigInt( tele_elem, "fromy" );
      fromz := GetConfigInt( tele_elem, "fromz" );
      fromrealm := GetConfigString( tele_elem, "fromrealm" );
      if(Lower(fromrealm) == facet)
        // Not sure why either of the following lines were used but left them here just in case.
    //		fromz := GetMapInfo(fromx, fromy).z;
    //		fromz := GetMapInfo(fromx, fromy, fromrealm).z;
        fromz := CInt(GetStandingHeight( fromx, fromy, fromz, fromrealm ).z);
        foreach thing in ListItemsNearLocationOfType(fromx, fromy, fromz, 0, 0x16200, fromrealm);
          thing := thing; // Stop stupid 'unused variable' compiler warning!
          checker := 1;
        endforeach
        if(checker == 0)
          tox := GetConfigInt( tele_elem, "tox" );
          toy := GetConfigInt( tele_elem, "toy" );
          toz := GetConfigInt( tele_elem, "toz" );
  //        toz := GetMapInfo(tox, toy).z;
          torealm := GetConfigString( tele_elem, "torealm" );
          teleporter := CreateItemAtLocation(fromx, fromy, fromz, "systemteleporter", 1, fromrealm);
          if(teleporter)
            SetObjProperty(teleporter, "DestX", tox);
            SetObjProperty(teleporter, "DestY", toy);
            SetObjProperty(teleporter, "DestZ", toz);
            SetObjProperty(teleporter, "DestRealm", torealm);
          else
            LogToFile(LogFile, "Unable to create teleporter at " + CStr(fromx) + " " + CStr(fromy) + " " + CStr(fromz) + " " + CStr(fromrealm), 1);
          endif // teleporter

        endif     // checker == 0
        checker := 0;
      endif // fromrealm) == facet
    endforeach
    SetGlobalProperty("SystemTeleportersCreatedForFacet_" + facet, 1);
endfunction












/*


    var maxint := GetConfigMaxIntKey(cfg);
    for(i:=407; i<=maxint; i:=i+1)
      tele_elem := FindConfigElem( cfg, i );
      fromx := GetConfigInt( tele_elem, "fromx" );
      fromy := GetConfigInt( tele_elem, "fromy" );
      fromz := GetConfigInt( tele_elem, "fromz" );
      fromrealm := GetConfigString( tele_elem, "fromrealm" );
  //		fromz := GetMapInfo(fromx, fromy).z;
  //		fromz := GetMapInfo(fromx, fromy, fromrealm).z;
      fromz := GetStandingHeight( fromx, fromy, fromz, fromrealm ).z;
      foreach thing in ListItemsNearLocationOfType(fromx, fromy, fromz, 0, 0x26200, fromrealm);
        checker := 1;
      endforeach
      if(checker == 0)
        tox := GetConfigInt( tele_elem, "tox" );
        toy := GetConfigInt( tele_elem, "toy" );
        toz := GetMapInfo(tox, toy).z;
        torealm := GetConfigString( tele_elem, "torealm" );
        teleporter := CreateItemAtLocation(fromx, fromy, fromz, "systemteleporter", 1, fromrealm);
        if(teleporter)
          SetObjProperty(teleporter, "DestX", tox);
          SetObjProperty(teleporter, "DestY", toy);
          SetObjProperty(teleporter, "DestZ", toz);
          SetObjProperty(teleporter, "DestRealm", torealm);
        else
          Print("Unable to create teleporter at " + CStr(fromx) + " " + CStr(fromy) + " " + CStr(fromz) + " " + CStr(fromrealm));
        endif
        if(i==409)
          Print("teleporter at " + CStr(fromx) + " " + CStr(fromy) + " " + CStr(fromz) + " " + CStr(fromrealm));
        endif
      endif
      checker := 0;
    endfor
    SetGlobalProperty("SystemTeleportersCreated", 1);
endfunction
*/
function CreateDungeonTeleporters(facet)

  foreach tele in teleporters
    var fromx := tele[1];
    var fromy := tele[2];
    var fromz := tele[3];

    var tox := tele[4];
    var toy := tele[5];
    var toz := tele[6];
    var teleporter := CreateItemAtLocation(fromx, fromy, fromz, "systemteleporter", 1);
    if(teleporter)
      teleporter.graphic := 3948;
      SetObjProperty(teleporter, "DestX", tox);
      SetObjProperty(teleporter, "DestY", toy);
      SetObjProperty(teleporter, "DestZ", toz);
      SetObjProperty(teleporter, "DestRealm", facet);
    endif
  endforeach

endfunction
