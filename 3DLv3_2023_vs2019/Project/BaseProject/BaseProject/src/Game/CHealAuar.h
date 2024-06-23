#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 回復オーラ
 武器クラス継承
*/
class CHealAura : public CWeapon
{
public:
	// コンストラク
	CHealAura(float angle, float dist);
	// デストラクタ
	~CHealAura();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// オーラ開始
	void StartAura();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	//カウント
	int mCount;
	// モデルデータ読み込み
	CModel* mpHealAura;
	// 経過時間
	float mElapsedHealTime;

	float mAngle;      // 盾の回転角度
	float mDistance;   // 持ち主からの距離
	float mBaseScale;  // 持ち主のベーススケール値
};