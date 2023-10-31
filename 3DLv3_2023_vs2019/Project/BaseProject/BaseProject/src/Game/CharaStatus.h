#pragma once

//キャラクターステータス
struct CharaStatus
{
	int level;         //レベル
	int hp;            //HP
	int power;         //攻撃力
	int defense;       //防御力
	float mobility;    //移動性
	int SpecialAttack; //特殊攻撃
	float volume;      //大きさ
}; 

//プレイヤーの最大レベル
#define PLAYER_LEVEL_MAX 100

//プレイヤーのレベルごとにステータスのテーブル
extern const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX];