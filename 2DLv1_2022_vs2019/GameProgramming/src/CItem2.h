#pragma once
#include"CCharacter.h"

class CItem2 : public CCharacter
{
public:
	//void srand(unsigned int seed);
	//衝突処理1
	void Collision();
	//衝突処理3
	void Collision(CCharacter* m, CCharacter* o);
	//CItem2(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CItem2(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
	//void Render();
	static int mItem2;
private:
	//int main(void);
};