#pragma once
#include "CCharacter.h"

class CAttack : public CCharacter
{
public:
	//衝突処理1
	void Collision();
	//衝突処理3
	//void Collision(CCharacter* m, CCharacter* o);
	//CAttack(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CAttack(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	//bool Collision(CRectangle* rect);
	void Update();
};