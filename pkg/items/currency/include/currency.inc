use uo;
use file;

include "include/myUtil";

var coinarray := {};
var createdamt := 0;


function createcoins( who, cont, cointype, numcoins )

	// Get the amount of 60k stacks, if any, we need to create.
	var stacks := CInt(numcoins / 60000);
	// Used to hold the remainder or the coin amount after we subtract the 60k stacks
	// or if 'numcoins' is less than 60k it holds the value of 'numcoins'.
	var remainder := 0;
	// A flag to indicate success creating the rquired stacks.
	var stackscreated := 0;
	// An objref populated if we successfully created the amount contained in 'remainder'.
	var created := 0;

	// If we have more than 60000 coins then let's get the remainder after
	// we subtract the total stack amounts from 'numcoins'.
	if(stacks)
		remainder := numcoins - (stacks * 60000);
		stackscreated := Create60kCoinStacks(who, cointype, stacks);
	// No stacks needed so 'remainder' equals the number of coins passed.
	else
		remainder := numcoins;
	endif
	
	// We tried to create the stacks but had no success
	if(stacks && !stackscreated)
		return 0;
	endif
	
	created := CreateItemInContainer(cont, cointype, remainder);
	// If we couldn't create the 'remainder' (or 'numcoins' if that was less than 60k)
	// we clean-up any stacks that were created and return a 'no success', 0.
	if(!created && remainder && stacks)
		SubtractFromStackArray (coinarray, createdamt);
		return 0;
	elseif(!created)
		return 0;
	endif
	
	// We were suiccessful in creating the coins in the container. Yay!
	return 1;
endfunction
	
	
function Create60kCoinStacks(who, cointyp, numstacks)
	var index :=1;
	var what;
	while(numstacks)
		what := CreateItemInBackpack( who, cointyp, 60000 );
		if(what)
			createdamt := createdamt + 60000;
			coinarray[index] := what;
			numstacks := numstacks - 1;
			index := index + 1;
		elseif(createdamt == 0)
			return 0;
		else
			SubtractFromStackArray(coinarray, createdamt);
			return 0;
		endif
	endwhile
	return 1;
		
endfunction