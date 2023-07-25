#pragma once
#ifndef CMEAT_H
#define CMEAT_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
緑枠クラス
キャラクタクラスを継承
*/

class CMeat : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CMeat();
	void Collision();
	//CMeat(位置,回転,拡縮)
	CMeat(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CFrame(モデル,位置,回転,拡縮)
	CMeat(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
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
