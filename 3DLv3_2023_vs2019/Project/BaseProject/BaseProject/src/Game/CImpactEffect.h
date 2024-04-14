#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CImpact;

// 衝撃エフェクトクラス
class CImpactEffect : public CTask
{
public:
	// コンストラクタ
	CImpactEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CImpactEffect();

	// 衝撃を開始
	void Start();
	// 衝撃を停止
	void Stop();

	// 衝撃をしているかどうか
	bool IsThrowing() const;

	// 衝撃時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 衝撃時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// 衝撃を作成
	void CreateImpact();

	// 衝撃の位置を取得
	CVector GetThrowPos() const;
	// 衝撃の方向を取得
	CVector GetThrowDir() const;

	// 発射した衝撃のエフェクトのリスト
	std::list<CImpact*> mImpact;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 衝撃時のオフセット位置
	CMatrix mThrowOffsetRot;	// 衝撃時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 衝撃をしているか
};