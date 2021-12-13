/* $Id: goTemplate.inc 375 2006-06-17 19:26:32Z austinheilman $
 *
 */


function Go_CheckInit(who)

        if ( !go_prop_base )
                SendSysMessage(who, "Creating initial go list dictionary...");
                var oldlocs := ReadConfigFile(":commands:config/golocs");
                var MaxLocs := GetConfigMaxIntKey(oldlocs);
                go_prop_base := dictionary;
                for i:=1 to MaxLocs
                        // Open files and set veriables
                        var dest := oldlocs[i];
						var facet := dest.realm;
						if( !facet )
							facet := _DEFAULT_REALM;
						endif
                        Go_SetLocation("base_Distro", who, dest.name, facet, dest.x, dest.y, dest.z);
                        SleepMS(2);
                endfor

                go_prop_base := go_elem.GetProp("base_Distro");
                go_keys_base := go_prop_base.keys();
                Go_RefreshGump("LocationIndex", "base_Distro");
        endif

        return 0;

endfunction

function Go_RefreshMyGumps(who)

        var elem := DFFindElement(go_data_gumps, "LocationIndex");
        elem.EraseProp("base_Distro");
        elem.EraseProp(who.acctname);

        elem := DFFindElement(go_data_gumps, "CopyIndex");
        elem.EraseProp("base_Distro");
        elem.EraseProp(who.acctname);

        return 1;

endfunction