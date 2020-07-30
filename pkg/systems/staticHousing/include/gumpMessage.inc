
use uo;
use os;
use basic;

function gumpmessage(mobile, text, movable := 0, hue := 1153)
	var index;
	var gumpdata := {};
	var lines := {};
	var closable := "";
	if ( !movable )
		closable := "noclose";
	endif
	var layout := array {
		closable,
		"page 0",
		"resizepic 0 0 3600 300 160",
		"resizepic 13 18 5054 270 130",
		"button 119 121 247 248 1 0 0"
	};
	lines := WordWrap(text, 260);
	var y_pos := 25;
	foreach line in lines
		gumpdata.Append(""+CStr(line));
		index := gumpdata.Size()-1;
		layout.append("text 20 " + CStr(y_pos) + " " + CStr(hue) + " " + CStr(index));
		y_pos := y_pos+20;
		SleepMS(2);
	endforeach

	SendDialogGump( mobile, layout, gumpdata );

endfunction


/*
program gump_msggump(who)

	var gumpdata := array {
		"New Label",
		"New Label",
		"New Label",
		"New Label"
	};

	SendDialogGump(who, gump, gumpdata);


endprogram

*/


function WordWrap(text, pixels)
	var wrap_lines := array{};
	var word_list := SplitWords(text);
	//var num_words := word_list.Size();

	var space_size := GFTextWidth(" ");

	var cur_line := "";
	var cur_length := 0;
	var start_new := 0;

	foreach word in ( word_list )
		var word_width := GFTextWidth(word);

		if ( word == "[P]" )
			// [P] will force a line break
			word := "";
			word_width := 0;
			start_new := 1;
		elseif ( (word_width+cur_length) > pixels )
			start_new := 1;
			if ( cur_line[Len(cur_line)] == " " )
				cur_line[Len(cur_line)] := "";
			endif
		endif

		if ( start_new )
			start_new := 0;
			wrap_lines.Append(cur_line);
			cur_line := "";
			cur_length := 0;
		endif
		if ( word )
			cur_line += word+" ";
			cur_length += word_width+space_size;
		endif

		SleepMS(2);
	endforeach

	//Make sure nothing got left behind.
	if ( wrap_lines[wrap_lines.Size()] != cur_line )
		if ( cur_line[Len(cur_line)] == " " )
			cur_line[Len(cur_line)] := "";
		endif
		wrap_lines.Append(cur_line);
	endif

	return wrap_lines;
endfunction

// function GFTextWidth(gump_text)
	// var gft_length := struct;
	// gft_length.+charsf :=
	// "||||||||||||||"+	//  2
	// "`',.;:::::::::"+	//  3
	// "!!!!!!!!!!!!!!"+	//  4
	// "il1[]]]]]]]]]\""+	//  5
	// "bcejotî(){}==="+	//  6
	// "adfpqrszIÑ~~~~"+	//  7
	// "ghnuvyÅ<>-----"+	//  8
	// "xJ578$?++++++ "+	//  9
	// "kCE2369^/*****"+	// 10
	// "mwABDFOPéô40%%"+	// 11
	// "GLNQSZ@#######"+	// 12
	// "HRT&__________"+	// 13
	// "KöUYYYYYYYYYYY"+	// 14
	// "XXXXXXXXXXXXXX"+	// 15
	// "VVVVVVVVVVVVVV"+	// 16
	// "MMMMMMMMMMMMMM"+	// 18
	// "WWWWWWWWWWWWWW";	// 21
	// gft_length.+width := array
	// {
		// 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
		// 12, 13, 14, 15, 16, 18, 21
	// };

	// var points:=0;
	// var j := Len(gump_text);

	// for ( j; j>0; j-=1 )
		// var i := Find(gft_length.charsf, gump_text[j], 1) - 1;
		// if ( i>=0 )
			// points += gft_length.width[CInt(i/14)+1];
		// else
			// print("XGFTextWidth: Unknown Character = '"+gump_text[j]+"'");
		// endif
	// endfor

	// return (points);
// endfunction

