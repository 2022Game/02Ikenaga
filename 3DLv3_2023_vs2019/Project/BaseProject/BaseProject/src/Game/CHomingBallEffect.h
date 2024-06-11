#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CHomingBall;

// ホーミングボールのエフェクトクラス
class CHomingBallEffect : public CTask
{
public:
	// コンストラクタ
	CHomingBallEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CHomingBallEffect();

	// ホーミングボールの発射を開始
	void Start();
	// ホーミングボールの発射を停止
	void Stop();

	// ホーミングボールを発射をしているかどうか
	bool IsThrowing() const;

	// ホーミングボールの発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// ホーミングボールの発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// ホーミングボールを作成
	void CreateLightningBall();

	// ホーミングボールの発射の位置を取得
	CVector GetThrowPos() const;
	// ホーミングボールの発射の方向を取得
	CVector GetThrowDir() const;

	// 発射したホーミングボールのエフェクトのリスト
	std::list<CHomingBall*> mHomingBall;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// ホーミングボール発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// ホーミングボール発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// ホーミングボールを発射をしているか
};