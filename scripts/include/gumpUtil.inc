/////////////////////////////////////////////
//
//  Shinigami's GumpUtil v1.02 (2000/04/29)
//    (some nice Functions for Gumps)
//
//  Author:    Shinigami
//  eMail:     Shinigami@gmx.net
//  Languages: English/German
//
/////////////////////////////////////////////

/////////////////
// Installation
/////////////////
//
// Move it to /pol/scripts/include and use it in your script:
//   Include "include/gumputil";

//////////////
// Functions
//////////////
//
// GumpTextWidth(gumptext)           // Returns the Width of GumpText
// GumpTextWidthFast(gumptext)       // Returns the Width of GumpText
// GumpTextLeft(gumptext, x, width)  // Returns the X-Value (Left-Alignment)
// GumpTextRight(gumptext, x, width) // Returns the X-Value (Right-Alignment)
// GumpTextMid(gumptext, x, width)   // Returns the X-Value (Mid-Alignment)
//
//   Parameters:
//     gumptext - a String
//     x        - X-Value from the Textbox
//     width    - Width from the Textbox
//
// Create_ScrollBar(x, y, height, pages, page, layout) // Shows a Scrollbar
//
//   Parameters:
//     x,y    - X-/Y-Value of the left-top Corner
//     height - Height
//     pages  - Number of Pages
//     page   - Actual Page Number
//     layout - Contains the actual Gump Layout
//   Returns:
//     Updated "layout"

//////////////////////
// Original Location
//////////////////////
//
// www.egroups.com
//
//   www.egroups.com/files/pol-scriptforum/Includes/gumputil.zip

///////////////////////////////////////////////////////////////////////////////

//////////////
// Constants
//////////////

var c34:=CChr(34);

var length:={ };
       length.+chars:={
         "",                   // Dummy (for Speed...)
         "|",                  //  2
         "`',.;:",             //  3
         c34+"!",              //  4
         "il1[]",              //  5
         "bcejot”ö(){}=",      //  6
         "adfpqrszI„ä~",       //  7
         "ghnuvy <>-",          //  8
         "xJ578$?ß+",          //  9
         "kCE2369^/*",         // 10
         "mwABDFOPŽÄ™Ö40%",    // 11
         "GLNQSZ#",            // 12
         "HRT&_",              // 13
         "KšÜUY",              // 14
         "X",                  // 15
         "V",                  // 16
         "M",                  // 18
         "W"                   // 21
       };
       length.+charsf:=        // Space as Filler is not allowed
         "|||||||||||||||"+    //  2
         "`',.;::::::::::"+    //  3
         c34+"!!!!!!!!!!!!!!"+ //  4
         "il1[]]]]]]]]]]]"+    //  5
         "bcejot”ö(){}==="+    //  6
         "adfpqrszI„ä~~~~"+    //  7
         "ghnuvy <>------"+    //  8
         "xJ578$?ß+++++++"+    //  9
         "kCE2369^/******"+    // 10
         "mwABDFOPŽÄ™Ö40%"+    // 11
         "GLNQSZ#########"+    // 12
         "HRT&___________"+    // 13
         "KšÜUYYYYYYYYYYY"+    // 14
         "XXXXXXXXXXXXXXX"+    // 15
         "VVVVVVVVVVVVVVV"+    // 16
         "MMMMMMMMMMMMMMM"+    // 18
         "WWWWWWWWWWWWWWW";    // 21
       length.+width:={
          2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
          12, 13, 14, 15, 16, 18, 21
       };

//////////////
// Functions
//////////////

// Returns the Width of GumpText

function GumpTextWidth(gumptext)
  var start_i:=len(length.chars);
  var points:=0;
  var j;

  for (j:=len(gumptext); j>0; j:=j-1)
    var i:=start_i;
    while ((i>0) && (!length.chars[i][CStr(gumptext[j])]))
      i:=i-1;
    endwhile;
    if (i>0) // siehe Speed-Dummy
      points:=points+length.width[i-1];
    else
      syslog("GumpTextWidth: Unknown Character = '"+gumptext[j]+"'");
    endif
  endfor

  return (points);
endfunction

// Returns the Width of GumpText

