var testlayout := array(
"page 0",
"gumppic   0  50 2620            ",
"gumppic  20  50 2621            ",
"gumppic 285  50 2621            ",
"gumppic 552  50 2622            ",
"gumppic   0  68 2623            ",
"gumppic  20  68 2624            ",
"gumppic 285  68 2624            ",
"gumppic 550  68 2625            ",
"gumppic   0 272 2623            ",
"gumppic  20 272 2624            ",
"gumppic 285 272 2624            ",
"gumppic 550 272 2625            ",
"gumppic   0 372 2623            ",
"gumppic  20 372 2624            ",
"gumppic 285 372 2624            ",
"gumppic 550 372 2625            ",
"gumppic   0 566 2626            ",
"gumppic  20 566 2627            ",
"gumppic 285 566 2627            ",
"gumppic 550 566 2628            ",
"gumppic  20  80 2621            ",         //top divider line
"gumppic 285  80 2621            ",         //top divider line
"gumppic  20 220 2621            ",         //mid divider line
"gumppic 285 220 2621            ",         //mid divider line
"gumppic  20 250 2621            ",         //account note box
"gumppic 285 250 2621            ",         //account note box
"gumppic  20 250 2620            ",         //account note box
"gumppic 535 250 2622            ",         //account note box
"gumppic  20 255 2623            ",         //account note box
"gumppic 535 255 2625            ",         //account note box
"gumppic  20 295 2623            ",         //account note box
"gumppic 535 295 2625            ",         //account note box
"gumppic  20 500 2627            ",         //account note box
"gumppic  20 500 2626            ",         //account note box
"gumppic 285 500 2627            ",         //account note box
"gumppic 537 500 2628            ",         //account note box
"text     20  59  910      0     ",         //account name text     (white)
"text    400  59  910      1     ",         //status text           (white)
"text     20  93  910      2     ",         //character 1 text      (white)
"text     20 118  910      3     ",         //character 2 text      (white)
"text     20 143  910      4     ",         //character 3 text      (white)
"text     20 168  910      5     ",         //character 4 text      (white)
"text     20 193  910      6     ",         //character 5 text      (white)
"text    340  90  910      7     ",         //ban this account text (white)
"text    292 110   37      8     ",         //warning text          (red)
"text    290 125   37      9     ",         //warning text          (red)
"text    288 140   37     10     ",         //warning text          (red)
"text    338 165  910     11     ",         //jail account text     (white)
"text     20 228  910     12     ",         //account notes text    (white)
"text    400 228  910     13     ",         //modify notes text     (white)
"text    450 548  910     14     ",         //cancel text     (white)
"button  500  91 4017 4019 1 0 11",         //ban account button
"button  500 166 4002 4004 1 0  2",         //jail account button
"button  508 228 4011 4013 1 0  3",         //modify notes button
"button  510 549 4020 4022 1 0  0",         //cancel button
                                            // Added by Shinigami
"textentry 30 260 500 20 910 25 25",        //modify notes text     (white)
"textentry 30 280 500 20 910 26 26",        //modify notes text     (white)
"textentry 30 300 500 20 910 27 27",        //modify notes text     (white)
"textentry 30 320 500 20 910 28 28",        //modify notes text     (white)
"textentry 30 340 500 20 910 29 29",        //modify notes text     (white)
"textentry 30 360 500 20 910 30 30",        //modify notes text     (white)
"textentry 30 380 500 20 910 31 31",        //modify notes text     (white)
"textentry 30 400 500 20 910 32 32",        //modify notes text     (white)
"textentry 30 420 500 20 910 33 33",        //modify notes text     (white)
"textentry 30 440 500 20 910 34 34",        //modify notes text     (white)
"textentry 30 460 500 20 910 35 35",        //modify notes text     (white)
"textentry 30 480 500 20 910 36 36",        //modify notes text     (white)
//"text     30 260  910     25     ",         //modify notes text     (white)
//"text     30 280  910     26     ",         //modify notes text     (white)
//"text     30 300  910     27     ",         //modify notes text     (white)
//"text     30 320  910     28     ",         //modify notes text     (white)
//"text     30 340  910     29     ",         //modify notes text     (white)
//"text     30 360  910     30     ",         //modify notes text     (white)
//"text     30 380  910     31     ",         //modify notes text     (white)
//"text     30 400  910     32     ",         //modify notes text     (white)
//"text     30 420  910     33     ",         //modify notes text     (white)
//"text     30 440  910     34     ",         //modify notes text     (white)
//"text     30 460  910     35     ",         //modify notes text     (white)
//"text     30 480  910     36     ",         //modify notes text     (white)
"text    125  59  100     15     ",         //account name text     (blue)
"text    455  59  100     16     ",         //account status text   (blue)
"text     20 520  910     23     ",         //email address text    (white)
"text     20 540  910     24     ",         //update email text     (white)
"text    130 520  100     22     ",         //email address         (blue)
"button   80 541 4020 4022 1 0  4",         //cancel button
"button  105  90 2440 2440 1 0  5",         //char slot 1 info
"button  105 115 2440 2440 1 0  6",         //char slot 2 info
"button  105 140 2440 2440 1 0  7",         //char slot 3 info
"button  105 165 2440 2440 1 0  8",         //char slot 4 info
"button  105 190 2440 2440 1 0  9",         //char slot 5 info
"text    110  93  100     17     ",         //char slot 1 text      (blue)
"text    110 118  100     18     ",         //char slot 2 text      (blue)
"text    110 143  100     19     ",         //char slot 3 text      (blue)
"text    110 168  100     20     ",         //char slot 4 text      (blue)
"text    110 193  100     21     "          //char slot 5 text      (blue)
);

