#include "CharaStatus.h"
#include "Maths.h"

//プレイヤーのレベルごとにステータス
const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX] =
{
	// レベル 経験値  HP   攻撃　  防御　移動性  特殊攻撃　大きさ　 カメラの高さ
	{    1,     1,    10,   10,     0,    1.2f,     2,       1.0f,     0.0f},
	{    2,     2,    20,   20,     2,    1.2f,     2,      1.02f,     0.3f},
	{    3,     3,    30,   30,     4,    1.2f,     2,      1.04f,     0.6f},
	{    4,     4,    40,   40,     6,    1.2f,     2,      1.06f,     0.9f},
	{    5,     5,    50,   50,     8,    1.2f,     4,      1.08f,     1.2f},
	{    6,     6,    60,   60,    10,    1.2f,     4,       1.1f,     1.5f},
	{    7,     7,    70,   70,    12,    1.2f,     4,      1.12f,     1.8f},
	{    8,     8,    80,   80,    14,    1.2f,     4,      1.14f,     2.1f},
	{    9,     9,    90,   90,    16,    1.2f,     4,      1.16f,     2.4f},
	{   10,    10,   100,  100,    18,    1.4f,     6,      1.18f,     2.7f},
	{   11,    11,   110,  110,    20,    1.4f,     6,       1.2f,     3.0f},
	{   12,    12,   120,  120,    22,    1.4f,     6,      1.22f,     3.3f},
	{   13,    13,   130,  130,    24,    1.4f,     6,      1.24f,     3.6f},
	{   14,    14,   140,  140,    26,    1.4f,     6,      1.26f,     3.9f},
	{   15,    15,   150,  150,    28,    1.4f,     8,      1.28f,     4.2f},
	{   16,    16,   160,  160,    30,    1.4f,     8,       1.3f,     4.5f},
	{   17,    17,   170,  170,    32,    1.4f,     8,      1.32f,     4.8f},
	{   18,    18,   180,  180,    34,    1.4f,     8,      1.34f,     5.1f},
	{   19,    19,   190,  190,    36,    1.4f,     8,      1.36f,     5.4f},
	{   20,    20,   200,  200,    38,    1.6f,    10,      1.38f,     5.7f},
	{   21,    21,   210,  210,    40,    1.6f,    10,       1.4f,     6.0f},
	{   22,    22,   220,  220,    42,    1.6f,    10,      1.42f,     6.3f},
	{   23,    23,   230,  230,    44,    1.6f,    10,      1.44f,     6.6f},
	{   24,    24,   240,  240,    46,    1.6f,    10,      1.46f,     6.9f},
	{   25,    25,   250,  250,    48,    1.6f,    12,      1.48f,     7.2f},
	{   26,    26,   260,  260,    50,    1.6f,    12,       1.5f,     7.5f},
	{   27,    27,   270,  270,    52,    1.6f,    12,      1.52f,     7.8f},
	{   28,    28,   280,  280,    54,    1.6f,    12,      1.54f,     8.1f},
	{   29,    29,   290,  290,    56,    1.6f,    12,      1.56f,     8.4f},
	{   30,    30,   300,  300,    58,    1.8f,    14,      1.58f,     8.7f},
	{   31,    31,   310,  310,    60,    1.8f,    14,       1.6f,     9.0f},
	{   32,    32,   320,  320,    62,    1.8f,    14,      1.62f,     9.3f},
	{   33,    33,   330,  330,    64,    1.8f,    14,      1.64f,     9.6f},
	{   34,    34,   340,  340,    66,    1.8f,    14,      1.66f,     9.9f},
	{   35,    35,   350,  350,    68,    1.8f,    16,      1.68f,    10.2f},
	{   36,    36,   360,  360,    70,    1.8f,    16,       1.7f,    10.5f},
	{   37,    37,   370,  370,    72,    1.8f,    16,      1.72f,    10.8f},
	{   38,    38,   380,  380,    74,    1.8f,    16,      1.74f,    11.1f},
	{   39,    39,   390,  390,    76,    1.8f,    16,      1.76f,    11.4f},
	{   40,    40,   400,  400,    78,    2.0f,    18,      1.78f,    11.7f},
	{   41,    41,   410,  410,    80,    2.0f,    18,       1.8f,    12.0f},
	{   42,    42,   420,  420,    82,    2.0f,    18,      1.82f,    12.3f},
	{   43,    43,   430,  430,    84,    2.0f,    18,      1.84f,    12.6f},
	{   44,    44,   440,  440,    86,    2.0f,    18,      1.86f,    12.9f},
	{   45,    45,   450,  450,    88,    2.0f,    20,      1.88f,    13.2f},
	{   46,    46,   460,  460,    90,    2.0f,    20,       1.9f,    13.5f},
	{   47,    47,   470,  470,    92,    2.0f,    20,      1.92f,    13.8f},
	{   48,    48,   480,  480,    94,    2.0f,    20,      1.94f,    14.1f},
	{   49,    49,   490,  490,    96,    2.0f,    20,      1.96f,    14.4f},
	{   50,    50,   500,  500,    98,    2.2f,    25,      1.98f,    14.7f},
	{   51,    51,   510,  510,   100,    2.2f,    25,       2.0f,    15.0f},
	{   52,    52,   520,  520,   102,    2.2f,    25,      2.02f,    15.3f},
	{   53,    53,   530,  530,   104,    2.2f,    25,      2.04f,    15.6f},
	{   54,    54,   540,  540,   106,    2.2f,    25,      2.06f,    15.9f},
	{   55,    55,   550,  550,   108,    2.2f,    30,      2.08f,    16.2f},
	{   56,    56,   560,  560,   110,    2.2f,    30,       2.1f,    16.5f},
	{   57,    57,   570,  570,   112,    2.2f,    30,      2.12f,    16.8f},
	{   58,    58,   580,  580,   114,    2.2f,    30,      2.14f,    17.1f},
	{   59,    59,   590,  590,   116,    2.2f,    30,      2.16f,    17.4f},
	{   60,    60,   600,  600,   118,    2.4f,    35,      2.18f,    17.7f},
	{   61,    61,   610,  610,   120,    2.4f,    35,       2.2f,    18.0f},
	{   62,    62,   620,  620,   122,    2.4f,    35,      2.22f,    18.3f},
	{   63,    63,   630,  630,   124,    2.4f,    35,      2.24f,    18.6f},
	{   64,    64,   640,  640,   126,    2.4f,    35,      2.26f,    18.9f},
	{   65,    65,   650,  650,   128,    2.4f,    40,      2.28f,    19.2f},
	{   66,    66,   660,  660,   130,    2.4f,    40,       2.3f,    19.5f},
	{   67,    67,   670,  670,   132,    2.4f,    40,      2.32f,    19.8f},
	{   68,    68,   680,  680,   134,    2.4f,    40,      2.34f,    20.1f},
	{   69,    69,   690,  690,   136,    2.4f,    40,      2.36f,    20.4f},
	{   70,    70,   700,  700,   138,    2.6f,    45,      2.38f,    20.7f},
	{   71,    71,   710,  710,   140,    2.6f,    45,       2.4f,    21.0f},
	{   72,    72,   720,  720,   142,    2.6f,    45,      2.42f,    21.3f},
	{   73,    73,   730,  730,   144,    2.6f,    45,      2.44f,    21.6f},
	{   74,    74,   740,  740,   146,    2.6f,    45,      2.46f,    21.9f},
	{   75,    75,   750,  750,   148,    2.6f,    50,      2.48f,    22.2f},
	{   76,    76,   760,  760,   150,    2.6f,    50,       2.5f,    22.5f},
	{   77,    77,   770,  770,   152,    2.6f,    50,      2.52f,    22.8f},
	{   78,    78,   780,  780,   154,    2.6f,    50,      2.54f,    23.1f},
	{   79,    79,   790,  790,   156,    2.6f,    50,      2.56f,    23.4f},
	{   80,    80,   800,  800,   158,    2.8f,    55,      2.58f,    23.7f},
	{   81,    81,   810,  810,   160,    2.8f,    55,       2.6f,    24.0f},
	{   82,    82,   820,  820,   162,    2.8f,    55,      2.62f,    24.3f},
	{   83,    83,   830,  830,   164,    2.8f,    55,      2.64f,    24.6f},
	{   84,    84,   840,  840,   166,    2.8f,    55,      2.66f,    24.9f},
	{   85,    85,   850,  850,   168,    2.8f,    60,      2.68f,    25.2f},
	{   86,    86,   860,  860,   170,    2.8f,    60,       2.7f,    25.5f},
	{   87,    87,   870,  870,   172,    2.8f,    60,      2.72f,    25.8f},
	{   88,    88,   880,  880,   174,    2.8f,    60,      2.74f,    26.1f},
	{   89,    89,   890,  890,   176,    2.8f,    60,      2.76f,    26.4f},
	{   90,    90,   900,  900,   178,    3.0f,    65,      2.78f,    26.7f},
	{   91,    91,   910,  910,   180,    3.0f,    65,       2.8f,    27.0f},
	{   92,    92,   920,  920,   182,    3.0f,    65,      2.82f,    27.4f},
	{   93,    93,   930,  930,   184,    3.0f,    65,      2.84f,    27.8f},
	{   94,    94,   940,  940,   186,    3.0f,    65,      2.86f,    28.2f},
	{   95,    95,   950,  950,   188,    3.0f,    70,      2.88f,    28.6f},
	{   96,    96,   960,  960,   190,    3.0f,    70,       2.9f,    29.0f},
	{   97,    97,   970,  970,   192,    3.0f,    70,      2.92f,    29.4f},
	{   98,    98,   980,  980,   194,    3.0f,    70,      2.94f,    29.8f},
	{   99,    99,   990,  990,   196,    3.0f,    70,      2.96f,    29.9f},
	{  100,   100,  1000, 1000,   200,    3.0f,    80,      2.98f,    30.0f},
};

