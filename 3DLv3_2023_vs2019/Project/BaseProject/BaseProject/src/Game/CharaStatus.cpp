#include "CharaStatus.h"

//プレイヤーのレベルごとにステータス
const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX] =
{
	//レベル　HP　 攻撃　 防御　 移動性   特殊攻撃　　　大きさ　　　 カメラの高さ
	{  1,     10,    5,    0,     1.0f,      2,          1.02f,           0.2f},
	{  2,     20,   10,    0,     1.0f,      2,          1.04f,           0.4f},
	{  3,     30,   15,    0,     1.0f,      2,          1.06f,           0.8f},
	{  4,     40,   20,    0,     1.0f,      2,          1.08f,           1.0f},
	{  5,     50,   25,    0,     1.0f,      4,          1.1f,           1.02f},
	{  6,     60,   30,    0,     1.0f,      4,          1.12f,          1.04f},
	{  7,     70,   35,    0,     1.0f,      4,          1.14f,          1.06f},
	{  8,     80,   40,    0,     1.0f,      4,          1.16f,          1.08f},
	{  9,     90,   45,    0,     1.0f,      4,          1.18f,           1.1f},
	{ 10,    100,   50,    0,     1.2f,      6,          1.2f,           1.12f},
	{ 11,    110,   55,    0,     1.2f,      6,          1.22f,          1.14f},
	{ 12,    120,   60,    0,     1.2f,      6,          1.24f,          1.16f},
	{ 13,    130,   65,    0,     1.2f,      6,          1.26f,          1.18f},
	{ 14,    140,   70,    0,     1.2f,      6,          1.28f,           1.2f},
	{ 15,    150,   75,    0,     1.2f,      8,          1.3f,           1.32f},
	{ 16,    160,   80,    0,     1.2f,      8,          1.32f,          1.34f},
	{ 17,    170,   85,    0,     1.2f,      8,          1.34f,          1.36f},
	{ 18,    180,   90,    0,     1.2f,      8,          1.36f,          1.38f},
	{ 19,    190,   95,    0,     1.2f,      8,          1.38f,           1.4f},
	{ 20,    200,  100,    0,     1.4f,     10,          1.4f,           1.42f},
	{ 21,    210,  105,    0,     1.4f,     10,          1.42f,          1.44f},
	{ 22,    220,  110,    0,     1.4f,     10,          1.44f,          1.46f},
	{ 23,    230,  115,    0,     1.4f,     10,          1.46f,          1.48f},
	{ 24,    240,  120,    0,     1.4f,     10,          1.48f,           1.5f},
	{ 25,    250,  125,    0,     1.4f,     12,          1.5f,           1.52f},
	{ 26,    260,  130,    0,     1.4f,     12,          1.52f,          1.54f},
	{ 27,    270,  135,    0,     1.4f,     12,          1.54f,          1.56f},
	{ 28,    280,  140,    0,     1.4f,     12,          1.56f,          1.58f},
	{ 29,    290,  145,    0,     1.4f,     12,          1.58f,           1.6f},
	{ 30,    300,  150,    0,     1.6f,     14,          1.6f,},
	{ 31,    310,  155,    0,     1.6f,     14,          1.62f,},
	{ 32,    320,  160,    0,     1.6f,     14,          1.64f,},
	{ 33,    330,  165,    0,     1.6f,     14,          1.66f,},
	{ 34,    340,  170,    0,     1.6f,     14,          1.68f,},
	{ 35,    350,  175,    0,     1.6f,     16,          1.7f,},
	{ 36,    360,  180,    0,     1.6f,     16,          1.72f,},
	{ 37,    370,  185,    0,     1.6f,     16,          1.74f,},
	{ 38,    380,  190,    0,     1.6f,     16,          1.76f,},
	{ 39,    390,  195,    0,     1.6f,     16,          1.78f,},
	{ 40,    400,  200,    0,     1.8f,     18,           1.8f,},
	{ 41,    410,  205,    0,     1.8f,     18,          1.82f,},
	{ 42,    420,  210,    0,     1.8f,     18,          1.84f,},
	{ 43,    430,  215,    0,     1.8f,     18,          1.86f,},
	{ 44,    440,  220,    0,     1.8f,     18,          1.88f,},
	{ 45,    450,  225,    0,     1.8f,     20,          1.9f,},
	{ 46,    460,  230,    0,     1.8f,     20,          1.92f,},
	{ 47,    470,  235,    0,     1.8f,     20,          1.94f,},
	{ 48,    480,  240,    0,     1.8f,     20,          1.96f,},
	{ 49,    490,  245,    0,     1.8f,     20,          1.98f,},
	{ 50,    500,  250,    0,     2.0f,     25,          2.0f,},
	{ 51,    510,  255,    0,     2.0f,     25,          2.02f,},
	{ 52,    520,  260,    0,     2.0f,     25,          2.04f,},
	{ 53,    530,  265,    0,     2.0f,     25,          2.06f,},
	{ 54,    540,  270,    0,     2.0f,     25,          2.08f,},
	{ 55,    550,  275,    0,     2.0f,     30,          2.1f,},
	{ 56,    560,  280,    0,     2.0f,     30,          2.12f,},
	{ 57,    570,  285,    0,     2.0f,     30,          2.14f,},
	{ 58,    580,  290,    0,     2.0f,     30,          2.16f,},
	{ 59,    590,  295,    0,     2.0f,     30,          2.18f,},
	{ 60,    600,  300,    0,     2.2f,     35,          2.2f,},
	{ 61,    610,  305,    0,     2.2f,     35,          2.22f,},
	{ 62,    620,  310,    0,     2.2f,     35,          2.24f,},
	{ 63,    630,  315,    0,     2.2f,     35,          2.26f,},
	{ 64,    640,  320,    0,     2.2f,     35,          2.28f,},
	{ 65,    650,  325,    0,     2.2f,     40,          2.3f,},
	{ 66,    660,  330,    0,     2.2f,     40,         2.32f,},
	{ 67,    670,  335,    0,     2.2f,     40,         2.34f,},
	{ 68,    680,  340,    0,     2.2f,     40,         2.36f,},
	{ 69,    690,  345,    0,     2.2f,     40,         2.38f,},
	{ 70,    700,  350,    0,     2.4f,     45,         2.4f,},
	{ 71,    710,  355,    0,     2.4f,     45,         2.42f,},
	{ 72,    720,  360,    0,     2.4f,     45,         2.44f,},
	{ 73,    730,  365,    0,     2.4f,     45,         2.46f,},
	{ 74,    740,  370,    0,     2.4f,     45,         2.48f,},
	{ 75,    750,  375,    0,     2.4f,     50,         2.5f,},
	{ 76,    760,  380,    0,     2.4f,     50,         2.52f,},
	{ 77,    770,  385,    0,     2.4f,     50,         2.54f,},
	{ 78,    780,  390,    0,     2.4f,     50,         2.56f,},
	{ 79,    790,  395,    0,     2.4f,     50,         2.58f,},
	{ 80,    800,  400,    0,     2.6f,     55,         2.6f,},
	{ 81,    810,  405,    0,     2.6f,     55,         2.62f,},
	{ 82,    820,  410,    0,     2.6f,     55,         2.64f,},
	{ 83,    830,  415,    0,     2.6f,     55,         2.66f,},
	{ 84,    840,  420,    0,     2.6f,     55,         2.68f,},
	{ 85,    850,  425,    0,     2.6f,     60,         2.7f,},
	{ 86,    860,  430,    0,     2.6f,     60,         2.72f,},
	{ 87,    870,  435,    0,     2.6f,     60,         2.74f,},
	{ 88,    880,  440,    0,     2.6f,     60,         2.76f,},
	{ 89,    890,  445,    0,     2.6f,     60,         2.78f,},
	{ 90,    900,  450,    0,     2.8f,     65,         2.8f,},
	{ 91,    910,  455,    0,     2.8f,     65,         2.82f,},
	{ 92,    920,  460,    0,     2.8f,     65,         2.84f,},
	{ 93,    930,  465,    0,     2.8f,     65,         2.86f,},
	{ 94,    940,  470,    0,     2.8f,     65,         2.88f,},
	{ 95,    950,  475,    0,     2.8f,     70,         2.9f},
	{ 96,    960,  480,    0,     2.8f,     70,         2.92f,},
	{ 97,    970,  485,    0,     2.8f,     70,         2.94f,},
	{ 98,    980,  490,    0,     2.8f,     70,         2.96f,},
	{ 99,    990,  495,    0,     2.8f,     70,         2.98f,},
	{100,   1000,  500,    0,     3.0f,     80,         3.0f,},
};

//エネミーのレベルごとにステータス
const CharaStatus ENEMY_STATUS[ENEMY__LEVEL_MAX] =
{
	//レベル　HP　 攻撃　 防御　 移動性   特殊攻撃　　　大きさ　　　カメラの高さ
	{  1,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  2,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  3,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  4,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  5,     60,    5,    0,     1.0f,      0,          1.0f,         0.0f},
};

//エネミー2のレベルごとにステータス
const CharaStatus ENEMY2_STATUS[ENEMY__LEVEL_MAX] =
{
	//レベル　HP　 攻撃　 防御　 移動性   特殊攻撃　　　大きさ　　　カメラの高さ
	{  1,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  2,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  3,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  4,     30,    2,    0,     1.0f,      0,          1.0f,         0.0f},
	{  5,     60,    5,    0,     1.0f,      0,          1.0f,         0.0f},
};