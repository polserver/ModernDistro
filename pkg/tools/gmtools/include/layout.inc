var layout := array(
"page 0",
"gumppic  10   0  255",
"gumppic   0   0  255",
"gumppic  20   0  255",
"button    3   3 4017 4019 0  1 0",

"page 1",
"gumppic   0   0  5120",
"gumppic   6   0  5121",
"gumppic  95   0  5121",
"gumppic 185   0  5121",
"gumppic 275   0  5122",
"gumppic   0   6  5123",
"gumppic   6   6  5124",
"gumppic  95   6  5124",
"gumppic 185   6  5124",
"gumppic 275   6  5125",
"gumppic   0  95  5123",
"gumppic   6  95  5124",
"gumppic  95  95  5124",
"gumppic 185  95  5124",
"gumppic 275  95  5125",
"gumppic   0 184  5123",
"gumppic   6 184  5124",
"gumppic  95 184  5124",
"gumppic 185 184  5124",
"gumppic 275 184  5125",
"gumppic   0 273  5126",
"gumppic   6 273  5127",
"gumppic  95 273  5127",
"gumppic 185 273  5127",
"gumppic 275 273  5128",
"gumppic  17  30  3001",
"text     75  10  1153  0        ",             //GM Item Tools text
"text    190 255  1153 11        ",             //cancel button text
"button  240 253 4020 4022 1 0  0",             //cancel button
"text     60 255 1153 13        ",             //minimize button text
"button   20 253 4017 4019 0 3  0",             //minimize button
"text     45  40 1153  1        ",             //turn page creature tools text
"button    5  38 4005 4007 0 5  0",             //turn page creature tools
"text     45  61 1153  9        ",             //turn page trap creation text
"button    5  59 4005 4007 0 2  0",             //turn page trap creation
"text     45  82 1153  5        ",             //travel tools text
"button    5  80 4005 4007 0 4  0",             //travel tools
"text     45 103 1153 26        ",             //item modification tools text
"button    5 101 4005 4007 0 6  0",             //item modification tools
"text     45 145 1153 32        ",             //recall text
"button    5 143 4023 4025 1 0 26",             //recall
"text     45 166 1153 33        ",             //gate text
"button    5 164 4023 4025 1 0 27",             //gate
"text     45 187 1153 34        ",             //mark text
"button    5 185 4023 4025 1 0 28",             //mark
"text     45 208 1153 37        ",             //account info text
"button    5 206 4023 4025 1 0 31",             //account info
"text     45 229 1153 10        ",             //remove GMtools text
"button    5 227 4020 4022 1 0 10",             //remove GMtools

"page 2",                                       //page 2 data
"gumppic   0   0  5120",
"gumppic   6   0  5121",
"gumppic  95   0  5121",
"gumppic 185   0  5121",
"gumppic 275   0  5122",
"gumppic   0   6  5123",
"gumppic   6   6  5124",
"gumppic  95   6  5124",
"gumppic 185   6  5124",
"gumppic 275   6  5125",
"gumppic   0  95  5123",
"gumppic   6  95  5124",
"gumppic  95  95  5124",
"gumppic 185  95  5124",
"gumppic 275  95  5125",
"gumppic   0 184  5123",
"gumppic   6 184  5124",
"gumppic  95 184  5124",
"gumppic 185 184  5124",
"gumppic 275 184  5125",
"gumppic   0 273  5126",
"gumppic   6 273  5127",
"gumppic  95 273  5127",
"gumppic 185 273  5127",
"gumppic 275 273  5128",
"gumppic  17  30  3001",
"text     75  10 1153  0        ",             //GM Item Tools text
"text    190 255 1153 11        ",             //cancel text
"button  240 253 4020 4022 1 0  0",             //cancel
"text     60 255 1153 14        ",             //previous page text
"button   20 253 4017 4019 0 1  0",             //previous page
"text     45  40 1153 12        ",             //inferno trap text
"button    5  38 4023 4025 1 0 11",             //inferno trap
"text     45  61 1153 23        ",             //explosion trap  text
"button    5  59 4023 4025 1 0 18",             //explosion trap
"text     45  82 1153 24        ",             //poison trap text
"button    5  80 4023 4025 1 0 19",             //poison trap
"text     45 103 1153 25        ",             //dart trap text
"button    5 101 4023 4025 1 0 20",             //dart trap
"text     45 145 1153 28        ",             //hideinferno 5 text
"button    5 143 4023 4025 1 0 22",             //hideinferno 5
"text     45 166 1153 29        ",             //hideinferno 10 text
"button    5 164 4023 4025 1 0 23",             //hideinferno 10
"text     45 187 1153 30        ",             //showinferno 5 text
"button    5 185 4023 4025 1 0 24",             //showinferno 5
"text     45 208 1153 31        ",             //showinferno 10 text
"button    5 206 4023 4025 1 0 25",             //showinferno 10
"text     45 229 1153 27        ",             //remove trap text
"button    5 227 4020 4022 1 0 21",             //remove trap

"page 3",                                       // default "minimized" layout

"page 4",                                       //page 4 data
"gumppic   0   0  5120",
"gumppic   6   0  5121",
"gumppic  95   0  5121",
"gumppic 185   0  5121",
"gumppic 275   0  5122",
"gumppic   0   6  5123",
"gumppic   6   6  5124",
"gumppic  95   6  5124",
"gumppic 185   6  5124",
"gumppic 275   6  5125",
"gumppic   0  95  5123",
"gumppic   6  95  5124",
"gumppic  95  95  5124",
"gumppic 185  95  5124",
"gumppic 275  95  5125",
"gumppic   0 184  5123",
"gumppic   6 184  5124",
"gumppic  95 184  5124",
"gumppic 185 184  5124",
"gumppic 275 184  5125",
"gumppic   0 273  5126",
"gumppic   6 273  5127",
"gumppic  95 273  5127",
"gumppic 185 273  5127",
"gumppic 275 273  5128",
"gumppic  17  30  3001",
"text     75  10 1153  0        ",             //GM Item Tools text
"text    190 255 1153 11        ",             //cancel gmtools text
"button  240 253 4020 4022 1 0  0",             //cancel gmtools
"text     60 255 1153 14        ",             //goto page 1 text
"button   20 253 4017 4019 0 1  0",             //goto page 1
"text     45  40 1153 15        ",             //goto player text
"button    5  38 4023 4025 1 0  5",             //goto player
"text     45  61 1153 16        ",             //goto location text
"button    5  59 4008 4010 1 0 12",             //goto location
"text     45  82 1153 35        ",             //permanent gate text
"button    5  80 4008 4010 1 0 29",             //permanent gate



"page 5",                                       // page 5 data
"gumppic   0   0  5120",
"gumppic   6   0  5121",
"gumppic  95   0  5121",
"gumppic 185   0  5121",
"gumppic 275   0  5122",
"gumppic   0   6  5123",
"gumppic   6   6  5124",
"gumppic  95   6  5124",
"gumppic 185   6  5124",
"gumppic 275   6  5125",
"gumppic   0  95  5123",
"gumppic   6  95  5124",
"gumppic  95  95  5124",
"gumppic 185  95  5124",
"gumppic 275  95  5125",
"gumppic   0 184  5123",
"gumppic   6 184  5124",
"gumppic  95 184  5124",
"gumppic 185 184  5124",
"gumppic 275 184  5125",
"gumppic   0 273  5126",
"gumppic   6 273  5127",
"gumppic  95 273  5127",
"gumppic 185 273  5127",
"gumppic 275 273  5128",
"gumppic  17  30  3001",
"text     75  10 1153  0        ",             //GM Item Tools text
"text    190 255 1153 11        ",             //cancel gmtools text
"button  240 253 4020 4022 1 0  0",             //cancel gmtools
"text     60 255 1153 14        ",             //goto page 1 text
"button   20 253 4017 4019 0 1  0",             //goto page 1
"text     45  40 1153 17        ",             //bind npc single text
"button    5  38 4023 4025 1 0  1",             //bind npc single
"text     45  61 1153  2        ",             //bind npc group text
"button    5  59 4023 4025 1 0  2",             //bind npc group
"text     45  82 1153 18        ",             //show spawnpoint text
"button    5  80 4023 4025 1 0 13",             //show spawnpoint
"text     45 103 1153 19        ",             //showspawn 5 text
"button    5 101 4023 4025 1 0 14",             //showspawn 5
"text     45 124 1153 20        ",             //showspawn 10 text
"button    5 122 4023 4025 1 0 15",             //showspawn 10

"page 6",                                       //page 6 data
"gumppic   0   0  5120",
"gumppic   6   0  5121",
"gumppic  95   0  5121",
"gumppic 185   0  5121",
"gumppic 275   0  5122",
"gumppic   0   6  5123",
"gumppic   6   6  5124",
"gumppic  95   6  5124",
"gumppic 185   6  5124",
"gumppic 275   6  5125",
"gumppic   0  95  5123",
"gumppic   6  95  5124",
"gumppic  95  95  5124",
"gumppic 185  95  5124",
"gumppic 275  95  5125",
"gumppic   0 184  5123",
"gumppic   6 184  5124",
"gumppic  95 184  5124",
"gumppic 185 184  5124",
"gumppic 275 184  5125",
"gumppic   0 273  5126",
"gumppic   6 273  5127",
"gumppic  95 273  5127",
"gumppic 185 273  5127",
"gumppic 275 273  5128",
"gumppic  17  30  3001",
"text     75  10 1153  0        ",             //GM Item Tools text
"text    190 255 1153 11        ",             //cancel text
"button  240 253 4020 4022 1 0  0",             //cancel
"text     60 255 1153 14        ",             //previous page text
"button   20 253 4017 4019 0 1  0",             //previous page
"text     45  40 1153  4        ",             //item addset text
"button    5  38 4023 4025 1 0  4",             //item addset
"text     45  61 1153  6        ",             //make invisible  text
"button    5  59 4023 4025 1 0  6",             //make invisible
"text     45  82 1153  7        ",             //crim flag text
"button    5  80 4023 4025 1 0  7",             //crim flag
"text     45 103 1153 36        ",             //light level text
"button    5 101 4023 4025 1 0 30",             //light level
"text     45 124 1153 38        ",             //create switch text
"button    5 122 4023 4025 1 0 32",             //create switch
"text     45 145 1153 39        ",             //create chest text
"button    5 143 4023 4025 1 0 33"              //create chest
);