// スライム(エネミー)のレ;ベルごとにステータス
const CharaStatus ENEMY_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　 経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,      2,      30,    3,     0,     1.0f,      0,      1.0f,    0.0f},
	{    2,      2,      30,    3,     0,     1.0f,      0,      1.0f,    0.0f},
	{    3,      2,      30,    3,     0,     1.0f,      0,      1.0f,    0.0f},
	{    4,      2,      30,    3,     0,     1.0f,      0,      1.0f,    0.0f},
	{    5,      6,     125,   24,     0,     1.0f,      0,      1.0f,    0.0f},
};

// マッシュルーム(エネミー)のレベルごとにステータス
const CharaStatus ENEMY2_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,      6,    150,    23,     0,     1.0f,      0,      1.0f,    0.0f},
	{    2,      6,    150,    23,     0,     1.0f,      0,      1.0f,    0.0f},
	{    3,      6,    150,    23,     0,     1.0f,      0,      1.0f,    0.0f},
	{    4,      6,    150,    23,     0,     1.0f,      0,      1.0f,    0.0f},
	{    5,     10,    300,    43,     0,     1.5f,      0,      1.0f,    0.0f},
};

// 亀(エネミー)のレベルごとにステータス
const CharaStatus ENEMY3_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,      9,    330,    53,    90,     1.0f,      0,      1.0f,    0.0f},
	{    2,      9,    330,    53,    90,     1.0f,      0,      1.0f,    0.0f},
	{    3,      9,    330,    53,    90,     1.0f,      0,      1.0f,    0.0f},
	{    4,      9,    330,    53,    90,     1.0f,      0,      1.0f,    0.0f},
	{    5,     13,    500,    93,   120,     1.5f,      0,      1.0f,    0.0f},
};

