#pragma once
#include"CCharacter.h"
#include"CUi.h"

class CItem11 : public CCharacter
{
public:
	static int mItem11;
	//static int Point();
	//衝突処理1
	void Collision();
	//衝突処理3
	void Collision(CCharacter* m, CCharacter* o);
	//CItem11(X座標,Y座標,幅,高さ,テクスチャのポインタ)
	CItem11(float x, float y, float w, float h, CTexture* pt);
	//Updateでは何もしない
	void Update();
	//void Render();
	//CUi* mpUi;
private:
	//static int mTime;
	//int mTime;
};
