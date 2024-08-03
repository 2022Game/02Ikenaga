#pragma once
#include "CBillBoardImage.h"

// プレイヤーのヒットエフェクト
class CPlayerHit : public CBillBoardImage
{
public:
	// コンストラクタ
	CPlayerHit(float height);

	// デストラクタ
	~CPlayerHit();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// 開始
	void StartHit(); 
	// 終了
	void EndHit();

	// 更新
	void Update() override;

private:

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

	// 自身のベーススケール値を算出
	float CalcScale() const;

	float mElapsedTime;  // 経過時間
	float mBaseScale;    // 持ち主のベーススケール値
	float mHeight;       // 高さ
};