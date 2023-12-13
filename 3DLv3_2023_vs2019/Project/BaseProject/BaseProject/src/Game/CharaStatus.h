#pragma once

//キャラのステータス
struct CharaStatus
{
    int level;         // レベル
	int exp;           // 経験値
	int hp;            // HP
	int power;         // 攻撃力
	int defense;       // 防御力
	float mobility;    // 移動性
	int SpecialAttack; // 特殊攻撃
	float volume;      // 大きさ
	float cameraHeight; // カメラの高さ

	// ステータスのリセット
	void Reset()
	{
		level = 1;
		exp = 0;
		power = 5;
		defense = 0;
		mobility = 1.0f;
		SpecialAttack = 2;
		volume = 1.02f;
		cameraHeight = 0.0f;
	}
}; 

// プレイヤーの最大レベル
#define PLAYER_LEVEL_MAX 100
// プレイヤーのレベルごとにステータスのテーブル
extern const CharaStatus PLAYER_STATUS[PLAYER_LEVEL_MAX];

// 敵の最大レベル
#define ENEMY__LEVEL_MAX 5
// 敵のレベルごとにステータスのテーブル
extern const CharaStatus ENEMY_STATUS[ENEMY__LEVEL_MAX];
extern const CharaStatus ENEMY2_STATUS[ENEMY__LEVEL_MAX];