var data := array(
    "GM Item Tools",                            //0 general info
    "Creature Tools",                           //1
    "bind group of NPCs to an item",            //2
    "set 'go-menu' to an item",                 //3
    "set 'addset' command to an item",          //4
    "Travel Tools",                             //5
    "set 'make-invisible' to an item",          //6
    "set 'criminal flag' to an item",           //7
    "magic wands",                              //8
    "Trap Creation Tools",                      //9
    "remove GMtools from an item",              //10
    "cancel",                                   //11
    "Create Inferno Trap",                      //12
    "minimize",                                 //13
    "previous page",                            //14
    "goto player",                              //15
    "goto location",                            //16
    "bind single NPC to an item",               //17
    "create spawnpoint",                        //18
    "Show Spawnpoint range 5",                  //19
    "Show Spawnpoint range 10",                 //20
    "Hide Spawnpoint range 5",                  //21
    "Hide Spawnpoint range 10",                 //22
    "Place Explosion Trap on an item",          //23
    "Place Poison Trap on an item",             //24
    "Place Dart Trap on an item",               //25
    "Item Modification Tools",                  //26
    "Remove Trap From Item",                    //27
    "Hide Infernos Range  5",                   //28
    "Hide Infernos Range 10",                   //29
    "Show Infernos Range  5",                   //30
    "Show Infernos Range 10",                   //31
    "Recall",                                   //32
    "Gate",                                     //33
    "Mark",                                     //34
    "Create Permanent Gate",                    //35
    "set 'light level' to an item",             //36
    "Account Management",                       //37
    "Create A Switch from an item",             //38
    "Create A Treasure Chest Spawn"             //38
);