#pragma once
#include "CObjectBase.h"
#include "CColliderSphere.h"
#include "CBillBoardImage.h"

/*
経験値クラス
オブジェクトベース継承
*/
class CExp : public CBillBoardImage
{
public:
	CExp();
	~CExp();

	// 初期設定
	void Setup(const CVector& pos, const CVector& dir);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	void Update() override;

private:
	CColliderSphere* mpColliderSphere;
	CVector mMoveSpeed;  //移動速度
};