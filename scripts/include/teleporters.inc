use uo;
use os;
use cfgfile;

//////////////////////////////////////////////////////////////
//															//
//		Start of Felucca (britannia realm) teleporters.		//
//		I believe these are for the Mondain's Legacy		//
//		map.	Yukiko										//
//															//
//////////////////////////////////////////////////////////////

var teleporters := {
////yew to t2a tunnel
    { 312,   786,  -24, "britannia",    5748,   361,    2,	"britannia"},
    { 313,   786,  -24, "britannia",    5749,   361,    2,	"britannia"},
    { 314,   786,  -24, "britannia",    5750,   361,    4,	"britannia"},
    {5748,   362,    2, "britannia",     312,   787,  -24,	"britannia"},
    {5749,   362,    0, "britannia",     313,   787,  -24,	"britannia"},
    {5750,   362,    3, "britannia",     314,   787,  -24,	"britannia"},

////jhelom teleporters
    {1142,  3621,    5, "britannia",    1414,  3828,    5,	"britannia"},
    {1406,  3996,    5, "britannia",    1414,  3828,    5,	"britannia"},
    {1409,  3824,    5, "britannia",    1124,  3623,    5,	"britannia"},
    {1419,  3832,    5, "britannia",    1466,  4015,    5,	"britannia"},

//// moonglow teleporters
    {4300,   968,    5, "britannia",    4442,  1122,    5,	"britannia"},
    {4436,  1107,    5, "britannia",    4300,   992,    5,	"britannia"},
    {4443,  1137,    5, "britannia",    4487,  1475,    5,	"britannia"},
    {4449,  1107,    5, "britannia",    4539,   890,   28,	"britannia"},
    {4449,  1115,    5, "britannia",    4671,  1135,   10,	"britannia"},
    {4496,  1475,   15, "britannia",    4442,  1122,    5,	"britannia"},
    {4540,   898,   32, "britannia",    4442,  1122,    5,	"britannia"},
    {4663,  1134,   13, "britannia",    4442,  1122,    5,	"britannia"},

////hedge maze
    {1163,  2208,   60, "britannia",    1171,  2202,    0,	"britannia"},
    {1165,  2205,   40, "britannia",    1163,  2209,   60,	"britannia"},
    {1166,  2237,   60, "britannia",    1163,  2208,    0,	"britannia"},

////destard
//enterance
    {1175,  2635,    0, "britannia",    5242,  1006,    0,	"britannia"},
    {1176,  2635,    0, "britannia",    5243,  1006,    0,	"britannia"},
    {1177,  2635,    0, "britannia",    5244,  1006,    0,	"britannia"},
    {5242,  1007,    0, "britannia",    1175,  2636,    0,	"britannia"},
    {5243,  1007,    0, "britannia",    1176,  2636,    0,	"britannia"},
    {5244,  1007,    0, "britannia",    1177,  2636,    0,	"britannia"},
//lvl1 - lvl2
    {5129,   909,  -28, "britannia",    5142,   798,   17,	"britannia"},
    {5130,   909,  -28, "britannia",    5143,   798,   17,	"britannia"},
    {5131,   909,  -28, "britannia",    5144,   798,   17,	"britannia"},
    {5132,   909,  -28, "britannia",    5145,   798,   17,	"britannia"},
    {5142,   797,   22, "britannia",    5129,   908,  -28,	"britannia"},
    {5143,   797,   22, "britannia",    5130,   908,  -28,	"britannia"},
    {5144,   797,   22, "britannia",    5131,   908,  -28,	"britannia"},
    {5145,   797,   22, "britannia",    5132,   908,  -28,	"britannia"},
//lvl2 - lvl3
    {5153,   808,  -25, "britannia",    5134,   984,   17,	"britannia"},
    {5153,   809,  -25, "britannia",    5134,   985,   17,	"britannia"},
    {5153,   810,  -25, "britannia",    5134,   986,   17,	"britannia"},
    {5153,   811,  -25, "britannia",    5134,   987,   17,	"britannia"},
    {5133,   984,   22, "britannia",    5152,   808,  -19,	"britannia"},
    {5133,   985,   22, "britannia",    5152,   809,  -19,	"britannia"},
    {5133,   986,   22, "britannia",    5152,   810,  -19,	"britannia"},
    {5133,   987,   22, "britannia",    5152,   811,  -19,	"britannia"},

////shame
//enterance
    { 512,  1559,    0, "britannia",    5394,   126,    0,	"britannia"},
    { 513,  1559,    0, "britannia",    5395,   126,    0,	"britannia"},
    { 514,  1559,    0, "britannia",    5396,   126,    0,	"britannia"},
    {5394,   127,    0, "britannia",     512,  1560,    0,	"britannia"},
    {5395,   127,    0, "britannia",     513,  1560,    0,	"britannia"},
    {5396,   127,    0, "britannia",     514,  1560,    0,	"britannia"},
//lvl1 - lvl2
    {5490,    19,  -30, "britannia",    5514,    10,    0,	"britannia"},
    {5514,    10,    0, "britannia",    5490,    19,  -30,	"britannia"},
//lvl2 - lvl3
    {5604,   102,    5, "britannia",    5514,   148,   25,	"britannia"},
    {5514,   148,   25, "britannia",    5604,   102,    5,	"britannia"},
//lvl3 - lvl4
    {5538,   170,    5, "britannia",    5513,   176,    5,	"britannia"},
    {5513,   176,    5, "britannia",    5538,   170,    5,	"britannia"},
//lvl4 - lvl5
    {5507,   162,    5, "britannia",    5875,    19,   -5,	"britannia"},
    {5875,    19,   -5, "britannia",    5507,   162,    5,	"britannia"},

////deceit
//enterance
    {4110,   430,    5, "britannia",    5187,   638,    0,	"britannia"},
    {4111,   430,    5, "britannia",    5188,   638,    0,	"britannia"},
    {4112,   430,    5, "britannia",    5189,   638,    0,	"britannia"},
    {5187,   639,    0, "britannia",    4110,   431,    5,	"britannia"},
    {5188,   639,    0, "britannia",    4111,   431,    5,	"britannia"},
    {5189,   639,    0, "britannia",    4112,   431,    5,	"britannia"},
//lvl1 - lvl2
    {5216,   587,  -18, "britannia",    5304,   531,   12,	"britannia"},
    {5217,   587,  -18, "britannia",    5305,   531,   12,	"britannia"},
    {5218,   587,  -18, "britannia",    5306,   531,   12,	"britannia"},
    {5304,   530,   17, "britannia",    5216,   586,  -13,	"britannia"},
    {5305,   530,   17, "britannia",    5217,   586,  -13,	"britannia"},
    {5306,   530,   17, "britannia",    5218,   586,  -13,	"britannia"},
//lvl2 - lvl3
    {5346,   578,    5, "britannia",    5137,   649,    5,	"britannia"},
    {5137,   649,    5, "britannia",    5346,   578,    5,	"britannia"},
//lvl3 - lvl4
    {5218,   761,  -28, "britannia",    5305,   650,   12,	"britannia"},
    {5219,   761,  -28, "britannia",    5306,   650,   12,	"britannia"},
    {5305,   649,   17, "britannia",    5218,   760,    1,	"britannia"},
    {5306,   549,   17, "britannia",    5219,   760,    1,	"britannia"},

////wrong
//enterance
    {2041,   215,   14, "britannia",    5824,   630,    0,	"britannia"},
    {2042,   215,   14, "britannia",    5825,   630,    0,	"britannia"},
    {2043,   215,   14, "britannia",    5826,   630,    0,	"britannia"},
    {5824,   631,    0, "britannia",    2041,   216,   14,	"britannia"},
    {5825,   631,    0, "britannia",    2042,   216,   14,	"britannia"},
    {5826,   631,    0, "britannia",    2043,   216,   14,	"britannia"},
//lvl1 - lvl2
    {5826,   592,    0, "britannia",    5690,   569,   25,	"britannia"},
    {5690,   569,   25, "britannia",    5826,   592,    0,	"britannia"},
//lvl2 - lvl3
    {5736,   551,   20, "britannia",    5703,   639,    0,	"britannia"},
    {5698,   662,    0, "britannia",    5736,   551,   20,	"britannia"},

////despise
//enterance
    {1296,  1080,    0, "britannia",    5587,   630,   30,	"britannia"},
    {1296,  1081,    0, "britannia",    5587,   631,   30,	"britannia"},
    {1296,  1082,    0, "britannia",    5587,   632,   30,	"britannia"},
    {5588,   630,   30, "britannia",    1297,  1080,    0,	"britannia"},
    {5588,   631,   30, "britannia",    1297,  1081,    0,	"britannia"},
    {5588,   632,   30, "britannia",    1297,  1082,    0,	"britannia"},
//lvl1 - lvl2
    {5570,   628,   57, "britannia",    5504,   569,   46,	"britannia"},
    {5570,   629,   57, "britannia",    5504,   570,   46,	"britannia"},
    {5570,   630,   57, "britannia",    5504,   571,   46,	"britannia"},
    {5505,   569,   41, "britannia",    5571,   628,   52,	"britannia"},
    {5505,   570,   41, "britannia",    5571,   629,   52,	"britannia"},
    {5505,   571,   41, "britannia",    5571,   630,   52,	"britannia"},
//lvl2 - lvl3
    {5571,   632,   12, "britannia",    5522,   672,   32,	"britannia"},
    {5571,   633,   12, "britannia",    5522,   673,   32,	"britannia"},
    {5571,   634,   12, "britannia",    5522,   674,   32,	"britannia"},
    {5523,   672,   37, "britannia",    5572,   632,   17,	"britannia"},
    {5523,   673,   37, "britannia",    5572,   633,   17,	"britannia"},
    {5523,   674,   37, "britannia",    5572,   634,   17,	"britannia"},
//lvl3 - lvl4
    {5385,   755,  -13, "britannia",    5410,   858,   57,	"britannia"},
    {5385,   756,  -13, "britannia",    5410,   859,   57,	"britannia"},
    {5385,   757,  -13, "britannia",    5410,   860,   57,	"britannia"},
    {5411,   858,   62, "britannia",    5386,   755,   -8,	"britannia"},
    {5411,   859,   62, "britannia",    5386,   756,   -8,	"britannia"},
    {5411,   860,   62, "britannia",    5386,   757,   -8,	"britannia"},

////covetous
//entrance
    {5455,  1864,    0, "britannia",    2498,   917,    0,	"britannia"},
    {5456,  1864,    0, "britannia",    2499,   917,    0,	"britannia"},
    {5457,  1864,    0, "britannia",    2500,   917,    0,	"britannia"},
    {2498,   916,    0, "britannia",    5455,  1863,    0,	"britannia"},
    {2499,   916,    0, "britannia",    5456,  1863,    0,	"britannia"},
    {2500,   916,    0, "britannia",    5457,  1863,    0,	"britannia"},
//lvl1 - lvl2
    {5392,  1959,    0, "britannia",    2420,   883,    0,	"britannia"},
    {5393,  1959,    0, "britannia",    2421,   883,    0,	"britannia"},
    {5394,  1959,    0, "britannia",    2422,   883,    0,	"britannia"},
    {2420,   884,    0, "britannia",    5392,  1958,    0,	"britannia"},
    {2421,   884,    0, "britannia",    5393,  1958,    0,	"britannia"},
    {2422,   884,    0, "britannia",    5394,  1958,    0,	"britannia"},
//lvl2 - lvl3
    {2384,   836,    0, "britannia",    5614,  1996,    0,	"britannia"},
    {2384,   837,    0, "britannia",    5614,  1997,    0,	"britannia"},
    {2384,   838,    0, "britannia",    5614,  1998,    0,	"britannia"},
    {5615,  1996,    0, "britannia",    2385,   836,    0,	"britannia"},
    {5615,  1997,    0, "britannia",    2385,   837,    0,	"britannia"},
    {5615,  1998,    0, "britannia",    2385,   838,    0,	"britannia"},
//lvl3 - lvl4
    {5388,  2027,    0, "britannia",    2455,   859,    0,	"britannia"},
    {5389,  2027,    0, "britannia",    2456,   859,    0,	"britannia"},
    {5390,  2027,    0, "britannia",    2457,   859,    0,	"britannia"},
    {2455,   858,    0, "britannia",    5388,  2026,    0,	"britannia"},
    {2456,   858,    0, "britannia",    5389,  2026,    0,	"britannia"},
    {2457,   858,    0, "britannia",    5390,  2026,    0,	"britannia"},
//lvl4 - lvl5
    {2544,   846,    0, "britannia",    5578,  1926,    0,	"britannia"},
    {2545,   846,    0, "britannia",    5579,  1926,    0,	"britannia"},
    {2546,   846,    0, "britannia",    5580,  1926,    0,	"britannia"},
    {5578,  1927,    0, "britannia",    2544,   847,    0,	"britannia"},
    {5579,  1927,    0, "britannia",    2545,   847,    0,	"britannia"},
    {5580,  1927,    0, "britannia",    2546,   847,    0,	"britannia"},
//lvl5-lvl6a
    {5556,  1827,  -15, "britannia",    5551,  1805,   12,	"britannia"},
    {5557,  1827,  -15, "britannia",    5552,  1805,   12,	"britannia"},
    {5551,  1804,   17, "britannia",    5556,  1826,  -10,	"britannia"},
    {5552,  1804,   17, "britannia",    5556,  1826,  -10,	"britannia"},
    {5552,  1804,   17, "britannia",    5557,  1826,  -10,	"britannia"},
//lvl5 - lvl6b
    {5595,  1840,  -14, "britannia",    5466,  1804,   12,	"britannia"},
    {5595,  1841,  -14, "britannia",    5466,  1805,   12,	"britannia"},
    {5465,  1804,   17, "britannia",    5594,  1840,   -9,	"britannia"},
    {5465,  1805,   17, "britannia",    5594,  1841,   -9,	"britannia"},
    {5465,  1805,   17, "britannia",    5594,  1841,   -9,	"britannia"},

////hythloth
//enterance
    {5904,    17,   61, "britannia",    4721,  3814,    0,	"britannia"},
    {5905,    17,   61, "britannia",    4722,  3814,    0,	"britannia"},
    {5906,    17,   61, "britannia",    4723,  3814,    0,	"britannia"},
    {4721,  3813,    0, "britannia",    5904,    18,   56,	"britannia"},
    {4722,  3813,    0, "britannia",    5905,    18,   56,	"britannia"},
    {4723,  3813,    0, "britannia",    5906,    18,   56,	"britannia"},
//lvl1 - lvl2
    {5905,    97,    0, "britannia",    5976,   169,    0,	"britannia"},
    {5976,   169,    0, "britannia",    5905,    97,    0,	"britannia"},
    {5976,   168,    0, "britannia",    5905,    97,    0,	"britannia"},
    {5977,   169,    0, "britannia",    5905,    97,    0,	"britannia"},
    {5977,   168,    0, "britannia",    5905,    97,    0,	"britannia"},
//lvl2 - lvl3
    {5922,   168,    7, "britannia",    6081,   144,  -10,	"britannia"},
    {5922,   169,    7, "britannia",    6081,   145,  -10,	"britannia"},
    {5922,   170,    7, "britannia",    6081,   146,  -10,	"britannia"},
    {6080,   144,   -5, "britannia",    5921,   169,   11,	"britannia"},
    {6080,   145,   -5, "britannia",    5921,   169,   12,	"britannia"},
    {6080,   146,   -5, "britannia",    5921,   169,   11,	"britannia"},
//lvl3 - lvl4
    {6041,   192,    7, "britannia",    6057,    87,   34,	"britannia"},
    {6041,   193,    7, "britannia",    6057,    89,   34,	"britannia"},
    {6041,   194,    9, "britannia",    6057,    90,   34,	"britannia"},
    {6056,    87,   39, "britannia",    6040,   192,   12,	"britannia"},
    {6056,    89,   39, "britannia",    6040,   193,   12,	"britannia"},
    {6056,    90,   39, "britannia",    6040,   194,   12,	"britannia"},

////wind
//enterance
    {5166,   245,   15, "britannia",    1361,   883,    0,	"britannia"},
    {1361,   883,    0, "britannia",    5166,   245,   15,	"britannia"},
//NE exit
    {5191,   152,    0, "britannia",    1361,   883,    0,	"britannia"},
// wind - park
    {5200,    71,   17, "britannia",    5211,    22,   15,	"britannia"},
    {5217,    18,   15, "britannia",    5200,    71,   17,	"britannia"},

////ice dungeon
//enterance
    {5882,   241,    0, "britannia",    1997,    81,    7,	"britannia"},
    {5882,   242,    0, "britannia",    1997,    81,    7,	"britannia"},
    {5882,   243,    0, "britannia",    1997,    81,    7,	"britannia"},
    {1997,    81,    7, "britannia",    5881,   242,    0,	"britannia"},
//lvl1 - orc fort
    {5849,   238,  -25, "britannia",    5831,   324,   27,	"britannia"},
    {5850,   238,  -25, "britannia",    5832,   324,   26,	"britannia"},
    {5851,   238,  -25, "britannia",    5833,   324,   28,	"britannia"},
    {5852,   238,  -25, "britannia",    5834,   324,   27,	"britannia"},
    {5853,   238,  -23, "britannia",    5835,   324,   27,	"britannia"},
    {5831,   323,   34, "britannia",    5849,   237,  -24,	"britannia"},
    {5832,   323,   34, "britannia",    5850,   237,  -24,	"britannia"},
    {5833,   323,   33, "britannia",    5851,   237,  -24,	"britannia"},
    {5834,   323,   33, "britannia",    5852,   237,  -25,	"britannia"},
    {5835,   323,   33, "britannia",    5853,   237,  -23,	"britannia"},
//lvl1 - lvl2
    {5704,   145,  -45, "britannia",    5705,   305,   12,	"britannia"},
    {5704,   146,  -45, "britannia",    5705,   306,   12,	"britannia"},
    {5704,   147,  -45, "britannia",    5705,   307,   12,	"britannia"},
    {5706,   303,   12, "britannia",    5705,   146,  -45,	"britannia"},
    {5706,   304,   12, "britannia",    5705,   146,  -45,	"britannia"},
    {5706,   305,   12, "britannia",    5705,   146,  -45,	"britannia"},
    {5706,   306,   12, "britannia",    5705,   147,  -45,	"britannia"},
    {5706,   307,   12, "britannia",    5705,   147,  -45,	"britannia"},
    {5706,   308,   12, "britannia",    5705,   147,  -45,	"britannia"},
//lvl1 - t2a exit
    {5872,   144,   32, "britannia",    5203,  2327,   27,	"britannia"},
    {5873,   144,   32, "britannia",    5203,  2327,   27,	"britannia"},
    {5874,   144,   32, "britannia",    5203,  2327,   27,	"britannia"},
    {5875,   144,   32, "britannia",    5203,  2327,   27,	"britannia"},
    {5876,   144,   32, "britannia",    5207,  2322,   27,	"britannia"},
    {5877,   144,   32, "britannia",    5207,  2322,   27,	"britannia"},
    {5878,   144,   32, "britannia",    5207,  2323,   26,	"britannia"},
    {5879,   144,   32, "britannia",    5207,  2323,   26,	"britannia"},
    {5203,  2327,   27, "britannia",    5874,   145,   30,	"britannia"},
    {5207,  2322, -255, "britannia",    5875,   145,   30,	"britannia"},
    {5207,  2323, -255, "britannia",    5876,   145,   30,	"britannia"},

////fire dungeon
//enterance
    {5827,  1502,    0, "britannia",    2922,  3406,    8,	"britannia"},
    {5828,  1502,   -1, "britannia",    2923,  3406,    9,	"britannia"},
    {5829,  1502,    2, "britannia",    2924,  3406,    7,	"britannia"},
    {2922,  3405,    7, "britannia",    5827,  1501,    0,	"britannia"},
    {2923,  3405,    6, "britannia",    5828,  1501,    0,	"britannia"},
    {2924,  3405,    7, "britannia",    5829,  1501,    1,	"britannia"},
//lvl1 - t2a exit
    {5790,  1415,   39, "britannia",    5758,  2907,   15,	"britannia"},
    {5790,  1416,   40, "britannia",    5758,  2908,   15,	"britannia"},
    {5790,  1417,   38, "britannia",    5758,  2909,   15,	"britannia"},
    {5757,  2907,   15, "britannia",    5789,  1415,   42,	"britannia"},
    {5757,  2908,   14, "britannia",    5789,  1416,   39,	"britannia"},
    {5757,  2909,   15, "britannia",    5789,  1417,   38,	"britannia"},
//lvl1 - lvl2
    {5786,  1335,  -13, "britannia",    5701,  1318,    8,	"britannia"},
    {5787,  1335,  -13, "britannia",    5702,  1318,    8,	"britannia"},
    {5788,  1335,  -13, "britannia",    5703,  1318,    8,	"britannia"},
    {5701,  1319,   13, "britannia",    5786,  1336,   -8,	"britannia"},
    {5702,  1319,   13, "britannia",    5787,  1336,   -8,	"britannia"},
    {5703,  1319,   13, "britannia",    5788,  1336,   -8,	"britannia"},

////volcano shop teleporter
    {1653,  2963,    0, "britannia",    1671,  2981,    0,	"britannia"},
    {1677,  2987,    0, "britannia",    1675,  2987,   20,	"britannia"},

////bucs den underground
//enterance
    {2674,  2242,    2, "britannia",    2677,  2242,  -20,	"britannia"},
    {2676,  2242,  -18, "britannia",    2675,  2242,    2,	"britannia"},
    {2676,  2242,  -18, "britannia",    2675,  2242,    2,	"britannia"},
//exit
    {2649,  2195,  -14, "britannia",    2648,  2193,    4,	"britannia"},
    {2669,  2072,  -19, "britannia",    2669,  2072,    5,	"britannia"},
    {2669,  2073,  -19, "britannia",    2669,  2073,    5,	"britannia"},

////britain sewer
//enterance
    {6031,  1499,   35, "britannia",    1490,  1640,   24,	"britannia"},
    {6031,  1501,   36, "britannia",    1490,  1642,   24,	"britannia"},
    {1491,  1640,   24, "britannia",    6032,  1499,   31,	"britannia"},
    {1491,  1642,   24, "britannia",    6032,  1501,   31,	"britannia"},
//lvl1 - cave
    {6140,  1431,    4, "britannia",    6137,  1408,    2,	"britannia"},
    {6141,  1431,    4, "britannia",    6138,  1408,    2,	"britannia"},
    {6137,  1409,    2, "britannia",    6140,  1432,    4,	"britannia"},
    {6138,  1409,    2, "britannia",    6141,  1432,    4,	"britannia"},
//cave - t2a exit
    {6126,  1410,   15, "britannia",    6074,  3333,    2,	"britannia"},
    {6127,  1410,   15, "britannia",    6075,  3333,    2,	"britannia"},
    {6074,  3332,    4, "britannia",    6126,  1409,   13,	"britannia"},
    {6075,  3332,    4, "britannia",    6127,  1409,   13,	"britannia"},

////minoc cave
//enterance
    {2399,   198,    0, "britannia",    6087,  3675,   17,	"britannia"},
    {2400,   198,    0, "britannia",    6087,  3676,   18,	"britannia"},
//exit to t2a
    {6086,  3675,   15, "britannia",    2399,   199,    0,	"britannia"},
    {6086,  3676,   16, "britannia",    2400,   199,    0,	"britannia"},

////vesper graveyard
//enterance
    {2776,   894,  -23, "britannia",    5685,   386,    1,	"britannia"},
    {2776,   895,  -23, "britannia",    5685,   387,    2,	"britannia"},
    {5686,   386,    2, "britannia",    2777,   894,  -23,	"britannia"},
    {5686,   387,    2, "britannia",    2777,   895,  -23,	"britannia"},
//cave - t2a graveyard
    {5658,   423,    8, "britannia",    5697,  3659,    2,	"britannia"},
    {5697,  3660,   -5, "britannia",    5658,   424,    0,	"britannia"},

////trinsic cave
//enterance
    {1628,  3320,    0, "britannia",    5899,  1409,   41,	"britannia"},
    {1629,  3320,    0, "britannia",    5900,  1409,   39,	"britannia"},
    {5898,  1411,   38, "britannia",    1628,  3321,    0,	"britannia"},
    {5899,  1411,   43, "britannia",    1628,  3321,    0,	"britannia"},
    {5900,  1411,   42, "britannia",    1629,  3321,    0,	"britannia"},
//lvl1 - lvl2
    {5918,  1410,  -29, "britannia",    5962,  1407,   57,	"britannia"},
    {5918,  1411,  -29, "britannia",    5962,  1408,   56,	"britannia"},
    {5918,  1412,  -29, "britannia",    5962,  1409,   56,	"britannia"},
    {5961,  1407,   59, "britannia",    5917,  1410,  -27,	"britannia"},
    {5961,  1408,   59, "britannia",    5917,  1411,  -27,	"britannia"},
    {5961,  1409,   59, "britannia",    5917,  1412,  -27,	"britannia"},
//lvl2 - west t2a
    {6025,  1344,  -26, "britannia",    5138,  3664,   31,	"britannia"},
    {6025,  1345,  -25, "britannia",    5138,  3665,   32,	"britannia"},
    {6025,  1346,  -25, "britannia",    5138,  3666,   32,	"britannia"},
    {5137,  3664,   27, "britannia",    6024,  1345,  -25,	"britannia"},
    {5137,  3665,   31, "britannia",    6024,  1346,  -25,	"britannia"},
//lvl2 - delucia
    {6005,  1378,    0, "britannia",    5151,  4062,   37,	"britannia"},
    {6005,  1379,    2, "britannia",    5151,  4063,   37,	"britannia"},
    {6005,  1380,    1, "britannia",    5151,  4064,   37,	"britannia"},
    {5150,  4062,   38, "britannia",    6004,  1378,    1,	"britannia"},
    {5150,  4063,   38, "britannia",    6004,  1379,   -1,	"britannia"},

////terathan keep
//enterance
    {5426,  3122, -74, "britannia",    5341,  1601,    0,	"britannia"},
    {5341,  1601,   0, "britannia",    5426,  3121,  -65,	"britannia"},

////spirituality shrine
    {1592,  2489,   17, "britannia",    1600,  2489,   10,	"britannia"},
    {1600,  2489,   10, "britannia",    1592,  2490,   17,	"britannia"},
    {1600,  2490,   10, "britannia",    1592,  2490,   17,	"britannia"},

// Lost Lands
	{ 6303, 327, 70, "britannia", 5577, 3018, 25, "britannia" },
	{ 5578, 3018, 25, "britannia", 6304, 329, 60, "britannia" },
	{ 6303, 328, 65, "britannia", 5579, 3019, 25, "britannia" },
	{ 5579, 3017, 25, "britannia", 6305, 330, 60, "britannia" },

//////////////////////////////////////////////////////////////
//															//
//		Start of Trammel (britannia_alt realm) teleporters.	//
//		I believe these are for the Mondain's Legacy		//
//		map.	Yukiko										//
//															//
//////////////////////////////////////////////////////////////

////yew to t2a tunnel
    { 312,   786,  -24, "britannia_alt",    5748,   361,    2,	"britannia_alt"},
    { 313,   786,  -24, "britannia_alt",    5749,   361,    2,	"britannia_alt"},
    { 314,   786,  -24, "britannia_alt",    5750,   361,    4,	"britannia_alt"},
    {5748,   362,    2, "britannia_alt",     312,   787,  -24,	"britannia_alt"},
    {5749,   362,    0, "britannia_alt",     313,   787,  -24,	"britannia_alt"},
    {5750,   362,    3, "britannia_alt",     314,   787,  -24,	"britannia_alt"},

////jhelom teleporters
    {1142,  3621,    5, "britannia_alt",    1414,  3828,    5,	"britannia_alt"},
    {1406,  3996,    5, "britannia_alt",    1414,  3828,    5,	"britannia_alt"},
    {1409,  3824,    5, "britannia_alt",    1124,  3623,    5,	"britannia_alt"},
    {1419,  3832,    5, "britannia_alt",    1466,  4015,    5,	"britannia_alt"},

//// moonglow teleporters
    {4300,   968,    5, "britannia_alt",    4442,  1122,    5,	"britannia_alt"},
    {4436,  1107,    5, "britannia_alt",    4300,   992,    5,	"britannia_alt"},
    {4443,  1137,    5, "britannia_alt",    4487,  1475,    5,	"britannia_alt"},
    {4449,  1107,    5, "britannia_alt",    4539,   890,   28,	"britannia_alt"},
    {4449,  1115,    5, "britannia_alt",    4671,  1135,   10,	"britannia_alt"},
    {4496,  1475,   15, "britannia_alt",    4442,  1122,    5,	"britannia_alt"},
    {4540,   898,   32, "britannia_alt",    4442,  1122,    5,	"britannia_alt"},
    {4663,  1134,   13, "britannia_alt",    4442,  1122,    5,	"britannia_alt"},

////hedge maze
    {1163,  2208,   60, "britannia_alt",    1171,  2202,    0,	"britannia_alt"},
    {1165,  2205,   40, "britannia_alt",    1163,  2209,   60,	"britannia_alt"},
    {1166,  2237,   60, "britannia_alt",    1163,  2208,    0,	"britannia_alt"},

////destard
//enterance
    {1175,  2635,    0, "britannia_alt",    5242,  1006,    0,	"britannia_alt"},
    {1176,  2635,    0, "britannia_alt",    5243,  1006,    0,	"britannia_alt"},
    {1177,  2635,    0, "britannia_alt",    5244,  1006,    0,	"britannia_alt"},
    {5242,  1007,    0, "britannia_alt",    1175,  2636,    0,	"britannia_alt"},
    {5243,  1007,    0, "britannia_alt",    1176,  2636,    0,	"britannia_alt"},
    {5244,  1007,    0, "britannia_alt",    1177,  2636,    0,	"britannia_alt"},
//lvl1 - lvl2
    {5129,   909,  -28, "britannia_alt",    5142,   798,   17,	"britannia_alt"},
    {5130,   909,  -28, "britannia_alt",    5143,   798,   17,	"britannia_alt"},
    {5131,   909,  -28, "britannia_alt",    5144,   798,   17,	"britannia_alt"},
    {5132,   909,  -28, "britannia_alt",    5145,   798,   17,	"britannia_alt"},
    {5142,   797,   22, "britannia_alt",    5129,   908,  -28,	"britannia_alt"},
    {5143,   797,   22, "britannia_alt",    5130,   908,  -28,	"britannia_alt"},
    {5144,   797,   22, "britannia_alt",    5131,   908,  -28,	"britannia_alt"},
    {5145,   797,   22, "britannia_alt",    5132,   908,  -28,	"britannia_alt"},
//lvl2 - lvl3
    {5153,   808,  -25, "britannia_alt",    5134,   984,   17,	"britannia_alt"},
    {5153,   809,  -25, "britannia_alt",    5134,   985,   17,	"britannia_alt"},
    {5153,   810,  -25, "britannia_alt",    5134,   986,   17,	"britannia_alt"},
    {5153,   811,  -25, "britannia_alt",    5134,   987,   17,	"britannia_alt"},
    {5133,   984,   22, "britannia_alt",    5152,   808,  -19,	"britannia_alt"},
    {5133,   985,   22, "britannia_alt",    5152,   809,  -19,	"britannia_alt"},
    {5133,   986,   22, "britannia_alt",    5152,   810,  -19,	"britannia_alt"},
    {5133,   987,   22, "britannia_alt",    5152,   811,  -19,	"britannia_alt"},

////shame
//enterance
    { 512,  1559,    0, "britannia_alt",    5394,   126,    0,	"britannia_alt"},
    { 513,  1559,    0, "britannia_alt",    5395,   126,    0,	"britannia_alt"},
    { 514,  1559,    0, "britannia_alt",    5396,   126,    0,	"britannia_alt"},
    {5394,   127,    0, "britannia_alt",     512,  1560,    0,	"britannia_alt"},
    {5395,   127,    0, "britannia_alt",     513,  1560,    0,	"britannia_alt"},
    {5396,   127,    0, "britannia_alt",     514,  1560,    0,	"britannia_alt"},
//lvl1 - lvl2
    {5490,    19,  -30, "britannia_alt",    5514,    10,    0,	"britannia_alt"},
    {5514,    10,    0, "britannia_alt",    5490,    19,  -30,	"britannia_alt"},
//lvl2 - lvl3
    {5604,   102,    5, "britannia_alt",    5514,   148,   25,	"britannia_alt"},
    {5514,   148,   25, "britannia_alt",    5604,   102,    5,	"britannia_alt"},
//lvl3 - lvl4
    {5538,   170,    5, "britannia_alt",    5513,   176,    5,	"britannia_alt"},
    {5513,   176,    5, "britannia_alt",    5538,   170,    5,	"britannia_alt"},
//lvl4 - lvl5
    {5507,   162,    5, "britannia_alt",    5875,    19,   -5,	"britannia_alt"},
    {5875,    19,   -5, "britannia_alt",    5507,   162,    5,	"britannia_alt"},

////deceit
//enterance
    {4110,   430,    5, "britannia_alt",    5187,   638,    0,	"britannia_alt"},
    {4111,   430,    5, "britannia_alt",    5188,   638,    0,	"britannia_alt"},
    {4112,   430,    5, "britannia_alt",    5189,   638,    0,	"britannia_alt"},
    {5187,   639,    0, "britannia_alt",    4110,   431,    5,	"britannia_alt"},
    {5188,   639,    0, "britannia_alt",    4111,   431,    5,	"britannia_alt"},
    {5189,   639,    0, "britannia_alt",    4112,   431,    5,	"britannia_alt"},
//lvl1 - lvl2
    {5216,   587,  -18, "britannia_alt",    5304,   531,   12,	"britannia_alt"},
    {5217,   587,  -18, "britannia_alt",    5305,   531,   12,	"britannia_alt"},
    {5218,   587,  -18, "britannia_alt",    5306,   531,   12,	"britannia_alt"},
    {5304,   530,   17, "britannia_alt",    5216,   586,  -13,	"britannia_alt"},
    {5305,   530,   17, "britannia_alt",    5217,   586,  -13,	"britannia_alt"},
    {5306,   530,   17, "britannia_alt",    5218,   586,  -13,	"britannia_alt"},
//lvl2 - lvl3
    {5346,   578,    5, "britannia_alt",    5137,   649,    5,	"britannia_alt"},
    {5137,   649,    5, "britannia_alt",    5346,   578,    5,	"britannia_alt"},
//lvl3 - lvl4
    {5218,   761,  -28, "britannia_alt",    5305,   650,   12,	"britannia_alt"},
    {5219,   761,  -28, "britannia_alt",    5306,   650,   12,	"britannia_alt"},
    {5305,   649,   17, "britannia_alt",    5218,   760,    1,	"britannia_alt"},
    {5306,   549,   17, "britannia_alt",    5219,   760,    1,	"britannia_alt"},

////wrong
//enterance
    {2041,   215,   14, "britannia_alt",    5824,   630,    0,	"britannia_alt"},
    {2042,   215,   14, "britannia_alt",    5825,   630,    0,	"britannia_alt"},
    {2043,   215,   14, "britannia_alt",    5826,   630,    0,	"britannia_alt"},
    {5824,   631,    0, "britannia_alt",    2041,   216,   14,	"britannia_alt"},
    {5825,   631,    0, "britannia_alt",    2042,   216,   14,	"britannia_alt"},
    {5826,   631,    0, "britannia_alt",    2043,   216,   14,	"britannia_alt"},
//lvl1 - lvl2
    {5826,   592,    0, "britannia_alt",    5690,   569,   25,	"britannia_alt"},
    {5690,   569,   25, "britannia_alt",    5826,   592,    0,	"britannia_alt"},
//lvl2 - lvl3
    {5736,   551,   20, "britannia_alt",    5703,   639,    0,	"britannia_alt"},
    {5698,   662,    0, "britannia_alt",    5736,   551,   20,	"britannia_alt"},

////despise
//enterance
    {1296,  1080,    0, "britannia_alt",    5587,   630,   30,	"britannia_alt"},
    {1296,  1081,    0, "britannia_alt",    5587,   631,   30,	"britannia_alt"},
    {1296,  1082,    0, "britannia_alt",    5587,   632,   30,	"britannia_alt"},
    {5588,   630,   30, "britannia_alt",    1297,  1080,    0,	"britannia_alt"},
    {5588,   631,   30, "britannia_alt",    1297,  1081,    0,	"britannia_alt"},
    {5588,   632,   30, "britannia_alt",    1297,  1082,    0,	"britannia_alt"},
//lvl1 - lvl2
    {5570,   628,   57, "britannia_alt",    5504,   569,   46,	"britannia_alt"},
    {5570,   629,   57, "britannia_alt",    5504,   570,   46,	"britannia_alt"},
    {5570,   630,   57, "britannia_alt",    5504,   571,   46,	"britannia_alt"},
    {5505,   569,   41, "britannia_alt",    5571,   628,   52,	"britannia_alt"},
    {5505,   570,   41, "britannia_alt",    5571,   629,   52,	"britannia_alt"},
    {5505,   571,   41, "britannia_alt",    5571,   630,   52,	"britannia_alt"},
//lvl2 - lvl3
    {5571,   632,   12, "britannia_alt",    5522,   672,   32,	"britannia_alt"},
    {5571,   633,   12, "britannia_alt",    5522,   673,   32,	"britannia_alt"},
    {5571,   634,   12, "britannia_alt",    5522,   674,   32,	"britannia_alt"},
    {5523,   672,   37, "britannia_alt",    5572,   632,   17,	"britannia_alt"},
    {5523,   673,   37, "britannia_alt",    5572,   633,   17,	"britannia_alt"},
    {5523,   674,   37, "britannia_alt",    5572,   634,   17,	"britannia_alt"},
//lvl3 - lvl4
    {5385,   755,  -13, "britannia_alt",    5410,   858,   57,	"britannia_alt"},
    {5385,   756,  -13, "britannia_alt",    5410,   859,   57,	"britannia_alt"},
    {5385,   757,  -13, "britannia_alt",    5410,   860,   57,	"britannia_alt"},
    {5411,   858,   62, "britannia_alt",    5386,   755,   -8,	"britannia_alt"},
    {5411,   859,   62, "britannia_alt",    5386,   756,   -8,	"britannia_alt"},
    {5411,   860,   62, "britannia_alt",    5386,   757,   -8,	"britannia_alt"},

////covetous
//entrance
    {5455,  1864,    0, "britannia_alt",    2498,   917,    0,	"britannia_alt"},
    {5456,  1864,    0, "britannia_alt",    2499,   917,    0,	"britannia_alt"},
    {5457,  1864,    0, "britannia_alt",    2500,   917,    0,	"britannia_alt"},
    {2498,   916,    0, "britannia_alt",    5455,  1863,    0,	"britannia_alt"},
    {2499,   916,    0, "britannia_alt",    5456,  1863,    0,	"britannia_alt"},
    {2500,   916,    0, "britannia_alt",    5457,  1863,    0,	"britannia_alt"},
//lvl1 - lvl2
    {5392,  1959,    0, "britannia_alt",    2420,   883,    0,	"britannia_alt"},
    {5393,  1959,    0, "britannia_alt",    2421,   883,    0,	"britannia_alt"},
    {5394,  1959,    0, "britannia_alt",    2422,   883,    0,	"britannia_alt"},
    {2420,   884,    0, "britannia_alt",    5392,  1958,    0,	"britannia_alt"},
    {2421,   884,    0, "britannia_alt",    5393,  1958,    0,	"britannia_alt"},
    {2422,   884,    0, "britannia_alt",    5394,  1958,    0,	"britannia_alt"},
//lvl2 - lvl3
    {2384,   836,    0, "britannia_alt",    5614,  1996,    0,	"britannia_alt"},
    {2384,   837,    0, "britannia_alt",    5614,  1997,    0,	"britannia_alt"},
    {2384,   838,    0, "britannia_alt",    5614,  1998,    0,	"britannia_alt"},
    {5615,  1996,    0, "britannia_alt",    2385,   836,    0,	"britannia_alt"},
    {5615,  1997,    0, "britannia_alt",    2385,   837,    0,	"britannia_alt"},
    {5615,  1998,    0, "britannia_alt",    2385,   838,    0,	"britannia_alt"},
//lvl3 - lvl4
    {5388,  2027,    0, "britannia_alt",    2455,   859,    0,	"britannia_alt"},
    {5389,  2027,    0, "britannia_alt",    2456,   859,    0,	"britannia_alt"},
    {5390,  2027,    0, "britannia_alt",    2457,   859,    0,	"britannia_alt"},
    {2455,   858,    0, "britannia_alt",    5388,  2026,    0,	"britannia_alt"},
    {2456,   858,    0, "britannia_alt",    5389,  2026,    0,	"britannia_alt"},
    {2457,   858,    0, "britannia_alt",    5390,  2026,    0,	"britannia_alt"},
//lvl4 - lvl5
    {2544,   846,    0, "britannia_alt",    5578,  1926,    0,	"britannia_alt"},
    {2545,   846,    0, "britannia_alt",    5579,  1926,    0,	"britannia_alt"},
    {2546,   846,    0, "britannia_alt",    5580,  1926,    0,	"britannia_alt"},
    {5578,  1927,    0, "britannia_alt",    2544,   847,    0,	"britannia_alt"},
    {5579,  1927,    0, "britannia_alt",    2545,   847,    0,	"britannia_alt"},
    {5580,  1927,    0, "britannia_alt",    2546,   847,    0,	"britannia_alt"},
//lvl5-lvl6a
    {5556,  1827,  -15, "britannia_alt",    5551,  1805,   12,	"britannia_alt"},
    {5557,  1827,  -15, "britannia_alt",    5552,  1805,   12,	"britannia_alt"},
    {5551,  1804,   17, "britannia_alt",    5556,  1826,  -10,	"britannia_alt"},
    {5552,  1804,   17, "britannia_alt",    5556,  1826,  -10,	"britannia_alt"},
    {5552,  1804,   17, "britannia_alt",    5557,  1826,  -10,	"britannia_alt"},
//lvl5 - lvl6b
    {5595,  1840,  -14, "britannia_alt",    5466,  1804,   12,	"britannia_alt"},
    {5595,  1841,  -14, "britannia_alt",    5466,  1805,   12,	"britannia_alt"},
    {5465,  1804,   17, "britannia_alt",    5594,  1840,   -9,	"britannia_alt"},
    {5465,  1805,   17, "britannia_alt",    5594,  1841,   -9,	"britannia_alt"},
    {5465,  1805,   17, "britannia_alt",    5594,  1841,   -9,	"britannia_alt"},

////hythloth
//enterance
    {5904,    17,   61, "britannia_alt",    4721,  3814,    0,	"britannia_alt"},
    {5905,    17,   61, "britannia_alt",    4722,  3814,    0,	"britannia_alt"},
    {5906,    17,   61, "britannia_alt",    4723,  3814,    0,	"britannia_alt"},
    {4721,  3813,    0, "britannia_alt",    5904,    18,   56,	"britannia_alt"},
    {4722,  3813,    0, "britannia_alt",    5905,    18,   56,	"britannia_alt"},
    {4723,  3813,    0, "britannia_alt",    5906,    18,   56,	"britannia_alt"},
//lvl1 - lvl2
    {5905,    97,    0, "britannia_alt",    5976,   169,    0,	"britannia_alt"},
    {5976,   169,    0, "britannia_alt",    5905,    97,    0,	"britannia_alt"},
    {5976,   168,    0, "britannia_alt",    5905,    97,    0,	"britannia_alt"},
    {5977,   169,    0, "britannia_alt",    5905,    97,    0,	"britannia_alt"},
    {5977,   168,    0, "britannia_alt",    5905,    97,    0,	"britannia_alt"},
//lvl2 - lvl3
    {5922,   168,    7, "britannia_alt",    6081,   144,  -10,	"britannia_alt"},
    {5922,   169,    7, "britannia_alt",    6081,   145,  -10,	"britannia_alt"},
    {5922,   170,    7, "britannia_alt",    6081,   146,  -10,	"britannia_alt"},
    {6080,   144,   -5, "britannia_alt",    5921,   169,   11,	"britannia_alt"},
    {6080,   145,   -5, "britannia_alt",    5921,   169,   12,	"britannia_alt"},
    {6080,   146,   -5, "britannia_alt",    5921,   169,   11,	"britannia_alt"},
//lvl3 - lvl4
    {6041,   192,    7, "britannia_alt",    6057,    87,   34,	"britannia_alt"},
    {6041,   193,    7, "britannia_alt",    6057,    89,   34,	"britannia_alt"},
    {6041,   194,    9, "britannia_alt",    6057,    90,   34,	"britannia_alt"},
    {6056,    87,   39, "britannia_alt",    6040,   192,   12,	"britannia_alt"},
    {6056,    89,   39, "britannia_alt",    6040,   193,   12,	"britannia_alt"},
    {6056,    90,   39, "britannia_alt",    6040,   194,   12,	"britannia_alt"},

////wind
//enterance
    {5166,   245,   15, "britannia_alt",    1361,   883,    0,	"britannia_alt"},
    {1361,   883,    0, "britannia_alt",    5166,   245,   15,	"britannia_alt"},
//NE exit
    {5191,   152,    0, "britannia_alt",    1361,   883,    0,	"britannia_alt"},
// wind - park
    {5200,    71,   17, "britannia_alt",    5211,    22,   15,	"britannia_alt"},
    {5217,    18,   15, "britannia_alt",    5200,    71,   17,	"britannia_alt"},

////ice dungeon
//enterance
    {5882,   241,    0, "britannia_alt",    1997,    81,    7,	"britannia_alt"},
    {5882,   242,    0, "britannia_alt",    1997,    81,    7,	"britannia_alt"},
    {5882,   243,    0, "britannia_alt",    1997,    81,    7,	"britannia_alt"},
    {1997,    81,    7, "britannia_alt",    5881,   242,    0,	"britannia_alt"},
//lvl1 - orc fort
    {5849,   238,  -25, "britannia_alt",    5831,   324,   27,	"britannia_alt"},
    {5850,   238,  -25, "britannia_alt",    5832,   324,   26,	"britannia_alt"},
    {5851,   238,  -25, "britannia_alt",    5833,   324,   28,	"britannia_alt"},
    {5852,   238,  -25, "britannia_alt",    5834,   324,   27,	"britannia_alt"},
    {5853,   238,  -23, "britannia_alt",    5835,   324,   27,	"britannia_alt"},
    {5831,   323,   34, "britannia_alt",    5849,   237,  -24,	"britannia_alt"},
    {5832,   323,   34, "britannia_alt",    5850,   237,  -24,	"britannia_alt"},
    {5833,   323,   33, "britannia_alt",    5851,   237,  -24,	"britannia_alt"},
    {5834,   323,   33, "britannia_alt",    5852,   237,  -25,	"britannia_alt"},
    {5835,   323,   33, "britannia_alt",    5853,   237,  -23,	"britannia_alt"},
//lvl1 - lvl2
    {5704,   145,  -45, "britannia_alt",    5705,   305,   12,	"britannia_alt"},
    {5704,   146,  -45, "britannia_alt",    5705,   306,   12,	"britannia_alt"},
    {5704,   147,  -45, "britannia_alt",    5705,   307,   12,	"britannia_alt"},
    {5706,   303,   12, "britannia_alt",    5705,   146,  -45,	"britannia_alt"},
    {5706,   304,   12, "britannia_alt",    5705,   146,  -45,	"britannia_alt"},
    {5706,   305,   12, "britannia_alt",    5705,   146,  -45,	"britannia_alt"},
    {5706,   306,   12, "britannia_alt",    5705,   147,  -45,	"britannia_alt"},
    {5706,   307,   12, "britannia_alt",    5705,   147,  -45,	"britannia_alt"},
    {5706,   308,   12, "britannia_alt",    5705,   147,  -45,	"britannia_alt"},
//lvl1 - t2a exit
    {5872,   144,   32, "britannia_alt",    5203,  2327,   27,	"britannia_alt"},
    {5873,   144,   32, "britannia_alt",    5203,  2327,   27,	"britannia_alt"},
    {5874,   144,   32, "britannia_alt",    5203,  2327,   27,	"britannia_alt"},
    {5875,   144,   32, "britannia_alt",    5203,  2327,   27,	"britannia_alt"},
    {5876,   144,   32, "britannia_alt",    5207,  2322,   27,	"britannia_alt"},
    {5877,   144,   32, "britannia_alt",    5207,  2322,   27,	"britannia_alt"},
    {5878,   144,   32, "britannia_alt",    5207,  2323,   26,	"britannia_alt"},
    {5879,   144,   32, "britannia_alt",    5207,  2323,   26,	"britannia_alt"},
    {5203,  2327,   27, "britannia_alt",    5874,   145,   30,	"britannia_alt"},
    {5207,  2322, -255, "britannia_alt",    5875,   145,   30,	"britannia_alt"},
    {5207,  2323, -255, "britannia_alt",    5876,   145,   30,	"britannia_alt"},

////fire dungeon
//enterance
    {5827,  1502,    0, "britannia_alt",    2922,  3406,    8,	"britannia_alt"},
    {5828,  1502,   -1, "britannia_alt",    2923,  3406,    9,	"britannia_alt"},
    {5829,  1502,    2, "britannia_alt",    2924,  3406,    7,	"britannia_alt"},
    {2922,  3405,    7, "britannia_alt",    5827,  1501,    0,	"britannia_alt"},
    {2923,  3405,    6, "britannia_alt",    5828,  1501,    0,	"britannia_alt"},
    {2924,  3405,    7, "britannia_alt",    5829,  1501,    1,	"britannia_alt"},
//lvl1 - t2a exit
    {5790,  1415,   39, "britannia_alt",    5758,  2907,   15,	"britannia_alt"},
    {5790,  1416,   40, "britannia_alt",    5758,  2908,   15,	"britannia_alt"},
    {5790,  1417,   38, "britannia_alt",    5758,  2909,   15,	"britannia_alt"},
    {5757,  2907,   15, "britannia_alt",    5789,  1415,   42,	"britannia_alt"},
    {5757,  2908,   14, "britannia_alt",    5789,  1416,   39,	"britannia_alt"},
    {5757,  2909,   15, "britannia_alt",    5789,  1417,   38,	"britannia_alt"},
//lvl1 - lvl2
    {5786,  1335,  -13, "britannia_alt",    5701,  1318,    8,	"britannia_alt"},
    {5787,  1335,  -13, "britannia_alt",    5702,  1318,    8,	"britannia_alt"},
    {5788,  1335,  -13, "britannia_alt",    5703,  1318,    8,	"britannia_alt"},
    {5701,  1319,   13, "britannia_alt",    5786,  1336,   -8,	"britannia_alt"},
    {5702,  1319,   13, "britannia_alt",    5787,  1336,   -8,	"britannia_alt"},
    {5703,  1319,   13, "britannia_alt",    5788,  1336,   -8,	"britannia_alt"},

////volcano shop teleporter
    {1653,  2963,    0, "britannia_alt",    1671,  2981,    0,	"britannia_alt"},
    {1677,  2987,    0, "britannia_alt",    1675,  2987,   20,	"britannia_alt"},

////bucs den underground
//enterance
    {2674,  2242,    2, "britannia_alt",    2677,  2242,  -20,	"britannia_alt"},
    {2676,  2242,  -18, "britannia_alt",    2675,  2242,    2,	"britannia_alt"},
    {2676,  2242,  -18, "britannia_alt",    2675,  2242,    2,	"britannia_alt"},
//exit
    {2649,  2195,  -14, "britannia_alt",    2648,  2193,    4,	"britannia_alt"},
    {2669,  2072,  -19, "britannia_alt",    2669,  2072,    5,	"britannia_alt"},
    {2669,  2073,  -19, "britannia_alt",    2669,  2073,    5,	"britannia_alt"},

////britain sewer
//enterance
    {6031,  1499,   35, "britannia_alt",    1490,  1640,   24,	"britannia_alt"},
    {6031,  1501,   36, "britannia_alt",    1490,  1642,   24,	"britannia_alt"},
    {1491,  1640,   24, "britannia_alt",    6032,  1499,   31,	"britannia_alt"},
    {1491,  1642,   24, "britannia_alt",    6032,  1501,   31,	"britannia_alt"},
//lvl1 - cave
    {6140,  1431,    4, "britannia_alt",    6137,  1408,    2,	"britannia_alt"},
    {6141,  1431,    4, "britannia_alt",    6138,  1408,    2,	"britannia_alt"},
    {6137,  1409,    2, "britannia_alt",    6140,  1432,    4,	"britannia_alt"},
    {6138,  1409,    2, "britannia_alt",    6141,  1432,    4,	"britannia_alt"},
//cave - t2a exit
    {6126,  1410,   15, "britannia_alt",    6074,  3333,    2,	"britannia_alt"},
    {6127,  1410,   15, "britannia_alt",    6075,  3333,    2,	"britannia_alt"},
    {6074,  3332,    4, "britannia_alt",    6126,  1409,   13,	"britannia_alt"},
    {6075,  3332,    4, "britannia_alt",    6127,  1409,   13,	"britannia_alt"},

////minoc cave
//enterance
    {2399,   198,    0, "britannia_alt",    6087,  3675,   17,	"britannia_alt"},
    {2400,   198,    0, "britannia_alt",    6087,  3676,   18,	"britannia_alt"},
//exit to t2a
    {6086,  3675,   15, "britannia_alt",    2399,   199,    0,	"britannia_alt"},
    {6086,  3676,   16, "britannia_alt",    2400,   199,    0,	"britannia_alt"},

////vesper graveyard
//enterance
    {2776,   894,  -23, "britannia_alt",    5685,   386,    1,	"britannia_alt"},
    {2776,   895,  -23, "britannia_alt",    5685,   387,    2,	"britannia_alt"},
    {5686,   386,    2, "britannia_alt",    2777,   894,  -23,	"britannia_alt"},
    {5686,   387,    2, "britannia_alt",    2777,   895,  -23,	"britannia_alt"},
//cave - t2a graveyard
    {5658,   423,    8, "britannia_alt",    5697,  3659,    2,	"britannia_alt"},
    {5697,  3660,   -5, "britannia_alt",    5658,   424,    0,	"britannia_alt"},

////trinsic cave
//enterance
    {1628,  3320,    0, "britannia_alt",    5899,  1409,   41,	"britannia_alt"},
    {1629,  3320,    0, "britannia_alt",    5900,  1409,   39,	"britannia_alt"},
    {5898,  1411,   38, "britannia_alt",    1628,  3321,    0,	"britannia_alt"},
    {5899,  1411,   43, "britannia_alt",    1628,  3321,    0,	"britannia_alt"},
    {5900,  1411,   42, "britannia_alt",    1629,  3321,    0,	"britannia_alt"},
//lvl1 - lvl2
    {5918,  1410,  -29, "britannia_alt",    5962,  1407,   57,	"britannia_alt"},
    {5918,  1411,  -29, "britannia_alt",    5962,  1408,   56,	"britannia_alt"},
    {5918,  1412,  -29, "britannia_alt",    5962,  1409,   56,	"britannia_alt"},
    {5961,  1407,   59, "britannia_alt",    5917,  1410,  -27,	"britannia_alt"},
    {5961,  1408,   59, "britannia_alt",    5917,  1411,  -27,	"britannia_alt"},
    {5961,  1409,   59, "britannia_alt",    5917,  1412,  -27,	"britannia_alt"},
//lvl2 - west t2a
    {6025,  1344,  -26, "britannia_alt",    5138,  3664,   31,	"britannia_alt"},
    {6025,  1345,  -25, "britannia_alt",    5138,  3665,   32,	"britannia_alt"},
    {6025,  1346,  -25, "britannia_alt",    5138,  3666,   32,	"britannia_alt"},
    {5137,  3664,   27, "britannia_alt",    6024,  1345,  -25,	"britannia_alt"},
    {5137,  3665,   31, "britannia_alt",    6024,  1346,  -25,	"britannia_alt"},
//lvl2 - delucia
    {6005,  1378,    0, "britannia_alt",    5151,  4062,   37,	"britannia_alt"},
    {6005,  1379,    2, "britannia_alt",    5151,  4063,   37,	"britannia_alt"},
    {6005,  1380,    1, "britannia_alt",    5151,  4064,   37,	"britannia_alt"},
    {5150,  4062,   38, "britannia_alt",    6004,  1378,    1,	"britannia_alt"},
    {5150,  4063,   38, "britannia_alt",    6004,  1379,   -1,	"britannia_alt"},

////terathan keep
//enterance
    {5426,  3122, -74, "britannia_alt",    5341,  1601,    0,	"britannia_alt"},
    {5341,  1601,   0, "britannia_alt",    5426,  3121,  -65,	"britannia_alt"},

////spirituality shrine
    {1592,  2489,   17, "britannia_alt",    1600,  2489,   10,	"britannia_alt"},
    {1600,  2489,   10, "britannia_alt",    1592,  2490,   17,	"britannia_alt"},
    {1600,  2490,   10, "britannia_alt",    1592,  2490,   17,	"britannia_alt"},

// Lost Lands
	{ 6303, 327, 70, "britannia_alt",	5577, 3018, 25, "britannia_alt" },
	{ 5578, 3018, 25, "britannia_alt", 6304, 329, 60, "britannia_alt" },
	{ 6303, 328, 65, "britannia_alt", 5579, 3019, 25, "britannia_alt" },
	{ 5579, 3017, 25, "britannia_alt", 6305, 330, 60, "britannia_alt" },

// Ilshenar
	{ 10, 872, -28, "ilshenar", 10, 1519, -28, "ilshenar" },
	{ 11, 872, -28, "ilshenar", 11, 1519, -27, "ilshenar" },
	{ 12, 872, -27, "ilshenar", 12, 1519, -27, "ilshenar" },
	{ 18, 1198, -5, "ilshenar", 348, 1428, 15, "ilshenar" },
	{ 19, 1198, -5, "ilshenar", 349, 1428, 15, "ilshenar" },
	{ 20, 1198, -5, "ilshenar", 350, 1428, 15, "ilshenar" },
	{ 21, 1198, -5, "ilshenar", 351, 1428, 15, "ilshenar" },
	{ 3, 1269, -8, "ilshenar", 666, 928, -83, "ilshenar" },
	{ 3, 1268, -8, "ilshenar", 666, 929, -83, "ilshenar" },
	{ 3, 1267, -8, "ilshenar", 666, 930, -83, "ilshenar" },
	{ 10, 1520, -29, "ilshenar", 10, 873, -28, "ilshenar" },
	{ 11, 1520, -28, "ilshenar", 11, 873, -28, "ilshenar" },
	{ 12, 1520, -30, "ilshenar", 12, 873, -27, "ilshenar" },
	{ 127, 12, -28, "ilshenar", 132, 12, -28, "ilshenar" },
	{ 83, 749, -23, "ilshenar", 938, 495, -35, "ilshenar" },
	{ 84, 749, -23, "ilshenar", 939, 495, -35, "ilshenar" },
	{ 85, 749, -23, "ilshenar", 940, 495, -35, "ilshenar" },
	{ 86, 749, -23, "ilshenar", 941, 495, -35, "ilshenar" },
	{ 77, 1364, -43, "ilshenar", 392, 1586, -13, "ilshenar" },
	{ 78, 1364, -46, "ilshenar", 393, 1586, -16, "ilshenar" },
	{ 79, 1364, -46, "ilshenar", 394, 1586, -16, "ilshenar" },
	{ 80, 1364, -46, "ilshenar", 395, 1586, -16, "ilshenar" },
	{ 81, 1364, -46, "ilshenar", 396, 1586, -16, "ilshenar" },
	{ 82, 1364, -43, "ilshenar", 397, 1586, -13, "ilshenar" },
	{ 131, 12, -28, "ilshenar", 127, 12, -28, "ilshenar" },
	{ 154, 88, -11, "ilshenar", 359, 40, -41, "ilshenar" },
	{ 154, 89, -11, "ilshenar", 359, 41, -41, "ilshenar" },
	{ 154, 90, -11, "ilshenar", 359, 42, -41, "ilshenar" },
	{ 131, 129, -18, "ilshenar", 265, 129, -36, "ilshenar" },
	{ 132, 129, -18, "ilshenar", 266, 129, -36, "ilshenar" },
	{ 133, 129, -18, "ilshenar", 267, 129, -36, "ilshenar" },
	{ 134, 129, -18, "ilshenar", 268, 129, -36, "ilshenar" },
	{ 165, 746, -16, "ilshenar", 637, 816, -47, "ilshenar" },
	{ 164, 746, -16, "ilshenar", 637, 816, -47, "ilshenar" },
	{ 156, 1472, -8, "ilshenar", 577, 1154, -116, "ilshenar" },
	{ 155, 1472, -8, "ilshenar", 576, 1154, -116, "ilshenar" },
	{ 154, 1472, -8, "ilshenar", 575, 1154, -116, "ilshenar" },
	{ 242, 27, -18, "ilshenar", 370, 31, -41, "ilshenar" },
	{ 242, 26, -18, "ilshenar", 370, 30, -41, "ilshenar" },
	{ 242, 25, -18, "ilshenar", 370, 29, -41, "ilshenar" },
	{ 225, 1334, -20, "ilshenar", 265, 1586, -28, "ilshenar" },
	{ 226, 1334, -20, "ilshenar", 266, 1586, -28, "ilshenar" },
	{ 227, 1334, -20, "ilshenar", 267, 1586, -28, "ilshenar" },
	{ 272, 141, -18, "ilshenar", 555, 427, -1, "ilshenar" },
	{ 273, 141, -18, "ilshenar", 556, 427, -1, "ilshenar" },
	{ 274, 141, -18, "ilshenar", 557, 427, -1, "ilshenar" },
	{ 265, 128, -41, "ilshenar", 131, 128, -21, "ilshenar" },
	{ 266, 128, -41, "ilshenar", 132, 128, -21, "ilshenar" },
	{ 267, 128, -41, "ilshenar", 133, 128, -21, "ilshenar" },
	{ 268, 128, -41, "ilshenar", 134, 128, -21, "ilshenar" },
	{ 313, 1329, -40, "ilshenar", 327, 1592, -13, "ilshenar" },
	{ 314, 1329, -36, "ilshenar", 328, 1592, -13, "ilshenar" },
	{ 315, 1329, -38, "ilshenar", 329, 1592, -13, "ilshenar" },
	{ 265, 1587, -28, "ilshenar", 225, 1335, -20, "ilshenar" },
	{ 266, 1587, -28, "ilshenar", 226, 1335, -20, "ilshenar" },
	{ 267, 1587, -28, "ilshenar", 227, 1335, -20, "ilshenar" },
	{ 369, 31, -46, "ilshenar", 241, 27, -18, "ilshenar" },
	{ 369, 30, -46, "ilshenar", 241, 26, -18, "ilshenar" },
	{ 369, 29, -46, "ilshenar", 241, 25, -18, "ilshenar" },
	{ 360, 40, -46, "ilshenar", 155, 88, -16, "ilshenar" },
	{ 360, 41, -46, "ilshenar", 155, 89, -16, "ilshenar" },
	{ 360, 42, -46, "ilshenar", 155, 90, -16, "ilshenar" },
	{ 348, 1427, 15, "ilshenar", 18, 1197, -6, "ilshenar" },
	{ 349, 1427, 15, "ilshenar", 19, 1197, -6, "ilshenar" },
	{ 350, 1427, 15, "ilshenar", 20, 1197, -6, "ilshenar" },
	{ 351, 1427, 15, "ilshenar", 21, 1197, -6, "ilshenar" },
	{ 327, 1593, -13, "ilshenar", 313, 1330, -39, "ilshenar" },
	{ 328, 1593, -13, "ilshenar", 314, 1330, -37, "ilshenar" },
	{ 329, 1593, -13, "ilshenar", 315, 1330, -35, "ilshenar" },
	{ 330, 1593, -13, "ilshenar", 315, 1330, -35, "ilshenar" },
	{ 429, 113, -28, "ilshenar", 549, 456, -40, "ilshenar" },
	{ 428, 113, -28, "ilshenar", 548, 456, -40, "ilshenar" },
	{ 427, 113, -28, "ilshenar", 547, 456, -40, "ilshenar" },
	{ 426, 113, -28, "ilshenar", 546, 456, -40, "ilshenar" },
	{ 392, 1587, -8, "ilshenar", 77, 1365, -38, "ilshenar" },
	{ 393, 1587, -13, "ilshenar", 78, 1365, -41, "ilshenar" },
	{ 394, 1587, -13, "ilshenar", 79, 1365, -41, "ilshenar" },
	{ 395, 1587, -13, "ilshenar", 80, 1365, -41, "ilshenar" },
	{ 396, 1587, -13, "ilshenar", 81, 1365, -41, "ilshenar" },
	{ 397, 1587, -8, "ilshenar", 82, 1365, -38, "ilshenar" },
	{ 555, 426, -6, "ilshenar", 272, 140, -21, "ilshenar" },
	{ 556, 426, -6, "ilshenar", 273, 140, -21, "ilshenar" },
	{ 557, 426, -6, "ilshenar", 274, 140, -21, "ilshenar" },
	{ 548, 455, -40, "ilshenar", 429, 112, -28, "ilshenar" },
	{ 547, 455, -40, "ilshenar", 428, 112, -28, "ilshenar" },
	{ 546, 455, -40, "ilshenar", 427, 112, -28, "ilshenar" },
	{ 575, 1155, -121, "ilshenar", 154, 1473, -8, "ilshenar" },
	{ 534, 1533, -4, "ilshenar", 812, 875, -39, "ilshenar" },
	{ 533, 1533, -4, "ilshenar", 811, 875, -39, "ilshenar" },
	{ 532, 1533, -4, "ilshenar", 810, 875, -40, "ilshenar" },
	{ 636, 815, -52, "ilshenar", 164, 745, -21, "ilshenar" },
	{ 637, 815, -52, "ilshenar", 165, 745, -21, "ilshenar" },
	{ 577, 1155, -120, "ilshenar", 156, 1473, -8, "ilshenar" },
	{ 576, 1155, -120, "ilshenar", 155, 1473, -8, "ilshenar" },
	{ 626, 1527, -28, "ilshenar", 650, 1298, -59, "ilshenar" },
	{ 627, 1527, -28, "ilshenar", 651, 1298, -59, "ilshenar" },
	{ 628, 1527, -28, "ilshenar", 652, 1298, -59, "ilshenar" },
	{ 629, 1527, -28, "ilshenar", 653, 1298, -59, "ilshenar" },
	{ 667, 928, -85, "ilshenar", 4, 1269, -11, "ilshenar" },
	{ 667, 929, -85, "ilshenar", 4, 1268, -11, "ilshenar" },
	{ 667, 930, -85, "ilshenar", 4, 1267, -11, "ilshenar" },
	{ 650, 1297, -58, "ilshenar", 626, 1526, -28, "ilshenar" },
	{ 651, 1297, -58, "ilshenar", 627, 1526, -28, "ilshenar" },
	{ 652, 1297, -58, "ilshenar", 628, 1526, -28, "ilshenar" },
	{ 653, 1297, -58, "ilshenar", 629, 1526, -28, "ilshenar" },
	{ 694, 1493, -53, "ilshenar", 713, 1493, -6, "ilshenar" },
	{ 694, 1492, -53, "ilshenar", 713, 1492, -6, "ilshenar" },
	{ 694, 1491, -53, "ilshenar", 713, 1491, -6, "ilshenar" },
	{ 694, 1490, -53, "ilshenar", 713, 1490, -6, "ilshenar" },
	{ 709, 667, -39, "ilshenar", 913, 451, -80, "ilshenar" },
	{ 710, 667, -39, "ilshenar", 913, 452, -80, "ilshenar" },
	{ 711, 667, -39, "ilshenar", 913, 453, -80, "ilshenar" },
	{ 712, 1493, -3, "ilshenar", 693, 1493, -53, "ilshenar" },
	{ 712, 1492, -3, "ilshenar", 693, 1492, -53, "ilshenar" },
	{ 712, 1491, -3, "ilshenar", 693, 1491, -53, "ilshenar" },
	{ 712, 1490, -3, "ilshenar", 693, 1490, -53, "ilshenar" },
	{ 722, 1508, -53, "ilshenar", 837, 1553, -11, "ilshenar" },
	{ 722, 1507, -53, "ilshenar", 837, 1552, -11, "ilshenar" },
	{ 722, 1506, -53, "ilshenar", 837, 1551, -11, "ilshenar" },
	{ 722, 1505, -53, "ilshenar", 837, 1550, -11, "ilshenar" },
	{ 751, 1479, -28, "ilshenar", 763, 1555, -28, "ilshenar" },
	{ 751, 1484, -28, "ilshenar", 763, 1480, -28, "ilshenar" },
	{ 763, 1479, -28, "ilshenar", 751, 1483, -28, "ilshenar" },
	{ 747, 1539, -28, "ilshenar", 785, 1515, -28, "ilshenar" },
	{ 747, 1555, -28, "ilshenar", 785, 1571, -28, "ilshenar" },
	{ 752, 1549, -28, "ilshenar", 751, 1473, -28, "ilshenar" },
	{ 828, 778, -80, "ilshenar", 1980, 114, -28, "ilshenar" },
	{ 812, 874, -39, "ilshenar", 534, 1532, -7, "ilshenar" },
	{ 811, 874, -39, "ilshenar", 533, 1532, -7, "ilshenar" },
	{ 810, 874, -39, "ilshenar", 532, 1532, -7, "ilshenar" },
	{ 775, 1467, -28, "ilshenar", 658, 1498, -28, "ilshenar" },
	{ 785, 1514, -28, "ilshenar", 748, 1539, -28, "ilshenar" },
	{ 775, 1492, -28, "ilshenar", 827, 1515, -28, "ilshenar" },
	{ 813, 1549, -1, "ilshenar", 841, 1437, -53, "ilshenar" },
	{ 813, 1548, -1, "ilshenar", 841, 1436, -53, "ilshenar" },
	{ 813, 1547, -1, "ilshenar", 841, 1435, -53, "ilshenar" },
	{ 813, 1546, -1, "ilshenar", 841, 1434, -53, "ilshenar" },
	{ 813, 1545, -1, "ilshenar", 841, 1433, -53, "ilshenar" },
	{ 785, 1570, -28, "ilshenar", 748, 1555, -28, "ilshenar" },
	{ 871, 1437, -6, "ilshenar", 893, 1453, -46, "ilshenar" },
	{ 892, 1453, -51, "ilshenar", 870, 1437, -11, "ilshenar" },
	{ 871, 1435, -6, "ilshenar", 893, 1451, -46, "ilshenar" },
	{ 892, 1451, -51, "ilshenar", 870, 1435, -11, "ilshenar" },
	{ 871, 1434, -6, "ilshenar", 893, 1450, -46, "ilshenar" },
	{ 892, 1450, -51, "ilshenar", 870, 1434, -11, "ilshenar" },
	{ 871, 1433, -6, "ilshenar", 893, 1449, -46, "ilshenar" },
	{ 892, 1449, -51, "ilshenar", 870, 1433, -11, "ilshenar" },
	{ 871, 1436, -6, "ilshenar", 894, 1452, -41, "ilshenar" },
	{ 840, 1437, -53, "ilshenar", 812, 1549, -6, "ilshenar" },
	{ 840, 1436, -53, "ilshenar", 812, 1548, -6, "ilshenar" },
	{ 840, 1435, -53, "ilshenar", 812, 1547, -6, "ilshenar" },
	{ 840, 1434, -53, "ilshenar", 812, 1546, -6, "ilshenar" },
	{ 840, 1433, -53, "ilshenar", 812, 1545, -6, "ilshenar" },
	{ 879, 1490, 24, "ilshenar", 952, 1425, -53, "ilshenar" },
	{ 879, 1491, 24, "ilshenar", 952, 1426, -53, "ilshenar" },
	{ 879, 1492, 24, "ilshenar", 952, 1427, -53, "ilshenar" },
	{ 879, 1493, 24, "ilshenar", 952, 1428, -53, "ilshenar" },
	{ 838, 1553, -6, "ilshenar", 723, 1508, -51, "ilshenar" },
	{ 838, 1552, -6, "ilshenar", 723, 1507, -51, "ilshenar" },
	{ 838, 1551, -6, "ilshenar", 723, 1506, -51, "ilshenar" },
	{ 838, 1550, -6, "ilshenar", 723, 1505, -51, "ilshenar" },
	{ 912, 451, -80, "ilshenar", 709, 668, -39, "ilshenar" },
	{ 912, 452, -80, "ilshenar", 710, 668, -39, "ilshenar" },
	{ 912, 453, -80, "ilshenar", 711, 668, -38, "ilshenar" },
	{ 938, 494, -40, "ilshenar", 83, 748, -26, "ilshenar" },
	{ 939, 494, -40, "ilshenar", 84, 748, -26, "ilshenar" },
	{ 940, 494, -40, "ilshenar", 85, 748, -26, "ilshenar" },
	{ 941, 494, -40, "ilshenar", 86, 748, -26, "ilshenar" },
	{ 950, 509, -60, "ilshenar", 943, 509, -60, "ilshenar" },
	{ 944, 509, -60, "ilshenar", 951, 509, -60, "ilshenar" },
	{ 944, 498, 0, "ilshenar", 960, 489, 0, "ilshenar" },
	{ 959, 489, 0, "ilshenar", 943, 498, 0, "ilshenar" },
	{ 940, 499, 0, "ilshenar", 939, 503, -31, "ilshenar" },
	{ 904, 1360, -21, "ilshenar", 1013, 1506, 0, "ilshenar" },
	{ 904, 1361, -21, "ilshenar", 1013, 1507, 0, "ilshenar" },
	{ 904, 1362, -21, "ilshenar", 1013, 1508, 0, "ilshenar" },
	{ 904, 1363, -21, "ilshenar", 1013, 1509, 0, "ilshenar" },
	{ 951, 1445, -6, "ilshenar", 947, 1467, -51, "ilshenar" },
	{ 948, 1467, -56, "ilshenar", 952, 1445, -11, "ilshenar" },
	{ 951, 1444, -6, "ilshenar", 947, 1466, -51, "ilshenar" },
	{ 948, 1466, -56, "ilshenar", 952, 1444, -11, "ilshenar" },
	{ 951, 1443, -6, "ilshenar", 947, 1465, -51, "ilshenar" },
	{ 948, 1465, -56, "ilshenar", 952, 1443, -11, "ilshenar" },
	{ 951, 1442, -6, "ilshenar", 947, 1464, -51, "ilshenar" },
	{ 948, 1464, -56, "ilshenar", 952, 1442, -11, "ilshenar" },
	{ 951, 1424, -53, "ilshenar", 878, 1490, 19, "ilshenar" },
	{ 951, 1425, -53, "ilshenar", 878, 1490, 19, "ilshenar" },
	{ 951, 1426, -53, "ilshenar", 878, 1491, 19, "ilshenar" },
	{ 951, 1427, -53, "ilshenar", 878, 1492, 19, "ilshenar" },
	{ 951, 1428, -53, "ilshenar", 878, 1493, 19, "ilshenar" },
	{ 951, 1429, -53, "ilshenar", 878, 1493, 19, "ilshenar" },
	{ 1014, 1506, 0, "ilshenar", 905, 1360, -21, "ilshenar" },
	{ 1014, 1507, 0, "ilshenar", 905, 1361, -21, "ilshenar" },
	{ 1014, 1508, 0, "ilshenar", 905, 1362, -21, "ilshenar" },
	{ 1014, 1509, 0, "ilshenar", 905, 1363, -21, "ilshenar" },
	{ 1066, 651, -58, "ilshenar", 1788, 68, -26, "ilshenar" },
	{ 1029, 1153, -24, "ilshenar", 1348, 1510, -3, "ilshenar" },
	{ 1029, 1154, -24, "ilshenar", 1348, 1511, -3, "ilshenar" },
	{ 1029, 1155, -24, "ilshenar", 1348, 1512, -3, "ilshenar" },
	{ 1139, 593, -80, "ilshenar", 1238, 582, -19, "ilshenar" },
	{ 1140, 593, -80, "ilshenar", 1238, 583, -19, "ilshenar" },
	{ 1141, 593, -80, "ilshenar", 1238, 584, -19, "ilshenar" },
	{ 1142, 593, -80, "ilshenar", 1238, 586, -19, "ilshenar" },
	{ 1237, 582, -19, "ilshenar", 1139, 594, -80, "ilshenar" },
	{ 1237, 583, -19, "ilshenar", 1140, 594, -80, "ilshenar" },
	{ 1237, 584, -19, "ilshenar", 1141, 594, -80, "ilshenar" },
	{ 1237, 585, -19, "ilshenar", 1142, 594, -80, "ilshenar" },
	{ 1237, 586, -19, "ilshenar", 1143, 594, -80, "ilshenar" },
	{ 1250, 1512, -28, "ilshenar", 1269, 1510, -28, "ilshenar" },
	{ 1268, 1510, -28, "ilshenar", 1249, 1512, -28, "ilshenar" },
	{ 1250, 1511, -28, "ilshenar", 1269, 1509, -28, "ilshenar" },
	{ 1268, 1509, -28, "ilshenar", 1249, 1511, -28, "ilshenar" },
	{ 1250, 1510, -28, "ilshenar", 1269, 1509, -28, "ilshenar" },
	{ 1268, 1508, -28, "ilshenar", 1249, 1510, -28, "ilshenar" },
	{ 1250, 1509, -28, "ilshenar", 1269, 1508, -28, "ilshenar" },
	{ 1250, 1508, -28, "ilshenar", 1269, 1508, -28, "ilshenar" },
	{ 1362, 1031, -13, "ilshenar", 1981, 1106, -21, "ilshenar" },
	{ 1363, 1031, -13, "ilshenar", 1982, 1106, -21, "ilshenar" },
	{ 1364, 1031, -13, "ilshenar", 1983, 1106, -21, "ilshenar" },
	{ 1363, 1105, -26, "ilshenar", 1515, 880, 10, "ilshenar" },
	{ 1349, 1509, -3, "ilshenar", 1030, 1153, -24, "ilshenar" },
	{ 1349, 1510, -3, "ilshenar", 1030, 1153, -24, "ilshenar" },
	{ 1349, 1511, -3, "ilshenar", 1030, 1154, -24, "ilshenar" },
	{ 1349, 1512, -3, "ilshenar", 1030, 1155, -24, "ilshenar" },
	{ 1419, 909, -10, "ilshenar", 1785, 993, -28, "ilshenar" },
	{ 1420, 909, -10, "ilshenar", 1786, 993, -28, "ilshenar" },
	{ 1421, 909, -10, "ilshenar", 1787, 993, -28, "ilshenar" },
	{ 1456, 1328, -27, "ilshenar", 1478, 1494, -28, "ilshenar" },
	{ 1456, 1329, -27, "ilshenar", 1478, 1495, -28, "ilshenar" },
	{ 1456, 1330, -27, "ilshenar", 1478, 1497, -28, "ilshenar" },
	{ 1489, 880, 10, "ilshenar", 1860, 983, -28, "ilshenar" },
	{ 1489, 879, 10, "ilshenar", 1860, 982, -28, "ilshenar" },
	{ 1489, 878, 10, "ilshenar", 1860, 981, -28, "ilshenar" },
	{ 1489, 877, 10, "ilshenar", 1860, 980, -28, "ilshenar" },
	{ 1516, 879, 10, "ilshenar", 1364, 1104, -26, "ilshenar" },
	{ 1479, 1493, -28, "ilshenar", 1457, 1328, -26, "ilshenar" },
	{ 1479, 1494, -28, "ilshenar", 1457, 1328, -26, "ilshenar" },
	{ 1479, 1495, -28, "ilshenar", 1457, 1329, -26, "ilshenar" },
	{ 1479, 1496, -28, "ilshenar", 1457, 1330, -25, "ilshenar" },
	{ 1479, 1497, -28, "ilshenar", 1457, 1330, -25, "ilshenar" },
	{ 1788, 68, -26, "ilshenar", 1066, 651, -58, "ilshenar" },
	{ 1787, 569, 74, "ilshenar", 2188, 319, -7, "ilshenar" },
	{ 1788, 569, 74, "ilshenar", 2189, 319, -7, "ilshenar" },
	{ 1783, 994, -28, "ilshenar", 1419, 910, -10, "ilshenar" },
	{ 1784, 994, -28, "ilshenar", 1419, 910, -10, "ilshenar" },
	{ 1785, 994, -28, "ilshenar", 1420, 910, -9, "ilshenar" },
	{ 1786, 994, -28, "ilshenar", 1420, 910, -9, "ilshenar" },
	{ 1787, 994, -28, "ilshenar", 1421, 910, -10, "ilshenar" },
	{ 1788, 994, -28, "ilshenar", 1421, 910, -10, "ilshenar" },
	{ 1745, 1236, -30, "ilshenar", 2112, 830, -16, "ilshenar" },
	{ 1746, 1236, -30, "ilshenar", 2113, 830, -16, "ilshenar" },
	{ 1747, 1236, -30, "ilshenar", 2114, 830, -16, "ilshenar" },
	{ 1748, 1236, -31, "ilshenar", 2115, 830, -16, "ilshenar" },
	{ 1861, 984, -28, "ilshenar", 1490, 880, 10, "ilshenar" },
	{ 1861, 983, -28, "ilshenar", 1490, 880, 10, "ilshenar" },
	{ 1861, 982, -28, "ilshenar", 1490, 879, 10, "ilshenar" },
	{ 1861, 981, -28, "ilshenar", 1490, 878, 10, "ilshenar" },
	{ 1861, 980, -28, "ilshenar", 1490, 877, 10, "ilshenar" },
	{ 1861, 979, -28, "ilshenar", 1490, 877, 10, "ilshenar" },
	{ 1979, 115, -28, "ilshenar", 827, 779, -80, "ilshenar" },
	{ 1981, 1107, -18, "ilshenar", 1362, 1032, -11, "ilshenar" },
	{ 1982, 1107, -18, "ilshenar", 1363, 1032, -11, "ilshenar" },
	{ 1983, 1107, -18, "ilshenar", 1364, 1032, -11, "ilshenar" },
	{ 2112, 829, -13, "ilshenar", 1745, 1235, -29, "ilshenar" },
	{ 2113, 829, -13, "ilshenar", 1746, 1235, -29, "ilshenar" },
	{ 2114, 829, -13, "ilshenar", 1747, 1235, -29, "ilshenar" },
	{ 2115, 829, -13, "ilshenar", 1748, 1235, -29, "ilshenar" },
	{ 2116, 829, -13, "ilshenar", 1748, 1235, -29, "ilshenar" },
	{ 2186, 33, -27, "ilshenar", 2186, 295, -27, "ilshenar" },
	{ 2187, 33, -27, "ilshenar", 2187, 295, -27, "ilshenar" },
	{ 2188, 33, -27, "ilshenar", 2188, 295, -27, "ilshenar" },
	{ 2189, 33, -27, "ilshenar", 2189, 295, -27, "ilshenar" },
	{ 2186, 294, -27, "ilshenar", 2186, 32, -27, "ilshenar" },
	{ 2187, 294, -27, "ilshenar", 2187, 32, -27, "ilshenar" },
	{ 2188, 294, -27, "ilshenar", 2188, 32, -27, "ilshenar" },
	{ 2189, 294, -27, "ilshenar", 2189, 32, -27, "ilshenar" },
	{ 2187, 320, -7, "ilshenar", 1787, 570, 71, "ilshenar" },
	{ 2188, 320, -7, "ilshenar", 1787, 570, 71, "ilshenar" },
	{ 2189, 320, -7, "ilshenar", 1788, 570, 71, "ilshenar" },

// Malas
	{ 433, 326, 4, "malas", 2172, 1144, -87, "malas" },
	{ 433, 372, -1, "malas", 480, 372, -1, "malas" },
	{ 433, 371, -1, "malas", 480, 371, -1, "malas" },
	{ 496, 49, 6, "malas", 2172, 1144, -87, "malas" },
	{ 479, 372, -1, "malas", 432, 372, -1, "malas" },
	{ 479, 371, -1, "malas", 432, 371, -1, "malas" },
	{ 2029, 1302, -90, "malas", 2029, 1305, -90, "malas" },
	{ 2029, 1304, -90, "malas", 2029, 1301, -90, "malas" },
	{ 2030, 1302, -90, "malas", 2030, 1305, -90, "malas" },
	{ 2030, 1304, -90, "malas", 2030, 1301, -90, "malas" },
	{ 2317, 1266, -110, "malas", 382, 131, 33, "malas" },
	{ 2317, 1267, -110, "malas", 382, 132, 33, "malas" },
	{ 2316, 1267, -110, "malas", 381, 132, 33, "malas" },
	{ 2315, 1267, -109, "malas", 380, 132, 33, "malas" },
	{ 2315, 1268, -110, "malas", 380, 133, 33, "malas" },
	{ 2315, 1269, -110, "malas", 380, 134, 33, "malas" },
	{ 2316, 1269, -110, "malas", 381, 134, 33, "malas" },
	{ 2317, 1268, -110, "malas", 382, 133, 33, "malas" },
	{ 2317, 1269, -110, "malas", 382, 134, 33, "malas" },
	{ 258, 783, 63, "tokuno", 6, 128, -1, "malas" },
	{ 4, 128, -1, "malas", 257, 783, 63, "tokuno" },
	{ 8, 128, -1, "malas", 260, 783, 63, "tokuno" },
	{ 259, 783, 63, "tokuno", 7, 128, -1, "malas" },
	{ 987, 194, 12, "tokuno", 66, 336, 1, "malas" },
	{ 987, 195, 12, "tokuno", 66, 337, 1, "malas" },
	{ 987, 196, 12, "tokuno", 66, 338, 1, "malas" },
	{ 987, 197, 12, "tokuno", 66, 339, 1, "malas" },
	{ 385, 810, -1, "malas", 404, 1164, 0, "malas" },
	{ 67, 351, -7, "malas", 62, 523, 1, "malas" },
	{ 67, 352, -7, "malas", 62, 524, 1, "malas" },
	{ 67, 353, -7, "malas", 62, 525, 1, "malas" },
	{ 67, 354, -7, "malas", 62, 526, 1, "malas" },
	{ 60, 523, 11, "malas", 64, 351, -5, "malas" },
	{ 60, 524, 11, "malas", 64, 352, -5, "malas" },
	{ 60, 525, 11, "malas", 64, 353, -5, "malas" },
	{ 60, 526, 11, "malas", 64, 354, -5, "malas" },
	{ 105, 555, -5, "malas", 74, 688, 1, "malas" },
	{ 105, 556, -5, "malas", 74, 689, 1, "malas" },
	{ 105, 557, -5, "malas", 74, 690, 1, "malas" },
	{ 105, 558, -5, "malas", 74, 691, 1, "malas" },
	{ 72, 688, 11, "malas", 100, 555, -2, "malas" },
	{ 72, 689, 11, "malas", 100, 555, -2, "malas" },
	{ 72, 690, 11, "malas", 100, 556, -2, "malas" },
	{ 72, 691, 11, "malas", 100, 556, -2, "malas" },
	{ 72, 692, 11, "malas", 100, 557, -2, "malas" },
	{ 72, 693, 11, "malas", 100, 557, -2, "malas" },
	{ 72, 694, 11, "malas", 100, 558, -2, "malas" },
	{ 172, 619, 1, "malas", 404, 1168, 0, "malas" },
	{ 403, 1169, 0, "malas", 174, 620, 1, "malas" },
	{ 404, 1169, 0, "malas", 174, 620, 1, "malas" },
	{ 405, 1169, 0, "malas", 174, 620, 1, "malas" },
	{ 412, 1123, 0, "malas", 411, 1116, 0, "malas" },
	{ 411, 1117, 0, "malas", 412, 1124, 0, "malas" },
	{ 421, 1092, 0, "malas", 412, 1085, 0, "malas" },
	{ 412, 1086, 0, "malas", 420, 1092, 0, "malas" },

// Tokuno
	{ 1364, 885, 16, "tokuno", 1375, 885, 21, "tokuno" },
	{ 1374, 885, 20, "tokuno", 1362, 885, 14, "tokuno" },
	{ 64, 336, 11, "malas", 985, 194, 21, "tokuno" },
	{ 64, 337, 11, "malas", 985, 195, 21, "tokuno" },
	{ 64, 338, 11, "malas", 985, 196, 21, "tokuno" },
	{ 64, 339, 11, "malas", 985, 197, 21, "tokuno" }
};

function CreateTeleporters()
  // if(GetGlobalProperty("SystemTeleportersCreated") == 1)
    // return;
  // endif
  foreach tele in teleporters
    var fromx := tele[1];
    var fromy := tele[2];
    var fromz := tele[3];
	var fromrealm := tele[4];
    var tox := tele[5];
    var toy := tele[6];
    var toz := tele[7];
	var torealm := tele[8];
    var teleporter := CreateItemAtLocation(fromx, fromy, fromz, "systemteleporter", 1, fromrealm);
    if(teleporter)
      // teleporter.graphic := 3948;
      SetObjProperty(teleporter, "DestX", tox);
      SetObjProperty(teleporter, "DestY", toy);
      SetObjProperty(teleporter, "DestZ", toz);
      SetObjProperty(teleporter, "DestRealm", torealm);
    endif
  endforeach
  // SetGlobalProperty("SystemTeleportersCreated", 1);
endfunction
