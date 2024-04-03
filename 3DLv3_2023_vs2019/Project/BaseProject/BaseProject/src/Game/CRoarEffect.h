#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CRoar;

// 雄叫びエフェクトクラス
class CRoarEffect : public CTask
{
public:
	// コンストラクタ
	CRoarEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CRoarEffect();

	// 雄叫びを開始
	void Start();
	// 雄叫びを停止
	void Stop();

	// 雄叫びをしているかどうか
	bool IsThrowing() const;

	// 雄叫び時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 雄叫び時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// 雄叫びを作成
	void CreateSlash();

	// 雄叫びの位置を取得
	CVector GetThrowPos() const;
	// 雄叫びの方向を取得
	CVector GetThrowDir() const;

	static CRoarEffect* spInstance;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// 雄叫び時のオフセット位置
	CMatrix mThrowOffsetRot;	// 雄叫び時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 雄叫びをしているか
};