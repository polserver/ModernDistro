use uo;
use file;

include "include/myUtil";

var coinarray := array {};
var createdamt := 0;

function createcoins( cont, cointype, numcoins )

  // Check to make sure there are coins to create.
  if(!numcoins)
    return -1;
  endif

	// Get the amount of 60k stacks, if any, we need to create.
	var stacks := CInt(numcoins / 60000);
	// Used to hold the remainder or the coin amount after we subtract the 60k stacks
	// or if 'numcoins' is less than 60k it will hold the value of 'numcoins'.
	var remainder := 0;
	// A flag to indicate success creating the required stacks.
	var stacks_created := 0;
	// An objref populated if we successfully created the amount contained in 'remainder'.
	var created_remainder := 0;
  // If numcoins > 60k we check for a remainder.
  // Otherwise we're creating a less than 60k coin stack.
  if(stacks)
    remainder := numcoins % 60000;
  else
    remainder := numcoins;
  endif
  // If there is a remainder then we try to create that first.
  // If there is no remainder we set it to -1 for check later.
  if(remainder)
    created_remainder := CreateItemInContainer(cont, cointype, remainder);
  else
    created_remainder := -1;
  endif
	// If we couldn't create the 'remainder' (or 'numcoins' if that was less than 60k)
	// we return a 'no success', 0.
	if(created_remainder.errortext)   // Failed to create the remainder
		return 0;
	endif

  // Next, if we have stacks to create, we try to create them.
	if(stacks)
		stacks_created := Create60kStacksInContainer(cont, cointype, stacks);
    // We tried to create the stacks but had no success.
    // So destroy the remainder if there is one.
    if(!stacks_created && created_remainder.errortext)
      DestroyItem(created_remainder);
      return 0;
    endif
	endif

	// We were suiccessful in creating the coins in the container. Yay!
	return 1;
endfunction
	
function Create60kStacksInContainer(what_container, type, numstacks)

  var quantity := 60000;
  var stackarray := array {};
  var success;
  var what;
  var  arrayindex := 1;
  
  // Try to create all of the required stacks.
  for index := 1 to numstacks
    success := 0;
    what := CreateItemInContainer( what_container, type, quantity );
    
    // If we fail to create a stack set success to 0 and exit the 'for' loop.
    // else set success, add the created stack to the stack array for
    // possible clean-up later, increment the index and continue through
    // the 'for' loop.
    if(what.errortext)
      success := 0;
      break;
    endif
      success := 1;
      stackarray[arrayindex] := what;
      arrayindex := arrayindex + 1;
  endfor
  
  // If all of the stacks were not created then clean-up the leftovers
  // and return with success == 0.
  if(success == 0)
    foreach staque in stackarray
      DestroyItem(staque);
    endforeach
    return success;
  endif
  
  // Yay!! We created all of the stacks!
  return success;

endfunction

function SpendCoin(who, quantity, cont, cointype)
    var hisGold := GetStacksOfTypeInContainer(cont, cointype);
    who := who;
    if(hisGold[1] < quantity)
	return 0;
    endif
    SubtractFromStackArray (hisGold, quantity);
    return 1;
endfunction
