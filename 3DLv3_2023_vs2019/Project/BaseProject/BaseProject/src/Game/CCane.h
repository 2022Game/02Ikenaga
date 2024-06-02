#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

/*
杖クラス
オブジェクトベース継承
*/
class CCane : public CWeapon
{
public:
	// コンストラク
	CCane();
	// デストラクタ
	~CCane();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// モデルデータ読み込み
	CModel* mpCane;
};
