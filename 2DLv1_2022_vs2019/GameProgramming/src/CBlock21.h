#pragma once

#include "CCharacter.h"
/*
* CBlock21
* ブロッククラス
* マップのブロックに使用する
*/
class CBlock21 : public CCharacter
{
public:
	//CBlock21(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CBlock21(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
};