#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include <list>

class CBreath;

// アイスブレスクラス
class CIceBreath : public CTask
{
public:

	// コンストラクタ
	CIceBreath(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);

	// デストラクタ
	~CIceBreath();

	// 発射開始
	void Start();

	// 発射停止
	void Stop();

	// アイスブレスを発射しているかどうか
	bool IsThrowing() const;

	// 発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);

	// 発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:

	// ブレスのエフェクトを作成
	void CreateBreath();

	// ブレスの発射位置を取得
	CVector GetThrowPos() const;
	// ブレスの発射方向を取得
	CVector GetThrowDir() const;

	// 発射したブレスのエフェクトのリスト
	std::list<CBreath*> mBreath;

	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// 発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 炎を発射しているか
};