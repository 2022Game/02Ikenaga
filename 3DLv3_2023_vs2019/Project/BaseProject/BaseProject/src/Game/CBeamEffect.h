#pragma once
#include "CTask.h"
#include "CObjectBase.h"

class CBeam;

// ビームエフェクトクラス
class CBeamEffect : public CTask
{
public:
	// コンストラクタ
	CBeamEffect(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	// デストラクタ
	~CBeamEffect();

	// ビームを開始
	void Start();
	// ビームを停止
	void Stop();

	// ビームをしているかどうか
	bool IsThrowing() const;

	// ビーム時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// ビーム時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);

	// 更新
	void Update() override;

private:
	// ビームを作成
	void CreateSlash();

	// ビームの位置を取得
	CVector GetThrowPos() const;
	// ビームの方向を取得
	CVector GetThrowDir() const;

	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowOffsetPos;	// ビーム時のオフセット位置
	CMatrix mThrowOffsetRot;	// ビーム時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 雄叫びをしているか
};