// エイ(エネミー)のレベルごとにステータス
const CharaStatus ENEMY4_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     11,    600,    83,    50,     1.0f,      0,      1.0f,    0.0f},
	{    2,     11,    600,    83,    50,     1.0f,      0,      1.0f,    0.0f},
	{    3,     11,    600,    83,    50,     1.0f,      0,      1.0f,    0.0f},
	{    4,     11,    600,    83,    50,     1.0f,      0,      1.0f,    0.0f},
	{    5,     15,    800,   113,   100,     1.0f,      0,      1.0f,    0.0f},
};

// 蜂(エネミー)のレベルごとにステータス
const CharaStatus ENEMY5_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     21,   1000,   103,    40,     1.0f,      0,      1.0f,    0.0f},
	{    2,     21,   1000,   103,    40,     1.0f,      0,      1.0f,    0.0f},
	{    3,     21,   1000,   103,    40,     1.0f,      0,      1.0f,    0.0f},
	{    4,     21,   1000,   103,    40,     1.0f,      0,      1.0f,    0.0f},
	{    5,     30,   1300,   153,    80,     1.5f,      0,      1.0f,    0.0f},
};

// サボテン(エネミー)のレベルごとにステータス
const CharaStatus ENEMY6_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値    HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     31,    1700,   283,   210,     1.0f,      0,      1.0f,    0.0f},
	{    2,     31,    1700,   283,   210,     1.0f,      0,      1.0f,    0.0f},
	{    3,     31,    1700,   283,   210,     1.0f,      0,      1.0f,    0.0f},
	{    4,     31,    1700,   283,   210,     1.0f,      0,      1.0f,    0.0f},
	{    5,     40,    2100,   343,   300,     1.5f,      0,      1.0f,    0.0f},
};

