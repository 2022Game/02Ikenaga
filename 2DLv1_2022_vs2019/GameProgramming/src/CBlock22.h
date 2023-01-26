#pragma once

#include "CCharacter.h"
/*
* CBlock22
* ブロッククラス
* マップのブロックに使用する
*/
class CBlock22 : public CCharacter
{
public:
	//CBlock22(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlock22(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
};
