#include "CharaStatus.h"

//プレイヤーのレベルごとにステータス
const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX] =
{
	//レベル　HP　 攻撃　 防御　 移動性   特殊攻撃 
	{  1,     10,    5,    0,     1.0f,      2},
	{  2,     20,   10,    0,     1.0f,      3},
	{  3,     30,   15,    0,     1.0f,      4},
	{  4,     40,   20,    0,     1.0f,      5},
	{  5,     50,   25,    0,     1.0f,      6},
	{  6,     60,   30,    0,     1.0f,      7},
	{  7,     70,   35,    0,     1.0f,      8},
	{  8,     80,   40,    0,     1.0f,      9},
	{  9,     90,   45,    0,     1.0f,     10},
	{ 10,    100,   50,    0,     1.2f,     11},
	{ 11,    110,   55,    0,     1.2f,     12},
	{ 12,    120,   60,    0,     1.2f,     13},
	{ 13,    130,   65,    0,     1.2f},
	{ 14,    140,   70,    0,     1.2f},
	{ 15,    150,   75,    0,     1.2f},
	{ 16,    160,   80,    0,     1.2f},
	{ 17,    170,   85,    0,     1.2f},
	{ 18,    180,   90,    0,     1.2f},
	{ 19,    190,   95,    0,     1.2f},
	{ 20,    200,  100,    0,     1.4f},
	{ 21,    210,  105,    0,     1.4f},
	{ 22,    220,  110,    0,     1.4f},
	{ 23,    230,  115,    0,     1.4f},
	{ 24,    240,  120,    0,     1.4f},
	{ 25,    250,  125,    0,     1.4f},
	{ 26,    260,  130,    0,     1.4f},
	{ 27,    270,  135,    0,     1.4f},
	{ 28,    280,  140,    0,     1.4f},
	{ 29,    290,  145,    0,     1.4f},
	{ 30,    300,  150,    0,     1.6f},
	{ 31,    310,  155,    0,     1.6f},
	{ 32,    320,  160,    0,     1.6f},
	{ 33,    330,  165,    0,     1.6f},
	{ 34,    340,  170,    0,     1.6f},
	{ 35,    350,  175,    0,     1.6f},
	{ 36,    360,  180,    0,     1.6f},
	{ 37,    370,  185,    0,     1.6f},
	{ 38,    380,  190,    0,     1.6f},
	{ 39,    390,  195,    0,     1.6f},
	{ 40,    400,  200,    0,     1.8f},
	{ 41,    410,  205,    0,     1.8f},
	{ 42,    420,  210,    0,     1.8f},
	{ 43,    430,  215,    0,     1.8f},
	{ 44,    440,  220,    0,     1.8f},
	{ 45,    450,  225,    0,     1.8f},
	{ 46,    460,  230,    0,     1.8f},
	{ 47,    470,  235,    0,     1.8f},
	{ 48,    480,  240,    0,     1.8f},
	{ 49,    490,  245,    0,     1.8f},
	{ 50,    500,  250,    0,     2.0f},
	{ 51,    510,  255,    0,     2.0f},
	{ 52,    520,  260,    0,     2.0f},
	{ 53,    530,  265,    0,     2.0f},
	{ 54,    540,  270,    0,     2.0f},
	{ 55,    550,  275,    0,     2.0f},
	{ 56,    560,  280,    0,     2.0f},
	{ 57,    570,  285,    0,     2.0f},
	{ 58,    580,  290,    0,     2.0f},
	{ 59,    590,  295,    0,     2.0f},
	{ 60,    600,  300,    0,     2.2f},
	{ 61,    610,  305,    0,     2.2f},
	{ 62,    620,  310,    0,     2.2f},
	{ 63,    630,  315,    0,     2.2f},
	{ 64,    640,  320,    0,     2.2f},
	{ 65,    650,  325,    0,     2.2f},
	{ 66,    660,  330,    0,     2.2f},
	{ 67,    670,  335,    0,     2.2f},
	{ 68,    680,  340,    0,     2.2f},
	{ 69,    690,  345,    0,     2.2f},
	{ 70,    700,  350,    0,     2.4f},
	{ 71,    710,  355,    0,     2.4f},
	{ 72,    720,  360,    0,     2.4f},
	{ 73,    730,  365,    0,     2.4f},
	{ 74,    740,  370,    0,     2.4f},
	{ 75,    750,  375,    0,     2.4f},
	{ 76,    760,  380,    0,     2.4f},
	{ 77,    770,  385,    0,     2.4f},
	{ 78,    780,  390,    0,     2.4f},
	{ 79,    790,  395,    0,     2.4f},
	{ 80,    800,  400,    0,     2.6f},
	{ 81,    810,  405,    0,     2.6f},
	{ 82,    820,  410,    0,     2.6f},
	{ 83,    830,  415,    0,     2.6f},
	{ 84,    840,  420,    0,     2.6f},
	{ 85,    850,  425,    0,     2.6f},
	{ 86,    860,  430,    0,     2.6f},
	{ 87,    870,  435,    0,     2.6f},
	{ 88,    880,  440,    0,     2.6f},
	{ 89,    890,  445,    0,     2.6f},
	{ 90,    900,  450,    0,     2.8f},
	{ 91,    910,  455,    0,     2.8f},
	{ 92,    920,  460,    0,     2.8f},
	{ 93,    930,  465,    0,     2.8f},
	{ 94,    940,  470,    0,     2.8f},
	{ 95,    950,  475,    0,     2.8f},
	{ 96,    960,  480,    0,     2.8f},
	{ 97,    970,  485,    0,     2.8f},
	{ 98,    980,  490,    0,     2.8f},
	{ 99,    990,  495,    0,     2.8f},
	{100,   1000,  500,    0,     3.0f},
};
