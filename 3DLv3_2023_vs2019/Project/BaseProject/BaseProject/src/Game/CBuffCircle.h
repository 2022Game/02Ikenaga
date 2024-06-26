#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 バフサークル
 武器クラス継承
*/
class CBuffCircle : public CWeapon
{
public:
	// コンストラク
	CBuffCircle();
	// デストラクタ
	~CBuffCircle();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// バフサークル開始
	void StartCircle();
	// バフサークル終了
	void EndCircle();

	// 更新
	void Update();
	// 描画
	void Render();

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

	EState mState;         // 現在の状態
	float mElapsedTime;    // 経過時間
	CModel* mpBuffCircle;  // モデルデータ読み込み
	float mBaseScale;      // 持ち主のベーススケール値
};