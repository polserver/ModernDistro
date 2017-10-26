use uo;
use os;
use cfgfile;

function bubble_sort_array(data, pos)
  var i := 1;
  var l := len(data);
  for(i:=1; i<=l; i:=i+1)
    var ind := data[i];
    var j := i;
    while((j > 0) && (data[j-1][pos] > ind[pos]))
      data[j] := data[j-1];
      j := j - 1;
      sleepms(1);
    endwhile
    data[j] := ind;
    sleepms(1);
  endfor
  return data;
endfunction
