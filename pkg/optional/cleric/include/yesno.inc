use uo;

function yesno(who, prompt:="", yestext:="Yes", notext:="No")
	var yesnolayout :=
	{
		"page 0",
		"noclose",
		"resizepic 10 10 3600 300 140",
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

	var input := SendDialogGump(who, yesnolayout, yesnodata);

	return input[0];
endfunction
