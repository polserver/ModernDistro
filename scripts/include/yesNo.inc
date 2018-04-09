use uo;

const MAX_YESNO_LINES := 7;

function YesNo(me, prompt)
  if (len(prompt) <= 10)
    return SmallYesNo(me, prompt);
  else
    return BigYesNo(me, prompt);
  endif
endfunction

function SmallYesNo(me, prompt)

  var layoutyn := array(
                      "page 0",
                      "nodispose",
                      "gumppic 0 0 2070",
                      "button 30 75 2130 2129 1 0 1",
                      "button 90 75 2119 2120 1 0 0",
                       "text 40 30 502 0"
	             );

  var datayn := array(  prompt  );
  var res := SendDialogGump( me, layoutyn, datayn );

  if (res)
    return 1;
  else
    return 0;
  endif

endfunction

function BigYesNo(me, prompt)

  var layoutyn := array(
                        "page 0",
			"nodispose",
                        "gumppic 0 0 1140",
                        "button 115 200 2130 2129 1 0 1",
                        "button 205 200 2119 2120 1 0 0",
                        "text 55 45 502 0",
                        "text 55 65 502 1",
                        "text 55 85 502 2",
                        "text 55 105 502 3",
                        "text 55 125 502 4",
                        "text 55 145 502 5",
                        "text 55 165 502 6"
	               );

  var datayn := packPromptToArray(prompt);
  var res := SendDialogGump( me, layoutyn, datayn );

  if (res)
    return 1;
  else
    return 0;
  endif

endfunction

function YesNoRes(me, prompt)
	var layout := array(
		"page 0",
		"gumppic 0 0 2070",
		"button 30 75 2130 2129 1 0 1",
		"button 90 75 2119 2120 1 0 0",
		"text 40 30 1355 0"
	);

	var data := array ( prompt );

	me.frozen := 1;
	var res := SendDialogGump(me, layout, data);
	me.frozen := 0;

	if (res)
		return 1;
	else
		return 0;
	endif
endfunction


/////////////////////////////////////////////////////////////
// Yes/No with definable prompt and responses
/////////////////////////////////////////////////////////////
function yesno_ex(who, prompt:="", yestext:="Yes", notext:="No")
	var yesnolayout :=
	{
		"page 0",
		"noclose",
		"resizepic 10 10 3600 300 140"	
	};
	var yesnodata :=	{};
	yesnodata.append(prompt);
	yesnolayout.append("text 30 25 500 " + (len(yesnodata)-1));
	
	yesnolayout.append("gumppic 70 50 96");
	
	yesnolayout.append("button 30 75 2103 2104 1 0 1");
	yesnodata.append(yestext);
	yesnolayout.append("text 45 70 500 " + (len(yesnodata)-1));
	
	yesnolayout.append("button 30 105 2103 2104 1 0 0");
	yesnodata.append(notext);
	yesnolayout.append("text 45 100 500 " + (len(yesnodata)-1));	
	
	var input := senddialoggump(who, yesnolayout, yesnodata);
	
	return input[0];	
endfunction


/* 
* YesNo Gump by Kinetix 
* Version 2.0 
* This new gump will actually extend itself based on the length of your prompt text, which means 
*  the text/gump overlap bug is gone. 
*/ 
use uo; 
use util; 

function YesNoVar(me, prompt, color := "#FFFFFF") 
  var lenny := cint(len(prompt) * 2)+160; 
  if(lenny < 180) lenny := 180; endif 
  var layout := array( 
     "nodispose", 
     "page 0", 
     "resizepic 30 30 5054 "+ lenny +" 110", 
     "gumppictiled 41 41 "+ (lenny - 22) +" 88 2624", 
     "checkertrans 40 40 "+ (lenny - 20) +" 90", 
     "htmlgump 40 45 "+ (lenny - 19) +" 59 0 0 0", 
     "button 50 102 4023 4025 1 0 1", 
     "button "+ (lenny - 20) +" 102 4017 4019 1 0 0" 
  ); 
  var data := array("<basefont color=\""+ color +"\"><center>"+ prompt +"</center></basefont"); 
  var res := SendDialogGump( me, layout, data ); 
  if (res) 
     return 1; 
  else 
     return 0; 
  endif 
endfunction

function packPromptToArray(prompt)
  var theArray := array;
  var theString := prompt;
  var i;

  for (i:=1; i<=MAX_YESNO_LINES; i:=i+1)
    theArray[i] := " ";
  endfor
  
  i := 0;

  while ((len(theString) > 0) && (i <= MAX_YESNO_LINES))
    i := i + 1;
    theArray[i] := StripSubstring(theString, 40);
  endwhile

  return theArray;
endfunction

function StripSubstring(byref theString, theSize)
  var subString;
  var newString;
  var pos := theSize;

  if (len(theString) <= theSize)
    subString := theString;
    theString := "";
    return subString;
  endif
  
  while ((theString[pos] != " ") && (pos > 1))
    pos := pos - 1;
  endwhile
  
  if (pos == 1)
    pos := theSize;
  endif
  
  subString := theString[pos+1, len(theString)];
  newString := theString[1,pos];
  
  theString := subString;
  
  return newString; 

endfunction