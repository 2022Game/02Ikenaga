#pragma once
#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
#include"CCollider.h"

/*
弾クラス
三角形クラスのインクルード
*/

class CBullet : public CCharacter3
{
public:
	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	/*void CBullet::Render() {
		CCharacter3::Render();
		mCollider.Render();
	}*/
	CBullet();
	//幅と奥行きの設定
	//Set(幅,奥行)
	void Set(float w, float d);
	//更新
	void Update();
	void Render();
private:
	CCollider mCollider;
	//生存時間
	int mLife;
	//三角形
	CTriangle mT;
};

#endif