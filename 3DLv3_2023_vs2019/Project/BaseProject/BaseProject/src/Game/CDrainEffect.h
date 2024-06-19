#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CDrain;

// ドレインのエフェクトクラス
class CDrainEffect : public CTask
{
public:
	// コンストラクタ
	CDrainEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CDrainEffect();

	// ドレインの発射を開始
	void Start();
	// ドレインの発射を停止
	void Stop();

	// ドレインを発射をしているかどうか
	bool IsThrowing() const;

	// ドレインの発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// ドレインの発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// ドレインを作成
	void CreateDrain();

	// ドレインの発射の位置を取得
	CVector GetThrowPos() const;
	// ドレインの発射の方向を取得
	CVector GetThrowDir() const;

	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// ドレイン発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// ドレイン発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// ドレインを発射をしているか
};