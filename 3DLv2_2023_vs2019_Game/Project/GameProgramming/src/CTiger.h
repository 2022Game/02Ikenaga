#pragma once
#ifndef CTIGER_H
#define CTIGER_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
緑枠クラス
キャラクタクラスを継承
*/

class CTiger : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CTiger();
	void Collision();
	//CTiger(位置,回転,拡縮)
	CTiger(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CTiger(モデル,位置,回転,拡縮)
	CTiger(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	int mark;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	//CCollider mCollider;
	//CColliderMesh mColliderMesh1;
	//コライダ
	//CCollider mCollider1;
	//CCollider mCollider2;
	//CCollider mCollider3;
};

#endif


