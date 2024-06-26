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

	// オーラ開始
	void StartCircle();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	//カウント
	int mCount;
	// モデルデータ読み込み
	CModel* mpBuffCircle;

	bool mPowerUp;
	float mElapsedPowerUpTime;  // 経過時間
	float mDistance;            // 持ち主からの距離
	float mBaseScale;           // 持ち主のベーススケール値
};