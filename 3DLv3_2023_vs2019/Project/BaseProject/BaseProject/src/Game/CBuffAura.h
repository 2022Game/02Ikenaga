#pragma once
#include "CBillBoardImage.h"

// バフオーラ
class CBuffAura : public CBillBoardImage
{
public:
	// コンストラクタ
	CBuffAura();

	// デストラクタ
	~CBuffAura();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// カラーを設定
	void SetColor(const CColor& color) override;

	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// バフオーラ開始
	void StartAura();
	// バフオーラ終了
	void EndAura();

	// 更新
	void Update() override;

private:

	// 自身のベーススケール値を算出
	float CalcScaleXZ() const;

	float CalcScaleY() const;

	// 待機中
	void UpdateIdle();
	// 開始
	void UpdateStart();
	// 終了待ち
	void UpdateWait();
	// 終了
	void UpdateEnd();

	// 状態
	enum EState
	{
		Idle,   // 待機中
		Start,  // 開始
		Wait,   // 終了待ち
		End,    // 終了
	};
	void ChangeState(EState state);

	EState mState;       // 現在の状態

	float mElapsedTime;  // 経過時間
	float mBaseScale;    // 持ち主のベーススケール値
};