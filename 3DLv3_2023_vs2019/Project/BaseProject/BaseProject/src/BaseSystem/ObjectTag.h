#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	          // フィールドを構成するオブジェクト
	eExp,             // 経験値
	eRideableObject,  // 乗ることができるオブジェクト
	ePortionRed,	  // アイテムの攻撃力アップ
	ePortionGreen,    // アイテムの回復
	ePortionBlue,     // アイテムの防御力アップ
	eWeapon,          // 武器

	ePlayer,          // プレイヤー
	eEnemy,           // エネミー
	eBullet,          // 弾丸
	eFlame,	          // 炎
	eSlash,	          // スラッシュ
	eWave,            // 波動
	eNeedle,          // 針
	eLightningBall,   // 雷球
	eElectricShock,   // 電撃
	eImpact,          // 衝撃
	eHomingBall,      // ホーミングボール

	eCamera,          // カメラ

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
