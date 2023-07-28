#pragma once
#ifndef CMEAT_H
#define CMEAT_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
緑枠クラス
キャラクタクラスを継承
*/

class CMeat : public CCharacter3 {
public:
	//インスタンスのポインタの取得
	static CMeat* Instance();
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
	//肉のインスタンス
	static CMeat* spInstance;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	CColliderLine mLine; //線分コライダ
	CCollider mCollider;
	//CColliderMesh mColliderMesh1;
};

#endif
