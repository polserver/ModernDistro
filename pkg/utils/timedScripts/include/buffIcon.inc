/*
        Written by Yukiko

        2019-10-13
*/
use uo;
use polsys;

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
// UpdateBuffIcon( mobile, byref cfg_elem, byref event, byref params := struct )      //
//                                                                                    //
// Purpose: Displays or removes a buff icon on a player's client.                     //
//                                                                                    //
// parameters:  mobile - the character referece to apply the buff icon.               //
//              cfg-elem - The element reference to an elements in timedScripts.cfg.  //
//              event - A struct containing information for the buff.                 //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
function UpdateBuffIcon( mobile, byref cfg_elem, byref event )

        var buff_icon := CInt( cfg_elem.BuffIcon );
        if( buff_icon )
            var amount := event.amount;
            if(amount)
                mobile.delbuff(buff_icon);
            endif

        endif
        return 1;
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
         cliloc_desc := 1075106; I think this is the wrong cliloc. "Fools!  You cannot defeat me."
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
   "disguised":
         BuffIcon := 1033;
   "animalform":
         BuffIcon := 1034;
   "paralyse":
         BuffIcon := 1037;
         cliloc_name := 1075827;
         cliloc_desc := 1075828;
   "bleed":
         BuffIcon := 1039;
         cliloc_name := 0x106a75;
         cliloc_desc := 0x106a76;
   "masscurse":
         BuffIcon := 1044;
         cliloc_name := 0x106a7f;
         cliloc_desc := 0x106a80;
  endcase
*/
