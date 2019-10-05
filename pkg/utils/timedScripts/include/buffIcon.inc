/*
        Created by *Edwards

        2010-09-26
*/
use uo;
use polsys;


function UpdateBuffIcon( mobile, byref cfg_elem, byref event )

        var buff_icon := CInt( cfg_elem.BuffIcon );
        // This section is my attempt to read cliloc info from the cfg file.
        // var cliloc_name := CInt( cfg_elem.cliloc_name );
        // if(cliloc_name.errortext)
        //   Print(cliloc_name.errortext);
        // else
        //   Print(CStr(cliloc_name));
        // endif
        // var cliloc_desc := CInt( cfg_elem.cliloc_desc );
        //////////////////////////////////////////////////////////////////////
        if( buff_icon )
                var level := event.level,
                    amount := event.amount;
                
                // SendBuffIcon( mobile, buff_icon, level, amount, cliloc_name, cliloc_desc ); // cfg cliloc stuff.
                SendBuffIcon( mobile, buff_icon, level, amount );
        endif
        
        return 1;
endfunction

// function SendBuffIcon( mobile, icon, modifier, duration, cliloc_name, cliloc_desc ) // cfg cliloc stuff.
function SendBuffIcon( mobile, icon, modifier, duration )

        Print("Buff icon is " + CStr(icon));

        if( mobile.IsA( POLCLASS_NPC ))
                return 0;
        endif

        var show := 0;
        if( duration )
                show := 1;
        endif

	var packet := CreatePacket( 0xDF, MSGLEN_VARIABLE );
	packet.SetInt32( 3, mobile.serial );
	packet.SetInt16( 7, icon );
	packet.SetInt16( 9, show );

        if( show )
                var clilocs := GetClilocsInfo( icon ),
                cliloc_name := clilocs[1],
                cliloc_desc := clilocs[2];

                packet.SetInt16( 15, icon );
                packet.SetInt16( 17, show );
                packet.SetInt16( 23, duration );
                packet.SetInt32( 28, cliloc_name );
                packet.SetInt32( 32, cliloc_desc );
                
                var text := "",
                    i;
                    
                for( i:=1; i<=3; i+=1 )
                        text := text+"\t";
                        text := text+CStr( modifier );
                        SleepMS(4);
                endfor

                packet.SetUnicodeStringFlipped( 42, CAscZ( text ), 1 );

                var Length_ID := packet.GetSize();
                packet.SetInt16( Length_ID, 0x1 );
        endif

        packet.SendPacket( mobile );

        return 1;
endfunction

function GetClilocsInfo( icon )

	var cliloc_name,
	    cliloc_desc;
             
        case( icon )
                1028:      //ReactiveArmor
                           cliloc_name := 1075812;
                           cliloc_desc := 1042971;
                           break;
                1029:      //Protection
                           cliloc_name := 1075814;
                           cliloc_desc := 1075815;
                           break;
                1035:      //Polymorph
                           cliloc_name := 1075824;
                           cliloc_desc := 1075820;
                           break;
                1040:      //Clumsy
                           cliloc_name := 0x106a77;
                           cliloc_desc := 0x106a78;
                           break;
                1041:      //Feeblemind
                           cliloc_name := 0x106a79;
                           cliloc_desc := 0x106a7a;
                           break;
                1042:      //Weaken
                           cliloc_name := 1075837;
                           cliloc_desc := 1075838;
                           break;
                1043:      //Curse
                           cliloc_name := 1075835;
                           cliloc_desc := 0x106a88;
                           break;
                1045:      //Agility
                           cliloc_name := 0x106a81;
                           cliloc_desc := 0x106a82;
                           break;
                1046:      //Cunning
                           cliloc_name := 0x106a83;
                           cliloc_desc := 0x106a84;
                           break;
                1047:      //Strength
                           cliloc_name := 0x106a85;
                           cliloc_desc := 0x106a86;
                           break;
                1048:      //Bless
                           cliloc_name := 0x106a87;
                           cliloc_desc := 0x106a88;
                           break;
                1037:      //Paralyze
                           cliloc_name := 1075827;
                           cliloc_desc := 1075828;
                           break;
                1036:      //Invisibility
                           cliloc_name := 1075825;
                           cliloc_desc := 1075826;
                           break;
                1038:      // Poison
                           cliloc_name := 0x0F8627;
                           cliloc_desc := 0x1069B1;
                           break;

        endcase
        
        return array{cliloc_name, cliloc_desc};
