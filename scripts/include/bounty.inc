use datafile;

var bountyfile := DFOpenDataFile("BountySystem", 1);

function AddBounty(killer, amt)
  var entry := DFFindElement(bountyfile, CStr(killer.serial), 1);
  print(entry);
  entry.SetProp("Name", killer.name);
  entry.SetProp("Bounty", CInt(DFGetProp(entry, "Bounty", 1, 0) + amt));
  entry.SetProp("Murders", CInt(DFGetProp(entry, "Murders", 1, 0) + 1));
  entry.SetProp("Expires", ReadGameClock() + 1209600);
endfunction

function FindBounty(who, serial)
  who := who; // stops ecompile "not used" warning
  var elem := DFFindElement(bountyfile, CStr(serial));
  if(elem)
    var holder := array;
    holder.append(DFGetProp(elem, "Name"));
    holder.append(DFGetProp(elem, "Bounty"));
    holder.append(DFGetProp(elem, "Murders"));
    bountyfile.DeleteElement(CStr(serial));
    return holder;
  else
    return 0;
  endif
endfunction

function ComposeBountyBoard()
  var pnames := bountyfile.keys();
  var data := array;
  Foreach thing in pnames
    var elem := DFFindElement(bountyfile, thing);
    var holder := array;
    holder.append(DFGetProp(elem, "Name"));
    holder.append(CInt(thing));
    holder.append(DFGetProp(elem, "Bounty"));
    holder.append(DFGetProp(elem, "Murders", 1, 1));
    data.append(holder);
  endforeach
  return bubble_sort_array(data, 3);
endfunction

function MaintainBountySystem()
  var count := 0;
  foreach thing in (bountyfile.keys())
    var elem := DFFindElement(bountyfile, thing);
    if(ReadGameClock() > DFGetProp(elem, "Expires"))
      bountyfile.DeleteElement(thing);
      count := count + 1;
    endif
    sleep(1);
  endforeach
  print("//////////////////////////////////////////");
  print("// Bounty System Maintenance");
  print("// Removed " + count + " expired bounties.");
  print("//////////////////////////////////////////");
endfunction
