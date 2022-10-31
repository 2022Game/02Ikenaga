#pragma once
#include"CCharacter.h"

class CItem3 : public CCharacter
{
public:
	//衝突処理1
	void Collision();
	//衝突処理3
	void Collision(CCharacter* m, CCharacter* o);
	//CItem3(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CItem3(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
};