function GumpTextWidthFast(gumptext)
  var points:=0;
  var j;

  for (j:=len(gumptext); j>0; j:=j-1)
    var i:=find(length.charsf, gumptext[j], 1)-1;
    if (i>=0)
      points:=points+length.width[CInt(i/15)+1];
    else
      syslog("GumpTextWidthFast: Unknown Character = '"+gumptext[j]+"'");
    endif
  endfor

  return (points);
endfunction

// Returns the X-Value (Left-Alignment)

function GumpTextLeft(gumptext, x, width)
  return (x);
endfunction

// Returns the X-Value (Right-Alignment)

function GumpTextRight(gumptext, x, width)
  return (x+(width-GumpTextWidthFast(gumptext)));
endfunction

// Returns the X-Value (Mid-Alignment)

function GumpTextMid(gumptext, x, width)
  return (x+CInt((width-GumpTextWidthFast(gumptext))/2));
endfunction

//////////////////////////////////////////////////
// Create_ScrollBar - Erzeugt einen Scrollbalken
//////////////////////////////////////////////////

function Create_ScrollBar(x, y, height, pages, page, layout)
  if (pages>1)
    var x_str:=CStr(x)+" ";
    var button_x_str:="button "+x_str;
    var gumppic_x_str:="gumppic "+x_str;
    var page_before:=" 0 "+CStr(page-1);
    var page_next:=" 0 "+CStr(page+1);

    var y_mark;
    if (pages==page)
      y_mark:=y+height-46;
      layout.append(gumppic_x_str+CStr(y_mark-7)+" 256");
    else
      y_mark:=y+CInt((height-67)/(pages-1))*(page-1)+21;
      layout.append(gumppic_x_str+CStr(y_mark)+" 256");
    endif
    layout.append("gumppic "+CStr(x+1)+" "+CStr(y_mark)+" 254");

    var y_bkg;
    var y_bkg_end;
    if (page>1)
      y_bkg_end:=y+21;
      for (y_bkg:=y_mark-32; y_bkg>y_bkg_end; y_bkg:=y_bkg-32)
        layout.append(button_x_str+CStr(y_bkg)+" 256 256"+page_before);
      endfor;
      layout.append(button_x_str+CStr(y_bkg_end)+" 256 256"+page_before);
      layout.append(button_x_str+CStr(y)+" 250 251"+page_before);
    else
      layout.append(gumppic_x_str+CStr(y)+" 250");
    endif
    if (page<pages)
      y_bkg_end:=y+height-21-32;
      for (y_bkg:=y_mark+25; y_bkg<y_bkg_end; y_bkg:=y_bkg+32)
        layout.append(button_x_str+CStr(y_bkg)+" 256 256"+page_next);
      endfor;
      layout.append(button_x_str+CStr(y_bkg_end)+" 256 256"+page_next);
      layout.append(button_x_str+CStr(y+height-21)+" 252 253"+page_next);
    else
      layout.append(gumppic_x_str+CStr(y+height-21)+" 252");
    endif
  endif
  return (layout);
endfunction

function WhichIndex(arr, value)
  var index := 1;
  if (arr.size() == 0)
	return 0;
  endif
  foreach member in arr
	if (member == value)
	  return index;
	else
	  index := index + 1;
	endif
  endforeach
  return 0;
endfunction

///////////////////////////////////////////////////////////
// gumputil.inc
//
// Gump utility finctions.
//
// REQUIRES: Mythmarri's GUMPS package - v2.1
//
// Creator: Mercy
// Contact: mkow@sympatico.ca
//
// Created: 19-Apr-2000
// Updated: 19-Apr-2000
//
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// GUMP CONSTS
///////////////////////////////////////////////////////////

// Backgrounds
const BKGND_MARBLE			:= 0x0a28;

// Stretchable Dividers
const DIV_BLACK_LINE		:= 0x005d;

// Book
const G_BIGBOOK				:= 0x01fe;
const G_BIGBOOK_L_CORNER	:= 0x01ff;
const G_BIGBOOK_R_CORNER	:= 0x0200;

// Small circles with spell circle numbers 1-8.
const G_CIRCLE1				:= 0x08b1;
const G_CIRCLE2				:= 0x08b2;
const G_CIRCLE3				:= 0x08b3;
const G_CIRCLE4				:= 0x08b4;
const G_CIRCLE5				:= 0x08b5;
const G_CIRCLE6				:= 0x08b6;
const G_CIRCLE7				:= 0x08b7;
const G_CIRCLE8				:= 0x08b8;

