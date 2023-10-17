#pragma once
#include <initializer_list>

// オブジェクト識別用のタグ
enum class ETag
{
	eNone = -1,

	eField,	// フィールドを構成するオブジェクト
	eBall,  //経験値
	eRideableObject,	// 乗ることができるオブジェクト
	eItem,	// アイテムの攻撃力アップ
	eItem2, // アイテムの回復
	eItem3, // アイテムの移動速度アップ

	ePlayer,// プレイヤー
	eEnemy,	// エネミー

	eCamera,// カメラ

	Num
};
// オブジェクト識別用のタグの初期化リスト
using Tags = std::initializer_list<ETag>;
