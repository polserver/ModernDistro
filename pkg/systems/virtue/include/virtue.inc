/*
 * This file will eventually have functions relating to the virtues
 * including the shrines and functions relating to the shrines.
*/

use uo;
use os;
use util;

include ":virtue:settings";
include "include/findCity";

// virtues are the canonical names of the eight virtues.
var virtues :=	{"valor", "honor", "compassion", "justice", "honesty", "spirituality", "sacrifice", "humility"};

// mantras are the words that must be spokenafter using the meditation skill near a shrine.
// Their order corresponds to the order of the virtues above, ie. Ra must be spoken at the Valor shine,
// Summ at the Honor shrint etc.
var mantras :=  {"Ra",    "Summ",   "Mu",        "Beh",     "Ahm",     "Om",           "Cah",       "Lum"};

// words_of_power - These are never used in game but they are documented and are included here
// for possible future expansion of the Distro. Their order corresponds to the order of the virtues above.
var words_of_power := {"Inopia", "Infama", "Vilis", "Malum", "Fallax", "Ignavus", "Avidus", "Veramocor"};

var settings := VS_GetSettingsCfgElem("Settings");

// virtuemax is the maximum absolute virtuescore a player can earn
// in a virtue (+/- virtuemax)
var virtuemax := CInt(settings.MaxRange);

// virtueamt is the default virtue points that are awarded if
// the calling programme does noy send a value for amt.
var virtueamt := CInt(settings.VirtueAdjustAmount);

///////////////////////////////
//
// VS_GetMantra(virtue_name)
//
// Purpose: Get the mantra based on the virtue.
//
//
// virtue_name - a string containing the name of a virtue.
//
// Purpose: Returns the mantra for the specified virtue. The correct
//          mantra for a virtue must be spoken by the player at the
//          shrine for that virtue.
//
// returns: String. The mantra for the virtue passeed to the function or
//          the string "_Error" if the virtue could not be found. a 0.
//
///////////////////////////////

function VS_GetMantra(virtue_name)

	foreach virtue in virtues
		if(virtue_name == virtue)
			return Lower(mantras[_virtue_iter]);
		endif
	endforeach
	// We did not find a matching virtue, so return "_Error".
	return "_Error";
endfunction

///////////////////////////////
//
// VS_AdjustVirtue(who, virtue, amt := 0, prob := 102)
//
// Purpose: Adds the amount sent in amt to a player's virtuescrore.
//
// who - ref to a mobile
//
// virtue - a string containing the name of a virtue with the first
//          letter capitalised, ie. Compassion
//
// amt - The amount of virtue points to award.
//
// prob - Probability that virtue points will be awarded.
//        This is an optional value that a calling programme can
//        use if the programmer wants to add some randomness to
//        the awarding of virtue points. This is a percentile amount.
//        If the calling programme specifies a probability it also
//        MUST specify an amount for amt.
//
// returns 1 if points are awarded, 0 if no points awarded.
//         It should always return a 1 unless prob is used. In which
//         case, if the percentile dice roll fails it will return a 0.
//
///////////////////////////////

function VS_AdjustVirtue(who, virtue, amt := 0, prob := 102)

	virtue := Lower(virtue);

	if(TypeOf(amt) == "String")
		amt := Lower(amt);
		if(amt.Find("d",2))
			amt := RandomDiceRoll(amt);
			if(amt.errortext)
				SysLog("The D-String string sent to VS_AdjustVirtue() in :virtue:virtue.inc is improperly formatted.\n" + amt.errortext);
				return 0;
			endif
		else
			SysLog("The value for 'amt' sent to VS_AdjustVirtue() in :virtue:virtue.inc was a string but not properly formatted for RandomDiceRoll().");
		endif
	endif

	// If the mobile is not a player character or the random dice roll here is greater than prob no virtue is awarded.
	if ( (!who.acctname) or (RandomInt(100) + 1 > prob) )
		return 0;
	endif

	if(!amt)
		amt := CInt(virtueamt);
	endif

	var virtuescore := VS_GetVirtueAmount(who, virtue);
	if (!virtuescore)
		virtuescore := 0;
	endif

	virtuescore := virtuescore + amt;
	if (virtuescore > virtuemax)
		virtuescore := virtuemax;
	elseif (virtuescore < (-1 * virtuemax))
		virtuescore := (-1 * virtuemax);
	endif

	VS_SetVirtueAmount(who, virtue, virtuescore);
	return 1;

endfunction

///////////////////////////////
//
// VS_GetVirtueAmount(who, virtue)
//
// who - ref to a mobile
//
// virtue - a string containing the name of a virtue with the first
//          letter capitalised, ie. Compassion
//
// Purpose: Retrieves a player's virtuescrore for the specified virtue.
//
// returns the integer value attached to the CProp "Virtue_" + <virtue>
///////////////////////////////

function VS_GetVirtueAmount(who, virtue)

  return CInt(GetObjProperty(who, "Virtue_" + VS_Capitalize(virtue)));

endfunction

///////////////////////////////
//
// VS_SetVirtueAmount(who, virtue)
//
// who - ref to a mobile
//
// virtue - a string containing the name of a virtue with the first
//          letter capitalised, ie. Compassion
//
// Purpose: Set a player's virtuescore for the specified virtue.
//
// returns the integer value attached to the CProp "Virtue_" + <virtue>
///////////////////////////////

function VS_SetVirtueAmount(who, virtue, virtuescore)

  return CInt(SetObjProperty(who, "Virtue_" + VS_Capitalize(virtue), virtuescore));

endfunction

/* Colours for the symbols in the virtue gump.
Valor: 234, 2117, 32
Honor: 17, 617, 317
Compassion: 2212, 2215, 52
Justice: 2209, 2211, 66
Honesty: 1347, 1351, 97
Spirituality: 2405, 2301, 1152
Sacrifice: 1546, 1551, 42
Humility: 1153, 2403, 2405
*/
/*
 * Cliloc numbers that describe the virtues:
 * 1052051 through 1052060
 *
 * 1073278 - 'The Virtues - are Humility, Sacrifice, Honor, Compassion, Valor, Honesty, Spirituality and Justice.'
 * 1073281 - 'Truth, Love and Courage are the three principles that make up the eight Virtues.'
 *
*/

