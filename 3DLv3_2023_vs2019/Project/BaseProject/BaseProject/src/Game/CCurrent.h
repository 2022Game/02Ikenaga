#pragma once
#include "CWeapon.h"
#include "CTexture.h"
#include <functional>

class CLineEffect;
class CColliderLine;

class CCurrent : public CWeapon
{
public:
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">エフェクトを発生させたオブジェクト</param>
	/// <param name="pos">発生位置</param>
	/// <param name="dir">方向</param>
	CCurrent(CObjectBase* owner, const CVector& pos, const CVector& dir);

	//デストラクタ
	~CCurrent();

	// ヒットした時のコールバック関数を設定
	void SetCollisionCallback(std::function<void()>callback);

	//攻撃開始
	void AttackStart()override;

	//攻撃終了
	void AttackEnd()override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

private:

	float mElapsedTime;  // 経過時間

	CLineEffect* mpLineEffect;
	TexAnimData mEffectAnimData;
	CColliderLine* mpAttackCol;          // 攻撃コライダー
	std::function<void()> mColCallback;  // コールバック関数
};