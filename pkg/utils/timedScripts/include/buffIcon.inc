/*
        Created by *Edwards

        2010-09-26
*/
use uo;
use polsys;


function UpdateBuffIcon( mobile, byref cfg_elem, byref event )

        var buff_icon := CInt( cfg_elem.BuffIcon );
        if( buff_icon )
                var level := event.level,
                    amount := event.amount;
                
                SendBuffIcon( mobile, buff_icon, level, amount );
        endif
        
        return 1;
endfunction

function SendBuffIcon( mobile, icon, modifier, duration )

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
        endcase
        
        return array{cliloc_name, cliloc_desc};
endfunction

/*

   "dismount":
         tile := 1001;
         cliloc1 := 1075635;
         cliloc2 := 1075636;
   "disarm":
          tile := 1002;
         cliloc1 := 1075637;
         cliloc2 := 1075638;
   "nightsight":
          tile := 1005;
         cliloc1 := 1075643;
         cliloc2 := 1075644;
   "deathstrike":
         tile := 1006;
   "evilomen":
         tile := 1007;
   "dontknow1":
         tile := 1008;
   "regeneration":
         tile := 1009;
         cliloc1 := 1044106;
         cliloc2 := 1075106;
   "divinefury":
         tile := 1010;
   "enemyofone":
         tile := 1011;
   "stealth":
         tile := 1012;
         cliloc1 := 1044107;
         cliloc2 := 1075655;
   "meditation":
         tile := 1013;
         cliloc1 := 1044106;
         cliloc2 := 1075106;
   "bloodoathcaster":
         tile := 1014;
   "bloodoathcurse":
         tile := 1015;
   "corpseskin":
         tile := 1016;
   "mindrot":
         tile := 1017;
   "painstrike":
         tile := 1018;
   "strangle":
         tile := 1019;
   "giftofrenewal":
         tile := 1020;
   "attunewapon":
         tile := 1021;
   "thunderstorm":
         tile := 1022;
   "essenceofwind":
         tile := 1023;
   "etheralvoyage":
         tile := 1024;
   "giftoflife":
         tile := 1025;
   "arcaneempowerment":
         tile := 1026;
   "mortalstrike":
         tile := 1027;
   "reactivearmor":
         tile := 1028;
         cliloc1 := 1075812;
         cliloc2 := 1075813;
   "protection":
         tile := 1029;
         cliloc1 := 1075814;
         cliloc2 := 1075815;
   "archprotection":
         tile := 1030;
         cliloc1 := 1075816;
         cliloc2 := 1075816;
   "magicreflection":
         tile := 1031;
         cliloc1 := 1075817;
         cliloc2 := 1075818;
   "incognito":
         tile := 1032;
         cliloc1 := 1075819;
         cliloc2 := 1075820;
   "disguised":
         tile := 1033;
   "animalform":
         tile := 1034;
   "polymorph":
         tile := 1035;
         cliloc1 := 1075824;
         cliloc2 := 1075820;
   "invisibility":
         tile := 1036;
         cliloc1 := 1075825;
         cliloc2 := 1075826;
   "paralyse":
         tile := 1037;
         cliloc1 := 1075827;
         cliloc2 := 1075828;
   "poison":
         tile := 1038;
         cliloc1 := 0x0F8627;
         cliloc2 := 0x1069B1;
   "bleed":
         tile := 1039;
         cliloc1 := 0x106a75;
         cliloc2 := 0x106a76;
   "clumsy":
         tile := 1040;
         cliloc1 := 0x106a77;
         cliloc2 := 0x106a78;
   "feeblemind":
         tile := 1041;
         cliloc1 := 0x106a79;
         cliloc2 := 0x106a7a;
   "weaken":
         tile := 1042;
         cliloc1 := 1075837;
         cliloc2 := 1075838;
   "curse":
         tile := 1043;
         cliloc1 := 1075835;
         cliloc2 := 1075836;
   "masscurse":
         tile := 1044;
         cliloc1 := 0x106a7f;
         cliloc2 := 0x106a80;
   "agility":
         tile := 1045;
         cliloc1 := 0x106a81;
         cliloc2 := 0x106a82;
   "cunning":
         tile := 1046;
         cliloc1 := 0x106a83;
         cliloc2 := 0x106a84;
   "strength":
         tile := 1047;
         cliloc1 := 0x106a85;
         cliloc2 := 0x106a86;
   "bless":
         tile := 1048;
         cliloc1 := 0x106a87;
         cliloc2 := 0x106a88;
  endcase
*/
