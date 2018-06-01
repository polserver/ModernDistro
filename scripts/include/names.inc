////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////
//
//  NAMES.INC - contains access routines to the randomname.
//
//  This file contain accessroutine to a random name within the names.cfg
//  file. Look at that file for the types of names you can request.
//
//  Author: Ignatius 1999/01/??
//
//  Recent Modifications
//      1999/01/31 Syzygy   Corrected syntax errors.
//      1999/01/27 Ignatius Cleaned it up a bit.
//
////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////

use cfgfile;
use util;
use uo;

// Nametypes
const NAME_BIRD_SMALL           :=  1;
const NAME_DAEMON               :=  2;
const NAME_DRAGON               :=  3;
const NAME_HUMAN_EVIL_MAGE      :=  4;
const NAME_HUMAN_FEMALE         :=  5;
const NAME_HUMAN_MALE           :=  6;
const NAME_HUMAN_PIRATE_FEMALE  :=  7;
const NAME_HUMAN_PIRATE_MALE    :=  8;
const NAME_LIZARDMAN            :=  9;
const NAME_OGRE                 :=  10;
const NAME_ORC                  :=  11;
const NAME_RATMAN               :=  12;

function GetRandomName( nametype )
    // check for nametype
    if ( ! nametype )
        print( "Error: No nametype supplied." );
        return "OSI";
    endif

    // Open the configfile.
    var nameconfigfile := ReadConfigFile("names");
    if ( ! nameconfigfile  )
        print( "Error: Could not open the names.cfg file." );
        return "OSI";
    endif

    // Obtain the namegroup/key.
    var namekey := FindConfigElem( nameconfigfile, nametype );
    if( ! namekey )
        print( "Error: The  nametype could not be found." );
        return "OSI";
    endif

    // Get the number of the name we are looking for.
    var nbrofnames := GetConfigInt( namekey, "Count" );
    var nbrofname := RandomInt( nbrofnames ) + 1;


    // Get the acutal name.
    var thename := GetConfigString( namekey, "Name"+CStr( nbrofname ) );
    if ( ! thename )
        print( "Error: The name could not be found.");
        return "OSI";
    endif


    // Last but not least, return the name.
    return thename; 
endfunction
