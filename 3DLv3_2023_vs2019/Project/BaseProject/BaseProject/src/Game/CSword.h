#pragma once
#include "CObjectBase.h"
#include "CColliderLine.h"
#include "CModel.h"
#include "CWeapon.h"

/*
 剣クラス
 武器クラス継承
*/
class CSword : public CWeapon
{
public:
	// コンストラク
	CSword();
	// デストラクタ
	~CSword();

	// 更新
	void Update();
	// 描画
	void Render();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	virtual void Collision(CCollider* self, CCollider* other, const CHitInfo& hit);

	// 武器の行列取得
	CMatrix Matrix() const override;

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;
	// 攻撃力アップ
	void PowerUp();

private:
	// モデルデータ読み込み
	CModel* mpSword;
	// 攻撃力アップ
	bool mPowerUp;
	// 経過時間(攻撃力アップ用)
	float mElapsedPowerUpTime;
	// 攻撃判定用のコライダー
	CColliderLine* mpAttackCol;
};

