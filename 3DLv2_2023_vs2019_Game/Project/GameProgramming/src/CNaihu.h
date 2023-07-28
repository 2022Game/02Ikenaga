#pragma once
#ifndef CNAIHU_H
#define CNAIHU_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
#include"CCollider.h"

/*
ナイフクラス
*/

class CNaihu : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CNaihu();
	void Collision();
	//(位置,回転,拡縮CNaihu
	CNaihu(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CNaihu(モデル,位置,回転,拡縮)
	CNaihu(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CCollider mCollider;
	//生存時間
	int mLife;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
};

#endif


