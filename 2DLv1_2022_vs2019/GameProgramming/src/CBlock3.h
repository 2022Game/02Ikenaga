#pragma once
#include "CCharacter.h"
/*
* CBlock3
* ブロッククラス
* マップのブロックに使用する
*/
class CBlock3 : public CCharacter
{
public:
	//衝突処理1
	void Collision();
	//衝突処理3
	void Collision(CCharacter* m, CCharacter* o);
	//CBlock(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlock3(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
private:
	//static int sNum;
};