// チェストモンスター(エネミー)のレベルごとにステータス
const CharaStatus ENEMY7_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値    HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     41,    2300,   343,   260,     1.0f,      0,      1.0f,    0.0f},
	{    2,     41,    2300,   343,   260,     1.0f,      0,      1.0f,    0.0f},
	{    3,     41,    2300,   343,   260,     1.0f,      0,      1.0f,    0.0f},
	{    4,     41,    2300,   343,   260,     1.0f,      0,      1.0f,    0.0f},
	{    5,     50,    2600,   403,   360,     1.5f,      0,      1.0f,    0.0f},
};

// 球体のモンスター(エネミー)のレベルごとにステータス
const CharaStatus ENEMY8_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     51,   2800,   363,    90,     1.0f,      0,      1.0f,    0.0f},
	{    2,     51,   2800,   363,    90,     1.0f,      0,      1.0f,    0.0f},
	{    3,     51,   2800,   363,    90,     1.0f,      0,      1.0f,    0.0f},
	{    4,     51,   2800,   363,    90,     1.0f,      0,      1.0f,    0.0f},
	{    5,     60,   3000,   443,   190,     1.5f,      0,      1.0f,    0.0f},
};

// ボクサー(エネミー)のレベルごとにステータス
const CharaStatus ENEMY9_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　経験値    HP　  攻撃　  防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     61,    3300,   483,    400,     1.0f,      0,      1.0f,    0.0f},
	{    2,     61,    3300,   483,    400,     1.0f,      0,      1.0f,    0.0f},
	{    3,     61,    3300,   483,    400,     1.0f,      0,      1.0f,    0.0f},
	{    4,     61,    3300,   483,    400,     1.0f,      0,      1.0f,    0.0f},
	{    5,     70,    4300,   523,    500,     1.5f,      0,      1.0f,    0.0f},
};

// ドラゴン(エネミー)のレベルごとにステータス
const CharaStatus ENEMY10_STATUS[ENEMY__LEVEL_MAX] =
{
	// レベル　 経験値   HP　  攻撃　 防御　 移動性   特殊攻撃　大きさ　カメラの高さ
	{    1,     100,   3300,   603,    90,     1.0f,      0,      1.0f,    0.0f},
	{    2,     100,   3300,   603,    90,     1.0f,      0,      1.0f,    0.0f},
	{    3,     100,   3300,   603,    90,     1.0f,      0,      1.0f,    0.0f},
	{    4,     100,   3300,   603,    90,     1.0f,      0,      1.0f,    0.0f},
	{    5,     200,   3300,   703,   190,     1.5f,      0,      1.0f,    0.0f},
};