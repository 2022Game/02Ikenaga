#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CCrack;

// ひび割れエフェクトクラス
class CCrackEffect : public CTask
{
public:
	// コンストラクタ
	CCrackEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CCrackEffect();

	// ひび割れを開始
	void Start();
	// ひび割れを停止
	void Stop();

	// ひび割れをしているかどうか
	bool IsThrowing() const;

	// ひび割れ時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// ひび割れ時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// ひび割れを作成
	void CreateSlash();

	// ひび割れの位置を取得
	CVector GetThrowPos() const;
	// ひび割れの方向を取得
	CVector GetThrowDir() const;

	static CCrackEffect* spInstance;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// ひび割れ時のオフセット位置
	CMatrix mThrowOffsetRot;	// ひび割れ時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// ひび割れをしているか
};