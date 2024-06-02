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
	CShieldRotate(const CVector& pos, const CColor& col);
	// デストラクタ
	~CShieldRotate();

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
	// 移動速度
	//CVector mMoveSpeed;
};
