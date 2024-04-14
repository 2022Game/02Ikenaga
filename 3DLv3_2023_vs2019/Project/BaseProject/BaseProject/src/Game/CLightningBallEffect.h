#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CLightningBall;

// 雷球エフェクトクラス
class CLightningBallEffect : public CTask
{
public:
	// コンストラクタ
	CLightningBallEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CLightningBallEffect();

	// 雷球発射を開始
	void Start();
	// 雷球発射を停止
	void Stop();

	// 雷球発射をしているかどうか
	bool IsThrowing() const;

	// 雷球発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 雷球発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// 雷球を作成
	void CreateLightningBall();

	// 雷球発射の位置を取得
	CVector GetThrowPos() const;
	// 雷球発射の方向を取得
	CVector GetThrowDir() const;

	// 発射した雷のエフェクトのリスト
	std::list<CLightningBall*> mLightningBall;
	static CLightningBallEffect* spInstance;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 雷球発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// 雷球発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 雷球発射をしているか
};