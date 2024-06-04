#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 回転するシールドクラス
 武器クラス継承
*/
class CShieldRotate : public CWeapon
{
public:
	// コンストラク
	CShieldRotate(float angle,float dist);
	// デストラクタ
	~CShieldRotate();

	void SetOwner(CCharaBase* owner) override;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	int mStateStep;
	// 防御力アップ(ポーション効果)
	bool mDefenseUp;
	// モデルデータ読み込み
	CModel* mpShieldRotate;
	// 経過時間(防御力アップ用)
	float mElapsedDefenseUpTime;

	float mAngle;      // 盾の回転角度
	float mDistance;   // 持ち主からの距離
	float mBaseScale;  // 持ち主のベーススケール値
};
