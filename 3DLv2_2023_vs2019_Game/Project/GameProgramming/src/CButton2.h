#pragma once
#ifndef CBUTTON2_H
#define CBUTTON2_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include"CColliderLine.h"
#include"CColliderMesh.h"

/*
ボタンクラス
キャラクタクラスを継承
*/

class CButton2 : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CButton2();
	void Collision();
	//CWater(位置,回転,拡縮)
	CButton2(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CButton2(モデル,位置,回転,拡縮)
	CButton2(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CColliderMesh mColliderMesh1;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
};

#endif
