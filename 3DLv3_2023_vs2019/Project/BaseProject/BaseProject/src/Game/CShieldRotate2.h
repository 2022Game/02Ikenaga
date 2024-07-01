#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 回転するシールド2クラス
 武器クラス継承
*/
class CShieldRotate2 : public CWeapon
{
public:
	// コンストラク
	CShieldRotate2(float angle, float dist);
	// デストラクタ
	~CShieldRotate2();

	// 持ち主を設定
	void SetOwner(CCharaBase* owner) override;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// 防御力アップ(ポーション効果)
	bool mDefenseUp;
	// モデルデータ読み込み
	CModel* mpShieldRotate2;
	// 経過時間(防御力アップ用)
	float mElapsedDefenseUpTime;

	float mAngle;      // 盾の回転角度
	float mDistance;   // 持ち主からの距離
};