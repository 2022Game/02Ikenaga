#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CSlashing;

// スラッシュクラス
class CSlash : public CTask
{
public:
	// コンストラクタ
	CSlash(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CSlash();

	// スラッシュを発射開始
	void Start();
	// スラッシを発射停止
	void Stop();

	// スラッシュを発射しているかどうか
	bool IsThrowing() const;

	// スラッシュの発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// スラッシュの発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// スラッシュを作成
	void CreateSlash();

	// スラッシュの発射位置を取得
	CVector GetThrowPos() const;
	// スラッシュの発射方向を取得
	CVector GetThrowDir() const;

	static CSlash* spInstance;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// 発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// スラッシュを発射しているか
};