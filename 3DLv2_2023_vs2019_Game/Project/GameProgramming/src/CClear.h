#pragma once
#ifndef CCLEAR_H
#define CCLEAR_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"
#include "CInput.h"

/*
クリアクラス
キャラクタクラスを継承
*/

class CClear : public CCharacter3 {
public:
	static int STcount(); //ステージカウント
	static CClear* Instance();
	//コンストラクタ
	CClear();
	void Collision();
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	//コンストラクタ
	//CClear(モデル,位置,回転,拡縮)
	CClear(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	CCollider mCollider;
	static int mSTcount; //ステージカウント
	static CClear* spInstance;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	//CCollider mCollider;
	CColliderMesh mColliderMesh1;
	CInput mInput;
};

#endif
