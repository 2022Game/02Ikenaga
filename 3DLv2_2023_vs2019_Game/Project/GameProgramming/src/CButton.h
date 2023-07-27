#pragma once
#ifndef CBUTTON_H
#define CBUTTON_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"
#include "CColliderLine.h"

/*
ボタンクラス
キャラクタクラスを継承
*/

class CButton : public CCharacter3 
{
public:
	static CButton* Instance();
	//コンストラクタ
	CButton();
	void Collision();
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CButton(モデル,位置,回転,拡縮)
	CButton(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	static CButton* spInstance;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	//CCollider mCollider;
	CColliderMesh mColliderMesh1;
};

#endif