endfunction

/*

   "dismount":
         BuffIcon := 1001;
         cliloc_name := 1075635;
         cliloc_desc := 1075636;
   "disarm":
          BuffIcon := 1002;
         cliloc_name := 1075637;
         cliloc_desc := 1075638;
   "nightsight":
          BuffIcon := 1005;
         cliloc_name := 1075643;
         cliloc_desc := 1075644;
   "deathstrike":
         BuffIcon := 1006;
   "evilomen":
         BuffIcon := 1007;
   "dontknow1":
         BuffIcon := 1008;
   "regeneration":
         BuffIcon := 1009;
         cliloc_name := 1044106;
         cliloc_desc := 1075106;
   "divinefury":
         BuffIcon := 1010;
   "enemyofone":
         BuffIcon := 1011;
   "stealth":
         BuffIcon := 1012;
         cliloc_name := 1044107;
         cliloc_desc := 1075655;
   "meditation":
         BuffIcon := 1013;
         cliloc_name := 1044106;
         cliloc_desc := 1075106;
   "bloodoathcaster":
         BuffIcon := 1014;
   "bloodoathcurse":
         BuffIcon := 1015;
   "corpseskin":
         BuffIcon := 1016;
   "mindrot":
         BuffIcon := 1017;
   "painstrike":
         BuffIcon := 1018;
   "strangle":
         BuffIcon := 1019;
   "giftofrenewal":
         BuffIcon := 1020;
   "attunewapon":
         BuffIcon := 1021;
   "thunderstorm":
         BuffIcon := 1022;
   "essenceofwind":
         BuffIcon := 1023;
   "etheralvoyage":
         BuffIcon := 1024;
   "giftoflife":
         BuffIcon := 1025;
   "arcaneempowerment":
         BuffIcon := 1026;
   "mortalstrike":
         BuffIcon := 1027;
   "reactivearmor":
         BuffIcon := 1028;
         cliloc_name := 1075812;
         cliloc_desc := 1075813;
   "protection":
         BuffIcon := 1029;
         cliloc_name := 1075814;
         cliloc_desc := 1075815;
   "archprotection":
         BuffIcon := 1030;
         cliloc_name := 1075816;
         cliloc_desc := 1075816;
   "magicreflection":
         BuffIcon := 1031;
         cliloc_name := 1075817;
         cliloc_desc := 1075818;
   "incognito":
         BuffIcon := 1032;
         cliloc_name := 1075819;
         cliloc_desc := 1075820;
   "disguised":
         BuffIcon := 1033;
   "animalform":
         BuffIcon := 1034;
   "polymorph":
         BuffIcon := 1035;
         cliloc_name := 1075824;
         cliloc_desc := 1075820;
   "invisibility":
         BuffIcon := 1036;
         cliloc_name := 1075825;
         cliloc_desc := 1075826;
   "paralyse":
         BuffIcon := 1037;
         cliloc_name := 1075827;
         cliloc_desc := 1075828;
   "poison":
         BuffIcon := 1038;
         cliloc_name := 0x0F8627;
         cliloc_desc := 0x1069B1;
   "bleed":
         BuffIcon := 1039;
         cliloc_name := 0x106a75;
         cliloc_desc := 0x106a76;
   "clumsy":
         BuffIcon := 1040;
         cliloc_name := 0x106a77;
         cliloc_desc := 0x106a78;
   "feeblemind":
         BuffIcon := 1041;
         cliloc_name := 0x106a79;
         cliloc_desc := 0x106a7a;
   "weaken":
         BuffIcon := 1042;
         cliloc_name := 1075837;
         cliloc_desc := 1075838;
   "curse":
         BuffIcon := 1043;
         cliloc_name := 1075835;
         cliloc_desc := 1075836;
   "masscurse":
         BuffIcon := 1044;
         cliloc_name := 0x106a7f;
         cliloc_desc := 0x106a80;
   "agility":
         BuffIcon := 1045;
         cliloc_name := 0x106a81;
         cliloc_desc := 0x106a82;
   "cunning":
         BuffIcon := 1046;
         cliloc_name := 0x106a83;
         cliloc_desc := 0x106a84;
   "strength":
         BuffIcon := 1047;
         cliloc_name := 0x106a85;
         cliloc_desc := 0x106a86;
   "bless":
         BuffIcon := 1048;
         cliloc_name := 0x106a87;
         cliloc_desc := 0x106a88;
  endcase
*/
