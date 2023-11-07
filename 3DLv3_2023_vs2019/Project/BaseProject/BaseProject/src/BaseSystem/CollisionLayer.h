#pragma once
#include <initializer_list>

// 衝突判定レイヤー
enum class ELayer
{
	eNone = -1,
	eField,
	ePlayer,

	eDamageCol,  //ダメージを受ける時のコライダー
	eAttackCol,  //攻撃を与える時のコライダー
};
// 衝突判定レイヤーの初期化リスト
using Layers = std::initializer_list<ELayer>;
