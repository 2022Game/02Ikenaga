#pragma once
#include "CCharacter.h"
/*
* CBlock5
* ブロッククラス
* マップのブロックに使用する
*/
class CBlock5 : public CCharacter
{
public:
	void Collision(CCharacter* m, CCharacter* o);
	//CBlock5(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlock5(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};