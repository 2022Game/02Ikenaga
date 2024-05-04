#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include "CColliderSphere.h"

class CWave;

// 波動エフェクトクラス
class CWaveEffect : public CTask
{
public:
	// コンストラクタ
	CWaveEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CWaveEffect();

	// 波動を開始
	void Start();
	// 波動を停止
	void Stop();

	// 波動をしているかどうか
	bool IsThrowing() const;

	// 波動時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 波動時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// 波動を作成
	void CreateWave();

	// 波動の位置を取得
	CVector GetThrowPos() const;
	// 波動の方向を取得
	CVector GetThrowDir() const;

	static CWaveEffect* spInstance;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 波動時のオフセット位置
	CMatrix mThrowOffsetRot;	// 波動時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 波動をしているか
};