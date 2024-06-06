#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include <list>

class CFlightFlame;

// 飛行中の火炎放射クラス
class CFlightFlamethrower : public CTask
{
public:
	// コンストラクタ
	CFlightFlamethrower(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CFlightFlamethrower();

	// 炎を発射開始
	void Start();
	// 炎を発射停止
	void Stop();

	// 炎を発射しているかどうか
	bool IsThrowing() const;

	// 発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// 炎のエフェクトを作成
	void CreateFlame();

	// 炎の発射位置を取得
	CVector GetThrowPos() const;
	// 炎の発射方向を取得
	CVector GetThrowDir() const;

	// 発射した炎のエフェクトのリスト
	std::list<CFlightFlame*> mFlightFlames;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// 発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 炎を発射しているか
};