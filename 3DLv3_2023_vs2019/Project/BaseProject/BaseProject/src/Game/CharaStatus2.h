#pragma once

//エネミー(スライム)のステータス
struct CharaStatus2
{
	int level;         //レベル
	int hp;            //HP
	int power;         //攻撃力
};

//エネミー(スライム)の最大レベル
#define ENEMY__LEVEL_MAX 5

//エネミーのレベルごとにステータスのテーブル
extern const CharaStatus2 ENEMY_STATUS[ENEMY__LEVEL_MAX];