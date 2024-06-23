#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 回復サークル
 武器クラス継承
*/
class CHealCircle : public CWeapon
{
public:
	// コンストラク
	CHealCircle(float angle, float dist);
	// デストラクタ
	~CHealCircle();

	// インスタンスのポインタの取得
	static CHealCircle* Instance();

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
	CModel* mpHealCircle;
	// インスタンス
	static CHealCircle* spInstance;
	// 経過時間
	float mElapsedHealTime;

	float mAngle;      // 回転角度
	float mDistance;   // 持ち主からの距離
	float mBaseScale;  // 持ち主のベーススケール値
};