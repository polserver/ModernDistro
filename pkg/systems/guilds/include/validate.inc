/*
        Maintened by POL Distro Team

        2009-05-30
*/
use uo;
use os;

function ValidStr( thestring )

         var i,
         valid_chars := {
                        "0", "1", "2", "3", "4",
                        "5", "6", "7", "8", "9",
                        "a", "b", "c", "d", "e",
                        "f", "g", "h", "i", "j",
                        "k", "l", "m", "n", "o",
                        "p", "q", "r", "s", "t",
                        "u", "v", "w", "x", "y",
                        "z", "$", "*", "&", "#",
                        "@", "!", "?", "+", "-",
                        "^", " " };

         thestring := Lower( thestring );

         var flag := 1;

         for( i:=1; i<=Len( thestring ); i += 1 )
                  if( !( thestring[i] in valid_chars ))
                           flag := 0;
                  endif
                  SleepMS(5);
         endfor

         return flag;
endfunction

function ValidAbbr( thestring )

         var flag := 1;

         foreach guilds in ListGuilds()
                  if( Lower( guilds.GetProp( "Abbr" )) == Lower( thestring ))
                           flag := 0;
                           break;
                  endif
                  SleepMS(5);
         endforeach

         return flag;
endfunction

function ValidName( thestring )

         var flag := 1;

         foreach guilds in ListGuilds()
                  if ( Lower( guilds.GetProp( "Name" )) == Lower( thestring ))
                           flag := 0;
                           break;
                  endif

                  SleepMS(5);
         endforeach

         return flag;
endfunction
