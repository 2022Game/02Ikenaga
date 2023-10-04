#pragma once
#include "CCollider.h"

/// <summary>
/// 線分コライダー
/// </summary>
class CColliderLine : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">線分の始点</param>
	/// <param name="v1">線分の終点</param>
	CColliderLine(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1);

	/// <summary>
	/// 線分コライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">線分の始点</param>
	/// <param name="v1">線分の終点</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1);

	/// <summary>
	/// 頂点取得
	/// </summary>
	/// <param name="index">インデックス値</param>
	/// <returns>頂点座標</returns>
	CVector V(int index) const;
	float Radius() const;

	// コライダー描画
	void Render();

private:
	CVector mV[2];	// 線分の頂点の配列
	float mRadius;
};
