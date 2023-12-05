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
	CExp(int exp);
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
	int mAddExp;   // 取得時に獲得する経験値量
	CColliderSphere* mpColliderSphere;
	CVector mMoveSpeed;  //移動速度

	// 状態
	enum class EState
	{
		Drop,       // 経験値を落とした時の処理
		Absorb,     // 経験値が吸収される時の処理
	};
	EState mState;  // 現在の状態
	float mElapsedTime;  // 経過時間計測用

};