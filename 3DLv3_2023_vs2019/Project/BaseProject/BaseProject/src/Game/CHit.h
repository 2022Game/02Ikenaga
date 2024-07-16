#pragma once
#include "CBillBoardImage.h"

// ヒット
class CHit : public CBillBoardImage
{
public:
	// コンストラクタ
	CHit(float size,float height);

	// デストラクタ
	~CHit();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// ヒットエフェクト開始
	void StartHitEffect();
	// ヒットエフェクト終了
	void EndHitEffect();

	// 更新
	void Update() override;

private:

	// 自身のベーススケール値を算出
	float CalcScale() const;

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
	float mSize;         // 大きさ
	float mHeight;       // 高さ
};