#pragma once
#ifndef CENEMY_H
#define CENEMY_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
エネミークラス
キャラクタクラスを継承
*/

class CEnemy : public CCharacter3{
public:
	static CEnemy* Instance();
	//コンストラクタ
	CEnemy();
	void Collision();
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CEnemy(モデル,位置,回転,拡縮)
	CEnemy(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CCollider mCollider;
	CColliderLine mLine3; //線分コライダ
	CColliderLine mLine2; //線分コライダ
	CColliderLine mLine; //線分コライダ
	static CEnemy* spInstance;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
};

#endif