var testdata := array(
    "Account Name:",                         //0
    "Status:",                               //1
    "Character 1",                           //2
    "Character 2",                           //3
    "Character 3",                           //4
    "Character 4",                           //5
    "Character 5",                           //6
    "Ban This Account?",                     //7
    "Banning an account will destroy all",   //8
    "characters and their bank boxes, this", //9
    "is Permanent, so be careful with this!",//10
    "Jail This Account?",                    //11
    "Account Notes:",                        //12
    "Modify Notes:",                         //13
    "Cancel:",                               //14
    "",                                      //15 player account name
    "",                                      //16 player account status
    "",                                      //17 player character 1
    "",                                      //18 player character 2
    "",                                      //19 player character 3
    "",                                      //20 player character 4
    "",                                      //21 player character 5
    "",                                      //22 player email address
    "Email Address:",                        //23
    "update:",                               //24 
    "",                                      //25
    "",                                      //26
    "",                                      //27
    "",                                      //28
    "",                                      //29
    "",                                      //30
    "",                                      //31
    "",                                      //32
    "",                                      //33
    "",                                      //34
    "",                                      //35
    "",                                      //36
    "in jail for",                           //37
    "more days.",                            //38
    ""                                       //39
);

var banlayout := array(
    "gumppic  20  60 2624            ",      //account note box
    "gumppic 285  60 2624            ",      //account note box
    "gumppic  20   5 2624            ",      //account note box
    "gumppic 285   5 2624            ",      //account note box
    "gumppic  20   0 2621            ",      //account note box
    "gumppic 285   0 2621            ",      //account note box
    "gumppic  20   0 2620            ",      //account note box
    "gumppic 535   0 2622            ",      //account note box
    "gumppic  20   5 2623            ",      //account note box
    "gumppic 535   5 2625            ",      //account note box
    "gumppic  20  45 2623            ",      //account note box
    "gumppic 535  45 2625            ",      //account note box
    "gumppic  20 250 2627            ",      //account note box
    "gumppic  20 250 2626            ",      //account note box
    "gumppic 285 250 2627            ",      //account note box
    "gumppic 537 250 2628            ",      //account note box
    "text     90  60   37      0     ",      //warning text          (red)
    "text     90  75   37      1     ",      //warning text          (red)
    "text     90  90   37      2     ",      //warning text          (red)
    "text    150 105   37      3     ",      //warning text          (red)
    "button  105 190  247  249 1 0  1",      //cancel button
    "button  405 190  242  240 1 0  2"       //confirm button
);

var bandata := array(
    "WARNING! Banning an account through this interface will",        //0
    "delete all account characters as well as destroy their bank",    //1
    "boxes! This is not reversable, use this feature with care!",     //2
    "do you still wish to ban this account?"                          //3
);