// Diamond Buttons
const G_BLUE_DIAMOND_1		:= 0x0868;
const G_BLUE_DIAMOND_0		:= 0x086a;
const G_RED_DIAMOND_1		:= 0x09a9;
const G_RED_DIAMOND_0		:= 0x09aa;

// Octagon Skill 1-3 Buttons
const G_SKILL1				:= 0x04d8;
const G_SKILL2				:= 0x04db;
const G_SKILL3				:= 0x04de;

// Standard buttons.
const G_RED_PREV_1			:= 0x04f1;
const G_RED_PREV_0			:= 0x04f2;
const G_GREEN_CONT_1		:= 0x04f4;
const G_GREEN_CONT_0		:= 0x04f5;
const G_GREEN_DONE_1		:= 0x051a;
const G_GREEN_DONE_0		:= 0x0519;
const G_GREEN_OK_0			:= 0x0992;
const G_GREEN_OK_1			:= 0x0993;

// Oval buttons with gold border.
const G_RED_OVAL_CANCEL_1	:= 0x0818;
const G_RED_OVAL_CANCEL_0	:= 0x0817;
const G_RED_OVAL_PREV_1		:= 0x0913;
const G_RED_OVAL_PREV_0		:= 0x0912;
const G_GREEN_OVAL_OK_1		:= 0x081b;
const G_GREEN_OVAL_OK_0		:= 0x081a;
const G_YELLOW_OVAL_APPLY_1	:= 0x084b;
const G_YELLOW_OVAL_APPLY_0	:= 0x084a;

// Other Buttons
const G_SMALL_GEM_0			:= 0x04b9;
const G_SMALL_GEM_1			:= 0x04ba;
const G_TINY_GEM_0			:= 0x0837;
const G_TINY_GEM_1			:= 0x0838;

// Other.
const G_SQUARE_PLUS			:= 0x151a;
const G_SQUARE_MINUS		:= 0x1519;

// Words
const G_SKILLS				:= 0x0834;
const G_SKILL_USE			:= 0x0836;

// Stone Gumps
const G_TAB_0				:= 0x138e;
const G_TAB_1				:= 0x138f;
const G_MINUS_0				:= 0x138c;
const G_MINUS_1				:= 0x138d;
const G_STONE_SPHERE		:= 0x13a8;
const G_STONE_XBOX_0		:= 0x138a;
const G_STONE_XBOX_1		:= 0x138b;
const G_STONE_CHECK_0		:= 0x13ba;
const G_STONE_CHECK_1		:= 0x13bb;
const G_STONE_X_0			:= 0x13bc;
const G_STONE_X_1			:= 0x13bd;
const G_STONE_UP			:= 0x1458;
const G_STONE_DOWN			:= 0x1459;
const G_STONE_LEFT			:= 0x1467;
const G_STONE_RIGHT			:= 0x1468;
const G_STONE_SQUARE		:= 0x00d2;
const G_STONE_SQUARE_CHECK	:= 0x00d3;
const G_STONE_SQUARE_BLANK	:= 0x00d4;
const G_GRAY_CIRCLE_0		:= 0x00d0;
const G_GRAY_CIRCLE_1		:= 0x00d1;


// Arrows
const G_TRIANGLE_UP			:= 0x15e0;
const G_TRIANGLE_DOWN		:= 0x15e2;

// Tombstones
const G_TOMBSTONE_ROUND		:= 0x0065;
const G_TOMBSTONE_SQUARE	:= 0x0066;

// Seperators
const G_WAX_SEAL			:= 0x005c;
const G_FLAT_LINE			:= 0x005d;
const G_LEAF				:= 0x005e;

// COLOURS
const COL_YELLOW			:= 52;
const COL_RED				:= 32;
const COL_BLUE				:= 87;
const COL_DK_BLUE			:= 96;
const COL_LT_BLUE			:= 88;

// Text Entry Backgrounds
const G_TX_MARBLE_L			:= 0x0988;
const G_TX_MARBLE_S_LT		:= 0x098b;
const G_TX_MARBLE_S_DK		:= 0x098c;
const G_TX_MARBLE_M			:= 0x098d;
const G_TX_MARBLE_SQ_L		:= 0x098e;
