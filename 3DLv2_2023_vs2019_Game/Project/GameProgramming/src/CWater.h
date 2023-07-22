#pragma once
#ifndef CWATER_H
#define CWATER_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
緑枠クラス
キャラクタクラスを継承
*/

class CWater : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CWater();
	void Collision();
	//CWater(位置,回転,拡縮)
	CWater(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CWater(モデル,位置,回転,拡縮)
	CWater(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	int mark;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	//CCollider mCollider;
	CColliderMesh mColliderMesh1;
	//コライダ
	//CCollider mCollider1;
	//CCollider mCollider2;
	//CCollider mCollider3;
};

#endif

