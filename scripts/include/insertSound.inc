/////////////////////////////////////////////////////////////////
//
// insertsound.inc v 1.0
//
//  This is the general control script to determine what sound is played
//    when items are inserted into a container.  The order of precedence is:
//      highest - item inserted by amount
//              - item inserted
//      lowest  - container item inserted into
//
//  Dependencies
//    Basic sounds for containers and inserted items are mapped in itemdesc.cfg
//    Names of sounds are included included in insertsound.cfg
//    Default sounds and complex sound assignments are included in insertsound.cfg
//
// Author: Valkyrie <Valkyrie@GatheringOrder.com>
// Creation date: 02 June 2003
//
//==To Do==
//
//
// See Sourceforge for complete revision history
//       
/////////////////////////////////////////////////////////////////

use cfgfile;

function inNoise(who, box, item)
	// Purpose: to determine a sound to play for insert and play it.
	// called by: insert scripts
	// returns: nothing
	

        var noise := "sound_defaultin";
        
        // the soundfile contains all the special config entries as well as the sound mappings.
        var soundfile := ReadConfigFile("::insertsound");
        
        //start with the objectdesc.cfg entry for the container
        var itemconfig := ReadConfigFile(":*:itemdesc");
        var boxnoise := itemconfig[box.objtype].OnInsertNoise;
        if ( boxnoise )
                noise := boxnoise;
        endif
        
        // next check to see if there's an itemdesc.cfg entry for the item
        var itemnoise:= itemconfig[item.objtype].InsertNoise;
        if ( itemnoise )
                noise := itemnoise;
        endif
        
        // last, check to see if there's a special config entry for this item
        var listing := FindConfigElem( soundfile, item.objtype );
        if ( listing )
                var breakpoints := GetConfigStringArray( listing, "inLevel" );
                foreach level in breakpoints
                level := SplitWords(level);
                        if ( item.amount >= cint(level[1]) )
                                noise := level[2];
                        else
                                break;
                        endif
                endforeach
        endif
        
        // Once the noise is determined, we need to play it.
        makeNoise(who, noise, soundfile);
        
        return;
endfunction

function outNoise(who, box, item)
	// Purpose: to determine a sound to play for removal and play it.
	// called by: remove scripts
	// returns: nothing
	
        var noise := "sound_defaultout";

        // the soundfile contains all the special config entries as well as the sound mappings.
        var soundfile := ReadConfigFile("::insertsound");
        
        //start with the objectdesc.cfg entry for the container
        var itemconfig := ReadConfigFile(":*:itemdesc");
        var boxnoise := itemconfig[box.objtype].OnRemoveNoise;
        if ( boxnoise )
                noise := boxnoise;
        endif
        
        // next check to see if there's an itemdesc.cfg entry for the item
        var itemnoise:= itemconfig[item.objtype].RemoveNoise;
        if ( itemnoise )
                noise := itemnoise;
        endif
        
        // last, check to see if there's a special config entry for this item
        var listing := FindConfigElem( soundfile, item.objtype );
        if ( listing )
                var breakpoints := GetConfigStringArray( listing, "outLevel" );
                foreach level in breakpoints
                level := SplitWords(level);
                        if ( item.amount >= cint(level[1]) )
                                noise := level[2];
                        else
                                break;
                        endif
                endforeach
        endif
        
        // Once the noise is determined, we need to play it.
        makeNoise(who, noise, soundfile);
        
        return;
        
endfunction

function makeNoise(who, noise, soundfile)
	// Purpose: to play a the sound as defined in soundfile by the string noise
	// called by: functions inNoise and outNoise
	// returns: nothing
	
        var playnoise;

        // figure out what noise will be played, by mapping the name to the config entry
        var noisedef := FindConfigElem(soundfile, "noisedefs");
        var arrayofnoises := GetConfigStringArray(noisedef, "sound");
        foreach sound in arrayofnoises
                var entry := SplitWords(sound);
                if ( noise == entry[1] )
                        playnoise := entry[2];
                endif
        endforeach

        // play it!  Well, if we know what it is...
        if (playnoise)
                playnoise := cint(playnoise);
                PlaySoundEffect(who, playnoise);
        else
                SendSysMessage(who, "noise " + noise + " not defined" );
        endif
        
        return;
endfunction
