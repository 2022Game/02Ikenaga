#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CColliderSphere.h"

/*
防御力アップのポーションクラス
オブジェクトベース継承
*/
class CPortionBlue : public CObjectBase
{
public:
	// コンストラク
	CPortionBlue();
	// デストラクタ
	~CPortionBlue();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	CModel* mpPortionBlue;  // モデルデータ読み込み
	CColliderSphere* mpColliderSphere;  // 球(衝突処理用)
	bool mDefenseUp;     // 防御力アップ(ポーション効果)
};
