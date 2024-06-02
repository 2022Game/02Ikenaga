#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
 盾クラス
 武器クラス継承
*/
class CShield : public CWeapon
{
public:
	// コンストラク
	CShield();
	// デストラクタ
	~CShield();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// モデルデータ読み込み
	CModel* mpShield;
};