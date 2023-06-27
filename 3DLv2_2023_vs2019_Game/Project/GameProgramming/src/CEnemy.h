#pragma once
#ifndef CENEMY_H
#define CENEMY_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

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
	//CEnemy3(位置,回転,拡縮)
	CEnemy(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CEnemy(モデル,位置,回転,拡縮)
	CEnemy(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	static CEnemy* spInstance;
	int mark;
	int mark2;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
};

#endif