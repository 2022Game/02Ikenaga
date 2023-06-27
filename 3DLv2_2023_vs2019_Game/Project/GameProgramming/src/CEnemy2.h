#pragma once
#ifndef CENEMY2_H
#define CENEMY2_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
エネミークラス
キャラクタクラスを継承
*/

class CEnemy2 : public CCharacter3 {
public:
	static CEnemy2* Instance();
	//コンストラクタ
	CEnemy2();
	void Collision();
	//CEnemy2(位置,回転,拡縮)
	CEnemy2(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CEnemy2(モデル,位置,回転,拡縮)
	CEnemy2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	static CEnemy2* spInstance;
	int mark;
	int mark2;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
};

#endif