#pragma once
#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタクラスのインクルード
#include"CCharacter3.h"
#include "CInput.h"
#include"CBullet.h"
#include"CColliderLine.h"
#include"CStage.h"
#include "CColliderMesh.h"
#include "CCollisionManager.h"

/*
プレイヤークラス
キャラクタクラスを継承
*/

class CPlayer : public CCharacter3 {
public:
	static int Hp();
	//CModel* model;
	//インスタンスのポインタの取得
	static CPlayer* Instance();
	void Collision();
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
	//CBullet bullet;
	CPlayer();
	//CPayer(位置,回転,スケール)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//更新処理
	void Update();
private:
	static int sHp;	//HP
	int jump;
	CCollider mCollider;
	//CColliderMesh mColliderMesh1;
	//プレイヤーのインスタンス
	static CPlayer* spInstance;
	//CColliderLine mLine4; //線分コライダ
	CColliderLine mLine3; //線分コライダ
	CColliderLine mLine2; //線分コライダ
	CColliderLine mLine; //線分コライダ
	CInput mInput;
};

#endif