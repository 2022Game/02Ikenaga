#pragma once
#pragma once
#ifndef CEXCLAMATIONMARK_H
#define CEXCLAMATIONMARK_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include"CCollider.h"
#include "CColliderMesh.h"

/*
エネミークラス
キャラクタクラスを継承
*/

class CExclamationMark : public CCharacter3 {
public:
	void Set(float w, float d);
	//コンストラクタ
	CExclamationMark();
	void Collision();
	//CEnemy3(位置,回転,拡縮)
	CExclamationMark(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	////確認用メソッド　削除予定
	//void CEnemy::Render() {
	//	CCharacter3::Render();
	//	mCollider1.Render();
	//	mCollider2.Render();
	//	mCollider3.Render();
	//}
	//コンストラクタ
	//CEnemy(モデル,位置,回転,拡縮)
	CExclamationMark(CModel* model, const CVector& position, const CVector& rotation, const CVector& scale);
private:
	int mark;
	CVector mPoint;  //目標地点
	//モデルデータ
	static CModel sModel;
	CCollider mCollider;
	CColliderMesh mColliderMesh1;
	//コライダ
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
};

#endif