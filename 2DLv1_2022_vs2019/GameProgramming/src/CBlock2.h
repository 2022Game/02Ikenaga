#pragma once
#include "CCharacter.h"
/*
* CBlock2
* ブロッククラス
* マップのブロックに使用する
*/
class CBlock2 : public CCharacter
{
public:
	//CBlock2(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlock2(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update() {}
};