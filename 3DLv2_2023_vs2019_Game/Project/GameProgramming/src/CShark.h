#pragma once
#ifndef CSHARK_H
#define CSHARK_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include"CColliderLine.h"

/*
緑枠クラス
キャラクタクラスを継承
*/

class CShark : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CShark();
	void Collision();
	//CWater(位置,回転,拡縮)
	CShark(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CShark(モデル,位置,回転,拡縮)
	CShark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	CCollider mCollider;
	CColliderLine mLine3; //線分コライダ
	CColliderLine mLine2; //線分コライダ
	CColliderLine mLine; //線分コライダ
};

#endif


