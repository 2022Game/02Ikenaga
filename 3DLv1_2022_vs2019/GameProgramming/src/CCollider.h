#pragma once
#ifndef CCOLLIDER_H
#define CCOLLIDER_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"

/*
コライダクラス
衝突判定データ
*/

class CCollisionManager;
class CCollider :public CTransform,public CTask {
	friend CCollisionManager;
public:
	//衝突処理
	//CCollision(コライダ1,コライダ2)
	//retrun:true(衝突している)false(衝突していない)
	static bool CCollision(CCollider* m, CCollider* o);
	~CCollider();
	//コンストラクタ
	//CCollider(親,新行列,位置,半径)
	CCollider(CCharacter3* parent, CMatrix* matrix, const CVector& position, float radius);
	//親ポインタの取得
	CCharacter3* Parent();
	//描画
	void Render();
protected:
	CCharacter3* mpParent;//親
	CMatrix* mpMatrix;//新行列
	float mRadius; //半径
};

#endif 
